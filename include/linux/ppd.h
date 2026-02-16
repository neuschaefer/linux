/* 
  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2007-2014 Intel Corporation.

  This program is free software; you can redistribute it and/or modify 
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE 

  Copyright(c) 2007-2014 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
/*
 * ppd.h. Packet processor driver header file.
 * (C) 2007, Texas Instruments, Inc.
 */

/* Version 0.5  - updated for PDSP Specifications of 11Nov2007 */

#ifndef _INCLUDE_PPD_H
#define _INCLUDE_PPD_H

/* To be moved. */
#ifdef CONFIG_TI_PACKET_PROCESSOR_SIMULATION
#include <asm/arch-avalanche/generic/_tistdtypes.h>
//typedef unsigned int Cppi4Queue;
#else
typedef unsigned char  Uint8;
typedef unsigned short Uint16;
typedef unsigned int   Uint32;
typedef char           Int8;
typedef short          Int16;
typedef int            Int32;
typedef unsigned int Cppi4Queue;
#endif /* CONFIG_TI_PACKET_PROCESSOR_SIMULATION */
/* Move ends */

typedef struct 
{
#define TI_PP_BOOTS_FW    0
#define TI_PP_CPDSP_FW    1
#define TI_PP_MPDSP_FW    2
#define TI_PP_QPDSP_FW    3
#define TI_PP_PrxPDSP_FW  4
#define TI_PP_CPDSP2_FW   5

    Uint32  id;        /* As specified above */
    Uint32* data;      /* firmware "data", ie. the instructions array */
    Uint32  size;      /* size of firmware in bytes */

} TI_PP_firmware_t;

typedef struct
{
    Uint16 dflt_host_rx_q;      /* Fwd pkts to Q when PID in pkt do not match */
    Uint16 dflt_host_rx_dst_tag;/* Dst tag for the host RX queue */ 
    Uint16 host_ev_queue;       /* Q for PDSP(s) to post events */
    Uint16 host_q_mgr;          /* Mgr for the event queue */ 
    Uint32 aux_ses_rec_phy_mem; /* Phy mem for additional session records */
 
} TI_PP_cfg_t;

#define TI_PP_L2_LUT_DST_MAC_EN    (0x5 << 0)
#define TI_PP_L2_LUT_SRC_MAC_EN    (0x5 << 1)
#define TI_PP_L2_LUT_ETH_TYPE_EN   (0x1 << 4)
#define TI_PP_L2_LUT_VLAN_ID_EN    (0x1 << 5)
#define TI_PP_L2_LUT_IN_PID_EN     (0x1 << 7)
#define TI_PP_L2_LUT_VLAN_PRI_EN   (0x1 << 8)
#define TI_PP_L2_LUT_DOCSIS_ID_EN  (0x1 << 9)

typedef struct 
{
    Uint8  dst_mac[6];      /* Destination mac bytes                          */
    Uint8  pad0[2];
    Uint8  src_mac[6];      /* Source mac bytes                               */ 
    Uint8  pad1[2];    
    Uint16 ether_type;      /* Ethernet type                                  */
    Uint16 vlan_id;         /* Vlan id                                        */
    Uint8  pad2[1];
    Uint8  ingress_pid;     /* Ingress PID                                    */
    Uint8  vlan_pri_bits;   /* Vlan user priority bits                        */
    Uint8  docsis_id;       /* Docsis ID, Cable specific                      */

    Uint32 enable_flags;    /* Fields valid in this structure                 */
 
} TI_PP_lut_802_3_t;

#define TI_PP_L3_LUT_DST_IPV4_EN          (0x1 << 0)
#define TI_PP_L3_LUT_SRC_IPV4_EN          (0x1 << 1)
#define TI_PP_L3_LUT_TCP_UDP_DST_PORT_EN  (0x1 << 2)
#define TI_PP_L3_LUT_TCP_UDP_SRC_PORT_EN  (0x1 << 3)
#define TI_PP_L3_LUT_PPPOE_SES_ID_EN      (0x1 << 4)
#define TI_PP_L3_LUT_IPV4_FRAG_FLAG_EN    (0x1 << 7)
#define TI_PP_L3_LUT_IPV4_PROTO_EN        (0x1 << 8)
#define TI_PP_L3_LUT_IPV4_TOS_EN          (0x1 << 9)

