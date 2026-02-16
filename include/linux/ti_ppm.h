/*
 * ti_ppm.h - Header file for the Packet Processor.
 *
 * Description:
 *  This file contains data structures and definitions that are available
 *  to the users of the PPM Interface to configure and program the packet
 *  processor.
 *
 * Copyright (C) <2008>, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */
#ifndef __TI_PPM_H__
#define __TI_PPM_H__


#define MAX_ALLOWED_QOS_CLUSTERS_PER_DEVICE     16

/* OS Specific Section Begin */

/**************************************************************************
 * STRUCTURE NAME : PPM_OS_CONTEXT
 **************************************************************************
 * DESCRIPTION   :
 *  This structure defines information that is required to provide critical
 *  access protection. Information stored in this structure is specific to
 *  the OS.
 **************************************************************************/
typedef struct PPM_OS_CONTEXT
{
    int     flags;
}PPM_OS_CONTEXT;

/* OS Specific Section End */

/************** PLEASE DO NOT MODIFY BEYOND THIS LINE ******************/

/**************************************************************************
 ****************************** Limit Definitions *************************
 **************************************************************************/

/* These are the maximum number of PID,VPID & Sessions that are supported.*/
#define TI_PP_MAX_PID                          32
#define TI_PP_MAX_VPID                         32
#ifdef CONFIG_INTEL_PP_TUNNEL_SUPPORT
#define TI_PP_MAX_ACCLERABLE_SESSIONS           2
#define TI_PP_MAX_ACCLERABLE_VOICE_SESSIONS    0
#else
#define TI_PP_MAX_ACCLERABLE_SESSIONS          256
#define TI_PP_MAX_ACCLERABLE_VOICE_SESSIONS    8
#endif
#define TI_PP_MAX_ACCLERABLE_NONVOICE_SESSIONS ((TI_PP_MAX_ACCLERABLE_SESSIONS) - (TI_PP_MAX_ACCLERABLE_VOICE_SESSIONS))



/* Special Case Defintion used in the PPM Create Session to indicate that
 * the Ingress VPID is a DONT CARE! */
#define TI_PP_IGNORE_INGRESS_VPID              0xFF

/* Maximum Egress Properties that can be defined in a session.
 * - In theory this is TI_PP_MAX_VPID - 1 because in the worst
 *   case a packet can be flooded onto all interfaces except the
 *   interface on which the packet was received. But this is real
 *   world and the System Integrator should tune this appropriately
 *   for their system. */
#define TI_PP_MAX_EGRESS_PROPERTY               5

/**************************************************************************
 ****************************** Event Identifiers *************************
 **************************************************************************/

/* The Sub-system identifier. */
#define TI_PP_SUBSYSTEM_BIT_MASK               0xE00000

/* Module Defintions and bit mask. */
#define TI_PP_MODULE_BIT_MASK                  0x1F0000
#define TI_PP_MODULE                           0x00000
#define TI_PPD_MODULE                          0x10000
#define TI_PPM_MODULE                          0x20000

/* Event Bit Mask. */
#define TI_PP_EVENT_BIT_MASK                   0xFFFF

/***************************** PPM Events. *********************************/

/* Fatal Errors: */
#define TI_PPM_OUT_OF_MEMORY                   (TI_PPM_MODULE + 0x1)
#define TI_PPM_INTERNAL_ERROR                  (TI_PPM_MODULE + 0x2)

/* PID/VPID Related Events. */
#define TI_PPM_CREATE_PID_FAILED               (TI_PPM_MODULE + 0x5)
#define TI_PPM_PID_CREATED                     (TI_PPM_MODULE + 0x6)
#define TI_PPM_DELETE_PID_FAILED               (TI_PPM_MODULE + 0x7)
#define TI_PPM_PID_DELETED                     (TI_PPM_MODULE + 0x8)
#define TI_PPM_CREATE_VPID_FAILED              (TI_PPM_MODULE + 0x9)
#define TI_PPM_VPID_CREATED                    (TI_PPM_MODULE + 0xa)
#define TI_PPM_DELETE_VPID_FAILED              (TI_PPM_MODULE + 0xb)
#define TI_PPM_VPID_DELETED                    (TI_PPM_MODULE + 0xc)

/* Session Related Events. */
#define TI_PPM_CREATE_SESSION_FAILED           (TI_PPM_MODULE + 0xd)
#define TI_PPM_SESSION_CREATED                 (TI_PPM_MODULE + 0xe)
#define TI_PPM_DELETE_SESSION_FAILED           (TI_PPM_MODULE + 0xf)
#define TI_PPM_SESSION_DELETED                 (TI_PPM_MODULE + 0x10)
#define TI_PPM_MODIFY_SESSION_FAILED           (TI_PPM_MODULE + 0x11)
#define TI_PPM_SESSION_MODIFIED                (TI_PPM_MODULE + 0x12)

/***************************** PPD Events. *********************************/

#define TI_PPD_TDOX_EVALUATION                 (TI_PPD_MODULE + 0x1)

/***************************** PP Events.  *********************************/

