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
/*! @file u_net_oid.h
 * $RCSfile: $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * @par Description:
 *      network interface driver oid
 */
/*----------------------------------------------------------------------------*/
/*!
* @addtogroup  groupMW_INET
* @{
*/

#ifndef U_NET_OID_H
#define U_NET_OID_H

#include "u_net_common.h"
/* 
    The following OID is use to manage low level driver,
    in system, the OID is 4 bytes length and the first 2 byets
    is mask as driver group.
*/
#define GET_OID_GROUP(X)    (X&0xffff0000)      /**< get driver oid group  */

#define OID_GROUP_CMN_IF    0x00010000          /**< driver oid group common   */
#define OID_GROUP_ETHER     0x00020000          /**< driver oid group ethernet */
#define OID_GROUP_802_11    0x00040000          /**< driver oid group 802.11   */

/*!
 * @enum NET_OID_T
 * @brief network oid
 * @code
 * typedef enum 
 * {
 *     OID_CMN_OID_START        =   OID_GROUP_CMN_IF,
 *     OID_CMN_IF_INDEX,
 *     OID_CMN_IF_DESCR,
 *     OID_CMN_IF_TYPE,
 *     OID_CMN_IF_MTU,
 *     OID_CMN_IF_SPEED,
 *     OID_CMN_IF_PHYADDRESS,
 *     OID_CMN_IF_MULTICASTADDRESSLIST,
 *     OID_CMN_IF_MULTICASTADDRESS,
 *     OID_CMN_IF_ADMIN_STATUS,
 *     OID_CMN_IF_OPERSTATUS,
 *     OID_CMN_IF_LASTCHANGE,
 *     OID_CMN_IF_NAME,
 *     OID_CMN_IF_PROMISCUOUS_MODE,
 *     OID_CMN_IF_IN_OCTETS,
 *     OID_CMN_IF_IN_UCASTPKT,
 *     OID_CMN_IF_IN_DISCARDS,
 *     OID_CMN_IF_IN_ERRORS,
 *     OID_CMN_IF_IN_UNKNOWN_PROTOS,
 *     OID_CMN_IF_OUT_OCTETS,
 *     OID_CMN_IF_OUT_UCASTPKTS,
 *     OID_CMN_IF_OUT_DISCARDS,
 *     OID_CMN_IF_OUT_ERRORS,
 *     OID_CMN_IF_IN_MULTICAST_PKTS,
 *     OID_CMN_IF_IN_BROADCAST_PKTS,
 *     OID_CMN_IF_OUT_MULTICAST_PKTS,
 *     OID_CMN_IF_OUT_BROADCAST_PKTS,
 *     OID_CMN_IF_IP_ADDRESS,
 *     OID_CMN_IF_MULTICASTADDRESS_DEL,
 *     OID_CMN_IF_CONNECT_STATUS,
 *     OID_CMN_IF_NIC_STATUS,
 *     OID_CMN_IF_PHYADDRESS_DEFAULT,
 *     OID_ETHER_OID_START     =   OID_GROUP_ETHER,        
 *     OID_ETHER_ALIGNMENT_ERRORS,
 *     OID_ETHER_FCSERRORS,
 *     OID_ETHER_SINGLECOLLISIONFRAMES,
 *     OID_ETHER_MULTIPLECOLLISIONFRAMES,
 *     OID_ETHER_DEFERREDTRANSMISSIONS,
 *     OID_ETHER_LATECOLLISIONS,
 *     OID_ETHER_EXCESSIVECOLLISIONS,
 *     OID_ETHER_INTERNALMACTRANSMITERRORS,
 *     OID_ETHER_FRAMETOOLONGS,
 *     OID_ETHER_INTERNALMACRECEIVEERRORS,
 *     OID_ETHER_SYMBOLERRORS,
 *     OID_ETHER_DUPLEXSTATUS,
 *     OID_802_11_OID_START    =   OID_GROUP_802_11,
 *     OID_802_11_SSID,
 *     OID_802_11_SCAN,
 *     OID_802_11_BSSID_LIST_SCAN,
 *     OID_802_11_INFRASTRUCTURE_MODE,
 *     OID_802_11_AUTHENTICATION_MODE,
 *     OID_802_11_DISASSOCIATE,
 *     OID_802_11_TX_POWER_LEVEL,
 *     OID_802_11_POWER_MODE,
 *     OID_802_11_RSSI,
 *     OID_802_11_ENCRYPTION_STATUS
 * }   NET_OID_T;
 * @endcode
 *
 * @li@c OID_CMN_OID_START,                              - common OID start
 * @li@c OID_CMN_IF_INDEX,                               - interface index
 * @li@c OID_CMN_IF_DESCR,                               - interface description 
 * @li@c OID_CMN_IF_TYPE,                                - interface type
 * @li@c OID_CMN_IF_MTU,                                 - interface MTU
 * @li@c OID_CMN_IF_SPEED,                               - interface speed
 * @li@c OID_CMN_IF_PHYADDRESS,                          - interface physical address
 * @li@c OID_CMN_IF_MULTICASTADDRESSLIST,                - interface multicast address list
 * @li@c OID_CMN_IF_MULTICASTADDRESS,                    - interface multicast address
 * @li@c OID_CMN_IF_ADMIN_STATUS,                        - interface adminstrator status
 * @li@c OID_CMN_IF_OPERSTATUS,                          - interface operation status
 * @li@c OID_CMN_IF_LASTCHANGE,                          - interface last change
 * @li@c OID_CMN_IF_NAME,                                - interface name
 * @li@c OID_CMN_IF_PROMISCUOUS_MODE,                    - interface promiscuous mode
 * @li@c OID_CMN_IF_IN_OCTETS,                           - interface in octets
 * @li@c OID_CMN_IF_IN_UCASTPKT,                         - interface in unicast packet
 * @li@c OID_CMN_IF_IN_DISCARDS,                         - interface in discards
 * @li@c OID_CMN_IF_IN_ERRORS,                           - interface in errors
 * @li@c OID_CMN_IF_IN_UNKNOWN_PROTOS,                   - interface in unknwon protocols
 * @li@c OID_CMN_IF_OUT_OCTETS,                          - interface out octets
 * @li@c OID_CMN_IF_OUT_UCASTPKTS,                       - interface out unicast packets
 * @li@c OID_CMN_IF_OUT_DISCARDS,                        - interface out discards
 * @li@c OID_CMN_IF_OUT_ERRORS,                          - interface out errors
 * @li@c OID_CMN_IF_IN_MULTICAST_PKTS,                   - interface in multicast packets
 * @li@c OID_CMN_IF_IN_BROADCAST_PKTS,                   - interface in broadcast packets
 * @li@c OID_CMN_IF_OUT_MULTICAST_PKTS,                  - interface out multicast packets
 * @li@c OID_CMN_IF_OUT_BROADCAST_PKTS,                  - interface out broadcast packets
 * @li@c OID_CMN_IF_IP_ADDRESS,                          - interface IP address
 * @li@c OID_CMN_IF_MULTICASTADDRESS_DEL,                - interface multicast address del
 * @li@c OID_CMN_IF_CONNECT_STATUS,                      - interface connect status
 * @li@c OID_CMN_IF_NIC_STATUS,                          - interface NIC status
 * @li@c OID_CMN_IF_PHYADDRESS_DEFAULT,                  - interface default physical address 
 * @li@c OID_ETHER_OID_START,                            - ethernet oid start
 * @li@c OID_ETHER_ALIGNMENT_ERRORS,                     - ethernet alignment errors
 * @li@c OID_ETHER_FCSERRORS,                            - ethernet fcs erros
 * @li@c OID_ETHER_SINGLECOLLISIONFRAMES,                - ethernet single collision frames
 * @li@c OID_ETHER_MULTIPLECOLLISIONFRAMES,              - ethernet multiple collision frames
 * @li@c OID_ETHER_DEFERREDTRANSMISSIONS,                - ethernet deferred transmissions
 * @li@c OID_ETHER_LATECOLLISIONS,                       - ethernet late collisions
 * @li@c OID_ETHER_EXCESSIVECOLLISIONS,                  - ethernet excessive collisions
 * @li@c OID_ETHER_INTERNALMACTRANSMITERRORS,            - ethernet internal MAC transmit erros
 * @li@c OID_ETHER_FRAMETOOLONGS,                        - ethernet frame too longs
 * @li@c OID_ETHER_INTERNALMACRECEIVEERRORS,             - ethernet internal MAC receive errors
 * @li@c OID_ETHER_SYMBOLERRORS,                         - ethernet symbol errors
 * @li@c OID_ETHER_DUPLEXSTATUS,                         - ethernet duplex status
 * @li@c OID_802_11_OID_START,                           - 802.11 oid start
 * @li@c OID_802_11_SSID,                                - 802.11 ssid
 * @li@c OID_802_11_SCAN,                                - 802.11 scan
 * @li@c OID_802_11_BSSID_LIST_SCAN,                     - 802.11 bssid list scan
 * @li@c OID_802_11_INFRASTRUCTURE_MODE,                 - 802.11 infrastructure mode
 * @li@c OID_802_11_AUTHENTICATION_MODE,                 - 802.11 authentication mode
 * @li@c OID_802_11_DISASSOCIATE,                        - 802.11 disassociate
 * @li@c OID_802_11_TX_POWER_LEVEL,                      - 802.11 tx power level
 * @li@c OID_802_11_POWER_MODE,                          - 802.11 power mode
 * @li@c OID_802_11_RSSI,                                - 802.11 rssi
 * @li@c OID_802_11_ENCRYPTION_STATUS                    - 802.11 encryption status
 */
