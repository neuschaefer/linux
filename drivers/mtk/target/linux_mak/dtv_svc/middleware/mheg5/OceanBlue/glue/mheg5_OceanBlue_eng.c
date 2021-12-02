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
#include "inc/u_common.h"
#include "os/inc/x_os.h"
#include "mheg5/x_mheg5.h"
#include "strm_mngr/scc/u_scc.h"
#include "res_mngr/drv/u_irrc_btn_def.h"

#include "mheg5/OceanBlue/glue/state_mngr/state_mngr.h"
#include "mheg5/OceanBlue/glue/si_loader/sil.h"
#include "mheg5/OceanBlue/glue/strm_ctrl/sct.h"
#include "mheg5/OceanBlue/glue/gui_render/grd.h"
#include "mheg5/OceanBlue/glue/ssf/mheg5_section.h"
#include "mheg5/OceanBlue/glue/misc/mheg5_misc.h"
#include "mheg5/OceanBlue/glue/ci/mheg5_ci.h"
#ifdef MHEG5_IC_SUPPORT
#include "mheg5/OceanBlue/glue/ic/mheg5_ic.h"
#endif
#include "mheg5/OceanBlue/glue/mheg5_OceanBlue_eng.h"

/* OceanBlue header files. */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_errors.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_misc.h"

#include "chip_spec/x_chip_spec.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define DEFAULT_GUI_WIDTH   ((INT32) 720)
#define DEFAULT_GUI_HEIGHT  ((INT32) 576)

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
static UINT32 _ui4_eng_code;

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 mheg5_obs_eng_msg_proc(MHEG5_MSG_T* pt_msg);


