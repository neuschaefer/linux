/*
 * ppd_pvt.h
 * ppd_pvt.h. Packet processor driver private header file.
 *
 *  Version 0.3
 *
 * (C) 2007, Texas Instruments, Inc.
 * Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 * kind, whether express or implied; without even the implied warranty
 *  for more details.
 *
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */

#ifndef _INCLUDE_PPD_PVT_H
#define _INCLUDE_PPD_PVT_H

#if defined (CONFIG_MACH_PUMA6)
    #include <puma6_cppi.h>
#endif

#if defined (CONFIG_MACH_PUMA5)
    #include <puma5_cppi.h>
#endif

#define PPD_ENABLE_LUT_DUMP

#define PPD_DEBUG_ERR   (1<<0)
#define PPD_DEBUG_MSG   (1<<1)
#define PPD_DEBUG_CMD   (1<<2)
#define PPD_DEBUG_EVN   (1<<3)

#define PPD_DEBUG (PPD_DEBUG_ERR)
//#define PPD_DEBUG (PPD_DEBUG_ERR|PPD_DEBUG_MSG)

#define STRUCT_B0_3(b0, b1, b2, b3) struct {Uint8 b3; Uint8 b2; Uint8 b1; Uint8 b0;}
#define STRUCT_B0_1_S0(b0, b1, s0)  struct {Uint16 s0; Uint8 b1; Uint8 b0; }
#define STRUCT_S0_B0_1(s0, b0, b1)  struct {Uint8 b1; Uint8 b0; Uint16 s0;}
#define STRUCT_S0_1(s0, s1)         struct {Uint16 s1; Uint16 s0;}
#define STRUCT_W0(w0)               struct {Uint32 w0;}

#ifdef CONFIG_MACH_PUMA5
/* LUT L2-ETH Enable Flags */
#define TI_PP_L2_LUT_DST_MAC_EN    (0x5 << 0)
#define TI_PP_L2_LUT_SRC_MAC_EN    (0x5 << 1)
#define TI_PP_L2_LUT_ETH_TYPE_EN   (0x1 << 4)
#define TI_PP_L2_LUT_VLAN_ID_EN    (0x1 << 5)
#define TI_PP_L2_LUT_PKT_KEY_EN    (0x1 << 6)
#define TI_PP_L2_LUT_IN_PID_EN     (0x1 << 7)
#define TI_PP_L2_LUT_VLAN_PRI_EN   (0x1 << 8)
#define TI_PP_L2_LUT_DOCSIS_ID_EN  (0x1 << 9)

/* LUT L3-IPv4 Enable Flags */
#define TI_PP_L3_LUT_DST_IPV4_EN          (0x1 << 0)
#define TI_PP_L3_LUT_SRC_IPV4_EN          (0x1 << 1)
#define TI_PP_L3_LUT_TCP_UDP_DST_PORT_EN  (0x1 << 2)
#define TI_PP_L3_LUT_TCP_UDP_SRC_PORT_EN  (0x1 << 3)
#define TI_PP_L3_LUT_PPPOE_SES_ID_EN        (0x1 << 4)
#define TI_PP_L3_LUT_PKT_KEY_EN         (0x1 << 6)
#define TI_PP_L3_LUT_IPV4_FRAG_FLAG_EN    (0x1 << 7)
#define TI_PP_L3_LUT_IPV4_PROTO_EN        (0x1 << 8)
#define TI_PP_L3_LUT_IPV4_TOS_EN          (0x1 << 9)

