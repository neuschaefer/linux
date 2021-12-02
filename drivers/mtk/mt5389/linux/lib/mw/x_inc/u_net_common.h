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
/*! @file u_net_common.h
 * $RCSfile: $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * @par Description:
 *      network common type or value
 */
/*----------------------------------------------------------------------------*/
/*!
* @addtogroup  groupMW_INET
* @{
*/

#ifndef U_NET_COMMON_H
#define U_NET_COMMON_H

#include "u_common.h"

/* BSD-style types */
#ifndef ANDROID
#ifndef __KERNEL__
typedef unsigned char       u_char;
typedef unsigned short      u_short;
typedef unsigned int        u_int;
typedef unsigned long       u_long;

typedef signed char         int8_t;
typedef unsigned char       u_int8_t;
typedef unsigned char       uint8_t;
typedef short               int16_t;
typedef unsigned short      u_int16_t;
typedef unsigned short      uint16_t;
typedef int                 int32_t;
typedef unsigned int        u_int32_t;
typedef unsigned int        uint32_t;
typedef long long           int64_t;
typedef unsigned long long  uint64_t;
#endif
#endif

/* no monitor notify */
/*!
 * @enum NI_DRV_EV_T
 * @brief network interface state changing notify event
 * @code
 * typedef enum
 * {
 *     NI_DRV_EV_UNKNOW,
 *     NI_DRV_EV_ETHERNET_PLUGIN,
 *     NI_DRV_EV_ETHERNET_UNPLUG,
 *     NI_DRV_EV_IP_ADDRESS_CHANGED,
 *     NI_DRV_EV_IP_ADDRESS_EXPIRED
 * } NI_DRV_EV_T;
 * @endcode
 *
 * @li@c NI_DRV_EV_UNKNOW               - unknown event
 * @li@c NI_DRV_EV_ETHERNET_PLUGIN      - ethernet plugin
 * @li@c NI_DRV_EV_ETHERNET_UNPLUG      - ethernet unplug
 * @li@c NI_DRV_EV_IP_ADDRESS_CHANGED   - ip address changed
 * @li@c NI_DRV_EV_IP_ADDRESS_EXPIRED   - ip address expired
 */
typedef enum
{
    NI_DRV_EV_UNKNOW,
    /* ethernet */
    NI_DRV_EV_ETHERNET_PLUGIN,
    NI_DRV_EV_ETHERNET_UNPLUG,

    /* wlan */
    NI_DRV_EV_WLAN_ASSOC = NI_DRV_EV_ETHERNET_PLUGIN,
    NI_DRV_EV_WLAN_DISASSOC = NI_DRV_EV_ETHERNET_UNPLUG,

    /* ip0 */
    NI_DRV_EV_IP_ADDRESS_CHANGED,
    NI_DRV_EV_IP_ADDRESS_EXPIRED
} NI_DRV_EV_T;

/**
 * NiHal retuen code
 * Network Interface Hardware Abstract Layer Interface
 */
/*!
 * @brief network common return value
 * @code
 * enum
 * {
 *     NET_OK = 0,
 *     NET_FAILED = -65535,
 *     NET_CREATE_THREAD_FAILED,
 *     NET_INITIATED,
 *     NET_NOT_INITIATED,
 *     NET_INV_ARG,
 *     NET_IP_STACK_ERROR,
 *     NET_DRIVER_ERROR
 * };
 * @endcode
 *
 * @li@c NET_OK                     - OK
 * @li@c NET_FAILED                 - failed
 * @li@c NET_CREATE_THREAD_FAILED   - create thread failed
 * @li@c NET_INITIATED              - initiated
 * @li@c NET_NOT_INITIATED          - not initiated
 * @li@c NET_INV_ARG                - invalid argument
 * @li@c NET_IP_STACK_ERROR         - IP stack error
 * @li@c NET_DRIVER_ERROR           - driver error
 */
enum
{
    NET_OK = 0,
    NET_FAILED = -65535,
    NET_CREATE_THREAD_FAILED,
    NET_INITIATED,
    NET_NOT_INITIATED,
    NET_INV_ARG,
    NET_IP_STACK_ERROR,
    NET_DRIVER_ERROR
};

