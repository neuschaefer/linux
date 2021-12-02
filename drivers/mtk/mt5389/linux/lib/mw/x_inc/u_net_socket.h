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
/*! @file u_net_socket.h
 * $RCSfile: $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * @par Description:
 *      network socket defines
 */
/*----------------------------------------------------------------------------*/
/*!
* @addtogroup  groupMW_INET
* @{
*/

#ifndef U_NET_SOCKET_H
#define U_NET_SOCKET_H

#ifdef _USE_LINUX
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
/*#include <sys/stat.h>*/
/*#include <sys/time.h>*/
#include <sys/select.h>
#include <sys/poll.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include <unistd.h>
/*#include <fcntl.h>*/

#include <errno.h>

#define SOCK_EBADF             EBADFD  /* which SOCK_EBADF is our nucleus and EBADFD is linux define */
#define SOCK_EFAULT            EFAULT
#define SOCK_EINVAL            EINVAL
#define SOCK_EMFILE            EMFILE
#define SOCK_ENOPROTOOPT       ENOPROTOOPT
#define SOCK_EPROTONOSUPPORT   EPROTONOSUPPORT
#define SOCK_EOPNOTSUPP        EOPNOTSUPP
#define SOCK_EAFNOSUPPORT      EAFNOSUPPORT
#define SOCK_EADDRINUSE        EADDRINUSE
#define SOCK_EADDRNOTAVAIL     EADDRNOTAVAIL
#define SOCK_ECONNABORTED      ECONNABORTED
#define SOCK_ECONNRESET        ECONNRESET
#define SOCK_ENOBUFS           ENOBUFS
#define SOCK_EISCONN           EISCONN
#define SOCK_ENOTCONN          ENOTCONN
#define SOCK_ECONNREFUSED      ECONNREFUSED
#define SOCK_EHOSTUNREACH      EHOSTUNREACH
#define SOCK_ETIMEDOUT         ETIMEDOUT
#define SOCK_EWOULDBLOCK       EWOULDBLOCK /* EAGAIN */
#define SOCK_EINPROGRESS       EINPROGRESS
#define SOCK_EALREADY          EALREADY

#define SOCK_HOSTNOTFOUND      HOST_NOT_FOUND
#define SOCK_NORECOVERY        NO_RECOVERY
#define SOCK_NOMEMORY          ENOMEM

#else

#include "u_net_common.h"
#include "time.h"

/*
 * Definitions of bits in internet address integers.
 * On subnets, the decomposition of addresses to host and net parts
 * is done according to subnet mask, not the masks here.
 */
#define IN_CLASSA(i)        (((UINT32)(i) & 0x80000000) == 0)           /**< internet address class A      */
#define IN_CLASSA_NET       0xff000000                                  /**< internet address class A net address  */
#define IN_CLASSA_NSHIFT    24                                          /**< internet address class A shift        */
#define IN_CLASSA_HOST      0x00ffffff                                  /**< internet address class A host address */
#define IN_CLASSA_MAX       128                                         /**< internet address class A max          */

#define IN_CLASSB(i)        (((UINT32)(i) & 0xc0000000) == 0x80000000)  /**< address class B      */
#define IN_CLASSB_NET       0xffff0000                                  /**< internet address class B net address  */
#define IN_CLASSB_NSHIFT    16                                          /**< internet address class B shift        */
#define IN_CLASSB_HOST      0x0000ffff                                  /**< internet address class B host address */
#define IN_CLASSB_MAX       65536                                       /**< internet address class B max          */

#define IN_CLASSC(i)        (((UINT32)(i) & 0xe0000000) == 0xc0000000)  /**< address class C      */
#define IN_CLASSC_NET       0xffffff00                                  /**< internet address class C net address  */
#define IN_CLASSC_NSHIFT    8                                           /**< internet address class C shift        */
#define IN_CLASSC_HOST      0x000000ff                                  /**< internet address class C host address */

#define IN_CLASSD(i)        (((UINT32)(i) & 0xf0000000) == 0xe0000000)  /**< address class D      */
#define IN_CLASSD_NET       0xf0000000  /**< internet address class D net address (These ones aren't really)  */
#define IN_CLASSD_NSHIFT    28          /**< internet address class D shift (net and host fields) */
#define IN_CLASSD_HOST      0x0fffffff  /**< internet address class D host address (routing needn't know) */
#define IN_MULTICAST(i)     IN_CLASSD(i)                                /**< internet address multicast address    */