/* LUT L3-IPv6 Enable Flags */
#define TI_PP_L3_LUT_DST_IPV6_EN          (0x1 << 0)
#define TI_PP_L3_LUT_SRC_IPV6_EN          (0x1 << 1)
//#define TI_PP_L3_LUT_TCP_UDP_DST_PORT_EN  (0x1 << 2)
//#define TI_PP_L3_LUT_TCP_UDP_SRC_PORT_EN  (0x1 << 3)
//#define TI_PP_L3_LUT_PPPOE_SES_ID_EN      (0x1 << 4)
#define TI_PP_L3_LUT_IPV6_FLOW_LBL_LOW_EN (0x1 << 5)
//#define TI_PP_L3_LUT_PKT_KEY_EN             (0x1 << 6)
#define TI_PP_L3_LUT_IPV6_FLOW_LBL_HI_EN  (0x1 << 7)
#define TI_PP_L3_LUT_IPV6_NEXT_HDR_EN     (0x1 << 8)
#define TI_PP_L3_LUT_IPV6_T_CLASS_EN      (0x1 << 9)
#define TI_PP_L3_LUT_IPV6_FLOW_LBL_EN     (TI_PP_L3_LUT_IPV6_FLOW_LBL_LOW_EN | TI_PP_L3_LUT_IPV6_FLOW_LBL_HI_EN)
#define TI_PP_L3_LUT_IPV6_DSLITE_EN       (TI_PP_L3_LUT_PPPOE_SES_ID_EN | TI_PP_L3_LUT_IPV6_FLOW_LBL_LOW_EN)    // we reuse both flags since DsLite dst ip is set instead of these fields

#else // CONFIG_MACH_PUMA6

/* LUT1 L2-ETH Enable Flags */
#define TI_PP_L2_LUT_DST_MAC_EN           (0x3)
#define TI_PP_L2_LUT_SRC_MAC_EN           (0x7 << 1)
#define TI_PP_L2_LUT_ETH_TYPE_EN          (0x1 << 5)
#define TI_PP_L2_LUT_IN_PID_EN            (0x1 << 6)
#define TI_PP_L2_LUT_PKT_KEY_EN           (0x1 << 7)

/* LUT1 L3-IP Enable Flags */
#define TI_PP_L3_LUT_LAN_IPV4_EN          (0x1)
#define TI_PP_L3_LUT_LAN_IPV6_EN          (0x3F)
#define TI_PP_L3_LUT_PPPOE_SES_ID_EN      (0x3 << 6)
#define TI_PP_L3_LUT_IP_PROTO_EN          (0x1 << 8)
#define TI_PP_L3_LUT_PKT_KEY_EN           (0x1 << 9)


/* LUT2 Enable Flags */
#define TI_PP_LUT2_WAN_IP_EN                0x01
#define TI_PP_LUT2_IPV6_FLOW_LABEL_EN       0x02
#define TI_PP_LUT2_FIRST_VLAN_EN            0x04
#define TI_PP_LUT2_SECOND_VLAN_EN           0x08
#define TI_PP_LUT2_L4_SRC_PORT_EN           0x10
#define TI_PP_LUT2_L4_DEST_PORT_EN          0x20
#define TI_PP_LUT2_TOS_TRAFFIC_CLASS_EN     0x40

#define TI_PP_L3_LUT_IPV6_DSLITE_EN         TI_PP_LUT2_IPV6_FLOW_LABEL_EN

#endif

/* LUT Entry Type */
#define TI_PP_LUT_DATA_L2_ETH               (0x00)
#define TI_PP_LUT_DATA_L2_UNDEF             (0x07)
#define TI_PP_LUT_DATA_L3_IPV4              (0x08)
#define TI_PP_LUT_DATA_L3_IPV6              (0x09)
#define TI_PP_LUT_DATA_L3_DSLITE            (0x0A)
#define TI_PP_LUT_DATA_L3_DS_GRE            (0x0B)
#define TI_PP_LUT_DATA_L3_UNDEF             (0x0F)


