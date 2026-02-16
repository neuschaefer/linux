/*
  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2014-2017 Intel Corporation.

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

  Copyright(c) 2014-2017 Intel Corporation. All rights reserved.

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
#ifndef     _AVALANCHE_PP_H
#define     _AVALANCHE_PP_H
#ifdef CONFIG_ARM_AVALANCHE_SOC
#include <asm-arm/arch-avalanche/generic/_tistdtypes.h>

#ifdef __KERNEL__

#if defined(PUMA6_SOC_TYPE) && PUMA6_SOC_TYPE
#include <asm-arm/arch-avalanche/puma6/puma6_cppi_prv.h>
#endif

#if PUMA7_SOC_TYPE
#include <asm-arm/arch-avalanche/puma7/puma7_cppi.h>
#include <asm-arm/arch-avalanche/puma7/puma7_pp.h>
#endif

#else
#include <puma_autoconf.h>
#if defined(PUMA6_SOC_TYPE) && PUMA6_SOC_TYPE
#include <puma6_cppi_prv.h>
#endif
#ifdef PUMA7_SOC_TYPE
#include <puma7_cppi_prv.h>
#endif
#endif

#else /* AP-CPU */
#ifdef __KERNEL__
#include <linux/avalanche/generic/_tistdtypes.h>
#include <linux/avalanche/puma7/puma7_defs.h>
#include <linux/avalanche/puma7/puma7_cppi.h>
#include <linux/avalanche/puma7/puma7_pp.h>
#else
#include "_tistdtypes.h"
#endif
#endif

#include <linux/ioctl.h>
#include <asm/byteorder.h>
/**************************************************************************
 ****************************** Limit Definitions *************************
 **************************************************************************/
/* These are the maximum number of PID,VPID & Sessions that are supported.*/
#if defined(PUMA6_SOC_TYPE) && PUMA6_SOC_TYPE
#define AVALANCHE_PP_MAX_PID                            32
#define AVALANCHE_PP_MAX_VPID                           32
#define AVALANCHE_PP_MAX_ACCELERATED_SESSIONS           2048
#define AVALANCHE_PP_MAX_LUT1_KEYS                      256
#define AVALANCHE_PP_MAX_ACCELERATED_TDOX_SESSIONS      256
#define MAX_ALLOWED_QOS_CLUSTERS_PER_DEVICE             16
#define AVALANCHE_PP_INVALID_SESSION                    (AVALANCHE_PP_MAX_ACCELERATED_SESSIONS)
#else
#define AVALANCHE_PP_MAX_PID                            32
#define AVALANCHE_PP_MAX_VPID                           51
#define AVALANCHE_PP_MAX_LUT_ENTRIES                    (3 * 1024)
#define AVALANCHE_PP_MAX_ACCELERATED_TDOX_SESSIONS      500
#define VPID_IF_NAME_SIZE                               16
#define AVALANCHE_PP_INVALID_SESSION                    (35 * 1024)
#endif

#ifdef CONFIG_WIFI_MESH_TUNNEL
#define AVALANCHE_PP_MAX_ACCELERATED_VOICE_SESSIONS     0
#else
#define AVALANCHE_PP_MAX_ACCELERATED_VOICE_SESSIONS     16
#endif

#define DDH_MAX_DROP_SESSIONS                                   (128)
#define DDH_HOST_PPS_THRESHOLD                                  (9000)
#define DDH_LOW_SESSION_UTILIZATION_THRESHOLD                   (80)
#define DDH_HIGH_SESSION_UTILIZATION_THRESHOLD                  (90)
#define DDH_US_SESSION_UTILIZATION_THRESHOLD                    (80)
#define DDH_EVENT_SHORT_TIMEOUT_SECONDS                         (5)
#define DDH_EVENT_LONG_TIMEOUT_SECONDS                          (10)
#define DDH_EVENT_BACK_TO_NORMAL_TIMEOUT_SECONDS                (60)
#define DDH_NOTIFICATIONS_NUM_OF_SAMPLING_WITHOUT_STATE_SWITCH  (3)
#define DDH_NUM_SHORT_CONNECTION_PACKETS_THRESHOLD              (10)                                            /* Num packets for which we bypass egress hook */
#define DDH_NUM_SHORT_CONNECTIONS_THRESHOLD                     (500)                                           /* Threshold to trigger DDH */
#define DDH_NUM_SCB_COLLISIONS_THRESHOLD                        (DDH_NUM_SHORT_CONNECTIONS_THRESHOLD)           /* Threshold to trigger DDH */

#define MSS_CLAMPING_MAX_VALUE                                  (1460)
#define MSS_CLAMPING_MIN_VALUE                                  (800)
/* ID of the destination group who are listening to DDH notifications socket. */
#define AVALANCHE_PP_DDH_NOIFICATIONS_NETLINK_GROUP             1

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_VERSION_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure contains four version notation numbers.
 **************************************************************************/
#if PUMA7_OR_NEWER_SOC_TYPE
typedef struct
{
    Uint32  SequencerPPver;
    Uint32  SequencerPDSPver;
    Uint32  PrefetcherPPver;
    Uint32  PrefetcherPDSPver;
    Uint32  Classifier2PPver;
    Uint32  Classifier2PDSPver;
    Uint32  SessionCachePPver;
    Uint32  SessionCachePDSPver;
    Uint32  Classifier1PPver;
    Uint32  Classifier1PDSPver;
    Uint32  ModifierPPver;
    Uint32  ModifierPDSPver;
    Uint32  ReSequencerPPver;
    Uint32  ReSequencerPDSPver;
    Uint32  TurboDOXPPver;
    Uint32  TurboDOXPDSPver;
    Uint32  AQMdPPver;
    Uint32  AQMdPDSPver;
    Uint32  QoSPPver;
    Uint32  QoSPDSPver;
    Uint32  DpiPPver;
    Uint32  DpiPDSPver;
    Uint32  McPPver;
    Uint32  McPDSPver;
    Uint32  AccumulatorPPver;
    Uint32  AccumulatorPDSPver;
    Uint32  RecyclerPPver;
    Uint32  RecyclerPDSPver;
    Uint32  WifiTxPPver;
    Uint32  WifiTxPDSPver;
    Uint32  WifiRxPPver;
    Uint32  WifiRxPDSPver;
    Uint32  MocaPPver;
    Uint32  MocaPDSPver;
    Uint32  CryptoPPver;
    Uint32  CryptoPDSPver;
    Uint32  EncryptPPver;
    Uint32  EncryptPDSPver;
    Uint32  DecryptPPver;
    Uint32  DecryptPDSPver;

    Uint32  MinPPver;
    Uint32  MaxPPver;
}
AVALANCHE_PP_VERSION_t;
#else
typedef struct  // former TI_PP_VERSION
{
    unsigned char   v0;
    unsigned char   v1;
    unsigned char   v2;
    unsigned char   v3;
}
AVALANCHE_PP_VERSION_t;
#endif

/* ******************************************************************** */
/*                                                                      */
/*                      ____ ___ ____                                   */
/*                     |  _ \_ _|  _ \                                  */
/*                     | |_) | || | | |                                 */
/*                     |  __/| || |_| |                                 */
/*                     |_|  |___|____/                                  */
/*                                                                      */
/*                                                                      */
/* ******************************************************************** */

/**************************************************************************
 ****************************** PID Flag Definitions **********************
 **************************************************************************/

#define AVALANCHE_PP_PID_VLAN_PRIO_MAP            0x1
#define AVALANCHE_PP_PID_DIFFSRV_PRIO_MAP         0x2
#define AVALANCHE_PP_PID_PRIO_OFF_TX_DST_TAG      0x4
#define AVALANCHE_PP_PID_CLASSIFY_BYPASS          0x8
#define AVALANCHE_PP_PID_DISCARD_ALL_RX           0x40

/**************************************************************************
 ****************************** PID Type Definitions **********************
 **************************************************************************/

#define AVALANCHE_PP_PID_TYPE_UNDEFINED            0x0
#define AVALANCHE_PP_PID_TYPE_ETHERNET             0x1
#define AVALANCHE_PP_PID_TYPE_INFRASTRUCTURE       0x2
#define AVALANCHE_PP_PID_TYPE_USBBULK              0x3
#define AVALANCHE_PP_PID_TYPE_CDC                  0x4
#define AVALANCHE_PP_PID_TYPE_DOCSIS               0x5
#define AVALANCHE_PP_PID_TYPE_ETHERNETSWITCH       0x6

/**************************************************************************
 ****************************** PID Ingress Framing ***********************
 **************************************************************************/

#define AVALANCHE_PP_PID_INGRESS_ETHERNET          0x1
#define AVALANCHE_PP_PID_INGRESS_IPV4              0x2
#define AVALANCHE_PP_PID_INGRESS_IPV6              0x4
#define AVALANCHE_PP_PID_INGRESS_IPOE              0x8
#define AVALANCHE_PP_PID_INGRESS_IPOA              0x10
#define AVALANCHE_PP_PID_INGRESS_PPPOE             0x20
#define AVALANCHE_PP_PID_INGRESS_PPPOA             0x40

/**************************************************************************
 ****************************** PID Generic Definitions *******************
 **************************************************************************/
#define AVALANCHE_PP_PID_NO_DST_TAG                0x3FFF

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_PID_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the PID. The information in the PID structure
 *  is very hardware specific and is typically filled up by the drivers
 **************************************************************************/
typedef struct // former TI_PP_PID
{
    Uint8           pid_handle;
    Uint8           priv_flags;
    Uint8           type;
    Uint8           ingress_framing;

    Uint8           dflt_pri_drp;
    Uint8           pri_mapping;
    Uint16          dflt_host_q;
    Uint16          dflt_infra_q;

    Uint16          dflt_dst_tag;
    Uint16          tx_pri_q_map[ 8 ];
    Uint8           tx_hw_data[ 64 ];
    Uint8           tx_hw_data_len;
}
AVALANCHE_PP_PID_t;

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_ACC_CH_INFO_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the PID. The information in the PID structure
 *  is very hardware specific and is typically filled up by the drivers
 **************************************************************************/
typedef struct
{
    Uint16          Index;
    Uint8           Channel;
    Uint8           Command;
    Uint32          Param0Ret;
    Uint32          Param1;
    Uint32          Param2;
}
AVALANCHE_PP_ACC_CH_INFO_t;

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_PID_RANGE_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the PID range.
 **************************************************************************/
typedef struct // former TI_PP_PID_RANGE
{
    Uint8       port_num;
    Uint8       type;
    Uint8       count;
    Uint8       base_index;
}
AVALANCHE_PP_PID_RANGE_t;


/* ******************************************************************** */
/*                                                                      */
/*                       ___       ____                                 */
/*                      / _ \  ___/ ___|                                */
/*                     | | | |/ _ \___ \                                */
/*                     | |_| | (_) |__) |                               */
/*                      \__\_\\___/____/                                */
/*                                                                      */
/*                                                                      */
/* ******************************************************************** */
#if defined(PUMA6_SOC_TYPE) && PUMA6_SOC_TYPE
#define AVALANCHE_PP_QOS_CLST_MAX_INDX          31
#define AVALANCHE_PP_QOS_QUEUE_MAX_INDX         (PAL_CPPI41_SR_QPDSP_QOS_Q_LAST - PAL_CPPI41_SR_QPDSP_QOS_Q_BASE)
#endif
#define AVALANCHE_PP_QOS_CLST_MAX_EGRESS_QCNT   4
#define AVALANCHE_PP_QOS_CLST_MAX_QCNT          8
/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_QOS_QUEUE
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes a QOS Queue configuration. Generally QOS
 *  configuration for all queues associated to a single cluster is contained in
 *  corresponding Cluster configuration.
 **************************************************************************/
typedef struct // former TI_PP_QOS_QUEUE
{
    Uint8               q_num;                  /* Index of the QOS queue (offset from QOS queue base) */
    Uint8               flags;                  /* Control how packets in the queue should be handled */
    Uint16              egr_q;                  /* Queue manager and queue index of forwarding queue */

    Uint32              congst_thrsh_bytes;     /* The size in bytes at which point the QOS queue is considered to be congested. */
    Uint16              congst_thrsh_packets;   /* The maximum number of packets to be kept in QOS queue */

    /*Strict Schedluing parameters */
    Uint32              it_credit_bytes;        /* The amount of forwarding byte ?credit? that the queue receives every 25us. */
    Uint16              it_credit_packets;      /* The amount of forwarding packets ?credit? that the queue receives every 25us. */
    Uint32              max_credit_bytes;       /* The maximum amount of forwarding byte ?credit? that the queue is allowed to hold at the end of the 25us iteration. */
    Uint16              max_credit_packets;     /* The maximum amount of forwarding byte ?credit? that the queue is allowed to hold at the end of the 25us iteration. */

    /* WFQ Schedluing parameters */
    Uint16              q_weight;               /* The Weight given to queue i */
    Uint16              cpqpi;                  /* Credit byte per queue per iteration (CPQP(i))  This is the relative number of bytes per iteration for queue based on its w(i) */
}
AVALANCHE_PP_QOS_QUEUE_t;

/* QoS manager scheduling mode */
typedef enum
{
    PP_QOS_SCH_STRICT,
    PP_QOS_SCH_WFQ,
    PP_QOS_SCH_GRR
} PP_QOS_SCH_MODE_e;

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_QOS_CLST_CFG
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes a QOS cluster. It contains the configuration for
 *  all the associated Queues
 **************************************************************************/
typedef struct // former TI_PP_QOS_CLST_CFG
{
    AVALANCHE_PP_QOS_QUEUE_t    qos_q_cfg[ AVALANCHE_PP_QOS_CLST_MAX_QCNT ]; /* Configuration for all the queues associated with the cluster, arranged in order of priority (qos_q_cfg[0] being lower in priority than qos_q_cfg[1]) */
    PP_QOS_SCH_MODE_e           sch_mode;                 /* Scheduling mode */
    Uint8                       qos_q_cnt;                /* Number of QOS queues in the cluster (1 to 8) */
    Uint8                       flags;
#define PP_QOS_CLUSTER_TYPE_WFQ       (1<<0)
#define PP_QOS_CLUSTER_TYPE_HOST      (1<<1)
#define PP_QOS_CLUSTER_TYPE_GRR       (1<<2)

    Uint8                       grr_first_bitmask;        /* For Grouped RR Cluster only (Grouped RR cluster does't support QDA) */
    Uint8                       iteration_divider;        /* For Host QoS Cluster only (Host QoS cluster does't support egress congested algo) */
    Uint32                      grr_cluster_it_credit_bytes;
    Uint16                      grr_cluster_it_credit_packets;
    Uint16                      res;

    /*Strict Schedluing parameters */
    Uint32                      global_credit_bytes;      /* The amount of global bytes credit available to the next QOS queue in the cluster */
    Uint32                      max_global_credit_bytes;  /* The maximum amount of global credit allowed to carry over to the next  queue. */

    Uint16                      global_credit_packets;    /* The amount of global pkt credit available to the next QOS queue in the cluster */
    Uint16                      max_global_credit_packets; /* The maximum amount of global credit allowed to carry over to the next  queue. */

    Uint32                      egr_congst_thrsh_bytes1;  /* Egress Congestion Bytes Threshold 1 */
    Uint32                      egr_congst_thrsh_bytes2;  /* Egress Congestion Bytes Threshold 2 */
    Uint32                      egr_congst_thrsh_bytes3;  /* Egress Congestion Bytes Threshold 3 */
    Uint32                      egr_congst_thrsh_bytes4;  /* Egress Congestion Bytes Threshold 4 */

    Uint16                      egr_congst_thrsh_packets1; /* Egress Congestion Pkts Threshold 1*/
    Uint16                      egr_congst_thrsh_packets2; /* Egress Congestion Pkts Threshold 2 */
    Uint16                      egr_congst_thrsh_packets3; /* Egress Congestion Pkts Threshold 3 */
    Uint16                      egr_congst_thrsh_packets4; /* Egress Congestion Pkts Threshold 4 */

    Uint8                       shaperClusters[4];
    
    /* WFQ Scheduling parameters */
    Uint32                       line_rate_bps;              /* Line Rate  In bits per second. (i.e 1Gbps for RGMII)  */
    Uint32                       credit_per_iteration;       /* The Max bytes that can be transmitted per iteration. (this field is automatically configured from the line rate and iteration */
    Uint32                       sum_of_weights;               /* The total Weights that is configured to the cluster */
}
AVALANCHE_PP_QOS_CLST_CFG_t;

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_QOS_QUEUE_STATS
 **************************************************************************
 * DESCRIPTION   :
 *  The structure contains packet statistics for a QOS queue.
 **************************************************************************/
