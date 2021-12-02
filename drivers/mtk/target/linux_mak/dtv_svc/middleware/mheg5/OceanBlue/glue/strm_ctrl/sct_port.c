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
 * $RCSfile: sct_port.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $ Vincent Hsu
 * $MD5HEX: $
 *
 * Description: 
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#include "cli/u_cli.h"
#include "cli/x_cli.h"
#include "dbg/def_dbg_level_mw.h"
#include "strm_mngr/scc/x_scc.h"
#include "strm_mngr/u_sm.h"
#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"
#include "strm_mngr/video_hdlr/u_sm_video_hdlr.h"
#include "os/inc/x_os.h"
#include "os/thread/thread_lib.h"
#include "dbg/x_dbg.h"
#include "svctx/x_svctx.h"
#include "mheg5/u_mheg5.h"
#include "mheg5/x_mheg5.h"
#include "mheg5/mheg5.h"
#include "inc/x_common.h"
#include "handle/u_handle.h"
#include "handle/handle.h"
#include "graphic/u_gl.h"
#include "svl/x_svl.h"
#include "mheg5/OceanBlue/glue/strm_ctrl/sct.h"
#include "mheg5/OceanBlue/glue/si_loader/sil.h"
#ifdef MHEG5_IC_SUPPORT
#include "mheg5/OceanBlue/glue/ic/mheg5_ic.h"
#endif
#include "scdb/u_scdb.h"
#include "scdb/x_scdb_api.h"
#include "mheg5/OceanBlue/glue/mheg5_OceanBlue_eng.h"

/* MHEG-5 stack header files */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_errors.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_audio.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_video.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_ci.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_graphics.h"


#if 0
    #undef MHEG5_KEEP_SYS_VIDEO_MODE
#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Audio/video stream type */
#define STREAMTYPE_VIDEO_11172_2                ((UINT8)   1)
#define STREAMTYPE_VIDEO_13818_2                ((UINT8)   2)
#define STREAMTYPE_AUDIO_11172_3                ((UINT8)   3)
#define STREAMTYPE_AUDIO_13818_3                ((UINT8)   4)
#define STREAMTYPE_VIDEO_DIGICIPHER_2           ((UINT8) 128)
#define STREAMTYPE_AUDIO_ATSC_A53               ((UINT8) 129)
#define STREAMTYPE_VIDEO_H264                   ((UINT8) 27)
#define STREAMTYPE_AUDIO_HE_ACC                 ((UINT8) 17)

#define STREAMTYPE_PES_PRIV_13818_1             ((UINT8) 6)


typedef enum
{
    PLAYED_FROM_MEMORY,
    PLAYED_FROM_STREAM
} PLAY_TYPE_T;

typedef struct __DECODE_INFO_T
{
    HANDLE_T    h_lock;
    BOOL        b_err;
} _DECODE_INFO_T;

typedef struct _SCT_AUDIO_CTRL_CTX_T
{
    HANDLE_T        h_audio;
    
    HANDLE_T        h_main_audio;

    HANDLE_T        h_sub_audio;

    HANDLE_T        h_audio_lock;

    HANDLE_T        h_a_sync_lock;

    HANDLE_T        h_a_sync_timer;

    UINT32          ui4_a_sync_count;

    BOOL            b_a_sync_timeout;
    
    PLAY_TYPE_T     e_a_play_type;

    UINT8*          pui1_audio_buffer;
    
    UINT32          ui4_audio_buffer_len;
    
    BOOL            b_keep_audio_buffer;
    
    BOOL            b_resume_audio_clip;

    tmMH5AudioEncoding_t     t_aclip_encoding;

    tmMHEG5AudioControl_t    e_a_ctrl;

    tmMHEG5AudioControl_t    e_resume_a_ctrl;
    
    BOOL                     b_aclip_enabled;
    
    BOOL                     b_a_user;
    
    BOOL                     b_as_set;
    
    BOOL                     b_a_pid_ok;
    
    MPEG_2_PID_T             t_as_pid;
    
    STREAM_COMP_ID_T         t_a_comp_id;
    
    SCDB_REC_T               t_a_scdb_rec;
    
    _DECODE_INFO_T           t_a_dec_info;
    
    tmMHEG5DvbLocator_t      t_a_locator;
  
    ISO_639_LANG_T           t_def_lang;
    
    ISO_639_LANG_T           t_gui_lang;

    MPEG_2_PID_T             t_a_pcr_pid;

    UINT16                   ui2_a_svc_id;

    BOOL                     b_default_audio_lang;

    INT32                    i4_a_comp_tag;

    BOOL                     b_audio_mute;

    AUD_ENC_T                t_aud_enc;

    MPEG_2_PID_T             t_ad_as_pid;

    AUD_ENC_T                t_ad_aud_enc;

} SCT_AUDIO_CTRL_CTX_T;

typedef struct _SCT_VIDEO_CTRL_CTX_T
{
    HANDLE_T        h_video;
    
    HANDLE_T        h_main_video;
    
    HANDLE_T        h_sub_video;

    HANDLE_T        h_video_lock;

    HANDLE_T        h_v_sync_lock;

    HANDLE_T        h_v_sync_timer;

    UINT32          ui4_v_sync_count;

    BOOL            b_v_sync_timeout;
    
    PLAY_TYPE_T     e_v_play_type;

    SCC_VID_IFRAME_BUFF_T    t_iframe;
    
    tmMHEG5VideoControl_t    e_v_ctrl;

    tmMHEG5VideoControl_t    e_resume_v_ctrl;
    
    BOOL                     b_iframe_enabled;

    BOOL                     b_iframe_began;

    BOOL                     b_iframe_set;
    
    BOOL                     b_v_user;
    
    BOOL                     b_vs_set;

    BOOL                     b_v_stream_set;
    
    BOOL                     b_v_pid_ok;
    
    MPEG_2_PID_T             t_vs_pid;

    MPEG_2_PID_T             t_vs_ori_pid;
    
    STREAM_COMP_ID_T         t_v_comp_id;
    
    SCDB_REC_T               t_v_scdb_rec;
    
    _DECODE_INFO_T           t_v_dec_info;
    
    tmMHEG5DvbLocator_t      t_v_locator;

    MPEG_2_PID_T             t_v_pcr_pid;

    UINT16                   ui2_v_svc_id;

    INT32                    i4_v_comp_tag;

    BOOL                     b_video_frozen;

    MPEG_2_PID_T             t_v_pid_to_be_resumed;

    VID_ENC_T                t_vid_enc;

    BOOL                     b_v_pid_updated;

    x_mheg5_video_update_nfy     pf_video_update_nfy;

    x_mheg5_sbtl_visibility_nfy  pf_sbtl_visibility_nfy;/** to suport simultaneous MHEG5 and subtitle*/

    VOID*                    pv_video_update_nfy_tag;

    VOID*                    pv_sbtl_visibility_nfy_tag;/** to suport simultaneous MHEG5 and subtitle*/

    x_mheg5_vid_info_update_nfy   pf_vid_info_update_nfy;
    VOID*                         pf_vid_info_update_nfy_tag;


    MHEG5_SUBTITLE_CFG_T     t_sbtl_cfg;

    tmMHEG5DvbSubtitleShowState_t e_sbtl_eng_state;   /** to suport simultaneous MHEG5 and subtitle*/                  

} SCT_VIDEO_CTRL_CTX_T;

typedef struct _SCT_DISPLAY_CFG_T
{
    SCC_VID_OVER_SCAN_CLIPPER_T  t_dflt_ovs_clip;
    
    /* The data follows have to be protected with critical section */
    SCC_VID_OVER_SCAN_CLIPPER_T  t_ovs_clip_runtime;
    
    BOOL                         b_adjust_overscan;

    BOOL                         b_scr_mode_by_mheg5;
    
    BOOL                         b_vdp_mode_by_mheg;

    SCC_VID_SCREEN_MODE_T        e_screen_auto_mode;
    
    SCC_VID_SCREEN_MODE_T        e_vs_ori_scr_mode;
    
    SCC_VID_SCREEN_MODE_T        e_screen_mode;
    
    MHEG5_VIDEO_MODE_T           e_vid_mode;

    MHEG5_VIDEO_MODE_T           e_mh5_ori_vid_mode;
    
    GL_RECT_T                    t_rc_video;
    
    GL_RECT_T                    t_resume_rc_video;
    
    tmMHEG5WidescreenAlign_t     e_wam;
    
    tmMHEG5SceneAspectRatio_t    e_scene_ar;
    
    tmMHEG5DispAspectRatio_t     e_display_ar;
    
    MHEG5_WSS_T                  e_wss;
    
    tmMHEG5DisplayAspectRatioConversion_t e_decfc;
    
    MHEG5_ASPECT_RATIO_T         e_src_aspect_ratio;
    
    UINT8                        ui1_afd;
    
    VSH_REGION_INFO_T            t_prv_src_rgn;
    
    VSH_REGION_INFO_T            t_prv_dst_rgn;
    
    VSH_REGION_INFO_T            t_orig_src_rgn;
    
    VSH_REGION_INFO_T            t_full_src_rgn;
    
    VSH_REGION_INFO_T            t_full_dst_rgn;
    
    VSH_REGION_INFO_T            t_xlat_src_rgn;
    
    VSH_REGION_INFO_T            t_xlat_dst_rgn;
    
    UINT32                       ui4_real_vw;
    
    UINT32                       ui4_real_vh;

	HANDLE_T                     h_mutex;

} SCT_DISPLAY_CFG_T;

typedef struct _SCT_GNRL_INFO_T
{
    UINT32      ui4_eng_code;
    
    HANDLE_T    h_svctx; /*main*/

    HANDLE_T    h_sync_count_lock;

    BOOL        b_mheg5_running;

    BOOL        b_force_no_action;

    UINT16      ui2_launch_on_id;

    UINT16      ui2_launch_ts_id;

    UINT16      ui2_launch_svc_id;
    
} SCT_GNRL_INFO_T;

typedef struct __STREAM_CONFIG_T
{
    PLAY_TYPE_T     e_audio_play_type;
} _STREAM_CONFIG_T;


#define ASPECT_MODE_USER_LETTER_BOX         ((INT32) 0)
#define ASPECT_MODE_USER_CENTRE_CUT_OUT     ((INT32) 1)

#define MAX_REGION_WIDTH                    ((UINT32) VSH_REGION_MAX_WIDTH)
#define MAX_REGION_HEIGHT                   ((UINT32) VSH_REGION_MAX_HEIGHT)

#define VIDEO_CIF_WIDTH                     ((UINT32) 352)
#define VIDEO_CIF_HEIGHT                    ((UINT32) 288)

#define SCT_MSG_ADJUST_VIDEO                ((UINT16) (MHEG5_MSG_SPECIAL_START+0))
#define SCT_MSG_ADJUST_OSD                  ((UINT16) (MHEG5_MSG_SPECIAL_START+1))
#define SCT_MSG_NOTIFY_VIDEO_CHANGE         ((UINT16) (MHEG5_MSG_SPECIAL_START+2))
#define SCT_MSG_A_V_SYNC_UNLOCK             ((UINT16) (MHEG5_MSG_SPECIAL_START+3))
#define SCT_MSG_RESET_OVERSCAN              ((UINT16) (MHEG5_MSG_SPECIAL_START+4))
#define SCT_MSG_SET_FREEZE                  ((UINT16) (MHEG5_MSG_SPECIAL_START+5))
#define SCT_MSG_SET_SVCTX_SCR_MODE          ((UINT16) (MHEG5_MSG_SPECIAL_START+6))
#define SCT_MSG_ADJUST_OSD_EX               ((UINT16) (MHEG5_MSG_SPECIAL_START+7))
#define SCT_MSG_NOTIFY_VID_INFO_UPDATE      ((UINT16) (MHEG5_MSG_SPECIAL_START+8))

#define MHEG5_SCT_MSG_SEND_RETRY            ((UINT16) 10)

#define ADJUST_VIDEO_FLAG_NONE              ((UINT32) 0x0)
#define ADJUST_VIDEO_FLAG_PID_CHANGE        ((UINT32) 0x1)
#define ADJUST_VIDEO_FLAG_VIDEO_UPDATE      ((UINT32) 0x2)

#define A_V_SYNC_TIMEOUT                    (1000)

#define VIDEO_SYNC                          ((UINT32)    0)
#define AUDIO_SYNC                          ((UINT32)    1)
#define SCT_UNLOCK_FROM_TIMEOUT             ((UINT32) 0x10)

#define MHEG5_DEF_COMP_TAG                  ((INT32) -1)

#define IS_FULL_SCREEN(rc)                              \
        ((RECT_W(rc) == MHEG5_SCREEN_WIDTH)          && \
         (RECT_H(rc) == MHEG5_SCREEN_HEIGHT)         && \
         (-2 <= (rc)->i4_left && (rc)->i4_left <= 2) && \
         (-2 <= (rc)->i4_top  && (rc)->i4_top  <= 2))

#define IS_DIFF_REGION(old, new)                    \
        (((old)->ui4_x      != (new)->ui4_x)     || \
         ((old)->ui4_y      != (new)->ui4_y)     || \
         ((old)->ui4_width  != (new)->ui4_width) || \
         ((old)->ui4_height != (new)->ui4_height))

#ifdef CLI_LVL_ALL
#ifdef  DBG_LEVEL_MODULE
#undef  DBG_LEVEL_MODULE
#endif

#define DBG_LEVEL_MODULE            mheg5_sct_get_dbg_level()

static UINT16    _ui2_mheg5_sct_dbg_level = DBG_INIT_LEVEL_MW_MHEG5;
#endif

static x_mheg5_ad_fct  _pf_ad_fct = NULL;
static VOID*   _pv_ad_fct_tag = NULL;
    
/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static VOID     _mheg5_resolve_nfy(
     BOOL                   b_found, 
     MPEG_2_PID_T           t_pid, 
     MPEG_2_PID_T           t_pcr_pid,
     UINT8                  ui1_enc_type,
     VOID*                  pv_tag);

static VOID     _lock_audio_module      (VOID);
static VOID     _unlock_audio_module    (VOID);

static VOID     _lock_video_module      (VOID);
static VOID     _unlock_video_module    (VOID);

static VOID     _lock_display_module      (VOID);
static VOID     _unlock_display_module    (VOID);

static INT32    _aclip_close(BOOL b_sync);
    
static VOID     _aclip_lock(VOID);

static INT32    _aclip_nfy_fct(
    HANDLE_T                h_aud,
    SCC_AUD_CLIP_COND_T     e_nfy_cond,
    VOID                    *pv_nfy_tag,
    UINT32                  ui4_data_1,
    UINT32                  ui4_data_2);

static INT32    _aclip_restart(VOID);
    
static INT32    _aclip_stop(BOOL b_sync, BOOL b_keep_audio_buffer);

static VOID     _aclip_unlock(VOID);

static VOID     _adjust_video_for_force_4x3(
    MHEG5_WSS_T                             e_wss,
    tmMHEG5DispAspectRatio_t                e_display_ar,
    tmMHEG5DisplayAspectRatioConversion_t   e_decfc,
    VSH_REGION_INFO_T*                      pt_region);

static VOID     _adjust_video_for_fullscreen(
    MHEG5_WSS_T                             e_wss,
    tmMHEG5DispAspectRatio_t                e_display_ar,
    tmMHEG5DisplayAspectRatioConversion_t   e_decfc,
    VSH_REGION_INFO_T*                      pt_region);

static VOID     _adjust_with_video_mode(
    MHEG5_VIDEO_MODE_T                      e_vid_mode,
    MHEG5_WSS_T                             e_wss,
    tmMHEG5DispAspectRatio_t                e_display_ar,
    tmMHEG5DisplayAspectRatioConversion_t   e_decfc,
    VSH_REGION_INFO_T*                      pt_region);

static VOID     _adjust_with_overscan(
    VSH_REGION_INFO_T*                  pt_src_region,
    VSH_REGION_INFO_T*                  pt_dst_region,
    const SCC_VID_OVER_SCAN_CLIPPER_T*  pt_ovs_clip);

static VOID     _apply_wss(
    VSH_REGION_INFO_T* pt_src_region,
    VSH_REGION_INFO_T* pt_dst_region);

#if 0
static BOOL     _av_same_channel(VOID);
#endif

static BOOL     _calc_overscan_clipper(
    BOOL                            b_fullscreen,
    SCC_VID_OVER_SCAN_CLIPPER_T*    pt_ovs);

static BOOL     _calc_video_adjustment(
    MHEG5_VIDEO_MODE_T  e_vid_mode,
    VSH_REGION_INFO_T*  pt_src_region,
    VSH_REGION_INFO_T*  pt_dst_region);

static VOID     _clip_region(
    VSH_REGION_INFO_T*  pt_region, 
    UINT32              ui4_max_w,
    UINT32              ui4_max_h);

static VOID     _clip_video(
    VSH_REGION_INFO_T*  pt_src_region,
    VSH_REGION_INFO_T*  pt_dsp_region,
    BOOL                b_coord_tfx,
    const GL_RECT_T*    pt_bound,
    const GL_RECT_T*    pt_src,
    const GL_RECT_T*    pt_dst);

static MHEG5_DECFC_T _convert_decfc(tmMHEG5DisplayAspectRatioConversion_t e_decfc);

static INT32    _evaluate_audio(VOID);

static INT32    _evaluate_video(VOID);

static VOID     _get_quarter_transformation(
    tmMHEG5DisplayAspectRatioConversion_t*  pe_decfc,
    MHEG5_WSS_T                             e_wss,
    MHEG5_ASPECT_RATIO_T                    e_src_aspect_ratio);

static VOID     _get_receiver_transformation(
    tmMHEG5DisplayAspectRatioConversion_t*  pe_decfc, 
    MHEG5_WSS_T*                            pe_wss,
    UINT8                                   ui1_afd,
    MHEG5_ASPECT_RATIO_T                    e_src_aspect_ratio,
    INT32                                   i4_usr_aspect_mode);

static BOOL     _get_transformation(
    tmMHEG5DisplayAspectRatioConversion_t*  pe_decfc, 
    MHEG5_WSS_T*                            pe_wss,
    UINT8                                   ui1_afd,
    MHEG5_ASPECT_RATIO_T                    e_src_aspect_ratio);

static INT32    _iframe_close(BOOL b_sync);

static VOID     _iframe_lock(VOID);

static INT32    _iframe_nfy_fct(
    HANDLE_T                h_aud,
    SCC_VID_IFRAME_COND_T   e_nfy_cond,
    VOID                	*pv_nfy_tag,
    UINT32                  ui4_data_1,
    UINT32                  ui4_data_2);

static BOOL     _iframe_notify_err(VOID);

static VOID     _iframe_unlock(VOID);

static INT32    _init_dec_info(volatile _DECODE_INFO_T* pt_dec_info);

static BOOL     _in_mheg_context(VOID);

static INT32    _is_fullscreen_cif(VOID);

static BOOL     _is_video_mode_supported(MHEG5_VIDEO_MODE_T e_vid_mode);

static VOID     _lock_a_v_sync(UINT32 ui4_option);

static VOID     _lock_sync_count(VOID);

static VOID     _save_current_stream(STREAM_TYPE_T e_type);

static VOID     _scale_to_video_region(VSH_REGION_INFO_T* pt_region);

static VOID     _sct_post_video_adjustment(
    const MHEG5_DISPLAY_PARAM_T*    pt_dspy_param,
    VSH_REGION_INFO_T*              pt_src_region, 
    VSH_REGION_INFO_T*              pt_dst_region);

static VOID     _select_stream_by_pid_nfy(
    HANDLE_T            h_svctx,
    SVCTX_COND_T        e_nfy_cond,
    SVCTX_NTFY_CODE_T   e_code,
    STREAM_TYPE_T       e_stream_type,
    VOID*               pv_nfy_tag);

static VOID     _set_14x9_letterbox_transformation(
    const GL_RECT_T*    pt_video,
    GL_RECT_T*          pt_src,
    GL_RECT_T*          pt_dst);

static VOID     _set_center_cut_out_transformation(
    const GL_RECT_T*    pt_video,
    GL_RECT_T*          pt_src,
    GL_RECT_T*          pt_dst);

static VOID     _set_av_sync_data(BOOL b_auto);

static VOID     _set_ignore_transformation(
    const GL_RECT_T*    pt_video,
    GL_RECT_T*          pt_src,
    GL_RECT_T*          pt_dst);

static VOID     _set_internal_screen_mode(BOOL b_internal_screen_mode);

static VOID     _set_letterbox_transformation(
    const GL_RECT_T*    pt_video,
    GL_RECT_T*          pt_src,
    GL_RECT_T*          pt_dst);

static VOID     _set_pillarbox_transformation(
    const GL_RECT_T*    pt_video,
    GL_RECT_T*          pt_src,
    GL_RECT_T*          pt_dst);

static VOID     _set_stream_select_block(
    MPEG_2_PID_T        *pt_pid,
    BOOL                b_force_lock);

static VOID     _set_vdp_mode(SCC_VID_MODE_T e_mode);

static VOID     _sync_a_v_timer_cb(
    HANDLE_T            h_handle,
    VOID                *pv_tag);

static VOID     _unlock_a_v_sync(
    UINT32              ui4_option);

static VOID     _unlock_sync_count(VOID);

static VOID _sct_notify_video_update(
    MHEG5_VIDEO_UPDATE_NFY_T *pt_data);

#if 1
/** to suport simultaneous MHEG5 and subtitle*/
static VOID _sct_notify_sbtl_visibility(
    MHEG5_SBTL_STATE_T   e_state);
#endif

static INT32 _init_sct_gnrl_info(SCT_GNRL_INFO_T* pt_gnrl_info, UINT32 ui4_eng_code);

static INT32 _init_audio_ctrl_ctx(SCT_AUDIO_CTRL_CTX_T* pt_audio_ctrl);

static INT32 _init_video_ctrl_ctx(SCT_VIDEO_CTRL_CTX_T* pt_video_ctrl);

static INT32 _init_display_cfg(
    SCT_DISPLAY_CFG_T*                  pt_display_cfg,
    MHEG5_DISPLAY_ASPECT_RATIO_T        e_display_ar,
    const SCC_VID_OVER_SCAN_CLIPPER_T*  pt_ovs_clip);

static INT32 _stop_current_stream(
    STREAM_TYPE_T e_strm_type);

static INT32 _sct_get_full_screen_reg(
    VSH_REGION_INFO_T*          pt_full_src_reg,
    VSH_REGION_INFO_T*          pt_full_dst_reg);

static VOID _sct_notify_vid_info_update(MHEG5_VID_INFO_UPDATE_NFY_T *pt_data);

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static SCT_GNRL_INFO_T      g_t_gnrl_info;
static SCT_AUDIO_CTRL_CTX_T g_t_audio_ctrl;
static SCT_VIDEO_CTRL_CTX_T g_t_video_ctrl;
static SCT_DISPLAY_CFG_T    g_t_display_cfg;

static _STREAM_CONFIG_T         _t_cfg_default;
static _STREAM_CONFIG_T         _t_cfg_mheg5;


#define SCT_GET_GNRL_INFO(p_av_ctrl)       \
    do{                                    \
        (p_av_ctrl) = &(g_t_gnrl_info);    \
    } while(0)
    
#define SCT_GET_AUDIO_CTRL_CTX(p_a_ctrl)   \
    do{                                    \
        (p_a_ctrl) = &(g_t_audio_ctrl);    \
    } while(0)

#define SCT_GET_VIDEO_CTRL_CTX(p_v_ctrl)   \
    do{                                    \
        (p_v_ctrl) = &(g_t_video_ctrl);    \
    } while(0)

#define SCT_GET_CI_PLUS_CTRL_CTX(p_ci_ctrl) \
    do{                                     \
        (p_ci_ctrl) = &(g_t_ci_plus_ctrl);  \
    } while(0)

#define SCT_GET_DISPLAY_CFG(p_dis_cfg)      \
    do{                                     \
        (p_dis_cfg) = &(g_t_display_cfg);   \
    } while(0)

#define SCT_GET_SVCTX_HANDLE()  (g_t_gnrl_info.h_svctx)

#define SCT_GET_ENGINE_CODE()   (g_t_gnrl_info.ui4_eng_code)

#define IS_MHEG5_RUNNING()      (g_t_gnrl_info.b_mheg5_running)

#define IS_FORCE_NO_ACTION()    (g_t_gnrl_info.b_force_no_action)

/*-----------------------------------------------------------------------------
                    External functions
 ----------------------------------------------------------------------------*/
extern INT32 _convert_err_code(tmMHEG5Err_t t_err);


/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
INT32 sct_adjust_freeze_mode(VOID)
{
    SCT_VIDEO_CTRL_CTX_T* pt_video_ctrl = NULL;
    INT32   i4_ret = SMR_OK;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    
    SCT_GET_GNRL_INFO(pt_gnrl_info);
    
    if (TRUE == IS_MHEG5_RUNNING())
    {
        SCT_GET_VIDEO_CTRL_CTX(pt_video_ctrl);
        
        MHEG5_ASSERT(NULL != pt_video_ctrl);
        
        if (TRUE == pt_video_ctrl->b_video_frozen)
        {
            i4_ret = x_svctx_freeze(pt_gnrl_info->h_svctx, ST_MASK_VIDEO);
        }
    }
    
    return ((i4_ret == SMR_OK) ? MHEG5R_OK : MHEG5R_INTERNAL_ERROR);
}

INT32 sct_get_display_adjustment(
    UINT8                       ui1_afd,
    MHEG5_ASPECT_RATIO_T        e_src_aspect_ratio,
    SCC_VID_SCREEN_MODE_T       e_screen_mode,
    MHEG5_VIDEO_MODE_T          e_vid_mode,
    VSH_REGION_INFO_T*          pt_src_region, 
    VSH_REGION_INFO_T*          pt_dst_region)
{
    MHEG5_MSG_T     t_msg;
    CRIT_STATE_T    t_cs;
    SCT_DISPLAY_CFG_T*    pt_dis_cfg    = NULL;
    SCT_VIDEO_CTRL_CTX_T* pt_video_ctrl = NULL;
    INT32           i4_ret         = MHEG5R_INV_STATE;
    BOOL            b_vm_changed   = FALSE;
    BOOL            b_video        = FALSE;
    BOOL            b_osd          = FALSE;
    BOOL            b_video_change = FALSE;
    
    DBG_INFO((
        "{MHEG5 Stack} %s: AFD = %d, e_src_aspect_ratio = %d\r\n", 
        __FUNCTION__, 
        ui1_afd,
        e_src_aspect_ratio));

    SCT_GET_VIDEO_CTRL_CTX(pt_video_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_video_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);
    
    t_cs = x_crit_start();
#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    if (!IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video))
    {
        e_screen_mode = sct_get_screen_auto_mode_enum();
        e_vid_mode    = MHEG5_VIDEO_MODE_AUTO;
    }

    b_osd = TRUE;
#endif
    pt_dis_cfg->ui1_afd            = ui1_afd;
    pt_dis_cfg->e_src_aspect_ratio = e_src_aspect_ratio;
    pt_dis_cfg->e_screen_mode      = e_screen_mode;

    /* TODO: the video dimension must be passed from the ASP callback */
    if (PLAYED_FROM_STREAM == pt_video_ctrl->e_v_play_type)
    {
        pt_dis_cfg->ui4_real_vw = (UINT32)MHEG5_SCREEN_WIDTH;
        pt_dis_cfg->ui4_real_vh = (UINT32)MHEG5_SCREEN_HEIGHT;
    }

    if (pt_dis_cfg->e_vid_mode != e_vid_mode)
    {
        pt_dis_cfg->e_vid_mode  = e_vid_mode;
        b_vm_changed = TRUE;
    }

#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    if (IS_MHEG5_RUNNING() &&
        (!IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video) ||
         pt_dis_cfg->e_screen_mode == sct_get_screen_auto_mode_enum() ||
         pt_video_ctrl->t_vs_pid != pt_video_ctrl->t_vs_ori_pid))
#else
    if (IS_MHEG5_RUNNING())
#endif
    {
        if (!_is_video_mode_supported(e_vid_mode))
        {
            e_vid_mode = MHEG5_VIDEO_MODE_FULLSCREEN;
        }

        b_video = b_vm_changed;

        _get_transformation(&pt_dis_cfg->e_decfc, &pt_dis_cfg->e_wss, pt_dis_cfg->ui1_afd, pt_dis_cfg->e_src_aspect_ratio);

        /* adjust video */
        if (FALSE == _calc_video_adjustment(
            e_vid_mode,
            pt_src_region, 
            pt_dst_region))
        {
            pt_src_region->ui4_x      = 0;
            pt_src_region->ui4_y      = 0;
            pt_src_region->ui4_width  = 0;
            pt_src_region->ui4_height = 0;

            *pt_dst_region = *pt_src_region;
        }

        if (IS_DIFF_REGION(&pt_dis_cfg->t_prv_src_rgn, pt_src_region) ||
            IS_DIFF_REGION(&pt_dis_cfg->t_prv_dst_rgn, pt_dst_region))
        {
            b_video_change = TRUE;
        }
        else
        {
            b_video_change = FALSE;
        }

        pt_dis_cfg->t_prv_src_rgn = *pt_src_region;
        pt_dis_cfg->t_prv_dst_rgn = *pt_dst_region;

        b_osd = TRUE;

        i4_ret = MHEG5R_OK;
    }
#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    else
    {
        if (MHEG5_DISPLAY_ASPECT_RATIO_16x9 == pt_dis_cfg->e_display_ar)
        {
            pt_dis_cfg->e_wss = MHEG5_WSS_16_9FF;
        }
        else
        {
            pt_dis_cfg->e_wss = MHEG5_WSS_4_3FF;
        }
    }
#endif


    x_crit_end(t_cs);

    if (b_video_change)
    {
        _sct_notify_video_update(NULL);
    }
    
    if (b_video)
    {
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_NOTIFY_VIDEO_CHANGE;

        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }

    if (b_osd)
    {
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_ADJUST_OSD;

        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }
    
    return (i4_ret);
}

INT32 sct_evaluate(VOID)
{
    _lock_video_module();
    
    _evaluate_video();

    _unlock_video_module();
    

    _lock_audio_module();
    
    _evaluate_audio();

    _unlock_audio_module();
    
    return MHEG5R_OK;
}

INT32 sct_force_no_action(BOOL b_force)
{
    SCT_GNRL_INFO_T* pt_gnrl_info = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    
    pt_gnrl_info->b_force_no_action = b_force;

    return MHEG5R_OK;
}

SCC_VID_SCREEN_MODE_T sct_get_screen_auto_mode_enum(VOID)
{
    
    SCT_DISPLAY_CFG_T*    pt_dis_cfg = NULL;

    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);

    return (pt_dis_cfg->e_screen_auto_mode);
}

BOOL sct_is_in_mheg5_screen_mode(VOID)
{
    
    SCT_DISPLAY_CFG_T*    pt_dis_cfg = NULL;

    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);

    return (pt_dis_cfg->b_scr_mode_by_mheg5);
}

static INT32 _init_sct_gnrl_info(SCT_GNRL_INFO_T* pt_gnrl_info, UINT32 ui4_eng_code)
{
    INT32 i4_ret;

    if (NULL == pt_gnrl_info)
    {
        return MHEG5R_INV_ARG;
    }

    pt_gnrl_info->ui4_eng_code      = ui4_eng_code;
    pt_gnrl_info->b_force_no_action = FALSE;
    pt_gnrl_info->b_mheg5_running   = FALSE;
    pt_gnrl_info->ui2_launch_on_id  = 0;
    pt_gnrl_info->ui2_launch_ts_id  = 0;
    pt_gnrl_info->ui2_launch_svc_id = 0;
    pt_gnrl_info->h_svctx           = NULL_HANDLE;
    /* create the sync-count lock */
    i4_ret = x_sema_create(&pt_gnrl_info->h_sync_count_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("{MHEG5 Stack} create the sync-count lock failed. (%d)\n", i4_ret));
        pt_gnrl_info->h_sync_count_lock = NULL_HANDLE;
        return MHEG5R_OUT_OF_RESOURCES;
    }

    return MHEG5R_OK;
}