#ifdef CHIP_SPEC_SUPPORT
/* linux platform should use anther way to get chip bounding.*/
/* extern INT32 IS_MHEG5Support(void); */
#else
extern INT32 IS_MHEG5Support(void);
#endif

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
INT32 x_mheg5_OceanBlue_stack_init(
    const CHAR*                         ps_eng_name,
    const CHAR*                         ps_tuner_name,
    MHEG5_DISPLAY_ASPECT_RATIO_T        e_display_ar,
    const SCC_VID_OVER_SCAN_CLIPPER_T*  pt_ovs_clip)
{
    MHEG5_FUNC_TBL_T    t_ftbl;
    INT32               i4_ret;
    
#ifdef CHIP_SPEC_SUPPORT
    BOOL b_chip_mheg5_bond = FALSE;
#endif    
    i4_ret = MHEG5R_INIT_FAIL;

#ifdef CHIP_SPEC_SUPPORT
    /* linux platform should use anther way to get chip bounding.*/
    i4_ret = x_chip_spec_get_MHEG5_bond (&b_chip_mheg5_bond);
    if ((CHIPSPECR_OK == i4_ret) &&
        (TRUE == b_chip_mheg5_bond))
#else
    if (IS_MHEG5Support())
#endif
    {
        i4_ret = MHEG5R_INV_ARG;

        if (ps_eng_name && ps_tuner_name)
        {
            t_ftbl.pf_load_si                  = sil_load;
            t_ftbl.pf_unload_si                = sil_unload;
            t_ftbl.pf_get_display_adjustment   = sct_get_display_adjustment;
            t_ftbl.pf_query_display_adjustment = sct_query_display_adjustment;
            t_ftbl.pf_query_display_adjustment_ex = sct_query_display_adjustment_ex;
            t_ftbl.pf_query_display_b_auto_mode   = sct_query_display_b_auto_mode;
            t_ftbl.pf_evaluate                 = sct_evaluate;
            t_ftbl.pf_resolve                  = sil_resolve;
            t_ftbl.pf_start_service            = sct_start;
            t_ftbl.pf_resume_service           = sct_resume;
            t_ftbl.pf_stop_service             = sct_stop;
            t_ftbl.pf_pause_service            = sct_pause;
            t_ftbl.pf_init_osd                 = grd_init_osd;
            t_ftbl.pf_show                     = grd_show;
            t_ftbl.pf_adjust_osd               = grd_adjust_osd;
            t_ftbl.pf_adjust_osd_ex            = grd_adjust_osd_ex;
            t_ftbl.pf_set_def_lang             = sct_set_default_lang;
            t_ftbl.pf_set_gui_lang             = sct_set_gui_lang;
            t_ftbl.pf_msg_proc                 = mheg5_obs_eng_msg_proc;
            t_ftbl.pf_set_vdp_mode             = sct_set_vdp_mode;
            t_ftbl.pf_set_video_info           = sct_set_video_info;
            t_ftbl.pf_set_visibility           = grd_set_visibility;
            t_ftbl.pf_update_screen_mode       = sct_update_screen_mode;
            t_ftbl.pf_adjust_freeze_mode       = sct_adjust_freeze_mode;
            t_ftbl.pf_set_dvb_locator          = sct_set_dvb_locator;
            t_ftbl.pf_adjust_order             = grd_adjust_order;
            t_ftbl.pf_mheg5_open               = stmg_mheg5_open;
            t_ftbl.pf_mheg5_close              = stmg_mheg5_close;
            t_ftbl.pf_key_press                = mheg5_misc_notify_key_press;
            t_ftbl.pf_start_exec               = stmg_mheg5_start_exec;
            t_ftbl.pf_stop_exec                = stmg_mheg5_stop_exec;
            t_ftbl.pf_pause_exec               = stmg_mheg5_pause_exec;
            t_ftbl.pf_reg_eng_state_nfy        = grd_reg_state_nfy;
            t_ftbl.pf_reg_video_update_nfy     = sct_reg_video_update_nfy;
            t_ftbl.pf_reg_sbtl_visibility_nfy  = sct_reg_sbtl_visibility_nfy;         /** to suport simultaneous MHEG5 and subtitle **/
            t_ftbl.pf_set_sbtl_cfg_info        = sct_set_sbtl_cfg_info;         /** to suport simultaneous MHEG5 and subtitle **/
            t_ftbl.pf_reg_tune_method_update_nfy = sil_reg_tune_method_update_nfy;
            t_ftbl.pf_reg_tune_svc_cb_fct      = sil_reg_tune_svc_cb_fct;
        #ifdef CI_PLUS_SUPPORT
            t_ftbl.pf_reg_ci_nfy               = mheg5_ci_reg_nfy;
            t_ftbl.pf_start_ci_app             = mheg5_ci_start_ci_app;
            t_ftbl.pf_stop_ci_apps             = mheg5_ci_stop_ci_apps;
        #endif
            t_ftbl.pf_apply_supported_prof     = sil_apply_supported_prof;
        #ifdef MHEG5_IC_SUPPORT
            t_ftbl.pf_set_ic_status                = mheg5_ic_nfy_state_changed;
            t_ftbl.pf_reg_ics_guid_visibility_nfy  = mheg5_ics_reg_guid_visibility_nfy;
            t_ftbl.pf_set_ics_guid_result          = mheg5_ics_set_eng_guid_result;
            t_ftbl.pf_reg_pin_support_cb_fct       = mheg5_ic_reg_pin_support_cb_fct;
            t_ftbl.pf_notify_audio_desc_pref_changed= sct_audio_desc_pref_changed;
            t_ftbl.pf_reg_ad_cb_fct                 = sct_reg_ad_cb_fct;
        #endif
            t_ftbl.pf_reg_vid_info_update_nfy      = sct_reg_vid_info_update_nfy;
            t_ftbl.pf_get_vid_info                 = sct_get_vid_info;
            i4_ret = x_mheg5_register_eng(
                ps_eng_name, 
                &t_ftbl, 
                &_ui4_eng_code);

            if (MHEG5R_OK == i4_ret)
            {
                mheg5_section_init(ps_tuner_name);

                #ifdef CI_PLUS_SUPPORT
                i4_ret = mheg5_ci_init();
                #else
                i4_ret = MHEG5R_OK;
                #endif

                if (MHEG5R_OK == i4_ret)
                {
                    i4_ret = sil_init(
                        _ui4_eng_code,
                        ps_tuner_name);

                    if (MHEG5R_OK == i4_ret)
                    {
    	                i4_ret = grd_init(
    	                    _ui4_eng_code,
    	                    pt_ovs_clip);
    	                
    	                if (MHEG5R_OK == i4_ret)
    	                {
    	                    i4_ret = sct_init(
    	                        _ui4_eng_code,
    	                        e_display_ar,
    	                        pt_ovs_clip);

                            if (MHEG5R_OK == i4_ret)
                            {
                                i4_ret = stmg_init();
                            }
    	                }
                    }
                }
                
                if (MHEG5R_OK != i4_ret)
                {
                    x_mheg5_unregister_eng(ps_eng_name);
                }
            }
        }
    }
    
    return (i4_ret);
}