typedef enum 
{
    OID_CMN_OID_START        =   OID_GROUP_CMN_IF,
    OID_CMN_IF_INDEX,                               /* param : INT32 */
    OID_CMN_IF_DESCR,                               /* param : CHAR[256] */
    OID_CMN_IF_TYPE,                                /* param : UINT32 */
    OID_CMN_IF_MTU,                                 /* param : UINT32 */
    OID_CMN_IF_SPEED,                               /* param : UINT32 */
    OID_CMN_IF_PHYADDRESS,                          /* param : MAC_ADDRESS_T */
    OID_CMN_IF_MULTICASTADDRESSLIST,                /* param : NET_MAC_ADDRESS_LIST_T */
    OID_CMN_IF_MULTICASTADDRESS,                    /* param : MAC_ADDRESS_T */
    OID_CMN_IF_ADMIN_STATUS,                        /* param : UINT32 */
    OID_CMN_IF_OPERSTATUS,                          /* param : UINT32 */
    OID_CMN_IF_LASTCHANGE,                          /* param : UINT32 */
    OID_CMN_IF_NAME,                                /* param : CHAR[256] */
    OID_CMN_IF_PROMISCUOUS_MODE,                    /* param : UINT32 */
    /* the following info will record in net_dev */
    OID_CMN_IF_IN_OCTETS,                           /* param : UINT64 */
    OID_CMN_IF_IN_UCASTPKT,                         /* param : UINT64 */
    OID_CMN_IF_IN_DISCARDS,                         /* param : UINT64 */
    OID_CMN_IF_IN_ERRORS,                           /* param : UINT64 */
    OID_CMN_IF_IN_UNKNOWN_PROTOS,                   /* param : UINT64 */
    OID_CMN_IF_OUT_OCTETS,                          /* param : UINT64 */
    OID_CMN_IF_OUT_UCASTPKTS,                       /* param : UINT64 */
    OID_CMN_IF_OUT_DISCARDS,                        /* param : UINT64 */
    OID_CMN_IF_OUT_ERRORS,                          /* param : UINT64 */
    OID_CMN_IF_IN_MULTICAST_PKTS,                   /* param : UINT64 */
    OID_CMN_IF_IN_BROADCAST_PKTS,                   /* param : UINT64 */
    OID_CMN_IF_OUT_MULTICAST_PKTS,                  /* param : UINT64 */
    OID_CMN_IF_OUT_BROADCAST_PKTS,                  /* param : UINT64 */
    OID_CMN_IF_IP_ADDRESS,                          /* param : UINT32, IP address (get only) */
    OID_CMN_IF_MULTICASTADDRESS_DEL,                /* remove multicast addr */
    OID_CMN_IF_CONNECT_STATUS,                      /* param : UINT32 */
    OID_CMN_IF_NIC_STATUS,                          /* param : UINT32 */
    OID_CMN_IF_PHYADDRESS_DEFAULT,                  /* param : UINT32 */
    /* -------------------------------------------------------------- */
    OID_ETHER_OID_START     =   OID_GROUP_ETHER,        
    OID_ETHER_ALIGNMENT_ERRORS,                     /* param : UINT32 */
    OID_ETHER_FCSERRORS,                            /* param : UINT32 */
    OID_ETHER_SINGLECOLLISIONFRAMES,                /* param : UINT32 */
    OID_ETHER_MULTIPLECOLLISIONFRAMES,              /* param : UINT32 */
    OID_ETHER_DEFERREDTRANSMISSIONS,                /* param : UINT32 */
    OID_ETHER_LATECOLLISIONS,                       /* param : UINT32 */
    OID_ETHER_EXCESSIVECOLLISIONS,                  /* param : UINT32 */
    OID_ETHER_INTERNALMACTRANSMITERRORS,            /* param : UINT32 */
    OID_ETHER_FRAMETOOLONGS,                        /* param : UINT32 */
    OID_ETHER_INTERNALMACRECEIVEERRORS,             /* param : UINT32 */
    OID_ETHER_SYMBOLERRORS,                         /* param : UINT32 */
    OID_ETHER_DUPLEXSTATUS,                         /* param : UINT32 */
    /* -------------------------------------------------------------- */
    OID_802_11_OID_START    =   OID_GROUP_802_11,
    OID_802_11_SSID,                                /* param : NET_802_11_SSID_T */
    OID_802_11_SCAN_,                                /* param : NULL */
    OID_802_11_BSSID_LIST_SCAN,                     /* param : NET_802_11_BSSID_LIST_T */
    OID_802_11_INFRASTRUCTURE_MODE,                 /* param : NET_802_11_NETWORK_INFRASTRUCTURE_T */
    OID_802_11_AUTHENTICATION_MODE,                 /* param : NET_802_11_AUTH_MODE_T */
    OID_802_11_DISASSOCIATE_,                        /* param : NULL */        
    OID_802_11_TX_POWER_LEVEL,                      /* param : NET_802_11_TX_POWER_LEVEL_T */
    OID_802_11_POWER_MODE,                          /* param : NET_802_11_POWER_MODE_T */
    OID_802_11_RSSI,                                /* param : NET_802_11_RSSI_T */
    OID_802_11_ENCRYPTION_STATUS                    /* param : NET_802_11_ENCRYPTION_STATUS_T*/
    /* -------------------------------------------------------------- */

}   NET_OID_T;

