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
/*! @file u_net_ni_if.h
 * $RCSfile: $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * @par Description:
 *      dhcp client
 */
/*----------------------------------------------------------------------------*/
/*!
* @addtogroup  groupMW_INET
* @{
*/

#ifndef U_NET_NI_IF_H
#define U_NET_NI_IF_H

#include "u_net_common.h"
#include "u_net_drv_if.h"

#define MAX_NI_INTERFACE    16   /**< maximum network interface support in system */
#define MAX_LEARNING_ENTRY  16   /**< maximum learning entry support in learning table */

#define MAX_IP_INSTANCE     4    /**< maximum IP instance number       */

/**
 * Interface define
 * Note,
 *      The maximum length of type is 4 bytes.
 */
#define NI_UNKNOW      "uwn"                            /**< unknown device */
#define NI_LOOPBACK    "l0"     /**< software loopback device */
#define NI_ETHERNET_0  "eth0"   /**< eth0, ethernet device 0 */
#define NI_ETHERNET_1  "eth1"   /**< eth1, ethernet device 1*/
#define NI_ETHERNET_2  "eth2"   /**< eth2, ethernet device 2*/
#define NI_ETHERNET_3  "eth3"   /**< eth3, ethernet device 3*/
#define NI_USB_0       "usb0"   /**< usb0, usb device 0*/
#define NI_USB_1       "usb1"   /**< usb1, usb device 1*/
#define NI_USB_2       "usb2"   /**< usb2, usb device 2*/
#define NI_USB_3       "usb3"   /**< usb3, usb device 3*/
#define NI_WIRELESS_0  "wifi0"  /**< wifi0, wireless device 0*/
#define NI_WIRELESS_1  "wifi1"  /**< wifi1, wireless device 1*/
#define NI_WIRELESS_2  "wifi2"  /**< wifi2, wireless device 2*/
#define NI_WIRELESS_3  "wifi3"  /**< wifi3, wireless device 3*/
#define NI_IP_0        "ip0"    /**< ip0, ip device 0*/
#define NI_IP_1        "ip1"    /**< ip1, ip device 1*/
#define NI_IP_2        "ip2"    /**< ip2, ip device 2*/
#define NI_IP_3        "ip3"    /**< ip3, ip device 3*/

/* Forward declare */
typedef struct IF_TABLE_S       IF_TABLE_T;
typedef struct NET_NI_S         NET_NI_T;
typedef struct LearningEntry_S  LearningTable_T;
typedef struct L2SWITCH_S       L2_SWITCH_T;

/* following type is defined in http://www.iana.org/assignments/ianaiftype-mib */
#define IF_TYPE_OTHER              1                    /**< interface type other */
#define IF_TYPE_ETHERNET           6                    /**< interface type ethernet   */
#define IF_TYPE_PPP                23                   /**< interface type ppp        */
#define IF_TYPE_SOFTWARE_LOOPBACK  24                   /**< interface type loopback   */
#define IF_TYPE_MODEM              48                   /**< interface type modem      */
#define IF_TYPE_IEEE80211          71                   /**< interface type ieee802.11 (wireless) */

/* Our internal type */
#define IF_TYPE_IP              32768                   /**< internal: type ip (interface type ip) */

/* notify api */
/*!
 * @brief network interface state change callback
 * @param in e_ev   - state event
 */
typedef VOID (*ni_ev_notify_fnct) (NI_DRV_EV_T e_ev);

/**
 * interface information
 */