#define IN_EXPERIMENTAL(i)  (((UINT32)(i) & 0xf0000000) == 0xf0000000)  /**< internet address experimental         */
#define IN_BADCLASS(i)      (((UINT32)(i) & 0xf0000000) == 0xf0000000)  /**< internet address bad class            */

#define INADDR_ANY          (UINT32)0x00000000                          /**< internet address any          */
#define INADDR_LOOPBACK     (UINT32)0x7f000001                          /**< loopback addr        */
#define INADDR_BROADCAST    (UINT32)0xffffffff  /**< broadcast addr (must be masked) */
#define INADDR_NONE         0xffffffff          /**< none address, -1 return         */

#define INADDR_UNSPEC_GROUP     (UINT32)0xe0000000  /**< internet address 224.0.0.0            */
#define INADDR_ALLHOSTS_GROUP   (UINT32)0xe0000001  /**< internet address 224.0.0.1            */
#define INADDR_ALLRTRS_GROUP    (UINT32)0xe0000002  /**< internet address 224.0.0.2            */
#define INADDR_MAX_LOCAL_GROUP  (UINT32)0xe00000ff  /**< internet address 224.0.0.255          */

#define IN_LOOPBACKNET      127          /**< loopback network address (official) */

/*
 * XPG4.2 states that inclusion of <netinet/in.h> must pull these
 * in and that inclusion of <sys/socket.h> must pull in sa_family_t.
 * We put there here because there are other headers that require
 * these types and <sys/socket.h> and <netinet/in.h> will indirectly
 * include <sys/types.h>.  Thus we are compliant without too many hoops.
 */
typedef unsigned int    in_addr_t;      /* base type for internet address */
typedef unsigned short  in_port_t;      /* IP port type */
typedef unsigned char   sa_family_t;    /* sockaddr address family type */
typedef unsigned int    socklen_t;      /* length type for network syscalls */

#ifndef __SSIZE_T_
#define __SSIZE_T_

typedef int ssize_t;

#endif

/*
 * Structure used by kernel to store most
 * addresses.
 */

/* Domain */
#define AF_INET     2                         /**< the address type for IPv4: UDP, TCP, etc. */
#define AF_INET6    24                        /**< the address type for IPv6 to indicate IPv6 */

/*
 * Types
 */
#define SOCK_STREAM     1                     /**< socket stream type */
#define SOCK_DGRAM      2                     /**< socket datagram type */
#define SOCK_RAW        3                     /**< socket raw-protocol interface type */
#define SOCK_RDM        4                     /**< socket reliably-delivered message */
#define SOCK_SEQPACKET  5                     /**< socket sequenced packet stream  */

/*
 * Protocols
 */