#define TI_PP_L3_LUT_DST_IPV6_HASH_EN     (0x1 << 0)
#define TI_PP_L3_LUT_SRC_IPV6_HASH_EN     (0x1 << 1)
#define TI_PP_L3_LUT_IPV6_FLOW_LBL_EN     (0x5 << 5)
#define TI_PP_L3_LUT_IPV6_NEXT_HDR_EN     (0x1 << 8)
#define TI_PP_L3_LUT_IPV6_T_CLASS_EN      (0x1 << 9)

typedef struct
{
    Uint32 dst_ip;       /* Destination IP address                         */
    Uint32 src_ip;       /* Source IP address                              */
    Uint16 tcp_udp_dst_port;/* TCP / UDP destination port                     */
    Uint16 tcp_udp_src_port;/* TCP / UDP source port                          */
    Uint16 pppoe_ses_id;    /* PPPOE session id                               */
    Uint8  pad0[3];
    Uint8  frag_flag;       /* Should SR handle frags                         */ 
    Uint8  proto;           /* Protocol carried in the IP frame               */
    Uint8  tos;             /* TOS from the IPV4 header                       */

    Uint32 enable_flags;    /* Fields valid in this structure                 */

} TI_PP_lut_ipv4_t;

typedef struct
{
    Uint32 dst_ip_hash;     /* Hash of destination IPV6 address               */ 
    Uint32 src_ip_hash;     /* Hash of source IPV6 address                    */
    Uint16 tcp_udp_dst_port;/* TCP or UDP destination port                    */
    Uint16 tcp_udp_src_port;/* TCP or UDP source port                         */                           
    Uint16 pppoe_ses_id;    /* PPPOE session id                               */
    Uint16 pad0;
    Uint32 flow_label;    
    Uint8  pad1;
    Uint8  next_header;
    Uint8  traffic_class;
    Uint32 enable_flags;    /* Fields valid in this structure                 */

    Uint8  dst_ip[16];
    Uint8  src_ip[16];

} TI_PP_lut_ipv6_t;

typedef struct 
{
#define TI_PP_LUT_DATA_L2_ETH               (0x00)
#define TI_PP_LUT_DATA_L2_UNDEF             (0x07)
#define TI_PP_LUT_DATA_L3_IPV4              (0x80)
#define TI_PP_LUT_DATA_L3_IPV6              (0x90)
#define TI_PP_LUT_DATA_L3_UNDEF             (0xF0)

    Uint8  type;               /* see above, host provided */
    Uint8  pad0[3];

    union
    {
       TI_PP_lut_802_3_t    data_802_3;
       TI_PP_lut_ipv4_t     data_ipv4;
       TI_PP_lut_ipv6_t     data_ipv6;

    } u;

} TI_PP_lut_t;

typedef struct 
{
#define TI_PP_MOD_IPV4_SRC_ADDR               (0x1 << 0)
#define TI_PP_MOD_IPV4_DST_ADDR               (0x1 << 1)
#define TI_PP_MOD_IPV4_SRC_PORT               (0x1 << 4)
#define TI_PP_MOD_IPV4_DST_PORT               (0x1 << 5)

    Uint16 flags;       /* Types mentioned above, Host provided */ 
    Uint16 pad0;
    Uint32  dst_ip;
    Uint32  src_ip;
    Uint16 dst_port;
    Uint16 src_port;

} TI_PP_ipv4_mod_data_t;

typedef struct 
{
    Uint8  mod_ipv4;
    Uint8  pad0[3];
    TI_PP_ipv4_mod_data_t ipv4_data;

} TI_PP_l3_5_mod_data_t;

typedef struct egress_framing
{
#define TI_PP_EGR_MOD_L2                      (0x00)
#define TI_PP_EGR_IP_FRAG                     (0x01)

    Uint8          frame_code;        /* Egress framing, mentioned above, Host*/
    Uint8          egress_vpid;       /* Egress VPID, Host provided */
    Uint8          pri_drop;
    Uint8          turbo_id;          /* DOCSIS, Host provided */
    Uint8          pad2[2];
    Uint32         turbo_tcp_ack_num; /* DOCSIS, Host provided */
    Uint8          l2_mod_data[32];   /* Depends on frame code, host provided */

#define TI_PP_L3_5_MOD_REC_VALID              (0x01)
    Uint8          l3_5_mod_data_valid_flag;
    Uint8          pad3[3];
    TI_PP_l3_5_mod_data_t l3_5_mod_data[1]; /* Modify rec, Host provided */

} TI_PP_egr_framing_t;

