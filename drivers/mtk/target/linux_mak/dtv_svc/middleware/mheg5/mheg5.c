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
#include "start/x_start_common.h"
#include "os/inc/x_os.h"
#include "mheg5/mheg5.h"
#include "mheg5/OceanBlue/glue/strm_ctrl/sct.h"
#include "mheg5/OceanBlue/glue/gui_render/grd.h"
#include "mheg5/mheg5_cli.h"
#include "chip_spec/x_chip_spec.h"
#ifdef MHEG5_NVM_SUPPORT
#include "mheg5/OceanBlue/glue/nvm/mheg5_nvm.h"
#endif
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define MHEG5_MSG_Q_NAME                "mheg5_msg_q"
#define MHEG5_THREAD_NAME               "mw_mheg5_th"
#define MHEG5_NUM_OF_MSGS               ((UINT16) 64)
#define MHEG5_MSG_DEFAULT_PRIORITY      ((UINT8) 128)

#define MHEG5_THREAD_DEFAULT_PRIORITY   ((UINT8)   128)
#define MHEG5_THREAD_DEFAULT_STACK_SIZE ((SIZE_T) 4096) 

#define MHEG5_MAX_ENG_NAME              32

#define MIN(x, y)                       (((x) < (y)) ? (x) : (y))

typedef struct __MHEG5_ENGINE_T
{
    CHAR    sz_name[MHEG5_MAX_ENG_NAME];
    UINT32  ui4_code;

    MHEG5_FUNC_TBL_T    t_ftbl;
} _MHEG5_ENGINE_T;

typedef struct __MHEG5_INTL_MSG_T
{
    UINT32      ui4_eng_code;
    MHEG5_MSG_T t_mheg5_msg;
} _MHEG5_INTL_MSG_T;

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
static HANDLE_T         _h_mheg5_thread;
static HANDLE_T         _h_mheg5_msg_q;

static _MHEG5_ENGINE_T  _t_engine;
static UINT16           _ui2_max_eng_num;
static UINT16           _ui2_reg_eng_num;
static BOOL             _b_init = FALSE;
static UINT32           _ui4_count;

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static BOOL _check_func_tbl(MHEG5_FUNC_TBL_T* pt_ftbl);

static _MHEG5_ENGINE_T* _find_engine_by_code(UINT32 ui4_eng_code);

static _MHEG5_ENGINE_T* _find_engine_by_name(const CHAR* ps_eng_name);
    
static _MHEG5_ENGINE_T* _new_engine(
    const CHAR*         ps_eng_name,
    MHEG5_FUNC_TBL_T*   pt_ftbl,
    UINT32*             pui4_eng_code);

static BOOL _init_context(THREAD_DESCR_T* pt_thread_desc);

static VOID _mheg5_thread_main(VOID *pv_arg);

static VOID _remove_engine(_MHEG5_ENGINE_T* pt_eng);

#ifdef CHIP_SPEC_SUPPORT
/* linux platform should use anther way to get chip bounding.*/
/* extern INT32 IS_MHEG5Support(void); */
#else
extern INT32 IS_MHEG5Support(void);
#endif
/*-----------------------------------------------------------------------------
 * exported methods implementations
 *---------------------------------------------------------------------------*/
INT32 x_mheg5_adjust_order(UINT32 ui4_eng_code, HANDLE_T h_ref_widget, WGL_INSERT_MODE_T e_insert)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        (*(pt_eng->t_ftbl.pf_adjust_order))(h_ref_widget, e_insert);

        i4_ret = MHEG5R_OK;
    }

    return i4_ret;
}

