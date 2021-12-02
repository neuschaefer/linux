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

#ifndef U_NET_DRV_IF_H
#define U_NET_DRV_IF_H

#include "u_net_common.h"

/* PKT_BUFF_T.ui4_tag */
#define TX_PKT_BUFF_TAG      0xBBBEEFFF

#define PKT_BUFF_HEADER      48
#define PKT_BUFF_DATA_SIZE   1536
#define PKT_BUFF_TAILER      0
#define PKT_BUFF_SIZE        (PKT_BUFF_HEADER + PKT_BUFF_DATA_SIZE + PKT_BUFF_TAILER)
#define PKT_BUFF_NUM         64 /* refer to AT_DIX_BUFF_NUMBER */

/**
 * Forward declare
 */
typedef struct _PKT_BUFF_S     PKT_BUFF_T;
typedef struct _NET_DRV_IF_S NET_DRV_IF_T;

/**
 * Low level driver interface to network device (Hardware abstract layer)
 *
 * netdrv_init_fnct,
 *      driver init api, upper layer call this API to init driver,
 *      before this API is called, driver can not initial anything.
 *
 * netdrv_deinit_fnct,
 *      upper layer use this function to release driver, after
 *      this callback is called all driver allocate resource
 *      must be release.
 *
 * netdrv_start_fnct,
 *      infom driver the transmit and receive is enable
 *
 * netdrv_stop_fnct,
 *      inform drvier to stop transmit and receive packet
 *
 * netdrv_txmit_fnct,
 *      upper layer use this API to transmit a packet
 *
 * netdrv_set_fnct,
 *      upper layer use this API to change interface behavior
 *
 * netdrv_get_fnct,
 *      upper layer use this API to get driver information
 *
 * pkt_free_fnct,
 *      when packet is consumed, caller use this API to release
 *      packet
 */
typedef INT32 (*netdrv_init_fnct)   (NET_DRV_IF_T *pt_drv_if);
typedef INT32 (*netdrv_deinit_fnct) (NET_DRV_IF_T *pt_drv_if);
typedef INT32 (*netdrv_start_fnct)  (NET_DRV_IF_T *pt_drv_if);
typedef INT32 (*netdrv_stop_fnct)   (NET_DRV_IF_T *pt_drv_if);
typedef INT32 (*netdrv_txmit_fnct)  (NET_DRV_IF_T *pt_drv_if, PKT_BUFF_T *pt_pkb);
typedef INT32 (*netdrv_set_fnct)    (NET_DRV_IF_T *pt_drv_if, INT32 i4_cmd, VOID *pv_param);
typedef INT32 (*netdrv_get_fnct)    (NET_DRV_IF_T *pt_drv_if, INT32 i4_cmd, VOID *pv_param);
typedef INT32 (*pkt_free_fnct)      (NET_DRV_IF_T *pt_drv_if, PKT_BUFF_T *pt_pkt);

/**
 * Notify API for upper layer
 *
 * ni_tx_complete_fnct,
 *      when driver transmit packet complete, the driver will use
 *      this API to notify upper layer
 *
 * ni_indicate_fnct,
 *      when driver received packet from its DMA, call this function
 *      to notify upper layer prepare for packet processing
 */
typedef INT32 (*ni_tx_complete_fnct)(NET_DRV_IF_T*, PKT_BUFF_T *pt_pkb);
typedef INT32 (*ni_indicate_fnct)   (NET_DRV_IF_T*, PKT_BUFF_T *pt_pkb);
typedef INT32 (*ni_dev_ev_fnct)     (NET_DRV_IF_T*, NI_DRV_EV_T e_event);

typedef struct _LIST_ENTRY_S
{
    struct _LIST_ENTRY_S   *pt_next;
} LIST_ENTRY_T;

typedef struct _LIST_HEAD_S
{
    LIST_ENTRY_T      *pt_head;
    LIST_ENTRY_T      *pt_tail;
    UINT32                 ui4_count;
} LIST_HEAD_T;

/**
 * PKT_BUFF_T
 *      Internal packet structure
 *
 * @param   ui4_tag, reserve
 * @param   ui4_time_stamp, trace packet incoming time
 * @param   pui1_data, raw packet buffer pointer
 * @param   ui4_len, raw packet buffer length
 * @param   t_chksum_info, packet checksum info
 * @param   pt_if_src,     source network device interface
 * @param   pt_if_dst,     destination network device interface
 * @param   ui2_refcnt,    packet buffer refernce count
 * @param   pf_pkt_free,   free packet
 */