static INT32 _init_audio_ctrl_ctx(SCT_AUDIO_CTRL_CTX_T* pt_audio_ctrl)
{
    INT32 i4_ret;
    
    if (NULL == pt_audio_ctrl)
    {
        return MHEG5R_INV_ARG;
    }

    do
    {
        i4_ret = MHEG5R_OK;
        SM_COMMAND_T    at_cmd[3];

        x_memset(pt_audio_ctrl, 0, sizeof(SCT_AUDIO_CTRL_CTX_T));
        
        /* create the audio mutex */
        i4_ret = x_sema_create(&pt_audio_ctrl->h_audio_lock, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK);
        if (OSR_OK != i4_ret)
        {
            DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed as %d.\n", __FUNCTION__, __LINE__, i4_ret));
            pt_audio_ctrl->h_audio_lock = NULL_HANDLE;
            i4_ret = MHEG5R_OUT_OF_RESOURCES;
            break;
        }
        
        /* create the audio-sync lock */
        i4_ret = x_sema_create(&pt_audio_ctrl->h_a_sync_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
        if (OSR_OK != i4_ret)
        {
            DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed as %d.\n", __FUNCTION__, __LINE__, i4_ret));
            pt_audio_ctrl->h_a_sync_lock = NULL_HANDLE;
            i4_ret = MHEG5R_OUT_OF_RESOURCES;
            break;
        }

        /* create the audio-sync timer */
        i4_ret = x_timer_create(&pt_audio_ctrl->h_a_sync_timer);
        if (OSR_OK != i4_ret)
        {
            DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed as %d.\n", __FUNCTION__, __LINE__, i4_ret));
            pt_audio_ctrl->h_a_sync_timer = NULL_HANDLE;
            i4_ret = MHEG5R_OUT_OF_RESOURCES;
            break;
        }
        
        /* init audio decoder info */
        i4_ret = _init_dec_info(&pt_audio_ctrl->t_a_dec_info);
        if (MHEG5R_OK != i4_ret)
        {
            DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed as %d.\n", __FUNCTION__, __LINE__, i4_ret));
            i4_ret = MHEG5R_OUT_OF_RESOURCES;
            break;
        }
             
        /* command for opening an audio component */
        at_cmd[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
        at_cmd[0].u.ui4_data = SCC_CMD_TYPE_AUDIO;
        at_cmd[1].e_code     = SCC_CMD_CODE_NAME;
        at_cmd[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
        at_cmd[2].e_code     = SM_CMD_CODE_END;
        
        /* open the audio component */    
        i4_ret = x_scc_comp_open(at_cmd, &pt_audio_ctrl->h_main_audio);
        if (SMR_OK != i4_ret)
        {
            DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed as %d.\n", __FUNCTION__, __LINE__, i4_ret));
            pt_audio_ctrl->h_audio = NULL_HANDLE;
            i4_ret = MHEG5R_OUT_OF_RESOURCES;
            break;
        }
        
        /* command for opening an audio component */
        at_cmd[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
        at_cmd[0].u.ui4_data = SCC_CMD_TYPE_AUDIO;
        at_cmd[1].e_code     = SCC_CMD_CODE_NAME;
        at_cmd[1].u.ps_text  = SN_PRES_SUB_DISPLAY;
        at_cmd[2].e_code     = SM_CMD_CODE_END;
        
        /* open the audio component */    
        i4_ret = x_scc_comp_open(at_cmd, &pt_audio_ctrl->h_sub_audio);
        if (SMR_OK != i4_ret)
        {
            DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed as %d.\n", __FUNCTION__, __LINE__, i4_ret));
            pt_audio_ctrl->h_audio = NULL_HANDLE;
            i4_ret = MHEG5R_OUT_OF_RESOURCES;
            break;
        }
        
        pt_audio_ctrl->h_audio = pt_audio_ctrl->h_main_audio;

        /* other fields would be initialized in sct_start()*/
        pt_audio_ctrl->b_aclip_enabled      = FALSE;
        pt_audio_ctrl->b_as_set             = FALSE;
        pt_audio_ctrl->b_audio_mute         = FALSE;
        pt_audio_ctrl->b_a_pid_ok           = FALSE;
        pt_audio_ctrl->b_a_sync_timeout     = FALSE;
        pt_audio_ctrl->b_a_user             = FALSE;
        pt_audio_ctrl->b_default_audio_lang = FALSE;
        pt_audio_ctrl->b_keep_audio_buffer  = FALSE;
        pt_audio_ctrl->b_resume_audio_clip  = FALSE;
        pt_audio_ctrl->e_a_ctrl             = MHEG5_AUDIO_NONE;
        pt_audio_ctrl->e_a_play_type        = PLAYED_FROM_STREAM;
        pt_audio_ctrl->e_resume_a_ctrl      = MHEG5_AUDIO_NONE;
        pt_audio_ctrl->t_aclip_encoding     = MHEG5_MPEG2_AUDIO;
        pt_audio_ctrl->i4_a_comp_tag        = 0;
        pt_audio_ctrl->pui1_audio_buffer    = NULL;
        pt_audio_ctrl->t_as_pid             = MPEG_2_NULL_PID;
        pt_audio_ctrl->t_a_comp_id.e_type   = ST_UNKNOWN;
        pt_audio_ctrl->t_a_comp_id.pv_stream_tag = NULL;
        x_memset(&pt_audio_ctrl->t_a_locator, 0, sizeof(tmMHEG5DvbLocator_t));
        pt_audio_ctrl->t_a_pcr_pid          = MPEG_2_NULL_PID;
        x_memset(&pt_audio_ctrl->t_a_scdb_rec, 0, sizeof(SCDB_REC_T));
        x_strcpy(pt_audio_ctrl->t_def_lang, ISO_639_DEFAULT);
        x_strcpy(pt_audio_ctrl->t_gui_lang, ISO_639_DEFAULT);
        pt_audio_ctrl->ui2_a_svc_id         = 0;
        pt_audio_ctrl->ui4_audio_buffer_len = 0;
        pt_audio_ctrl->ui4_a_sync_count     = 1;
        pt_audio_ctrl->t_aud_enc            = AUD_ENC_MPEG_2;

    } while(0);

    if (MHEG5R_OK != i4_ret)
    {
        if (NULL_HANDLE != pt_audio_ctrl->h_audio_lock)
        {
            x_sema_delete(pt_audio_ctrl->h_audio_lock);
            pt_audio_ctrl->h_audio_lock = NULL_HANDLE;
        }

        if (NULL_HANDLE != pt_audio_ctrl->h_a_sync_lock)
        {
            x_sema_delete(pt_audio_ctrl->h_a_sync_lock);
            pt_audio_ctrl->h_a_sync_lock = NULL_HANDLE;
        }
        
        if (NULL_HANDLE != pt_audio_ctrl->h_a_sync_timer)
        {
            x_timer_delete(pt_audio_ctrl->h_a_sync_timer);
            pt_audio_ctrl->h_a_sync_timer = NULL_HANDLE;
        }
        
        if (NULL_HANDLE != pt_audio_ctrl->t_a_dec_info.h_lock)
        {
            x_sema_delete(pt_audio_ctrl->t_a_dec_info.h_lock);
            pt_audio_ctrl->t_a_dec_info.h_lock = NULL_HANDLE;
        }
        
        if (NULL_HANDLE != pt_audio_ctrl->h_main_audio)
        {
            x_scc_comp_close(pt_audio_ctrl->h_main_audio);
            pt_audio_ctrl->h_main_audio = NULL_HANDLE;
        }
        
        if (NULL_HANDLE != pt_audio_ctrl->h_sub_audio)
        {
            x_scc_comp_close(pt_audio_ctrl->h_sub_audio);
            pt_audio_ctrl->h_sub_audio = NULL_HANDLE;
        }
    }

    return i4_ret;
    
}

static INT32 _init_video_ctrl_ctx(SCT_VIDEO_CTRL_CTX_T* pt_video_ctrl)
{
    INT32 i4_ret;
    
    if (NULL == pt_video_ctrl)
    {
        return MHEG5R_INV_ARG;
    }

    do
    {
        i4_ret = MHEG5R_OK;
        SM_COMMAND_T    at_cmd[3];

        x_memset(pt_video_ctrl, 0, sizeof(SCT_VIDEO_CTRL_CTX_T));
        
        /* create the vidoe mutex */
        i4_ret = x_sema_create(&pt_video_ctrl->h_video_lock, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK);
        if (OSR_OK != i4_ret)
        {
            DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed as %d.\n", __FUNCTION__, __LINE__, i4_ret));
            pt_video_ctrl->h_video_lock = NULL_HANDLE;
            i4_ret = MHEG5R_OUT_OF_RESOURCES;
            break;
        }
        
        /* create the video-sync lock */
        i4_ret = x_sema_create(&pt_video_ctrl->h_v_sync_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
        if (OSR_OK != i4_ret)
        {
            DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed as %d.\n", __FUNCTION__, __LINE__, i4_ret));
            pt_video_ctrl->h_v_sync_lock = NULL_HANDLE;
            i4_ret = MHEG5R_OUT_OF_RESOURCES;
            break;
        }

        /* create the video-sync timer */
        i4_ret = x_timer_create(&pt_video_ctrl->h_v_sync_timer);
        if (OSR_OK != i4_ret)
        {
            DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed as %d.\n", __FUNCTION__, __LINE__, i4_ret));
            pt_video_ctrl->h_v_sync_timer = NULL_HANDLE;
            i4_ret = MHEG5R_OUT_OF_RESOURCES;
            break;
        }
        
        /* init vidoe decoder info */
        i4_ret = _init_dec_info(&pt_video_ctrl->t_v_dec_info);
        if (MHEG5R_OK != i4_ret)
        {
            DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed as %d.\n", __FUNCTION__, __LINE__, i4_ret));
            i4_ret = MHEG5R_OUT_OF_RESOURCES;
            break;
        }
        
        /* command for opening a video component */
        at_cmd[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
        at_cmd[0].u.ui4_data = SCC_CMD_TYPE_VIDEO;
        at_cmd[1].e_code     = SCC_CMD_CODE_NAME;
        at_cmd[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
        at_cmd[2].e_code     = SM_CMD_CODE_END;

        /* open the video component */    
        i4_ret = x_scc_comp_open(at_cmd, &pt_video_ctrl->h_main_video);
        if (i4_ret != SMR_OK)
        {
            DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed as %d.\n", __FUNCTION__, __LINE__, i4_ret));
            pt_video_ctrl->h_video = NULL_HANDLE;
            break;
        }
        
        /* command for opening a video component */
        at_cmd[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
        at_cmd[0].u.ui4_data = SCC_CMD_TYPE_VIDEO;
        at_cmd[1].e_code     = SCC_CMD_CODE_NAME;
        at_cmd[1].u.ps_text  = SN_PRES_SUB_DISPLAY;
        at_cmd[2].e_code     = SM_CMD_CODE_END;

        /* open the video component */    
        i4_ret = x_scc_comp_open(at_cmd, &pt_video_ctrl->h_sub_video);
        if (i4_ret != SMR_OK)
        {
            DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed as %d.\n", __FUNCTION__, __LINE__, i4_ret));
            pt_video_ctrl->h_video = NULL_HANDLE;
            break;
        }

        pt_video_ctrl->h_video = pt_video_ctrl->h_main_video;

        /* other fields would be initialized in sct_start()*/
        pt_video_ctrl->b_iframe_began        = FALSE;
        pt_video_ctrl->b_iframe_enabled      = FALSE;
        pt_video_ctrl->b_video_frozen        = FALSE;
        pt_video_ctrl->b_vs_set              = FALSE;
        pt_video_ctrl->b_v_pid_ok            = FALSE;
        pt_video_ctrl->b_v_sync_timeout      = FALSE;
        pt_video_ctrl->b_v_user              = FALSE;
        pt_video_ctrl->b_iframe_set          = FALSE;
        pt_video_ctrl->b_v_stream_set        = FALSE;
        pt_video_ctrl->e_v_ctrl              = MHEG5_VIDEO_NONE;
        pt_video_ctrl->e_v_play_type         = PLAYED_FROM_STREAM;
        pt_video_ctrl->e_resume_v_ctrl       = MHEG5_VIDEO_NONE;
        pt_video_ctrl->i4_v_comp_tag         = 0;
        x_memset(&pt_video_ctrl->t_iframe, 0, sizeof(SCC_VID_IFRAME_BUFF_T));
        pt_video_ctrl->t_vs_pid              = MPEG_2_NULL_PID;
        pt_video_ctrl->t_vs_ori_pid          = MPEG_2_NULL_PID;
        pt_video_ctrl->t_v_comp_id.e_type    = ST_UNKNOWN;
        pt_video_ctrl->t_v_comp_id.pv_stream_tag = NULL;
        x_memset(&pt_video_ctrl->t_v_locator, 0, sizeof(tmMHEG5DvbLocator_t));
        pt_video_ctrl->t_v_pcr_pid           = MPEG_2_NULL_PID;
        x_memset(&pt_video_ctrl->t_v_scdb_rec, 0, sizeof(SCDB_REC_T));
        pt_video_ctrl->t_v_pid_to_be_resumed = MPEG_2_NULL_PID;
        pt_video_ctrl->ui2_v_svc_id          = 0;
        pt_video_ctrl->ui4_v_sync_count      = 1;
        pt_video_ctrl->t_vid_enc             = VID_ENC_MPEG_2;
        pt_video_ctrl->b_v_pid_updated       = FALSE;
    } while(0);

    if (MHEG5R_OK != i4_ret)
    {
        if (NULL_HANDLE != pt_video_ctrl->h_video_lock)
        {
            x_sema_delete(pt_video_ctrl->h_video_lock);
            pt_video_ctrl->h_video_lock = NULL_HANDLE;
        }

        if (NULL_HANDLE != pt_video_ctrl->h_v_sync_lock)
        {
            x_sema_delete(pt_video_ctrl->h_v_sync_lock);
            pt_video_ctrl->h_v_sync_lock = NULL_HANDLE;
        }
        
        if (NULL_HANDLE != pt_video_ctrl->h_v_sync_timer)
        {
            x_timer_delete(pt_video_ctrl->h_v_sync_timer);
            pt_video_ctrl->h_v_sync_timer = NULL_HANDLE;
        }
        
        if (NULL_HANDLE != pt_video_ctrl->t_v_dec_info.h_lock)
        {
            x_sema_delete(pt_video_ctrl->t_v_dec_info.h_lock);
            pt_video_ctrl->t_v_dec_info.h_lock = NULL_HANDLE;
        }

        if (NULL_HANDLE != pt_video_ctrl->h_main_video)
        {
            x_scc_comp_close(pt_video_ctrl->h_main_video);
            pt_video_ctrl->h_main_video = NULL_HANDLE;
        }
        
        if (NULL_HANDLE != pt_video_ctrl->h_sub_video)
        {
            x_scc_comp_close(pt_video_ctrl->h_sub_video);
            pt_video_ctrl->h_sub_video = NULL_HANDLE;
        }
    }

    return i4_ret;
    
}

static INT32 _init_display_cfg(
    SCT_DISPLAY_CFG_T*                  pt_display_cfg,
    MHEG5_DISPLAY_ASPECT_RATIO_T        e_display_ar,
    const SCC_VID_OVER_SCAN_CLIPPER_T*  pt_ovs_clip)
{
    if (NULL == pt_display_cfg)
    {
        return MHEG5R_INV_ARG;
    }
    
    if (MHEG5_DISPLAY_ASPECT_RATIO_4_3 == e_display_ar)
    {
        pt_display_cfg->e_display_ar = MHEG5_DISPLAY_ASPECT_RATIO_4x3;
    }
    else
    {
        pt_display_cfg->e_display_ar = MHEG5_DISPLAY_ASPECT_RATIO_16x9;
    }

    /* create the display mutex */
    if (OSR_OK != x_sema_create(&pt_display_cfg->h_mutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK))
    {
        DBG_ERROR(("{MHEG5 Stack} Func:%s Line:%d failed.\n", __FUNCTION__, __LINE__));
        pt_display_cfg->h_mutex = NULL_HANDLE;
        return MHEG5R_OUT_OF_RESOURCES;
    }

    pt_display_cfg->b_adjust_overscan   = FALSE;
    pt_display_cfg->b_scr_mode_by_mheg5 = FALSE;
    pt_display_cfg->b_vdp_mode_by_mheg  = FALSE;
    pt_display_cfg->e_decfc             = MHEG5_DISPLAY_AR_CONVERSION_UNKNOWN;
    pt_display_cfg->e_display_ar        = MHEG5_DISPLAY_ASPECT_RATIO_16x9;
    pt_display_cfg->e_scene_ar          = MHEG5_ASPECT_RATIO_UNSEPCIFIED;
    pt_display_cfg->e_screen_auto_mode  = SCC_VID_SCREEN_MODE_CUSTOM_DEF_0;
    pt_display_cfg->e_vs_ori_scr_mode   = SCC_VID_SCREEN_MODE_CUSTOM_DEF_0;
    pt_display_cfg->e_screen_mode       = SCC_VID_SCREEN_MODE_NORMAL;
    pt_display_cfg->e_src_aspect_ratio  = MHEG5_ASPECT_RATIO_UNKNOWN;
    pt_display_cfg->e_vid_mode          = MHEG5_VIDEO_MODE_FULLSCREEN;
    pt_display_cfg->e_mh5_ori_vid_mode  = MHEG5_VIDEO_MODE_AUTO;
    pt_display_cfg->e_wam               = MHEG5_ALIGNMENT_CENTRE_CUT_OUT;
    pt_display_cfg->e_wss               = MHEG5_WSS_16_9FF;
    x_memset(&pt_display_cfg->t_prv_dst_rgn, 0, sizeof(VSH_REGION_INFO_T));
    x_memset(&pt_display_cfg->t_prv_src_rgn, 0, sizeof(VSH_REGION_INFO_T));
    pt_display_cfg->t_rc_video.i4_left  = 0;
    pt_display_cfg->t_rc_video.i4_top   = 0;
    pt_display_cfg->t_rc_video.i4_right = MHEG5_SCREEN_WIDTH;
    pt_display_cfg->t_rc_video.i4_bottom= MHEG5_SCREEN_HEIGHT;
    pt_display_cfg->t_resume_rc_video.i4_left   = 0;
    pt_display_cfg->t_resume_rc_video.i4_top    = 0;
    pt_display_cfg->t_resume_rc_video.i4_right  = MHEG5_SCREEN_WIDTH;
    pt_display_cfg->t_resume_rc_video.i4_bottom = MHEG5_SCREEN_HEIGHT;
    pt_display_cfg->t_dflt_ovs_clip     = *((SCC_VID_OVER_SCAN_CLIPPER_T*) pt_ovs_clip);
    pt_display_cfg->t_ovs_clip_runtime  = *((SCC_VID_OVER_SCAN_CLIPPER_T*) pt_ovs_clip);
    pt_display_cfg->ui1_afd             = 0;
    pt_display_cfg->ui4_real_vh         = 0;
    pt_display_cfg->ui4_real_vw         = 0;

    return MHEG5R_OK;
}



INT32 sct_init(
    UINT32                              ui4_eng_code, 
    MHEG5_DISPLAY_ASPECT_RATIO_T        e_display_ar,
    const SCC_VID_OVER_SCAN_CLIPPER_T*  pt_ovs_clip)
{
    INT32               i4_ret;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);   

    x_memset(pt_gnrl_info, 0, sizeof(SCT_GNRL_INFO_T));
    x_memset(pt_a_ctrl, 0, sizeof(SCT_AUDIO_CTRL_CTX_T));
    x_memset(pt_v_ctrl, 0, sizeof(SCT_VIDEO_CTRL_CTX_T));
    x_memset(pt_dis_cfg, 0, sizeof(SCT_DISPLAY_CFG_T));
    
    i4_ret = _init_sct_gnrl_info(pt_gnrl_info, ui4_eng_code);
    if (MHEG5R_OK != i4_ret)
    {
        return i4_ret;
    }

    i4_ret = _init_audio_ctrl_ctx(pt_a_ctrl);
    if (MHEG5R_OK != i4_ret)
    {
        return i4_ret;
    }
    
    i4_ret = _init_video_ctrl_ctx(pt_v_ctrl);
    if (MHEG5R_OK != i4_ret)
    {
        return i4_ret;
    }
    
    i4_ret = _init_display_cfg(pt_dis_cfg, e_display_ar, pt_ovs_clip);
    if (MHEG5R_OK != i4_ret)
    {
        return i4_ret;
    }

    return MHEG5R_OK;
}

INT32 sct_pause(VOID)
{
    INT32                   i4_ret = MHEG5R_INTERNAL_ERROR;
    tmMHEG5Err_t            t_err;
    BOOL                    b_v_user, b_a_user;
    SCC_VID_SCREEN_MODE_T   e_scr_mode;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);

    #if 0
    if (pt_gnrl_info->b_mheg5_running == FALSE)
    {
        return MHEG5R_OK;
    }
    #endif

    _lock_video_module();
    _lock_audio_module();

    pt_gnrl_info->b_mheg5_running = FALSE;
    
    if (IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video))
    {
        x_scc_vid_get_scr_mode(pt_v_ctrl->h_video, &pt_dis_cfg->e_vs_ori_scr_mode);
    }

    pt_dis_cfg->t_resume_rc_video  = pt_dis_cfg->t_rc_video;
    pt_v_ctrl->e_resume_v_ctrl = pt_v_ctrl->e_v_ctrl;
    pt_a_ctrl->e_resume_a_ctrl = pt_a_ctrl->e_a_ctrl;

    b_v_user   = pt_v_ctrl->b_v_user;
    b_a_user   = pt_a_ctrl->b_a_user;
    e_scr_mode = pt_dis_cfg->e_vs_ori_scr_mode;

    if (pt_a_ctrl->b_audio_mute)
    {
        x_scc_aud_switch(pt_a_ctrl->h_audio, pt_a_ctrl->b_audio_mute);
    }

    if (pt_a_ctrl->b_aclip_enabled)
    {
        _aclip_lock();
        
        pt_a_ctrl->b_resume_audio_clip = TRUE;
        _aclip_stop(TRUE, TRUE);

        _aclip_unlock();
    }
    else
    {
        pt_a_ctrl->b_resume_audio_clip = FALSE;
    }

    /* remember current PID for resuming later. Because tmMHEG5SetVideoControl(MHEG5_VIDEO_NONE) would reset _t_vs_pid */
    pt_v_ctrl->t_v_pid_to_be_resumed = pt_v_ctrl->t_vs_pid;

    _unlock_audio_module();
    _unlock_video_module();

    t_err = tmMHEG5VideoIFrameStop();
    
    if (b_v_user)
    {
        S_VideoPosition position;
        
        position.scaling.left = 0;
        position.scaling.top = 0;
        position.scaling.width = (U32BIT)MHEG5_SCREEN_WIDTH;
        position.scaling.height = (U32BIT)MHEG5_SCREEN_HEIGHT;
        DVP_VideoPosition(&position);
        t_err = tmMHEG5SetVideoControl(MHEG5_VIDEO_USER);
    }
    else
    {
        t_err = tmMHEG5SetVideoControl(MHEG5_VIDEO_NONE);
    }

    if (b_a_user)
    {
        t_err = tmMHEG5SetAudioControl(MHEG5_AUDIO_USER);
    }
    else
    {
        t_err = tmMHEG5SetAudioControl(MHEG5_AUDIO_NONE);
    }

#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    x_scc_vid_set_scr_mode(pt_v_ctrl->h_video, e_scr_mode);
#endif

    if (t_err == MHEG5_OK)
    {
        i4_ret = MHEG5R_OK;
    }

    if (!IS_FULL_SCREEN(&pt_dis_cfg->t_resume_rc_video) || pt_dis_cfg->b_vdp_mode_by_mheg)
    {
        _set_vdp_mode(SCC_VID_NORMAL);
    }

    _set_av_sync_data(TRUE);
    
    return i4_ret;
}

INT32 sct_proc_msg(MHEG5_MSG_T* pt_msg)
{
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);   

    switch (pt_msg->ui2_msg_type)
    {
        case SCT_MSG_NOTIFY_VIDEO_CHANGE:
            tmMHEG5NotifyVideoPrefChanged();
            break;

        case SCT_MSG_NOTIFY_VID_INFO_UPDATE:
            _sct_notify_vid_info_update(NULL);
            break;
            
        case SCT_MSG_ADJUST_VIDEO:
        {
            CRIT_STATE_T            t_cs;
        #ifndef MHEG5_KEEP_SYS_VIDEO_MODE
            SCC_VID_SCREEN_MODE_T   e_scr_mode;
        #endif

        #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
            MHEG5_MSG_T             t_msg;
            MPEG_2_PID_T            t_current_pid, t_original_pid;
            BOOL                    b_iframe_set, b_v_stream_set;
        #endif
            BOOL                b_video;                   
            STREAM_COMP_ID_T    t_comp_id;
            SCDB_REC_T          t_scdb;
    
            
            MHEG5_DBG_INFO(("{MHEG5 Stack} Adjust VIDEO...\r\n"));

            t_cs = x_crit_start();

        #ifndef MHEG5_KEEP_SYS_VIDEO_MODE
            e_scr_mode = pt_dis_cfg->e_screen_mode;
        #endif

        #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
            t_current_pid  = pt_v_ctrl->t_vs_pid;
            t_original_pid = pt_v_ctrl->t_vs_ori_pid;
            b_iframe_set   = pt_v_ctrl->b_iframe_set;
            b_v_stream_set = pt_v_ctrl->b_v_stream_set;
        #endif
            
            x_crit_end(t_cs);

            if (((UINT32)pt_msg->pv_nfy_tag) & ADJUST_VIDEO_FLAG_PID_CHANGE ||
                ((UINT32)pt_msg->pv_nfy_tag) & ADJUST_VIDEO_FLAG_VIDEO_UPDATE)
            {
                _sct_notify_video_update(NULL);
            }
            
        #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
            if (!IS_MHEG5_RUNNING())
            {
                break;
            }
            
            if (IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video) && 
                !b_iframe_set &&
                   (       
                        !b_v_stream_set || 
                        (t_current_pid == t_original_pid) || 
                        (t_current_pid == 0 )   
                    )
                )
            {
                x_scc_vid_set_scr_mode(pt_v_ctrl->h_video, pt_dis_cfg->e_vs_ori_scr_mode);
            }
            else
            {
                x_scc_vid_set_scr_mode(pt_v_ctrl->h_video, sct_get_screen_auto_mode_enum());
            }


            b_video = (x_svctx_get_using_strm_data(pt_gnrl_info->h_svctx, ST_VIDEO, &t_comp_id, &t_scdb) == SVCTXR_OK);

            t_msg.ui1_module   = MHEG5_MOD_SCT;
            if ((pt_dis_cfg->e_screen_mode !=  x_mheg5_get_screen_auto_mode_enum()) &&
                (b_video == TRUE) &&
                (pt_v_ctrl->e_v_play_type == PLAYED_FROM_STREAM) &&
                (MHEG5_VIDEO_NONE != pt_v_ctrl->e_v_ctrl) &&
                (MHEG5_ASPECT_RATIO_UNKNOWN != pt_dis_cfg->e_src_aspect_ratio))
            {
                t_msg.ui2_msg_type = SCT_MSG_ADJUST_OSD_EX;
            }
            else
            {
                t_msg.ui2_msg_type = SCT_MSG_ADJUST_OSD;
            }

            x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
            /* 
                DTG Test 6.8.2A, in scene5, video would be scaled and repositioned, we had
                unfroze the video first as the chip limitation, so here we have to freeze the
                video again.
            */
            if (IS_MHEG5_RUNNING() && pt_v_ctrl->b_video_frozen)
            {
                MHEG5_VIDEO_UPDATE_NFY_T    t_data;
                
                x_svctx_freeze(pt_gnrl_info->h_svctx, ST_MASK_VIDEO);
                
                t_data.ui4_nfy_flag = MHEG5_VIDEO_UPDATE_PARAM_FREEZE;
                t_data.b_freeze     = pt_v_ctrl->b_video_frozen;

                _sct_notify_video_update(&t_data);
            }
        #else
            x_scc_vid_set_scr_mode(pt_v_ctrl->h_video, e_scr_mode);
        #endif
        }
        break;

        case SCT_MSG_ADJUST_OSD:
        {
            MHEG5_CNV_CFG_T t_cnv_cfg;
            CRIT_STATE_T    t_cs;
            
            MHEG5_DBG_INFO(("{MHEG5 Stack} Adjust OSD...\r\n"));

            t_cs = x_crit_start();
            
            t_cnv_cfg.h_scc_video  = pt_v_ctrl->h_video;
            t_cnv_cfg.e_decfc      = _convert_decfc(pt_dis_cfg->e_decfc);
            t_cnv_cfg.e_wss        = pt_dis_cfg->e_wss;
        #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
            t_cnv_cfg.e_vid_mode   = MHEG5_VIDEO_MODE_AUTO;
        #else
            t_cnv_cfg.e_vid_mode   = pt_dis_cfg->e_vid_mode;
        #endif
            t_cnv_cfg.e_display_ar = 
                ((MHEG5_DISPLAY_ASPECT_RATIO_4x3 == pt_dis_cfg->e_display_ar) ?
                    MHEG5_DISPLAY_ASPECT_RATIO_4_3 : MHEG5_DISPLAY_ASPECT_RATIO_16_9);
            
            x_crit_end(t_cs);
            
            x_mheg5_adjust_osd(SCT_GET_ENGINE_CODE(), &t_cnv_cfg);
        }
        break;

        case SCT_MSG_ADJUST_OSD_EX:
        {
            MHEG5_CNV_CFG_T         t_cnv_cfg;
            MHEG5_DBG_INFO(("{MHEG5 Stack} Adjust OSD EX...\r\n"));
            
            x_memset(&t_cnv_cfg, 0, sizeof(MHEG5_CNV_CFG_T));
            
            VSH_REGION_INFO_T       t_full_src_reg;
            VSH_REGION_INFO_T       t_full_dst_reg;

            x_memset(&t_full_src_reg, 0, sizeof(VSH_REGION_INFO_T));
            x_memset(&t_full_dst_reg, 0, sizeof(VSH_REGION_INFO_T));
            
            t_cnv_cfg.h_scc_video = pt_v_ctrl->h_video;
            t_cnv_cfg.e_src_aspect_ratio = (MHEG5_ASPECT_RATIO_T)pt_dis_cfg->e_src_aspect_ratio;
            t_cnv_cfg.e_wam =  (MHEG5_WAM_T)pt_dis_cfg->e_wam;
            t_cnv_cfg.e_scene_ar = (MHEG5_ASPECT_RATIO_T)pt_dis_cfg->e_scene_ar;

            if(_sct_get_full_screen_reg(& t_full_src_reg,
                                         &t_full_dst_reg) != MHEG5R_OK)
            {
                MHEG5_DBG_INFO(("{MHEG5 Stack} _sct_get_full_screen_reg failed\r\n"));
                break;
            }
                
            x_mheg5_adjust_osd_ex(SCT_GET_ENGINE_CODE(), 
                                  &t_cnv_cfg,
                                  &pt_dis_cfg->t_orig_src_rgn,
                                  &t_full_src_reg,
                                  &t_full_dst_reg,
                                  &pt_dis_cfg->t_xlat_src_rgn,
                                  &pt_dis_cfg->t_xlat_dst_rgn);
        }
        break;

        case SCT_MSG_A_V_SYNC_UNLOCK:
            _unlock_a_v_sync((UINT32)pt_msg->pv_nfy_tag);
            break;

        case SCT_MSG_RESET_OVERSCAN:
        {
            CRIT_STATE_T                    t_cs;
            BOOL                            b_val;
            SCC_VID_OVER_SCAN_CLIPPER_T     t_ovs;
			
			_lock_display_module();

            t_cs = x_crit_start();
            
            b_val = (IS_MHEG5_RUNNING())? IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video): TRUE;

            x_crit_end(t_cs);
            
            b_val = _calc_overscan_clipper(b_val, &t_ovs);
            
            t_cs = x_crit_start();
            
            pt_dis_cfg->b_adjust_overscan  = b_val;
            pt_dis_cfg->t_ovs_clip_runtime = t_ovs;

            x_crit_end(t_cs);

			_unlock_display_module();
        }
        break;
#if 0
        case SCT_MSG_SET_FREEZE:
        {
            MHEG5_VIDEO_UPDATE_NFY_T    t_data;

            if (!_b_video_frozen && _b_mheg5_running)
            {
                x_scc_vid_set_mode(_h_video, SCC_VID_FREEZE);
                _b_video_frozen = TRUE;

                t_data.ui4_nfy_flag = MHEG5_VIDEO_UPDATE_PARAM_FREEZE;
                t_data.b_freeze     = _b_video_frozen;

                _sct_notify_video_update(&t_data);
            }
            
            break;
        }
#endif

        case SCT_MSG_SET_SVCTX_SCR_MODE:
        {
            SVCTX_MHEG_SCREEN_MODE_T    t_svctx_mheg5_scr_mode;
            
            t_svctx_mheg5_scr_mode.b_enable = pt_dis_cfg->b_scr_mode_by_mheg5;
            t_svctx_mheg5_scr_mode.e_scr_mode = pt_dis_cfg->e_screen_auto_mode;
            
            x_svctx_set(
                SCT_GET_SVCTX_HANDLE(), 
                SVCTX_COMMON_SET_MHEG_SCREEN_MODE,
                (VOID*)&t_svctx_mheg5_scr_mode,
                sizeof(SVCTX_MHEG_SCREEN_MODE_T));
        }
        break;

        case MHEG5_MSG_SWITCH_VIDEO_PATH:
        {
            CRIT_STATE_T t_cs;
            t_cs = x_crit_start();
            if ((UINT32)pt_msg->pv_nfy_tag == (UINT32)TRUE)
            {
                pt_v_ctrl->h_video = pt_v_ctrl->h_main_video;
            }
            else
            {
                pt_v_ctrl->h_video = pt_v_ctrl->h_sub_video;
            }
            x_crit_end(t_cs);
        }
        break;
        
        case MHEG5_MSG_SWITCH_AUDIO_PATH:
        {
            CRIT_STATE_T t_cs;
            SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;

            SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
            MHEG5_ASSERT(NULL != pt_a_ctrl);

            t_cs = x_crit_start();
            if ((UINT32)pt_msg->pv_nfy_tag == (UINT32)TRUE)
            {
                pt_a_ctrl->h_audio = pt_a_ctrl->h_main_audio;
            }
            else
            {
                pt_a_ctrl->h_audio = pt_a_ctrl->h_sub_audio;
            }
            x_crit_end(t_cs);
        }
        break;
        

        default:
            break;
    }

    return MHEG5R_OK;
}

INT32 sct_query_display_adjustment(
    const MHEG5_DISPLAY_PARAM_T*    pt_dspy_param,
    VSH_REGION_INFO_T*              pt_src_region, 
    VSH_REGION_INFO_T*              pt_dst_region)
{
    MHEG5_MSG_T             t_msg;
    CRIT_STATE_T            t_cs;
    INT32                   i4_ret         = MHEG5R_INV_STATE;
    BOOL                    b_vm_changed   = FALSE;
    BOOL                    b_video        = FALSE;
    BOOL                    b_osd          = FALSE;
    BOOL                    b_video_change = FALSE;
    MHEG5_VIDEO_MODE_T      e_vid_mode     = pt_dspy_param->e_vid_mode;
    SCC_VID_SCREEN_MODE_T   e_screen_mode  = pt_dspy_param->e_screen_mode;
#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    MPEG_2_PID_T            t_current_pid;
#endif
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);
    
    DBG_INFO((
        "{MHEG5 Stack} %s: AFD = %d, e_src_aspect_ratio = %d, src_w = %d, src_h = %d\r\n", 
        __FUNCTION__, 
        pt_dspy_param->ui1_afd,
        pt_dspy_param->e_src_aspect_ratio,
        pt_dspy_param->ui2_video_width,
        pt_dspy_param->ui2_video_height));
    
    t_cs = x_crit_start();

#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    t_current_pid = pt_v_ctrl->t_vs_pid;

    if ((!IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video)) ||
        (t_current_pid != pt_v_ctrl->t_vs_ori_pid && t_current_pid != 0) ||
        (pt_v_ctrl->b_iframe_set && IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video)))
    {
        e_screen_mode = sct_get_screen_auto_mode_enum();
        e_vid_mode    = MHEG5_VIDEO_MODE_AUTO;
    }

    b_osd = TRUE;
#endif

    pt_dis_cfg->ui1_afd            = pt_dspy_param->ui1_afd;
    pt_dis_cfg->e_src_aspect_ratio = pt_dspy_param->e_src_aspect_ratio;
    pt_dis_cfg->e_screen_mode      = e_screen_mode;

    if (PLAYED_FROM_STREAM == pt_v_ctrl->e_v_play_type)
    {
        pt_dis_cfg->ui4_real_vw = (UINT32) pt_dspy_param->ui2_video_width;
        pt_dis_cfg->ui4_real_vh = (UINT32) pt_dspy_param->ui2_video_height;
    }

    if (pt_dis_cfg->e_vid_mode != e_vid_mode)
    {
        pt_dis_cfg->e_vid_mode  = e_vid_mode;
        b_vm_changed = TRUE;
    }

#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    if (IS_MHEG5_RUNNING()&&
        ((!IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video)) ||
         (pt_dis_cfg->e_screen_mode == sct_get_screen_auto_mode_enum()) ||
         (t_current_pid != pt_v_ctrl->t_vs_ori_pid && t_current_pid != 0) ||
         (pt_v_ctrl->b_iframe_set && IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video))))
#else
    if (IS_MHEG5_RUNNING())
#endif

    {
        if (!_is_video_mode_supported(e_vid_mode))
        {
            e_vid_mode = MHEG5_VIDEO_MODE_FULLSCREEN;
        }

        b_video = b_vm_changed;

        _get_transformation(&pt_dis_cfg->e_decfc, &pt_dis_cfg->e_wss, pt_dis_cfg->ui1_afd, pt_dis_cfg->e_src_aspect_ratio);

            /* adjust video */
        if (FALSE == _calc_video_adjustment(
            e_vid_mode,
            pt_src_region, 
            pt_dst_region))
        {
            pt_src_region->ui4_x      = 0;
            pt_src_region->ui4_y      = 0;
            pt_src_region->ui4_width  = 0;
            pt_src_region->ui4_height = 0;

            *pt_dst_region = *pt_src_region;
        }

        if (IS_DIFF_REGION(&pt_dis_cfg->t_prv_src_rgn, pt_src_region) ||
            IS_DIFF_REGION(&pt_dis_cfg->t_prv_dst_rgn, pt_dst_region))
        {
            b_video_change = TRUE;
        }
        else
        {
            b_video_change = FALSE;
        }

        pt_dis_cfg->t_prv_src_rgn = *pt_src_region;
        pt_dis_cfg->t_prv_dst_rgn = *pt_dst_region;

    #if 0    
        if ((_e_wss != e_wss) || b_vm_changed)
    #endif
        {
            b_osd = TRUE;
        }

        _sct_post_video_adjustment(pt_dspy_param, pt_src_region, pt_dst_region);

        i4_ret = MHEG5R_OK;
    }
#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    else
    {
        if (MHEG5_DISPLAY_ASPECT_RATIO_16x9 == pt_dis_cfg->e_display_ar)
        {
            pt_dis_cfg->e_wss = MHEG5_WSS_16_9FF;
        }
        else
        {
            pt_dis_cfg->e_wss = MHEG5_WSS_4_3FF;
        }
    }