typedef Uint8 TI_PP_ses_id_t;

typedef struct session_properties
{
    TI_PP_ses_id_t  ses_id_index;
    Uint8           pad0[3];

#define TI_PP_SES_FLAG_IDLE_TMOUT             (1 <<  0)
#define TI_PP_SES_FLAG_NO_TMOUT               (1 <<  1)
#define TI_PP_SES_FLAG_NO_IN_STATS            (1 <<  2)
#define TI_PP_SES_FLAG_NO_OUT_STATS           (1 <<  3)
#define TI_PP_SES_FLAG_XLUDE_ETH_HDR_STATS    (1 <<  4)
#define TI_PP_SES_FLAG_UPDATE_TTL             (1 <<  5)
#define TI_PP_SES_FLAG_HANDLE_ZERO_TTL        (1 <<  6)
#define TI_PP_SES_FLAG_HANDLE_IP_FRAG         (1 <<  7)
#define TI_PP_SES_FLAG_HANDLE_IP_OPTIONS      (1 <<  8)
#define TI_PP_SES_FLAG_HANDLE_TCP_CONTROL     (1 <<  9)
#define TI_PP_SES_FLAG_USE_FULL_SRC_IPV6      (1 << 10)
#define TI_PP_SES_FLAG_USE_FULL_DST_IPV6      (1 << 11)
#define TI_PP_SES_FLAG_HANDLE_TURBODOX        (1 << 12)
    Uint16          flags;         /* Flags, mentioned above, Host provided */

#define TI_PP_SES_STATUS_LUT_IPV6_CLSN        (1 <<  0)
#define TI_PP_SES_STATUS_STATS_OVERFLOW       (1 <<  1)
#define TI_PP_SES_STATUS_SES_TMOUT            (1 <<  2)

    Uint8           status;
    Uint8           ingress_vpid;  /* Ingress VPID, Host provided */

    Uint32          ses_timeout;   /* Depends on ses flags, 10us unit, Host */

    TI_PP_lut_t     l2_clfy;       /* L2 classification tuples, Host provided */
    TI_PP_lut_t     l3_clfy;       /* L3 classification tuples, Host provided */

    Uint8           pad1[3];
    Uint8           num_egress;

#define TI_PP_SES_MAX_EGRESS                  (0x05)
    TI_PP_egr_framing_t egr_rec[TI_PP_SES_MAX_EGRESS];

} TI_PP_ses_prop_t;

typedef struct 
{
    Uint8  base_index;
    Uint8  count;
    Uint8  type;
    Uint8  port_num;            /* May be moved into input arg */

} TI_PP_cppi_pid_cfg_t;

typedef Uint8 TI_PP_pid_t;

typedef struct
{
   TI_PP_pid_t pid_index;
   Uint8   pad0[2];

#define TI_PP_PID_TYPE_UNDEF                  (0)
#define TI_PP_PID_TYPE_ETH                    (1)
#define TI_PP_PID_TYPE_INFRASTRUCTURE         (2)
#define TI_PP_PID_TYPE_USB_RNDIS              (3)
#define TI_PP_PID_TYPE_USB_CDC                (4)
#define TI_PP_PID_TYPE_DOCSIS                 (5)
    Uint8  type;          /* Types, mentioned above, Host provided */ 

#define TI_PP_PID_FLG_RX_VLAN_PRI_EN          (1 << 0)
#define TI_PP_PID_FLG_RX_DSCP_EN              (1 << 1)
#define TI_PP_PID_FLG_TX_DST_TAG_PRI_DISBL    (1 << 2)
#define TI_PP_PID_FLG_RX_FWD_DFLT_PID         (1 << 3)
#define TI_PP_PID_FLG_RX_DISBL                (1 << 6)
#define TI_PP_PID_FLG_VALID                   (1 << 7)
    Uint8  flags;         /* Flags, mentioned above, Host provided */  

#define TI_PP_PID_IN_FRM_ETH                  (1 << 0)
#define TI_PP_PID_IN_FRM_IPV4                 (1 << 1)
#define TI_PP_PID_IN_FRM_IPV6                 (1 << 2)
#define TI_PP_PID_IN_FRM_IPOE                 (1 << 3)
#define TI_PP_PID_IN_FRM_IPOA                 (1 << 4)
#define TI_PP_PID_IN_FRM_PPPOE                (1 << 5)
#define TI_PP_PID_IN_FRM_PPPOA                (1 << 6)
    Uint8  in_framing;    /* Ingress frames, mentioned above, Host provided */   

    Uint8  dflt_pri_drp;  /* Drop priority, Host provided */
    Uint8  pri_mapping;
    Uint16 dflt_fwd_q;    /* No session hit, fwd to this QMgr and Queue */
    Uint16 dflt_dst_tag;

    Uint16 egress_mtu;
    Uint16 tx_pri_q_map[8]; /* Queue Manager: indices  */   

#define TI_PP_PID_HW_DATA_HEAD                (1 << 1)
#define TI_PP_PID_HW_DATA_TAIL                (1 << 2)    
    Uint8  hw_data_flag;
    Uint8  hw_data_len;
    Int16  hw_data_offset; /* insert @: +ve for head, -ve from tail */
    Uint8  hw_data[64];

} TI_PP_pid_params_t;

