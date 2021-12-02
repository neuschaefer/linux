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
#ifndef _X_MHEG5_H_
#define _X_MHEG5_H_
/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_mheg5.h"
#include "x_scc.h"
#include "u_scc.h"
#include "u_wgl.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
extern INT32 x_mheg5_adjust_freeze_mode(UINT32 ui4_eng_code);

extern INT32 x_mheg5_adjust_order(
    UINT32                      ui4_eng_code,
    HANDLE_T                    h_ref_widget,
    WGL_INSERT_MODE_T           e_insert);

extern INT32 x_mheg5_evaluate_stream(UINT32 ui4_eng_code);

extern INT32 x_mheg5_get_display_adjustment(
    UINT32                      ui4_eng_code,
    UINT8                       ui1_afd,
    MHEG5_ASPECT_RATIO_T        e_src_aspect_ratio,
    SCC_VID_SCREEN_MODE_T       e_screen_mode,
    MHEG5_VIDEO_MODE_T          e_vid_mode,
    VSH_REGION_INFO_T*          pt_src_region, 
    VSH_REGION_INFO_T*          pt_dst_region);

extern INT32 x_mheg5_get_eng_code(
    const CHAR* ps_eng_name,
    UINT32*     pui4_eng_code);

extern SCC_VID_SCREEN_MODE_T x_mheg5_get_screen_auto_mode_enum(VOID);

extern INT32 x_mheg5_init_osd(
    UINT32                      ui4_eng_code,
    HANDLE_T                    h_mheg5_canvas,
    HANDLE_T                    h_sbtl_canvas,
    GL_HPALETTE_T               h_palette,
    const GL_COLOR_T*           pt_color_key);

extern INT32 x_mheg5_reg_eng_state_nfy(
    UINT32                      ui4_eng_code,
    x_mheg5_eng_state_nfy_fct   pf_state_nfy,
    VOID*                       pv_tag);


extern INT32 x_mheg5_load_si(
    UINT32                      ui4_eng_code,
    HANDLE_T                    h_svctx,
    UINT16                      ui2_svl_id,
    UINT16                      ui2_svl_rec_id,
    x_mheg5_eng_load_nfy_fct    pf_load_nfy,
    VOID*                       pv_tag);

extern INT32 x_mheg5_pause_service(UINT32 ui4_eng_code);

extern INT32 x_mheg5_query_display_adjustment(
    UINT32                          ui4_eng_code,
    const MHEG5_DISPLAY_PARAM_T*    pt_dspy_param,
    VSH_REGION_INFO_T*              pt_src_region, 
    VSH_REGION_INFO_T*              pt_dst_region);

extern INT32 x_mheg5_query_display_adjustment_ex(
    UINT32                          ui4_eng_code,
    MHEG5_DISPLAY_PARAM_T*          pt_dspy_param,
    VSH_REGION_INFO_T*              pt_src_region, 
    VSH_REGION_INFO_T*              pt_dst_region);

extern BOOL x_mheg5_query_display_b_auto_mode(
    UINT32                          ui4_eng_code);


extern INT32 x_mheg5_register_eng(
    const CHAR*         ps_eng_name,
    MHEG5_FUNC_TBL_T*   pt_ftbl,
    UINT32*             pui4_eng_code);

extern INT32 x_mheg5_resolve(
    UINT32                  ui4_eng_code,
    MHEG5_STREAM_DESC_T*    pt_strm_desc,
    x_mheg5_resolve_nfy     pf_resolve_nfy,
    VOID*                   pv_tag,
    STREAM_PID_INFO_T*      pt_pid_info);

extern INT32 x_mheg5_resume_service(UINT32 ui4_eng_code);

extern INT32 x_mheg5_send_msg(
    UINT32          ui4_eng_code,
    MHEG5_MSG_T*    pt_msg,
    UINT16          ui2_retry);

extern INT32 x_mheg5_adjust_osd(
    UINT32                  ui4_eng_code,
    const MHEG5_CNV_CFG_T*  pt_cnv_cfg);

extern INT32 x_mheg5_adjust_osd_ex(
    UINT32                        ui4_eng_code,
    const MHEG5_CNV_CFG_T*        pt_cnv_cfg,
    const VSH_REGION_INFO_T*      pt_orig_src_reg,
    const VSH_REGION_INFO_T*      pt_full_src_reg,
    const VSH_REGION_INFO_T*      pt_full_dst_reg,
    const VSH_REGION_INFO_T*      pt_xlat_src_reg,
    const VSH_REGION_INFO_T*      pt_xlat_dst_reg);



extern INT32 x_mheg5_set_gui_lang(
    UINT32          ui4_eng_code,
    ISO_639_LANG_T  s639_lang);

extern INT32 x_mheg5_set_default_lang(
    UINT32          ui4_eng_code,
    ISO_639_LANG_T  s639_lang);

extern VOID x_mheg5_set_screen_auto_mode_enum(SCC_VID_SCREEN_MODE_T e_auto_mode);

extern INT32 x_mheg5_show(
    UINT32                      ui4_eng_code,
    MHEG5_COEXIST_DISPLAY_T     e_display,
    BOOL                        b_get_focus);

extern INT32 x_mheg5_start_service(
    UINT32                      ui4_eng_code,
    HANDLE_T                    h_svctx,
    UINT16                      ui2_svl_id,
    UINT16                      ui2_svl_rec_id);

extern INT32 x_mheg5_stop_service(UINT32 ui4_eng_code, MHEG5_STOP_SVC_PARAM_T *pt_param);

extern INT32 x_mheg5_strm_force_no_act(BOOL b_force);

extern INT32 x_mheg5_unload_si(UINT32 ui4_eng_code);