/*!
 * @struct IF_TABLE_S
 * @brief network interface type
 * @code
 * struct IF_TABLE_S
 * {
 *     INT32   i4_ifIndex;
 *     INT32   i4_ifType;
 *     INT32   i4_ifMtu;
 *     UINT64  ui8_ifSpeed;
 *     INT32   i4_ifOperStatus;
 *     UINT32  ui4_ifLastChange;
 *     INT32   i4_ifLinkUpDownTrapEnable;
 *     INT32   i4_ifPromiscuousMode;
 *     UINT64  ui8_ifInOctets;
 *     UINT64  ui8_ifInUcastPkts;
 *     UINT32  ui4_ifInDiscards;
 *     UINT32  ui4_ifInErrors;
 *     UINT32  ui4_ifInUnknownProtos;
 *     UINT64  ui8_ifOutOctets;
 *     UINT64  ui8_ifOutUcastPkts;
 *     UINT32  ui4_ifOutDiscards;
 *     UINT32  ui4_ifOutErrors;
 *     UINT64  ui8_ifInMulticastPkts;
 *     UINT64  ui8_ifInBroadcastPkts;
 *     UINT64  ui8_ifOutMulticastPkts;
 *     UINT64  ui8_ifOutBroadcastPkts;
 *     INT32   i4_ip_version;
 *     union   {
 *         UCHAR   uac_ipv4[4];
 *         UINT32  ui4_ipv4;
 *         UCHAR   uac_ipv6[16];
 *     }ip;
 *     union   {
 *         UCHAR   uac_v4[4];
 *         UINT32  ui4_v4;
 *         UCHAR   uac_v6[16];
 *     }mask;
 *     union   {
 *         UCHAR   uac_v4[4];
 *         UINT32  ui4_v4;
 *         UCHAR   uac_v6[16];
 *     }gateway;
 *     INT32   i4_dhcpc_handle;
 *     CHAR    sz_ifDescr[256];
 *     CHAR    sz_ifName[8];
 *     CHAR    ac_ifPhysAddress[6];
 * };
 * @endcode
 *
 * @li@c i4_ifIndex                 - interface index
 * @li@c i4_ifType                  - interface type
 * @li@c i4_ifMtu                   - MTU
 * @li@c ui8_ifSpeed                - interface speed
 * @li@c i4_ifOperStatus            - operation status
 * @li@c ui4_ifLastChange           - last change
 * @li@c i4_ifLinkUpDownTrapEnable  - link up/down trap enable
 * @li@c i4_ifPromiscuousMode       - promiscuous mode
 * @li@c ui8_ifInOctets             - in octets
 * @li@c ui8_ifInUcastPkts          - in unicast packets
 * @li@c ui4_ifInDiscards           - in discards
 * @li@c ui4_ifInErrors             - in errors
 * @li@c ui4_ifInUnknownProtos      - in unknown protocols
 * @li@c ui8_ifOutOctets            - out octets
 * @li@c ui8_ifOutUcastPkts         - out unicast packets
 * @li@c ui4_ifOutDiscards          - out discards
 * @li@c ui4_ifOutErrors            - out errors
 * @li@c ui8_ifInMulticastPkts      - in multicast packets
 * @li@c ui8_ifInBroadcastPkts      - in broadcast packets
 * @li@c ui8_ifOutMulticastPkts     - out multicast packets
 * @li@c ui8_ifOutBroadcastPkts     - out broadcast packets
 * @li@c i4_ip_version              - ip version
 * @li@c ip.uac_ipv4[4]             - ip address 
 * @li@c ip.ui4_ipv4                - ip address v4
 * @li@c ip.uac_ipv6[16]            - ip address v6
 * @li@c mask.uac_v4[4]             - netmask 
 * @li@c mask.ui4_v4                - netmask v4
 * @li@c mask.uac_v6[16]            - netmask v6
 * @li@c gateway.uac_v4[4]          - gateway 
 * @li@c gateway.ui4_v4             - gateway v4
 * @li@c gateway.uac_v6[16]         - gateway v6
 * @li@c i4_dhcpc_handle            - dhcp handle
 * @li@c sz_ifDescr[256]            - interface description
 * @li@c sz_ifName[8]               - interface name
 * @li@c ac_ifPhysAddress[6]        - interface hardware address
 */
struct IF_TABLE_S
{
    INT32   i4_ifIndex;
    INT32   i4_ifType;
    INT32   i4_ifMtu;
    UINT64  ui8_ifSpeed;
    INT32   i4_ifOperStatus;
    UINT32  ui4_ifLastChange;
    INT32   i4_ifLinkUpDownTrapEnable;
    INT32   i4_ifPromiscuousMode;
    UINT64  ui8_ifInOctets;
    UINT64  ui8_ifInUcastPkts;
    UINT32  ui4_ifInDiscards;
    UINT32  ui4_ifInErrors;
    UINT32  ui4_ifInUnknownProtos;
    UINT64  ui8_ifOutOctets;
    UINT64  ui8_ifOutUcastPkts;
    UINT32  ui4_ifOutDiscards;
    UINT32  ui4_ifOutErrors;
    UINT64  ui8_ifInMulticastPkts;
    UINT64  ui8_ifInBroadcastPkts;
    UINT64  ui8_ifOutMulticastPkts;
    UINT64  ui8_ifOutBroadcastPkts;