#define TI_PP_SESSION_EXPIRATION               (TI_PP_MODULE  + 0x1)

/**************************************************************************
 ****************************** PPM OS Abstraction ************************
 **************************************************************************/

/* These define the PPM OS Abstraction Layer */
typedef void* (*OS_MEMCPY)(void* dst, void* src, int num_bytes);
typedef int   (*OS_MEMCMP)(void* dst, void* src, int num_bytes);
typedef void* (*OS_MEMSET)(void* dst, int c, int num_bytes);
typedef void* (*OS_MALLOC)(int num_bytes);
typedef void  (*OS_FREE)(void* ptr_memory);
typedef void  (*OS_CRITICAL_SECTION_START)(PPM_OS_CONTEXT* ctx);
typedef void  (*OS_CRITICAL_SECTION_END)  (PPM_OS_CONTEXT* ctx);
typedef int   (*OS_INIT)(void);
typedef int   (*OS_DEINIT)(void);

/**************************************************************************
 * STRUCTURE NAME : TI_PPM_OS_FUNC_TABLE
 **************************************************************************
 * DESCRIPTION   :
 *  The function is used to provide the OS Abstraction API which are used
 *  by the PPM.
 **************************************************************************/
typedef struct TI_PPM_OS_FUNC_TABLE
{
    OS_MEMCPY                   memcpy;
    OS_MEMCMP                   memcmp;
    OS_MEMSET                   memset;
    OS_MALLOC                   malloc;
    OS_FREE                     free;
    OS_CRITICAL_SECTION_START   critical_section_start;
    OS_CRITICAL_SECTION_END     critical_section_end;
}TI_PPM_OS_FUNC_TABLE;

/**************************************************************************
 ************************** PSP:PPD Specific configuration ****************
 **************************************************************************/

#define TI_PP_BOOTS_FW    0
#define TI_PP_CPDSP_FW    1
#define TI_PP_MPDSP_FW    2
#define TI_PP_QPDSP_FW    3
#define TI_PP_PrxPDSP_FW  4
#define TI_PP_CPDSP2_FW   5

/**************************************************************************
 * STRUCTURE NAME : TI_PP_FIRMWARE
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the Firmware.
 **************************************************************************/
typedef struct TI_PP_FIRMWARE
{
    unsigned int  id;
    unsigned int* data;
    unsigned int  size;
} TI_PP_FIRMWARE;

/**************************************************************************
 * STRUCTURE NAME : TI_PPD_CONFIG
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the PPD Configuration.
 **************************************************************************/
typedef struct TI_PPD_CONFIG
{
    unsigned short dflt_host_rx_q;      /* Fwd pkts to Q when PID in pkt do not match */
    unsigned short dflt_host_rx_dst_tag;/* Dst tag for the host RX queue */
    unsigned short host_ev_queue;       /* Q for PDSP(s) to post events */
    unsigned short host_q_mgr;          /* Mgr for the event queue */
    unsigned int   sync_max_pkt;        /* Max number of pkts that can be stored on session syncrnzn q */
    unsigned int   sync_timeout_10us;   /* Max time to wait for session syncrnzn in 10pow(-5) sec */
    unsigned int   buff_pool_indx;      /* Pool for PP internal 128 byte buffers */
} TI_PPD_CONFIG;

/**************************************************************************
 ****************************** PID Flag Definitions **********************
 **************************************************************************/

#define TI_PP_PID_VLAN_PRIO_MAP            0x1
#define TI_PP_PID_DIFFSRV_PRIO_MAP         0x2
#define TI_PP_PID_PRIO_OFF_TX_DST_TAG      0x4
#define TI_PP_PID_CLASSIFY_BYPASS          0x8
#define TI_PP_PID_DISCARD_ALL_RX           0x40

/**************************************************************************
 ****************************** PID Type Definitions **********************
 **************************************************************************/

#define TI_PP_PID_TYPE_UNDEFINED            0x0
#define TI_PP_PID_TYPE_ETHERNET             0x1
#define TI_PP_PID_TYPE_INFRASTRUCTURE       0x2
#define TI_PP_PID_TYPE_USBBULK              0x3
#define TI_PP_PID_TYPE_CDC                  0x4
#define TI_PP_PID_TYPE_DOCSIS               0x5
#define TI_PP_PID_TYPE_ETHERNETSWITCH       0x6

/**************************************************************************
 ****************************** PID Ingress Framing ***********************
 **************************************************************************/

#define TI_PP_PID_INGRESS_ETHERNET          0x1
#define TI_PP_PID_INGRESS_IPV4              0x2
#define TI_PP_PID_INGRESS_IPV6              0x4
#define TI_PP_PID_INGRESS_IPOE              0x8
#define TI_PP_PID_INGRESS_IPOA              0x10
#define TI_PP_PID_INGRESS_PPPOE             0x20
#define TI_PP_PID_INGRESS_PPPOA             0x40

/**************************************************************************
 ****************************** PID Private Type **************************
 **************************************************************************/

