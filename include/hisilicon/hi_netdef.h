/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2009-2019, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_netdef.h
  版 本 号   : 初稿
  生成日期   : D2011_07_29
  最近修改   :

******************************************************************************/
#ifndef __HI_NETDEF_H__
#define __HI_NETDEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*网络序转换,系统只支持little-endian*/
#ifndef HI_NTOLL
#define HI_NTOLL(data)    ((hi_ulong64)( ((hi_ulong64)(data)  >> 56) \
                                        | ((hi_ulong64)(data) << 56)\
                                        | (((hi_ulong64)(data) >> 40)  & 0x000000000000ff00 ) \
                                        | (((hi_ulong64)(data) << 40 ) & 0x00ff000000000000 )\
                                        | (((hi_ulong64)(data) >> 24)  & 0x0000000000ff0000 )\
                                        | (((hi_ulong64)(data) << 24)  & 0x0000ff0000000000 )\
                                        | (((hi_ulong64)(data) >> 8  )  & 0x00000000ff000000 )\
                                        | (((hi_ulong64)(data) << 8  )  & 0x000000ff00000000 )))
#endif

#ifndef HI_NTOHL
#define HI_NTOHL(data)    ((hi_uint32)( ((hi_uint32)(data)  >> 24) \
                                        | ((hi_uint32)(data)  << 24)  \
                                        | (((hi_uint32)(data) >> 8) & 0x0000ff00 )   \
                                        | (((hi_uint32)(data) << 8) & 0x00ff0000 )))
#endif
#ifndef HI_HTONL
#define HI_HTONL(data)    ((hi_uint32)( ((hi_uint32)(data)  >> 24) \
                                        | ((hi_uint32)(data)  << 24)  \
                                        | (((hi_uint32)(data) >> 8) & 0x0000ff00 )   \
                                        | (((hi_uint32)(data) << 8) & 0x00ff0000 )))
#endif
#ifndef HI_NTOHS
#define HI_NTOHS(data)    ((hi_ushort16)(((hi_ushort16)(data) >> 8 ) \
                                        | ((hi_ushort16)(data) << 8 )))
#endif

#ifndef HI_HTONS
#define HI_HTONS(data)    ((hi_ushort16) (((hi_ushort16)(data) >> 8 ) \
                                        | ((hi_ushort16)(data) << 8 )))
#endif

#define HI_USHORT(byte0, byte1)  ( ( HI_BITMASK_LOW08 & (hi_uint32)(byte0))    \
                                | ( (HI_BITMASK_LOW08 & (hi_uint32)(byte1)) << HI_NUM_8) )
#define HI_UINT(byte0, byte1, byte2, byte3)  ( (HI_BITMASK_LOW08 & (hi_uint32)(byte0))    \
                                | ((HI_BITMASK_LOW08 & (hi_uint32)(byte1)) << HI_NUM_8) \
                                | ((HI_BITMASK_LOW08 & (hi_uint32)(byte2)) << (HI_NUM_8 * 2)) \
                                | ((HI_BITMASK_LOW08 & (hi_uint32)(byte3)) << (HI_NUM_8 * 3)) )


/****************************************************/

/*------------------------------------------------------------------
| PRI(3bits) | CFI(1bit) | TAG(12bits) | Ether-Type(2bytes) | DATA |  
--------------------------------------------------------------------*/
#define HI_VLAN_PRI_MAX              (0x7)
#define HI_VLAN_PRI_INVALID          (0xFF)

#define HI_GET_VLAN_VID(TAG)         ((TAG)&0x0FFF)
#define HI_GET_VLAN_PRI(TAG)         (((TAG)&0xE000)>>13)
#define HI_GET_VLAN_TAG(PRI,VID)     (((((hi_ushort16)PRI)&0x7)<<13)|((VID)&0x0FFF))

#define HI_INADDR_ANY       (0x00000000)
#define HI_INADDR_NONE      0xFFFFFFFFUL
#define HI_INVALID_SOCKET   0xFFFFFFFFUL

#define HI_MTU              1500
#define HI_MAC_MTU          2000