#if PUMA7_OR_NEWER_SOC_TYPE
typedef struct // former TI_PP_QOS_QUEUE_STATS
{
    Uint64      fwd_pkts;            /* Number of packets forwarded to the Egress Queue */
    Uint64      drp_cnt;             /* Number of packets dropped due to congestion */
}
#else
typedef struct // former TI_PP_QOS_QUEUE_STATS
{
    Uint32      fwd_pkts;            /* Number of packets forwarded to the Egress Queue */
    Uint32      drp_cnt;             /* Number of packets dropped due to congestion */
}
#endif
AVALANCHE_PP_QOS_QUEUE_STATS_t;

/* ******************************************************************** */
/*                                                                      */
/*                __     ______ ___ ____                                */
/*                \ \   / /  _ \_ _|  _ \                               */
/*                 \ \ / /| |_) | || | | |                              */
/*                  \ V / |  __/| || |_| |                              */
/*                   \_/  |_|  |___|____/                               */
/*                                                                      */
/*                                                                      */
/* ******************************************************************** */
/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_VPID_TYPE_e
 **************************************************************************
 * DESCRIPTION   :
 *  The enumeration defines the type of network to which a VPID is
 *  connected.
 **************************************************************************/
typedef enum // former TI_PP_VPID_TYPE
{
    AVALANCHE_PP_VPID_ETHERNET   = 0x0,
    AVALANCHE_PP_VPID_VLAN       = 0x1,
#ifdef CONFIG_WIFI_MESH_TUNNEL
    AVALANCHE_PP_DOUBLE_VLAN_ETH_MESH,
    AVALANCHE_PP_VPID_VLAN_WIFI_MESH,
    AVALANCHE_PP_VPID_VLAN_ETH_MESH,
#endif
}
AVALANCHE_PP_VPID_TYPE_e;

/**************************************************************************
 ****************************** VPID Flag Definitions *********************
 **************************************************************************/
#define AVALANCHE_PP_VPID_FLG_RX_DFLT_FWD          (1 << 3)
#define AVALANCHE_PP_VPID_FLG_TX_DISBL             (1 << 5)
#define AVALANCHE_PP_VPID_FLG_RX_DISBL             (1 << 6)
#define AVALANCHE_PP_VPID_FLG_VALID                (1 << 7)

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_VPID_INFO_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the VPID entity. All packets which enter and
 *  exit the PP do so via a VPID Entity. Thus VPID's can be considered as
 *  end points for all the networking traffic through the box. They map
 *  to the OS networking interface objects which are the end points
 *  connecting the driver to the OS networking stack.
 **************************************************************************/
typedef struct // former TI_PP_VPID
{
    /* This is the VPID Handle. Users dont need to populate this field since
     * this handle will be filled up the PPM on the successful creation
     * of the VPID. */
    Uint8                           vpid_handle;

    /* This is the parent PID handle. All VPID are related to the PID
     * Users need to specify the Parent PID handle in this field. */
    Uint8                           parent_pid_handle;

    /* This describes the VPID Type. VPID are networking endpoints and
     * which describe the type of network to which they are connected. */
    AVALANCHE_PP_VPID_TYPE_e        type;

    /* Private Data. */
    Uint16                           flags;


    /* This is an optional VLAN Identifier associated with the VPID. This
     * is required only if the VPID is attached to a VLAN enabled network
     * Thus this field will be used only if the "type" is either AVALANCHE_PP_VLAN
     * or AVALANCHE_PP_VLAN_PPPoE */
    Uint16                          vlan_identifier;

#if defined(PUMA6_SOC_TYPE) && PUMA6_SOC_TYPE
    /* These are the QoS related settings */
    AVALANCHE_PP_QOS_CLST_CFG_t *   qos_cluster[ MAX_ALLOWED_QOS_CLUSTERS_PER_DEVICE ];
    unsigned char                   qos_clusters_count;
#else
    /* This is the name of the interface. */
    char                            devName[VPID_IF_NAME_SIZE];
#endif
}
AVALANCHE_PP_VPID_INFO_t;

/**************************************************************************
 * STRUCTURE NAME : TI_PP_VPID_STATS
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the VPID statistics for the Packet Processor.
 **************************************************************************/
#if PUMA7_OR_NEWER_SOC_TYPE
typedef struct // former TI_PP_VPID_STATS
{
    Uint64      rx_byte;
    Uint64      rx_unicast_pkt;
    Uint64      rx_broadcast_pkt;
    Uint64      rx_multicast_pkt;
    Uint64      rx_discard_pkt;

    Uint64      tx_byte;
    Uint64      tx_unicast_pkt;
    Uint64      tx_broadcast_pkt;
    Uint64      tx_multicast_pkt;
    Uint64      tx_discard_pkt;
}AVALANCHE_PP_VPID_STATS_t;
#else
typedef struct // former TI_PP_VPID_STATS
{
    Uint64      rx_byte;
    Uint32      rx_unicast_pkt;
    Uint32      rx_broadcast_pkt;
    Uint32      rx_multicast_pkt;
    Uint32      rx_discard_pkt;

    Uint64      tx_byte;
    Uint32      tx_unicast_pkt;
    Uint32      tx_broadcast_pkt;
    Uint32      tx_multicast_pkt;
    Uint32      tx_discard_pkt;

    Uint32      tx_error;
}AVALANCHE_PP_VPID_STATS_t;
#endif

/* ******************************************************************** */
/*                                                                      */
/*                 ____                _                                */
/*                / ___|  ___  ___ ___(_) ___  _ __                     */
/*                \___ \ / _ \/ __/ __| |/ _ \| '_ \                    */
/*                 ___) |  __/\__ \__ \ | (_) | | | |                   */
/*                |____/ \___||___/___/_|\___/|_| |_|                   */
/*                                                                      */
/*                                                                      */
/* ******************************************************************** */
#if defined(PUMA6_SOC_TYPE) && PUMA6_SOC_TYPE
typedef enum
{
    AVALANCHE_PP_LUT_ENTRY_L2_ETHERNET,
#ifdef CONFIG_WIFI_MESH_TUNNEL
    AVALANCHE_PP_LUT_ENTRY_WIFI_MESH,
#else
   AVALANCHE_PP_LUT_ENTRY_reserved_1,
#endif
    AVALANCHE_PP_LUT_ENTRY_reserved_2,
    AVALANCHE_PP_LUT_ENTRY_reserved_3,
    AVALANCHE_PP_LUT_ENTRY_reserved_4,
    AVALANCHE_PP_LUT_ENTRY_reserved_5,
    AVALANCHE_PP_LUT_ENTRY_reserved_6,
    AVALANCHE_PP_LUT_ENTRY_L2_UNDEFINED,

    AVALANCHE_PP_LUT_ENTRY_L3_IPV4,
    AVALANCHE_PP_LUT_ENTRY_L3_IPV6,
    AVALANCHE_PP_LUT_ENTRY_L3_DSLITE,
    AVALANCHE_PP_LUT_ENTRY_L3_GRE,
    AVALANCHE_PP_LUT_ENTRY_reserved_12,
    AVALANCHE_PP_LUT_ENTRY_reserved_13,
    AVALANCHE_PP_LUT_ENTRY_reserved_14,
    AVALANCHE_PP_LUT_ENTRY_L3_UNDEFINED,
}
AVALANCHE_PP_LUT_ENTRY_TYPE_e;

/* LUT1 L2-ETH Enable Flags */
typedef enum
{
    AVALANCHE_PP_LUT1_FIELD_ENABLE_PID           = 0x00000040,
    AVALANCHE_PP_LUT1_FIELD_ENABLE_MAC_DST       = 0x00000003,
    AVALANCHE_PP_LUT1_FIELD_ENABLE_MAC_SRC       = 0x0000000E,
    AVALANCHE_PP_LUT1_FIELD_ENABLE_ETH_TYPE      = 0x00000020,
    AVALANCHE_PP_LUT1_FIELD_ENABLE_L2_ENTRY_TYPE = 0x00000080,
    AVALANCHE_PP_LUT1_FIELD_ENABLE_L2_MAX_VAL    = 0xFFFFFFFF
}
AVALANCHE_PP_LUT1_L2_FIELD_ENABLE_e;

/* LUT1 L3-IP Enable Flags */
typedef enum
{
    AVALANCHE_PP_LUT1_FIELD_ENABLE_LAN_IPv4      = 0x00000001,
    AVALANCHE_PP_LUT1_FIELD_ENABLE_LAN_IPv6      = 0x0000003F,
    AVALANCHE_PP_LUT1_FIELD_ENABLE_IP_PROTOCOL   = 0x00000100,

    AVALANCHE_PP_LUT1_FIELD_ENABLE_L3_ENTRY_TYPE = 0x00000200,
    AVALANCHE_PP_LUT1_FIELD_ENABLE_L3_MAX_VAL    = 0xFFFFFFFF
}
AVALANCHE_PP_LUT1_L3_FIELD_ENABLE_e;

typedef struct
{
    union
    {
        /*------------------------------------------*/
        Uint8       raw[48];
        /*------------------------------------------*/
        struct
        {
            struct
            {
                Uint8                                   dstmac[6];
                Uint8                                   srcmac[6];
                Uint16                                  eth_type;
                Uint16                                  res_S3;
                Uint8                                   res_B3;
                Uint8                                   res_B2;
                Uint8                                   entry_type;             /* Type: AVALANCHE_PP_LUT_ENTRY_TYPE_e */
                Uint8                                   pid_handle;             /* Type: PP_PID_NUM_e */

                AVALANCHE_PP_LUT1_L2_FIELD_ENABLE_e     enable_flags;
            }
            L2;

            struct
            {
                union
                {
                    Uint32                              v4;
                    Uint32                              v6[ 4 ];
                }
                LAN_addr_IP;

                Uint8                                   entry_type;              /* Type: AVALANCHE_PP_LUT_ENTRY_TYPE_e */
                Uint8                                   ip_protocol;
                Uint16                                  PPPoE_session_id;
                #define                                 AVALANCHE_PP_SESSION_PPPOE_INVALID  0xFFFF

                AVALANCHE_PP_LUT1_L3_FIELD_ENABLE_e     enable_flags;
            }
            L3;

        }
        fields;
        /*------------------------------------------*/
    }u;
}
__Avalanche_PP_LUT1_inputs_t;

/* LUT2 Enable Flags */
typedef enum
{
    AVALANCHE_PP_LUT2_FIELD_ENABLE_WAN_IP           = 0x01,
    AVALANCHE_PP_LUT2_FIELD_ENABLE_IPV6_FLOW        = 0x02,
    AVALANCHE_PP_LUT2_FIELD_ENABLE_DSLITE_IPV4      = AVALANCHE_PP_LUT2_FIELD_ENABLE_IPV6_FLOW,
    AVALANCHE_PP_LUT2_FIELD_ENABLE_1ST_VLAN         = 0x04,
    AVALANCHE_PP_LUT2_FIELD_ENABLE_2ND_VLAN         = 0x08,
    AVALANCHE_PP_LUT2_FIELD_ENABLE_SRC_PORT         = 0x10,
    AVALANCHE_PP_LUT2_FIELD_ENABLE_DST_PORT         = 0x20,
    AVALANCHE_PP_LUT2_FIELD_ENABLE_IP_TOS           = 0x40
}
AVALANCHE_PP_LUT2_FIELD_ENABLE_e;

#ifndef CONFIG_WIFI_MESH_TUNNEL
typedef struct
{
    union
    {
        /*------------------------------------------*/
        Uint8       raw[32];
        /*------------------------------------------*/
        struct
        {
            union
            {
                Uint32                              v4;
                Uint32                              v6[ 4 ];
            }
            WAN_addr_IP;

            union
            {
                Uint32                              v4_dsLite;
                Uint8                               v6_FlowLabel[ 4 ];
            }IP;

            Uint16                                  firstVLAN;
            Uint16                                  secondVLAN;
            Uint16                                  L4_SrcPort;
            Uint16                                  L4_DstPort;

            Uint8                                   TOS;
            Uint8                                   LUT1_key;
            Uint8                                   entry_type;                  /* Type: AVALANCHE_PP_LUT_ENTRY_TYPE_e */

            Uint8                                   enable_flags;               /* Type: AVALANCHE_PP_LUT2_FIELD_ENABLE_e */
        }
        fields;
        /*------------------------------------------*/
    }u;
}
__Avalanche_PP_LUT2_inputs_t;

#else
typedef struct
{
    union
    {
        /*------------------------------------------*/
        Uint8       raw[32];
        /*------------------------------------------*/
        struct
        {

            Uint16                                  vlan_id;
            Uint8                                   frame_control_flags;
            Uint8                                   packet_type;

            Uint16                                  src_intf_id;
            Uint16                                  dst_intf_id;

            Uint8                                   receive_mac[6];
            Uint8                                   transmit_mac[6];
            Uint8                                   dst_mac[6];
            Uint8                                   src_mac[6];


        }
        fields;
        /*------------------------------------------*/

    }u;
}
__Avalanche_PP_LUT2_inputs_t;    /* For WIFI MESH mode*/

#endif

typedef struct
{

    __Avalanche_PP_LUT1_inputs_t        LUT1;
    __Avalanche_PP_LUT2_inputs_t        LUT2;
}
__Avalanche_PP_LUTs_Data_t;

#else // #if PUMA7_OR_NEWER_SOC_TYPE

#define PP_LOOKUP_FIELD_UNKNOWN_VALUE           0
#define PP_LOOKUP_FIELD_PPPOE_SESSION_INVALID   0xFFFF

/* Define for EtherType 0xFFFF - special MoCA packets */
#define ETH_P_CTP_MOCA          (0xFFFF)

/* Enum for Classification mode */
typedef enum
{
    CLASSIFICATION_MODE_IPV4_IPV6_L4   = 0,   // Accelerate packets with ip v4/6 and tcp/udp (L4 is mandatory)
    CLASSIFICATION_MODE_IPV4_IPV6      = 1,   // Accelerate packets with ip v4/6 (L4 is optional)
    CLASSIFICATION_MODE_MOCA_IRREG     = 2    // Accelerate packets with ip v4/6/ and L3=ffff for MoCA only (L4 is optional)
}PP_CLASSIFICATION_MODE_e;

#ifdef CONFIG_LTQ_PPA_DATAPATH
#define CLASSIFICATION_MODE_DEFAULT CLASSIFICATION_MODE_IPV4_IPV6_L4
#else
#define CLASSIFICATION_MODE_DEFAULT CLASSIFICATION_MODE_IPV4_IPV6
#endif