INT32 x_mheg5_adjust_osd(
    UINT32                  ui4_eng_code,
    const MHEG5_CNV_CFG_T*  pt_cnv_cfg)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_adjust_osd))(pt_cnv_cfg);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_adjust_osd_ex(
    UINT32                        ui4_eng_code,
    const MHEG5_CNV_CFG_T*        pt_cnv_cfg,
    const VSH_REGION_INFO_T*      pt_orig_src_reg,
    const VSH_REGION_INFO_T*      pt_full_src_reg,
    const VSH_REGION_INFO_T*      pt_full_dst_reg,
    const VSH_REGION_INFO_T*      pt_xlat_src_reg,
    const VSH_REGION_INFO_T*      pt_xlat_dst_reg)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_adjust_osd_ex))(pt_cnv_cfg,
                                                      pt_orig_src_reg,
                                                      pt_full_src_reg,
                                                      pt_full_dst_reg,
                                                      pt_xlat_src_reg,
                                                      pt_xlat_dst_reg);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_adjust_freeze_mode(UINT32 ui4_eng_code)
{
    INT32               i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T     *pt_eng;

    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_adjust_freeze_mode();
    }

    return i4_ret;
}

INT32 x_mheg5_evaluate_stream(UINT32 ui4_eng_code)
{
    INT32            i4_ret;
    _MHEG5_ENGINE_T* pt_eng;
    
    i4_ret = MHEG5R_ENG_NOT_FOUND;

    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_evaluate))();
    }
    
    return (i4_ret);
}

INT32 x_mheg5_get_display_adjustment(
    UINT32                      ui4_eng_code,
    UINT8                       ui1_afd,
    MHEG5_ASPECT_RATIO_T        e_src_aspect_ratio,
    SCC_VID_SCREEN_MODE_T       e_screen_mode,
    MHEG5_VIDEO_MODE_T          e_vid_mode,
    VSH_REGION_INFO_T*          pt_src_region, 
    VSH_REGION_INFO_T*          pt_dst_region)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_get_display_adjustment))(
            ui1_afd,
            e_src_aspect_ratio,
            e_screen_mode,
            e_vid_mode,
            pt_src_region,
            pt_dst_region);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_get_eng_code(
    const CHAR* ps_eng_name,
    UINT32*     pui4_eng_code)
{
    _MHEG5_ENGINE_T* pt_eng;
    INT32            i4_ret = MHEG5R_INV_STATE;
    
    if (_b_init)
    {
        i4_ret = MHEG5R_INV_ARG;
        
        if (ps_eng_name && pui4_eng_code)
        {
            i4_ret = MHEG5R_ENG_NOT_FOUND;
            
            pt_eng = _find_engine_by_name(ps_eng_name);

            if (pt_eng)
            {
                *pui4_eng_code = pt_eng->ui4_code;
                i4_ret = MHEG5R_OK;
            }
        }
    }
    
    return (i4_ret);
}

SCC_VID_SCREEN_MODE_T x_mheg5_get_screen_auto_mode_enum(VOID)
{
    return sct_get_screen_auto_mode_enum();
}

INT32 x_mheg5_init_osd(
    UINT32                      ui4_eng_code,
    HANDLE_T                    h_mheg5_canvas,
    HANDLE_T                    h_sbtl_canvas,
    GL_HPALETTE_T               h_palette,
    const GL_COLOR_T*           pt_color_key)
{
    INT32            i4_ret = MHEG5R_INV_ARG;
    _MHEG5_ENGINE_T* pt_eng;
    
    if (NULL_HANDLE != h_mheg5_canvas && NULL_HANDLE != h_sbtl_canvas)
    {
        i4_ret = MHEG5R_ENG_NOT_FOUND;

        pt_eng = _find_engine_by_code(ui4_eng_code);

        if (pt_eng)
        {
            i4_ret = (*(pt_eng->t_ftbl.pf_init_osd))(
                h_mheg5_canvas,
                h_sbtl_canvas,
                h_palette,
                pt_color_key);
        }
    }
    
    return (i4_ret);
}

INT32 x_mheg5_reg_eng_state_nfy(
    UINT32                      ui4_eng_code,
    x_mheg5_eng_state_nfy_fct   pf_state_nfy,
    VOID*                       pv_tag)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_reg_eng_state_nfy))(
            pf_state_nfy,
            pv_tag);
    }
    
    return (i4_ret);
}
        