/*!
 * @struct NET_MAC_ADDRESS_LIST_T
 * @brief MAC address list
 * @code
 * typedef struct _NET_MAC_ADDRESS_LIST_S
 * {
 *     UINT32                ui4_NumberOfItems;
 *     MAC_ADDRESS_T         t_MulticastList [1];
 * }   NET_MAC_ADDRESS_LIST_T;
 * @endcode
 *
 * @li@c ui4_NumberOfItems  - number of items
 * @li@c t_MulticastList    - multicast list
 */
typedef struct _NET_MAC_ADDRESS_LIST_S
{
    UINT32                ui4_NumberOfItems;
    MAC_ADDRESS_T         t_MulticastList [1];
}   NET_MAC_ADDRESS_LIST_T;

/* Received signal strength indication (in unit of dBm) */
typedef INT32          NET_802_11_RSSI_T;

/*--------------------------------------------------------------*/
/* Set/Query data rates.                                        */
/*--------------------------------------------------------------*/
#define NET_802_11_MAX_LEN_RATES                     8              /**< 802.11 max rates length */
typedef UINT8   NET_802_11_RATES_T[NET_802_11_MAX_LEN_RATES];

/*--------------------------------------------------------------*/
/* Set/Query SSID setting                                       */
/*--------------------------------------------------------------*/
#define NET_802_11_MAX_LEN_SSID                      32             /**< 802.11 max ssid length  */