/* Session cache deletion thresholds */
#ifdef CONFIG_SESS_TIMEOUT_BY_THRESH
#define SESSION_CACHE_DEFAULT_TIMEOUT_BY_THRESHOLD_ENABLE  1
#define SESSION_CACHE_DEFAULT_TIMEOUT_BY_THRESHOLD_LOW     100
#define SESSION_CACHE_DEFAULT_TIMEOUT_BY_THRESHOLD_HIGH    1500
#else
#define SESSION_CACHE_DEFAULT_TIMEOUT_BY_THRESHOLD_ENABLE  0
#endif


typedef enum
{
    PP_LOOKUP_FIELD_L2_TYPE_ETHERNET            = 1
}PP_LOOKUP_FIELD_L2_TYPES_e;

typedef enum
{
    PP_LOOKUP_FIELD_L3_TYPE_IPv4                = 1,
    PP_LOOKUP_FIELD_L3_TYPE_IPv6                = 2,
    PP_LOOKUP_FIELD_L3_TYPE_FFFF                = 3
}PP_LOOKUP_FIELD_L3_TYPES_e;

typedef enum
{
    PP_LOOKUP_FIELD_L4_PROTOCOL_UDP             = 1,
    PP_LOOKUP_FIELD_L4_PROTOCOL_TCP             = 2,
    PP_LOOKUP_FIELD_L4_PROTOCOL_L2TPv3          = 3,
    PP_LOOKUP_FIELD_L4_PROTOCOL_ICMP            = 4,
    PP_LOOKUP_FIELD_L4_PROTOCOL_0xFF            = 15
}PP_LOOKUP_FIELD_L4_TYPES_e;

typedef enum
{
    PP_LOOKUP_FIELD_TUNNEL_TYPE_GRE                = 1,
    PP_LOOKUP_FIELD_TUNNEL_TYPE_DsLITE             = 2,
    PP_LOOKUP_FIELD_TUNNEL_TYPE_PPPoE              = 3,
    PP_LOOKUP_FIELD_TUNNEL_TYPE_6rd                = 4,
    PP_LOOKUP_FIELD_TUNNEL_TYPE_L2TPv3             = 5,
    PP_LOOKUP_FIELD_TUNNEL_TYPE_IPSEC_ESP          = 6,
    PP_LOOKUP_FIELD_TUNNEL_TYPE_GRE_IPSEC_ESP      = 7,             /* PP_LOOKUP_FIELD_TUNNEL_TYPE_GRE + PP_LOOKUP_FIELD_TUNNEL_TYPE_IPSEC_ESP */
    PP_LOOKUP_FIELD_TUNNEL_TYPE_DsLITE_IPSEC_ESP   = 8,             /* PP_LOOKUP_FIELD_TUNNEL_TYPE_DsLITE + PP_LOOKUP_FIELD_TUNNEL_TYPE_IPSEC_ESP */
    PP_LOOKUP_FIELD_TUNNEL_TYPE_IPSEC_AH           = 9,
    PP_LOOKUP_FIELD_TUNNEL_TYPE_GRE_IPSEC_AH       = 10,            /* PP_LOOKUP_FIELD_TUNNEL_TYPE_GRE + PP_LOOKUP_FIELD_TUNNEL_TYPE_IPSEC_AH */
    PP_LOOKUP_FIELD_TUNNEL_TYPE_DsLITE_IPSEC_AH    = 11,            /* PP_LOOKUP_FIELD_TUNNEL_TYPE_GRE + PP_LOOKUP_FIELD_TUNNEL_TYPE_IPSEC_AH */
    /* PP_LOOKUP_FIELD_TUNNEL_TYPE_RESERVED1          = 12, */
    /* PP_LOOKUP_FIELD_TUNNEL_TYPE_RESERVED2          = 13, */
    /* PP_LOOKUP_FIELD_TUNNEL_TYPE_RESERVED3          = 14, */
    PP_LOOKUP_FIELD_TUNNEL_TYPE_LAST               = 12
}PP_LOOKUP_FIELD_TUNNEL_TYPE_e;

typedef enum
{
    PP_LOOKUP_FIELD_FLAGS_VLAN1                 = 0x01,
    PP_LOOKUP_FIELD_FLAGS_VLAN2                 = 0x02
}PP_LOOKUP_FIELD_FLAGS_e;

#define ICMP_ECHO_REQUEST      (1<<0)       /* Identical to the classifier flags */
#define ICMP_ECHO_REPLY        (1<<1)       /* Identical to the classifier flags */

typedef struct
{
    Uint8       PID;                    /* Type: PP_PID_NUM_e                                    */
#if defined(__BIG_ENDIAN_BITFIELD)
    Uint8       L2_type:4,              /* Type: PP_LOOKUP_FIELD_L2_TYPES_e                      */
                L3_type:4;              /* Type: PP_LOOKUP_FIELD_L3_TYPES_e                      */
#elif defined (__LITTLE_ENDIAN_BITFIELD)
    Uint8       L3_type:4,              /* Type: PP_LOOKUP_FIELD_L2_TYPES_e                      */
        L2_type:4;              /* Type: PP_LOOKUP_FIELD_L3_TYPES_e                      */
#endif
#if defined(__BIG_ENDIAN_BITFIELD)
    Uint8       L4_Protocol:4,          /* Type: PP_LOOKUP_FIELD_L4_TYPES_e                      */
                Tunnel_type:4;          /* Type: PP_LOOKUP_FIELD_TUNNEL_TYPE_e                   */
#elif defined (__LITTLE_ENDIAN_BITFIELD)
    Uint8       Tunnel_type:4,          /* Type: PP_LOOKUP_FIELD_L4_TYPES_e                      */
                L4_Protocol:4;          /* Type: PP_LOOKUP_FIELD_TUNNEL_TYPE_e                   */
#endif
    Uint8       Flags;                  /* Type: PP_LOOKUP_FIELD_FLAGS_e                         */
    Uint8       dstmac[6];              /* Destination MAC address                               */
    Uint8       srcmac[6];              /* Source MAC address                                    */
    __be16      Vlan1;                  /* Packet's external VLAN if exists                      */
    __be16      Vlan2;                  /* Packet's internal VLAN if exists                      */

    union
    {
        __be32  v4;
        __be32  v6[ 4 ];
    }
    SRC_IP;                             /* For IPv4 only the first 4 MSB are set, all other bytes must be 0          */

    union
    {
        __be32  v4;
        __be32  v6[ 4 ];
    }
    DST_IP;                             /* For IPv4 only the first 4 MSB are set, all other bytes must be 0          */

    Uint8       IPv6_Flow_Label[4];     /* IPv6 Flow Label (MSbits must be 0). For DS-Lite holds IPv4 destination IP */
    Uint8       ToS;                    /* IP TOS for IPv4, Traffic Class for IPv6               */
    Uint8       Reserved;               /* Must be 0                                             */
    __be16      Tunn_L2_session_id;     /* 0xFFFF if none                                        */
    __be16      L4_SRC_PORT;            /* UDP/TCP Source       Port. If not set, must be 0      */
    __be16      L4_DST_PORT;            /* UDP/TCP Destintation Port. If not set, must be 0      */
}
__Avalanche_PP_LUTs_Data_t;

#endif

typedef enum
{
    AVALANCHE_PP_DEV_TYPE_UNKNOWN,
    AVALANCHE_PP_DEV_TYPE_WAN,
    AVALANCHE_PP_DEV_TYPE_LAN,
}
AVALANCHE_PP_DEV_TYPE_e;

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_INGRESS_SESSION_PROPERTY_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the session properties. Fundamentally a session
 *  consists of 2 properties i.e. Ingress and Egress. Each prorty internally
 *  describes the interface on which the packet was rxed (Ingress) or txed
 *  (Egress) and how the packet looked at Ingress or Egress.
 **************************************************************************/
typedef struct // former TI_PP_SESSION_PROPERTY
{
    Uint8                               vpid_handle;
#if PUMA7_OR_NEWER_SOC_TYPE
    Uint8                               host_strip_off;
#else
    Uint8                               pid_type;       /* Needed for WAN/LAN direction selection */
#endif
    Uint8                               flags;
                                        #define AVALANCHE_PP_INGRESS_MULTICAST     0x1
                                        #define AVALANCHE_PP_FULL_CLASSIFICATION   0x2
                                        #define AVALANCHE_PP_L2_CLASSIFICATION     0x4
                                       
#if PUMA7_OR_NEWER_SOC_TYPE
    Uint8                               isFirstFrag;
#else
    Uint8                               reserved1;
#endif

#if defined(PUMA6_SOC_TYPE) && PUMA6_SOC_TYPE
    Bool                                isTunnel;
#endif

    __Avalanche_PP_LUTs_Data_t          lookup;

}AVALANCHE_PP_INGRESS_SESSION_PROPERTY_t;

#if PUMA7_OR_NEWER_SOC_TYPE
#define AVALNCHE_PP_WRAP_HEADER_MAX_LEN 96
#else
#define AVALNCHE_PP_WRAP_HEADER_MAX_LEN 64

typedef enum
{
    AVALANCHE_PP_EGRESS_FIELD_ENABLE_L2                     = 0x0001,
    AVALANCHE_PP_EGRESS_FIELD_ENABLE_VLAN                   = 0x0002,
    AVALANCHE_PP_EGRESS_FIELD_ENABLE_IP                     = 0x0004,
    AVALANCHE_PP_EGRESS_FIELD_ENABLE_L4                     = 0x0008,
    AVALANCHE_PP_EGRESS_FIELD_ENABLE_PSI                    = 0x0010,
    AVALANCHE_PP_EGRESS_FIELD_ENABLE_ENCAPSULATION          = 0x0020,
    AVALANCHE_PP_EGRESS_FIELD_ENABLE_TDOX_ENABLED           = 0x0040,
    AVALANCHE_PP_EGRESS_FIELD_ENABLE_TDOX_SKIP_TIMESTAMP    = 0x0080,
    AVALANCHE_PP_EGRESS_FIELD_ENABLE_TCP_SYN                = 0x0100,
    AVALANCHE_PP_EGRESS_FIELD_ENABLE_TCP_CTRL               = 0x0200
    /* Note: The maximum for this enum is 16 bits */
}
AVALANCHE_PP_EGRESS_FIELD_ENABLE_e;

#endif
/**
 * Get bit field from 32 bits value
 *
 * @param value Value to extract the bit field from
 * @param off bit field's first bit
 * @param len bit field's length in bits
 * @return __u32
 */
static inline __u32 __bit_field32_get(__u32 val, __u32 off, __u32 len)
{
    __u32 mask;
    mask = ((0xFFFFFFFF >> (32 - len)) << off);
    return ((val & mask) >> off);
}

/**
 * Set a bit field value into a 32 bits value
 *
 * @param value Value to set the bit field
 * @param bf_val bit field value
 * @param off bit field's first bit
 * @param len bit field's length in bits
 * @return __u32
 */
static inline __u32 __bit_field32_set(__u32 var, __u32 val, __u32 off, __u32 len)
{
    __u32 mask;
    mask = ((0xFFFFFFFF >> (32 - len)) << off);
    var &= ~(mask);
    return (var | (mask & (val << off)));
}

/**
 * Get bit field from 16 bits value
 *
 * @param value Value to extract the bit field from
 * @param off bit field's first bit
 * @param len bit field's length in bits
 * @return __u16
 */
static inline __u16 __bit_field16_get(__u16 val, __u16 off, __u16 len)
{
    __u16 mask;
    mask = ((0xFFFF >> (16 - len)) << off);
    return ((val & mask) >> off);
}

/**
 * Set a bit field value into a 16 bits value
 *
 * @param value Value to set the bit field
 * @param bf_val bit field value
 * @param off bit field's first bit
 * @param len bit field's length in bits
 * @return __u16
 */
static inline __u16 __bit_field16_set(__u16 var, __u16 val, __u16 off, __u16 len)
{
        __u16 mask;
        mask = ((0xFFFF >> (16 - len)) << off);
        var &= ~(mask);
        return (var | (mask & (val << off)));
}

/**
 * Get bit field from 8 bits value
 *
 * @param value Value to extract the bit field from
 * @param off bit field's first bit
 * @param len bit field's length in bits
 * @return __u8
 */
static inline __u8 __bit_field8_get(__u8 val, __u8 off, __u8 len)
{
    __u8 mask;
    mask = ((0xFF >> (8 - len)) << off);
    return ((val & mask) >> off);
}

/**
 * Set a bit field value into a 8 bits value
 *
 * @param value Value to set the bit field
 * @param bf_val bit field value
 * @param off bit field's first bit
 * @param len bit field's length in bits
 * @return __u8
 */
static inline __u8 __bit_field8_set(__u8 var, __u8 val, __u8 off, __u8 len)
{
        __u8 mask;
        mask = ((0xFF >> (8 - len)) << off);
        var &= ~(mask);
        return (var | (mask & (val << off)));
}


#if PUMA7_OR_NEWER_SOC_TYPE
/**
 * struct pp_psi_t Packet processor protocol specific
 * information saved in the session record.
 * @psi 32bit big endian bit field:
 * [31-24] - service flow index (sf_index)
 * [23-16] - reserved (res1)
 * [15]    - don't encrypt flag
 * [14-13] - fw internal bits
 * [12]    - tdox candidate flag
 * [11]    - high priority flag
 * [10]    - tdox ignore timestamp flag
 * [9]     - tdox enabled flag
 * [0-8]   - tdox_id
 * @note ALWAYS use the pp_psi_get_* or
 *       pp_psi_set_* macros
 */
struct pp_psi_t {
    __be32  psi;
};
/* for backward compatible */
typedef struct pp_psi_t AVALANCHE_PP_PSI_t;

/* AVALANCHE_PP_PSI_t misc bit fields gets */
#define pp_psi_get_tdox_id(pp_psi)              (__bit_field32_get(be32_to_cpu(pp_psi.psi),  0, 9)) /* tdox_id bits 0-8         */
#define pp_psi_get_tdox_enabled(pp_psi)         (__bit_field32_get(be32_to_cpu(pp_psi.psi),  9, 1)) /* tdox enabled bit 9       */
#define pp_psi_get_tdox_ignore_ts(pp_psi)       (__bit_field32_get(be32_to_cpu(pp_psi.psi), 10, 1)) /* tdox_ignore_ts bit 10    */
#define pp_psi_get_high_priority(pp_psi)        (__bit_field32_get(be32_to_cpu(pp_psi.psi), 11, 1)) /* high_priority bit 11     */
#define pp_psi_get_tdox_candidate(pp_psi)       (__bit_field32_get(be32_to_cpu(pp_psi.psi), 12, 1)) /* tdox_candidate bit 12    */
#define pp_psi_get_tdox_signature(pp_psi)       (__bit_field32_get(be32_to_cpu(pp_psi.psi), 13, 1)) /* tdox_signature bit 13    */
#define pp_psi_get_res2(pp_psi)                 (__bit_field32_get(be32_to_cpu(pp_psi.psi), 14, 1)) /* reserved bit 14          */
#define pp_psi_get_do_not_encrypt(pp_psi)       (__bit_field32_get(be32_to_cpu(pp_psi.psi), 15, 1)) /* do_not_encrypt bit 15    */
#define pp_psi_get_res1(pp_psi)                 (__bit_field32_get(be32_to_cpu(pp_psi.psi), 16, 8)) /* reserved bits 16-23      */
#define pp_psi_get_sf_index(pp_psi)             (__bit_field32_get(be32_to_cpu(pp_psi.psi), 24, 8)) /* sf index bits 24-31      */