#define HI_MAC_LEN          6   /*MAC地址长度*/
#define HI_ETH_HLEN         14      /* Total octets in header.   */
#define HI_VLAN_HLEN        4
#define HI_VLAN_MASK        0xEFFF
#define HI_VLAN_DEF_TPID    0x8100
#define HI_VLAN_DEF_TPID1   0x88a8
#define HI_IPVERSION        4
#define HI_IPV6ERSION       6
#define HI_IPV4_LEN         4
#define HI_IPV6_LEN         16
#define HI_PPP_LEN          8

#define HI_SKB_RESV_LEN     256
#define HI_SKB_CB_LEN       64

#define HI_AF_UNSPEC	0
#define HI_AF_UNIX		1	/* Unix domain sockets 		*/
#define HI_AF_LOCAL	1	/* POSIX name for AF_UNIX	*/
#define HI_AF_INET		2	/* Internet IP Protocol 	*/
#define HI_AF_AX25		3	/* Amateur Radio AX.25 		*/
#define HI_AF_IPX		4	/* Novell IPX 			*/
#define HI_AF_APPLETALK	5	/* AppleTalk DDP 		*/
#define HI_AF_NETROM	6	/* Amateur Radio NET/ROM 	*/
#define HI_AF_BRIDGE	7	/* Multiprotocol bridge 	*/
#define HI_AF_ATMPVC	8	/* ATM PVCs			*/
#define HI_AF_X25		9	/* Reserved for X.25 project 	*/
#define HI_AF_INET6	10	/* IP version 6			*/
#define HI_AF_ROSE		11	/* Amateur Radio X.25 PLP	*/
#define HI_AF_DECnet	12	/* Reserved for DECnet project	*/
#define HI_AF_NETBEUI	13	/* Reserved for 802.2LLC project*/
#define HI_AF_SECURITY	14	/* Security callback pseudo AF */
#define HI_AF_KEY		15      /* PF_KEY key management API */
#define HI_AF_NETLINK	16
#define HI_AF_ROUTE	AF_NETLINK /* Alias to emulate 4.4BSD */
#define HI_AF_PACKET	17	/* Packet family		*/
#define HI_AF_ASH		18	/* Ash				*/
#define HI_AF_ECONET	19	/* Acorn Econet			*/
#define HI_AF_ATMSVC	20	/* ATM SVCs			*/
#define HI_AF_RDS		21	/* RDS sockets 			*/
#define HI_AF_SNA		22	/* Linux SNA Project (nutters!) */
#define HI_AF_IRDA		23	/* IRDA sockets			*/
#define HI_AF_PPPOX	24	/* PPPoX sockets		*/
#define HI_AF_WANPIPE	25	/* Wanpipe API Sockets */
#define HI_AF_LLC		26	/* Linux LLC			*/
#define HI_AF_CAN		29	/* Controller Area Network      */
#define HI_AF_TIPC		30	/* TIPC sockets			*/
#define HI_AF_BLUETOOTH	31	/* Bluetooth sockets 		*/
#define HI_AF_IUCV		32	/* IUCV sockets			*/
#define HI_AF_RXRPC	33	/* RxRPC sockets 		*/
#define HI_AF_ISDN		34	/* mISDN sockets 		*/
#define HI_AF_PHONET	35	/* Phonet sockets		*/
#define HI_AF_IEEE802154	36	/* IEEE802154 sockets		*/
#define HI_AF_CAIF		37	/* CAIF sockets			*/
#define HI_AF_ALG		38	/* Algorithm sockets		*/
#define HI_AF_NFC		39	/* NFC sockets			*/
#define HI_AF_VSOCK	40	/* vSockets			*/
#define HI_AF_MAX		41	/* For now.. */