INT32 x_mheg5_load_si(
    UINT32                      ui4_eng_code,
    HANDLE_T                    h_svctx,
    UINT16                      ui2_svl_id,
    UINT16                      ui2_svl_rec_id,
    x_mheg5_eng_load_nfy_fct    pf_load_nfy,
    VOID*                       pv_tag)
{
    INT32            i4_ret = MHEG5R_INV_ARG;
    _MHEG5_ENGINE_T* pt_eng;
    
    /*if (pf_load_nfy)*/
    {
        i4_ret = MHEG5R_ENG_NOT_FOUND;

        pt_eng = _find_engine_by_code(ui4_eng_code);

        if (pt_eng)
        {
            i4_ret = (*(pt_eng->t_ftbl.pf_load_si))(
                h_svctx, 
                ui2_svl_id, 
                ui2_svl_rec_id,
                pf_load_nfy,
                pv_tag);
        }
    }
    
    return (i4_ret);
}

INT32 x_mheg5_pause_service(UINT32 ui4_eng_code)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_pause_service))();
    }
    
    return (i4_ret);
}

INT32 x_mheg5_query_display_adjustment(
    UINT32                          ui4_eng_code,
    const MHEG5_DISPLAY_PARAM_T*    pt_dspy_param,
    VSH_REGION_INFO_T*              pt_src_region, 
    VSH_REGION_INFO_T*              pt_dst_region)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_query_display_adjustment))(
            pt_dspy_param,
            pt_src_region,
            pt_dst_region);
    }
    
    return (i4_ret);
}
INT32 x_mheg5_query_display_adjustment_ex(
    UINT32                          ui4_eng_code,
    MHEG5_DISPLAY_PARAM_T*          pt_dspy_param,
    VSH_REGION_INFO_T*              pt_src_region, 
    VSH_REGION_INFO_T*              pt_dst_region)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);
    
    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_query_display_adjustment_ex))(
            pt_dspy_param,
            pt_src_region,
            pt_dst_region);
    }
    
    return (i4_ret);
}

BOOL x_mheg5_query_display_b_auto_mode(
    UINT32                          ui4_eng_code)
{
    BOOL            b_ret = FALSE;
    _MHEG5_ENGINE_T* pt_eng;

    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        b_ret = (*(pt_eng->t_ftbl.pf_query_display_b_auto_mode))();
    }
    
    return (b_ret);
}


INT32 x_mheg5_register_eng(
    const CHAR*         ps_eng_name,
    MHEG5_FUNC_TBL_T*   pt_ftbl,
    UINT32*             pui4_eng_code)
{
    INT32 i4_ret = MHEG5R_INTERNAL_ERROR;
    
    if (_b_init && (_ui2_reg_eng_num < _ui2_max_eng_num))
    {
        if ((ps_eng_name) && 
            (ps_eng_name[0] != '\0') && 
            (_check_func_tbl(pt_ftbl)))
        {
            if (_new_engine(ps_eng_name, pt_ftbl, pui4_eng_code))
            {
                i4_ret = MHEG5R_OK;
            }
        }
        else
        {
            i4_ret = MHEG5R_INV_ARG;
        }
    }
    else
    {
        i4_ret = MHEG5R_INV_STATE;
    }

    return (i4_ret);
}

INT32 x_mheg5_resolve(
    UINT32                  ui4_eng_code,
    MHEG5_STREAM_DESC_T*    pt_strm_desc,
    x_mheg5_resolve_nfy     pf_resolve_nfy,
    VOID*                   pv_tag,
    STREAM_PID_INFO_T*      pt_pid_info)
{
    INT32            i4_ret = MHEG5R_INV_ARG;
    _MHEG5_ENGINE_T* pt_eng;
    
    if (pt_strm_desc && pf_resolve_nfy && pt_pid_info)
    {
        i4_ret = MHEG5R_ENG_NOT_FOUND;

        pt_eng = _find_engine_by_code(ui4_eng_code);

        if (pt_eng)
        {
            i4_ret = (*(pt_eng->t_ftbl.pf_resolve))(
                pt_strm_desc,
                pf_resolve_nfy,
                pv_tag,
                pt_pid_info);
        }
    }
    
    return (i4_ret);
}

INT32 x_mheg5_resume_service(UINT32 ui4_eng_code)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_resume_service))();
    }
    
    return (i4_ret);
}

