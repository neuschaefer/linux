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
 * $RCSfile: x_isdb_si_eng.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains ARIB-SI engine specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_ISDB_SI_ENG_H_
#define _X_ISDB_SI_ENG_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_isdb_si_eng.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern INT32    x_isdb_si_get_bq_id (
                    HANDLE_T            h_bat,
                    UINT16*             pui2_bq_id,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_on_ts_svc_id (
                    HANDLE_T            h_sdtt,
                    ISDB_SI_ON_TS_SVC_ID_INFO_T* pt_on_ts_svc_id,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_num_content_group_entries (
                    HANDLE_T            h_sdtt,
                    UINT16*             pui2_num_entries,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_scontent_group_entry (
                    HANDLE_T            h_sdtt,
                    UINT16              ui2_idx,
                    ISDB_SI_CONTENT_GROUP_ENTRY_INFO_T* pt_scontent_info,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_sdtt_schedule_desc_entry(
                    HANDLE_T            h_sdtt,
                    UINT16              ui2_idx,
                    UINT16              ui2_schedule_desc_idx,
                    ISDB_SI_SCHEDULE_DESC_INFO_T* pt_schedule_desc_info,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_num_logo_data_entries (
                    HANDLE_T            h_cdt,
                    UINT16*             pui2_num_entries,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_logo_entry_by_idx (
                    HANDLE_T            h_cdt,
                    UINT16              ui2_idx,
                    ISDB_SI_LOGO_INFO_T*pt_logo_info,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_logo_data_by_idx (
                    HANDLE_T            h_cdt,
                    UINT16              ui2_idx,
                    UINT16              ui2_data_len,
                    VOID*               pt_logo_data,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_num_content_ver_entries (
                    HANDLE_T            h_pcat,
                    UINT16*             pui2_num_entries,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_content_ver_entry (
                    HANDLE_T            h_pcat,
                    UINT16              ui2_idx,
                    ISDB_SI_CONTENT_VER_ENTRY_INFO_T* pt_content_ver_info,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_schedule_desc_entry(
                    HANDLE_T            h_pcat,
                    UINT16              ui2_idx,
                    UINT16              ui2_schedule_desc_idx,
                    ISDB_SI_SCHEDULE_DESC_INFO_T* pt_schedule_desc_info,
                    TM_COND_T*          pe_cond);

extern INT32    x_isdb_si_get_num_brdcst_ids (
                    HANDLE_T            h_bit,
                    UINT16*             pui2_num_brdcst_id,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_brdcst_id_by_idx (
                    HANDLE_T            h_bit,
                    UINT16              ui2_idx,
                    UINT8*              pui1_brdcst_id,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_num_net_board_entries ( 
                    HANDLE_T            h_nbit, 
                    UINT16*             pui2_num_entries, 
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_net_board_entry (
                    HANDLE_T            h_nbit,
                    UINT16              ui2_idx,
                    ISDB_SI_NET_BOARD_INFO_T* pt_evt_info,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_net_board_key_id_by_idx (
                    HANDLE_T            h_nbit,
                    UINT16              ui2_idx,
                    UINT16              ui2_key_idx,
                    UINT16*             pui2_key_id,
                    TM_COND_T*          pe_cond
                    ); 
extern INT32    x_isdb_si_get_num_desc_ids (
                    HANDLE_T            h_ldt,
                    UINT16*             pui2_num_desc_id,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_desc_id_by_idx (
                    HANDLE_T            h_ldt,
                    UINT16              ui2_idx,
                    UINT8*              pui1_desc_id,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_nw_id (
                    HANDLE_T            h_nit,
                    UINT16*             pui2_nw_id,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_num_evt_entries ( 
                    HANDLE_T            h_eit, 
                    UINT16*             pui2_num_entries, 
                    TM_COND_T*          pe_cond
                    ); 
extern INT32    x_isdb_si_get_evt_entry (
                    HANDLE_T            h_eit,
                    UINT16              ui2_idx,
                    ISDB_SI_EVT_ENTRY_INFO_T* pt_evt_info,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_evt_ext_cond(
                    HANDLE_T            h_heit,
                    TM_COND_T*          pe_ext_cond,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_num_ts_entries (
                    HANDLE_T            h_nit, 
                    UINT16*             pui2_num_entries, 
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_ts_entry (
                    HANDLE_T            h_nit,
                    UINT16              ui2_idx,
                    ISDB_SI_ON_TS_ID_INFO_T*  pt_ts_info,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_num_evt_relation_node_entries(
                    HANDLE_T            h_ert,
                    UINT16*             pui2_num_entries,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_evt_relation_node_entry (
                    HANDLE_T            h_ert,
                    UINT16              ui2_idx,
                    ISDB_SI_EVT_RELATION_NODE_INFO_T*  pt_node_info,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_num_local_evt_entries (
                    HANDLE_T            h_lit, 
                    UINT16*             pui2_num_entries, 
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_local_evt_id_entry (
                    HANDLE_T            h_lit,
                    UINT16              ui2_idx,
                    UINT16*             pui2_local_evt_id,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_num_svc_entries ( 
                    HANDLE_T            h_sdt, 
                    UINT16*             pui2_num_entries, 
                    TM_COND_T*          pe_cond
                    ); 
extern INT32    x_isdb_si_get_svc_entry (
                    HANDLE_T            h_sdt,
                    UINT16              ui2_idx,
                    ISDB_SI_SVC_ENTRY_INFO_T* pt_svc_info,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_svc_ids (
                    HANDLE_T            h_eit,
                    ISDB_SI_ON_TS_SVC_ID_INFO_T* pt_svc_info,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_ts_ids (
                    HANDLE_T            h_sdt,
                    ISDB_SI_ON_TS_ID_INFO_T*  pt_ts_info,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_get_sched_end_time ( 
                    HANDLE_T            h_heit_s, 
                    ISDB_SI_SCHED_END_TIME_INFO_T*   pt_sched_end_time_info,
                    TM_COND_T*          pe_cond
                    ); 
extern INT32    x_isdb_si_mon_get_num_nw_id (
                    HANDLE_T            h_mon_nit,
                    UINT16*             pui2_num_nw_id,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_mon_get_nw_id (
                    HANDLE_T            h_mon_nit,
                    UINT16              ui2_idx,
                    UINT16*             pui2_nw_id,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_mon_get_on_ts_id ( 
                    HANDLE_T            h_mon_sdt, 
                    UINT16              ui2_idx, 
                    ISDB_SI_ON_TS_ID_INFO_T* pt_on_ts_id, 
                    TM_COND_T*          pe_cond
                    ); 
extern INT32    x_isdb_si_txt_loc_descr ( 
                    HANDLE_T            h_obj, 
                    const UINT8*        pui1_txt, 
                    SIZE_T              z_txt_len,
                    ISO_3166_COUNT_T    t_count_code,
                    HANDLE_T*           ph_txt
                    ); 
extern INT32    x_isdb_si_get_txt_len ( 
                    HANDLE_T            h_txt, 
                    ISDB_SI_GET_TXT_MODE_T e_get_mode, 
                    SIZE_T*             pz_txt_len
                    );
extern INT32    x_isdb_si_get_txt ( 
                    HANDLE_T            h_txt, 
                    ISDB_SI_GET_TXT_MODE_T e_get_mode, 
                    CHAR*               ps_txt, 
                    SIZE_T*             pz_txt_len
                    ); 
extern INT32    x_isdb_si_get_txt_style_size ( 
                    HANDLE_T            h_txt, 
                    SIZE_T*             pz_style_str_size
                    ); 
extern INT32    x_isdb_si_get_txt_style ( 
                    HANDLE_T            h_txt, 
                    STYLE_STR_T*        pt_style_str, 
                    SIZE_T*             pz_style_str_size
                    ); 
extern INT32    x_isdb_si_init (
                    const CHAR*         ps_name,
                    ISDB_SI_CONFIG_T*   pt_config
                    );
extern INT32    x_isdb_si_load_cdt_by_key_id (
                    HANDLE_T            h_root,
                    UINT16              ui2_on_id,
                    UINT16              ui2_dd_id,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_cdt,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_lsdtt_by_maker_model_id (
                    HANDLE_T            h_root,
                    UINT16              ui2_maker_model_id,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_lsdtt,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_hsdtt_by_maker_model_id (
                    HANDLE_T            h_root,
                    UINT16              ui2_maker_model_id,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_hsdtt,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_bat (
                    HANDLE_T            h_root,
                    UINT16              ui2_bq_id,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_bat,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_leit (
                    HANDLE_T            h_root,
                    UINT16              ui2_on_id,
                    UINT16              ui2_ts_id,
                    UINT16              ui2_svc_id,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_leit,
                    TM_COND_T*          pe_cond
                    );                    
extern INT32    x_isdb_si_load_meit (
                    HANDLE_T            h_root,
                    UINT16              ui2_on_id,
                    UINT16              ui2_ts_id,
                    UINT16              ui2_svc_id,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_meit,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_heit_pf (
                    HANDLE_T            h_root,
                    UINT16              ui2_on_id,
                    UINT16              ui2_ts_id,
                    UINT16              ui2_svc_id,
                    BOOL                b_high_prio,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_heit_pf,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_heit_pf_by_idx (
                    HANDLE_T            h_sdt,
                    UINT16              ui2_idx,
                    BOOL                b_high_prio,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_heit_pf,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_heit_pf_by_svc_id (
                    HANDLE_T            h_sdt,
                    UINT16              ui2_svc_id, 
                    BOOL                b_high_prio,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_heit_pf,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_heit_s (
                    HANDLE_T            h_root,
                    UINT16              ui2_on_id,
                    UINT16              ui2_ts_id,
                    UINT16              svc_id,
                    TIME_T              t_time,
                    BOOL                b_high_prio,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_heit_s,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_heit_s_by_idx (
                    HANDLE_T            h_sdt,
                    UINT16              ui2_idx,
                    TIME_T              t_time,
                    BOOL                b_high_prio,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_heit_s,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_heit_s_by_svc_id (
                    HANDLE_T            h_sdt,
                    UINT16              ui2_svc_id,
                    TIME_T              t_time,
                    BOOL                b_high_prio,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_heit_s,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_nit (
                    HANDLE_T            h_root,
                    UINT16              ui2_nw_id,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_nit,
                    TM_COND_T*          pe_cond
                    );                    
extern INT32    x_isdb_si_load_nit_actual (
                    HANDLE_T            h_root,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_nit,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_pcat (
                    HANDLE_T            h_root,
                    UINT16              ui2_on_id,
                    UINT16              ui2_ts_id,
                    UINT16              ui2_svc_id,
                    UINT32              ui4_content_id,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_pcat,
                    TM_COND_T*          pe_cond
                    );                    
extern INT32    x_isdb_si_load_bit (
                    HANDLE_T            h_root,
                    UINT16              ui2_on_id,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_bit,
                    TM_COND_T*          pe_cond
                    );                    
extern INT32    x_isdb_si_load_nbit (
                    HANDLE_T            h_root,
                    UINT16              ui2_on_id,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_nbit,
                    TM_COND_T*          pe_cond);
                    
extern INT32    x_isdb_si_load_ldt (
                    HANDLE_T            h_root,
                    UINT16              ui2_on_id,
                    UINT16              ui2_ts_id,
                    UINT16              ui2_svc_id,/*original_service_id*/
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_ldt,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_rst (
                    HANDLE_T            h_root,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_rst,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_sdt (
                    HANDLE_T            h_root,
                    UINT16              ui2_on_id,
                    UINT16              ui2_ts_id,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_sdt,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_group_ert (
                    HANDLE_T            h_root,
                    UINT16              ui2_info_provider_id,
                    UINT16              ui2_evt_relation_id,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_ert,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_seg_ert (
                    HANDLE_T            h_root,
                    UINT16              ui2_info_provider_id,
                    UINT16              ui2_evt_relation_id,
                    MPEG_2_PID_T        t_pid,
                    VOID*               pv_nfy_tag,
                    x_tm_nfy_fct        pf_nfy,
                    HANDLE_T*           ph_ert,
                    TM_COND_T*          pe_cond
                    );
extern INT32    x_isdb_si_load_group_lit (
                    HANDLE_T        h_root,
                    UINT16          ui2_on_id,
                    UINT16          ui2_ts_id,
                    UINT16          ui2_svc_id,
                    UINT16          ui2_evt_id,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_lit,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_load_group_lit_by_idx (
                    HANDLE_T        h_eit,
                    UINT16          ui2_idx,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_lit,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_load_group_lit_by_evt_id (
                    HANDLE_T        h_eit,
                    UINT16          ui2_evt_id,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_lit,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_load_seg_lit (
                    HANDLE_T        h_root,
                    UINT16          ui2_on_id,
                    UINT16          ui2_ts_id,
                    UINT16          ui2_svc_id,
                    UINT16          ui2_evt_id,
                    MPEG_2_PID_T    t_pid,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_lit,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_load_seg_lit_by_idx (
                    HANDLE_T        h_eit,
                    UINT16          ui2_idx,
                    MPEG_2_PID_T    t_pid,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_lit,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_load_seg_lit_by_evt_id (
                    HANDLE_T        h_eit,
                    UINT16          ui2_evt_id,
                    MPEG_2_PID_T    t_pid,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_lit,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_load_seg_itt (
                    HANDLE_T        h_root,
                    UINT16          ui2_on_id,
                    UINT16          ui2_ts_id,
                    UINT16          ui2_svc_id,
                    UINT16          ui2_evt_id,
                    MPEG_2_PID_T    t_pid,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_itt,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_load_seg_itt_by_idx (
                    HANDLE_T        h_eit,
                    UINT16          ui2_idx,
                    MPEG_2_PID_T    t_pid,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_itt,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_load_seg_itt_by_evt_id (
                    HANDLE_T        h_eit,
                    UINT16          ui2_evt_id,
                    MPEG_2_PID_T    t_pid,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_itt,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_load_sdt_by_idx (
                    HANDLE_T        h_bat_nit,
                    UINT16          ui2_idx,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_sdt,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_load_sdt_by_ts_descr (
                    HANDLE_T        h_root,
                    TS_DESCR_T*     pt_ts_descr,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_sdt,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_load_sdt_actual (
                    HANDLE_T        h_root,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_sdt,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_load_tdt (
                    HANDLE_T        h_root,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_tdt,
                    TM_COND_T*      pe_cond
                    );/*No support*/
extern INT32    x_isdb_si_load_tot (
                    HANDLE_T        h_root,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_tot,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_mon_bat (
                    HANDLE_T        h_root,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_bat_mon,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_mon_nit (
                    HANDLE_T        h_root,
                    BOOL            b_actual_ts,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_nit_mon,
                    TM_COND_T*      pe_cond
                    );
extern INT32    x_isdb_si_mon_sdt (
                    HANDLE_T        h_root,
                    BOOL            b_actual_ts,
                    VOID*           pv_nfy_tag,
                    x_tm_nfy_fct    pf_nfy,
                    HANDLE_T*       ph_sdt_mon,
                    TM_COND_T*      pe_cond
                    );

#endif /* _X_ISDB_SI_ENG_H_ */