#endif

    x_crit_end(t_cs);

    if (i4_ret == MHEG5R_OK)
    {
        if (!IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video))
        {
            _set_internal_screen_mode(TRUE);
        }
        else if (t_current_pid != pt_v_ctrl->t_vs_ori_pid && t_current_pid != 0)
        {
            _set_internal_screen_mode(TRUE);
        }
        else if (pt_v_ctrl->e_v_ctrl == MHEG5_VIDEO_NONE && 
        	       pt_v_ctrl->b_v_stream_set == TRUE       &&
        	       pt_v_ctrl->b_vs_set == FALSE)
        {
            _set_internal_screen_mode(TRUE);
        }
        else
        {
            _set_internal_screen_mode(FALSE);
        }
    }
    else
    {
        _set_internal_screen_mode(FALSE);
    }

    if (b_video_change)
    {
        _sct_notify_video_update(NULL);
    }

    if (IS_MHEG5_RUNNING())
    {
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_RESET_OVERSCAN;
        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }
    
    if (b_video)
    {
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_NOTIFY_VIDEO_CHANGE;

        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }

    if (b_osd)
    {
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_ADJUST_OSD;

        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }
    
    /* 
        The following logic is wrote for passing the scene 5 of DTG Test 6.8.2A. In the test case, the 
        video would be froze and then scaled and repositioned, but as the limitation of the chip, we
        had unfreeze the video first before scaling and repositioning, so we have to chose a occasion
        to freeze the video again. And the following logic just do that thing.
        
        However in scene1 of DTG Test 6.8.2A, video would be froze and unfroze by MHEG5 application,
        when unfreeze video, this function would be invoked and the following logic would send a 
        message to SCT thread, so if this message is processed after x_svctx_unfreeze(), the video
        would be froze again in SCT thread, so the unfreeze operation would have no effects.
        Now we remove the following logic and the corresponding logic in SCT thread.
        
        To pass the scene 5 of 6.8.2A, we would freeze the video in another place.
    */
    #if 0
    if (_b_mheg5_running && _b_video_frozen)
    {
        MHEG5_VIDEO_UPDATE_NFY_T    t_data;

        _b_video_frozen = FALSE;

        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_SET_FREEZE;

        x_mheg5_send_msg(_ui4_eng_code, &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
        
        t_data.ui4_nfy_flag = MHEG5_VIDEO_UPDATE_PARAM_FREEZE;
        t_data.b_freeze     = _b_video_frozen;

        _sct_notify_video_update(&t_data);
    }
    #endif
    
    return (i4_ret);
    
}
INT32 sct_query_display_adjustment_ex(
    const MHEG5_DISPLAY_PARAM_T*    pt_dspy_param,
    VSH_REGION_INFO_T*              pt_src_region, 
    VSH_REGION_INFO_T*              pt_dst_region)
{
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    MHEG5_MSG_T             t_msg;
    INT32                   i4_ret = MHEG5R_OK;
    
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    
    MHEG5_ASSERT(NULL != pt_dis_cfg);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    
    _get_transformation(&pt_dis_cfg->e_decfc, &pt_dis_cfg->e_wss, pt_dis_cfg->ui1_afd, pt_dis_cfg->e_src_aspect_ratio);
    
    pt_dis_cfg->e_src_aspect_ratio = pt_dspy_param->e_src_aspect_ratio;
    pt_dis_cfg->e_screen_mode      = pt_dspy_param->e_screen_mode;
    pt_dis_cfg->ui1_afd            = pt_dspy_param->ui1_afd;
    pt_dis_cfg->e_vid_mode         = pt_dspy_param->e_vid_mode;
    
    if(IS_DIFF_REGION(&pt_dis_cfg->t_xlat_dst_rgn, pt_dst_region))
    {
       pt_dis_cfg->t_xlat_dst_rgn = *pt_dst_region;
    }

    if(IS_DIFF_REGION(&pt_dis_cfg->t_xlat_src_rgn, pt_src_region))
    {
       pt_dis_cfg->t_xlat_src_rgn = *pt_src_region;
    }

    if (PLAYED_FROM_STREAM == pt_v_ctrl->e_v_play_type)
    {
       pt_dis_cfg->t_orig_src_rgn.ui4_x      = 0;
       pt_dis_cfg->t_orig_src_rgn.ui4_y      = 0;
       pt_dis_cfg->t_orig_src_rgn.ui4_width  = (UINT32) pt_dspy_param->ui2_video_width;
       pt_dis_cfg->t_orig_src_rgn.ui4_height = (UINT32) pt_dspy_param->ui2_video_height;
    }
    
    _set_internal_screen_mode(FALSE);
    
    if(1)
    {
       x_memset(&t_msg, 0, sizeof(MHEG5_MSG_T));
       t_msg.ui1_module   = MHEG5_MOD_SCT;
       t_msg.ui2_msg_type = SCT_MSG_ADJUST_OSD_EX;
       
       x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }

    
    return i4_ret;
}

BOOL sct_query_display_b_auto_mode(VOID)
{
#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;
    
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);
    
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);
    
    if ((!IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video)) ||
        (pt_v_ctrl->b_iframe_set && IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video)))
    {
        return TRUE;
    }
#endif

    return FALSE;
}

VOID sct_set_screen_auto_mode_enum(SCC_VID_SCREEN_MODE_T e_auto_mode)
{
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);
    
    pt_dis_cfg->e_screen_auto_mode = e_auto_mode;
}

INT32 sct_set_default_lang(
    ISO_639_LANG_T  s639_lang)
{
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_a_ctrl);
    
    _lock_audio_module();
    
    x_memcpy(pt_a_ctrl->t_def_lang, s639_lang, sizeof(ISO_639_LANG_T));
    
    _unlock_audio_module();

    
#ifdef MHEG5_IC_SUPPORT
    mheg5_ics_handle_audio_lang_chg();
#endif

    return MHEG5R_OK;
}

INT32 sct_get_default_lang(
    ISO_639_LANG_T  s639_lang)
{
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_a_ctrl);
    
    _lock_audio_module();
    
    x_memcpy(s639_lang, pt_a_ctrl->t_def_lang, sizeof(ISO_639_LANG_T));

    _unlock_audio_module();

    return MHEG5R_OK;
}


INT32 sct_set_dvb_locator(UINT16 ui2_on_id, UINT16 ui2_ts_id, UINT16 ui2_svc_id)
{
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    
    MHEG5_ASSERT(NULL != pt_gnrl_info);

    pt_gnrl_info->ui2_launch_on_id  = ui2_on_id;
    pt_gnrl_info->ui2_launch_ts_id  = ui2_ts_id;
    pt_gnrl_info->ui2_launch_svc_id = ui2_svc_id;

    return MHEG5R_OK;
}

INT32 sct_set_gui_lang(
    ISO_639_LANG_T  s639_lang)
{
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_a_ctrl);

    _lock_audio_module();
    
    x_memcpy(pt_a_ctrl->t_gui_lang, s639_lang, sizeof(ISO_639_LANG_T));

    _unlock_audio_module();

    return MHEG5R_OK;
}

INT32 sct_get_gui_lang(
    ISO_639_LANG_T  s639_lang)
{
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_a_ctrl);

    _lock_audio_module();
    
    x_memcpy(s639_lang, pt_a_ctrl->t_gui_lang, sizeof(ISO_639_LANG_T));

    _unlock_audio_module();

    return MHEG5R_OK;
}


INT32 sct_set_default_overscan(SCC_VID_OVER_SCAN_CLIPPER_T *pt_clipper)
{
    MHEG5_MSG_T     t_msg;

    if (pt_clipper)
    {
        SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;
            
        SCT_GET_DISPLAY_CFG(pt_dis_cfg);
        MHEG5_ASSERT(NULL != pt_dis_cfg);
            
        pt_dis_cfg->t_dflt_ovs_clip = *pt_clipper;
    }

    if (IS_MHEG5_RUNNING())
    {
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_RESET_OVERSCAN;
        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }

    return MHEG5R_OK;
}

VOID sct_set_vdp_mode(SCC_VID_MODE_T e_mode, BOOL b_resume)
{
    GL_RECT_T   *pt_rect;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);   

    if (pt_v_ctrl->e_v_ctrl == MHEG5_VIDEO_NONE && e_mode == SCC_VID_BLANK && !pt_v_ctrl->b_iframe_began)
    {
        return;
    }
    
    pt_rect = (b_resume)? &pt_dis_cfg->t_resume_rc_video: &pt_dis_cfg->t_rc_video;

    if (!IS_FULL_SCREEN(pt_rect) || 
        /* check the user video state for [DTV00133931] since a full-screen MHEG video might be played */
        !pt_v_ctrl->b_v_user               ||
        /* check if there is an I-frame playing [DTV00134884] */
        (pt_v_ctrl->b_iframe_began && e_mode == SCC_VID_BLANK))
    {
    #if 0
        x_svctx_set_video_plane(_h_svctx, e_mode);
    #else
        _set_vdp_mode(e_mode);
    #endif
    }
}

VOID sct_set_video_info(VOID)
{
    INT32   i4_ret;
    
    if (!IS_MHEG5_RUNNING())
    {
        SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
        SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

        SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
        SCT_GET_DISPLAY_CFG(pt_dis_cfg);

        MHEG5_ASSERT(NULL != pt_v_ctrl);
        MHEG5_ASSERT(NULL != pt_dis_cfg);
    
        i4_ret = x_svctx_get_using_strm_data(
            SCT_GET_SVCTX_HANDLE(),
            ST_VIDEO,
            &pt_v_ctrl->t_v_comp_id,
            &pt_v_ctrl->t_v_scdb_rec);
    
        pt_v_ctrl->b_v_user = (SVCTXR_OK == i4_ret);
    
        if (IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video))
        {
            x_scc_vid_get_scr_mode(pt_v_ctrl->h_video, &pt_dis_cfg->e_vs_ori_scr_mode);
        }
    
        if (i4_ret == SVCTXR_OK)
        {
            pt_v_ctrl->t_vs_ori_pid = pt_v_ctrl->t_v_scdb_rec.u.t_video_mpeg.ui2_pid;
        }
        else
        {
            pt_v_ctrl->t_vs_ori_pid = 0;
        }
    }
}
INT32 sct_reg_video_update_nfy(x_mheg5_video_update_nfy pf_nfy, VOID* pv_tag)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    pt_v_ctrl->pf_video_update_nfy = pf_nfy;
    pt_v_ctrl->pv_video_update_nfy_tag = pv_tag;

    return MHEG5R_OK;
}

INT32 sct_reg_sbtl_visibility_nfy(x_mheg5_sbtl_visibility_nfy pf_nfy,  VOID* pv_tag)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);
    
    pt_v_ctrl->pf_sbtl_visibility_nfy = pf_nfy;
    pt_v_ctrl->pv_sbtl_visibility_nfy_tag = pv_tag;

    return MHEG5R_OK;
}

INT32 sct_set_sbtl_cfg_info(MHEG5_SUBTITLE_CFG_T* pt_cfg)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    if (pt_cfg != NULL)
    {
        #ifdef MHEG5_IC_SUPPORT
        if(pt_v_ctrl->t_sbtl_cfg.b_on != pt_cfg->b_on)
        {
            tmMHEG5NotifySubtitlePrefChanged();
        }
        #endif
        x_memcpy(&pt_v_ctrl->t_sbtl_cfg, pt_cfg, sizeof(MHEG5_SUBTITLE_CFG_T));
        #ifdef MHEG5_IC_SUPPORT
        mheg5_ics_handle_sbtl_visibility();        
        #endif      
        
        return MHEG5R_OK;
    }
    else
    {
        
        return MHEG5R_INV_ARG;
    }
}

INT32 sct_reg_vid_info_update_nfy(x_mheg5_vid_info_update_nfy pf_nfy, VOID* pv_tag)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    pt_v_ctrl->pf_vid_info_update_nfy =  pf_nfy;
    pt_v_ctrl->pv_video_update_nfy_tag = pv_tag;

    return MHEG5R_OK;
}

INT32 sct_get_vid_info(MHEG5_VID_INFO_T*  pt_vid_info)
{
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);

    pt_vid_info->b_mheg5_running = IS_MHEG5_RUNNING();

    pt_vid_info->ui1_afd    = pt_dis_cfg->ui1_afd ;
    
    pt_vid_info->e_src_aspect_ratio = pt_dis_cfg->e_src_aspect_ratio;
    
    pt_vid_info->e_wam      = (MHEG5_WAM_T)pt_dis_cfg->e_wam;
    pt_vid_info->e_scene_ar = (MHEG5_ASPECT_RATIO_T)pt_dis_cfg->e_scene_ar;
    
    return MHEG5R_OK;
}

INT32 sct_get_sbtl_cfg_info(MHEG5_SUBTITLE_CFG_T* pt_cfg)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    if (pt_cfg != NULL)
    {
        x_memcpy(pt_cfg, &pt_v_ctrl->t_sbtl_cfg, sizeof(MHEG5_SUBTITLE_CFG_T));
        
        return MHEG5R_OK;
    }
    else
    {
        
        return MHEG5R_INV_ARG;
    }
}


VOID _sct_notify_video_update(MHEG5_VIDEO_UPDATE_NFY_T *pt_data)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    if (pt_v_ctrl->pf_video_update_nfy != NULL)
    {
        pt_v_ctrl->pf_video_update_nfy(pt_data, pt_v_ctrl->pv_video_update_nfy_tag);
    }
}

VOID _sct_notify_vid_info_update(MHEG5_VID_INFO_UPDATE_NFY_T *pt_data)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    if ((pt_v_ctrl->pf_vid_info_update_nfy != NULL) &&
        IS_MHEG5_RUNNING())
    {
        pt_v_ctrl->pf_vid_info_update_nfy(pt_data, 
                                          pt_v_ctrl->pf_vid_info_update_nfy_tag);
    }
}

#if 1
VOID _sct_notify_sbtl_visibility(MHEG5_SBTL_STATE_T  e_show_state)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    static MHEG5_SBTL_STATE_T e_old_state = MHEG5_SBTL_STOP;
    
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    if (pt_v_ctrl->pf_sbtl_visibility_nfy != NULL &&
        e_old_state != e_show_state)
    {
        e_old_state = e_show_state;
        pt_v_ctrl->pf_sbtl_visibility_nfy(e_show_state, pt_v_ctrl->pv_sbtl_visibility_nfy_tag);
    }
    
#if 0    
    /* Try run */
    {
        
        SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
        SCT_GET_GNRL_INFO(pt_gnrl_info);
        BOOL b_show = TRUE;

        MHEG5_ASSERT(NULL != pt_gnrl_info);

        if (MHEG5_SBTL_HIDE == e_show_state)
        {
            b_show = FALSE;
        }
        
        #ifdef MHEG5_IC_SUPPORT
        x_svctx_set_stream_attr(
            pt_gnrl_info->h_sub_svctx,
            ST_SUBTITLE,
            (b_show) ? SM_SBTLH_SET_TYPE_VISIBLE: SM_SBTLH_SET_TYPE_INVISIBLE,
            NULL,
            0);
        #endif
        
        x_svctx_set_stream_attr(
            pt_gnrl_info->h_svctx,
            ST_SUBTITLE,
            (b_show) ? SM_SBTLH_SET_TYPE_VISIBLE: SM_SBTLH_SET_TYPE_INVISIBLE,
            NULL,
            0);
    }
#endif    
}
#endif
VOID sct_show_data(VOID)
{
#ifdef CLI_SUPPORT
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_v_ctrl);

    #define     MH5_DUMP(var)      x_dbg_stmt("%s:\t\t%d\n", #var, (INT32)var);
    
    x_dbg_stmt("=====================================\n");
    MH5_DUMP(pt_v_ctrl->e_v_ctrl);
    MH5_DUMP(pt_v_ctrl->i4_v_comp_tag);
    MH5_DUMP(pt_v_ctrl->t_vs_pid);
    MH5_DUMP(pt_a_ctrl->e_a_ctrl);
    MH5_DUMP(pt_a_ctrl->i4_a_comp_tag);
    MH5_DUMP(pt_a_ctrl->t_as_pid);
    x_dbg_stmt("=====================================\n");
#endif
}

INT32 sct_start(
    HANDLE_T    h_svctx,
    UINT16      ui2_svl_id, 
    UINT16      ui2_svl_rec_id)
{
    MHEG5_CNV_CFG_T t_cnv_cfg;
    CRIT_STATE_T    t_cs;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);

    if (pt_gnrl_info->b_mheg5_running == TRUE)
    {
        return MHEG5R_OK;
    }

    _lock_video_module();
    _lock_audio_module();

    pt_gnrl_info->h_svctx    = h_svctx;
    
    pt_v_ctrl->b_v_user   = FALSE;
    pt_v_ctrl->b_vs_set   = FALSE;
    pt_v_ctrl->b_v_pid_ok = FALSE;
    pt_v_ctrl->e_v_ctrl   = MHEG5_VIDEO_USER;

    pt_a_ctrl->b_a_user   = FALSE;
    pt_a_ctrl->b_as_set   = FALSE;
    pt_a_ctrl->b_a_pid_ok = FALSE;
    pt_a_ctrl->e_a_ctrl   = MHEG5_AUDIO_USER;

    pt_a_ctrl->ui2_a_svc_id = 0;
    pt_v_ctrl->ui2_v_svc_id = 0;

    pt_gnrl_info->ui2_launch_on_id  = 0;
    pt_gnrl_info->ui2_launch_ts_id  = 0;
    pt_gnrl_info->ui2_launch_svc_id = 0;

    pt_a_ctrl->t_a_pcr_pid = MPEG_2_NULL_PID;
    pt_v_ctrl->t_v_pcr_pid = MPEG_2_NULL_PID;

    pt_v_ctrl->t_vs_pid        = 0;
    pt_v_ctrl->t_vs_ori_pid    = 0;

    pt_v_ctrl->t_v_pid_to_be_resumed = MPEG_2_NULL_PID;

    pt_a_ctrl->b_default_audio_lang = FALSE;

    pt_v_ctrl->e_resume_v_ctrl = MHEG5_VIDEO_USER;
    pt_a_ctrl->e_resume_a_ctrl = MHEG5_AUDIO_USER;

    pt_v_ctrl->t_vid_enc = VID_ENC_MPEG_2;
    pt_v_ctrl->b_v_pid_updated = FALSE;

    pt_a_ctrl->t_aud_enc = AUD_ENC_MPEG_2;

#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    pt_v_ctrl->b_v_stream_set = FALSE;
    pt_v_ctrl->b_iframe_set   = FALSE;

    x_scc_vid_get_scr_mode(pt_v_ctrl->h_video, &pt_dis_cfg->e_vs_ori_scr_mode);
#endif
    
    _save_current_stream(ST_VIDEO);
    _save_current_stream(ST_AUDIO);
    
    t_cs = x_crit_start();
    
    pt_dis_cfg->e_wam = MHEG5_ALIGNMENT_CENTRE_CUT_OUT;

    if (MHEG5_DISPLAY_ASPECT_RATIO_4x3 == pt_dis_cfg->e_display_ar)
    {
        pt_dis_cfg->e_wss = MHEG5_WSS_4_3FF;
    }
    else
    {
        pt_dis_cfg->e_wss = MHEG5_WSS_16_9FF;
    }

    pt_dis_cfg->e_decfc    = MHEG5_DISPLAY_AR_CONVERSION_UNKNOWN;
    pt_dis_cfg->e_scene_ar = MHEG5_ASPECT_RATIO_UNSEPCIFIED;
    pt_dis_cfg->e_src_aspect_ratio = MHEG5_ASPECT_RATIO_UNKNOWN;
    pt_dis_cfg->ui1_afd            = 0;
    
    SET_RECT_BY_SIZE(&pt_dis_cfg->t_rc_video, 0, 0, MHEG5_SCREEN_WIDTH, MHEG5_SCREEN_HEIGHT);
    SET_RECT_BY_SIZE(&pt_dis_cfg->t_resume_rc_video, 0, 0, MHEG5_SCREEN_WIDTH, MHEG5_SCREEN_HEIGHT);

    pt_dis_cfg->b_adjust_overscan = FALSE;
    
    pt_gnrl_info->b_mheg5_running = TRUE;

    t_cnv_cfg.h_scc_video  = pt_v_ctrl->h_video;
    t_cnv_cfg.e_decfc      = _convert_decfc(pt_dis_cfg->e_decfc);
    t_cnv_cfg.e_wss        = pt_dis_cfg->e_wss;
    t_cnv_cfg.e_vid_mode   = pt_dis_cfg->e_vid_mode;
    t_cnv_cfg.e_display_ar = 
        ((MHEG5_DISPLAY_ASPECT_RATIO_4x3 == pt_dis_cfg->e_display_ar) ?
            MHEG5_DISPLAY_ASPECT_RATIO_4_3 : MHEG5_DISPLAY_ASPECT_RATIO_16_9);
            
    x_crit_end(t_cs);
    
    x_mheg5_adjust_osd(SCT_GET_ENGINE_CODE(), &t_cnv_cfg);
    
    _unlock_audio_module();
    _unlock_video_module();
    
    return MHEG5R_OK;
}

VOID sct_update_screen_mode(SCC_VID_SCREEN_MODE_T e_screen_mode)
{
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;
    
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);
    
    MHEG5_ASSERT(NULL != pt_dis_cfg);   
    
    pt_dis_cfg->e_vs_ori_scr_mode = e_screen_mode;
}

VOID sct_reset_state(VOID)
{
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_gnrl_info);
    
    if (pt_gnrl_info->b_mheg5_running)
    {
        pt_gnrl_info->b_mheg5_running = FALSE;
    }
}

INT32 sct_resume(VOID)
{
    INT32                       i4_ret = MHEG5R_INTERNAL_ERROR;
    tmMHEG5Err_t                t_err;
    GL_RECT_T                   t_rc_video;
    tmMHEG5VideoControl_t       e_v_ctrl;
    tmMHEG5AudioControl_t       e_a_ctrl;
    MHEG5_VIDEO_UPDATE_NFY_T    t_data;
    S_VideoPosition             position;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);

    #if 0
    if (pt_gnrl_info->b_mheg5_running == TRUE)
    {
        return MHEG5R_OK;
    }
    #endif

    _lock_video_module();
    _lock_audio_module();

    pt_gnrl_info->b_mheg5_running = TRUE;
    
    pt_dis_cfg->t_rc_video = pt_dis_cfg->t_resume_rc_video;
    t_rc_video  = pt_dis_cfg->t_rc_video;
    e_v_ctrl    = pt_v_ctrl->e_resume_v_ctrl;
    e_a_ctrl    = pt_a_ctrl->e_resume_a_ctrl;

    x_scc_vid_get_scr_mode(pt_v_ctrl->h_video, &pt_dis_cfg->e_vs_ori_scr_mode);

    /* consider the MTS / AUDIO selection case */
    if (pt_a_ctrl->e_resume_a_ctrl == MHEG5_AUDIO_MHEG && pt_a_ctrl->b_default_audio_lang)
    {
        SCDB_REC_T      t_scdb_rec;

        if (x_svctx_get_using_scdb_rec(
                pt_gnrl_info->h_svctx,
                ST_AUDIO,
                &t_scdb_rec) == SVCTXR_OK)
        {
            pt_a_ctrl->t_as_pid = t_scdb_rec.u.t_audio_mpeg.ui2_pid;
        }
    }

    /* reset current PID for resuming. Because tmMHEG5SetVideoControl(MHEG5_VIDEO_NONE) would reset _t_vs_pid previously */
    if (pt_v_ctrl->t_v_pid_to_be_resumed != 0)
    {
        pt_v_ctrl->t_vs_pid = pt_v_ctrl->t_v_pid_to_be_resumed;
    }
    
    _unlock_audio_module();
    _unlock_video_module();

    t_err = tmMHEG5SetVideoControl(e_v_ctrl);
    t_err = tmMHEG5SetAudioControl(e_a_ctrl);

    position.scaling.left = t_rc_video.i4_left;
    position.scaling.top = t_rc_video.i4_top;
    position.scaling.width = (U32BIT)RECT_W(&t_rc_video);
    position.scaling.height = (U32BIT)RECT_H(&t_rc_video);
    DVP_VideoPosition(&position);

    
    /* reset the default video mode of SVCTX. Or the "change TV" function could not work properly in BBCi of 
       CD29_BBC_641833000.trp after pausing and resuming [DTV00133479] */
    _set_vdp_mode(SCC_VID_NORMAL);

    if (pt_v_ctrl->b_video_frozen)
    {
        x_svctx_freeze(pt_gnrl_info->h_svctx, ST_MASK_VIDEO);

        t_data.b_freeze     = pt_v_ctrl->b_video_frozen;
        t_data.ui4_nfy_flag = MHEG5_VIDEO_UPDATE_PARAM_FREEZE;

        _sct_notify_video_update(&t_data);
    }

    x_scc_aud_switch(pt_a_ctrl->h_audio, !pt_a_ctrl->b_audio_mute);

    _lock_audio_module();
    _aclip_restart();
    _unlock_audio_module();

    if (t_err == MHEG5_OK)
    {
        i4_ret = MHEG5R_OK;
    }
    
    return i4_ret;
}

INT32 sct_stop(MHEG5_STOP_SVC_PARAM_T *pt_param)
{
    CRIT_STATE_T            t_cs;
    SCC_VID_SCREEN_MODE_T   e_mode;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);

    if (pt_gnrl_info->b_mheg5_running == FALSE)
    {
        return MHEG5R_OK;
    }
    
    t_cs = x_crit_start();
    
    pt_gnrl_info->b_mheg5_running = FALSE;
    pt_v_ctrl->t_vs_pid        = 0;
    pt_v_ctrl->t_vs_ori_pid    = 0;

    pt_v_ctrl->b_video_frozen  = FALSE;
    pt_a_ctrl->b_audio_mute    = FALSE;

    pt_gnrl_info->ui2_launch_on_id  = 0;
    pt_gnrl_info->ui2_launch_ts_id  = 0;
    pt_gnrl_info->ui2_launch_svc_id = 0;

    SET_RECT(&pt_dis_cfg->t_rc_video, 0, 0, MHEG5_SCREEN_WIDTH, MHEG5_SCREEN_HEIGHT);
    SET_RECT(&pt_dis_cfg->t_resume_rc_video, 0, 0, MHEG5_SCREEN_WIDTH, MHEG5_SCREEN_HEIGHT);

    x_crit_end(t_cs);

    /* force the video region to be adjusted since sct_proc_msg() would have no effect 
       when MHEG-5 is not running [DTV00134990] */
    if (x_scc_vid_get_scr_mode(pt_v_ctrl->h_video, &e_mode) == SMR_OK)
    {
        x_scc_vid_set_scr_mode(pt_v_ctrl->h_video, e_mode);
    }

    _set_av_sync_data(TRUE);

#if 0
    x_svctx_set_video_plane(_h_svctx, SCC_VID_NORMAL);
#else

    if ((pt_param == NULL) ||
        ((pt_param->ui4_op_flag & MHEG5_STOP_SVC_PARAM_ADJ_VDP) &&
         (pt_param->b_adjust_vdp == TRUE)))
    {
        _set_vdp_mode(SCC_VID_NORMAL);
    }
#endif

    _lock_audio_module();

    if (pt_a_ctrl->b_aclip_enabled)
    {
        #if 0
        tmMHEG5AudioStop();
        tmMHEG5NotifyAudioPlayStopped();
        #else
        /* Marked for NDT support */
        #if 0
        _aclip_lock();
        
        pt_a_ctrl->b_resume_audio_clip = FALSE;
        _aclip_stop(TRUE, FALSE);
        
        _aclip_unlock();
        #endif
        #endif
    }
    _set_stream_select_block(NULL, FALSE);
    _unlock_audio_module();
    
    return MHEG5R_OK;
}

BOOL sct_is_mheg5_running()
{
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    
    return (pt_gnrl_info->b_mheg5_running);
}


static VOID _save_configuration(_STREAM_CONFIG_T* pt_cfg)
{
}

static VOID _restore_configuration(_STREAM_CONFIG_T* pt_cfg)
{
}

INT32 sct_restore_default(VOID)
{
    _lock_video_module();
    _lock_audio_module();

    _save_configuration(&_t_cfg_mheg5);

    _restore_configuration(&_t_cfg_default);
    
    _unlock_audio_module();
    _unlock_video_module();

    return MHEG5R_OK;
}

INT32 sct_restore_mheg5(VOID)
{
    _lock_video_module();
    _lock_audio_module();

    _save_configuration(&_t_cfg_default);

    _restore_configuration(&_t_cfg_mheg5);
    
    _unlock_audio_module();
    _unlock_video_module();

    return MHEG5R_OK;
}

INT32 sct_audio_desc_pref_changed(VOID)
{
    #ifdef MHEG5_IC_SUPPORT
    if(MHEG5_OK == tmMHEG5NotifyAudioDescPrefChanged())
    {
        return MHEG5R_OK;
    }
    else
    {
        return MHEG5R_INTERNAL_ERROR;
    }
    #else
    return MHEG5R_INTERNAL_ERROR;
    #endif
}


INT32 sct_reg_ad_cb_fct(
    x_mheg5_ad_fct      pf_fct, 
    VOID*               pv_tag)
{
    _pf_ad_fct     = pf_fct;
    _pv_ad_fct_tag = pv_tag;
    
    return MHEG5R_OK;
}

/* A/V/S playback */
tmMHEG5Err_t tmMHEG5PlayAudioClip( U8BIT* pAudioData,
                                          U32BIT dataLength,
                                          U32BIT repeatCount,
                                          tmMH5AudioEncoding_t encoding )
{
    tmMHEG5Err_t    t_err;
    INT32           i4_ret;
    SCC_AUD_CLIP_T  t_aud_clip;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);

    DBG_INFO((
        "{MHEG5 Stack} %s: pAudioData = %d, dataLengthe = %d, repeatCount = %d\r\n",
        __FUNCTION__,
        (UINT32) pAudioData,
        dataLength,
        repeatCount));
    
    /* if repeatCount == 0, then loop infinitely */
    
    if (pAudioData == NULL || dataLength == 0 || 
        ((encoding != MHEG5_MPEG2_AUDIO)&&(encoding != MHEG5_HEAAC_AUDIO)&&(encoding != MHEG5_E_AC3_AUDIO)))
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }

    _lock_audio_module();

    DBG_INFO(("{MHEG5 Stack} %s: Before acquiring INITIAL lock.\r\n", __FUNCTION__));
    
    _aclip_lock();
    DBG_INFO(("{MHEG5 Stack} %s: Acquired INITIAL lock...\r\n", __FUNCTION__));
    if (MHEG5R_INTERNAL_ERROR == _aclip_stop(TRUE, FALSE))
    {
        DBG_ERROR(("{MHEG5 Stack} Close audio clip component failed.\r\n"));
        _aclip_unlock();
        _unlock_audio_module(); 
        return MHEG5_ERR_OTHER; 
    }
    
    _save_current_stream(ST_AUDIO);
    
    /* close the audio service */
    _stop_current_stream(ST_AUDIO);

    x_thread_delay(100);
    
    t_err = MHEG5_ERR_OTHER;

    /* The MPEG2 audio data passed MUST be copied before use, as the caller retains exclusive ownership */
    pt_a_ctrl->pui1_audio_buffer = (UINT8*) x_mem_realloc((VOID*)pt_a_ctrl->pui1_audio_buffer, (SIZE_T) (dataLength * sizeof(UINT8)));
    
    if (pt_a_ctrl->pui1_audio_buffer)
    {
        x_memcpy((VOID*) pt_a_ctrl->pui1_audio_buffer, pAudioData, (SIZE_T) dataLength);

        /* setup the data of this audio clip */
        /* all output port */
        t_aud_clip.ui4_out_port_mask              = ~SCC_AUD_OUT_PORT_MASK_NONE;
        /* all output channel */
        t_aud_clip.ui4_ch_mask                    = ~SCC_AUD_CH_MASK_NONE;
        t_aud_clip.ui4_repeat_cnt                 = (INT32)repeatCount;
        if (encoding == MHEG5_MPEG2_AUDIO)
        {
            t_aud_clip.u.t_clip_buff.e_fmt            = ASH_AUDIO_FMT_MPEG;
        }
        else if (encoding == MHEG5_HEAAC_AUDIO)
        {
            t_aud_clip.u.t_clip_buff.e_fmt            = ASH_AUDIO_FMT_AAC;
        }
        else
        {
            t_aud_clip.u.t_clip_buff.e_fmt            = ASH_AUDIO_FMT_E_AC3;
        }
        
        t_aud_clip.u.t_clip_buff.pui1_clip_buff   = (UINT8*) pt_a_ctrl->pui1_audio_buffer;
        t_aud_clip.u.t_clip_buff.z_clip_buff_size = (SIZE_T) dataLength;
        t_aud_clip.e_type                         = SCC_AUD_CLIP_TYPE_CLIP_BUFF;

        i4_ret = x_scc_aud_set_clip(
            pt_a_ctrl->h_audio, 
            &t_aud_clip, 
            _aclip_nfy_fct, 
            (VOID*) &pt_a_ctrl->t_a_dec_info);
        if (SMR_OK == i4_ret)
        {
            pt_a_ctrl->b_aclip_enabled= TRUE;

            i4_ret = x_scc_aud_set_clip_ctrl(pt_a_ctrl->h_audio, SCC_AUD_CTRL_MODE_PLAY);
            if (SMR_OK == i4_ret)
            {
                _aclip_lock();
                DBG_INFO(("{MHEG5 Stack} x_scc_aud_set_clip_ctrl() = %d succeeded.\n", i4_ret));
                pt_a_ctrl->e_a_play_type = PLAYED_FROM_MEMORY;
                pt_a_ctrl->ui4_audio_buffer_len = dataLength;
                pt_a_ctrl->t_aclip_encoding = encoding;
                t_err = MHEG5_OK;
            }
            else
            {
                DBG_ERROR(("{MHEG5 Stack} x_scc_aud_set_clip_ctrl() = %d failed.\n", i4_ret));
            }
        }
        else
        {
            DBG_ERROR(("{MHEG5 Stack} x_scc_aud_set_clip() = %d failed.\n", i4_ret));
        }
    }
    else
    {
        DBG_ERROR(("{MHEG5 Stack} %s: Out of memory.\n", __FUNCTION__));
    }
    
        
    _aclip_unlock();

    _unlock_audio_module();
    
    return (t_err);
}

tmMHEG5Err_t tmMHEG5AudioStop(void)
{
    tmMHEG5Err_t t_err;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);

    DBG_INFO((
        "{MHEG5 Stack} %s:\r\n",
        __FUNCTION__));
    
    _lock_audio_module();

    _aclip_lock();
    DBG_INFO(("{MHEG5 Stack} %s: Acquired STOP lock...\r\n", __FUNCTION__));
    
    t_err = MHEG5_OK;
    
    if (MHEG5R_OK == _aclip_stop(TRUE, FALSE))
    {
        t_err = MHEG5_OK;
    }
    else
    {
        t_err = MHEG5_ERR_OTHER;
    }
    
    _aclip_unlock();
        
    pt_a_ctrl->e_a_play_type = PLAYED_FROM_STREAM;
    
    _unlock_audio_module();
    
    return (t_err);
}

tmMHEG5Err_t tmMHEG5GetCurrentFormatConversion(tmMHEG5DisplayAspectRatioConversion_t *decFC)
{
    CRIT_STATE_T t_cs;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);

    DBG_INFO((
        "{MHEG5 Stack} %s:\r\n",
        __FUNCTION__));
    
    if (decFC == NULL)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }

    t_cs = x_crit_start();

    *decFC = pt_dis_cfg->e_decfc;
    
    x_crit_end(t_cs);
    
    return MHEG5_OK;
}

tmMHEG5Err_t tmMHEG5GetDisplayAspectRatio(tmMHEG5DispAspectRatio_t *aspectRatio)
{
    CRIT_STATE_T                t_cs;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);

    DBG_INFO((
        "{MHEG5 Stack} %s:\r\n",
        __FUNCTION__));
    
    if (aspectRatio == NULL)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }

    t_cs = x_crit_start();

    *aspectRatio = pt_dis_cfg->e_display_ar;
    
    x_crit_end(t_cs);

    return MHEG5_OK;
}