INT32 x_mheg5_send_msg(
    UINT32          ui4_eng_code,
    MHEG5_MSG_T*    pt_msg,
    UINT16          ui2_retry)
{
    #define RETRY_WAIT  10
    
    _MHEG5_INTL_MSG_T   t_intl_msg;
    INT32               i4_ret = MHEG5R_INV_STATE;
    UINT16              ui2_retried = 0;
    
    if (_b_init)
    {
        i4_ret = MHEG5R_INV_ARG;
        
        if (pt_msg)
        {
            t_intl_msg.ui4_eng_code = ui4_eng_code;
            t_intl_msg.t_mheg5_msg  = (*pt_msg);

            i4_ret = MHEG5R_INTERNAL_ERROR;
            
            while (ui2_retried++ < ui2_retry)
            {
                if (x_msg_q_send(
                    _h_mheg5_msg_q,
                    &t_intl_msg,
                    sizeof(_MHEG5_INTL_MSG_T),
                    MHEG5_MSG_DEFAULT_PRIORITY) == OSR_OK)
                {
                    i4_ret = MHEG5R_OK;
                    break;
                }
                else
                {
                    /* Send message fail, wait 10 ms */
                    x_thread_delay(RETRY_WAIT);
                }
            }
        }
    }
    
    return (i4_ret);
}

INT32 x_mheg5_set_default_lang(
    UINT32          ui4_eng_code,
    ISO_639_LANG_T  s639_lang)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_set_def_lang))(s639_lang);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_set_gui_lang(
    UINT32          ui4_eng_code,
    ISO_639_LANG_T  s639_lang)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_set_gui_lang))(s639_lang);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_show(
    UINT32                      ui4_eng_code,
    MHEG5_COEXIST_DISPLAY_T     e_display,
    BOOL                        b_get_focus)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_show))(e_display, b_get_focus);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_start_service(
    UINT32                      ui4_eng_code,
    HANDLE_T                    h_svctx,
    UINT16                      ui2_svl_id,
    UINT16                      ui2_svl_rec_id)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_start_service))(
            h_svctx,
            ui2_svl_id,
            ui2_svl_rec_id);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_stop_service(UINT32 ui4_eng_code, MHEG5_STOP_SVC_PARAM_T *pt_param)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_stop_service))(pt_param);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_strm_force_no_act(BOOL b_force)
{
    return sct_force_no_action(b_force);
}

INT32 x_mheg5_unload_si(UINT32 ui4_eng_code)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_unload_si))();
    }
    
    return (i4_ret);
}

INT32 x_mheg5_unregister_eng(const CHAR* ps_eng_name)
{
    INT32 i4_ret = MHEG5R_INV_STATE;
    
    if (_b_init)
    {
        i4_ret = MHEG5R_INV_ARG;
        
        if ((ps_eng_name) && 
            (ps_eng_name[0] != '\0'))
        {
            _MHEG5_ENGINE_T* pt_eng;

            i4_ret = MHEG5R_ENG_NOT_FOUND;
            
            pt_eng = _find_engine_by_name(ps_eng_name);

            if (pt_eng)
            {
                _remove_engine(pt_eng);
                i4_ret = MHEG5R_OK;
            }
        }
    }

    return (i4_ret);
}

INT32 x_mheg5_update_screen_mode(
    UINT32                  ui4_eng_code,
    SCC_VID_SCREEN_MODE_T   e_screen_mode)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = MHEG5R_OK;

        pt_eng->t_ftbl.pf_update_screen_mode(e_screen_mode);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_set_dvb_locator(UINT16 ui2_on_id, UINT16 ui2_ts_id, UINT16 ui2_svc_id)
{
    return sct_set_dvb_locator(ui2_on_id, ui2_ts_id, ui2_svc_id);
}

VOID x_mheg5_set_screen_auto_mode_enum(SCC_VID_SCREEN_MODE_T e_auto_mode)
{
    sct_set_screen_auto_mode_enum(e_auto_mode);
}