/* AVALANCHE_PP_PSI_t misc bit fields sets */
#define pp_psi_set_tdox_id(pp_psi, val)         ((pp_psi.psi) = cpu_to_be32(__bit_field32_set(be32_to_cpu(pp_psi.psi), val,  0, 9))) /* tdox_id bits 0-8         */
#define pp_psi_set_tdox_enabled(pp_psi, val)    ((pp_psi.psi) = cpu_to_be32(__bit_field32_set(be32_to_cpu(pp_psi.psi), val,  9, 1))) /* tdox enabled bit 9       */
#define pp_psi_set_tdox_ignore_ts(pp_psi, val)  ((pp_psi.psi) = cpu_to_be32(__bit_field32_set(be32_to_cpu(pp_psi.psi), val, 10, 1))) /* tdox_ignore_ts bit 10    */
#define pp_psi_set_high_priority(pp_psi, val)   ((pp_psi.psi) = cpu_to_be32(__bit_field32_set(be32_to_cpu(pp_psi.psi), val, 11, 1))) /* high_priority bit 11     */
#define pp_psi_set_tdox_candidate(pp_psi, val)  ((pp_psi.psi) = cpu_to_be32(__bit_field32_set(be32_to_cpu(pp_psi.psi), val, 12, 1))) /* tdox_candidate bit 12    */
#define pp_psi_set_tdox_signature(pp_psi, val)  ((pp_psi.psi) = cpu_to_be32(__bit_field32_set(be32_to_cpu(pp_psi.psi), val, 13, 1))) /* tdox_signature bit 13    */
#define pp_psi_set_res2(pp_psi, val)            ((pp_psi.psi) = cpu_to_be32(__bit_field32_set(be32_to_cpu(pp_psi.psi), val, 14, 1))) /* reserved bit 14          */
#define pp_psi_set_do_not_encrypt(pp_psi, val)  ((pp_psi.psi) = cpu_to_be32(__bit_field32_set(be32_to_cpu(pp_psi.psi), val, 15, 1))) /* do_not_encrypt bit 15    */
#define pp_psi_set_res1(pp_psi, val)            ((pp_psi.psi) = cpu_to_be32(__bit_field32_set(be32_to_cpu(pp_psi.psi), val, 16, 8))) /* reserved bits 16-23      */
#define pp_psi_set_sf_index(pp_psi, val)        ((pp_psi.psi) = cpu_to_be32(__bit_field32_set(be32_to_cpu(pp_psi.psi), val, 24, 8))) /* sf index bits 24-31      */
#else
typedef struct
{
    Uint8                               sf_index;
    Uint8                               phs;
    Uint8                               tcp_flags;
    Uint8                               tdox_id;
}
AVALANCHE_PP_PSI_t;
#endif

#ifdef CONFIG_WIFI_PROXY
/**
 * @class wifi traffic class
 * @vpid vpid of wifi device
 * @wifi_session_info wifi specific session info
 *
 */
struct wifi_proxy_psi_t {
    Uint8  wifi_class;
    Uint8  vpid;
    Uint16 wifi_session_info;
};
#endif

#ifndef CONFIG_WIFI_MESH_TUNNEL

#define AVALANCHE_PP_EGRESS_DROP_SESS    0x01

#if PUMA7_OR_NEWER_SOC_TYPE

#define PP_MAX_MC_MEMBERS     (8)
#define PP_MAX_MC_GROUPS      (64)
#define PP_INVALID_MC_GROUPS  (PP_MAX_MC_GROUPS)

typedef struct
{
    Uint8   Flags;
#define PP_MCAST_FLAG_MEMBER_VALID (0x01)
#define PP_MCAST_FLAG_VLAN         (0x02)
#define PP_MCAST_FLAG_PSI          (0x04)
    Uint8   Vpid;
    Uint16  EgressQ;
    Uint32  Vlan;
    Uint32  Psi;
}
PP_Multiast_Member_Info_t;

typedef struct
{
    PP_Multiast_Member_Info_t GroupMember[PP_MAX_MC_MEMBERS];
}
PP_Multiast_Group_Info_t;

typedef struct
{
    Uint8                    GroupIndex;
    Uint8                    MembersCount;
    Uint8                    WaitingMembers;
    Uint8                    Res;
    PP_Multiast_Group_Info_t groupInfo;
}
PP_Multicast_Session_Rec_t;
#endif

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_EGRESS_SESSION_PROPERTY_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the session properties. Fundamentally a session
 *  consists of 2 properties i.e. Ingress and Egress. Each property internally
 *  describes the interface on which the packet was rxed (Ingress) or txed
 *  (Egress) and how the packet looked at Ingress or Egress.
 **************************************************************************/
#if PUMA7_OR_NEWER_SOC_TYPE

typedef struct
{
    Uint8                       vpid_handle;
    Uint16                      enable;
                                #define AVALANCHE_PP_EGRESS_FIELD_ENABLE_VLAN                   0x01
                                #define AVALANCHE_PP_EGRESS_FIELD_ENABLE_TCP_SYN                0x02
                                #define AVALANCHE_PP_EGRESS_FIELD_ENABLE_TCP_CTRL               0x04
                                #define AVALANCHE_PP_EGRESS_FIELD_ENABLE_DOCSIS                 0x08
                                #define AVALANCHE_PP_EGRESS_WH_IPv4                             0x10
                                #define AVALANCHE_PP_EGRESS_WH_IPv6                             0x20
                                #define AVALANCHE_PP_EGRESS_WIFI_PID                            0x40
                                #define AVALANCHE_PP_EGRESS_TUNNEL_BY_PASS                      0x80
                                #define AVALANCHE_PP_EGRESS_MULTICAST                           0x100
                                #define AVALANCHE_PP_EGRESS_ICMP                                0x200
                                #define AVALANCHE_PP_EGRESS_ENABLE_FRAG_BP                      0x400
                                #define AVALANCHE_PP_EGRESS_FIELD_ENABLE_TCP_NO_ACK             0x800
                                #define AVALANCHE_PP_EGRESS_DO_TCP_CTRL                         0x1000
                                #define AVALANCHE_PP_EGRESS_MULTI_DROP_SESSION                  0x2000
                                #define AVALANCHE_PP_EGRESS_FIELD_MAC_HASH_CALCULATED           0x4000
    Uint8                       reserved;

    __Avalanche_PP_LUTs_Data_t  lookup;

    /**
     * All structures in this union MUST be saved in big endian
     * layout, meaning, to read or write from or to one of the
     * struct members you MUST use: be16_to_cpu, be32_to_cpu,
     * cpu_to_be16 and cpu_to_be32.
     * Also, DO NO USE bit fields!!!
     */
    union
    {
#ifdef CONFIG_WIFI_PROXY
        struct wifi_proxy_psi_t wifi_psi;
#endif
        AVALANCHE_PP_PSI_t      us_fields;
        Uint32                  psi_word;
    }
    psi;

    Uint16                      eth_type;
    Uint8                       wrapHeaderOffLayer3;
    Uint8                       wrapHeaderLen;

    Uint8                       wrapHeader[ AVALNCHE_PP_WRAP_HEADER_MAX_LEN ];

    PP_Multicast_Session_Rec_t  multicastInfo;

} AVALANCHE_PP_EGRESS_SESSION_PROPERTY_t;
#else
typedef struct // former TI_PP_SESSION_PROPERTY
{
    Uint8                               vpid_handle;
    Uint8                               pid_type;       /* Needed for WAN/LAN direction selection */

    Uint16                              enable;             /* Type: AVALANCHE_PP_EGRESS_FIELD_ENABLE_e */
    Bool                                isTunnel;

    union
    {
        AVALANCHE_PP_PSI_t              us_fields;
        Uint32                          us_psi_word;
    }
    psi;
#define psi_word                        psi.us_psi_word
#define psi_tdox_id                     psi.us_fields.tdox_id

    Uint32                              tdox_tcp_ack_number;

    Uint8                               tdox_handle;

    Uint8                               l2_packet_type;     /* Type : AVALANCHE_PP_LUT_ENTRY_TYPE_e */
    Uint8                               wrapHeaderDataLenOffset;
    Uint8                               wrapHeaderLen;

    Uint8                               wrapHeader[ AVALNCHE_PP_WRAP_HEADER_MAX_LEN ];

    Uint8                               dstmac[6];
    Uint8                               srcmac[6];
    Uint16                              vlan;
    Uint16                              eth_type;

    Uint8                               l3_packet_type;     /* Type : AVALANCHE_PP_LUT_ENTRY_TYPE_e */
    Uint8                               tunnel_type;        /* Type : AVALANCHE_PP_LUT_ENTRY_TYPE_e */
    Uint8                               ip_protocol;
    Uint8                               TOS;
    union
    {
        Uint32                          v4;
        Uint32                          v6[ 4 ];
    }
    SRC_IP;
    union
    {
        Uint32                          v4;
        Uint32                          v6[ 4 ];
    }
    DST_IP;

    Uint16                              L4_SrcPort;
    Uint16                              L4_DstPort;

    Uint16                              pppoe_sid;
    Uint8                               drop_sess;
    Uint8                               wrapHeader_type;     /* Type : AVALANCHE_PP_LUT_ENTRY_TYPE_e */
}
AVALANCHE_PP_EGRESS_SESSION_PROPERTY_t;
#endif
#else

typedef enum
{
    AVALANCHE_PP_WIFI_MESH_B_HEADER_SIZE  = 22,   /* ethernet header + 2 Vlans (22B) */
    AVALANCHE_PP_WIFI_MESH_CB_HEADER_SIZE = 49,   /* ethernet header (18B) + dragon header (13B) + ethernet header (18B) */
    AVALANCHE_PP_WIFI_MESH_CA_HEADER_SIZE = 61,   /* ethernet header (18B)- 2bytes of ethernet type + dragon header (13B) + wifi header (30B)*/

} AVALANCHE_PP_WIFI_MESH_HEADER_SIZE_e;

#define AVALNCHE_PP_WIFI_MESH_HEADER_MAX_LEN        (61)

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_EGRESS_SESSION_PROPERTY_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the session properties. Fundamentally a session
 *  consists of 2 properties i.e. Ingress and Egress. Each property internally
 *  describes the interface on which the packet was rxed (Ingress) or txed
 *  (Egress) and how the packet looked at Ingress or Egress.
 **************************************************************************/
typedef struct // former TI_PP_SESSION_PROPERTY
{
    Uint8                               vpid_handle;
    Uint8                               reserved;
    Uint16                              enable;             /* Type: AVALANCHE_PP_EGRESS_FIELD_ENABLE_e */

    Uint8                               dstmac[6];
    Uint8                               srcmac[6];

    Uint8                               newHeader[AVALNCHE_PP_WIFI_MESH_HEADER_MAX_LEN];

} AVALANCHE_PP_EGRESS_SESSION_PROPERTY_t;
#endif

#if PUMA7_OR_NEWER_SOC_TYPE
typedef enum
{
    AVALANCHE_PP_SESSIONS_TYPE_DATA,
    AVALANCHE_PP_SESSIONS_TYPE_VOICE,
    AVALANCHE_PP_SESSIONS_TYPE_MAX
}
AVALANCHE_PP_SESSIONS_TYPE_e;
#else
typedef enum
{
    AVALANCHE_PP_SESSIONS_POOL_DATA,
    AVALANCHE_PP_SESSIONS_POOL_VOICE,
    AVALANCHE_PP_SESSIONS_POOL_MAX
} AVALANCHE_PP_SESSIONS_POOL_ID_e;
#endif

typedef struct
{
    Uint32  packets_forwarded;
    Uint32  bytes_forwarded;
    Int32   last_update_time;
} AVALANCHE_PP_SESSION_TDOX_STATS_t;

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_SESSION_INFO_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the Session
 **************************************************************************/
typedef struct // former TI_PP_SESSION
{
    /* This is the session handle. Users dont need to populate this
     * information since this handle will be filled up the PPM on the
     * successful creation of the session. */
    Uint32                  session_handle;

    /* Session Timeout indicates the number of micro-seconds of inactivity
     * after which the PP generates an event to the host. The field if set
     * to 0 indicates that the session needs to be configured permanently
     * and is not subject to IDLE based timeouts. */
    Uint32                  session_timeout;

    /* Flag which indicates the priority of the session.
     * With the introduction of QoS this will play an important part.
     * priority 0 is the lowest priority, highest priority is depend
     * on the number of queues in the cluster*/
    Uint8                   priority;
#if PUMA7_OR_NEWER_SOC_TYPE
    Uint8                   phy_cluster_id;
#else
    Uint8                   cluster;
#endif
    /* Flag which indicates if the session was for a ROUTER or BRIDGE
     * This information is required because if the session is for the
     * ROUTER the PDSP needs to ensure that all packets matching their
     * session have their TTL decremented. On the other hand for BRIDGE
     * sessions this is not do be done. */
    Uint8                   is_routable_session;
#if PUMA7_OR_NEWER_SOC_TYPE
    Uint8                   session_type;   /* Type: AVALANCHE_PP_SESSIONS_TYPE_e */
#else
    Uint8                   session_pool;   /* Type: AVALANCHE_PP_SESSIONS_POOL_ID_e */
#endif

    /* Ingress Properties:-
     *  These describe the ingress interface on which the packet was
     *  received and how the packet looks like when it arrives into the
     *  box. */
    AVALANCHE_PP_INGRESS_SESSION_PROPERTY_t     ingress;

    /* Egress Properties:-
     *  These properties describe the egress interface on which the packet
     *  is to be transmitted and how the packet will look like when it
     *  leaves the box. */
    AVALANCHE_PP_EGRESS_SESSION_PROPERTY_t  egress;


#if defined(PUMA6_SOC_TYPE) && PUMA6_SOC_TYPE

    AVALANCHE_PP_SESSION_TDOX_STATS_t   tdox_stats;
#else
    /* Added 12 bytes as reserved until Atom sync */
    Uint32                  timeout_count;
    Uint32                  packet_count_at_timeout;
    Uint32                  reserved3;

    Uint8                   is_irreg_moca;
#endif
    Uint32                  scb_id;
} AVALANCHE_PP_SESSION_INFO_t;

#define HOSTD_DESC_PROTOCOL_SPECIFIC_SESSION_INDEX_MASK (0x0000FFFF)

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_SESSION_STATS_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the session statistics for the Packet Processor.
 **************************************************************************/
typedef struct // former TI_PP_SESSION_STATS
{
    Uint64          bytes_forwarded;
#if PUMA7_OR_NEWER_SOC_TYPE
    Uint64          packets_forwarded;
#else
    Uint32          packets_forwarded;
#endif
}
AVALANCHE_PP_SESSION_STATS_t;

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_EXTENDED_SESSION_STATS_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the extended session statistics used in ioctl.
 **************************************************************************/
typedef struct
{
    Uint32                          session_id;
    Bool                            session_get_ret_val;
    AVALANCHE_PP_SESSION_STATS_t    session_stats;
}
AVALANCHE_PP_EXTENDED_SESSION_STATS_t;

/**************************************************************************
 * STRUCTURE NAME : PP_HAL_SESSION_RECORD_NON_ACCELERATED_STATS_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the session non accelerated counters
 *  These counters count packets which was not accelerated and reached host
 **************************************************************************/
typedef struct
{
    Uint32  PacketCounterNonAccelerated;
    Uint32  ByteCounterNonAccelerated;
}
PP_HAL_SESSION_RECORD_NON_ACCELERATED_STATS_t;

/**************************************************************************
 * STRUCTURE NAME : TI_PP_GLOBAL_STATS
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the global statistics for the Packet Processor.
 **************************************************************************/
#if PUMA7_OR_NEWER_SOC_TYPE

typedef struct
{
    Uint32 cmp_recv      ;
    Uint32 cmp_recv_valid;
    Uint32 req_sent      ;
    Uint32 dev_cnt3      ;

} wifi_tx_dev_stats_t;