#define TI_PP_PID_HW_DATA_HEAD                (1 << 1)
#define TI_PP_PID_HW_DATA_TAIL                (1 << 2)

/**************************************************************************
 ****************************** PID Generic Definitions *******************
 **************************************************************************/
#define TI_PP_PID_NO_DST_TAG                0x3FFF

/**************************************************************************
 * STRUCTURE NAME : TI_PP_PID
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the PID. The information in the PID structure
 *  is very hardware specific and is typically filled up by the drivers
 **************************************************************************/
typedef struct TI_PP_PID
{
    unsigned char   pid_handle;
    unsigned char   priv_flags;
    unsigned char   type;
    unsigned char   ingress_framing;
    unsigned char   dflt_pri_drp;
    unsigned char   pri_mapping;
    unsigned short  dflt_fwd_q;
    unsigned short  dflt_dst_tag;
    unsigned short  tx_pri_q_map[8];
    unsigned char   tx_hw_data_len;
    unsigned char   tx_hw_data[64];
}TI_PP_PID;

/**************************************************************************
 * STRUCTURE NAME : TI_PP_PID_RANGE
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the PID range.
 **************************************************************************/
typedef struct TI_PP_PID_RANGE
{
    unsigned char   port_num;
    unsigned char   type;
    unsigned char   count;
    unsigned char   base_index;
}TI_PP_PID_RANGE;

/**************************************************************************
 ****************************** VPID Private Type *************************
 **************************************************************************/

#define TI_PP_VPID_PRIV_DATA_HEAD             (1 << 1)
#define TI_PP_VPID_PRIV_DATA_TAIL             (1 << 2)

/**************************************************************************
 * STRUCTURE NAME : TI_PP_VPID_TYPE
 **************************************************************************
 * DESCRIPTION   :
 *  The enumeration defines the type of network to which a VPID is
 *  connected.
 **************************************************************************/
typedef enum TI_PP_VPID_TYPE
{
    TI_PP_ETHERNET   = 0x0,
    TI_PP_VLAN       = 0x1
}TI_PP_VPID_TYPE;

/**************************************************************************
 ****************************** VPID Flag Definitions *********************
 **************************************************************************/
#define TI_PP_VPID_FLG_RX_DFLT_FWD          (1 << 3)
#define TI_PP_VPID_FLG_TX_DISBL             (1 << 5)
#define TI_PP_VPID_FLG_RX_DISBL             (1 << 6)
#define TI_PP_VPID_FLG_VALID                (1 << 7)

/**************************************************************************
 * STRUCTURE NAME : TI_PP_VPID
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the VPID entity. All packets which enter and
 *  exit the PP do so via a VPID Entity. Thus VPID's can be considered as
 *  end points for all the networking traffic through the box. They map
 *  to the OS networking interface objects which are the end points
 *  connecting the driver to the OS networking stack.
 **************************************************************************/
typedef struct TI_PP_VPID
{
    /* This is the VPID Handle. Users dont need to populate this field since
     * this handle will be filled up the PPM on the successful creation
     * of the VPID. */
    unsigned char   vpid_handle;

    /* This is the parent PID handle. All VPID are related to the PID
     * Users need to specify the Parent PID handle in this field. */
    unsigned char   parent_pid_handle;

    /* This describes the VPID Type. VPID are networking endpoints and
     * which describe the type of network to which they are connected. */
    TI_PP_VPID_TYPE  type;

    /* These are the QoS related settings */
    unsigned char                   qos_clusters_count;
    struct TI_PP_QOS_CLST_CFG *     qos_cluster[MAX_ALLOWED_QOS_CLUSTERS_PER_DEVICE];

    /* This is the Egress MTU associated with the VPID. This is required
     * since all packets transmitted via this interface should adhere to
     * the MTU specified here. */
    unsigned int    egress_mtu;

    /* This is an optional VLAN Identifier associated with the VPID. This
     * is required only if the VPID is attached to a VLAN enabled network
     * Thus this field will be used only if the "type" is either TI_PP_VLAN
     * or TI_PP_VLAN_PPPoE */
    unsigned short  vlan_identifier;

    /* This is an optional PPP Session Identifier associated with the VPID
     * This is required if the interface is a PPP Endpoint. This fields is
     *
     * checked only if the "type" field is TI_PP_PPPoE or TI_PP_VLAN_PPPoE */
    unsigned short  ppp_session_id;

    /* Private Data. */
    unsigned char   priv_vpid_flags;
    unsigned char   priv_unused[2];
    unsigned char   priv_tx_data_len;
    unsigned char   priv_tx_data[16];
    unsigned char   priv_rx_tag;
}TI_PP_VPID;

/**************************************************************************
 * STRUCTURE NAME : TI_PP_VPID_STATS
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the VPID statistics for the Packet Processor.
 **************************************************************************/
typedef struct TI_PP_VPID_STATS
{
    unsigned int rx_unicast_pkt;
    unsigned int rx_broadcast_pkt;
    unsigned int rx_multicast_pkt;
    unsigned int rx_byte_hi;
    unsigned int rx_byte_lo;
    unsigned int rx_discard;
    unsigned int tx_unicast_pkt;
    unsigned int tx_broadcast_pkt;
    unsigned int tx_multicast_pkt;
    unsigned int tx_byte_hi;
    unsigned int tx_byte_lo;
    unsigned int tx_error;
    unsigned int tx_discard;
} TI_PP_VPID_STATS;