INT32 x_mheg5_set_vdp_mode(
    UINT32          ui4_eng_code,
    SCC_VID_MODE_T  e_mode,
    BOOL            b_resume)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = MHEG5R_OK;

        pt_eng->t_ftbl.pf_set_vdp_mode(e_mode, b_resume);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_set_video_info(
    UINT32          ui4_eng_code)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = MHEG5R_OK;

        pt_eng->t_ftbl.pf_set_video_info();
    }
    
    return (i4_ret);
}

INT32 x_mheg5_reg_video_update_nfy(
    UINT32                   ui4_eng_code,
    x_mheg5_video_update_nfy pf_nfy,
    VOID*                    pv_tag)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_reg_video_update_nfy))(
            pf_nfy,
            pv_tag);
    }
    
    return (i4_ret);
}

extern INT32 x_mheg5_reg_sbtl_visibility_nfy(
    UINT32                              ui4_eng_code,
    x_mheg5_sbtl_visibility_nfy         pf_nfy,
    VOID*                               pv_tag)
{
    INT32       i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T*    pt_eng;

    pt_eng = _find_engine_by_code(ui4_eng_code);

    if(pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_reg_sbtl_visibility_nfy))(
            pf_nfy,
            pv_tag);
    }

    return (i4_ret);
}

extern INT32 x_mheg5_set_subtitle_info(
    UINT32                 ui4_eng_code,
    MHEG5_SUBTITLE_CFG_T*  pt_cfg)
{
    INT32       i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T*    pt_eng;

    pt_eng = _find_engine_by_code(ui4_eng_code);

    if(pt_eng)
    {
        i4_ret = (*(pt_eng->t_ftbl.pf_set_sbtl_cfg_info))(pt_cfg);
    }

    return (i4_ret);
}


INT32 x_mheg5_set_visibility(
    UINT32          ui4_eng_code,
    WGL_SW_CMD_T    e_sw_cmd)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_set_visibility(e_sw_cmd);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_set_default_overscan(SCC_VID_OVER_SCAN_CLIPPER_T *pt_clipper)
{
    sct_set_default_overscan(pt_clipper);
    grd_set_default_overscan(pt_clipper);

    return MHEG5R_OK;
}

BOOL x_mheg5_with_internal_screen_mode(VOID)
{
    return sct_is_in_mheg5_screen_mode();
}

INT32 x_mheg5_reg_tune_method_update_nfy(
    UINT32                         ui4_eng_code,
    x_mheg5_tune_method_update_nfy pf_tune_method_update_nfy,
    VOID*                          pv_tag)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_reg_tune_method_update_nfy(pf_tune_method_update_nfy, pv_tag);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_reg_tune_svc_cb_fct(
    UINT32               ui4_eng_code,
    x_mheg5_tune_svc_fct pf_tune_svc_fct,
    VOID*                pv_tag)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_reg_tune_svc_cb_fct(pf_tune_svc_fct, pv_tag);
    }
    
    return (i4_ret);
}




INT32 x_mheg5_open(
    UINT32 ui4_eng_code,
    MHEG5_SETUP_T* pt_setup)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_mheg5_open(pt_setup);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_close(
    UINT32 ui4_eng_code)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_mheg5_close();
    }
    
    return (i4_ret);
}

INT32 x_mheg5_key_press(
    UINT32 ui4_eng_code,
    UINT32 ui4_key_code,
    BOOL*  pb_in_func_grp,
    BOOL   b_process)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_key_press(ui4_key_code, pb_in_func_grp, b_process);
    }
    
    return (i4_ret);
}
    
INT32 x_mheg5_start_execution(
    UINT32 ui4_eng_code,
    MHEG5_START_EXEC_T* pt_start_exec)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_start_exec(pt_start_exec);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_stop_execution(
    UINT32 ui4_eng_code,
    MHEG5_STOP_EXEC_T* pt_stop_exec)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_stop_exec(pt_stop_exec);
    }
    
    return (i4_ret);
}
    
INT32 x_mheg5_pause_execution(
    UINT32 ui4_eng_code,
    MHEG5_PAUSE_EXEC_T* pt_pause_exec)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_pause_exec(pt_pause_exec);
    }
    
    return (i4_ret);
}