extern INT32 x_mheg5_unregister_eng(const CHAR* ps_eng_name);

extern INT32 x_mheg5_update_screen_mode(
    UINT32                  ui4_eng_code,
    SCC_VID_SCREEN_MODE_T   e_screen_mode);

extern BOOL x_mheg5_with_internal_screen_mode(VOID);

extern INT32 x_mheg5_set_dvb_locator(UINT16 ui2_on_id, UINT16 ui2_ts_id, UINT16 ui2_svc_id);

extern INT32 x_mheg5_set_vdp_mode(
    UINT32          ui4_eng_code,
    SCC_VID_MODE_T  e_mode,
    BOOL            b_resume);

extern INT32 x_mheg5_set_video_info(
    UINT32          ui4_eng_code);

extern INT32 x_mheg5_reg_video_update_nfy(
    UINT32                   ui4_eng_code,
    x_mheg5_video_update_nfy pf_nfy,
    VOID*                    pv_tag);

extern INT32 x_mheg5_reg_sbtl_visibility_nfy(
    UINT32                      ui4_eng_code,
    x_mheg5_sbtl_visibility_nfy pf_nfy,
    VOID*                       pv_tag);

extern INT32 x_mheg5_set_subtitle_info(
    UINT32                 ui4_eng_code,
    MHEG5_SUBTITLE_CFG_T*  pt_cfg);

extern INT32 x_mheg5_set_visibility(
    UINT32          ui4_eng_code,
    WGL_SW_CMD_T    e_sw_cmd);

extern INT32 x_mheg5_set_default_overscan(SCC_VID_OVER_SCAN_CLIPPER_T *pt_clipper);

extern INT32 x_mheg5_reg_tune_method_update_nfy(
    UINT32                         ui4_eng_code,
    x_mheg5_tune_method_update_nfy pf_tune_method_update_nfy,
    VOID*                          pv_tag);


extern INT32 x_mheg5_reg_tune_svc_cb_fct(
    UINT32               ui4_eng_code,
    x_mheg5_tune_svc_fct pf_tune_svc_fct,
    VOID*                pv_tag);

extern INT32 x_mheg5_open(
    UINT32 ui4_eng_code,
    MHEG5_SETUP_T* pt_setup);

extern INT32 x_mheg5_close(
    UINT32 ui4_eng_code);

extern INT32 x_mheg5_key_press(
    UINT32 ui4_eng_code,
    UINT32 ui4_key_code,
    BOOL*  pb_in_func_grp,
    BOOL   b_process);
    
extern INT32 x_mheg5_start_execution(
    UINT32 ui4_eng_code,
    MHEG5_START_EXEC_T* pt_start_exec);

extern INT32 x_mheg5_stop_execution(
    UINT32 ui4_eng_code,
    MHEG5_STOP_EXEC_T* pt_stop_exec);
    
extern INT32 x_mheg5_pause_execution(
    UINT32 ui4_eng_code,
    MHEG5_PAUSE_EXEC_T* pt_pause_exec);


#ifdef CI_PLUS_SUPPORT
extern INT32 x_mheg5_reg_ci_nfy(
    UINT32            ui4_eng_code,
    x_mheg5_ci_nfy    pf_func,
    VOID*             pv_tag);

extern INT32 x_mheg5_start_ci_application(
    UINT32            ui4_eng_code,
    MHEG5_CI_START_APP_T* pt_start_ci_app);

extern INT32 x_mheg5_stop_ci_applications(
    UINT32            ui4_eng_code,
    MHEG5_CI_STOP_APPS_T*  pt_stop_ci_apps);
#endif

extern INT32 x_mheg5_apply_supported_profile(
    UINT32              ui4_eng_code,
    ISO_3166_COUNT_T    s3166_country,
    MHEG5_PROFILE_ID_T* pe_profile_id,
    BOOL                b_apply);

#ifdef MHEG5_IC_SUPPORT
extern INT32 x_mheg5_set_ic_status(
    UINT32            ui4_eng_code,
    MHEG5_IC_STATE_T  e_new_ic_status);

extern INT32 x_mheg5_reg_ics_guid_visibility_nfy(
    UINT32                          ui4_eng_code,
    x_mheg5_ics_guid_visibility_nfy pf_nfy,
    VOID*                           pv_tag);

extern INT32 x_mheg5_set_ics_guid_result(
    UINT32          ui4_eng_code,
    BOOL            b_result);

extern INT32 x_mheg5_reg_pin_support_cb_fct(
    UINT32                  ui4_eng_code,
    x_mheg5_pin_support_fct pf_pin_support_fct,
    VOID*                   pv_tag);

extern INT32 x_mheg5_notify_audio_desc_pref_changed(UINT32 ui4_eng_code);

extern INT32 x_mheg5_reg_audio_desc_cb_fct(
    UINT32               ui4_eng_code,
    x_mheg5_ad_fct       pf_ad_fct,
    VOID*                pv_tag);

#endif /*MHEG5_IC_SUPPORT*/

#ifdef MHEG5_NVM_SUPPORT
extern INT32 x_mheg5_init_tps(
	#ifdef LINUX_TURNKEY_SOLUTION
		VOID
	#else
		const CHAR* ps_tps_dev
	#endif
);

extern INT32 x_mheg5_clear_tps(VOID);
#endif

extern INT32 x_mheg5_reg_vid_info_update_nfy(
        UINT32                         ui4_eng_code,
        x_mheg5_vid_info_update_nfy    pf_func,
        VOID*                          pv_tag);

extern INT32 x_mheg5_get_vid_info(
        UINT32                  ui4_eng_code,
        MHEG5_VID_INFO_T*       pt_vid_info);
#endif /* _U_MHEG5_H_ */