/*!
 * @brief ni return value
 * @code
 * enum
 * {
 *     NET_NI_OK = 0,
 *     NET_NI_ERROR = -65535,
 *     NET_NI_INV_ARG,
 *     NET_NI_NOT_EXIST,
 *     NET_NI_ALLOC_MEM_FAILED,
 *     NET_NI_MSG_QUEUE_FAILED
 * };
 * @endcode
 *
 * @li@c NET_NI_OK                  - OK
 * @li@c NET_NI_ERROR               - error
 * @li@c NET_NI_INV_ARG             - invalid argument
 * @li@c NET_NI_NOT_EXIST           - network interface not exist
 * @li@c NET_NI_ALLOC_MEM_FAILED    - out of memory
 * @li@c NET_NI_MSG_QUEUE_FAILED    - ni msg queue failed
 */
enum
{
    NET_NI_OK = 0,
    NET_NI_ERROR = -65535,
    NET_NI_INV_ARG,
    NET_NI_NOT_EXIST,
    NET_NI_ALLOC_MEM_FAILED,
    NET_NI_MSG_QUEUE_FAILED
};

/**
 * DrvHal return code
 */
/*!
 * @enum INET_DRV_RET_CODE_T
 * @brief driver return value
 * @code
 * enum 
 * {
 *     NET_DRV_OK = 0,
 *     NET_DRV_ERROR = -65535,
 *     NET_DRV_INV_ARG,
 *     NET_DRV_INV_OID,
 *     NET_DRV_PKT_ERROR,
 *     NET_DRV_PKT_TOO_SMALL,
 *     NET_DRV_PKT_TOO_BIG,
 *     NET_DRV_HW_ERROR,
 *     NET_DRV_NOT_INIT,
 *     NET_DRV_NOT_START,
 *     NET_DRV_ALREADY_INIT,
 *     NET_DRV_ALREADY_START,
 *     NET_DRV_ALREADY_STOP,
 *     NET_DRV_NAME_DUPLICATE,
 *     NET_DRV_REG_FULL,
 *     NET_DRV_PKT_BUFF_ERROR,
 *     NET_DRV_NO_RESOURCE,
 *     NET_DRV_NO_ASSOCICATION
 * };
 * @endcode
 *
 * @li@c NET_DRV_OK                 - OK
 * @li@c NET_DRV_ERROR              - error
 * @li@c NET_DRV_INV_ARG            - invalid argument
 * @li@c NET_DRV_INV_OID            - invalid OID value
 * @li@c NET_DRV_PKT_ERROR          - packet error
 * @li@c NET_DRV_PKT_TOO_SMALL      - packet too small
 * @li@c NET_DRV_PKT_TOO_BIG        - packet too big
 * @li@c NET_DRV_HW_ERROR           - hardware error
 * @li@c NET_DRV_NOT_INIT           - not init
 * @li@c NET_DRV_NOT_START          - not start
 * @li@c NET_DRV_ALREADY_INIT       - already init
 * @li@c NET_DRV_ALREADY_START      - already start
 * @li@c NET_DRV_ALREADY_STOP       - already stop
 * @li@c NET_DRV_NAME_DUPLICATE     - name duplicate
 * @li@c NET_DRV_REG_FULL           - reg full
 * @li@c NET_DRV_PKT_BUFF_ERROR     - packet buffer error
 * @li@c NET_DRV_NO_RESOURCE        - no resouce
 * @li@c NET_DRV_NO_ASSOCICATION    - no assocication
 */
enum 
{
    NET_DRV_OK = 0,
    NET_DRV_ERROR = -65535,
    NET_DRV_INV_ARG,
    NET_DRV_INV_OID,
    NET_DRV_PKT_ERROR,
    NET_DRV_PKT_TOO_SMALL,
    NET_DRV_PKT_TOO_BIG,
    NET_DRV_HW_ERROR,
    NET_DRV_NOT_INIT,
    NET_DRV_NOT_START,
    NET_DRV_ALREADY_INIT,
    NET_DRV_ALREADY_START,
    NET_DRV_ALREADY_STOP,
    NET_DRV_NAME_DUPLICATE,
    NET_DRV_REG_FULL,
    NET_DRV_PKT_BUFF_ERROR,
    NET_DRV_NO_RESOURCE,
    NET_DRV_NO_ASSOCICATION
};

/* Layer 2 switch return code */
/*!
 * @brief switch return value
 * @code
 * enum
 * {
 *     NET_SWITCH_OK = 0,
 *     NET_SWITCH_INVALID = -65535,
 *     NET_SWITCH_MALLOC_FAILED
 * };
 * @endcode
 *
 * @li@c NET_SWITCH_OK              - OK
 * @li@c NET_SWITCH_INVALID         - switch invalid
 * @li@c NET_SWITCH_MALLOC_FAILED   - out of memory
 */