/**************************************************************************
 ************** Application Specific Bit Enable fields ********************
 **************************************************************************/

/* These bit enables are used to identify which of the Application specific
 * fields are specified and which fields are wildcarded. */
#define TI_PP_SESSION_APP_RAW_INFO1_VALID       0x1
#define TI_PP_SESSION_APP_RAW_INFO2_VALID       0x2
#define TI_PP_SESSION_APP_RAW_INFO3_VALID       0x4

#define TI_PP_SESSION_APP_RAW_INFO1_B0_VALID    0x10
#define TI_PP_SESSION_APP_RAW_INFO1_B1_VALID    0x20
#define TI_PP_SESSION_APP_RAW_INFO1_B2_VALID    0x40
#define TI_PP_SESSION_APP_RAW_INFO1_B3_VALID    0x80

#define TI_PP_SESSION_APP_RAW_INFO1_B3_PLUS_VALID   0x100
#define TI_PP_SESSION_APP_HIL_USED01                0x80000000

/**************************************************************************
 * STRUCTURE NAME : TI_PP_APP_DESC
 **************************************************************************
 * DESCRIPTION   :
 *  The structure defines the Application specific Fields inside the packet.
 **************************************************************************/
typedef struct TI_PP_APP_DESC
{
    union
    {
        unsigned int        raw_app_info1;
        struct
        {
            unsigned char   raw_app_info1_b0;
            unsigned char   raw_app_info1_b1;
            unsigned char   raw_app_info1_b2;
            unsigned char   raw_app_info1_b3;
        };

    }u;

    unsigned int            raw_app_info2;
    unsigned int            raw_app_info3;

    /* NOTE: It is always not possible to specify all the fields. In case
     * certain fields need to be wild carded; ensure that the corresponding
     * bit is not set. */
    unsigned int    enables;
}TI_PP_APP_DESC;

/**************************************************************************
 * STRUCTURE NAME : TI_PP_ETH_DESC
 **************************************************************************
 * DESCRIPTION   :
 *  The structure defines the Ethernet Fields inside the packet.
 **************************************************************************/
#define TI_PP_SESSION_L2_RAW_VALID              0x1

typedef struct TI_PP_L2_RAW_DESC
{
    /* These fields describe the layer2 Ethernet header. */
#ifdef CONFIG_INTEL_PP_TUNNEL_SUPPORT
    unsigned char   tx_buff[128];
#else
    unsigned char   tx_buff[24];
#endif
    unsigned char   tx_buff_len;

    /* NOTE: It is always not possible to specify all the fields. In case
     * certain fields need to be wild carded; ensure that the corresponding
     * bit is not set. */
    unsigned int    enables;
}TI_PP_L2_RAW_DESC;


/**************************************************************************
 **************************** Layer2 Bit Enable fields ********************
 **************************************************************************/

/* These bit enables are used to identify which of the layer2 specific
 * fields are specified and which fields are wildcarded. */
#define TI_PP_SESSION_L2_DSTMAC_VALID           0x1
#define TI_PP_SESSION_L2_SRCMAC_VALID           0x2
#define TI_PP_SESSION_L2_VLAN_VALID             0x4
#define TI_PP_SESSION_L2_GRE_US_VALID           0x8
#define TI_PP_SESSION_L2_GRE_DS_VALID           0x10

/**************************************************************************
 * STRUCTURE NAME : TI_PP_ETH_DESC
 **************************************************************************
 * DESCRIPTION   :
 *  The structure defines the Ethernet Fields inside the packet.
 **************************************************************************/
typedef struct TI_PP_ETH_DESC
{
    /* These fields describe the layer2 Ethernet header. */
    unsigned char   dstmac[6];
    unsigned char   srcmac[6];
    unsigned short  vlan_tag;
#define TI_PP_SESSION_PPPOE_INVALID 0xFFFF
    unsigned short	pppoe_sid;

    /* NOTE: It is always not possible to specify all the fields. In case
     * certain fields need to be wildcarded; ensure that the corresponding
     * bit is not set. */
    unsigned int    enables;
}TI_PP_ETH_DESC;

/**************************************************************************
 **************************** IPv4 Bit Enable fields ********************
 **************************************************************************/

/* These bit enables are used to identify which of the IPv4 specific
 * fields are specified and which fields are wildcarded. */
#define TI_PP_SESSION_IPV4_DSTIP_VALID          0x1
#define TI_PP_SESSION_IPV4_SRCIP_VALID          0x2
#define TI_PP_SESSION_IPV4_PROTOCOL_VALID       0x4
#define TI_PP_SESSION_IPV4_TOS_VALID            0x8
#define TI_PP_SESSION_IPV4_DST_PORT_VALID       0x10
#define TI_PP_SESSION_IPV4_SRC_PORT_VALID       0x20