/* Valid flags in Modification Record */
#define TI_PP_MOD_IPSRC_VALID       (1<<0)
#define TI_PP_MOD_IPDST_VALID       (1<<1)
#define TI_PP_MOD_IPADR_VALID       (1<<2)
#define TI_PP_MOD_L3CHK_VALID       (1<<3)
#define TI_PP_MOD_SRCPORT_VALID     (1<<4)
#define TI_PP_MOD_DSTPORT_VALID     (1<<5)
#define TI_PP_MOD_PORTS_VALID       (1<<6)
#define TI_PP_MOD_L4CHK_VALID       (1<<7)
#define TI_PP_MOD_IPTOS_VALID       (1<<8)

/* Egress Framing Record */
#define TI_PP_EGR_FRM_STRIP_L2          (1<<0)
#define TI_PP_EGR_FRM_TURBODOX_EN       (1<<2)
#define TI_PP_EGR_FRM_PPPOE_HDR         (1<<4)
#define TI_PP_EGR_FRM_REFRAME_IP        (1<<5)
#define TI_PP_EGR_FRM_TURBODOX_ADV_EN   (1<<7)

/* Egress Flags */
#ifdef CONFIG_MACH_PUMA5
#define TI_PP_EGR_FLAG_DSLITE_US               (1 << 0)
#define TI_PP_EGR_FLAG_DSLITE_DS               (1 << 1)
#define TI_PP_EGR_FLAG_GRE_US                  (1 << 2)
#define TI_PP_EGR_FLAG_GRE_DS                  (1 << 3)
#define TI_PP_EGR_FLAG_NEW_HEADER_HAS_IPV6     (1 << 4)
#define TI_PP_EGR_FLAG_SESS_MAPPED_TO_LOW_PRIO (1 << 5) 
#else
#define TI_PP_EGR_FLAG_NEW_HEADER_HAS_IPV4  (1 << 2)
#define TI_PP_EGR_FLAG_NEW_HEADER_HAS_IPV6  (1 << 3)
#define TI_PP_EGR_FLAG_NEW_HEADER_PTR       (1 << 4)
#define TI_PP_EGR_FLAG_NEW_HEADER_INTERNAL  (1 << 5)
#define TI_PP_EGR_FLAG_MOD_REC_VALID        (1 << 6)
#endif

typedef Uint8 TI_PP_ses_id_t;

/* Base Session */
#define TI_PP_SES_FLAG_IDLE_TMOUT           (1 << 0)
#ifdef CONFIG_MACH_PUMA5
#define TI_PP_SES_FLAG_PASS_AFTER_TMOUT     (1 << 1)
#else
#define TI_PP_SES_FLAG_TCP_CONTROL          (1 << 1)
#endif
#define TI_PP_SES_FLAG_NO_INGRESS_STATS     (1 << 2)
#define TI_PP_SES_FLAG_NO_EGRESS_STATS      (1 << 3)
#define TI_PP_SES_FLAG_XLUDE_ETH_HDR_STATS  (1 << 4)
#define TI_PP_SES_FLAG_UPDATE_TTL           (1 << 5)
#define TI_PP_SES_FLAG_PROC_TTL_EXP         (1 << 6)
#define TI_PP_SES_FLAG_IP_FRAG              (1 << 7)
#define TI_PP_SES_FLAG_PROC_IP_OPTS         (1 << 8)
#define TI_PP_SES_FLAG_IPV6_CLASS_MASK      (3 << 8)
#define TI_PP_SES_FLAG_IPV6_CLASS_SET(x)    \
    (((x) <<  8) & TI_PP_SES_FLAG_IPV6_CLASS_MASK)
#ifdef CONFIG_MACH_PUMA5
#define TI_PP_SES_FLAG_USE_FULL_SRC_IPV6    (1 << 10)
#define TI_PP_SES_FLAG_USE_FULL_DST_IPV6    (1 << 11)
#define TI_PP_SES_FLAG_TCP_CONTROL          (1 << 12)
#else
#define TI_PP_SES_TUNNEL                    (1 << 10)
#define TI_PP_SES_TUNNEL_DS                 (0 << 11)
#define TI_PP_SES_TUNNEL_US                 (1 << 11)
#define TI_PP_SES_TUNNEL_GRE                (0 << 12)
#define TI_PP_SES_TUNNEL_DS_LITE            (1 << 12)
#define TI_PP_SES_TUNNEL_RES2               (2 << 12)
#define TI_PP_SES_TUNNEL_RES3               (3 << 12)