typedef Uint8 TI_PP_vpid_t;

typedef struct 
{
    Uint32 rx_unicast_pkts;     /* Rx unicast packets */  
    Uint32 rx_bcast_pkts;       /* Rx broadcast packets */
    Uint32 rx_mcast_pkts;       /* Rx multicast packets */
    Uint32 rx_bytes_hi;         /* Rx bytes (63:32) */
    Uint32 rx_bytes_lo;         /* Rx bytes (31:0) */
    Uint32 rx_discard_pkts;     /* Rx discard packets */
    Uint32 tx_ucast_pkts;       /* Tx unicast packets */
    Uint32 tx_bcast_pkts;       /* Tx broadcast packets */
    Uint32 tx_mcast_pkts;       /* Tx multicast packets */
    Uint32 tx_bytes_hi;         /* Tx bytes (63:32) */
    Uint32 tx_bytes_lo;         /* Tx bytes (31:0) */    
    Uint32 tx_errors;           /* Tx errors */
    Uint32 tx_discard_pkts;     /* Tx discard */

} TI_PP_vpid_stats_t;

typedef struct 
{
    TI_PP_vpid_t vpid_index;
    Uint8 pad0[3];

#define TI_PP_VPID_FLG_RX_VLAN_PRI_EN         (1 << 0)
#define TI_PP_VPID_FLG_RX_DSCP_PRI_EN         (1 << 1)
#define TI_PP_VPID_FLG_TX_DST_TAG_PRI_EN      (1 << 2)
#define TI_PP_VPID_FLG_RX_FWD_DFLT_PID        (1 << 3)
#define TI_PP_VPID_FLG_TX_DISBL               (1 << 4)
#define TI_PP_VPID_FLG_RX_DISBL               (1 << 5)

    Uint8  flags;           /* Configuration flags, above, Host provided */
    Uint8  parent_pid;      /* Parent PID of this VPID */
    Uint8  pad1[2];

#define TI_PP_VPID_PRIV_DATA_HEAD             (1 << 1)
#define TI_PP_VPID_PRIV_DATA_TAIL             (1 << 2)    
    Uint8  priv_data_flag;
    Uint8  priv_data_len;
    Int16  priv_data_offset; /* insert @: +ve for head, -ve from tail */
    Uint8  priv_data[32];

} TI_PP_vpid_params_t;

typedef struct 
{
    Uint32 in_pkts;           /* Pkts received by SR */
    Uint32 searched_pkts;     /* Pkts submitted for LUT search */
    Uint32 search_matches;    /* # of searches with a hit */
    Uint32 in_sync_pkts;      /* pkts awaiting synchronization */
    Uint32 fwd_pkts;          /* pkts forwarded by SR */
    Uint32 ip_fwd_pkts;       /* IP pkts forwarded by SR */
    Uint32 ip_frag_ok;        /* IP pkts that were successfully fragmented */
    Uint32 ip_frag_fails;     /* IP pkts for which fragmentation failed */
    Uint32 ip_frag_creates;   /* the number of fragments created in SR */
    
} TI_PP_srl_pkt_stats_t;

typedef struct
{
    Uint32 fwd_pkts;
    Uint32 fwd_bytes_hi;
    Uint32 fwd_bytes_lo;

} TI_PP_ses_pkt_stats_t;

#endif /* !_INCLUDE_PPD_H */