#ifdef CI_PLUS_SUPPORT
INT32 x_mheg5_reg_ci_nfy(
    UINT32            ui4_eng_code,
    x_mheg5_ci_nfy    pf_func,
    VOID*             pv_tag)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_reg_ci_nfy(pf_func, pv_tag);
    }
    
    return (i4_ret);

}

INT32 x_mheg5_start_ci_application(
    UINT32            ui4_eng_code,
    MHEG5_CI_START_APP_T* pt_start_ci_app)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_start_ci_app(pt_start_ci_app);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_stop_ci_applications(
    UINT32            ui4_eng_code,
    MHEG5_CI_STOP_APPS_T*  pt_stop_ci_apps)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_stop_ci_apps(pt_stop_ci_apps);
    }
    
    return (i4_ret);
}
#endif

INT32 x_mheg5_apply_supported_profile(
    UINT32              ui4_eng_code,
    ISO_3166_COUNT_T    s3166_country,
    MHEG5_PROFILE_ID_T* pe_profile_id,
    BOOL                b_apply)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_apply_supported_prof(s3166_country, pe_profile_id, b_apply);
    }
    
    return (i4_ret);

}

#ifdef MHEG5_IC_SUPPORT
INT32 x_mheg5_set_ic_status(
    UINT32            ui4_eng_code,
    MHEG5_IC_STATE_T  e_new_ic_status)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_set_ic_status(e_new_ic_status);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_reg_ics_guid_visibility_nfy(
    UINT32                          ui4_eng_code,
    x_mheg5_ics_guid_visibility_nfy pf_nfy,
    VOID*                           pv_tag)
{
    INT32 i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;

    pt_eng = _find_engine_by_code(ui4_eng_code);

    if(pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_reg_ics_guid_visibility_nfy(
            pf_nfy,
            pv_tag);
    }

    return (i4_ret);
}

INT32 x_mheg5_set_ics_guid_result(
    UINT32          ui4_eng_code,
    BOOL            b_result)
{
    INT32 i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;

    pt_eng = _find_engine_by_code(ui4_eng_code);

    if(pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_set_ics_guid_result(b_result);
    }

    return i4_ret;

}

INT32 x_mheg5_reg_pin_support_cb_fct(
    UINT32                  ui4_eng_code,
    x_mheg5_pin_support_fct pf_pin_support_fct,
    VOID*                   pv_tag)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_reg_pin_support_cb_fct(pf_pin_support_fct, pv_tag);
    }
    
    return (i4_ret);
}

INT32 x_mheg5_notify_audio_desc_pref_changed(UINT32 ui4_eng_code)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_notify_audio_desc_pref_changed();
    }
    
    return (i4_ret);
}

INT32 x_mheg5_reg_audio_desc_cb_fct(
    UINT32              ui4_eng_code,
    x_mheg5_ad_fct      pf_ad_fct,
    VOID*               pv_tag)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_reg_ad_cb_fct(pf_ad_fct, pv_tag);
    }
    
    return (i4_ret);
}

#endif

#ifdef MHEG5_NVM_SUPPORT
INT32 x_mheg5_init_tps(
	#ifdef LINUX_TURNKEY_SOLUTION
		VOID
	#else
		const CHAR* ps_tps_dev
	#endif
    )
{
    return nvm_init_tps(
			#ifndef LINUX_TURNKEY_SOLUTION
					   ps_tps_dev
			#endif
			);
}

INT32 x_mheg5_clear_tps(VOID)
{
    return nvm_clear_tps();
}
#endif

INT32 x_mheg5_reg_vid_info_update_nfy(
    UINT32                         ui4_eng_code,
    x_mheg5_vid_info_update_nfy    pf_func,
    VOID*                          pv_tag)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_reg_vid_info_update_nfy(pf_func, pv_tag);
    }
    
    return (i4_ret);

}