#define HI_ETH_P_LOOP       0x0060      /* Ethernet Loopback packet */
#define HI_ETH_P_PUP        0x0200      /* Xerox PUP packet     */
#define HI_ETH_P_PUPAT      0x0201      /* Xerox PUP Addr Trans packet  */
#define HI_ETH_P_IP         0x0800      /* Internet Protocol packet */
#define HI_ETH_P_X25        0x0805      /* CCITT X.25           */
#define HI_ETH_P_ARP        0x0806      /* Address Resolution packet    */
#define HI_ETH_P_BPQ        0x08FF      /* G8BPQ AX.25 Ethernet Packet  [ NOT AN OFFICIALLY REGISTERED ID ] */
#define HI_ETH_P_IEEEPUP    0x0a00      /* Xerox IEEE802.3 PUP packet */
#define HI_ETH_P_IEEEPUPAT  0x0a01      /* Xerox IEEE802.3 PUP Addr Trans packet */
#define HI_ETH_P_DEC        0x6000      /* DEC Assigned proto           */
#define HI_ETH_P_DNA_DL     0x6001      /* DEC DNA Dump/Load            */
#define HI_ETH_P_DNA_RC     0x6002      /* DEC DNA Remote Console       */
#define HI_ETH_P_DNA_RT     0x6003      /* DEC DNA Routing              */
#define HI_ETH_P_LAT        0x6004      /* DEC LAT                      */
#define HI_ETH_P_DIAG       0x6005      /* DEC Diagnostics              */
#define HI_ETH_P_CUST       0x6006      /* DEC Customer use             */
#define HI_ETH_P_SCA        0x6007      /* DEC Systems Comms Arch       */
#define HI_ETH_P_RARP       0x8035      /* Reverse Addr Res packet  */
#define HI_ETH_P_ATALK      0x809B      /* Appletalk DDP        */
#define HI_ETH_P_AARP       0x80F3      /* Appletalk AARP       */
#define HI_ETH_P_8021Q      0x8100      /* 802.1Q VLAN Extended Header  */
#define HI_ETH_P_IPX        0x8137      /* IPX over DIX         */
#define HI_ETH_P_IPV4       0x0800      /* IPv4       */
#define HI_ETH_P_IPV6       0x86DD      /* IPv6 over bluebook       */
#define HI_ETH_P_SLOW       0x8809      /* Slow Protocol. See 802.3ad 43B */
#define HI_ETH_P_WCCP       0x883E      /* Web-cache coordination protocol */
#define HI_ETH_P_PPP_DISC   0x8863      /* PPPoE discovery messages     */
#define HI_ETH_P_PPP_SES    0x8864      /* PPPoE session messages   */
#define HI_ETH_P_MPLS_UC    0x8847      /* MPLS Unicast traffic     */
#define HI_ETH_P_MPLS_MC    0x8848      /* MPLS Multicast traffic   */
#define HI_ETH_P_ATMMPOA    0x884c      /* MultiProtocol Over ATM   */
#define HI_ETH_P_ATMFATE    0x8884      /* Frame-based ATM Transport*/
#define HI_ETH_P_PPP_PRO_IP   0x0021
#define HI_ETH_P_PPP_PRO_IPv6 0x0057
#define HI_PPP_RESV_SESSIONID 0xFFFF    /* reserved PPPoE session ID   */

typedef enum {
  HI_IPPROTO_IP_E = 0,      /* Dummy protocol for TCP       */
  HI_IPPROTO_ICMP_E = 1,        /* Internet Control Message Protocol    */
  HI_IPPROTO_IGMP_E = 2,        /* Internet Group Management Protocol   */
  HI_IPPROTO_IPIP_E = 4,        /* IPIP tunnels (older KA9Q tunnels use 94) */
  HI_IPPROTO_TCP_E = 6,     /* Transmission Control Protocol    */
  HI_IPPROTO_EGP_E = 8,     /* Exterior Gateway Protocol        */
  HI_IPPROTO_PUP_E = 12,        /* PUP protocol             */
  HI_IPPROTO_UDP_E = 17,        /* User Datagram Protocol       */
  HI_IPPROTO_IDP_E = 22,        /* XNS IDP protocol         */
  HI_IPPROTO_DCCP_E = 33,       /* Datagram Congestion Control Protocol */
  HI_IPPROTO_RSVP_E = 46,       /* RSVP protocol            */
  HI_IPPROTO_GRE_E = 47,        /* Cisco GRE tunnels (rfc 1701,1702)    */

  HI_IPPROTO_IPV6_E = 41,/* IPv6-in-IPv4 tunnelling     */

  HI_IPPROTO_ESP_E = 50,            /* Encapsulation Security Payload protocol */
  HI_IPPROTO_AH_E = 51,             /* Authentication Header protocol       */
  HI_IPPROTO_BEETPH_E = 94,        /* IP option pseudo header for BEET */
  HI_IPPROTO_PIM_E    = 103,        /* Protocol Independent Multicast   */

  HI_IPPROTO_COMP_E   = 108,                /* Compression Header protocol */
  HI_IPPROTO_ZERO_HOP_E   = 114,                /* any 0-hop protocol   */

  HI_IPPROTO_SCTP_E   = 132,        /* Stream Control Transport Protocol    */
  HI_IPPROTO_UDPLITE_E = 136,   /* UDP-Lite (RFC 3828)          */

  HI_IPPROTO_RAW_E   = 255,     /* Raw IP packets           */
  HI_IPPROTO_MAX_E,
}hi_ipproto_e;