/* Network types include OFDM 5G(a) and 2.4G(g) */
/*!
 * @enum NET_802_11_NETWORK_TYPE_T
 * @brief 802.11 network type
 * @code
 * typedef enum
 * {
 *     IEEE_802_11_FH,
 *     IEEE_802_11_DS,
 *     IEEE_802_11_OFDM5,
 *     IEEE_802_11_OFDM24,
 *     IEEE_802_11_AUTOMODE,
 *     IEEE_802_11_NETWORK_TYPE_MAX
 * }   NET_802_11_NETWORK_TYPE_T;
 * @endcode
 *
 * @li@c IEEE_802_11_FH                 - 802.11 type FH
 * @li@c IEEE_802_11_DS                 - 802.11 Dtype S
 * @li@c IEEE_802_11_OFDM5              - 802.11 Otype FDM5
 * @li@c IEEE_802_11_OFDM24             - 802.11 Otype FDM24
 * @li@c IEEE_802_11_AUTOMODE           - 802.11 atype utomode
 * @li@c IEEE_802_11_NETWORK_TYPE_MAX   - 802.11 max network type, upper bound, not real case
 */
typedef enum
{
    IEEE_802_11_FH,
    IEEE_802_11_DS,
    IEEE_802_11_OFDM5,
    IEEE_802_11_OFDM24,
    IEEE_802_11_AUTOMODE,
    IEEE_802_11_NETWORK_TYPE_MAX                    /* Upper bound, not real case */
}   NET_802_11_NETWORK_TYPE_T;