tmMHEG5Err_t tmMHEG5SetAudioControl(tmMHEG5AudioControl_t control)
{
    tmMHEG5Err_t    t_err;
    INT32           i4_ret;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_a_ctrl);
    
    DBG_INFO((
        "{MHEG5 Stack} %s: control = %d\r\n",
        __FUNCTION__,
        control));

    if ((MHEG5_AUDIO_NONE != control) &&
        (MHEG5_AUDIO_MHEG != control) &&
        (MHEG5_AUDIO_USER != control))
    {
        DBG_ERROR(("{MHEG5 Stack} %s: Invalid parameter.\r\n", __FUNCTION__));
        return MHEG5_ERR_BAD_PARAMETER;
    }

    /* MHEG engine sends commands when it's in pause mode */
    if (_in_mheg_context() && !pt_gnrl_info->b_mheg5_running)
    {
        if (control == MHEG5_AUDIO_NONE)
        {
            pt_a_ctrl->e_resume_a_ctrl = control;
        }
        /* should have no action [DTV00134049] */
        else
        {
            STREAM_COMP_ID_T    t_comp_id;
            SCDB_REC_T          t_scdb;
            BOOL                b_with_as = FALSE;
            
            b_with_as = (x_svctx_get_using_strm_data(pt_gnrl_info->h_svctx, ST_AUDIO, &t_comp_id, &t_scdb) == SVCTXR_OK);
            
            if ( pt_a_ctrl->e_a_ctrl == MHEG5_AUDIO_USER && b_with_as == TRUE)
            {
                 pt_a_ctrl->e_resume_a_ctrl = control;
                
                return MHEG5_ERR_OTHER;
            }
        }
    }

    _lock_audio_module();
    
    t_err = MHEG5_ERR_OTHER;
    
    if (PLAYED_FROM_STREAM == pt_a_ctrl->e_a_play_type)
    {
        if (pt_a_ctrl->e_a_ctrl != control)
        {
            i4_ret = SVCTXR_OK;

            switch (control)
            {
                case MHEG5_AUDIO_NONE:
                {
                    STREAM_COMP_ID_T    t_comp_id;
                    SCDB_REC_T          t_scdb;
                    BOOL                b_with_as = FALSE;
                    
                    _set_stream_select_block(NULL, TRUE);
                    _save_current_stream(ST_AUDIO);

                    b_with_as = (x_svctx_get_using_strm_data(pt_gnrl_info->h_svctx, ST_AUDIO, &t_comp_id, &t_scdb) == SVCTXR_OK);

                    if (b_with_as)
                    {
                        i4_ret = x_svctx_sync_stop_stream(pt_gnrl_info->h_svctx, ST_AUDIO, 0); 
                    }

                    if (pt_a_ctrl->e_a_ctrl == MHEG5_AUDIO_MHEG)
                    {         
                        /* [DTV00316506] engine wants to be notified once stream stop event happened */
                        tmMHEG5NotifyAudioStopped((BOOLEAN)FALSE);

                        /* [DTV00327231] when engine sets control to NONE, the saved stream states must be reset */
                        if(_in_mheg_context())
                        {
                            pt_a_ctrl->b_as_set   = FALSE;
                            pt_a_ctrl->b_a_pid_ok = FALSE;
                        } 
                     }                    
                    break;
                }

                case MHEG5_AUDIO_MHEG:
                    if (MHEG5_AUDIO_NONE == pt_a_ctrl->e_a_ctrl)
                    {
                        if (pt_a_ctrl->b_as_set && pt_a_ctrl->b_a_pid_ok)
                        {
                            #if 1
                            SVCTX_STREAM_INFO_T    t_strm_info;

                            t_strm_info.e_strm_type              = ST_AUDIO;
                            t_strm_info.t_mpeg_2_pid             = pt_a_ctrl->t_as_pid;
                            t_strm_info.u.t_aud_stream.e_aud_enc = pt_a_ctrl->t_aud_enc;

                            
                            i4_ret = x_svctx_select_mpeg_stream_by_pid_ex(
                                 pt_gnrl_info->h_svctx,
                                 &t_strm_info,
                                 _select_stream_by_pid_nfy,
                                 NULL);
                            t_strm_info.e_strm_type              = ST_AUDIO;
                            t_strm_info.t_mpeg_2_pid             = pt_a_ctrl->t_ad_as_pid;
                            t_strm_info.u.t_aud_stream.e_aud_enc = pt_a_ctrl->t_ad_aud_enc;            
                            
                            i4_ret = x_svctx_select_mpeg_ad_stream_by_pid(
                                 pt_gnrl_info->h_svctx,
                                 &t_strm_info,
                                 _select_stream_by_pid_nfy,
                                 NULL);
                            #else
                            i4_ret = x_svctx_select_mpeg_stream_by_pid(
                                pt_gnrl_info->h_svctx, 
                                ST_AUDIO, 
                                pt_a_ctrl->t_as_pid, 
                                _select_stream_by_pid_nfy, 
                                NULL);
                            #endif

                            _set_stream_select_block(&pt_a_ctrl->t_as_pid, FALSE);
            
                            _set_av_sync_data(FALSE);

                            _lock_a_v_sync(AUDIO_SYNC);

                            tmMHEG5NotifyAudioStarted((BOOLEAN)FALSE);
                        }
                    }
                    else
                    {
                        _save_current_stream(ST_AUDIO);
                        
                        if ((!pt_a_ctrl->b_a_user)   ||
                            (!pt_a_ctrl->b_as_set)   ||
                            (!pt_a_ctrl->b_a_pid_ok) ||
                            (pt_a_ctrl->t_as_pid != pt_a_ctrl->t_a_scdb_rec.u.t_audio_mpeg.ui2_pid))
                        {
                            i4_ret = SVCTXR_OK;
                            
                            if (pt_a_ctrl->t_as_pid != 0 && pt_a_ctrl->t_as_pid != MPEG_2_NULL_PID)
                            {
                                i4_ret = _stop_current_stream(ST_AUDIO);
                                
                                if ((SVCTXR_OK == i4_ret) && (pt_a_ctrl->b_as_set) && (pt_a_ctrl->b_a_pid_ok))
                                {
                                    SIZE_T                      z_status_size = sizeof(ASH_AUDIO_DECODE_STATUS_T);
                                    ASH_AUDIO_DECODE_STATUS_T   t_status = ASH_AUDIO_DECODE_UNKNOWN;
                                    
                                    #if 1
                                    SVCTX_STREAM_INFO_T    t_strm_info;
                                    
                                    t_strm_info.e_strm_type              = ST_AUDIO;
                                    t_strm_info.t_mpeg_2_pid             = pt_a_ctrl->t_as_pid;
                                    t_strm_info.u.t_aud_stream.e_aud_enc = pt_a_ctrl->t_aud_enc;
                                    
                                    
                                    i4_ret = x_svctx_select_mpeg_stream_by_pid_ex(
                                         pt_gnrl_info->h_svctx,
                                         &t_strm_info,
                                         _select_stream_by_pid_nfy,
                                         NULL);
                                    t_strm_info.e_strm_type              = ST_AUDIO;
                                    t_strm_info.t_mpeg_2_pid             = pt_a_ctrl->t_ad_as_pid;
                                    t_strm_info.u.t_aud_stream.e_aud_enc = pt_a_ctrl->t_ad_aud_enc;            
                                    
                                    i4_ret = x_svctx_select_mpeg_ad_stream_by_pid(
                                         pt_gnrl_info->h_svctx,
                                         &t_strm_info,
                                         _select_stream_by_pid_nfy,
                                         NULL);
                                    #else
                                    i4_ret = x_svctx_select_mpeg_stream_by_pid(
                                        pt_gnrl_info->h_svctx, 
                                        ST_AUDIO, 
                                        pt_a_ctrl->t_as_pid, 
                                        _select_stream_by_pid_nfy, 
                                        NULL);
                                    #endif

                                    pt_a_ctrl->e_a_ctrl = control;

                                    _set_stream_select_block(&pt_a_ctrl->t_as_pid, FALSE);
                                    
                                    _set_av_sync_data(FALSE);

                                    _lock_a_v_sync(AUDIO_SYNC);

                                    tmMHEG5NotifyAudioStarted((BOOLEAN)FALSE);

                                    i4_ret = x_svctx_get(
                                        pt_gnrl_info->h_svctx,
                                        SVCTX_COMMON_GET_AUDIO_DECODE_STATUS,
                                        (VOID *)&t_status,
                                        &z_status_size);

                                    if (i4_ret         == SVCTXR_OK                         &&
                                        t_status       == ASH_AUDIO_DECODE_NORMAL           &&
                                        z_status_size  == sizeof(ASH_AUDIO_DECODE_STATUS_T) &&
                                        pt_a_ctrl->i4_a_comp_tag == MHEG5_DEF_COMP_TAG                &&
                                        !pt_a_ctrl->b_a_sync_timeout)
                                    {
                                        _save_current_stream(ST_AUDIO);
                                    }
                                }
                            }
                            else if ((pt_a_ctrl->b_as_set) && (!pt_a_ctrl->b_a_pid_ok))
                            {
                                _stop_current_stream(ST_AUDIO);
                                i4_ret = SVCTXR_OK;
                            }
                        }
                        else  /* [DTV00316506] engine wants to be notified once stream playing event happened */
                        {
                            tmMHEG5NotifyAudioStarted((BOOLEAN)FALSE);
                        }
                    }                    
                   
                    break;

                case MHEG5_AUDIO_USER:
                    _set_stream_select_block(NULL, FALSE);
                    
                    if (MHEG5_AUDIO_NONE == pt_a_ctrl->e_a_ctrl)
                    {
                        if (pt_a_ctrl->b_a_user)
                        {
                            i4_ret = x_svctx_select_stream(
                                pt_gnrl_info->h_svctx,
                                &pt_a_ctrl->t_a_comp_id,
                                NULL,
                                NULL);

                            _set_av_sync_data(TRUE);

                            /* [DTV00130210] The video and the audio is not synchronous in MHEG5 page */ 
                            pt_a_ctrl->e_a_ctrl = control;
                        }
                        else
                        {
                            tmMHEG5DvbLocator_t     t_dvb_loc;

                            t_dvb_loc.original_network_id = pt_gnrl_info->ui2_launch_on_id;
                            t_dvb_loc.transport_stream_id = pt_gnrl_info->ui2_launch_ts_id;
                            t_dvb_loc.service_id          = pt_gnrl_info->ui2_launch_svc_id;
                            t_dvb_loc.ComponentTag        = -1;

                            if (sil_is_pmt_available() && tmMHEG5SetAudioStream(&t_dvb_loc) == MHEG5_OK)
                            {
                                if ((pt_a_ctrl->b_as_set) && (pt_a_ctrl->b_a_pid_ok))
                                {
                                    SIZE_T                      z_status_size = sizeof(ASH_AUDIO_DECODE_STATUS_T);
                                    ASH_AUDIO_DECODE_STATUS_T   t_status = ASH_AUDIO_DECODE_UNKNOWN;
                                    
                                    #if 1
                                    SVCTX_STREAM_INFO_T    t_strm_info;
                                    
                                    t_strm_info.e_strm_type              = ST_AUDIO;
                                    t_strm_info.t_mpeg_2_pid             = pt_a_ctrl->t_as_pid;
                                    t_strm_info.u.t_aud_stream.e_aud_enc = pt_a_ctrl->t_aud_enc;
                                    
                                    
                                    i4_ret = x_svctx_select_mpeg_stream_by_pid_ex(
                                         pt_gnrl_info->h_svctx,
                                         &t_strm_info,
                                         _select_stream_by_pid_nfy,
                                         NULL);
                                    t_strm_info.e_strm_type              = ST_AUDIO;
                                    t_strm_info.t_mpeg_2_pid             = pt_a_ctrl->t_ad_as_pid;
                                    t_strm_info.u.t_aud_stream.e_aud_enc = pt_a_ctrl->t_ad_aud_enc;            
                                    
                                    i4_ret = x_svctx_select_mpeg_ad_stream_by_pid(
                                         pt_gnrl_info->h_svctx,
                                         &t_strm_info,
                                         _select_stream_by_pid_nfy,
                                         NULL);
                                    #else
                                    i4_ret = x_svctx_select_mpeg_stream_by_pid(
                                        pt_gnrl_info->h_svctx, 
                                        ST_AUDIO, 
                                        pt_a_ctrl->t_as_pid, 
                                        _select_stream_by_pid_nfy, 
                                        NULL);
                                    #endif

                                    pt_a_ctrl->e_a_ctrl = control;

                                    _set_stream_select_block(&pt_a_ctrl->t_as_pid, FALSE);
                                    
                                    _set_av_sync_data(FALSE);

                                    _lock_a_v_sync(AUDIO_SYNC);

                                    i4_ret = x_svctx_get(
                                        pt_gnrl_info->h_svctx,
                                        SVCTX_COMMON_GET_AUDIO_DECODE_STATUS,
                                        (VOID *)&t_status,
                                        &z_status_size);

                                    if (i4_ret         == SVCTXR_OK                         &&
                                        t_status       == ASH_AUDIO_DECODE_NORMAL           &&
                                        z_status_size  == sizeof(ASH_AUDIO_DECODE_STATUS_T) &&
                                        pt_a_ctrl->i4_a_comp_tag == MHEG5_DEF_COMP_TAG                &&
                                        !pt_a_ctrl->b_a_sync_timeout)
                                    {
                                        _save_current_stream(ST_AUDIO);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                    #ifdef MW_PIP_POP_SUPPORT
                        if (_in_mheg_context() && !pt_gnrl_info->b_mheg5_running)
                        {
                            SVCTX_SCREEN_STATUS_T e_scr_status;
                            SIZE_T z_size;

                            z_size = sizeof(SVCTX_SCREEN_STATUS_T);
                            i4_ret = x_svctx_get( pt_gnrl_info->h_svctx, 
                                                  SVCTX_COMMON_GET_SCREEN_STATUS,
                                                  (VOID *)&e_scr_status,
                                                  (VOID *)&z_size);
                            if ((i4_ret == SVCTXR_OK) &&
                                (e_scr_status == SVCTX_SCREEN_STATUS_MULTIPLE_FULL ||
                                 e_scr_status == SVCTX_SCREEN_STATUS_MULTIPLE_PART))
                            {
                                /* PIP/POP mode */
                                break;
                            }
                        }
                    #endif
                        _save_current_stream(ST_AUDIO);
                        if ((!pt_a_ctrl->b_a_user)   ||
                            (!pt_a_ctrl->b_as_set)   ||
                            (!pt_a_ctrl->b_a_pid_ok) ||
                            (pt_a_ctrl->t_as_pid != pt_a_ctrl->t_a_scdb_rec.u.t_audio_mpeg.ui2_pid))
                        {
                            i4_ret = _stop_current_stream(ST_AUDIO);
                            tmMHEG5NotifyAudioStopped((BOOLEAN)FALSE);
                            if ((SVCTXR_OK == i4_ret) && (pt_a_ctrl->b_a_user))
                            {
                                i4_ret = x_svctx_select_stream(
                                    pt_gnrl_info->h_svctx,
                                    &pt_a_ctrl->t_a_comp_id,
                                    NULL,
                                    NULL);

                                _set_av_sync_data(TRUE);

                                /* [DTV00130210] The video and the audio is not synchronous in MHEG5 page */
                                pt_a_ctrl->e_a_ctrl = control;
                            }
                        }
                    }
                    break;

                default:
                    i4_ret = MHEG5R_INTERNAL_ERROR;
                    break;
            }

            if (SVCTXR_OK == i4_ret)
            {
                pt_a_ctrl->e_a_ctrl = control;
                t_err = MHEG5_OK;
            }
        }    
        else
        {
            t_err = MHEG5_OK;
        }
    }
    else
    {
        pt_a_ctrl->e_a_ctrl = control;
        
        t_err = MHEG5_OK;
    }
    
    _unlock_audio_module();
    
    return (t_err);
}

tmMHEG5Err_t tmMHEG5SetAudioMute(tmMHEG5Mute_t muteState)
{
    INT32   i4_ret;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);

    DBG_INFO((
        "{MHEG5 Stack} %s: muteState = %d\r\n",
        __FUNCTION__,
        muteState));
    
    _lock_audio_module();

#if 1
    i4_ret = x_scc_aud_switch(pt_a_ctrl->h_audio, (muteState != MHEG5_MUTE_ON));
    pt_a_ctrl->b_audio_mute = !(muteState != MHEG5_MUTE_ON);
#else
    if (MHEG5_MUTE_ON == muteState)
    {
        i4_ret = x_scc_aud_mute(_h_audio);
    }
    else
    {
        i4_ret = x_scc_aud_unmute(_h_audio);
    }
#endif

    _unlock_audio_module();
    
    if (SMR_OK != i4_ret)
    {
        DBG_ERROR(("{MHEG5 Stack} Mute/unmute audio failed!\r\n"));
    }
    
    return ((SMR_OK == i4_ret) ? MHEG5_OK : MHEG5_ERR_OTHER);
}

tmMHEG5Err_t tmMHEG5SetAudioSpeed(tmMHEG5AudioSpeed_t speed)
{
    INT32        i4_ret;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);

    DBG_INFO((
        "{MHEG5 Stack} %s: speed = %d\r\n",
        __FUNCTION__,
        speed));
    
    _lock_audio_module();
    
    i4_ret = SMR_INV_ARG;
    
    /* played from memory */
    if (pt_a_ctrl->e_a_play_type == PLAYED_FROM_MEMORY)
    {
        switch (speed)
        {
            case MHEG5_AUDIO_SPEED_PAUSE:
                _aclip_lock();
                i4_ret = x_scc_aud_set_clip_ctrl(pt_a_ctrl->h_audio, SCC_AUD_CTRL_MODE_PAUSE);
                break;
                
            case MHEG5_AUDIO_SPEED_PLAY:
                _aclip_lock();
                i4_ret = x_scc_aud_set_clip_ctrl(pt_a_ctrl->h_audio, SCC_AUD_CTRL_MODE_RESUME);
                break;
                
            default:
                break;
        }
    }
    /* played from the stream */
    else if (pt_a_ctrl->e_a_play_type == PLAYED_FROM_STREAM)
    {
        switch (speed)
        {
            case MHEG5_AUDIO_SPEED_PAUSE:
            #if 1
                i4_ret = x_scc_aud_switch(pt_a_ctrl->h_audio, FALSE);
                pt_a_ctrl->b_audio_mute = TRUE;
            #else
                i4_ret = x_scc_aud_mute(_h_audio);
            #endif
                break;
                
            case MHEG5_AUDIO_SPEED_PLAY:
            #if 1
                i4_ret = x_scc_aud_switch(pt_a_ctrl->h_audio, TRUE);
                pt_a_ctrl->b_audio_mute = FALSE;
            #else
                i4_ret = x_scc_aud_unmute(_h_audio);
            #endif
                break;
                
            default:
                break;
        }
    }

    _unlock_audio_module();
    
    if (SMR_OK == i4_ret)
    {
        return MHEG5_OK;
    }
    else if (SMR_INV_ARG == i4_ret)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }
    else
    {
        return MHEG5_ERR_OTHER;
    }
}

INT32 sct_set_audio_desc_stream(ptmMHEG5DvbLocator_t pDvbLocator)
{
    INT32                       i4_ret;
    MHEG5_STREAM_DESC_T         t_strm_desc;
    MPEG_2_PID_T                t_pid;
    SCDB_REC_T                  t_scdb_rec;
    volatile STREAM_PID_INFO_T  t_sinfo;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    BOOL                    b_pid_resolved = FALSE;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_a_ctrl);

    DBG_INFO((
        "{MHEG5 Stack} %s: pDvbLocator = %d\r\n",
        __FUNCTION__,
        (UINT32) pDvbLocator));
    
    if (pDvbLocator == NULL)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }

    _lock_audio_module();

    t_strm_desc.e_strm_type = ST_AUDIO;
    t_strm_desc.ui2_on_id   = pDvbLocator->original_network_id;
    t_strm_desc.ui2_ts_id   = pDvbLocator->transport_stream_id;
    t_strm_desc.ui2_svc_id  = pDvbLocator->service_id;
    t_strm_desc.i4_comp_tag = pDvbLocator->ComponentTag;

    if((pDvbLocator->ComponentTag == MHEG5_DEF_COMP_TAG) 
        && (pDvbLocator->service_id == pt_gnrl_info->ui2_launch_svc_id))
    {
    	i4_ret = x_svctx_get_using_scdb_rec(
    		pt_gnrl_info->h_svctx,
    		ST_AUDIO,
    		&t_scdb_rec);

    	if (i4_ret != SVCTXR_OK)
    	{
    		DBG_ERROR(("{MHEG5 Stack} x_svctx_get_using_scdb_rec() failed. (%d) Line: %d\r\n", i4_ret, __LINE__));
    		x_memcpy(t_strm_desc.s_lang, pt_a_ctrl->t_def_lang, sizeof(ISO_639_LANG_T));
    	}
    	else
    	{
    		x_memcpy(t_strm_desc.s_lang, t_scdb_rec.u.t_audio_mpeg.s_lang, sizeof(ISO_639_LANG_T));
    	}
        t_strm_desc.e_aud_type = AUD_TYPE_VISUAL_IMPAIRED;

    	x_memcpy(t_strm_desc.s_gui_lang, pt_a_ctrl->t_gui_lang, sizeof(ISO_639_LANG_T));

        {                    
            _unlock_audio_module();
                
            /* query the PID */
            i4_ret = x_mheg5_resolve(
                pt_gnrl_info->ui4_eng_code,
                &t_strm_desc,
                _mheg5_resolve_nfy,
                (VOID *) &t_sinfo,
                (STREAM_PID_INFO_T*)&t_sinfo);

            if (i4_ret != MHEG5R_OK)
            {
                DBG_ERROR((
                    "{MHEG5 Stack} x_mheg5_resolve() = %d failed (on = %d, ts = %d, src = %d, tag = %d).\r\n", 
                    i4_ret,
                    pDvbLocator->original_network_id,
                    pDvbLocator->transport_stream_id,
                    pDvbLocator->service_id,
                    pDvbLocator->ComponentTag));
            }        
            else
            {
                b_pid_resolved = TRUE;
            }       
        
            if (b_pid_resolved == TRUE)
            {
                t_pid        = t_sinfo.t_pid;
                pt_a_ctrl->t_ad_aud_enc   = (t_sinfo.ui1_enc_type == 0) ? (AUD_ENC_MPEG_2) : (t_sinfo.ui1_enc_type);
            }
            else
            {
                t_pid                       =   MPEG_2_NULL_PID;
                pt_a_ctrl->t_ad_aud_enc      =   AUD_ENC_UNKNOWN;
                DBG_INFO(("{MHEG5}sct_set_audio_desc_stream.qurey pid failed!\n"));
            }            
        }
        
        _lock_audio_module();
        
        if ((MHEG5_AUDIO_MHEG == pt_a_ctrl->e_a_ctrl) && (b_pid_resolved == TRUE))
        {     /* play */
            SVCTX_STREAM_INFO_T    t_strm_info;
            
            t_strm_info.e_strm_type              = ST_AUDIO;
            t_strm_info.t_mpeg_2_pid             = t_pid;
            t_strm_info.u.t_aud_stream.e_aud_enc = pt_a_ctrl->t_ad_aud_enc;            
            
            i4_ret = x_svctx_select_mpeg_ad_stream_by_pid(
                 pt_gnrl_info->h_svctx,
                 &t_strm_info,
                 _select_stream_by_pid_nfy,
                 NULL);

            _set_av_sync_data(FALSE);

            if (i4_ret != SVCTXR_OK)
            {
                DBG_ERROR(("{MHEG5 Stack} x_svctx_select_mpeg_stream_by_pid() = %d failed.\n", i4_ret));
                _unlock_audio_module();
                return MHEG5_ERR_OTHER;
            }
            else
            {
                _set_stream_select_block(&t_pid, FALSE);
            }

            _lock_a_v_sync(AUDIO_SYNC);
        }
        
        pt_a_ctrl->t_ad_as_pid    = t_pid;   
        
    }
    else
    {        
        DBG_INFO(("{MHEG5}sct_set_audio_desc_stream.ComponentTag != -1, not default audio.\n"));
    }
    _unlock_audio_module();
    
    return MHEG5_OK;
}    

tmMHEG5Err_t tmMHEG5SetAudioStream(ptmMHEG5DvbLocator_t pDvbLocator)
{
    INT32                       i4_ret;
    MHEG5_STREAM_DESC_T         t_strm_desc;
    MPEG_2_PID_T                t_pid;
    SCDB_REC_T                  t_scdb_rec;
    volatile STREAM_PID_INFO_T  t_sinfo;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    BOOL                    b_pid_resolved = FALSE;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_a_ctrl);

    DBG_INFO((
        "{MHEG5 Stack} %s: pDvbLocator = %d\r\n",
        __FUNCTION__,
        (UINT32) pDvbLocator));
    
    if (pDvbLocator == NULL)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }

    _lock_audio_module();

    if (PLAYED_FROM_MEMORY == pt_a_ctrl->e_a_play_type)
    {
        _aclip_lock();
        DBG_INFO(("{MHEG5 Stack} %s: Acquired INITIAL lock...\r\n", __FUNCTION__));
        
        if (MHEG5R_INTERNAL_ERROR == _aclip_stop(TRUE, FALSE))
        {
            DBG_ERROR(("{MHEG5 Stack} Close audio clip component failed.\r\n"));
            _aclip_unlock();
            _unlock_audio_module(); 
            return MHEG5_ERR_OTHER; 
        }

        _aclip_unlock();
    }
    
    _save_current_stream(ST_AUDIO);

    t_strm_desc.e_strm_type = ST_AUDIO;
    t_strm_desc.ui2_on_id   = pDvbLocator->original_network_id;
    t_strm_desc.ui2_ts_id   = pDvbLocator->transport_stream_id;
    t_strm_desc.ui2_svc_id  = pDvbLocator->service_id;
    t_strm_desc.i4_comp_tag = pDvbLocator->ComponentTag;

    pt_a_ctrl->b_default_audio_lang   = (pDvbLocator->ComponentTag == MHEG5_DEF_COMP_TAG && pDvbLocator->service_id == pt_gnrl_info->ui2_launch_svc_id)? TRUE: FALSE;

#if 0
	x_memcpy(t_strm_desc.s_lang, _t_def_lang, sizeof(ISO_639_LANG_T));
#else
	i4_ret = x_svctx_get_using_scdb_rec(
		pt_gnrl_info->h_svctx,
		ST_AUDIO,
		&t_scdb_rec);

	if (i4_ret != SVCTXR_OK)
	{
		DBG_ERROR(("{MHEG5 Stack} x_svctx_get_using_scdb_rec() failed. (%d) Line: %d\r\n", i4_ret, __LINE__));
		x_memcpy(t_strm_desc.s_lang, pt_a_ctrl->t_def_lang, sizeof(ISO_639_LANG_T));
		t_strm_desc.e_aud_type = AUD_TYPE_UNKNOWN;
	}
	else
	{
		x_memcpy(t_strm_desc.s_lang, t_scdb_rec.u.t_audio_mpeg.s_lang, sizeof(ISO_639_LANG_T));
		t_strm_desc.e_aud_type = t_scdb_rec.u.t_audio_mpeg.e_type;
	}

	x_memcpy(t_strm_desc.s_gui_lang, pt_a_ctrl->t_gui_lang, sizeof(ISO_639_LANG_T));
#endif

    if (pDvbLocator->original_network_id == pt_gnrl_info->ui2_launch_on_id  &&
        pDvbLocator->transport_stream_id == pt_gnrl_info->ui2_launch_ts_id  &&
        pDvbLocator->service_id          == pt_gnrl_info->ui2_launch_svc_id &&
        pDvbLocator->ComponentTag        == MHEG5_DEF_COMP_TAG              &&
        pt_a_ctrl->b_a_user              == TRUE)
    {
        t_pid = pt_a_ctrl->t_a_scdb_rec.u.t_audio_mpeg.ui2_pid;
        pt_a_ctrl->t_aud_enc = (pt_a_ctrl->t_a_scdb_rec.u.t_audio_mpeg.e_enc == 0) ? (AUD_ENC_MPEG_2) : (pt_a_ctrl->t_a_scdb_rec.u.t_audio_mpeg.e_enc);
        pt_a_ctrl->ui2_a_svc_id = pDvbLocator->service_id;
        _unlock_audio_module();
		
		/* Check for NDT, since NDT could keep previous service's AV, we have to ensure it's current service's audio component */
		b_pid_resolved = sil_is_aud_pid_belong_to_cur_svc(
							pDvbLocator->original_network_id,
							pDvbLocator->transport_stream_id,
							pDvbLocator->service_id,
							t_pid);
    }

	if (!b_pid_resolved)
    {        
        t_sinfo.b_waiting = TRUE;
        
        _unlock_audio_module();

        pt_a_ctrl->ui2_a_svc_id = pDvbLocator->service_id;
            
        /* query the PID */
        i4_ret = x_mheg5_resolve(
            pt_gnrl_info->ui4_eng_code,
            &t_strm_desc,
            _mheg5_resolve_nfy,
            (VOID *) &t_sinfo,
            (STREAM_PID_INFO_T*)&t_sinfo);

        if (i4_ret != MHEG5R_OK &&
            i4_ret != MHEG5R_ASYNC_RESPONSE)
        {
            DBG_INFO((
                "{MHEG5 Stack} x_mheg5_resolve() = %d failed (on = %d, ts = %d, src = %d, tag = %d).\r\n", 
                i4_ret,
                pDvbLocator->original_network_id,
                pDvbLocator->transport_stream_id,
                pDvbLocator->service_id,
                pDvbLocator->ComponentTag));

            /* For NDT support */
            _stop_current_stream(ST_AUDIO);
        }
        /* if the query is async */
        else if (i4_ret == MHEG5R_ASYNC_RESPONSE)
        {
            /* spin lock until the PID is got */
            while (t_sinfo.b_waiting)
            {
                x_thread_delay(10);
            }

            /* if the PID is not found */
            if (t_sinfo.b_found == FALSE)
            {
                /* For NDT support */
                _stop_current_stream(ST_AUDIO);
            }
            else
            {
                b_pid_resolved = TRUE;
            }
        }
        else
        {
            b_pid_resolved = TRUE;
        }
        
        
        if (b_pid_resolved == TRUE)
        {
            t_pid        = t_sinfo.t_pid;
            pt_a_ctrl->t_a_pcr_pid = t_sinfo.t_pcr_pid;
            pt_a_ctrl->t_aud_enc   = (t_sinfo.ui1_enc_type == 0) ? (AUD_ENC_MPEG_2) : (t_sinfo.ui1_enc_type);
        }
        else
        {
            t_pid                  =  MPEG_2_NULL_PID;
            pt_a_ctrl->t_a_pcr_pid =  MPEG_2_NULL_PID;
            pt_a_ctrl->t_aud_enc   = AUD_ENC_UNKNOWN;
        }
        
    }
    
    _lock_audio_module();
    
    pt_a_ctrl->e_a_play_type = PLAYED_FROM_STREAM;

    if ((MHEG5_AUDIO_MHEG == pt_a_ctrl->e_a_ctrl) && (b_pid_resolved == TRUE))
    {
    #if 0
        i4_ret = x_svctx_get_using_scdb_rec(
            _h_svctx,
            ST_AUDIO,
            &t_scdb_rec);
    #else
        i4_ret = SVCTXR_OK;
    #endif

        if ((i4_ret != SVCTXR_OK) ||
            (t_pid != t_scdb_rec.u.t_audio_mpeg.ui2_pid))
        {
            SIZE_T                      z_status_size = sizeof(ASH_AUDIO_DECODE_STATUS_T);
            ASH_AUDIO_DECODE_STATUS_T   t_status = ASH_AUDIO_DECODE_UNKNOWN;
            
            _stop_current_stream(ST_AUDIO);

            tmMHEG5NotifyAudioStopped((BOOLEAN)FALSE);

            /* play */
            
            #if 1
            {
            SVCTX_STREAM_INFO_T    t_strm_info;
            
            t_strm_info.e_strm_type              = ST_AUDIO;
            t_strm_info.t_mpeg_2_pid             = t_pid;
            t_strm_info.u.t_aud_stream.e_aud_enc = pt_a_ctrl->t_aud_enc;
            
            
            i4_ret = x_svctx_select_mpeg_stream_by_pid_ex(
                 pt_gnrl_info->h_svctx,
                 &t_strm_info,
                 _select_stream_by_pid_nfy,
                 NULL);
            }
            #else
            i4_ret = x_svctx_select_mpeg_stream_by_pid(pt_gnrl_info->h_svctx, ST_AUDIO, t_pid, _select_stream_by_pid_nfy, NULL);
            #endif

            _set_av_sync_data(FALSE);

            if (i4_ret != SVCTXR_OK)
            {
                DBG_ERROR(("{MHEG5 Stack} x_svctx_select_mpeg_stream_by_pid() = %d failed.\n", i4_ret));
                _unlock_audio_module();
                return MHEG5_ERR_OTHER;
            }
            else
            {
                _set_stream_select_block(&t_pid, FALSE);
            }

            _lock_a_v_sync(AUDIO_SYNC);

            tmMHEG5NotifyAudioStarted((BOOLEAN)FALSE);

            i4_ret = x_svctx_get(
                pt_gnrl_info->h_svctx,
                SVCTX_COMMON_GET_AUDIO_DECODE_STATUS,
                (VOID *)&t_status,
                &z_status_size);

            if (i4_ret                    == SVCTXR_OK                         &&
                t_status                  == ASH_AUDIO_DECODE_NORMAL           &&
                z_status_size             == sizeof(ASH_AUDIO_DECODE_STATUS_T) &&
                pDvbLocator->ComponentTag == MHEG5_DEF_COMP_TAG                &&
                !pt_a_ctrl->b_a_sync_timeout)
            {
                _save_current_stream(ST_AUDIO);
            }
        }
    }
    
    pt_a_ctrl->t_a_locator = *pDvbLocator;
    pt_a_ctrl->b_as_set    = TRUE;
    pt_a_ctrl->t_as_pid    = t_pid;
    pt_a_ctrl->b_a_pid_ok  = b_pid_resolved;

    pt_a_ctrl->i4_a_comp_tag = pDvbLocator->ComponentTag;
    
    _unlock_audio_module();
    sct_set_audio_desc_stream(pDvbLocator);
    
    return MHEG5_OK;
}

tmMHEG5Err_t tmMHEG5SetAudioVolumeAdjustment(S32BIT volumeAdjust)
{
    INT32   i4_ret;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);

    DBG_INFO((
        "{MHEG5 Stack} %s: volumeAdjust = %d\r\n",
        __FUNCTION__,
        volumeAdjust));
    
    _lock_audio_module();

#if 1
    i4_ret = x_scc_aud_switch(pt_a_ctrl->h_audio, (volumeAdjust >= 0));
    pt_a_ctrl->b_audio_mute = !(volumeAdjust >= 0);
#else
    if (volumeAdjust < 0)
    {
        i4_ret = x_scc_aud_mute(_h_audio);
    }
    else
    {
        i4_ret = x_scc_aud_unmute(_h_audio);
    }
#endif

    _unlock_audio_module();
    
    if (SMR_OK != i4_ret)
    {
        DBG_ERROR(("{MHEG5 Stack} Adjusting audio volume failed!\r\n"));
    }
    
    return ((SMR_OK == i4_ret) ? MHEG5_OK : MHEG5_ERR_OTHER);
}

MHEG5_SBTL_STATE_T sct_get_subtitle_visibility()
{
    SCT_VIDEO_CTRL_CTX_T*       pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*          pt_dis_cfg = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);

    if (IS_MHEG5_RUNNING() &&
        !pt_v_ctrl->b_video_frozen &&
        pt_v_ctrl->e_sbtl_eng_state == MHEG5_DVB_SUBTITLE_SHOW)
    {
        return MHEG5_SBTL_SHOW;
    }
    else
    {
        return MHEG5_SBTL_HIDE;
    }
}

MHEG5_SBTL_STATE_T sct_get_sbtl_visibility()
{
    SCT_VIDEO_CTRL_CTX_T*       pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*          pt_dis_cfg = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);

    if (IS_MHEG5_RUNNING() &&
        pt_v_ctrl->t_sbtl_cfg.b_on &&
        !pt_v_ctrl->b_video_frozen &&
        IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video) &&
        pt_v_ctrl->e_sbtl_eng_state == MHEG5_DVB_SUBTITLE_SHOW)
    {
        return MHEG5_SBTL_SHOW;
    }
    else
    {
        return MHEG5_SBTL_HIDE;
    }
}