    /* IP address */
    INT32   i4_ip_version;
    union   {
        UCHAR   uac_ipv4[4];
        UINT32  ui4_ipv4;
        UCHAR   uac_ipv6[16];
    }ip;

    union   {
        UCHAR   uac_v4[4];
        UINT32  ui4_v4;
        UCHAR   uac_v6[16];
    }mask;

    union   {
        UCHAR   uac_v4[4];
        UINT32  ui4_v4;
        UCHAR   uac_v6[16];
    }gateway;

    INT32   i4_dhcpc_handle;
    CHAR    sz_ifDescr[256];
    CHAR    sz_ifName[8];
    CHAR    ac_ifPhysAddress[6];
};

/**
 *  Layer 2 switch to dispatch packet to proper network device
 */
/*!
 * @struct LearningEntry_S
 * @brief learning entry
 * @code
 * struct LearningEntry_S
 * {
 * 	CHAR    ac_SaMac[6];
 * 	INT16   i2_AgentOutSupport;
 * 	UINT32 ui4_AgentOutTime;
 * 	INT32   i4_NiHandle;
 * 	LearningTable_T *pt_next;
 * };
 * @endcode
 *
 * @li@c ac_SaMac[6]           - MAC address
 * @li@c i2_AgentOutSupport    - default value is 0
 * @li@c ui4_AgentOutTime      - agent out time in second
 * @li@c i4_NiHandle           - the handle to network interface
 * @li@c pt_next               - point to next entry in same hash value
 */
struct LearningEntry_S
{
	CHAR    ac_SaMac[6];           /* MAC address */
	INT16   i2_AgentOutSupport;    /* default value is 0 */
	UINT32 ui4_AgentOutTime;       /* agent out time in second */
	INT32   i4_NiHandle;           /* the handle to network interface */
	LearningTable_T *pt_next;  /* point to next entry in same hash value */
};

/*!
 * @struct L2SWITCH_S
 * @brief layer 2 switch
 * @code
 * struct L2SWITCH_S
 * {
 *     CHAR              sz_name[8];
 *     INT32             i4_Lt_entry;
 *     LearningTable_T   t_lt[MAX_LEARNING_ENTRY];
 *     INT32             i4_NiCnt;
 *     INT32             ai4_NiHandle[MAX_NI_INTERFACE];
 *     INT32             i4_DefaultNi;
 *     struct L2SWITCH_S *pt_next;
 * };
 * @endcode
 *
 * @li@c sz_name[8]                          - name
 * @li@c i4_Lt_entry					     - the entry cnt 
 * @li@c t_lt[MAX_LEARNING_ENTRY]            - the default entry is 256
 * @li@c i4_NiCnt                            - ni cnt
 * @li@c ai4_NiHandle[MAX_NI_INTERFACE]      - the network interface handle
 * @li@c i4_DefaultNi                        - the default ni
 * @li@c pt_next                             - next switch entry
 */
struct L2SWITCH_S
{
    CHAR              sz_name[8];
    INT32             i4_Lt_entry;					       /* the entry cnt */
    LearningTable_T   t_lt[MAX_LEARNING_ENTRY];            /* the default entry is 256 */
    INT32             i4_NiCnt;                            /* ni cnt */
    INT32             ai4_NiHandle[MAX_NI_INTERFACE];      /* the network interface handle */
    INT32             i4_DefaultNi;                        /* the default ni */
    struct L2SWITCH_S *pt_next;
};

/**
 * packet sniffer hook array(list)
 */
#define NET_PKT_FILTER_CONTINUE  ((INT32) 0)            /**< packet filter continue */
#define NET_PKT_FILTER_DISCARD   ((INT32) 1)            /**< packet filter discard  */

/*!
 * @brief packet filter hook function
 * @param in pt_buff   - packet buffer
 */
typedef INT32 (*net_pkt_hook_fn) (PKT_BUFF_T *pt_buff);

/*!
 * @struct PKT_HOOK_T
 * @brief packet hook
 * @code
 * typedef struct pkt_hook_s
 * {
 *     INT32              i4_priority;
 *     INT32              i4_niHandle;
 *     net_pkt_hook_fn    pf_sniffer;
 *     struct pkt_hook_s *pt_next;
 * }PKT_HOOK_T;
 * @endcode
 *
 * @li@c i4_priority        - priority
 * @li@c i4_niHandle        - ni handle
 * @li@c pf_sniffer         - sniffer
 * @li@c pt_next            - next hook
 */