typedef struct
{
    Uint32 req_recv      ;
    Uint32 req_recv_valid;
    Uint32 req_drops     ;
    Uint32 cmp_sent      ;
} wifi_rx_dev_stats_t;

typedef struct
{
    Uint32 rx_pkts              ;
    Uint32 tx_pkts              ;
    Uint32 cmp_null_buffer      ;
    Uint32 cmp_read_retries     ;
    Uint32 cmp_irq_cnt          ;
    Uint32 cmp_addr_out_of_range;
    Uint32 cmp_pattern_buffer   ;
    Uint32 req_irq_cnt          ;
    Uint32 req_addr_out_of_range;
    Uint32 req_out_of_order     ;
    Uint32 req_int_desc_starv   ;
    Uint32 cmp_buffptr_error    ;
    Uint32 cmp_bpq_empty        ;
    Uint32 cnt13                ;
    Uint32 cnt14                ;
    Uint32 cnt15                ;
    Uint32 cnt16                ;
    Uint32 cnt17                ;
} avalanche_pp_wifi_tx_stats_t;

typedef struct
{
    Uint32 rx_pkts              ;
    Uint32 tx_pkts              ;
    Uint32 cmp_buff_starv       ;
    Uint32 req_desc_starv       ;
    Uint32 req_null_buffer      ;
    Uint32 req_zero_dateLen     ;
    Uint32 req_read_retries     ;
    Uint32 req_irq_cnt          ;
    Uint32 req_addr_out_of_range;
    Uint32 req_vpid_invalid     ;
    Uint32 req_pattern_buffer   ;
    Uint32 cmp_irq_cnt          ;
    Uint32 cmp_addr_out_of_range;
    Uint32 cnt13                ;
    Uint32 cnt14                ;
    Uint32 cnt15                ;
    Uint32 cnt16                ;
    Uint32 cnt17                ;
} avalanche_pp_wifi_rx_stats_t;

typedef struct
{
    avalanche_pp_wifi_tx_stats_t tx_stats;
    avalanche_pp_wifi_rx_stats_t rx_stats;
#ifdef __KERNEL__
    wifi_tx_dev_stats_t tx_dev_stats[WIFI_DEVS_CNT];
    wifi_rx_dev_stats_t rx_dev_stats[WIFI_DEVS_CNT];
#else
    wifi_tx_dev_stats_t tx_dev_stats[4];
    wifi_rx_dev_stats_t rx_dev_stats[4];
#endif
} avalanche_pp_wifi_stats_t;

typedef struct
{
    /* Classifier1 */
    Uint32      Classifier1_rx_pkts;
    Uint32      Classifier1_pkts_frwrd_to_next_hop;
    Uint32      Classifier1_pkts_frwrd_to_qos;
    Uint32      Classifier1_pkts_frwrd_to_reseq_for_discard;
    Uint32      Classifier1_pkts_default_hop_list_set;
    Uint32      Classifier1_pkts_default_frag_hop_list_set;
    Uint32      Classifier1_tcp_pure_ack;

    /* Classifier2 */
    Uint32      Classifier2_bithash_matches;
    Uint32      Classifier2_rx_pkts;
    Uint32      Classifier2_session_cache_searches;
    Uint32      Classifier2_session_cache_matches;
    Uint32      Classifier2_pkts_frwrd_to_classifier1;
    Uint32      Classifier2_pkts_frwrd_to_resequencer_for_discard;
    Uint32      Classifier2_res1;
    Uint32      Classifier2_res2;
    Uint32      Classifier2_res3;

    /* Modifier */
    Uint32      Modifier_rx_pkts;
    Uint32      Modifier_ipv4_pkts;
    Uint32      Modifier_ipv6_pkts;
    Uint32      Modifier_pkts_frwrd_to_nexthop;
    Uint32      Modifier_pkts_frwrd_to_qos;
    Uint32      Modifier_pkts_marked_with_ptid;
    Uint32      Modifier_oversize_pkts;
    Uint32      Modifier_pkts_modified;

    /* Session Cache */
    Uint32      SessionCache_lookup_attempts;
    Uint32      SessionCache_lookup_failures;
    Uint32      SessionCache_lookup_matches;
    Uint32      SessionCache_lookup_matches_internal;
    Uint32      SessionCache_lru_cleared;
    Uint32      SessionCache_lookup_matches_external;
    Uint32      SessionCache_lookup_matches_popped_from_internal;
    Uint32      SessionCache_no_free_McDMA_channel;

    /* Prefetcher */
    Uint32      Prefetcher_rx_pkts;
    Uint32      Prefetcher_pkts_frwrd_to_classifer2;
    Uint32      Prefetcher_descriptors_starvation;
    Uint32      Prefetcher_res1;
    Uint32      Prefetcher_res2;

    /* TurboDOX */
    Uint32      TurboDOX_not_progressing_acks; // borrowed from Prefetcher for debugging, can be returned if required
    Uint32      TurboDOX_ack_supp_limit_time_reached; // borrowed from Prefetcher for debugging, can be returned if required
    Uint32      TurboDOX_ack_supp_limit_packets_reached; // borrowed from Prefetcher for debugging, can be returned if required
    Uint32      TurboDOX_rx_pkts;
    Uint32      TurboDOX_pkts_frwrd_to_next_hop;
    Uint32      TurboDOX_pkts_frwrd_to_qos;
    Uint32      TurboDOX_enabled_pkts;
    Uint32      TurboDOX_qulified_pkts;
    Uint32      TurboDOX_non_qulified_pkts;
    Uint32      TurboDOX_suppressed_pkts;

    /* ReSequencer */
    Uint32      ReSequencer_host_rx_pkts;
    Uint32      ReSequencer_rx_pkts;
    Uint32      ReSequencer_discarded_pkts;
    Uint32      ReSequencer_match_seq_num_pkts;
    Uint32      ReSequencer_pkts_frwrd_to_next_hop;
    Uint32      ReSequencer_pkts_frwrd_to_qos;
    Uint32      ReSequencer_pkts_frwrd_to_host;
    Uint32      ReSequencer_host_forwarded_pkts;
    Uint32      ReSequencer_host_discarded_pkts;

    /* Sync Q */
    Uint32      SyncQ_allocated_q;
    Uint32      SyncQ_free;
    Uint32      SyncQ_alloc_starvation;
    Uint32      SyncQ_PTID_received;
    Uint32      SyncQ_timer_expired;
    Uint32      SyncQ_threshold_overflow;
    Uint32      SyncQ_PTID_sent;
    Uint32      SyncQ_Sent_To_SyncQ;
    Uint32      SyncQ_Not_Allocated_HostPkt;
    Uint32      SyncQ_Not_Allocated_AccPkt;

} AVALANCHE_PP_STAGE1_STATS_t;

typedef struct
{
    Uint32      QoS_rx_pkts;
    Uint32      QoS_pkts_forward_manually;
    Uint32      QoS_pkts_forward_using_full_divert;
    Uint32      QoS_pkts_forward_using_qda;
    Uint32      QoS_dropped_pkts;
    Uint32      QoS_empty_q_on_manual_forwarding;
    Uint32      QoS_res7;
    Uint32      QoS_res8;

    Uint32      Accumulator_rx_pkts;
    Uint32      Accumulator_interrupts;
    Uint32      Accumulator_drops;
    Uint32      Accumulator_res4;
    Uint32      Accumulator_res5;
    Uint32      Accumulator_res6;
    Uint32      Accumulator_res7;
    Uint32      Accumulator_res8;

    Uint32      Recycler_gqmgr0;
    Uint32      Recycler_gqmgr1;
    Uint32      Recycler_gqmgr2;
    Uint32      Recycler_usqmgr;
    Uint32      Recycler_loqmgr;
    Uint32      Recycler_res6;
    Uint32      Recycler_res7;
    Uint32      Recycler_res8;
} AVALANCHE_PP_STAGE2_Part1_STATS_t;

typedef struct
{
    Uint32      Sequencer_rx_pkts;
    Uint32      Sequencer_pkts_frwrd_to_prefetcher;
    Uint32      Sequencer_pkts_frwrd_to_defaultQ;
    Uint32      Sequencer_pkts_discarded;
    Uint32      Sequencer_fifo_full;
    Uint32      Sequencer_res6;
    Uint32      Sequencer_res7;
    Uint32      Sequencer_res8;

    Uint32      AQM_pkts_frwrd_to_QoS;
    Uint32      AQM_pkts_discarded;
    Uint32      AQM_move_to_dls_interrupt;
    Uint32      AQM_move_to_normal_mode_intrpt;
    Uint32      AQM_sent_wakeup_upon_timer_expiry;
    Uint32      AQM_sent_wakeup_upon_bytes_thrshold_crossed;
    Uint32      MCAST_rx_pkts;
    Uint32      MCAST_tx_pkts;

    Uint32      DPI_res1;
    Uint32      DPI_res2;
    Uint32      DPI_res3;
    Uint32      DPI_res4;
    Uint32      DPI_res5;
    Uint32      DPI_res6;
    Uint32      DPI_res7;
    Uint32      DPI_res8;

    Uint32      MC_rx_pkts;
    Uint32      MC_pkts_frwrd_to_qos;
    Uint32      MC_pkts_frwrd_to_host;
    Uint32      MC_pkts_frwrd_to_next_hop;
    Uint32      MC_pkts_pushed_to_db;
    Uint32      MC_pkts_popped_from_db;
    Uint32      MC_pkts_timed_out_from_db;
    Uint32      MC_db_overflow_pkts;
    Uint32      MC_reassembly_concat_pkts;
    Uint32      MC_reassembly_no_concat_pkts;
    Uint32      MC_fragmenation_desc_starvations;
    Uint32      MC_fragmenation_on_tunnel_header;
    Uint32      MC_fragmenation_on_encap_pkt;
    Uint32      MC_res14;
    Uint32      MC_res15;
    Uint32      MC_res16;

    avalanche_pp_wifi_stats_t wifi_stats;

    Uint32      MoCA_rx_pkts;
    Uint32      MoCA_tx_pkts;
    Uint32      MoCA_res3;
    Uint32      MoCA_res4;
    Uint32      MoCA_res5;
    Uint32      MoCA_res6;
    Uint32      MoCA_res7;
    Uint32      MoCA_res8;
} AVALANCHE_PP_STAGE2_Part2_STATS_t;

typedef struct
{
    Uint32      NoSessionCounter;
    Uint32      NoSessionFragCounter;
    Uint32      MinimumSessionCounter;
    Uint32      UsDocsisTcpSessionCounter;
    Uint32      TunnelEncapSessionCounter;
    Uint32      TunnelDecapSessionCounter;
    Uint32      TunnelEncapUsDocsisTcpSessionCounter;
}AVALANCHE_HOP_LIST_STATS_t;

#define AVALANCHE_PP_US_SERVICE_FLOW_COUNTERS 16
typedef struct
{
    // one 64-bit counter per service flow. 16 is a reasonable upper bound on the number of active service flows, although the system supports more (56?).
    // In reality, only 1-2 flows are used.
    Uint64 us_sent_packets[AVALANCHE_PP_US_SERVICE_FLOW_COUNTERS];
} AVALANCHE_PP_US_STATS_t;

typedef struct
{
    AVALANCHE_PP_STAGE1_STATS_t         stage1;
    AVALANCHE_PP_STAGE2_Part1_STATS_t   stage2_1;
    AVALANCHE_PP_STAGE2_Part2_STATS_t   stage2_2;
    AVALANCHE_HOP_LIST_STATS_t          hopListCounters;                        /* Not PP counters */
} AVALANCHE_PP_GLOBAL_STATS_t;

#else
typedef struct // former TI_PP_GLOBAL_STATS
{
    Uint32      ppdsp_rx_pkts;
    Uint32      ppdsp_pkts_frwrd_to_cpdsp1;
    Uint32      ppdsp_not_enough_descriptors;

    Uint32      cpdsp1_rx_pkts;
    Uint32      cpdsp1_lut1_search_attempts;
    Uint32      cpdsp1_lut1_matches;
    Uint32      cpdsp1_pkts_frwrd_to_cpdsp2;

    Uint32      cpdsp2_rx_pkts;
    Uint32      cpdsp2_lut2_search_attempts;
    Uint32      cpdsp2_lut2_matches;
    Uint32      cpdsp2_pkts_frwrd_to_mpdsp;
    Uint32      cpdsp2_synch_timeout_events;
    Uint32      cpdsp2_reassembly_db_full;
    Uint32      cpdsp2_reassembly_db_timeout;

    Uint32      mpdsp_rx_pkts;
    Uint32      mpdsp_ipv4_rx_pkts;
    Uint32      mpdsp_ipv6_rx_pkts;
    Uint32      mpdsp_frwrd_to_host;
    Uint32      mpdsp_frwrd_to_qpdsp;
    Uint32      mpdsp_frwrd_to_synch_q;
    Uint32      mpdsp_discards;
    Uint32      mpdsp_synchq_overflow_events;

    Uint32      qpdsp_ooo_discards;
} AVALANCHE_PP_GLOBAL_STATS_t;
#endif

/**************************************************************************
 * STRUCTURE NAME : AVALANCHE_PP_SESSION_DELETE_CB_PARAM1_t
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the param1 of the session delete function call back.
 *  Since the session handle is maximum 2 bytes value, it possible to use the
 *  other 2 bytes for additional info, 1 byte will be used for saving the
 *  cpe_number of the deleted session
 * Note: former TI_PP_GLOBAL_STATS
 **************************************************************************/
struct AVALANCHE_PP_SESSION_DELETE_CB_PARAM1_t{

    Uint32  session_handle;
    Uint8   cpe_number;
    Uint8   spare;
};

typedef enum
{
    PP_RC_SUCCESS,
    PP_RC_FAILURE,
    PP_RC_INVALID_PARAM,
    PP_RC_OUT_OF_MEMORY,
    PP_RC_OBJECT_EXIST,

    PP_RC_FW_FAILURE = 100,
}
AVALANCHE_PP_RET_e;

typedef enum {
    AVALANCHE_PP_DDH_MODE_INTERNAL,
    AVALANCHE_PP_DDH_MODE_EXTERNAL,
} AVALANCHE_PP_Defensive_Mode_e;

typedef enum {
    AVALANCHE_PP_DDH_STATE_NORMAL           = 0x0,
    AVALANCHE_PP_DDH_STATE_DS_DEFENSIVE     = 0x1,
    AVALANCHE_PP_DDH_STATE_FULL_DEFENSIVE   = 0x2,
    AVALANCHE_PP_DDH_STATES_COUNT           /* Must be the last enum! */
} AVALANCHE_PP_DDH_STATE_e;

enum AVALANCHE_PP_DDH_NOTIFICATIONS_STATE {
    AVALANCHE_PP_DDH_NOTIFICATIONS_STATE_NORMAL = 0x0,
    AVALANCHE_PP_DDH_NOTIFICATIONS_STATE_DOS = 0x1,
};

typedef Uint8                            avalanche_pp_ackSupp_ioctl_param_t;
typedef Uint8                            avalanche_pp_psm_ioctl_param_t;
typedef Uint32                           avalanche_pp_frag_mode_ioctl_param_t;
typedef Uint32                           avalanche_pp_defensive_state_notification_timeout_ioctl_param_t;
typedef AVALANCHE_PP_Defensive_Mode_e    avalanche_pp_defensive_mode_ioctl_param_t;
typedef Uint8                            avalanche_pp_defensive_state_ioctl_param_t;

#if defined(PUMA6_SOC_TYPE) && PUMA6_SOC_TYPE
typedef struct {

    Uint8   bytePkts;
    Uint32  index;
    Uint32  newValue;
}avalanche_pp_Qos_ioctl_params_t;
#endif