struct _PKT_BUFF_S
{
    LIST_ENTRY_T                t_link;         /* link to next pkt buffer */
    UINT32                      ui4_tag;        /* assign by driver */
    UINT32                      ui4_time_stamp; /* assign by driver */
    UINT8                      *pui1_data_sp;  /* data start pointer */
    UINT32                      ui4_len_total;  /* total length */
    UINT8                      *pui1_data;     /* assign by driver */
    UINT32                      ui4_len;        /* assign by driver */

    NET_CHKSUM_T                t_chksum_info;  /* assign by driver */
    INT32                       i4_SrcNiHandle; /* assign by L2 : source interface */
    INT32                       i4_DestNiHandle;/* assign by L2 : destination interface */

    UINT16                      ui2_refcnt;     /* assign by L2 : reference count of this packet */
#define IEEE8021Q_VLAN_TAGGING   ((UINT16)(1<<0))
    UINT16                      ui2_flag;       /* assign by L2 */

    pkt_free_fnct               pf_pkt_free;    /* driver need to assign free function pointer in initial packet buffer */
};

/**
 * _NET_DRV_IF_S,
 *      driver layer API collection, this is interface between upper
 *      layer and driver
 */
struct _NET_DRV_IF_S
{
    netdrv_init_fnct        pf_drv_init;        /* Called after device is detached from network. */
    netdrv_deinit_fnct      pf_drv_deinit;
    netdrv_start_fnct       pf_drv_start;
    netdrv_stop_fnct        pf_drv_stop;
    netdrv_txmit_fnct       pf_drv_txmit;
    netdrv_set_fnct         pf_drv_set;
    netdrv_get_fnct         pf_drv_get;

    /* NI assigned */
    ni_tx_complete_fnct     pf_tx_complete;
    ni_indicate_fnct        pf_ni_indicate;
    ni_dev_ev_fnct          pf_ni_event_notify;

    /* upper layer assigned */
    INT16                   i2_dev_idx;

    /* The address to its upper layer interface */
    INT32                   i4_ni_handle;

    /* pointer to private data */
    VOID                    *pv_priv;
};

/**
 * NetworkDriverEntry
 *
 * low level driver entry point, each driver owner need provide this API for upper layer
 * to install itself to upper layer.
 */
typedef VOID (*NetworkDriverEntry)(NET_DRV_IF_T *pt_driver);

/**
 * Forward declare
 */
struct _NET_SUPC_IF_S;

/**
 * NI to WPA supplicant
 *
 * supc_deinit_fnct,
 *      NI call this API to deinit Supc
 *
 * supc_msg_fnct,
 *      NI call this API to send messages to Supc
 *
 * -- Notify API for NI --
 *
 * ni_supc_event_fnct,
 *      supc use this API to send events to notify upper layer
 */
typedef INT32 (*supc_deinit_fnct)       (struct _NET_SUPC_IF_S *pt_drv_if);
//typedef INT32 (*supc_msg_fnct)          (struct _NET_SUPC_IF_S *prSupcMsg);
typedef INT32 (*ni_supc_event_fnct)     (INT32 i4MsgId, VOID *pvParam);
typedef INT32 (*ni_supc_set_fnct)    (struct _NET_SUPC_IF_S *supc_ni_if, INT32 i4_cmd, VOID *pv_param);
typedef INT32 (*ni_supc_get_fnct)    (struct _NET_SUPC_IF_S *supc_ni_if, INT32 i4_cmd, VOID *pv_param);


typedef struct _NET_SUPC_IF_S
{
    //supc_msg_fnct           pf_supc_msg_fnct;
    ni_supc_set_fnct		pf_ni_set_fnct;
    ni_supc_get_fnct		pf_ni_get_fnct;

    /* NI assigne */
    ni_supc_event_fnct      pf_ni_event_nfy;

    /* NI interface */
    NET_DRV_IF_T            *pt_ni_wndrv_if;

    /* pointer to private data */
    VOID                    *pv_priv;
} NET_SUPC_IF_T;


#endif