enum
{
    NET_SWITCH_OK = 0,
    NET_SWITCH_INVALID = -65535,
    NET_SWITCH_MALLOC_FAILED
};

/**
 * Loopback
 */
#define NET_INADDR_LOOPBACK         ((UINT32) 0x7f000001)   /**< loopback address (network order/big-endian) */

/**
 * Packet header checksum indicator
 */
/*!
 * @enum NET_CHKSUM_FLAG_T
 * @brief network check-sum flag
 * @code
 * typedef enum
 * {
 *     CHKSUM_NONE = 0,
 *     CHKSUM_HW,
 *     CHKSUM_UNNECESSARY
 * } NET_CHKSUM_FLAG_T;
 * @endcode
 *
 * @li@c CHKSUM_NONE        - no checksum 
 * @li@c CHKSUM_HW          - checksum hardware
 * @li@c CHKSUM_UNNECESSARY - unnecessary checksum
 */
typedef enum
{
    CHKSUM_NONE = 0,
    CHKSUM_HW,
    CHKSUM_UNNECESSARY
} NET_CHKSUM_FLAG_T;

/**
 *
  ifOperStatus, base on RFC 2233
  The current operational state of the interface.
  The testing(3) state indicates that no operational packets can be passed.
  If ifAdminStatus is down(2) then ifOperStatus should be down(2).
  If ifAdminStatus is changed to up(1)
  then ifOperStatus should change to up(1) if the interface is ready to transmit and
  receive network traffic; it should change to dormant(5) if the interface is
  waiting for external actions (such as a serial line waiting for an incoming
  connection); it should remain in the down(2) state if and only if there is a
  fault that prevents it from going to the up(1) state; it should remain in the
  notPresent(6) state if the interface has missing (typically, hardware) components.
 */
#define NET_IF_STATE_UP                 ((INT32) 1)     /**< interface UP               */ 
#define NET_IF_STATE_DOWN               ((INT32) 2)     /**< interface DOWN             */
#define NET_IF_STATE_TESTING            ((INT32) 3)     /**< interface testing          */
#define NET_IF_STATE_UNKNOWN            ((INT32) 4)     /**< interface unknown          */
#define NET_IF_STATE_DORMANT            ((INT32) 5)     /**< interface dormant          */
#define NET_IF_STATE_NOTPRESENT         ((INT32) 6)     /**< interface notpresent       */
#define NET_IF_STATE_LOWERLAYERDOWN     ((INT32) 7)     /**< interface lower layer down */

/**
 * Common type define
 * MAC_ADDRESS_T
 */
typedef UCHAR MAC_ADDRESS_T[6];

/*!
 * @struct NET_PHY_ADDRESS_T
 * @brief hold hardware address
 * @code
 * typedef struct _NET_PHY_ADDRESS_S
 * {
 *     UINT32                  ui4_SetToFlash;
 *     MAC_ADDRESS_T           t_MacAddress;
 * }   NET_PHY_ADDRESS_T;
 * @endcode
 *
 * @li@c ui4_SetToFlash     - indicate write to flash or not (0: no write; 1: need write)
 * @li@c t_MacAddress       - mac address array
 */
typedef struct _NET_PHY_ADDRESS_S
{
    UINT32                  ui4_SetToFlash;  /* TRUE: set to flash and register, FALSE: set to register only */
    MAC_ADDRESS_T           t_MacAddress;
}   NET_PHY_ADDRESS_T;

/**
 * Frame Checksum
 */
/*!
 * @struct NET_CHKSUM_T
 * @brief hold hardware address
 * @code
 * typedef struct _HW_FRAME_CHKSUM_S
 * {
 *     NET_CHKSUM_FLAG_T       e_chksum_flag;
 *     BOOL                    b_ip_header_chksum;
 *     UINT16                  ui2_ip_payload_chksum;
 * }   NET_CHKSUM_T;
 * @endcode
 * @li@c e_chksum_flag          - checksum flag
 * @li@c b_ip_header_chksum     - ip header checksum
 * @li@c ui2_ip_payload_chksum  - ip payload checksum
 */

typedef struct _HW_FRAME_CHKSUM_S
{
    NET_CHKSUM_FLAG_T       e_chksum_flag;
    BOOL                    b_ip_header_chksum;
    UINT16                  ui2_ip_payload_chksum;
}   NET_CHKSUM_T;