/* There QoS may be defined either for physical or virtual device
The QoS setting hooks are being triggered by PID creation.
In case there is a need in alternative QoS scheme to be created it can be
specified by setting the  qos_virtual_scheme_idx to a valid non default index
This alternative scheme creation is being triggered from VPID creation.
It has to be defined by the appropriate device drived */
#define AVALANCHE_PP_NETDEV_PP_QOS_PROFILE_DEFAULT   (-1)

typedef struct {
    Int8    device_name[16];
    Int32   qos_virtual_scheme_idx;
}avalanche_pp_dev_ioctl_param_t;

enum AVALANCHE_PP_DDH_NOTIFICATIONS_MESSAGE_TYPE {
    AVALANCHE_PP_NEW_MODE_NOTIFICATION_MSG_TYPE,
};

struct AVALANCHE_PP_DDH_NOTIFICATIONS_INFO {
    enum AVALANCHE_PP_DDH_NOTIFICATIONS_STATE current_state;
    Uint32                                    number_of_us_sessions;
    Uint32                                    number_of_ds_sessions;
};

struct AVALANCHE_PP_DDH_NOTIFICATIONS_MESSAGE {
    enum AVALANCHE_PP_DDH_NOTIFICATIONS_MESSAGE_TYPE message_type;
    union {
        struct AVALANCHE_PP_DDH_NOTIFICATIONS_INFO   ddh_state_info;
    } u;
};

#ifdef __KERNEL__
/* **************************************************************************************** */
/*                                                                                          */
/*                                                                                          */
/*                                                                                          */
/*                      KERNEL only Stuff                                                   */
/*                                                                                          */
/*                                                                                          */
/*                                                                                          */
/* **************************************************************************************** */

typedef enum
{
    PP_LIST_ID_INGRESS,
    PP_LIST_ID_EGRESS,
    PP_LIST_ID_EGRESS_TCP,
    PP_LIST_ID_EGRESS_TDOX,
    PP_LIST_ID_ALL,
}
PP_LIST_ID_e;

typedef enum
{
    AVALANCHE_PP_BLACKLIST_EMPTY    = 0x0,
    AVALANCHE_PP_BLACKLIST_FULL     = 0x1,
    AVALANCHE_PP_BLACKLIST_IN_USE   = 0x2,
}
AVALANCHE_PP_BLACKLIST_INFO_e;


typedef AVALANCHE_PP_RET_e   (* AVALANCHE_DDH_NOTIFY_FN_t)(AVALANCHE_PP_DDH_STATE_e *state);
typedef AVALANCHE_PP_RET_e   (* AVALANCHE_EXEC_HOOK_FN_t)(AVALANCHE_PP_SESSION_INFO_t *ptr_session, Ptr data);

typedef struct
{
    AVALANCHE_PP_DDH_STATE_e    ddh_state;                          /* Current DDH state */
    AVALANCHE_DDH_NOTIFY_FN_t   notify_cb;                          /* DDH Event notification callback */
    Bool                        multi_drop_enabled;                 /* A flag indicates if multi-drop is enabled or not */
    Bool                        smart_prioritization_enabled;
    Bool                        l2_classification_enabled;
    Bool                        l2_classification_enabled_by_default;
    Uint8                       l2_classification_pid;
}
AVALANCHE_PP_Defensive_State_t;
#define SCB_FLAG_VALID_CONNECTION    0x1    /* Set the flag to 1 indicates that this a valid connection */
#define SCB_FLAG_CONNTRACK_ATTACHED  0x2    /* Set the flag to 1 indicates that this an active connection */ 
#define SCB_FLAG_ZOMBIE              0x4    /* Set the flag to 1 indicates that this connection is about to be deleted */ 
typedef struct
{
    Uint32  conntrack_handle;
    Uint32  scb_packet_count;            // Counts number of host packet in this short connection
    Uint32  scb_flags;
}
AVALANCHE_PP_SCB_Entry_t;

/* PID and VPID Management API */
#if PUMA7_OR_NEWER_SOC_TYPE
extern AVALANCHE_PP_RET_e    avalanche_pp_open_acc_ch           ( AVALANCHE_PP_ACC_CH_INFO_t * ptr_ch_cfg);
extern AVALANCHE_PP_RET_e    avalanche_pp_close_acc_ch          ( AVALANCHE_PP_ACC_CH_INFO_t * ptr_ch_cfg);
extern AVALANCHE_PP_RET_e    avalanche_pp_set_sync_q_max_thrshold( Uint16 threshold );
extern AVALANCHE_PP_RET_e    avalanche_pp_counter64_read        ( Uint64 * dest, volatile Ptr src );
#endif
extern AVALANCHE_PP_RET_e    avalanche_pp_pid_create            ( AVALANCHE_PP_PID_t * ptr_pid, void * ptr_netdev );
extern AVALANCHE_PP_RET_e    avalanche_pp_pid_delete            ( Uint8     pid_handle );
extern AVALANCHE_PP_RET_e    avalanche_pp_pid_config_range      ( AVALANCHE_PP_PID_RANGE_t *    pid_range );
extern AVALANCHE_PP_RET_e    avalanche_pp_pid_remove_range      ( Uint32    port_num );
extern AVALANCHE_PP_RET_e    avalanche_pp_pid_set_flags         ( Uint8     pid_handle,     Uint32  new_flags );
extern AVALANCHE_PP_RET_e    avalanche_pp_pid_get_list          ( Uint8 *   num_entries, AVALANCHE_PP_PID_t ** pid_list );
extern AVALANCHE_PP_RET_e    avalanche_pp_pid_get_info          ( Uint8     pid_handle,  AVALANCHE_PP_PID_t ** ptr_pid );

extern AVALANCHE_PP_RET_e    avalanche_pp_vpid_create           ( AVALANCHE_PP_VPID_INFO_t *    ptr_vpid );
extern AVALANCHE_PP_RET_e    avalanche_pp_vpid_delete           ( Uint8     vpid_handle );
extern AVALANCHE_PP_RET_e    avalanche_pp_vpid_set_flags        ( Uint8     vpid_handle,    Uint32  new_flags );
extern AVALANCHE_PP_RET_e    avalanche_pp_vpid_get_list         ( Uint8     parent_pid_handle,  Uint8 *     num_entries, AVALANCHE_PP_VPID_INFO_t **     vpid_list );
extern AVALANCHE_PP_RET_e    avalanche_pp_vpid_get_info         ( Uint8     vpid_handle,                                 AVALANCHE_PP_VPID_INFO_t **     ptr_vpid );

#ifdef CONFIG_WIFI_MESH_TUNNEL
extern AVALANCHE_PP_RET_e    avalanche_pp_wifi_mesh_get_vpid_packet_header_size           ( AVALANCHE_PP_VPID_TYPE_e        vpid_type, AVALANCHE_PP_WIFI_MESH_HEADER_SIZE_e* headerSize);
#endif

/* Session Management API */
extern AVALANCHE_PP_RET_e    avalanche_pp_session_create        ( AVALANCHE_PP_SESSION_INFO_t *  ptr_session, void * pkt_ptr );
#if PUMA7_OR_NEWER_SOC_TYPE
/* Multicast support */
extern AVALANCHE_PP_RET_e    avalanche_pp_check_multiple_egress ( AVALANCHE_PP_SESSION_INFO_t *ptr_session );
extern AVALANCHE_PP_RET_e    avalanche_pp_del_mcast_member      ( void *ptr );
extern AVALANCHE_PP_RET_e    avalanche_pp_add_mcast_member      ( void *ptr );
extern AVALANCHE_PP_RET_e    avalanche_pp_set_mc_vpid_info_timeout (Uint8 timeoutSeconds);
extern AVALANCHE_PP_RET_e    avalanche_pp_get_mc_vpid_info_timeout (Uint8 *timeoutSeconds);
extern AVALANCHE_PP_RET_e    avalanche_pp_mcast_dbg_add_member  ( Uint8 group_index, Uint8 vpid, Uint16 cluster, Uint32 psi );
extern AVALANCHE_PP_RET_e    avalanche_pp_delete_mc_session     ( void *ptr, Bool deleteMCVpidInfo );

extern AVALANCHE_PP_RET_e    avalanche_pp_session_delete        ( Uint32    session_handle,     AVALANCHE_PP_SESSION_STATS_t *  ptr_session_stats, Bool deleteFromFW, Bool deleteMCVpidInfo );
extern AVALANCHE_PP_RET_e    avalanche_pp_override_egress_queue ( Uint32 session_handle,     Uint8 override_qMgr,                               Uint16 override_qNum);
#else
extern AVALANCHE_PP_RET_e    avalanche_pp_session_delete        ( Uint32    session_handle,     AVALANCHE_PP_SESSION_STATS_t *  ptr_session_stats );
#endif
extern AVALANCHE_PP_RET_e    avalanche_pp_session_get_list      ( Uint8     vpid_handle,        PP_LIST_ID_e   list_id, Uint32 * num_entries, Uint32 * session_handle_list );
extern AVALANCHE_PP_RET_e    avalanche_pp_session_get_info      ( Uint32    session_handle,     AVALANCHE_PP_SESSION_INFO_t**  ptr_session_info );
extern AVALANCHE_PP_RET_e    avalanche_pp_session_set_timeout   ( Uint8     timeoutSeconds);
extern AVALANCHE_PP_RET_e    avalanche_pp_flush_sessions        ( Uint8     vpid_handle, PP_LIST_ID_e   list_id );

extern AVALANCHE_PP_RET_e   avalanche_pp_session_list_execute      ( Uint8     vpid_handle, PP_LIST_ID_e   list_id,    AVALANCHE_EXEC_HOOK_FN_t   handler, Ptr  data );
extern AVALANCHE_PP_RET_e   avalanche_pp_flush_sessions_per_mac    ( Uint8     mac[6]);
extern AVALANCHE_PP_RET_e   avalanche_pp_session_pre_action_bind   ( Uint8     vpid_handle,                            AVALANCHE_EXEC_HOOK_FN_t   handler, Ptr  data );
extern AVALANCHE_PP_RET_e   avalanche_pp_session_post_action_bind  ( Uint8     vpid_handle,                            AVALANCHE_EXEC_HOOK_FN_t   handler, Ptr  data );

#ifdef CONFIG_WIFI_MESH_TUNNEL
extern AVALANCHE_PP_RET_e    avalanche_pp_sessions_delete_by_addr ( Uint8 mask, Uint8* srcMacAdr, Uint8* dstMacAdr, Uint8* rxMacAdr, Uint8* txMacAdr, Uint32* num_deleted_sessions);
#endif

/* Statistics API */
extern AVALANCHE_PP_RET_e   avalanche_pp_modify_stats_counters              ( Uint32 session_handle, Uint32 packet_size);
extern AVALANCHE_PP_RET_e   avalanche_pp_get_stats_session                  ( Uint32 session_handle, AVALANCHE_PP_SESSION_STATS_t* ptr_session_stats );
extern AVALANCHE_PP_RET_e   avalanche_pp_get_stats_vpid                     ( Uint8  vpid_handle, AVALANCHE_PP_VPID_STATS_t* ptr_vpid_stats );
extern AVALANCHE_PP_RET_e   avalanche_pp_get_stats_global                   ( AVALANCHE_PP_GLOBAL_STATS_t* ptr_stats );
extern AVALANCHE_PP_RET_e   avalanche_pp_reset_stats_global                 ( void );
extern void                 avalanche_pp_substract_stats_global_counters(AVALANCHE_PP_GLOBAL_STATS_t* old_stats, AVALANCHE_PP_GLOBAL_STATS_t* new_stats);

extern AVALANCHE_PP_RET_e   avalanche_pp_set_defensive_state (AVALANCHE_PP_DDH_STATE_e state);
extern AVALANCHE_PP_RET_e   avalanche_pp_get_defensive_state (AVALANCHE_PP_DDH_STATE_e *state);
extern AVALANCHE_PP_RET_e   avalanche_pp_set_external_defensive_state(avalanche_pp_defensive_state_ioctl_param_t state);
extern AVALANCHE_PP_RET_e   avalanche_pp_set_l2classification_default_mode(Bool l2_classification_default_mode);
extern AVALANCHE_PP_RET_e   avalanche_pp_get_l2classification_default_mode(Bool *l2_classification_default_mode);
extern AVALANCHE_PP_RET_e   avalanche_pp_set_defensive_mode  (AVALANCHE_PP_Defensive_Mode_e defensive_mode);
extern AVALANCHE_PP_RET_e   avalanche_pp_get_defensive_mode  (AVALANCHE_PP_Defensive_Mode_e* defensive_mode);
extern AVALANCHE_PP_RET_e   avalanche_pp_support_multi_drop  (Bool enable_multi_drop);
extern AVALANCHE_PP_RET_e   avalanche_pp_support_smart_prioritization (Bool enable_smart_prioritization);
extern AVALANCHE_PP_RET_e   avalanche_pp_get_smart_prioritization  (Bool *smart_prioritization_enabled);
extern AVALANCHE_PP_RET_e   avalanche_pp_support_l2_classification  (Bool enable_l2_classification, Uint8 pid_handle);
extern AVALANCHE_PP_RET_e   avalanche_pp_get_l2_classification  (Bool *l2_classification_enabled, Uint8* pid_handle);
extern AVALANCHE_PP_RET_e   avalanche_pp_get_blacklist_info(Bool *multi_drop_enabled, Uint8 ip_type, AVALANCHE_PP_BLACKLIST_INFO_e *blacklist_info);
extern AVALANCHE_PP_RET_e   avalanche_pp_stub_qos_get_queue_stats (Uint32 qos_qnum, AVALANCHE_PP_QOS_QUEUE_STATS_t* stats);
extern AVALANCHE_PP_RET_e   avalanche_pp_get_host_packets(Uint32 *host_packets);
extern AVALANCHE_PP_RET_e   avalanche_pp_defensive_state_notify_bind (AVALANCHE_DDH_NOTIFY_FN_t handler);

#ifdef CONFIG_WIFI_MESH_TUNNEL

#define SRC_MAC_ADDR_COMPARE       BIT0
#define DST_MAC_ADDR_COMPARE       BIT1
#define TX_MAC_ADDR_COMPARE        BIT2
#define RX_MAC_ADDR_COMPARE        BIT3

#define MAC_ADDR_SIZE              6
#endif

/* Event Handler Framework API */
typedef enum
{
    PP_EV_PID_CREATED                   ,
    PP_EV_PID_CREATE_FAIL               ,
    PP_EV_PID_DELETED                   ,
    PP_EV_PID_DELETE_FAIL               ,
    PP_EV_VPID_CREATED                  ,
    PP_EV_VPID_CREATE_FAILED            ,
    PP_EV_VPID_DELETED                  ,
    PP_EV_VPID_DELETE_FAILED            ,
    PP_EV_SESSION_CREATED               ,
    PP_EV_SESSION_CREATE_FAILED         ,
    PP_EV_SESSION_DELETED               ,
    PP_EV_SESSION_DELETE_FAILED         ,
    PP_EV_SESSION_EXPIRED               ,
    PP_EV_MISC_TRIGGER_TDOX_EVALUATION  ,
    PP_EV_DDH                           ,
    PP_EV_DDH_NOTIFY                    ,
    PP_EV_MAX                           ,
    PP_EV_MAXVAL = 0xFFFFFFFF
}
AVALANCHE_PP_EVENT_e;

typedef AVALANCHE_PP_RET_e  (* AVALANCHE_EVENT_HANDLER_t)   ( AVALANCHE_PP_EVENT_e  event, Uint32  param1, Uint32 param2 );

