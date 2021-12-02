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
 * $RCSfile: x_atsc_si_eng.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains ATSC-SI engine specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_ATSC_SI_H_
#define _X_ATSC_SI_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_atsc_si_eng.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

extern INT32 x_atsc_si_get_tbl_version (HANDLE_T    h_obj,
                                        UINT8*      pui1_tbl_version,
                                        TM_COND_T*  pe_cond);

extern INT32 x_atsc_si_get_prot_version (HANDLE_T    h_obj,
                                         UINT8*      pui1_prot_version,
                                         TM_COND_T*  pe_cond);

extern INT32 x_atsc_si_get_num_mgt_entries (HANDLE_T    h_mgt,
                                            UINT16*     pui2_num_entries,
                                            TM_COND_T*  pe_cond);

extern INT32 x_atsc_si_get_mgt_entry (HANDLE_T             h_mgt,
                                      UINT16               ui2_idx,
                                      ATSC_SI_MGT_INFO_T*  pt_mgt_info,
                                      TM_COND_T*           pe_cond);

extern INT32 x_atsc_si_get_vct_ts_id (HANDLE_T    h_vct,
                                      UINT16*     pui2_ts_id,
                                      TM_COND_T*  pe_cond);

extern INT32 x_atsc_si_get_num_vct_entries (HANDLE_T    h_vct,
                                            UINT16*     pui2_num_entries,
                                            TM_COND_T*  pe_cond);

extern INT32 x_atsc_si_get_vct_entry (HANDLE_T             h_vct,
                                      UINT16               ui2_idx,
                                      ATSC_SI_VCT_INFO_T*  pt_cvct_info,
                                      TM_COND_T*           pe_cond);

extern INT32 x_atsc_si_get_rrt_region (HANDLE_T    h_rrt,
                                       UINT8*      pui1_region,
                                       TM_COND_T*  pe_cond);

extern INT32 x_atsc_si_get_num_rrt_entries (HANDLE_T    h_rrt,
                                            UINT16*     pui2_num_entries,
                                            TM_COND_T*  pe_cond);

extern INT32 x_atsc_si_get_rrt_entry (HANDLE_T             h_rrt,
                                      UINT16               ui2_idx,
                                      ATSC_SI_RRT_INFO_T*  pt_rrt_info,
                                      TM_COND_T*           pe_cond);

extern INT32 x_atsc_si_get_eit_src_id (HANDLE_T    h_eit,
                                       UINT16*     pui2_src_id,
                                       TM_COND_T*  pe_cond);

extern INT32 x_atsc_si_get_num_eit_entries (HANDLE_T    h_eit,
                                            UINT16*     pui2_num_entries,
                                            TM_COND_T*  pe_cond);

extern INT32 x_atsc_si_get_eit_entry (HANDLE_T             h_eit,
                                      UINT16               ui2_idx,
                                      ATSC_SI_EIT_INFO_T*  pt_eit_info,
                                      TM_COND_T*           pe_cond);

extern INT32 x_atsc_si_get_ett_etm_id (HANDLE_T    h_ett,
                                       UINT32*     pui4_etm_id,
                                       TM_COND_T*  pe_cond);

extern INT32 x_atsc_si_get_txt_num_lang (HANDLE_T    h_txt,
                                         UINT16*     pui2_num_lang);

extern INT32 x_atsc_si_get_txt_info (HANDLE_T                  h_txt,
                                     UINT16                    ui2_lang_idx,
                                     ATSC_SI_TXT_LANG_INFO_T*  pt_lang_info);

extern INT32 x_atsc_si_get_txt_seg_info (HANDLE_T                 h_txt,
                                         UINT16                   ui2_lang_idx,
                                         UINT16                   ui2_seg_idx,
                                         ATSC_SI_TXT_SEG_INFO_T*  pt_text_seg_info);