#define IPPROTO_IP            0                 /**< Standard well-defined IP protocols: dummy for IP */
#define IPPROTO_HOPOPTS       IPPROTO_IP        /**< Standard well-defined IP protocols: Hop-by-hop option header */
#define IPPROTO_ICMP          1                 /**< Standard well-defined IP protocols: control message protocol */
#define IPPROTO_IGMP          2                 /**< Standard well-defined IP protocols: group mgmt protocol */
#define IPPROTO_GGP           3                 /**< Standard well-defined IP protocols: gateway^2 (deprecated) */
#define IPPROTO_IPIP          4                 /**< Standard well-defined IP protocols: IP inside IP */
#define IPPROTO_IPV4          IPPROTO_IPIP      /**< Standard well-defined IP protocols: IP inside IP */
#define IPPROTO_TCP           6                 /**< Standard well-defined IP protocols: tcp */
#define IPPROTO_EGP           8                 /**< Standard well-defined IP protocols: exterior gateway protocol */
#define IPPROTO_PUP           12                /**< Standard well-defined IP protocols: pup */
#define IPPROTO_UDP           17                /**< Standard well-defined IP protocols: user datagram protocol */
#define IPPROTO_IDP           22                /**< Standard well-defined IP protocols: xns idp */
#define IPPROTO_TP            29                /**< Standard well-defined IP protocols: tp-4 w/ class negotiation */
#define IPPROTO_IPV6          41                /**< Standard well-defined IP protocols: IPv6 in IPv6 */
#define IPPROTO_ROUTING       43                /**< Standard well-defined IP protocols: Routing header */
#define IPPROTO_FRAGMENT      44                /**< Standard well-defined IP protocols: Fragmentation/reassembly header */
#define IPPROTO_RSVP          46                /**< Standard well-defined IP protocols: resource reservation */
#define IPPROTO_GRE           47                /**< Standard well-defined IP protocols: GRE encap, RFCs 1701/1702 */
#define IPPROTO_ESP           50                /**< Standard well-defined IP protocols: Encap. Security Payload */
#define IPPROTO_AH            51                /**< Standard well-defined IP protocols: Authentication header */
#define IPPROTO_MOBILE        55                /**< Standard well-defined IP protocols: IP Mobility, RFC 2004 */
#define IPPROTO_ICMPV6        58                /**< Standard well-defined IP protocols: ICMP for IPv6 */
#define IPPROTO_NONE          59                /**< Standard well-defined IP protocols: No next header */
#define IPPROTO_DSTOPTS       60                /**< Standard well-defined IP protocols: Destination options header */
#define IPPROTO_EON           80                /**< Standard well-defined IP protocols: ISO cnlp */
#define IPPROTO_ETHERIP       97                /**< Standard well-defined IP protocols: Ethernet in IPv4 */
#define IPPROTO_ENCAP         98                /**< Standard well-defined IP protocols: encapsulation header */
#define IPPROTO_PIM           103               /**< Standard well-defined IP protocols: Protocol indep. multicast */
#define IPPROTO_IPCOMP        108               /**< Standard well-defined IP protocols: IP Payload Comp. Protocol */
#define IPPROTO_CARP          112               /**< Standard well-defined IP protocols: CARP */
#define IPPROTO_PFSYNC        240               /**< Standard well-defined IP protocols: PFSYNC */
#define IPPROTO_RAW           255               /**< Standard well-defined IP protocols: raw IP packet */                    

#define IPPROTO_MAX   256

/* Flags we can use with send/ and recv.
 */
#define MSG_OOB             1               /**< snd/rcv flag OOB */
#define MSG_PEEK            2               /**< snd/rcv flag PEEK */
#define MSG_DONTROUTE       4               /**< snd/rcv flag DONTROUTE */
#define MSG_DONTWAIT        0x40            /**< snd/rcv flag Nonblocking io    */
#define MSG_WAITALL         0x100           /**< snd/rcv flag Wait for a full request */

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define SOL_SOCKET  0xffff      /**< options for socket level */

/*
 * Option flags per-socket.
 */
#define SO_DEBUG        0x0001    /**< turn on debugging info recording */
#define SO_ACCEPTCONN   0x0002    /**< socket has had listen() */
#define SO_REUSEADDR    0x0004    /**< allow local address reuse */
#define SO_KEEPALIVE    0x0008    /**< keep connections alive */
#define SO_DONTROUTE    0x0010    /**< just use interface addresses */
#define SO_BROADCAST    0x0020    /**< permit sending of broadcast msgs */
#define SO_USELOOPBACK  0x0040    /**< bypass hardware when possible */
#define SO_LINGER       0x0080    /**< linger on close if data present */
#define SO_OOBINLINE    0x0100    /**< leave received OOB data in line */
#define SO_REUSEPORT    0x0200    /**< allow local address & port reuse */
#define SO_JUMBO        0x0400    /**< try to use jumbograms */

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF       0x1001    /**< send buffer size */
#define SO_RCVBUF       0x1002    /**< receive buffer size */
#define SO_SNDLOWAT     0x1003    /**< send low-water mark */
#define SO_RCVLOWAT     0x1004    /**< receive low-water mark */
#define SO_SNDTIMEO     0x1005    /**< send timeout */
#define SO_RCVTIMEO     0x1006    /**< receive timeout */
#define SO_ERROR        0x1007    /**< get error status and clear */
#define SO_TYPE         0x1008    /**< get socket type */
#define SO_NETPROC      0x1020    /**< multiplex; network processing */

/*
 * Options for use with [gs]etsockopt at the IP level.
 * First word of comment is data type; bool is stored in int.
 */