typedef struct pkt_hook_s
{
    INT32              i4_priority;
    INT32              i4_niHandle;
    net_pkt_hook_fn    pf_sniffer;
    struct pkt_hook_s *pt_next;
}PKT_HOOK_T;

/**
 * net_ni_receive_pkt
 * @param pt_ni
 * @param pt_buff
 *
 * @return INT32
 */
/*!
 * @brief ni receive packet function
 * @param in pt_ni   - ni pointer
 * @param in pt_buff - packet buffer
 */
typedef INT32 (*net_ni_receive_pkt) (NET_NI_T *pt_ni, PKT_BUFF_T *pt_buff);

/**
 * net_ni_transmit_pkt
 * @param pt_ni
 * @param pt_buff
 *
 * @return INT32
 */
/*!
 * @brief ni transmit packet function
 * @param in pt_ni   - ni pointer
 * @param in pt_buff - packet buffer
 */
typedef INT32 (*net_ni_transmit_pkt) (NET_NI_T *pt_ni, PKT_BUFF_T *pt_buff);

/**
 * Hardware abstract layer, the upper layer use this type to
 * communication with low layer driver.
 */
/*!
 * @struct NET_NI_S
 * @brief network interface
 * @code
 * struct NET_NI_S
 * {
 *     NET_DRV_IF_T            t_drv_if;
 * 	   net_ni_transmit_pkt     pf_TransmitPacket;
 * 	   net_ni_receive_pkt      pf_ReceivePacket;
 *     L2_SWITCH_T             *pt_switch;
 *     PKT_HOOK_T              *pt_pktHook;
 *     IF_TABLE_T              t_if_Table;
 *     struct NET_NI_S        *pt_next;
 *     struct NET_NI_S        *pt_associate_ni;
 * };
 * @endcode
 *
 * @li@c t_drv_if           - driver interface
 * @li@c pf_TransmitPacket  - transmit packet
 * @li@c pf_ReceivePacket   - receive packet
 * @li@c pt_switch          - switch
 * @li@c pt_pktHook         - packet hook
 * @li@c t_if_Table         - interface table
 * @li@c pt_next            - next pointer
 * @li@c pt_associate_ni    - pointer for IpNI to physical interface 
 */
struct NET_NI_S
{
    /* low level driver interface */
    NET_DRV_IF_T            t_drv_if;

	/* ni transmit */
	net_ni_transmit_pkt     pf_TransmitPacket;

	/* ni receive */
	net_ni_receive_pkt      pf_ReceivePacket;

    /* pointer to layer 2 switch */
    L2_SWITCH_T             *pt_switch;

    /* packet sniffer hook */
    PKT_HOOK_T              *pt_pktHook;

    /* interface information */
    IF_TABLE_T              t_if_Table;

    /* point to next */
    struct NET_NI_S        *pt_next;
    struct NET_NI_S        *pt_associate_ni; /* pointer for IpNI to physical interface */
};

/* Packet flow
 * Incoming packet -> IRQ -> Drv.ni_indicate_fnct -> SourceNi->pf_ReceivePacket ->
 * x_Switch_TransmitPacket(pt_switch, packet, srcIf) -> DestinationNi->pf_TransmitPacket ->
 * case 1: to IP stack,
 *         DestinationNi equal to IPNi
 *         pf_TransmitPacket call pf_ReceivePacket
 *         pf_ReceivePacket copy packet to Access internal API
 *         call ni_tx_complete_fnct to release packet.
 *         finished.
 * case 2: to other network interface
 *         pf_TransmitPacket call drv->netdrv_txmit_fnct
 *         drv->netdrv_txmit_fnct send packet out,
 *         call ni_tx_complete_fnct to release packet.
 */

/* Ni StartUp flow
 * ---------------
 * 1. create new swithc for dispatch packet
 *    pt_NewSwitch = x_Switch_New()
 * 2. create ni for each physical network interface
 *    pt_EthNi = x_Ni_New("eth0");
 * 3. Register ni to switch
 *    x_Switch_AddDefaultNi(pt_NewSwitch, (INT32) pt_EthNi);
 * 4. Register drv to ni
 *    x_Ni_InstallDrvEntry(pt_EthNi, EthDrvEntry); --> EthDrvEntry is a callback provided by driver
 * 5. Ni Start
 *    x_Ni_Start(pt_EthNi);
 */


#endif

/*! @} */