extern INT32 x_atsc_si_get_txt (HANDLE_T  h_txt,
                                UINT16    ui2_lang_idx,
                                UINT16    ui2_seg_idx,
                                CHAR*     ps_txt,
                                SIZE_T*   pz_txt_len);

extern INT32 x_atsc_si_get_txt_len (HANDLE_T  h_txt,
                                    SIZE_T*   pz_txt_len);

extern INT32 x_atsc_si_get_txt_len_by_lang (HANDLE_T        h_txt,
                                            ISO_639_LANG_T  t_lang,
                                            SIZE_T*         pz_event_info_len);

extern INT32 x_atsc_si_get_txt_by_lang (HANDLE_T        h_txt,
                                        ISO_639_LANG_T  t_lang,
                                        SIZE_T*         pz_event_info_len,
                                        CHAR*           ps_txt);

extern INT32 x_atsc_si_get_eas_event_data (HANDLE_T     h_eas,
                                           EAS_EVENT_T* pt_eas_info,
                                           TM_COND_T*   pe_cond);

extern INT32 x_atsc_si_get_eas_evt_code_len (HANDLE_T   h_eas,
                                             SIZE_T*    pz_evt_code_len,
                                             TM_COND_T* pe_cond);

extern INT32 x_atsc_si_get_eas_evt_code (HANDLE_T   h_eas,
                                         CHAR*      ps_evt_code,  
                                         SIZE_T*    pz_evt_code_len,
                                         TM_COND_T* pe_cond);

extern INT32 x_atsc_si_get_eas_num_loc_entries (HANDLE_T    h_eas,
                                                UINT16*     pui2_num_entries,
                                                TM_COND_T*  pe_cond);

extern INT32 x_atsc_si_get_eas_loc_entry (HANDLE_T          h_eas,
                                          UINT16            ui2_idx,
                                          EAS_LOC_INFO_T*   pt_location,
                                          TM_COND_T*        pe_cond);

extern INT32 x_atsc_si_get_eas_num_xcept_entries (HANDLE_T      h_eas,
                                                  UINT16*       pui2_num_entries,
                                                  TM_COND_T*    pe_cond);

extern INT32 x_atsc_si_get_eas_xcept_entry (HANDLE_T            h_eas,
                                            UINT16              ui2_idx,
                                            EAS_XCEPT_INFO_T*   pt_xcept,
                                            TM_COND_T*          pe_cond);

extern INT32 x_atsc_si_txt_loc_vct_sname (HANDLE_T   h_vct,
                                          UINT16     ui2_idx,
                                          HANDLE_T*  ph_txt);

extern INT32 x_atsc_si_txt_loc_rrt_region (HANDLE_T   h_rrt,
                                           HANDLE_T*  ph_txt);

extern INT32 x_atsc_si_txt_loc_rrt_dim (HANDLE_T   h_rrt,
                                        UINT16     ui2_dim_idx,
                                        HANDLE_T*  ph_txt);

extern INT32 x_atsc_si_txt_loc_rrt_abrv_rating (HANDLE_T   h_rrt,
                                                UINT16     ui2_dim_idx,
                                                UINT16     ui2_val_idx,
                                                HANDLE_T*  ph_txt);

extern INT32 x_atsc_si_txt_loc_rrt_rating (HANDLE_T   h_rrt,
                                           UINT16     ui2_dim_idx,
                                           UINT16     ui2_val_idx,
                                           HANDLE_T*  ph_txt);

extern INT32 x_atsc_si_txt_loc_eit_title (HANDLE_T   h_eit,
                                          UINT16     ui2_idx,
                                          HANDLE_T*  ph_txt);

extern INT32 x_atsc_si_txt_loc_ett_ext_text (HANDLE_T   h_ett,
                                             HANDLE_T*  ph_txt);

extern INT32 x_atsc_si_txt_loc_descr (HANDLE_T      h_obj,
                                      const UINT8*  pui1_multi_lang_txt,
                                      HANDLE_T*     ph_txt);