/**************************************************************************
 * STRUCTURE NAME : TI_PP_IPV4_DESC
 **************************************************************************
 * DESCRIPTION   :
 *  The structure defines the IPv4 Fields inside the packet.
 **************************************************************************/
#define TI_PP_IPV4_HEADER_TOTAL_LENGTH_OFFSET       2  /* Defines the offset in bytes from start of IPv4 header for the "Total Length" field */
#define TI_PP_IPV4_HEADER_RAW_SIZE_MAX              64 /* Defines the maximum size we copy from the IPv4 header */

typedef struct TI_PP_IPV4_DESC
{
    /* The fields here describe the layer3 IPv4 header. */
    unsigned int    dst_ip;
    unsigned int    src_ip;
    unsigned char   protocol;
    unsigned char   tos;

    /* These fields describe the layer4 header. */
    unsigned short  dst_port;
    unsigned short  src_port;

    /* NOTE: It is always not possible to specify all the fields. In case
     * certain fields need to be wildcarded; ensure that the corresponding
     * bit is not set. */
    unsigned int    enables;

    /* ipv4HdrRaw parameters are used for US GRE to save the raw IPv4+GRE header that should be prepended to the packet */
    unsigned char   ipv4HdrRaw[TI_PP_IPV4_HEADER_RAW_SIZE_MAX];
    unsigned char   ipv4HdrRawLen;
    unsigned char   ipv4HdrRawOffset;
}TI_PP_IPV4_DESC;

/**************************************************************************
 **************************** IPv6 Bit Enable fields ********************
 **************************************************************************/

/* These bit enables are used to identify which of the IPv6 specific
 * fields are specified and which fields are wildcarded. */
#define TI_PP_SESSION_IPV6_DSTIP_VALID          0x1
#define TI_PP_SESSION_IPV6_SRCIP_VALID          0x2
#define TI_PP_SESSION_IPV6_NEXTHDR_VALID        0x4
#define TI_PP_SESSION_IPV6_TRCLASS_VALID        0x8
#define TI_PP_SESSION_IPV6_FLOWLBL_VALID        0x10
#define TI_PP_SESSION_IPV6_DST_PORT_VALID       0x20
#define TI_PP_SESSION_IPV6_SRC_PORT_VALID       0x40
#define TI_PP_SESSION_IPV6_DSLITE_DSTIP_VALID   0x80

/**************************************************************************
 * STRUCTURE NAME : TI_PP_IPV6_DESC
 **************************************************************************
 * DESCRIPTION   :
 *  The structure defines the IPv6 Fields inside the packet.
 **************************************************************************/
 #define TI_PP_IPV6_HEADER_PAYLOAD_LENGTH_OFFSET    4  // Defines the offset in bytes from start of IPv6 header for the "Payload Length" field
 #define TI_PP_IPV6_HEADER_RAW_SIZE_MAX             64 // Defines the maximum size we copy from the IPv6 header
typedef struct TI_PP_IPV6_DESC
{
    /* These fields describe the layer3 IPv6 header. */
    unsigned int    dst_ip[4];
    unsigned int    src_ip[4];

    unsigned char   next_header;

    /* Used for prioritization/sepcial handling of packets */
    unsigned char   traffic_class;
    unsigned int    flow_label;

    /* These fields describe the layer4 header */
    unsigned short  dst_port;
    unsigned short  src_port;

    /* NOTE: It is always not possible to specify all the fields. In case
     * certain fields need to be wildcarded; ensure that the corresponding
     * bit is not set. */
    unsigned int    enables;

    unsigned int    dsLite_dst_ip;

    unsigned char   ipv6HdrTotalSize;
    unsigned char   ipv6HdrRaw[TI_PP_IPV6_HEADER_RAW_SIZE_MAX];
    unsigned char   ipv6HdrRawOffset;

}TI_PP_IPV6_DESC;


/**************************************************************************
 ******************************* Layer3 Type ******************************
 **************************************************************************/

/**************************************************************************
 * STRUCTURE NAME : TI_PP_PACKET_TYPE
 **************************************************************************
 * DESCRIPTION   :
 *  The enumeration defines the type of packet.
 **************************************************************************/
typedef enum TI_PP_PACKET_TYPE
{
    TI_PP_NO_TYPE       = 0x0,
    TI_PP_ETH_TYPE      = 0x1,
    TI_PP_IPV4_TYPE     = 0x2,
    TI_PP_IPV6_TYPE     = 0x4,
    TI_PP_APP_SPEC_TYPE = 0x8,
    TI_PP_L2_RAW_TYPE   = 0x10,
}TI_PP_PACKET_TYPE;

/**************************************************************************
 * STRUCTURE NAME : TI_PP_PACKET_DESC
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the various fields inside a packet. Each packet
 *  will have a layer2 description and either an IPv4 or IPv6 description
 **************************************************************************/