#define TI_PP_SES_FLAG_TUNNEL_MASK          (0xF << 10)
#define TI_PP_SES_FLAG_GRE_US               (TI_PP_SES_TUNNEL | TI_PP_SES_TUNNEL_US | TI_PP_SES_TUNNEL_GRE)
#define TI_PP_SES_FLAG_GRE_DS               (TI_PP_SES_TUNNEL | TI_PP_SES_TUNNEL_DS | TI_PP_SES_TUNNEL_GRE)
#define TI_PP_SES_FLAG_DS_LITE_US           (TI_PP_SES_TUNNEL | TI_PP_SES_TUNNEL_US | TI_PP_SES_TUNNEL_DS_LITE)
#define TI_PP_SES_FLAG_DS_LITE_DS           (TI_PP_SES_TUNNEL | TI_PP_SES_TUNNEL_DS | TI_PP_SES_TUNNEL_DS_LITE)

#define TI_PP_SES_FLAG_DSLITE_US_FRAG_IPv4  (1 << 14)

#endif

/* Session Command Bits */
#define TI_PP_SES_CBIT_TABLEADD         7
#define TI_PP_SES_CBIT_TABLEREMOVE      6
#define TI_PP_SES_CBIT_SESSIONPAUSE     5
#define TI_PP_SES_CBIT_SESSIONRESUME    4
#define TI_PP_SES_CBIT_STATECHANGE      3

/* Session start state */
#define TI_PP_SES_STATE_IDLE            (0x00)
#define TI_PP_SES_STATE_NEEDSYNC        (0x01)
#ifdef CONFIG_MACH_PUMA6
#define TI_PP_SES_STATE_QUEUE           (0x02)
#define TI_PP_SES_STATE_FWD             (0x03)
#define TI_PP_SES_STATE_DIVERT          (0x04)
#else
#define TI_PP_SES_STATE_NEEDSYNC_END    (0x02)
#define TI_PP_SES_STATE_QUEUE           (0x03)
#define TI_PP_SES_STATE_FWD             (0x04)
#define TI_PP_SES_STATE_DIVERT          (0x05)
#endif

#if defined (CONFIG_MACH_PUMA6)
#define SESSION_ADD_SYNC                (0 << 12) /* bit 12, bit 13 are not set */
#define SESSION_REMOVE                  (1 << 12)
#define SESSION_CHANGE                  (2 << 12)
#define SESSION_LUT1_INDEX_VALID        (1 << 14)
#else
/*
 * Session Commands
 */
#define SESSION_ADD_SYNC                ((1<<TI_PP_SES_CBIT_TABLEADD) | (1<<TI_PP_SES_CBIT_STATECHANGE) | TI_PP_SES_STATE_NEEDSYNC)
#define SESSION_REMOVE                  ((1<<TI_PP_SES_CBIT_TABLEREMOVE) | (1<<TI_PP_SES_CBIT_STATECHANGE) | TI_PP_SES_STATE_IDLE)
#define SESSION_ADD_FORWARD             ((1<<TI_PP_SES_CBIT_TABLEADD) | (1<<TI_PP_SES_CBIT_STATECHANGE) | TI_PP_SES_STATE_FWD)
#define SESSION_UPDATE                  ((1<<TI_PP_SES_CBIT_TABLEADD) | (1<<TI_PP_SES_CBIT_TABLEREMOVE))
#define SESSION_STATE_SYNC              ((1<<TI_PP_SES_CBIT_STATECHANGE) | TI_PP_SES_STATE_NEEDSYNC)
#define SESSION_STATE_FORWARD           ((1<<TI_PP_SES_CBIT_STATECHANGE) | TI_PP_SES_STATE_FWD)
#define SESSION_STATE_DIVERT            ((1<<TI_PP_SES_CBIT_STATECHANGE) | TI_PP_SES_STATE_DIVERT)
#endif