#ifndef LINUX_TURNKEY_SOLUTION
#ifndef _USE_LINUX
/* define byte endian */
#ifdef _CPU_LITTLE_ENDIAN_
    #define htons(s) \
        (UINT16)((((UINT16)(s)<<8)&0xff00U)|(((UINT16)(s)>>8)&0x00ffU))

    #define htonl(l) \
        (UINT32)((((UINT32)(l)>>24)&0x00ffU)|(((UINT32)(l)>>8)&0xff00U) \
        |(((UINT32)(l)<<8)&0xff0000U)|(((UINT32)(l)<<24)&0xff000000U))

#else
    #define htons(s) (s)
    #define htonl(l) (l)
#endif

#define ntohs(s) htons(s)
#define ntohl(l) htonl(l)
#endif
#endif /* LINUX_TURNKEY_SOLUTION*/

/* MT_DHCP4_INFO_T.i2_status */
#define X_DHCP4_ZERO             (0)    /**< DHCP 0     */
#define X_DHCP4_STAT_INIT        (1)    /**< DHCP init  */
#define X_DHCP4_STAT_SELECTING   (2)    /**< DHCP selecting  */
#define X_DHCP4_STAT_REQUESTING  (3)    /**< DHCP requesting */
#define X_DHCP4_STAT_BOUND       (4)    /**< DHCP bound      */
#define X_DHCP4_STAT_RENEWING    (5)    /**< DHCP renewing   */
#define X_DHCP4_STAT_REBINDING   (6)    /**< DHCP rebinding  */
#define X_DHCP4_STAT_ERR         (7)    /**< DHCP error      */

/* MT_DHCP4_INFO_T.i2_lastevent or
 * MT_DHCPC_MSG_T.i4_event */
#define X_DHCP4_EVENT_RECV_OFFER     (1)    /**< DHCP recv offer    */
#define X_DHCP4_EVENT_RECV_ACK       (2)    /**< DHCP recv ack      */
#define X_DHCP4_EVENT_RECV_NAK       (3)    /**< DHCP recv nak      */
#define X_DHCP4_EVENT_SEND_BOUNDREQ  (4)    /**< DHCP send boundreq */
#define X_DHCP4_EVENT_SEND_RELEASE   (5)    /**< DHCP send release  */
#define X_DHCP4_EVENT_SEND_ERR       (6)    /**< DHCP send error    */
#define X_DHCP4_EVENT_CHANGE_INFO    (7)    /**< DHCP change info   */
#define X_DHCP4_EVENT_NOT_YET        (8)    /**< DHCP not yet       */
#define X_DHCP4_EVENT_TIMEOUT        (9)    /**< DHCP timeout       */

#define MAX_DHCP4_DOMAIN_LENGTH  (256)      /**< max dhcp domain length */

/*!
 * @struct MT_DHCP4_INFO_S
 * @brief DHCP information
 * @code
 * typedef struct _MT_DHCP4_INFO_S
 * {
 *     INT16   i2_status;            
 *     INT16   i2_lastevent;         
 *     UINT32  ui4_get_time;         
 *     UINT32  ui4_remain_renewal;   
 *     UINT32  ui4_remain_rebinding; 
 *     UINT32  ui4_remain_lease;     
 *     UINT32  ui4_renewal;          
 *     UINT32  ui4_rebinding;        
 *     UINT32  ui4_lease;            
 *     UINT32  ui4_server;           
 *     UINT32  ui4_ipaddr;           
 *     UINT32  ui4_subnet;           
 *     UINT32  ui4_router;           
 *     UINT32  ui4_dns1;             
 *     UINT32  ui4_dns2;             
 *     CHAR    ac_domain[MAX_DHCP4_DOMAIN_LENGTH];
 *     INT16   i2_domain_len;
 *     UINT16  ui2_change_flag;
 * } MT_DHCP4_INFO_T;
 * @endcode
 *
 * @li@c i2_status                           - DHCP Status 
 * @li@c i2_lastevent                        - occurd last event 
 * @li@c ui4_get_time                        - system time of receiving DHCPACK 
 * @li@c ui4_remain_renewal                  - remain time of renewal   
 * @li@c ui4_remain_rebinding                - remain time of rebinding 
 * @li@c ui4_remain_lease                    - remain time of lease     
 * @li@c ui4_renewal                         - renewal time   
 * @li@c ui4_rebinding                       - rebinding time 
 * @li@c ui4_lease                           - lease time     
 * @li@c ui4_server                          - DHCP server IP address 
 * @li@c ui4_ipaddr                          - My IP address 
 * @li@c ui4_subnet                          - IP subnet address 
 * @li@c ui4_router                          - default router IP address 
 * @li@c ui4_dns1                            - primary DNS IP address 
 * @li@c ui4_dns2                            - secondary DNS IP address 
 * @li@c ac_domain[MAX_DHCP4_DOMAIN_LENGTH]  - domain name 
 * @li@c i2_domain_len                       - domain length
 * @li@c ui2_change_flag                     - change flag
 */