tmMHEG5Err_t tmMHEG5SetDvbSubtitleVisibility(tmMHEG5DvbSubtitleShowState_t showState)
{
    DBG_INFO((
        "{MHEG5 Stack} %s: showState = %d\r\n",
        __FUNCTION__,
        showState));
#if 1
    SCT_VIDEO_CTRL_CTX_T*       pt_video_ctrl     = NULL;  
    MHEG5_SBTL_STATE_T          e_sbtl_show_state;

    SCT_GET_VIDEO_CTRL_CTX(pt_video_ctrl);
        
    MHEG5_ASSERT(NULL != pt_video_ctrl);

    _lock_video_module();
    
    pt_video_ctrl->e_sbtl_eng_state = showState;
    e_sbtl_show_state = sct_get_sbtl_visibility();
    
    _unlock_video_module();

    _sct_notify_sbtl_visibility(e_sbtl_show_state);
#ifdef MHEG5_IC_SUPPORT
    mheg5_ics_handle_sbtl_visibility();
#endif
    return MHEG5_OK;

#else

    return MHEG5_OK;

#endif

}

tmMHEG5Err_t tmMHEG5SetSceneAspectRatio(tmMHEG5SceneAspectRatio_t sceneAspectRatio)
{
    CRIT_STATE_T    t_cs;
    MHEG5_MSG_T     t_msg;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;
    BOOL            b_adjust_video    = FALSE;
    BOOL            b_vid_info_update = FALSE;
    
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);

    DBG_INFO((
        "{MHEG5 Stack} %s: sceneAspectRatio = %d\r\n",
        __FUNCTION__,
        sceneAspectRatio));

    t_cs = x_crit_start();
    
    if (pt_dis_cfg->e_scene_ar != sceneAspectRatio)
    {
        pt_dis_cfg->e_scene_ar = sceneAspectRatio;
        b_adjust_video = TRUE;
        b_vid_info_update = TRUE;
    }

    x_crit_end(t_cs);

    if (b_adjust_video)
    {
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_ADJUST_VIDEO;
        t_msg.pv_nfy_tag   = (VOID *)ADJUST_VIDEO_FLAG_NONE;

        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }

    if (b_vid_info_update)
    {
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_NOTIFY_VID_INFO_UPDATE;
        t_msg.pv_nfy_tag   = (VOID *)ADJUST_VIDEO_FLAG_NONE;

        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }
    
    return MHEG5_OK;
}

tmMHEG5Err_t tmMHEG5SetVideoControl(tmMHEG5VideoControl_t control)
{
    INT32           i4_ret;
    BOOL            b_frozen;
    HANDLE_T        h_svctx;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);   

#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    BOOL                    b_set_stream       = FALSE;
    SCC_VID_SCREEN_MODE_T   e_screen_mode      = sct_get_screen_auto_mode_enum();
    MHEG5_VIDEO_MODE_T      e_mheg5_video_mode = MHEG5_VIDEO_MODE_AUTO;
#endif
    
    DBG_INFO((
        "{MHEG5 Stack}DBG_INFO %s: control = %d\r\n",
        __FUNCTION__,
        control));

    if ((MHEG5_VIDEO_NONE != control) &&
        (MHEG5_VIDEO_MHEG != control) &&
        (MHEG5_VIDEO_USER != control))
    {
        DBG_ERROR(("{MHEG5 Stack} %s: Invalid parameter.\r\n", __FUNCTION__));
        return MHEG5_ERR_BAD_PARAMETER;
    }

    h_svctx = pt_gnrl_info->h_svctx;

    /* MHEG engine sends commands when it's in pause mode */
    if (_in_mheg_context() && !pt_gnrl_info->b_mheg5_running)
    {
        if (control == MHEG5_VIDEO_NONE)
        {
            pt_v_ctrl->e_resume_v_ctrl = control;
        }
        /* should have no action [DTV00134049] */
        else
        {
            STREAM_COMP_ID_T    t_comp_id;
            SCDB_REC_T          t_scdb;
            BOOL                b_with_vs = FALSE;
            
            b_with_vs = (x_svctx_get_using_strm_data(h_svctx, ST_VIDEO, &t_comp_id, &t_scdb) == SVCTXR_OK);
            
            if (pt_v_ctrl->e_v_ctrl == MHEG5_VIDEO_USER && b_with_vs == TRUE)
            {
                pt_v_ctrl->e_resume_v_ctrl = control;

                return MHEG5_ERR_OTHER;
            }
        }
    }
    
    i4_ret = MHEG5R_INV_STATE;
    
    _lock_video_module();
        
    if (PLAYED_FROM_STREAM == pt_v_ctrl->e_v_play_type)
    {
        if (pt_v_ctrl->e_v_ctrl != control)
        {
            i4_ret = SVCTXR_OK;
            
            switch (control)
            {
                case MHEG5_VIDEO_NONE:
                {
                    STREAM_COMP_ID_T    t_comp_id;
                    SCDB_REC_T          t_scdb;
                    BOOL                b_with_vs = FALSE;
                    pt_v_ctrl->t_vs_pid = 0;
                    _save_current_stream(ST_VIDEO);

                    b_with_vs = (x_svctx_get_using_strm_data(h_svctx, ST_VIDEO, &t_comp_id, &t_scdb) == SVCTXR_OK);

                    if (b_with_vs)
                    {
                        i4_ret = x_svctx_sync_stop_stream(h_svctx, ST_VIDEO, 0);
                        _sct_notify_sbtl_visibility(MHEG5_SBTL_HIDE);
                    }

                    if (pt_v_ctrl->e_v_ctrl == MHEG5_VIDEO_MHEG)
                    {
                        /* [DTV00316506] engine wants to be notified once stream stop event happened */
                        tmMHEG5NotifyVideoStopped();

                        /* [DTV00327231] when engine sets control to NONE, the saved stream states must be reset */
                        if(_in_mheg_context())
                        {
                            pt_v_ctrl->b_vs_set   = FALSE;
                            pt_v_ctrl->b_v_pid_ok = FALSE;
                        }
                    } 
                    
                    break;
                }
                    
                case MHEG5_VIDEO_MHEG:
                    if (MHEG5_VIDEO_NONE == pt_v_ctrl->e_v_ctrl)
                    {
                        if ((pt_v_ctrl->b_vs_set) && (pt_v_ctrl->b_v_pid_ok))
                        {
                            SVCTX_STREAM_INFO_T    t_strm_info;

                            if ((SVCTXR_OK == x_svctx_is_frozen(h_svctx, &b_frozen)) && b_frozen)
                            {
                                i4_ret = x_svctx_unfreeze(h_svctx);
                            }
                        
                            t_strm_info.e_strm_type              = ST_VIDEO;
                            t_strm_info.t_mpeg_2_pid             = pt_v_ctrl->t_vs_pid;
                            t_strm_info.u.t_vid_stream.e_vid_enc = pt_v_ctrl->t_vid_enc;

                            i4_ret = x_svctx_select_mpeg_stream_by_pid_ex(
                                h_svctx,
                                &t_strm_info,
                                _select_stream_by_pid_nfy,
                                NULL);

                        #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
                            b_set_stream       = TRUE;
                            e_screen_mode      = sct_get_screen_auto_mode_enum();
                            e_mheg5_video_mode = MHEG5_VIDEO_MODE_AUTO;
                        #endif

                            _set_av_sync_data(FALSE);

                            _lock_a_v_sync(VIDEO_SYNC);

                            tmMHEG5NotifyVideoStarted();
                        }
                    }
                    else
                    {
                        _save_current_stream(ST_VIDEO);
                        
                        if ((!pt_v_ctrl->b_v_user)   ||
                            (!pt_v_ctrl->b_vs_set)   ||
                            (!pt_v_ctrl->b_v_pid_ok) ||
                            (pt_v_ctrl->t_vs_pid != pt_v_ctrl->t_v_scdb_rec.u.t_video_mpeg.ui2_pid))
                        {
                            i4_ret = SVCTXR_OK;

                            if (pt_v_ctrl->t_vs_pid != 0 && pt_v_ctrl->t_vs_pid != MPEG_2_NULL_PID)
                            {
                                i4_ret = _stop_current_stream(ST_VIDEO);
                                
                                if ((SVCTXR_OK == i4_ret) && (pt_v_ctrl->b_vs_set) && (pt_v_ctrl->b_v_pid_ok))
                                {
                                    SIZE_T                  z_status_size = sizeof(VSH_DIG_DECODE_STATUS_T);
                                    SVCTX_STREAM_INFO_T     t_strm_info;
                                    VSH_DIG_DECODE_STATUS_T t_status = VSH_DIG_DECODE_UNKNOWN;
                                    
                                    if ((SVCTXR_OK == x_svctx_is_frozen(h_svctx, &b_frozen)) && b_frozen)
                                    {
                                        i4_ret = x_svctx_unfreeze(h_svctx);
                                    }

                                    t_strm_info.e_strm_type              = ST_VIDEO;
                                    t_strm_info.t_mpeg_2_pid             = pt_v_ctrl->t_vs_pid;
                                    t_strm_info.u.t_vid_stream.e_vid_enc = pt_v_ctrl->t_vid_enc;
                                    
                                    i4_ret = x_svctx_select_mpeg_stream_by_pid_ex(
                                        h_svctx,
                                        &t_strm_info,
                                        _select_stream_by_pid_nfy,
                                        NULL);

                                #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
                                    b_set_stream       = TRUE;
                                    e_screen_mode      = sct_get_screen_auto_mode_enum();
                                    e_mheg5_video_mode = MHEG5_VIDEO_MODE_AUTO;
                                #endif

                                    pt_v_ctrl->e_v_ctrl = control;

                                    _set_av_sync_data(FALSE);

                                    _lock_a_v_sync(VIDEO_SYNC);

                                    tmMHEG5NotifyVideoStarted();

                                    i4_ret = x_svctx_get(
                                        h_svctx,
                                        SVCTX_COMMON_GET_VIDEO_DECODE_STATUS,
                                        (VOID *)&t_status,
                                        &z_status_size);

                                    if (i4_ret         == SVCTXR_OK                       &&
                                        t_status       == VSH_DIG_DECODE_NORMAL           &&
                                        z_status_size  == sizeof(VSH_DIG_DECODE_STATUS_T) &&
                                        pt_v_ctrl->i4_v_comp_tag == MHEG5_DEF_COMP_TAG              &&
                                        !pt_v_ctrl->b_v_sync_timeout)
                                    {
                                        _save_current_stream(ST_VIDEO);
                                    }

                                }
                            }
                            else if ((pt_v_ctrl->b_vs_set) && (!pt_v_ctrl->b_v_pid_ok))
                            {
                                _stop_current_stream(ST_VIDEO);
                                i4_ret = SVCTXR_OK;
                            }
                        }                        
                        else  /* [DTV00316506] engine wants to be notified once stream playing event happened */
                        {
                            tmMHEG5NotifyVideoStarted();
                        }
                    }
                    
                    _sct_notify_sbtl_visibility(sct_get_sbtl_visibility());
                    break;

                case MHEG5_VIDEO_USER:
                    if (MHEG5_VIDEO_NONE == pt_v_ctrl->e_v_ctrl)
                    {
                        if (pt_v_ctrl->b_v_user)
                        {
                            _set_stream_select_block(NULL, FALSE);
                            
                            if ((SVCTXR_OK == x_svctx_is_frozen(h_svctx, &b_frozen)) && b_frozen)
                            {
                                i4_ret = x_svctx_unfreeze(h_svctx);
                            }
                            
                            i4_ret = x_svctx_select_stream(
                                h_svctx,
                                &pt_v_ctrl->t_v_comp_id,
                                NULL,
                                NULL);

                        #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
                            b_set_stream       = TRUE;
                            e_screen_mode      = pt_dis_cfg->e_vs_ori_scr_mode;
                            e_mheg5_video_mode = pt_dis_cfg->e_mh5_ori_vid_mode;
                        #endif

                            _set_av_sync_data(TRUE);

                            /* [DTV00130210] The video and the audio is not synchronous in MHEG5 page */
                            pt_v_ctrl->e_v_ctrl = control;
                        }
                        else
                        {
                            tmMHEG5DvbLocator_t     t_dvb_loc;

                            t_dvb_loc.original_network_id = pt_gnrl_info->ui2_launch_on_id;
                            t_dvb_loc.transport_stream_id = pt_gnrl_info->ui2_launch_ts_id;
                            t_dvb_loc.service_id          = pt_gnrl_info->ui2_launch_svc_id;
                            t_dvb_loc.ComponentTag        = -1;

                            if (sil_is_pmt_available() && tmMHEG5SetVideoStream(&t_dvb_loc) == MHEG5_OK)
                            {
                                if ((pt_v_ctrl->b_vs_set) && (pt_v_ctrl->b_v_pid_ok))
                                {
                                    SIZE_T                  z_status_size = sizeof(VSH_DIG_DECODE_STATUS_T);
                                    SVCTX_STREAM_INFO_T     t_strm_info;
                                    VSH_DIG_DECODE_STATUS_T t_status = VSH_DIG_DECODE_UNKNOWN;
                                    
                                    if ((SVCTXR_OK == x_svctx_is_frozen(h_svctx, &b_frozen)) && b_frozen)
                                    {
                                        i4_ret = x_svctx_unfreeze(h_svctx);
                                    }

                                    t_strm_info.e_strm_type              = ST_VIDEO;
                                    t_strm_info.t_mpeg_2_pid             = pt_v_ctrl->t_vs_pid;
                                    t_strm_info.u.t_vid_stream.e_vid_enc = pt_v_ctrl->t_vid_enc;
                                    
                                    i4_ret = x_svctx_select_mpeg_stream_by_pid_ex(
                                        h_svctx,
                                        &t_strm_info,
                                        _select_stream_by_pid_nfy,
                                        NULL);

                                #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
                                    b_set_stream       = TRUE;
                                    e_screen_mode      = sct_get_screen_auto_mode_enum();
                                    e_mheg5_video_mode = MHEG5_VIDEO_MODE_AUTO;
                                #endif

                                    pt_v_ctrl->e_v_ctrl = control;
                                
                                    _set_av_sync_data(FALSE);

                                    _lock_a_v_sync(VIDEO_SYNC);

                                    i4_ret = x_svctx_get(
                                        h_svctx,
                                        SVCTX_COMMON_GET_VIDEO_DECODE_STATUS,
                                        (VOID *)&t_status,
                                        &z_status_size);

                                    if (i4_ret         == SVCTXR_OK                       &&
                                        t_status       == VSH_DIG_DECODE_NORMAL           &&
                                        z_status_size  == sizeof(VSH_DIG_DECODE_STATUS_T) &&
                                        pt_v_ctrl->i4_v_comp_tag == MHEG5_DEF_COMP_TAG              &&
                                        !pt_v_ctrl->b_v_sync_timeout)
                                    {
                                        _save_current_stream(ST_VIDEO);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        /* When MHEG5 is not running, the stream could be changed */
                    #ifdef MW_PIP_POP_SUPPORT
                        if (_in_mheg_context() && !pt_gnrl_info->b_mheg5_running)
                        {
                            SVCTX_SCREEN_STATUS_T e_scr_status;
                            SIZE_T z_size;

                            z_size = sizeof(SVCTX_SCREEN_STATUS_T);
                            i4_ret = x_svctx_get( h_svctx, 
                                                  SVCTX_COMMON_GET_SCREEN_STATUS,
                                                  (VOID *)&e_scr_status,
                                                  (VOID *)&z_size);
                            if ((i4_ret == SVCTXR_OK) &&
                                (e_scr_status == SVCTX_SCREEN_STATUS_MULTIPLE_FULL ||
                                 e_scr_status == SVCTX_SCREEN_STATUS_MULTIPLE_PART))
                            {
                                /* PIP/POP mode */
                                break;
                            }
                        }
                    #endif

                        _save_current_stream(ST_VIDEO);

                        if ((!pt_v_ctrl->b_v_user)   ||
                            (!pt_v_ctrl->b_vs_set)   ||
                            (!pt_v_ctrl->b_v_pid_ok) ||
                            (pt_v_ctrl->t_vs_pid != pt_v_ctrl->t_v_scdb_rec.u.t_video_mpeg.ui2_pid))
                        {
                            i4_ret = _stop_current_stream(ST_VIDEO);

                            tmMHEG5NotifyVideoStopped();
                            
                            if ((SVCTXR_OK == i4_ret) && (pt_v_ctrl->b_v_user))
                            {
                                _set_stream_select_block(NULL, FALSE);
                                
                                if ((SVCTXR_OK == x_svctx_is_frozen(h_svctx, &b_frozen)) && b_frozen)
                                {
                                    i4_ret = x_svctx_unfreeze(h_svctx);
                                }
                            
                                i4_ret = x_svctx_select_stream(
                                    h_svctx,
                                    &pt_v_ctrl->t_v_comp_id,
                                    NULL,
                                    NULL);

                            #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
                                b_set_stream       = TRUE;
                                e_screen_mode      = pt_dis_cfg->e_vs_ori_scr_mode;
                                e_mheg5_video_mode = pt_dis_cfg->e_mh5_ori_vid_mode;
                            #endif

                                _set_av_sync_data(TRUE);

                                /* [DTV00130210] The video and the audio is not synchronous in MHEG5 page */
                                pt_v_ctrl->e_v_ctrl = control;
                            }
                        }
                        else if (pt_v_ctrl->b_v_pid_updated)
                        {
                            _set_stream_select_block(NULL, FALSE);
                            
                            if ((SVCTXR_OK == x_svctx_is_frozen(h_svctx, &b_frozen)) && b_frozen)
                            {
                                i4_ret = x_svctx_unfreeze(h_svctx);
                            }
                            
                            i4_ret = x_svctx_select_stream(
                                h_svctx,
                                &pt_v_ctrl->t_v_comp_id,
                                NULL,
                                NULL);
                        
                        #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
                            b_set_stream       = TRUE;
                            e_screen_mode      = pt_dis_cfg->e_vs_ori_scr_mode;
                            e_mheg5_video_mode = pt_dis_cfg->e_mh5_ori_vid_mode;
                        #endif
                        
                            _set_av_sync_data(TRUE);
                        
                            /* [DTV00130210] The video and the audio is not synchronous in MHEG5 page */
                            pt_v_ctrl->e_v_ctrl = control;
                            pt_v_ctrl->b_v_pid_updated = FALSE;
                        }
                    }
                    
                    _sct_notify_sbtl_visibility(sct_get_sbtl_visibility());
                    break;

                default:
                    i4_ret = MHEG5R_INTERNAL_ERROR;
                    break;
            }

            if (SVCTXR_OK == i4_ret)
            {
                pt_v_ctrl->e_v_ctrl = control;
                i4_ret = MHEG5R_OK;
            }
        }    
        else
        {
            i4_ret = MHEG5R_OK;
        }
    }
    
    _unlock_video_module();
    
    if (MHEG5R_OK == i4_ret)
    {
        MHEG5_MSG_T t_msg;

        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_ADJUST_VIDEO;
        t_msg.pv_nfy_tag   = (VOID *)ADJUST_VIDEO_FLAG_NONE;

    #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
        if (b_set_stream)
        {
            pt_v_ctrl->b_v_stream_set = TRUE;
            pt_dis_cfg->e_screen_mode  = e_screen_mode;
            pt_dis_cfg->e_vid_mode     = e_mheg5_video_mode;
        }
    #endif

        x_mheg5_send_msg(pt_gnrl_info->ui4_eng_code, &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
        
        return MHEG5_OK;
    }
    else
    {
        DBG_ERROR(("{MHEG5 Stack} %s: execution error.\r\n", __FUNCTION__));
        return MHEG5_ERR_OTHER;
    }
}

void DVP_VideoPosition( const S_VideoPosition* position )
{
    GL_RECT_T                   t_ori_rect;
    MHEG5_MSG_T                 t_msg;
    CRIT_STATE_T                t_cs;
    SCC_VID_OVER_SCAN_CLIPPER_T t_ovs;
    BOOL                        b_val;
#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    BOOL                        b_full_screen;
#endif
    MHEG5_SBTL_STATE_T          e_sbtl_show_state = MHEG5_SBTL_SHOW;
    SCT_GNRL_INFO_T*            pt_gnrl_info    = NULL;
    SCT_VIDEO_CTRL_CTX_T*       pt_v_ctrl       = NULL;
    SCT_DISPLAY_CFG_T*          pt_dis_cfg      = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);
    
    DBG_INFO((
        "{MHEG5 Stack} %s: xPosition = %d, yPosition = %d, w = %d, h = %d\r\n",
        __FUNCTION__,
        position->scaling.left,
        position->scaling.top,
        position->scaling.width,
        position->scaling.height));
    
    if ((position->scaling.left >= MHEG5_SCREEN_WIDTH) || (position->scaling.top >= MHEG5_SCREEN_HEIGHT) || (position->scaling.width == 0) || (position->scaling.height == 0))
    {
        DBG_ERROR(("{MHEG5 Stack} %s: Invalid parameters.\r\n", __FUNCTION__));
        return ;
    }
    _lock_display_module();
    t_cs = x_crit_start();
    
    t_ori_rect = pt_dis_cfg->t_rc_video;
    
    SET_RECT(&pt_dis_cfg->t_rc_video, 
        position->scaling.left, 
        position->scaling.top, 
        (position->scaling.left+(S32BIT)(position->scaling.width)), 
        (position->scaling.top+(S32BIT)(position->scaling.height)));

    if (_in_mheg_context() && !pt_gnrl_info->b_mheg5_running)
    {
        pt_dis_cfg->t_resume_rc_video = pt_dis_cfg->t_rc_video;
    }

#if 0    
    DBG_INFO((
        "{MHEG5 Stack} Video rect changed: (%d, %d) - (%d, %d)\r\n",
        _t_rc_video.i4_left,
        _t_rc_video.i4_top,
        _t_rc_video.i4_right,
        _t_rc_video.i4_bottom));
#endif

    b_val = IS_FULL_SCREEN(&pt_dis_cfg->t_rc_video);

#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    b_full_screen = b_val;
#endif

    e_sbtl_show_state = sct_get_sbtl_visibility();

    x_crit_end(t_cs);
    
    b_val = _calc_overscan_clipper(b_val, &t_ovs);
    
    t_cs = x_crit_start();
    
    pt_dis_cfg->b_adjust_overscan  = b_val;
    pt_dis_cfg->t_ovs_clip_runtime = t_ovs;

    x_crit_end(t_cs);
	_unlock_display_module();

    _sct_notify_sbtl_visibility(e_sbtl_show_state);
#ifdef MHEG5_IC_SUPPORT
    mheg5_ics_handle_sbtl_visibility();
#endif
    t_msg.ui1_module   = MHEG5_MOD_SCT;
    t_msg.ui2_msg_type = SCT_MSG_ADJUST_VIDEO;
    t_msg.pv_nfy_tag   = (VOID *)ADJUST_VIDEO_FLAG_NONE;

    if (pt_dis_cfg->t_rc_video.i4_left   != t_ori_rect.i4_left  ||
        pt_dis_cfg->t_rc_video.i4_top    != t_ori_rect.i4_top   ||
        pt_dis_cfg->t_rc_video.i4_right  != t_ori_rect.i4_right ||
        pt_dis_cfg->t_rc_video.i4_bottom != t_ori_rect.i4_bottom)
    {
        t_msg.pv_nfy_tag = (VOID *)ADJUST_VIDEO_FLAG_VIDEO_UPDATE;
    }

    if (pt_v_ctrl->b_video_frozen && !b_full_screen)
    {
        x_svctx_unfreeze(pt_gnrl_info->h_svctx);
    }
    
#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    if (b_full_screen && !pt_v_ctrl->b_v_stream_set)
    {
        pt_dis_cfg->e_screen_mode = pt_dis_cfg->e_vs_ori_scr_mode;
        pt_dis_cfg->e_vid_mode    = pt_dis_cfg->e_mh5_ori_vid_mode;
    }
#endif

    x_mheg5_send_msg(pt_gnrl_info->ui4_eng_code, &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    
    return ;
}

tmMHEG5Err_t tmMHEG5SetVideoStream(ptmMHEG5DvbLocator_t pDvbLocator)
{
    INT32                       i4_ret;
    MHEG5_STREAM_DESC_T         t_strm_desc;
    MPEG_2_PID_T                t_pid;
    SCDB_REC_T                  t_scdb_rec;
    volatile STREAM_PID_INFO_T  t_sinfo;
    BOOL                        b_video;
    BOOL                        b_frozen;
    BOOL                        b_pid_change;

#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    BOOL                        b_set_stream = FALSE;
#endif
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;
    BOOL                    b_pid_resolved = FALSE;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);  

    DBG_INFO((
        "{MHEG5 Stack} %s: pDvbLocator = %d\r\n",
        __FUNCTION__,
        (UINT32) pDvbLocator));
    
    if (pDvbLocator == NULL)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }

    _lock_video_module();

    if (PLAYED_FROM_MEMORY == pt_v_ctrl->e_v_play_type)
    {
        _iframe_lock();
        DBG_INFO(("{MHEG5 Stack} %s: Acquired INITIAL lock...\r\n", __FUNCTION__));
        
        /* stop the previously playing I-frame if any */
        if (MHEG5R_INTERNAL_ERROR == _iframe_close(TRUE))
        {
            DBG_ERROR(("{MHEG5 Stack} Close I-Frame component failed.\r\n"));
            _iframe_unlock();
            _unlock_video_module(); 
            return MHEG5_ERR_OTHER;
        }
    
        _iframe_unlock(); /* release the lock (not used within this API) */
    }
    
    _save_current_stream(ST_VIDEO);
    
    pt_v_ctrl->e_v_play_type = PLAYED_FROM_STREAM;

    t_strm_desc.e_strm_type = ST_VIDEO;
    t_strm_desc.ui2_on_id   = pDvbLocator->original_network_id;
    t_strm_desc.ui2_ts_id   = pDvbLocator->transport_stream_id;
    t_strm_desc.ui2_svc_id  = pDvbLocator->service_id;
    t_strm_desc.i4_comp_tag = pDvbLocator->ComponentTag;
    
    t_sinfo.b_waiting = TRUE;
    
    _unlock_video_module();

    pt_v_ctrl->ui2_v_svc_id = pDvbLocator->service_id;
    
    /* query the PID */
    i4_ret = x_mheg5_resolve(
        pt_gnrl_info->ui4_eng_code,
        &t_strm_desc,
        _mheg5_resolve_nfy,
        (VOID *)&t_sinfo,
        (STREAM_PID_INFO_T*)&t_sinfo);

    if (i4_ret != MHEG5R_OK &&
        i4_ret != MHEG5R_ASYNC_RESPONSE)
    {
        DBG_INFO((
            "{MHEG5 Stack} x_mheg5_resolve() = %d failed (on = %d, ts = %d, src = %d, tag = %d).\r\n", 
            i4_ret,
            pDvbLocator->original_network_id,
            pDvbLocator->transport_stream_id,
            pDvbLocator->service_id,
            pDvbLocator->ComponentTag));

        /* For NDT support */
        _stop_current_stream(ST_VIDEO);
    }
    /* if the query is async */
    else if (i4_ret == MHEG5R_ASYNC_RESPONSE)
    {
        /* spin lock until the PID is got */
        while (t_sinfo.b_waiting)
        {
            x_thread_delay(10);
        }

        /* if the PID is not found */
        if (t_sinfo.b_found == FALSE)
        {
            /* For NDT support */
            _stop_current_stream(ST_VIDEO);
        }
        else
        {
            b_pid_resolved = TRUE;
        }
    }
    else
    {
        b_pid_resolved = TRUE;
    }

    _lock_video_module();

    if (b_pid_resolved == TRUE)
    {
        t_pid                  = t_sinfo.t_pid;
        pt_v_ctrl->t_v_pcr_pid = t_sinfo.t_pcr_pid;
        pt_v_ctrl->t_vid_enc   = (t_sinfo.ui1_enc_type == 0) ? (VID_ENC_MPEG_2) : (t_sinfo.ui1_enc_type);
    }
    else
    {
        t_pid                  = MPEG_2_NULL_PID;
        pt_v_ctrl->t_v_pcr_pid = MPEG_2_NULL_PID;
        pt_v_ctrl->t_vid_enc   = VID_ENC_UNKNOWN;
    }
    
    if ((MHEG5_VIDEO_MHEG == pt_v_ctrl->e_v_ctrl) && (b_pid_resolved == TRUE))
    {
        i4_ret = x_svctx_get_using_scdb_rec(
            pt_gnrl_info->h_svctx,
            ST_VIDEO,
            &t_scdb_rec);

        if ((i4_ret != SVCTXR_OK) ||
            (t_pid != t_scdb_rec.u.t_video_mpeg.ui2_pid))
        {
            SIZE_T                  z_status_size = sizeof(VSH_DIG_DECODE_STATUS_T);
            SVCTX_STREAM_INFO_T     t_strm_info;
            VSH_DIG_DECODE_STATUS_T t_status = VSH_DIG_DECODE_UNKNOWN;

            /* stop */
            _stop_current_stream(ST_VIDEO);

            tmMHEG5NotifyVideoStopped();

            if ((SVCTXR_OK == x_svctx_is_frozen(pt_gnrl_info->h_svctx, &b_frozen)) && b_frozen)
            {
                i4_ret = x_svctx_unfreeze(pt_gnrl_info->h_svctx);
            }
                            
            /* play */
            t_strm_info.e_strm_type              = ST_VIDEO;
            t_strm_info.t_mpeg_2_pid             = t_pid;
            t_strm_info.u.t_vid_stream.e_vid_enc = pt_v_ctrl->t_vid_enc;

            i4_ret = x_svctx_select_mpeg_stream_by_pid_ex(
                pt_gnrl_info->h_svctx,
                &t_strm_info,
                _select_stream_by_pid_nfy,
                NULL);

            if (i4_ret != SVCTXR_OK)
            {
                DBG_ERROR(("{MHEG5 Stack} x_svctx_select_mpeg_stream_by_pid() = %d failed.\n", i4_ret));
                _unlock_video_module();
                return MHEG5_ERR_OTHER;
            }

            _set_av_sync_data(FALSE);

            _lock_a_v_sync(VIDEO_SYNC);

            tmMHEG5NotifyVideoStarted();

            i4_ret = x_svctx_get(
                pt_gnrl_info->h_svctx,
                SVCTX_COMMON_GET_VIDEO_DECODE_STATUS,
                (VOID *)&t_status,
                &z_status_size);

            if (i4_ret                    == SVCTXR_OK                       &&
                t_status                  == VSH_DIG_DECODE_NORMAL           &&
                z_status_size             == sizeof(VSH_DIG_DECODE_STATUS_T) &&
                pDvbLocator->ComponentTag == MHEG5_DEF_COMP_TAG              &&
                !pt_v_ctrl->b_v_sync_timeout)
            {
                _save_current_stream(ST_VIDEO);
            }

        #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
            b_set_stream      = TRUE;
        #endif
        }
    }

    b_pid_change = (pt_v_ctrl->t_vs_pid != t_pid);
    
    pt_v_ctrl->t_v_locator = *pDvbLocator;
    pt_v_ctrl->b_vs_set    = TRUE;
    pt_v_ctrl->t_vs_pid    = t_pid;
    pt_v_ctrl->b_v_pid_ok  = b_pid_resolved;

    pt_v_ctrl->i4_v_comp_tag = pDvbLocator->ComponentTag;
    
    b_video = (MHEG5_VIDEO_MHEG == pt_v_ctrl->e_v_ctrl);
    
    _unlock_video_module();

    if (b_video && b_pid_resolved)
    {
        MHEG5_MSG_T t_msg;

        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_ADJUST_VIDEO;
        t_msg.pv_nfy_tag   = (VOID *)((b_pid_change)? ADJUST_VIDEO_FLAG_PID_CHANGE: ADJUST_VIDEO_FLAG_NONE);

    #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
        if (b_set_stream)
        {
            pt_v_ctrl->b_v_stream_set = TRUE;
            pt_dis_cfg->e_screen_mode  = sct_get_screen_auto_mode_enum();
            pt_dis_cfg->e_vid_mode     = MHEG5_VIDEO_MODE_AUTO;
        }
    #endif

        x_mheg5_send_msg(pt_gnrl_info->ui4_eng_code, &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }
    
    return MHEG5_OK;
}

tmMHEG5Err_t tmMHEG5SetVideoUpdating(tmMHEG5VideoState_t displayState)
{
    INT32           i4_ret;
    tmMHEG5Err_t    t_err;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_v_ctrl);

    DBG_INFO((
        "{MHEG5 Stack} %s: displayState = %d\r\n",
        __FUNCTION__,
        displayState));
    
    _lock_video_module();

    t_err = MHEG5_ERR_BAD_PARAMETER;
    
    /* I-frame */
    if (pt_v_ctrl->e_v_play_type == PLAYED_FROM_MEMORY)
    {
        switch (displayState)
        {
            case MHEG5_VIDEO_FREEZE:
                i4_ret = x_scc_vid_set_mode(pt_v_ctrl->h_video, SCC_VID_FREEZE);
                pt_v_ctrl->b_video_frozen = TRUE;
                t_err = (i4_ret == SMR_OK)? MHEG5_OK: MHEG5_ERR_OTHER;
                break;
                
            case MHEG5_VIDEO_UNFREEZE:
                i4_ret = x_scc_vid_set_mode(pt_v_ctrl->h_video, SCC_VID_NORMAL);
                pt_v_ctrl->b_video_frozen = FALSE;
                t_err = (i4_ret == SMR_OK)? MHEG5_OK: MHEG5_ERR_OTHER;
                break;
                
            default:
                break;
        }
    }
    /* video stream */
    else if (pt_v_ctrl->e_v_play_type == PLAYED_FROM_STREAM)
    {
        MHEG5_VIDEO_UPDATE_NFY_T    t_data;
        
        switch (displayState)
        {
            case MHEG5_VIDEO_FREEZE:
                i4_ret = x_svctx_freeze(pt_gnrl_info->h_svctx, ST_MASK_VIDEO);
                pt_v_ctrl->b_video_frozen = TRUE;
                t_err = (i4_ret == SVCTXR_OK)? MHEG5_OK: MHEG5_ERR_OTHER;
                _sct_notify_sbtl_visibility(MHEG5_SBTL_HIDE);
                break;
                
            case MHEG5_VIDEO_UNFREEZE:
                i4_ret = x_svctx_unfreeze(pt_gnrl_info->h_svctx);
                pt_v_ctrl->b_video_frozen = FALSE;
                t_err = (i4_ret == SVCTXR_OK)? MHEG5_OK: MHEG5_ERR_OTHER;
                _sct_notify_sbtl_visibility(sct_get_sbtl_visibility());
                break;
                
            default:
                break;
        }

        t_data.ui4_nfy_flag = MHEG5_VIDEO_UPDATE_PARAM_FREEZE;
        t_data.b_freeze     = pt_v_ctrl->b_video_frozen;

        _sct_notify_video_update(&t_data);
    }

    _unlock_video_module();
    
    return (t_err);
}

tmMHEG5Err_t tmMHEG5SetWidescreenAlignment(tmMHEG5WidescreenAlign_t align)
{
    MHEG5_MSG_T     t_msg;
    CRIT_STATE_T    t_cs;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;
    BOOL            b_adjust_video    = FALSE;
    BOOL            b_vid_info_update = FALSE;
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);  
    
    DBG_INFO((
        "{MHEG5 Stack} %s: align = %d\r\n",
        __FUNCTION__,
        align));

    t_cs = x_crit_start();
    if (pt_dis_cfg->e_wam != align)
    {
        pt_dis_cfg->e_wam = align;
        b_adjust_video    = TRUE;
        b_vid_info_update = TRUE;
    }

    x_crit_end(t_cs);

    if (b_adjust_video)
    {            
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_ADJUST_VIDEO;
        t_msg.pv_nfy_tag   = (VOID *)ADJUST_VIDEO_FLAG_NONE;

        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }

    if (b_vid_info_update)
    {
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_NOTIFY_VID_INFO_UPDATE;
        t_msg.pv_nfy_tag   = (VOID *)ADJUST_VIDEO_FLAG_NONE;

        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }
    
    return MHEG5_OK;
}

tmMHEG5Err_t tmMHEG5DecodeIFrame(U8BIT *pVideoData, U32BIT dataLength, tmMH5VideoEncoding_t encoding)
{
    INT32                       i4_ret;
    tmMHEG5Err_t                t_err;
    VSH_REGION_INFO_T           t_region;
    UINT32                      ui4_w, ui4_h;
    CRIT_STATE_T                t_cs;
    VSH_SET_PLANE_ORDER_INFO_T  t_vdp_order;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);  
    
    DBG_INFO((
        "{MHEG5 Stack} %s: pVideoData = %d, dataLength = %d\r\n",
        __FUNCTION__,
        (UINT32) pVideoData,
        dataLength));
    
    if (pVideoData == NULL || dataLength == 0 || 
        ((encoding != MHEG5_MPEG2_VIDEO) && (encoding != MHEG5_H_264_VIDEO)))
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }

    if (pt_gnrl_info->b_force_no_action || !pt_gnrl_info->b_mheg5_running)
    {
        return MHEG5_OK;
    }

    _lock_video_module();

    DBG_INFO(("{MHEG5 Stack} %s: Before acquiring INITIAL lock.\r\n", __FUNCTION__));
    
    _iframe_lock();
    DBG_INFO(("{MHEG5 Stack} %s: Acquired INITIAL lock...\r\n", __FUNCTION__));
    
    if (MHEG5R_INTERNAL_ERROR == _iframe_close(TRUE))
    {
        DBG_ERROR(("{MHEG5 Stack} Close I-Frame component failed.\r\n"));
        _iframe_unlock();
        _unlock_video_module(); 
        return MHEG5_ERR_OTHER; 
    }