#define IP_OPTIONS          1    /**< buf/ip_opts; set/get IP options */
#define IP_HDRINCL          2    /**< int; header is included with data */
#define IP_TOS              3    /**< int; IP type of service and preced. */
#define IP_TTL              4    /**< int; IP time to live */
#define IP_RECVOPTS         5    /**< bool; receive all IP opts w/dgram */
#define IP_RECVRETOPTS      6    /**< bool; receive IP opts for response */
#define IP_RECVDSTADDR      7    /**< bool; receive IP dst addr w/dgram */
#define IP_RETOPTS          8    /**< ip_opts; set/get IP options */
#define IP_MULTICAST_IF     9    /**< in_addr; set/get IP multicast i/f  */
#define IP_MULTICAST_TTL    10   /**< u_char; set/get IP multicast ttl */
#define IP_MULTICAST_LOOP   11   /**< u_char; set/get IP multicast loopback */
#define IP_ADD_MEMBERSHIP   12   /**< ip_mreq; add an IP group membership */
#define IP_DROP_MEMBERSHIP  13   /**< ip_mreq; drop an IP group membership */

/*
 * Options for use with [gs]etsockopt at the IPV6 level.
 * First word of comment is data type; bool is stored in int.
 */
#define IPV6_OPTIONS        1  /**< buf/ip6_opts; set/get IP6 options */
/* no hdrincl */
#define IPV6_SOCKOPT_RESERVED1  3  /**< reserved for future use */
#define IPV6_UNICAST_HOPS   4  /**< int; IP6 hops */
#define IPV6_RECVOPTS       5  /**< bool; receive all IP6 opts w/dgram */
#define IPV6_RECVRETOPTS    6  /**< bool; receive IP6 opts for response */
#define IPV6_RECVDSTADDR    7  /**< bool; receive IP6 dst addr w/dgram */
#define IPV6_RETOPTS        8  /**< ip6_opts; set/get IP6 options */
#define IPV6_MULTICAST_IF   9  /**< u_char; set/get IP6 multicast i/f  */
#define IPV6_MULTICAST_HOPS 10 /**< u_char; set/get IP6 multicast hops */
#define IPV6_MULTICAST_LOOP 11 /**< u_char; set/get IP6 multicast loopback */
#define IPV6_JOIN_GROUP     12 /**< ip6_mreq; join a group membership */
#define IPV6_LEAVE_GROUP    13 /**< ip6_mreq; leave a group membership */
#define IPV6_PORTRANGE      14 /**< int; range to choose for unspec port */
#define ICMP6_FILTER        18 /**< icmp6_filter; icmp6 filter */
#define IPV6_PKTINFO        19 /**< bool; send/rcv if, src/dst addr */
#define IPV6_HOPLIMIT       20 /**< bool; hop limit */
#define IPV6_NEXTHOP        21 /**< bool; next hop addr */
#define IPV6_HOPOPTS        22 /**< bool; hop-by-hop option */
#define IPV6_DSTOPTS        23 /**< bool; destination option */
#define IPV6_RTHDR      24 /**< bool; routing header */
#define IPV6_PKTOPTIONS     25 /**< buf/cmsghdr; set/get IPv6 options */
#define IPV6_CHECKSUM       26 /**< int; checksum offset for raw socket */
#define IPV6_V6ONLY     27 /**< bool; make AF_INET6 sockets v6 only */

/**
 * Poll event
 */
#define POLLIN      0x0001    /**< There is data to read */
#define POLLPRI     0x0002    /**< Not support */
#define POLLOUT     0x0004    /**< Writing now will not block */
#define POLLERR     0x0008    /**< Error condition */
#define POLLHUP     0x0010    /**< Hung up */
#define POLLNVAL    0x0020    /**< Invalid request: fd not open */

/**
 * fcntl flag
 */
#define O_NONBLOCK  0x0001   /**< fcntl flag indicate non blocking */

/**
 * fcntl cmd
 */
#define F_GETFD     0x0001   /**< fcntl cmd get */
#define F_SETFD     0x0002   /**< fcntl cmd set */

/**
 * Error code
 */