/*!
 * @struct NET_802_11_NETWORK_TYPE_LIST_T
 * @brief 802.11 network type list
 * @code
 * typedef struct _NET_802_11_NETWORK_TYPE_LIST_S
 * {
 *     UINT32                           ui4_NumberOfItems;
 *     NET_802_11_NETWORK_TYPE_T        e_NetworkType [1];
 * }   NET_802_11_NETWORK_TYPE_LIST_T;
 * @endcode
 *
 * @li@c ui4_NumberOfItems      - number of items (at least 1)
 * @li@c e_NetworkType          - network type
 */
typedef struct _NET_802_11_NETWORK_TYPE_LIST_S
{
    UINT32                           ui4_NumberOfItems;      /* At least 1 */
    NET_802_11_NETWORK_TYPE_T        e_NetworkType [1];
}   NET_802_11_NETWORK_TYPE_LIST_T;

/*--------------------------------------------------------------*/
/* Set/Query network operation mode.                            */
/*--------------------------------------------------------------*/
/*!
 * @enum NET_802_11_NETWORK_INFRASTRUCTURE_T
 * @brief 802.11 network infrastructure
 * @code
 * typedef enum
 * {
 *     IEEE_802_11_NET_TYPE_IBSS,
 *     IEEE_802_11_NET_TYPE_INFRA,
 *     IEEE_802_11_NET_TYPE_AUTO_UNKNOWN,
 *     IEEE_802_11_NET_TYPE_INFRA_MAX
 * }   NET_802_11_NETWORK_INFRASTRUCTURE_T;
 * @endcode
 *
 * @li@c IEEE_802_11_NET_TYPE_IBSS                      - 802.11 network type Ad Hoc 
 * @li@c IEEE_802_11_NET_TYPE_INFRA                     - 802.11 network type Infrastructure STA 
 * @li@c IEEE_802_11_NET_TYPE_AUTO_UNKNOWN              - 802.11 network type Automatic to join BSS 
 * @li@c IEEE_802_11_NET_TYPE_INFRA_MAX                 - 802.11 network type Upper bound, not real case 
 */
typedef enum
{
    IEEE_802_11_NET_TYPE_IBSS,                      /* Ad Hoc */
    IEEE_802_11_NET_TYPE_INFRA,                     /* Infrastructure STA */
    IEEE_802_11_NET_TYPE_AUTO_UNKNOWN,               /* Automatic to join BSS */
    IEEE_802_11_NET_TYPE_INFRA_MAX                   /* Upper bound, not real case */
}   NET_802_11_NETWORK_INFRASTRUCTURE_T;