typedef struct _MT_DHCP4_INFO_S
{
    INT16   i2_status;             /* DHCP Status */
    INT16   i2_lastevent;          /* occurd last event */
    UINT32  ui4_get_time;          /* system time of receiving DHCPACK */
    UINT32  ui4_remain_renewal;    /* remain time of renewal   */
    UINT32  ui4_remain_rebinding;  /* remain time of rebinding */
    UINT32  ui4_remain_lease;      /* remain time of lease     */
    UINT32  ui4_renewal;           /* renewal time   */
    UINT32  ui4_rebinding;         /* rebinding time */
    UINT32  ui4_lease;             /* lease time     */
    UINT32  ui4_server;            /* DHCP server IP address */
    UINT32  ui4_ipaddr;            /* My IP address */
    UINT32  ui4_subnet;            /* IP subnet address */
    UINT32  ui4_router;            /* default router IP address */
    UINT32  ui4_dns1;              /* primary DNS IP address */
    UINT32  ui4_dns2;              /* secondary DNS IP address */
    CHAR    ac_domain[MAX_DHCP4_DOMAIN_LENGTH];  /* domain name */
    INT16   i2_domain_len;
    UINT16  ui2_change_flag;
} MT_DHCP4_INFO_T;

/*!
 * @struct MT_NET_CFG_DEV_INFO_S
 * @brief network config device info
 * @code
 * typedef struct _MT_NET_CFG_DEV_INFO_S
 * {
 * 	INT32   i4_ifType;
 * 	INT32   i4_status;
 *     UINT64  ui8_ifSpeed;
 * 	CHAR    sz_ifName[8];
 *     CHAR    ac_ifPhysAddress[6];
 * } MT_NET_CFG_DEV_INFO;
 * @endcode
 * 
 * @li@c i4_ifType              - interface type
 * @li@c i4_status              - status
 * @li@c ui8_ifSpeed            - interface speed
 * @li@c sz_ifName[8]           - interface name
 * @li@c ac_ifPhysAddress[6]    - hardware (MAC) address
 */
typedef struct _MT_NET_CFG_DEV_INFO_S
{
	INT32   i4_ifType;
	INT32   i4_status;
    UINT64  ui8_ifSpeed;
	CHAR    sz_ifName[8];
    CHAR    ac_ifPhysAddress[6];
} MT_NET_CFG_DEV_INFO;

/*!
 * @struct MT_NET_CFG_DEV_LIST_S
 * @brief network config device list
 * @code
 * typedef struct _MT_NET_CFG_DEV_LIST_S
 * {
 *     UINT32 ui4_num;
 *     MT_NET_CFG_DEV_INFO *pt_dev_list;
 * } MT_NET_CFG_DEV_LIST;
 * @endcode
 *
 * @li@c ui4_num        - list number
 * @li@c pt_dev_list    - device list
 */
typedef struct _MT_NET_CFG_DEV_LIST_S
{
    UINT32 ui4_num;
    MT_NET_CFG_DEV_INFO *pt_dev_list;
} MT_NET_CFG_DEV_LIST;

/*!
 * @enum NET_COND_T
 * @brief network condition notify event
 * @code
 * typedef enum
 * {
 *     NET_COND_STARTED
 * } NET_COND_T;
 * @endcode
 *
 * @li@c NET_COND_STARTED               - network started
 */
typedef enum
{
    NET_COND_STARTED = 0,
} NET_COND_T;

/*!
 * @brief network condition client callback function
 * @param in ui4_tag   - callback tag
 * @param in e_cond    - condition
 */
typedef INT32 (*x_network_status_nfy)(UINT32 ui4_tag,  NET_COND_T e_cond);

#endif

/*! @} */