/*
 *	NextHeader field of IPv6 header
 */
typedef enum {
  HI_NEXTHDR_HOP_E       = 0,	 /* Hop-by-hop option header. */
  HI_NEXTHDR_TCP_E       = 6,	 /* TCP segment. */
  HI_NEXTHDR_UDP_E       = 17,	 /* UDP message. */
  HI_NEXTHDR_IPV6_E      = 41,	 /* IPv6 in IPv6 */
  HI_NEXTHDR_ROUTING_E   = 43,	 /* Routing header. */
  HI_NEXTHDR_FRAGMENT_E  = 44,	 /* Fragmentation/reassembly header. */
  HI_NEXTHDR_ESP_E       = 50,	 /* Encapsulating security payload. */
  HI_NEXTHDR_AUTH_E      = 51,	 /* Authentication header. */
  HI_NEXTHDR_ICMP_E      = 58,	 /* ICMP for IPv6. */
  HI_NEXTHDR_NONE_E      = 59,	 /* No next header */
  HI_NEXTHDR_DEST_E      = 60,	 /* Destination options header. */
  HI_NEXTHDR_MOBILITY_E  = 135,  /* Mobility header. */
  HI_NEXTHDR_MAX_E       = 255,
}hi_ipv6_nexthd_e;

#define HI_NETWORK_IS_VALID_MAC_ADDR(addr)                                               \
        (( !( 0x01 & (addr)[0] ) ) &&                                                    \
         ( ( (addr)[0] | (addr)[1] | (addr)[2] | (addr)[3] | (addr)[4] | (addr)[5]) ) )


#define  HI_NET_DEVICE_TYPE_ROOT  0
#define  HI_NET_DEVICE_TYPE_WAN   1
#define  HI_NET_DEVICE_TYPE_LAN   2


#pragma pack(1)

 
typedef struct {
	union 
	{
		hi_uchar8    uc_addr8[16];
		hi_ushort16  us_addr16[8];
		hi_uint32    ui_addr32[4];
	};
}hi_in6_addr_s;

/* 网络数据结构 */
typedef struct {
	hi_uchar8       uc_dest[HI_MAC_LEN];	/* destination eth addr	*/
	hi_uchar8       uc_source[HI_MAC_LEN];	/* source ether addr	*/
	hi_ushort16     us_proto;		       /* packet type ID field	*/
}hi_ethhdr_s;//__attribute__((packed))

typedef struct
{
    hi_uchar8   uc_version : 4;
    hi_uchar8   uc_type    : 4;
    hi_uchar8   uc_code;
    hi_ushort16 us_sid;
    hi_ushort16 us_len;
    hi_ushort16 us_proto; 
}hi_pppoehdr_s; 


typedef struct {
	hi_ushort16 us_subtype;
	hi_ushort16 us_vlan;
}hi_vlanhdr_s;

typedef struct {
    hi_uchar8       uc_ihl:4,
                    uc_version:4;
    hi_uchar8       uc_tos;
    hi_ushort16     us_tot_len;
    hi_ushort16     us_id;
    hi_ushort16     us_frag_off;
    hi_uchar8       uc_ttl;
    hi_uchar8       uc_protocol;
    hi_ushort16     us_check;
    hi_uint32       ui_saddr;
    hi_uint32       ui_daddr;
    /*The options start here. */
}hi_iphdr_s;

typedef struct {
	hi_uchar8      uc_priority:4,
	               uc_version:4;
	hi_uchar8		 uc_flow_lbl[3];

	hi_ushort16	 us_payload_len;
	hi_uchar8		 uc_nexthdr;
	hi_uchar8		 uc_hop_limit;

	hi_in6_addr_s	 st_saddr;
	hi_in6_addr_s	 st_daddr;
} hi_ipv6hdr_s;