/*!
 * @struct NET_802_11_CONFIG_FH_T
 * @brief 802.11 config FH
 * @code
 * typedef struct _NET_802_11_CONFIG_FH_S
 * {
 *     UINT32             ui4_Length;
 *     UINT32             ui4_HopPattern;
 *     UINT32             ui4_HopSet;
 *     UINT32             ui4_DwellTime;
 * }   NET_802_11_CONFIG_FH_T;
 * @endcode
 *
 * @li@c ui4_Length              - Length of structure 
 * @li@c ui4_HopPattern          - Defined as 802.11 
 * @li@c ui4_HopSet              - to one if non-802.11 
 * @li@c ui4_DwellTime           - In unit of Kusec 
 */
typedef struct _NET_802_11_CONFIG_FH_S
{
    UINT32             ui4_Length;             /* Length of structure */
    UINT32             ui4_HopPattern;         /* Defined as 802.11 */
    UINT32             ui4_HopSet;             /* to one if non-802.11 */
    UINT32             ui4_DwellTime;          /* In unit of Kusec */
}   NET_802_11_CONFIG_FH_T;

/*!
 * @struct NET_802_11_CONFIG_T
 * @brief 802.11 config
 * @code
 * typedef struct _NET_802_11_CONFIG_S
 * {
 *     UINT32                          ui4_Length;
 *     UINT32                          ui4_BeaconPeriod;
 *     UINT32                          ui4_ATIMWindow;
 *     UINT32                          ui4_DSConfig;
 *     NET_802_11_CONFIG_FH_T          t_FHConfig;
 * }   NET_802_11_CONFIG_T;
 * @endcode
 *
 * @li@c ui4_Length             - Length of structure 
 * @li@c ui4_BeaconPeriod       - In unit of Kusec
 * @li@c ui4_ATIMWindow         - In unit of Kusec 
 * @li@c ui4_DSConfig           - Channel frequency in unit of kHz 
 * @li@c t_FHConfig             - fh config
 */
typedef struct _NET_802_11_CONFIG_S
{
    UINT32                          ui4_Length;             /* Length of structure */
    UINT32                          ui4_BeaconPeriod;       /* In unit of Kusec */
    UINT32                          ui4_ATIMWindow;         /* In unit of Kusec */
    UINT32                          ui4_DSConfig;           /* Channel frequency in unit of kHz */
    NET_802_11_CONFIG_FH_T          t_FHConfig;
}   NET_802_11_CONFIG_T;

/*--------------------------------------------------------------*/
/* Query scan result.                                           */
/*--------------------------------------------------------------*/

#if 0
/*!
 * @struct NET_802_11_BSSID_LIST_T
 * @brief 802.11 bssid list
 * @code
 * typedef struct _NET_802_11_BSSID_LIST_S
 * {
 *     UINT32                                  NumberOfItems;
 *     NET_802_11_BSSID_T                      Bssid[1];
 * }   NET_802_11_BSSID_LIST_T;
 * @endcode
 *
 * @li@c NumberOfItems      - number of item (at least 1)
 * @li@c Bssid[1]           - bbsid array
 */
typedef struct _NET_802_11_BSSID_LIST_S
{
    UINT32                                  NumberOfItems;                  /* at least 1 */
    NET_802_11_BSSID_T                      Bssid[1];
}   NET_802_11_BSSID_LIST_T;
#endif

/*--------------------------------------------------------------*/
/* Set/Query power saving mode                                  */
/*--------------------------------------------------------------*/
/*!
 * @enum NET_802_11_POWER_MODE_T
 * @brief 802.11 power mode
 * @code
 * typedef enum
 * {
 *     IEEE_802_11_POWER_MODE_CAM,
 *     IEEE_802_11_POWER_MODE_MAX_PSP,
 *     IEEE_802_11_POWER_MODE_FAST_PSP,
 *     IEEE_802_11_POWER_MODE_MAX
 * }   NET_802_11_POWER_MODE_T;
 * @endcode
 *
 * @li@c IEEE_802_11_POWER_MODE_CAM             - power mode cam
 * @li@c IEEE_802_11_POWER_MODE_MAX_PSP         - power mode max psp
 * @li@c IEEE_802_11_POWER_MODE_FAST_PSP        - power mode fast psp
 * @li@c IEEE_802_11_POWER_MODE_MAX             - Upper bound, not real case 
 */