INT32 mheg5_obs_convert_err_code(
    tmMHEG5Err_t            t_err)
{
    switch (t_err)
    {
        case MHEG5_OK:
            return MHEG5R_OK;
            
        case MHEG5_ERR_OTHER:
            return MHEG5R_INTERNAL_ERROR;

        case MHEG5_ERR_BAD_PARAMETER:
            return MHEG5R_INV_ARG;

        case MHEG5_ERR_CI_SESSION_NOT_ESTABLISHED:
            return MHEG5R_INTERNAL_ERROR;

        case MHEG5_ERR_ALLOCATING_MEMORY_REGION:
            return MHEG5R_OUT_OF_MEMORY;
            
        case MHEG5_ERR_COMP_NOT_OPEN:
            return MHEG5R_COMP_NOT_OPEN;

        case MHEG5_ERR_COMP_ALREADY_OPEN:
            return MHEG5R_COMP_ALREADY_OPEN;

        case MHEG5_ERR_COMP_NOT_STARTED:
            return MHEG5R_COMP_NOT_STARTED;

        case MHEG5_ERR_COMP_ALREADY_STARTED:
            return MHEG5R_COMP_ALREADY_STARTED;

        default:
            return MHEG5R_INTERNAL_ERROR;
    }
}

    

tmMHEG5KeyPress_t mheg5_obs_convert_key_code(
    UINT32                  ui4_key)
{
    switch (ui4_key)
    {
        case BTN_RED:
            return MHEG5_KEYPRESS_RED;
            
        case BTN_GREEN:
            return MHEG5_KEYPRESS_GREEN;
            
        case BTN_YELLOW:
            return MHEG5_KEYPRESS_YELLOW;
            
        case BTN_BLUE:
            return MHEG5_KEYPRESS_BLUE;
            
        case BTN_TEXT:
            return MHEG5_KEYPRESS_TEXT;
            
        case BTN_EXIT:
            return MHEG5_KEYPRESS_CANCEL;

        case BTN_CURSOR_UP:
            return MHEG5_KEYPRESS_UP;
            
        case BTN_CURSOR_DOWN:
            return MHEG5_KEYPRESS_DOWN;
            
        case BTN_CURSOR_LEFT:
            return MHEG5_KEYPRESS_LEFT;
            
        case BTN_CURSOR_RIGHT:
            return MHEG5_KEYPRESS_RIGHT;
            
        case BTN_SELECT:
            return MHEG5_KEYPRESS_SELECT;
        
        case BTN_DIGIT_0:
            return MHEG5_KEYPRESS_0;
            
        case BTN_DIGIT_1:
            return MHEG5_KEYPRESS_1;

        case BTN_DIGIT_2:
            return MHEG5_KEYPRESS_2;
            
        case BTN_DIGIT_3:
            return MHEG5_KEYPRESS_3;
            
        case BTN_DIGIT_4:
            return MHEG5_KEYPRESS_4;
            
        case BTN_DIGIT_5:
            return MHEG5_KEYPRESS_5;

        case BTN_DIGIT_6:
            return MHEG5_KEYPRESS_6;
            
        case BTN_DIGIT_7:
            return MHEG5_KEYPRESS_7;
            
        case BTN_DIGIT_8:
            return MHEG5_KEYPRESS_8;
            
        case BTN_DIGIT_9:
            return MHEG5_KEYPRESS_9;

        case BTN_EPG:
            return MHEG5_KEYPRESS_EPG;

        case BTN_PRG_INFO:
            return MHEG5_KEYPRESS_INFO;

        default:
            /* should not arrive here */
            /*MHEG5_ASSERT(0);*/
            break;
    }

    return ((tmMHEG5KeyPress_t) -1);
}





/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
static INT32 mheg5_obs_eng_msg_proc(MHEG5_MSG_T* pt_msg)
{
    INT32 i4_ret = MHEG5R_INV_ARG;
    
    if (pt_msg)
    {
        i4_ret = MHEG5R_OK;
        
        switch (pt_msg->ui1_module)
        {
            case MHEG5_MOD_SIL:
                i4_ret = sil_proc_msg(pt_msg);
                break;

            case MHEG5_MOD_SCT:
                i4_ret = sct_proc_msg(pt_msg);
                break;

            case MHEG5_MOD_CI: 
                #ifdef CI_PLUS_SUPPORT
                i4_ret = mheg5_ci_proc_msg(pt_msg);
                #endif
                break;

            default:
                break;
        }
    }

    return (i4_ret);
}