#define SOCK_EBADF             (-2)     /**< Bad file number */ 
#define SOCK_EFAULT            (-3)     /**< Bad address          */
#define SOCK_EINVAL            (-4)     /**< Invalid argument          */
#define SOCK_EMFILE            (-5)     /**< Too many open files          */
#define SOCK_ENOPROTOOPT       (-6)     /**< Protocol not available     */
#define SOCK_EPROTONOSUPPORT   (-7)     /**< Protocol not supported  */
#define SOCK_EOPNOTSUPP        (-8)     /**< Operation not supported on transport endpoint      */
#define SOCK_EAFNOSUPPORT      (-9)     /**< Address family not supported by protocol    */
#define SOCK_EADDRINUSE       (-10)     /**< Address already in use      */
#define SOCK_EADDRNOTAVAIL    (-11)     /**< Cannot assign requested address   */
#define SOCK_ECONNABORTED     (-12)     /**< Software caused connection abort    */
#define SOCK_ECONNRESET       (-13)     /**< Connection reset by peer      */
#define SOCK_ENOBUFS          (-14)     /**< No buffer space available         */
#define SOCK_EISCONN          (-15)     /**< Transport endpoint is already connected         */
#define SOCK_ENOTCONN         (-16)     /**< Transport endpoint is not connected        */
#define SOCK_ECONNREFUSED     (-17)     /**< Connection refused    */
#define SOCK_EHOSTUNREACH     (-18)     /**< No route to host    */
#define SOCK_ETIMEDOUT        (-19)     /**< Timer expired       */
#define SOCK_EWOULDBLOCK      (-20)     /**< Operation would block     */
#define SOCK_EINPROGRESS      (-21)     /**< Operation now in progress     */
#define SOCK_EALREADY         (-22)     /**< Operation already in progress */
                                                             
#define SOCK_HOSTNOTFOUND     (-23)     /**< host not found    */
#define SOCK_NORECOVERY       (-24)     /**< no recovery      */
#define SOCK_NOMEMORY         (-25)     /**< no memory        */

#define SOCK_EAGAIN           SOCK_EWOULDBLOCK      /**< Try again     */

/*
 * User-settable options (used with setsockopt).
 */
#define TCP_NODELAY      0x01   /**< don't delay send to coalesce pkts */
#define TCP_MAXSEG       0x02   /**< set maximum segment size */
#define TCP_KEEPALIVE  0x04    /**< send KEEPALIVE probes when idle for pcb->keepalive miliseconds */

/*
 * Structure used for manipulating linger option.
 */
/*!
 * @struct linger
 * @brief Structure used for manipulating linger option.
 * @code
 * struct linger
 * {
 *     int l_onoff;
 *     int l_linger;
 * };
 * @endcode
 *
 * @li@c l_onoff     - option on/off
 * @li@c l_linger    - linger time
 */
struct  linger {
    int l_onoff;        /* option on/off */
    int l_linger;       /* linger time */
};

/*!
 * @struct sockaddr
 * @brief socket address structure
 * @code
 * struct sockaddr
 * {
 *     u_char          sa_len;
 *     u_char          sa_family;
 *     char            sa_data[14];
 * };
 * @endcode
 *
 * @li@c sa_len     - total length
 * @li@c sa_family  - address family, AF_xxx
 * @li@c sa_data    - actually longer; address value
 */
struct sockaddr {
  u_char          sa_len;       /* total length */
  u_char          sa_family;    /* address family */
  char            sa_data[14];  /* actually longer; address value */
};

/*
 * IP Version 4 Internet address (a structure for historical reasons)
 */
/*!
 * @struct in_addr
 * @brief Internet address
 * @code
 * struct in_addr
 * {
 *     in_addr_t       s_addr;
 * };
 * @endcode
 *
 * @li@c s_addr     - address value
 */
struct in_addr {
  in_addr_t s_addr;
};

/*
 * IP Version 4 socket address.
 */
/*!
 * @struct sockaddr_in
 * @brief IPv4 socket address
 * @code
 * struct sockaddr_in
 * {
 *     u_char      sin_len;
 *     u_char      sin_family;
 *     u_short     sin_port;
 *     struct      in_addr sin_addr;
 *     char        sin_zero[8];
 * };
 * @endcode
 *
 * @li@c sin_len    - address length
 * @li@c sin_family - address family
 * @li@c sin_port   - port number
 * @li@c sin_addr   - internet address
 * @li@c sin_zero   - dummy data
 */
struct sockaddr_in {
  u_char      sin_len;
  u_char      sin_family;
  u_short     sin_port;
  struct      in_addr sin_addr;
  char        sin_zero[8];
};