typedef enum
{
    IEEE_802_11_POWER_MODE_CAM,
    IEEE_802_11_POWER_MODE_MAX_PSP,
    IEEE_802_11_POWER_MODE_FAST_PSP,
    IEEE_802_11_POWER_MODE_MAX                      /* Upper bound, not real case */
}   NET_802_11_POWER_MODE_T;


/* In unit of milliwatts */
typedef UINT32         NET_802_11_TX_POWER_LEVEL_T;

/* Encryption types */
/*!
 * @enum NET_802_11_ENCRYPTION_STATUS_T
 * @brief 802.11 encryption status
 * @code
 * typedef enum
 * {
 *     IEEE_802_11_WEP_ENABLED,
 *     IEEE_802_11_ENCRYPTION1_ENABLED = IEEE_802_11_WEP_ENABLED,
 *     IEEE_802_11_WEP_DISABLED,
 *     IEEE_802_11_ENCRYPTION_DISABLED = IEEE_802_11_WEP_DISABLED,
 *     IEEE_802_11_WEP_KEY_ABSENT,
 *     IEEE_802_11_ENCRYPTION1_KEY_ABSENT = IEEE_802_11_WEP_KEY_ABSENT,
 *     IEEE_802_11_WEP_NOT_SUPPORTED,
 *     IEEE_802_11_ENCRYPTION_NOT_SUPPORTED = IEEE_802_11_WEP_NOT_SUPPORTED,
 *     IEEE_802_11_ENCRYPTION2_ENABLED,
 *     IEEE_802_11_ENCRYPTION2_KEY_ABSENT,
 *     IEEE_802_11_ENCRYPTION3_ENABLED,
 *     IEEE_802_11_ENCRYPTION3_KEY_ABSENT
 * }   NET_802_11_ENCRYPTION_STATUS_T;
 * @endcode
 *
 * @li@c IEEE_802_11_WEP_ENABLED                - wep enabled
 * @li@c IEEE_802_11_ENCRYPTION1_ENABLED        - encryption enabled
 * @li@c IEEE_802_11_WEP_DISABLED               - wep disable
 * @li@c IEEE_802_11_ENCRYPTION_DISABLED        - encryption disabled
 * @li@c IEEE_802_11_WEP_KEY_ABSENT             - wep key absent
 * @li@c IEEE_802_11_ENCRYPTION1_KEY_ABSENT     - encryption key absent
 * @li@c IEEE_802_11_WEP_NOT_SUPPORTED          - wep not supported
 * @li@c IEEE_802_11_ENCRYPTION_NOT_SUPPORTED   - encryption not supported
 * @li@c IEEE_802_11_ENCRYPTION2_ENABLED        - encryption 2 enabled
 * @li@c IEEE_802_11_ENCRYPTION2_KEY_ABSENT     - encryption 2 key absent
 * @li@c IEEE_802_11_ENCRYPTION3_ENABLED        - encryption 3 enabled
 * @li@c IEEE_802_11_ENCRYPTION3_KEY_ABSENT     - encryption 3 key absent
 */
typedef enum
{
    IEEE_802_11_WEP_ENABLED,
    IEEE_802_11_ENCRYPTION1_ENABLED = IEEE_802_11_WEP_ENABLED,
    IEEE_802_11_WEP_DISABLED,
    IEEE_802_11_ENCRYPTION_DISABLED = IEEE_802_11_WEP_DISABLED,
    IEEE_802_11_WEP_KEY_ABSENT,
    IEEE_802_11_ENCRYPTION1_KEY_ABSENT = IEEE_802_11_WEP_KEY_ABSENT,
    IEEE_802_11_WEP_NOT_SUPPORTED,
    IEEE_802_11_ENCRYPTION_NOT_SUPPORTED = IEEE_802_11_WEP_NOT_SUPPORTED,
    IEEE_802_11_ENCRYPTION2_ENABLED,
    IEEE_802_11_ENCRYPTION2_KEY_ABSENT,
    IEEE_802_11_ENCRYPTION3_ENABLED,
    IEEE_802_11_ENCRYPTION3_KEY_ABSENT
}   NET_802_11_ENCRYPTION_STATUS_T;

#endif 

/*! @} */