extern AVALANCHE_PP_RET_e   avalanche_pp_set_ack_suppression        ( Uint8     enDis );
extern AVALANCHE_PP_RET_e   avalanche_pp_configure_ack_suppression  ( Uint8 maxPackets, Uint32 maxTime );
extern AVALANCHE_PP_RET_e   avalanche_pp_set_traffic_prioritization_mode ( Uint8     priorityMode);

extern AVALANCHE_PP_RET_e   avalanche_pp_event_handler_register     ( Uint32 *  handle_event_handler, AVALANCHE_EVENT_HANDLER_t   handler );
extern AVALANCHE_PP_RET_e   avalanche_pp_event_handler_unregister   ( Uint32    handle_event_handler );
extern AVALANCHE_PP_RET_e   avalanche_pp_event_report( AVALANCHE_PP_EVENT_e  event, Uint32 param1, Uint32 param2 );

#if defined(PUMA6_SOC_TYPE) && PUMA6_SOC_TYPE
/* QoS API. */
extern AVALANCHE_PP_RET_e   avalanche_pp_qos_cluster_setup      ( Uint8     clst_indx,  AVALANCHE_PP_QOS_CLST_CFG_t*    clst_cfg );
extern AVALANCHE_PP_RET_e   avalanche_pp_qos_cluster_enable     ( Uint8     clst_indx );
extern AVALANCHE_PP_RET_e   avalanche_pp_qos_cluster_disable    ( Uint8     clst_indx );
extern AVALANCHE_PP_RET_e   avalanche_pp_qos_get_queue_stats    ( Uint32    qos_qnum,   AVALANCHE_PP_QOS_QUEUE_STATS_t*     stats );
extern AVALANCHE_PP_RET_e   avalanche_pp_qos_set_cluster_max_global_credit  ( Bool creditTypeBytes, Uint8 cluster_id,   Uint32 max_global_credit );
extern AVALANCHE_PP_RET_e   avalanche_pp_qos_set_queue_max_credit           ( Bool creditTypeBytes, Uint8 queue_id,     Uint32 max_credit );
extern AVALANCHE_PP_RET_e   avalanche_pp_qos_set_queue_iteration_credit     ( Bool creditTypeBytes, Uint8 queue_id,     Uint32 it_credit  );
extern AVALANCHE_PP_RET_e   avalanche_pp_qos_set_queue_congestion           ( Bool creditTypeBytes, Uint8 queue_id,     Uint32 it_credit  );
#endif

/* Power Saving Mode (PSM) API. */
extern AVALANCHE_PP_RET_e    avalanche_pp_psm                       ( Uint8     onOff );
extern AVALANCHE_PP_RET_e    avalanche_pp_hw_init                   ( void );
extern AVALANCHE_PP_RET_e    avalanche_pp_hw_interrupt_bundle_init  ( void );

#if PUMA7_OR_NEWER_SOC_TYPE
extern AVALANCHE_PP_RET_e    avalanche_pp_set_classification_mode(PP_CLASSIFICATION_MODE_e);
extern AVALANCHE_PP_RET_e    avalanche_pp_set_support_bp_frag(Uint8 EnDis);
extern PP_CLASSIFICATION_MODE_e avalanche_pp_get_classification_mode( void );
#ifdef CONFIG_SESS_TIMEOUT_BY_THRESH
extern AVALANCHE_PP_RET_e avalanche_pp_sess_timeout_thresholds_enable(Uint8);
extern AVALANCHE_PP_RET_e avalanche_pp_sess_timeout_thresholds_set(Uint16, Uint16);
#endif

extern AVALANCHE_PP_RET_e avalanche_set_debug_g_queue_num(Uint16 queue_num, Uint16 debugSet); 
#endif

/* MISC APIs */
#define AVALANCHE_PP_LUT_HISTOGRAM_SIZE             8
#if defined(PUMA6_SOC_TYPE) && PUMA6_SOC_TYPE
#define AVALANCHE_PP_LUT1_HISTOGRAM_RESOLUTION      (AVALANCHE_PP_MAX_LUT1_KEYS / AVALANCHE_PP_LUT_HISTOGRAM_SIZE)
#define AVALANCHE_PP_LUT2_HISTOGRAM_RESOLUTION      (AVALANCHE_PP_MAX_ACCELERATED_SESSIONS / AVALANCHE_PP_LUT_HISTOGRAM_SIZE)
#endif

typedef struct
{
    Uint32                              active_PIDs;
    Uint32                              active_VPIDs;
    Uint32                              active_sessions;
    Uint32                              active_us_sessions;
    Uint32                              active_ds_sessions;
    Uint32                              active_drop_sessions;
    Uint32                              active_multi_drop_sessions;
    Uint32                              active_irreg_moca_sessions;
    Uint32                              max_active_sessions;
    Uint32                              lut2_histogram[AVALANCHE_PP_LUT_HISTOGRAM_SIZE];
    Uint32                              lut2_starvation;
    Uint32                              tdox_starvation;
#if PUMA7_OR_NEWER_SOC_TYPE
    Uint32                              tdox_sessions;
    Uint32                              tdox_candidates;
#else
    Uint32                              lut1_histogram[AVALANCHE_PP_LUT_HISTOGRAM_SIZE];
    Uint32                              lut1_starvation;
    Uint32                              active_lut1_keys;
    Uint32                              max_active_lut1_keys;
#endif
}
AVALANCHE_PP_Misc_Statistics_t;

#if PUMA7_OR_NEWER_SOC_TYPE
extern AVALANCHE_PP_RET_e    avalanche_pp_event_init(void);
#endif

extern AVALANCHE_PP_RET_e    avalanche_pp_event_poll_timer_init(void);
extern AVALANCHE_PP_RET_e    avalanche_pp_set_ddh_notify_event_interval(Uint32 number_of_miliseconds_for_interval);
extern AVALANCHE_PP_RET_e    avalanche_pp_reset_ddh_notify_event_counter(void);

#ifndef CONFIG_WIFI_MESH_TUNNEL
extern AVALANCHE_PP_RET_e    avalanche_pp_session_tdox_capability_set( Uint32 session_handle, Bool     enable );
#if PUMA7_OR_NEWER_SOC_TYPE
extern AVALANCHE_PP_RET_e    avalanche_pp_session_tdox_capability_get( Uint32 session_handle, Bool *   enable, Bool * qualified );
extern AVALANCHE_PP_RET_e    avalanche_pp_get_tdox_candidate_session( Uint32* session_handle);
extern AVALANCHE_PP_RET_e    avalanche_pp_configure_tdox_params( Uint16 avgPktSize,  Uint16 evalPPS, Uint16 evalTime);
extern Uint32                avalanche_pp_get_max_sessions( void );
#else
extern AVALANCHE_PP_RET_e    avalanche_pp_session_tdox_capability_get( Uint32 session_handle, Bool *   enable );
#endif
#endif
extern AVALANCHE_PP_RET_e    avalanche_pp_version_get( AVALANCHE_PP_VERSION_t * version );
extern AVALANCHE_PP_RET_e    avalanche_pp_get_db_stats ( AVALANCHE_PP_Misc_Statistics_t * stats_ptr );
extern AVALANCHE_PP_RET_e    avalanche_pp_reset_db_stats ( void );
extern AVALANCHE_PP_RET_e    avalanche_pp_modify_stats_counters   ( Uint32 session_handle, Uint32 packet_size);

extern Bool                  avalanche_pp_state_is_active( void );
extern Bool                  avalanche_pp_state_is_psm( void );

/*---------------------*/
/*    WIFI Proxy       */
/*---------------------*/
#ifdef CONFIG_WIFI_PROXY

/**
 * Ring configuration structure for the sender entity
 * @base_addr ring base address
 * @elem_sz_order element size power of 2 order
 * @sz_mask ring size mask, number of elements can be written to
 *          the ring in a mask form, (4 elements) = 0b11, (8
 *          elements) = 0b111 and so on
 * @hw_cnt_addr HW counter address
 * @rmt_head_ptr_addr remote head pointer address
 * @note this struct is aligned to wifi fw pdsps mailbox
 *       commands, DO NOT change it.
 * @note this struct MUST be 4 bytes aligned
 */
struct send_ring_t {
    Uint32  base_addr;
    Uint16  elem_sz_order;
    Uint16  sz_mask;
    Uint32  hw_cnt_addr;
    Uint32  rmt_head_ptr_addr;
};

/**
 * Ring configuration structure for the receiver entity
 * @base_addr ring base address
 * @elem_sz_order element size power of 2 order
 * @sz_mask ring size mask, number of elements can be written to
 *          the ring in a mask form, (4 elements) = 0b11, (8
 *          elements) = 0b111 and so on
 * @hw_cnt_addr HW counter address
 * @rmt_tail_ptr_addr remote tail pointer address
 * @note this struct is aligned to wifi fw pdsps mailbox
 *       commands, DO NOT change it.
 * @note this struct MUST be 4 bytes aligned
 */
struct recv_ring_t {
    Uint32  base_addr;
    Uint16  elem_sz_order;
    Uint16  sz_mask;
    Uint32  hw_cnt_addr;
    Uint32  rmt_tail_ptr_addr;
};

/**
 * wifi tx channel rings configuration, FW to wifi SOC
 * @req_ring used by the FW to send packets to wifi SOC
 * @cmp_ring used by the wifi SOC to recycle buffers back to FW
 * @note this struct MUST be 4 bytes aligned
 */
struct wifi_proxy_tx_ch_t {
    struct send_ring_t req_ring;
    struct recv_ring_t cmp_ring;
};

/**
 * wifi rx channel rings configuration, wifi SOC to FW
 * @req_ring used by FW to send wifi SOC empty buffers to use
 * @cmp_ring used by wifi SOC to send packets to FW
 * @note this struct MUST be 4 bytes aligned
 */
struct wifi_proxy_rx_ch_t {
    struct send_ring_t cmp_ring;
    struct recv_ring_t req_ring;
};

/**
 * wifi device channel configuration.
 * @wifi_dev_id device's index
 * @tx_channel tx channel configuration
 * @rx_channel rx channel configuration
 */
struct wifi_proxy_dev_ch_t {
    wifi_dev_id_e       wifi_dev_id;
    struct wifi_proxy_tx_ch_t tx_channel;
    struct wifi_proxy_rx_ch_t rx_channel;
};

/**
 * wifi_proxy_fw_init_info_t
 * @bMgr tx & rx buffers's buffer manager
 * @bPool tx & rx buffers's buffer pool
 * @tx_int_qMgr tx pdsp internal descriptors free queue manager
 * @tx_int_qNum tx pdsp internal descriptors free queue number
 * @tx_int_cnt number of desriptors in tx_int free queue
 * @rx_freeQNum rx pdsp free queue number
 * @rx_freeQMgr rx pdsp free queue manager number
 * @rx_destQNum rx pdsp destination queue number
 * @rx_destQMgr rx pdsp destination queue manager number
 */
struct wifi_proxy_fw_init_info_t {
    Uint32 rx_bMgr;
    Uint32 rx_bPool;
    Uint32 rx_freeQNum;
    Uint32 rx_freeQMgr;
    Uint32 rx_destQNum;
    Uint32 rx_destQMgr;
};

#ifdef CONFIG_WIFI_PROXY_DEBUG
/**
 * wifi_proxy_debug_init_info_t
 * @counter_index PP's debug IP counter index (0-3)
 * @buff_start_addr FW's log messages buffer start address, this
 *                  will be configured to the counter start
 *                  address register
 * @buff_end_addr FW's log messages buffer end address, this
 *                will be configured to the counter roll over
 *                register
 */
struct wifi_proxy_debug_init_info_t {
    Uint32 counter_index;
    Uint32 buff_start_addr;
    Uint32 buff_end_addr;
};
#endif

/**
 * init info for initializing wifi proxy.
 * @buffers_base_addr base address to use for buffer pool.
 * @buffer_sz buffer size of each buffer.
 * @buffer_cnt number of buffers.
 */
struct wifi_proxy_init_info_t {
    Uint32 buffers_base_addr;
    Uint32 buffer_sz;
    Uint32 buffer_cnt;
};

/**
 * wifi_proxy_init_ret_info_t
 * info returned as reply to wifi_proxy_init_info_t
 * @addresses addresses to map by atom
 * @lengths addresses lengths
 */
struct wifi_proxy_init_ret_info_t {
    Uint32 addresses[2];
    Uint32 lengths[2];
};

/**
 * wifi device indexes enumeration
 */
typedef enum {
    WIFI_DEV_MODE_PARTIAL,
    WIFI_DEV_MODE_FULL,
    WIFI_DEV_MODES_CNT
} wifi_dev_mode_e;
#define WIFI_DEV_MODE_VALID(mode) \
    ((WIFI_DEV_MODE_PARTIAL <= (mode)) && ((mode) < WIFI_DEV_MODES_CNT))
#define WIFI_DEV_MOD_STR(mode)    \
    ((mode) == WIFI_DEV_MODE_PARTIAL ? "WIFI Proxy Partial Mode" : \
    ((mode) == WIFI_DEV_MODE_FULL    ? "WIFI Proxy Full Mode"    : \
    "WiFi Proxy invalid mode"))

/**
 * wifi_proxy_add_ch_info_t
 * @dev_id device id
 * @tx_ch tx channel configuration, fw to wifi soc
 * @rx_ch rx channel configuration, wifi soc to fw
 * @f_mode specify if the device is working in full offload mode
 * @author iemergi (2/2/2016)
 */
struct wifi_proxy_add_ch_info_t {
    wifi_dev_id_e               dev_id;
    struct wifi_proxy_tx_ch_t   tx_ch;
    struct wifi_proxy_rx_ch_t   rx_ch;
    wifi_dev_mode_e             mode;
    __u8                        headRoom;
    __u16                       payload;   
    __u8                        seq_num;
};

/**
 * wifi proxy add channel return structure
 * @fw_to_dev_req_tail_addr fw to dev request ring tail pointer
 *                   address in PDSP's local memory (DMEM)
 * @fw_to_dev_cmp_head_addr fw to dev complete ring head pointer
 *                   address in PDSP's local memory (DMEM)
 * @dev_to_fw_cmp_tail_addr dev to fw complete ring tail pointer
 *                   address in PDSP's local memory (DMEM)
 * @dev_to_fw_req_head_addr dev to fw request ring head pointer
 *                   address in PDSP's local memory (DMEM)
 */
struct wifi_proxy_add_ch_ret_info_t {
    Uint32 fw_to_dev_req_tail_addr;
    Uint32 fw_to_dev_cmp_head_addr;
    Uint32 dev_to_fw_cmp_tail_addr;
    Uint32 dev_to_fw_req_head_addr;
};

/**
 * wifi device statistics structure.
 * @tx_req_sent_pkt_num number of packets sent on tx request
 *                      ring
 * @tx_cmp_recv_pkt_num number of packets receive on tx complete
 *                      ring
 * @rx_cmp_sent_pkt_num number of packets sent on rx complete
 *                      ring
 * @rx_req_recv_pkt_num number of packets received on rx request
 *                      ring
 */
struct pp_hal_wifi_device_stats{
    wifi_dev_state_e state;
    Uint32 tx_req_sent_pkt_num;
    Uint32 tx_cmp_recv_pkt_num;
    Uint32 rx_cmp_sent_pkt_num;
    Uint32 rx_req_recv_pkt_num;
};

#define WIFI_TX_PDSP_DMEM_DEVICE_CTRL_OFFSET        0x100
#define WIFI_TX_PDSP_DMEM_DEVICE_MARKS_DB           0x120
#define WIFI_TX_PDSP_DMEM_CTRL_DESC_PER_DEVS        0x130
#define WIFI_TX_PDSP_DMEM_DEVICE_DB_OFFSET          0x200
#define WIFI_TX_PDSP_DMEM_BUFFER_POOLS_DB_OFFSET        0xC00
#define WIFI_TX_PDSP_BUFFER_POOLS_DB_MAX_NUM_ENTRIES    PAL_CPPI41_BMGR_MAX_POOLS
#define WIFI_RX_PDSP_DMEM_DEVICE_CTRL_OFFSET        0x100
#define WIFI_RX_PDSP_DMEM_DEVICE_DB_OFFSET              0x200