typedef struct TI_PP_PACKET_DESC
{
    /* Describe the type of packet*/
    TI_PP_PACKET_TYPE       packet_type;
    union
    {
       TI_PP_ETH_DESC       eth_desc;
       TI_PP_L2_RAW_DESC    l2raw_desc;
       TI_PP_IPV4_DESC      ipv4_desc;
       TI_PP_IPV6_DESC      ipv6_desc;
       TI_PP_APP_DESC       app_desc;
    } u;
} TI_PP_PACKET_DESC;

/**************************************************************************
 * STRUCTURE NAME : TI_PP_SESSION_PROPERTY
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the session properties. Fundamentally a session
 *  consists of 2 properties i.e. Ingress and Egress. Each prorty internally
 *  describes the interface on which the packet was rxed (Ingress) or txed
 *  (Egress) and how the packet looked at Ingress or Egress.
 **************************************************************************/
typedef struct TI_PP_SESSION_PROPERTY
{
    /* The fields here describe the VPID handle on which the packet was either
     * received or on which the packet will be transmitted. */
    unsigned char       vpid_handle;

    /************************************************************/
    /* IMPORTANT (!)                                            */
    /* The following two fields (L2 and L3) MUST be adjacent    */
    /* due to packets classification requirements               */
    /************************************************************/
    /* This field describes the Layer2 header */
    TI_PP_PACKET_DESC   l2_packet;

    /* This field describes the Layer3 and Layer4 headers. */
    TI_PP_PACKET_DESC   l3l4_packet;
    TI_PP_PACKET_DESC   app_specific_data;
    TI_PP_PACKET_DESC   l2_raw_packet;
}TI_PP_SESSION_PROPERTY;

/**************************************************************************
 * STRUCTURE NAME : TI_PP_SESSION
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the Session
 **************************************************************************/
typedef struct TI_PP_SESSION
{
    /* Session Timeout indicates the number of micro-seconds of inactivity
     * after which the PP generates an event to the host. The field if set
     * to 0 indicates that the session needs to be configured permanently
     * and is not subject to IDLE based timeouts. */
    unsigned int            session_timeout;

#ifdef CONFIG_INTEL_PP_TUNNEL_SUPPORT
    /* For tunnels, this parameter holds the TCI VLANs for add/replace operation */
    unsigned int            tunnel_vlans;
#endif

    /* This is the session handle. Users dont need to populate this
     * information since this handle will be filled up the PPM on the
     * successful creation of the session. */
    unsigned char           session_handle;

    /* Flag which indicates if the session was for a ROUTER or BRIDGE
     * This information is required because if the session is for the
     * ROUTER the PDSP needs to ensure that all packets matching their
     * session have their TTL decremented. On the other hand for BRIDGE
     * sessions this is not do be done. */
    unsigned char           is_routable_session;

    /* Flag which indicates the priority of the session.
     * With the introduction of QoS this will play an important part. */
    unsigned char           priority;
    unsigned char           cluster;

    /* Ingress Properties:-
     *  These describe the ingress interface on which the packet was
     *  received and how the packet looks like when it arrives into the
     *  box. */
    TI_PP_SESSION_PROPERTY  ingress;

    /* Counter which indicates the number of egress session properties
     * which are valid in the field below. Note: This should be at least 1 */
    unsigned short          num_egress;

    /* Egress Properties:-
     *  These properties describe the egress interface on which the packet
     *  is to be transmitted and how the packet will look like when it
     *  leaves the box. There are multiple egress properites because a
     *  packet could be flooded onto multiple interfaces. */
    TI_PP_SESSION_PROPERTY  egress[TI_PP_MAX_EGRESS_PROPERTY];
}TI_PP_SESSION;

/**************************************************************************
 * STRUCTURE NAME : TI_PP_SESSION_STATS
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the session statistics for the Packet Processor.
 **************************************************************************/
typedef struct TI_PP_SESSION_STATS
{
    unsigned int    alive_idle_time;
    unsigned int    packets_forwarded;
    unsigned int    bytes_forwarded_hi;
    unsigned int    bytes_forwarded_lo;
} TI_PP_SESSION_STATS;

/**************************************************************************
 * STRUCTURE NAME : TI_PP_GLOBAL_STATS
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the global statistics for the Packet Processor.
 **************************************************************************/
typedef struct TI_PP_GLOBAL_STATS
{
    unsigned int    packets_rxed;
    unsigned int    packets_searched;
    unsigned int    search_matched;
    unsigned int    sync_delay;
    unsigned int    packets_fwd;
    unsigned int    ipv4_packets_fwd;
    unsigned int    desc_starved;
    unsigned int    buffer_starved;
} TI_PP_GLOBAL_STATS;

/**************************************************************************
 * STRUCTURE NAME : TI_PP_QOS_QUEUE
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes a QOS Queue configuration. Generally QOS
 *  configuration for all queues associated to a single cluster is contained in
 *  corresponding Cluster configuration.
 **************************************************************************/