#if 0    
    if (MHEG5R_OK != _iframe_open())
    {
        DBG_ERROR(("{MHEG5 Stack} Open I-Frame component failed.\r\n"));
        _iframe_unlock();
        _unlock_video_module();
        return MHEG5_ERR_OTHER;
    }
#endif    
    _save_current_stream(ST_VIDEO);
                    
    /* close the video service */
    _stop_current_stream(ST_VIDEO);
    
    x_thread_delay(100);
    
    i4_ret = x_svctx_unfreeze(pt_gnrl_info->h_svctx);

    /* set the notify function */
    i4_ret = x_scc_vid_set_iframe_nfy(pt_v_ctrl->h_video, _iframe_nfy_fct, (VOID*) &pt_v_ctrl->t_v_dec_info);

    if (i4_ret != SMR_OK)
    {
        DBG_ERROR(("{MHEG5 Stack} x_scc_vid_set_iframe_nfy() = %d failed.\r\n", i4_ret));
        _iframe_unlock();
        _unlock_video_module();
        return MHEG5_ERR_OTHER;
    }

    pt_v_ctrl->b_iframe_enabled = TRUE;

#ifdef MHEG5_KEEP_SYS_VIDEO_MODE
    pt_v_ctrl->b_iframe_set = TRUE;
#endif
    
    /* Set IFrame source region */
    t_region.ui4_x      = 0;
    t_region.ui4_y      = 0;
    t_region.ui4_width  = VSH_REGION_MAX_WIDTH;
    t_region.ui4_height = VSH_REGION_MAX_HEIGHT;
    
    i4_ret = x_scc_vid_set_iframe_src_region(pt_v_ctrl->h_video, &t_region);
    i4_ret = x_scc_vid_set_iframe_disp_region(pt_v_ctrl->h_video, &t_region);

    /* restore the plane order */
    t_vdp_order.e_order_ctrl = VSH_PLANE_ORDER_CTRL_BOTTOM;
    
    i4_ret = x_scc_vid_set_iframe_pla_order(pt_v_ctrl->h_video, &t_vdp_order);

    if (!pt_v_ctrl->b_iframe_began)
    {
        /* set the decoder type */
        if (encoding == MHEG5_MPEG2_VIDEO)
        {
            i4_ret = x_scc_vid_set_iframe_codec(pt_v_ctrl->h_video, SCC_VID_IFRAME_CODEC_MPEG_2);
        }
        else
        {
            i4_ret = x_scc_vid_set_iframe_codec(pt_v_ctrl->h_video, SCC_VID_IFRAME_CODEC_H264);
        }
        if (i4_ret != SMR_OK)
        {
            DBG_ERROR(("{MHEG5 Stack} x_scc_vid_set_iframe_codec() = %d failed.\r\n", i4_ret));
            _iframe_unlock();
            _unlock_video_module();
            return MHEG5_ERR_OTHER;
        }
        
        /* into the play mode */
        i4_ret = x_scc_vid_set_iframe_ctrl(pt_v_ctrl->h_video, SCC_VID_IFRAME_CTRL_BEGIN);

        if (i4_ret != SMR_OK)
        {
            DBG_ERROR(("{MHEG5 Stack} x_scc_vid_set_iframe_ctrl() = %d failed.\r\n", i4_ret));
            _iframe_unlock();
            _unlock_video_module();
            return MHEG5_ERR_OTHER;
        }

        _iframe_lock();
        DBG_INFO(("{MHEG5 Stack} %s: Acquired CTRL_BEGIN lock...\r\n", __FUNCTION__));
    }
    else
    {
        pt_v_ctrl->t_v_dec_info.b_err = FALSE;
    }
    
    t_err = MHEG5_ERR_OTHER;

    pt_v_ctrl->b_iframe_began = TRUE;

    x_svctx_set(pt_gnrl_info->h_svctx, SVCTX_CH_SET_VID_I_FRAME_MODE, (VOID *)&pt_v_ctrl->b_iframe_began, sizeof(BOOL));
    
    if (!_iframe_notify_err())
    {
        pt_v_ctrl->e_v_play_type = PLAYED_FROM_MEMORY;

        /* allocate memory for this I-frame */
        pt_v_ctrl->t_iframe.z_iframe_size = (SIZE_T) dataLength;
        
        if (SMR_OK == x_scc_vid_iframe_alloc_buffer(pt_v_ctrl->h_video, &pt_v_ctrl->t_iframe))
        {
            /* setup the playing region information */
            x_memcpy(pt_v_ctrl->t_iframe.puc_iframe_buff, pVideoData, dataLength);
            
            /* set the I-frame buffer */
            i4_ret = x_scc_vid_set_iframe_buffer(pt_v_ctrl->h_video, &pt_v_ctrl->t_iframe);

            if (SMR_OK == i4_ret)
            {
                _iframe_lock();
                DBG_INFO(("{MHEG5 Stack} %s: Acquired SET_BUFFER lock...\r\n", __FUNCTION__));

                if (!_iframe_notify_err())
                {
                    /* decode */
                    i4_ret = x_scc_vid_iframe_decode(pt_v_ctrl->h_video, &pt_v_ctrl->t_iframe);

                    if (SMR_OK == i4_ret)
                    {
                        _iframe_lock();
                        DBG_INFO(("{MHEG5 Stack} x_scc_vid_iframe_decode() = %d succeeded.\n", i4_ret));

                        i4_ret = x_scc_vid_get_iframe_resolution(pt_v_ctrl->h_video, &ui4_w, &ui4_h);

                        t_cs = x_crit_start();
                        
                        if (SMR_OK == i4_ret)
                        {
                            pt_dis_cfg->ui4_real_vw = ui4_w;
                            pt_dis_cfg->ui4_real_vh = ui4_h;
                            
                        }
                        else
                        {
                            pt_dis_cfg->ui4_real_vw = (UINT32)MHEG5_SCREEN_WIDTH;
                            pt_dis_cfg->ui4_real_vh = (UINT32)MHEG5_SCREEN_HEIGHT;
                        }

                        x_crit_end(t_cs);
                        
                        t_err = MHEG5_OK;
                    }
                    else
                    {
                        DBG_ERROR(("{MHEG5 Stack} x_scc_vid_iframe_decode() = %d failed.\n", i4_ret));
                    }
                }
                else
                {
                    DBG_ERROR(("{MHEG5 Stack} I-Frame decoder notifies ERROR for x_scc_vid_set_iframe_buffer!\r\n"));
                }
            }
            else
            {
                DBG_ERROR(("{MHEG5 Stack} x_scc_vid_set_iframe_buffer() = %d failed.\n", i4_ret));
            }
        }
        else
        {
            DBG_ERROR(("{MHEG5 Stack} x_scc_vid_iframe_alloc_buffer() = %d failed.\n", i4_ret));
        }
    }
    else
    {
        DBG_ERROR(("{MHEG5 Stack} I-Frame decoder notifies ERROR for SCC_VID_IFRAME_CTRL_BEGIN!\r\n"));
    }
    
    _iframe_unlock();
    
    _unlock_video_module();
    
    if (MHEG5_OK == t_err)
    {
        MHEG5_MSG_T t_msg;

        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_ADJUST_VIDEO;
        t_msg.pv_nfy_tag   = (VOID *)ADJUST_VIDEO_FLAG_NONE;

        x_mheg5_send_msg(pt_gnrl_info->ui4_eng_code, &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }
    
    return (t_err);
}

tmMHEG5Err_t tmMHEG5VideoIFrameStop(void)
{
    tmMHEG5Err_t                t_err;
    MHEG5_VIDEO_UPDATE_NFY_T    t_nfy;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    
    DBG_INFO((
        "{MHEG5 Stack} %s:\r\n",
        __FUNCTION__));

    t_nfy.ui4_nfy_flag   = MHEG5_VIDEO_UPDATE_PARAM_IFRAME;
    t_nfy.e_iframe_state = MHEG5_IFRAME_STOP;

    if (pt_gnrl_info->b_force_no_action)
    {
        _sct_notify_video_update(&t_nfy);
        return MHEG5_OK;
    }
    
    _lock_video_module();

    _iframe_lock();

    pt_v_ctrl->b_iframe_began = FALSE;
    pt_v_ctrl->b_iframe_set   = FALSE;

    x_svctx_set(pt_gnrl_info->h_svctx, SVCTX_CH_SET_VID_I_FRAME_MODE, (VOID *)&pt_v_ctrl->b_iframe_began, sizeof(BOOL));
    
    DBG_INFO(("{MHEG5 Stack} %s: Acquired STOP lock...\r\n", __FUNCTION__));
    
    if (MHEG5R_OK == _iframe_close(TRUE))
    {
        DBG_INFO(("{MHEG5 Stack} %s: Stopping I-Frame succeeded.\r\n", __FUNCTION__));
        t_err = MHEG5_OK;
    }
    else
    {
        DBG_ERROR(("{MHEG5 Stack} %s: Stopping I-Frame failed.\r\n", __FUNCTION__));
        t_err = MHEG5_ERR_OTHER;
    }
    
    _iframe_unlock();
    
    pt_v_ctrl->e_v_play_type = PLAYED_FROM_STREAM;
    
    _unlock_video_module();

    _sct_notify_video_update(&t_nfy);
    
    return (t_err);
}

tmMHEG5Err_t tmMHEG5GetSubtitlePref(BOOLEAN *viewerPref)
{
    MHEG5_SUBTITLE_CFG_T t_cfg;
    
    x_memset(&t_cfg, 0, sizeof(MHEG5_SUBTITLE_CFG_T));
        
    if(NULL == viewerPref)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }

    if(MHEG5_OK == sct_get_sbtl_cfg_info(&t_cfg))
    {
        *viewerPref = t_cfg.b_on;
        return MHEG5_OK;
    }
    else
    {
        DBG_ERROR(("{MHEG5 Stack} %s: Getting Subtitle Info failed.\r\n", __FUNCTION__));
            
    }    
    return MHEG5_ERR_OTHER;
}

tmMHEG5Err_t tmMHEG5GetAudioDescPref(BOOLEAN *viewerPref)
{
    MHEG5_ICS_AUDIO_DESC_T  e_audio_pref = MHEG5_AUDIO_DESC_NORMAL;
    
    if(NULL == viewerPref)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }
    
    if (_pf_ad_fct != NULL)
    {
        if (MHEG5R_OK == _pf_ad_fct(_pv_ad_fct_tag, &e_audio_pref))
        {
            if((MHEG5_AUDIO_DESC_HEARING_IMPAIRED == e_audio_pref) 
            || (MHEG5_AUDIO_DESC_VISUALLY_IMPAIRED == e_audio_pref))
            {
                *viewerPref = TRUE;
                return MHEG5_OK;
            }
            else if(MHEG5_AUDIO_DESC_NORMAL == e_audio_pref)
            {
                *viewerPref = FALSE;
                return MHEG5_OK;
            }
        }
        else
        {
            DBG_ERROR(("{MHEG5 tmMHEG5GetAudioDescPref} fail!\n"));
        }
    }
    
    return MHEG5_ERR_OTHER;
}

#if 0
tmMHEG5Err_t tmMHEG5GetAudioDescPref(BOOLEAN *viewerPref)
{
	return MHEG5_ERR_OTHER;
}

tmMHEG5Err_t tmMHEG5GetSubtitlePref(BOOLEAN *viewerPref)
{
	return MHEG5_ERR_OTHER;
}
#endif


static VOID _lock_audio_module(VOID)
{
    SCT_AUDIO_CTRL_CTX_T* pt_a_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);
    
    x_sema_lock(pt_a_ctrl->h_audio_lock, X_SEMA_OPTION_WAIT);
}

static VOID _unlock_audio_module(VOID)
{
    SCT_AUDIO_CTRL_CTX_T* pt_a_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);

    x_sema_unlock(pt_a_ctrl->h_audio_lock);
}

static VOID _lock_video_module(VOID)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    x_sema_lock(pt_v_ctrl->h_video_lock, X_SEMA_OPTION_WAIT);
}

static VOID _unlock_video_module(VOID)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    x_sema_unlock(pt_v_ctrl->h_video_lock);
}


static VOID _lock_display_module(VOID)
{
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);  

    x_sema_lock(pt_dis_cfg->h_mutex, X_SEMA_OPTION_WAIT);
}

static VOID _unlock_display_module(VOID)
{
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);  

    x_sema_unlock(pt_dis_cfg->h_mutex);
}


static INT32 _aclip_close(BOOL b_sync)
{
    INT32 i4_ret;
    SCT_AUDIO_CTRL_CTX_T* pt_a_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);
    
    if (pt_a_ctrl->b_aclip_enabled)
    {
        if (SMR_OK == x_scc_aud_set_clip_ctrl(pt_a_ctrl->h_audio, SCC_AUD_CTRL_MODE_STOP))
        {
            if (b_sync)
            {
                _aclip_lock(); /* wait for the close to complete */
                DBG_INFO(("{MHEG5 Stack} %s: Acquired STOP lock...\r\n", __FUNCTION__));
            }
            i4_ret = MHEG5R_OK;
        }
        else
        {
            i4_ret = MHEG5R_INTERNAL_ERROR;
            DBG_ERROR(("{MHEG5 Stack} %s: x_scc_aud_set_clip_ctrl() failed.\n", __FUNCTION__));
        } 
    }
    else
    {
        i4_ret = MHEG5R_INV_STATE;
    }

    return (i4_ret);
}

static VOID _aclip_lock(VOID)
{
    SCT_AUDIO_CTRL_CTX_T* pt_a_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);
    
    x_sema_lock(pt_a_ctrl->t_a_dec_info.h_lock, X_SEMA_OPTION_WAIT);
}

static INT32 _aclip_nfy_fct(
    HANDLE_T            h_aud,
    SCC_AUD_CLIP_COND_T e_nfy_cond,
    VOID                *pv_nfy_tag,
    UINT32              ui4_data_1,
    UINT32              ui4_data_2)
{
    SCT_AUDIO_CTRL_CTX_T* pt_a_ctrl = NULL;

    _DECODE_INFO_T* pt_dec_info = (_DECODE_INFO_T*) pv_nfy_tag;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    
    MHEG5_ASSERT(NULL != pt_a_ctrl);
    
    /* play, pause, stop */
    if (e_nfy_cond == SCC_AUD_CLIP_COND_CTRL_DONE)
    {
        switch (ui4_data_1)
        {
            /* play */
            case SCC_AUD_CLIP_COND_PLAY_CTRL_DONE:
                DBG_INFO((
                    "{MHEG5 Stack} Audio clip callback notify: SCC_AUD_CLIP_COND_PLAY_CTRL_DONE\r\n"));
                pt_dec_info->b_err = FALSE;
                tmMHEG5NotifyAudioStarted((BOOLEAN)TRUE);
                x_sema_unlock(pt_dec_info->h_lock);
                break;
                
            /* stop */
            case SCC_AUD_CLIP_COND_STOP_CTRL_DONE:
                DBG_INFO(("{MHEG5 Stack} Audio clip callback notify: SCC_AUD_CLIP_COND_STOP_CTRL_DONE\r\n"));
                if ((NULL != pt_a_ctrl->pui1_audio_buffer) && (FALSE == pt_a_ctrl->b_keep_audio_buffer))
                {
                    x_mem_free((VOID*) pt_a_ctrl->pui1_audio_buffer);
                    pt_a_ctrl->pui1_audio_buffer = NULL;
                    pt_a_ctrl->ui4_audio_buffer_len = 0;
                    pt_a_ctrl->t_aclip_encoding = MHEG5_MPEG2_AUDIO;
                }
                pt_a_ctrl->b_aclip_enabled = FALSE;
                pt_dec_info->b_err = FALSE;
                x_sema_unlock(pt_dec_info->h_lock);
                break;
                
            /* pause */
            case SCC_AUD_CLIP_COND_PAUSE_CTRL_DONE:
                DBG_INFO((
                    "{MHEG5 Stack} AClip callback notify: SCC_AUD_CLIP_COND_PAUSE_CTRL_DONE\r\n"));
                pt_dec_info->b_err = FALSE;
                x_sema_unlock(pt_dec_info->h_lock);
                break;

            /* resume */
            case SCC_AUD_CLIP_COND_RESUME_CTRL_DONE:
                DBG_INFO((
                    "{MHEG5 Stack} AClip callback notify: SCC_AUD_CLIP_COND_RESUME_CTRL_DONE\r\n"));
                pt_dec_info->b_err = FALSE;
                x_sema_unlock(pt_dec_info->h_lock);
                break;

            default:
                DBG_INFO((
                    "{MHEG5 Stack} Unkonw return state in audio clip.\n"));
                break;
        }
    }
    /* loop end */
    else if (e_nfy_cond == SCC_AUD_CLIP_COND_AUD_CLIP_DONE)
    {
        DBG_INFO((
            "{MHEG5 Stack} Audio clip callback notify: SCC_AUD_CLIP_COND_AUD_CLIP_DONE\r\n"));

        tmMHEG5NotifyAudioStopped((BOOLEAN)TRUE);

        pt_a_ctrl->e_a_play_type = PLAYED_FROM_STREAM;

        pt_dec_info->b_err = FALSE;
        x_sema_unlock(pt_dec_info->h_lock);
    }
    else /* e_nfy_cond == SCC_AUD_CLIP_COND_ERROR */
    {
        DBG_ERROR(("{MHEG5 Stack} ERROR! Audio clip decoding failed (in callback)!\r\n"));
        pt_dec_info->b_err = TRUE;
        x_sema_unlock(pt_dec_info->h_lock);
        
        return MHEG5R_INTERNAL_ERROR;
    }

    return MHEG5R_OK;
}

static INT32 _aclip_stop(BOOL b_sync, BOOL b_keep_audio_buffer)
{
    INT32 i4_ret;
    SCT_AUDIO_CTRL_CTX_T* pt_a_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);

    pt_a_ctrl->b_keep_audio_buffer = b_keep_audio_buffer;
    
    i4_ret = _aclip_close(b_sync);
        
    pt_a_ctrl->e_a_play_type = PLAYED_FROM_STREAM;
    
    return i4_ret;
}

static INT32 _aclip_restart(VOID)
{
    SCT_AUDIO_CTRL_CTX_T* pt_a_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);
    
    if ((NULL != pt_a_ctrl->pui1_audio_buffer) && (TRUE == pt_a_ctrl->b_keep_audio_buffer) && (pt_a_ctrl->ui4_audio_buffer_len > 0))
    {
        tmMHEG5Err_t    t_err;
        pt_a_ctrl->b_keep_audio_buffer = FALSE;
        t_err = tmMHEG5PlayAudioClip((U8BIT *)pt_a_ctrl->pui1_audio_buffer, (U32BIT)pt_a_ctrl->ui4_audio_buffer_len, (U32BIT)0, pt_a_ctrl->t_aclip_encoding);
        if (t_err == MHEG5_OK)
        {
            return MHEG5R_OK;
        }
        else
        {
            return MHEG5R_INTERNAL_ERROR;
        }
    }
    else
    {
        return MHEG5R_INV_STATE;
    }
}

static VOID _aclip_unlock(VOID)
{
    SCT_AUDIO_CTRL_CTX_T* pt_a_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);
    
    x_sema_unlock(pt_a_ctrl->t_a_dec_info.h_lock);
}

static VOID _adjust_video_for_force_4x3(
    MHEG5_WSS_T                             e_wss,
    tmMHEG5DispAspectRatio_t                e_display_ar,
    tmMHEG5DisplayAspectRatioConversion_t   e_decfc,
    VSH_REGION_INFO_T*                      pt_region)
{
    switch (e_wss)
    {
        case MHEG5_WSS_4_3FF:
            /* no need to adjust */
            break;
            
        case MHEG5_WSS_16_9FF:
            if (MHEG5_DISPLAY_ASPECT_RATIO_16x9 == e_display_ar)
            {
                INT32 i4_half_w  = (INT32) (MAX_REGION_WIDTH / 2);
                INT32 i4_dst_l = (INT32) pt_region->ui4_x;
                INT32 i4_dst_r = (INT32) (pt_region->ui4_x + pt_region->ui4_width);
                
                pt_region->ui4_x     = (UINT32) ((((i4_dst_l - i4_half_w) * 3) >> 2) + i4_half_w);
                pt_region->ui4_width = (UINT32) ((((i4_dst_r - i4_half_w) * 3) >> 2) + i4_half_w) - pt_region->ui4_x;
            }
            break;
            
        case MHEG5_WSS_14_9:
        case MHEG5_WSS_4_3_SP_14_9:
            if (MHEG5_DISPLAY_ASPECT_RATIO_16x9 == e_display_ar)
            {
                INT32 i4_half_w  = (INT32) (MAX_REGION_WIDTH / 2);
                INT32 i4_dst_l = (INT32) pt_region->ui4_x;
                INT32 i4_dst_r = (INT32) (pt_region->ui4_x + pt_region->ui4_width);
                
                pt_region->ui4_x     = (UINT32) (((i4_dst_l - i4_half_w) * 6) / 7 + i4_half_w);
                pt_region->ui4_width = (UINT32) (((i4_dst_r - i4_half_w) * 6) / 7 + i4_half_w) - pt_region->ui4_x;
            }
            break;
                
        default:
            break;
    }
}

static VOID _adjust_video_for_fullscreen(
    MHEG5_WSS_T                             e_wss,
    tmMHEG5DispAspectRatio_t                e_display_ar,
    tmMHEG5DisplayAspectRatioConversion_t   e_decfc,
    VSH_REGION_INFO_T*                      pt_region)
{
    switch (e_wss)
    {
        case MHEG5_WSS_4_3FF:
            if (MHEG5_DISPLAY_ASPECT_RATIO_16x9 == e_display_ar)
            {
                INT32 i4_half_w  = (INT32) (MAX_REGION_WIDTH / 2);
                INT32 i4_dst_l = (INT32) pt_region->ui4_x;
                INT32 i4_dst_r = (INT32) (pt_region->ui4_x + pt_region->ui4_width);

                pt_region->ui4_x     = (UINT32) (((i4_dst_l - i4_half_w) << 2) / 3 + i4_half_w);
                pt_region->ui4_width = (UINT32) (((i4_dst_r - i4_half_w) << 2) / 3 + i4_half_w) - pt_region->ui4_x;
            }
            break;
            
        case MHEG5_WSS_14_9:
            if (MHEG5_DISPLAY_ASPECT_RATIO_16x9 == e_display_ar)
            {
                INT32 i4_half_w  = (INT32) (MAX_REGION_WIDTH / 2);
                INT32 i4_dst_l = (INT32) pt_region->ui4_x;
                INT32 i4_dst_r = (INT32) (pt_region->ui4_x + pt_region->ui4_width);
                
                pt_region->ui4_x     = (UINT32) (((i4_dst_l - i4_half_w) * 7) / 6 + i4_half_w);
                pt_region->ui4_width = (UINT32) (((i4_dst_r - i4_half_w) * 7) / 6 + i4_half_w) - pt_region->ui4_x;
            }
            else if ((MHEG5_DISPLAY_ASPECT_RATIO_4x3 == e_display_ar) &&
                     (MHEG5_DISPLAY_AR_CONVERSION_LETTER_BOX_14_9 == e_decfc))
            {
                INT32 i4_half_h  = (INT32) (MAX_REGION_HEIGHT / 2);
                INT32 i4_dst_top = (INT32) pt_region->ui4_y;
                INT32 i4_dst_bot = (INT32) (pt_region->ui4_y + pt_region->ui4_height);
                
                pt_region->ui4_y      = (UINT32) (((i4_dst_top - i4_half_h) * 7) / 6 + i4_half_h);
                pt_region->ui4_height = (UINT32) (((i4_dst_bot - i4_half_h) * 7) / 6 + i4_half_h) - pt_region->ui4_y;
            }
            break;
            
        case MHEG5_WSS_4_3_SP_14_9:
            if (MHEG5_DISPLAY_ASPECT_RATIO_16x9 == e_display_ar)
            {
                INT32 i4_half_w  = (INT32) (MAX_REGION_WIDTH / 2);
                INT32 i4_dst_l = (INT32) pt_region->ui4_x;
                INT32 i4_dst_r = (INT32) (pt_region->ui4_x + pt_region->ui4_width);
                
                pt_region->ui4_x     = (UINT32) (((i4_dst_l - i4_half_w) * 7) / 6 + i4_half_w);
                pt_region->ui4_width = (UINT32) (((i4_dst_r - i4_half_w) * 7) / 6 + i4_half_w) - pt_region->ui4_x;
            }
            break;
                
        case MHEG5_WSS_16_9:
            if ((MHEG5_DISPLAY_ASPECT_RATIO_4x3 == e_display_ar) &&
                (MHEG5_DISPLAY_AR_CONVERSION_LETTER_BOX == e_decfc))
            {
                INT32 i4_half_h  = (INT32) (MAX_REGION_HEIGHT / 2);
                INT32 i4_dst_top = (INT32) pt_region->ui4_y;
                INT32 i4_dst_bot = (INT32) (pt_region->ui4_y + pt_region->ui4_height);
                
                pt_region->ui4_y      = (UINT32) (((i4_dst_top - i4_half_h) << 2) / 3 + i4_half_h);
                pt_region->ui4_height = (UINT32) (((i4_dst_bot - i4_half_h) << 2) / 3 + i4_half_h) - pt_region->ui4_y;
            }
            break;
            
        default:
            break;
    }
}

static VOID _adjust_with_overscan(
    VSH_REGION_INFO_T*                  pt_src_region,
    VSH_REGION_INFO_T*                  pt_dst_region,
    const SCC_VID_OVER_SCAN_CLIPPER_T*  pt_ovs_clip)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    if (NULL_HANDLE != pt_v_ctrl->h_video)
    {
    #if 0
        SCC_VID_OVER_SCAN_CLIPPER_T t_ovs_clip_scaled;
        
        const UINT32 ui4_hr = MAX_REGION_WIDTH  / VSH_REGION_MAX_WIDTH;
        const UINT32 ui4_vr = MAX_REGION_HEIGHT / VSH_REGION_MAX_HEIGHT;
    #endif
            
        GL_RECT_T   t_src, t_dst, t_bound;
        UINT32      ui4_sum;
        INT64       x1, x2;
            
    #if 0
        t_ovs_clip_scaled.ui4_left   = ui4_hr * pt_ovs_clip->ui4_left;
        t_ovs_clip_scaled.ui4_right  = ui4_hr * pt_ovs_clip->ui4_right;
        t_ovs_clip_scaled.ui4_top    = ui4_vr * pt_ovs_clip->ui4_top;
        t_ovs_clip_scaled.ui4_bottom = ui4_vr * pt_ovs_clip->ui4_bottom;
    #endif
            
        SET_RECT_BY_SIZE(
            &t_src, 
            (INT32)pt_src_region->ui4_x,
            (INT32)pt_src_region->ui4_y,
            pt_src_region->ui4_width,
            pt_src_region->ui4_height);

        SET_RECT_BY_SIZE(
            &t_dst, 
            (INT32)pt_dst_region->ui4_x,
            (INT32)pt_dst_region->ui4_y,
            pt_dst_region->ui4_width,
            pt_dst_region->ui4_height);

        SET_RECT_BY_SIZE(&t_bound, 0, 0, (INT32)MAX_REGION_WIDTH, (INT32)MAX_REGION_HEIGHT);
        
        ui4_sum = (UINT32) (pt_ovs_clip->ui4_left + pt_ovs_clip->ui4_right);
        x1 = ((INT64) MAX_REGION_WIDTH * pt_ovs_clip->ui4_left << 8) / (VSH_REGION_MAX_WIDTH - ui4_sum);
        x2 = ((INT64) MAX_REGION_WIDTH * ui4_sum << 8) / (VSH_REGION_MAX_WIDTH - ui4_sum);

        t_dst.i4_left  = (INT32)(((t_dst.i4_left  << 8) + (t_dst.i4_left  * x2) / (INT32) MAX_REGION_WIDTH - x1 + 0x80) >> 8);
        t_dst.i4_right = (INT32)(((t_dst.i4_right << 8) + (t_dst.i4_right * x2) / (INT32) MAX_REGION_WIDTH - x1 + 0x80) >> 8);
        
        ui4_sum = (UINT32) (pt_ovs_clip->ui4_top + pt_ovs_clip->ui4_bottom);
        x1 = ((INT64) MAX_REGION_HEIGHT * pt_ovs_clip->ui4_top << 8) / (VSH_REGION_MAX_HEIGHT - ui4_sum);
        x2 = ((INT64) MAX_REGION_HEIGHT * ui4_sum << 8) / (VSH_REGION_MAX_HEIGHT - ui4_sum);

        t_dst.i4_top    = (INT32)(((t_dst.i4_top    << 8) + (t_dst.i4_top    * x2) / (INT32) MAX_REGION_WIDTH - x1 + 0x80) >> 8);
        t_dst.i4_bottom = (INT32)(((t_dst.i4_bottom << 8) + (t_dst.i4_bottom * x2) / (INT32) MAX_REGION_WIDTH - x1 + 0x80) >> 8);
    
        _clip_video(
            pt_src_region,
            pt_dst_region,
            FALSE,
            &t_bound,
            &t_src,
            &t_dst);
    }
}

static VOID _adjust_with_video_mode(
    MHEG5_VIDEO_MODE_T                      e_vid_mode,
    MHEG5_WSS_T                             e_wss,
    tmMHEG5DispAspectRatio_t                e_display_ar,
    tmMHEG5DisplayAspectRatioConversion_t   e_decfc,
    VSH_REGION_INFO_T*                      pt_region)
{
    switch (e_vid_mode)
    {
        case MHEG5_VIDEO_MODE_FULLSCREEN:
            _adjust_video_for_fullscreen(e_wss, e_display_ar, e_decfc, pt_region);
            break;
            
        case MHEG5_VIDEO_MODE_FORCE_4x3:
        case MHEG5_VIDEO_MODE_GEN_LETTER_BOX:
            _adjust_video_for_force_4x3(e_wss, e_display_ar, e_decfc, pt_region);
            break;
            
        case MHEG5_VIDEO_MODE_AUTO:
        default:
            break;
    }
}

static VOID _apply_wss(
    VSH_REGION_INFO_T* pt_src_region,
    VSH_REGION_INFO_T* pt_dst_region)
{
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);  
    
    if ((MHEG5_ASPECT_RATIO_UNSEPCIFIED != pt_dis_cfg->e_scene_ar) || 
        (RECT_W(&pt_dis_cfg->t_rc_video) >= MHEG5_SCREEN_WIDTH &&
         RECT_H(&pt_dis_cfg->t_rc_video) >= MHEG5_SCREEN_HEIGHT))
    {
        BOOL b_apply = (MHEG5_DISPLAY_ASPECT_RATIO_16x9 == pt_dis_cfg->e_display_ar) &&
                       (MHEG5_ASPECT_RATIO_16x9 != pt_dis_cfg->e_scene_ar);
                    
        /* Apply WSS to display */
        switch (pt_dis_cfg->e_wss)
        {
            case MHEG5_WSS_4_3FF:
                if (b_apply)
                {
                    INT32 i4_half_w = (INT32) (MAX_REGION_WIDTH / 2);
                    INT32 i4_dst_l  = (INT32) pt_dst_region->ui4_x;
                    INT32 i4_dst_r  = (INT32) (pt_dst_region->ui4_x + pt_dst_region->ui4_width);
                    
                    i4_dst_l = (i4_dst_l - i4_half_w) * 3 / 4 + i4_half_w;
                    i4_dst_r = (i4_dst_r - i4_half_w) * 3 / 4 + i4_half_w;
                    
                    pt_dst_region->ui4_x     = (UINT32) i4_dst_l;
                    pt_dst_region->ui4_width = (UINT32) (i4_dst_r - i4_dst_l);
                }
                break;

            case MHEG5_WSS_14_9:
            case MHEG5_WSS_4_3_SP_14_9:
                if (b_apply)
                {
                    INT32 i4_half_h   = (INT32) (MAX_REGION_HEIGHT / 2);
                    INT32 i4_dst_top  = (INT32) pt_dst_region->ui4_y;
                    INT32 i4_dst_bot  = (INT32) (pt_dst_region->ui4_y + pt_dst_region->ui4_height);
                    INT32 i4_src_bot  = (INT32) (pt_src_region->ui4_y + pt_src_region->ui4_height);
                    INT32 ui4_y_ratio = (INT32)((pt_src_region->ui4_height << 12) * 6 / (pt_dst_region->ui4_height * 7));
                    
                    INT32 i4_half_w = (INT32) (MAX_REGION_WIDTH / 2);
                    INT32 i4_dst_l  = (INT32) pt_dst_region->ui4_x;
                    INT32 i4_dst_r  = (INT32) (pt_dst_region->ui4_x + pt_dst_region->ui4_width);
                    
                    i4_dst_top = (i4_dst_top - i4_half_h) * 7 / 6 + i4_half_h;
                    i4_dst_bot = (i4_dst_bot - i4_half_h) * 7 / 6 + i4_half_h;
                
                    if (i4_dst_top < 0)
                    {
                        pt_src_region->ui4_y += (((UINT32) (-i4_dst_top) * ui4_y_ratio) >> 12);
                        pt_dst_region->ui4_y  = 0;
                    }
                    else
                    {
                        pt_dst_region->ui4_y = (UINT32) i4_dst_top;
                    }

                    if (i4_dst_bot > (INT32)MAX_REGION_HEIGHT)
                    {
                        i4_src_bot -= (((UINT32) (i4_dst_bot - MAX_REGION_HEIGHT) * ui4_y_ratio) >> 12);
                        pt_src_region->ui4_height = (UINT32) (i4_src_bot - pt_src_region->ui4_y);
                        pt_dst_region->ui4_height = (UINT32) (MAX_REGION_HEIGHT - pt_dst_region->ui4_y);
                    }
                    else
                    {
                        pt_src_region->ui4_height = (UINT32) (i4_src_bot - pt_src_region->ui4_y);
                        pt_dst_region->ui4_height = (UINT32) (i4_dst_bot - pt_src_region->ui4_y);
                    }
                    
                    
                    i4_dst_l = (i4_dst_l - i4_half_w) * 6 / 7 + i4_half_w;
                    i4_dst_r = (i4_dst_r - i4_half_w) * 6 / 7 + i4_half_w;
                    
                    pt_dst_region->ui4_x     = (UINT32) i4_dst_l;
                    pt_dst_region->ui4_width = (UINT32) (i4_dst_r - i4_dst_l);
                }
                break;
                
            case MHEG5_WSS_16_9:
                if (b_apply)
                {
                    INT32 i4_half_h   = (INT32) (MAX_REGION_HEIGHT / 2);
                    INT32 i4_dst_top  = (INT32) pt_dst_region->ui4_y;
                    INT32 i4_dst_bot  = (INT32) (pt_dst_region->ui4_y + pt_dst_region->ui4_height);
                    INT32 i4_src_bot  = (INT32) (pt_src_region->ui4_y + pt_src_region->ui4_height);
                    INT32 ui4_y_ratio = (INT32)((pt_src_region->ui4_height << 12) * 3 / (pt_dst_region->ui4_height * 4));
                    
                    i4_dst_top = (i4_dst_top - i4_half_h) * 4 / 3 + i4_half_h;
                    i4_dst_bot = (i4_dst_bot - i4_half_h) * 4 / 3 + i4_half_h;
                
                    if (i4_dst_top < 0)
                    {
                        pt_src_region->ui4_y += (((UINT32) (-i4_dst_top) * ui4_y_ratio) >> 12);
                        pt_dst_region->ui4_y  = 0;
                    }
                    else
                    {
                        pt_dst_region->ui4_y = (UINT32) i4_dst_top;
                    }

                    if (i4_dst_bot > (INT32)MAX_REGION_HEIGHT)
                    {
                        i4_src_bot -= (((UINT32) (i4_dst_bot - MAX_REGION_HEIGHT) * ui4_y_ratio) >> 12);
                        pt_src_region->ui4_height = (UINT32) (i4_src_bot - pt_src_region->ui4_y);
                        pt_dst_region->ui4_height = (UINT32) (MAX_REGION_HEIGHT - pt_dst_region->ui4_y);
                    }
                    else
                    {
                        pt_src_region->ui4_height = (UINT32) (i4_src_bot - pt_src_region->ui4_y);
                        pt_dst_region->ui4_height = (UINT32) (i4_dst_bot - pt_src_region->ui4_y);
                    }                
                }
                break;
                
            case MHEG5_WSS_16_9FF:
            default:
                break;
        }
    }

    _clip_region(pt_src_region, MAX_REGION_WIDTH, MAX_REGION_HEIGHT);
    _clip_region(pt_dst_region, MAX_REGION_WIDTH, MAX_REGION_HEIGHT);
}

