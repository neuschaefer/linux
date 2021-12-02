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
/*---------------------------------------------------------------------------*
 * Description:
 *    system startup
 *       IP stack -> Switch -> NI -> Drv
 *---------------------------------------------------------------------------*/
#ifndef X_NET_NI_IF_H
#define X_NET_NI_IF_H

#include "u_net_drv_if.h"
#include "u_net_ni_if.h"

/**
 * Layer 2 switch
 */
extern L2_SWITCH_T *x_net_switch_new    (CHAR *ps_name);
extern INT32        x_net_switch_add_default_ni  (L2_SWITCH_T *pt_Switch, INT32 i4_NiHandle);
extern INT32        x_net_switch_add_ni          (L2_SWITCH_T *pt_Switch, INT32 i4_NiHandle);
extern INT32        x_net_switch_add_learn_entry (L2_SWITCH_T *pt_Switch, UINT8 *pac_Mac, INT32 i4_NiHandle);
extern INT32        x_net_swtich_find_learn_entry(L2_SWITCH_T *pt_Switch, UINT8 *pac_Mac);
extern INT32        x_net_switch_transmit_packet(L2_SWITCH_T *pt_Switch, PKT_BUFF_T *pt_packet, INT32 i4_NiHandle);

/**
 * Network interface utility
 */
extern NET_NI_T    *x_net_ni_new(CHAR *ps_name, INT32 i4_ifType);
extern INT32        x_net_ni_install_drv_entry(NET_NI_T *pt_ni, NetworkDriverEntry pf_fn);
extern INT32        x_net_ni_start(NET_NI_T *pt_ni);
extern INT32        x_net_ni_stop(NET_NI_T *pt_ni);
extern INT32        x_net_ni_install_transmit_packet_callback(NET_NI_T *pt_ni, net_ni_transmit_pkt pf_fn);
extern INT32        x_net_ni_install_receive_packet_callback(NET_NI_T *pt_ni, net_ni_receive_pkt pf_fn);
extern INT32        x_net_ni_packet_indication(NET_DRV_IF_T *pt_drv, PKT_BUFF_T *pt_pkb);
extern INT32        x_net_ni_tx_completed(NET_DRV_IF_T *pt_drv, PKT_BUFF_T *pt_pkb);
extern INT32        x_net_ni_rx_packet(NET_NI_T *pt_ni, PKT_BUFF_T *pt_buff);
extern INT32        x_net_ni_tx_packet(NET_NI_T *pt_ni, PKT_BUFF_T *pt_buff);
extern NET_NI_T    *x_net_ni_get_by_name(CHAR *ps_name);
extern VOID         x_net_ni_list_reset(VOID);
extern INT32        x_net_ni_set_mac(CHAR *psz_name, CHAR *pc_mac);
extern INT32        x_net_ni_save_mac (const CHAR *psz_name, UINT8 *pui1_mac);
extern INT32        x_net_ni_get_mac(CHAR *psz_name, CHAR *pac_mac);
extern INT32        x_net_ni_set_if_type(NET_NI_T *pt_ni, INT32 i4_type);
extern NET_NI_T    *x_net_ni_list_get(VOID);
extern INT32        x_net_ni_add_assocate_ni(NET_NI_T *pt_ni1, NET_NI_T *pt_ni2);
extern VOID         x_net_ping(INT32 i4_ip_version, UINT32 ui4_src_ip, UINT32 ui4_dest_ip, INT16 i2_try_cnt, INT32 i4_size, INT16 i2_interval);
extern VOID         x_net_ni_show_by_name(CHAR *psz_name);
extern VOID         x_net_ni_show_all(VOID);
extern VOID         x_net_ping4 (CHAR * ps_ip, INT32  i4_len, INT16  i2_wait, VOID (*notify_fn) (INT16 i2_rsp_time));
extern VOID         x_net_ni_debug(VOID);
extern INT32        x_net_ni_add_pkt_hook(CHAR *ps_niName, INT32 i4_priority, net_pkt_hook_fn fn);
extern INT32        x_net_discard_netbios_service_pkt(PKT_BUFF_T *pt_buff);

/* if information get */
extern INT32        x_net_ni_get(CHAR *ps_niName, INT32 i4_cmd, VOID *pv_param);
extern INT32        x_net_ni_set(CHAR *ps_niName, INT32 i4_cmd, VOID *pv_param);

extern INT32        x_net_ni_reg_ev_notify(CHAR *ps_ni_name, NI_DRV_EV_T e_ev, ni_ev_notify_fnct pf_notify);
extern INT32        x_net_ni_unreg_ev_notify(CHAR *ps_ni_name, NI_DRV_EV_T e_ev, ni_ev_notify_fnct pf_notify);
extern INT32        x_net_ni_ev_send(NET_NI_T *pt_ni, NI_DRV_EV_T e_ev);

/**
 * x_net_if_join
 *
 * Add multicast IP address to interface
 *
 * @param ps_niName
 * @param ui4_ip, the IP address (network order)
 *
 * @return INT32
 * NET_OK, ok
 * NET_NI_NOT_EXIST, ni not exist
 */
extern INT32 x_net_if_join(const CHAR* ps_niName, UINT32 ui4_ip);

/**
 * x_net_if_leave
 *
 * leave a multicast group
 *
 * @param ps_niName
 * @param ui4_ip, ip address (network order)
 *
 * @return INT32
 */
extern INT32 x_net_if_leave(const CHAR* ps_niName, UINT32 ui4_ip);

/**
 * x_net_if_multi_list
 *
 * get multicast list
 *
 * @param ps_niName
 * @param paui4_ip, local buffer for multicast IP
 * @param i4_size, size of paui4_ip
 *
 * @return INT32
 * >0, the # set of multicast IP
 */
extern INT32 x_net_if_multi_list(const CHAR* ps_niName, UINT32 *paui4_ip, INT32 i4_size);

/**
 * x_net_get_mac_by_ip
 *
 * get MAC by IP address
 *
 * @param ps_ip, IP address string
 * @param puac_mac, output, need allocate by users. UCHAR mac[6]
 *
 * @return 0 OK, else failure.
 */
extern INT32 x_net_get_mac_by_ip (const CHAR *ps_ip, UCHAR *puac_mac);

#endif