/* PDSP commands */
#define SRPDSP_SR_OPEN                  0x80
#define SRPDSP_SR_FLUSH_ALL             0x81
#define SRPDSP_SR_FLUSH_MANY            0x82

#ifdef CONFIG_MACH_PUMA6

#else
#define SRPDSP_SR_CLOSE                 0x81
#define SRPDSP_SR_STATS_GET             0x82
#define SRPDSP_SR_STATS_CLEAR           0x83
    #define STATS_TYPE_SRGLOBAL             0x01
    #define STATS_TYPE_VPID                 0x02
    #define STATS_TYPE_SESSION              0x03
#endif

#define SRPDSP_PID_COMMAND              0x84
    #define PID_ADD                         0x01    
    #define PID_REMOVE                      0x02    
    #define PID_CHANGE_FLAGS                0x03    
    #define PID_INSTALL_CONFIG_RANGE        0x04
    #define PID_REMOVE_CONFIG_RANGE         0x05
#define SRPDSP_VPID_COMMAND             0x85
    #define VPID_ADD                        0x01    
    #define VPID_REMOVE                     0x02    
    #define VPID_CHANGE_FLAGS               0x03    
#define SRPDSP_SESSION_COMMAND          0x86
#define SRPDSP_SR_STATUS                0x87
#define SRPDSP_QOS_CLUSTER              0xA0
    #define QOS_CLUSTER_ENABLE              0x01    
    #define QOS_CLUSTER_DISABLE             0x00    
#define SRPDSP_SR_SETPSM                0x88
#define SRPDSP_SR_VERSION               0x89

#ifdef CONFIG_MACH_PUMA5
#define SRPDSP_SR_TUNNEL_MODE           0x8A
#define SRPDSP_SR_CM_ADDR               0x8B
#endif

#define SRPDSP_SR_ACK_SUPPRESS          0x8C

/* Success Code */
#define SR_RETCODE_SUCCESS          1

/* PDSP error codes */
#define SRPDSP_ENORES                   -1
#define SRPDSP_EINVCMD                  -2
#define SRPDSP_EINVOPT                  -3
#define SRPDSP_EINVINDEX                -4
#define SRPDSP_EALREADYOPEN             -5
#define SRPDSP_ENOTOPEN                 -6
#define SRPDSP_EMAPERROR                -7
#define SRPDSP_EINVPORT                 -8
#define SRPDSP_EINVPID                  -9
#define SRPDSP_EPAUSELIMITEXCEED        -10
#define SRPDSP_ESESSIONNOTPAUSED        -11
#define SRPDSP_ESESSIONPAUSED           -12
#define SRPDSP_EREOPENINVALID           -13
#define SRPDSP_EINTERROR                -99

#define EVENT_POLLTIME_MSECS            100


typedef Uint8 TI_PP_pid_t;
typedef Uint8 TI_PP_vpid_t;


#define PP_NUM_PDSP                 3

/* With PDSP freq 200MHz it will take 20 sec to overflow the counter so we will
 * set 10 sec as polling time.
 */
#define PDSP_POLLTIME_MSECS         10000

/* PDSPs in system are identified with following ids */
#define CPDSP       0
#define MPDSP       1
#define QPDSP       2
#define APDSP       3
/*
 * PDSP Registers structure.
 *  The structure instance variable points to PDSP register space directly.
 */
typedef volatile struct
{
    Uint32    control;             /* 0x00 */
    Uint32    status;              /* 0x04 */
    Uint32    wakeup_en;           /* 0x08 */
    Uint32    cycle_count;         /* 0x0C */
    Uint32    stall_count;         /* 0x10 */
    Uint32    pad0[3];
    Uint32    table_block_index0;  /* 0x20 */
    Uint32    table_block_index1;  /* 0x24 */
    Uint32    table_program_ptr0;  /* 0x28 */
    Uint32    table_program_ptr1;  /* 0x2C */

} pdsp_regs_t;