/**
 * proxy tx pdsp buffer pool info entry layout
 *
 * @buffStart specify the bottom address for buffer in BP
 * @buffEnd specify the top address for buffer in BP
 * @bpqAddr address for buffer pool corresponding queue (BPQ)
 * @poolNum buffer pool number
 * @poolOffset buffer pool offset in buffer manager
 *
 * @note the struct contains reserved fields in order to be
 * aligned to the actual memory layout
 */
struct __attribute__((packed)) fw_tx_pdsp_bp_info_entry {
    __be32    buffStart;
    __be32    buffEnd;
    __be32    bpqAddr;
    __u8      poolNum;
    __u8      poolOffset;
    __be16    reserved;
};

/**
 * proxy tx pdsp devices control register layout
 *
 * @suspend specify if the device is in suspend state
 * @valid specify if the device's entry is valid
 * @stat_counters_off offset to device's statistical counters
 * @free_buffers number of buffers the device can use
 * @max_buffers maximum number of buffers the device can use
 *
 * @note the struct contains reserve fields in order to be align to
 * the actual memory layout
 */
struct __attribute__((packed)) fw_tx_pdsp_devs_ctrl_reg {
#if defined(__BIG_ENDIAN_BITFIELD)
    __u8    reserve:1,
            pdsp_res:2,
            mode:1,
            cmp_irq:1,
            req_irq:1,
            suspend:1,
            valid:1;
#elif defined (__LITTLE_ENDIAN_BITFIELD)
    __u8    valid:1,
            suspend:1,
            req_irq:1,
            cmp_irq:1,
            mode:1,
            pdsp_res:2,
            reserve:1;
#endif
    __u8    rsvd1;
    __be16  stat_counters_off;
    __be16  free_buffers;
    __be16  max_buffers ;
};

/**
 * proxy tx pdsp devices database entry structure
 *
 * @req_ring request ring structure
 * @req_ring_head request ring head pointer
 * @req_ring_tail request ring tail pointer
 * @cmp_ring complete request structure
 * @cmp_ring_head complete ring head pointer
 * @cmp_ring_tail complete ring tail pointer
 * @mcdma_page_offset the offset to the page the McDMA is using
 * @mcdma_page_num_entries number of entries ceing copied to
 *                         mcdma_page_offset
 * @mcdma_done_page_offset the offset to the page the McDMA is
 *                         done using
 * @mcdma_done_num_entries number of entries copied to
 *                         mcdma_done_page_offset
 * @pdsp_page_offset offset to next free page the pdsp can use
 * @mcdma_ch_regs_offset the offset to mcdma channel regs that
 *                       the device is using
 * @pagesBase mcdma pages base offset
 *
 */
struct __attribute__((packed)) fw_tx_pdsp_devs_db_entry {
    /* request ring */
    struct send_ring_t req_ring;
    __be32             req_ring_head;
    __be32             req_ring_tail;
    /* request ring control */
    Uint16             input_q_off;
    Uint8              rsvd1;
    Uint8              num_elemments_pushed;
    Uint32             rsvd2;
    /* complete ring */
    struct recv_ring_t cmp_ring;
    __be32             cmp_ring_head;
    __be32             cmp_ring_tail;
    /* complete ring control */
    __be16             mcdma_page_offset;
    __be16             pdsp_page_offset;
    __u8               mcdma_page_num_entries;
    __u8               mcdma_ch_regs_offset;
    __be16             pagesBase;
};

/**
 * proxy rx pdsp devices control register layout
 *
 * @suspend specify if the device is in suspend state
 * @valid specify if the device's entry is valid
 * @pid device's pid
 * @stat_counters_off offset to device's statistical counters
 * @free_buffers number of buffers the device can use
 * @max_buffers maximum number of buffers the device can use
 *
 * @note the struct contains reserve fields in order to be align to
 * the actual memory layout
 */
struct __attribute__((packed)) fw_rx_pdsp_devs_ctrl_reg {
#if defined(__BIG_ENDIAN_BITFIELD)
    __u8    reserve:3,
            mode:1,
            cmp_irq:1,
            req_irq:1,
            suspend:1,
            valid:1;
#elif defined (__LITTLE_ENDIAN_BITFIELD)
    __u8    valid:1,
            suspend:1,
            req_irq:1,
            cmp_irq:1,
            mode:1,
            reserve:3;
#endif
    __u8    pid;
    __be16  stat_counters_off;
    __be16  free_buffers;
    __be16  max_buffers ;
};
/**
 * proxy rx pdsp devices database entry structure
 *
 * @cmp_ring complete ring structure
 * @cmp_ring_head complete ring head pointer
 * @cmp_ring_tail complete ring tail pointer
 * @mark mark field to mark buffers when send on complete ring
 * @req_ring request ring structure
 * @req_ring_head request ring head pointer
 * @req_ring_tail request ring tail pointer
 * @mcdma_page_offset the offset to the page the McDMA is using
 * @mcdma_page_num_entries number of entries ceing copied to
 *                         mcdma_page_offset
 * @mcdma_done_page_offset the offset to the page the McDMA is
 *                         done using
 * @mcdma_done_num_entries number of entries copied to
 *                         mcdma_done_page_offset
 * @mcdma_ch_regs_offset the offset to mcdma channel regs that
 *                       the device is using
 * @pdsp_page_offset offset to next free page the pdsp can use
 * @pagesBase mcdma pages base offset
 * @note the struct contains reserve fields in order to be
 * align to the actual memory layout
 */
struct __attribute__((packed, aligned(64)))fw_rx_pdsp_devs_db_entry {
    /* complete ring */
    struct recv_ring_t cmp_ring;
    __be32             cmp_ring_head;
    __be32             cmp_ring_tail;
    /* complete ring control */
    __be32             mark;
    __be32             rsvd1;
    /* request ring */
    struct send_ring_t req_ring;
    __be32             req_ring_head;
    __be32             req_ring_tail;
    /* request ring control */
    __be16             mcdma_page_offset;
    __be16             next_free_page_offset;
    __u8               mcdma_page_num_entries;
    __u8               mcdma_ch_regs_offset;
    __be16             pagesBase;
};

AVALANCHE_PP_RET_e avalanche_pp_wifi_init(struct wifi_proxy_fw_init_info_t *init_info);
AVALANCHE_PP_RET_e avalanche_pp_wifi_dmem_init(void);
AVALANCHE_PP_RET_e avalanche_pp_wifi_dmem_buffer_pools_init(void);
AVALANCHE_PP_RET_e avalanche_pp_add_wifi_dev_ch(struct wifi_proxy_add_ch_info_t *info,
                                                struct wifi_proxy_add_ch_ret_info_t *ret_vals);
AVALANCHE_PP_RET_e avalanche_pp_rem_wifi_dev_ch(wifi_dev_id_e wifi_dev_id);
AVALANCHE_PP_RET_e avalanche_pp_start_wifi_dev_ch(wifi_dev_id_e wifi_dev_id);
AVALANCHE_PP_RET_e avalanche_pp_stop_wifi_dev_ch(wifi_dev_id_e wifi_dev_id);
AVALANCHE_PP_RET_e avalanche_pp_get_dev_ch_stats(wifi_dev_id_e dev_id,
                                                 struct pp_hal_wifi_device_stats *dev_stats);

#endif

#if 0

/* Utility API. */

extern int avalanche_pp_set_ack_suppression(int enDis);
#endif

#endif

typedef     struct
{
    Uint32  session_record_base_address;
    Uint32  session_timeout_base_address;
    Uint32  hop_list_base_address;
    Uint16  syncQThreshold;
    Uint8   bAccRateLimitActive;
    Uint8   mocaPaddingMode;
    Uint32  tdoxAckSuppressionEn;
    Uint32  bypassFragSupport;
#ifndef CONFIG_ARM_AVALANCHE_SOC
    Uint32  ppIrqNum;
    Uint32  atomIntcStatusSetRegAddr;
#endif
    Uint32  moca_fused_out;
}avalanche_pp_db_address_param_t;

#define BYPASS_FRAGMENTATION_DEFAULT_MODE 0

typedef enum {
    ADD_ADDR = 0,
    RM_ADDR,
    FLUSH_LIST,
    RD_VALID_ENTRIES_COUNT,
    IS_ADDR_EXIST,
    LOCAL_ADDR_OP_MAX,
}avalanche_pp_local_addr_op_type_e;

typedef enum {
    MTA_MAC_ADDR = 0,
    MTA_RTP_IPV4_ADDR,
    MTA_MNG_IPV4_ADDR,
    MTA_RTP_IPV6_ADDR,
    MTA_MNG_IPV6_ADDR,
    WAN_MAC_ADDR,
    GW_MAC_ADDR,
    LAN0_MAC_ADDR,
    RND_MAC_ADDR,
    MULTI_DROP_IPV4_ADDR,
    MULTI_DROP_IPV6_ADDR,
    LOCAL_ADDR_TYPE_COUNT
}avalanche_pp_local_addr_type_e;

typedef struct
{
    avalanche_pp_local_addr_op_type_e op_type;
    avalanche_pp_local_addr_type_e addr_type;
    union
    {
        Uint8  mac_addr[6];
        Uint32 ipv4;
        Uint32 ipv6[4];
        Uint8  all;
    }u;
    Uint8 valid_entries_cnt;
}avalanche_pp_local_dev_addr_ioctl_params_t;

/***************** Sanity manager api *******************/
typedef struct
{
    Uint8* pktDataIngressDevName;
    Uint8* pktDataEgressDevName;
    Uint8* pktDataIngressP;
    Uint8* pktDataEgressP;
    Uint32 pktDataIngressSize;
    Uint32 pktDataEgressSize;
    Uint32 sessionHandleRet;
}
sanity_session_info_t;

#ifdef CONFIG_PP_SANITY_TESTS
#define SANITY_PROC_NAME "sanity"
#endif

/************** Power management info ****************/

#define MAX_MONITORED_OBJECTS_NUM 16

typedef struct
{
    Uint32* currResourcesNumRegAddr;
    Uint32 lpmThreshold;

}AVALANCHE_PP_OBJECT_INFO_t;

typedef struct
{
    AVALANCHE_PP_OBJECT_INFO_t PMobjectsInfos[MAX_MONITORED_OBJECTS_NUM];
    Uint32 ppsStatCntPhyAddr;
    Uint32 validEntriesCount;

}AVALANCHE_PP_PM_INFO_t;

/********************************************************************************************************/
/* IOCTL commands:

   If you are adding new ioctl's to the kernel, you should use the _IO
   macros defined in <linux/ioctl.h> _IO macros are used to create ioctl numbers:

    _IO(type, nr)         - an ioctl with no parameter.
   _IOW(type, nr, size)  - an ioctl with write parameters (copy_from_user), kernel would actually read data from user space
   _IOR(type, nr, size)  - an ioctl with read parameters (copy_to_user), kernel would actually write data to user space
   _IOWR(type, nr, size) - an ioctl with both write and read parameters

   'Write' and 'read' are from the user's point of view, just like the
    system calls 'write' and 'read'.  For example, a SET_FOO ioctl would
    be _IOW, although the kernel would actually read data from user space;
    a GET_FOO ioctl would be _IOR, although the kernel would actually write
    data to user space.

    The first argument to _IO, _IOW, _IOR, or _IOWR is an identifying letter
    or number from the SoC_ModuleIds_e enum located in this file.

    The second argument to _IO, _IOW, _IOR, or _IOWR is a sequence number
    to distinguish ioctls from each other.

   The third argument to _IOW, _IOR, or _IOWR is the type of the data going
   into the kernel or coming out of the kernel (e.g.  'int' or 'struct foo').

   NOTE!  Do NOT use sizeof(arg) as the third argument as this results in
   your ioctl thinking it passes an argument of type size_t.

*/
#define PP_DRIVER_MODULE_ID                    (0xDF)

#define PP_DRIVER_FLUSH_ALL_SESSIONS                    _IO   (PP_DRIVER_MODULE_ID, 1)
#define PP_DRIVER_PSM                                   _IOWR (PP_DRIVER_MODULE_ID, 2, avalanche_pp_psm_ioctl_param_t)
#define PP_DRIVER_DELETE_VPID                           _IOWR (PP_DRIVER_MODULE_ID, 3, avalanche_pp_dev_ioctl_param_t)
#define PP_DRIVER_ADD_VPID                              _IOWR (PP_DRIVER_MODULE_ID, 4, avalanche_pp_dev_ioctl_param_t)
#define PP_DRIVER_SET_FRAG_MODE                         _IOWR (PP_DRIVER_MODULE_ID, 5, avalanche_pp_frag_mode_ioctl_param_t)
#define PP_DRIVER_SET_LOCAL_DEV_ADDR                    _IOWR (PP_DRIVER_MODULE_ID, 6, avalanche_pp_local_dev_addr_ioctl_params_t)
#define PP_DRIVER_KERNEL_POST_INIT                      _IO   (PP_DRIVER_MODULE_ID, 7)
#define PP_DRIVER_SET_ACK_SUPP                          _IOWR (PP_DRIVER_MODULE_ID, 8, avalanche_pp_ackSupp_ioctl_param_t)
#define PP_DRIVER_GET_DB_PARAMS                         _IOWR (PP_DRIVER_MODULE_ID, 9, avalanche_pp_db_address_param_t)
#define PP_DRIVER_GET_SESSION_STATS                     _IOWR (PP_DRIVER_MODULE_ID, 10, AVALANCHE_PP_EXTENDED_SESSION_STATS_t)
#define PP_DRIVER_SHOW_LOCAL_DEV_ADDR                   _IOWR (PP_DRIVER_MODULE_ID, 11, avalanche_pp_local_dev_addr_ioctl_params_t)
#define PP_DRIVER_SET_DEFENSIVE_MODE                    _IOWR (PP_DRIVER_MODULE_ID, 12, avalanche_pp_defensive_mode_ioctl_param_t)
#define PP_DRIVER_SET_DEFENSIVE_STATE                   _IOWR (PP_DRIVER_MODULE_ID, 13, avalanche_pp_defensive_state_ioctl_param_t)
#define PP_DRIVER_CHANGE_L2CLASSIFICATION_DEFAULT_MODE  _IOWR (PP_DRIVER_MODULE_ID, 14, avalanche_pp_l2classification_default_mode_ioctl_param_t)
#define PP_DRIVER_SET_DDH_NOTIFICATION_TIMEOUT          _IOWR (PP_DRIVER_MODULE_ID, 15, avalanche_pp_defensive_state_notification_timeout_ioctl_param_t)
#define PP_DRIVER_PRINT_BITHASH                         _IO   (PP_DRIVER_MODULE_ID, 16)

#if PUMA7_OR_NEWER_SOC_TYPE
#define PP_DRIVER_GET_MAX_SESSIONS                      _IOWR (PP_DRIVER_MODULE_ID, 17, Uint32)
#endif

#define PP_DRIVER_SET_MSS_CLAMPING                      _IOWR (PP_DRIVER_MODULE_ID, 18, Uint16)
/* CNI IOCTL event types */
typedef enum
{
    CNI_IOCTL_SET_GBE_DBG_PRINT,
    CNI_IOCTL_SET_CLUSTER_CFG
}CNI_IOCTL_CMDS;

#endif //   _AVALANCHE_PP_H