typedef struct TI_PP_QOS_QUEUE
{
    unsigned char   q_num;                  /* Index of the QOS queue (offset from
                                            QOS queue base) */

#define TI_PP_QOS_Q_REALTIME            (1<<0)
    unsigned char   flags;                  /* Control how packets in the queue
                                            should be handled. Available options:
                                            TI_PP_QOS_Q_REALTIME - Disable
                                            scaling of the credit. */

    unsigned short  egr_q;                  /* Queue manager and queue index of
                                            forwarding queue */

    unsigned short  it_credit_bytes;        /* The amount of forwarding byte
                                            “credit” that the queue receives
                                            every 25us. */

    unsigned short  it_credit_packets;      /* The amount of forwarding packets
                                            “credit” that the queue receives
                                            every 25us. */

    unsigned int  max_credit_bytes;      /* The maximum amount of forwarding
                                           byte “credit” that the queue is
                                           allowed to hold at the end of the
                                           25us iteration. */

    unsigned short  max_credit_packets;    /* The maximum amount of forwarding
                                           byte “credit” that the queue is
                                           allowed to hold at the end of the
                                           25us iteration. */

    unsigned short   congst_thrsh_bytes;   /* The size in bytes at which point the
                                           QOS queue is considered to be
                                           congested. */

    unsigned short   congst_thrsh_packets; /* The maximum number of packets
                                           to be kept in QOS queue */
}TI_PP_QOS_QUEUE;

/**************************************************************************
 * STRUCTURE NAME : TI_PP_QOS_CLST_CFG
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes a QOS cluster. It contains the configuration for
 *  all the associated Queues
 **************************************************************************/
typedef struct TI_PP_QOS_CLST_CFG
{
    unsigned char   qos_q_cnt;          /* Number of QOS queues in the cluster
                                           (1 to 9) */

#define TI_PP_QOS_CLST_MAX_QCNT         9
    TI_PP_QOS_QUEUE qos_q_cfg [TI_PP_QOS_CLST_MAX_QCNT]; /* Configuration for
                                                            all the queues
                                                            associated with the
                                                            cluster, arranged
                                                            in order of
                                                            priority
                                                            (qos_q_cfg[0] being
                                                            lower in priority
                                                            than qos_q_cfg[1])
                                                          */

    unsigned int     global_credit_bytes;      /* The amount of global bytes credit
                                                   available to the next QOS queue in
                                                   the cluster */

    unsigned int     max_global_credit_bytes;  /* The maximum amount of global credit
                                                    allowed to carry over to the next
                                                    queue. */

    unsigned short    global_credit_packets;    /* The amount of global pkt credit
                                                   available to the next QOS queue in
                                                   the cluster */

    unsigned short    max_global_credit_packets; /* The maximum amount of global credit
                                                    allowed to carry over to the next
                                                    queue. */

    unsigned short    egr_congst_thrsh_bytes1;  /* Egress Congestion Bytes Threshold 1 */
    unsigned short    egr_congst_thrsh_bytes2;  /* Egress Congestion Bytes Threshold 2 */
    unsigned short    egr_congst_thrsh_bytes3;  /* Egress Congestion Bytes Threshold 3 */
    unsigned short    egr_congst_thrsh_bytes4;  /* Egress Congestion Bytes Threshold 4 */

    unsigned char     egr_congst_thrsh_packets1; /* Egress Congestion Pkts Threshold 1*/
    unsigned char     egr_congst_thrsh_packets2; /* Egress Congestion Pkts Threshold 2 */
    unsigned char     egr_congst_thrsh_packets3; /* Egress Congestion Pkts Threshold 3 */
    unsigned char     egr_congst_thrsh_packets4; /* Egress Congestion Pkts Threshold 4 */

}TI_PP_QOS_CLST_CFG;



/**************************************************************************
 * STRUCTURE NAME : TI_PP_QOS_QUEUE_STATS
 **************************************************************************
 * DESCRIPTION   :
 *  The structure contains packet statistics for a QOS queue.
 **************************************************************************/
typedef struct TI_PP_QOS_QUEUE_STATS
{
    unsigned int   fwd_pkts;            /* Number of packets forwarded to the
                                           Egress Queue */
    unsigned int   drp_cnt;             /* Number of packets dropped due to
                                           congestion */
}TI_PP_QOS_QUEUE_STATS;


/**************************************************************************
 * STRUCTURE NAME : TI_PP_VERSION
 **************************************************************************
 * DESCRIPTION   :
 *  The structure contains four version notation numbers.
 **************************************************************************/
typedef struct  TI_PP_VERSION
{
    unsigned char   v0;
    unsigned char   v1;
    unsigned char   v2;
    unsigned char   v3;
}TI_PP_VERSION;

/**************************************************************************
 * STRUCTURE NAME : PPM_STATUS
 **************************************************************************
 * DESCRIPTION   :
 *  Defines the status of the various internal structures.
 **************************************************************************/
typedef enum PPM_STATUS
{
    INACTIVE = 0x0,
    ACTIVE   = 0x1,
    PPM_PSM  = 0x2
}PPM_STATUS;


/**************************************************************************
 **************************** PPM Exported API ****************************
 **************************************************************************/
#ifdef __KERNEL__

/* Initialization and Cleanup API */
extern int ti_ppm_initialize (TI_PPM_OS_FUNC_TABLE* ptr_os_table);