#define PDSP_CTL_BIG_EN_BIT         (1 << 14)
#define PDSP_CTL_EN_BIT             (1 <<  1)
#define PDSP_CTL_N_RST_BIT          (1 <<  0)
#define PDSP_REG_RUN_STATE_BIT      (1<<15)
#define PDSP_REG_SLEEP_BIT          (1<<2)
#define PDSP_REG_COUNT_ENABLE_BIT   (1<<3)
#define PDSP_REG_SINGLE_STEP_BIT    (1<<8)

/*
 * PDSP register space overlay pointer.
 *
 */
typedef pdsp_regs_t* PDSP_RegsOvly;

typedef struct PPD_PDSP_COUNTS
{
    Uint32 cycle_cnt_hi;        /* Count of number of cycles PDSP is 'Running'
                                   (high 32 bits) */
    Uint32 cycle_cnt_lo;        /* Count of number of cycles PDSP is 'Running'
                                   (low 32 bits) */
    Uint32 stall_cnt_hi;        /* Count of number of cycles PDSP is stalled
                                   forinstruction while 'Running' (high 32 bits)
                                   */
    Uint32 stall_cnt_lo;        /* Count of number of cycles PDSP is stalled for
                                   instruction while 'Running' (low 32 bits) */

    Uint32 cycle_sec_cnt_hi;    /* Count of number of cycles PDSP is 'Running'
                                   (high 32 bits) */
    Uint32 cycle_sec_cnt_lo;    /* Count of number of cycles PDSP is 'Running'
                                   (low 32 bits) */

} ppd_pdsp_counts;

typedef struct pdsp_poll_cfg
{

    PAL_OsTimerHandle       hTimer;
    Uint32                  polltime_msecs;
} ppd_pdsp_poll_cfg;

typedef struct
{
    Uint32 cycle_cnt_hi;       /* PDSP cycle count value at time of session
                                  creation */
    Uint32 cycle_cnt_lo;       /* low 32 bits */

} ppd_ses_timestamp;

/*
 *                      Session related data structures
 */

#if defined (CONFIG_MACH_PUMA6)
typedef struct
{
    Uint8   IngressVPID;
    Uint8   StatusFlags;
    Uint16  SessionFlags;
    Uint32  TimeoutThresh;
    Uint32  ReferenceTime;
    Uint8   SynchQ;
    Uint8   State;
    Uint16  Res1;
} ppdSessionBaseRecord_t;

typedef struct
{
    Uint8   Priority;
    Uint8   EgressVPID;
    Uint8   FramingCode;
    Uint8   NewHeaderSize;
    Uint16  TxDestTag;
    Uint16  TxQueueBase;
    Uint8   EgressFlags;
    Uint8   NextEgressRecIdx;
    Uint8   EgressPidType;
    Uint8   UsPayloadLenOff;
    Uint32  UsTurboDoxAck;
    Uint8   UsSfIndex;
    Uint8   UsPhsIndex;
    Uint8   UsTcpFlags;
    Uint8   UsTdoxSessId;
} ppdSessionEgressRecord_t;

typedef struct
{
    Uint8   Res1;
    Uint8   Tos;
    Uint16  ModFlags;
    Uint32  IpSrc;
    Uint32  IpDst;
    Uint16  PortSrc;
    Uint16  PortDst;
    Uint16  L3ChecksumDelta;
    Uint16  L4ChecksumDelta;
} ppdSessionModificationRecord_t;

typedef struct
{   
    ppdSessionBaseRecord_t          sessionBaseRecord;
    ppdSessionEgressRecord_t        sessionEgressRecord;
    ppdSessionModificationRecord_t  sessionModificationRecord;
    Uint8                           sessionNewHeader[24];
} ppdSessionInfo_t;

