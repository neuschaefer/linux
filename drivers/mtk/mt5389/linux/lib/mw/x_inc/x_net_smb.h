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
#ifndef X_NET_SMB_H
#define X_NET_SMB_H

#include "u_common.h"
#include "u_net_common.h"

extern INT64 _smb_get_ms (VOID);
extern INT64 _smb_get_tick (VOID);
extern UINT32 _smb_os_get_sys_tick (VOID);

#ifdef NET_SMB_TEST_DEBUG
extern INT32 net_smb_dbg;
#ifndef NET_SMB_MODULE_NAME
#define NET_SMB_MODULE_NAME "SMB"
#endif /* NET_SMB_MODULE_NAME */
#define SMB_LOG_ERROR(...) do { INT64 i8_cctt = 0; if (net_smb_dbg <= 0) break; i8_cctt = _smb_get_ms(); x_dbg_stmt ("[%s] %lld.%03lld, %s.%d >>> ", NET_SMB_MODULE_NAME, i8_cctt/1000, i8_cctt%1000, __FUNCTION__, __LINE__); x_dbg_stmt (__VA_ARGS__); } while (0)
#define SMB_LOG_INFO(...) do { INT64 i8_cctt = 0; if (net_smb_dbg <= 1) break; i8_cctt =_smb_get_ms(); x_dbg_stmt ("[%s] %lld.%03lld, %s.%d >>> ", NET_SMB_MODULE_NAME, i8_cctt/1000, i8_cctt%1000, __FUNCTION__, __LINE__); x_dbg_stmt (__VA_ARGS__); } while (0)
#define SMB_LOG_DETAIL(...) do { INT64 i8_cctt = 0; if (net_smb_dbg <= 2) break; i8_cctt =_smb_get_ms(); x_dbg_stmt ("[%s] %lld.%03lld, %s.%d >>> ", NET_SMB_MODULE_NAME, i8_cctt/1000, i8_cctt%1000, __FUNCTION__, __LINE__); x_dbg_stmt (__VA_ARGS__); } while (0)
#else
#define SMB_LOG_ERROR(...)
#define SMB_LOG_INFO(...)
#define SMB_LOG_DETAIL(...)
#endif /* NET_SMB_TEST_DEBUG */

typedef enum {
    NET_SMB_DEVICE_EVENT_ADD,
    NET_SMB_DEVICE_EVENT_REMOVE
} NET_SMB_DEVICE_EVENT_T;

typedef VOID (*x_net_smb_dev_notify) (NET_SMB_DEVICE_EVENT_T e, const CHAR* ps_srv, VOID* pv_tag);

extern INT32 x_net_smb_grps (HANDLE_T *ph_handle);
extern INT32 x_net_smb_grps_show (HANDLE_T h_handle);
extern INT32 x_net_smb_grps_delete (HANDLE_T h_handle);

extern INT32 x_net_smb_servers_group_get (HANDLE_T h_handle, const CHAR *ps_server, CHAR **pps_group);
extern INT32 x_net_smb_servers_group_get2 (const CHAR *ps_server, CHAR **pps_group);
extern INT32 x_net_smb_servers_check_server (HANDLE_T h_handle, const CHAR* ps_server);
extern INT32 x_net_smb_servers_check_server2 (const CHAR* ps_server);
extern INT32 x_net_smb_servers_num (HANDLE_T h_handle);
extern INT32 x_net_smb_servers_get (HANDLE_T h_handle, INT32 i4_idx, CHAR **pps_server);
extern INT32 x_net_smb_servers (HANDLE_T *ph_handle);
extern INT32 x_net_smb_servers_show (HANDLE_T h_handle);
extern INT32 x_net_smb_servers_delete (HANDLE_T h_handle);
extern INT32 x_net_smb_servers_comp (HANDLE_T h_left, HANDLE_T h_right);
extern INT32 x_net_smb_servers_comp_details (HANDLE_T h_left, HANDLE_T h_right, CHAR ***ppps_great, INT32 *pi4_great_num, CHAR ***ppps_less, INT32 *pi4_less_num);
extern INT32 x_net_smb_comp_details_free (CHAR **pps_strs, INT32 i4_num);

extern INT32 x_net_smb_shares_who (HANDLE_T h_handle, CHAR** pps_server);
extern INT32 x_net_smb_shares_num (HANDLE_T h_handle);
extern INT32 x_net_smb_shares_get (HANDLE_T h_handle, INT32 i4_idx, CHAR **pps_share);
extern INT32 x_net_smb_shares (HANDLE_T *ph_handle, const CHAR *ps_server);
extern INT32 x_net_smb_shares_show (HANDLE_T h_handle);
extern INT32 x_net_smb_shares_delete (HANDLE_T h_handle);
extern INT32 x_net_smb_shares_comp (HANDLE_T h_left, HANDLE_T h_right);
extern INT32 x_net_smb_shares_comp_details (HANDLE_T h_left, HANDLE_T h_right, CHAR ***ppps_great, INT32 *pi4_great_num, CHAR ***ppps_less, INT32 *pi4_less_num);

extern INT32 x_net_smb_mount (const CHAR* ps_server, const CHAR* ps_share, const CHAR* ps_dev);
extern INT32 x_net_smb_umount (const CHAR* ps_server, const CHAR* ps_share, const CHAR* ps_dev);
extern INT32 x_net_smb_umount_all (VOID);
extern INT32 x_net_smb_umount_server (const CHAR* ps_server);
extern INT32 x_net_smb_mount_points_show (VOID);

extern INT32 x_net_smb_set_root (const CHAR* ps_root);
extern CHAR* x_net_smb_get_root (VOID);

extern INT32 x_net_smb_init (VOID);
extern INT32 x_net_smb_deinit (VOID);
extern INT32 x_net_smb_start (x_net_smb_dev_notify fn);
extern INT32 x_net_smb_stop (VOID);

extern INT32 x_net_smb_perms_show (VOID);
extern INT32 x_net_smb_set_perm (const CHAR* ps_server, const CHAR* ps_share, const CHAR* ps_user, const CHAR* ps_passwd, const CHAR* ps_group, INT32 i4_auto);
extern INT32 x_net_smb_free_all_perm (VOID);

extern INT32 x_net_smb_add_server_ex (const CHAR* ps_ip, const CHAR* ps_grp);
extern INT32 x_net_smb_remove_server_ex (const CHAR* ps_ip);
extern INT32 x_net_smb_show_server_ex (VOID);

extern INT32 x_net_smb_ip_cache_show (VOID);
extern INT32 x_net_smb_show_cmd_list (VOID);

extern INT32 x_net_smb_refresh (VOID);
extern INT32 x_net_smb_refresh_ex (VOID);
extern INT32 x_net_smb_mount_points_refresh (HANDLE_T h_handle);


#endif /* X_NET_SMB_H */


