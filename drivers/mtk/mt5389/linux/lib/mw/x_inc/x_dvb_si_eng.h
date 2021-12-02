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
/*-----------------------------------------------------------------------------
 * $RCSfile: x_dvb_si_eng.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains DVB-SI engine specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_DVB_SI_H_
#define _X_DVB_SI_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_dvb_si_eng.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

extern INT32 x_dvb_si_get_bq_id (
                    HANDLE_T    h_bat,
                    UINT16*     pui2_bq_id,
                    TM_COND_T*  pe_cond);

extern INT32 x_dvb_si_get_nw_id (
                    HANDLE_T    h_nit,
                    UINT16*     pui2_nw_id,
                    TM_COND_T*  pe_cond);

extern INT32 x_dvb_si_get_num_evt_entries ( 
                    HANDLE_T    h_eit, 
                    UINT16*     pui2_num_entries, 
                    TM_COND_T*  pe_cond); 

extern INT32 x_dvb_si_get_evt_entry (
                    HANDLE_T                  h_eit,
                    UINT16                    ui2_idx,
                    DVB_SI_EVT_ENTRY_INFO_T*  pt_evt_info,
                    TM_COND_T*                pe_cond);

/* For ISDB */
#ifdef MW_ISDB_EIT_SUPPORT /* dyin: 081030 */
extern INT32 x_dvb_si_get_evt_ext_cond(
                    HANDLE_T                  h_eit,
                    TM_COND_T*                pe_ext_cond,
                    TM_COND_T*                pe_cond);
#endif

extern INT32 x_dvb_si_get_num_ts_entries (
                    HANDLE_T    h_nit, 
                    UINT16*     pui2_num_entries, 
                    TM_COND_T*  pe_cond);

extern INT32 x_dvb_si_get_ts_entry (
                    HANDLE_T                  h_nit,
                    UINT16                    ui2_idx,
                    DVB_SI_ON_TS_ID_INFO_T*   pt_ts_info,
                    TM_COND_T*                pe_cond);

extern INT32 x_dvb_si_get_num_svc_entries ( 
                    HANDLE_T    h_sdt, 
                    UINT16*     pui2_num_entries, 
                    TM_COND_T*  pe_cond); 

extern INT32 x_dvb_si_get_svc_entry (
                    HANDLE_T                  h_sdt,
                    UINT16                    ui2_idx,
                    DVB_SI_SVC_ENTRY_INFO_T*  pt_svc_info,
                    TM_COND_T*                pe_cond);

extern INT32 x_dvb_si_get_svc_ids (
                    HANDLE_T                     h_eit,
                    DVB_SI_ON_TS_SVC_ID_INFO_T*  pt_svc_info,
                    TM_COND_T*                   pe_cond);

extern INT32 x_dvb_si_get_ts_ids (
                    HANDLE_T                 h_sdt,
                    DVB_SI_ON_TS_ID_INFO_T*  pt_ts_info,
                    TM_COND_T*               pe_cond);

extern INT32 x_dvb_si_get_sched_end_time ( 
                    HANDLE_T                        h_eit_s, 
                    DVB_SI_SCHED_END_TIME_INFO_T*   pt_sched_end_time_info, 
                    TM_COND_T*                      pe_cond); 

extern INT32 x_dvb_si_get_num_app_entries (HANDLE_T    h_ait, 
                                   UINT16*     pui2_num_entries, 
                                   TM_COND_T*  pe_cond);

extern INT32 x_dvb_si_get_app_entry (HANDLE_T                     h_ait,
                             UINT16                           ui2_idx,
                             DVB_SI_AIT_APP_ID_CTRL_CODE_T*   pt_app_info,
                             TM_COND_T*                pe_cond);



extern INT32 x_dvb_si_mon_get_num_nw_id (
                    HANDLE_T    h_mon_nit,
                    UINT16*     pui2_num_nw_id,
                    TM_COND_T*  pe_cond);

extern INT32 x_dvb_si_mon_get_num_oui (
                    HANDLE_T    h_mon_unt,
                    UINT16*     pui2_num_oui,
                    TM_COND_T*  pe_cond);

extern INT32 x_dvb_si_mon_get_nw_id (
                    HANDLE_T    h_mon_nit,
                    UINT16      ui2_idx,
                    UINT16*     pui2_nw_id,
                    TM_COND_T*  pe_cond);

extern INT32 x_dvb_si_mon_get_on_ts_id ( 
                    HANDLE_T                  h_mon_sdt, 
                    UINT16                    ui2_idx, 
                    DVB_SI_ON_TS_ID_INFO_T*   pt_on_ts_id, 
                    TM_COND_T*                pe_cond); 

extern INT32 x_dvb_si_mon_get_oui ( 
                    HANDLE_T                  h_mon_unt, 
                    UINT16                    ui2_idx, 
                    UINT32*                   pui4_oui, 
                    TM_COND_T*                pe_cond); 

extern INT32 x_dvb_si_txt_loc_descr ( 
                    HANDLE_T                h_obj, 
                    const UINT8*            pui1_txt, 
                    SIZE_T                  z_txt_len,
                    CHAR*                   t_count_code,
                    HANDLE_T*               ph_txt); 

extern INT32 x_dvb_si_get_txt_len ( 
                    HANDLE_T                h_txt, 
                    DVB_SI_GET_TXT_MODE_T   e_get_mode, 
                    SIZE_T*                 pz_txt_len);