typedef struct
{   
    ppdSessionEgressRecord_t        sessionEgressRecord;
    ppdSessionModificationRecord_t  sessionModificationRecord;
    Uint8                           sessionNewHeader[24];
} ppdMulticastSessionInfo_t;
#endif

typedef struct
{
    union
    {
        STRUCT_S0_B0_1 (
                flags,      /* HST */
                status,
                ingress_vpid    /* PPM */
                ) s;
        Uint32 frags;

    } w0;

    Uint32 w1_ipv6_rec;     /* HST */

    union
    {
        STRUCT_B0_3 (
                prev_collision,  /* ??? */
                next_collision,  /* ??? */
                reserved1,      /* NRQ */
                reserved2       /* NRQ */
                ) s;
        Uint32 frags;

    } w2;

    Uint32 w3_egress_ptr;
    union
    {
        Uint32 w4_ses_timeout;      /* HST */
        Uint32 w4_tunnel_config;
    } w4;

    union
    {
        Uint32 w5_ses_ref_time;     /* NRQ */
        Uint32 w5_tunnel_vlans;
    } w5;
    Uint32 w6_fwd_pkts_cnt;     /* NRQ */
    Uint32 w7_fwd_B_hi_cnt;     /* NRQ */
    Uint32 w8_fwd_B_lo_cnt;     /* NRQ */

} ppd_ses_info_blk_t;

typedef struct
{
    Uint32 w0_next_egr_ptr;

    union
    {
       STRUCT_B0_3 (
               l2_hdr_size,
               frame_code,
               egress_vpid,
               priority
               ) s;
        Uint32 frags;

    } w1;

    union
    {
        STRUCT_S0_1 (
                fwd_q_index,    /* Fowarding queue manager and index */
                fwd_dst_tag     /* Destination tag for default forwarding */
                ) s;
        Uint32 frags;

    } w2;

    union
    {
        STRUCT_B0_3 (
                offset_802_3_len,
                egress_dev,
                flags,
                dsLiteIpv6PayloadLengthOffset
                ) s;
        Uint32 frags;

    } w3;

    Uint32 w4_turbo_tcp_ack_num;
    Uint32 w5_l2_hdr_ptr;
    Uint32 w6_mod_rec_ptr;
    Uint32 w7_proto_spec;

} ppd_egress_rec_t;

typedef struct
{
    union
    {
        STRUCT_S0_B0_1 (
                flags,
                tos,
                reserved
                ) s;
        Uint32 frags;

    } w0;

    Uint32  w1_ip_src;
    Uint32  w2_ip_dst;

    union
    {
        Uint16 port_dst;
        Uint16 port_src;
        Uint32 frags;

    } w3;

    union
    {
        Uint16 l4_chksum_delta;
        Uint16 l3_chksum_delta;
        Uint32 frags;

    } w4;

} ppd_pkt_mod_rec_t;

typedef struct
{
    Uint32 w0_src_w3;
    Uint32 w1_src_w2;
    Uint32 w2_src_w1;
    Uint32 w3_src_w0;
    Uint32 w4_dst_w3;
    Uint32 w5_dst_w2;
    Uint32 w6_dst_w1;
    Uint32 w7_dst_w0;

} ppd_ipv6_rec_t;


/* Minimum buffer to be supplied to hold dump data per block */
#define PPD_DUMP_BUFF_BLK_LEN       1000

#define WSTR_W0(idx, w, w0_nm) \
            "w%02d: 0x%08x {%8s : 0x%08x}\n", \
            idx, w, w0_nm, ((w) >> 0) & 0xffffffff

#define WSTR_S1_0(idx, w, s1_nm, s0_nm) \
            "w%02d: 0x%08x {%8s : 0x%04x, %8s : 0x%04x}\n", \
            idx, w, s1_nm, ((w) >> 16) & 0xffff, s0_nm, ((w) >> 0) & 0xffff