#if 0
static BOOL _av_same_channel(VOID)
{
    return ((_ui2_a_svc_id == _ui2_v_svc_id) && (_ui2_a_svc_id != 0));
}
#endif

static BOOL _calc_overscan_clipper(
    BOOL                            b_fullscreen,
    SCC_VID_OVER_SCAN_CLIPPER_T*    pt_ovs)
{
    BOOL b_ret = FALSE;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);   
    
    if (NULL_HANDLE != pt_v_ctrl->h_video)
    {
        if (b_fullscreen)
        {
            x_scc_vid_set_overscan_clipper(pt_v_ctrl->h_video, &pt_dis_cfg->t_dflt_ovs_clip);
        }
        else
        {
            if (SMR_OK == x_scc_vid_get_overscan_clipper(pt_v_ctrl->h_video, pt_ovs))
            {
                SCC_VID_OVER_SCAN_CLIPPER_T t_ovs_clip = {0, 0, 0, 0};
                
                if ((0 == pt_ovs->ui4_left)  &&
                    (0 == pt_ovs->ui4_right) &&
                    (0 == pt_ovs->ui4_top)   &&
                    (0 == pt_ovs->ui4_bottom))
                {
                    *pt_ovs = pt_dis_cfg->t_dflt_ovs_clip;
                }

                if (pt_gnrl_info->b_mheg5_running)
                {
                    x_scc_vid_set_overscan_clipper(pt_v_ctrl->h_video, &t_ovs_clip);
                }
            }
            else
            {
                (*pt_ovs) = pt_dis_cfg->t_dflt_ovs_clip;
            }
            
            b_ret = TRUE;
        }
    }

    return (b_ret);
}

static BOOL _calc_video_adjustment(
    MHEG5_VIDEO_MODE_T  e_vid_mode,
    VSH_REGION_INFO_T*  pt_src_region,
    VSH_REGION_INFO_T*  pt_dst_region)
{
    GL_RECT_T   t_src, t_dst, t_bound;
    INT32       i4_ratio;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);  

    i4_ratio = _is_fullscreen_cif();
    
    if (2 == i4_ratio)
    {
        UINT32 ui4_sw = (pt_dis_cfg->ui4_real_vw << 1);
        UINT32 ui4_sh = (pt_dis_cfg->ui4_real_vh << 1);

        SET_RECT_BY_SIZE(
            &t_bound,
            (MHEG5_SCREEN_WIDTH  - ui4_sw) / 2,
            (MHEG5_SCREEN_HEIGHT - ui4_sh) / 2,
            ui4_sw,
            ui4_sh);
    }
    else if (1 == i4_ratio)
    {
        SET_RECT_BY_SIZE(
            &t_bound,
            ((INT32) RECT_W(&pt_dis_cfg->t_rc_video) - (INT32) pt_dis_cfg->ui4_real_vw) / 2 + pt_dis_cfg->t_rc_video.i4_left,
            ((INT32) RECT_H(&pt_dis_cfg->t_rc_video) - (INT32) pt_dis_cfg->ui4_real_vh) / 2 + pt_dis_cfg->t_rc_video.i4_top,
            pt_dis_cfg->ui4_real_vw,
            pt_dis_cfg->ui4_real_vh);
    }
    else
    {
        t_bound = pt_dis_cfg->t_rc_video;
    }
    
    switch (pt_dis_cfg->e_decfc)
    {
        case MHEG5_DISPLAY_AR_CONVERSION_PILLAR_BOX:
            _set_pillarbox_transformation(&t_bound, &t_src, &t_dst);
            break;
            
        case MHEG5_DISPLAY_AR_CONVERSION_LETTER_BOX:
            _set_letterbox_transformation(&t_bound, &t_src, &t_dst);
            break;
            
        case MHEG5_DISPLAY_AR_CONVERSION_PAN_SCAN:
            _set_center_cut_out_transformation(&t_bound, &t_src, &t_dst);
            break;
            
        case MHEG5_DISPLAY_AR_CONVERSION_LETTER_BOX_14_9:
            _set_14x9_letterbox_transformation(&t_bound, &t_src, &t_dst);
            break;
            
        case MHEG5_DISPLAY_AR_CONVERSION_IGNORE:
        default:
            _set_ignore_transformation(&t_bound, &t_src, &t_dst);
            break;
    }

    if (RECT_EMPTY(&t_src) || RECT_EMPTY(&t_dst))
    {
        return FALSE;
    }
    
#if 0
    DBG_INFO((
        "{MHEG5 Stack} After DecFC <source> (%d, %d), (%d, %d)\r\n",
        t_src.i4_left,
        t_src.i4_top,
        t_src.i4_right,
        t_src.i4_bottom));

    DBG_INFO((
        "{MHEG5 Stack} After DecFC <destination> (%d, %d), (%d, %d)\r\n\r\n",
        t_dst.i4_left,
        t_dst.i4_top,
        t_dst.i4_right,
        t_dst.i4_bottom));
#endif

    SET_RECT_BY_SIZE(&t_bound, 0, 0, (INT32)MAX_REGION_WIDTH, (INT32)MAX_REGION_HEIGHT);
    
    _clip_video(
        pt_src_region,
        pt_dst_region,
        TRUE,
        &t_bound,
        &t_src,
        &t_dst);

    if (0 == pt_src_region->ui4_width  || 
        0 == pt_src_region->ui4_height ||
        0 == pt_dst_region->ui4_width  ||
        0 == pt_dst_region->ui4_height)
    {
        return FALSE;
    }
    
#if 0
    DBG_INFO((
        "{MHEG5 Stack} After clipping <source> (%d, %d), (%d, %d)\r\n",
        pt_src_region->ui4_x,
        pt_src_region->ui4_y,
        pt_src_region->ui4_width,
        pt_src_region->ui4_height));

    DBG_INFO((
        "{MHEG5 Stack} After clipping <destination> (%d, %d), (%d, %d)\r\n\r\n",
        pt_dst_region->ui4_x,
        pt_dst_region->ui4_y,
        pt_dst_region->ui4_width,
        pt_dst_region->ui4_height));
#endif

    _apply_wss(pt_src_region, pt_dst_region);

    if (0 == pt_src_region->ui4_width  || 
        0 == pt_src_region->ui4_height ||
        0 == pt_dst_region->ui4_width  ||
        0 == pt_dst_region->ui4_height)
    {
        return FALSE;
    }
    
#if 0
    DBG_INFO((
        "{MHEG5 Stack} After WSS <source> (%d, %d), (%d, %d)\r\n",
        pt_src_region->ui4_x,
        pt_src_region->ui4_y,
        pt_src_region->ui4_width,
        pt_src_region->ui4_height));

    DBG_INFO((
        "{MHEG5 Stack} After WSS <destination> (%d, %d), (%d, %d)\r\n\r\n",
        pt_dst_region->ui4_x,
        pt_dst_region->ui4_y,
        pt_dst_region->ui4_width,
        pt_dst_region->ui4_height));
#endif

    _adjust_with_video_mode(
        e_vid_mode, 
        pt_dis_cfg->e_wss, 
        pt_dis_cfg->e_display_ar,
        pt_dis_cfg->e_decfc,
        pt_dst_region);
    
    if (0 == pt_dst_region->ui4_width  ||
        0 == pt_dst_region->ui4_height)
    {
        return FALSE;
    }
    
#if 0
    DBG_INFO((
        "{MHEG5 Stack} After screen mode adjustment <destination> (%d, %d), (%d, %d)\r\n\r\n",
        pt_dst_region->ui4_x,
        pt_dst_region->ui4_y,
        pt_dst_region->ui4_width,
        pt_dst_region->ui4_height));
#endif

    if (pt_dis_cfg->b_adjust_overscan)
    {
        _adjust_with_overscan(
            pt_src_region,
            pt_dst_region,
            &pt_dis_cfg->t_ovs_clip_runtime);

        if (0 == pt_src_region->ui4_width  || 
            0 == pt_src_region->ui4_height ||
            0 == pt_dst_region->ui4_width  ||
            0 == pt_dst_region->ui4_height)
        {
            return FALSE;
        }
    }

#if 0    
    DBG_INFO((
        "{MHEG5 Stack} After overscan clipping (%d) <source> (%d, %d), (%d, %d)\r\n",
        pt_src_region->ui4_x,
        pt_src_region->ui4_y,
        pt_src_region->ui4_width,
        pt_src_region->ui4_height));

    DBG_INFO((
        "{MHEG5 Stack} After overscan clipping (%d) <destination> (%d, %d), (%d, %d)\r\n\r\n",
        pt_dst_region->ui4_x,
        pt_dst_region->ui4_y,
        pt_dst_region->ui4_width,
        pt_dst_region->ui4_height));
#endif

    _scale_to_video_region(pt_src_region);
    _scale_to_video_region(pt_dst_region);

    return TRUE;
}

static VOID _clip_region(
    VSH_REGION_INFO_T*  pt_region,
    UINT32              ui4_max_w,
    UINT32              ui4_max_h)
{
    if (pt_region->ui4_x + pt_region->ui4_width > ui4_max_w)
    {
        pt_region->ui4_width = ui4_max_w - pt_region->ui4_x;
    }

    if (pt_region->ui4_y + pt_region->ui4_height > ui4_max_h)
    {
        pt_region->ui4_height = ui4_max_h - pt_region->ui4_y;
    }
}

static VOID _clip_video(
    VSH_REGION_INFO_T*  pt_src_region,
    VSH_REGION_INFO_T*  pt_dsp_region,
    BOOL                b_coord_tfx,
    const GL_RECT_T*    pt_bound,
    const GL_RECT_T*    pt_src,
    const GL_RECT_T*    pt_dst)
{
    INT32   i4_tl, i4_tt, i4_tr, i4_tb;
    UINT32  ui4_rx, ui4_ry;
    UINT32  ui4_src_w, ui4_src_h;
    UINT32  ui4_src_adj_x, ui4_src_adj_y;
    UINT32  ui4_fraction;
    
    if (b_coord_tfx)
    {
        i4_tl = (INT32) pt_dst->i4_left   * (INT32) MAX_REGION_WIDTH  / MHEG5_SCREEN_WIDTH;
        i4_tt = (INT32) pt_dst->i4_top    * (INT32) MAX_REGION_HEIGHT / MHEG5_SCREEN_HEIGHT;
        i4_tr = (INT32) pt_dst->i4_right  * (INT32) MAX_REGION_WIDTH  / MHEG5_SCREEN_WIDTH;
        i4_tb = (INT32) pt_dst->i4_bottom * (INT32) MAX_REGION_HEIGHT / MHEG5_SCREEN_HEIGHT;
        ui4_fraction = 8;
    }
    else
    {
        i4_tl = (INT32) pt_dst->i4_left;
        i4_tt = (INT32) pt_dst->i4_top;
        i4_tr = (INT32) pt_dst->i4_right;
        i4_tb = (INT32) pt_dst->i4_bottom;
        ui4_fraction = 11;
    }
    
    ui4_src_w = (UINT32)RECT_W(pt_src);
    ui4_src_h = (UINT32)RECT_H(pt_src);

    ui4_rx = (UINT32)(((((INT64)(ui4_src_w << ui4_fraction)) << 16 ) / ((INT64)(i4_tr - i4_tl)) + 0x8000) >> 16);
	ui4_ry = (UINT32)(((((INT64)(ui4_src_h << ui4_fraction)) << 16 ) / ((INT64)(i4_tb - i4_tt)) + 0x8000) >> 16);

    ui4_src_adj_x = ui4_src_adj_y = 0;
    
    if (i4_tl < pt_bound->i4_left)
    {
        ui4_src_adj_x = ((((UINT32) (pt_bound->i4_left - i4_tl)) * ui4_rx) >> ui4_fraction);
        pt_src_region->ui4_x = (UINT32) pt_src->i4_left + ui4_src_adj_x;
        pt_dsp_region->ui4_x = (UINT32) pt_bound->i4_left;
    }
    else
    {
        pt_src_region->ui4_x = (UINT32) pt_src->i4_left;
        pt_dsp_region->ui4_x = (UINT32) i4_tl;
    }

    if (i4_tt < pt_bound->i4_top)
    {
        ui4_src_adj_y = ((((UINT32) (pt_bound->i4_top - i4_tt)) * ui4_ry) >> ui4_fraction);
        pt_src_region->ui4_y = (UINT32) pt_src->i4_top + ui4_src_adj_y;
        pt_dsp_region->ui4_y = (UINT32) pt_bound->i4_top;
    }
    else
    {
        pt_src_region->ui4_y = (UINT32) pt_src->i4_top;
        pt_dsp_region->ui4_y = (UINT32) i4_tt;
    }

    if (i4_tr >= pt_bound->i4_right)
    {
        pt_src_region->ui4_width = ui4_src_w - ui4_src_adj_x - ((((UINT32) (i4_tr - pt_bound->i4_right)) * ui4_rx) >> ui4_fraction);
        pt_dsp_region->ui4_width = (UINT32)(pt_bound->i4_right - pt_dsp_region->ui4_x);
    }
    else
    {
        pt_src_region->ui4_width = ui4_src_w - ui4_src_adj_x;
        pt_dsp_region->ui4_width = (UINT32) i4_tr - pt_dsp_region->ui4_x;
    }
    
    if (i4_tb > pt_bound->i4_bottom)
    {
        pt_src_region->ui4_height = ui4_src_h - ui4_src_adj_y - ((((UINT32) (i4_tb - pt_bound->i4_bottom)) * ui4_ry) >> ui4_fraction);
        pt_dsp_region->ui4_height = (UINT32)(pt_bound->i4_bottom - pt_dsp_region->ui4_y);
    }
    else
    {
        pt_src_region->ui4_height = ui4_src_h - ui4_src_adj_y;
        pt_dsp_region->ui4_height = (UINT32) i4_tb  - pt_dsp_region->ui4_y;
    }

    _clip_region(pt_src_region, MAX_REGION_WIDTH, MAX_REGION_HEIGHT);
    _clip_region(pt_dsp_region, MAX_REGION_WIDTH, MAX_REGION_HEIGHT);
}

static MHEG5_DECFC_T _convert_decfc(tmMHEG5DisplayAspectRatioConversion_t e_decfc)
{
    switch (e_decfc)
    {
        case MHEG5_DISPLAY_AR_CONVERSION_UNKNOWN:
            return MHEG5_DECFC_UNKNOWN;

        case MHEG5_DISPLAY_AR_CONVERSION_IGNORE:
            return MHEG5_DECFC_IGNORE;

        case MHEG5_DISPLAY_AR_CONVERSION_PAN_SCAN:
            return MHEG5_DECFC_PAN_SCAN;

        case MHEG5_DISPLAY_AR_CONVERSION_LETTER_BOX:
            return MHEG5_DECFC_LETTER_BOX;

        case MHEG5_DISPLAY_AR_CONVERSION_LETTER_BOX_14_9:
            return MHEG5_DECFC_LETTER_BOX_14_9;

        case MHEG5_DISPLAY_AR_CONVERSION_PILLAR_BOX:
            return MHEG5_DECFC_PILLAR_BOX;

        case MHEG5_DISPLAY_AR_CONVERSION_ZOOM_4_3:
            return MHEG5_DECFC_ZOOM_4_3;

        case MHEG5_DISPLAY_AR_CONVERSION_ZOOM_14_9:
            return MHEG5_DECFC_ZOOM_14_9;

        default:
            break;
    }

    return MHEG5_DECFC_UNKNOWN;
}

static INT32 _evaluate_audio(VOID)
{
    INT32               i4_ret;
    STREAM_PID_INFO_T   t_sinfo;
    MPEG_2_PID_T        t_pid;
    MHEG5_STREAM_DESC_T t_strm_desc;
    SCDB_REC_T          t_scdb_rec;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_a_ctrl);

    if (pt_a_ctrl->b_as_set)
    {
        t_strm_desc.e_strm_type = ST_AUDIO;
        t_strm_desc.ui2_on_id   = pt_a_ctrl->t_a_locator.original_network_id;
        t_strm_desc.ui2_ts_id   = pt_a_ctrl->t_a_locator.transport_stream_id;
        t_strm_desc.ui2_svc_id  = pt_a_ctrl->t_a_locator.service_id;
        t_strm_desc.i4_comp_tag = pt_a_ctrl->t_a_locator.ComponentTag;
    #if 0
        x_memcpy(t_strm_desc.s_lang, _t_def_lang, sizeof(ISO_639_LANG_T));
    #else
        i4_ret = x_svctx_get_using_scdb_rec(
            pt_gnrl_info->h_svctx,
            ST_AUDIO,
            &t_scdb_rec);

        if (i4_ret != SVCTXR_OK)
        {
            DBG_ERROR(("{MHEG5 Stack} x_svctx_get_using_scdb_rec() failed. (%d) Line: %d\r\n", i4_ret, __LINE__));
            x_memcpy(t_strm_desc.s_lang, pt_a_ctrl->t_def_lang, sizeof(ISO_639_LANG_T));
            t_strm_desc.e_aud_type = AUD_TYPE_UNKNOWN;
        }
        else
        {
            x_memcpy(t_strm_desc.s_lang, t_scdb_rec.u.t_audio_mpeg.s_lang, sizeof(ISO_639_LANG_T));
            t_strm_desc.e_aud_type = t_scdb_rec.u.t_audio_mpeg.e_type;
        }

        x_memcpy(t_strm_desc.s_gui_lang, pt_a_ctrl->t_gui_lang, sizeof(ISO_639_LANG_T));
    #endif
        
        t_sinfo.b_waiting = FALSE;
        
        /* query the PID */
        i4_ret = x_mheg5_resolve(
            pt_gnrl_info->ui4_eng_code,
            &t_strm_desc,
            _mheg5_resolve_nfy,
            (VOID *) &t_sinfo,
            (STREAM_PID_INFO_T*)&t_sinfo);

        if (i4_ret == MHEG5R_ASYNC_RESPONSE)
        {
            /* spin lock until the PID is got */
            while (t_sinfo.b_waiting);

            if (t_sinfo.b_found)
            {
                i4_ret = MHEG5R_OK;
            }
        }

        if (MHEG5R_OK == i4_ret)
        {
            t_pid        = t_sinfo.t_pid;
            pt_a_ctrl->t_a_pcr_pid = t_sinfo.t_pcr_pid;
            pt_a_ctrl->t_aud_enc   = (t_sinfo.ui1_enc_type == 0) ? (AUD_ENC_MPEG_2) : (t_sinfo.ui1_enc_type);
            
            DBG_INFO((
                "{MHEG5 Stack} Re-evaluate audio PID. OLD(%d) -> NEW(%d)\r\n",
                pt_a_ctrl->t_as_pid,
                t_pid));

            if ((!pt_a_ctrl->b_a_pid_ok) || (t_pid != pt_a_ctrl->t_as_pid))
            {
                pt_a_ctrl->t_as_pid = t_pid;
                
                if ((MHEG5_AUDIO_MHEG   == pt_a_ctrl->e_a_ctrl) &&
                    (PLAYED_FROM_STREAM == pt_a_ctrl->e_a_play_type))
                {
                    if (pt_a_ctrl->b_a_pid_ok)
                    {
                        _stop_current_stream(ST_AUDIO);
                        tmMHEG5NotifyAudioStopped((BOOLEAN)FALSE);
                    }

                    /* play */
                    #if 1
                    {
                    SVCTX_STREAM_INFO_T    t_strm_info;
                    
                    t_strm_info.e_strm_type              = ST_AUDIO;
                    t_strm_info.t_mpeg_2_pid             = t_pid;
                    t_strm_info.u.t_aud_stream.e_aud_enc = pt_a_ctrl->t_aud_enc;
                    
                    
                    i4_ret = x_svctx_select_mpeg_stream_by_pid_ex(
                         pt_gnrl_info->h_svctx,
                         &t_strm_info,
                         NULL,
                         NULL);
                    }
                    #else
                    i4_ret = x_svctx_select_mpeg_stream_by_pid(pt_gnrl_info->h_svctx, ST_AUDIO, t_pid, NULL, NULL);
                    #endif

                    if (i4_ret != SVCTXR_OK)
                    {
                        DBG_ERROR(("{MHEG5 Stack} x_svctx_select_mpeg_stream_by_pid() = %d failed.\n", i4_ret));
                    }
                    else
                    {
                        tmMHEG5NotifyAudioStarted((BOOLEAN)FALSE);
                        _set_stream_select_block(&t_pid, FALSE);
                        pt_a_ctrl->b_a_pid_ok = TRUE;
                        if (pt_a_ctrl->b_a_user)
                        {                            
                            _save_current_stream(ST_AUDIO);
                        }
                    
                    }
                }
            }
        }
        else
        {
            DBG_INFO((
                "{MHEG5 Stack} Cannot find the audio PID.\r\n"));

            if ((MHEG5_AUDIO_MHEG   == pt_a_ctrl->e_a_ctrl) &&
                (PLAYED_FROM_STREAM == pt_a_ctrl->e_a_play_type))
            {
                _stop_current_stream(ST_AUDIO);
                pt_a_ctrl->b_a_pid_ok = FALSE;
                tmMHEG5NotifyAudioStopped((BOOLEAN)FALSE);
            }
        }
    }
    
    return MHEG5R_OK;
}

static INT32 _evaluate_video(VOID)
{
    INT32               i4_ret;
    STREAM_PID_INFO_T   t_sinfo;
    MPEG_2_PID_T        t_pid;
    MHEG5_STREAM_DESC_T t_strm_desc;
    BOOL                b_frozen;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    
    if (pt_v_ctrl->b_vs_set)
    {
        t_strm_desc.e_strm_type = ST_VIDEO;
        t_strm_desc.ui2_on_id   = pt_v_ctrl->t_v_locator.original_network_id;
        t_strm_desc.ui2_ts_id   = pt_v_ctrl->t_v_locator.transport_stream_id;
        t_strm_desc.ui2_svc_id  = pt_v_ctrl->t_v_locator.service_id;
        t_strm_desc.i4_comp_tag = pt_v_ctrl->t_v_locator.ComponentTag;
        
        t_sinfo.b_waiting = TRUE;
        
        /* query the PID */
        i4_ret = x_mheg5_resolve(
            pt_gnrl_info->ui4_eng_code,
            &t_strm_desc,
            _mheg5_resolve_nfy,
            (VOID *) &t_sinfo,
            (STREAM_PID_INFO_T*)&t_sinfo);

        if (i4_ret == MHEG5R_ASYNC_RESPONSE)
        {
            /* spin lock until the PID is got */
            while (t_sinfo.b_waiting);

            if (t_sinfo.b_found)
            {
                i4_ret = MHEG5R_OK;
            }
        }

        if (MHEG5R_OK == i4_ret)
        {
            t_pid        = t_sinfo.t_pid;
            pt_v_ctrl->t_v_pcr_pid = t_sinfo.t_pcr_pid;
            pt_v_ctrl->t_vid_enc   = (t_sinfo.ui1_enc_type == 0) ? (VID_ENC_MPEG_2) : (t_sinfo.ui1_enc_type);;

            DBG_INFO((
                "{MHEG5 Stack} Re-evaluate video PID. OLD(%d) -> NEW(%d)\r\n",
                pt_v_ctrl->t_vs_pid,
                t_pid));

            if ((!pt_v_ctrl->b_v_pid_ok) || (t_pid != pt_v_ctrl->t_vs_pid))
            {
                pt_v_ctrl->t_vs_pid = t_pid;
                
                if ((MHEG5_VIDEO_MHEG   == pt_v_ctrl->e_v_ctrl) &&
                    (PLAYED_FROM_STREAM == pt_v_ctrl->e_v_play_type))
                {
                    SVCTX_STREAM_INFO_T   t_strm_info;

                    if (pt_v_ctrl->b_v_pid_ok)
                    {
                        _stop_current_stream(ST_VIDEO);
                        tmMHEG5NotifyVideoStopped();
                    }

                    if ((SVCTXR_OK == x_svctx_is_frozen(pt_gnrl_info->h_svctx, &b_frozen)) && b_frozen)
                    {
                        i4_ret = x_svctx_unfreeze(pt_gnrl_info->h_svctx);
                    }
            
                    /* play */
                    t_strm_info.e_strm_type              = ST_VIDEO;
                    t_strm_info.t_mpeg_2_pid             = t_pid;
                    t_strm_info.u.t_vid_stream.e_vid_enc = pt_v_ctrl->t_vid_enc;

                    i4_ret = x_svctx_select_mpeg_stream_by_pid_ex(
                        pt_gnrl_info->h_svctx,
                        &t_strm_info,
                        NULL,
                        NULL);

                    if (i4_ret != SVCTXR_OK)
                    {
                        DBG_ERROR(("{MHEG5 Stack} x_svctx_select_mpeg_stream_by_pid() = %d failed.\n", i4_ret));
                    }
                    else
                    {
                        tmMHEG5NotifyVideoStarted();
                        pt_v_ctrl->b_v_pid_ok = TRUE;
                        if (pt_v_ctrl->b_v_user)
                        {
                            if (pt_v_ctrl->t_vs_ori_pid != pt_v_ctrl->t_vs_pid)
                            {
                                pt_v_ctrl->b_v_pid_updated = TRUE;
                            }
                            _save_current_stream(ST_VIDEO);
                        }
                    }
                }
            }
        }
        else
        {
            DBG_INFO((
                "{MHEG5 Stack} Cannot find the video PID.\r\n"));

            if ((MHEG5_VIDEO_MHEG   == pt_v_ctrl->e_v_ctrl) &&
                (PLAYED_FROM_STREAM == pt_v_ctrl->e_v_play_type))
            {
                _stop_current_stream(ST_VIDEO);
                pt_v_ctrl->b_v_pid_ok = FALSE;
                tmMHEG5NotifyVideoStopped();
            }
        }
    }
    
    return MHEG5R_OK;
}
    
static VOID _get_quarter_transformation(
    tmMHEG5DisplayAspectRatioConversion_t*  pe_decfc,
    MHEG5_WSS_T                             e_wss,
    MHEG5_ASPECT_RATIO_T                    e_src_aspect_ratio)
{
    switch (e_wss)
    {
        case MHEG5_WSS_4_3FF:
            if (MHEG5_ASPECT_RATIO_4_3 == e_src_aspect_ratio)
            {
                *pe_decfc = MHEG5_DISPLAY_AR_CONVERSION_IGNORE;
            }
            else
            {
                *pe_decfc = MHEG5_DISPLAY_AR_CONVERSION_PAN_SCAN;
            }
            break;

        case MHEG5_WSS_16_9FF:
            if (MHEG5_ASPECT_RATIO_4_3 == e_src_aspect_ratio)
            {
                *pe_decfc = MHEG5_DISPLAY_AR_CONVERSION_PILLAR_BOX;
            }
            else
            {
                *pe_decfc = MHEG5_DISPLAY_AR_CONVERSION_IGNORE;
            }
            break;
            
        default:
            break;
    }
}

static VOID _get_receiver_transformation(
    tmMHEG5DisplayAspectRatioConversion_t*  pe_decfc, 
    MHEG5_WSS_T*                            pe_wss,
    UINT8                                   ui1_afd,
    MHEG5_ASPECT_RATIO_T                    e_src_aspect_ratio,
    INT32                                   i4_usr_aspect_mode)
{
    tmMHEG5DisplayAspectRatioConversion_t   e_decfc = MHEG5_DISPLAY_AR_CONVERSION_IGNORE;
    MHEG5_WSS_T                             e_wss   = MHEG5_WSS_4_3FF;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);  

    if (MHEG5_ASPECT_RATIO_4_3 == e_src_aspect_ratio)
    {
        /* 4:3 on 16:9 or 4:3 on 4:3 */
        switch (ui1_afd)
        {
            case 8:
            case 9:
            case 12:
                e_wss = MHEG5_WSS_4_3FF;  /* 0001 */
                break;
            case 10:
            case 14:
            case 15:
                e_wss = MHEG5_WSS_16_9;  /* 1101 */
                break;
            case 11:
                e_wss = MHEG5_WSS_14_9;  /* 1000 */
                break;
            case 13:
                e_wss = MHEG5_WSS_4_3_SP_14_9;  /* 0111 */
                break;
        }
    }
    else
    {
        if (MHEG5_DISPLAY_ASPECT_RATIO_4x3 == pt_dis_cfg->e_display_ar)
        {
            /* 16:9 on 4:3 */
            switch (ui1_afd)
            {
                case 8:
                case 12:
                case 15:
                    if (ASPECT_MODE_USER_LETTER_BOX == i4_usr_aspect_mode)
                    {
                        /* Letterbox */
                        e_wss   = MHEG5_WSS_16_9;  /* 1101 */
                        e_decfc = MHEG5_DISPLAY_AR_CONVERSION_LETTER_BOX;
                    }
                    else
                    {
                        e_wss   = MHEG5_WSS_4_3FF;  /* 0001 */
                        e_decfc = MHEG5_DISPLAY_AR_CONVERSION_PAN_SCAN;
                    }
                    break;

                case 9:
                case 13:
                    e_wss   = MHEG5_WSS_4_3FF;  /* 0001 */
                    e_decfc = MHEG5_DISPLAY_AR_CONVERSION_PAN_SCAN;
                    break;

                case 10:
                    e_wss   = MHEG5_WSS_16_9;  /* 1101 */
                    e_decfc = MHEG5_DISPLAY_AR_CONVERSION_LETTER_BOX;
                    break;

                case 11:
                    if (ASPECT_MODE_USER_LETTER_BOX == i4_usr_aspect_mode)
                    {
                        e_wss   = MHEG5_WSS_14_9;  /* 1000 */
                        e_decfc = MHEG5_DISPLAY_AR_CONVERSION_LETTER_BOX_14_9;
                    }
                    else
                    {
                        e_wss   = MHEG5_WSS_4_3FF;  /* 0001 */
                        e_decfc = MHEG5_DISPLAY_AR_CONVERSION_PAN_SCAN;
                    }
                    break;

                case 14:
                    if (ASPECT_MODE_USER_LETTER_BOX == i4_usr_aspect_mode)
                    {
                        /* Letterbox */
                        e_wss   = MHEG5_WSS_14_9;  /* 1000 */
                        e_decfc = MHEG5_DISPLAY_AR_CONVERSION_LETTER_BOX_14_9;
                    }
                    else
                    {
                        e_wss   = MHEG5_WSS_4_3FF;  /* 0001 */
                        e_decfc = MHEG5_DISPLAY_AR_CONVERSION_PAN_SCAN;
                    }
                    break;
            }
        }
        else
        {
            /* 16:9 on 16:9 */
            switch (ui1_afd)
            {
                case 8:
                case 10:
                case 11:
                case 12:
                case 14:
                case 15:
                    e_wss   = MHEG5_WSS_16_9FF;  /* 1110 */
                    e_decfc = MHEG5_DISPLAY_AR_CONVERSION_IGNORE;
                    break;
                case 9:
                    e_wss   = MHEG5_WSS_4_3FF;  /* 0001 */
                    e_decfc = MHEG5_DISPLAY_AR_CONVERSION_PAN_SCAN;
                    break;
                case 13:
                    e_wss   = MHEG5_WSS_4_3_SP_14_9;  /* 0111 */
                    e_decfc = MHEG5_DISPLAY_AR_CONVERSION_PAN_SCAN;
                    break;
            }
        }
    }

    if (pe_decfc)
    {
        *pe_decfc = e_decfc;
    }

    if (pe_wss)
    {
        *pe_wss = e_wss;
    }
}

static BOOL _get_transformation(
    tmMHEG5DisplayAspectRatioConversion_t*  pe_decfc, 
    MHEG5_WSS_T*                            pe_wss,
    UINT8                                   ui1_afd,
    MHEG5_ASPECT_RATIO_T                    e_src_aspect_ratio)
{
    #define QUARTER_VIDEO_WIDTH     (MHEG5_SCREEN_WIDTH  / 2)
    #define QUARTER_VIDEO_HEIGHT    (MHEG5_SCREEN_HEIGHT / 2)
    
    BOOL    b_video    = FALSE;
    BOOL    b_quarter  = FALSE;
    BOOL    b_wss_ok   = TRUE;
    BOOL    b_decfc_ok = TRUE;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_v_ctrl);
    MHEG5_ASSERT(NULL != pt_dis_cfg);   

    b_video = (pt_v_ctrl->e_v_play_type == PLAYED_FROM_STREAM) &&
              (MHEG5_VIDEO_NONE != pt_v_ctrl->e_v_ctrl) &&
              (MHEG5_ASPECT_RATIO_UNKNOWN != e_src_aspect_ratio);

    /* derive WSS & DecFC */
    if (pt_gnrl_info->b_mheg5_running)
    {
        b_quarter = (RECT_W(&pt_dis_cfg->t_rc_video) == QUARTER_VIDEO_WIDTH) &&
                    (RECT_H(&pt_dis_cfg->t_rc_video) == QUARTER_VIDEO_HEIGHT);
    
        switch (pt_dis_cfg->e_scene_ar)
        {
            case MHEG5_ASPECT_RATIO_16x9:
                *pe_wss = MHEG5_WSS_16_9FF;
                break;

            case MHEG5_ASPECT_RATIO_4x3:
                *pe_wss = MHEG5_WSS_4_3FF;
                break;

            case MHEG5_ASPECT_RATIO_UNSEPCIFIED:
                if (!b_video || b_quarter)
                {
                    if (MHEG5_DISPLAY_ASPECT_RATIO_16x9 == pt_dis_cfg->e_display_ar)
                    {
                        *pe_wss = MHEG5_WSS_16_9FF;
                    }
                    else /* MHEG5_DISPLAY_ASPECT_RATIO_4x3 == _e_display_ar */
                    {
                        *pe_wss = MHEG5_WSS_4_3FF;
                    }
                }
                else
                {
                    /* need to apply receiver default */
                    b_wss_ok = FALSE;
                }
                
                break;
            
            default:
                DBG_ERROR(("{MHEG5 Stack} Invalid scene aspect ratio!\r\n"));
                DBG_ABORT(DBG_MOD_MHEG_5);
                return FALSE;
        }

        /* derive DecFC */
        if (b_video)
        {
            if (b_quarter)
            {
                _get_quarter_transformation(
                    pe_decfc,
                    *pe_wss,
                    e_src_aspect_ratio);
            }
            else
            {
                if (MHEG5_ASPECT_RATIO_UNSEPCIFIED == pt_dis_cfg->e_scene_ar)
                {
                    b_decfc_ok = FALSE;
                }
                else
                {
                    *pe_decfc = MHEG5_DISPLAY_AR_CONVERSION_IGNORE;

                    if ((MHEG5_ASPECT_RATIO_4x3  == pt_dis_cfg->e_scene_ar) &&
                        (MHEG5_ASPECT_RATIO_16_9 == e_src_aspect_ratio))
                    {
                        if (MHEG5_ALIGNMENT_CENTRE_CUT_OUT == pt_dis_cfg->e_wam)
                        {
                            *pe_decfc = MHEG5_DISPLAY_AR_CONVERSION_PAN_SCAN;
                        }
                        else
                        {
                            *pe_decfc = MHEG5_DISPLAY_AR_CONVERSION_LETTER_BOX;
                        }
                    }
                }
            }
        }
        else
        {
            *pe_decfc = MHEG5_DISPLAY_AR_CONVERSION_IGNORE;
        }
    }
    else
    {
        b_wss_ok   = FALSE;
        b_decfc_ok = FALSE;
    }

    if (!b_decfc_ok || !b_wss_ok)
    {
        _get_receiver_transformation(
            (b_decfc_ok ? NULL : pe_decfc), 
            (b_wss_ok ? NULL : pe_wss),
            ui1_afd,
            e_src_aspect_ratio,
            ASPECT_MODE_USER_LETTER_BOX);
    #if 0
        DBG_INFO(("{MHEG5 Stack} Get transformation (receiver default):\r\n"));
        DBG_INFO((
            "              MHEG-5 = %d, video = %d, q_video = %d\r\n"
            "              AFD = %d, scene_asp = %d, src_asp = %d, decfc = %d, wss = %d\r\n",
            _b_mheg5_running,
            b_video,
            b_quarter,
            ui1_afd,
            _e_scene_ar,
            e_src_aspect_ratio,
            *pe_decfc,
            *pe_wss));
    #endif
    }
    else
    {
    #if 0
        DBG_INFO(("{MHEG5 Stack} Get transformation (with MHEG-5):\r\n"));
        DBG_INFO((
            "              MHEG-5 = %d, video = %d, q_video = %d\r\n"
            "              AFD = %d, scene_asp = %d, src_asp = %d, decfc = %d, wss = %d\r\n",
            _b_mheg5_running,
            b_video,
            b_quarter,
            ui1_afd,
            _e_scene_ar,
            e_src_aspect_ratio,
            *pe_decfc,
            *pe_wss));
    #endif
    }

    return TRUE;
}