/**
 * Multicast
 */
/*!
 * @struct ip_mreq
 * @brief Request struct for multicast socket ops
 * @code
 * struct ip_mreq
 * {
 *     struct in_addr imr_multiaddr;
 *     struct in_addr imr_interface;
 * };
 * @endcode
 *
 * @li@c imr_multiaddr   - IP multicast address of group 
 * @li@c imr_interface   - local IP address of interface
 */
struct ip_mreq
{
    struct in_addr imr_multiaddr;   /* IP multicast address of group */
    struct in_addr imr_interface;   /* local IP address of interface */
};

/**
 * poll struct
 */
/*!
 * @struct pollfd
 * @brief Data structure describing a polling request.
 * @code
 * struct pollfd
 * {
 *     int fd;
 *     short events;
 *     short revents;
 * };
 * @endcode
 *
 * @li@c fd          - file descriptor 
 * @li@c events      - requested events 
 * @li@c revents     - returned events
 */
struct pollfd {
    int fd;           /* file descriptor */
    short events;     /* requested events */
    short revents;    /* returned events */
};

/*
 * Select uses bit masks of file descriptors in longs.  These macros
 * manipulate such bit fields (the filesystem macros use chars).
 * FD_SETSIZE may be defined by the user, but the default here should
 * be enough for most uses.
 */
/* #define FD_SETSIZE  1024 */

#define FD_SETSIZE  120     /**< Maximum number of file descriptors in `fd_set'. */

#define NBBY    8               /**< number of bits in a byte */

typedef UINT32   FD_MASK;
#define NFDBITS (sizeof(FD_MASK) * NBBY)     /**< = 32, bits per mask */

#define _howmany(x, y)   (((x) + ((y) - 1)) / (y))
#define MT_FDS_BITS_SIZE  _howmany(FD_SETSIZE, NFDBITS) /**< word numbers */

/*!
 * @struct fd_set
 * @brief fd_set for select
 * @code
 * typedef struct fd_set_t
 * {
 *     FD_MASK fds_bits[MT_FDS_BITS_SIZE];
 * } fd_set;
 * @endcode
 *
 * @li@c fds_bits       - bits for FD 
 */
typedef struct fd_set_t {
    FD_MASK fds_bits[MT_FDS_BITS_SIZE];
} fd_set;

#define FDSET_MASK(n)  ((FD_MASK)1 << ((n) % NFDBITS))
#define FD_SET(n, p)    ((p)->fds_bits[(n)/NFDBITS] |= FDSET_MASK(n))   /**< add a given FD from a set */
#define FD_CLR(n, p)    ((p)->fds_bits[(n)/NFDBITS] &= ~FDSET_MASK(n))  /**< remove a given FD from a set */
#define FD_ISSET(n, p)  ((p)->fds_bits[(n)/NFDBITS] & FDSET_MASK(n))    /**< test to see a FD is part of the set */
#define FD_ZERO(p)      x_memset(p, 0, sizeof(*(p)))                    /**< clear a set */

/*
 * For shutdown API
 * These are the valid values for the "how" field used by shutdown(2).
 */
#define SHUT_RD         0   /**< No more receptions */
#define SHUT_WR         1   /**< No more transmissions. */
#define SHUT_RDWR       2   /**< No more receptions or transmissions. */

/* data type for
 *      gethostbyname
 *      gethostbyaddr
*/
/*!
 * @struct hostent
 * @brief Description of data base entry for a single host.
 * @code
 * struct hostent
 * {
 *     char  *h_name;
 *     char **h_aliases;
 *     int    h_addrtype;
 *     int    h_length;
 *     char **h_addr_list;
 * };
 * @endcode
 *
 * @li@c h_name         - Official name of host
 * @li@c h_aliases      - Alias list
 * @li@c h_addrtype     - Host address type
 * @li@c h_length       - Length of address
 * @li@c h_addr_list    - List of addresses from name server
 */
struct hostent
{
    char  *h_name;
    char **h_aliases;
    int    h_addrtype;
    int    h_length;
    char **h_addr_list;
};

#define h_addr h_addr_list[0] /**< Address, for backward compatibility, the first address in list. */

/* ioctl command */
#define FIONREAD    0       /**< cmd for ioctl */

#endif /* _USE_LINUX */

#endif

/*! @} */