extern INT32 x_atsc_si_txt_loc_eas_nat_of_act (HANDLE_T     h_eas,
                                               HANDLE_T*    ph_txt);

extern INT32 x_atsc_si_txt_loc_eas_alert (HANDLE_T  h_eas,
                                          HANDLE_T* ph_txt);

extern INT32 x_atsc_si_init (const CHAR*                ps_name,
                             const ATSC_SI_CONFIG_T*    pt_config);

extern INT32 x_atsc_si_load_stt (HANDLE_T       h_root,
                                 VOID*          pv_nfy_tag,
                                 x_tm_nfy_fct   pf_nfy,
                                 HANDLE_T*      ph_stt,
                                 TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_stt_by_ts_descr (HANDLE_T       h_root,
                                             TS_DESCR_T*    pt_ts_descr,
                                             VOID*          pv_nfy_tag,
                                             x_tm_nfy_fct   pf_nfy,
                                             HANDLE_T*      ph_stt,
                                             TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_mgt_by_ts_id (HANDLE_T       h_root,
                                          UINT16         ui2_ts_id,
                                          TM_SRC_TYPE_T  e_src_type,
                                          VOID*          pv_src_info,
                                          VOID*          pv_nfy_tag,
                                          x_tm_nfy_fct   pf_nfy,
                                          HANDLE_T*      ph_mgt,
                                          TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_mgt_by_ts_descr (HANDLE_T       h_root,
                                             TS_DESCR_T*    pt_ts_descr,
                                             TM_SRC_TYPE_T  e_src_type,
                                             VOID*          pv_src_info,
                                             VOID*          pv_nfy_tag,
                                             x_tm_nfy_fct   pf_nfy,
                                             HANDLE_T*      ph_mgt,
                                             TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_mgt_by_ts_descr_wo_time_check (
                                             HANDLE_T       h_root,
                                             TS_DESCR_T*    pt_ts_descr,
                                             TM_SRC_TYPE_T  e_src_type,
                                             VOID*          pv_src_info,
                                             VOID*          pv_nfy_tag,
                                             x_tm_nfy_fct   pf_nfy,
                                             HANDLE_T*      ph_mgt,
                                             TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_mgt (HANDLE_T       h_root,
                                 TM_SRC_TYPE_T  e_src_type,
                                 VOID*          pv_src_info,
                                 VOID*          pv_nfy_tag,
                                 x_tm_nfy_fct   pf_nfy,
                                 HANDLE_T*      ph_mgt,
                                 TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_cvct (HANDLE_T       h_mgt,
                                  TM_SRC_TYPE_T  e_src_type,
                                  VOID*          pv_src_info,
                                  VOID*          pv_nfy_tag,
                                  x_tm_nfy_fct   pf_nfy,
                                  HANDLE_T*      ph_cvct,
                                  TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_tvct (HANDLE_T       h_mgt,
                                  TM_SRC_TYPE_T  e_src_type,
                                  VOID*          pv_src_info,
                                  VOID*          pv_nfy_tag,
                                  x_tm_nfy_fct   pf_nfy,
                                  HANDLE_T*      ph_tvct,
                                  TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_cvct_by_ts_id (HANDLE_T       h_mgt,
                                           TM_SRC_TYPE_T  e_src_type,
                                           VOID*          pv_src_info,
                                           VOID*          pv_nfy_tag,
                                           x_tm_nfy_fct   pf_nfy,
                                           HANDLE_T*      ph_cvct,
                                           TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_tvct_by_ts_id (HANDLE_T       h_mgt,
                                           TM_SRC_TYPE_T  e_src_type,
                                           VOID*          pv_src_info,
                                           VOID*          pv_nfy_tag,
                                           x_tm_nfy_fct   pf_nfy,
                                           HANDLE_T*      ph_tvct,
                                           TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_rrt (HANDLE_T       h_mgt,
                                 UINT8          ui1_region,
                                 TM_SRC_TYPE_T  e_src_type,
                                 VOID*          pv_src_info,
                                 VOID*          pv_nfy_tag,
                                 x_tm_nfy_fct   pf_nfy,
                                 HANDLE_T*      ph_rrt,
                                 TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_eit_by_idx (HANDLE_T       h_vct,
                                        UINT16         ui2_idx,
                                        TIME_T         t_time,
                                        TM_SRC_TYPE_T  e_src_type,
                                        VOID*          pv_src_info,
                                        VOID*          pv_nfy_tag,
                                        x_tm_nfy_fct   pf_nfy,
                                        HANDLE_T*      ph_eit,
                                        TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_eit_by_src_id (HANDLE_T       h_obj,
                                           UINT16         ui2_src_id,
                                           TIME_T         t_time,
                                           TM_SRC_TYPE_T  e_src_type,
                                           VOID*          pv_src_info,
                                           VOID*          pv_nfy_tag,
                                           x_tm_nfy_fct   pf_nfy,
                                           HANDLE_T*      ph_eit,
                                           TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_eit_0_by_idx (HANDLE_T       h_vct,
                                         UINT16         ui2_idx,
                                         TM_SRC_TYPE_T  e_src_type,
                                         VOID*          pv_src_info,
                                         VOID*          pv_nfy_tag,
                                         x_tm_nfy_fct   pf_nfy,
                                         HANDLE_T*      ph_eit,
                                         TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_eit_0_by_src_id (HANDLE_T       h_obj,
                                            UINT16         ui2_src_id,
                                            TM_SRC_TYPE_T  e_src_type,
                                            VOID*          pv_src_info,
                                            VOID*          pv_nfy_tag,
                                            x_tm_nfy_fct   pf_nfy,
                                            HANDLE_T*      ph_eit,
                                            TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_ett_by_idx (HANDLE_T       h_obj,
                                        UINT16         ui2_idx,
                                        TM_SRC_TYPE_T  e_src_type,
                                        VOID*          pv_src_info,
                                        VOID*          pv_nfy_tag,
                                        x_tm_nfy_fct   pf_nfy,
                                        HANDLE_T*      ph_ett,
                                        TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_ett_by_src_id (HANDLE_T       h_mgt,
                                           UINT16         ui2_src_id,
                                           TM_SRC_TYPE_T  e_src_type,
                                           VOID*          pv_src_info,
                                           VOID*          pv_nfy_tag,
                                           x_tm_nfy_fct   pf_nfy,
                                           HANDLE_T*      ph_ett,
                                           TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_ett_by_src_evt_id (HANDLE_T       h_mgt,
                                               UINT16         ui2_src_id,
                                               UINT16         ui2_evt_id,
                                               TIME_T         t_time,
                                               TM_SRC_TYPE_T  e_src_type,
                                               VOID*          pv_src_info,
                                               VOID*          pv_nfy_tag,
                                               x_tm_nfy_fct   pf_nfy,
                                               HANDLE_T*      ph_ett,
                                               TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_load_eas (HANDLE_T       h_root,
                                 VOID*          pv_nfy_tag,
                                 x_tm_nfy_fct   pf_nfy,
                                 HANDLE_T*      ph_eas,
                                 TM_COND_T*     pe_cond);

extern INT32 x_atsc_si_set_vct_by_ts_id (HANDLE_T    h_vct,
                                         UINT16      ui2_ts_id,
                                         TM_COND_T*  pe_cond);

#ifdef RAW_MEM_DISPLAY
extern INT32 x_atsc_si_reg_raw_mem_nfy (atsc_si_rm_nfy_fct pf_nfy);

extern INT32 x_atsc_si_raw_mem_set (ATSC_SI_RM_SET_T e_set_type);

#endif /* RAW_MEM_DISPLAY */

#endif /* _X_ATSC_SI_H_ */