INT32 x_mheg5_get_vid_info(
        UINT32                  ui4_eng_code,
        MHEG5_VID_INFO_T*       pt_vid_info)
{
    INT32            i4_ret = MHEG5R_ENG_NOT_FOUND;
    _MHEG5_ENGINE_T* pt_eng;
    
    pt_eng = _find_engine_by_code(ui4_eng_code);

    if (pt_eng)
    {
        i4_ret = pt_eng->t_ftbl.pf_get_vid_info(pt_vid_info);
    }
    
    return (i4_ret);

}

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
INT32 mheg5_init(THREAD_DESCR_T* pt_thread_desc)
{
    INT32 i4_ret = INITR_FAIL;
#ifdef CHIP_SPEC_SUPPORT
    BOOL b_chip_mheg5_bond = FALSE;
#endif     

#ifdef CHIP_SPEC_SUPPORT
    /* linux platform should use anther way to get chip bounding.*/
    i4_ret = x_chip_spec_get_MHEG5_bond (&b_chip_mheg5_bond);
    if ((CHIPSPECR_OK == i4_ret) &&
        (TRUE == b_chip_mheg5_bond) &&
        (_init_context(pt_thread_desc)))
#else    
    if (IS_MHEG5Support() &&
        (_init_context(pt_thread_desc)))
#endif
    {
        _ui2_max_eng_num = 1;
        _ui2_reg_eng_num = 0;
        _ui4_count       = 0;
        _b_init          = TRUE;
        i4_ret = INITR_OK;

        mheg5_cli_init();
    }

    return (i4_ret);
}

#ifdef MHEG5_ENABLE_MEM_PROFILING
#define MHEG5_MAX_SUB_MODULE      ((INT32) 8)

static INT32 g_ai4_mem_peak[MHEG5_MAX_SUB_MODULE];
static INT32 g_ai4_mem_all[MHEG5_MAX_SUB_MODULE];

VOID* mheg5_mem_alloc(INT32 i4_module, const CHAR* ps_name, SIZE_T z_size)
{
    VOID* pv_mem = x_mem_alloc(z_size);

    if (pv_mem)
    {
        mheg5_mem_profiling(i4_module, ps_name, (INT32) z_size);
    }

    return (pv_mem);
}

VOID mheg5_mem_free(INT32 i4_module, const CHAR* ps_name, VOID* pv_mem, SIZE_T z_size)
{
    x_mem_free(pv_mem);

    mheg5_mem_profiling(i4_module, ps_name, - (INT32) z_size);
}

VOID mheg5_mem_profiling(INT32 i4_module, const CHAR* ps_name, INT32 x)
{
    if (i4_module < MHEG5_MAX_SUB_MODULE)
    {
        g_ai4_mem_all[i4_module] += x;

        if (g_ai4_mem_all[i4_module] > g_ai4_mem_peak[i4_module])
        {
            g_ai4_mem_peak[i4_module] = g_ai4_mem_all[i4_module];
        #if 0    
            MHEG5_DBG_ERROR(("\r\n*******************************************************************************************\r\n"));
            MHEG5_DBG_ERROR(("{MHEG5 Stack MEM (%s)} %s: %8d Bytes.\r\n", ps_name, (x > 0) ? "Allocate" : "Free    ", x));
            MHEG5_DBG_ERROR(("{MHEG5 Stack MEM (%s)} All:      %8d Bytes.\r\n", ps_name, g_ai4_mem_all[i4_module]));
            MHEG5_DBG_ERROR(("{MHEG5 Stack MEM (%s)} Peak:     %8d Bytes.\r\n", ps_name, g_ai4_mem_peak[i4_module]));
            MHEG5_DBG_ERROR(("\r\n*******************************************************************************************\r\n"));
        #endif    
        }
    }
    else
    {
        MHEG5_DBG_ERROR(("\r\n*******************************************************************************************\r\n"));
        MHEG5_DBG_ERROR(("{MHEG5 Stack MEM (%s)} ERROR! Index (%d) is beyond the valid range!", ps_name, i4_module));
        MHEG5_DBG_ERROR(("\r\n*******************************************************************************************\r\n"));
    }
}
#endif