#define WSTR_B3_0(idx, w, b3_nm, b2_nm, b1_nm, b0_nm) \
            "w%02d: 0x%08x {%8s :   0x%02x, %8s :   0x%02x, %8s :   0x%02x, \
            %8s :   0x%02x}\n",\
            idx, w, b3_nm, ((w) >> 24) & 0xff, b2_nm, ((w) >> 16) & 0xff, \
            b1_nm, ((w) >> 8) & 0xff, b0_nm, ((w) >> 0) & 0xff

#define WSTR_S0_B1_0(idx, w, s0_nm, b1_nm, b0_nm) \
            "w%02d: 0x%08x {%8s : 0x%04x, %8s :   0x%02x, %8s :   0x%02x}\n", \
            idx, w, s0_nm, ((w) >> 16) & 0xffff, \
            b1_nm, ((w) >> 8) & 0xff, b0_nm, ((w) >> 0) & 0xff

#define WSTR_B1_0_S0(idx, w, b1_nm, b0_nm, s0_nm) \
            "w%02d: 0x%08x {%8s :   0x%02x, %8s :   0x%02x, %8s : 0x%04x}\n", \
            idx, w, b1_nm, ((w) >> 24) & 0xff, \
            b0_nm, ((w) >> 16) & 0xff, s0_nm, ((w) >> 0) & 0xffff

extern Uint32               g_ppd_init_done;
extern Uint32               g_session_slots [];
extern PDSP_RegsOvly        g_pdsp_regs [];
extern ppd_pdsp_counts      g_pdsp_counts [];
extern ppd_ses_timestamp    g_ses_timestamp [];

#define IS_SESSION_SLOT_FILLED(ses_id) \
        (g_session_slots[(Uint32)(ses_id)/32] & (1<<((Uint32)(ses_id)%32)))

#ifdef PPD_ENABLE_LUT_DUMP
#define PP_LUT_SEARCH_WIDTH         40

/* Store LUT the 'enables' words as part of LUT info during session creation, so
 * that these will be decoded during providing dump of LUT to have only the
 * enabled fields dumped.
 */
#define PPD_LUT_DUMP_WORDS          ((PP_LUT_SEARCH_WIDTH/4) + 2)

extern Uint32 g_lut_dump_buff [TI_PP_MAX_ACCLERABLE_SESSIONS][PPD_LUT_DUMP_WORDS];

#define LUT_DATA_TYPE(lut_entry_base)   (((Uint32*)lut_entry_base)[4] & 0xff)
#define LUT_DATA_FLAGS(lut_entry_base)  (((Uint32*)lut_entry_base)[5])

#endif /* PPD_ENABLE_LUT_DUMP */

extern ppd_ses_info_blk_t *gp_ses_blk;

typedef struct
{
#ifdef CONFIG_MACH_PUMA5
    PAL_Cppi4QueueHnd       eventQHnd;
    PAL_Cppi4QueueHnd       eventFQHnd;
#endif
    TI_PPD_EVENT_HANDLER    pfnEventHdlr;
    PAL_OsTimerHandle       hTimer;
    Uint32                  polltime_msecs;
} ppd_event_hdlr_cfg_t;

extern Uint32 g_is_in_psm;
extern ppd_event_hdlr_cfg_t g_ppd_event_hdlr_cfg;
extern ppd_pdsp_poll_cfg    g_pdsp_poll_cfg;

/* Forward declaration */
extern Int32 ppd_set_psm (Uint32 enable);
extern void  ppd_pdsp_poll_timer (unsigned long data);
#if defined PPD_ENABLE_LUT_DUMP
Int32 ppd_dump_ipv6_lut (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len);
Int32 ppd_dump_ipv4_lut (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len);
Int32 ppd_dump_eth_lut (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len);
#endif

#endif /* !_INCLUDE_PPD_PVT_H */