extern INT32 x_dvb_si_get_txt ( 
                    HANDLE_T                h_txt, 
                    DVB_SI_GET_TXT_MODE_T   e_get_mode, 
                    CHAR*                   ps_txt, 
                    SIZE_T*                 pz_txt_len); 

extern INT32 x_dvb_si_get_txt_style_size ( 
                    HANDLE_T                h_txt, 
                    SIZE_T*                 pz_style_str_size); 

extern INT32 x_dvb_si_get_txt_style ( 
                    HANDLE_T                h_txt, 
                    STYLE_STR_T*            pt_style_str, 
                    SIZE_T*                 pz_style_str_size); 

extern INT32 x_dvb_si_init (
                    const CHAR*       ps_name,
                    DVB_SI_CONFIG_T*  pt_config);

extern INT32 x_dvb_si_load_bat (
                    HANDLE_T      h_root,
                    UINT16        ui2_bq_id,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_bat,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_eit_pf (
                    HANDLE_T      h_root,
                    UINT16        ui2_on_id,
                    UINT16        ui2_ts_id,
                    UINT16        ui2_svc_id,
                    BOOL          b_high_prio,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_eit_pf,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_eit_pf_by_idx (
                    HANDLE_T      h_sdt,
                    UINT16        ui2_idx,
                    BOOL          b_high_prio,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_eit_pf,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_eit_pf_by_svc_id (
                    HANDLE_T      h_sdt,
                    UINT16        ui2_svc_id, 
                    BOOL          b_high_prio,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_eit_pf,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_eit_s (
                    HANDLE_T      h_root,
                    UINT16        ui2_on_id,
                    UINT16        ui2_ts_id,
                    UINT16        svc_id,
                    TIME_T        t_time,
                    BOOL          b_high_prio,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_eit_s,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_eit_pf_sections (
                    HANDLE_T      h_root,
                    BOOL          b_actual_only,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_eit_pf,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_eit_s_sections (
                        HANDLE_T      h_root,
                        BOOL    b_actual_only,
                        UINT8   ui1_tbl_id,
                        VOID*         pv_nfy_tag,
                        x_tm_nfy_fct  pf_nfy,
                        HANDLE_T*     ph_eit_s,
                        TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_set_eit_section_mask_clear (HANDLE_T      h_eit,
                    DVB_SI_ON_TS_SVC_ID_SEC_INFO_T*          pv_sec_info,
                    TM_COND_T*     pe_cond);


extern INT32 x_dvb_si_load_eit_s_by_idx (
                    HANDLE_T      h_sdt,
                    UINT16        ui2_idx,
                    TIME_T        t_time,
                    BOOL          b_high_prio,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_eit_s,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_eit_s_by_svc_id (
                    HANDLE_T      h_sdt,
                    UINT16        ui2_svc_id,
                    TIME_T        t_time,
                    BOOL          b_high_prio,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_eit_s,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_nit (
                    HANDLE_T      h_root,
                    UINT16        ui2_nw_id,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_nit,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_nit_actual (
                    HANDLE_T      h_root,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_nit,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_rst (
                    HANDLE_T      h_root,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_rst,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_sdt (
                    HANDLE_T      h_root,
                    UINT16        ui2_on_id,
                    UINT16        ui2_ts_id,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_sdt,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_sdt_by_idx (
                    HANDLE_T      h_bat_nit,
                    UINT16        ui2_idx,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_sdt,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_sdt_by_ts_descr (
                    HANDLE_T      h_root,
                    TS_DESCR_T*   pt_ts_descr,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_sdt,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_sdt_actual (
                    HANDLE_T      h_root,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_sdt,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_tdt (
                    HANDLE_T      h_root,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_tdt,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_tot (
                    HANDLE_T      h_root,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_tot,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_unt (
                    HANDLE_T      h_root,
                    MPEG_2_PID_T  t_pid,
                    UINT32        ui4_oui,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_unt,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_load_ait (HANDLE_T      h_root,
                         MPEG_2_PID_T   t_pid,
                         UINT16         ui2_app_type,
                         VOID*         pv_nfy_tag,
                         x_tm_nfy_fct  pf_nfy,
                         HANDLE_T*     ph_ait,
                         TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_mon_bat (
                    HANDLE_T      h_root,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_bat_mon,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_mon_nit (
                    HANDLE_T      h_root,
                    BOOL          b_actual_ts,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_nit_mon,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_mon_sdt (
                    HANDLE_T      h_root,
                    BOOL          b_actual_ts,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_sdt_mon,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_mon_unt (
                    HANDLE_T      h_root,
                    MPEG_2_PID_T  t_pid,
                    VOID*         pv_nfy_tag,
                    x_tm_nfy_fct  pf_nfy,
                    HANDLE_T*     ph_unt_mon,
                    TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_mon_ait (HANDLE_T      h_root,
                        MPEG_2_PID_T  t_pid,
                        VOID*         pv_nfy_tag,
                        x_tm_nfy_fct  pf_nfy,
                        HANDLE_T*     ph_ait_mon,
                        TM_COND_T*    pe_cond);

extern INT32 x_dvb_si_mon_get_num_app_types (HANDLE_T    h_mon_ait,
                                  UINT16*     pui2_num_app_types,
                                  TM_COND_T*  pe_cond);


extern INT32 x_dvb_si_mon_get_app_type (HANDLE_T    h_mon_ait,
                              UINT16      ui2_idx,
                              UINT16*     pui2_app_type,
                              TM_COND_T*  pe_cond);

#endif /* _X_DVB_SI_H_ */