/* PID and VPID Management API */
extern int ti_ppm_create_pid (TI_PP_PID* ptr_pid, void * netdevPtr);
extern int ti_ppm_delete_pid (unsigned char pid_handle);
extern int ti_ppm_config_pid_range (TI_PP_PID_RANGE *pid_range);
extern int ti_ppm_remove_pid_range (unsigned int port_num);
extern int ti_ppm_set_pid_flags (unsigned char pid_handle, unsigned int new_flags);
extern int ti_ppm_create_vpid (TI_PP_VPID* ptr_vpid);
extern int ti_ppm_delete_vpid (unsigned char vpid_handle);
extern int ti_ppm_set_vpid_flags (unsigned char vpid_handle, unsigned int new_flags);
extern int ti_ppm_get_pid  (int num_pid, TI_PP_PID* pid);
extern int ti_ppm_get_pid_info  (int pid_handle, TI_PP_PID* ptr_pid);
extern int ti_ppm_get_vpid (int pid_handle, int num_vpid, TI_PP_VPID* vpid);
extern int ti_ppm_get_vpid_info (int vpid_handle, TI_PP_VPID* ptr_vpid);
extern int ti_ppm_dispaly_session_info(int session_id);
extern int ti_ppm_dispaly_qos_queue_info(int queue_id);
extern int ti_ppm_dispaly_qos_cluster_info(int cluster_id);

void ti_ppm_delete_drop_sessions(int *netfilter_null_vpid_handle,int *docsis_null_vpid_handle,unsigned int ip_mode,
								unsigned int* src_ip,unsigned int* dst_ip,
								unsigned short src_port, unsigned short dst_port);

/* Session Management API */
extern int ti_ppm_create_session (TI_PP_SESSION* ptr_session, void* ptr, int isTunnel, unsigned char isVoice, int *isNew);
extern int ti_ppm_delete_session (unsigned char session_handle, TI_PP_SESSION_STATS* ptr_session_stats);
extern int ti_ppm_check_session (TI_PP_SESSION* ptr_session);
extern int ti_ppm_lookup_session (unsigned char session_handle);
extern int ti_ppm_get_session (int vpid_handle, int num_sessions, unsigned char* ptr_session);
extern int ti_ppm_get_session_info (int session_handle, TI_PP_SESSION* ptr_session);
extern int ti_ppm_flush_sessions (int vpid_handle);

/* Statistics API */
extern int ti_ppm_get_session_stats (unsigned char session_handle, TI_PP_SESSION_STATS* ptr_session_stats);
extern int ti_ppm_clear_session_stats (unsigned char session_handle);
extern int ti_ppm_get_vpid_stats (unsigned char vpid_handle, TI_PP_VPID_STATS* ptr_vpid_stats);
extern int ti_ppm_clear_vpid_stats (unsigned char vpid_handle);
extern int ti_ppm_get_global_stats (TI_PP_GLOBAL_STATS* ptr_stats);
extern int ti_ppm_clear_global_stats (void);


/* Utility API. */
extern int ti_ppm_get_version ( TI_PP_VERSION *version );
extern int ti_ppm_health_check (void);

/* Event Handler Framework API */
extern unsigned int ti_ppm_register_event_handler (void (*event_handler)(unsigned int id, unsigned int p1, unsigned int p2));
extern int ti_ppm_unregister_event_handler (unsigned int handle_event_handler);

/* QoS API. */
extern int ti_ppm_qos_cluster_setup (unsigned char clst_indx, TI_PP_QOS_CLST_CFG* clst_cfg);
extern int ti_ppm_qos_cluster_enable (unsigned char clst_indx);
extern int ti_ppm_qos_cluster_disable (unsigned char clst_indx);
extern int ti_ppm_get_qos_q_stats (unsigned char qos_qnum, TI_PP_QOS_QUEUE_STATS *stats);
extern int ti_ppm_get_n_clear_qos_q_stats (unsigned char qos_qnum, TI_PP_QOS_QUEUE_STATS *stats);
extern int ti_ppm_set_qos_cluster_max_global_credit(int byteCredit, int cluster_id,int max_global_credit);
extern int ti_ppm_set_qos_queue_max_credit(int byteCredit, int queue_id,int max_credit);
extern int ti_ppm_set_qos_queue_iteration_credit(int byteCredit, int queue_id,int it_credit);

/* Power Saving Mode (PSM) API. */
extern int ti_ppm_enable_psm(void);
extern int ti_ppm_disable_psm(void);
extern PPM_STATUS ti_pp_get_status(void);
extern int ti_ppm_set_ack_suppression(int enDis);

extern int ti_ppm_set_mta_mac_address (unsigned char* mtaAddress);

#ifdef CONFIG_INTEL_PP_TUNNEL_SUPPORT
extern int ti_ppm_set_tunnel_mode (int tunnelMode);
extern int ti_ppm_set_cm_mac_address (unsigned char* cmAddress);
#endif

#endif /* __KERNEL__ */

#endif /* __TI_PPM_H__ */