static VOID _iframe_lock(VOID)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);
    
    x_sema_lock(pt_v_ctrl->t_v_dec_info.h_lock, X_SEMA_OPTION_WAIT);
}

static INT32 _iframe_nfy_fct(
    HANDLE_T                h_video,
    SCC_VID_IFRAME_COND_T   e_nfy_cond,
    VOID                	*pv_nfy_tag,
    UINT32                  ui4_data_1,
    UINT32                  ui4_data_2)
{
    _DECODE_INFO_T* pt_dec_info = (_DECODE_INFO_T*) pv_nfy_tag;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    /* play and stop */
    if (e_nfy_cond == SCC_VID_IFRAME_COND_CTRL_DONE)
    {
        switch (ui4_data_1)
        {
            /* play */
            case SCC_VID_IFRAME_COND_BEGIN_CTRL_DONE:
                DBG_INFO(("{MHEG5 Stack} IFrame callback notify: SCC_VID_IFRAME_COND_BEGIN_CTRL_DONE\r\n"));
                pt_dec_info->b_err = FALSE;
                x_sema_unlock(pt_dec_info->h_lock);
                break;
            /* stop */
            case SCC_VID_IFRAME_COND_END_CTRL_DONE:
                DBG_INFO(("{MHEG5 Stack} IFrame callback notify: SCC_VID_IFRAME_COND_END_CTRL_DONE\r\n"));
                pt_v_ctrl->b_iframe_enabled= FALSE;
                pt_dec_info->b_err = FALSE;
                x_sema_unlock(pt_dec_info->h_lock);
                break;
            /* reset */
            case SCC_VID_IFRAME_COND_RESET_CTRL_DONE:
                DBG_INFO(("{MHEG5 Stack} IFrame callback notify: SCC_VID_IFRAME_COND_RESET_CTRL_DONE\r\n"));
                break;
            default:
                DBG_INFO(("{MHEG5 Stack} IFrame callback notify: unrecognized code\r\n"));
                return MHEG5R_INTERNAL_ERROR;
        }
    }
    /* decoded || set buffer */
    else if (
        (e_nfy_cond == SCC_VID_IFRAME_COND_DECODE_DONE) ||
        (e_nfy_cond == SCC_VID_IFRAME_COND_SET_BUFF_DONE))
    {
        if (e_nfy_cond == SCC_VID_IFRAME_COND_DECODE_DONE)
        {
            MHEG5_VIDEO_UPDATE_NFY_T    t_nfy;

            t_nfy.ui4_nfy_flag   = MHEG5_VIDEO_UPDATE_PARAM_IFRAME;
            t_nfy.e_iframe_state = MHEG5_IFRAME_PLAY;

            _sct_notify_video_update(&t_nfy);
            
            DBG_INFO(("{MHEG5 Stack} IFrame callback notify: SCC_VID_IFRAME_COND_DECODE_DONE\r\n"));
        }
        else
        {
            DBG_ERROR(("{MHEG5 Stack} IFrame callback notify: NOT SCC_VID_IFRAME_COND_SET_BUFF_DONE\r\n"));
        }
        
        pt_dec_info->b_err = FALSE;
        x_sema_unlock(pt_dec_info->h_lock);
    }
    else /* SCC_VID_IFRAME_COND_ERROR */
    {
        DBG_ERROR(("{MHEG5 Stack} ERROR! I-Frame decoding failed (in callback)!\r\n"));
        pt_dec_info->b_err = TRUE;
        x_sema_unlock(pt_dec_info->h_lock);
        
        return MHEG5R_INTERNAL_ERROR;
    }
    
    return MHEG5R_OK;
}

static BOOL _iframe_notify_err(VOID)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);
    
    return (pt_v_ctrl->t_v_dec_info.b_err);
}

static INT32 _iframe_close(BOOL b_sync)
{
    INT32 i4_ret;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    if (pt_v_ctrl->b_iframe_enabled)
    {
        if ((NULL == pt_v_ctrl->t_iframe.puc_iframe_buff) ||
            (SMR_OK == x_scc_vid_iframe_free_buffer(pt_v_ctrl->h_video, &pt_v_ctrl->t_iframe)))
        {
            pt_v_ctrl->t_iframe.puc_iframe_buff = NULL;
            pt_v_ctrl->t_iframe.z_iframe_size   = 0;

            if (!pt_v_ctrl->b_iframe_began)
            {
                if (SMR_OK == x_scc_vid_set_iframe_ctrl(pt_v_ctrl->h_video, SCC_VID_IFRAME_CTRL_END))
                {
                    if (b_sync)
                    {
                        _iframe_lock(); /* wait for the close to complete */
                        DBG_INFO(("{MHEG5 Stack} %s: Acquired STOP lock...\r\n", __FUNCTION__));
                    }
                    i4_ret = MHEG5R_OK;
                }
                else
                {
                    i4_ret = MHEG5R_INTERNAL_ERROR;
                    DBG_ERROR(("{MHEG5 Stack} %s: x_scc_vid_set_iframe_ctrl() failed.\n", __FUNCTION__));
                }
            }
            else
            {
                i4_ret = MHEG5R_OK;
                pt_v_ctrl->b_iframe_enabled   = FALSE;
                pt_v_ctrl->t_v_dec_info.b_err = FALSE;
            }
        }
        else
        {
            i4_ret = MHEG5R_INTERNAL_ERROR;
            DBG_ERROR(("{MHEG5 Stack} Freeing I-Frame buffer failed!\r\n"));
        }
    }
    else
    {
        i4_ret = MHEG5R_INV_STATE;
    }

    return (i4_ret);
}

static VOID _iframe_unlock(VOID)
{
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_v_ctrl);

    x_sema_unlock(pt_v_ctrl->t_v_dec_info.h_lock);
}

static INT32 _init_dec_info(volatile _DECODE_INFO_T* pt_dec_info)
{
    INT32 i4_ret;
    
    i4_ret = x_sema_create((HANDLE_T*) &(pt_dec_info->h_lock), X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);

    if (i4_ret == OSR_OK)
    {
        pt_dec_info->b_err = FALSE;
        i4_ret = MHEG5R_OK;
    }
    else
    {
        pt_dec_info->h_lock = NULL_HANDLE;
        DBG_ERROR(("{MHEG5 Stack} x_sema_create() = %d failed.\n", i4_ret));
        i4_ret = MHEG5R_OUT_OF_RESOURCES;
    }

    return (i4_ret);
}

static BOOL _in_mheg_context(VOID)
{
    OS_THREAD_T     *pt_thread = NULL;
    HANDLE_T        h_thread = NULL_HANDLE;

    if (x_thread_self(&h_thread) == OSR_OK)
    {
        if (handle_get_obj(h_thread, (VOID **)&pt_thread) == HR_OK)
        {
            if (pt_thread != NULL)
            {
                /* MHEG engine sends commands when it's in pause mode */
                if (x_strcmp(pt_thread->s_name, "MHEG") == 0)
                {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

static INT32 _is_fullscreen_cif(VOID)
{
    INT32 i4_ratio = 0;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

    SCT_GET_DISPLAY_CFG(pt_dis_cfg);

    MHEG5_ASSERT(NULL != pt_dis_cfg);
    
    if (pt_dis_cfg->ui4_real_vh <= VIDEO_CIF_HEIGHT && pt_dis_cfg->ui4_real_vw <= VIDEO_CIF_WIDTH)
    {
        INT32 i4_w = RECT_W(&pt_dis_cfg->t_rc_video);
        INT32 i4_h = RECT_H(&pt_dis_cfg->t_rc_video);
        INT32 i4_rw = 0;
        INT32 i4_rh = 0;
        
        if ((pt_dis_cfg->ui4_real_vw > 0) && 
            (pt_dis_cfg->ui4_real_vh > 0) &&
            ((MHEG5_SCREEN_WIDTH  == i4_w && MHEG5_SCREEN_HEIGHT == i4_h) ||
             (MHEG5_SCREEN_WIDTH  == (i4_w << 1) && MHEG5_SCREEN_HEIGHT == (i4_h << 1))))
        {
            i4_rw = i4_w / pt_dis_cfg->ui4_real_vw;
            i4_rh = i4_h / pt_dis_cfg->ui4_real_vh;

            i4_ratio = ((i4_rw == i4_rh) ? i4_rw : 0);
        }
    }

    return (i4_ratio);
}

static BOOL _is_video_mode_supported(MHEG5_VIDEO_MODE_T e_vid_mode)
{
    switch (e_vid_mode)
    {
        case MHEG5_VIDEO_MODE_AUTO:
        case MHEG5_VIDEO_MODE_FULLSCREEN:
        case MHEG5_VIDEO_MODE_FORCE_4x3:
        case MHEG5_VIDEO_MODE_GEN_LETTER_BOX:
            return TRUE;
        default:
            break;
    }

    return FALSE;
}

static VOID _lock_a_v_sync(UINT32 ui4_option)
{
    HANDLE_T    h_lock = NULL_HANDLE;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_v_ctrl);

    x_timer_start(
        (ui4_option == AUDIO_SYNC)? (pt_a_ctrl->h_a_sync_timer): (pt_v_ctrl->h_v_sync_timer),
        A_V_SYNC_TIMEOUT,
        X_TIMER_FLAG_ONCE,
        _sync_a_v_timer_cb,
        (VOID *)ui4_option);

    _lock_sync_count();

    if (ui4_option == AUDIO_SYNC)
    {
        h_lock = pt_a_ctrl->h_a_sync_lock;
        pt_a_ctrl->ui4_a_sync_count += 1;
        pt_a_ctrl->b_a_sync_timeout = FALSE;
    }
    else if (ui4_option == VIDEO_SYNC)
    {
        h_lock = pt_v_ctrl->h_v_sync_lock;
        pt_v_ctrl->ui4_v_sync_count += 1;
        pt_v_ctrl->b_v_sync_timeout = FALSE;
    }

    _unlock_sync_count();
    
    x_sema_lock(h_lock, X_SEMA_OPTION_WAIT);
}

static VOID _lock_sync_count(VOID)
{
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);

    MHEG5_ASSERT(NULL != pt_gnrl_info);

    x_sema_lock(pt_gnrl_info->h_sync_count_lock, X_SEMA_OPTION_WAIT);
}

static VOID _mheg5_resolve_nfy(
     BOOL            b_found,
     MPEG_2_PID_T    t_pid, 
     MPEG_2_PID_T    t_pcr_pid,
     UINT8           ui1_enc_type,
     VOID*           pv_tag)
{
    ((STREAM_PID_INFO_T *) pv_tag)->t_pid     = t_pid;
    ((STREAM_PID_INFO_T *) pv_tag)->t_pcr_pid = t_pcr_pid;
    ((STREAM_PID_INFO_T *) pv_tag)->ui1_enc_type = ui1_enc_type;
    ((STREAM_PID_INFO_T *) pv_tag)->b_found   = b_found;
    ((STREAM_PID_INFO_T *) pv_tag)->b_waiting = FALSE;
}

static VOID _save_current_stream(STREAM_TYPE_T e_type)
{
    INT32 i4_ret;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);

    MHEG5_ASSERT(NULL != pt_gnrl_info);

    if (ST_VIDEO == e_type)
    {
        SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
        SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;

        SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
        SCT_GET_DISPLAY_CFG(pt_dis_cfg);
        
        MHEG5_ASSERT(NULL != pt_v_ctrl);
        MHEG5_ASSERT(NULL != pt_dis_cfg);  

        if ((PLAYED_FROM_STREAM == pt_v_ctrl->e_v_play_type) && 
            ((MHEG5_VIDEO_USER == pt_v_ctrl->e_v_ctrl) || 
             ((pt_v_ctrl->e_v_ctrl == MHEG5_VIDEO_MHEG) && (pt_v_ctrl->i4_v_comp_tag == MHEG5_DEF_COMP_TAG) && (pt_gnrl_info->ui2_launch_svc_id == pt_v_ctrl->ui2_v_svc_id))))
        {
            i4_ret = x_svctx_get_using_strm_data(
                pt_gnrl_info->h_svctx,
                ST_VIDEO,
                &pt_v_ctrl->t_v_comp_id,
                &pt_v_ctrl->t_v_scdb_rec);

            pt_v_ctrl->b_v_user = (SVCTXR_OK == i4_ret);

        #ifdef MHEG5_KEEP_SYS_VIDEO_MODE
            pt_dis_cfg->e_mh5_ori_vid_mode = pt_dis_cfg->e_vid_mode;
            pt_v_ctrl->t_vs_ori_pid = pt_v_ctrl->t_v_scdb_rec.u.t_video_mpeg.ui2_pid;
        #endif
        }
    }
    else if (ST_AUDIO == e_type)
    {
        SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
        SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
        MHEG5_ASSERT(NULL != pt_a_ctrl);
        
        if ((PLAYED_FROM_STREAM == pt_a_ctrl->e_a_play_type) &&
            ((MHEG5_AUDIO_USER == pt_a_ctrl->e_a_ctrl) || 
             ((pt_a_ctrl->e_a_ctrl == MHEG5_AUDIO_MHEG) && (pt_a_ctrl->i4_a_comp_tag == MHEG5_DEF_COMP_TAG) && (pt_gnrl_info->ui2_launch_svc_id == pt_a_ctrl->ui2_a_svc_id))))
        {
            i4_ret = x_svctx_get_using_strm_data(
                pt_gnrl_info->h_svctx,
                ST_AUDIO,
                &pt_a_ctrl->t_a_comp_id,
                &pt_a_ctrl->t_a_scdb_rec);

            pt_a_ctrl->b_a_user = (SVCTXR_OK == i4_ret);
        }
    }
}

static VOID _scale_to_video_region(VSH_REGION_INFO_T* pt_region)
{
    pt_region->ui4_x      = (pt_region->ui4_x * VSH_REGION_MAX_WIDTH + MAX_REGION_WIDTH)  / MAX_REGION_WIDTH;
    pt_region->ui4_y      = (pt_region->ui4_y * VSH_REGION_MAX_HEIGHT + MAX_REGION_HEIGHT) / MAX_REGION_HEIGHT;
    pt_region->ui4_width  = (pt_region->ui4_width  * VSH_REGION_MAX_WIDTH)  / MAX_REGION_WIDTH;
    pt_region->ui4_height = (pt_region->ui4_height * VSH_REGION_MAX_HEIGHT) / MAX_REGION_HEIGHT;

    _clip_region(pt_region, VSH_REGION_MAX_WIDTH, VSH_REGION_MAX_HEIGHT);
}

static VOID _sct_post_video_adjustment(
    const MHEG5_DISPLAY_PARAM_T*    pt_dspy_param,
    VSH_REGION_INFO_T*              pt_src_region, 
    VSH_REGION_INFO_T*              pt_dst_region)
{
    if (pt_dspy_param->ui2_video_width  == 352 &&
        pt_dspy_param->ui2_video_height == 288 &&
        pt_src_region->ui4_x      == 1                      &&
        pt_src_region->ui4_y      == 1                      &&
        pt_src_region->ui4_width  == (MAX_REGION_WIDTH - 1) &&
        pt_src_region->ui4_height == (MAX_REGION_HEIGHT - 1))
    {
        pt_src_region->ui4_x      = 0;
        pt_src_region->ui4_y      = 0;
        pt_src_region->ui4_width  = MAX_REGION_WIDTH;
        pt_src_region->ui4_height = MAX_REGION_HEIGHT;
    }
}

static VOID _select_stream_by_pid_nfy(
    HANDLE_T            h_svctx,
    SVCTX_COND_T        e_nfy_cond,
    SVCTX_NTFY_CODE_T   e_code,
    STREAM_TYPE_T       e_stream_type,
    VOID*               pv_nfy_tag)
{
    MHEG5_MSG_T         t_msg;

    t_msg.ui1_module   = MHEG5_MOD_SCT;
    t_msg.ui2_msg_type = SCT_MSG_A_V_SYNC_UNLOCK;
    
    if (e_code == SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE)
    {
        t_msg.pv_nfy_tag = (VOID *)AUDIO_SYNC;
        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }
    else if (e_code == SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE)
    {
        t_msg.pv_nfy_tag = (VOID *)VIDEO_SYNC;
        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }
}

/* Return "14:9 LETTER-BOX" transformation */
static VOID _set_14x9_letterbox_transformation(
    const GL_RECT_T*    pt_video,
    GL_RECT_T*          pt_src,
    GL_RECT_T*          pt_dst)
{
    pt_src->i4_left   = (INT32)(MAX_REGION_WIDTH / 16);
    pt_src->i4_top    = 0;
    pt_src->i4_right  = (INT32)(MAX_REGION_WIDTH * 15 / 16);
    pt_src->i4_bottom = (INT32)MAX_REGION_HEIGHT;

    pt_dst->i4_left   = pt_video->i4_left;
    pt_dst->i4_top    = pt_video->i4_top + RECT_H(pt_video) / 14;
    pt_dst->i4_right  = pt_video->i4_right;
    pt_dst->i4_bottom = pt_video->i4_bottom - RECT_H(pt_video) / 14;
}

static VOID _set_av_sync_data(BOOL b_auto)
{
    SM_SESS_GNRC_AV_SYNC_INFO_T     t_av_sync;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;
    
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);
    
    MHEG5_ASSERT(NULL != pt_v_ctrl);

    t_av_sync.e_av_sync_type    = SM_SESS_GNRC_AV_SYNC_FORCE_PCR_PID;
    t_av_sync.u.t_force_pcr_pid = pt_v_ctrl->t_v_pcr_pid;

    if (b_auto == TRUE)
    {
        t_av_sync.e_av_sync_type = SM_SESS_GNRC_AV_SYNC_AUTO;
    }

    x_svctx_set_session_attr(
        SCT_GET_SVCTX_HANDLE(),
        ST_VIDEO,
        SM_SESS_GNRC_SET_TYPE_AV_SYNC,
        (VOID *)&t_av_sync,
        sizeof(SM_SESS_GNRC_AV_SYNC_INFO_T));
}

/* Return "CENTRE-CUT-OUT" transformation */
static VOID _set_center_cut_out_transformation(
    const GL_RECT_T*    pt_video,
    GL_RECT_T*          pt_src,
    GL_RECT_T*          pt_dst)
{
    pt_src->i4_left   = (INT32)(MAX_REGION_WIDTH / 8);
    pt_src->i4_top    = 0;
    pt_src->i4_right  = (INT32)(MAX_REGION_WIDTH * 7 / 8);
    pt_src->i4_bottom = (INT32)MAX_REGION_HEIGHT;

    *pt_dst = *pt_video;
}

static INT32 _sct_get_full_screen_reg(
    VSH_REGION_INFO_T*          pt_full_src_reg,
    VSH_REGION_INFO_T*          pt_full_dst_reg)
{            
    size_t                  z_len = 0;
    INT32                   i4_ret = 0;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    
    SCT_GET_GNRL_INFO(pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_gnrl_info);
    
    z_len = sizeof(VSH_REGION_INFO_T);
    
    i4_ret = x_svctx_get_stream_attr(pt_gnrl_info->h_svctx,
                             ST_VIDEO,
                             SM_VSH_GET_TYPE_VIDEO_REGION,
                             pt_full_src_reg,
                             &z_len);
    if(SVCTXR_OK != i4_ret)
    {
        return MHEG5R_INTERNAL_ERROR;
    }
    
    i4_ret =  x_svctx_get_stream_attr(pt_gnrl_info->h_svctx,
                     ST_VIDEO,
                     SM_VSH_GET_TYPE_DISP_REGION,
                     pt_full_dst_reg,
                     &z_len);
    
    if(SVCTXR_OK != i4_ret)
    {
        return MHEG5R_INTERNAL_ERROR;
    }
    
    return MHEG5R_OK;
}

/* Return "IGNORE" transformation */
static VOID _set_ignore_transformation(
    const GL_RECT_T*    pt_video,
    GL_RECT_T*          pt_src,
    GL_RECT_T*          pt_dst)
{
    pt_src->i4_left   = 0;
    pt_src->i4_top    = 0;
    pt_src->i4_right  = (INT32)MAX_REGION_WIDTH;
    pt_src->i4_bottom = (INT32)MAX_REGION_HEIGHT;
   
    *pt_dst = *pt_video;
}

static VOID _set_internal_screen_mode(BOOL b_internal_screen_mode)
{
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;
    
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);
    
    MHEG5_ASSERT(NULL != pt_dis_cfg);  

    if (pt_dis_cfg->b_scr_mode_by_mheg5 != b_internal_screen_mode)
    {
        MHEG5_VIDEO_UPDATE_NFY_T    t_data;
        MHEG5_MSG_T         t_msg;

        pt_dis_cfg->b_scr_mode_by_mheg5 = b_internal_screen_mode;
        
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = SCT_MSG_SET_SVCTX_SCR_MODE;
        
        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
                    
        t_data.ui4_nfy_flag         = MHEG5_VIDEO_UPDATE_PARAM_SCR_MDOE;
        t_data.b_internal_scrn_mode = b_internal_screen_mode;
    
        _sct_notify_video_update(&t_data);
    }
}

/* Return "LETTER-BOX" transformation */
static VOID _set_letterbox_transformation(
    const GL_RECT_T*    pt_video,
    GL_RECT_T*          pt_src,
    GL_RECT_T*          pt_dst)
{
    pt_src->i4_left   = 0;
    pt_src->i4_top    = 0;
    pt_src->i4_right  = (INT32)MAX_REGION_WIDTH;
    pt_src->i4_bottom = (INT32)MAX_REGION_HEIGHT;

    pt_dst->i4_left   = pt_video->i4_left;
    pt_dst->i4_top    = pt_video->i4_top + RECT_H(pt_video) / 8;
    pt_dst->i4_right  = pt_video->i4_right;
    pt_dst->i4_bottom = pt_video->i4_bottom - RECT_H(pt_video) / 8;
}

/* Return "PILLAR-BOX" transformation */
static VOID _set_pillarbox_transformation(
    const GL_RECT_T*    pt_video,
    GL_RECT_T*          pt_src,
    GL_RECT_T*          pt_dst)
{
    pt_src->i4_left   = 0;
    pt_src->i4_top    = 0;
    pt_src->i4_right  = (INT32)MAX_REGION_WIDTH;
    pt_src->i4_bottom = (INT32)MAX_REGION_HEIGHT;

    pt_dst->i4_left   = pt_video->i4_left + RECT_W(pt_video) / 8;
    pt_dst->i4_top    = pt_video->i4_top;
    pt_dst->i4_right  = pt_video->i4_right - RECT_W(pt_video) / 8;
    pt_dst->i4_bottom = pt_video->i4_bottom;
}

static VOID _set_stream_select_block(MPEG_2_PID_T *pt_pid, BOOL b_force_lock)
{
    INT32               i4_ret;
    BOOL                b_block = FALSE;
    HANDLE_T            h_scdb;
    UINT32              ui4_db_version;
    UINT16              ui2_i;
    STREAM_COMP_ID_T    t_stream_desc;
    SCDB_REC_T          t_scdb_rec;

    if (pt_pid != NULL)
    {
        BOOL            b_found = FALSE;
        
        i4_ret = x_svctx_open_scdb(SCT_GET_SVCTX_HANDLE(), ST_AUDIO, &h_scdb, NULL, NULL);

        if (i4_ret != SVCTXR_OK)
        {
            return;
        }

        ui2_i          = 0;
        ui4_db_version = SCDB_NULL_VER_ID;

        x_memset(&t_stream_desc, 0, sizeof(STREAM_COMP_ID_T));

        /* enumerate all scdb rec in this service */
        while (1)
        {
            i4_ret = x_scdb_get_rec_by_idx(
                h_scdb,
                ST_AUDIO,
                ui2_i,
                &t_stream_desc,
                &t_scdb_rec,
                &ui4_db_version);

            ui2_i++;

            if (i4_ret == SCDBR_REC_NOT_FOUND)
            {
                break;
            }
            else if (i4_ret == SCDBR_DB_MODIFIED)
            {
                ui2_i          = 0;
                ui4_db_version = SCDB_NULL_VER_ID;
            }
            else if (i4_ret == SCDBR_OK)
            {
                if (t_stream_desc.e_type == ST_AUDIO && t_scdb_rec.u.t_audio_mpeg.ui2_pid == *pt_pid)
                {
                    b_found = TRUE;
                    break;
                }
            }
            else
            {
                b_found = TRUE;
                break;
            }
        }

        /* if the specified audio PID is not in current service, block the stream selection */;
        if (!b_found)
        {
            b_block = TRUE;
        }
    }

    if (pt_pid == NULL && b_force_lock)
    {
        b_block = TRUE;
    }
                                
    x_svctx_set(
        SCT_GET_SVCTX_HANDLE(), 
        SVCTX_COMMON_SET_STREAM_SELECT_BLOCK,
        &b_block,
        sizeof(b_block));

    if (pt_pid != NULL)
    {
        x_svctx_close_scdb(h_scdb);
    }
}

static VOID _set_vdp_mode(SCC_VID_MODE_T e_mode)
{
    INT32               i4_ret;
    SIZE_T              z_ori_size = sizeof(SCC_VID_MODE_T);
    SCC_VID_MODE_T      e_ori_mode;
    SCT_DISPLAY_CFG_T*      pt_dis_cfg = NULL;
    
    SCT_GET_DISPLAY_CFG(pt_dis_cfg);
    
    MHEG5_ASSERT(NULL != pt_dis_cfg);
    
    /* [DTV00138132] Display transient when enter Multimedia source in Mheg 5 page. */
    if (e_mode == SCC_VID_NORMAL)
    {
        x_thread_delay(100);
    }

    i4_ret = x_svctx_get(SCT_GET_SVCTX_HANDLE(), SVCTX_COMMON_GET_DEFAULT_VIDEO_MODE, (VOID *)&e_ori_mode, &z_ori_size);

    if (i4_ret == SMR_OK)
    {
        if (e_mode != SCC_VID_NORMAL || e_ori_mode == SCC_VID_BLANK)
        {
            /* NOTE: Using a SVCTX API in this function might cause a DEAD LOCK since 
               _nav_mheg5_canvas_callback() would call this API in WM thread. However,
               _nav_mheg5_canvas_callback() only happens with e_mode == SCC_VID_BLANK,
               so we only call SVCTX API in the case where e_mode != SCC_VID_BLANK.
             */
            if (e_mode != SCC_VID_BLANK)
            {
                INT32               i4_res;
                SIZE_T              z_size = sizeof(SVC_BLOCK_COND_T);
                SVC_BLOCK_COND_T    e_cond;

                i4_res = x_svctx_get(SCT_GET_SVCTX_HANDLE(), SVCTX_COMMON_GET_TYPE_BLOCK_COND, (VOID *)&e_cond, &z_size);

                /* do nothing when this service is locked */
                if (i4_res == SVCTXR_OK)
                {
                    if (e_cond == SVC_BLOCK_COND_EXCEED_RATING ||
                        e_cond == SVC_BLOCK_COND_USER_BLOCK_CH ||
                        e_cond == SVC_BLOCK_COND_USER_BLOCK_INP)
                    {
                        return;
                    }
                }
                else
                {
                    DBG_ERROR(("{MHEG5 Stack} get SVCTX BLOCK COND failed = %d\n", i4_res));
                }
            }

            if (x_svctx_set(SCT_GET_SVCTX_HANDLE(), SVCTX_COMMON_SET_DEFAULT_VIDEO_MODE, (VOID *)&e_mode, sizeof(SCC_VID_MODE_T)) == SVCTXR_OK)
            {
                pt_dis_cfg->b_vdp_mode_by_mheg = (e_mode != SCC_VID_NORMAL);
            }
        }
    }
}

static VOID _sync_a_v_timer_cb(HANDLE_T h_handle, VOID *pv_tag)
{
    MHEG5_MSG_T         t_msg;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_v_ctrl);

    t_msg.ui1_module   = MHEG5_MOD_SCT;
    t_msg.ui2_msg_type = SCT_MSG_A_V_SYNC_UNLOCK;

    if (h_handle == pt_a_ctrl->h_a_sync_timer && (UINT32)pv_tag == AUDIO_SYNC)
    {
        t_msg.pv_nfy_tag = (VOID *)(AUDIO_SYNC | SCT_UNLOCK_FROM_TIMEOUT);
        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }
    else if (h_handle == pt_v_ctrl->h_v_sync_timer && (UINT32)pv_tag == VIDEO_SYNC)
    {
        t_msg.pv_nfy_tag = (VOID *)(VIDEO_SYNC | SCT_UNLOCK_FROM_TIMEOUT);
        x_mheg5_send_msg(SCT_GET_ENGINE_CODE(), &t_msg, MHEG5_SCT_MSG_SEND_RETRY);
    }
}

static VOID _unlock_a_v_sync(UINT32 ui4_option)
{
    BOOL        *pb_timeout = NULL;
    INT32       i4_ret;
    HANDLE_T    h_lock = NULL_HANDLE;
    SCT_AUDIO_CTRL_CTX_T*   pt_a_ctrl = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl = NULL;

    SCT_GET_AUDIO_CTRL_CTX(pt_a_ctrl);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_a_ctrl);
    MHEG5_ASSERT(NULL != pt_v_ctrl);

    _lock_sync_count();

    if ((ui4_option & ~SCT_UNLOCK_FROM_TIMEOUT) == AUDIO_SYNC)
    {
        h_lock     = pt_a_ctrl->h_a_sync_lock;
        pb_timeout = &pt_a_ctrl->b_a_sync_timeout;

        /* do nothing since there is no one locking this lock */
        if (pt_a_ctrl->ui4_a_sync_count == 1)
        {
            _unlock_sync_count();
            return;
        }
        else if (pt_a_ctrl->ui4_a_sync_count == 0)
        {
            DBG_ABORT(DBG_MOD_MHEG_5);
        }
        else
        {
            pt_a_ctrl->ui4_a_sync_count -= 1;
        }
    }
    else if ((ui4_option & ~SCT_UNLOCK_FROM_TIMEOUT) == VIDEO_SYNC)
    {
        h_lock     = pt_v_ctrl->h_v_sync_lock;
        pb_timeout = &pt_v_ctrl->b_v_sync_timeout;

        /* do nothing since there is no one locking this lock */
        if (pt_v_ctrl->ui4_v_sync_count == 1)
        {
            _unlock_sync_count();
            return;
        }
        else if (pt_v_ctrl->ui4_v_sync_count == 0)
        {
            DBG_ABORT(DBG_MOD_MHEG_5);
        }
        else
        {
            pt_v_ctrl->ui4_v_sync_count -= 1;
        }
    }

    if ((ui4_option & SCT_UNLOCK_FROM_TIMEOUT) && pb_timeout != NULL)
    {
        *pb_timeout = TRUE;
    }

    _unlock_sync_count();
    
    i4_ret = x_sema_unlock(h_lock);

    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("{MHEG5 Stack} unlock_a_v_sync = %d\n", i4_ret));
    }
}

static VOID _unlock_sync_count(VOID)
{
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;

    SCT_GET_GNRL_INFO(pt_gnrl_info);

    MHEG5_ASSERT(NULL != pt_gnrl_info);

    x_sema_unlock(pt_gnrl_info->h_sync_count_lock);
}

static INT32 _stop_current_stream(STREAM_TYPE_T e_strm_type)
{
    INT32 i4_ret = SVCTXR_OK;
    
    if (e_strm_type == ST_AUDIO || e_strm_type == ST_VIDEO)
    {
        BOOL                b_with_av = FALSE;
        SCT_GNRL_INFO_T*    pt_gnrl_info = NULL;

        SCT_GET_GNRL_INFO(pt_gnrl_info);
        MHEG5_ASSERT(NULL != pt_gnrl_info);
    #if 1
        {
            STREAM_COMP_ID_T    t_comp_id;
            SCDB_REC_T          t_scdb;
    
            b_with_av = (x_svctx_get_using_strm_data(pt_gnrl_info->h_svctx, e_strm_type, &t_comp_id, &t_scdb) == SVCTXR_OK);
        }
    #else
        {
            UINT32 ui4_strm_mode;
            SIZE_T z_size;
        
            z_size = sizeof(UINT32);
            i4_ret = x_svctx_get( pt_gnrl_info->h_svctx, 
                                  SVCTX_COMMON_GET_STREAM_MODE,
                                  (VOID *)&ui4_strm_mode,
                                  (VOID *)&z_size);
                                  
            if (i4_ret == SVCTXR_OK)
            {
                if (e_strm_type == ST_AUDIO)
                {
                    b_with_av = (ui4_strm_mode & ST_MASK_AUDIO) ? (TRUE) : (FALSE);
                }
                else
                {
                    b_with_av = (ui4_strm_mode & ST_MASK_VIDEO) ? (TRUE) : (FALSE);
                }
            }
        }
    #endif
        if (b_with_av == TRUE)
        {
            /* For NDT support */
            i4_ret = x_svctx_sync_stop_stream(pt_gnrl_info->h_svctx, e_strm_type, 0);
        }
    }

    return i4_ret;
}

BOOL sct_check_if_main_path_have_video(VOID)
{
    BOOL                    b_with_video = FALSE;
    SCT_GNRL_INFO_T*        pt_gnrl_info = NULL;
    SCT_VIDEO_CTRL_CTX_T*   pt_v_ctrl    = NULL;
	STREAM_COMP_ID_T	    t_comp_id;
	SCDB_REC_T			    t_scdb;

    SCT_GET_GNRL_INFO(pt_gnrl_info);
    SCT_GET_VIDEO_CTRL_CTX(pt_v_ctrl);

    MHEG5_ASSERT(NULL != pt_gnrl_info);
    MHEG5_ASSERT(NULL != pt_v_ctrl);

    b_with_video = pt_v_ctrl->b_iframe_set;

	if (!b_with_video)
	{
        b_with_video = (x_svctx_get_using_strm_data(pt_gnrl_info->h_svctx, ST_VIDEO, &t_comp_id, &t_scdb) == SVCTXR_OK);
	}

	return b_with_video;
}

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: _mheg5_ic_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 mheg5_sct_get_dbg_level(VOID)
{
    return _ui2_mheg5_sct_dbg_level;
}

/*-----------------------------------------------------------------------------
 * Name: _mheg5_ic_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 ----------------------------------------------------------------------------*/
BOOL mheg5_sct_set_dbg_level(UINT16 ui2_level)
{

    _ui2_mheg5_sct_dbg_level = ui2_level;
    return TRUE;

}
#endif