typedef struct {
	hi_uchar8      uc_nexthdr;
	hi_uchar8		 uc_extlen;
	hi_uchar8		 uc_rsv[2];
} hi_ipv6nxt_s;

typedef struct {
    hi_ushort16 us_hrd;		/* format of hardware address	*/
    hi_ushort16 us_pro;		/* format of protocol address	*/
    hi_uchar8   uc_hln;		/* length of hardware address	*/
    hi_uchar8   uc_pln;		/* length of protocol address	*/
    hi_ushort16 us_op;		/* ARP opcode (command)		*/
    hi_uchar8   uc_smac[HI_MAC_LEN];    /* 发送者硬件地址         */
    hi_uint32   ui_sip;                 /* 发送者IP地址           */
    hi_uchar8   uc_dmac[HI_MAC_LEN];    /* 目标机硬件地址         */
    hi_uint32   ui_dip;                 /* 目标机IP地址           */
}hi_arphdr_s;

typedef struct {
  hi_uchar8     uc_type;
  hi_uchar8     uc_code;
  hi_ushort16   us_checksum;
  union {
	struct {
		hi_ushort16	us_id;
		hi_ushort16	us_sequence;
	}echo;
	hi_uint32	ui_gateway;
	struct {
		hi_ushort16	us_unused;
		hi_ushort16	us_mtu;
	}frag;
  }un;
}hi_icmphdr_s;

typedef struct {
	hi_ushort16	us_source;
	hi_ushort16	us_dest;
	hi_ushort16	us_len;
	hi_ushort16	us_check;
}hi_udphdr_s;

typedef struct {
    hi_ushort16 us_source;
    hi_ushort16 us_dest;
    hi_uint32   ui_seq;
    hi_uint32   ui_ack_seq;
    union {
        struct {
            hi_uchar8   uc_rsv0:4,
                        uc_len :4;
        };
        hi_uchar8   uc_value1;
    };
    union {
        struct {
            hi_uchar8   uc_fin :1,
                        uc_syn :1,
                        uc_rst :1,
                        uc_psh :1,
                        uc_ack :1,
                        uc_urg :1,
                        uc_rsv1:2;
        };
        hi_uchar8   uc_value2;
    };
    hi_ushort16 us_window;
    hi_ushort16 us_check;
    hi_ushort16 us_urg_ptr;
}hi_tcphdr_s;

typedef struct {
    hi_uchar8 uc_type;
    hi_uchar8 uc_len;
    hi_ushort16 us_data;
}hi_tcpopt_s;

#pragma pack()

typedef struct {
    hi_uint32   ui_sip;
    hi_uint32   ui_dip;
    hi_uchar8   uc_pad;
    hi_uchar8   uc_proto;
    hi_ushort16 us_len;
}hi_pseudohdr_s;

typedef struct {
	hi_uint32	ui_s_addr;
}hi_in_addr_s;

typedef struct {
    hi_ushort16 us_sa_family; /* 地址族， AF_xxx */
    hi_uchar8   uc_sa_data[14]; /* 14 字节的协议地址 */ 
}hi_sockaddr_s;

typedef struct {
    hi_ushort16     us_sin_family; /* 地址族 */
    hi_ushort16     us_sin_port; /* 端口号 */
    hi_in_addr_s    st_sin_addr; /* IP地址 */
    hi_uchar8       uc_sin_zero[8]; /* 填充0 以保持与struct sockaddr同样大小 */
}hi_sockaddr_in_s;


typedef struct
{
    hi_uint32 ui_v4;
} hi_ipv4_s;

typedef struct
{
    hi_uchar8 uc_v6[16];
} hi_ipv6_s;

typedef union
{
    hi_ipv4_s  st_v4;
    hi_ipv6_s  st_v6;    
} hi_ip_addr_u; 


/* IP VERSION */
typedef enum
{
    HI_NOIP_E, 
    HI_IPV4_E, 
    HI_IPV6_E, 
    HI_IP_VER_CNT_E
} hi_ip_ver_e; 


typedef struct
{
    hi_ip_addr_u un_ip; 
    hi_ip_ver_e  em_ver; 
}hi_ip_addr_s; 


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_NETDEF_H__ */