/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
static BOOL _check_func_tbl(MHEG5_FUNC_TBL_T* pt_ftbl)
{
    if ((pt_ftbl) &&
        (pt_ftbl->pf_msg_proc) &&
        (pt_ftbl->pf_load_si)  &&
        (pt_ftbl->pf_unload_si))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static _MHEG5_ENGINE_T* _find_engine_by_code(UINT32 ui4_eng_code)
{
    if (_t_engine.ui4_code == ui4_eng_code)
    {
        return (&_t_engine);
    }

    return NULL;
}

static _MHEG5_ENGINE_T* _find_engine_by_name(const CHAR* ps_eng_name)
{
    SIZE_T z_len = MIN(sizeof(ps_eng_name), sizeof(_t_engine.sz_name));
    
    if (0 == x_strncmp(_t_engine.sz_name, ps_eng_name, z_len))
    {
        return (&_t_engine);
    }

    return NULL;
}

static _MHEG5_ENGINE_T* _new_engine(
    const CHAR*         ps_eng_name,
    MHEG5_FUNC_TBL_T*   pt_ftbl,
    UINT32*             pui4_eng_code)
{
    SIZE_T z_len = MIN(x_strlen(ps_eng_name), MHEG5_MAX_ENG_NAME - 1);
    
    x_strncpy(
        _t_engine.sz_name, 
        ps_eng_name, 
        z_len);

    _t_engine.sz_name[z_len] = '\0';
    _t_engine.ui4_code       = _ui4_count++;
    _t_engine.t_ftbl         = *pt_ftbl;

    *pui4_eng_code = _t_engine.ui4_code;
    
    return (&_t_engine);
}

static BOOL _init_context(THREAD_DESCR_T* pt_thread_desc)
{
    HANDLE_T    h_msg_q;
    UINT16      ui2_num_msgs;
    UINT8       ui1_priority;
    SIZE_T      z_stack_size;

    ui2_num_msgs = (pt_thread_desc->ui2_num_msgs == 0) ? MHEG5_NUM_OF_MSGS : pt_thread_desc->ui2_num_msgs;
    
    if (OSR_OK != x_msg_q_create(
        &h_msg_q, 
        MHEG5_MSG_Q_NAME,
        sizeof(_MHEG5_INTL_MSG_T), 
        ui2_num_msgs))
    {
        return FALSE;
    }
    _h_mheg5_msg_q = h_msg_q;
    
    z_stack_size = (pt_thread_desc->z_stack_size == 0) ? MHEG5_THREAD_DEFAULT_STACK_SIZE : pt_thread_desc->z_stack_size;
    ui1_priority = (pt_thread_desc->ui1_priority == 0) ? MHEG5_THREAD_DEFAULT_PRIORITY : pt_thread_desc->ui1_priority;
    
    if (OSR_OK != x_thread_create(
        &_h_mheg5_thread, 
        MHEG5_THREAD_NAME, 
        z_stack_size,
        ui1_priority,
        _mheg5_thread_main,
        0,
        NULL))
    {
        x_msg_q_delete(h_msg_q);
        _h_mheg5_msg_q = NULL_HANDLE;
        return FALSE;
    }

    return TRUE;
}

static VOID _mheg5_thread_main(VOID *pv_arg)
{
    INT32               i4_ret;
    SIZE_T              z_msg_size;
    UINT16              ui2_index;
    _MHEG5_INTL_MSG_T   t_intl_msg;
    _MHEG5_ENGINE_T*    pt_eng;
    
    while (1)
    {
        z_msg_size = sizeof(_MHEG5_INTL_MSG_T);
        
        i4_ret = x_msg_q_receive(
            &ui2_index,
            (VOID*) &t_intl_msg,
            &z_msg_size,
            &_h_mheg5_msg_q,
            1,
            X_MSGQ_OPTION_WAIT);

        if (OSR_OK == i4_ret)
        {
            pt_eng = _find_engine_by_code(t_intl_msg.ui4_eng_code);

            if (pt_eng)
            {
                (*(pt_eng->t_ftbl.pf_msg_proc))(&(t_intl_msg.t_mheg5_msg));
            }
        }
    }
}

static VOID _remove_engine(_MHEG5_ENGINE_T* pt_eng)
{
    /* If multiple mheg5 engines are supported, this API has to be enhanced */
    x_memset(pt_eng, 0, sizeof(_MHEG5_ENGINE_T));
}

