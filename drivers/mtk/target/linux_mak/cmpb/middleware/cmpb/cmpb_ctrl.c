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
 * $RCSfile: cmpb_ctrl.c,v $
 * $Revision: #20 $
 * $Date: 2013/01/04 $
 * $Author: pan.huang $
 * $SWAuthor:  $
 * $MD5HEX: d908c026a37c5be69fa4ecb1520bc593 $
 *
 * Description:
 *         This is the source file for common playback adapter
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include <stdbool.h>
#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"
#include "svctx/u_svctx.h"
#include "svctx/x_svctx.h"
#include "svctx/c_svctx.h"
#include "mutil/minfo/u_minfo.h"
#include "svctx/mm_hdlr/u_mm_hdlr.h"

#include "cmpb_util.h"
#include "IMtkPb_ErrorCode.h"
#include "IMtkPb_Ctrl.h"
#include "IMtkPb_Ctrl_DTV.h"
#include "IMtkPb_Ctrl_Ext.h"
#include "IMtkPb_Misc.h"

#include "handle/c_handle.h"
#include "handle/x_handle.h"
#include "mutil/minfo/c_minfo.h"
#include "os/inc/os.h"
#include "os/inc/c_os.h"
#include "os/inc/x_os.h"
#include "unicode/c_uc_str.h"
#include "strm_mngr/scc/c_scc.h"
#include "strm_mngr/scc/x_scc.h"
#include "conn_mngr/cm_playback/u_playback_handler.h"

#include "cap/x_cap.h"
#include "cap/c_cap.h"

#ifdef LINUX_TURNKEY_SOLUTION
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#endif
#include <stdio.h>

/*-----------------------------------------------------------------------------
 * macros, typedefs, enums
 *---------------------------------------------------------------------------*/
#define BUF_THRESHOLD_LOW           50
#define BUF_THRESHOLD_HIGH          20
#define MAX_CMPB_INST_NUM           4
#define CMPB_HANDLE_TAG             0x65430000
#define INVALIDE_CMPB_HANDLE        (IMTK_PB_HANDLE_T)0xFFFFFFFF

#define MAX_SET_AUD_NUM             1
#define MAX_GET_AUD_NUM             8

#define MAX_SET_VID_NUM             1
#define MAX_GET_VID_NUM             1

#define MAX_GET_SUB_NUM             8

#define MAX_CMD_CNT                 15

#define CHECK_CMD_IDX(idx)     \
if(idx >= MAX_CMD_CNT)         \
{                              \
    break;                     \
}

#define CMPB_SVCTX_NAME_MAIN                ("main_svctx")
#define CMPB_SVCTX_NAME_SUB                 ("sub_svctx")
#define CMPB_SVCTX_NAME_THIRD               ("thrd_svctx")
#define CMPB_SVCTX_NAME_TV_BYPASS           ("tv_bypass_svctx")
#define CMPB_SVCTX_NAME_MONITOR_BYPASS      ("mn_bypass_svctx")

#define SUPPORT_UTF8_METAINFO       1

#ifdef LINUX_TURNKEY_SOLUTION

#define CMPB_HANDLE_VALID           c_handle_valid
#define CMPB_SEMA_CREATE            c_sema_create
#define CMPB_SEMA_DELETE            c_sema_delete
#define CMPB_SEMA_LOCK              c_sema_lock
#define CMPB_SEMA_UNLOCK            c_sema_unlock
#define CMPB_UC_W2S_STRNCPY         c_uc_w2s_strncpy
#define CMPB_MEM_FREE               c_mem_free
#define CMPB_MEM_CALLOC             c_mem_calloc
#define CMPB_MEM_PART_ALLOC         c_mem_part_alloc
#define CMPB_MEM_PART_CREATE        c_mem_part_create
#define CMPB_MEM_PART_DELETE        c_mem_part_delete
#define CMPB_MEMSET                 c_memset
#define CMPB_MEMCPY                 c_memcpy
#define CMPB_STRNCPY                c_strncpy
#define CMPB_STRCMP                 c_strcmp
#define CMPB_SPRINTF                c_sprintf
#define CMPB_STRLEN                 c_strlen

#define CMPB_MSG_Q_CREATE           x_msg_q_create
#define CMPB_MSG_Q_DELETE           x_msg_q_delete
#define CMPB_MSG_Q_SEND             x_msg_q_send
#define CMPB_MSG_Q_RECEIVE          x_msg_q_receive
#define CMPB_THREAD_CREATE          x_thread_create
#define CMPB_THREAD_EXIT            x_thread_exit
#define CMPB_THREAD_DELAY           c_thread_delay
#define CMPB_TIMER_CREATE           x_timer_create
#define CMPB_TIMER_DELETE           x_timer_delete
#define CMPB_TIMER_START            x_timer_start
#define CMPB_TIMER_STOP             x_timer_stop

#define CMPB_SVCTX_OPEN             c_svctx_open
#define CMPB_SVCTX_CLOSE            c_svctx_close
#define CMPB_SVCTX_SET              c_svctx_set
#define CMPB_SVCTX_GET              c_svctx_get
#define CMPB_SVCTX_SELECT_SVC       c_svctx_select_svc
#define CMPB_SVCTX_SYNC_STOP_SVC    c_svctx_sync_stop_svc
#define CMPB_SVCTX_SELECT_STREAM    c_svctx_select_stream
#define CMPB_SVCTX_STOP_STREAM      c_svctx_stop_stream
#define CMPB_SVCTX_SYNC_STOP_STREAM         c_svctx_sync_stop_stream
#define CMPB_SVCTX_GET_GET_STREAM_DATA      c_svctx_get_using_strm_data
#define CMPB_SVCTX_SET_STREAM_ATTR  c_svctx_set_stream_attr
#define CMPB_SVCTX_SET_SESSION_ATTR c_svctx_set_session_attr
#define CMPB_SVCTX_MEDIA_OPEN       c_svctx_media_open
#define CMPB_SVCTX_MEDIA_CLOSE      c_svctx_media_close
#define CMPB_SVCTX_MEDIA_SEEK       c_svctx_media_seek
#define CMPB_SVCTX_MEDIA_SET_INFO   c_svctx_media_set_info
#define CMPB_SVCTX_MEDIA_GET_INFO   c_svctx_media_get_info
#define CMPB_SVCTX_GET_STREAM_COMP_ID   c_svctx_get_stream_comp_id
#define CMPB_SVCTX_GENERIC_SET_INFO     c_svctx_generic_set_info
#define CMPB_SVCTX_GENERIC_GET_INFO     c_svctx_generic_get_info
#define CMPB_SVCTX_MEDIA_LOAD_THUMBNAIL     c_svctx_media_load_thumbnail

#define CMPB_SVCTX_GET_VIDEO_SRC_RESOLUTION     c_svctx_get_video_src_resolution

#define CMPB_C_SCC_COMP_OPEN                c_scc_comp_open
#define CMPB_C_SCC_COMP_CLOSE               c_scc_comp_close
#define CMPB_C_SCC_COMP_GRP_SET             c_scc_comp_grp_set
#define CMPB_C_SCC_COMP_AUD_SET_CLIP        c_scc_aud_set_clip
#define CMPB_C_SCC_COMP_AUD_SET_CLIP_CTRL   c_scc_aud_set_clip_ctrl
#define CMPB_C_SCC_VID_SET_SUPER_FREEZE     c_scc_vid_set_super_freeze
#define CMPB_C_SCC_COMP_GET_AUD_BUF_INFO    c_scc_aud_get_upload_data_buffer_info
#define CMPB_C_SCC_GET_VOLUME               c_scc_aud_get_volume
#define CMPB_C_SCC_VID_SET_SRM_MODE         c_scc_vid_set_srm_mode
#define CMPB_C_SCC_VID_SET_BG               c_scc_vid_set_bg
#define CMPB_C_SCC_VID_GET_BG               c_scc_vid_get_bg

#ifdef MW_CAP_LOGO_SUPPORT
#define CMPB_CAP_OPEN                       c_cap_open
#define CMPB_CAP_CAPTURE                    c_cap_capture
#define CMPB_CAP_GET_CAPTURE_DATA           c_cap_get_capture_data
#define CMPB_CAP_CLOSE                      c_cap_close
#define CMPB_CAP_SAVE                       c_cap_save
#define CMPB_CAP_SYNC_STOP_CAP              c_cap_sync_stop_capture
#define CMPB_CAP_SYNC_STOP_SAVE             c_cap_sync_stop_save
#define CMPB_CAP_SELECT_AS_BOOTLOGO         c_cap_select_as_bootlogo
#define CMPB_CAP_QUERY_CAPABILITY           c_cap_query_capabilty
#endif

#define CMPB_CVT_UTF16_TO_UTF8              x_uc_w2s_to_ps
#else

#define CMPB_HANDLE_VALID           x_handle_valid
#define CMPB_SEMA_CREATE            x_sema_create
#define CMPB_SEMA_DELETE            x_sema_delete
#define CMPB_SEMA_LOCK              x_sema_lock
#define CMPB_SEMA_UNLOCK            x_sema_unlock
#define CMPB_UC_W2S_STRNCPY         x_uc_w2s_strncpy
#define CMPB_MEM_FREE               x_mem_free
#define CMPB_MEM_CALLOC             x_mem_calloc
#define CMPB_MEM_PART_ALLOC         x_mem_part_alloc
#define CMPB_MEM_PART_CREATE        x_mem_part_create
#define CMPB_MEM_PART_DELETE        x_mem_part_delete
#define CMPB_MEMSET                 x_memset
#define CMPB_MEMCPY                 x_memcpy
#define CMPB_STRNCPY                x_strncpy
#define CMPB_STRCMP                 x_strcmp
#define CMPB_SPRINTF                x_sprintf
#define CMPB_STRLEN                 x_strlen

#define CMPB_MSG_Q_CREATE           x_msg_q_create
#define CMPB_MSG_Q_DELETE           x_msg_q_delete
#define CMPB_MSG_Q_SEND             x_msg_q_send
#define CMPB_MSG_Q_RECEIVE          x_msg_q_receive
#define CMPB_THREAD_CREATE          x_thread_create
#define CMPB_THREAD_EXIT            x_thread_exit
#define CMPB_THREAD_DELAY           x_thread_delay
#define CMPB_TIMER_CREATE           x_timer_create
#define CMPB_TIMER_DELETE           x_timer_delete
#define CMPB_TIMER_START            x_timer_start
#define CMPB_TIMER_STOP             x_timer_stop

#define CMPB_SVCTX_OPEN             x_svctx_open
#define CMPB_SVCTX_CLOSE            x_svctx_close
#define CMPB_SVCTX_SET              x_svctx_set
#define CMPB_SVCTX_GET              x_svctx_get
#define CMPB_SVCTX_SELECT_SVC       x_svctx_select_svc
#define CMPB_SVCTX_SYNC_STOP_SVC    x_svctx_sync_stop_svc
#define CMPB_SVCTX_SELECT_STREAM    x_svctx_select_stream
#define CMPB_SVCTX_STOP_STREAM      x_svctx_stop_stream
#define CMPB_SVCTX_SYNC_STOP_STREAM         x_svctx_sync_stop_stream
#define CMPB_SVCTX_GET_GET_STREAM_DATA      x_svctx_get_using_strm_data
#define CMPB_SVCTX_SET_STREAM_ATTR  x_svctx_set_stream_attr
#define CMPB_SVCTX_SET_SESSION_ATTR x_svctx_set_session_attr
#define CMPB_SVCTX_MEDIA_OPEN       x_svctx_media_open
#define CMPB_SVCTX_MEDIA_CLOSE      x_svctx_media_close
#define CMPB_SVCTX_MEDIA_SEEK       x_svctx_media_seek
#define CMPB_SVCTX_MEDIA_SET_INFO   x_svctx_media_set_info
#define CMPB_SVCTX_MEDIA_GET_INFO   x_svctx_media_get_info
#define CMPB_SVCTX_GET_STREAM_COMP_ID   x_svctx_get_stream_comp_id
#define CMPB_SVCTX_GENERIC_SET_INFO     x_svctx_generic_set_info
#define CMPB_SVCTX_GENERIC_GET_INFO     x_svctx_generic_get_info
#define CMPB_SVCTX_MEDIA_LOAD_THUMBNAIL     x_svctx_media_load_thumbnail

#define CMPB_SVCTX_GET_VIDEO_SRC_RESOLUTION     x_svctx_get_video_src_resolution

#define CMPB_C_SCC_COMP_OPEN                x_scc_comp_open
#define CMPB_C_SCC_COMP_CLOSE               x_scc_comp_close
#define CMPB_C_SCC_COMP_GRP_SET             x_scc_comp_grp_set
#define CMPB_C_SCC_COMP_AUD_SET_CLIP        x_scc_aud_set_clip
#define CMPB_C_SCC_COMP_AUD_SET_CLIP_CTRL   x_scc_aud_set_clip_ctrl
#define CMPB_C_SCC_VID_SET_SUPER_FREEZE     x_scc_vid_set_super_freeze
#define CMPB_C_SCC_COMP_GET_AUD_BUF_INFO    x_scc_aud_get_upload_data_buffer_info
#define CMPB_C_SCC_GET_VOLUME               x_scc_aud_get_volume
#define CMPB_C_SCC_VID_SET_SRM_MODE         x_scc_vid_set_srm_mode
#define CMPB_C_SCC_VID_SET_BG               x_scc_vid_set_bg
#define CMPB_C_SCC_VID_GET_BG               x_scc_vid_get_bg


#ifdef MW_CAP_LOGO_SUPPORT
#define CMPB_CAP_OPEN                       x_cap_open
#define CMPB_CAP_CAPTURE                    x_cap_capture
#define CMPB_CAP_GET_CAPTURE_DATA           x_cap_get_capture_data
#define CMPB_CAP_CLOSE                      x_cap_close
#define CMPB_CAP_SAVE                       x_cap_save
#define CMPB_CAP_SYNC_STOP_CAP              x_cap_sync_stop_capture
#define CMPB_CAP_SYNC_STOP_SAVE             x_cap_sync_stop_save
#define CMPB_CAP_SELECT_AS_BOOTLOGO         x_cap_select_as_bootlogo
#define CMPB_CAP_QUERY_CAPABILITY           x_cap_query_capabilty
#endif

#define CMPB_CVT_UTF16_TO_UTF8              x_uc_w2s_to_ps


#endif

/*! @enum   CMPB_CTRL_STATE_T 
 *  @brief  playback state 
 *   
 */
typedef enum
{
    CMPB_CTRL_UNKNOWN        =   0, ///< Unknown state            
    CMPB_CTRL_CLOSED,               ///< Closed state               
    CMPB_CTRL_TO_SETINFO,           ///< Opened state
    CMPB_CTRL_SETINFO_OK,           ///< Set Info OK State
    CMPB_CTRL_OPENED,               ///< Opened state         
    CMPB_CTRL_READY,                ///< Ready state
    CMPB_CTRL_PRELOADING,           ///< Preloading state
    CMPB_CTRL_PLAYING,              ///< Playing state               
    CMPB_CTRL_PAUSED,               ///< Paused state              
    CMPB_CTRL_STEP,                 ///< Step state
    CMPB_CTRL_TRICK,                ///< FF or FR
    CMPB_CTRL_STOPPED,              ///< Stopped state               
    CMPB_CTRL_TRANSIT               ///< Transition state               
} CMPB_CTRL_STATE_T;


/*! @struct CMPB_CTRL_SETTING_T 
*  @brief  playback control setting structure
*  
*/ 

typedef enum
{
    IMTK_PB_CTRL_PUSH_EVENT_BUF_CONSUMED            =   1
} IMTK_PB_CTRL_PUSH_EVENT_T;

typedef VOID (*x_playback_push_nfy_fct)(
                                        IMTK_PB_CTRL_PUSH_EVENT_T       e_event,
                                        VOID*                           pv_tag,
                                        UINT32                          ui4_data);

typedef struct 
{
    HANDLE_T                        h_mem_part; /*to be removed*/
    VOID*                           pv_buffer;
    VOID*                           pv_private;    
    VOID*                           pv_private2;    
    SIZE_T                          z_buffer_size;
    
} IMTK_CTRL_PUSH_BUF_INFO_T;

typedef struct
{
    BOOL                                b_iter;
    UINT16                              ui2_aud_num;
    UINT16                              ui2_sel_idx;
    STREAM_COMP_ID_T                    at_comp_id[MAX_GET_AUD_NUM];
    SCDB_REC_T                          at_scdb_rec[MAX_GET_AUD_NUM];
    IMTK_PB_CTRL_GET_AUD_TRACK_INFO_T   at_get_info[MAX_GET_AUD_NUM];        
    char                                at_get_audio_language[MAX_GET_AUD_NUM][4];
} AUDIO_INFO_T;

typedef struct 
{
    HANDLE_T                        hHandle;
    UINT32                          ui4_svc_id;   /*reserved for client process id assignment*/
    UINT64                          ui8_preroll;
    x_playback_push_nfy_fct         pf_nfy;
} IMTK_CTRL_PUSH_INFO_T;

#define SUB_LANG_LEN    5
typedef struct
{
    BOOL                        b_iter;
    UINT16                      ui2_sub_num;
    UINT16                      ui2_sel_idx;
    STREAM_COMP_ID_T            at_comp_id[MAX_GET_SUB_NUM];
    CHAR                        as_lang[MAX_GET_SUB_NUM][SUB_LANG_LEN];
} SUBTITLE_INFO_T;



typedef struct
{
    IMTK_PB_HANDLE_T                        h_cmpb;
    HANDLE_T                                h_sema;
    IMTK_PB_CTRL_BUFFERING_MODEL_T          e_model;
    BOOL                                    b_app_set_media_info;
    BOOL                                    b_media_opened;
    BOOL                                    b_set_engparm_async;
    HANDLE_T                                h_svctx_hdl;
    HANDLE_T                                h_media_hdl;
    HANDLE_T                                h_time_code_update;
    CMPB_CTRL_STATE_T                       e_old_state;
    CMPB_CTRL_STATE_T                       e_cur_state;
    CMPB_CTRL_STATE_T                       e_new_state;
    IMTK_PB_CTRL_SPEED_T                    e_cur_speed;
    BOOL                                    b_preload_to_play;
    BOOL                                    b_parse_done;
    BOOL                                    b_open_media_error;
    BOOL                                    b_start_parse;
    BOOL                                    b_closing;
    UINT16                                  u2_cur_aud_trk;
    UINT32                                  ui4_play_flag;
    UINT32                                  ui4_av_info;
    UINT32                                  ui4_av_update;
    INT32                                   i4_push_buf_cnt;
    
    HANDLE_T                                h_uri_sema;
    HANDLE_T                                h_uri_file;
    
    union
    {
        IMTK_PB_CTRL_URI_INFO_T             t_uri;
        IMTK_PB_CTRL_PULL_INFO_T            t_pull;
        IMTK_CTRL_PUSH_INFO_T               t_push;
    } u_info;

    BOOL                                    b_media_info_is_set;
    struct
    {
        IMTK_PB_CTRL_SET_MEDIA_INFO_T       t_set_info;
        IMTK_PB_CTRL_GET_MEDIA_INFO_T       t_get_info;
    } u_media;
    IMTK_PB_CTRL_SET_MEDIA_INFO_T           t_push_strm_info;/*Just for push app master*/

    /* ------------ Video Stream Info----------- */
    BOOL                                    b_vid_track_is_set;
    uint16_t                                u2VideoTrackNum;
    union
    {
        IMTK_PB_CTRL_GET_VID_TRACK_INFO_T   t_get_info[MAX_GET_VID_NUM];
    } u_vid_track;
    uint16_t                                ui2_pvr_video_pid;

    /* ------------ Audio Stream Info----------- */
    BOOL                                    b_aud_track_is_set;
    uint16_t                                u2AudioTrackNum;  
    AUDIO_INFO_T                            t_audio_info;

    uint16_t                                ui2_pvr_audio_pid;

    /* ------------ Subtitle Stream Info----------- */
    BOOL                                    b_sbtl_track_is_set;
    uint16_t                                u2SubtitleTrackNum;
    IMTK_PB_CTRL_SUBTITLE_INFO_T            at_sub_tracks[MAX_GET_SUB_NUM];
    
    /* ------------ All Stream Info----------- */
    MM_STRM_INFO_T                          t_strm_info;

    BOOL                                    b_need_set_rgn;
    VSH_REGION_INFO_T                       t_vide_rgn;
    VSH_REGION_INFO_T                       t_disp_rgn;
    
    IMtkPb_Ctrl_Nfy_Fct                     pf_notify;          ///< application register callback function
    VOID*                                   pv_tag;
    UINT32                                  ui4_data;
    UINT32                                  ui4_last_timecode;
    BOOL                                    fg_buf_rdy;
    BOOL                                    fg_play_normal;
    BOOL                                    fg_during_seek;
    BOOL                                    fg_eos;
    IMTK_CTRL_PUSH_BUF_INFO_T               t_push_buf;
    UINT32                                  ui4_push_buf_high;
    UINT32                                  ui4_push_buf_low;
    UINT32                                  ui4_push_buf_level;
    IMTK_PB_CTRL_PUSH_BUF_STATUS_T          e_buf_status;
#ifdef MM_SEND_BUFFER_PREPLAY_SUPPORT
    IMTK_PB_CTRL_PRESTORE_DATA_T*           pt_prestore_data_header;  
    IMTK_PB_CTRL_PRESTORE_DATA_T*           pt_prestore_data_footer; 
#endif        
    UINT8                                   ui1_group_mode;
    SRC_SUB_TYPE_T                          t_src_sub_type;
    
    SUBTITLE_INFO_T                         t_subtitle_info;
    IMTK_PB_CTRL_SET_EXTRA_INFO_T           t_ext_info;
    BOOL                                    b_audio_changing;
    BOOL                                    b_video_changing;
    BOOL                                    b_subtl_changing;

    UINT32                                  ui4_title_idx;
    UINT32                                  ui4_playlist_idx;
    UINT32                                  ui4_chap_idx;
    UINT16                                  ui2_svc_pid;
    UINT16                                  ui2_program_idx;

    UINT32                                  ui4_start_tick;
    UINT32                                  ui4_end_tick;
    UINT32                                  ui4_total_dur;

    IMTK_PB_CTRL_DRM_TYPE_T                 eDRMType;
    HANDLE_T                                h_gl_plane;

    IMTK_PB_CTRL_DRM_INFO_T                 t_drm_info;
    IMTK_PB_THUMBNAIL_RESULT_T              e_thumbnail_result;
    IMTK_PB_CTRL_PROFILE_TYPE_T             e_cmpb_profile;
    char*                                   ac_url_list[IMTK_PB_CTRL_URL_TYPE_MAX];

    UINT8*                                  pui1_proxy_url; 
    UINT8*                                  pui1_agent_url; 
    BOOL                                    b_monitor;
} CMPB_CTRL_INST_T;

typedef enum
{
    CMPB_CB_MSG_MEDIA_OPEN_NFY = 0,
    CMPB_CB_MSG_MEDIA_PLAY_NFY,
    CMPB_CB_MSG_MEDIA_PUSH_NFY
} CMPB_CB_MSG_TYPE_T;

typedef struct
{
    HANDLE_T                        h_ctx_hdl;
    SVCTX_COND_T                    e_ctx_cond;
    SVCTX_NTFY_CODE_T               e_nfy_code;
    STREAM_TYPE_T                   e_stm_type;
    VOID*                           pv_nfy_tag;
    VOID*                           pv_data;
} CMPB_CB_MSG_MEDIA_OPEN_T;

typedef struct
{
    HANDLE_T                        h_ctx_hdl;
    SVCTX_COND_T                    e_ctx_cond;
    SVCTX_NTFY_CODE_T               e_nfy_code;
    STREAM_TYPE_T                   e_stm_type;
    VOID*                           pv_nfy_tag;
} CMPB_CB_MSG_MEDIA_PLAY_T;

typedef struct
{
    MM_PLAYBACK_CTRL_PUSH_EVENT_T   e_event;
    UINT32                          ui4_data;
} CMPB_CB_MSG_MEDIA_PUSH_T;

typedef struct
{
    CMPB_CTRL_INST_T*               pt_cmpb_inst;
    CMPB_CB_MSG_TYPE_T              e_msg_type;

    union
    {
        CMPB_CB_MSG_MEDIA_OPEN_T            t_media_open_event;
        CMPB_CB_MSG_MEDIA_PLAY_T            t_media_play_event;
        CMPB_CB_MSG_MEDIA_PUSH_T            t_media_push_event;
    } u;
} CMPB_CB_MSG_T;

typedef enum
{
    CMPB_UOP_TYPE_PLAY            =   1,
    CMPB_UOP_TYPE_STOP,
    CMPB_UOP_TYPE_PAUSE,                 
    CMPB_UOP_TYPE_TRICK,                  
    CMPB_UOP_TYPE_SEEK,        
    CMPB_UOP_TYPE_STEP,
    CMPB_UOP_TYPE_AUDIO_CHG,
    CMPB_UOP_TYPE_VIDEO_CHG,
    CMPB_UOP_TYPE_SUBTT_CHG  
    /*to be extend*/
} CMPB_UOP_TYPE_T;

typedef struct
{
    IMtkPb_Ctrl_Ext_Buf_Cb_Fct    pf_upload_data_nfy;
    VOID*                         pv_nfy_tag;
} CMPB_AUD_UPLOAD_DATA_NFT_INFO_T;

typedef struct
{
    VOID*                               pv_muxbuf_phy_addr;
    VOID*                               pv_muxbuf_vir_addr;
    UINT32                              ui4_mux_buf_len;
}T_MUX_INFO_T;

/*-----------------------------------------------------------------------------
 *                   data declarations
 ----------------------------------------------------------------------------*/
static CMPB_CTRL_INST_T*    apt_cmpb_inst[MAX_CMPB_INST_NUM]={NULL};

static UINT32  gui4_get_time_interval = 500; /*ms*/

BOOL g_is_get_info = FALSE;
BOOL g_is_using_info = FALSE;
extern BOOL b_pb_info_ready;

static HANDLE_T h_cmpb_cb_thread = NULL_HANDLE;
static HANDLE_T h_cmpb_cb_msg_que = NULL_HANDLE;
static BOOL b_cb_inited = FALSE;

static HANDLE_T h_main_svctx = NULL_HANDLE;
static HANDLE_T h_sub_svctx = NULL_HANDLE;
static HANDLE_T h_thrd_svctx = NULL_HANDLE;

#ifdef MW_TV_AV_BYPASS_SUPPORT
static HANDLE_T h_tv_bypass_svctx = NULL_HANDLE;
static HANDLE_T h_monitor_bypass_svctx = NULL_HANDLE;
static BOOL b_tv_scart_enabled = FALSE;
static BOOL b_monitor_scart_enabled = FALSE;
#endif
static CMPB_AUD_UPLOAD_DATA_NFT_INFO_T t_aud_upload_nfy_info = {0};
static T_MUX_INFO_T t_mux_info = {0,0,0};


#if SUPPORT_UTF8_METAINFO
#define MAX_METAINFO_LEN    256
static UINT16 g_ui2MetaStr[MAX_METAINFO_LEN] = {0};

#define UTF_IS_SURROGATE(uchar) (((uchar)&0xfffff800)==0xd800)
#define UTF_IS_SURROGATE_FIRST(c) (((c)&0x400)==0)
#define U16_IS_TRAIL(c) (((c)&0xfffffc00)==0xdc00)
#define UTF_IS_TRAIL(uchar) U16_IS_TRAIL(uchar)
#define UTF_SURROGATE_OFFSET ((0xd800<<10UL)+0xdc00-0x10000)
#define UTF16_GET_PAIR_VALUE(first, second) \
    (((first)<<10UL)+(second)-UTF_SURROGATE_OFFSET)

#define UTF8_CHAR_LENGTH(c) \
        ((uint32_t)(c)<=0x7f ? 1 : \
            ((uint32_t)(c)<=0x7ff ? 2 : \
                ((uint32_t)((c)-0x10000)>0xfffff ? 3 : 4) \
            ) \
        )

#define __TERMINATE_STRING(dest, destCapacity, length, pErrorCode)      \
    if(pErrorCode!=NULL && (*pErrorCode != 0)) {                    \
        /* not a public function, so no complete argument checking */   \
                                                                        \
        if(length<0) {                                                  \
            /* assume that the caller handles this */                   \
        } else if(length<destCapacity) {                                \
            /* NUL-terminate the string, the NUL fits */                \
            dest[length]=0;                                             \
            /* unset the not-terminated warning but leave all others */ \
            *pErrorCode=0;                               \
        } else if(length==destCapacity) {                               \
            /* unable to NUL-terminate, but the string itself fit - set a warning code */ \
            *pErrorCode=(-2);                \
        } else /* length>destCapacity */ {                              \
            /* even the string itself did not fit - set an error code */ \
            *pErrorCode=-3;                        \
        }                                                               \
    }


static int32_t 
u_terminateChars(char *dest, int32_t destCapacity, int32_t length, int32_t *pErrorCode) {
    __TERMINATE_STRING(dest, destCapacity, length, pErrorCode);
    return length;
}



static uint8_t *
_appendUTF8(uint8_t *pDest, int32_t c) {
    /* c<=0x7f is handled by the caller, here it is 0x80<=c<=0x10ffff */
    if((c)<=0x7ff) {
        *pDest++=(uint8_t)((c>>6)|0xc0);
        *pDest++=(uint8_t)((c&0x3f)|0x80);
    } else if((uint32_t)(c)<=0xffff) {
        *pDest++=(uint8_t)((c>>12)|0xe0);
        *pDest++=(uint8_t)(((c>>6)&0x3f)|0x80);
        *pDest++=(uint8_t)(((c)&0x3f)|0x80);
    } else /* if((uint32_t)(c)<=0x10ffff) */ {
        *pDest++=(uint8_t)(((c)>>18)|0xf0);
        *pDest++=(uint8_t)((((c)>>12)&0x3f)|0x80);
        *pDest++=(uint8_t)((((c)>>6)&0x3f)|0x80);
        *pDest++=(uint8_t)(((c)&0x3f)|0x80);
    }
    return pDest;
}


static char* 
u_strToUTF8(char *dest,
            int32_t destCapacity,
            int32_t *pDestLength,
            const uint16_t *pSrc,
            int32_t srcLength,
            int32_t *pErrorCode){

    int32_t reqLength=0;
    const uint16_t *pSrcLimit;
    uint32_t ch=0,ch2=0;
    uint8_t *pDest = (uint8_t *)dest;
    uint8_t *pDestLimit = pDest + destCapacity;


    /* args check */
    if(pErrorCode==NULL){
        return NULL;
    }

    if((pSrc==NULL) || (srcLength < -1) || (destCapacity<0) || (!dest && destCapacity > 0)){
        *pErrorCode = -1;
        return NULL;
    }

    if(srcLength==-1) {
        while((ch=*pSrc)!=0 && pDest!=pDestLimit) {
            ++pSrc;
            if(ch <= 0x7f) {
                *pDest++ = (char)ch;
                ++reqLength;
                continue;
            }

            /*need not check for NUL because NUL fails UTF_IS_TRAIL() anyway*/
            if(UTF_IS_SURROGATE(ch)) {
                if(UTF_IS_SURROGATE_FIRST(ch) && UTF_IS_TRAIL(ch2=*pSrc)) {
                    ++pSrc;
                    ch=UTF16_GET_PAIR_VALUE(ch, ch2);
                } else {
                    /* Unicode 3.2 forbids surrogate code points in UTF-8 */
                    *pErrorCode = 1;
                    return NULL;
                }
            }
            reqLength += UTF8_CHAR_LENGTH(ch);
            /* do we have enough room in destination? */
            if(destCapacity< reqLength){
                break;
            }
            /* convert and append*/
            pDest=_appendUTF8(pDest, ch);
        }
        while((ch=*pSrc++)!=0) {
            if(ch<=0x7f) {
                ++reqLength;
            } else if(ch<=0x7ff) {
                reqLength+=2;
            } else if(!UTF_IS_SURROGATE(ch)) {
                reqLength+=3;
            } else if(UTF_IS_SURROGATE_FIRST(ch) && UTF_IS_TRAIL(ch2=*pSrc)) {
                ++pSrc;
                reqLength+=4;
            } else {
                /* Unicode 3.2 forbids surrogate code points in UTF-8 */
                *pErrorCode = 1;
                return NULL;
            }
        }
    } else {
        pSrcLimit = pSrc+srcLength;
        while(pSrc<pSrcLimit && pDest<pDestLimit) {
            ch=*pSrc++;
            if(ch <= 0x7f) {
                *pDest++ = (char)ch;
                ++reqLength;
                continue;
            }

            if(UTF_IS_SURROGATE(ch)) {
                if(UTF_IS_SURROGATE_FIRST(ch) && pSrc<pSrcLimit && UTF_IS_TRAIL(ch2=*pSrc)) {
                    ++pSrc;
                    ch=UTF16_GET_PAIR_VALUE(ch, ch2);
                } else {
                    /* Unicode 3.2 forbids surrogate code points in UTF-8 */
                    *pErrorCode = 1;
                    return NULL;
                }
            }
            reqLength += UTF8_CHAR_LENGTH(ch);
            /* do we have enough room in destination? */
            if(destCapacity< reqLength){
                break;
            }
            /* convert and append*/
            pDest=_appendUTF8(pDest, ch);
        }
        while(pSrc<pSrcLimit) {
            ch=*pSrc++;
            if(ch<=0x7f) {
                ++reqLength;
            } else if(ch<=0x7ff) {
                reqLength+=2;
            } else if(!UTF_IS_SURROGATE(ch)) {
                reqLength+=3;
            } else if(UTF_IS_SURROGATE_FIRST(ch) && pSrc<pSrcLimit && UTF_IS_TRAIL(ch2=*pSrc)) {
                ++pSrc;
                reqLength+=4;
            } else {
                /* Unicode 3.2 forbids surrogate code points in UTF-8 */
                *pErrorCode = 1;
                return NULL;
            }
        }
    }

    if(pDestLength){
        *pDestLength = reqLength;
    }

    /* Terminate the buffer */
    u_terminateChars((char*)dest,destCapacity,reqLength,pErrorCode);

    return (char*)dest;
}

static INT32 x_uc_w2s_to_ps(
    const uint16_t*  w2s_src,
    CHAR*           ps_dst,
    UINT32          ui4_dst_len)
{
    int32_t length;
    int32_t errorCode = 0;

    if( w2s_src == (uint16_t*)NULL ||
        ps_dst  == (CHAR*)NULL )
    {
        return -1;
    }

    errorCode = 0;
    u_strToUTF8(ps_dst, ui4_dst_len, &length, w2s_src, -1, &errorCode);
    if((errorCode < 0))
    {
        return errorCode;
    }

    return 0;
}


#endif

/***************add for android************/
HANDLE_T    h_sema_thumbnail = NULL_HANDLE; //use semaphone to aoid conflict between normal play & thumbnail play

/*-----------------------------------------------------------------------------
 *                   functions declaraions
 ----------------------------------------------------------------------------*/
#define FOURCC(a,b,c,d)  ((((UINT32)(a))<<24)  \
                               | ((UINT32)((b)<<16)) \
                               | ((UINT32)((c)<<8))  \
                               | ((UINT32)(d)))


static INT32 _set_video_freeze(BOOL b_enable);
static IMTK_PB_ERROR_CODE_T _set_thumbnail_mode(BOOL OnOff, BOOL IsMain);
static IMTK_PB_ERROR_CODE_T _cmpb_ctrl_speed_convert(IMTK_PB_CTRL_SPEED_T*    e_cmpb_speed,
                                           MM_SPEED_TYPE_T*         e_mm_speed,
                                           bool                     b_is_cmpb_to_mm);



static UINT16 _cmpb_map_bit_depth(IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_T e_depth)
{
    switch (e_depth)
    {
    case IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_8:
        return 8;

    case IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_16:
        return 16;

    case IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_20:
        return 20;

    case IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_24:
        return 24;

    default:
        return 16;
    }
}

static UINT8 _cmpb_map_channels(IMTK_PB_CTRL_AUD_CH_NUM_T e_channel)
{
    switch (e_channel)
    {
    case IMTK_PB_CTRL_AUD_CH_MONO:
        return 1;

    case IMTK_PB_CTRL_AUD_CH_STEREO:
        return 2;

    case IMTK_PB_CTRL_AUD_CH_3_0:
        return 3;

    case IMTK_PB_CTRL_AUD_CH_4_0:
        return 4;

    case IMTK_PB_CTRL_AUD_CH_5_0:
        return 5;

    case IMTK_PB_CTRL_AUD_CH_5_1:
        return 6; /*???*/

    case IMTK_PB_CTRL_AUD_CH_7_1:
        return 8; /*???*/
        
    default:
        return 2;
    }
}

static IMTK_PB_CTRL_AUD_CH_NUM_T _cmpb_map_echannel(UINT8 ui1_channels)
{
    switch (ui1_channels)
    {
    case 1:
        return IMTK_PB_CTRL_AUD_CH_MONO;

    case 2:
        return IMTK_PB_CTRL_AUD_CH_STEREO;

    case 3:
        return IMTK_PB_CTRL_AUD_CH_3_0;

    case 4:
        return IMTK_PB_CTRL_AUD_CH_4_0;

    case 5:
        return IMTK_PB_CTRL_AUD_CH_5_0;

    case 6:
        return IMTK_PB_CTRL_AUD_CH_5_1; /*???*/

    case 8:
        return IMTK_PB_CTRL_AUD_CH_7_1; /*???*/
        
    default:
        return IMTK_PB_CTRL_AUD_CH_STEREO;
    }
}

static UINT32 _cmpb_map_sample_rate(IMTK_PB_CTRL_AUD_SAMPLE_RATE_T eSampleRate)
{
    switch (eSampleRate)
    {
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_8K:
        return 8000;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_16K:
        return 16000;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_32K:
        return 32000;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_11K:
        return 11025; /*return 11024*/
    
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_22K:
        return 22050; /*return 22255*/

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_44K:
        return 44100;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_12K:
        return 12000;
        
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_24K:
        return 24000;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_48K:
        return 48000;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_96K:
        return 96000;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_192K:
        return 192000;
        
    default:
        return 48000;
    }
}

IMTK_PB_CTRL_AUD_SAMPLE_RATE_T _cmpb_cal_sample_rate(UINT32 ui4_sample_rate)
{
    UINT32 ui4_threshold;
    
    if (ui4_sample_rate<32000)
    {
        ui4_threshold = 10;
    }
    else
    {
        ui4_threshold = 20;
    }

    ui4_sample_rate = ui4_sample_rate/100;

    if (ui4_sample_rate > (80 - ui4_threshold) && ui4_sample_rate < (80 + ui4_threshold))
    {
        return IMTK_PB_CTRL_AUD_SAMPLE_RATE_8K;
    }
    else if (ui4_sample_rate > (110 - ui4_threshold) && ui4_sample_rate < (110 + ui4_threshold))
    {
        return IMTK_PB_CTRL_AUD_SAMPLE_RATE_11K;
    }
    else if (ui4_sample_rate > (120 - ui4_threshold) && ui4_sample_rate < (120 + ui4_threshold))
    {
        return IMTK_PB_CTRL_AUD_SAMPLE_RATE_12K;
    }
    else if (ui4_sample_rate > (160 - ui4_threshold) && ui4_sample_rate < (160 + ui4_threshold))
    {
        return IMTK_PB_CTRL_AUD_SAMPLE_RATE_16K;
    }
    else if (ui4_sample_rate > (220 - ui4_threshold) && ui4_sample_rate < (220 + ui4_threshold))
    {
        return IMTK_PB_CTRL_AUD_SAMPLE_RATE_22K;
    }
    else if (ui4_sample_rate > (240 - ui4_threshold) && ui4_sample_rate < (240 + ui4_threshold))
    {
        return IMTK_PB_CTRL_AUD_SAMPLE_RATE_24K;
    }
    else if (ui4_sample_rate > (320 - ui4_threshold) && ui4_sample_rate < (320 + ui4_threshold))
    {
        return IMTK_PB_CTRL_AUD_SAMPLE_RATE_32K;
    }
    else if (ui4_sample_rate > (441 - ui4_threshold) && ui4_sample_rate < (441 + ui4_threshold))
    {
        return IMTK_PB_CTRL_AUD_SAMPLE_RATE_44K;
    }
    else if (ui4_sample_rate > (480 - ui4_threshold) && ui4_sample_rate < (480 + ui4_threshold))
    {
        return IMTK_PB_CTRL_AUD_SAMPLE_RATE_48K;
    }
    else if (ui4_sample_rate > (960 - ui4_threshold) && ui4_sample_rate < (960 + ui4_threshold))
    {
        return IMTK_PB_CTRL_AUD_SAMPLE_RATE_96K;
    }
    else if (ui4_sample_rate > (1920 - ui4_threshold) && ui4_sample_rate < (1920 + ui4_threshold))
    {
        return IMTK_PB_CTRL_AUD_SAMPLE_RATE_192K;
    }
    else
    {
        return IMTK_PB_CTRL_AUD_SAMPLE_RATE_44K;
    }
}

static VOID _cmpb_ctrl_media_open_nfy_fct(
                    HANDLE_T                        h_ctx_hdl, 
                    SVCTX_COND_T                    e_ctx_cond,
                    SVCTX_NTFY_CODE_T               e_nfy_code,
                    STREAM_TYPE_T                   e_stm_type,
                    VOID*                           pv_nfy_tag,
                    VOID*                           pv_data
                    );

static VOID _cmpb_ctrl_media_play_nfy_fct(
                    HANDLE_T                        h_ctx_hdl, 
                    SVCTX_COND_T                    e_ctx_cond,
                    SVCTX_NTFY_CODE_T               e_nfy_code,
                    STREAM_TYPE_T                   e_stm_type,
                    VOID*                           pv_nfy_tag 
                    );
static VOID _cmpb_ctrl_media_open_nfy_fct_body(
                    HANDLE_T                        h_ctx_hdl, 
                    SVCTX_COND_T                    e_ctx_cond,
                    SVCTX_NTFY_CODE_T               e_nfy_code,
                    STREAM_TYPE_T                   e_stm_type,
                    VOID*                           pv_nfy_tag,
                    VOID*                           pv_data
                    );
static VOID _cmpb_ctrl_media_play_nfy_fct_body(
                    HANDLE_T                        h_ctx_hdl, 
                    SVCTX_COND_T                    e_ctx_cond,
                    SVCTX_NTFY_CODE_T               e_nfy_code,
                    STREAM_TYPE_T                   e_stm_type,
                    VOID*                           pv_nfy_tag 
                    );

static VOID _cmpb_ctrl_push_nfy_fct(
                                MM_PLAYBACK_CTRL_PUSH_EVENT_T   e_event,
                                VOID*                           pv_tag,
                                UINT32                          ui4_data);
static VOID _cmpb_ctrl_push_nfy_fct_body(
                                CMPB_CTRL_INST_T*               pt_cmpb_inst,
                                MM_PLAYBACK_CTRL_PUSH_EVENT_T   e_event,
                                UINT32                          ui4_data);

static IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetCurrentPos_inr(CMPB_CTRL_INST_T* pt_cmpb_inst,
                                                                UINT32*         pu4CurTime,   
                                                                UINT64*         pu8CurPos);

static BOOL _cmpb_audio_strm_comp_iter(
                    UINT16                          ui2_num_recs,     /* in  */
                    UINT16                          ui2_idx,          /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    const SCDB_REC_T*               pt_scdb_rec,      /* in  */
                    VOID*                           pv_fltr_tag       /* in  */
                    );

static BOOL _cmpb_subtitle_strm_comp_iter(
                    UINT16                          ui2_num_recs,     /* in  */
                    UINT16                          ui2_idx,          /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    const SCDB_REC_T*               pt_scdb_rec,      /* in  */
                    VOID*                           pv_fltr_tag       /* in  */
                    );

static BOOL _cmpb_video_strm_comp_iter(
                    UINT16                          ui2_num_recs,     /* in  */
                    UINT16                          ui2_idx,          /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    const SCDB_REC_T*               pt_scdb_rec,      /* in  */
                    VOID*                           pv_fltr_tag       /* in  */
                    );

/*-----------------------------------------------------------------------------
 *                   Internal functions implementations
 -----------------------------------------------------------------------------*/

static INT32 _cmpb_set_tv_mode(SCC_TV_MODE_T e_mode)
{
    SM_COMMAND_T at_sm_cmds[4];
    
    at_sm_cmds[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data = SCC_CMD_TYPE_DISP;
    at_sm_cmds[1].e_code     = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
    at_sm_cmds[2].e_code     = SCC_CMD_CODE_DISP_TV_MODE;
    at_sm_cmds[2].u.ui4_data = (UINT32)e_mode;
    at_sm_cmds[3].e_code     = SM_CMD_CODE_END;

    return CMPB_C_SCC_COMP_GRP_SET(at_sm_cmds);
}

static VOID _cmpb_cb_send_msg(const CMPB_CB_MSG_T* pt_msg)
{
    INT32               i4_ret; 
    INT32               i4_retry;

    i4_retry = 0;
    while ( i4_retry < 10 )
    {
        i4_retry++;
        i4_ret = CMPB_MSG_Q_SEND( h_cmpb_cb_msg_que,
                               pt_msg,
                               sizeof( CMPB_CB_MSG_T ),
                               0 );
        if ( i4_ret != OSR_OK )
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            CMPB_THREAD_DELAY( 10 );
            continue;
        }

        /* success */
        return;
    }
}

static VOID _cmbp_callback_thread(VOID*   pv_data)
{
    INT32                       i4_ret;
    CMPB_CB_MSG_T               t_msg;
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    SIZE_T                      z_msg_size = sizeof(CMPB_CB_MSG_T);
    UINT16                      ui2_index;

    while (1)
    {
        CMPB_MEMSET( &t_msg, 0, sizeof(CMPB_CB_MSG_T) );

        i4_ret = CMPB_MSG_Q_RECEIVE( &ui2_index,
                                  &t_msg,
                                  &z_msg_size,
                                  &h_cmpb_cb_msg_que,
                                  (UINT16)1,
                                  X_MSGQ_OPTION_WAIT );
        if ( i4_ret == OSR_NO_MSG )
        {
            continue;
        }
        
        if ( i4_ret != OSR_OK )
        {
            //ASSERT(0, ("_cmbp_callback_thread CMPB_MSG_Q_RECEIVE error=%d\n", i4_ret));
            CMPB_THREAD_EXIT();
            return;
        }

        pt_cmpb_inst = t_msg.pt_cmpb_inst;
        if (NULL == pt_cmpb_inst)
        {
            //ASSERT(0, ("_cmbp_callback_thread pt_cmpb_inst=NULL\n"));
            continue;
        }

        switch( t_msg.e_msg_type )
        {
        case CMPB_CB_MSG_MEDIA_OPEN_NFY:
            _cmpb_ctrl_media_open_nfy_fct_body (
                    t_msg.u.t_media_open_event.h_ctx_hdl, 
                    t_msg.u.t_media_open_event.e_ctx_cond,
                    t_msg.u.t_media_open_event.e_nfy_code,
                    t_msg.u.t_media_open_event.e_stm_type,
                    t_msg.u.t_media_open_event.pv_nfy_tag,
                    t_msg.u.t_media_open_event.pv_data);
            break;

        case CMPB_CB_MSG_MEDIA_PLAY_NFY:
            _cmpb_ctrl_media_play_nfy_fct_body(
                    t_msg.u.t_media_play_event.h_ctx_hdl, 
                    t_msg.u.t_media_play_event.e_ctx_cond,
                    t_msg.u.t_media_play_event.e_nfy_code,
                    t_msg.u.t_media_play_event.e_stm_type,
                    t_msg.u.t_media_play_event.pv_nfy_tag );
            break;

        case CMPB_CB_MSG_MEDIA_PUSH_NFY:
            _cmpb_ctrl_push_nfy_fct_body(
                                pt_cmpb_inst,
                                t_msg.u.t_media_push_event.e_event,
                                t_msg.u.t_media_push_event.ui4_data);
            break;

        default:
            //ASSERT(0, ("_cmbp_callback_thread t_msg.e_msg_type=%d\n", t_msg.e_msg_type));
            break;
        }
    }
}

static VOID _cmpb_cb_init(VOID)
{
    INT32   i4_ret;
    
    if (b_cb_inited == TRUE)
    {
        return;
    }

    if (NULL_HANDLE == h_cmpb_cb_msg_que)
    {
        i4_ret = CMPB_MSG_Q_CREATE( &h_cmpb_cb_msg_que,
                                 "cmpb_callback_queue",
                                 sizeof( CMPB_CB_MSG_T ),
                                 200 );
        if ( i4_ret != OSR_OK )
        {
            return;
        }
    }

    if (NULL_HANDLE == h_cmpb_cb_thread)
    {
        i4_ret = CMPB_THREAD_CREATE(&h_cmpb_cb_thread,
                                 "cmpb_callback_thread",
                                 8192,
                                 100,
                                 _cmbp_callback_thread,
                                 sizeof(HANDLE_T *),
                                 (VOID *)&h_cmpb_cb_msg_que);

        if (i4_ret != OSR_OK)
        {
            CMPB_MSG_Q_DELETE(h_cmpb_cb_thread);
            return;
        }
    }
#ifdef ANDROID
    if(NULL_HANDLE == h_sema_thumbnail)
    {
        i4_ret = CMPB_SEMA_CREATE(
                                &(h_sema_thumbnail),
                                X_SEMA_TYPE_BINARY,
                                X_SEMA_STATE_UNLOCK
                                );
        if(OSR_OK != i4_ret)
        {
            //printf("create sema h_sema_thumbnail failed, i4_ret = %d \n", i4_ret);
        }
    }
#endif
}

static VOID _cmpb_free_handle(IMTK_PB_HANDLE_T hHandle)
{
    UINT32 ui4_i=0;
    
    if ((hHandle&0xFFFF0000) == CMPB_HANDLE_TAG)
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i<MAX_CMPB_INST_NUM)
        {
            apt_cmpb_inst[ui4_i] = NULL;
        }
    }
}


static VOID _cmpb_get_new_handle(CMPB_CTRL_INST_T* pt_ctrl_inst, IMTK_PB_HANDLE_T* ph_Handle)
{
    UINT32 ui4_i=0;

    *ph_Handle = INVALIDE_CMPB_HANDLE;
    
    for (;ui4_i < MAX_CMPB_INST_NUM; ui4_i++)
    {
        if (apt_cmpb_inst[ui4_i] == NULL)        
        {
            apt_cmpb_inst[ui4_i] = pt_ctrl_inst;
            *ph_Handle = ui4_i | CMPB_HANDLE_TAG;
            return;
        }
    }       
}


static VOID _cmpb_get_inst_by_handle(CMPB_CTRL_INST_T** ppt_ctrl_inst,IMTK_PB_HANDLE_T h_Handle)
{
    UINT32 ui4_i=0;

    *ppt_ctrl_inst = NULL;
    
    if ( (h_Handle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = h_Handle&0x0000FFFF;
        if (ui4_i < MAX_CMPB_INST_NUM)
        {
            *ppt_ctrl_inst = apt_cmpb_inst[ui4_i];
        }
    }
    
}


static IMTK_PB_CTRL_STATE_T _cmpb_mapping_state(CMPB_CTRL_STATE_T e_state)
{
    IMTK_PB_CTRL_STATE_T e_out_state = IMTK_PB_CTRL_UNKNOWN;

    switch (e_state)
    {
       
        case CMPB_CTRL_CLOSED:
            e_out_state = IMTK_PB_CTRL_CLOSED;
            break;
        case CMPB_CTRL_TO_SETINFO:
            e_out_state = IMTK_PB_CTRL_TO_SETINFO;
            break;
        case CMPB_CTRL_SETINFO_OK:
            break;  
        case CMPB_CTRL_OPENED:
            e_out_state = IMTK_PB_CTRL_OPENED;
            break;
        case CMPB_CTRL_READY:
            e_out_state = IMTK_PB_CTRL_READY;
            break;
        case CMPB_CTRL_PLAYING:
            e_out_state = IMTK_PB_CTRL_PLAYING;
            break;
        case CMPB_CTRL_PAUSED:
            e_out_state = IMTK_PB_CTRL_PAUSED;
            break;
        case CMPB_CTRL_STOPPED:
            e_out_state = IMTK_PB_CTRL_READY;
            break;           
        default :
            break;
    }
    
    return e_out_state;
}

#if 0
static VOID _cmpb_free_set_strm_info(CMPB_CTRL_INST_T *pt_inst)
{

    if (pt_inst)
    {
        if (pt_inst->t_strm_info.pt_stm_atrbt)
        {
            CMPB_MEM_FREE(pt_inst->t_strm_info.pt_stm_atrbt);
            pt_inst->t_strm_info.pt_stm_atrbt = NULL;
            pt_inst->t_strm_info.ui2_num_stm = 0;
        }
    }
}
#endif

static VOID _cmpb_free_instance(CMPB_CTRL_INST_T *pt_inst)
{
    INT32   i4_ret;
    
    if (pt_inst)
    {
#if 0
        _cmpb_free_set_strm_info(pt_inst);
#endif
        if (CMPB_HANDLE_VALID(pt_inst->h_sema))
        {
            i4_ret = CMPB_SEMA_DELETE(pt_inst->h_sema);

            if(OSR_OK != i4_ret)
            {
            }
        }
                
        if (CMPB_HANDLE_VALID(pt_inst->h_uri_sema))
        {
            i4_ret = CMPB_SEMA_DELETE(pt_inst->h_uri_sema);

            if(OSR_OK != i4_ret)
            {
            }
        }
                
        CMPB_MEM_FREE(pt_inst);
    }
}


static  IMTK_PB_CTRL_AUD_ENC_T _cmpb_get_svctx_aud_enc(MINFO_INFO_AUDIO_ENC_T e_enc)
{
    IMTK_PB_CTRL_AUD_ENC_T e_aud_enc = IMTK_PB_CTRL_AUD_ENC_UNKNOWN;

    switch (e_enc)
    {
        case MINFO_INFO_AUD_ENC_MPEG_1:
        case MINFO_INFO_AUD_ENC_MPEG_2:
            e_aud_enc = IMTK_PB_CTRL_AUD_ENC_MPEG;
            break;
            
        case MINFO_INFO_AUD_ENC_AC3:
            e_aud_enc = IMTK_PB_CTRL_AUD_ENC_DD;
            break;
        case MINFO_INFO_AUD_ENC_EC3:
            e_aud_enc = IMTK_PB_CTRL_AUD_ENC_DDP;
            break;
        case MINFO_INFO_AUD_ENC_LPCM:
        case MINFO_INFO_AUD_ENC_PCM:
        case MINFO_INFO_AUD_ENC_ADPCM:
            e_aud_enc = IMTK_PB_CTRL_AUD_ENC_PCM;
            break;

        case MINFO_INFO_AUD_ENC_AAC:
            e_aud_enc = IMTK_PB_CTRL_AUD_ENC_AAC;
            break;
            
        case MINFO_INFO_AUD_ENC_DTS:
            e_aud_enc = IMTK_PB_CTRL_AUD_ENC_DTS;
            break;

        case MINFO_INFO_AUD_ENC_WMA_V1:
        case MINFO_INFO_AUD_ENC_WMA_V2:
        case MINFO_INFO_AUD_ENC_WMA_V3:
            e_aud_enc = IMTK_PB_CTRL_AUD_ENC_WMA;
            break;

        case MINFO_INFO_AUD_ENC_COOK:
            e_aud_enc = IMTK_PB_CTRL_AUD_ENC_COOK;
            break;
            
        default :
            break;
    }
    
    return e_aud_enc;
}


static  IMTK_PB_CTRL_VID_ENC_T _cmpb_get_svctx_vid_enc(MINFO_INFO_VIDEO_ENC_T e_enc)
{
    IMTK_PB_CTRL_VID_ENC_T e_vid_enc = IMTK_PB_CTRL_VID_ENC_UNKNOWN;

    switch (e_enc)
    {
        case MINFO_INFO_VID_ENC_MPEG_1:
        case MINFO_INFO_VID_ENC_MPEG_2:
            e_vid_enc = IMTK_PB_CTRL_VID_ENC_MPEG1_2;
            break;
        case MINFO_INFO_VID_ENC_H263:
            e_vid_enc = IMTK_PB_CTRL_VID_ENC_H263;
            break;
        case MINFO_INFO_VID_ENC_H264:
            e_vid_enc = IMTK_PB_CTRL_VID_ENC_H264;
            break;

        case MINFO_INFO_VID_ENC_DIVX_311:
        case MINFO_INFO_VID_ENC_DIVX_4:
        case MINFO_INFO_VID_ENC_DIVX_5:
            e_vid_enc = IMTK_PB_CTRL_VID_ENC_DIVX311;
            break;

        case MINFO_INFO_VID_ENC_MPEG_4:
			e_vid_enc = IMTK_PB_CTRL_VID_ENC_MPEG4;
            break;
        case MINFO_INFO_VID_ENC_XVID:
            e_vid_enc = IMTK_PB_CTRL_VID_ENC_XVID;
            break;
                    
        case MINFO_INFO_VID_ENC_WMV1:
            e_vid_enc = IMTK_PB_CTRL_VID_ENC_WMV1;
            break;
        case MINFO_INFO_VID_ENC_WMV2:
            e_vid_enc = IMTK_PB_CTRL_VID_ENC_WMV2;
            break;
        case MINFO_INFO_VID_ENC_WMV3:
            e_vid_enc = IMTK_PB_CTRL_VID_ENC_WMV3;
            break;
        case MINFO_INFO_VID_ENC_WVC1:
            e_vid_enc = IMTK_PB_CTRL_VID_ENC_VC1;
            break;

        case MINFO_INFO_VID_ENC_RV8:
            e_vid_enc = IMTK_PB_CTRL_VID_ENC_RV8;
            break;
        case MINFO_INFO_VID_ENC_RV9:
            e_vid_enc = IMTK_PB_CTRL_VID_ENC_RV9_10;
            break;

        case MINFO_INFO_VID_ENC_SORENSON:
            e_vid_enc = IMTK_PB_CTRL_VID_ENC_SORENSON_SPARK;
            break;
            
        default :
            break;
    }
    
    return e_vid_enc;
}

/* Just for push common file use, the follow would be:lib-uri open, get info, close, app-push*/
static IMTK_PB_ERROR_CODE_T _cmpb_fill_push_mode_strm_info( IMTK_PB_HANDLE_T      hHandle,
                                                            MM_STRM_INFO_T*       pt_strm_info)
{
    CMPB_CTRL_INST_T*                   pt_cmpb_inst;
    MM_STRM_ATRBT_T*                    pt_stm_atrbt;
    UINT16                              ui2_i;
    BOOL                                b_aud_filled = FALSE;
    BOOL                                b_vid_filled = FALSE;

    if(pt_strm_info == NULL)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    pt_stm_atrbt = pt_strm_info->at_stm_atrbt;
    pt_cmpb_inst->t_push_strm_info.eMediaType = pt_cmpb_inst->u_media.t_get_info.eMediaType;

    for (ui2_i=0; ui2_i < pt_strm_info->ui2_num_stm; ui2_i++)
    {/* Only Fill first Audio/Video */
        if (ST_AUDIO == pt_stm_atrbt[ui2_i].e_type)
        {
            switch(pt_cmpb_inst->u_media.t_get_info.eMediaType)
            {
            case IMTK_PB_CTRL_MEDIA_TYPE_AVI:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tAviInfo.tAviAudInfo.eAudEnc
                    = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_audio_stm_attr.e_enc);
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tAviInfo.tAviAudInfo.u1StreamIdx
                    = pt_stm_atrbt[ui2_i].t_stm_atrbt.t_stm_id.u.t_avi_stm_id;
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tAviInfo.tAviAudInfo.fgVbr
                    = (pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_audio_stm_attr.ui4_cbr_sub_vbr > 0);
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tAviInfo.tAviAudInfo.u4Scale
                    = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_audio_stm_attr.ui4_scale;
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tAviInfo.tAviAudInfo.u4Rate
                    = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_audio_stm_attr.ui4_rate;
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tAviInfo.tAviAudInfo.u4Bps
                    = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_audio_stm_attr.ui4_avg_bytes_sec;
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_PS:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tPsInfo.eAudInfo.eAudEnc
                    = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ps_audio_stm_attr.e_enc);
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tPsInfo.eAudInfo.tAudStrmIdInfo.u1StrmId
                    = pt_stm_atrbt[ui2_i].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id;
                break;
                
            case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS:
                /* Can't get info in this step*/
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_ASF:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc
                    = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_asf_audio_stm_attr.e_enc);
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tAsfInfo.tAsfAudInfo.u1StreamIdx
                    = pt_stm_atrbt[ui2_i].t_stm_atrbt.t_stm_id.u.t_asf_stm_id;
                /* To be added */
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_MKV:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tMkvInfo.tMkvAudInfo.eAudEnc
                    = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mkv_audio_stm_attr.e_enc);
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_OGG:
                //pt_cmpb_inst->t_push_strm_info.uFormatInfo.tOggInfo.tOggAudInfo.eAudEnc
                //    = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_audio_stm_attr.e_enc);
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_AUDIO_ES:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc
                    = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc);
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_MP4:
                /* Not support push mode*/
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_RM:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tRmInfo.tRmAudInfo.eAudEnc
                    = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_rm_audio_stm_attr.e_enc);
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_FLV:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tFlvInfo.tFlvAudInfo.eAudEnc
                    = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_rm_audio_stm_attr.e_enc);
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tFlvInfo.tFlvAudInfo.u1StreamIdx
                    = pt_stm_atrbt[ui2_i].t_stm_atrbt.t_stm_id.u.t_flv_stm_id;
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tFlvInfo.tFlvAudInfo.fgVbr = FALSE;
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tFlvInfo.tFlvAudInfo.u4Bps
                    = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_flv_audio_stm_attr.ui4_avg_bytes_per_sec;                
                break;
            default:
                break;
            }
            b_aud_filled = TRUE;
        }
        else if(ST_VIDEO == pt_stm_atrbt[ui2_i].e_type)
        {
            switch(pt_cmpb_inst->u_media.t_get_info.eMediaType)
            {
            case IMTK_PB_CTRL_MEDIA_TYPE_AVI:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tAviInfo.tAviVidInfo.eVidEnc
                    = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_video_stm_attr.e_enc);
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_PS:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tPsInfo.eVidInfo.eVidEnc
                    = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ps_video_stm_attr.e_enc);
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS:
                /* Can't get info in this step*/                
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_ASF:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc
                    = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_asf_video_stm_attr.e_enc);
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_MKV:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tMkvInfo.tMkvVidInfo.uVidCodec.eVidEnc
                    = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mkv_video_stm_attr.e_enc);
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_OGG:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tOggInfo.tOggVidInfo.eVidEnc
                    = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_video_stm_attr.e_enc);
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_VIDEO_ES:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tVideoEsInfo.eVidEnc
                    = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_es_video_stm_attr.e_enc);
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_MP4:
                /* Not support push mode*/
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_RM:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tRmInfo.tRmVidInfo.eVidEnc
                    = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_rm_video_stm_attr.e_enc);
                break;
            case IMTK_PB_CTRL_MEDIA_TYPE_FLV:
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tFlvInfo.tFlvVidInfo.eVidEnc
                    = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_rm_video_stm_attr.e_enc);
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tFlvInfo.tFlvVidInfo.u1StreamIdx
                    = pt_stm_atrbt[ui2_i].t_stm_atrbt.t_stm_id.u.t_flv_stm_id;
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tFlvInfo.tFlvVidInfo.u4Scale
                    = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_flv_video_stm_attr.ui4_scale;    
                pt_cmpb_inst->t_push_strm_info.uFormatInfo.tFlvInfo.tFlvVidInfo.u4Rate
                    = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_flv_video_stm_attr.ui4_rate;                 
                break;
            default:
                break;
            }
            b_vid_filled = TRUE;
        }
        if(b_aud_filled && b_vid_filled)
        {
            break;
        }
    }    
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T _cmpb_fill_strm_info(IMTK_PB_HANDLE_T      hHandle)
{
    CMPB_CTRL_INST_T*                   pt_cmpb_inst;
    INT32                               i4_ret;   
    SIZE_T                              z_size;
    MM_STRM_INFO_T                      t_strm_info;
    UINT16                              ui2_svctx_num_stm;
    MM_STRM_ATRBT_T*                    pt_stm_atrbt;
    IMTK_PB_CTRL_GET_VID_TRACK_INFO_T*  pt_vid_track_info;
    IMTK_PB_CTRL_GET_AUD_TRACK_INFO_T*  pt_aud_track_info;
    UINT16                              ui2_i;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    /*  Get Media Info */
    CMPB_MEMSET(&t_strm_info, 0, sizeof(MM_STRM_INFO_T));
    z_size = sizeof(MM_STRM_INFO_T);

    i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                    pt_cmpb_inst->h_svctx_hdl,
                    pt_cmpb_inst->h_media_hdl,
                    SVCTX_MM_GET_TYPE_STRM_INFO,
                    (VOID*)&t_strm_info,
                    z_size
                    );
    if(SVCTXR_OK != i4_ret)
    {
        CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_GET_INFO() returns %d\n", i4_ret));
    }

    pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_UNKNOWN;
    
    switch (t_strm_info.t_format.e_media_type)
    {
        case MEDIA_TYPE_CONTAINER:        
        
            if (MEDIA_CONTNR_SUBTYPE_ASF == t_strm_info.t_format.t_media_subtype.u.e_contnr_subtype)
            {
                pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_ASF;
            }
            else if (MEDIA_CONTNR_SUBTYPE_MP4 == t_strm_info.t_format.t_media_subtype.u.e_contnr_subtype)
            {
                pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_MP4;
            }
            else if (MEDIA_CONTNR_SUBTYPE_AVI == t_strm_info.t_format.t_media_subtype.u.e_contnr_subtype)
            {
                DRM_INFO_SVCTX_T t_drm_info;
                
                z_size = sizeof(DRM_INFO_SVCTX_T);

                CMPB_MEMSET(&t_drm_info, 0, sizeof(t_drm_info));
                i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                pt_cmpb_inst->h_svctx_hdl,
                                pt_cmpb_inst->h_media_hdl,
                                SVCTX_DRM_GET_TYPE_INFO,
                                (VOID*)&t_drm_info,
                                z_size
                                );
                if (SVCTXR_OK == i4_ret)
                {
                    if (DRM_TYPE_DIVX_DRM == t_drm_info.t_drm_type)
                    {
                        pt_cmpb_inst->eDRMType = IMTK_PB_CTRL_DRM_TYPE_DIVX_DRM;
                    }
                }
                pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_AVI;
            }
            else if (MEDIA_CONTNR_SUBTYPE_MATROSKA == t_strm_info.t_format.t_media_subtype.u.e_contnr_subtype)
            {
                pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_MKV;
            }
            else if (MEDIA_CONTNR_SUBTYPE_OGG == t_strm_info.t_format.t_media_subtype.u.e_contnr_subtype)
            {
                pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_OGG;
            }
            else if (MEDIA_CONTNR_SUBTYPE_RM == t_strm_info.t_format.t_media_subtype.u.e_contnr_subtype)
            {
                pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_RM;
            }
            else if (MEDIA_CONTNR_SUBTYPE_FLV == t_strm_info.t_format.t_media_subtype.u.e_contnr_subtype)
            {
                pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_FLV;
            }
            break;
            
        case MEDIA_TYPE_TS:
            pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS;
            break;
            
        case MEDIA_TYPE_PS:
            pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_PS;
            break;
            
        case MEDIA_TYPE_VIDEO_ES:
            pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_VIDEO_ES;
            break;
            
        case MEDIA_TYPE_AUDIO_ES:        
            if (MEDIA_AUD_SUBTYPE_MP3 == t_strm_info.t_format.t_media_subtype.u.e_aud_subtype)
            {
                pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_AUDIO_ES;
            }
            else if (MEDIA_AUD_SUBTYPE_WAVE == t_strm_info.t_format.t_media_subtype.u.e_aud_subtype)
            {
                pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_WAV;
            }
            break;
            
        default:
            break;
            
    } /* switch e_media_type */

    /* Fill Stream Get info for APP-Master PUSH mode USE*/
    _cmpb_fill_push_mode_strm_info(hHandle, &t_strm_info);/* No need check result ??? */
    
    /* get stream num and info */
    ui2_svctx_num_stm = t_strm_info.ui2_num_stm;
    pt_cmpb_inst->u_media.t_get_info.u2AudioTrackNum = 0;
    pt_cmpb_inst->u2VideoTrackNum = 0;
    pt_cmpb_inst->u2SubtitleTrackNum = 0;
    pt_stm_atrbt = t_strm_info.at_stm_atrbt;
    
    for (ui2_i=0; ui2_i < ui2_svctx_num_stm; ui2_i++)
    {
        if ((ST_MM_SUBTITLE == pt_stm_atrbt[ui2_i].e_type) || (ST_SUBTITLE  == pt_stm_atrbt[ui2_i].e_type))
        {
            if(pt_cmpb_inst->u2SubtitleTrackNum >= MAX_GET_SUB_NUM)
            {
                break;
            }
            
            switch (pt_cmpb_inst->u_media.t_get_info.eMediaType)
            {
            case IMTK_PB_CTRL_MEDIA_TYPE_AVI:
                CMPB_STRNCPY((CHAR*)(pt_cmpb_inst->at_sub_tracks[pt_cmpb_inst->u2SubtitleTrackNum].s_lang),
                             pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_sp_stm_attr.s_lang,
                             SUB_LANG_LEN-1);
                break;

            case IMTK_PB_CTRL_MEDIA_TYPE_MKV:
                CMPB_STRNCPY((CHAR*)(pt_cmpb_inst->at_sub_tracks[pt_cmpb_inst->u2SubtitleTrackNum].s_lang),
                             pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mkv_sp_stm_attr.s_lang,
                             SUB_LANG_LEN-1);
                break;
                
            case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS:
                CMPB_STRNCPY((CHAR*)(pt_cmpb_inst->at_sub_tracks[pt_cmpb_inst->u2SubtitleTrackNum].s_lang),
                             pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ts_sp_stm_attr.s_lang,
                             SUB_LANG_LEN-1);
                break;

            default:
                break;
            }
            
            pt_cmpb_inst->u2SubtitleTrackNum++;
        }
        else if (ST_AUDIO == pt_stm_atrbt[ui2_i].e_type)
        {                                        
            if(pt_cmpb_inst->u_media.t_get_info.u2AudioTrackNum >= MAX_GET_AUD_NUM)
            {
                break;
            }
            pt_aud_track_info =  &(pt_cmpb_inst->t_audio_info.at_get_info[pt_cmpb_inst->u_media.t_get_info.u2AudioTrackNum]);
            pt_aud_track_info->eAudEnc = IMTK_PB_CTRL_AUD_ENC_UNKNOWN;
            pt_cmpb_inst->u_media.t_get_info.u2AudioTrackNum++;
            
            switch (pt_cmpb_inst->u_media.t_get_info.eMediaType)
            {                
            case IMTK_PB_CTRL_MEDIA_TYPE_MP4:                      
                pt_aud_track_info->eAudEnc = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp4i_audio_stm_attr.e_enc);                    
                pt_aud_track_info->eChNum = _cmpb_map_echannel(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp4i_audio_stm_attr.i2_channels);                    
                pt_aud_track_info->eSampleRate = _cmpb_cal_sample_rate((UINT32)pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp4i_audio_stm_attr.i4_sample_rate);                    
                pt_aud_track_info->u4BitRate = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp4i_audio_stm_attr.i4_sample_rate * pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp4i_audio_stm_attr.i4_sample_size*8;                    
                CMPB_STRNCPY((CHAR*)(pt_aud_track_info->s_lang), pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp4i_audio_stm_attr.s_lang, ISO_639_LANG_LEN);                                                    
                break;                                    
                    
            case IMTK_PB_CTRL_MEDIA_TYPE_ASF:                     
                pt_aud_track_info->eAudEnc = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_asf_audio_stm_attr.e_enc);                    
                pt_aud_track_info->eChNum = _cmpb_map_echannel(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_channels);                    
                pt_aud_track_info->eSampleRate = _cmpb_cal_sample_rate((UINT32)pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_asf_audio_stm_attr.i4_samples_per_sec);                    
                pt_aud_track_info->u4BitRate = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_asf_audio_stm_attr.ui4_avg_bytes_per_sec*8;                    
                CMPB_STRNCPY((CHAR*)(pt_aud_track_info->s_lang), pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_asf_audio_stm_attr.s_lang, ISO_639_LANG_LEN);                                                    
                break;                                    
                    
            case IMTK_PB_CTRL_MEDIA_TYPE_AVI:
                pt_aud_track_info->eAudEnc = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_audio_stm_attr.e_enc);
                pt_aud_track_info->eChNum = _cmpb_map_echannel(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_audio_stm_attr.i2_channels);                    
                pt_aud_track_info->eSampleRate = _cmpb_cal_sample_rate((UINT32)pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_audio_stm_attr.i4_samples_per_sec);                    
                pt_aud_track_info->u4BitRate = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_audio_stm_attr.ui4_avg_bytes_sec*8;                    
                CMPB_STRNCPY((CHAR*)(pt_aud_track_info->s_lang), pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_audio_stm_attr.s_lang, ISO_639_LANG_LEN);                                                    
                break;                                    
                    
            case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_PS:                    
                pt_aud_track_info->eAudEnc = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ps_audio_stm_attr.e_enc);                    
                pt_aud_track_info->eChNum = _cmpb_map_echannel(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ps_audio_stm_attr.i2_channels);                    
                pt_aud_track_info->eSampleRate = _cmpb_cal_sample_rate((UINT32)pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ps_audio_stm_attr.i4_sampling_frequency);                    
                pt_aud_track_info->u4BitRate = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ps_audio_stm_attr.i4_sampling_frequency * pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ps_audio_stm_attr.i4_samples_per_quant;                    
                CMPB_STRNCPY((CHAR*)(pt_aud_track_info->s_lang), pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ps_audio_stm_attr.s_lang, ISO_639_LANG_LEN);                                                    
                break;                                    
                    
            case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS:                      
                pt_aud_track_info->eAudEnc = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ts_audio_stm_attr.e_enc);                    
                pt_aud_track_info->eChNum = _cmpb_map_echannel(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ts_audio_stm_attr.ui2_channels);                    
                pt_aud_track_info->eSampleRate = _cmpb_cal_sample_rate((UINT32)pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ts_audio_stm_attr.ui4_sample_rate);                    
                pt_aud_track_info->u4BitRate = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ts_audio_stm_attr.ui4_sample_rate
                                                    * pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ts_audio_stm_attr.ui1_bit_depth;
                CMPB_STRNCPY((CHAR*)(pt_aud_track_info->s_lang), pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ts_audio_stm_attr.s_lang, ISO_639_LANG_LEN);
                break;                
            case IMTK_PB_CTRL_MEDIA_TYPE_MKV:                    
                pt_aud_track_info->eAudEnc = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mkv_audio_stm_attr.e_enc);                    
                pt_aud_track_info->eChNum = _cmpb_map_echannel(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mkv_audio_stm_attr.i2_channels);                    
                pt_aud_track_info->eSampleRate = _cmpb_cal_sample_rate((UINT32)pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mkv_audio_stm_attr.i4_samples_per_sec);                    
                pt_aud_track_info->u4BitRate = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mkv_audio_stm_attr.ui4_avg_bytes_sec*8;                    
                CMPB_STRNCPY((CHAR*)(pt_aud_track_info->s_lang), pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mkv_audio_stm_attr.s_lang, ISO_639_LANG_LEN);                                
                break;                    

            case IMTK_PB_CTRL_MEDIA_TYPE_RM:
        pt_aud_track_info->eAudEnc = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_rm_audio_stm_attr.e_enc);
                pt_aud_track_info->eChNum = _cmpb_map_echannel(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_rm_audio_stm_attr.ui2_num_channel);
                pt_aud_track_info->eSampleRate = _cmpb_cal_sample_rate((UINT32)pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_rm_audio_stm_attr.ui4_sample_rate);
                pt_aud_track_info->u4BitRate = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_rm_audio_stm_attr.ui4_avg_bit_rate*8;
                break;

        case IMTK_PB_CTRL_MEDIA_TYPE_FLV:
        pt_aud_track_info->eAudEnc = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_flv_audio_stm_attr.e_enc);
                pt_aud_track_info->eChNum = _cmpb_map_echannel(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_flv_audio_stm_attr.i2_channels);
                pt_aud_track_info->eSampleRate = _cmpb_cal_sample_rate((UINT32)pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_flv_audio_stm_attr.i4_samples_per_sec);
                pt_aud_track_info->u4BitRate = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_flv_audio_stm_attr.ui4_avg_bytes_per_sec*8;
                break;
                    
            case IMTK_PB_CTRL_MEDIA_TYPE_OGG:                    
                if (pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_audio_stm_attr.t_ogg_audio_type == OGG_STRM_TYPE_VORBIS)                    
                {                        
                    pt_aud_track_info->eAudEnc = IMTK_PB_CTRL_AUD_ENC_VORBIS;                        
                    pt_aud_track_info->eChNum = _cmpb_map_echannel(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mkv_audio_stm_attr.i2_channels);                        
                    pt_aud_track_info->eSampleRate = _cmpb_cal_sample_rate((UINT32)pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui2_bit_per_sample);                        
                    pt_aud_track_info->u4BitRate = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui4_avg_bytes_per_sec*8;                    
                }                    
                else if (pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_audio_stm_attr.t_ogg_audio_type == OGG_STRM_TYPE_AUDIO)                    
                {                        
                    pt_aud_track_info->eAudEnc = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.e_enc);                        
                    pt_aud_track_info->eChNum = _cmpb_map_echannel(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui2_channels);                        
                    pt_aud_track_info->eSampleRate = _cmpb_cal_sample_rate((UINT32)pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui2_bit_per_sample);                        
                    pt_aud_track_info->u4BitRate = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui4_avg_bytes_per_sec*8;                    
                }                                                   
                break;                                    
                    
            case IMTK_PB_CTRL_MEDIA_TYPE_AUDIO_ES:                    
                pt_aud_track_info->eAudEnc = _cmpb_get_svctx_aud_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp3_audio_stm_attr.e_enc);                    
                pt_aud_track_info->eChNum = _cmpb_map_echannel(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp3_audio_stm_attr.ui1_channels);                    
                pt_aud_track_info->eSampleRate = _cmpb_cal_sample_rate((UINT32)pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp3_audio_stm_attr.ui4_sample_rate);                    
                pt_aud_track_info->u4BitRate = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp3_audio_stm_attr.ui4_bitrate;                    
                CMPB_STRNCPY((CHAR*)(pt_aud_track_info->s_lang), pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp3_audio_stm_attr.s_lang, ISO_639_LANG_LEN);                                                    
                break;                                                       
                    
            default :                    
                break;            
                    }
        }
        else if (ST_VIDEO == pt_stm_atrbt[ui2_i].e_type)
        {                                        
            pt_vid_track_info =  &(pt_cmpb_inst->u_vid_track.t_get_info[pt_cmpb_inst->u2VideoTrackNum]);
            pt_vid_track_info->eVidEnc = IMTK_PB_CTRL_VID_ENC_UNKNOWN;
            pt_cmpb_inst->u2VideoTrackNum++;
            
            switch (pt_cmpb_inst->u_media.t_get_info.eMediaType)
            {
                case IMTK_PB_CTRL_MEDIA_TYPE_MP4:                            
                    pt_vid_track_info->eVidEnc = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp4i_video_stm_attr.e_enc);
                    pt_vid_track_info->u4Width = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp4i_video_stm_attr.i4_width;
                    pt_vid_track_info->u4Height = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mp4i_video_stm_attr.i4_height;                            
                    /* pt_vid_track_info->u4BitRate = 0; */
                    break;
                    
                case IMTK_PB_CTRL_MEDIA_TYPE_ASF:                            
                    pt_vid_track_info->eVidEnc = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_asf_video_stm_attr.e_enc);
                    pt_vid_track_info->u4Width = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_asf_video_stm_attr.i4_width;
                    pt_vid_track_info->u4Height = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_asf_video_stm_attr.i4_height;
                    /* pt_vid_track_info->u4BitRate = 0; */
                    break;
                    
                case IMTK_PB_CTRL_MEDIA_TYPE_AVI:
                    pt_vid_track_info->eVidEnc = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_video_stm_attr.e_enc);
                    pt_vid_track_info->u4Width = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_video_stm_attr.i4_width;
                    pt_vid_track_info->u4Height = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_avi_video_stm_attr.i4_height;
                    /* pt_vid_track_info->u4BitRate = 0; */
                    break;
                    
                case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_PS:
                    pt_vid_track_info->eVidEnc = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ps_video_stm_attr.e_enc);
                    pt_vid_track_info->u4Width = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ps_video_stm_attr.i4_width;
                    pt_vid_track_info->u4Height = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ps_video_stm_attr.i4_height;
                    /* pt_vid_track_info->u4BitRate = 0; */
                    break;
                    
                case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS:  
                    pt_vid_track_info->eVidEnc = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ts_video_stm_attr.e_enc);
                    pt_vid_track_info->u4Width = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ts_video_stm_attr.ui4_width;
                    pt_vid_track_info->u4Height = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ts_video_stm_attr.ui4_height;                          
                    break;

                case IMTK_PB_CTRL_MEDIA_TYPE_MKV:
                    pt_vid_track_info->eVidEnc = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mkv_video_stm_attr.e_enc);
                    pt_vid_track_info->u4Width = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mkv_video_stm_attr.i4_width;
                    pt_vid_track_info->u4Height = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_mkv_video_stm_attr.i4_height;
                    /* pt_vid_track_info->u4BitRate = 0; */
                    break;   

                case IMTK_PB_CTRL_MEDIA_TYPE_RM:
                    pt_vid_track_info->eVidEnc = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_rm_video_stm_attr.e_enc);
                    pt_vid_track_info->u4Width = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_rm_video_stm_attr.ui2_frm_width;
                    pt_vid_track_info->u4Height = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_rm_video_stm_attr.ui2_frm_height;
                    /* pt_vid_track_info->u4BitRate = 0; */
                    break;

                case IMTK_PB_CTRL_MEDIA_TYPE_FLV:
                    pt_vid_track_info->eVidEnc = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_flv_video_stm_attr.e_enc);
                    pt_vid_track_info->u4Width = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_flv_video_stm_attr.i4_width;
                    pt_vid_track_info->u4Height = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_flv_video_stm_attr.i4_height;
                    /* pt_vid_track_info->u4BitRate = 0; */
                    break;
                    
                case IMTK_PB_CTRL_MEDIA_TYPE_OGG:
                    pt_vid_track_info->eVidEnc = _cmpb_get_svctx_vid_enc(pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_video_stm_attr.e_enc);
                    pt_vid_track_info->u4Width = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_video_stm_attr.ui4_width;
                    pt_vid_track_info->u4Height = pt_stm_atrbt[ui2_i].t_stm_atrbt.u.t_ogg_video_stm_attr.ui4_height;
                    /* pt_vid_track_info->u4BitRate = 0; */
                    break;
                    
                default :
                    break;
            }
        }
        else
        {                    
        }
    }
    return IMTK_PB_ERROR_CODE_OK;
}

static VOID _cmpb_get_timecode_callback(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, (IMTK_PB_HANDLE_T)pv_tag);
    if (pt_cmpb_inst == NULL)
    {
        return ;
    }

    if (pt_cmpb_inst->e_cur_state == CMPB_CTRL_PLAYING ||
        pt_cmpb_inst->e_cur_state == CMPB_CTRL_PAUSED  ||
        pt_cmpb_inst->e_cur_state == CMPB_CTRL_TRICK)
    {
        #if 1
        if (pt_cmpb_inst->fg_play_normal && !pt_cmpb_inst->fg_during_seek)
        {
            _cmpb_ctrl_media_play_nfy_fct(
                    pt_cmpb_inst->h_svctx_hdl, 
                    SVCTX_COND_PRESENTING,
                    SVCTX_NTFY_CODE_MEDIA_TIME_UPDATE,
                    ST_AUDIO,
                    pt_cmpb_inst 
                    );
        }
        #else
        pt_cmpb_inst->ui4_data = 0;
        if (pt_cmpb_inst->fg_play_normal && !pt_cmpb_inst->fg_during_seek)
        {
            if (pt_cmpb_inst->pf_notify)
            {
                pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_CUR_TIME_UPDATE, 
                                        pt_cmpb_inst->pv_tag,
                                        pt_cmpb_inst->ui4_data
                                        );
            }
        }
        #endif
    }
}

static VOID _cmpb_ctrl_push_nfy_fct(
                                MM_PLAYBACK_CTRL_PUSH_EVENT_T   e_event,
                                VOID*                           pv_tag,
                                UINT32                          ui4_data)
{
    CMPB_CB_MSG_T               t_cb_msg;
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, (IMTK_PB_HANDLE_T)pv_tag);
    if (pt_cmpb_inst == NULL)
    {
        return ;
    }
    
    if (e_event != MM_PLAYBACK_CTRL_PUSH_EVENT_BUF_CONSUMED &&
        e_event != MM_PLAYBACK_CTRL_PUSH_EVENT_BUF_OVERFLOW &&
        e_event != MM_PLAYBACK_CTRL_PUSH_EVENT_BUF_UNDRFLOW)
    {
        return;
    }
    
    t_cb_msg.pt_cmpb_inst = pt_cmpb_inst;
    t_cb_msg.e_msg_type = CMPB_CB_MSG_MEDIA_PUSH_NFY;

    t_cb_msg.u.t_media_push_event.e_event = e_event;
    t_cb_msg.u.t_media_push_event.ui4_data = ui4_data;

    _cmpb_cb_send_msg(&t_cb_msg);

}

static VOID _cmpb_ctrl_push_nfy_fct_body(
                                CMPB_CTRL_INST_T*               pt_cmpb_inst,
                                MM_PLAYBACK_CTRL_PUSH_EVENT_T   e_event,
                                UINT32                          ui4_data)
{
    INT32                       i4_ret;
    VOID*                       pv_buf;
    IMTK_PB_CTRL_EVENT_T        e_event_type = IMTK_PB_CTRL_EVENT_UNKNOWN;

    i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 

    if(OSR_OK != i4_ret)
    {
        return;
    }
    
    switch(e_event)
    {
    case MM_PLAYBACK_CTRL_PUSH_EVENT_BUF_CONSUMED:
        if ((pt_cmpb_inst->t_push_buf.h_mem_part) && (pt_cmpb_inst->e_cur_state==CMPB_CTRL_PLAYING))
        {
            UINT32  ui4_size;
            UINT32  ui4_tmp_lvl;
            
            pv_buf = (VOID*)(((UINT8*)pt_cmpb_inst->t_push_buf.pv_buffer) + ui4_data);
            ui4_size = (UINT32)os_mem_get_mem_ptr_size(pv_buf);

            ui4_tmp_lvl = pt_cmpb_inst->ui4_push_buf_level + ui4_size;
            pt_cmpb_inst->e_buf_status = IMTK_PB_CTRL_PUSH_BUF_NORMAL;

            if ((pt_cmpb_inst->ui4_push_buf_level <= pt_cmpb_inst->ui4_push_buf_high)
                && (ui4_tmp_lvl>pt_cmpb_inst->ui4_push_buf_high))
            {
                e_event_type = IMTK_PB_CTRL_EVENT_BUFFER_LOW;
                pt_cmpb_inst->e_buf_status = IMTK_PB_CTRL_PUSH_BUF_LOW;
            }
            
            pt_cmpb_inst->ui4_push_buf_level = ui4_tmp_lvl;
            CMPB_MEM_FREE(pv_buf);
        }
        if(pt_cmpb_inst->i4_push_buf_cnt > 0)
        {
            pt_cmpb_inst->i4_push_buf_cnt --;
        }
        break;
    default:
        break;
    }
    
    if(e_event_type != IMTK_PB_CTRL_EVENT_UNKNOWN)
    {
        pt_cmpb_inst->ui4_data = 0;        
        if (pt_cmpb_inst->pf_notify)
        {
            pt_cmpb_inst->pf_notify(e_event_type, 
                                    pt_cmpb_inst->pv_tag,
                                    pt_cmpb_inst->ui4_data
                                    );
        }
    }
    
    CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);
}

#ifdef MM_SEND_BUFFER_PREPLAY_SUPPORT
static IMTK_PB_ERROR_CODE_T _cmpb_ctrl_prestore_data_release(IMTK_PB_HANDLE_T      hHandle)
{
    CMPB_CTRL_INST_T*               pt_cmpb_inst        = NULL; 
    IMTK_PB_CTRL_PRESTORE_DATA_T*   pt_prestore_data    = NULL;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    } 

    while (pt_cmpb_inst->pt_prestore_data_header != NULL)
    {
        pt_prestore_data = pt_cmpb_inst->pt_prestore_data_header->ptNext;
        
        CMPB_MEM_FREE(pt_cmpb_inst->pt_prestore_data_header->pu1Buffer);
        pt_cmpb_inst->pt_prestore_data_header->pu1Buffer = NULL;
        CMPB_MEM_FREE(pt_cmpb_inst->pt_prestore_data_header);
        pt_cmpb_inst->pt_prestore_data_header = NULL;
        
        pt_cmpb_inst->pt_prestore_data_header = pt_prestore_data;
    }

    return IMTK_PB_ERROR_CODE_OK;
}

static IMTK_PB_ERROR_CODE_T _cmpb_ctrl_prestore_data_save(IMTK_PB_HANDLE_T      hHandle, 
                                           uint32_t              u4BufSize,
                                           uint8_t**             ppu1PushBuf)
{
    IMTK_PB_ERROR_CODE_T           e_ret            = IMTK_PB_ERROR_CODE_OK;
    CMPB_CTRL_INST_T*              pt_cmpb_inst     = NULL; 
    IMTK_PB_CTRL_PRESTORE_DATA_T*  pt_prestore_data = NULL;

    do
    {
         _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

        if (NULL == pt_cmpb_inst)
        {
            e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
            break;
        } 

        if (ppu1PushBuf == NULL || u4BufSize == 0)
        {
            e_ret =  IMTK_PB_ERROR_CODE_INV_ARG;
            break;
        }

        pt_prestore_data = (IMTK_PB_CTRL_PRESTORE_DATA_T*)CMPB_MEM_CALLOC(1, sizeof(IMTK_PB_CTRL_PRESTORE_DATA_T));
        if (pt_prestore_data == NULL)
        {
            e_ret = IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
            break;
        }
        CMPB_MEMSET(pt_prestore_data, 0, sizeof(IMTK_PB_CTRL_PRESTORE_DATA_T));

        pt_prestore_data->pu1Buffer = (uint8_t*)CMPB_MEM_CALLOC(1, u4BufSize);
        if (pt_prestore_data->pu1Buffer == NULL)
        {
            e_ret = IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
            break;
        }
        CMPB_MEMSET(pt_prestore_data->pu1Buffer, 0, u4BufSize);
        
        pt_prestore_data->u4BufferSize = u4BufSize;
        *ppu1PushBuf = pt_prestore_data->pu1Buffer;

        if (pt_cmpb_inst->pt_prestore_data_header == NULL)
        {
            pt_cmpb_inst->pt_prestore_data_header = pt_prestore_data;
        }
        else
        {
            pt_cmpb_inst->pt_prestore_data_footer->ptNext = pt_prestore_data;
        }
        pt_cmpb_inst->pt_prestore_data_footer = pt_prestore_data;
        
    }while(FALSE);

    if (e_ret != IMTK_PB_ERROR_CODE_OK)
    {
        _cmpb_ctrl_prestore_data_release(hHandle);
    }

    return e_ret;
}

static IMTK_PB_ERROR_CODE_T _cmpb_ctrl_prestore_data_push(IMTK_PB_HANDLE_T      hHandle)
{
    INT32                           i4_ret               = SVCTXR_OK;
    IMTK_PB_ERROR_CODE_T            e_ret                = IMTK_PB_ERROR_CODE_OK;
    CMPB_CTRL_INST_T*               pt_cmpb_inst         = NULL; 
    IMTK_PB_CTRL_PRESTORE_DATA_T*   pt_prestore_data     = NULL;
    UINT8*                          pui1_buffer          = NULL;
    UINT32                          ui4_tmp_lvl          = 0;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    if (NULL_HANDLE == pt_cmpb_inst->t_push_buf.h_mem_part)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    pt_prestore_data = pt_cmpb_inst->pt_prestore_data_header;
    while (pt_prestore_data != NULL)
    {
        /*GET BUFFER*/
        pui1_buffer = CMPB_MEM_PART_ALLOC(pt_cmpb_inst->t_push_buf.h_mem_part,
                                         (SIZE_T)pt_prestore_data->u4BufferSize);             
        if(pui1_buffer == NULL)
        {
            e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
            break;
        }
        
        /*COPY*/
        CMPB_MEMCPY(pui1_buffer, pt_prestore_data->pu1Buffer, pt_prestore_data->u4BufferSize);
        
        /*SEND DATA*/
        i4_ret = CMPB_SVCTX_MEDIA_SET_INFO(
                    pt_cmpb_inst->h_svctx_hdl,
                    pt_cmpb_inst->h_media_hdl,
                    SVCTX_MM_SET_TYPE_SEND_PUSH_BUF,
                    (VOID*)((UINT32)pui1_buffer - (UINT32)(pt_cmpb_inst->t_push_buf.pv_buffer)),
                    (SIZE_T)pt_prestore_data->u4BufferSize
                    );
        if(i4_ret != SVCTXR_OK)
        {
            e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
            break;
        } 

        if (pt_cmpb_inst->ui4_push_buf_level < pt_prestore_data->u4BufferSize)
        {
            ui4_tmp_lvl = 0;
        }
        else
        {
            ui4_tmp_lvl = pt_cmpb_inst->ui4_push_buf_level - pt_prestore_data->u4BufferSize;
        }
        
        /*BUFFER HIGH*/
        if ((pt_cmpb_inst->ui4_push_buf_level >= pt_cmpb_inst->ui4_push_buf_low)
            && (ui4_tmp_lvl<pt_cmpb_inst->ui4_push_buf_low))
        {      
            CMPB_THREAD_DELAY(100);
        }

        pt_cmpb_inst->ui4_push_buf_level = ui4_tmp_lvl;
        
        pt_prestore_data = pt_prestore_data->ptNext;
    }

    /* RELEASE*/
    _cmpb_ctrl_prestore_data_release(hHandle);
    
    return e_ret; 
}
#endif

/*! @brief          Callback function for media open.
 *  @param [in]     h_ctx_hdl               The handle of svctx instance.
 *  @param [in]     e_ctx_cond              The callback condition.
 *  @param [in]     e_nfy_code              The callback code.
 *  @param [in]     e_stm_type              The stream type.
 *  @param [in]     pv_nfy_tag              Reference to the notify tag.
 *  @param [in]     pv_file_hdl             Reference to the notify data.
 *  @return         VOID
 *  @note
 *  @see            
 */
static VOID _cmpb_ctrl_media_open_nfy_fct(
                    HANDLE_T                        h_ctx_hdl, 
                    SVCTX_COND_T                    e_ctx_cond,
                    SVCTX_NTFY_CODE_T               e_nfy_code,
                    STREAM_TYPE_T                   e_stm_type,
                    VOID*                           pv_nfy_tag,
                    VOID*                           pv_data
                    )
{
    CMPB_CB_MSG_T t_cb_msg;
    
    t_cb_msg.pt_cmpb_inst = (CMPB_CTRL_INST_T* )pv_nfy_tag;
    t_cb_msg.e_msg_type = CMPB_CB_MSG_MEDIA_OPEN_NFY;
    
    t_cb_msg.u.t_media_open_event.h_ctx_hdl = h_ctx_hdl;
    t_cb_msg.u.t_media_open_event.e_ctx_cond = e_ctx_cond;
    t_cb_msg.u.t_media_open_event.e_nfy_code = e_nfy_code;
    t_cb_msg.u.t_media_open_event.e_stm_type = e_stm_type;
    t_cb_msg.u.t_media_open_event.pv_nfy_tag = pv_nfy_tag;
    t_cb_msg.u.t_media_open_event.pv_data = pv_data;
    
    _cmpb_cb_send_msg(&t_cb_msg);
}

static VOID _cmpb_ctrl_media_open_nfy_fct_body(
                    HANDLE_T                        h_ctx_hdl, 
                    SVCTX_COND_T                    e_ctx_cond,
                    SVCTX_NTFY_CODE_T               e_nfy_code,
                    STREAM_TYPE_T                   e_stm_type,
                    VOID*                           pv_nfy_tag,
                    VOID*                           pv_data
                    )
{

    CMPB_CTRL_INST_T*                   pt_cmpb_inst;
    INT32                               i4_ret;
    MINFO_INFO_T                        t_minfo_rec;    
    MM_SVC_DUR_INFO_T                   t_dur;    
    SIZE_T                              z_size;
    MINFO_TYPE_META_DATA_T*             pt_meta_data = NULL;
    MEDIA_FORMAT_T                      t_media_fmt;
    BOOL                                b_is_ts = FALSE;
    INT32                               event_type = 0;
    BOOL                                b_event_need_trigger = FALSE;
    

    if (NULL == pv_nfy_tag)
    {
        return;
    }
    
    pt_cmpb_inst = (CMPB_CTRL_INST_T* )pv_nfy_tag;

    //////this message should process before lock h_sema//////
    if(e_nfy_code == SVCTX_NTFY_CODE_MEDIA_URI_FILE_OPENED)
    //if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_URI) /*URI mode*/
    {
        pt_cmpb_inst->h_uri_file = (HANDLE_T)pv_data;
        return;
    }
    else if(e_nfy_code == SVCTX_NTFY_CODE_MEDIA_DESC_ALLOCED)
    {
        pt_cmpb_inst->h_media_hdl = (HANDLE_T)pv_data;
    }
    
    i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 

    if(OSR_OK != i4_ret)
    {
        return;
    }
    
    switch (e_nfy_code)
    {
        case SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSING:
            break;

        case SVCTX_NTFY_CODE_MEDIA_ERROR:
            pt_cmpb_inst->ui4_data = 0;
            pt_cmpb_inst->b_parse_done = TRUE;
            pt_cmpb_inst->b_start_parse = FALSE;

            pt_cmpb_inst->u_media.t_get_info.u2AudioTrackNum = 0;
            pt_cmpb_inst->u_media.t_get_info.u4TotalDuration = 0;
            pt_cmpb_inst->u_media.t_get_info.u8Size = 0;
            pt_cmpb_inst->u_media.t_get_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_UNKNOWN;
            pt_cmpb_inst->e_cur_state = CMPB_CTRL_READY; 

            pt_cmpb_inst->b_open_media_error = TRUE;

            
            if(pt_cmpb_inst->h_media_hdl)
            {
                pt_cmpb_inst->ui4_data  = (UINT32)IMTK_PB_CTRL_ERROR_FILE_CORRUPT;
            }
            else
            {
                pt_cmpb_inst->ui4_data  = (UINT32)IMTK_PB_CTRL_ERROR_FILE_NOT_SUPPORT;
            }
            event_type = IMTK_PB_CTRL_EVENT_PLAYBACK_ERROR;
            b_event_need_trigger = TRUE;

            if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_URI &&
               (CMPB_HANDLE_VALID(pt_cmpb_inst->h_uri_sema) == TRUE)) /*URI mode*/
            {
                pt_cmpb_inst->h_uri_file = (HANDLE_T)0XFFFFFFFF;
                i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_uri_sema);
            }
            break;
        
        case SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSED:
            if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_URI &&
               (CMPB_HANDLE_VALID(pt_cmpb_inst->h_uri_sema) == TRUE)) /*URI mode*/
            {
                i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_uri_sema);
            }
            
            if(pt_cmpb_inst->b_set_engparm_async)
            {
                event_type = IMTK_PB_CTRL_EVENT_PREPARE_DONE;
                b_event_need_trigger = TRUE;
            }
            if ((CMPB_CTRL_TRANSIT == pt_cmpb_inst->e_cur_state) 
                && (CMPB_CTRL_OPENED == pt_cmpb_inst->e_new_state)
                )
            {
                pt_cmpb_inst->e_cur_state = pt_cmpb_inst->e_new_state;
            }
            z_size = sizeof(t_media_fmt);
            i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                            pt_cmpb_inst->h_svctx_hdl,
                            pt_cmpb_inst->h_media_hdl,
                            SVCTX_MM_GET_TYPE_MEDIA_FORMAT,
                            (VOID*)&t_media_fmt,
                            z_size
                            );

            if((pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PULL ||
                pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_URI) && 
                t_media_fmt.e_media_type == MEDIA_TYPE_TS)
            {
                b_is_ts = TRUE;
            }
            /* Media / Stream info ready, backup to instance data structure */
            if(!b_is_ts)
            {
                z_size = sizeof(MINFO_INFO_T);
                CMPB_MEMSET(&t_minfo_rec, 0, sizeof(MINFO_INFO_T));

                i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                pt_cmpb_inst->h_svctx_hdl,
                                pt_cmpb_inst->h_media_hdl,
                                SVCTX_MM_GET_TYPE_META_DATA,
                                (VOID*)&t_minfo_rec,
                                z_size
                                );
                                
                if(SVCTXR_OK != i4_ret)
                {
                    CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_GET_INFO() returns %d\n", i4_ret));
                    //return;/*meta data maybe not exist*/
                }
                else
                {
                    pt_meta_data = &t_minfo_rec.u.t_meta_data;
                    /*  Get Duration */
                    pt_cmpb_inst->u_media.t_get_info.u4TotalDuration = (UINT32)(pt_meta_data->ui8_pb_duration*1000); /*in millisecond*/

                }
            }
            else
            {
                /* Get  ts File duration */
                CMPB_MEMSET(&t_dur, 0, sizeof(MM_SVC_DUR_INFO_T));
                z_size = sizeof(MM_SVC_DUR_INFO_T);     

                t_dur.t_dur_type = MM_SVC_DUR_TYPE_ALL;
                t_dur.ui2_idx = 0;
                                        
                i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                pt_cmpb_inst->h_svctx_hdl,
                                pt_cmpb_inst->h_media_hdl,
                                SVCTX_MM_GET_TYPE_DUR,
                                (VOID*)&t_dur,
                                z_size
                                );
                if(SVCTXR_OK == i4_ret)
                {
                    pt_cmpb_inst->u_media.t_get_info.u4TotalDuration = (UINT32)(t_dur.ui8_duration * 1000);
                }
            }
            
            /* Get File Size */
            CMPB_MEMSET(&t_dur, 0, sizeof(MM_SVC_DUR_INFO_T));
            z_size = sizeof(MM_SVC_DUR_INFO_T);     

            t_dur.t_dur_type = MM_SVC_DUR_TYPE_ALL_OFFSET;
            t_dur.ui2_idx = 0;
                                    
            i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                            pt_cmpb_inst->h_svctx_hdl,
                            pt_cmpb_inst->h_media_hdl,
                            SVCTX_MM_GET_TYPE_DUR,
                            (VOID*)&t_dur,
                            z_size
                            );

            if(SVCTXR_OK != i4_ret)
            {
                CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_GET_INFO() returns %d\n", i4_ret));
                /*return;*/
            }
            else
            {
                pt_cmpb_inst->u_media.t_get_info.u8Size = t_dur.ui8_duration;
            }


            if(pt_cmpb_inst->e_model != IMTK_PB_CTRL_BUFFERING_MODEL_PUSH)
            {
                _cmpb_fill_strm_info(pt_cmpb_inst->h_cmpb);
            }
            CMPB_DBG_ERROR(("IMTK_PB_CTRL_EVENT_PRE_PARSE_DONE\n"));
            
            pt_cmpb_inst->u2AudioTrackNum = pt_cmpb_inst->u_media.t_get_info.u2AudioTrackNum;
            pt_cmpb_inst->ui4_data = 0;
            pt_cmpb_inst->b_parse_done = TRUE;
            pt_cmpb_inst->b_start_parse = FALSE;
#if 0
            if (pt_cmpb_inst->pf_notify && b_event_need_trigger)
            {
                pt_cmpb_inst->pf_notify(event_type, 
                                        pt_cmpb_inst->pv_tag,
                                        pt_cmpb_inst->ui4_data
                                        );
            }
#endif
            break;
            
        default :
            break;
    }
    
    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    if(OSR_OK != i4_ret)
    {        
    }

    if(b_event_need_trigger && pt_cmpb_inst->pf_notify && !pt_cmpb_inst->b_closing)
    {
        pt_cmpb_inst->pf_notify(event_type, 
                                pt_cmpb_inst->pv_tag,
                                pt_cmpb_inst->ui4_data
                                );
    }
}

/*! @brief          Callback function for media open.
 *  @param [in]     h_ctx_hdl               The handle of svctx instance.
 *  @param [in]     e_ctx_cond              The callback condition.     
 *  @param [in]     e_nfy_code              The callback code.           
 *  @param [in]     e_stm_type              The stream type.             
 *  @param [in]     pv_nfy_tag              Reference to the notify tag. 
 *  @return         VOID
 *  @note
 *  @see            
 */
static VOID _cmpb_ctrl_media_play_nfy_fct(
                    HANDLE_T                        h_ctx_hdl, 
                    SVCTX_COND_T                    e_ctx_cond,
                    SVCTX_NTFY_CODE_T               e_nfy_code,
                    STREAM_TYPE_T                   e_stm_type,
                    VOID*                           pv_nfy_tag 
                    )
{
    CMPB_CB_MSG_T t_cb_msg;
    
    t_cb_msg.pt_cmpb_inst = (CMPB_CTRL_INST_T* )pv_nfy_tag;
    t_cb_msg.e_msg_type = CMPB_CB_MSG_MEDIA_PLAY_NFY;
    
    t_cb_msg.u.t_media_play_event.h_ctx_hdl = h_ctx_hdl;
    t_cb_msg.u.t_media_play_event.e_ctx_cond = e_ctx_cond;
    t_cb_msg.u.t_media_play_event.e_nfy_code = e_nfy_code;
    t_cb_msg.u.t_media_play_event.e_stm_type = e_stm_type;
    t_cb_msg.u.t_media_play_event.pv_nfy_tag = pv_nfy_tag;

    _cmpb_cb_send_msg(&t_cb_msg);
}

static VOID _cmpb_ctrl_media_play_nfy_fct_body(
                    HANDLE_T                        h_ctx_hdl, 
                    SVCTX_COND_T                    e_ctx_cond,
                    SVCTX_NTFY_CODE_T               e_nfy_code,
                    STREAM_TYPE_T                   e_stm_type,
                    VOID*                           pv_nfy_tag 
                    )
{

    /* INT32                   i4_ret; */
    CMPB_CTRL_INST_T*       pt_cmpb_inst;
    IMTK_PB_CTRL_EVENT_T    e_event_type = IMTK_PB_CTRL_EVENT_UNKNOWN;
    BOOL                    b_event_need_trigger = FALSE;
    INT32                   i4_ret;
    BOOL                    b_video_fmt_update = FALSE;
    IMTK_PB_CTRL_SPEED_T    t_cmpb_speed;
    MM_SPEED_TYPE_T         t_mm_speed;
    
    pt_cmpb_inst = (CMPB_CTRL_INST_T*)pv_nfy_tag;    

/*
    if (SVCTX_NTFY_CODE_MEDIA_SEEK_READY == e_nfy_code)
    {
        i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
        if(OSR_OK != i4_ret)
        {            
        }
    }
*/ 
    i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
    if(OSR_OK != i4_ret)
    {
        return;
    }
        
    switch (e_nfy_code)
    {
        case SVCTX_NTFY_CODE_REACH_VALID_RANGE_START:
            e_event_type = IMTK_PB_CTRL_EVENT_REACH_VALID_RANGE_START;
            b_event_need_trigger = TRUE;
            break;

        case SVCTX_NTFY_CODE_MEDIA_VS_BIT_RATE:
            e_event_type = IMTK_PB_CTRL_EVENT_BITRATE_UPDATE;
            b_event_need_trigger = TRUE;
            break;
        case SVCTX_NTFY_CODE_REACH_VALID_RANGE_END:
            e_event_type = IMTK_PB_CTRL_EVENT_REACH_VALID_RANGE_END;
            if ( pt_cmpb_inst->ui4_av_info & IMTK_PB_CTRL_PLAY_FLAG_VIDEO )
            {
                b_event_need_trigger = TRUE;
            }
            break;    
        case SVCTX_NTFY_CODE_MEDIA_REPLAY:
            pt_cmpb_inst->e_cur_speed = IMTK_PB_CTRL_SPEED_1X;
            pt_cmpb_inst->fg_eos = FALSE;
            break;

        case SVCTX_NTFY_CODE_MEDIA_FRAMERATE_NOT_SUPPORT:
        pt_cmpb_inst->ui4_data  = IMTK_PB_CTRL_EVENT_FRAMERATE_NOT_SUPPORT;
            e_event_type = IMTK_PB_CTRL_EVENT_PLAYBACK_ERROR;
            b_event_need_trigger = TRUE;
            break;
#if 0

        case SVCTX_NTFY_CODE_MEDIA_RESOLUTION_NOT_SUPPORT:
            (uint32_t)e_event_type = IMTK_PB_CTRL_EVENT_RESOLUTION_NOT_SUPPORT;
            b_event_need_trigger = TRUE;
            break;
#endif
        case SVCTX_NTFY_CODE_SCDB_MODIFY:
        {
            STREAM_COMP_ID_T    t_comp_id;
            SCDB_REC_T          t_scdb_rec  = {0};
            STREAM_TYPE_T       e_type      = ST_AUDIO;
            
            if (pt_cmpb_inst->b_audio_changing)
            {
                e_type = ST_AUDIO;
            }
            else if(pt_cmpb_inst->b_video_changing)
            {
                e_type = ST_VIDEO;
            }
            else if(pt_cmpb_inst->b_subtl_changing)
            {
                e_type = ST_SUBTITLE;
            }
            else
            {/* Not supported temperally*/
                break;
            }
            CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);
            
            CMPB_SVCTX_GET_GET_STREAM_DATA(
                    pt_cmpb_inst->h_svctx_hdl,
                    e_type,
                    &t_comp_id,
                    &t_scdb_rec
                    );

            CMPB_SVCTX_SYNC_STOP_STREAM(pt_cmpb_inst->h_svctx_hdl, e_type, 0);

            CMPB_SVCTX_SELECT_STREAM(
                    pt_cmpb_inst->h_svctx_hdl,
                    &t_comp_id,
                    _cmpb_ctrl_media_play_nfy_fct,
                    (VOID*)pt_cmpb_inst);
            CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        }
            break;
        case SVCTX_NTFY_CODE_OWN_SERVICE_CHANGING:
            break;
            
        case SVCTX_NTFY_CODE_CRNT_EVN_READY:
            break;
        case SVCTX_NTFY_CODE_MEDIA_SEEK_READY:
            pt_cmpb_inst->fg_during_seek = FALSE;
            pt_cmpb_inst->ui4_data       = (UINT32)IMTK_PB_CTRL_TIMESEEK_OK;
            e_event_type = IMTK_PB_CTRL_EVENT_TIMESEEK_DONE;
            b_event_need_trigger = TRUE;
            break;
        case SVCTX_NTFY_CODE_MEDIA_SEEK_ERROR:
            pt_cmpb_inst->fg_during_seek = FALSE;
            pt_cmpb_inst->ui4_data       = (UINT32)IMTK_PB_CTRL_TIMESEEK_FAIL;
            e_event_type = IMTK_PB_CTRL_EVENT_TIMESEEK_DONE;
            b_event_need_trigger = TRUE;
            break;
        
        case SVCTX_NTFY_CODE_MEDIA_UNDERFLOW:
            e_event_type = IMTK_PB_CTRL_EVENT_BUFFER_UNDERFLOW;
            b_event_need_trigger = TRUE;
            break;

        case SVCTX_NTFY_CODE_MEDIA_DURATION_UPDATE:
            e_event_type = IMTK_PB_CTRL_EVENT_TOTAL_TIME_UPDATE;
            b_event_need_trigger = TRUE;    
            break;

        case SVCTX_NTFY_CODE_MEDIA_TIME_UPDATE:
            e_event_type = IMTK_PB_CTRL_EVENT_CUR_TIME_UPDATE;
            b_event_need_trigger = TRUE;   
            break;

        case SVCTX_NTFY_CODE_NO_CC_AVIL_INDICATOR:
            break;            

        case SVCTX_NTFY_CODE_MEDIA_SPEED_UPDATE:
        #if 0
            e_event_type = IMTK_PB_CTRL_EVENT_SPEED_UPDATE;
            b_event_need_trigger = TRUE;
        #endif   
            break;
        
        case SVCTX_NTFY_CODE_SCDB_ADD:
            break;
            
        case SVCTX_NTFY_CODE_NORMAL:          
            break;

        case SVCTX_NTFY_CODE_BUF_READY:
            pt_cmpb_inst->fg_buf_rdy = TRUE;
#ifdef LINUX_TURNKEY_SOLUTION
            e_event_type = IMTK_PB_CTRL_EVENT_BUFFER_READY;
            b_event_need_trigger = TRUE;
#endif
            break;

        case SVCTX_NTFY_CODE_STREAM_STARTED:
            break;


        case SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE:
            b_event_need_trigger = TRUE;
            b_video_fmt_update = TRUE;
            if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PUSH)
            {
                _set_video_freeze(FALSE);
            }
case SVCTX_NTFY_CODE_MEDIA_AS_CODEC_NOT_SUPPORT:
        case SVCTX_NTFY_CODE_MEDIA_VS_CODEC_NOT_SUPPORT:
        case SVCTX_NTFY_CODE_MEDIA_HD_NOT_SUPPORT:
        case SVCTX_NTFY_CODE_MEDIA_RESOLUTION_NOT_SUPPORT:          
        case SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE:
            if (0 == pt_cmpb_inst->ui4_av_info)
            {
                STREAM_COMP_ID_T        t_comp_id;
                SCDB_REC_T              t_scdb_rec = {0};
                INT32 i4_strm_ret;
                
                i4_strm_ret = CMPB_SVCTX_GET_STREAM_COMP_ID(
                                        pt_cmpb_inst->h_svctx_hdl,
                                        ST_VIDEO,
                                        _cmpb_video_strm_comp_iter,
                                        (VOID*)pt_cmpb_inst->h_cmpb,
                                        &t_comp_id,
                                        &t_scdb_rec
                                        );
                if (SVCTXR_OK == i4_strm_ret)
                {
                    pt_cmpb_inst->ui4_av_info |= IMTK_PB_CTRL_PLAY_FLAG_VIDEO;
                }
                
                i4_strm_ret = CMPB_SVCTX_GET_STREAM_COMP_ID(
                                        pt_cmpb_inst->h_svctx_hdl,
                                        ST_AUDIO,
                                        _cmpb_audio_strm_comp_iter,
                                        (VOID*)pt_cmpb_inst->h_cmpb,
                                        &t_comp_id,
                                        &t_scdb_rec
                                        );
                if (SVCTXR_OK == i4_strm_ret)
                {
                    pt_cmpb_inst->ui4_av_info |= IMTK_PB_CTRL_PLAY_FLAG_AUDIO;
                }
            }

            if ((SVCTX_NTFY_CODE_MEDIA_AS_CODEC_NOT_SUPPORT==e_nfy_code) ||
                (SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE==e_nfy_code))
            {
                if (SVCTX_NTFY_CODE_MEDIA_AS_CODEC_NOT_SUPPORT==e_nfy_code)
                {
                    if (pt_cmpb_inst->pf_notify)
                    {
                        pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_PLAYBACK_ERROR, 
                                                pt_cmpb_inst->pv_tag,
                                                IMTK_PB_CTRL_ERROR_AUDIO_UNPLAYABLE
                                                );
                    }
                }
                else
                {
                    if (IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS == pt_cmpb_inst->u_media.t_get_info.eMediaType)
                    {
                        if (!pt_cmpb_inst->t_audio_info.b_iter)
                        {
                            UINT16  ui2_i;
                            IMTK_PB_CTRL_GET_AUD_TRACK_INFO_T*  pt_aud_track_info;
                            STREAM_COMP_ID_T        t_comp_id;
                            SCDB_REC_T              t_scdb_rec = {0};
    
                            pt_cmpb_inst->t_audio_info.b_iter = TRUE;
                            CMPB_SVCTX_GET_STREAM_COMP_ID(
                                    pt_cmpb_inst->h_svctx_hdl,
                                    ST_AUDIO,
                                    _cmpb_audio_strm_comp_iter,
                                    (VOID*)pt_cmpb_inst->h_cmpb,
                                    &t_comp_id,
                                    &t_scdb_rec
                                    );
                            pt_cmpb_inst->u_media.t_get_info.u2AudioTrackNum 
                                = pt_cmpb_inst->t_audio_info.ui2_aud_num;
    
                            for (ui2_i=0; ui2_i<pt_cmpb_inst->t_audio_info.ui2_aud_num; ui2_i++)
                            {
                                pt_aud_track_info =  &(pt_cmpb_inst->t_audio_info.at_get_info[ui2_i]);
                            
                                switch(pt_cmpb_inst->t_audio_info.at_scdb_rec[ui2_i].e_rec_type)
                                {
                                case SCDB_REC_TYPE_AUDIO_MPEG:
                                    switch (pt_cmpb_inst->t_audio_info.at_scdb_rec[ui2_i].u.t_audio_mpeg.e_enc)
                                    {
                                    case AUD_ENC_AC3:
                                    case AUD_ENC_E_AC3:
                                        break;
    
                                    case AUD_ENC_MPEG_1:
                                        if (AUD_LAYER_3 == pt_cmpb_inst->t_audio_info.at_scdb_rec[ui2_i].u.t_audio_mpeg.e_layer)
                                        {
                                            pt_aud_track_info->eAudEnc = IMTK_PB_CTRL_AUD_ENC_MP3;
                                        }
                                        else
                                        {
                                            pt_aud_track_info->eAudEnc = IMTK_PB_CTRL_AUD_ENC_MPEG;
                                        }
                                        break;
    
                                    case AUD_ENC_MPEG_2:
                                        pt_aud_track_info->eAudEnc = IMTK_PB_CTRL_AUD_ENC_MPEG;
                                        break;
                                    
                                    case AUD_ENC_AAC:
                                        pt_aud_track_info->eAudEnc = IMTK_PB_CTRL_AUD_ENC_AAC;
                                        break;
    
                                    default:
                                        pt_aud_track_info->eAudEnc = IMTK_PB_CTRL_AUD_ENC_UNKNOWN;
                                        break;
                                    }
                                    
                                    break;
                                    
                                default:
                                    pt_aud_track_info->eAudEnc = IMTK_PB_CTRL_AUD_ENC_UNKNOWN;
                                    break;
                                }
                            }
                        }
                    }
                }
                pt_cmpb_inst->ui4_av_update |= IMTK_PB_CTRL_PLAY_FLAG_AUDIO;
            }
            else
            {
                if ((SVCTX_NTFY_CODE_MEDIA_VS_CODEC_NOT_SUPPORT==e_nfy_code) ||
                    (SVCTX_NTFY_CODE_MEDIA_VS_CODEC_NOT_SUPPORT==e_nfy_code) ||
                    (SVCTX_NTFY_CODE_MEDIA_RESOLUTION_NOT_SUPPORT==e_nfy_code))
                {
                    if (SVCTX_NTFY_CODE_MEDIA_RESOLUTION_NOT_SUPPORT==e_nfy_code)
                    {
                        if (pt_cmpb_inst->pf_notify)
                        {
                            pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_RESOLUTION_NOT_SUPPORT, 
                                                    pt_cmpb_inst->pv_tag,
                                                    0
                                                    );
                        }
                    }
                    if (pt_cmpb_inst->pf_notify)
                    {
                        pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_PLAYBACK_ERROR, 
                                                pt_cmpb_inst->pv_tag,
                                                IMTK_PB_CTRL_ERROR_VIDEO_UNPLAYABLE
                                                );
                    }
                }
                pt_cmpb_inst->ui4_av_update |= IMTK_PB_CTRL_PLAY_FLAG_VIDEO;
            }

            if (!pt_cmpb_inst->fg_play_normal)
            {
                UINT32 ui4_play_flag;
                UINT32 ui4_av_info;
                UINT32 ui4_av_update;

                ui4_play_flag = pt_cmpb_inst->ui4_play_flag;
                ui4_av_info = pt_cmpb_inst->ui4_av_info;
                ui4_av_update = pt_cmpb_inst->ui4_av_update;

                if ((pt_cmpb_inst->ui4_av_update&IMTK_PB_CTRL_PLAY_FLAG_VIDEO) ||
                    (0 == ((ui4_play_flag&ui4_av_info)&(~ui4_av_update))))
                {
                    SM_SESS_GNRC_UOP_CAP_INFO_T t_cap_info;
                        
                    if ((ui4_play_flag&ui4_av_info)&IMTK_PB_CTRL_PLAY_FLAG_VIDEO)
                    {
                        if (pt_cmpb_inst->pf_notify)
                        {
                            pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_ASP_UPDATE, 
                                                    pt_cmpb_inst->pv_tag,
                                                    0
                                                    );
                        }
                    }

                    CMPB_MEMSET(&t_cap_info, 0, sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T));
                    t_cap_info.e_uop = _SM_SESS_GNRC_UOP_TRICK;
                    t_cap_info.u.t_speed = MM_SPEED_TYPE_PAUSE;

                    i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                pt_cmpb_inst->h_svctx_hdl,
                                pt_cmpb_inst->h_media_hdl,
                                SVCTX_MM_GET_TYPE_UOP_CAP_INFO,
                                (VOID*)&t_cap_info,
                                sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T));
    
                    if ((SVCTXR_OK == i4_ret) && (t_cap_info.b_is_allowed))
                    {
                        pt_cmpb_inst->fg_play_normal = TRUE;
                        e_event_type = IMTK_PB_CTRL_EVENT_PLAY_DONE;
                        b_event_need_trigger = TRUE;
                    }
                }
            }
            break;
        case SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC:
        case SVCTX_NTFY_CODE_VIDEO_ONLY_SVC:
        {
            SCC_VID_MODE_T e_mode = SCC_VID_DEINT;
            CMPB_SVCTX_SET(pt_cmpb_inst->h_svctx_hdl, 
                        SVCTX_COMMON_SET_DEFAULT_VIDEO_MODE, 
                        (VOID *)&e_mode, 
                        sizeof(SCC_VID_MODE_T)); 
        }
            break;
        case SVCTX_NTFY_CODE_AUDIO_ONLY_SVC:
            #if 0
            pt_cmpb_inst->ui4_data  = (UINT32)IMTK_PB_CTRL_ERROR_VIDEO_UNPLAYABLE;
            e_event_type = IMTK_PB_CTRL_EVENT_PLAYBACK_ERROR;
            b_event_need_trigger = TRUE;    //do not send video unplayble when in audio only svc
            #endif
            break;

        case SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC:
            if(pt_cmpb_inst->u_media.t_get_info.eMediaType != IMTK_PB_CTRL_MEDIA_TYPE_OGG)
            {
                pt_cmpb_inst->ui4_data  = (UINT32)IMTK_PB_CTRL_ERROR_VIDEO_UNPLAYABLE | 
                                      (UINT32)IMTK_PB_CTRL_ERROR_AUDIO_UNPLAYABLE;
            }
            else 
            {
                pt_cmpb_inst->ui4_data  = (UINT32)IMTK_PB_CTRL_ERROR_AUDIO_UNPLAYABLE;
            }
            e_event_type = IMTK_PB_CTRL_EVENT_PLAYBACK_ERROR;
            b_event_need_trigger = TRUE;   
            break;

        case SVCTX_NTFY_CODE_IN_STOPPING:        
            break;

        case SVCTX_NTFY_CODE_MEDIA_EOS:
            if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PUSH)
            {
                e_event_type = IMTK_PB_CTRL_EVENT_EOS;
            }
            else
            {
                 e_event_type = IMTK_PB_CTRL_EVENT_EOS;
                //e_event_type = IMTK_PB_CTRL_EVENT_TOTAL_TIME_UPDATE; /*force send time code to AP, or else when play to end, the timecode not equal to duration*/
            }
            b_event_need_trigger = TRUE;   
            
            pt_cmpb_inst->fg_eos = TRUE;       
            break;
		#if 1
		case SVCTX_NTFY_CODE_MEDIA_BOS: 
			//fprintf(stderr, "cmpb get BOS \n");
			//fflush(stderr);
			b_event_need_trigger = TRUE; 
            e_event_type = IMTK_PB_CTRL_EVENT_BOS;
			break;	
		#endif
        case SVCTX_NTFY_CODE_STOPPED:
            //if (pt_cmpb_inst->fg_eos)
            {
                pt_cmpb_inst->e_cur_state = CMPB_CTRL_STOPPED;
                pt_cmpb_inst->ui4_last_timecode = 0;
                e_event_type = IMTK_PB_CTRL_EVENT_STOPPED;
                b_event_need_trigger = TRUE;
            }
            break;
            
        case SVCTX_NTFY_CODE_MEDIA_LOST:
            pt_cmpb_inst->ui4_data = 0;
            e_event_type = IMTK_PB_CTRL_EVENT_PLAYBACK_ERROR;
            b_event_need_trigger = TRUE;
            break;

        case SVCTX_NTFY_CODE_MEDIA_EOF:
            e_event_type = IMTK_PB_CTRL_EVENT_EOF;
            b_event_need_trigger = TRUE;
            break;
        case SVCTX_NTFY_CODE_DRM_NONE:
        case SVCTX_NTFY_CODE_DRM_UNKNOWN_ERR:
        case SVCTX_NTFY_CODE_DRM_LIC_EXPIRATION_ERR:
        case SVCTX_NTFY_CODE_DRM_SRVDATA_ACCESS_FAIL:
        case SVCTX_NTFY_CODE_DRM_INV_BUSINESS_TKN:
        case SVCTX_NTFY_CODE_DRM_CERT_EXPIRED:
        case SVCTX_NTFY_CODE_DRM_LIC_NOT_ALLOW:
        case SVCTX_NTFY_CODE_DRM_SRV_END:
        case SVCTX_NTFY_CODE_DRM_SRV_UNAVAIL:
        case SVCTX_NTFY_CODE_DRM_SRV_BUSY:
        case SVCTX_NTFY_CODE_DRM_DEV_NOT_REG:
        case SVCTX_NTFY_CODE_DRM_CONTENT_EXHAUSTED:
        case SVCTX_NTFY_CODE_DRM_UNKNOWN_SRV_ERR:
        case SVCTX_NTFY_CODE_DRM_INV_USR_AGENT:
        case SVCTX_NTFY_CODE_DRM_UNSUPPORTED_VER:
        case SVCTX_NTFY_CODE_DRM_INV_CONTENT_TYPE:
        case SVCTX_NTFY_CODE_DRM_INV_SRV_TKN:
        case SVCTX_NTFY_CODE_DRM_INV_ASSET_ID:  
        case SVCTX_NTFY_CODE_DRM_INIT_ERROR:
        case SVCTX_NTFY_CODE_DRM_LICENSE_EXPIRED_ERROR:
        case SVCTX_NTFY_CODE_DRM_LICENSE_ACQUISITION_ERROR:
        case SVCTX_NTFY_CODE_DRM_LICENSE_SERVER_CONNECTION_ERROR:
        case SVCTX_NTFY_CODE_DRM_TOKEN_ACQUISITION_ERROR:
        case SVCTX_NTFY_CODE_DRM_CAD_ACQUISITION_ERROR:
            e_event_type = (IMTK_PB_CTRL_EVENT_T)(IMTK_PB_CTRL_EVENT_DRM_STATUS_START + (e_nfy_code - SVCTX_NTFY_CODE_DRM_STATUS_START));
            if (pt_cmpb_inst->pf_notify)
            {
                pt_cmpb_inst->ui4_data = 0;
                pt_cmpb_inst->pf_notify(e_event_type, 
                                        pt_cmpb_inst->pv_tag,
                                        pt_cmpb_inst->ui4_data);
            }
            break;
            
        default :
            break;
    }
    CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);

    if (b_event_need_trigger)
    {
        if(e_event_type != IMTK_PB_CTRL_EVENT_TIMESEEK_DONE &&
           e_event_type != IMTK_PB_CTRL_EVENT_PLAYBACK_ERROR)
        {
            pt_cmpb_inst->ui4_data = 0;
        }
        /*if (IMTK_PB_CTRL_EVENT_BITRATE_UPDATE == e_event_type)
        {
            SIZE_T  z_size;

            z_size = sizeof(UINT32);
            CMPB_SVCTX_GET(
                    pt_cmpb_inst->h_svctx_hdl,
                    SVCTX_MM_GET_VS_BIT_RATE,
                    (VOID*)&pt_cmpb_inst->ui4_data,
                    &z_size
                    );
        }
        else*/ if(IMTK_PB_CTRL_EVENT_CUR_TIME_UPDATE == e_event_type)
        {
            UINT32 ui4_time;
            UINT64 ui8_pos;
            IMTK_PB_ERROR_CODE_T e_ret_code;

            ui4_time = 0;
            ui8_pos  = 0;
            
	      if (!pt_cmpb_inst->fg_play_normal)
            {
                UINT32 ui4_play_flag;
                UINT32 ui4_av_info;
                UINT32 ui4_av_update;

                ui4_play_flag = pt_cmpb_inst->ui4_play_flag;
                ui4_av_info = pt_cmpb_inst->ui4_av_info;
                ui4_av_update = pt_cmpb_inst->ui4_av_update;

                if (pt_cmpb_inst->ui4_av_info &&
                    ((pt_cmpb_inst->ui4_av_update&IMTK_PB_CTRL_PLAY_FLAG_VIDEO) ||
                    (0 == ((ui4_play_flag&ui4_av_info)&(~ui4_av_update)))))
                {
                    SM_SESS_GNRC_UOP_CAP_INFO_T t_cap_info;

                    CMPB_MEMSET(&t_cap_info, 0, sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T));
                    t_cap_info.e_uop = _SM_SESS_GNRC_UOP_TRICK;
                    t_cap_info.u.t_speed = MM_SPEED_TYPE_PAUSE;

                    i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                pt_cmpb_inst->h_svctx_hdl,
                                pt_cmpb_inst->h_media_hdl,
                                SVCTX_MM_GET_TYPE_UOP_CAP_INFO,
                                (VOID*)&t_cap_info,
                                sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T));

                    if ((SVCTXR_OK == i4_ret) && (t_cap_info.b_is_allowed))
                    {
                        pt_cmpb_inst->fg_play_normal = TRUE;

                        if(pt_cmpb_inst->b_need_set_rgn)
                        {
                            SIZE_T                  z_size;
            
                            z_size = sizeof(VSH_REGION_INFO_T);
            
                            i4_ret = CMPB_SVCTX_SET_STREAM_ATTR(
                                        pt_cmpb_inst->h_svctx_hdl,
                                        ST_VIDEO,
                                        SM_VSH_SET_TYPE_VIDEO_REGION,
                                        (VOID*)&(pt_cmpb_inst->t_vide_rgn),
                                        z_size
                                        );
            
                            if(SVCTXR_OK != i4_ret)
                            {
                                CMPB_DBG_ERROR(("CMPB_SVCTX_SET_STREAM_ATTR(SM_VSH_SET_TYPE_VIDEO_REGION) returns %d\n", i4_ret));
                            }

                            i4_ret = CMPB_SVCTX_SET_STREAM_ATTR(
                                        pt_cmpb_inst->h_svctx_hdl,
                                        ST_VIDEO,
                                        SM_VSH_SET_TYPE_DISP_REGION,
                                        (VOID*)&(pt_cmpb_inst->t_disp_rgn),
                                        z_size
                                        );
            
                            if(SVCTXR_OK != i4_ret)
                            {
                                CMPB_DBG_ERROR(("CMPB_SVCTX_SET_STREAM_ATTR(SM_VSH_SET_TYPE_DISP_REGION) returns %d\n", i4_ret));
                            }
            
                            pt_cmpb_inst->b_need_set_rgn = FALSE;
                        }
                        
    
                        if (pt_cmpb_inst->pf_notify)
                        {
                            pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_PLAY_DONE, 
                                                    pt_cmpb_inst->pv_tag,
                                                    0
                                                    );
                        }
                    }
                }
            }

            if (!pt_cmpb_inst->fg_play_normal || pt_cmpb_inst->fg_during_seek)
            {
                return; /*do not send time update message*/
            }
			
            e_ret_code = IMtkPb_Ctrl_GetCurrentPos_inr(pt_cmpb_inst,
                                          &ui4_time,   
                                          &ui8_pos);
            if(e_ret_code == IMTK_PB_ERROR_CODE_OK)
            {
                if(pt_cmpb_inst->fg_eos)
                {
                    if(pt_cmpb_inst->u_media.t_get_info.u4TotalDuration != 0)
                    {
                        pt_cmpb_inst->ui4_data = pt_cmpb_inst->u_media.t_get_info.u4TotalDuration;
                    }
                    else
                    {
                        pt_cmpb_inst->ui4_data = pt_cmpb_inst->u_media.t_get_info.u8Size;
                    }
                }
                else
                {
                    /*if((pt_cmpb_inst->u_media.t_get_info.u4TotalDuration != 0) || 
                        (pt_cmpb_inst->e_cmpb_profile == IMTK_PB_CTRL_PROFILE_VUDU) || 
                        (pt_cmpb_inst->e_cmpb_profile == IMTK_PB_CTRL_PROFILE_NETFLIX_NRD))*/
                    {
                        pt_cmpb_inst->ui4_data = ui4_time;
                    }
                    /*else if (pt_cmpb_inst->e_model != IMTK_PB_CTRL_BUFFERING_MODEL_PUSH)
                    {
                        pt_cmpb_inst->ui4_data = (UINT32)ui8_pos;                        
                        if (pt_cmpb_inst->pf_notify && !pt_cmpb_inst->b_closing)
                        {
                            pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_POSITION_UPDATE, 
                                                    pt_cmpb_inst->pv_tag,
                                                    pt_cmpb_inst->ui4_data
                                                    );
                        }
                        return;
                    }*/
                }
            }
            else
            {
                return; /*do not send time update message*/
            }
        }   
        else if(IMTK_PB_CTRL_EVENT_TOTAL_TIME_UPDATE == e_event_type)
        {
            MEDIA_FORMAT_T    t_media_fmt;
            BOOL              b_is_ts;
            MM_SVC_DUR_INFO_T t_time_dur;
            
            i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                            pt_cmpb_inst->h_svctx_hdl,
                            pt_cmpb_inst->h_media_hdl,
                            SVCTX_MM_GET_TYPE_MEDIA_FORMAT,
                            (VOID*)&t_media_fmt,
                            sizeof(t_media_fmt)
                            );
            if((pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PULL ||
                pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_URI) && 
                t_media_fmt.e_media_type == MEDIA_TYPE_TS)
            {
                b_is_ts = TRUE;
            }
            if(!pt_cmpb_inst->u_media.t_get_info.u4TotalDuration && b_is_ts)
            {
                return ;
            }    
            
            t_time_dur.t_dur_type = MM_SVC_DUR_TYPE_ALL;
            t_time_dur.ui2_idx    = 0;
            
            i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                            pt_cmpb_inst->h_svctx_hdl,
                            pt_cmpb_inst->h_media_hdl,
                            SVCTX_MM_GET_TYPE_DUR,
                            (VOID*)&t_time_dur,
                            sizeof(t_time_dur)
                            );
            pt_cmpb_inst->ui4_data = (UINT32)(t_time_dur.ui8_duration * 1000) /*milli seconds*/;
            pt_cmpb_inst->u_media.t_get_info.u4TotalDuration = pt_cmpb_inst->ui4_data;
        }  
        /*else if (IMTK_PB_CTRL_EVENT_SPEED_UPDATE == e_event_type)
        {
            uint32_t    z_get_info_size = sizeof(IMTK_PB_CTRL_SPEED_T);
            i4_ret = CMPB_SVCTX_GET(
                        pt_cmpb_inst->h_svctx_hdl,
                        SVCTX_MM_GET_TYPE_SPEED,
                        &t_mm_speed,
                        &z_get_info_size);
            if (i4_ret == SVCTXR_OK)
            {
                i4_ret = _cmpb_ctrl_speed_convert(&t_cmpb_speed, &t_mm_speed, FALSE);
                if (i4_ret == IMTK_PB_CB_ERROR_CODE_OK)
                {
                    pt_cmpb_inst->ui4_data = (UINT32)t_cmpb_speed;
                    pt_cmpb_inst->e_cur_speed = t_cmpb_speed;
                }
            }
        }*/
        else if (b_video_fmt_update)
        {
            if (e_event_type == IMTK_PB_CTRL_EVENT_PLAY_DONE)
            {
                /*if (pt_cmpb_inst->pf_notify)
                {
                    pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_ASP_UPDATE, 
                                            pt_cmpb_inst->pv_tag,
                                            pt_cmpb_inst->ui4_data);
                }*/
                if(pt_cmpb_inst->b_need_set_rgn)
                {
                    SIZE_T                  z_size;

                    z_size = sizeof(VSH_REGION_INFO_T);

                    i4_ret = CMPB_SVCTX_SET_STREAM_ATTR(
                                pt_cmpb_inst->h_svctx_hdl,
                                ST_VIDEO,
                                SM_VSH_SET_TYPE_VIDEO_REGION,
                                (VOID*)&(pt_cmpb_inst->t_vide_rgn),
                                z_size
                                );

                    if(SVCTXR_OK != i4_ret)
                    {
                        CMPB_DBG_ERROR(("CMPB_SVCTX_SET_STREAM_ATTR(SM_VSH_SET_TYPE_VIDEO_REGION) returns %d\n", i4_ret));
                    }

                    i4_ret = CMPB_SVCTX_SET_STREAM_ATTR(
                                pt_cmpb_inst->h_svctx_hdl,
                                ST_VIDEO,
                                SM_VSH_SET_TYPE_DISP_REGION,
                                (VOID*)&(pt_cmpb_inst->t_disp_rgn),
                                z_size
                                );

                    if(SVCTXR_OK != i4_ret)
                    {
                        CMPB_DBG_ERROR(("CMPB_SVCTX_SET_STREAM_ATTR(SM_VSH_SET_TYPE_DISP_REGION) returns %d\n", i4_ret));
                    }

                    pt_cmpb_inst->b_need_set_rgn = FALSE;
                }

                /*if (pt_cmpb_inst->pf_notify)
                {
                    pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_ASP_UPDATE, 
                                            pt_cmpb_inst->pv_tag,
                                            pt_cmpb_inst->ui4_data
                                            );
                }*/
            }
            else
            {
                //e_event_type = IMTK_PB_CTRL_EVENT_ASP_UPDATE;
            }
        }
        
        if (pt_cmpb_inst->pf_notify && !pt_cmpb_inst->b_closing)
        {
            pt_cmpb_inst->pf_notify(e_event_type, 
                                    pt_cmpb_inst->pv_tag,
                                    pt_cmpb_inst->ui4_data
                                    );
        }
    }
}


static UINT32 _cmpb_buffer_status_cb(
                    HANDLE_T                    h_svctx,
                    SVCTX_BUFCTL_NTFY_CODE_T    e_code,
                    VOID*                       pv_nfy_tag,
                    UINT32                      ui4_data
                    )
{
    CMPB_CTRL_INST_T*       pt_cmpb_inst;
    
    pt_cmpb_inst = (CMPB_CTRL_INST_T*)pv_nfy_tag;
    
    if (pt_cmpb_inst->b_preload_to_play)
    {
        return SVCTXR_OK;
    }
    else
    {
        return SVCTXR_PENDING;    
    }
    
}

static BOOL _cmpb_video_strm_comp_filter(
                    UINT16                          ui2_num_recs,     /* in  */
                    UINT16                          ui2_idx,          /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    const SCDB_REC_T*               pt_scdb_rec,      /* in  */
                    VOID*                           pv_fltr_tag       /* in  */
                    )
{
    if((NULL == pt_comp_id) || (NULL == pt_scdb_rec))
    {
        CMPB_ASSERT(FALSE)
        return FALSE;
    }
    
    return TRUE;
}

static BOOL _cmpb_audio_strm_comp_filter(
                    UINT16                          ui2_num_recs,     /* in  */
                    UINT16                          ui2_idx,          /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    const SCDB_REC_T*               pt_scdb_rec,      /* in  */
                    VOID*                           pv_fltr_tag       /* in  */
                    )
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    IMTK_PB_HANDLE_T            hHandle;

    hHandle = (IMTK_PB_HANDLE_T)pv_fltr_tag;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return FALSE;
    }

    if((NULL == pt_comp_id) || (NULL == pt_scdb_rec))
    {
        CMPB_ASSERT(FALSE)
        return FALSE;
    }
    
    if (pt_cmpb_inst->t_src_sub_type == SRC_SUB_TYPE_VESTEL_TS &&
        pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_AUDIO_MPEG)
    {
        if (pt_scdb_rec->u.t_audio_mpeg.ui2_pid == pt_cmpb_inst->ui2_pvr_audio_pid)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    
    if (ui2_idx+1 == pt_cmpb_inst->t_audio_info.ui2_sel_idx)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _cmpb_subtitle_strm_comp_filter(
                    UINT16                          ui2_num_recs,     /* in  */
                    UINT16                          ui2_idx,          /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    const SCDB_REC_T*               pt_scdb_rec,      /* in  */
                    VOID*                           pv_fltr_tag       /* in  */
                    )
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    IMTK_PB_HANDLE_T            hHandle;

    hHandle = (IMTK_PB_HANDLE_T)pv_fltr_tag;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return FALSE;
    }

    if((NULL == pt_comp_id) || (NULL == pt_scdb_rec))
    {
        CMPB_ASSERT(FALSE)
        return FALSE;
    }

    if (ui2_idx+1 == pt_cmpb_inst->t_subtitle_info.ui2_sel_idx)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _cmpb_subtitle_strm_comp_iter(
                    UINT16                          ui2_num_recs,     /* in  */
                    UINT16                          ui2_idx,          /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    const SCDB_REC_T*               pt_scdb_rec,      /* in  */
                    VOID*                           pv_fltr_tag       /* in  */
                    )
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    IMTK_PB_HANDLE_T            hHandle;

    hHandle = (IMTK_PB_HANDLE_T)pv_fltr_tag;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return FALSE;
    }

    if((NULL == pt_comp_id) || (NULL == pt_scdb_rec))
    {
        CMPB_ASSERT(FALSE)
        return FALSE;
    }

    if((MAX_GET_SUB_NUM <= ui2_idx) ||
       (ui2_num_recs <= ui2_idx))
    {
        return TRUE;
    }

    /* update subtitle info */
    CMPB_MEMCPY(&pt_cmpb_inst->t_subtitle_info.at_comp_id[ui2_idx], pt_comp_id, sizeof(STREAM_COMP_ID_T));

    if (pt_scdb_rec->e_rec_type  == SCDB_REC_TYPE_SUBTITLE_DIVX)
    {
        CMPB_STRNCPY(pt_cmpb_inst->t_subtitle_info.as_lang[ui2_idx], 
                    pt_scdb_rec->u.t_subtitle_avi.s_lang,
                    SUB_LANG_LEN-1);
    }
    else if (pt_scdb_rec->e_rec_type  == SCDB_REC_TYPE_SUBTITLE_MKV)
    {
        CMPB_STRNCPY(pt_cmpb_inst->t_subtitle_info.as_lang[ui2_idx], 
                    pt_scdb_rec->u.t_subtitle_mkv.s_lang,
                    SUB_LANG_LEN-1);
    }
    else if(pt_scdb_rec->e_rec_type  == SCDB_REC_TYPE_SUBTITLE_DVB)
    {
        CMPB_STRNCPY(pt_cmpb_inst->t_subtitle_info.as_lang[ui2_idx], 
                    pt_scdb_rec->u.t_dvb_sbtl.s_lang,
                    SUB_LANG_LEN-1);
    }

    pt_cmpb_inst->t_subtitle_info.ui2_sub_num++;

    return FALSE;
}


static BOOL _cmpb_audio_strm_comp_iter(
                    UINT16                          ui2_num_recs,     /* in  */
                    UINT16                          ui2_idx,          /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    const SCDB_REC_T*               pt_scdb_rec,      /* in  */
                    VOID*                           pv_fltr_tag       /* in  */
                    )
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    IMTK_PB_HANDLE_T            hHandle;

    hHandle = (IMTK_PB_HANDLE_T)pv_fltr_tag;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return FALSE;
    }

    if((NULL == pt_comp_id) || (NULL == pt_scdb_rec))
    {
        CMPB_ASSERT(FALSE)
        return FALSE;
    }

    if((MAX_GET_AUD_NUM <= ui2_idx) ||
       (ui2_num_recs <= ui2_idx))
    {
        return TRUE;
    }

    /* update audio info */
    CMPB_MEMCPY(&pt_cmpb_inst->t_audio_info.at_comp_id[ui2_idx], pt_comp_id, sizeof(STREAM_COMP_ID_T));
    CMPB_MEMCPY(&pt_cmpb_inst->t_audio_info.at_scdb_rec[ui2_idx], pt_scdb_rec, sizeof(SCDB_REC_T));

    pt_cmpb_inst->t_audio_info.ui2_aud_num++;

    return FALSE;
}

static BOOL _cmpb_video_strm_comp_iter(
                    UINT16                          ui2_num_recs,     /* in  */
                    UINT16                          ui2_idx,          /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    const SCDB_REC_T*               pt_scdb_rec,      /* in  */
                    VOID*                           pv_fltr_tag       /* in  */
                    )
{
    return TRUE;
}

static IMTK_PB_ERROR_CODE_T _cmpb_ctrl_speed_convert(IMTK_PB_CTRL_SPEED_T*    e_cmpb_speed,
                                           MM_SPEED_TYPE_T*         e_mm_speed,
                                           bool                     b_is_cmpb_to_mm)
{
    if (e_cmpb_speed == NULL || e_mm_speed == NULL)
    {
        return IMTK_PB_CB_ERROR_CODE_NOT_OK;
    }
    
    if (b_is_cmpb_to_mm)
    {
        switch (*e_cmpb_speed)
        {
            case IMTK_PB_CTRL_SPEED_FR_32X:
                *e_mm_speed = MM_SPEED_TYPE_REWIND_32X;
                break;
            case IMTK_PB_CTRL_SPEED_FR_16X:
                *e_mm_speed = MM_SPEED_TYPE_REWIND_16X;
                break;
            case IMTK_PB_CTRL_SPEED_FR_8X:
                *e_mm_speed = MM_SPEED_TYPE_REWIND_8X;
                break;
            case IMTK_PB_CTRL_SPEED_FR_4X:
                *e_mm_speed = MM_SPEED_TYPE_REWIND_4X;
                break;
            case IMTK_PB_CTRL_SPEED_FR_2X:
                *e_mm_speed = MM_SPEED_TYPE_REWIND_2X;
                break;
            case IMTK_PB_CTRL_SPEED_FR_1X:
                *e_mm_speed = MM_SPEED_TYPE_REWIND_1X;
                break;
            case IMTK_PB_CTRL_SPEED_FF_32X:
                *e_mm_speed = MM_SPEED_TYPE_FORWARD_32X;
                break;
            case IMTK_PB_CTRL_SPEED_FF_16X:
                *e_mm_speed = MM_SPEED_TYPE_FORWARD_16X;
                break;
            case IMTK_PB_CTRL_SPEED_FF_8X:
                *e_mm_speed = MM_SPEED_TYPE_FORWARD_8X;
                break;
            case IMTK_PB_CTRL_SPEED_FF_4X:
                *e_mm_speed = MM_SPEED_TYPE_FORWARD_4X;
                break;
            case IMTK_PB_CTRL_SPEED_FF_2X:
                *e_mm_speed = MM_SPEED_TYPE_FORWARD_2X;
                break;
            case IMTK_PB_CTRL_SPEED_FF_1X:
                *e_mm_speed = MM_SPEED_TYPE_FORWARD_1X;
                break;     
            case IMTK_PB_CTRL_SPEED_ZERO:
                *e_mm_speed = MM_SPEED_TYPE_PAUSE;
                break;
            default:
                return IMTK_PB_CB_ERROR_CODE_NOT_OK;
           
        }
    }
    else
    {
        switch (*e_mm_speed)
        {
            case MM_SPEED_TYPE_REWIND_32X:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_FR_32X;
                break;
            case MM_SPEED_TYPE_REWIND_16X:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_FR_16X;
                break;
            case MM_SPEED_TYPE_REWIND_8X:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_FR_8X;
                break;
            case MM_SPEED_TYPE_REWIND_4X:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_FR_4X;
                break;
            case MM_SPEED_TYPE_REWIND_2X:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_FR_2X;
                break;
            case MM_SPEED_TYPE_REWIND_1X:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_FR_1X;
                break;
            case MM_SPEED_TYPE_FORWARD_32X:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_FF_32X;
                break;
            case MM_SPEED_TYPE_FORWARD_16X:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_FF_16X;
                break;
            case MM_SPEED_TYPE_FORWARD_8X:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_FF_8X;
                break;
            case MM_SPEED_TYPE_FORWARD_4X:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_FF_4X;
                break;
            case MM_SPEED_TYPE_FORWARD_2X:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_FF_2X;
                break;
            case MM_SPEED_TYPE_FORWARD_1X:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_FF_1X;
                break;     
            case MM_SPEED_TYPE_PAUSE:
                *e_cmpb_speed = IMTK_PB_CTRL_SPEED_ZERO;
                break;
            default:
                return IMTK_PB_CB_ERROR_CODE_NOT_OK;
           
        }
    }

    return IMTK_PB_CB_ERROR_CODE_OK;
}

static BOOL _cmpb_ctrl_set_speed(CMPB_CTRL_INST_T* pt_cmpb_inst, 
                                 IMTK_PB_CTRL_SPEED_T e_speed)
{

    MM_SVC_TRICK_INFO_T         t_trick;
    INT32                       i4_ret;
    
    if (NULL == pt_cmpb_inst)
    {
        return FALSE;
    }

    switch(e_speed)
    {
        case IMTK_PB_CTRL_SPEED_FR_32X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_32X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_16X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_16X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_8X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_8X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_4X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_4X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_3X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_3X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_2X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_2X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_1X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_1X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_1_2X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_1_DIV_2X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_1_3X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_1_DIV_3X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_1_4X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_1_DIV_4X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_1_8X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_1_DIV_8X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_1_16X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_1_DIV_16X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_1_32X:
            t_trick.e_speed_type = MM_SPEED_TYPE_REWIND_1_DIV_32X;
            break;
            
        case IMTK_PB_CTRL_SPEED_ZERO:
            t_trick.e_speed_type = MM_SPEED_TYPE_PAUSE;
            break;
            
        case IMTK_PB_CTRL_SPEED_FF_1_32X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_1_DIV_32X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_1_16X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_1_DIV_16X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_1_8X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_1_DIV_8X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_1_4X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_1_DIV_4X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_1_3X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_1_DIV_3X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_1_2X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_1_DIV_2X;
            break;
        case IMTK_PB_CTRL_SPEED_1X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_2X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_2X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_3X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_3X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_4X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_4X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_8X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_8X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_16X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_16X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_32X:
            t_trick.e_speed_type = MM_SPEED_TYPE_FORWARD_32X;
            break;        

        default:
            return FALSE;
    }
       
    t_trick.u.ui4_data = 0;
    
    i4_ret = CMPB_SVCTX_SET(
                    pt_cmpb_inst->h_svctx_hdl,
                    SVCTX_MM_SET_TYPE_TRICK,
                    (VOID*)&t_trick,
                    sizeof(MM_SVC_TRICK_INFO_T)
                    );
                    
    if(SVCTXR_OK != i4_ret)
    {
        CMPB_DBG_ERROR(("CMPB_SVCTX_SET() returns %d\n", i4_ret));
        return FALSE;
    }

    return TRUE;

}


static BOOL _cmpb_ctrl_uop_cab_check(IMTK_PB_HANDLE_T hHandle , CMPB_UOP_TYPE_T    e_uop)
{
     CMPB_CTRL_INST_T*   pt_cmpb_inst = NULL;    
     _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
     if (NULL == pt_cmpb_inst)
     {
          return FALSE;
     }
     /*case 1 : not allow any uop except STOP while seeking*/
     if((pt_cmpb_inst->fg_during_seek && (e_uop != CMPB_UOP_TYPE_STOP)) || 
         pt_cmpb_inst->b_open_media_error ||
         (pt_cmpb_inst->fg_eos && (e_uop != CMPB_UOP_TYPE_STOP)))
     {
          return FALSE;
     }
     return TRUE;
}

static INT32 _set_video_freeze(BOOL b_enable)
{
    SM_COMMAND_T    at_sm_cmds[4];
    INT32           i4_ret;
    HANDLE_T        h_scc_vid = NULL_HANDLE;
    SCC_VID_SUPER_FREEZE_T  t_freeze;
    
    /* create the scc handles */
    at_sm_cmds[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data = SCC_CMD_TYPE_VIDEO;
    at_sm_cmds[1].e_code     = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
    at_sm_cmds[2].e_code     = SM_CMD_CODE_END;
    at_sm_cmds[2].u.ui4_data = 0;
    i4_ret = CMPB_C_SCC_COMP_OPEN(at_sm_cmds, &h_scc_vid);
    if(i4_ret != SMR_OK)
    {
        return SVCTXR_FAIL;
    }

    t_freeze.e_type = SCC_VID_SUPER_FREEZE_TYPE_FORCE;
    t_freeze.b_enable = b_enable;
    i4_ret = CMPB_C_SCC_VID_SET_SUPER_FREEZE(h_scc_vid, &t_freeze);

    CMPB_C_SCC_COMP_CLOSE(h_scc_vid);

    return i4_ret;

}

/*-----------------------------------------------------------------------------
 *                   External functions implementations
 ----------------------------------------------------------------------------*/
IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Init(void)
{    
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Terminate(void)
{   
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CustomizeHttpReqHdr(IMTK_PB_HANDLE_T hHandle, 
                                                             uint8_t*         pu1String)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_PlayEx(IMTK_PB_HANDLE_T   hHandle,
                                            uint32_t           u4Time,
                                            IMTK_PB_CTRL_PLAY_INFO_T* ptPlayInfo)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_TimeSeekEx(IMTK_PB_HANDLE_T   hHandle,
                                                  uint32_t           u4Time,
                                                  IMTK_PB_CTRL_TIME_SEEK_INFO_T* ptTimeSeekInfo)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetPlayMode(IMTK_PB_HANDLE_T                hHandle,
                                                  IMTK_PB_CTRL_PLAY_MODE_INFO_T*  ptPlayModeInfo)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Open(IMTK_PB_HANDLE_T*                 phHandle,
                                        IMTK_PB_CTRL_BUFFERING_MODEL_T    eBufferingModel,
                                        IMTK_PB_CTRL_OPERATING_MODEL_T    eOperatingModel,
                                        uint8_t*                          pu1Profile)
{

    INT32                   i4_ret;
    CMPB_CTRL_INST_T*       pt_cmpb_inst = NULL;
    IMTK_PB_ERROR_CODE_T    e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    HANDLE_T                h_svctx_hdl = NULL_HANDLE;

    /*printf("IMtkPb_Ctrl_Open\n");*/

    if (NULL_HANDLE == h_main_svctx)
    {
        CMPB_SVCTX_OPEN(CMPB_SVCTX_NAME_MAIN, &(h_main_svctx));
        CMPB_SVCTX_OPEN(CMPB_SVCTX_NAME_SUB, &(h_sub_svctx));
        CMPB_SVCTX_OPEN(CMPB_SVCTX_NAME_THIRD, &(h_thrd_svctx));
        
#ifdef MW_TV_AV_BYPASS_SUPPORT
        CMPB_SVCTX_OPEN(CMPB_SVCTX_NAME_TV_BYPASS, &(h_tv_bypass_svctx));
        CMPB_SVCTX_OPEN(CMPB_SVCTX_NAME_MONITOR_BYPASS, &(h_monitor_bypass_svctx));
#endif
    }
    
    do
    {
        _cmpb_cb_init();

        pt_cmpb_inst = (CMPB_CTRL_INST_T*)CMPB_MEM_CALLOC(1,sizeof(CMPB_CTRL_INST_T));

        if (NULL == pt_cmpb_inst)
        {
            e_ret_code = IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
            break;
        }
        CMPB_MEMSET(pt_cmpb_inst, 0, sizeof(CMPB_CTRL_INST_T));
        
        i4_ret = CMPB_SEMA_CREATE(
                                &(pt_cmpb_inst->h_sema),
                                X_SEMA_TYPE_BINARY,
                                X_SEMA_STATE_LOCK
                                );
        if(OSR_OK != i4_ret)
        {
            e_ret_code = IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
            break;
        }

        if(eBufferingModel == IMTK_PB_CTRL_BUFFERING_MODEL_URI)
        {
            i4_ret = CMPB_SEMA_CREATE(
                                &(pt_cmpb_inst->h_uri_sema),
                                X_SEMA_TYPE_BINARY,
                                X_SEMA_STATE_LOCK
                                );
            if(OSR_OK != i4_ret)
            {
                e_ret_code = IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
                break;
            }
        }

        _cmpb_get_new_handle(pt_cmpb_inst, phHandle);
        
        if (*phHandle == INVALIDE_CMPB_HANDLE)
        {
            break;
        }

        pt_cmpb_inst->h_cmpb = *phHandle;
        pt_cmpb_inst->e_model = eBufferingModel;
        pt_cmpb_inst->b_app_set_media_info = (BOOL)(eOperatingModel == IMTK_PB_CTRL_APP_MASTER);

        /* init c_svctx instance */
        if (pu1Profile)
        {
            pt_cmpb_inst->t_src_sub_type = SRC_SUB_TYPE_NETWORK_UNKNOWN;
            if (0==CMPB_STRCMP("main_svctx", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->ui1_group_mode = 0;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_MAIN_SVCTX;
                i4_ret = CMPB_SVCTX_OPEN("main_svctx", &(h_svctx_hdl));
            }
            else if (0==CMPB_STRCMP("sub_svctx", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->ui1_group_mode = 1;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_SUB_SVCTX;
                i4_ret = CMPB_SVCTX_OPEN("sub_svctx", &(h_svctx_hdl));
            }
            else if (0==CMPB_STRCMP("thrd_svctx", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->ui1_group_mode = 2;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_THRD_SVCTX;
                i4_ret = CMPB_SVCTX_OPEN("thrd_svctx", &(h_svctx_hdl));
            }
            else if (0==CMPB_STRCMP("NETFLIX_NRD2.1", (CHAR*)pu1Profile) ||
                     0==CMPB_STRCMP("NETFLIX_NRD3.1", (CHAR*)pu1Profile) ||
                     0==CMPB_STRCMP("NETFLIX_NRD", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->t_src_sub_type = SRC_SUB_TYPE_NETWORK_NETFLIX;
                pt_cmpb_inst->ui1_group_mode = 0;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_NETFLIX_NRD;
                i4_ret = CMPB_SVCTX_OPEN("main_svctx", &(h_svctx_hdl));
            }
            else if (0==CMPB_STRCMP("MAIN_MP3_ONLY", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->t_src_sub_type = SRC_SUB_TYPE_NETWORK_MP3_ONLY;
                pt_cmpb_inst->ui1_group_mode = 0;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_MAIN_MP3_ONLY;
                i4_ret = CMPB_SVCTX_OPEN("main_svctx", &(h_svctx_hdl));
            }
            else if (0==CMPB_STRCMP("skype_main_video", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->t_src_sub_type = SRC_SUB_TYPE_NETWORK_SKYPE;
                pt_cmpb_inst->ui1_group_mode = 0;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_SKYPE_MAIN_VIDEO;
                i4_ret = CMPB_SVCTX_OPEN("main_svctx", &(h_svctx_hdl));
            }
            else if (0==CMPB_STRCMP("skype_sub_video", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->t_src_sub_type = SRC_SUB_TYPE_NETWORK_SKYPE;
                pt_cmpb_inst->ui1_group_mode = 1;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_SKYPE_SUB_VIDEO;
                i4_ret = CMPB_SVCTX_OPEN("sub_svctx", &(h_svctx_hdl));
            }
            else if (0==CMPB_STRCMP("skype_remote_audio", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->t_src_sub_type = SRC_SUB_TYPE_NETWORK_SKYPE;
                pt_cmpb_inst->ui1_group_mode = 2;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_SKYPE_REMOTE_AUDIO;
                i4_ret = CMPB_SVCTX_OPEN("thrd_svctx", &(h_svctx_hdl));
            }
            else if (0==CMPB_STRCMP("MAIN_PUSH_ON_SUB", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->ui1_group_mode = 0;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_MAIN_PUSH_ON_SUB;
                i4_ret = CMPB_SVCTX_OPEN("sub_svctx", &(h_svctx_hdl));
            }
            else if (0==x_strcmp("MAIN_PUSH_MHP", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->t_src_sub_type = SRC_SUB_TYPE_PUSH_MHP;
                pt_cmpb_inst->ui1_group_mode = 0;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_MAIN_PUSH_MHP;
                i4_ret = CMPB_SVCTX_OPEN("main_svctx", &(h_svctx_hdl));
            }
            else if (0==x_strcmp("MAIN_PUSH_MHP_SUB", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->t_src_sub_type = SRC_SUB_TYPE_PUSH_MHP;
                pt_cmpb_inst->ui1_group_mode = 0;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_MAIN_PUSH_MHP_SUB;
                i4_ret = CMPB_SVCTX_OPEN("sub_svctx", &(h_svctx_hdl));
            }
            else if (0==x_strcmp("VESTEL_TS", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->t_src_sub_type = SRC_SUB_TYPE_VESTEL_TS;
                pt_cmpb_inst->ui1_group_mode = 0;
                i4_ret = CMPB_SVCTX_OPEN("main_svctx", &(h_svctx_hdl));
            }
            else if (0==x_strcmp("SUB_FREE_RUN", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->t_src_sub_type = SRC_SUB_TYPE_SUB_FREE_RUN;
                pt_cmpb_inst->ui1_group_mode = 1;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_SUB_FREE_RUN;
                i4_ret = CMPB_SVCTX_OPEN("sub_svctx", &(h_svctx_hdl));
            }
            else if (0==x_strcmp("AUTO_DETECT_NEXT_VID", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->ui1_group_mode = 0;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_AUTO_DETECT_NEXT_VID;
                i4_ret = CMPB_SVCTX_OPEN("main_svctx", &(h_svctx_hdl));
            }
            else if (0==x_strcmp("USB", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->ui1_group_mode = 0;
                pt_cmpb_inst->t_src_sub_type = SRC_SUB_TYPE_HIGH_SPEED_STORAGE;
                i4_ret = CMPB_SVCTX_OPEN("main_svctx", &(h_svctx_hdl));
            }
            else if (0==x_strcmp("SUB_USB", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->ui1_group_mode = 1;
                pt_cmpb_inst->t_src_sub_type = SRC_SUB_TYPE_HIGH_SPEED_STORAGE;
                i4_ret = CMPB_SVCTX_OPEN("sub_svctx", &(h_svctx_hdl));
            }  
            else if (0==x_strcmp("VUDU", (CHAR*)pu1Profile))
            {
                pt_cmpb_inst->ui1_group_mode = 0;
                pt_cmpb_inst->t_src_sub_type = IMTK_PB_CTRL_PROFILE_DEFAULT;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_VUDU;
                i4_ret = CMPB_SVCTX_OPEN("main_svctx", &(h_svctx_hdl));
            }  
            else
            {
                pt_cmpb_inst->ui1_group_mode = 0;
                pt_cmpb_inst->e_cmpb_profile = IMTK_PB_CTRL_PROFILE_DEFAULT;
                i4_ret = CMPB_SVCTX_OPEN("main_svctx", &(h_svctx_hdl));
            }
        }
        else
        {
            i4_ret = CMPB_SVCTX_OPEN(CMPB_SVCTX_NAME_MAIN, &(h_svctx_hdl));
            pt_cmpb_inst->ui1_group_mode = 0;
        }

        if(SVCTXR_OK != i4_ret)
        {
            CMPB_DBG_ERROR(("CMPB_SVCTX_OPEN() returns %d\n", i4_ret));
            break;
        }

        pt_cmpb_inst->h_svctx_hdl = h_svctx_hdl;
        pt_cmpb_inst->h_media_hdl = NULL_HANDLE;

        pt_cmpb_inst->t_vide_rgn.ui4_x = 0;
        pt_cmpb_inst->t_vide_rgn.ui4_y = 0;
        pt_cmpb_inst->t_vide_rgn.ui4_width  = VSH_REGION_MAX_WIDTH;
        pt_cmpb_inst->t_vide_rgn.ui4_height = VSH_REGION_MAX_HEIGHT;

        pt_cmpb_inst->t_disp_rgn.ui4_x = 0;
        pt_cmpb_inst->t_disp_rgn.ui4_y = 0;
        pt_cmpb_inst->t_disp_rgn.ui4_width  = VSH_REGION_MAX_WIDTH;
        pt_cmpb_inst->t_disp_rgn.ui4_height = VSH_REGION_MAX_HEIGHT;  

        pt_cmpb_inst->b_preload_to_play = FALSE;

        if (eOperatingModel == IMTK_PB_CTRL_APP_MASTER)
        {
            pt_cmpb_inst->e_cur_state = CMPB_CTRL_TO_SETINFO;
        }
        else
        {
            pt_cmpb_inst->e_cur_state = CMPB_CTRL_OPENED;
        }

        if(pt_cmpb_inst->h_time_code_update == NULL_HANDLE)
        {
            i4_ret = CMPB_TIMER_CREATE(&pt_cmpb_inst->h_time_code_update);
            if (i4_ret != OSR_OK) 
            {
                CMPB_DBG_ERROR(("CMPB_TIMER_CREATE() returns %d\n", i4_ret));
                break;
            }
        }

        i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
        if(OSR_OK != i4_ret)
        {
            
        }
                
        return IMTK_PB_ERROR_CODE_OK;
        
    } while(0);


    if(pt_cmpb_inst != NULL)
    {
        i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
        if(OSR_OK != i4_ret)
        {
        
        }
        _cmpb_free_instance(pt_cmpb_inst);
    }

    if (NULL != phHandle)
    {
        if (*phHandle != INVALIDE_CMPB_HANDLE)
        {
            _cmpb_free_handle(*phHandle);
        }
    }
    
    /* clean resource if fail */
    
    return e_ret_code;        
    
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Close(IMTK_PB_HANDLE_T  h_Handle)                                      
{
    uint8_t             ui1_i = 0;
    CMPB_CTRL_INST_T*   pt_cmpb_inst = NULL;
    INT32               i4_ret;

    /*printf("IMtkPb_Ctrl_Close\n");*/
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, h_Handle);

    if (pt_cmpb_inst)
    {
        pt_cmpb_inst->b_closing = TRUE;
        //if not parsing done, abort read when playing http file        
        if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_URI ) /*URI mode*/
        {
            while(pt_cmpb_inst->h_uri_file == 0)
            {
                CMPB_THREAD_DELAY(100);
            }
            if(pt_cmpb_inst->h_uri_file != 0xFFFFFFFF)
            {
                i4_ret = CMPB_SVCTX_MEDIA_SET_INFO(
                                    pt_cmpb_inst->h_svctx_hdl,
                                    pt_cmpb_inst->h_media_hdl,
                                    SVCTX_MM_SET_CANCEL_URI_READ,
                                    &pt_cmpb_inst->h_uri_file,
                                    sizeof(HANDLE_T*)
                                    );

                if(SVCTXR_OK != i4_ret)
                {
                    CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_SET_INFO() returns %d\n", i4_ret));
                }
            }
            
            i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_uri_sema, X_SEMA_OPTION_WAIT);
        }  
        
        if (pt_cmpb_inst->b_start_parse)
        {
            while (1)
            {
                if (pt_cmpb_inst->b_parse_done || pt_cmpb_inst->b_open_media_error)
                {
                    break;
                }
                CMPB_THREAD_DELAY(100);
            }
        }

        if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PUSH)
        {
            _set_video_freeze(FALSE);
        }
        
        if(pt_cmpb_inst->h_media_hdl)
        {
            CMPB_SVCTX_MEDIA_CLOSE(pt_cmpb_inst->h_svctx_hdl, pt_cmpb_inst->h_media_hdl);
            pt_cmpb_inst->h_media_hdl = NULL_HANDLE;
        }
        
        CMPB_SVCTX_CLOSE(pt_cmpb_inst->h_svctx_hdl);                

        if(pt_cmpb_inst->pui1_proxy_url != NULL)
        {
            CMPB_MEM_FREE(pt_cmpb_inst->pui1_proxy_url);
            pt_cmpb_inst->pui1_proxy_url = NULL;
        }

        if(pt_cmpb_inst->pui1_agent_url != NULL)
        {
            CMPB_MEM_FREE(pt_cmpb_inst->pui1_agent_url);
            pt_cmpb_inst->pui1_agent_url = NULL;
        }

        if(pt_cmpb_inst->t_drm_info.e_drm_type == IMTK_PB_CTRL_DRM_TYPE_MLN)
        {
            if(pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url != NULL)
            {
                CMPB_MEM_FREE(pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url);
                pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url = NULL;
            }
            if(pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_afl_tkn != NULL)
            {
                CMPB_MEM_FREE(pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_afl_tkn);
                pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_afl_tkn = NULL;
            }
        }

        for(ui1_i = 0; ui1_i<IMTK_PB_CTRL_URL_TYPE_MAX; ui1_i++)
        {
            if(pt_cmpb_inst->ac_url_list[ui1_i] != NULL)
            {
                CMPB_MEM_FREE(pt_cmpb_inst->ac_url_list[ui1_i]);
                pt_cmpb_inst->ac_url_list[ui1_i] = NULL;
            }
        }

        if (CMPB_HANDLE_VALID(pt_cmpb_inst->h_time_code_update) == TRUE) 
        {
            CMPB_TIMER_DELETE(pt_cmpb_inst->h_time_code_update);
            pt_cmpb_inst->h_time_code_update = NULL_HANDLE;
        }
#ifdef MM_SEND_BUFFER_PREPLAY_SUPPORT
        _cmpb_ctrl_prestore_data_release(h_Handle);
#endif  
        _cmpb_free_instance(pt_cmpb_inst);        
    }

    _cmpb_free_handle(h_Handle);
    
    
    return IMTK_PB_ERROR_CODE_OK;
}

///add for some custumor only do open once to play all media files
IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Close_MediaInfo(IMTK_PB_HANDLE_T  h_Handle)
{
    CMPB_CTRL_INST_T*   pt_cmpb_inst = NULL;
    INT32               i4_ret;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, h_Handle);

    if (pt_cmpb_inst)
    {
        if (pt_cmpb_inst->b_start_parse)
        {
            while (1)
            {
                if (pt_cmpb_inst->b_parse_done)
                {
                    break;
                }
                CMPB_THREAD_DELAY(100);
            }
        }
        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT);        
        if(OSR_OK != i4_ret)
        {
            
        }

        if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PUSH)
        {
            _set_video_freeze(FALSE);
        }

        if(pt_cmpb_inst->h_media_hdl)
        {
            CMPB_SVCTX_MEDIA_CLOSE(pt_cmpb_inst->h_svctx_hdl, pt_cmpb_inst->h_media_hdl);
            pt_cmpb_inst->h_media_hdl = NULL_HANDLE;
        }
        
        if(pt_cmpb_inst->pui1_proxy_url != NULL)
        {
            CMPB_MEM_FREE(pt_cmpb_inst->pui1_proxy_url);
            pt_cmpb_inst->pui1_proxy_url = NULL;
        }

        if(pt_cmpb_inst->pui1_agent_url != NULL)
        {
            CMPB_MEM_FREE(pt_cmpb_inst->pui1_agent_url);
            pt_cmpb_inst->pui1_agent_url = NULL;
        }

        pt_cmpb_inst->b_start_parse = FALSE;
        pt_cmpb_inst->b_parse_done  = FALSE;

        CMPB_MEMSET(&pt_cmpb_inst->u_media.t_get_info, 0, sizeof(IMTK_PB_CTRL_GET_MEDIA_INFO_T));
       
    }

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);    
    if(OSR_OK != i4_ret)
    {
    
    }

    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetDivxDRMInfo(IMTK_PB_HANDLE_T                 hHandle,
                                                IMTK_PB_CTRL_DIVX_DRM_INFO_T*    ptDRMInfo)
{
    INT32                   i4_ret;
    CMPB_CTRL_INST_T*       pt_cmpb_inst = NULL;
    SIZE_T                  z_size;

    if (NULL == ptDRMInfo)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    switch (ptDRMInfo->eInfoType)
    {
    case IMTK_PB_CTRL_DIVX_DRM_BASIC:
        {
            DRM_INFO_SVCTX_T        t_drm_info;
            DIVX_DRM_BASIC_INFO_T   t_drm_info_basic;

            CMPB_MEMSET(&t_drm_info, 0, sizeof(DRM_INFO_SVCTX_T));
            CMPB_MEMSET(&t_drm_info_basic, 0, sizeof(DIVX_DRM_BASIC_INFO_T));

            t_drm_info.u.t_divx_drm_info_svctx.pt_divx_drm_basic_info = &t_drm_info_basic;

            z_size = sizeof(DRM_INFO_SVCTX_T);

            i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_DRM_GET_TYPE_INFO,
                        (VOID*)&t_drm_info,
                        z_size
                        );
            if (SVCTXR_OK == i4_ret)
            {
                if (DRM_TYPE_DIVX_DRM == t_drm_info.t_drm_type)
                {
                    ptDRMInfo->uInfo.tBasic.u1Flag = t_drm_info_basic.ui1_flag;
                    ptDRMInfo->uInfo.tBasic.u1UseCount = t_drm_info_basic.ui1_use_count;
                    ptDRMInfo->uInfo.tBasic.u1UseLimit = t_drm_info_basic.ui1_use_limit;
                }
                else
                {
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }
            }
            else
            {
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }
        }
        break;

    case IMTK_PB_CTRL_DIVX_DRM_REGISTRATION:
        i4_ret = CMPB_SVCTX_GENERIC_GET_INFO (
                        pt_cmpb_inst->h_svctx_hdl,
                        NULL_HANDLE,
                        SVCTX_GENGRIC_GET_TYPE_DIVX_DRM_REGISTRATION_CODE,
                        ptDRMInfo->uInfo.tRegistration.pu1RegistrationCode,
                        sizeof(DIVX_DRM_UNION_GENERIC_INFO_T)
                        );
        if (i4_ret != SVCTXR_OK)
        {
            return IMTK_PB_ERROR_CODE_NOT_OK;
        }
        break;

    case IMTK_PB_CTRL_DIVX_DRM_UI_HELP:
        i4_ret = CMPB_SVCTX_GENERIC_GET_INFO (
                  pt_cmpb_inst->h_svctx_hdl,
                  NULL_HANDLE,
                  SVCTX_GENGRIC_GET_TYPE_DIVX_DRM_UI_HELP_INFO,
                  &(ptDRMInfo->uInfo.tUIHelp.u4DivxDrmUIHelpInfo),
                  sizeof(DIVX_DRM_UNION_GENERIC_INFO_T));
        if (i4_ret != SVCTXR_OK)
        {
            return IMTK_PB_ERROR_CODE_NOT_OK;
        }
        break;

    default:
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_DivxDRMDeactivation(IMTK_PB_HANDLE_T                            hHandle,
                                                     IMTK_PB_CTRL_DIVX_DRM_DEACTIVATION_INFO_T*  ptDeactivation)
{
    INT32                   i4_ret;
    CMPB_CTRL_INST_T*       pt_cmpb_inst = NULL;

    if (NULL == ptDeactivation)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    i4_ret = CMPB_SVCTX_GENERIC_SET_INFO (
                    pt_cmpb_inst->h_svctx_hdl,
                    NULL_HANDLE,
                    SVCTX_GENGRIC_SET_TYPE_DIVX_DRM_DEACTIVATION,
                    ptDeactivation->pu1DeactivationCode,
                    sizeof(DIVX_DRM_UNION_GENERIC_INFO_T)
                    );

    if (i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    return IMTK_PB_ERROR_CODE_OK;
}

#define DIVX_DRM_MEMORY_LENGTH  56
static IMtkPb_Ctrl_Divx_ReadDrmMem_Fct pfn_drm_memory_read = NULL;
static IMtkPb_Ctrl_Divx_WriteDrmMem_Fct pfn_drm_memory_write = NULL;

static BOOL _divx_drm_memory_read_func(UINT8* pui1_divx_drm_mem)
{
    IMTK_PB_CB_ERROR_CODE_T e_ret_code=IMTK_PB_CB_ERROR_CODE_NOT_OK;

    if (pfn_drm_memory_read)
    {
        e_ret_code = pfn_drm_memory_read(pui1_divx_drm_mem, 0, DIVX_DRM_MEMORY_LENGTH);
    }
    
    if (e_ret_code == IMTK_PB_CB_ERROR_CODE_OK)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _divx_drm_memory_write_func(UINT8* pui1_divx_drm_mem)
{
    IMTK_PB_CB_ERROR_CODE_T e_ret_code=IMTK_PB_CB_ERROR_CODE_NOT_OK;

    if (pfn_drm_memory_read)
    {
        e_ret_code = pfn_drm_memory_write(pui1_divx_drm_mem, 0, DIVX_DRM_MEMORY_LENGTH);
    }
    
    if (e_ret_code == IMTK_PB_CB_ERROR_CODE_OK)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


extern IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetDivxDRMRW(IMTK_PB_HANDLE_T                       hHandle,
                                                     IMTK_PB_CTRL_DIVX_DRM_MEM_RW_T*        ptDrmMemRW)
{
    INT32                           i4_ret;
    CMPB_CTRL_INST_T*               pt_cmpb_inst = NULL;
    DIVX_DRM_UNION_GENERIC_INFO_T   t_genric;

    if (NULL == ptDrmMemRW)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    pfn_drm_memory_read = ptDrmMemRW->pfnRead;
    pfn_drm_memory_write = ptDrmMemRW->pfnWrite;
    t_genric.t_divx_drm_memory_rw_func_info.pfn_divx_drm_memory_read_func = _divx_drm_memory_read_func;
    t_genric.t_divx_drm_memory_rw_func_info.pfn_divx_drm_memory_write_func = _divx_drm_memory_write_func;

    i4_ret = CMPB_SVCTX_GENERIC_SET_INFO (
                    pt_cmpb_inst->h_svctx_hdl,
                    NULL_HANDLE,
                    SVCTX_GENGRIC_SET_TYPE_DIVX_DRM_MEMORY_RW_FUNC,
                    &t_genric,
                    sizeof(DIVX_DRM_UNION_GENERIC_INFO_T)
                    );

    if (i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetASP(IMTK_PB_HANDLE_T     hHandle,
                                        IMTK_PB_CTRL_ASP_T*  ptAsp)
{
    INT32                       i4_ret;
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    VSH_SRC_RESOLUTION_INFO_T   t_resolution;
    
    if (NULL == ptAsp)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    i4_ret = CMPB_SVCTX_GET_VIDEO_SRC_RESOLUTION(pt_cmpb_inst->h_svctx_hdl,
                                                 &t_resolution);
                                                 
    if (i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    ptAsp->u4PixelW  = t_resolution.ui4_aspect_ratio_w;
    ptAsp->u4PixelH  = t_resolution.ui4_aspect_ratio_h;
    ptAsp->u4SrcW  = t_resolution.ui4_width;
    ptAsp->u4SrcH  = t_resolution.ui4_height;
    ptAsp->fgIsDisplay  = t_resolution.b_is_display_aspect_ratio;

    return IMTK_PB_ERROR_CODE_OK;
}


extern IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetMetaInfo(IMTK_PB_HANDLE_T        hHandle,
                                                    IMTK_PB_CTRL_META_T*    ptMeta)
{
    INT32                       i4_ret;
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    MINFO_INFO_T                t_minfo_rec;
    SIZE_T                      z_size;
    MINFO_TYPE_META_DATA_T*     pt_meta_date = NULL;

    if (NULL == ptMeta)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    CMPB_MEMSET(&t_minfo_rec, 0, sizeof(MINFO_INFO_T));
    pt_meta_date = &t_minfo_rec.u.t_meta_data;

    pt_meta_date->z_title_len = 128;
    pt_meta_date->pwc_str_title = ptMeta->pu2Title;
    
    pt_meta_date->z_genre_len = 128;
    pt_meta_date->pwc_str_genre = ptMeta->pu2Genre;

    pt_meta_date->z_artist_len = 128;
    pt_meta_date->pwc_str_artist = ptMeta->pu2Artist;
    
    z_size = sizeof(MINFO_INFO_T);
    i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                    pt_cmpb_inst->h_svctx_hdl,
                    pt_cmpb_inst->h_media_hdl,
                    SVCTX_MM_GET_TYPE_META_DATA,
                    (VOID*)&t_minfo_rec,
                    z_size
                    );

    if (i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    ptMeta->u4TitleLen = (uint32_t)pt_meta_date->z_title_len;
    ptMeta->u4GenreLen = (uint32_t)pt_meta_date->z_genre_len;
    ptMeta->u4ArtistLen = (uint32_t)pt_meta_date->z_artist_len;

    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetNavigateInfo(IMTK_PB_HANDLE_T                hHandle,
                                                 IMTK_PB_CTRL_NAV_INFO_T*        ptNavInfo)
{
    INT32                           i4_ret;
    CMPB_CTRL_INST_T*               pt_cmpb_inst = NULL;
    SIZE_T                          z_size;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    switch (ptNavInfo->eNavMode)
    {
    case IMTK_PB_CTRL_NAV_TITLE_NUM:
        z_size = sizeof(UINT32);
        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_TYPE_TITLE_NUM,
                        (VOID*)&ptNavInfo->uInfo.u4TitleNum,
                        z_size
                        );
                                        
        break;

    case IMTK_PB_CTRL_NAV_TITLE_INFO:
        z_size = sizeof(IMTK_PB_CTRL_TITLE_INFO_T);
        ptNavInfo->uInfo.tTitle.u4TitleLen = 128;
        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_TYPE_TITLE_INFO,
                        (VOID*)&ptNavInfo->uInfo.tTitle,
                        z_size
                        );
        break;

    case IMTK_PB_CTRL_NAV_PLAYLIST_INFO:
        z_size = sizeof(IMTK_PB_CTRL_PLAYLIST_INFO_T);
        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_TYPE_PLAYLIST_INFO,
                        (VOID*)&ptNavInfo->uInfo.tPlaylist,
                        z_size
                        );

        break;

    case IMTK_PB_CTRL_NAV_CHAP_INFO:
        z_size = sizeof(IMTK_PB_CTRL_CHAP_INFO_T);
        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_TYPE_CHAP_INFO,
                        (VOID*)&ptNavInfo->uInfo.tChap,
                        z_size
                        );
        break;

    case IMTK_PB_CTRL_NAV_DISP_INFO:
        z_size = sizeof(IMTK_PB_CTRL_DISP_INFO_T);
        ptNavInfo->uInfo.tDisp.u4ChapterLen = 128;
        ptNavInfo->uInfo.tDisp.u4CountryLen = 128;
        ptNavInfo->uInfo.tDisp.u4LanguageLen = 128;
        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_TYPE_DISP_INFO,
                        (VOID*)&ptNavInfo->uInfo.tDisp,
                        z_size
                        );
        break;

    case IMTK_PB_CTRL_NAV_CUR_CHAP_ID:
        z_size = sizeof(IMTK_PB_CTRL_CUR_CHAP_ID_T);
        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_TYPE_CUR_CHAP_INFO,
                        (VOID*)&ptNavInfo->uInfo.tCurChapID,
                        z_size
                        );
        break;

    default:
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    if (i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_ChapSeek(IMTK_PB_HANDLE_T   hHandle,
                                          uint32_t           u4PlaylistIdx,
                                          uint32_t           u4ChapIdx)
{
    INT32                           i4_ret;
    CMPB_CTRL_INST_T*               pt_cmpb_inst = NULL;
    MM_SVC_POS_INFO_T               t_pos;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    CMPB_MEMSET(&t_pos, 0, sizeof(MM_SVC_POS_INFO_T));

    t_pos.t_pos_type    = MM_SVC_POS_TYPE_CHAPTER;
    t_pos.ui2_idx       = (UINT16)0;
    t_pos.ui8_pos_val = MM_DIVX_PLUS_ID_TO_POS(pt_cmpb_inst->ui4_playlist_idx, pt_cmpb_inst->ui4_chap_idx);

    pt_cmpb_inst->fg_during_seek = TRUE;
    
    i4_ret = CMPB_SVCTX_MEDIA_SEEK(
                    pt_cmpb_inst->h_svctx_hdl,
                    _cmpb_ctrl_media_play_nfy_fct,
                    (VOID*)pt_cmpb_inst,
                    &t_pos
                    );

    if (i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetNavigateInfo(IMTK_PB_HANDLE_T    hHandle,
                                                 uint32_t            u4TitleIdx,
                                                 uint32_t            u4PlaylistIdx,
                                                 uint32_t            u4ChapIdx)
{
    CMPB_CTRL_INST_T*               pt_cmpb_inst = NULL;


    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    pt_cmpb_inst->ui4_title_idx = u4TitleIdx;
    pt_cmpb_inst->ui4_playlist_idx = u4PlaylistIdx;
    pt_cmpb_inst->ui4_chap_idx = u4ChapIdx;

    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_RegCallback(IMTK_PB_HANDLE_T        hHandle,
                                             VOID*                   pvAppCbTag,
                                             IMtkPb_Ctrl_Nfy_Fct     pfnCallback)
{
    INT32                   i4_ret;
    CMPB_CTRL_INST_T*       pt_cmpb_inst = NULL;
    IMTK_PB_ERROR_CODE_T    e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return e_ret_code;
    }
    
    do
    {        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }
        
        pt_cmpb_inst->pf_notify = pfnCallback;
        pt_cmpb_inst->pv_tag = pvAppCbTag;
        
        e_ret_code = IMTK_PB_ERROR_CODE_OK;
        
    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
        
    }
    
    return e_ret_code;
}

#ifdef CLI_TEST
MM_STRM_INFO_T              g_t_strm_info;
#endif
IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetEngineParam(IMTK_PB_HANDLE_T             hHandle,     
                                                IMTK_PB_CTRL_ENGINE_PARAM_T*   ptParam)
{
    UINT8                       ui1_i           = 0;
    UINT8                       ui1_j           = 0;
    CMPB_CTRL_INST_T*           pt_cmpb_inst    = NULL;
    INT32                       i4_ret          = 0;
    MM_SVC_COMMAND_T            at_cmd[MAX_CMD_CNT];
    MM_PB_CTRL_PULL_INFO_T      t_pull          = {0};
    MM_STRM_INFO_T              t_strm_info;
    MINFO_INFO_T                t_meta_data;
    MM_EXT_SBTL_INFO            t_sbtl_info;
    MM_LYRIC_INFO               t_lyrc_info;               

    CMPB_MEMSET(&t_strm_info, 0, sizeof(MM_STRM_INFO_T));
    CMPB_MEMSET(&t_meta_data, 0, sizeof(MINFO_INFO_T));
    CMPB_MEMSET(&t_sbtl_info, 0, sizeof(MM_EXT_SBTL_INFO));
    CMPB_MEMSET(&t_lyrc_info, 0, sizeof(MM_LYRIC_INFO));

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (pt_cmpb_inst == NULL)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    do
    {
        if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PULL)
        {
#ifdef CLI_TEST
            if(!g_is_get_info)
#endif
            {
                i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
            }

            pt_cmpb_inst->ui4_play_flag = ptParam->u4PlayFlag;

            CMPB_MEMCPY(&(pt_cmpb_inst->u_info.t_pull), 
                     &ptParam->uBufferModelParam.tPullInfo, 
                     sizeof(IMTK_PB_CTRL_PULL_INFO_T));

            t_pull.h_pb = hHandle;
            t_pull.pv_app_tag = ptParam->uBufferModelParam.tPullInfo.pvAppTag;
            t_pull.pf_open = (x_mm_playback_pull_open_fct)(ptParam->uBufferModelParam.tPullInfo.pfnOpen);
            t_pull.pf_close = (x_mm_playback_pull_close_fct)(ptParam->uBufferModelParam.tPullInfo.pfnClose);
            t_pull.pf_read = (x_mm_playback_pull_read_fct)(ptParam->uBufferModelParam.tPullInfo.pfnRead);
            t_pull.pf_read_async = (x_mm_playback_pull_read_async_fct)(ptParam->uBufferModelParam.tPullInfo.pfnReadAsync);
            t_pull.pf_byteseek = (x_mm_playback_pull_byteseek_fct)(ptParam->uBufferModelParam.tPullInfo.pfnByteSeek);
            t_pull.pf_abort_read_async = (x_mm_playback_pull_abort_read_async_fct)(ptParam->uBufferModelParam.tPullInfo.pfnAbortReadAsync);
            t_pull.pf_get_input_len = (x_mm_playback_pull_get_input_lengh_fct)(ptParam->uBufferModelParam.tPullInfo.pfnGetInputLen);

                 
            /* start open media */
            #if 0
            at_cmd[0].e_code = MM_SVC_CMD_CODE_MEDIA_PATH;
            if(pt_cmpb_inst->b_app_set_media_info
#ifdef CLI_TEST
             && !g_is_get_info
#endif
              ) /*used for app master*/
            {
                at_cmd[0].u.ps_text = "http://192.168.0.1/test.mp4";
            }
            else
            {
                at_cmd[0].u.ps_text = NULL;
            }
            #endif

            at_cmd[ui1_i].e_code        = MM_SVC_CMD_CODE_SRC_TYPE;
            at_cmd[ui1_i++].u.ui1_data  = SRC_TYPE_MEDIA_NETWORK;

            at_cmd[ui1_i].e_code        = MM_SVC_CMD_CODE_SET_PROTOCL;
            at_cmd[ui1_i++].u.ui4_data  = PROTOCL_MODE_PULL;
            
            at_cmd[ui1_i].e_code        = MM_SVC_CMD_CODE_SET_PULL_MODE_INFO;
            at_cmd[ui1_i++].u.pv_data   = &t_pull;
            
            at_cmd[ui1_i].e_code        = MM_SVC_CMD_CODE_SET_SRC_SUB_TYPE;
            at_cmd[ui1_i++].u.ui1_data  = pt_cmpb_inst->t_src_sub_type;

			at_cmd[ui1_i].e_code        = MM_SVC_CMD_CODE_SET_PVR_TOTAl_DUR;
            at_cmd[ui1_i++].u.ui4_data  = pt_cmpb_inst->ui4_total_dur;
			
            if(pt_cmpb_inst->e_cmpb_profile == IMTK_PB_CTRL_PROFILE_AUTO_DETECT_NEXT_VID)
            {
                at_cmd[ui1_i].e_code        = MM_SVC_CMD_CODE_SET_FILL_SCDB_MODE;
                at_cmd[ui1_i++].u.ui1_data  = MM_SVC_FILL_SCDB_APPEND;
            }
            
            if(
#ifdef CLI_TEST
                (!g_is_get_info) &&
#endif
                pt_cmpb_inst->b_app_set_media_info)
            {
                t_meta_data.e_info_type = MINFO_INFO_TYPE_META_DATA;

                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_META_INFO;
                at_cmd[ui1_i++].u.pv_data = &t_meta_data;

                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_STRM_INFO;
                at_cmd[ui1_i++].u.pv_data = &t_strm_info;
                
            }

            if (pt_cmpb_inst->ac_url_list[IMTK_PB_CTRL_URL_TYPE_EXT_SBTL_FULL_PATH] != NULL)
            {
                /* init external subtitle info */
                t_sbtl_info.e_type = MM_EXT_SBTL_TYPE_FULL;
                t_sbtl_info.u.ps_full = pt_cmpb_inst->ac_url_list[IMTK_PB_CTRL_URL_TYPE_EXT_SBTL_FULL_PATH];//"srt;ass;ssa;sub;smi;idx;txt;psb;mpl;";
                t_sbtl_info.ps_prf_lang_lst = "eng;chi;fre;"; /* For MW test */
                
                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_EXT_SBTL_PATH;
                at_cmd[ui1_i++].u.pv_data = &t_sbtl_info;
            }
            else if(pt_cmpb_inst->ac_url_list[IMTK_PB_CTRL_URL_TYPE_LYRIC_FULL_PATH] != NULL)
            {
                t_lyrc_info.e_type = MM_LYRIC_TYPE_LRC_FULL_PATH;
                t_lyrc_info.u.ps_full = pt_cmpb_inst->ac_url_list[IMTK_PB_CTRL_URL_TYPE_LYRIC_FULL_PATH];
                t_lyrc_info.ps_prf_lang_lst = "eng;chi;fre;";
                
                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_EXT_LYRIC_PATH;
                at_cmd[ui1_i++].u.pv_data = &t_lyrc_info;
            }
            
            CHECK_CMD_IDX(ui1_i);
            
            at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_END;

#ifdef CLI_TEST
            if(!g_is_get_info)
#endif
            {
                pt_cmpb_inst->e_old_state = pt_cmpb_inst->e_cur_state;
                //pt_cmpb_inst->e_cur_state = CMPB_CTRL_OPENED;
                pt_cmpb_inst->e_cur_state = CMPB_CTRL_READY;
                //pt_cmpb_inst->e_new_state = CMPB_CTRL_OPENED;
                pt_cmpb_inst->e_cur_state = CMPB_CTRL_READY;
            }

            pt_cmpb_inst->b_parse_done = FALSE;
            pt_cmpb_inst->b_start_parse = TRUE;
            if (pt_cmpb_inst->h_media_hdl)
            {
                CMPB_SVCTX_MEDIA_CLOSE(pt_cmpb_inst->h_svctx_hdl, pt_cmpb_inst->h_media_hdl);
                pt_cmpb_inst->h_media_hdl = NULL_HANDLE;
                CMPB_MEMSET(&pt_cmpb_inst->u_media.t_get_info,0,sizeof(IMTK_PB_CTRL_GET_MEDIA_INFO_T));
            }
            i4_ret = CMPB_SVCTX_MEDIA_OPEN(
                            pt_cmpb_inst->h_svctx_hdl,
                            at_cmd,
                            _cmpb_ctrl_media_open_nfy_fct,
                            (VOID*)pt_cmpb_inst,
                            &(pt_cmpb_inst->h_media_hdl)
                            );
            if(SVCTXR_OK != i4_ret)
            {
                pt_cmpb_inst->h_media_hdl = NULL_HANDLE;
                pt_cmpb_inst->b_start_parse = FALSE;
                pt_cmpb_inst->b_open_media_error = TRUE;
                CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_OPEN() returns %d\n", i4_ret));
                
                _cmpb_ctrl_media_open_nfy_fct(
                    pt_cmpb_inst->h_svctx_hdl, 
                    SVCTX_COND_PENDING,
                    SVCTX_NTFY_CODE_MEDIA_ERROR,
                    ST_UNKNOWN,
                    pt_cmpb_inst,
                    (VOID*)IMTK_PB_CTRL_ERROR_FILE_NOT_SUPPORT
                    );
                
                break;
            }

            //pt_cmpb_inst->h_media_hdl = h_media_hdl;

#ifdef CLI_TEST
            if(g_is_get_info)
            {
                do
                {
                    CMPB_THREAD_DELAY(100);
                    i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                        pt_cmpb_inst->h_svctx_hdl,
                                        pt_cmpb_inst->h_media_hdl,
                                        SVCTX_MM_GET_TYPE_STRM_INFO,
                                        (VOID*)&g_t_strm_info,
                                        sizeof(MM_STRM_INFO_T)
                                        );
                }while(i4_ret == MINFOR_INFO_NOT_READY);

                if(SVCTXR_OK != i4_ret)
                {
                    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema); 
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }
                 /*
                            i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                                pt_cmpb_inst->h_svctx_hdl,
                                                pt_cmpb_inst->h_media_hdl,
                                                SVCTX_MM_GET_TYPE_META_DATA,
                                                (VOID*)&g_t_meta_data,
                                                sizeof(MM_STRM_INFO_T)
                                                );
                            if(SVCTXR_OK != i4_ret)
                            {
                                i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema); 
                                return IMTK_PB_ERROR_CODE_NOT_OK;
                            }*/
                CMPB_SVCTX_MEDIA_CLOSE(pt_cmpb_inst->h_svctx_hdl,
                                    pt_cmpb_inst->h_media_hdl);
                g_is_get_info = FALSE;
                continue;
            }
#endif
            i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
        
            if(OSR_OK != i4_ret)
            {
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }        
            while (1)
            {
                if (pt_cmpb_inst->b_parse_done || !pt_cmpb_inst->b_start_parse)
                {
                    if(pt_cmpb_inst->b_open_media_error)
                    { /*open media error*/
                        return IMTK_PB_ERROR_CODE_NOT_OK;
                    }
                    break;
                }
                CMPB_THREAD_DELAY(100);
            }

            return IMTK_PB_ERROR_CODE_OK;
        }
        else if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_URI) /*URI mode*/
        {
            if (pt_cmpb_inst->h_media_hdl)
            {
                return IMTK_PB_ERROR_CODE_OK;
            }

#ifdef CLI_TEST
            if(!g_is_get_info)
#endif
            {
                i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
            }

            if(OSR_OK != i4_ret)
            {
                break;
            }

            pt_cmpb_inst->ui4_play_flag = ptParam->u4PlayFlag;
            
            /* check state */
            if ((CMPB_CTRL_TO_SETINFO != pt_cmpb_inst->e_cur_state) 
                && ( CMPB_CTRL_OPENED != pt_cmpb_inst->e_cur_state)
                )
            {
                break;
            }
            
            /* set URI */
            CMPB_MEMCPY(&(pt_cmpb_inst->u_info.t_uri), 
                         &ptParam->uBufferModelParam.tUriInfo, 
                     sizeof(IMTK_PB_CTRL_URI_INFO_T)
                     );

            /* start open media */
            at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_MEDIA_PATH;
            at_cmd[ui1_i++].u.ps_text = (CHAR*)(pt_cmpb_inst->u_info.t_uri.pu1URI);

            at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SRC_TYPE;
            at_cmd[ui1_i++].u.ui1_data = SRC_TYPE_MEDIA_NETWORK;

            at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_PROTOCL;
            at_cmd[ui1_i++].u.ui4_data = PROTOCL_MODE_URI;

            if(pt_cmpb_inst->e_cmpb_profile == IMTK_PB_CTRL_PROFILE_AUTO_DETECT_NEXT_VID)
            {
                at_cmd[ui1_i].e_code        = MM_SVC_CMD_CODE_SET_FILL_SCDB_MODE;
                at_cmd[ui1_i++].u.ui1_data  = MM_SVC_FILL_SCDB_APPEND;
            }

            if(
#ifdef CLI_TEST
                (!g_is_get_info) &&
#endif
                pt_cmpb_inst->b_app_set_media_info)
            {
                t_meta_data.e_info_type = MINFO_INFO_TYPE_META_DATA;

                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_META_INFO;
                at_cmd[ui1_i++].u.pv_data = &t_meta_data;

                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_STRM_INFO;
                at_cmd[ui1_i++].u.pv_data = &t_strm_info;
            }

            if(pt_cmpb_inst->t_drm_info.e_drm_type != IMTK_PB_CTRL_DRM_TYPE_NONE)
            {
                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_DRM_INFO;
                at_cmd[ui1_i++].u.pv_data = &(pt_cmpb_inst->t_drm_info);
            }
            
            if(pt_cmpb_inst->pui1_proxy_url != NULL && pt_cmpb_inst->b_monitor)
            {
                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_PROXY_URL;
                at_cmd[ui1_i++].u.pv_data = (VOID*)(pt_cmpb_inst->pui1_proxy_url);
            }

            if(pt_cmpb_inst->pui1_agent_url != NULL && pt_cmpb_inst->b_monitor)
            {
                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_AGENT_URL;
                at_cmd[ui1_i++].u.pv_data = (VOID*)(pt_cmpb_inst->pui1_agent_url);
            }

            for(ui1_j = 0; ui1_j<IMTK_PB_CTRL_URL_TYPE_MAX; ui1_j++)
            {
                if(pt_cmpb_inst->ac_url_list[ui1_j] != NULL)
                {
                    if (ui1_j == IMTK_PB_CTRL_URL_TYPE_EXT_SBTL_FULL_PATH)
                    {                        
                        /* init external subtitle info */
                        t_sbtl_info.e_type = MM_EXT_SBTL_TYPE_FULL;
                        t_sbtl_info.u.ps_full = pt_cmpb_inst->ac_url_list[IMTK_PB_CTRL_URL_TYPE_EXT_SBTL_FULL_PATH];//"srt;ass;ssa;sub;smi;idx;txt;psb;mpl;";
                        t_sbtl_info.ps_prf_lang_lst = "eng;chi;fre;"; /* For MW test */
                        
                        at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_EXT_SBTL_PATH;
                        at_cmd[ui1_i++].u.pv_data = &t_sbtl_info;
                        continue;
                    }
                    else if(ui1_j == IMTK_PB_CTRL_URL_TYPE_LYRIC_FULL_PATH)
                    {
                        t_lyrc_info.e_type = MM_LYRIC_TYPE_LRC_FULL_PATH;
                        t_lyrc_info.u.ps_lyric_list = pt_cmpb_inst->ac_url_list[IMTK_PB_CTRL_URL_TYPE_LYRIC_FULL_PATH];
                        t_lyrc_info.ps_prf_lang_lst = "eng;chi;fre;";
                        
                        at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_EXT_LYRIC_PATH;
                        at_cmd[ui1_i++].u.pv_data = &t_lyrc_info;
                        continue;
                    }
                    at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_PROXY_URL+ui1_j;
                    at_cmd[ui1_i++].u.pv_data = (VOID*)(pt_cmpb_inst->ac_url_list[ui1_j]);
                    CHECK_CMD_IDX(ui1_i);
                }
            }

            CHECK_CMD_IDX(ui1_i);
            //Notice: at_cmd Maybe overflow            
            at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_END;
            
#ifdef CLI_TEST
            if(!g_is_get_info)
#endif
            {
                pt_cmpb_inst->e_old_state = pt_cmpb_inst->e_cur_state;
                    //pt_cmpb_inst->e_cur_state = CMPB_CTRL_OPENED;
                    pt_cmpb_inst->e_cur_state = CMPB_CTRL_READY;
                    //pt_cmpb_inst->e_new_state = CMPB_CTRL_OPENED;
                    pt_cmpb_inst->e_cur_state = CMPB_CTRL_READY;
            }

            pt_cmpb_inst->b_parse_done = FALSE;
            pt_cmpb_inst->b_start_parse = TRUE;
            if (pt_cmpb_inst->h_media_hdl)
            {
                CMPB_SVCTX_MEDIA_CLOSE(pt_cmpb_inst->h_svctx_hdl, pt_cmpb_inst->h_media_hdl);
                pt_cmpb_inst->h_media_hdl = NULL_HANDLE;
            }
            i4_ret = CMPB_SVCTX_MEDIA_OPEN(
                            pt_cmpb_inst->h_svctx_hdl,
                            at_cmd,
                            _cmpb_ctrl_media_open_nfy_fct,
                            (VOID*)pt_cmpb_inst,
                            &(pt_cmpb_inst->h_media_hdl)
                            );

            if(SVCTXR_OK != i4_ret)
            {
                UINT32  u4Data;
                pt_cmpb_inst->h_media_hdl = NULL_HANDLE;
                pt_cmpb_inst->b_start_parse = FALSE;
                pt_cmpb_inst->b_open_media_error = TRUE;
                CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_OPEN() returns %d\n", i4_ret));

                if(SVCTXR_TIMEOUT != i4_ret)
                {
                    u4Data = IMTK_PB_CTRL_ERROR_FILE_NOT_SUPPORT;
                }
                else
                {
                    u4Data = IMTK_PB_CTRL_ERROR_OPEN_FILE_FAIL;
                }

                _cmpb_ctrl_media_open_nfy_fct(
                    pt_cmpb_inst->h_svctx_hdl, 
                    SVCTX_COND_PENDING,
                    SVCTX_NTFY_CODE_MEDIA_ERROR,
                    ST_UNKNOWN,
                    pt_cmpb_inst,
                    (VOID*)u4Data
                    );
                
                break;
            }
              
            //pt_cmpb_inst->h_media_hdl = h_media_hdl;

#ifdef CLI_TEST
            if(g_is_get_info)
            {
                do
                {
                    CMPB_THREAD_DELAY(100);
                    i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                        pt_cmpb_inst->h_svctx_hdl,
                                        pt_cmpb_inst->h_media_hdl,
                                        SVCTX_MM_GET_TYPE_STRM_INFO,
                                        (VOID*)&g_t_strm_info,
                                        sizeof(MM_STRM_INFO_T)
                                        );
                }while(i4_ret == MINFOR_INFO_NOT_READY);
                if(SVCTXR_OK != i4_ret)
                {
                    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema); 
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }
                /*i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                    pt_cmpb_inst->h_svctx_hdl,
                                    pt_cmpb_inst->h_media_hdl,
                                    SVCTX_MM_GET_TYPE_META_DATA,
                                    (VOID*)&g_t_meta_data,
                                    sizeof(MM_STRM_INFO_T)
                                    );
                            if(SVCTXR_OK != i4_ret)
                            {
                                i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema); 
                                return IMTK_PB_ERROR_CODE_NOT_OK;
                            }*/
                CMPB_SVCTX_MEDIA_CLOSE(pt_cmpb_inst->h_svctx_hdl,
                                    pt_cmpb_inst->h_media_hdl);
                g_is_get_info = FALSE;
                continue;
            }
#endif
            i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
        
            if(OSR_OK != i4_ret)
            {            
            }
            
            while (1)
            {
                if (pt_cmpb_inst->b_parse_done || !pt_cmpb_inst->b_start_parse)
                {
                    if(pt_cmpb_inst->b_open_media_error)
                    { /*open media error*/
                        return IMTK_PB_ERROR_CODE_NOT_OK;
                    }
                    break;
                }
                CMPB_THREAD_DELAY(100);
            }

            return IMTK_PB_ERROR_CODE_OK;
        }
        else /*push mode*/
        {
            CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
            pt_cmpb_inst->ui4_play_flag = ptParam->u4PlayFlag;
            CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);
            return IMTK_PB_ERROR_CODE_OK;
        }
    } while(1);

    if (CMPB_CTRL_TRANSIT == pt_cmpb_inst->e_cur_state)
    {
        pt_cmpb_inst->e_cur_state = pt_cmpb_inst->e_old_state;
        pt_cmpb_inst->e_new_state = pt_cmpb_inst->e_old_state;
    }
    
    pt_cmpb_inst->b_parse_done = TRUE;
    pt_cmpb_inst->b_start_parse = FALSE;
    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
        
    }
    
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetEngineParamAsync(IMTK_PB_HANDLE_T             hHandle,     
                                                            IMTK_PB_CTRL_ENGINE_PARAM_T*   ptParam)
{
    UINT8                       ui1_i           = 0;
    UINT8                       ui1_j           = 0;
    CMPB_CTRL_INST_T*           pt_cmpb_inst    = NULL;
    INT32                       i4_ret = 0;
    MM_SVC_COMMAND_T            at_cmd[10];
    MM_PB_CTRL_PULL_INFO_T      t_pull = {0};
    MM_STRM_INFO_T              t_strm_info;
    MINFO_INFO_T                t_meta_data;
    MM_EXT_SBTL_INFO            t_sbtl_info;
    MM_LYRIC_INFO               t_lyrc_info;               

    CMPB_MEMSET(&t_strm_info, 0, sizeof(MM_STRM_INFO_T));
    CMPB_MEMSET(&t_meta_data, 0, sizeof(MINFO_INFO_T));
    CMPB_MEMSET(&t_sbtl_info, 0, sizeof(MM_EXT_SBTL_INFO));
    CMPB_MEMSET(&t_lyrc_info, 0, sizeof(MM_LYRIC_INFO));

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (pt_cmpb_inst == NULL)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    do
    {
        if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PULL)
        {
#ifdef CLI_TEST
            if(!g_is_get_info)
#endif
            {
                i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
            }

            pt_cmpb_inst->ui4_play_flag = ptParam->u4PlayFlag;

            CMPB_MEMCPY(&(pt_cmpb_inst->u_info.t_pull), 
                     &ptParam->uBufferModelParam.tPullInfo, 
                     sizeof(IMTK_PB_CTRL_PULL_INFO_T));

            t_pull.h_pb = hHandle;
            t_pull.pv_app_tag = ptParam->uBufferModelParam.tPullInfo.pvAppTag;
            t_pull.pf_open = (x_mm_playback_pull_open_fct)(ptParam->uBufferModelParam.tPullInfo.pfnOpen);
            t_pull.pf_close = (x_mm_playback_pull_close_fct)(ptParam->uBufferModelParam.tPullInfo.pfnClose);
            t_pull.pf_read = (x_mm_playback_pull_read_fct)(ptParam->uBufferModelParam.tPullInfo.pfnRead);
            t_pull.pf_read_async = (x_mm_playback_pull_read_async_fct)(ptParam->uBufferModelParam.tPullInfo.pfnReadAsync);
            t_pull.pf_byteseek = (x_mm_playback_pull_byteseek_fct)(ptParam->uBufferModelParam.tPullInfo.pfnByteSeek);
            t_pull.pf_abort_read_async = (x_mm_playback_pull_abort_read_async_fct)(ptParam->uBufferModelParam.tPullInfo.pfnAbortReadAsync);
            t_pull.pf_get_input_len = (x_mm_playback_pull_get_input_lengh_fct)(ptParam->uBufferModelParam.tPullInfo.pfnGetInputLen);

                 
            /* start open media */
            at_cmd[ui1_i].e_code        = MM_SVC_CMD_CODE_SRC_TYPE;
            at_cmd[ui1_i++].u.ui1_data  = SRC_TYPE_MEDIA_NETWORK;

            at_cmd[ui1_i].e_code        = MM_SVC_CMD_CODE_SET_PROTOCL;
            at_cmd[ui1_i++].u.ui4_data  = PROTOCL_MODE_PULL;

            at_cmd[ui1_i].e_code        = MM_SVC_CMD_CODE_SET_PULL_MODE_INFO;
            at_cmd[ui1_i++].u.pv_data   = &t_pull;
            
            at_cmd[ui1_i].e_code        = MM_SVC_CMD_CODE_SET_SRC_SUB_TYPE;
            at_cmd[ui1_i++].u.ui1_data  = pt_cmpb_inst->t_src_sub_type;
            
            at_cmd[ui1_i].e_code        = MM_SVC_CMD_CODE_END;

            if(
#ifdef CLI_TEST
                (!g_is_get_info) &&
#endif
                pt_cmpb_inst->b_app_set_media_info)
            {
                t_meta_data.e_info_type = MINFO_INFO_TYPE_META_DATA;

                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_META_INFO;
                at_cmd[ui1_i++].u.pv_data = &t_meta_data;

                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_STRM_INFO;
                at_cmd[ui1_i++].u.pv_data = &t_strm_info;
            }

            if (pt_cmpb_inst->ac_url_list[IMTK_PB_CTRL_URL_TYPE_EXT_SBTL_FULL_PATH] != NULL)
            {
                /* init external subtitle info */
                t_sbtl_info.e_type = MM_EXT_SBTL_TYPE_FULL;
                t_sbtl_info.u.ps_full = pt_cmpb_inst->ac_url_list[IMTK_PB_CTRL_URL_TYPE_EXT_SBTL_FULL_PATH];//"srt;ass;ssa;sub;smi;idx;txt;psb;mpl;";
                t_sbtl_info.ps_prf_lang_lst = "eng;chi;fre;"; /* For MW test */
                
                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_EXT_SBTL_PATH;
                at_cmd[ui1_i++].u.pv_data = &t_sbtl_info;
            }
            else if(pt_cmpb_inst->ac_url_list[IMTK_PB_CTRL_URL_TYPE_LYRIC_FULL_PATH] != NULL)
            {
                t_lyrc_info.e_type = MM_LYRIC_TYPE_LRC_FULL_PATH;
                t_lyrc_info.u.ps_full = pt_cmpb_inst->ac_url_list[IMTK_PB_CTRL_URL_TYPE_LYRIC_FULL_PATH];
                t_lyrc_info.ps_prf_lang_lst = "eng;chi;fre;";
                
                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_EXT_LYRIC_PATH;
                at_cmd[ui1_i++].u.pv_data = &t_lyrc_info;
            }

            at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_END;

#ifdef CLI_TEST
            if(!g_is_get_info)
#endif
            {
                pt_cmpb_inst->e_old_state = pt_cmpb_inst->e_cur_state;
                //pt_cmpb_inst->e_cur_state = CMPB_CTRL_OPENED;
                pt_cmpb_inst->e_cur_state = CMPB_CTRL_READY;
                //pt_cmpb_inst->e_new_state = CMPB_CTRL_OPENED;
                pt_cmpb_inst->e_cur_state = CMPB_CTRL_READY;
            }

            pt_cmpb_inst->b_parse_done = FALSE;
            pt_cmpb_inst->b_start_parse = TRUE;
            pt_cmpb_inst->b_set_engparm_async = TRUE;
            if (pt_cmpb_inst->h_media_hdl)
            {
                CMPB_SVCTX_MEDIA_CLOSE(pt_cmpb_inst->h_svctx_hdl, pt_cmpb_inst->h_media_hdl);
                pt_cmpb_inst->h_media_hdl = NULL_HANDLE;
            }
            i4_ret = CMPB_SVCTX_MEDIA_OPEN(
                            pt_cmpb_inst->h_svctx_hdl,
                            at_cmd,
                            _cmpb_ctrl_media_open_nfy_fct,
                            (VOID*)pt_cmpb_inst,
                            &(pt_cmpb_inst->h_media_hdl)
                            );
            if(SVCTXR_OK != i4_ret)
            {
                pt_cmpb_inst->h_media_hdl       = NULL_HANDLE;
                pt_cmpb_inst->b_start_parse     = FALSE;
                pt_cmpb_inst->b_open_media_error= TRUE;
                pt_cmpb_inst->h_uri_file  = (HANDLE_T)0xFFFFFFFF;
                CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_OPEN() returns %d\n", i4_ret));

                _cmpb_ctrl_media_open_nfy_fct(
                    pt_cmpb_inst->h_svctx_hdl, 
                    SVCTX_COND_PENDING,
                    SVCTX_NTFY_CODE_MEDIA_ERROR,
                    ST_UNKNOWN,
                    pt_cmpb_inst,
                    (VOID*)IMTK_PB_CTRL_ERROR_FILE_NOT_SUPPORT
                    );
                
                break;
            }

            //pt_cmpb_inst->h_media_hdl = h_media_hdl;

#ifdef CLI_TEST
            if(g_is_get_info)
            {
                do
                {
                    CMPB_THREAD_DELAY(100);
                    i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                        pt_cmpb_inst->h_svctx_hdl,
                                        pt_cmpb_inst->h_media_hdl,
                                        SVCTX_MM_GET_TYPE_STRM_INFO,
                                        (VOID*)&g_t_strm_info,
                                        sizeof(MM_STRM_INFO_T)
                                        );
                }while(i4_ret == MINFOR_INFO_NOT_READY);

                if(SVCTXR_OK != i4_ret)
                {
                    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema); 
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }
                 /*
                i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                    pt_cmpb_inst->h_svctx_hdl,
                                    pt_cmpb_inst->h_media_hdl,
                                    SVCTX_MM_GET_TYPE_META_DATA,
                                    (VOID*)&g_t_meta_data,
                                    sizeof(MM_STRM_INFO_T)
                                    );
                if(SVCTXR_OK != i4_ret)
                {
                    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema); 
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }*/
                CMPB_SVCTX_MEDIA_CLOSE(pt_cmpb_inst->h_svctx_hdl,
                                    pt_cmpb_inst->h_media_hdl);
                g_is_get_info = FALSE;
                continue;
            }
#endif
            i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
        
            if(OSR_OK != i4_ret)
            {
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }
#if 0
            while (1)
            {
                if (pt_cmpb_inst->b_parse_done || !pt_cmpb_inst->b_start_parse)
                {
                    if(pt_cmpb_inst->b_open_media_error)
                    { /*open media error*/
                        return IMTK_PB_ERROR_CODE_NOT_OK;
                    }
                    break;
                }
                CMPB_THREAD_DELAY(100);
            }
#endif
            return IMTK_PB_ERROR_CODE_OK;
        }
        else if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_URI) /*URI mode*/
        {
            if (pt_cmpb_inst->h_media_hdl)
            {
                //pt_cmpb_inst->h_uri_file = (HANDLE_T)0XFFFFFFFF;
                i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_uri_sema);
                return IMTK_PB_ERROR_CODE_OK;
            }

#ifdef CLI_TEST
            if(!g_is_get_info)
#endif
            {
                i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
            }

            if(OSR_OK != i4_ret)
            {
                break;
            }

            pt_cmpb_inst->ui4_play_flag = ptParam->u4PlayFlag;
            
            /* check state */
            if ((CMPB_CTRL_TO_SETINFO != pt_cmpb_inst->e_cur_state) 
                && ( CMPB_CTRL_OPENED != pt_cmpb_inst->e_cur_state)
                )
            {
                break;
            }
            
            /* set URI */
            CMPB_MEMCPY(&(pt_cmpb_inst->u_info.t_uri), 
                         &ptParam->uBufferModelParam.tUriInfo, 
                     sizeof(IMTK_PB_CTRL_URI_INFO_T)
                     );

            /* start open media */
            at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_MEDIA_PATH;
            at_cmd[ui1_i++].u.ps_text = (CHAR*)(pt_cmpb_inst->u_info.t_uri.pu1URI);

            at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SRC_TYPE;
            at_cmd[ui1_i++].u.ui1_data = SRC_TYPE_MEDIA_NETWORK;

            at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_PROTOCL;
            at_cmd[ui1_i++].u.ui4_data = PROTOCL_MODE_URI;

            if(
#ifdef CLI_TEST
                (!g_is_get_info) &&
#endif
                pt_cmpb_inst->b_app_set_media_info)
            {
                t_meta_data.e_info_type = MINFO_INFO_TYPE_META_DATA;

                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_META_INFO;
                at_cmd[ui1_i++].u.pv_data = &t_meta_data;

                at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_STRM_INFO;
                at_cmd[ui1_i++].u.pv_data = &t_strm_info;
            }

            for(ui1_j = 0; ui1_j<IMTK_PB_CTRL_URL_TYPE_MAX; ui1_j++)
            {
                if(pt_cmpb_inst->ac_url_list[ui1_j] != NULL)
                {
                    if (ui1_j == IMTK_PB_CTRL_URL_TYPE_EXT_SBTL_FULL_PATH)
                    {                        
                        /* init external subtitle info */
                        t_sbtl_info.e_type = MM_EXT_SBTL_TYPE_FULL;
                        t_sbtl_info.u.ps_full = pt_cmpb_inst->ac_url_list[IMTK_PB_CTRL_URL_TYPE_EXT_SBTL_FULL_PATH];//"srt;ass;ssa;sub;smi;idx;txt;psb;mpl;";
                        t_sbtl_info.ps_prf_lang_lst = "eng;chi;fre;"; /* For MW test */
                        
                        at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_EXT_SBTL_PATH;
                        at_cmd[ui1_i++].u.pv_data = &t_sbtl_info;
                        continue;
                    }
                    else if(ui1_j == IMTK_PB_CTRL_URL_TYPE_LYRIC_FULL_PATH)
                    {
                        t_lyrc_info.e_type = MM_LYRIC_TYPE_LRC_FULL_PATH;
                        t_lyrc_info.u.ps_lyric_list = pt_cmpb_inst->ac_url_list[IMTK_PB_CTRL_URL_TYPE_LYRIC_FULL_PATH];
                        t_lyrc_info.ps_prf_lang_lst = "eng;chi;fre;";
                        
                        at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_EXT_LYRIC_PATH;
                        at_cmd[ui1_i++].u.pv_data = &t_lyrc_info;
                        continue;
                    }
                    at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_SET_PROXY_URL+ui1_j;
                    at_cmd[ui1_i++].u.pv_data = (VOID*)(pt_cmpb_inst->ac_url_list[ui1_j]);
                    CHECK_CMD_IDX(ui1_i);
                }
            }
            //Notice: at_cmd Maybe overflow
            at_cmd[ui1_i].e_code = MM_SVC_CMD_CODE_END;

#ifdef CLI_TEST
            if(!g_is_get_info)
#endif
            {
                pt_cmpb_inst->e_old_state = pt_cmpb_inst->e_cur_state;
                    //pt_cmpb_inst->e_cur_state = CMPB_CTRL_OPENED;
                    pt_cmpb_inst->e_cur_state = CMPB_CTRL_READY;
                    //pt_cmpb_inst->e_new_state = CMPB_CTRL_OPENED;
                    pt_cmpb_inst->e_cur_state = CMPB_CTRL_READY;
            }

            pt_cmpb_inst->b_parse_done = FALSE;
            pt_cmpb_inst->b_start_parse = TRUE;
            pt_cmpb_inst->b_set_engparm_async = TRUE;
            if (pt_cmpb_inst->h_media_hdl)
            {
                CMPB_SVCTX_MEDIA_CLOSE(pt_cmpb_inst->h_svctx_hdl, pt_cmpb_inst->h_media_hdl);
                pt_cmpb_inst->h_media_hdl = NULL_HANDLE;
            }
            i4_ret = CMPB_SVCTX_MEDIA_OPEN(
                            pt_cmpb_inst->h_svctx_hdl,
                            at_cmd,
                            _cmpb_ctrl_media_open_nfy_fct,
                            (VOID*)pt_cmpb_inst,
                            &(pt_cmpb_inst->h_media_hdl)
                            );

            if(SVCTXR_OK != i4_ret)
            {
                UINT32 u4Data;
                pt_cmpb_inst->h_media_hdl       = NULL_HANDLE;
                pt_cmpb_inst->b_start_parse     = FALSE;
                pt_cmpb_inst->b_open_media_error= TRUE;
                CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_OPEN() returns %d\n", i4_ret));
                
                if(SVCTXR_TIMEOUT != i4_ret)
                {
                    u4Data = IMTK_PB_CTRL_ERROR_FILE_NOT_SUPPORT;
                }
                else
                {
                    u4Data = IMTK_PB_CTRL_ERROR_OPEN_FILE_FAIL;
                }
                
                _cmpb_ctrl_media_open_nfy_fct(
                    pt_cmpb_inst->h_svctx_hdl, 
                    SVCTX_COND_PENDING,
                    SVCTX_NTFY_CODE_MEDIA_ERROR,
                    ST_UNKNOWN,
                    pt_cmpb_inst,
                    (VOID*)u4Data
                    );
                
                break;
            }
              
            //pt_cmpb_inst->h_media_hdl = h_media_hdl;

#ifdef CLI_TEST
            if(g_is_get_info)
            {
                do
                {
                    CMPB_THREAD_DELAY(100);
                    i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                        pt_cmpb_inst->h_svctx_hdl,
                                        pt_cmpb_inst->h_media_hdl,
                                        SVCTX_MM_GET_TYPE_STRM_INFO,
                                        (VOID*)&g_t_strm_info,
                                        sizeof(MM_STRM_INFO_T)
                                        );
                }while(i4_ret == MINFOR_INFO_NOT_READY);
                if(SVCTXR_OK != i4_ret)
                {
                    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema); 
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }
                /*i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                                    pt_cmpb_inst->h_svctx_hdl,
                                    pt_cmpb_inst->h_media_hdl,
                                    SVCTX_MM_GET_TYPE_META_DATA,
                                    (VOID*)&g_t_meta_data,
                                    sizeof(MM_STRM_INFO_T)
                                    );
                            if(SVCTXR_OK != i4_ret)
                            {
                                i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema); 
                                return IMTK_PB_ERROR_CODE_NOT_OK;
                            }*/
                CMPB_SVCTX_MEDIA_CLOSE(pt_cmpb_inst->h_svctx_hdl,
                                    pt_cmpb_inst->h_media_hdl);
                g_is_get_info = FALSE;
                continue;
            }
#endif
            i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
        
            if(OSR_OK != i4_ret)
            {            
            }
#if 0
            while (1)
            {
                if (pt_cmpb_inst->b_parse_done || !pt_cmpb_inst->b_start_parse)
                {
                    if(pt_cmpb_inst->b_open_media_error)
                    { /*open media error*/
                        return IMTK_PB_ERROR_CODE_NOT_OK;
                    }
                    break;
                }
                CMPB_THREAD_DELAY(100);
            }
#endif
            return IMTK_PB_ERROR_CODE_OK;
        }
        else /*push mode*/
        {
            CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
            pt_cmpb_inst->ui4_play_flag = ptParam->u4PlayFlag;
            CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);
            return IMTK_PB_ERROR_CODE_OK;
        }
    } while(1);

    if (CMPB_CTRL_TRANSIT == pt_cmpb_inst->e_cur_state)
    {
        pt_cmpb_inst->e_cur_state = pt_cmpb_inst->e_old_state;
        pt_cmpb_inst->e_new_state = pt_cmpb_inst->e_old_state;
    }
    
    pt_cmpb_inst->b_parse_done = TRUE;
    pt_cmpb_inst->b_start_parse = FALSE;
    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
        
    }
    if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_URI &&
       (CMPB_HANDLE_VALID(pt_cmpb_inst->h_uri_sema) == TRUE)) /*URI mode*/
    {
        pt_cmpb_inst->h_uri_file = (HANDLE_T)0XFFFFFFFF;
        i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_uri_sema);
    }
    
    return IMTK_PB_ERROR_CODE_NOT_OK;
}


static MINFO_INFO_VIDEO_ENC_T _get_mm_video_enc_4cc(uint8_t *pui1_4cc)
{
    UINT32 ui4_4cc;
    MINFO_INFO_VIDEO_ENC_T e_mm_enc = MINFO_INFO_VID_ENC_UNKNOWN;

    ui4_4cc = pui1_4cc[0] << 24 |
              pui1_4cc[1] << 16 |
              pui1_4cc[2] << 8  |
              pui1_4cc[3];
    
    switch(ui4_4cc)
    {
    case FOURCC('X', 'V', 'I', 'D'):   /*xvid*/
    case FOURCC('x', 'v', 'i', 'd'):
    case FOURCC('3', 'I', 'V', '1'):   /* 3ivx Delta 1.0~3.5*/
    case FOURCC('3', 'i', 'v', '1'):
    case FOURCC('3', 'I', 'V', '2'):   /* 3ivx Delta 4.0*/
    case FOURCC('3', 'i', 'v', '2'):
    case FOURCC('F', 'V', 'F', 'W'):  /*ffmpeg VFW*/
    case FOURCC('f', 'v', 'f', 'w'):
    case FOURCC('D', 'M', 'K', '2'):  /*a file from Gapha_Tong has this 4cc*/
    case FOURCC('F', 'M', 'P', '4'):  /*some avi has "FMP4" codec 4cc*/
      e_mm_enc = MINFO_INFO_VID_ENC_MPEG_4;
      break;
    case FOURCC('E', 'M', '2', 'V'):  /*tempt.*/
    case FOURCC('M', 'P', 'G', '2'):
    case FOURCC('m', 'p', 'g', '2'):
    case FOURCC('M', 'P', 'E', 'G'):
    case FOURCC('M', 'P', 'G', 'V'):           
    case FOURCC('m', 'p', 'g', 'v'):                     
      e_mm_enc = MINFO_INFO_VID_ENC_MPEG_2;
      break;
    case FOURCC('H', '2', '6', '4'):
    case FOURCC('h', '2', '6', '4'):
      e_mm_enc = MINFO_INFO_VID_ENC_H264;
      break;
    case FOURCC('R', 'M', 'P', '4'):  /*RMP4*/
    case FOURCC('r', 'm', 'p', '4'):
      e_mm_enc = MINFO_INFO_VID_ENC_UNKNOWN;
      break;
    case FOURCC('D', 'I', 'V', '3'):  /*DivX 3.11*/
    case FOURCC('d', 'i', 'v', '3'):
    case FOURCC('D', 'I', 'V', '4'):
    case FOURCC('d', 'i', 'v', '4'):
    case FOURCC('D', 'I', 'V', '5'):
    case FOURCC('d', 'i', 'v', '5'):
    case FOURCC('D', 'I', 'V', '6'):
    case FOURCC('d', 'i', 'v', '6'):
    case FOURCC('M', 'P', '4', '3'):  /*Microsoft MPEG-4 v3*/
    case FOURCC('m', 'p', '4', '3'):
    /* case FOURCC('M', 'P', '4', 'S'): */
    case FOURCC('C', 'O', 'L', '1'):  /* cool codec*/
    case FOURCC('c', 'o', 'l', '1'):
    case FOURCC('A', 'P', '4', '1'):  /*AngelPotion*/
    case FOURCC('a', 'p', '4', '1'):
    case FOURCC('n', 'A', 'V', 'I'):  /*nAVI*/
      e_mm_enc = MINFO_INFO_VID_ENC_DIVX_311;
      break;
    case FOURCC('D', 'I', 'V', 'X'):  /*DivX 4.x*/
    case FOURCC('d', 'i', 'v', 'x'):
      e_mm_enc = MINFO_INFO_VID_ENC_DIVX_4;
      break;
    case FOURCC('D', 'X', '5', '0'):  /* DivX 5.x*/
    case FOURCC('d', 'x', '5', '0'):
      e_mm_enc = MINFO_INFO_VID_ENC_DIVX_5;
      break;
    case FOURCC('m', 'j', 'p', 'g'):    
    case FOURCC('M', 'J', 'P', 'G'):
      e_mm_enc = MINFO_INFO_VID_ENC_MJPG;
      break;
    case FOURCC('w', 'm', 'v', '3'):    
    case FOURCC('W', 'M', 'V', '3'):
      e_mm_enc = MINFO_INFO_VID_ENC_WMV3;
      break;
    case FOURCC('m', 'p', 'g', '1'):
    case FOURCC('M', 'P', 'G', '1'):
      e_mm_enc = MINFO_INFO_VID_ENC_MPEG_1;
      break;

    /*asf codec*/
    case FOURCC('W','V','C','1'):
    case FOURCC('W','M','V','A'):
      e_mm_enc =   MINFO_INFO_VID_ENC_WVC1;
      break;

    default:
        break;
    }

    return e_mm_enc;
}
    
static MINFO_INFO_VIDEO_ENC_T _get_mm_video_enc(IMTK_PB_CTRL_VID_ENC_T e_pb_vid_enc)
{
    MINFO_INFO_VIDEO_ENC_T e_mm_enc = MINFO_INFO_VID_ENC_UNKNOWN;
    switch(e_pb_vid_enc)
    {
    case IMTK_PB_CTRL_VID_ENC_H264:
        e_mm_enc = MINFO_INFO_VID_ENC_H264;
        break;
    case IMTK_PB_CTRL_VID_ENC_VC1:
        e_mm_enc = MINFO_INFO_VID_ENC_WVC1;
        break;
    case IMTK_PB_CTRL_VID_ENC_WMV1:
        e_mm_enc = MINFO_INFO_VID_ENC_WMV1;
        break;
    case IMTK_PB_CTRL_VID_ENC_WMV2:
        e_mm_enc = MINFO_INFO_VID_ENC_WMV2;
        break;
    case IMTK_PB_CTRL_VID_ENC_WMV3:
        e_mm_enc = MINFO_INFO_VID_ENC_WMV3;
        break;
    case IMTK_PB_CTRL_VID_ENC_MPEG1_2:
        e_mm_enc = MINFO_INFO_VID_ENC_MPEG_2;
        break;    
    case IMTK_PB_CTRL_VID_ENC_MPEG4:
        e_mm_enc = MINFO_INFO_VID_ENC_MPEG_4;
        break;
	case IMTK_PB_CTRL_VID_ENC_XVID:
		e_mm_enc = MINFO_INFO_VID_ENC_XVID;
		break;
    case IMTK_PB_CTRL_VID_ENC_DIVX311:
        e_mm_enc = MINFO_INFO_VID_ENC_DIVX_311;
        break;
    case IMTK_PB_CTRL_VID_ENC_RV8:
        e_mm_enc = MINFO_INFO_VID_ENC_RV8;
        break;
    case IMTK_PB_CTRL_VID_ENC_RV9_10:
        e_mm_enc = MINFO_INFO_VID_ENC_RV9;
        break;

    case IMTK_PB_CTRL_VID_ENC_MJPEG:
        e_mm_enc = MINFO_INFO_VID_ENC_MJPG;
        break;

    case IMTK_PB_CTRL_VID_ENC_VP6:
        e_mm_enc = MINFO_INFO_VID_ENC_VP6;
        break;

    case IMTK_PB_CTRL_VID_ENC_VP8:
        e_mm_enc = MINFO_INFO_VID_ENC_VP8;
        break;

    case IMTK_PB_CTRL_VID_ENC_SORENSON_SPARK:
//        e_mm_enc = MINFO_INFO_VID_ENC_SORENSON;
        e_mm_enc =MINFO_INFO_VID_ENC_H263;
        break;
    /*case IMTK_PB_CTRL_VID_ENC_NV12:
        e_mm_enc = MINFO_INFO_VID_ENC_NV12;
        break;*/

    default:
        break;
    }  

    return e_mm_enc;
}

static MINFO_INFO_AUDIO_ENC_T _get_mm_audio_enc(IMTK_PB_CTRL_AUD_ENC_T e_pb_aud_enc, 
                                                      UINT8 e_pb_pcm_type,
                                                      MM_AUD_PCM_TYPE_T *p_mm_pcm_type)
{
    MINFO_INFO_AUDIO_ENC_T e_mm_enc = MINFO_INFO_AUD_ENC_UNKNOWN;

    switch(e_pb_aud_enc)
    {
    case IMTK_PB_CTRL_AUD_ENC_PCM:
        switch(e_pb_pcm_type)
        {
        case IMTK_PB_CTRL_AUD_PCM_TYPE_MS_ADPCM:
            e_mm_enc = MINFO_INFO_AUD_ENC_ADPCM;
            if(p_mm_pcm_type)
            {
                *p_mm_pcm_type = MM_AUD_PCM_TYPE_MS_ADPCM;
            }
            break;
        case IMTK_PB_CTRL_AUD_PCM_TYPE_IMA_ADPCM:
            e_mm_enc = MINFO_INFO_AUD_ENC_ADPCM;
            if(p_mm_pcm_type)
            {
                *p_mm_pcm_type = MM_AUD_PCM_TYPE_IMA_ADPCM;
            }
            break;

        case IMTK_PB_CTRL_AUD_PCM_TYPE_NORMAL:
        default:
            e_mm_enc = MINFO_INFO_AUD_ENC_PCM;
            if(p_mm_pcm_type)
            {
                *p_mm_pcm_type = MM_AUD_PCM_TYPE_NORMAL;
            }
            break;
        }
        
        break;
    case IMTK_PB_CTRL_AUD_ENC_MPEG:
        e_mm_enc = MINFO_INFO_AUD_ENC_MPEG_1;

        break;
    case IMTK_PB_CTRL_AUD_ENC_MP3:
        e_mm_enc = MINFO_INFO_AUD_ENC_MPEG_1;

        break;
    case IMTK_PB_CTRL_AUD_ENC_AAC:
        e_mm_enc = MINFO_INFO_AUD_ENC_AAC;

        break;
    case IMTK_PB_CTRL_AUD_ENC_DD:
        e_mm_enc = MINFO_INFO_AUD_ENC_AC3;
    
        break;

    case IMTK_PB_CTRL_AUD_ENC_DDP:
        e_mm_enc = MINFO_INFO_AUD_ENC_EC3;
    
        break;

    case IMTK_PB_CTRL_AUD_ENC_DTS:
        e_mm_enc = MINFO_INFO_AUD_ENC_DTS;
        break;
    case IMTK_PB_CTRL_AUD_ENC_WMA:
        e_mm_enc = MINFO_INFO_AUD_ENC_WMA_V1;
        break;
    case IMTK_PB_CTRL_AUD_ENC_COOK:
        e_mm_enc = MINFO_INFO_AUD_ENC_COOK;
        break;
    default:
        break;
    }   

    return e_mm_enc;
}

#if 0
static UINT32 _get_rate_by_frm_rate(IMTK_PB_CTRL_FRAME_RATE_T eFrmRate)
{
    UINT32 ui4_rate = 0;
    
    switch (eFrmRate)
    {
    case IMTK_PB_CTRL_FRAME_RATE_23_976:
        ui4_rate = 23976;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_24:
        ui4_rate = 24000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_25:
        ui4_rate = 25000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_29_97:
        ui4_rate = 29970;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_30:
        ui4_rate = 30000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_50:
        ui4_rate = 50000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_59_94:
        ui4_rate = 59940;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_60:
        ui4_rate = 60000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_120:
        ui4_rate = 120000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_1:
        ui4_rate =  1000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_5:
        ui4_rate =  5000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_8:
        ui4_rate =  8000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_10:
        ui4_rate = 23976;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_12:
        ui4_rate = 10000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_15:
        ui4_rate = 15000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_2:
        ui4_rate =  2000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_6:
        ui4_rate =  6000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_48:
        ui4_rate = 48000;
        break;
    
    case IMTK_PB_CTRL_FRAME_RATE_70:
        ui4_rate = 70000;
        break;
        
    default:
        ui4_rate = 24000;
        break;
    }
    return ui4_rate;
}
#endif
static MEDIA_VID_SUBTYPE_T _get_vid_es_sub_type_by_encode(MINFO_INFO_VIDEO_ENC_T e_vid_enc)
{
    switch(e_vid_enc)
    {
        case MINFO_INFO_VID_ENC_MPEG_1:
            return MEDIA_VID_SUBTYPE_M1V;
            
        case MINFO_INFO_VID_ENC_MPEG_2:
            return MEDIA_VID_SUBTYPE_M2V;
            
        case MINFO_INFO_VID_ENC_MPEG_4:
            return MEDIA_VID_SUBTYPE_M4V;
            
        case MINFO_INFO_VID_ENC_H264:
            return MEDIA_VID_SUBTYPE_H264;
            
        case MINFO_INFO_VID_ENC_WVC1:
            return MEDIA_VID_SUBTYPE_VC1;
            
        default:
            return MEDIA_VID_SUBTYPE_UNKNOWN;
    }
}

static MEDIA_AUD_SUBTYPE_T _get_aud_es_sub_type_by_encode(MINFO_INFO_AUDIO_ENC_T e_aud_enc)
{
    switch(e_aud_enc)
    {
        case MINFO_INFO_AUD_ENC_MPEG_2:
            return MEDIA_AUD_SUBTYPE_MP2;
            
        case MINFO_INFO_AUD_ENC_MPEG_1:
            return MEDIA_AUD_SUBTYPE_MP3;
            
        case MINFO_INFO_AUD_ENC_WMA_V1:
        case MINFO_INFO_AUD_ENC_WMA_V2:
        case MINFO_INFO_AUD_ENC_WMA_V3:
            return MEDIA_AUD_SUBTYPE_WMA;
            
        case MINFO_INFO_AUD_ENC_LPCM:
            return MEDIA_AUD_SUBTYPE_LPCM;
            
        case MINFO_INFO_AUD_ENC_AAC:
            return MEDIA_AUD_SUBTYPE_AAC;
            
        /*case :
            return ;
            
        case :
            return ;
            
        case :
            return ;
    ,
    MEDIA_AUD_SUBTYPE_WAVE,
    MEDIA_AUD_SUBTYPE_AIF,
    MEDIA_AUD_SUBTYPE_AC3


    MINFO_INFO_AUD_ENC_UNKNOWN =0,
    MINFO_INFO_AUD_ENC_AC3,
    MINFO_INFO_AUD_ENC_PCM,
    MINFO_INFO_AUD_ENC_ADPCM,
    MINFO_INFO_AUD_ENC_TV_SYS,
    MINFO_INFO_AUD_ENC_DTS,
    MINFO_INFO_AUD_ENC_EU_CANAL_PLUS,
    MINFO_INFO_AUD_ENC_WMA_V2,
    MINFO_INFO_AUD_ENC_WMA_V3,
    MINFO_INFO_AUD_ENC_COOK,
    MINFO_INFO_AUD_ENC_VORBIS*/
            
        default:
            return MEDIA_AUD_SUBTYPE_UNKNOWN;
    }
}

static IMTK_PB_ERROR_CODE_T _push_mode_strm_info_convert(IMTK_PB_HANDLE_T  hHandle,
                                                         MM_STRM_INFO_T*   pt_mm_strm_info)
{
    CMPB_CTRL_INST_T*               pt_cmpb_inst;
    MINFO_INFO_VIDEO_ENC_T          e_vid_enc = (MINFO_INFO_VIDEO_ENC_T)0;
    MINFO_INFO_AUDIO_ENC_T          e_aud_enc = (MINFO_INFO_AUDIO_ENC_T)0;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);    
    if (pt_cmpb_inst == NULL)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    switch(pt_cmpb_inst->u_media.t_set_info.eMediaType)
    {
        case IMTK_PB_CTRL_MEDIA_TYPE_MTK_P0:
        {
            MM_AUD_PCM_TYPE_T                   e_pcm_type  = MM_AUD_PCM_TYPE_NORMAL;
            IMTK_PB_CTRL_MEDIA_MTK_P0_INFO_T*   ptMtkP0Info = NULL;
            MINFO_TYPE_STM_ATRBT_T*             pt_stm_atrbt= NULL;

            ptMtkP0Info  = &(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info);
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            
            e_vid_enc = _get_mm_video_enc(ptMtkP0Info->tVidInfo.eVidEnc);            
            e_aud_enc = _get_mm_audio_enc(ptMtkP0Info->tAudInfo.eAudEnc,
                                          ptMtkP0Info->tAudInfo.uAudCodecInfo.t_pcm_info.e_pcm_type,
                                          &e_pcm_type);

            pt_mm_strm_info->t_format.e_media_type = MEDIA_TYPE_CONTAINER;
            pt_mm_strm_info->t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_CMPB;
            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_CMPB_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                pt_vid_stm_atrbt = &(pt_stm_atrbt->u.t_cmpb_video_stm_attr);
                
                pt_vid_stm_atrbt->e_enc     = e_vid_enc;
                //pt_vid_stm_atrbt->ui1_sample_id = 0;
                pt_vid_stm_atrbt->i4_width  = ptMtkP0Info->tVidInfo.tAspRatio.u4DispWidth;
                pt_vid_stm_atrbt->i4_height = ptMtkP0Info->tVidInfo.tAspRatio.u4DispHeight;
                pt_vid_stm_atrbt->ui4_scale = 1000;
                //pt_vid_stm_atrbt->ui4_rate  = _get_rate_by_frm_rate(ptMtkP0Info->tVidInfo.eFrmRate);                
                //pt_vid_stm_atrbt->ui4_seq_hdr       = 0;
                //pt_vid_stm_atrbt->ui2_seq_hdr_size  = 0;
                //pt_vid_stm_atrbt->pu1_buff          = NULL;
                //pt_vid_stm_atrbt->ui4_buff_size     = 0;
                
                pt_stm_atrbt->b_is_codec_support = TRUE;
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_VIDEO;
                pt_mm_strm_info->ui2_num_stm++;                
            }
            
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_CMPB_AUDIO_STM_ATRBT_T* pt_aud_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                pt_stm_atrbt->b_is_codec_support = TRUE;
                //pt_stm_atrbt->t_stm_id           = 0;
                
                pt_aud_stm_atrbt = &(pt_stm_atrbt->u.t_cmpb_audio_stm_attr);
             
                pt_aud_stm_atrbt->e_enc             = e_aud_enc;
                pt_aud_stm_atrbt->ui4_stream_id     = 0;
                //pt_aud_stm_atrbt->ui2_hdr_src       = 0;
                //pt_aud_stm_atrbt->ui2_channels      = ptMtkP0Info->tAudInfo.uAudCodecInfo.tPcmInfo.u4NumCh;
                //pt_aud_stm_atrbt->ui4_sample_per_sec= 0;
                //pt_aud_stm_atrbt->ui4_bits_per_sec  = 0;
                //pt_aud_stm_atrbt->ui4_block_size  = 0;
                //pt_aud_stm_atrbt->ui2_encoder_opt = 0;
                //pt_aud_stm_atrbt->e_pcm_type        = e_pcm_type;
                //pt_aud_stm_atrbt->ui4_sample_rate   = ptMtkP0Info->tAudInfo.uAudCodecInfo.tPcmInfo.u4SampleRate;
                //pt_aud_stm_atrbt->ui2_block_align   = ptMtkP0Info->tAudInfo.uAudCodecInfo.tPcmInfo.u2BlockAlign;
                //pt_aud_stm_atrbt->ui2_bits_per_sample= ptMtkP0Info->tAudInfo.uAudCodecInfo.tPcmInfo.u2BitsPerSample;
                //pt_aud_stm_atrbt->fg_big_endian     = ptMtkP0Info->tAudInfo.uAudCodecInfo.tPcmInfo.fgBigEndian;
                if (ptMtkP0Info->tAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_MP3)
                {
                    pt_aud_stm_atrbt->ui1_mpg_layer = 3;
                }
                else
                {
                    pt_aud_stm_atrbt->ui1_mpg_layer = 0;
                }
                
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_AUDIO;
                pt_mm_strm_info->ui2_num_stm++;                
            }
        }
            break;
            
        case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS:
        {
            IMTK_PB_CTRL_MEDIA_MPEG2_TS_INFO_T* ptTsInfo    = NULL;
            MINFO_TYPE_STM_ATRBT_T*             pt_stm_atrbt= NULL;

            ptTsInfo     = &(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo);
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            
            e_vid_enc = _get_mm_video_enc(ptTsInfo->eVidInfo.eVidEnc);                
            e_aud_enc = _get_mm_audio_enc(ptTsInfo->eAudInfo.eAudEnc, 0, NULL);

            pt_mm_strm_info->t_format.e_media_type = MEDIA_TYPE_TS;
            
            if (ptTsInfo->ePacketType == IMTK_PB_CTRL_MPEG2_TS_PACKET_TYPE_188BYTE)
            {
                pt_mm_strm_info->t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_188;
            }
            else if (ptTsInfo->ePacketType == IMTK_PB_CTRL_MPEG2_TS_PACKET_TYPE_192BYTE)
            {
                pt_mm_strm_info->t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192;
            }

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                pt_stm_atrbt->b_is_codec_support = TRUE;                

                pt_stm_atrbt->u.t_ps_video_stm_attr.e_enc       = e_vid_enc;
                pt_stm_atrbt->t_stm_id.u.t_ps_stm_id.ui1_stm_id = ptTsInfo->eVidInfo.u2Pid;
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_VIDEO;
                pt_mm_strm_info->ui2_num_stm++;
            }
            
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                pt_stm_atrbt->b_is_codec_support= TRUE;

                pt_stm_atrbt->u.t_ps_audio_stm_attr.e_enc       = e_aud_enc;
                pt_stm_atrbt->t_stm_id.u.t_ps_stm_id.ui1_stm_id = ptTsInfo->eAudInfo.u2Pid;
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_AUDIO;
                pt_mm_strm_info->ui2_num_stm++;
            }
        }
            break;
            
        case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_PS:
        {
            IMTK_PB_CTRL_MEDIA_MPEG2_PS_INFO_T* ptPsInfo    = NULL;
            MINFO_TYPE_STM_ATRBT_T*             pt_stm_atrbt= NULL;

            ptPsInfo     = &(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tPsInfo);
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            
            e_vid_enc = _get_mm_video_enc(ptPsInfo->eVidInfo.eVidEnc);            
            e_aud_enc = _get_mm_audio_enc(ptPsInfo->eAudInfo.eAudEnc, 0, NULL);

            pt_mm_strm_info->t_format.e_media_type = MEDIA_TYPE_PS;
            pt_mm_strm_info->t_format.t_media_subtype.u.e_ps_subtype = MEDIA_PS_SUBTYPE_MPG;

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_PS_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt->b_is_codec_support= TRUE;
                pt_stm_atrbt->t_stm_id.u.t_ps_stm_id.ui1_stm_id 
                            = ptPsInfo->eVidInfo.tVidStrmIdInfo.u1StrmId;
                pt_stm_atrbt->t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = 0;
                
                pt_vid_stm_atrbt = &(pt_stm_atrbt->u.t_ps_video_stm_attr);
                
                pt_vid_stm_atrbt->ui1_stream_id = ptPsInfo->eVidInfo.tVidStrmIdInfo.u1StrmId;
                pt_vid_stm_atrbt->e_enc         = e_vid_enc;
                //pt_vid_stm_atrbt->e_vid_fmt     = MINFO_INFO_VID_FMT_UNKNOWN;
                //pt_vid_stm_atrbt->i4_width      = 0;
                //pt_vid_stm_atrbt->i4_height     = 0;
                //pt_vid_stm_atrbt->b_default     = 0;
    
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_VIDEO;
                pt_mm_strm_info->ui2_num_stm++;                
            }
            
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_PS_AUDIO_STM_ATRBT_T* pt_aud_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt->b_is_codec_support= TRUE;
                pt_stm_atrbt->t_stm_id.u.t_ps_stm_id.ui1_stm_id 
                    = ptPsInfo->eAudInfo.tAudStrmIdInfo.u1StrmId;
                pt_stm_atrbt->t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id 
                    = ptPsInfo->eAudInfo.tAudStrmIdInfo.u1SubStrmIdPriStrm;
                
                pt_aud_stm_atrbt = &(pt_stm_atrbt->u.t_ps_audio_stm_attr);

                pt_aud_stm_atrbt->e_enc                 = e_aud_enc;
                if(ptPsInfo->eAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_MP3)
                {
                    pt_aud_stm_atrbt->e_layer = 3;
                }
                //pt_aud_stm_atrbt->ui1_sub_stream_id     = 0;
                //pt_aud_stm_atrbt->i4_samples_per_quant  = 0;
                //pt_aud_stm_atrbt->i4_sampling_frequency = 0;
                pt_aud_stm_atrbt->i2_channels           =  1;//ptPsInfo->eAudInfo.uAudCodecInfo.t_pcm_info.u4NumCh;
                pt_aud_stm_atrbt->b_default             = 0;

                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_AUDIO;
                pt_mm_strm_info->ui2_num_stm++;  
            }
        }
            break;
#if 0
        case IMTK_PB_CTRL_MEDIA_TYPE_AVI:
        {
            IMTK_PB_CTRL_MEDIA_AVI_INFO_T*  ptAviInfo    = NULL;
            MINFO_TYPE_STM_ATRBT_T*         pt_stm_atrbt = NULL;

            ptAviInfo     = &(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo);
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            
            e_vid_enc = _get_mm_video_enc(ptAviInfo->tAviVidInfo.eVidEnc);            
            e_aud_enc = _get_mm_audio_enc(ptAviInfo->tAviAudInfo.eAudEnc, 0, NULL);

            pt_mm_strm_info->t_format.e_media_type = MEDIA_TYPE_CONTAINER;
            pt_mm_strm_info->t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_AVI;

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_AVI_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt->t_stm_id.u.t_avi_stm_id   = ptAviInfo->tAviVidInfo.u1StreamIdx;
                /*pt_stm_atrbt->t_stm_id.u.t_avi_stm_id   = (UINT32)ptAviInfo->tAviVidInfo.au1FourCC[3] << 24 | 
                                                          (UINT32)ptAviInfo->tAviVidInfo.au1FourCC[2] << 16 | 
                                                          (UINT32)ptAviInfo->tAviVidInfo.au1FourCC[1] << 8  |
                                                          (UINT32)ptAviInfo->tAviVidInfo.au1FourCC[0];  
                 */
                pt_stm_atrbt->b_is_codec_support        = TRUE;
                
                pt_vid_stm_atrbt = &(pt_stm_atrbt->u.t_avi_video_stm_attr);                

                pt_vid_stm_atrbt->e_enc             = e_vid_enc;
                pt_vid_stm_atrbt->ui4_scale         = ptAviInfo->tAviVidInfo.u4Scale;
                pt_vid_stm_atrbt->ui4_rate          = ptAviInfo->tAviVidInfo.u4Rate;
                //pt_vid_stm_atrbt->ui4_sample_size   = 0;
                pt_vid_stm_atrbt->i4_width          = ptAviInfo->tAviVidInfo.u4Width;
                pt_vid_stm_atrbt->i4_height         = ptAviInfo->tAviVidInfo.u4Height;
                //pt_vid_stm_atrbt->ui4_length        = 0;
                //pt_vid_stm_atrbt->ui8_duration      = 0;
                //pt_vid_stm_atrbt->b_default         = 0;
                //pt_vid_stm_atrbt->ui4_codec_info_size = 0;
                //pt_vid_stm_atrbt->pui1_codec_info   = NULL;
                //pt_vid_stm_atrbt->ui4_indx_offset   = 0;
                //pt_vid_stm_atrbt->ui4_indx_size     = 0;

                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_VIDEO;
                pt_mm_strm_info->ui2_num_stm++;                
            }
            
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_AVI_AUDIO_STM_ATRBT_T* pt_aud_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                pt_stm_atrbt->b_is_codec_support        = TRUE;          
                pt_stm_atrbt->t_stm_id.u.t_avi_stm_id   = ptAviInfo->tAviAudInfo.u1StreamIdx;
                /*pt_stm_atrbt->t_stm_id.u.t_avi_stm_id   = (UINT32)ptAviInfo->tAviAudInfo.au1FourCC[3] << 24 | 
                                                          (UINT32)ptAviInfo->tAviAudInfo.au1FourCC[2] << 16 | 
                                                          (UINT32)ptAviInfo->tAviAudInfo.au1FourCC[1] << 8  |
                                                          (UINT32)ptAviInfo->tAviAudInfo.au1FourCC[0];  
                */
                pt_aud_stm_atrbt = &(pt_stm_atrbt->u.t_avi_audio_stm_attr);
                
                pt_aud_stm_atrbt->e_enc             = e_aud_enc;
                if(ptAviInfo->tAviAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_MP3)
                {
                    pt_aud_stm_atrbt->e_layer = 3;
                }
                //pt_aud_stm_atrbt->e_layer           = 0;
                pt_aud_stm_atrbt->ui4_scale         = ptAviInfo->tAviAudInfo.u4Scale;
                pt_aud_stm_atrbt->ui4_rate          = ptAviInfo->tAviAudInfo.u4Rate;
                if(ptAviInfo->tAviAudInfo.fgVbr)
                {
                    pt_aud_stm_atrbt->ui4_sample_size = 0;
                }
                else
                {
                    pt_aud_stm_atrbt->ui4_sample_size = 1;
                }
                pt_aud_stm_atrbt->i2_channels       = 1;//(INT16)ptAviInfo->tAviAudInfo.uAudCodecInfo.t_pcm_info.u4NumCh;
                //pt_aud_stm_atrbt->i4_samples_per_sec         = 0;
                pt_aud_stm_atrbt->i2_block_align    = (INT16)ptAviInfo->tAviAudInfo.uAudCodecInfo.t_pcm_info.u2BlockAlign;
                pt_aud_stm_atrbt->i2_bits_per_sample= (INT16)ptAviInfo->tAviAudInfo.uAudCodecInfo.t_pcm_info.u2BitsPerSample;
                pt_aud_stm_atrbt->ui4_avg_bytes_sec         = ptAviInfo->tAviAudInfo.u4Bps;
                //pt_aud_stm_atrbt->ui4_length        = 0;
                //pt_aud_stm_atrbt->ui8_duration      = 0;
                //pt_aud_stm_atrbt->ui2_encode_opts   = 0;
                //pt_aud_stm_atrbt->s_lang            = 0;
                //pt_aud_stm_atrbt->b_default         = 0;
                //pt_aud_stm_atrbt->ui4_codec_info_size= 0;
                //pt_aud_stm_atrbt->pui1_codec_info   = 0;
                //pt_aud_stm_atrbt->ui4_indx_offset   = 0;
                //pt_aud_stm_atrbt->ui4_indx_size     = 0;
                //pt_aud_stm_atrbt->ui4_cbr_sub_vbr   = 0;
                
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_AUDIO;  
                pt_mm_strm_info->ui2_num_stm++;                
            }
        }
            break;
            
        case IMTK_PB_CTRL_MEDIA_TYPE_ASF:
        {
            IMTK_PB_CTRL_MEDIA_ASF_INFO_T*  ptAsfInfo    = NULL;
            MINFO_TYPE_STM_ATRBT_T*         pt_stm_atrbt = NULL;

            ptAsfInfo     = &(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo);
            pt_stm_atrbt  = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            
            e_vid_enc = _get_mm_video_enc(ptAsfInfo->tAsfVidInfo.eVidEnc);            
            e_aud_enc = _get_mm_audio_enc(ptAsfInfo->tAsfAudInfo.eAudEnc, 0, NULL);

            pt_mm_strm_info->t_format.e_media_type = MEDIA_TYPE_CONTAINER;
            pt_mm_strm_info->t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_ASF;

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_ASF_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt->b_is_codec_support = TRUE;
                pt_stm_atrbt->t_stm_id.u.t_asf_stm_id = ptAsfInfo->tAsfVidInfo.u1StreamIdx;
                
                pt_vid_stm_atrbt = &(pt_stm_atrbt->u.t_asf_video_stm_attr);
                
                pt_vid_stm_atrbt->e_enc     = e_vid_enc;
                pt_vid_stm_atrbt->ui4_scale = ptAsfInfo->tAsfVidInfo.u4_Scale;
                pt_vid_stm_atrbt->ui4_rate  = ptAsfInfo->tAsfVidInfo.u4_Rate;
                switch(e_vid_enc)
                {
                case MINFO_INFO_VID_ENC_WMV1:
                case MINFO_INFO_VID_ENC_WMV2:
                case MINFO_INFO_VID_ENC_WMV3:
                case MINFO_INFO_VID_ENC_DIVX_311:
                case MINFO_INFO_VID_ENC_DIVX_4:
                case MINFO_INFO_VID_ENC_DIVX_5:
                    pt_vid_stm_atrbt->i4_width  = 
                        ptAsfInfo->tAsfVidInfo.uVidCodecInfo.t_wmv3_info.u4Width;
                    pt_vid_stm_atrbt->i4_height = 
                        ptAsfInfo->tAsfVidInfo.uVidCodecInfo.t_wmv3_info.u4Height;
                    break;

                default:
                    break;
                }
                
                //pt_vid_stm_atrbt->b_default = 0;
                pt_vid_stm_atrbt->ui4_codec_info_size = ptAsfInfo->tAsfVidInfo.u2CodecPrivInfoLen;
                pt_vid_stm_atrbt->pui1_codec_info = ptAsfInfo->tAsfVidInfo.au1CodecPrivInfo;
                pt_vid_stm_atrbt->ui4_packet_sz = ptAsfInfo->u4DataPacketSize;

                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_VIDEO;
                pt_mm_strm_info->ui2_num_stm++;
            }
            
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                static UINT8 aui1_array[10];
                MINFO_ASF_AUDIO_STM_ATRBT_T* pt_aud_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt->b_is_codec_support        = TRUE;
                pt_stm_atrbt->t_stm_id.u.t_asf_stm_id   = ptAsfInfo->tAsfAudInfo.u1StreamIdx;
                
                pt_aud_stm_atrbt = &(pt_stm_atrbt->u.t_asf_audio_stm_attr);
                
                pt_aud_stm_atrbt->e_enc             = e_aud_enc;
                if(ptAsfInfo->tAsfAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_MP3)
                {
                    pt_aud_stm_atrbt->e_layer = 3;
                }

                pt_aud_stm_atrbt->i2_channels       = (INT16)ptAsfInfo->tAsfAudInfo.uAudCodecInfo.t_wma_info.u2NumCh;
                pt_aud_stm_atrbt->i4_samples_per_sec= (INT32)ptAsfInfo->tAsfAudInfo.uAudCodecInfo.t_wma_info.u4SamplePerSec;
                pt_aud_stm_atrbt->i2_block_align    = (INT16)ptAsfInfo->tAsfAudInfo.uAudCodecInfo.t_wma_info.u4BlockSz;
                pt_aud_stm_atrbt->ui4_avg_bytes_per_sec = ptAsfInfo->tAsfAudInfo.uAudCodecInfo.t_wma_info.u4BitsPerSec;
                pt_aud_stm_atrbt->ui4_codec_info_size = 8;
                pt_aud_stm_atrbt->pui1_codec_info = aui1_array;
                aui1_array[4] = (UINT8)(ptAsfInfo->tAsfAudInfo.uAudCodecInfo.tWmaInfo.u2EncoderOpt);
                aui1_array[5] = (UINT8)(ptAsfInfo->tAsfAudInfo.uAudCodecInfo.tWmaInfo.u2EncoderOpt >> 8);
                
                if(ptAsfInfo->tAsfAudInfo.uAudCodecInfo.tWmaInfo.u4SamplePerSec != 0)
                {
                    pt_aud_stm_atrbt->i2_bits_per_sample= (INT16)ptAsfInfo->tAsfAudInfo.uAudCodecInfo.t_wma_info.u4BitsPerSec /
                                                      (INT16)ptAsfInfo->tAsfAudInfo.uAudCodecInfo.t_wma_info.u4SamplePerSec;
                }
                else
                {
                    pt_aud_stm_atrbt->i2_bits_per_sample = 0;
                }
                //pt_aud_stm_atrbt->s_lang          = 0;
                //pt_aud_stm_atrbt->b_default       = 0;
                
                pt_aud_stm_atrbt->ui4_packet_sz   = ptAsfInfo->u4DataPacketSize;
                
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_AUDIO;
                pt_mm_strm_info->ui2_num_stm++;
            }
        }
            break;
            
        case IMTK_PB_CTRL_MEDIA_TYPE_MKV:
        {
            IMTK_PB_CTRL_MEDIA_MKV_INFO_T*  ptMkvInfo    = NULL;
            MINFO_TYPE_STM_ATRBT_T*         pt_stm_atrbt = NULL;

            ptMkvInfo     = &(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo);
            pt_stm_atrbt  = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            
            e_vid_enc = _get_mm_video_enc(ptMkvInfo->tMkvVidInfo.uVidCodec.eVidEnc);            
            e_aud_enc = _get_mm_audio_enc(ptMkvInfo->tMkvAudInfo.eAudEnc, 0, NULL);

            pt_mm_strm_info->t_format.e_media_type = MEDIA_TYPE_CONTAINER;
            pt_mm_strm_info->t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MATROSKA;

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_MKV_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt->b_is_codec_support = TRUE;
                pt_stm_atrbt->t_stm_id.u.t_mkv_stm_id = ptMkvInfo->tMkvVidInfo.u8VidTrackNo;
                
                pt_vid_stm_atrbt = &(pt_stm_atrbt->u.t_mkv_video_stm_attr);
                
                pt_vid_stm_atrbt->e_enc         = e_vid_enc;
                //pt_vid_stm_atrbt->e_codec_type  = MINFO_MKV_V_CODEC_TYPE_UNKNOWN;
                //pt_vid_stm_atrbt->ui4_scale     = 0;
                //pt_vid_stm_atrbt->ui4_rate      = 0;
//                pt_vid_stm_atrbt->t_track_timecode_scale = ptMkvInfo->tMkvVidInfo.u8TrackTimeCodeScale;                
                //pt_vid_stm_atrbt->i4_width      = 0;
                //pt_vid_stm_atrbt->i4_height     = 0;
                //pt_vid_stm_atrbt->i4_dsp_width  = 0;
                //pt_vid_stm_atrbt->i4_dsp_height = 0;
                //pt_vid_stm_atrbt->b_default     = 0;
                //pt_vid_stm_atrbt->b_divx_plus   = 0;
                pt_vid_stm_atrbt->ui4_codec_info_size = ptMkvInfo->tMkvVidInfo.u2CodecPrivInfoLen;
                pt_vid_stm_atrbt->pui1_codec_info     = ptMkvInfo->tMkvVidInfo.au1CodecPrivInfo;
                
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_VIDEO;
                pt_mm_strm_info->ui2_num_stm++;
            }
            
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_MKV_AUDIO_STM_ATRBT_T* pt_aud_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                pt_stm_atrbt->b_is_codec_support = TRUE;
                pt_stm_atrbt->t_stm_id.u.t_mkv_stm_id = ptMkvInfo->tMkvAudInfo.u8AudTrackNo;
                
                pt_aud_stm_atrbt = &(pt_stm_atrbt->u.t_mkv_audio_stm_attr);
                
                pt_aud_stm_atrbt->e_enc     = e_aud_enc;
                if(ptMkvInfo->tMkvAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_MP3)
                {
                    pt_aud_stm_atrbt->e_layer = 3;
                }
                //pt_aud_stm_atrbt->ui4_scale = 0;
                //pt_aud_stm_atrbt->ui4_rate = 0;
//                pt_aud_stm_atrbt->t_track_timecode_scale = ptMkvInfo->tMkvAudInfo.u8TrackTimeCodeScale;
                pt_aud_stm_atrbt->i2_channels = (INT16)ptMkvInfo->tMkvAudInfo.uAudCodecInfo.t_pcm_info.u4NumCh;
                pt_aud_stm_atrbt->i2_block_align = (INT16)ptMkvInfo->tMkvAudInfo.uAudCodecInfo.t_pcm_info.u2BlockAlign;
                pt_aud_stm_atrbt->i2_bits_per_sample = (INT16)ptMkvInfo->tMkvAudInfo.uAudCodecInfo.t_pcm_info.u2BitsPerSample;
                pt_aud_stm_atrbt->i4_samples_per_sec = (INT32)ptMkvInfo->tMkvAudInfo.uAudCodecInfo.t_pcm_info.u4SampleRate;
                if(ptMkvInfo->tMkvAudInfo.uAudCodecInfo.tPcmInfo.fgBigEndian)
                {
                    pt_aud_stm_atrbt->e_align = MINFO_INFO_AUD_PCM_BIG_ENDIAN;
                }
                else
                {
                    pt_aud_stm_atrbt->e_align = MINFO_INFO_AUD_PCM_LITTLE_ENDIAN;
                }
                //pt_aud_stm_atrbt->ui4_avg_bytes_sec = 0;
                //pt_aud_stm_atrbt->ui2_encode_opts   = 0;
                //pt_aud_stm_atrbt->s_lang            = 0;
                //pt_aud_stm_atrbt->b_default         = 0;
                //pt_aud_stm_atrbt->b_divx_plus       = 0;
                //pt_aud_stm_atrbt->ui4_codec_info_size = 0;
                //pt_aud_stm_atrbt->pui1_codec_info   = 0;
                
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_AUDIO;
                pt_mm_strm_info->ui2_num_stm++;                
            }
        }
            break;
            
        case IMTK_PB_CTRL_MEDIA_TYPE_MP4:/* Lib Master only ??? */
            break;
#endif
        case IMTK_PB_CTRL_MEDIA_TYPE_VIDEO_ES:
        {
            IMTK_PB_CTRL_MEDIA_VIDEO_ES_INFO_T* ptVideoEsInfo   = NULL;
            MINFO_TYPE_STM_ATRBT_T*             pt_stm_atrbt    = NULL;

            ptVideoEsInfo   = &(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tVideoEsInfo);
            pt_stm_atrbt    = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);


            
            e_vid_enc = _get_mm_video_enc(ptVideoEsInfo->eVidEnc);            

            pt_mm_strm_info->t_format.e_media_type = MEDIA_TYPE_VIDEO_ES;
            pt_mm_strm_info->t_format.t_media_subtype.u.e_vid_subtype
                                    = _get_vid_es_sub_type_by_encode(e_vid_enc);

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                pt_stm_atrbt->b_is_codec_support = TRUE;
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_VIDEO;
                pt_mm_strm_info->ui2_num_stm++;
            }
        }
            break;
        case IMTK_PB_CTRL_MEDIA_TYPE_AUDIO_ES:
        {
            IMTK_PB_CTRL_MEDIA_AUDIO_ES_INFO_T* ptAudioEsInfo   = NULL;
            MINFO_TYPE_STM_ATRBT_T*             pt_stm_atrbt    = NULL;

            ptAudioEsInfo   = &(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAudioEsInfo);
            pt_stm_atrbt    = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);

            
            e_aud_enc = _get_mm_audio_enc(ptAudioEsInfo->tAudInfo.eAudEnc, 0, NULL);            

            pt_mm_strm_info->t_format.e_media_type = MEDIA_TYPE_AUDIO_ES;
            pt_mm_strm_info->t_format.t_media_subtype.u.e_aud_subtype
                                    = _get_aud_es_sub_type_by_encode(e_aud_enc);

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                pt_stm_atrbt->b_is_codec_support = TRUE;
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_VIDEO;
                pt_mm_strm_info->ui2_num_stm++;
            }
        }        
            break;
#if 0
        case IMTK_PB_CTRL_MEDIA_TYPE_OGG:/* Lib Master only ??? */
            break;
            
        case IMTK_PB_CTRL_MEDIA_TYPE_WAV:/* Lib Master only ??? */
            break;
        case IMTK_PB_CTRL_MEDIA_TYPE_RM:/* Lib Master only ??? */
        {
            IMTK_PB_CTRL_MEDIA_RM_INFO_T*   ptRmInfo    = NULL;
            MINFO_TYPE_STM_ATRBT_T*         pt_stm_atrbt = NULL;

            ptRmInfo     = &(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tRmInfo);
            pt_stm_atrbt  = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            
            e_vid_enc = _get_mm_video_enc(ptRmInfo->tRmVidInfo.eVidEnc);            
            e_aud_enc = _get_mm_audio_enc(ptRmInfo->tRmAudInfo.eAudEnc, 0, NULL);

            pt_mm_strm_info->t_format.e_media_type = MEDIA_TYPE_CONTAINER;
            pt_mm_strm_info->t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_RM;

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_RM_VIDEO_STM_ATRBT_T*     pt_vid_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt->b_is_codec_support = TRUE;
                pt_stm_atrbt->t_stm_id.u.t_rm_stm_id = 0;
                
                pt_vid_stm_atrbt = &(pt_stm_atrbt->u.t_rm_video_stm_attr);
                
                pt_vid_stm_atrbt->e_enc             = e_vid_enc;
                //pt_vid_stm_atrbt->ui2_strm_num      = 0;
                //pt_vid_stm_atrbt->ui4_preroll       = 0;
                pt_vid_stm_atrbt->ui4_duration      = (UINT32)-1;                
                //pt_vid_stm_atrbt->ui4_max_bit_rate  = 0;
                //pt_vid_stm_atrbt->ui4_avg_bit_rate  = 0;
                //pt_vid_stm_atrbt->ui4_max_packet_size= 0;
                //pt_vid_stm_atrbt->ui4_avg_packet_size= 0;
                //pt_vid_stm_atrbt->ui4_start_time    = 0;
                //pt_vid_stm_atrbt->ui4_codec_id      = 0;
                pt_vid_stm_atrbt->ui2_frm_width     = ptRmInfo->tRmVidInfo.ui2_frm_width;
                pt_vid_stm_atrbt->ui2_frm_height    = ptRmInfo->tRmVidInfo.ui2_frm_height;
                //pt_vid_stm_atrbt->ui2_bit_count     = 0;
                //pt_vid_stm_atrbt->ui2_pad_width     = 0;
                //pt_vid_stm_atrbt->ui2_pad_height    = 0;
                pt_vid_stm_atrbt->ui4_frm_rate    = _cmpb_map_vid_frm_rate(ptRmInfo->tRmVidInfo.uVidCodecInfo.t_rv9_10_info.eFrmRate);

                pt_vid_stm_atrbt->ui4_spo_extra_flags= ptRmInfo->tRmVidInfo.ui4_spo_extra_flags;
                //pt_vid_stm_atrbt->ui4_version       = 0;
                pt_vid_stm_atrbt->ui4_specific      = (UINT32)ptRmInfo->tRmVidInfo.aui4_specific;
                //pt_vid_stm_atrbt->ui1_ecc_mask      = 0;                

                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_VIDEO;
                pt_mm_strm_info->ui2_num_stm++;
            }
            
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_RM_AUDIO_STM_ATRBT_T*     pt_aud_stm_atrbt = NULL;
                UINT8*                          pui1_opaque_data;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                pt_stm_atrbt->b_is_codec_support = TRUE;
                pt_stm_atrbt->t_stm_id.u.t_rm_stm_id = 1;
                
                pt_aud_stm_atrbt = &(pt_stm_atrbt->u.t_rm_audio_stm_attr);
                
                pt_aud_stm_atrbt->e_enc             = e_aud_enc;                
                pt_aud_stm_atrbt->ui2_strm_num      = 1; 
                pt_aud_stm_atrbt->ui4_preroll       = 0; 
                pt_aud_stm_atrbt->ui4_duration      = (UINT32)-1; 
                //pt_aud_stm_atrbt->ui4_max_bit_rate  = 0; 
                //pt_aud_stm_atrbt->ui4_avg_bit_rate  = 0; 
                //pt_aud_stm_atrbt->ui4_max_packet_size = 0; 
                //pt_aud_stm_atrbt->ui4_avg_packet_size = 0; 
                //pt_aud_stm_atrbt->ui4_start_time    = 0; 
                //pt_aud_stm_atrbt->ui2_version       = 0; 
                //pt_aud_stm_atrbt->ui2_revision      = 0; 
                //pt_aud_stm_atrbt->ui2_hdr_bytes     = 0; 
                pt_aud_stm_atrbt->ui2_flavor_idx = ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u2FlavorIdx; 
                //pt_aud_stm_atrbt->ui4_granularity   = 0; 
                //pt_aud_stm_atrbt->ui4_total_bytes   = 0; 
                //pt_aud_stm_atrbt->ui4_bytes_per_min = 0; 
                //pt_aud_stm_atrbt->ui4_bytes_per_min2= 0; 
                pt_aud_stm_atrbt->ui2_intlv_factor  = ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u2CookInlvFac; 
                pt_aud_stm_atrbt->ui2_intlv_blk_sz  = ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u2CookBlockSz; 
                
                pt_aud_stm_atrbt->ui2_codec_frm_sz  = ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u4FrameSize; 
                //pt_aud_stm_atrbt->ui4_user_data     = 0; 
                pt_aud_stm_atrbt->ui4_sample_rate   = _cmpb_map_aud_sample_rate(ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.eSampleRate); 
                pt_aud_stm_atrbt->ui4_sample_rate   <<= 16;
                //pt_aud_stm_atrbt->ui4_act_sample_rate= 0; 
                pt_aud_stm_atrbt->ui2_sample_sz     = ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u2SamplePerFrame; 
                pt_aud_stm_atrbt->ui2_num_channel   = _cmpb_map_channels(ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.eNumCh); 
                //pt_aud_stm_atrbt->ui4_intlvr_id     = 0; 
                //pt_aud_stm_atrbt->ui4_codec_id      = 0; 
                //pt_aud_stm_atrbt->ui1_intlv_flag    = 0; 
                //pt_aud_stm_atrbt->ui1_can_copy_flag = 0; 
                //pt_aud_stm_atrbt->ui1_strm_type     = 0; 
                //pt_aud_stm_atrbt->ui1_has_intlv_ptrn= 0; 
                //pt_aud_stm_atrbt->pv_intlv_ptrn     = 0; 
                pt_aud_stm_atrbt->ui4_opaque_data_sz= 16;
                pui1_opaque_data = CMPB_MEM_CALLOC(1,pt_aud_stm_atrbt->ui4_opaque_data_sz); 
                pt_aud_stm_atrbt->pv_codec_opaque_data = pui1_opaque_data;      
                if(pt_aud_stm_atrbt->pv_codec_opaque_data == NULL)
                {   /*FIX ME:  this pointer should be copy into another buffer when do media_open fuction */
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }
                CMPB_MEMSET(pui1_opaque_data, 0, pt_aud_stm_atrbt->ui4_opaque_data_sz);

                pui1_opaque_data[6] = (UINT8)(ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u2RegionNum >> 8);
                pui1_opaque_data[7] = (UINT8)(ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u2RegionNum);
                if(ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u4CplRegionStart != 0 &&
                   ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u2QBitsNum != 0 )
                {
                    pui1_opaque_data[0] = 1; /*major version*/
                    pui1_opaque_data[3] = 3; /*minor version*/

                    pui1_opaque_data[12] = (UINT8)(ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u4CplRegionStart >> 8);
                    pui1_opaque_data[13] = (UINT8)(ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u4CplRegionStart);
                    pui1_opaque_data[14] = (UINT8)(ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u2QBitsNum >> 8);
                    pui1_opaque_data[15] = (UINT8)(ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u2QBitsNum);
                }
                //ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u4FrameSize
                //ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u2RegionNum
                //ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u4CplRegionStart
                //ptRmInfo->tRmAudInfo.uAudCodecInfo.t_cook_info.u2QBitsNum
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_AUDIO;
                pt_mm_strm_info->ui2_num_stm++;                
            }
        }
            break;
        case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS_TIMESHIFT:
            break;
#endif
        case IMTK_PB_CTRL_MEDIA_TYPE_FLV:
        {
            IMTK_PB_CTRL_MEDIA_FLV_INFO_T*  ptFlvInfo    = NULL;
            MINFO_TYPE_STM_ATRBT_T*         pt_stm_atrbt = NULL;

            ptFlvInfo    = &(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tFlvInfo);
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            
            e_vid_enc = _get_mm_video_enc(ptFlvInfo->tFlvVidInfo.eVidEnc);            
            e_aud_enc = _get_mm_audio_enc(ptFlvInfo->tFlvAudInfo.eAudEnc, 0, NULL);

            pt_mm_strm_info->t_format.e_media_type = MEDIA_TYPE_CONTAINER;
            pt_mm_strm_info->t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_FLV;

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_FLV_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt->t_stm_id.u.t_flv_stm_id   = ptFlvInfo->tFlvVidInfo.u1StreamIdx; 
                pt_stm_atrbt->b_is_codec_support        = TRUE;
                
                pt_vid_stm_atrbt = &(pt_stm_atrbt->u.t_flv_video_stm_attr);                

                pt_vid_stm_atrbt->e_enc             = e_vid_enc;
                pt_vid_stm_atrbt->ui4_scale         = ptFlvInfo->tFlvVidInfo.u4Scale;
                pt_vid_stm_atrbt->ui4_rate          = ptFlvInfo->tFlvVidInfo.u4Rate;
                //pt_vid_stm_atrbt->ui4_sample_size   = 0;
                pt_vid_stm_atrbt->i4_width          = ptFlvInfo->tFlvVidInfo.u4Width;
                pt_vid_stm_atrbt->i4_height         = ptFlvInfo->tFlvVidInfo.u4Height;
                //pt_vid_stm_atrbt->ui4_length        = 0;
                pt_vid_stm_atrbt->ui8_duration      = (UINT64)-1;
                //pt_vid_stm_atrbt->b_default         = 0;
                //pt_vid_stm_atrbt->ui4_codec_info_size = 0;
                //pt_vid_stm_atrbt->pui1_codec_info   = NULL;
                //pt_vid_stm_atrbt->ui4_indx_offset   = 0;
                //pt_vid_stm_atrbt->ui4_indx_size     = 0;

                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_VIDEO;
                pt_mm_strm_info->ui2_num_stm++;                
            }
            
            pt_stm_atrbt = &(pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].t_stm_atrbt);
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_FLV_AUDIO_STM_ATRBT_T* pt_aud_stm_atrbt = NULL;

                if(pt_mm_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                pt_stm_atrbt->b_is_codec_support        = TRUE;          
                pt_stm_atrbt->t_stm_id.u.t_flv_stm_id   = ptFlvInfo->tFlvAudInfo.u1StreamIdx;
                pt_aud_stm_atrbt = &(pt_stm_atrbt->u.t_flv_audio_stm_attr);
                
                pt_aud_stm_atrbt->e_enc             = e_aud_enc;
                if(ptFlvInfo->tFlvAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_MP3)
                {
                    pt_aud_stm_atrbt->e_layer = 3;
                }
                //pt_aud_stm_atrbt->e_layer           = 0;

                if(ptFlvInfo->tFlvAudInfo.fgVbr)
                {
                    pt_aud_stm_atrbt->i2_bits_per_sample = 0;
                }
                else
                {
                    pt_aud_stm_atrbt->i2_bits_per_sample = 16;
                }
                pt_aud_stm_atrbt->i2_channels       = _cmpb_map_channels(ptFlvInfo->tFlvAudInfo.uAudCodecInfo.t_pcm_info.eNumCh);;
                pt_aud_stm_atrbt->i4_samples_per_sec  = (INT32)ptFlvInfo->tFlvAudInfo.uAudCodecInfo.t_pcm_info.eBitsPerSample;
                pt_aud_stm_atrbt->ui4_avg_bytes_per_sec = ptFlvInfo->tFlvAudInfo.u4Bps;
                pt_aud_stm_atrbt->ui8_duration      = (UINT64)-1;

                //pt_aud_stm_atrbt->b_default         = 0;
                //pt_aud_stm_atrbt->ui4_codec_info_size= 0;
                //pt_aud_stm_atrbt->pui1_codec_info   = 0;
                //pt_aud_stm_atrbt->ui4_indx_offset   = 0;
                //pt_aud_stm_atrbt->ui4_indx_size     = 0;
                //pt_aud_stm_atrbt->ui4_cbr_sub_vbr   = 0;
                
                pt_mm_strm_info->at_stm_atrbt[pt_mm_strm_info->ui2_num_stm].e_type = ST_AUDIO;  
                pt_mm_strm_info->ui2_num_stm++;                
            }
        }
            break;
        default:
            break;                
    }
    return IMTK_PB_ERROR_CODE_OK;
}
static IMTK_PB_ERROR_CODE_T _push_mode_open_media(IMTK_PB_HANDLE_T     hHandle)
{
    CMPB_CTRL_INST_T*               pt_cmpb_inst;
    INT32                           i4_ret;
    MM_SVC_COMMAND_T                at_cmd[7];
    MM_STRM_INFO_T                  t_mm_strm_info;
    IMTK_CTRL_PUSH_INFO_T           t_push_info;
    IMTK_PB_CTRL_SETTING_T          t_Setting;

    CMPB_MEMSET(&t_mm_strm_info, 0, sizeof(MM_STRM_INFO_T));
    CMPB_MEMSET(&t_push_info, 0, sizeof(IMTK_CTRL_PUSH_INFO_T));
    CMPB_MEMSET(&t_Setting, 0, sizeof(IMTK_PB_CTRL_SETTING_T));

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (pt_cmpb_inst == NULL)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 

    /*------------------prepare param ------------------------*/
        t_push_info.hHandle = hHandle;
        t_push_info.ui4_svc_id = 0;
        t_push_info.ui8_preroll = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.u8PrerollTime;
        t_push_info.pf_nfy = (x_playback_push_nfy_fct)_cmpb_ctrl_push_nfy_fct;

        _push_mode_strm_info_convert(hHandle, &t_mm_strm_info);

#if 0
        if (pt_cmpb_inst->u_media.t_set_info.eMediaType == IMTK_PB_CTRL_MEDIA_TYPE_MTK_P0)
        {
            MM_AUD_PCM_TYPE_T e_pcm_type;

            e_vid_enc = 
                _get_mm_video_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tVidInfo.eVidEnc);
            
            e_aud_enc
                = _get_mm_audio_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.eAudEnc,
                pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_pcm_info.e_pcm_type,
                &e_pcm_type);

            t_mm_strm_info.t_format.e_media_type = MEDIA_TYPE_CONTAINER;
            t_mm_strm_info.t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_CMPB;
            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_CMPB_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                pt_vid_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.u.t_cmpb_video_stm_attr);
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_VIDEO;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;
                pt_vid_stm_atrbt->e_enc = e_vid_enc;
                pt_vid_stm_atrbt->i4_width = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tVidInfo.tAspRatio.u4DispWidth;
                pt_vid_stm_atrbt->i4_height= pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tVidInfo.tAspRatio.u4DispHeight;
                if (e_vid_enc == MINFO_INFO_VID_ENC_WVC1)
                {
                    pt_vid_stm_atrbt->pu1_buff = c_mem_alloc(IMTK_PB_CTRL_VID_RESERVED_CODEC_INFO_LEN);
                    if(!pt_vid_stm_atrbt->pu1_buff)
                    {
                        return IMTK_PB_ERROR_CODE_NOT_OK;
                    }
                    c_memset(pt_vid_stm_atrbt->pu1_buff, 0, IMTK_PB_CTRL_VID_RESERVED_CODEC_INFO_LEN);
                    c_memcpy(pt_vid_stm_atrbt->pu1_buff,
                             pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tVidInfo.uVidCodecInfo.t_res_info.u1Reserved,
                             IMTK_PB_CTRL_VID_RESERVED_CODEC_INFO_LEN);
                
                    pt_vid_stm_atrbt->ui4_buff_size = IMTK_PB_CTRL_VID_RESERVED_CODEC_INFO_LEN;
                }
                pt_vid_stm_atrbt->ui4_scale = 1000;
                pt_vid_stm_atrbt->ui4_rate = 60000;
#if 0           /*tMtkP0Info.tVidInfo.eFrmRate has been removed*/
                switch (pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tVidInfo.eFrmRate)
                {
                case IMTK_PB_CTRL_FRAME_RATE_23_976:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 23976;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_24:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 24000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_25:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 25000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_29_97:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 29970;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_30:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 30000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_50:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 50000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_59_94:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 59940;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_60:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 60000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_120:
                    pt_vid_stm_atrbt->ui4_scale =  1000;
                    pt_vid_stm_atrbt->ui4_rate = 120000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_1:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate =  1000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_5:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate =  5000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_8:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate =  8000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_10:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 23976;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_12:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 10000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_15:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 15000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_2:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate =  2000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_6:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate =  6000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_48:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 48000;
                    break;
                
                case IMTK_PB_CTRL_FRAME_RATE_70:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 70000;
                    break;
                    
                default:
                    pt_vid_stm_atrbt->ui4_scale = 1000;
                    pt_vid_stm_atrbt->ui4_rate = 24000;
                    break;
                }
#endif
                t_mm_strm_info.ui2_num_stm++;
            }
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_CMPB_AUDIO_STM_ATRBT_T* pt_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.u.t_cmpb_audio_stm_attr);

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_AUDIO;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;
                pt_stm_atrbt->e_enc = e_aud_enc;
                pt_stm_atrbt->ui4_stream_id = 0;

                if (pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.eAudEnc != IMTK_PB_CTRL_AUD_ENC_WMA)
                {                
                    if (pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_MP3)
                    {
                        pt_stm_atrbt->ui1_mpg_layer = 3;
                    }
                    else
                    {
                        pt_stm_atrbt->ui1_mpg_layer = 0;
                    }

                    pt_stm_atrbt->e_enc = e_aud_enc;
                    pt_stm_atrbt->e_pcm_type = e_pcm_type;

                    pt_stm_atrbt->ui2_channels = _cmpb_map_channels(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_pcm_info.eNumCh);
                    pt_stm_atrbt->ui4_sample_rate = _cmpb_map_sample_rate(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_pcm_info.eSampleRate);
                    pt_stm_atrbt->ui2_block_align = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_pcm_info.u2BlockAlign;
                    pt_stm_atrbt->ui2_bits_per_sample = _cmpb_map_bit_depth(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_pcm_info.eBitsPerSample);
                    pt_stm_atrbt->fg_big_endian = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_pcm_info.fgBigEndian;
                    pt_stm_atrbt->ui4_stream_id = 0;
                }
                else
                {
                    pt_stm_atrbt->ui2_channels       = _cmpb_map_channels(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_wma_info.eNumCh);;
                    pt_stm_atrbt->ui4_sample_per_sec = _cmpb_map_sample_rate(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_wma_info.eSampleRate);
                    pt_stm_atrbt->ui4_block_size     = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_wma_info.u2BlockAlign;
                    pt_stm_atrbt->ui2_encoder_opt    = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_wma_info.u2EncoderOpt;
                    pt_stm_atrbt->ui4_bits_per_sec   = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_wma_info.u4AvgBytesPerSec;
                }

                t_mm_strm_info.ui2_num_stm++;
            }
        }
        else if (pt_cmpb_inst->u_media.t_set_info.eMediaType == IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS)
        {
            DVB_SBTL_TYPE_T                 e_sbtl_type;
            
            e_vid_enc = 
                _get_mm_video_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.eVidInfo.eVidEnc);
            
            e_aud_enc
                = _get_mm_audio_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.eAudInfo.eAudEnc, 0, NULL);
            e_sbtl_type = pt_cmpb_inst->at_sub_tracks[0].e_sbtl_type;

            t_mm_strm_info.t_format.e_media_type = MEDIA_TYPE_TS;
            
            if (pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.ePacketType == IMTK_PB_CTRL_MPEG2_TS_PACKET_TYPE_188BYTE)
            {
                t_mm_strm_info.t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_188;
            }
            else if (pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.ePacketType == IMTK_PB_CTRL_MPEG2_TS_PACKET_TYPE_192BYTE)
            {
                t_mm_strm_info.t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192;
            }

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_PS_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_vid_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.u.t_ps_video_stm_attr);
                pt_vid_stm_atrbt->e_enc = e_vid_enc;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id
                    = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.eVidInfo.u2Pid;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = 0;
                
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_VIDEO;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;

                t_mm_strm_info.ui2_num_stm++;
            }
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_PS_AUDIO_STM_ATRBT_T* pt_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.u.t_ps_audio_stm_attr);
                pt_stm_atrbt->e_enc = e_aud_enc;
                if(IMTK_PB_CTRL_AUD_ENC_MP3 == pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.eAudInfo.eAudEnc)
                {
                    pt_stm_atrbt->e_layer = 3;
                }

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id 
                    = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.eAudInfo.u2Pid;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = 0;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_AUDIO;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;
                t_mm_strm_info.ui2_num_stm++;
            }
            if(e_sbtl_type != DVB_SBTL_UNKNOWN)
            {
                MINFO_TYPE_STM_ATRBT_T* pt_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                pt_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt);
                
                CMPB_MEMCPY (&(pt_stm_atrbt->u.t_cmpb_subtl_stm_attr),
                             &(pt_cmpb_inst->at_sub_tracks[0]),
                             sizeof(IMTK_PB_CTRL_SUBTITLE_INFO_T));             
                pt_cmpb_inst->h_gl_plane = pt_cmpb_inst->at_sub_tracks[0].h_gl_plane;
                pt_stm_atrbt->t_stm_id.u.t_ps_stm_id.ui1_stm_id
                        = pt_cmpb_inst->at_sub_tracks[0].ui2_pid;
                pt_stm_atrbt->t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = 0;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_SUBTITLE;   
                pt_stm_atrbt->b_is_codec_support = TRUE;
                t_mm_strm_info.ui2_num_stm++;
            }
        }
        else if (pt_cmpb_inst->u_media.t_set_info.eMediaType == IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_PS)
        {
            e_vid_enc = 
                _get_mm_video_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tPsInfo.eVidInfo.eVidEnc);
            
            e_aud_enc
                = _get_mm_audio_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tPsInfo.eAudInfo.eAudEnc, 0, NULL);

            t_mm_strm_info.t_format.e_media_type = MEDIA_TYPE_PS;
            t_mm_strm_info.t_format.t_media_subtype.u.e_ps_subtype = MEDIA_PS_SUBTYPE_MPG;

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_PS_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_vid_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.u.t_ps_video_stm_attr);
                pt_vid_stm_atrbt->e_enc = e_vid_enc;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id
                    = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tPsInfo.eVidInfo.tVidStrmIdInfo.u1StrmId;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = 0;
                
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_VIDEO;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;

                t_mm_strm_info.ui2_num_stm++;
            }
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_PS_AUDIO_STM_ATRBT_T* pt_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.u.t_ps_audio_stm_attr);
                pt_stm_atrbt->e_enc = e_aud_enc;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id 
                    = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tPsInfo.eAudInfo.tAudStrmIdInfo.u1StrmId;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id 
                    = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tPsInfo.eAudInfo.tAudStrmIdInfo.u1SubStrmIdPriStrm;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_AUDIO;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;
                t_mm_strm_info.ui2_num_stm++;
            }
        }
        else if (pt_cmpb_inst->u_media.t_set_info.eMediaType == IMTK_PB_CTRL_MEDIA_TYPE_AVI)
        {
            e_vid_enc = 
                _get_mm_video_enc_4cc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviVidInfo.au1FourCC);
            
            e_aud_enc
                = _get_mm_audio_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviAudInfo.eAudEnc, 0, NULL);

            t_mm_strm_info.t_format.e_media_type = MEDIA_TYPE_CONTAINER;
            t_mm_strm_info.t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_AVI;

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_AVI_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_vid_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.u.t_avi_video_stm_attr);
                pt_vid_stm_atrbt->e_enc = e_vid_enc;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_avi_stm_id
                    = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviVidInfo.u1StreamIdx;
                
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_VIDEO;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;

                pt_vid_stm_atrbt->i4_width = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviVidInfo.u4Width;
                pt_vid_stm_atrbt->i4_height= pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviVidInfo.u4Height;
                pt_vid_stm_atrbt->ui4_scale = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviVidInfo.u4Scale;
                pt_vid_stm_atrbt->ui4_rate = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviVidInfo.u4Rate;

                t_mm_strm_info.ui2_num_stm++;
            }
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_AVI_AUDIO_STM_ATRBT_T* pt_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.u.t_avi_audio_stm_attr);
                pt_stm_atrbt->e_enc = e_aud_enc;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_avi_stm_id 
                    = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviAudInfo.u1StreamIdx;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_AUDIO;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;

                pt_stm_atrbt->i2_channels = (INT16)_cmpb_map_channels(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviAudInfo.uAudCodecInfo.t_pcm_info.eNumCh);
                pt_stm_atrbt->ui4_rate = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviAudInfo.u4Rate;
                pt_stm_atrbt->i2_block_align = (INT16)pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviAudInfo.uAudCodecInfo.t_pcm_info.u2BlockAlign;
                pt_stm_atrbt->i2_bits_per_sample = (INT16)_cmpb_map_bit_depth(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviAudInfo.uAudCodecInfo.t_pcm_info.eBitsPerSample);
                
                t_mm_strm_info.ui2_num_stm++;
            }
        }
        else if (pt_cmpb_inst->u_media.t_set_info.eMediaType == IMTK_PB_CTRL_MEDIA_TYPE_ASF)
        {
            e_vid_enc = 
                _get_mm_video_enc_4cc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfVidInfo.au1FourCC);
            
            e_aud_enc
                = _get_mm_audio_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc, 0, NULL);

            t_mm_strm_info.t_format.e_media_type = MEDIA_TYPE_CONTAINER;
            t_mm_strm_info.t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_ASF;

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_ASF_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_vid_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.u.t_asf_video_stm_attr);
                pt_vid_stm_atrbt->e_enc = e_vid_enc;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_asf_stm_id
                    = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfVidInfo.u1StreamIdx;
                
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_VIDEO;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;
#if 0
                pt_vid_stm_atrbt->i4_width = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfVidInfo.u4Width;
                pt_vid_stm_atrbt->i4_height= pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfVidInfo.u4Height;
                pt_vid_stm_atrbt->ui4_scale = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfVidInfo.u4Scale;
                pt_vid_stm_atrbt->ui4_rate = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfVidInfo.u4Rate;
#endif
                t_mm_strm_info.ui2_num_stm++;
            }
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_ASF_AUDIO_STM_ATRBT_T* pt_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr);
                pt_stm_atrbt->e_enc = e_aud_enc;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_asf_stm_id 
                    = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfAudInfo.u1StreamIdx;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_AUDIO;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;

                pt_stm_atrbt->i2_channels = (INT16)_cmpb_map_channels(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_pcm_info.eNumCh);
                //pt_stm_atrbt->ui4_rate = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfAudInfo.u4Rate;
                pt_stm_atrbt->i2_block_align = (INT16)pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_pcm_info.u2BlockAlign;
                pt_stm_atrbt->i2_bits_per_sample = (INT16)_cmpb_map_bit_depth(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_pcm_info.eBitsPerSample);
                
                t_mm_strm_info.ui2_num_stm++;
            }
        }
        else if (pt_cmpb_inst->u_media.t_set_info.eMediaType == IMTK_PB_CTRL_MEDIA_TYPE_MKV)
        {
            e_vid_enc = 
                _get_mm_video_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvVidInfo.uVidCodec.eVidEnc);
            
            e_aud_enc
                = _get_mm_audio_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvAudInfo.eAudEnc, 0, NULL);

            t_mm_strm_info.t_format.e_media_type = MEDIA_TYPE_CONTAINER;
            t_mm_strm_info.t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MATROSKA;

            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_MKV_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_vid_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.u.t_mkv_video_stm_attr);
                pt_vid_stm_atrbt->e_enc = e_vid_enc;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_mkv_stm_id = 0;
                    //= pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvVidInfo.u1StreamIdx;
                
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_VIDEO;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;
#if 0
                pt_vid_stm_atrbt->i4_width = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvVidInfo.u4Width;
                pt_vid_stm_atrbt->i4_height= pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvVidInfo.u4Height;
                pt_vid_stm_atrbt->ui4_scale = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvVidInfo.u4Scale;
                pt_vid_stm_atrbt->ui4_rate = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvVidInfo.u4Rate;
#endif
                t_mm_strm_info.ui2_num_stm++;
            }
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_MKV_AUDIO_STM_ATRBT_T* pt_stm_atrbt = NULL;

                if(t_mm_strm_info.ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.u.t_mkv_audio_stm_attr);
                pt_stm_atrbt->e_enc = e_aud_enc;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_mkv_stm_id = 0;
                   // = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvAudInfo.u1StreamIdx;

                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].e_type = ST_AUDIO;
                t_mm_strm_info.at_stm_atrbt[t_mm_strm_info.ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;

                pt_stm_atrbt->i2_channels = (INT16)_cmpb_map_channels(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvAudInfo.uAudCodecInfo.t_pcm_info.eNumCh);
                pt_stm_atrbt->ui4_rate = 0;//pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvAudInfo.u4Rate;
                pt_stm_atrbt->i2_block_align = (INT16)pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvAudInfo.uAudCodecInfo.t_pcm_info.u2BlockAlign;
                pt_stm_atrbt->i2_bits_per_sample = (INT16)_cmpb_map_bit_depth(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvAudInfo.uAudCodecInfo.t_pcm_info.eBitsPerSample);
                
                t_mm_strm_info.ui2_num_stm++;
            }
        }
        else
        {
            CMPB_DBG_ERROR(("!!!!!! _push_mode_open_media not support container %d\n", 
                pt_cmpb_inst->u_media.t_set_info.eMediaType));
        }

#if 0
        switch(pt_cmpb_inst->u_media.t_set_info.eMediaType)
        {
        
        case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS:
            t_vid_attr.e_enc = 
                _get_mm_video_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.eVidInfo.eVidEnc);
            t_vid_attr.ui1_sample_id
                = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.eVidInfo.u2Pid;            

            t_aud_attr.e_enc
                = _get_mm_audio_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.eAudInfo.eAudEnc, 0, NULL);
            t_aud_attr.ui4_stream_id
                = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.eAudInfo.u2Pid;
                    break;
                    
        case IMTK_PB_CTRL_MEDIA_TYPE_VIDEO_ES:
            t_vid_attr.e_enc = 
                _get_mm_video_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tVideoEsInfo.eVidEnc);
                    break;
                    
        case IMTK_PB_CTRL_MEDIA_TYPE_AUDIO_ES:
            t_aud_attr.e_enc
                = _get_mm_audio_enc(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc,
                pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.e_pcm_type,
                &t_aud_attr.e_pcm_type);

            t_aud_attr.ui2_channels
                = _cmpb_map_channels(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eNumCh);
            t_aud_attr.ui4_sample_per_sec
                = _cmpb_map_sample_rate(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eSampleRate);
            t_aud_attr.ui2_block_align
                = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.u2BlockAlign;
            t_aud_attr.ui2_bits_per_sample
                = _cmpb_map_bit_depth(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eBitsPerSample);
            t_aud_attr.fg_big_endian 
                = pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.fgBigEndian;
                    break;       
                default:
                    break;
                }
#endif
#endif
                
    /*------------------start open media ------------------------*/
        at_cmd[0].e_code = MM_SVC_CMD_CODE_SRC_TYPE;
        at_cmd[0].u.ui1_data = SRC_TYPE_MEDIA_NETWORK;

        at_cmd[1].e_code = MM_SVC_CMD_CODE_SET_PROTOCL;
        at_cmd[1].u.ui4_data = PROTOCL_MODE_PUSH;

        at_cmd[2].e_code = MM_SVC_CMD_CODE_SET_META_INFO;
        at_cmd[2].u.pv_data = NULL; //&t_meta_data;

        at_cmd[3].e_code = MM_SVC_CMD_CODE_SET_STRM_INFO;
        at_cmd[3].u.pv_data = &t_mm_strm_info;
        
        at_cmd[4].e_code = MM_SVC_CMD_CODE_SET_PUSH_MODE_INFO;
        at_cmd[4].u.pv_data = &t_push_info;
        
        at_cmd[5].e_code = MM_SVC_CMD_CODE_SET_SRC_SUB_TYPE;
        at_cmd[5].u.ui1_data = pt_cmpb_inst->t_src_sub_type;

        at_cmd[6].e_code = MM_SVC_CMD_CODE_END;

        pt_cmpb_inst->e_old_state = pt_cmpb_inst->e_cur_state;
        pt_cmpb_inst->e_cur_state = CMPB_CTRL_OPENED;
        pt_cmpb_inst->e_new_state = CMPB_CTRL_OPENED;
        
        if (pt_cmpb_inst->h_media_hdl)
        {
            CMPB_SVCTX_MEDIA_CLOSE(pt_cmpb_inst->h_svctx_hdl, pt_cmpb_inst->h_media_hdl);
            pt_cmpb_inst->h_media_hdl = NULL_HANDLE;
        }
        i4_ret = CMPB_SVCTX_MEDIA_OPEN(
                        pt_cmpb_inst->h_svctx_hdl,
                        at_cmd,
                        _cmpb_ctrl_media_open_nfy_fct,
                        (VOID*)pt_cmpb_inst,
                        &(pt_cmpb_inst->h_media_hdl)
                        );
        if(SVCTXR_OK != i4_ret)
        {
            pt_cmpb_inst->h_media_hdl = NULL_HANDLE;
            CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_OPEN() returns %d\n", i4_ret));
            break;
        }

        //pt_cmpb_inst->h_media_hdl = h_media_hdl;
               
    }while(0);

    /*Free memory*/
    if (pt_cmpb_inst->u_media.t_set_info.eMediaType == IMTK_PB_CTRL_MEDIA_TYPE_MTK_P0)
    {
        MINFO_TYPE_STM_ATRBT_T*             pt_stm_atrbt = NULL;
        UINT8                               u1i          = 0;
        
        for (u1i=0; u1i<SUPPORTED_STRM_NUM_MAX;u1i++)
        {
            if (t_mm_strm_info.at_stm_atrbt[u1i].e_type == ST_VIDEO)
            {
                pt_stm_atrbt = &(t_mm_strm_info.at_stm_atrbt[u1i].t_stm_atrbt);
                break;
            }
        }
        if (pt_stm_atrbt && pt_stm_atrbt->u.t_cmpb_video_stm_attr.pu1_buff)
        {
            CMPB_MEM_FREE(pt_stm_atrbt->u.t_cmpb_video_stm_attr.pu1_buff);
            pt_stm_atrbt->u.t_cmpb_video_stm_attr.pu1_buff = NULL;
        }
    }

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);    
    if(OSR_OK != i4_ret)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    } 

    pt_cmpb_inst->e_cur_state = CMPB_CTRL_READY;
    
    return IMTK_PB_ERROR_CODE_OK;
}

static IMTK_PB_ERROR_CODE_T _Cmpb_Ctrl_Cvt_MediaInfo(
                                    IMTK_PB_CTRL_SET_MEDIA_INFO_T* pt_cmpb_info,          
                                    MM_STRM_INFO_T*                pt_strm_info)
{
    if(pt_cmpb_info == NULL || pt_strm_info == NULL)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    CMPB_MEMSET(pt_strm_info, 0, sizeof(MM_STRM_INFO_T));

    switch(pt_cmpb_info->eMediaType)
    {
    case IMTK_PB_CTRL_MEDIA_TYPE_AVI:
        pt_strm_info->t_format.e_media_type    = MEDIA_TYPE_CONTAINER;
        pt_strm_info->t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_AVI;
        break;
    case IMTK_PB_CTRL_MEDIA_TYPE_FLV:
        pt_strm_info->t_format.e_media_type    = MEDIA_TYPE_CONTAINER;
        pt_strm_info->t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_FLV;
        break;
    case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_PS:
        pt_strm_info->t_format.e_media_type    = MEDIA_TYPE_PS;
        pt_strm_info->t_format.t_media_subtype.u.e_ps_subtype = MEDIA_PS_SUBTYPE_MPG;
        break;

    case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS_TIMESHIFT:
    case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS:
        {
            MINFO_INFO_VIDEO_ENC_T          e_vid_enc;
            MINFO_INFO_AUDIO_ENC_T          e_aud_enc;

            e_vid_enc = 
                _get_mm_video_enc(pt_cmpb_info->uFormatInfo.tTsInfo.eVidInfo.eVidEnc);
            
            e_aud_enc
                = _get_mm_audio_enc(pt_cmpb_info->uFormatInfo.tTsInfo.eAudInfo.eAudEnc, 0, NULL);
    
            pt_strm_info->t_format.e_media_type = MEDIA_TYPE_TS;
            
            if (IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS_TIMESHIFT == pt_cmpb_info->eMediaType)
            {
                pt_strm_info->t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_TIME_SHIFT_CMPB;
            }
            else if (pt_cmpb_info->uFormatInfo.tTsInfo.ePacketType == IMTK_PB_CTRL_MPEG2_TS_PACKET_TYPE_188BYTE)
            {
                pt_strm_info->t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_188;
            }
            else if (pt_cmpb_info->uFormatInfo.tTsInfo.ePacketType == IMTK_PB_CTRL_MPEG2_TS_PACKET_TYPE_192BYTE)
            {
                pt_strm_info->t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192;
            }
    
            if(e_vid_enc != MINFO_INFO_VID_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_PS_VIDEO_STM_ATRBT_T* pt_vid_stm_atrbt = NULL;
    
                if(pt_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_vid_stm_atrbt = &(pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_ps_video_stm_attr);
                pt_vid_stm_atrbt->e_enc = e_vid_enc;
    
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id
                    = pt_cmpb_info->uFormatInfo.tTsInfo.eVidInfo.u2Pid;
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = 0;
                
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].e_type = ST_VIDEO;
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;
    
                pt_strm_info->ui2_num_stm++;
            }
            if(e_aud_enc != MINFO_INFO_AUD_ENC_UNKNOWN)/*have assigned before*/
            {
                MINFO_PS_AUDIO_STM_ATRBT_T* pt_stm_atrbt = NULL;
    
                if(pt_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                
                pt_stm_atrbt = &(pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_ps_audio_stm_attr);
                pt_stm_atrbt->e_enc = e_aud_enc;
                if(IMTK_PB_CTRL_AUD_ENC_MP3 == pt_cmpb_info->uFormatInfo.tTsInfo.eAudInfo.eAudEnc)
                {
                    pt_stm_atrbt->e_layer = 3;
                }
    
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id 
                    = pt_cmpb_info->uFormatInfo.tTsInfo.eAudInfo.u2Pid;
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = 0;
    
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].e_type = ST_AUDIO;
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.b_is_codec_support = TRUE;
                pt_strm_info->ui2_num_stm++;
            }
        }
        break;
    case IMTK_PB_CTRL_MEDIA_TYPE_ASF:
        pt_strm_info->t_format.e_media_type    = MEDIA_TYPE_CONTAINER;
        pt_strm_info->t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_ASF;
        if(_get_mm_video_enc_4cc(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfVidInfo.au1FourCC) != MINFO_INFO_VID_ENC_UNKNOWN)
        {
            if(pt_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
            {
                break;
            }
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].e_type = ST_VIDEO;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_asf_stm_id = 
                pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfVidInfo.u1StreamIdx;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.b_is_codec_support = 1;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_video_stm_attr.e_enc = 
                _get_mm_video_enc_4cc(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfVidInfo.au1FourCC);
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_video_stm_attr.ui4_codec_info_size
                = pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfVidInfo.u2CodecPrivInfoLen;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_video_stm_attr.pui1_codec_info
                = (UINT8*)pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfVidInfo.au1CodecPrivInfo;

            pt_strm_info->ui2_num_stm ++;
        }
        if(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc != IMTK_PB_CTRL_AUD_ENC_UNKNOWN)
        {
            if(pt_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
            {
                break;
            }
            
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].e_type = ST_AUDIO;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_asf_stm_id = 
                pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.u1StreamIdx;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.b_is_codec_support = 1;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.e_enc = 
                _get_mm_audio_enc(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc, 0, NULL);
            if(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_MP3)
            {
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.e_layer = 3;
            }
            else if(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_PCM)
            {
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_bits_per_sample =
                    (INT16)_cmpb_map_bit_depth(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_pcm_info.eBitsPerSample);
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_channels =
                    (INT16)_cmpb_map_channels(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_pcm_info.eNumCh);
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i4_samples_per_sec =
                    (INT32)_cmpb_map_sample_rate(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_pcm_info.eSampleRate);
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_block_align =
                    (INT16)pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_pcm_info.u2BlockAlign;
            }
            else if(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_WMA)
            {
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.ui4_avg_bytes_per_sec = 
                    (UINT32)pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_wma_info.u4AvgBytesPerSec;
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_bits_per_sample =
                    (INT16)(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_wma_info.u4AvgBytesPerSec * 8 /
                            _cmpb_map_sample_rate(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_wma_info.eSampleRate));
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_channels =
                    (INT16)_cmpb_map_channels(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_wma_info.eNumCh);
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i4_samples_per_sec =
                    (INT32)_cmpb_map_sample_rate(pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_wma_info.eSampleRate);
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_block_align =
                    (INT16)pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_wma_info.u2BlockAlign;
            }
            pt_strm_info->ui2_num_stm ++;
        }
        break;
    case IMTK_PB_CTRL_MEDIA_TYPE_MP4:
        pt_strm_info->t_format.e_media_type    = MEDIA_TYPE_CONTAINER;
        pt_strm_info->t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MP4;
        break;
    case IMTK_PB_CTRL_MEDIA_TYPE_MKV:
        pt_strm_info->t_format.e_media_type    = MEDIA_TYPE_CONTAINER;
        pt_strm_info->t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MATROSKA;
        break;
    case IMTK_PB_CTRL_MEDIA_TYPE_VIDEO_ES:
        pt_strm_info->t_format.e_media_type    = MEDIA_TYPE_VIDEO_ES;
        pt_strm_info->t_format.t_media_subtype.u.e_vid_subtype = MEDIA_VID_SUBTYPE_M1V;
        
        if(pt_cmpb_info->uFormatInfo.tVideoEsInfo.eVidEnc != IMTK_PB_CTRL_VID_ENC_UNKNOWN)
        {
            if(pt_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
            {
                break;
            }            
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].e_type = ST_VIDEO;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.b_is_codec_support = 1;
        }
        pt_strm_info->ui2_num_stm ++;
        break;
    case IMTK_PB_CTRL_MEDIA_TYPE_AUDIO_ES:
        pt_strm_info->t_format.e_media_type    = MEDIA_TYPE_AUDIO_ES;        
        pt_strm_info->t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_UNKNOWN;
        
        if(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc != IMTK_PB_CTRL_AUD_ENC_UNKNOWN)
        {
            if(pt_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
            {
                break;
            }
            
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].e_type = ST_AUDIO;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.t_stm_id.u.t_asf_stm_id = 
                pt_cmpb_info->uFormatInfo.tAsfInfo.tAsfAudInfo.u1StreamIdx;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.b_is_codec_support = 1;
            
            if(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_MP3)
            {
                pt_strm_info->t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP3;
                
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_mp3_audio_stm_attr.e_enc = 
                    _get_mm_audio_enc(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc, 0, NULL);
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_mp3_audio_stm_attr.e_layer = 3;
            }
            else if(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_AAC)
            {
                pt_strm_info->t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AAC;
                
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc = 
                    _get_mm_audio_enc(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc, 0, NULL);
            }
            else if(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_MPEG)
            {
                pt_strm_info->t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP2;
                
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc = 
                    _get_mm_audio_enc(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc, 0, NULL);
            }
            else if(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_PCM)
            {
                pt_strm_info->t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_LPCM;

                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_bits_per_sample =
                    (INT16)_cmpb_map_bit_depth(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eBitsPerSample);
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_channels =
                    (INT16)_cmpb_map_channels(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eNumCh);
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i4_samples_per_sec =
                    (INT32)_cmpb_map_sample_rate(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eSampleRate);
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_block_align =
                    (INT16)pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.u2BlockAlign;
            }
            else if(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_WMA)
            {
                pt_strm_info->t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_WMA;
                
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.ui4_avg_bytes_per_sec = 
                    (UINT32)pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_wma_info.u4AvgBytesPerSec;
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_bits_per_sample =
                    (INT16)(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_wma_info.u4AvgBytesPerSec * 8 /
                            _cmpb_map_sample_rate(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_wma_info.eSampleRate));
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_channels =
                    (INT16)_cmpb_map_channels(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_wma_info.eNumCh);
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i4_samples_per_sec =
                    (INT32)_cmpb_map_sample_rate(pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_wma_info.eSampleRate);
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_block_align =
                    (INT16)pt_cmpb_info->uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_wma_info.u2BlockAlign;               
            }
            pt_strm_info->ui2_num_stm ++;
        }
        break;
    case IMTK_PB_CTRL_MEDIA_TYPE_MTK_P0:
        pt_strm_info->t_format.e_media_type    = MEDIA_TYPE_CONTAINER;
        pt_strm_info->t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_CMPB;
        break;
        
    default:
        return IMTK_PB_ERROR_CODE_OK;
    } 

    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetMediaInfo(IMTK_PB_HANDLE_T                   hHandle,          
                                              IMTK_PB_CTRL_SET_MEDIA_INFO_T*     ptMediaInfo)
{
    CMPB_CTRL_INST_T* pt_cmpb_inst = NULL;
    MM_PB_CTRL_PULL_INFO_T  t_pull = {0};
    INT32                   i4_ret = 0;
    MM_STRM_INFO_T          t_strm_info;
    UINT8                   ui1_i;
    MM_STRM_CODEC_INFO_T    t_strm_codec_info;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);


    if (pt_cmpb_inst != NULL)
    {
        if (!pt_cmpb_inst->b_app_set_media_info)
        {
            return IMTK_PB_ERROR_CODE_INV_ARG;
        }
        if(ptMediaInfo != NULL)
        {
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info),
                     ptMediaInfo, 
                     sizeof(IMTK_PB_CTRL_SET_MEDIA_INFO_T));
            if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PULL)
            {
                switch(pt_cmpb_inst->u_media.t_set_info.eMediaType)
                {
                case IMTK_PB_CTRL_MEDIA_TYPE_ASF:
                    t_pull.ui8_preroll         = 
                        pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.u8PrerollTime;
                    t_pull.ui4_max_acces_unt   = 
                        pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.u4DataPacketSize;
                    t_pull.ui4_min_acces_unt   = 
                        pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.u4DataPacketSize;
                    t_pull.ui8_media_data_ofst = 0;
                    t_pull.ui8_media_duration  = 
                        pt_cmpb_inst->u_media.t_set_info.u4TotalDuration*90;
                    t_pull.ui8_media_data_size = 
                        pt_cmpb_inst->u_media.t_set_info.u8Size;
                    break;
                
                default:
                    t_pull.ui8_preroll         = 0;
                    t_pull.ui4_max_acces_unt   = 0;
                    t_pull.ui4_min_acces_unt   = 0;
                    t_pull.ui8_media_data_ofst = 0;

                    t_pull.ui8_media_duration  = 
                        pt_cmpb_inst->u_media.t_set_info.u4TotalDuration*90;
                    t_pull.ui8_media_data_size = 
                        pt_cmpb_inst->u_media.t_set_info.u8Size;
                    break;
                }

                i4_ret = CMPB_SVCTX_MEDIA_SET_INFO(
                                pt_cmpb_inst->h_svctx_hdl,
                                pt_cmpb_inst->h_media_hdl,
                                SVCTX_MM_SET_TYPE_PULL_INFO,
                                (VOID*)&t_pull,
                                sizeof(MM_PB_CTRL_PULL_INFO_T)
                                );
                                
                if(SVCTXR_OK != i4_ret)
                {
                    CMPB_DBG_ERROR(("CMPB_SVCTX_SET() returns %d\n", i4_ret));
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }

                /*set stream info*/
                _Cmpb_Ctrl_Cvt_MediaInfo(ptMediaInfo, &t_strm_info);
                //t_strm_info = g_t_strm_info;
                i4_ret = CMPB_SVCTX_MEDIA_SET_INFO(
                                pt_cmpb_inst->h_svctx_hdl,
                                pt_cmpb_inst->h_media_hdl,
                                SVCTX_MM_SET_TYPE_STRM_INFO,
                                (VOID*)&t_strm_info,
                                sizeof(MM_STRM_INFO_T)
                                );
                                
                if(SVCTXR_OK != i4_ret)
                {
                    CMPB_DBG_ERROR(("CMPB_SVCTX_SET() returns %d\n", i4_ret));
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }

                for (ui1_i=0; ui1_i<t_strm_info.ui2_num_stm; ui1_i++)
                {
                    if(ui1_i >= SUPPORTED_STRM_NUM_MAX)
                    {
                        break;
                    }
                    if (t_strm_info.t_format.e_media_type == MEDIA_TYPE_CONTAINER)
                    {
                        if (t_strm_info.t_format.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
                        {
                            t_strm_codec_info.ui1_strm_idx = ui1_i;
                            if (t_strm_info.at_stm_atrbt[ui1_i].e_type == ST_AUDIO)
                            {
                                if (IMTK_PB_CTRL_AUD_ENC_WMA == ptMediaInfo->uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc)
                                {
                                    UINT16* pui2_codec_optn;
                                    t_strm_codec_info.ui4_codec_info_size = 6;
                                    pui2_codec_optn = (UINT16*)t_strm_codec_info.aui1_codec_info;
                                    pui2_codec_optn[2] = ptMediaInfo->uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_wma_info.u2EncoderOpt;
                                }
                                else
                                {
                                    t_strm_codec_info.ui4_codec_info_size 
                                        = t_strm_info.at_stm_atrbt[ui1_i].t_stm_atrbt.u.t_asf_audio_stm_attr.ui4_codec_info_size;
                                    CMPB_MEMCPY(t_strm_codec_info.aui1_codec_info,
                                             t_strm_info.at_stm_atrbt[ui1_i].t_stm_atrbt.u.t_asf_audio_stm_attr.pui1_codec_info,
                                             t_strm_info.at_stm_atrbt[ui1_i].t_stm_atrbt.u.t_asf_audio_stm_attr.ui4_codec_info_size);
                                }
                            }
                            else if (t_strm_info.at_stm_atrbt[ui1_i].e_type == ST_VIDEO)
                            {
                                t_strm_codec_info.ui4_codec_info_size 
                                    = t_strm_info.at_stm_atrbt[ui1_i].t_stm_atrbt.u.t_asf_video_stm_attr.ui4_codec_info_size;
                                CMPB_MEMCPY(t_strm_codec_info.aui1_codec_info,
                                         t_strm_info.at_stm_atrbt[ui1_i].t_stm_atrbt.u.t_asf_video_stm_attr.pui1_codec_info,
                                         t_strm_info.at_stm_atrbt[ui1_i].t_stm_atrbt.u.t_asf_video_stm_attr.ui4_codec_info_size);
                            }
                            i4_ret = CMPB_SVCTX_MEDIA_SET_INFO(
                                            pt_cmpb_inst->h_svctx_hdl,
                                            pt_cmpb_inst->h_media_hdl,
                                            SVCTX_MM_SET_TYPE_STRM_CODEC_INFO,
                                            (VOID*)&t_strm_codec_info,
                                            sizeof(MM_STRM_CODEC_INFO_T)
                                            );
                                            
                            if(SVCTXR_OK != i4_ret)
                            {
                                CMPB_DBG_ERROR(("CMPB_SVCTX_SET() returns %d\n", i4_ret));
                                return IMTK_PB_ERROR_CODE_NOT_OK;
                            }
                        }
                    }
                }
            }
            
            pt_cmpb_inst->b_media_info_is_set = TRUE;
        }
        
        return IMTK_PB_ERROR_CODE_OK;
    }
    
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_ChangeAudio(IMTK_PB_HANDLE_T         hHandle, 
                                                   IMTK_PB_CTRL_AUD_INFO_T* ptAudInfo)
{
    CMPB_CTRL_INST_T*       pt_cmpb_inst;
    INT32                   i4_ret;
    IMTK_PB_ERROR_CODE_T    e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    MM_STRM_ATRBT_T         t_mm_stm_atrbt;
    MINFO_INFO_AUDIO_ENC_T  e_aud_enc;

    if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_AUDIO_CHG))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL== pt_cmpb_inst)
    {
        return e_ret_code;
    }
    
    do
    {        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        switch(pt_cmpb_inst->u_media.t_set_info.eMediaType)
        {
        case IMTK_PB_CTRL_MEDIA_TYPE_AVI:
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviAudInfo),
                 ptAudInfo,
                 sizeof(IMTK_PB_CTRL_AVI_AUD_INFO_T)
                 );
            break;
            
        case IMTK_PB_CTRL_MEDIA_TYPE_FLV:
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tFlvInfo.tFlvAudInfo),
                 ptAudInfo,
                 sizeof(IMTK_PB_CTRL_FLV_AUD_INFO_T)
                 );
            break;

        case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_PS:
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tPsInfo.eAudInfo),
                 ptAudInfo,
                 sizeof(IMTK_PB_CTRL_MPEG2_PS_AUD_INFO_T)
                 );
            break;
        case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS:
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.eAudInfo),
                 ptAudInfo,
                 sizeof(IMTK_PB_CTRL_MPEG2_TS_AUD_INFO_T)
                 );

            e_aud_enc = 
                _get_mm_audio_enc(ptAudInfo->tMpgTsAudInfo.eAudEnc, 0, NULL);
            
            t_mm_stm_atrbt.t_stm_atrbt.u.t_ps_audio_stm_attr.e_enc = e_aud_enc;

            t_mm_stm_atrbt.t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id
                = ptAudInfo->tMpgTsAudInfo.u2Pid;
            t_mm_stm_atrbt.t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = 0;
            
            t_mm_stm_atrbt.e_type = ST_AUDIO;
            t_mm_stm_atrbt.t_stm_atrbt.b_is_codec_support = TRUE;

            break;
        case IMTK_PB_CTRL_MEDIA_TYPE_ASF:
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfAudInfo),
                 ptAudInfo,
                 sizeof(IMTK_PB_CTRL_ASF_AUD_INFO_T)
                 );
            break;

        case IMTK_PB_CTRL_MEDIA_TYPE_MKV:
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvAudInfo),
                 ptAudInfo,
                 sizeof(IMTK_PB_CTRL_MKV_AUD_INFO_T)
                 );
            break;
        case IMTK_PB_CTRL_MEDIA_TYPE_MP4:
        case IMTK_PB_CTRL_MEDIA_TYPE_MTK_P0:
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMtkP0Info.tAudInfo),
                 ptAudInfo,
                 sizeof(IMTK_PB_CTRL_BASIC_AUDIO_INFO_T)
                 );
            break;
        default:
            break;
        }

        e_ret_code = IMTK_PB_ERROR_CODE_OK; 
    } while(0);

    pt_cmpb_inst->b_audio_changing = TRUE;
    CMPB_SVCTX_SET(
        pt_cmpb_inst->h_svctx_hdl,
        SVCTX_MM_SET_TYPE_CHANGE_STRM,
        (VOID*)&t_mm_stm_atrbt,
        sizeof(MM_STRM_ATRBT_T)
        );

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
    }
    
    return e_ret_code;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_ChangeVideo(IMTK_PB_HANDLE_T         hHandle, 
                                             IMTK_PB_CTRL_VID_INFO_T* ptVidInfo)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    MM_STRM_ATRBT_T             t_mm_stm_atrbt;
    MINFO_INFO_VIDEO_ENC_T      e_vid_enc;

    if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_VIDEO_CHG))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL== pt_cmpb_inst)
    {
        return e_ret_code;
    }
    
    do
    {        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        switch(pt_cmpb_inst->u_media.t_set_info.eMediaType)
        {
        case IMTK_PB_CTRL_MEDIA_TYPE_AVI:
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAviInfo.tAviVidInfo),
                 ptVidInfo,
                 sizeof(IMTK_PB_CTRL_AVI_VID_INFO_T)
                 );
            break;
            
        case IMTK_PB_CTRL_MEDIA_TYPE_FLV:
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tFlvInfo.tFlvVidInfo),
                 ptVidInfo,
                 sizeof(IMTK_PB_CTRL_FLV_VID_INFO_T)
                 );
            break;
            
        case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_PS:
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tPsInfo.eVidInfo),
                 ptVidInfo,
                 sizeof(IMTK_PB_CTRL_MPEG2_PS_VID_INFO_T)
                 );
            break;
        case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS:
            
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tTsInfo.eVidInfo),
                 ptVidInfo,
                 sizeof(IMTK_PB_CTRL_MPEG2_TS_VID_INFO_T)
                 );

            e_vid_enc = 
                _get_mm_video_enc(ptVidInfo->tMpgTsVidInfo.eVidEnc);
            
            t_mm_stm_atrbt.t_stm_atrbt.u.t_ps_video_stm_attr.e_enc = e_vid_enc;

            t_mm_stm_atrbt.t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id
                = ptVidInfo->tMpgTsVidInfo.u2Pid;
            t_mm_stm_atrbt.t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = 0;
            
            t_mm_stm_atrbt.e_type = ST_VIDEO;
            t_mm_stm_atrbt.t_stm_atrbt.b_is_codec_support = TRUE;

            break;
        case IMTK_PB_CTRL_MEDIA_TYPE_ASF:
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tAsfInfo.tAsfVidInfo),
                 ptVidInfo,
                 sizeof(IMTK_PB_CTRL_ASF_VID_INFO_T)
                 );
            break;

        case IMTK_PB_CTRL_MEDIA_TYPE_MKV:
            CMPB_MEMCPY(&(pt_cmpb_inst->u_media.t_set_info.uFormatInfo.tMkvInfo.tMkvVidInfo),
                 ptVidInfo,
                 sizeof(IMTK_PB_CTRL_MKV_VID_INFO_T)
                 );
            break;
        default:
            break;
        }

        e_ret_code = IMTK_PB_ERROR_CODE_OK; 
    } while(0);
    
    pt_cmpb_inst->b_video_changing = TRUE;

    CMPB_SVCTX_SET(
        pt_cmpb_inst->h_svctx_hdl,
        SVCTX_MM_SET_TYPE_CHANGE_STRM,
        (VOID*)&t_mm_stm_atrbt,
        sizeof(MM_STRM_ATRBT_T)
        );

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
    }
    
    return e_ret_code;
}

/* Used before play*/
IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetSubtitleInfo(
                                            IMTK_PB_HANDLE_T                hHandle, 
                                            IMTK_PB_CTRL_SUBTITLE_INFO_T*   ptSubInfo)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL== pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    if(ptSubInfo->e_sbtl_type != DVB_SBTL_UNKNOWN)
    {
        CMPB_MEMCPY (&(pt_cmpb_inst->at_sub_tracks[0]),
                     ptSubInfo,
                     sizeof(IMTK_PB_CTRL_SUBTITLE_INFO_T));
        pt_cmpb_inst->b_sbtl_track_is_set = TRUE;
    }
    return IMTK_PB_ERROR_CODE_OK;
}

/* Used after play*/
IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_ChangeSubtitle(IMTK_PB_HANDLE_T            hHandle, 
                                             IMTK_PB_CTRL_SUBTITLE_INFO_T*  ptSubInfo)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    MM_STRM_ATRBT_T             t_mm_stm_atrbt;

    if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_SUBTT_CHG))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL== pt_cmpb_inst)
    {
        return e_ret_code;
    }
    
    do
    {        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        switch(pt_cmpb_inst->u_media.t_set_info.eMediaType)
        {
        case IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS:
            
            CMPB_MEMCPY(&(pt_cmpb_inst->at_sub_tracks[0]),
                 ptSubInfo,
                 sizeof(IMTK_PB_CTRL_SUBTITLE_INFO_T));

            CMPB_MEMCPY (&(t_mm_stm_atrbt.t_stm_atrbt.u.t_cmpb_subtl_stm_attr),
                         ptSubInfo,
                         sizeof(IMTK_PB_CTRL_SUBTITLE_INFO_T));                
            t_mm_stm_atrbt.t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id
                    = ptSubInfo->ui2_pid;
            t_mm_stm_atrbt.t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = 0;
            
            t_mm_stm_atrbt.e_type = ST_SUBTITLE;
            t_mm_stm_atrbt.t_stm_atrbt.b_is_codec_support = TRUE;
            break;
        default:
            break;
        }

        e_ret_code = IMTK_PB_ERROR_CODE_OK; 
    } while(0);
    
    pt_cmpb_inst->b_subtl_changing = TRUE;

    CMPB_SVCTX_SET(
        pt_cmpb_inst->h_svctx_hdl,
        SVCTX_MM_SET_TYPE_CHANGE_STRM,
        (VOID*)&t_mm_stm_atrbt,
        sizeof(MM_STRM_ATRBT_T)
        );

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
    }
    
    return e_ret_code;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetTimeshiftBufRange(IMTK_PB_HANDLE_T                       hHandle,
                                                      IMTK_PB_CTRL_REC_BUF_RANGE_INFO_T*     ptRangeInfo)
{
    INT32                   i4_ret;
    CMPB_CTRL_INST_T*       pt_cmpb_inst;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL== pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    pt_cmpb_inst->ui4_end_tick = ptRangeInfo->t_safe.u4End;	
    pt_cmpb_inst->ui4_start_tick = ptRangeInfo->t_safe.u4Start;

    i4_ret = CMPB_SVCTX_SET_SESSION_ATTR(
                    pt_cmpb_inst->h_svctx_hdl,
                    ST_UNKNOWN,
                    SM_SESS_GNRC_SET_TYPE_REC_BUF_RANGE,
                    (VOID*)ptRangeInfo,
                    sizeof(IMTK_PB_CTRL_REC_BUF_RANGE_INFO_T));

    if (i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    return IMTK_PB_ERROR_CODE_OK;
}

extern IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetTimeshiftCtrlBlk(IMTK_PB_HANDLE_T                hHandle,
                                                            IMTK_PB_TICK_CTRL_BLK_T*        ptCtrlBlk)
{
    INT32                   i4_ret;
    CMPB_CTRL_INST_T*       pt_cmpb_inst;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL== pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    i4_ret = CMPB_SVCTX_MEDIA_SET_INFO(
                    pt_cmpb_inst->h_svctx_hdl,
                    pt_cmpb_inst->h_media_hdl,
                    SVCTX_MM_SET_TYPE_TICK_CTRL_BLK,
                    (VOID*)ptCtrlBlk,
                    sizeof(IMTK_PB_TICK_CTRL_BLK_T));

    if (i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    return IMTK_PB_ERROR_CODE_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetSpeed(IMTK_PB_HANDLE_T        hHandle,
                                              IMTK_PB_CTRL_SPEED_T    eSpeed)
{
    CMPB_CTRL_INST_T*       pt_cmpb_inst;
    INT32                   i4_ret;
    IMTK_PB_ERROR_CODE_T    e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;

     if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_TRICK))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL== pt_cmpb_inst)
    {
        return e_ret_code;
    }

    if(pt_cmpb_inst->e_cur_speed == eSpeed)
    {
        return IMTK_PB_ERROR_CODE_OK;
    }
   
    do
    {        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        if(_cmpb_ctrl_set_speed(pt_cmpb_inst, eSpeed))
        {
            if (pt_cmpb_inst->e_cur_state ==CMPB_CTRL_PAUSED)
            {
                pt_cmpb_inst->e_cur_state = CMPB_CTRL_PLAYING;
            }            
            pt_cmpb_inst->e_cur_speed = eSpeed;
            if(pt_cmpb_inst->fg_eos && 
                pt_cmpb_inst->e_cur_speed < IMTK_PB_CTRL_SPEED_ZERO)
            {
                pt_cmpb_inst->fg_eos = FALSE;
            }
            e_ret_code = IMTK_PB_ERROR_CODE_OK;
        }
        
    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
    }
    
    return e_ret_code;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetStrmNum(IMTK_PB_HANDLE_T            hHandle,
                                                  IMTK_PB_CTRL_STRM_NUM_T*    ptStrmNum)
{
    CMPB_CTRL_INST_T* pt_cmpb_inst;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (pt_cmpb_inst && ptStrmNum)
    {
        ptStrmNum->u1_audio_strm_num = pt_cmpb_inst->u_media.t_get_info.u2AudioTrackNum;
        ptStrmNum->u1_video_strm_num = (UINT8)pt_cmpb_inst->u2VideoTrackNum;
        ptStrmNum->u1_subtl_strm_num = (UINT8)pt_cmpb_inst->u2SubtitleTrackNum;
        ptStrmNum->u1_total_strm_num = ptStrmNum->u1_audio_strm_num +
                                       ptStrmNum->u1_video_strm_num +
                                       ptStrmNum->u1_subtl_strm_num;
        
        return IMTK_PB_ERROR_CODE_OK;
    }
    
    return IMTK_PB_ERROR_CODE_NOT_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetSpeed(IMTK_PB_HANDLE_T        hHandle,
                                              IMTK_PB_CTRL_SPEED_T*    peSpeed)
{
    CMPB_CTRL_INST_T*       pt_cmpb_inst;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst || NULL == peSpeed)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    *peSpeed = pt_cmpb_inst->e_cur_speed;    
    
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetCurAudTrackIndex(IMTK_PB_HANDLE_T      hHandle,
                                                     uint16_t*             pu2ATrack)
{
    CMPB_CTRL_INST_T*       pt_cmpb_inst;
    INT32                   i4_ret;
    IMTK_PB_ERROR_CODE_T    e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if ((NULL==pt_cmpb_inst) || (NULL==pu2ATrack))
    {
        return e_ret_code;
    }

 
    i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
    
    if(OSR_OK != i4_ret)
    {
        return e_ret_code;
    }

    if (pt_cmpb_inst->t_audio_info.ui2_sel_idx)
    {
        *pu2ATrack = pt_cmpb_inst->t_audio_info.ui2_sel_idx-1;
    }
    else
    {
        *pu2ATrack = 0;
    }

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK == i4_ret)
    {
        e_ret_code = IMTK_PB_ERROR_CODE_OK;
    }

    return e_ret_code;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetAudTrack(IMTK_PB_HANDLE_T           hHandle,
                                                  uint16_t              u2ATrack)
{
    CMPB_CTRL_INST_T*       pt_cmpb_inst;
    INT32                   i4_ret;
    STREAM_COMP_ID_T        t_comp_id;
    SCDB_REC_T              t_scdb_rec = {0};
    IMTK_PB_ERROR_CODE_T    e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;

     if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_AUDIO_CHG))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (NULL== pt_cmpb_inst || u2ATrack >= MAX_GET_AUD_NUM)
    {
        return e_ret_code;
    }

    if(u2ATrack >= pt_cmpb_inst->u2AudioTrackNum)
    {
         return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    do
    {        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        if ((CMPB_CTRL_READY == pt_cmpb_inst->e_cur_state) ||
            (CMPB_CTRL_STOPPED == pt_cmpb_inst->e_cur_state))
        {
            pt_cmpb_inst->t_audio_info.ui2_sel_idx = u2ATrack+1;
            break;
        }

        if (!pt_cmpb_inst->t_audio_info.b_iter)
        {
            pt_cmpb_inst->t_audio_info.b_iter = TRUE;
            CMPB_SVCTX_GET_STREAM_COMP_ID(
                    pt_cmpb_inst->h_svctx_hdl,
                    ST_AUDIO,
                    _cmpb_audio_strm_comp_iter,
                    (VOID*)hHandle,
                    &t_comp_id,
                    &t_scdb_rec
                    );
        }

        if (pt_cmpb_inst->e_cur_state == CMPB_CTRL_PAUSED || 
            pt_cmpb_inst->e_cur_state == CMPB_CTRL_PLAYING)
        {
            if(pt_cmpb_inst->u2_cur_aud_trk != u2ATrack)
            {
                pt_cmpb_inst->u2_cur_aud_trk = u2ATrack;
                pt_cmpb_inst->t_audio_info.ui2_sel_idx = u2ATrack+1;

                i4_ret = CMPB_SVCTX_SELECT_STREAM(
                        pt_cmpb_inst->h_svctx_hdl,
                        &pt_cmpb_inst->t_audio_info.at_comp_id[u2ATrack],
                        _cmpb_ctrl_media_play_nfy_fct,
                        (VOID*)pt_cmpb_inst
                        );
                        
                if(SVCTXR_OK != i4_ret)
                {
                    CMPB_DBG_ERROR(("CMPB_SVCTX_SELECT_SVC() returns %d\n", i4_ret));
                    break;
                }
            }
            e_ret_code = IMTK_PB_ERROR_CODE_OK; 
        }

    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
    }
    
    return e_ret_code;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetSubtitleShowHide(IMTK_PB_HANDLE_T               hHandle,
                                                        bool    b_sbtl_show_hide)
{
    CMPB_CTRL_INST_T*       pt_cmpb_inst = NULL;
    INT32                   i4_ret = 0;
    IMTK_PB_ERROR_CODE_T    e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    

     if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_TRICK))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    if(!pt_cmpb_inst->fg_play_normal)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL== pt_cmpb_inst)
    {
        return e_ret_code;
    }
   
    do
    {        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }
        
        i4_ret = CMPB_SVCTX_SET_STREAM_ATTR(
                    pt_cmpb_inst->h_svctx_hdl,
                    ST_SUBTITLE,
                    (b_sbtl_show_hide)? SM_SBTLH_SET_TYPE_INVISIBLE : SM_SBTLH_SET_TYPE_VISIBLE,
                    NULL,
                    sizeof(bool)
                    );
                    
        if(SVCTXR_OK != i4_ret)
        {
            CMPB_DBG_ERROR(("CMPB_SVCTX_SET() returns %d\n", i4_ret));
            break;
        }
        e_ret_code = IMTK_PB_ERROR_CODE_OK; 
    }while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
    }
    
    return e_ret_code;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetSubtitleTrack(IMTK_PB_HANDLE_T            hHandle,
                                                  uint16_t                    u2STrack)
{
    CMPB_CTRL_INST_T*       pt_cmpb_inst;
    INT32                   i4_ret;
    STREAM_COMP_ID_T        t_comp_id;
    SCDB_REC_T              t_scdb_rec = {0};
    IMTK_PB_ERROR_CODE_T    e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;

    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (NULL== pt_cmpb_inst)
    {
        return e_ret_code;
    }
    
    if((u2STrack >= pt_cmpb_inst->u2SubtitleTrackNum) && (IMTK_CTRL_SUBTITLE_OFF != u2STrack))
    {
         return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    do
    {        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        if ((CMPB_CTRL_READY == pt_cmpb_inst->e_cur_state) ||
            (CMPB_CTRL_STOPPED == pt_cmpb_inst->e_cur_state))
        {
            if (IMTK_CTRL_SUBTITLE_OFF == u2STrack)
            {
                pt_cmpb_inst->t_subtitle_info.ui2_sel_idx = 0;
            }
            else
            {
                pt_cmpb_inst->t_subtitle_info.ui2_sel_idx = u2STrack+1;
            }
            break;
        }

        if (!pt_cmpb_inst->t_subtitle_info.b_iter)
        {
            pt_cmpb_inst->t_subtitle_info.b_iter = TRUE;
            CMPB_SVCTX_GET_STREAM_COMP_ID(
                    pt_cmpb_inst->h_svctx_hdl,
                    ST_SUBTITLE,
                    _cmpb_subtitle_strm_comp_iter,
                    (VOID*)hHandle,
                    &t_comp_id,
                    &t_scdb_rec
                    );
        }

        if (pt_cmpb_inst->e_cur_state == CMPB_CTRL_PAUSED || 
            pt_cmpb_inst->e_cur_state == CMPB_CTRL_PLAYING)
        {
            if (IMTK_CTRL_SUBTITLE_OFF == u2STrack)
            {
                pt_cmpb_inst->t_subtitle_info.ui2_sel_idx = 0;
                CMPB_SVCTX_STOP_STREAM(pt_cmpb_inst->h_svctx_hdl, ST_SUBTITLE);
                e_ret_code = IMTK_PB_ERROR_CODE_OK; 
            }
            else
            {
                pt_cmpb_inst->t_subtitle_info.ui2_sel_idx = u2STrack+1;
    
                i4_ret = CMPB_SVCTX_SELECT_STREAM(
                            pt_cmpb_inst->h_svctx_hdl,
                            &pt_cmpb_inst->t_subtitle_info.at_comp_id[u2STrack],
                            _cmpb_ctrl_media_play_nfy_fct,
                            (VOID*)pt_cmpb_inst
                            );
                            
                if(SVCTXR_OK != i4_ret)
                {
                    CMPB_DBG_ERROR(("CMPB_SVCTX_SELECT_SVC() returns %d\n", i4_ret));
                    break;
                }
                e_ret_code = IMTK_PB_ERROR_CODE_OK; 
            }
        }
        
    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
    }
    
    return e_ret_code;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetDisplayRectangle(IMTK_PB_HANDLE_T        hHandle,          
                                                     IMTK_PB_CTRL_RECT_T*    ptSrcRect,
                                                     IMTK_PB_CTRL_RECT_T*    ptDstRect)
{
    CMPB_CTRL_INST_T*       pt_cmpb_inst;
    SIZE_T                  z_size;
    INT32                   i4_ret;
    IMTK_PB_ERROR_CODE_T    e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);


    if (NULL == pt_cmpb_inst || pt_cmpb_inst->b_open_media_error)
    {
        return e_ret_code;
    }
    
    do
    {        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        z_size = sizeof(VSH_REGION_INFO_T);
        pt_cmpb_inst->b_need_set_rgn         = TRUE;

        if(pt_cmpb_inst->t_vide_rgn.ui4_height != ptSrcRect->u4H*10 ||
            pt_cmpb_inst->t_vide_rgn.ui4_width != ptSrcRect->u4W*10 || 
            pt_cmpb_inst->t_vide_rgn.ui4_x     != ptSrcRect->u4X*10 ||
            pt_cmpb_inst->t_vide_rgn.ui4_y     != ptSrcRect->u4Y*10)
        {
            pt_cmpb_inst->t_vide_rgn.ui4_height  = ptSrcRect->u4H*10;
            pt_cmpb_inst->t_vide_rgn.ui4_width   = ptSrcRect->u4W*10;
            pt_cmpb_inst->t_vide_rgn.ui4_x       = ptSrcRect->u4X*10;
            pt_cmpb_inst->t_vide_rgn.ui4_y       = ptSrcRect->u4Y*10;
            
            i4_ret = CMPB_SVCTX_SET_STREAM_ATTR(
                        pt_cmpb_inst->h_svctx_hdl,
                        ST_VIDEO,
                        SM_VSH_SET_TYPE_VIDEO_REGION,
                        (VOID*)&(pt_cmpb_inst->t_vide_rgn),
                        z_size
                        );
                        
            if(SVCTXR_OK != i4_ret)
            {
                CMPB_DBG_ERROR(("CMPB_SVCTX_SET_STREAM_ATTR() returns %d\n", i4_ret));
                break;
            }
        }

        if(pt_cmpb_inst->t_disp_rgn.ui4_height != ptDstRect->u4H*10 ||
            pt_cmpb_inst->t_disp_rgn.ui4_width != ptDstRect->u4W*10 || 
            pt_cmpb_inst->t_disp_rgn.ui4_x     != ptDstRect->u4X*10 ||
            pt_cmpb_inst->t_disp_rgn.ui4_y     != ptDstRect->u4Y*10)
        {
            pt_cmpb_inst->t_disp_rgn.ui4_height  = ptDstRect->u4H*10;
            pt_cmpb_inst->t_disp_rgn.ui4_width   = ptDstRect->u4W*10;
            pt_cmpb_inst->t_disp_rgn.ui4_x       = ptDstRect->u4X*10;
            pt_cmpb_inst->t_disp_rgn.ui4_y       = ptDstRect->u4Y*10;
            
            i4_ret = CMPB_SVCTX_SET_STREAM_ATTR(
                    pt_cmpb_inst->h_svctx_hdl,
                    ST_VIDEO,
                    SM_VSH_SET_TYPE_DISP_REGION,
                    (VOID*)&(pt_cmpb_inst->t_disp_rgn),
                    z_size
                    );
                    
            if(SVCTXR_OK != i4_ret)
            {
                CMPB_DBG_ERROR(("CMPB_SVCTX_SET_STREAM_ATTR() returns %d\n", i4_ret));
                break;
            }
        }

        e_ret_code = IMTK_PB_ERROR_CODE_OK;
        
    } while(0);

    
    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
    }
    
    return e_ret_code;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetAudioVolume(IMTK_PB_CTRL_AUD_VOLUME_INFO_T t_volume_info)
{
    INT32           i4_ret  = 0;
    unsigned int    ui4_i   = 0;
    SM_COMMAND_T    at_sm_cmds[5];
    
    at_sm_cmds[ui4_i].e_code        = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[ui4_i++].u.ui4_data  = SCC_CMD_TYPE_AUDIO;
    at_sm_cmds[ui4_i].e_code        = SCC_CMD_CODE_NAME;
    at_sm_cmds[ui4_i++].u.ps_text   = SN_PRES_MAIN_DISPLAY;
    at_sm_cmds[ui4_i].e_code        = SCC_CMD_CODE_AUDIO_VOLUME;
    at_sm_cmds[ui4_i++].u.pv_data   = (VOID*)&t_volume_info;
    at_sm_cmds[ui4_i].e_code        = SM_CMD_CODE_END;
    at_sm_cmds[ui4_i++].u.ui4_data  = 0;
        
    i4_ret  = CMPB_C_SCC_COMP_GRP_SET(at_sm_cmds);        
    return (i4_ret == SMR_OK) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetAudioVolume(IMTK_PB_CTRL_AUD_VOLUME_INFO_T* t_volume_info)
{
    INT32   i4_ret;
    HANDLE_T h_scc_aud  = NULL_HANDLE;
    SM_COMMAND_T at_sm_cmds[3];

    /* create the scc handles */
    at_sm_cmds[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data = SCC_CMD_TYPE_AUDIO;
    at_sm_cmds[1].e_code     = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
    at_sm_cmds[2].e_code     = SM_CMD_CODE_END;
    at_sm_cmds[2].u.ui4_data = 0;
    i4_ret = CMPB_C_SCC_COMP_OPEN(at_sm_cmds, &h_scc_aud);
    if(i4_ret != SMR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
        
    i4_ret = CMPB_C_SCC_GET_VOLUME(h_scc_aud, (SCC_AUD_VOLUME_INFO_T*)t_volume_info); 
    if(i4_ret != SMR_OK)
    {
       
    }

    i4_ret = CMPB_C_SCC_COMP_CLOSE(h_scc_aud);
    
    return (i4_ret == SMR_OK) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_NOT_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetAudioMute(bool b_mute)
{
    INT32 i4_ret;
    SM_COMMAND_T    at_sm_cmds[4];

    at_sm_cmds[0].e_code        = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data    = SCC_CMD_TYPE_AUDIO;
    at_sm_cmds[1].e_code        = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text     = SN_PRES_MAIN_DISPLAY;
    at_sm_cmds[2].e_code        = SCC_CMD_CODE_AUDIO_MUTE;
    at_sm_cmds[2].u.b_boolean   = b_mute;
    at_sm_cmds[3].e_code        = SM_CMD_CODE_END;
    at_sm_cmds[3].u.ui4_data    = 0;

    i4_ret = CMPB_C_SCC_COMP_GRP_SET(at_sm_cmds);
    if (i4_ret != SMR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    at_sm_cmds[0].e_code        = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data    = SCC_CMD_TYPE_AUDIO;
    at_sm_cmds[1].e_code        = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text     = SN_PRES_AUX_SOUND;
    at_sm_cmds[2].e_code        = SCC_CMD_CODE_AUDIO_MUTE;
    at_sm_cmds[2].u.b_boolean   = b_mute;
    at_sm_cmds[3].e_code        = SM_CMD_CODE_END;
    at_sm_cmds[3].u.ui4_data    = 0;

    i4_ret = CMPB_C_SCC_COMP_GRP_SET(at_sm_cmds);
    if (i4_ret != SMR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetBuffer(IMTK_PB_HANDLE_T      hHandle, 
                                           uint32_t              u4BufSize,
                                           uint8_t**             ppu1PushBuf)
{
    CMPB_CTRL_INST_T*   pt_cmpb_inst = NULL;    
    UINT32              ui4_tmp_lvl;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }    

    if(pt_cmpb_inst->e_new_state == CMPB_CTRL_STOPPED)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    /* BEFORE PLAYING: just prestore */
    if (NULL_HANDLE == pt_cmpb_inst->t_push_buf.h_mem_part)
    {
#ifndef MM_SEND_BUFFER_PREPLAY_SUPPORT
        return IMTK_PB_ERROR_CODE_NOT_OK;
#else
        pt_cmpb_inst->e_buf_status = IMTK_PB_CTRL_PUSH_BUF_PRESTORE;
        return _cmpb_ctrl_prestore_data_save(hHandle, u4BufSize, ppu1PushBuf);
#endif    
    }

    /* PLAYING */
    *ppu1PushBuf = CMPB_MEM_PART_ALLOC(pt_cmpb_inst->t_push_buf.h_mem_part,
                                    (SIZE_T)u4BufSize);             
    if(*ppu1PushBuf == NULL)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    if (pt_cmpb_inst->ui4_push_buf_level < u4BufSize)
    {
        ui4_tmp_lvl = 0;
    }
    else
    {
        ui4_tmp_lvl = pt_cmpb_inst->ui4_push_buf_level - u4BufSize;
    }
    pt_cmpb_inst->e_buf_status = IMTK_PB_CTRL_PUSH_BUF_NORMAL;

    if ((pt_cmpb_inst->ui4_push_buf_level >= pt_cmpb_inst->ui4_push_buf_low)
        && (ui4_tmp_lvl<pt_cmpb_inst->ui4_push_buf_low))
    {
        pt_cmpb_inst->ui4_data = 0;        
        pt_cmpb_inst->e_buf_status = IMTK_PB_CTRL_PUSH_BUF_HIGH;      
        if (pt_cmpb_inst->pf_notify)
        {
            pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_BUFFER_HIGH, 
                                    pt_cmpb_inst->pv_tag,
                                    pt_cmpb_inst->ui4_data
                                    );
        }
    }

    pt_cmpb_inst->ui4_push_buf_level = ui4_tmp_lvl;

    return IMTK_PB_ERROR_CODE_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CancelGetBufferPending(IMTK_PB_HANDLE_T    hHandle)
{
    return IMTK_PB_ERROR_CODE_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SendData(IMTK_PB_HANDLE_T      hHandle, 
                                          uint32_t              u4BufSize,
                                          uint8_t*              pu1PushBuf)
{
    INT32               i4_ret;
    CMPB_CTRL_INST_T*   pt_cmpb_inst = NULL;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }    
    if(pt_cmpb_inst->e_new_state == CMPB_CTRL_STOPPED)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    /* BEFORE PLAY: just return ok.*/
#ifdef MM_SEND_BUFFER_PREPLAY_SUPPORT
    if (pt_cmpb_inst->e_buf_status == IMTK_PB_CTRL_PUSH_BUF_PRESTORE)
    {
        return IMTK_PB_ERROR_CODE_OK;
    }
#endif    
    if (NULL_HANDLE == pt_cmpb_inst->t_push_buf.h_mem_part)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    /* PLAYING: push the prestore data at first. */
#ifdef MM_SEND_BUFFER_PREPLAY_SUPPORT
    i4_ret = _cmpb_ctrl_prestore_data_push(hHandle);
    if(i4_ret != IMTK_PB_ERROR_CODE_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
#endif
    i4_ret = CMPB_SVCTX_MEDIA_SET_INFO(
                    pt_cmpb_inst->h_svctx_hdl,
                    pt_cmpb_inst->h_media_hdl,
                    SVCTX_MM_SET_TYPE_SEND_PUSH_BUF,
                    (VOID*)((UINT32)pu1PushBuf - (UINT32)(pt_cmpb_inst->t_push_buf.pv_buffer)),
                    (SIZE_T)u4BufSize
                    );
    if(i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    pt_cmpb_inst->i4_push_buf_cnt ++;
    
    return IMTK_PB_ERROR_CODE_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_ReleaseBuffer(IMTK_PB_HANDLE_T      hHandle, 
                                               uint8_t*              pu1PushBuf)
{
    UINT32              ui4_size;
    UINT32              ui4_tmp_lvl;
    CMPB_CTRL_INST_T*   pt_cmpb_inst = NULL;

    if (NULL == pu1PushBuf)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    if(pu1PushBuf != NULL)
    {
        ui4_size = (UINT32)os_mem_get_mem_ptr_size(pu1PushBuf);
    
        ui4_tmp_lvl = pt_cmpb_inst->ui4_push_buf_level + ui4_size;
        pt_cmpb_inst->e_buf_status = IMTK_PB_CTRL_PUSH_BUF_NORMAL;
    
        if ((pt_cmpb_inst->ui4_push_buf_level <= pt_cmpb_inst->ui4_push_buf_high)
            && (ui4_tmp_lvl>pt_cmpb_inst->ui4_push_buf_high))
        {
            pt_cmpb_inst->ui4_data = 0;        
            pt_cmpb_inst->e_buf_status = IMTK_PB_CTRL_PUSH_BUF_LOW;      
            if (pt_cmpb_inst->pf_notify)
            {
                pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_BUFFER_LOW, 
                                        pt_cmpb_inst->pv_tag,
                                        pt_cmpb_inst->ui4_data
                                        );
            }
        }
        
        pt_cmpb_inst->ui4_push_buf_level = ui4_tmp_lvl;

        CMPB_MEM_FREE((VOID*)pu1PushBuf);
    }
            
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetPushModelEOS  ( IMTK_PB_HANDLE_T  hHandle  )  
{
    INT32                               i4_ret;
    CMPB_CTRL_INST_T*                   pt_cmpb_inst = NULL;    
    PLAYBACK_HANDLER_ITEM_IBC_INFO_T    t_ibc;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    t_ibc.e_cmd_type = PLAYBACK_CTRL_IBC_TYPE_EOS;
    i4_ret = CMPB_SVCTX_MEDIA_SET_INFO(
                    pt_cmpb_inst->h_svctx_hdl,
                    pt_cmpb_inst->h_media_hdl,
                    SVCTX_MM_SET_TYPE_SEND_IBC,
                    (VOID*)&t_ibc,
                    (SIZE_T)sizeof(PLAYBACK_HANDLER_ITEM_IBC_INFO_T)
                    );
    if(i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }    
    
    return IMTK_PB_ERROR_CODE_OK;

}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SendCmd(IMTK_PB_HANDLE_T       hHandle,
                                         IMTK_PB_CTRL_IBC*      ptIBC)
{
    INT32                               i4_ret;
    CMPB_CTRL_INST_T*                   pt_cmpb_inst = NULL;
    PLAYBACK_HANDLER_ITEM_IBC_INFO_T    t_ibc;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }    

    CMPB_MEMCPY((VOID*)&t_ibc, (VOID*)ptIBC, sizeof(PLAYBACK_HANDLER_ITEM_IBC_INFO_T));
    switch (ptIBC->eCmdType)
    {
    case IMTK_PB_CTRL_IBC_TYPE_SET_ASF_PACKET_INFO:
        t_ibc.e_cmd_type = PLAYBACK_CTRL_IBC_TYPE_SET_ASF_PACKET_INFO;
        break;

    case IMTK_PB_CTRL_IBC_TYPE_SET_ENCRYPT_INFO:
        t_ibc.e_cmd_type = PLAYBACK_CTRL_IBC_TYPE_ENCRYPT;
        break;

    default:
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    i4_ret = CMPB_SVCTX_MEDIA_SET_INFO(
                    pt_cmpb_inst->h_svctx_hdl,
                    pt_cmpb_inst->h_media_hdl,
                    SVCTX_MM_SET_TYPE_SEND_IBC,
                    (VOID*)&t_ibc,
                    (SIZE_T)sizeof(PLAYBACK_HANDLER_ITEM_IBC_INFO_T)
                    );
    if(i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }    
    
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetEncryptInfo(
                            IMTK_PB_HANDLE_T                            hHandle,
                            IMTK_PB_CTRL_IBC_PARAM_SET_ENCRYPT_INFO*    ptInfo)
{
    INT32                               i4_ret;
    CMPB_CTRL_INST_T*                   pt_cmpb_inst = NULL;
    PLAYBACK_HANDLER_ITEM_IBC_INFO_T    t_ibc;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }    

    t_ibc.e_cmd_type = PLAYBACK_CTRL_IBC_TYPE_ENCRYPT;
    CMPB_MEMCPY((VOID*)&(t_ibc.t_param.t_set_encrypt_info_param), 
                (VOID*)ptInfo, 
                sizeof(IMTK_PB_CTRL_IBC_PARAM_SET_ENCRYPT_INFO));

    i4_ret = CMPB_SVCTX_MEDIA_SET_INFO(
                    pt_cmpb_inst->h_svctx_hdl,
                    pt_cmpb_inst->h_media_hdl,
                    SVCTX_MM_SET_TYPE_SEND_IBC,
                    (VOID*)&t_ibc,
                    (SIZE_T)sizeof(PLAYBACK_HANDLER_ITEM_IBC_INFO_T)
                    );
    if(i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }    
    
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Play(IMTK_PB_HANDLE_T   hHandle, 
                                         uint32_t           u4Time)
{

    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    UINT32                      ui4_stream_mode = 0;
    VSH_SET_PLANE_ORDER_INFO_T  t_vpla_ord;
    SRC_DESC_T                  t_src_desc = {0};
    SNK_DESC_T                  t_snk_desc = {0};
    MM_SPEED_TYPE_T             e_spd_type = MM_SPEED_TYPE_FORWARD_1X;
    IMTK_PB_CTRL_SETTING_T      t_setting;
    
    /*printf("IMtkPb_Ctrl_Play\n");*/
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return e_ret_code;
    }

    if (pt_cmpb_inst->e_cur_state == CMPB_CTRL_PLAYING)
    {
        if (pt_cmpb_inst->e_cur_speed != IMTK_PB_CTRL_SPEED_1X)
        {
            IMtkPb_Ctrl_SetSpeed(hHandle, IMTK_PB_CTRL_SPEED_1X);
        }
        return IMTK_PB_ERROR_CODE_OK;
    }
    
    if (pt_cmpb_inst->e_cur_state == CMPB_CTRL_PAUSED)
    {
        if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PUSH)
        {
            pt_cmpb_inst->e_buf_status = IMTK_PB_CTRL_PUSH_BUF_NORMAL;
            if (pt_cmpb_inst->ui4_push_buf_level < pt_cmpb_inst->ui4_push_buf_low)
            {
                pt_cmpb_inst->ui4_data = 0;        
                pt_cmpb_inst->e_buf_status = IMTK_PB_CTRL_PUSH_BUF_HIGH;       
                if (pt_cmpb_inst->pf_notify)
                {
                    pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_BUFFER_HIGH, 
                                            pt_cmpb_inst->pv_tag,
                                            pt_cmpb_inst->ui4_data
                                            );
                }
            }
            else if (pt_cmpb_inst->ui4_push_buf_level > pt_cmpb_inst->ui4_push_buf_high)
            {
                pt_cmpb_inst->ui4_data = 0;        
                pt_cmpb_inst->e_buf_status = IMTK_PB_CTRL_PUSH_BUF_LOW;
                if (pt_cmpb_inst->pf_notify)
                {
                    pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_BUFFER_LOW, 
                                            pt_cmpb_inst->pv_tag,
                                            pt_cmpb_inst->ui4_data
                                            );
                }
            }
        }

        pt_cmpb_inst->e_old_state = pt_cmpb_inst->e_cur_state;
        pt_cmpb_inst->e_cur_state = CMPB_CTRL_PLAYING;

        CMPB_MEMSET(&t_setting, 0, sizeof(IMTK_PB_CTRL_SETTING_T));
        return IMtkPb_Ctrl_SetSpeed(hHandle, IMTK_PB_CTRL_SPEED_1X);
    }

    if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PUSH)
    {   /* MHEG5 ICS requirements */
        /* CMPB Should open media again, as caller might change the audio, video, or subtitle */
        e_ret_code = _push_mode_open_media(hHandle);
        if(e_ret_code != IMTK_PB_ERROR_CODE_OK)
        {
            return e_ret_code;
        }
    }
    
    do
    {        
        if(h_sema_thumbnail)
        {
            i4_ret = CMPB_SEMA_LOCK(h_sema_thumbnail, X_SEMA_OPTION_WAIT); 
        }
        _set_thumbnail_mode(FALSE, (pt_cmpb_inst->ui1_group_mode == 0));
        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        if (CMPB_CTRL_PRELOADING == pt_cmpb_inst->e_cur_state)
        {
            pt_cmpb_inst->b_preload_to_play = TRUE;
            pt_cmpb_inst->e_cur_state = CMPB_CTRL_PLAYING;
            e_ret_code = IMTK_PB_ERROR_CODE_OK;
            
            break;
        }
                
        if ((CMPB_CTRL_READY != pt_cmpb_inst->e_cur_state) &&
            (CMPB_CTRL_STEP  != pt_cmpb_inst->e_cur_state) &&
            (CMPB_CTRL_TRICK != pt_cmpb_inst->e_cur_state) &&
            (CMPB_CTRL_STOPPED != pt_cmpb_inst->e_cur_state))
        {
            break;
        }

        if (pt_cmpb_inst->ui4_play_flag & IMTK_PB_CTRL_PLAY_FLAG_VIDEO)
        {
            ui4_stream_mode |= ST_MASK_VIDEO;
        }
        
        if (pt_cmpb_inst->ui4_play_flag & IMTK_PB_CTRL_PLAY_FLAG_AUDIO)
        {
            ui4_stream_mode |= ST_MASK_AUDIO;
            if (0 == pt_cmpb_inst->t_audio_info.ui2_sel_idx)
            {
                pt_cmpb_inst->t_audio_info.ui2_sel_idx = 1;
            }
        }
        
        if ( pt_cmpb_inst->t_src_sub_type != SRC_SUB_TYPE_VESTEL_TS )
        {
            ui4_stream_mode |= ST_MASK_SUBTITLE;
        }
        else if(pt_cmpb_inst->ui4_play_flag & IMTK_PB_CTRL_PLAY_FLAG_SUBTITLE)
        {
            ui4_stream_mode |= ST_MASK_SUBTITLE;
        }
        
        t_src_desc.u_info.t_mm.h_media = pt_cmpb_inst->h_media_hdl;
        t_src_desc.u_info.t_mm.t_speed = e_spd_type;
        t_src_desc.u_info.t_mm.ui2_pmt_idx = pt_cmpb_inst->ui2_program_idx;
        t_src_desc.u_info.t_mm.t_pos_info.t_pos_type = MM_SVC_POS_TYPE_ALL_TIME;
        t_src_desc.u_info.t_mm.t_pos_info.ui2_idx = (UINT16)0;
        t_src_desc.u_info.t_mm.t_pos_info.ui8_pos_val = (UINT64)u4Time * 90000;

        if(pt_cmpb_inst->t_src_sub_type == SRC_SUB_TYPE_SUB_FREE_RUN)
        {
            t_src_desc.u_info.t_mm.b_sub_free_run = TRUE;
        }
        if (pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_URI)
        {
            t_src_desc.u_info.t_mm.t_bufctrl_info.b_is_prebuff = FALSE;
            
            if (pt_cmpb_inst->u_info.t_uri.eBufSizeType == IMTK_PB_CTRL_BUF_SIZE_TYPE_BYTE)
            {
                t_src_desc.u_info.t_mm.t_bufctrl_info.e_ctrl_type = MM_SVC_BUFFCTRL_TYPE_BYTE;
                t_src_desc.u_info.t_mm.t_bufctrl_info.ui4_buf_size.ui4_bytes = pt_cmpb_inst->u_info.t_uri.uBufSize.u4Bytes;
            }
            else
            {
                t_src_desc.u_info.t_mm.t_bufctrl_info.e_ctrl_type = MM_SVC_BUFFCTRL_TYPE_SEC;
                t_src_desc.u_info.t_mm.t_bufctrl_info.ui4_buf_size.ui4_duration = pt_cmpb_inst->u_info.t_uri.uBufSize.u4Duration;
            }
            
            t_src_desc.u_info.t_mm.t_bufctrl_info.pf_nfy = _cmpb_buffer_status_cb;            
            t_src_desc.u_info.t_mm.t_bufctrl_info.ui4_keepbuf_threshold = pt_cmpb_inst->u_info.t_uri.u4KeepBufThreshold;
            t_src_desc.u_info.t_mm.t_bufctrl_info.ui4_rebuf_threshold = pt_cmpb_inst->u_info.t_uri.u4ReBufThreshold;
        }

        t_vpla_ord.e_order_ctrl = VSH_PLANE_ORDER_CTRL_BOTTOM;
        t_vpla_ord.u.ui1_num_layers = 0;

        t_src_desc.e_type = SRC_TYPE_MEDIA_NETWORK;
        

        if (!(pt_cmpb_inst->ui4_play_flag & IMTK_PB_CTRL_PLAY_FLAG_VIDEO))
        {
            pt_cmpb_inst->t_vide_rgn.ui4_width  = 0;
            pt_cmpb_inst->t_vide_rgn.ui4_height = 0;
            pt_cmpb_inst->t_disp_rgn.ui4_width  = 0;
            pt_cmpb_inst->t_disp_rgn.ui4_height = 0;
        }
        t_snk_desc.pt_video_region        = &(pt_cmpb_inst->t_vide_rgn);
        t_snk_desc.pt_disp_region         = &(pt_cmpb_inst->t_disp_rgn);
        t_snk_desc.pt_video_plane_order   = &t_vpla_ord;
        t_snk_desc.pt_video_strm_fltr_fct = _cmpb_video_strm_comp_filter;
        t_snk_desc.pv_video_strm_fltr_tag = (VOID*)hHandle;
        t_snk_desc.pt_audio_strm_fltr_fct = _cmpb_audio_strm_comp_filter;
        t_snk_desc.pv_audio_strm_fltr_tag = (VOID*)hHandle;
        t_snk_desc.u_text_strm.t_subtitle.pt_fltr_fct = _cmpb_subtitle_strm_comp_filter;
        t_snk_desc.u_text_strm.t_subtitle.pv_fltr_tag = (VOID*)hHandle;
        t_snk_desc.u_text_strm.t_subtitle.h_gl_plane = pt_cmpb_inst->h_gl_plane;
        t_snk_desc.e_text_strm_type = ST_SUBTITLE;
        
        if (pt_cmpb_inst->ui1_group_mode == 0)
        {
            t_snk_desc.ps_snk_grp_name        = SN_PRES_MAIN_DISPLAY;
        }
        else if (pt_cmpb_inst->ui1_group_mode == 1)
        {
            t_snk_desc.ps_snk_grp_name        = SN_PRES_SUB_DISPLAY;
        }
        else if (pt_cmpb_inst->ui1_group_mode == 2)
        {
            t_snk_desc.ps_snk_grp_name        = SN_PRES_TRD_SOUND;
        }
        else
        {
            t_snk_desc.ps_snk_grp_name        = SN_PRES_MAIN_DISPLAY;
        }
        pt_cmpb_inst->fg_buf_rdy          = FALSE;
        pt_cmpb_inst->fg_play_normal      = FALSE;

        if(pt_cmpb_inst->e_new_state != CMPB_CTRL_STOPPED)
        {
            CMPB_SVCTX_SYNC_STOP_SVC(pt_cmpb_inst->h_svctx_hdl, DEFAULT_STOP_SVC_TIMEOUT);

#ifdef MW_TV_AV_BYPASS_SUPPORT
            if(pt_cmpb_inst->e_cmpb_profile != IMTK_PB_CTRL_PROFILE_MAIN_PUSH_ON_SUB)
            {
                CMPB_SVCTX_SET(h_tv_bypass_svctx,
                                SVCTX_COMMON_SET_ENABLE_SVC_SELECT_WITH_ASSOCIATE,
                                &b_tv_scart_enabled,
                                sizeof(BOOL));

                CMPB_SVCTX_SET(h_monitor_bypass_svctx,
                                SVCTX_COMMON_SET_ENABLE_SVC_SELECT_WITH_ASSOCIATE,
                                &b_monitor_scart_enabled,
                                sizeof(BOOL));
            }
#endif
        }
        t_src_desc.ui4_title_idx = pt_cmpb_inst->ui4_title_idx;
        t_src_desc.ui4_playlist_idx = pt_cmpb_inst->ui4_playlist_idx;
        t_src_desc.ui4_chap_idx = pt_cmpb_inst->ui4_chap_idx;
        t_src_desc.u_info.t_mm.ui2_svc_pid = pt_cmpb_inst->ui2_svc_pid;

        if (pt_cmpb_inst->ui1_group_mode == 1)
        {
            _cmpb_set_tv_mode(SCC_TV_MODE_POP);
        }

        if(pt_cmpb_inst->e_cmpb_profile == IMTK_PB_CTRL_PROFILE_MAIN_PUSH_ON_SUB)
        {
            t_src_desc.u_info.t_mm.b_still_at_last = TRUE;
        }
        else
        {
            t_src_desc.u_info.t_mm.b_still_at_last = FALSE;
        }
        
        if (pt_cmpb_inst->t_ext_info.eExtraInfoType == IMTK_PB_CTRL_EXTRA_INFO_TYPE_3D_VIDEO)
        {
            UINT32  ui4_3d_type;
            BOOL    b_supported_3D = TRUE;

            switch(pt_cmpb_inst->t_ext_info.uExtraInfoParam.e3dVideoType)
            {
            case IMTK_PB_CTRL_3D_VIDEO_TAB_LE_T:
                ui4_3d_type = (UINT32)MM_SCOPIC_LT;
                break;

            case IMTK_PB_CTRL_3D_VIDEO_TAB_RE_T:
                ui4_3d_type = (UINT32)MM_SCOPIC_RT;
                break;

            case IMTK_PB_CTRL_3D_VIDEO_SBS_LE_L:
                ui4_3d_type = (UINT32)MM_SCOPIC_LF;
                break;

            case IMTK_PB_CTRL_3D_VIDEO_SBS_RE_L:
                ui4_3d_type = (UINT32)MM_SCOPIC_RF;
                break;

            default:
                b_supported_3D = FALSE;
                break;
            }

            if (b_supported_3D)
            {
                CMPB_SVCTX_MEDIA_SET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_SET_TYPE_3D_INFO,
                        (VOID*)&ui4_3d_type,
                        sizeof(UINT32));
            }        
        }

        pt_cmpb_inst->ui4_last_timecode = 0;
        pt_cmpb_inst->fg_eos = FALSE;

        i4_ret = CMPB_SVCTX_SELECT_SVC(
                        pt_cmpb_inst->h_svctx_hdl,
                        ui4_stream_mode,
                        &t_src_desc,
                        &t_snk_desc,
                        _cmpb_ctrl_media_play_nfy_fct,
                        (VOID*)pt_cmpb_inst
                        );

        if(SVCTXR_OK != i4_ret)
        {
            CMPB_DBG_ERROR(("CMPB_SVCTX_SELECT_SVC() returns %d\n", i4_ret));
            if (pt_cmpb_inst->ui1_group_mode == 1)
            {
                _cmpb_set_tv_mode(SCC_TV_MODE_NORMAL);
            }
            break;
        }

        pt_cmpb_inst->e_cur_state = CMPB_CTRL_PLAYING;
        e_ret_code = IMTK_PB_ERROR_CODE_OK;
        
        CMPB_TIMER_START(
                    pt_cmpb_inst->h_time_code_update,
                    gui4_get_time_interval,
                    X_TIMER_FLAG_REPEAT,
                    _cmpb_get_timecode_callback,
                    (VOID*)hHandle
                    );
        
    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
        
    }

    if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PUSH)
    {
        UINT8 aui1_part_name[20];
        
        CMPB_MEMSET(aui1_part_name, 0, 20);

        while(!pt_cmpb_inst->fg_buf_rdy)
        {
            CMPB_THREAD_DELAY(10);
        };
        
        /*--------------session opened and start get info------*/
        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_TYPE_PUSH_BUFF,
                        &(pt_cmpb_inst->t_push_buf),
                        sizeof(IMTK_CTRL_PUSH_BUF_INFO_T)
                        );
        if(i4_ret != SVCTXR_OK)
        {
            return IMTK_PB_ERROR_CODE_NOT_OK;
        }
        
        CMPB_SPRINTF((CHAR*)aui1_part_name, "pmem_cmpb_%d", pt_cmpb_inst->ui1_group_mode);
        i4_ret = CMPB_MEM_PART_CREATE((HANDLE_T*)&(pt_cmpb_inst->t_push_buf.h_mem_part),
                                    (CHAR*)aui1_part_name,
                                    pt_cmpb_inst->t_push_buf.pv_buffer,
                                    pt_cmpb_inst->t_push_buf.z_buffer_size,
                                    0);
        pt_cmpb_inst->ui4_push_buf_level = (UINT32)pt_cmpb_inst->t_push_buf.z_buffer_size;
        pt_cmpb_inst->ui4_push_buf_high = pt_cmpb_inst->ui4_push_buf_level 
                                        - (pt_cmpb_inst->ui4_push_buf_level*BUF_THRESHOLD_HIGH)/100;
        pt_cmpb_inst->ui4_push_buf_low = (pt_cmpb_inst->ui4_push_buf_level*BUF_THRESHOLD_LOW)/100;
        pt_cmpb_inst->e_buf_status = IMTK_PB_CTRL_PUSH_BUF_LOW;
        
        if (pt_cmpb_inst->pf_notify)
        {
            pt_cmpb_inst->pf_notify(IMTK_PB_CTRL_EVENT_BUFFER_LOW, 
                                    pt_cmpb_inst->pv_tag,
                                    pt_cmpb_inst->ui4_data
                                    );
        }
    }

    pt_cmpb_inst->e_cur_speed = IMTK_PB_CTRL_SPEED_1X;
    pt_cmpb_inst->e_new_state = CMPB_CTRL_PLAYING;
    
    return e_ret_code;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Pause(IMTK_PB_HANDLE_T  hHandle)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;

    if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_PAUSE))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    /*printf("IMtkPb_Ctrl_Pause\n");*/
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst || (!pt_cmpb_inst->fg_play_normal))
    {
        return e_ret_code;
    }
    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }
                        
        if (FALSE == _cmpb_ctrl_set_speed(pt_cmpb_inst,IMTK_PB_CTRL_SPEED_ZERO))
        {
            break;
        }

        pt_cmpb_inst->e_cur_speed = IMTK_PB_CTRL_SPEED_ZERO;
        
        pt_cmpb_inst->e_old_state = pt_cmpb_inst->e_cur_state;
        pt_cmpb_inst->e_cur_state = CMPB_CTRL_PAUSED;
        
        e_ret_code = IMTK_PB_ERROR_CODE_OK;
        
    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }
    
    return e_ret_code;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Stop(IMTK_PB_HANDLE_T   hHandle)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;

    if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_STOP))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    /*printf("IMtkPb_Ctrl_Stop\n");*/
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return e_ret_code;
    }
    do
    {
        if (pt_cmpb_inst->b_start_parse)
        {
            while (1)
            {
                if (pt_cmpb_inst->b_parse_done)
                {
                    break;
                }
                CMPB_THREAD_DELAY(100);
            }
        }

        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }
        /*  If you set super freeze here, the following SYNC_STOP_SVC can not stop video decoder.
            And it will cause TV video can not be displayed, so we marked it.
        */
        #if 0
        if(pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PUSH)
        {
            _set_video_freeze(TRUE);
        }
        #endif

        if (pt_cmpb_inst->e_cur_state != CMPB_CTRL_STOPPED)
        {
            pt_cmpb_inst->e_new_state = CMPB_CTRL_STOPPED;

            CMPB_TIMER_STOP(pt_cmpb_inst->h_time_code_update);

            i4_ret = CMPB_SVCTX_SYNC_STOP_SVC(
                        pt_cmpb_inst->h_svctx_hdl,
                        DEFAULT_STOP_SVC_TIMEOUT
                        );

            if (pt_cmpb_inst->ui1_group_mode == 1)
            {
                _cmpb_set_tv_mode(SCC_TV_MODE_NORMAL);
            }

            if(SVCTXR_OK != i4_ret)
            {
                CMPB_DBG_ERROR(("\n!!!!c_svctx_stop_svc() returns %d!!!!\n\n", i4_ret));
                break;
            }
            
			
		pt_cmpb_inst->ui4_av_info = 0;
		pt_cmpb_inst->ui4_av_update = 0;	

            
            pt_cmpb_inst->e_old_state = pt_cmpb_inst->e_cur_state;
            pt_cmpb_inst->e_cur_state = CMPB_CTRL_STOPPED;
            pt_cmpb_inst->e_new_state = CMPB_CTRL_STOPPED;
            pt_cmpb_inst->b_preload_to_play = FALSE;
            pt_cmpb_inst->ui4_last_timecode = 0;
        }
        pt_cmpb_inst->fg_eos = FALSE;
        e_ret_code = IMTK_PB_ERROR_CODE_OK;
        
    } while(0);

    if(pt_cmpb_inst->t_push_buf.h_mem_part != NULL_HANDLE)
    {
        CMPB_MEM_PART_DELETE(pt_cmpb_inst->t_push_buf.h_mem_part);
        pt_cmpb_inst->t_push_buf.h_mem_part = NULL_HANDLE;
        CMPB_SVCTX_MEDIA_SET_INFO(
                pt_cmpb_inst->h_svctx_hdl,
                pt_cmpb_inst->h_media_hdl,
                SVCTX_MM_SET_TYPE_REL_PUSH_BUF,
                &pt_cmpb_inst->t_push_buf,
                sizeof(IMTK_CTRL_PUSH_BUF_INFO_T)
                );
    }
    pt_cmpb_inst->fg_buf_rdy = FALSE;
    pt_cmpb_inst->fg_eos = FALSE;

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }

    if(h_sema_thumbnail)
    {
        i4_ret = CMPB_SEMA_UNLOCK(h_sema_thumbnail); 
    }

    return e_ret_code;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Step(IMTK_PB_HANDLE_T   hHandle,
                                      uint32_t            ui4StepAmount)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    MM_SVC_TRICK_INFO_T         t_trick;

    if (0 == ui4StepAmount)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_STEP))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    }    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst || (!pt_cmpb_inst->fg_play_normal))
    {
        return e_ret_code;
    }
    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        t_trick.e_speed_type = MM_SPEED_TYPE_STEP;
        t_trick.u.ui4_data = ui4StepAmount;             

        i4_ret = CMPB_SVCTX_SET(
                    pt_cmpb_inst->h_svctx_hdl,
                    SVCTX_MM_SET_TYPE_TRICK,
                    (VOID*)&t_trick,
                    sizeof(MM_SVC_TRICK_INFO_T)
                    );
                    
        if(SVCTXR_OK != i4_ret)
        {
            CMPB_DBG_ERROR(("CMPB_SVCTX_SET() returns %d\n", i4_ret));
            break;
        }

        pt_cmpb_inst->e_cur_state = CMPB_CTRL_STEP;
        pt_cmpb_inst->e_cur_speed = IMTK_PB_CTRL_SPEED_ZERO;

        e_ret_code = IMTK_PB_ERROR_CODE_OK;
        
    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }
    
    return e_ret_code;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_TimeSeek(IMTK_PB_HANDLE_T   hHandle, 
                                          uint32_t           u4Time)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    MM_SVC_POS_INFO_T           t_pos;
    
    if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_SEEK))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    } 
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst || (!pt_cmpb_inst->fg_play_normal))
    {
        return e_ret_code;
    }
    
    if(pt_cmpb_inst->u_media.t_get_info.u4TotalDuration == 0 &&
       pt_cmpb_inst->e_model == IMTK_PB_CTRL_BUFFERING_MODEL_PULL)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }
                        
        CMPB_MEMSET(&t_pos, 0, sizeof(MM_SVC_POS_INFO_T));

        t_pos.t_pos_type    = MM_SVC_POS_TYPE_ALL_TIME;
        t_pos.ui2_idx       = (UINT16)0;
        t_pos.ui8_pos_val   = (UINT64)u4Time;
        
        pt_cmpb_inst->fg_during_seek = TRUE;

        i4_ret = CMPB_SVCTX_MEDIA_SEEK(
                        pt_cmpb_inst->h_svctx_hdl,
                        _cmpb_ctrl_media_play_nfy_fct,
                        (VOID*)pt_cmpb_inst,
                        &t_pos
                        );
                        
        if(SVCTXR_OK != i4_ret && SVCTXR_ASYNC_NFY != i4_ret)
        {
            pt_cmpb_inst->fg_during_seek = FALSE;
            CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_SEEK() returns %d\n", i4_ret));
            break;
        }
        
        pt_cmpb_inst->ui4_last_timecode = u4Time * 1000;

        e_ret_code = IMTK_PB_ERROR_CODE_OK;
    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }
    
#if 0 /*shouldn't  block client*/
    while(pt_cmpb_inst->fg_during_seek) /*wait for seek done*/
    {
        DBG_INFO(("waiting for seek done \n"));
        x_thread_delay(100);
    };
#endif
    return e_ret_code;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_ByteSeek(IMTK_PB_HANDLE_T   hHandle, 
                                          uint64_t           u8Pos)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    MM_SVC_POS_INFO_T           t_pos;
    
    if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_SEEK))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    } 
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst || (!pt_cmpb_inst->fg_play_normal))
    {
        return e_ret_code;
    }
    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }
                        
        CMPB_MEMSET(&t_pos, 0, sizeof(MM_SVC_POS_INFO_T));

        t_pos.t_pos_type    = MM_SVC_POS_TYPE_ALL_OFFSET;
        t_pos.ui2_idx       = (UINT16)0;
        t_pos.ui8_pos_val   = u8Pos;

        pt_cmpb_inst->fg_during_seek = TRUE;
        i4_ret = CMPB_SVCTX_MEDIA_SEEK(
                        pt_cmpb_inst->h_svctx_hdl,
                        _cmpb_ctrl_media_play_nfy_fct,
                        (VOID*)pt_cmpb_inst,
                        &t_pos
                        );
                        
        if(SVCTXR_OK != i4_ret && SVCTXR_ASYNC_NFY != i4_ret)
        {
            pt_cmpb_inst->fg_during_seek = FALSE;
            CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_SEEK() returns %d\n", i4_ret));
            break;
        }
        
        e_ret_code = IMTK_PB_ERROR_CODE_OK;       
    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }
#if 0 /*shouldn't  block client*/
    while(pt_cmpb_inst->fg_during_seek) /*wait for seek done*/
    {
        DBG_INFO(("waiting for seek done \n"));
        x_thread_delay(100);
    };
#endif    
    return e_ret_code;
}

/*add cmpb pts seek*/

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_ByteSeekPts(IMTK_PB_HANDLE_T   hHandle, 
                                             uint64_t           u8Pos,
                                             uint64_t           u8Pts)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    MM_SVC_POS_INFO_T           t_pos;
    
    //fprintf(stderr, ">>> %s(pos: %lld, pts: %lld) %s.%d\n", __FUNCTION__, u8Pos, u8Pts, __FILE__, __LINE__);
    if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_SEEK))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    } 
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst || (!pt_cmpb_inst->fg_play_normal))
    {
        return e_ret_code;
    }
    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }
                        
        CMPB_MEMSET(&t_pos, 0, sizeof(MM_SVC_POS_INFO_T));

        t_pos.t_pos_type    = MM_SVC_POS_TYPE_ALL_OFFSET;
        t_pos.ui2_idx       = (UINT16)0;
        t_pos.ui8_pos_val   = u8Pos;
        t_pos.u.render_info.ui8_pts_info = (u8Pts != 0) ? u8Pts : (UINT64)-1;

        pt_cmpb_inst->fg_during_seek = TRUE;
        i4_ret = CMPB_SVCTX_MEDIA_SEEK(
                        pt_cmpb_inst->h_svctx_hdl,
                        _cmpb_ctrl_media_play_nfy_fct,
                        (VOID*)pt_cmpb_inst,
                        &t_pos
                        );
                        
        if(SVCTXR_OK != i4_ret && SVCTXR_ASYNC_NFY != i4_ret)
        {
            pt_cmpb_inst->fg_during_seek = FALSE;
            CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_SEEK() returns %d\n", i4_ret));
            break;
        }
        
        e_ret_code = IMTK_PB_ERROR_CODE_OK;       
    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }
#if 0 /*shouldn't  block client*/
    while(pt_cmpb_inst->fg_during_seek) /*wait for seek done*/
    {
        DBG_INFO(("waiting for seek done \n"));
        x_thread_delay(100);
    };
#endif    
    return e_ret_code;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetVideoResolution(IMTK_PB_HANDLE_T                   hHandle,
                                              uint32_t*     ptWidth,
                                              uint32_t*     ptHeight)
{
    CMPB_CTRL_INST_T* pt_cmpb_inst = NULL;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (pt_cmpb_inst && (pt_cmpb_inst->u2VideoTrackNum > 0))
    {
        *ptWidth = pt_cmpb_inst->u_vid_track.t_get_info[0].u4Width;
        *ptHeight = pt_cmpb_inst->u_vid_track.t_get_info[0].u4Height;
        return IMTK_PB_ERROR_CODE_OK;
    }
    
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetMediaInfo(IMTK_PB_HANDLE_T                   hHandle,
                                              IMTK_PB_CTRL_GET_MEDIA_INFO_T*     ptMediaInfo)
{
    CMPB_CTRL_INST_T* pt_cmpb_inst;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (pt_cmpb_inst)
    {
        CMPB_MEMCPY(ptMediaInfo,
                 &(pt_cmpb_inst->u_media.t_get_info),
                 sizeof(IMTK_PB_CTRL_GET_MEDIA_INFO_T)
                 );
        return IMTK_PB_ERROR_CODE_OK;
    }
    
    return IMTK_PB_ERROR_CODE_NOT_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetBufferFullness(IMTK_PB_HANDLE_T      hHandle, 
                                                   uint32_t*             pu4Percentage)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    MM_SVC_BUFFCTRL_INFO_T      t_buffer_control_info;
    SIZE_T                      z_size = sizeof(MM_SVC_BUFFCTRL_INFO_T);

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return e_ret_code;
    }
    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }
        
        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_TYPE_BUFFCTRL_INFO,
                        (VOID*)&t_buffer_control_info,
                        z_size
                        );
                        
        if(SVCTXR_OK != i4_ret)
        {
            CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_GET_INFO() returns %d\n", i4_ret));
            break;
        }

        if (0 == t_buffer_control_info.ui4_buf_size.ui4_bytes)
        {
            break;    
        }
        
        *pu4Percentage = (uint32_t)((t_buffer_control_info.ui4_buff_fullness*100)/t_buffer_control_info.ui4_buf_size.ui4_bytes);
        
        e_ret_code = IMTK_PB_ERROR_CODE_OK;
        
    } while(0);

    
    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }
    
    return e_ret_code;
        
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetAFifoFullness(IMTK_PB_HANDLE_T      hHandle, 
                                                   uint32_t*             pui4_data_sz)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    SIZE_T                      z_size = sizeof( UINT32 );

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return e_ret_code;
    }
    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }
        
        i4_ret = CMPB_SVCTX_GET(
                        pt_cmpb_inst->h_svctx_hdl,
                        SVCTX_MM_GET_TYPE_AFIFO_AMOUNT,
                        (VOID*)pui4_data_sz,
                        &z_size
                        );
                        
        if(SVCTXR_OK != i4_ret)
        {
            CMPB_DBG_ERROR(("CMPB_SVCTX_GET() returns %d\n", i4_ret));
            break;
        }
        
        e_ret_code = IMTK_PB_ERROR_CODE_OK;
        
    } while(0);

    
    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }
    
    return e_ret_code;
        
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetBufferStatus(IMTK_PB_HANDLE_T hHandle, IMTK_PB_CTRL_BUFFER_STATUS_T* ptBufferStatus)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    UINT32                      ui4_cur_time = 0;
    UINT64                      ui8_cur_pos = 0;
    UINT32                      ui4_avg_bitrate=0;
    MM_SVC_BUF_STATUS_T         t_buf_status;
    SIZE_T                      z_size = sizeof(MM_SVC_BUF_STATUS_T);

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if ((NULL==pt_cmpb_inst) || (NULL==ptBufferStatus))
    {
        return e_ret_code;
    }

    do
    {
        e_ret_code = IMtkPb_Ctrl_GetCurrentPos(hHandle, (uint32_t*)&ui4_cur_time, &ui8_cur_pos);
        if (e_ret_code != IMTK_PB_ERROR_CODE_OK)
        {
            return e_ret_code;
        }
        
        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;

        if (0 == ui4_cur_time)
        {
            return e_ret_code;
        }

        ui4_avg_bitrate = (ui8_cur_pos*1000)/ui4_cur_time;

        if (0 == ui4_avg_bitrate)
        {
            return e_ret_code;
        }

        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_BUF_STATUS,
                        (VOID*)&t_buf_status,
                        z_size
                        );

        if(SVCTXR_OK != i4_ret)
        {
            break;
        }

     /* ptBufferStatus->u4BufEndTime = t_buf_status.ui8_end_pos*1000/ui4_avg_bitrate;
        ptBufferStatus->u4BufBeginTime = t_buf_status.ui8_begin_pos*1000/ui4_avg_bitrate;*/
        ptBufferStatus->u4BufEndTime = (t_buf_status.ui8_end_pos - t_buf_status.ui8_data_ofst)*ui4_cur_time/ui8_cur_pos;
        ptBufferStatus->u4BufBeginTime = (t_buf_status.ui8_begin_pos - t_buf_status.ui8_data_ofst)*ui4_cur_time/ui8_cur_pos;

        if (ptBufferStatus->u4BufEndTime < ui4_cur_time)
        {
            ptBufferStatus->u4BufEndTime = ui4_cur_time;
        }

        if (ptBufferStatus->u4BufBeginTime > ptBufferStatus->u4BufEndTime)
        {
            ptBufferStatus->u4BufBeginTime = ptBufferStatus->u4BufEndTime;
        }

        if (ptBufferStatus->u4BufEndTime > ui4_cur_time)
        {
            ptBufferStatus->u4BufRemainDur = ptBufferStatus->u4BufEndTime - ui4_cur_time;
        }
        else
        {
            ptBufferStatus->u4BufRemainDur = 0;
        }

        ptBufferStatus->fgBufFull = FALSE;

        e_ret_code = IMTK_PB_ERROR_CODE_OK;

    } while(0);

    return e_ret_code;
}

extern IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_TimeToSize(IMTK_PB_HANDLE_T     hHandle,
                                                   uint32_t             u4Time,
                                                   uint32_t*            pu4Size)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    MM_SVC_TIME_TO_SIZE_T       t_timw2size_info;
    SIZE_T                      z_size = sizeof(MM_SVC_TIME_TO_SIZE_T);
    
    if (pu4Size == NULL)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return e_ret_code;
    }
    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }
        
        t_timw2size_info.ui4_ms = u4Time;
        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_TYPE_TIME_TO_SIZE,
                        (VOID*)&t_timw2size_info,
                        z_size
                        );
                        
        if(SVCTXR_OK != i4_ret)
        {
            CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_GET_INFO() returns %d\n", i4_ret));
            break;
        }
        
        *pu4Size = t_timw2size_info.ui4_size;

        e_ret_code = IMTK_PB_ERROR_CODE_OK;
        
    } while(0);

    
    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }
    
    return e_ret_code;
        
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetPlayParam(IMTK_PB_HANDLE_T            hHandle,
                                              IMTK_PB_CTRL_SETTING_T*     ptSetting)
{
    CMPB_CTRL_INST_T* pt_cmpb_inst;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (pt_cmpb_inst)
    {
        ptSetting->u2ATrack = pt_cmpb_inst->u_media.t_get_info.u2AudioTrackNum;
        ptSetting->eSpeed = pt_cmpb_inst->e_cur_speed;
        return IMTK_PB_ERROR_CODE_OK;
    }
    
    return IMTK_PB_ERROR_CODE_NOT_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetAudTrackInfo(IMTK_PB_HANDLE_T                    hHandle,
                                                 uint32_t                            u4TrackNum,
                                                 IMTK_PB_CTRL_GET_AUD_TRACK_INFO_T*  ptInfo)
{
    CMPB_CTRL_INST_T* pt_cmpb_inst;

    if (u4TrackNum >= MAX_GET_AUD_NUM)
        return IMTK_PB_ERROR_CODE_NOT_OK;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (pt_cmpb_inst)
    {
        CMPB_MEMCPY(ptInfo,
                 &(pt_cmpb_inst->t_audio_info.at_get_info[u4TrackNum]),
                 sizeof(IMTK_PB_CTRL_GET_AUD_TRACK_INFO_T)
                 );                   
        return IMTK_PB_ERROR_CODE_OK;
    }
    
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetSubTrackInfo(IMTK_PB_HANDLE_T                    hHandle,
                                                 uint32_t                            u4TrackNum,
                                                 IMTK_PB_CTRL_SUBTITLE_INFO_T*      ptInfo)
{
    CMPB_CTRL_INST_T* pt_cmpb_inst;

    if (u4TrackNum >= MAX_GET_SUB_NUM)
        return IMTK_PB_ERROR_CODE_NOT_OK;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (pt_cmpb_inst)
    {
        CMPB_MEMCPY(ptInfo,
                 &(pt_cmpb_inst->at_sub_tracks[u4TrackNum]),
                     sizeof(IMTK_PB_CTRL_SUBTITLE_INFO_T));                   
        return IMTK_PB_ERROR_CODE_OK;
    }
    
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetVidTrackInfo(IMTK_PB_HANDLE_T                    hHandle,
                                                 IMTK_PB_CTRL_GET_VID_TRACK_INFO_T*  ptInfo)
{
    CMPB_CTRL_INST_T* pt_cmpb_inst;
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (pt_cmpb_inst)
    {
        CMPB_MEMCPY(ptInfo, 
                 &(pt_cmpb_inst->u_vid_track.t_get_info[0]), 
                 sizeof(IMTK_PB_CTRL_GET_VID_TRACK_INFO_T)
                 );  
        return IMTK_PB_ERROR_CODE_OK;
    }
    
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetCurAudTrackInfo(IMTK_PB_HANDLE_T                   hHandle, 
                                                          IMTK_PB_CTRL_GET_AUD_TRACK_INFO_T*  ptInfo)
{
    CMPB_CTRL_INST_T*       pt_cmpb_inst;
    INT32                   i4_ret;
    IMTK_PB_ERROR_CODE_T    e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;

    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL== pt_cmpb_inst || NULL == ptInfo)
    {
        return e_ret_code;
    }
    
    do
    {        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        if (pt_cmpb_inst->e_cur_state == CMPB_CTRL_PAUSED || 
            pt_cmpb_inst->e_cur_state == CMPB_CTRL_PLAYING)
        {
            e_ret_code = IMtkPb_Ctrl_GetAudTrackInfo(hHandle, pt_cmpb_inst->u2_cur_aud_trk, ptInfo);
        }

    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
    }
    
    return e_ret_code;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetCurVidTrackInfo(IMTK_PB_HANDLE_T                    hHandle,
                                                          IMTK_PB_CTRL_GET_VID_TRACK_INFO_T*  ptInfo)
{
    CMPB_CTRL_INST_T*       pt_cmpb_inst;
    INT32                   i4_ret;
    IMTK_PB_ERROR_CODE_T    e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;

    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL== pt_cmpb_inst || NULL == ptInfo)
    {
        return e_ret_code;
    }
    
    do
    {        
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        if (pt_cmpb_inst->e_cur_state == CMPB_CTRL_PAUSED || 
            pt_cmpb_inst->e_cur_state == CMPB_CTRL_PLAYING)
        {
            e_ret_code = IMtkPb_Ctrl_GetVidTrackInfo(hHandle, ptInfo);
        }

    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {
    }
    
    return e_ret_code;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetCurrentPos(IMTK_PB_HANDLE_T  hHandle,
                                               uint32_t*         pu4CurTime,   
                                               uint64_t*         pu8CurPos)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

	if (!pt_cmpb_inst->fg_play_normal)
	{
		return e_ret_code;
	}
	
    if (NULL == pt_cmpb_inst)
    {
        return e_ret_code;
    }

    e_ret_code = IMtkPb_Ctrl_GetCurrentPos_inr(pt_cmpb_inst, (UINT32*)pu4CurTime, (UINT64*)pu8CurPos);
    
    return e_ret_code;
}

static IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetCurrentPos_inr(CMPB_CTRL_INST_T* pt_cmpb_inst,
                                                                UINT32*         pu4CurTime,   
                                                                UINT64*         pu8CurPos)
{
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    MM_SVC_TIME_CODE_INFO_T     t_tc_info={MM_SVC_TM_CODE_TYPE_ALL};
    MM_SVC_POS_INFO_T           t_pos_info={MM_SVC_POS_TYPE_ALL_OFFSET};
    SIZE_T                      z_size = 0;
    INT32                       i4_ret = 0;

    if (NULL == pt_cmpb_inst)
    {
        return e_ret_code;
    }
    
    if(pt_cmpb_inst->fg_during_seek)
    {
        return e_ret_code;
    }
    
    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        /* get current position */
        
        t_pos_info.t_pos_type = MM_SVC_POS_TYPE_ALL_OFFSET;
        t_pos_info.ui2_idx = (UINT16)0;
        z_size = sizeof(MM_SVC_POS_INFO_T);
        
        i4_ret = CMPB_SVCTX_GET(
                        pt_cmpb_inst->h_svctx_hdl,
                        SVCTX_MM_GET_TYPE_POSIT_INFO,
                        (VOID*)&t_pos_info,
                        &z_size
                        );
        if(SVCTXR_OK != i4_ret)
        {
            CMPB_DBG_ERROR(("CMPB_SVCTX_GET() returns %d\n", i4_ret));
            *pu8CurPos = 0;
        }
        else
        {
            *pu8CurPos = t_pos_info.ui8_pos_val;
        }

        /* get current time */
        t_tc_info.t_tm_code_type = MM_SVC_TM_CODE_TYPE_ALL;
        t_tc_info.ui2_idx = (UINT16)0;
        z_size = sizeof(MM_SVC_TIME_CODE_INFO_T);
        i4_ret = CMPB_SVCTX_GET(
                        pt_cmpb_inst->h_svctx_hdl,
                        SVCTX_MM_GET_TYPE_TIME_CODE_MS,
                        (VOID*)&t_tc_info,
                        &z_size
                        );
        if(SVCTXR_OK != i4_ret)
        {
            CMPB_DBG_ERROR(("CMPB_SVCTX_GET() returns %d\n", i4_ret));
            *pu4CurTime = pt_cmpb_inst->ui4_last_timecode;
        }
        else
        {
            *pu4CurTime = (UINT32)(t_tc_info.ui8_time); /*in millisecond*/            
        }

        if (pt_cmpb_inst->fg_eos)
        {
            if(*pu8CurPos < pt_cmpb_inst->u_media.t_get_info.u8Size)
            {
                *pu8CurPos = pt_cmpb_inst->u_media.t_get_info.u8Size;
            }

            if(*pu4CurTime < pt_cmpb_inst->u_media.t_get_info.u4TotalDuration)
            {
                *pu4CurTime = pt_cmpb_inst->u_media.t_get_info.u4TotalDuration;
            }
        }
        
        /*if(pt_cmpb_inst->ui4_last_timecode == *pu4CurTime &&
           *pu4CurTime != 0)
        {            
            e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
        }
        else*/
        {
            pt_cmpb_inst->ui4_last_timecode = *pu4CurTime;
            e_ret_code = IMTK_PB_ERROR_CODE_OK;
        }
    } while(0);
        
    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }
    
    return e_ret_code;
}



IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetState(IMTK_PB_HANDLE_T       hHandle,
                                          IMTK_PB_CTRL_STATE_T*  ptState)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    INT32                       i4_ret = 0;    

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return e_ret_code;
    }
    
    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        if (CMPB_CTRL_TRANSIT != pt_cmpb_inst->e_cur_state)
        {
            *ptState = _cmpb_mapping_state(pt_cmpb_inst->e_cur_state);
        }
        else
        {
            *ptState = _cmpb_mapping_state(pt_cmpb_inst->e_old_state);
        }
        
        e_ret_code = IMTK_PB_ERROR_CODE_OK;
        
    } while(0);
        
    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }
    
    return e_ret_code;
}

#if 0
IMTK_PB_ERROR_CODE_T IMtkPb_Sys_Init(void)
{
    INT32 i4_ret;
    UINT8 ui1_idx;

    CMPB_MEMSET(&at_snd_inst, 0, CMPB_SND_COUNT*sizeof(CMPB_SND_INST_T));

    for (ui1_idx=0; ui1_idx<CMPB_SND_COUNT; ui1_idx++)
    {
        i4_ret = CMPB_SEMA_CREATE(
                                &(at_snd_inst[ui1_idx].h_sema),
                                X_SEMA_TYPE_BINARY,
                                X_SEMA_STATE_UNLOCK
                                );
        if(OSR_OK != i4_ret)
        {
            break;
        }
    }

    if (ui1_idx<CMPB_SND_COUNT)
    {
        for (ui1_idx=0; ui1_idx<CMPB_SND_COUNT; ui1_idx++)
        {
            if (at_snd_inst[ui1_idx].h_sema != NULL_HANDLE)
            {
                CMPB_SEMA_DELETE(at_snd_inst[ui1_idx].h_sema);
                at_snd_inst[ui1_idx].h_sema = NULL_HANDLE;
            }
        }

        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Sys_Terminate(void)
{
    UINT8 ui1_idx;

    for (ui1_idx=0; ui1_idx<CMPB_SND_COUNT; ui1_idx++)
    {
        if (at_snd_inst[ui1_idx].h_sema != NULL_HANDLE)
        {
            CMPB_SEMA_DELETE(at_snd_inst[ui1_idx].h_sema);
            at_snd_inst[ui1_idx].h_sema = NULL_HANDLE;
        }
    }

    return IMTK_PB_ERROR_CODE_OK;
}
#endif

/*add new APIs*/
IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GenerateP0Header(uint8_t* pu1PktHdrBuf, IMTK_PB_CTRL_P0_PKT_INFO_T*  ptPktInfo)
{
    UCHAR uc_cnt = 0;
    UCHAR uc_strm_type;
    UCHAR uc_ppr;
    UCHAR uc_ps;
    
    if(NULL == pu1PktHdrBuf || NULL == ptPktInfo)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    CMPB_MEMSET(pu1PktHdrBuf, 0, 18);
    
    /*start code  0 -- 3 bytes*/
    pu1PktHdrBuf[uc_cnt ++] = 0x3E;
    pu1PktHdrBuf[uc_cnt ++] = 0xD1;
    pu1PktHdrBuf[uc_cnt ++] = 0xA7;
    pu1PktHdrBuf[uc_cnt ++] = 0xE4;

    /*pts 4 -- 11 bytes*/
    pu1PktHdrBuf[uc_cnt ++] = (UCHAR)ptPktInfo->u8Pts;
    pu1PktHdrBuf[uc_cnt ++] = (UCHAR)(ptPktInfo->u8Pts >> 8);
    pu1PktHdrBuf[uc_cnt ++] = (UCHAR)(ptPktInfo->u8Pts >> 16);
    pu1PktHdrBuf[uc_cnt ++] = (UCHAR)(ptPktInfo->u8Pts >> 24);
    pu1PktHdrBuf[uc_cnt ++] = (UCHAR)(ptPktInfo->u8Pts >> 32);
    pu1PktHdrBuf[uc_cnt ++] = (UCHAR)(ptPktInfo->u8Pts >> 40);
    pu1PktHdrBuf[uc_cnt ++] = (UCHAR)(ptPktInfo->u8Pts >> 48);
    pu1PktHdrBuf[uc_cnt ++] = (UCHAR)(ptPktInfo->u8Pts >> 56);

    /*payload size  12 -- 15 bytes*/
    pu1PktHdrBuf[uc_cnt ++] = (UCHAR)ptPktInfo->u4PayloadSz;
    pu1PktHdrBuf[uc_cnt ++] = (UCHAR)(ptPktInfo->u4PayloadSz >> 8);
    pu1PktHdrBuf[uc_cnt ++] = (UCHAR)(ptPktInfo->u4PayloadSz >> 16);
    pu1PktHdrBuf[uc_cnt ++] = (UCHAR)(ptPktInfo->u4PayloadSz >> 24);

    switch(ptPktInfo->eStreamType)
    {
    case IMTK_PB_CTRL_STREAM_TYPE_AUDIO:
        uc_strm_type = 2;
        break;
    case IMTK_PB_CTRL_STREAM_TYPE_VIDEO:
        uc_strm_type = 1;
        break;
    default:
        uc_strm_type = 0;
        break;
    }

    switch(ptPktInfo->ePPR)
    {
    case IMTK_PB_CTRL_P0_PPR_TYPE_FREE:
        uc_ppr = 0;
        break;
    case IMTK_PB_CTRL_P0_PPR_TYPE_OPPP:
        uc_ppr = 1;
        break;
    case IMTK_PB_CTRL_P0_PPR_TYPE_SEGMENT:
        uc_ppr = 2;
        break;
    default:
        uc_ppr = 0;
        break;
    }

    switch(ptPktInfo->ePS)
    {
    case IMTK_PB_CTRL_P0_PS_TYPE_FIRST:
        uc_ps = 0;
        break;
    case IMTK_PB_CTRL_P0_PS_TYPE_INTERMEDIATE:
        uc_ps = 1;
        break;
    case IMTK_PB_CTRL_P0_PS_TYPE_LAST:
        uc_ps = 2;
        break;
    default:
        uc_ps = 0;
        break;
    }

    pu1PktHdrBuf[uc_cnt] = (uc_ps << 5) | (uc_ppr << 3) | uc_strm_type;

    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GenerateRVPicHdr(uint8_t* pu1RVPicHdr, IMTK_PB_CTRL_P0_RV_PIC_INFO_T* ptRvPicInfo)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetCurrentPTS(IMTK_PB_HANDLE_T hHandle, uint64_t* pu8CurVidPTS, uint64_t* pu8CurAudPTS)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    INT32                       i4_ret = 0;
    MM_SVC_PTS_INFO_T           t_pts_info;
    SIZE_T                      z_size = 0;

    if(NULL == pu8CurVidPTS && NULL == pu8CurAudPTS)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return e_ret_code;
    }

    if(pu8CurVidPTS)
    {
        *pu8CurVidPTS = 0;
    }

    if(pu8CurAudPTS)
    {
        *pu8CurAudPTS = 0;
    }
    
    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }

        /* get current pts */
        z_size = sizeof(MM_SVC_PTS_INFO_T);
        i4_ret = CMPB_SVCTX_GET(
                        pt_cmpb_inst->h_svctx_hdl,
                        SVCTX_MM_GET_TYPE_PTS,
                        (VOID*)&t_pts_info,
                        &z_size
                        );
        if(SVCTXR_OK != i4_ret)
        {
            CMPB_DBG_ERROR(("CMPB_SVCTX_GET() returns %d\n", i4_ret));
        }
        else
        {
            if(pu8CurVidPTS)
            {
                *pu8CurVidPTS = t_pts_info.ui8_vid_pts;
            }

            if(pu8CurAudPTS)
            {
                *pu8CurAudPTS = t_pts_info.ui8_aud_pts;
            }

            e_ret_code = IMTK_PB_ERROR_CODE_OK;
        }       
    } while(0);

    
    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }
    
    return e_ret_code;    
    
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetExtraInfo(IMTK_PB_HANDLE_T hHandle, IMTK_PB_CTRL_SET_EXTRA_INFO_T* ptExtraInfo)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    INT32                       i4_ret = 0;

    if(NULL == ptExtraInfo)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return e_ret_code;
    }

    if(ptExtraInfo->eExtraInfoType != IMTK_PB_CTRL_EXTRA_INFO_TYPE_3D_VIDEO)
    {   /*FIX ME: TEMPLE SOLUTION*/
        return e_ret_code;
    }

    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 

        if(OSR_OK != i4_ret)
        {
            break;
        }

        pt_cmpb_inst->t_ext_info = *ptExtraInfo;        

        e_ret_code = IMTK_PB_ERROR_CODE_OK;

    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  

    if(OSR_OK != i4_ret)
    {
    }

    return e_ret_code;

}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetBufferStatus_Ext(
                                     IMTK_PB_HANDLE_T                   hHandle, 
                                     IMTK_PB_CTRL_BUFFER_STATUS_EXT_T*  ptBufferStatus)
{
    CMPB_CTRL_INST_T*                   pt_cmpb_inst = NULL;

    if(ptBufferStatus == NULL)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }   

    ptBufferStatus->u4BufBeginTime  = 0;/* Not Ready */
    ptBufferStatus->u4BufEndTime    = 0;/* Not Ready */
    ptBufferStatus->u4BufRemainDur  = 0;/* Not Ready */
    if((pt_cmpb_inst->e_buf_status == IMTK_PB_CTRL_PUSH_BUF_HIGH) || 
        (pt_cmpb_inst->e_buf_status == IMTK_PB_CTRL_PUSH_BUF_OVERFLOW))
    {
        ptBufferStatus->fgBufFull = TRUE;
    }
    else
    {
        ptBufferStatus->fgBufFull = FALSE;
    }
    ptBufferStatus->e_buf_status = pt_cmpb_inst->e_buf_status;
    
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_ClearVideo(IMTK_PB_HANDLE_T hHandle)
{
    _set_video_freeze(FALSE);
    return IMTK_PB_ERROR_CODE_OK;
}


/*****************add for extended runction***************************/
IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_EncPb(IMTK_ENCODER_HANDLE_T*        phHandle,
                                                IMTK_PB_HANDLE_T              hPbHandle,
                                                IMTK_PB_CTRL_STREAM_TYPE      eStreamType,
                                                IMTK_PB_CTRL_EXT_AV_CODEC_T*  ptCodec,
                                                uint8_t*                      pu1Profile)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_EncMixer(IMTK_ENCODER_HANDLE_T*         phHandle,
                                                   IMTK_MIXER_HANDLE_T            hMixHandle,
                                                   IMTK_PB_CTRL_EXT_AV_CODEC_T*   ptCodec,
                                                   uint8_t*                       pu1Profile)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_Mux(IMTK_MUXER_HANDLE_T*        phHandle,
                                             IMTK_ENCODER_HANDLE_T       hEncHandle1,
                                             IMTK_ENCODER_HANDLE_T       hEncHandle2,
                                             IMTK_PB_CTRL_MEDIA_TYPE_T*  ptMedia,
                                             uint8_t*                    pu1Profile)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_Mix(IMTK_MIXER_HANDLE_T*            phHandle,
                                            IMTK_PB_HANDLE_T                hPbHandle1,
                                            IMTK_PB_HANDLE_T                hPbHandle2,
                                            IMTK_PB_CTRL_STREAM_TYPE        eStreamType,
                                            IMTK_PB_CTRL_EXT_AVMIX_INFO_T*  ptInfo,
                                            uint8_t*                        pu1Profile)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetEncBufferParm(IMTK_ENCODER_HANDLE_T          hHandle,
                                                             IMtkPb_Ctrl_Ext_Enc_Buf_Cb_Fct     pfnCb,
                                                             void*                          pvAppCbTag)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_FreeEncCbBuffer(IMTK_ENCODER_HANDLE_T          hHandle,
                                                            IMTK_PB_CTRL_STREAM_TYPE       eType,
                                                            uint8_t*       pui1AU)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetMuxBufferParm(IMTK_MUXER_HANDLE_T            hHandle,
                                                             IMtkPb_Ctrl_Ext_Buf_Cb_Fct     pfnCb,
                                                             void*                          pvAppCbTag)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetMixBufferParm(IMTK_MIXER_HANDLE_T            hHandle,
                                                             IMtkPb_Ctrl_Ext_Buf_Cb_Fct     pfnCb,
                                                             void*                          pvAppCbTag)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetMixAudioSink(IMTK_MIXER_HANDLE_T          hHandle,
                                                            IMTK_PB_CTRL_EXT_ASINK_T     eSink)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetMixVideoSink(IMTK_MIXER_HANDLE_T         hHandle,
                                                            IMTK_PB_CTRL_EXT_VSINK_T    eSink,
                                                            IMTK_PB_CTRL_RECT_T*        ptSrcRect,
                                                            IMTK_PB_CTRL_RECT_T*        ptDstRect)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_EncEnable(IMTK_ENCODER_HANDLE_T      hHandle,
                                                      bool fgEnable)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_RequestKeyframe(IMTK_ENCODER_HANDLE_T      hHandle)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetVidBitRate(IMTK_ENCODER_HANDLE_T     hHandle,
                                                          uint32_t                  u4Bitrate)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetVidFrameRate(IMTK_ENCODER_HANDLE_T   hHandle,
                                                            uint32_t                u4Framerate)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetVidResolution(IMTK_ENCODER_HANDLE_T      hHandle,
                                                             uint32_t                   u4X,
                                                             uint32_t                   u4Y)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetEncoderRectangle(IMTK_ENCODER_HANDLE_T      hHandle,
                                                                IMTK_PB_CTRL_RECT_T*       ptRec)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetAudBitRate(IMTK_ENCODER_HANDLE_T     hHandle,
                                                          uint32_t                  u4Bitrate)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetAudSampleRate(IMTK_ENCODER_HANDLE_T          hHandle,
                                                             IMTK_PB_CTRL_AUD_SAMPLE_RATE_T eSampleRate)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetAudChannelNumber(IMTK_ENCODER_HANDLE_T       hHandle,
                                                                IMTK_PB_CTRL_AUD_CH_NUM_T   eChNum)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_StartSnapshot(IMTK_PB_HANDLE_T  hHandle,
                                                          IMTK_PB_CTRL_EXT_SNAPSHOT_FMT_T   eFmt,
                                                          IMTK_PB_CTRL_RECT_T*      ptSrcRect,
                                                          IMTK_PB_CTRL_EXT_SNAPSHOT_BUF_INFO_T* ptBuf)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_StopSnapshot(IMTK_PB_HANDLE_T  hHandle,
                                                        IMTK_PB_CTRL_EXT_SNAPSHOT_BUF_INFO_T* ptBuf)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}


IMTK_PB_ERROR_CODE_T _IMtkPb_Ctrl_Get_Mp3_Img_Info (
                                        IMTK_PB_HANDLE_T                hHandle, 
                                        IMTK_PB_CTRL_MP3_COVER_INFO_T*  pt_mp3_img)
{
    UINT8                       ui1_i       = 0;
    INT32                       i4_ret      = 0;
    CMPB_CTRL_INST_T*           pt_cmpb_inst= NULL;
    MINFO_INFO_T                t_minfo_rec;
    IMTK_PB_ERROR_CODE_T        e_Ret       = IMTK_PB_ERROR_CODE_OK;
    MEDIA_IMG_SUBTYPE_T         e_img_type;

    
    if ((NULL_HANDLE == hHandle)        || 
        (NULL        == pt_mp3_img))
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    CMPB_MEMSET(&t_minfo_rec,  0, sizeof(MINFO_INFO_T));

    do
    {
        t_minfo_rec.e_info_type = MINFO_INFO_TYPE_ID3_DATA;
        t_minfo_rec.u.t_id3_data.pui1_img_data_buf = pt_mp3_img->pui1_img_data_buf;
        t_minfo_rec.u.t_id3_data.ui4_img_buf_len   = pt_mp3_img->ui4_length;
        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_TYPE_META_DATA,
                        (VOID*)&t_minfo_rec,
                        sizeof(MINFO_INFO_T)
                        );                        
        if(SVCTXR_OK != i4_ret)
        {
            e_Ret = IMTK_PB_ERROR_CODE_NOT_OK;
            break;
        }

        if(t_minfo_rec.u.t_id3_data.ui4_pic_bitmask == 0)
        {/*No pic exist*/
            e_Ret = IMTK_PB_ERROR_CODE_NOT_OK;
            break;
        }
        
        for(ui1_i = 0; ui1_i <= MINFO_ID3_PIC_STUDIO_LOGOTYPE; ui1_i++)
        {
            if(t_minfo_rec.u.t_id3_data.ui4_pic_bitmask & ((UINT32)(1 << ui1_i)))
            {
                break; /* use i as the index to retrieve ID3 pic data */
            }
        }
        
        if(MINFO_ID3_PIC_STUDIO_LOGOTYPE <= ui1_i)
        {
            e_Ret = IMTK_PB_ERROR_CODE_NOT_OK;
            break;
        }
        e_img_type = t_minfo_rec.u.t_id3_data.at_id3_attatched_pic[ui1_i]
                                    .t_format.t_media_subtype.u.e_img_subtype;
        if(MEDIA_IMG_SUBTYPE_PNG == e_img_type)
        {
            pt_mp3_img->e_img_type = IMTK_MP3_COVER_IMG_TYPE_PNG;
        }
        else if(MEDIA_IMG_SUBTYPE_JPG == e_img_type)
        {
            pt_mp3_img->e_img_type = IMTK_MP3_COVER_IMG_TYPE_JPG;
        }
        else
        {
            pt_mp3_img->e_img_type = IMTK_MP3_COVER_IMG_TYPE_UNKNOWN;
            break;
        }        
        pt_mp3_img->ui4_width  = t_minfo_rec.u.t_id3_data.at_id3_attatched_pic[ui1_i].ui4_width;
        pt_mp3_img->ui4_height = t_minfo_rec.u.t_id3_data.at_id3_attatched_pic[ui1_i].ui4_height;
        pt_mp3_img->ui4_length = t_minfo_rec.u.t_id3_data.at_id3_attatched_pic[ui1_i].ui4_length;
        pt_mp3_img->pui1_img_data_buf = t_minfo_rec.u.t_id3_data.pui1_img_data_buf;
    }while(0);

    return e_Ret;
}

static IMTK_PB_ERROR_CODE_T _IMtkPb_Ctrl_Get_Meta_Data (IMTK_PB_HANDLE_T                hHandle,
                                                        IMTK_PB_CTRL_META_DATA_INFO_T*  pt_meta_info,
                                                        MINFO_INFO_T*                   pt_minfo_rec)
{
    BOOL                    b_use_minfo = TRUE;
    IMTK_PB_ERROR_CODE_T    e_ret = IMTK_PB_ERROR_CODE_OK;
    INT32                   i4_ret = 0;
    CMPB_CTRL_INST_T*       pt_cmpb_inst= NULL;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if((NULL== pt_cmpb_inst) || (pt_meta_info == NULL) || (pt_minfo_rec == NULL))
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    switch(pt_meta_info->e_meta_type)
    {
    case IMTK_PB_CTRL_META_TYPE_TITLE:
        pt_minfo_rec->u.t_meta_data.pwc_str_title   = pt_meta_info->pv_buf;
        pt_minfo_rec->u.t_meta_data.z_title_len     = pt_meta_info->ui2_buf_size;
        break;
    case IMTK_PB_CTRL_META_TYPE_DIRECTOR:
        pt_minfo_rec->u.t_meta_data.pwc_str_director= pt_meta_info->pv_buf;
        pt_minfo_rec->u.t_meta_data.z_director_len  = pt_meta_info->ui2_buf_size;
        break;
    case IMTK_PB_CTRL_META_TYPE_COPYRIGHT:
        pt_minfo_rec->u.t_meta_data.pwc_str_copyright= pt_meta_info->pv_buf;
        pt_minfo_rec->u.t_meta_data.z_copyright_len = pt_meta_info->ui2_buf_size;
        break;
    case IMTK_PB_CTRL_META_TYPE_YEAR:
        pt_minfo_rec->u.t_meta_data.pwc_str_year    = pt_meta_info->pv_buf;
        pt_minfo_rec->u.t_meta_data.z_year_len      = pt_meta_info->ui2_buf_size;
        break;
    case IMTK_PB_CTRL_META_TYPE_GENRE:
        pt_minfo_rec->u.t_meta_data.pwc_str_genre   = pt_meta_info->pv_buf;
        pt_minfo_rec->u.t_meta_data.z_genre_len     = pt_meta_info->ui2_buf_size;
        break;
    case IMTK_PB_CTRL_META_TYPE_ARTIST:
        pt_minfo_rec->u.t_meta_data.pwc_str_artist  = pt_meta_info->pv_buf;
        pt_minfo_rec->u.t_meta_data.z_artist_len    = pt_meta_info->ui2_buf_size;
        break;
    case IMTK_PB_CTRL_META_TYPE_ALBUM:
        pt_minfo_rec->u.t_meta_data.pwc_str_album   = pt_meta_info->pv_buf;
        pt_minfo_rec->u.t_meta_data.z_album_len     = pt_meta_info->ui2_buf_size;
        break;
    case IMTK_PB_CTRL_META_TYPE_DURATION:
        if(pt_meta_info->ui2_buf_size >= 4)
        {
            *((UINT32*)pt_meta_info->pv_buf) = pt_cmpb_inst->u_media.t_get_info.u4TotalDuration;
        }
        break;
    case IMTK_PB_CTRL_META_TYPE_BITRATE:
        if(pt_meta_info->ui2_buf_size >= 4)
        {
            *((UINT32*)pt_meta_info->pv_buf) = pt_cmpb_inst->u_media.t_get_info.u4AvgBitrate;
        }
        break;
    case IMTK_PB_CTRL_META_TYPE_SIZE:
        if(pt_meta_info->ui2_buf_size >= 8)
        {
            *((UINT64*)pt_meta_info->pv_buf) = pt_cmpb_inst->u_media.t_get_info.u8Size;
        }
        break;
    case IMTK_PB_CTRL_META_TYPE_VIDEO_HEIGHT:
        if(pt_meta_info->ui2_buf_size >= 4)
        {
            *((UINT32*)pt_meta_info->pv_buf) = pt_cmpb_inst->u_vid_track.t_get_info[0].u4Height;
        }
        break;
    case IMTK_PB_CTRL_META_TYPE_VIDEO_WIDTH:
        if(pt_meta_info->ui2_buf_size >= 4)
        {
            *((UINT32*)pt_meta_info->pv_buf) = pt_cmpb_inst->u_vid_track.t_get_info[0].u4Width;
        }
        break;
    case IMTK_PB_CTRL_META_TYPE_NUM_TRACKS:
        if(pt_meta_info->ui2_buf_size >= 4)
        {
            *((UINT32*)pt_meta_info->pv_buf) = pt_cmpb_inst->u_media.t_get_info.u2AudioTrackNum
                                              +pt_cmpb_inst->u2VideoTrackNum
                                              +pt_cmpb_inst->u2SubtitleTrackNum;
        }
        break;
    case IMTK_PB_CTRL_META_TYPE_FRAME_RATE:
        break;
    case IMTK_PB_CTRL_META_TYPE_DATE:
    case IMTK_PB_CTRL_META_TYPE_PROTECT:
    case IMTK_PB_CTRL_META_TYPE_CREATE_TIME:
    case IMTK_PB_CTRL_META_TYPE_ACCESS_TIME:
    case IMTK_PB_CTRL_META_TYPE_MODIFY_TIME:
    case IMTK_PB_CTRL_META_TYPE_RESOLUTION:
    case IMTK_PB_CTRL_META_TYPE_NEXT_TITLE:
    case IMTK_PB_CTRL_META_TYPE_NEXT_ARTIST:
        /*b_use_minfo = FALSE;
        break;*/        
    case IMTK_PB_CTRL_META_TYPE_CD_TRACK_NUMBER:
    case IMTK_PB_CTRL_META_TYPE_AUTHOR:
    case IMTK_PB_CTRL_META_TYPE_COMPOSER:
    case IMTK_PB_CTRL_META_TYPE_IS_DRM_CRIPPLED:
    case IMTK_PB_CTRL_META_TYPE_CODEC:
    case IMTK_PB_CTRL_META_TYPE_RATING:
    case IMTK_PB_CTRL_META_TYPE_COMMENT:
    case IMTK_PB_CTRL_META_TYPE_VIDEO_FORMAT:
    case IMTK_PB_CTRL_META_TYPE_WRITER:
    case IMTK_PB_CTRL_META_TYPE_MIMETYPE:
    case IMTK_PB_CTRL_META_TYPE_DISCNUMBER:
    case IMTK_PB_CTRL_META_TYPE_ALBUMARTIST:
    case IMTK_PB_CTRL_META_TYPE_INVAL:
    default:
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    if(b_use_minfo)
    {
        pt_minfo_rec->e_info_type = MINFO_INFO_TYPE_META_DATA;
        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_TYPE_META_DATA,
                        (VOID*)pt_minfo_rec,
                        sizeof(MINFO_INFO_T)
                        );                        
        if(SVCTXR_OK == i4_ret)
        {
            switch(pt_meta_info->e_meta_type)
            {
                case IMTK_PB_CTRL_META_TYPE_DURATION:
                    if(pt_meta_info->ui2_buf_size == 4)
                    {
                        *((UINT32*)pt_meta_info->pv_buf)
                                        = (UINT32)pt_minfo_rec->u.t_meta_data.ui8_pb_duration * 1000 /*ms*/;
                    }
                    break;
                case IMTK_PB_CTRL_META_TYPE_BITRATE:
                    if(pt_meta_info->ui2_buf_size == 4)
                    {
                        *((UINT32*)pt_meta_info->pv_buf)
                                        = pt_minfo_rec->u.t_meta_data.ui4_bitrate;
                    }
                    break;
                case IMTK_PB_CTRL_META_TYPE_FRAME_RATE:
                    if(pt_meta_info->ui2_buf_size >= sizeof(MINFO_TYPE_FRAME_RATE_T))
                    {
                        UINT32 *pTemp = (UINT32 *)pt_meta_info->pv_buf;
                        *pTemp      = pt_minfo_rec->u.t_meta_data.t_frame_rate.ui4_rate;
                        *(pTemp+1)  = pt_minfo_rec->u.t_meta_data.t_frame_rate.ui4_scale;
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
            e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
            /*meta data maybe not exist*/
        }
    }

    return e_ret;
}
IMTK_PB_ERROR_CODE_T _IMtkPb_Ctrl_Get_3D_video_info(IMTK_PB_HANDLE_T                    hHandle,
                                                              IMTK_PB_CTRL_3D_VIDEO_INFO_T*        pt3DVideoInfo)
{ 
    INT32                           i4_ret       = 0;
    CMPB_CTRL_INST_T*               pt_cmpb_inst = NULL;
    MM_3D_VID_INFO_T                t_3d_vid_info;
    IMTK_PB_CTRL_3D_VIDEO_INFO_T*   pt_3d_video_info = pt3DVideoInfo;

    if (pt_3d_video_info == NULL)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
            
    CMPB_MEMSET(&t_3d_vid_info, 0, sizeof(MM_3D_VID_INFO_T));

    i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                pt_cmpb_inst->h_svctx_hdl,
                pt_cmpb_inst->h_media_hdl,
                SVCTX_MM_GET_TYPE_3D_VID_INFO,
                &t_3d_vid_info,
                sizeof(MM_3D_VID_INFO_T)
                );
    if(i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    switch(t_3d_vid_info.e_tag3d_type)
    {
    case MM_SRC_TAG3D_MVC:
        pt_3d_video_info->eTag3dType = IMTK_PB_CTRL_TAG3D_MVC;
        break;
    case MM_SRC_TAG3D_FP:
        pt_3d_video_info->eTag3dType = IMTK_PB_CTRL_TAG3D_FP;
        break;
    case MM_SRC_TAG3D_FS:
        pt_3d_video_info->eTag3dType = IMTK_PB_CTRL_TAG3D_FS;
        break;
    case MM_SRC_TAG3D_TB:
        pt_3d_video_info->eTag3dType = IMTK_PB_CTRL_TAG3D_TB;
        break;
    case MM_SRC_TAG3D_SBS:
        pt_3d_video_info->eTag3dType = IMTK_PB_CTRL_TAG3D_SBS;
        break;
    case MM_SRC_TAG3D_REALD:
        pt_3d_video_info->eTag3dType = IMTK_PB_CTRL_TAG3D_REALD;
        break;
    case MM_SRC_TAG3D_SENSIO:
        pt_3d_video_info->eTag3dType = IMTK_PB_CTRL_TAG3D_SENSIO;
        break;
    case MM_SRC_TAG3D_TTDO:
        pt_3d_video_info->eTag3dType = IMTK_PB_CTRL_TAG3D_TTDO;
        break;
    case MM_SRC_TAG3D_2D:
        pt_3d_video_info->eTag3dType = IMTK_PB_CTRL_TAG3D_2D;
        break;
    case MM_SRC_TAG3D_NOT_SUPPORT:    
    default:
        pt_3d_video_info->eTag3dType = IMTK_PB_CTRL_TAG3D_NOT_SUPPORT;
        break;
    }

    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T _IMtkPb_Ctrl_Set_Sbtl_Attr(IMTK_PB_HANDLE_T            hHandle,
                                                IMTK_PB_CTRL_SBTL_ATTR *         ptSbtlAttr)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst;
    IMTK_PB_CTRL_SBTL_ATTR*     pt_sbtl_attr        = ptSbtlAttr;
    MM_SVC_SBTL_DISP_ATTR_T     t_nm_attr;
    size_t                      z_mm_info_size      = 0;
    bool                        b_normal_attr       = FALSE;
    INT32                       i4_ret = 0;

    if (pt_sbtl_attr == NULL)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    c_memset(&t_nm_attr,   0, sizeof(MM_SVC_SBTL_DISP_ATTR_T));

    while (pt_sbtl_attr != NULL)
    {
        switch (pt_sbtl_attr->eSbtlAttrType)
        {
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_DISP_MODE:
            {
                IMTK_PB_CTRL_SBTL_DISP_MODE_T*  pt_disp_mode = &pt_sbtl_attr->uAttrValue.tDispMode;
                MM_SVC_SBTL_DISP_MODE_T         t_disp_mode;

                c_memset(&t_disp_mode, 0, sizeof(MM_SVC_SBTL_DISP_MODE_T));

                switch(pt_disp_mode->eDispMode)
                {
                    case IMTK_PB_CTRL_SBTL_DISP_MODE_OFF:
                        t_disp_mode.e_disp_mode = MM_SBTL_DISP_MODE_OFF;
                        break;
                    case IMTK_PB_CTRL_SBTL_DISP_MODE_SINGLE_LINE:
                        t_disp_mode.e_disp_mode = MM_SBTL_DISP_MODE_SINGLE_LINE;
                        break;
                    case IMTK_PB_CTRL_SBTL_DISP_MODE_MULTI_LINE:
                        t_disp_mode.e_disp_mode = MM_SBTL_DISP_MODE_MULTI_LINE;
                        t_disp_mode.ui2_param   = pt_disp_mode->u2Param;
                        break;
                    default:
                        return IMTK_PB_ERROR_CODE_NOT_OK;
                }

                z_mm_info_size   = sizeof(MM_SVC_SBTL_DISP_MODE_T);

                if (pt_cmpb_inst->t_subtitle_info.ui2_sel_idx == 0/*subtitle off*/)
                {
                    i4_ret = CMPB_SVCTX_GENERIC_SET_INFO(pt_cmpb_inst->h_svctx_hdl,
                                                NULL_HANDLE,
                                                SVCTX_MM_SBTL_SET_TYPE_DISP_MODE,
                                                &t_disp_mode,
                                                z_mm_info_size);
                }
                else
                {
                    i4_ret = CMPB_SVCTX_SET( pt_cmpb_inst->h_svctx_hdl,
                                    SVCTX_MM_SBTL_SET_TYPE_DISP_MODE,
                                    &t_disp_mode,
                                    z_mm_info_size);
                }
            }
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_HILT_STL:
            {
                IMTK_PB_CTRL_SBTL_HILT_STL_T* pt_hilt_stl = &pt_sbtl_attr->uAttrValue.tHiltStl;
                MM_SVC_SBTL_HILT_STL_T        t_hlt_stl;

                c_memset(&t_hlt_stl,   0, sizeof(MM_SVC_SBTL_HILT_STL_T));
                
                switch(pt_hilt_stl->eHltStyle)
                {
                case IMTK_PB_CTRL_SBTL_HILT_STL_BY_LINE:
                    t_hlt_stl.e_hlt_style = MM_SBTL_HILT_STL_BY_LINE;
                    break;
                case IMTK_PB_CTRL_SBTL_HILT_STL_KARAOKE:
                    t_hlt_stl.e_hlt_style = MM_SBTL_HILT_STL_KARAOKE;
                    t_hlt_stl.ui2_param    = pt_hilt_stl->u2Param;
                    break;
                default:
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }

                z_mm_info_size   = sizeof(MM_SVC_SBTL_HILT_STL_T);

                if (pt_cmpb_inst->t_subtitle_info.ui2_sel_idx == 0/*subtitle off*/)
                {
                    i4_ret = CMPB_SVCTX_GENERIC_SET_INFO(pt_cmpb_inst->h_svctx_hdl,
                                                NULL_HANDLE,
                                                SVCTX_MM_SBTL_SET_TYPE_HILT_STL,
                                                &t_hlt_stl,
                                                z_mm_info_size);
                }
                else
                {
                    i4_ret = CMPB_SVCTX_SET( pt_cmpb_inst->h_svctx_hdl,
                                    SVCTX_MM_SBTL_SET_TYPE_HILT_STL,
                                    &t_hlt_stl,
                                    z_mm_info_size);
                }
                
            }
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_TIME_OFST:
            {
                IMTK_PB_CTRL_SBTL_TIME_OFST_T* pt_tm_ofst = &pt_sbtl_attr->uAttrValue.tTmOfst;
                MM_SVC_SBTL_TIME_OFST_T        t_tm_ofst;

                c_memset(&t_tm_ofst,   0, sizeof(MM_SVC_SBTL_TIME_OFST_T));
   
                switch(pt_tm_ofst->eTimeOfst)
                {
                case IMTK_PB_CTRL_SBTL_TIME_OFST_OFF:
                    t_tm_ofst.e_time_ofst = MM_SBTL_TIME_OFST_OFF;
                    break;
                case IMTK_PB_CTRL_SBTL_TIME_OFST_AUTO:
                    t_tm_ofst.e_time_ofst = MM_SBTL_TIME_OFST_AUTO;
                    break;
                case IMTK_PB_CTRL_SBTL_TIME_OFST_USER_DEF:
                    t_tm_ofst.e_time_ofst = MM_SBTL_TIME_OFST_USER_DEF;
                    break;
                default:
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }
                t_tm_ofst.ui4_ofst_value = pt_tm_ofst->u4OfstValue;
                z_mm_info_size   = sizeof(MM_SVC_SBTL_TIME_OFST_T);
                
                if (pt_cmpb_inst->t_subtitle_info.ui2_sel_idx == 0/*subtitle off*/)
                {
                    CMPB_SVCTX_GENERIC_SET_INFO(pt_cmpb_inst->h_svctx_hdl,
                                                NULL_HANDLE,
                                                SVCTX_MM_SBTL_SET_TYPE_TIME_OFST,
                                                &t_tm_ofst,
                                                z_mm_info_size);
                }
                else
                {
                    CMPB_SVCTX_SET( pt_cmpb_inst->h_svctx_hdl,
                                    SVCTX_MM_SBTL_SET_TYPE_TIME_OFST,
                                    &t_tm_ofst,
                                    z_mm_info_size);
                }
            }
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_FONT_ENC:
            {
                IMTK_PB_CTRL_SBTL_FONT_ENC_T* pt_font_enc = &pt_sbtl_attr->uAttrValue.tFontEnc;
                MM_SVC_SBTL_FONT_ENC_T        t_fnt_enc;

                c_memset(&t_fnt_enc,   0, sizeof(MM_SVC_SBTL_FONT_ENC_T));
                
                switch(pt_font_enc->eEncType)
                {
                case IMTK_PB_CTRL_SBTL_FONT_ENC_AUTO:
                    t_fnt_enc.e_enc_type = MM_SBTL_FONT_ENC_AUTO;
                    break;
                case IMTK_PB_CTRL_SBTL_FONT_ENC_UTF8:
                    t_fnt_enc.e_enc_type = MM_SBTL_FONT_ENC_UTF8;
                    break;
                case IMTK_PB_CTRL_SBTL_FONT_ENC_UTF16:
                    t_fnt_enc.e_enc_type = MM_SBTL_FONT_ENC_UTF16;
                    break;
                case IMTK_PB_CTRL_SBTL_FONT_ENC_BIG5:
                    t_fnt_enc.e_enc_type = MM_SBTL_FONT_ENC_BIG5;
                    break;
                case IMTK_PB_CTRL_SBTL_FONT_ENC_GB:
                    t_fnt_enc.e_enc_type = MM_SBTL_FONT_ENC_GB;
                    break;
                default:
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }
                t_fnt_enc.ui2_param = pt_font_enc->u2Param;
                z_mm_info_size   = sizeof(MM_SVC_SBTL_FONT_ENC_T);

                if (pt_cmpb_inst->t_subtitle_info.ui2_sel_idx == 0/*subtitle off*/)
                {
                    CMPB_SVCTX_GENERIC_SET_INFO(pt_cmpb_inst->h_svctx_hdl,
                                                NULL_HANDLE,
                                                SVCTX_MM_SBTL_SET_TYPE_FONT_ENC,
                                                &t_fnt_enc,
                                                z_mm_info_size);
                }
                else
                {
                    CMPB_SVCTX_SET( pt_cmpb_inst->h_svctx_hdl,
                                    SVCTX_MM_SBTL_SET_TYPE_FONT_ENC,
                                    &t_fnt_enc,
                                    z_mm_info_size);
                }
            }
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_HILT_ATTR:
            {
                /*Do Nothing*/
            }
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_SHOW_HIDE:
            {
                IMTK_PB_CTRL_SBTL_SHOW_HIDE_T*  pt_show_hide = &pt_sbtl_attr->uAttrValue.tShowHide;
                MM_SVC_SBTL_SHOW_HIDE_T         t_show_hide;
                
                c_memset(&t_show_hide, 0, sizeof(MM_SVC_SBTL_SHOW_HIDE_T));
                t_show_hide.b_sbtl_show = pt_show_hide->bSbtlShow;
                z_mm_info_size   = sizeof(MM_SVC_SBTL_SHOW_HIDE_T);

                if (pt_cmpb_inst->t_subtitle_info.ui2_sel_idx == 0/*subtitle off*/)
                {
                    CMPB_SVCTX_GENERIC_SET_INFO(pt_cmpb_inst->h_svctx_hdl,
                                                NULL_HANDLE,
                                                SVCTX_MM_SBTL_SET_TYPE_SHOW_HIDE,
                                                &t_show_hide,
                                                z_mm_info_size);
                }
                else
                {
                    CMPB_SVCTX_SET( pt_cmpb_inst->h_svctx_hdl,
                                    SVCTX_MM_SBTL_SET_TYPE_SHOW_HIDE,
                                    &t_show_hide,
                                    z_mm_info_size);
                }
            }
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_FNT_INFO :
            {
                IMTK_PB_CTRL_FONT_INFO_T* pt_fnt_info = &pt_sbtl_attr->uAttrValue.tFntInfo;
                
                switch(pt_fnt_info->eFontCmap)
                {
                    case IMTK_PB_CTRL_CMAP_ENC_MS_SYMBOL:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_MS_SYMBOL;
                        break;
                    case IMTK_PB_CTRL_CMAP_ENC_UNICODE:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_UNICODE;
                        break;    
                    case IMTK_PB_CTRL_CMAP_ENC_SJIS:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_SJIS;
                        break;
                    case IMTK_PB_CTRL_CMAP_ENC_GB2312:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_GB2312;
                        break;
                    case IMTK_PB_CTRL_CMAP_ENC_BIG5:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_BIG5;
                        break;
                    case IMTK_PB_CTRL_CMAP_ENC_WANSUNG:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_WANSUNG;
                        break;
                    case IMTK_PB_CTRL_CMAP_ENC_JOHAB:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_JOHAB;
                        break;    
                    case IMTK_PB_CTRL_CMAP_ENC_ADOBE_STANDARD:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_ADOBE_STANDARD;
                        break;      
                    case IMTK_PB_CTRL_CMAP_ENC_ADOBE_EXPERT:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_ADOBE_EXPERT;
                        break;  
                    case IMTK_PB_CTRL_CMAP_ENC_ADOBE_CUSTOM:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_ADOBE_CUSTOM;
                        break;  
                    case IMTK_PB_CTRL_CMAP_ENC_ADOBE_LATIN_1:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_ADOBE_LATIN_1;
                        break;  
                    case IMTK_PB_CTRL_CMAP_ENC_OLD_LATIN_2:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_OLD_LATIN_2;
                        break;  
                    case IMTK_PB_CTRL_CMAP_ENC_APPLE_ROMAN:   
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_APPLE_ROMAN;
                        break;
                    case IMTK_PB_CTRL_CMAP_ENC_NONE:
                    default:
                        t_nm_attr.t_font_info.e_font_cmap = FE_CMAP_ENC_NONE;
                        break;
                }
                switch(pt_fnt_info->eFontSize)
                {     
                    case IMTK_PB_CTRL_FNT_SIZE_MEDIUM:
                        t_nm_attr.t_font_info.e_font_size = FE_FNT_SIZE_MEDIUM;
                        break;
                    case IMTK_PB_CTRL_FNT_SIZE_LARGE:
                        t_nm_attr.t_font_info.e_font_size = FE_FNT_SIZE_LARGE;
                        break;
                    case IMTK_PB_CTRL_FNT_SIZE_CUSTOM:
                        t_nm_attr.t_font_info.e_font_size = FE_FNT_SIZE_CUSTOM;
                        break;
                    case IMTK_PB_CTRL_FNT_SIZE_NUMBER:
                        t_nm_attr.t_font_info.e_font_size = FE_FNT_SIZE_NUMBER;
                        break;
                    case IMTK_PB_CTRL_FNT_SIZE_SMALL:
                        t_nm_attr.t_font_info.e_font_size = FE_FNT_SIZE_SMALL;
                        break;
                    default:
                        return IMTK_PB_ERROR_CODE_NOT_OK;
                }

                switch(pt_fnt_info->eFontStyle)
                {
#ifdef FE_DISABLE_EDGE_EFFECT_SUPPORT
                    case IMTK_PB_CTRL_FNT_STYLE_ITALIC:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_ITALIC;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_BOLD:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_BOLD;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_UNDERLINE:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_UNDERLINE;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_STRIKEOUT:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_STRIKEOUT;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_OUTLINE:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_OUTLINE;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_REGULAR:
                    default:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
                        break;    
#else
                    case IMTK_PB_CTRL_FNT_STYLE_ITALIC:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_ITALIC;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_BOLD:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_BOLD;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_UNDERLINE:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_UNDERLINE;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_STRIKEOUT:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_STRIKEOUT;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_OUTLINE:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_OUTLINE;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_SHADOW_RIGHT:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_SHADOW_RIGHT;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_SHADOW_LEFT:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_SHADOW_LEFT;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_DEPRESSED :
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_DEPRESSED;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_RAISED :
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_RAISED;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_UNIFORM:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_UNIFORM;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_BLURRED:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_BLURRED;
                        break;
                    case IMTK_PB_CTRL_FNT_STYLE_REGULAR:
                    default:
                        t_nm_attr.t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
                        break;
#endif
                }
                
                t_nm_attr.t_font_info.i2_width        = pt_fnt_info->i2Width;
                t_nm_attr.t_font_info.ui1_custom_size = pt_fnt_info->u1CustomSize;
                c_memcpy(t_nm_attr.t_font_info.a_c_font_name, pt_fnt_info->acFontName, IMTK_PB_CTRL_MAX_FONT_NAME);

                t_nm_attr.t_attr_field |= MM_SBTL_ATTR_FNT_INFO;
                b_normal_attr           = TRUE;
            }
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_BKG_CLR  :
            {
                c_memcpy(&t_nm_attr.t_bkg_clr, &pt_sbtl_attr->uAttrValue.tBkgClr, sizeof(GL_COLOR_T)); 
                t_nm_attr.t_attr_field |= MM_SBTL_ATTR_BKG_CLR;
                b_normal_attr           = TRUE;
            }   
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_TXT_CLR  :
            {  
                c_memcpy(&t_nm_attr.t_txt_clr, &pt_sbtl_attr->uAttrValue.tTxtClr, sizeof(GL_COLOR_T));
                t_nm_attr.t_attr_field |= MM_SBTL_ATTR_TXT_CLR;
                b_normal_attr           = TRUE;
            }
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_EDG_CLR  :
            {  
                c_memcpy(&t_nm_attr.t_edge_clr, &pt_sbtl_attr->uAttrValue.tEdgClr, sizeof(GL_COLOR_T));
                t_nm_attr.t_attr_field |= MM_SBTL_ATTR_EDG_CLR;
                b_normal_attr           = TRUE;
            }
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_BDR_TYPE :
            {
                switch(pt_sbtl_attr->uAttrValue.eBdrType)
                {
                case IMTK_PB_CTRL_SBTL_BDR_TYPE_SOLID_LINE:
                    t_nm_attr.e_bdr_type = MM_SBTL_BDR_TYPE_SOLID_LINE;
                    break;
                case IMTK_PB_CTRL_SBTL_BDR_TYPE_NULL:
                default:    
                    t_nm_attr.e_bdr_type = MM_SBTL_BDR_TYPE_NULL;
                    break;
                }
                
                t_nm_attr.t_attr_field |= MM_SBTL_ATTR_BDR_TYPE;
                b_normal_attr           = TRUE;
            }
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_BDR_WIDTH:
            {
                t_nm_attr.ui4_bdr_width = pt_sbtl_attr->uAttrValue.u4BdrWidth;
                t_nm_attr.t_attr_field |= MM_SBTL_ATTR_BDR_WIDTH;
                b_normal_attr           = TRUE;
            }
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_ROLL_TYPE:
            {
                switch(pt_sbtl_attr->uAttrValue.eRollType)
                {
                case IMTK_PB_CTRL_SBTL_ROLL_TYPE_DEF:
                default:
                    t_nm_attr.e_roll_type = MM_SBTL_ROLL_TYPE_DEF;
                    break;
                }
                t_nm_attr.t_attr_field |= MM_SBTL_ATTR_ROLL_TYPE;
                b_normal_attr           = TRUE;
            }
            break;
            
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_DISP_RECT:
            {
                IMTK_PB_CTRL_RECT_T*  pt_disp_rect= &pt_sbtl_attr->uAttrValue.tDispRect;
                
                t_nm_attr.t_disp_rect.i4_left  = pt_disp_rect->u4X;
                t_nm_attr.t_disp_rect.i4_top    = pt_disp_rect->u4Y;
                t_nm_attr.t_disp_rect.i4_right   = pt_disp_rect->u4X + pt_disp_rect->u4W;
                t_nm_attr.t_disp_rect.i4_bottom = pt_disp_rect->u4Y + pt_disp_rect->u4H;

                t_nm_attr.t_attr_field |= MM_SBTL_ATTR_DISP_RECT;
                b_normal_attr           = TRUE;
            }
            break;

            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_OSD_OFFSET:
            {
                IMTK_PB_CTRL_POINT_T*  pt_osd_offset= &pt_sbtl_attr->uAttrValue.tOsdOffset;
                
                t_nm_attr.t_osd_offset.i4_x  = pt_osd_offset->u4X;
                t_nm_attr.t_osd_offset.i4_y  = pt_osd_offset->u4Y;

                t_nm_attr.t_attr_field |= MM_SBTL_ATTR_OSD_OFFSET;
                b_normal_attr           = TRUE;
            }
            break;
            case IMTK_PB_CTRL_SBTL_ATTR_TYPE_DEFAULT:
            {
                if (pt_cmpb_inst->t_subtitle_info.ui2_sel_idx == 0/*subtitle off*/)
                {
                    CMPB_SVCTX_GENERIC_SET_INFO(pt_cmpb_inst->h_svctx_hdl,
                                                NULL_HANDLE,
                                                SVCTX_MM_SBTL_SET_TYPE_DEFAULT,
                                                NULL,
                                                0);
                }
                else
                {
                    CMPB_SVCTX_SET( pt_cmpb_inst->h_svctx_hdl,
                                    SVCTX_MM_SBTL_SET_TYPE_DEFAULT,
                                    NULL,
                                    0);
                }
            }
            break;
            
            default:
                return IMTK_PB_ERROR_CODE_NOT_OK;
        }
        pt_sbtl_attr = pt_sbtl_attr->ptNext;
    }
    
    if (b_normal_attr)
    {
        z_mm_info_size = sizeof(MM_SVC_SBTL_DISP_ATTR_T);
        if (pt_cmpb_inst->t_subtitle_info.ui2_sel_idx == 0/*subtitle off*/)
        {
            CMPB_SVCTX_GENERIC_SET_INFO(pt_cmpb_inst->h_svctx_hdl,
                                        NULL_HANDLE,
                                        SVCTX_MM_SBTL_SET_TYPE_NRML_ATTR,
                                        &t_nm_attr,
                                        z_mm_info_size);
        }
        else
        {
            CMPB_SVCTX_SET( pt_cmpb_inst->h_svctx_hdl,
                            SVCTX_MM_SBTL_SET_TYPE_NRML_ATTR,
                            &t_nm_attr,
                            z_mm_info_size);
        }
    }

    return IMTK_PB_ERROR_CODE_OK;
}

#if 0
IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetSHM(IMTK_PB_HANDLE_T    hHandle,
                                        uint32_t            ui4_shm_len,
                                        int32_t*            pi4_shm_key)
{
    INT32   shmid   = 0;
    key_t   key     = CMPB_SHM_TAG|(g_shm_idx++);
                
    if((shmid = shmget(key, ui4_shm_len, IPC_CREAT|0666)) < 0)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    *pi4_shm_key = key;
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_FreeSHM(IMTK_PB_HANDLE_T    hHandle,
                                         int32_t             i4_shm_id)
{
    struct  shmid_ds    shmbuf;
        
    if(shmctl(i4_shm_id, IPC_RMID, &shmbuf) < 0)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    return IMTK_PB_ERROR_CODE_OK;
}
#else
IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetSHM(IMTK_PB_HANDLE_T    hHandle,
                                        uint32_t            ui4_buf_len,
                                        void**              ppv_phy_addr,
                                        void**              ppv_vir_addr)
{
    VOID*                   pv_phy_addr = NULL;
    VOID*                   pv_vir_addr = NULL;
    IMTK_PB_ERROR_CODE_T    e_ret       = IMTK_PB_ERROR_CODE_OK;

    pv_phy_addr = x_share_mem_alloc((SIZE_T)ui4_buf_len);
    if(pv_phy_addr == NULL)
    {
        return IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
    }

    pv_vir_addr = x_mmap_share_mem_rmmgr(pv_phy_addr, (SIZE_T)ui4_buf_len);
    if(pv_vir_addr == NULL)
    {
        if(pv_phy_addr != NULL)
        {
            x_share_mem_free(pv_phy_addr, (SIZE_T)ui4_buf_len);
        }
        return IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
    }
    CMPB_MEMSET(pv_vir_addr, 0, ui4_buf_len);
    *ppv_phy_addr = pv_phy_addr;
    *ppv_vir_addr = pv_vir_addr;
    
    return e_ret;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_FreeSHM(IMTK_PB_HANDLE_T    hHandle,
                                         uint32_t            ui4_buf_len,
                                         void*               pv_phy_addr,
                                         void*               pv_vir_addr)
{
    if(pv_phy_addr != NULL)
    {
        x_share_mem_free(pv_phy_addr, (SIZE_T)ui4_buf_len);
        pv_phy_addr = NULL;
    }
    if(pv_vir_addr != NULL)
    {
        x_munmap_share_mem(pv_vir_addr, (SIZE_T)ui4_buf_len);
        pv_vir_addr = NULL;
    }
    return IMTK_PB_ERROR_CODE_OK;
}
#endif

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Get(IMTK_PB_HANDLE_T            hHandle,
                                     IMTK_PB_CTRL_GET_TYPE_T     e_get_type,
                                     VOID*                       pv_get_info,
                                     uint32_t                    z_get_info_size)
{
    INT32                   i4_ret      = 0;
    IMTK_PB_ERROR_CODE_T    e_ret       = IMTK_PB_ERROR_CODE_OK;
    CMPB_CTRL_INST_T*       pt_cmpb_inst;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL== pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT);         
    if(OSR_OK != i4_ret)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    switch(e_get_type)
    {
        case IMTK_PB_CTRL_GET_TYPE_MEDIA_INFO:
        {
            IMTK_PB_CTRL_MEDIA_INFO_T* pt_media_info = NULL;
            
            pt_media_info = (IMTK_PB_CTRL_MEDIA_INFO_T*)pv_get_info;
            if((pt_media_info == NULL) || (z_get_info_size != sizeof(IMTK_PB_CTRL_MEDIA_INFO_T)))
            {
                e_ret = IMTK_PB_ERROR_CODE_INV_ARG;
                break;
            }
            
            CMPB_MEMSET(pt_media_info, 0, sizeof(IMTK_PB_CTRL_MEDIA_INFO_T));

            pt_media_info->eMediaType       = pt_cmpb_inst->u_media.t_get_info.eMediaType;
            pt_media_info->u4TotalDuration  = pt_cmpb_inst->u_media.t_get_info.u4TotalDuration;
            pt_media_info->u8Size           = pt_cmpb_inst->u_media.t_get_info.u8Size;
            pt_media_info->u4AvgBitrate     = pt_cmpb_inst->u_media.t_get_info.u4AvgBitrate;
            pt_media_info->u2AudioTrackNum  = pt_cmpb_inst->u_media.t_get_info.u2AudioTrackNum;
            pt_media_info->u2VideoTrackNum  = pt_cmpb_inst->u2VideoTrackNum;
            pt_media_info->u2SubtlTrackNum  = pt_cmpb_inst->u2SubtitleTrackNum;            
        }
            break;
        case IMTK_PB_CTRL_GET_TYPE_META_DATA:
        {
            MINFO_INFO_T                    t_minfo_rec;
            IMTK_PB_CTRL_META_DATA_INFO_T*  pt_meta_info = NULL;
            
            pt_meta_info = (IMTK_PB_CTRL_META_DATA_INFO_T*)pv_get_info;
            CMPB_MEMSET(&t_minfo_rec, 0, sizeof(MINFO_INFO_T));
            
            if((pt_meta_info == NULL) || (z_get_info_size != sizeof(IMTK_PB_CTRL_META_DATA_INFO_T)))
            {
                e_ret = IMTK_PB_ERROR_CODE_INV_ARG;
                break;
            }
            
 #if SUPPORT_UTF8_METAINFO
            {
                IMTK_PB_CTRL_META_DATA_INFO_T t_metainfo;
                t_metainfo.e_meta_type  = pt_meta_info->e_meta_type;
                t_metainfo.pv_buf       = g_ui2MetaStr;
                t_metainfo.ui2_buf_size = sizeof(g_ui2MetaStr);
                CMPB_MEMSET(g_ui2MetaStr, 0, sizeof(g_ui2MetaStr));
                CMPB_MEMSET(t_metainfo.pv_buf, 0, z_get_info_size);
                _IMtkPb_Ctrl_Get_Meta_Data (hHandle,
                                            &t_metainfo,
                                            &t_minfo_rec);
                CMPB_CVT_UTF16_TO_UTF8((UINT16*)t_metainfo.pv_buf, pt_meta_info->pv_buf, pt_meta_info->ui2_buf_size);
            }
#else
            e_ret = _IMtkPb_Ctrl_Get_Meta_Data (hHandle,
                                                pt_meta_info,
                                                &t_minfo_rec);
#endif
        }
            break;
        case IMTK_PB_CTRL_GET_TYPE_DRM_INFO:
        {
            IMTK_PB_CTRL_DRM_TYPE_T* pe_drm_type = NULL;
            pe_drm_type = (IMTK_PB_CTRL_DRM_TYPE_T*)pv_get_info;

            if((pe_drm_type == NULL) || (z_get_info_size == 0))
            {
                e_ret = IMTK_PB_ERROR_CODE_INV_ARG;
                break;
            }
            *pe_drm_type = pt_cmpb_inst->eDRMType;
        }        
        break;
        case IMTK_PB_CTRL_GET_TYPE_MP3_COVER:
        {
            IMTK_PB_CTRL_MP3_COVER_INFO_T*  pt_mp3_img  = NULL;
            
            pt_mp3_img = (IMTK_PB_CTRL_MP3_COVER_INFO_T*)pv_get_info;
            if((pt_mp3_img == NULL) || (z_get_info_size == 0))
            {
                e_ret = IMTK_PB_ERROR_CODE_INV_ARG;
                break;
            }
            e_ret = _IMtkPb_Ctrl_Get_Mp3_Img_Info ( hHandle, 
                                                    pt_mp3_img);
        }
        break;
        case IMTK_PB_CTRL_GET_TYPE_MEM_BAND_STATUS:
        {        
            i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_TYPE_PUSH_BUFF,
                        pv_get_info,
                        z_get_info_size
                        );
            if(i4_ret != SVCTXR_OK)
            {
                e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
                break;
            }
        }
        break;
        case IMTK_PB_CTRL_GET_TYPE_VIDEO_INFO:
        {
            i4_ret = CMPB_SVCTX_GET(
                        pt_cmpb_inst->h_svctx_hdl,
                        SVCTX_MM_GET_TYPE_VIDEO_INFO,
                        pv_get_info,/* IMTK_PB_CTRL_MM_VIDEO_INFO_T */
                        &z_get_info_size
                        );
            if(i4_ret != SVCTXR_OK)
            {
                e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
                break;
            }
        }
        break;

        case IMTK_PB_CTRL_GET_TYPE_END_TICK:
        {
            UINT32* pui4_end_tick = NULL;
            pui4_end_tick = (UINT32*)pv_get_info;

            if (pui4_end_tick == NULL || z_get_info_size != sizeof(UINT32))
            {
                return IMTK_PB_ERROR_CODE_INV_ARG;
            }

            *pui4_end_tick = pt_cmpb_inst->ui4_end_tick;
        }
        break;
        case IMTK_PB_CTRL_GET_TYPE_3D_VIDEO_INFO:
        {
            if (z_get_info_size != sizeof(IMTK_PB_CTRL_3D_VIDEO_INFO_T))
            {
                e_ret = IMTK_PB_ERROR_CODE_INV_ARG;
                break;
            }
            e_ret = _IMtkPb_Ctrl_Get_3D_video_info(hHandle,
                                                   (IMTK_PB_CTRL_3D_VIDEO_INFO_T*)pv_get_info);
        }
        break;
        case IMTK_PB_CTRL_GET_TYPE_DOWNLOAD_SPEED:
        {
            if(pt_cmpb_inst->e_model != IMTK_PB_CTRL_BUFFERING_MODEL_URI)
            {
                e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
                break;
            }
            i4_ret = CMPB_SVCTX_MEDIA_GET_INFO(
                        pt_cmpb_inst->h_svctx_hdl,
                        pt_cmpb_inst->h_media_hdl,
                        SVCTX_MM_GET_HTTP_DOWNLOAD_SPEED,
                        pv_get_info,
                        z_get_info_size
                        );
            if(i4_ret != SVCTXR_OK)
            {
                e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
                break;
            }
        }
        break;
        
        case IMTK_PB_CTRL_GET_TYPE_MEDIA_STRM_INFO:
        {
            if(z_get_info_size != sizeof(IMTK_PB_CTRL_SET_MEDIA_INFO_T))
            {
                e_ret = IMTK_PB_ERROR_CODE_INV_ARG;
                break;
            }
            if(pt_cmpb_inst->t_push_strm_info.eMediaType == IMTK_PB_CTRL_MEDIA_TYPE_UNKNOWN)
            {/* Not Ready Yet ??? */
                e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
                break;
            }
            CMPB_MEMCPY(pv_get_info, &(pt_cmpb_inst->t_push_strm_info), z_get_info_size);
        }
            break;
        case IMTK_PB_CTRL_GET_TYPE_SPEED:
            i4_ret = CMPB_SVCTX_GET(
                        pt_cmpb_inst->h_svctx_hdl,
                        SVCTX_MM_GET_TYPE_SPEED,
                        pv_get_info,
                        &z_get_info_size
                        );
            if(i4_ret != SVCTXR_OK)
            {
                e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
                break;
            }
        break;    
		case IMTK_PB_CTRL_GET_TYPE_START_TICK:
        {
            UINT32* pui4_start_tick = NULL;
            pui4_start_tick = (UINT32*)pv_get_info;

            if (pui4_start_tick == NULL || z_get_info_size != sizeof(UINT32))
            {
                return IMTK_PB_ERROR_CODE_INV_ARG;
            }

            *pui4_start_tick = pt_cmpb_inst->ui4_start_tick;
        }
        break;
        default:
            i4_ret = IMTK_PB_ERROR_CODE_INV_ARG;
            break;
    }

    CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);        
    
    return e_ret;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Set(IMTK_PB_HANDLE_T            hHandle,
                                     IMTK_PB_CTRL_SET_TYPE_T     e_set_type,
                                     VOID*                       pv_set_info,
                                     uint32_t*                   pz_set_info_size)
{
    IMTK_PB_ERROR_CODE_T        i4_ret          = IMTK_PB_ERROR_CODE_OK;
    CMPB_CTRL_INST_T*           pt_cmpb_inst;

    if (pv_set_info == NULL || pz_set_info_size == NULL) 
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL== pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT);         
    
    switch(e_set_type)
    {
        case IMTK_PB_CTRL_SET_TYPE_TS_PROGRAM_IDX: 
        {
            UINT16* pui2_idx = (UINT16*)pv_set_info;

            if (*pz_set_info_size != sizeof(UINT16))
            {
                i4_ret = IMTK_PB_ERROR_CODE_NOT_OK;
                break;
            }
            
            i4_ret = IMtkPb_Ctrl_Stop(hHandle);
            if (i4_ret != IMTK_PB_ERROR_CODE_OK)
            {
                break;
            }
            
            pt_cmpb_inst->ui2_program_idx = *pui2_idx;

            i4_ret = IMtkPb_Ctrl_Play(hHandle, 0);
            if (i4_ret != IMTK_PB_ERROR_CODE_OK)
            {
                break;
            }
        }   
        break;

        case IMTK_PB_CTRL_SET_TYPE_SERVICE_PID:
        {
            if (*pz_set_info_size == sizeof(UINT16))
            {
                pt_cmpb_inst->ui2_svc_pid = *((UINT16*)pv_set_info);
            }
            else
            {
                return IMTK_PB_ERROR_CODE_INV_ARG;
            }
        }
             break;
        case IMTK_PB_CTRL_SET_TYPE_PCR_PID:
        {
            if (*pz_set_info_size == sizeof(UINT16))
            {
                CMPB_SVCTX_MEDIA_SET_INFO(  pt_cmpb_inst->h_svctx_hdl,
                                            pt_cmpb_inst->h_media_hdl,
                                            SVCTX_MM_SET_TYPE_PCR_PID,
                                            (VOID*)pv_set_info,
                                            sizeof(UINT16));
            }
            else
            {
                return IMTK_PB_ERROR_CODE_INV_ARG;
            }
        }
             break;  
        case IMTK_PB_CTRL_SET_TYPE_AUDIO_PID:
        {
            if (*pz_set_info_size == sizeof(UINT16))
            {
                pt_cmpb_inst->ui2_pvr_audio_pid = *((UINT16*)pv_set_info);
            }
            else
            {
                return IMTK_PB_ERROR_CODE_INV_ARG;
            }
        }
             break;
        case IMTK_PB_CTRL_SET_TYPE_VIDEO_PID:
        {
            if (*pz_set_info_size == sizeof(UINT16))
            {
                pt_cmpb_inst->ui2_pvr_video_pid = *((UINT16*)pv_set_info);
            }
            else
            {
                return IMTK_PB_ERROR_CODE_INV_ARG;
            }
        }
             break;
             
#if 1
        case IMTK_PB_CTRL_SET_TYPE_TOTAL_DUR:
        {
            if (*pz_set_info_size == sizeof(UINT32))
            {
                pt_cmpb_inst->ui4_total_dur = *((UINT32*)pv_set_info);
            }
            else
            {
                return IMTK_PB_ERROR_CODE_INV_ARG;
            }
        }
             break;  
#endif
        case IMTK_PB_CTRL_SET_TYPE_CRYPT_INFO:
        {
            if (*pz_set_info_size == sizeof(IMTK_PB_CTRL_MM_CRYPT_INFO_T))
            {
                IMTK_PB_CTRL_MM_CRYPT_INFO_T* pt_cmpb_crypt = (IMTK_PB_CTRL_MM_CRYPT_INFO_T*)pv_set_info;
                MM_CRYPT_INFO_T               t_crypt_info;
                
                CMPB_MEMSET(&t_crypt_info, 0, sizeof(MM_CRYPT_INFO_T));

                
                    t_crypt_info.e_mm_crypt_type  = MM_CRYPT_TYPE_PVR;

                    switch (pt_cmpb_crypt->e_mode)
                    {
                    case IMTK_PB_CTRL_MM_CRYPT_MODE_3_DES_EBC:
                        t_crypt_info.u.t_pvr_crypt.e_mode = MM_CRYPT_MODE_3_DES_EBC;
                        break;
                    case IMTK_PB_CTRL_MM_CRYPT_MODE_3_DES_CBC:
                        t_crypt_info.u.t_pvr_crypt.e_mode = MM_CRYPT_MODE_3_DES_CBC;
                        break;
                    case IMTK_PB_CTRL_MM_CRYPT_MODE_DES_EBC:
                        t_crypt_info.u.t_pvr_crypt.e_mode = MM_CRYPT_MODE_DES_EBC;
                        break;
                    case IMTK_PB_CTRL_MM_CRYPT_MODE_DES_CBC:
                        t_crypt_info.u.t_pvr_crypt.e_mode = MM_CRYPT_MODE_DES_CBC;
                        break;
                    case IMTK_PB_CTRL_MM_CRYPT_MODE_3_AES_EBC:
                        t_crypt_info.u.t_pvr_crypt.e_mode = MM_CRYPT_MODE_AES_EBC;
                        break;
                    default:
                        t_crypt_info.u.t_pvr_crypt.e_mode = MM_CRYPT_MODE_INVALID;
                        break;
                    }
                    
                    CMPB_MEMCPY(t_crypt_info.u.t_pvr_crypt.aui4_odd_key_1, pt_cmpb_crypt->aui4_odd_key_1, sizeof(pt_cmpb_crypt->aui4_odd_key_1));
                    CMPB_MEMCPY(t_crypt_info.u.t_pvr_crypt.aui4_odd_key_2, pt_cmpb_crypt->aui4_odd_key_2, sizeof(pt_cmpb_crypt->aui4_odd_key_2));
                    CMPB_MEMCPY(t_crypt_info.u.t_pvr_crypt.aui4_even_key_1, pt_cmpb_crypt->aui4_even_key_1, sizeof(pt_cmpb_crypt->aui4_even_key_1));
                    CMPB_MEMCPY(t_crypt_info.u.t_pvr_crypt.aui4_even_key_2, pt_cmpb_crypt->aui4_even_key_2, sizeof(pt_cmpb_crypt->aui4_even_key_2));

                    t_crypt_info.u.t_pvr_crypt.e_key_idx = pt_cmpb_crypt->e_key_idx;
                
            
                CMPB_SVCTX_MEDIA_SET_INFO(  pt_cmpb_inst->h_svctx_hdl,
                                            pt_cmpb_inst->h_media_hdl,
                                            SVCTX_MM_SET_TYPE_CRYPT_KEY,
                                            (VOID*)&t_crypt_info,
                                            sizeof(MM_CRYPT_INFO_T));
            }
            else
            {
                return IMTK_PB_ERROR_CODE_INV_ARG;
            }
        }
            break;
        case IMTK_PB_CTRL_SET_TYPE_SBTL_ATTR:
        {
            if(!pt_cmpb_inst->fg_play_normal)
            {
                i4_ret = IMTK_PB_ERROR_CODE_NOT_OK;
                break;
            }
            i4_ret = _IMtkPb_Ctrl_Set_Sbtl_Attr(hHandle, pv_set_info);
        }
        break;
      
        case IMTK_PB_CTRL_SET_TYPE_MP3_META_LANG:
        {
            i4_ret = CMPB_SVCTX_MEDIA_SET_INFO(
                                pt_cmpb_inst->h_svctx_hdl,
                                pt_cmpb_inst->h_media_hdl,
                                SVCTX_MM_SET_TYPE_LANGUAGE,
                                pv_set_info,
                                *pz_set_info_size
                                );

            if(SVCTXR_OK != i4_ret)
            {
                CMPB_DBG_ERROR(("CMPB_SVCTX_SET() returns %d\n", i4_ret));
                i4_ret = IMTK_PB_ERROR_CODE_NOT_OK;
                break;
            }
        }
        break;
        
        default:
            i4_ret = IMTK_PB_ERROR_CODE_NOT_OK;
            break;
    }

    CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);         

    return i4_ret;
}

#ifdef MW_CAP_LOGO_SUPPORT
IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapOpen(
                            IMTK_PB_CTRL_CAP_SRC_TYPE_T     e_source,
                            IMtkPb_Ctrl_Cap_Nfy_Fct         pf_nfy,
                            const VOID*                     pv_nfy_tag,
                            IMtkPb_Ctrl_Cap_Custom_Nfy_Fct  pf_cust,
                            const VOID*                     pv_cust_tag,
                            IMTK_PB_HANDLE_T*               ph_cap)
{
    INT32   i4_ret = 0;
    i4_ret = CMPB_CAP_OPEN( e_source,
                            (c_cap_nfy_fct)pf_nfy,
                            pv_nfy_tag,
                            (c_cap_cust_fct)pf_cust,
                            pv_cust_tag,
                            (HANDLE_T*)ph_cap);
    
    return (i4_ret == CAPR_OK) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapCapture(
                                    IMTK_PB_HANDLE_T                        h_cap,
                                    const IMTK_PB_CTRL_CAP_CAPTURE_INFO_T*  pt_cap_info)
{
    INT32   i4_ret = 0;
    
    i4_ret = CMPB_CAP_CAPTURE(h_cap,
                              (CAP_CAPTURE_INFO_T*)pt_cap_info);
    
    return (i4_ret == CAPR_OK) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapGetData(IMTK_PB_HANDLE_T    h_cap,
                                            uint8_t**           pBuf,
                                            uint32_t*           len)
{
    INT32   i4_ret = 0;
    VOID*                   pv_phy_addr = NULL;
    VOID*                   pv_vir_addr = NULL;
    IMTK_PB_ERROR_CODE_T    e_ret       = IMTK_PB_ERROR_CODE_OK;

    if((pBuf == NULL) || (*pBuf == NULL) || (*len == 0))
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    pv_phy_addr = x_share_mem_alloc(*len);
    if(pv_phy_addr == NULL)
    {
        return IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
    }

    pv_vir_addr = x_mmap_share_mem_rmmgr(pv_phy_addr, *len);
    if(pv_vir_addr == NULL)
    {
        if(pv_phy_addr != NULL)
        {
            x_share_mem_free(pv_phy_addr, *len);
        }
        return IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
    }

    i4_ret = CMPB_CAP_GET_CAPTURE_DATA(h_cap, (UCHAR**)pv_phy_addr, (UINT32*)len);    
    if(e_ret == IMTK_PB_ERROR_CODE_OK)
    {
        CMPB_MEMCPY((VOID*)((uint32_t*)pBuf), pv_vir_addr, *len);
    }
    
    if(pv_phy_addr != NULL)
    {
        x_share_mem_free(pv_phy_addr, *len);
        pv_phy_addr = NULL;
    }
    if(pv_vir_addr != NULL)
    {
        x_munmap_share_mem(pv_vir_addr, *len);
        pv_phy_addr = NULL;
    }
    
    return (i4_ret == CAPR_OK) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapClose(IMTK_PB_HANDLE_T  h_cap)
{
    INT32   i4_ret = 0;
    
    i4_ret = CMPB_CAP_CLOSE(h_cap);
    
    return (i4_ret == CAPR_OK) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapSave(IMTK_PB_HANDLE_T                           h_cap,
                                         const IMTK_PB_CTRL_CAP_LOGO_SAVE_INFO_T*   pt_save_inf)
{
    INT32   i4_ret = 0;
    
    i4_ret = CMPB_CAP_SAVE( h_cap,
                            (CAP_LOGO_SAVE_INFO_T*)pt_save_inf);
    
    return (i4_ret == CAPR_OK) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapSyncStopCap(IMTK_PB_HANDLE_T   h_cap)
{
    INT32   i4_ret = 0;
    
    i4_ret = CMPB_CAP_SYNC_STOP_CAP( h_cap);
    
    return (i4_ret == CAPR_OK) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_NOT_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapSyncStopSave(IMTK_PB_HANDLE_T   h_cap)
{
    INT32   i4_ret = 0;
    
    i4_ret = CMPB_CAP_SYNC_STOP_SAVE( h_cap);
    
    return (i4_ret == CAPR_OK) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapSelectAsBootLogo(
                        const IMTK_PB_CTRL_CAP_LOGO_SELECT_INFO_T* pt_select_info)
{
    INT32   i4_ret = 0;
    
    i4_ret = CMPB_CAP_SELECT_AS_BOOTLOGO((CAP_LOGO_SELECT_INFO_T*)pt_select_info);
    
    return (i4_ret == CAPR_OK) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_NOT_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapQueryCapability(
                                IMTK_PB_CTRL_CAP_CAPABILITY_INFO_T*  pt_cap_info)
{
    INT32   i4_ret = 0;
    
    i4_ret = CMPB_CAP_QUERY_CAPABILITY((CAP_CAPABILITY_INFO_T*)pt_cap_info);
    
    return (i4_ret == CAPR_OK) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_NOT_OK;
}

#else
IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapOpen(
                            IMTK_PB_CTRL_CAP_SRC_TYPE_T     e_source,
                            IMtkPb_Ctrl_Cap_Nfy_Fct         pf_nfy,
                            const VOID*                     pv_nfy_tag,
                            IMtkPb_Ctrl_Cap_Custom_Nfy_Fct  pf_cust,
                            const VOID*                     pv_cust_tag,
                            IMTK_PB_HANDLE_T*               ph_cap)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapCapture(
                                    IMTK_PB_HANDLE_T                        h_cap,
                                    const IMTK_PB_CTRL_CAP_CAPTURE_INFO_T*  pt_cap_info)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapGetData(IMTK_PB_HANDLE_T    h_cap,
                                            uint8_t**           pBuf,
                                            uint32_t*           len)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapClose(IMTK_PB_HANDLE_T  h_cap)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapSave(IMTK_PB_HANDLE_T                           h_cap,
                                         const IMTK_PB_CTRL_CAP_LOGO_SAVE_INFO_T*   pt_save_inf)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapSyncStopCap(IMTK_PB_HANDLE_T   h_cap)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapSyncStopSave(IMTK_PB_HANDLE_T   h_cap)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapSelectAsBootLogo(
                        const IMTK_PB_CTRL_CAP_LOGO_SELECT_INFO_T* pt_select_info)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_CapQueryCapability(
                                IMTK_PB_CTRL_CAP_CAPABILITY_INFO_T*  pt_cap_info)
{
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

#endif

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetHttpProxy(IMTK_PB_HANDLE_T hHandle,
                                              uint8_t*         pu1String)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;

    if(pu1String == NULL)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    if(pt_cmpb_inst->e_model != IMTK_PB_CTRL_BUFFERING_MODEL_URI)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    pt_cmpb_inst->pui1_proxy_url = (UINT8*)CMPB_MEM_CALLOC(1, sizeof(UINT8)*(CMPB_STRLEN((CHAR*)pu1String)+1));
    if(pt_cmpb_inst->pui1_proxy_url == NULL)
    {
        return IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
    }
    CMPB_MEMSET(pt_cmpb_inst->pui1_proxy_url, 0, CMPB_STRLEN((CHAR*)pu1String)+1);
    CMPB_MEMCPY(pt_cmpb_inst->pui1_proxy_url,
                pu1String,
                CMPB_STRLEN((CHAR*)pu1String));
    
    return IMTK_PB_ERROR_CODE_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetHttpAgent(IMTK_PB_HANDLE_T hHandle,
                                              uint8_t*         pu1String)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;

    if(pu1String == NULL)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    if(pt_cmpb_inst->e_model != IMTK_PB_CTRL_BUFFERING_MODEL_URI)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    pt_cmpb_inst->pui1_agent_url = (UINT8*)CMPB_MEM_CALLOC(1, sizeof(UINT8)*(CMPB_STRLEN((CHAR*)pu1String)+1));
    if(pt_cmpb_inst->pui1_agent_url == NULL)
    {
        return IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
    }
    CMPB_MEMSET(pt_cmpb_inst->pui1_agent_url, 0, CMPB_STRLEN((CHAR*)pu1String)+1);
    CMPB_MEMCPY(pt_cmpb_inst->pui1_agent_url,
                pu1String,
                CMPB_STRLEN((CHAR*)pu1String));
    
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetHttpMonitor(IMTK_PB_HANDLE_T hHandle,
                                              bool         b_monitor)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    if(pt_cmpb_inst->e_model != IMTK_PB_CTRL_BUFFERING_MODEL_URI)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    pt_cmpb_inst->b_monitor = b_monitor;    
    
    return IMTK_PB_ERROR_CODE_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Set_DRM_Info(IMTK_PB_HANDLE_T           hHandle,
                                              IMTK_PB_CTRL_DRM_INFO_T*   pt_drm_inf)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;

    if(pt_drm_inf == NULL)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    if(pt_drm_inf->e_drm_type == IMTK_PB_CTRL_DRM_TYPE_MLN)
    {
        pt_cmpb_inst->t_drm_info.e_drm_type = IMTK_PB_CTRL_DRM_TYPE_MLN;

        pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url
            = (uint8_t*)CMPB_MEM_CALLOC(1,
                                        sizeof(uint8_t)*(pt_drm_inf->u.t_mln_drm_inf.ui2_act_tkn_url_length+1));
        if(pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url == NULL)
        {
            return IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
        }
        CMPB_MEMCPY(pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url,
                    pt_drm_inf->u.t_mln_drm_inf.pui1_act_tkn_url,
                    pt_drm_inf->u.t_mln_drm_inf.ui2_act_tkn_url_length);
        pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.ui2_act_tkn_url_length
                            = pt_drm_inf->u.t_mln_drm_inf.ui2_act_tkn_url_length;

        
        pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_afl_tkn
            = (uint8_t*)CMPB_MEM_CALLOC(1,
                                        sizeof(uint8_t)*(pt_drm_inf->u.t_mln_drm_inf.ui2_afl_tkn_length+1));
        if(pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_afl_tkn == NULL)
        {
            CMPB_MEM_FREE(pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url);
            pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url = NULL;
            return IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
        }
        CMPB_MEMCPY(pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.pui1_afl_tkn,
                    pt_drm_inf->u.t_mln_drm_inf.pui1_afl_tkn,
                    pt_drm_inf->u.t_mln_drm_inf.ui2_afl_tkn_length);
        pt_cmpb_inst->t_drm_info.u.t_mln_drm_inf.ui2_afl_tkn_length 
                            = pt_drm_inf->u.t_mln_drm_inf.ui2_afl_tkn_length;
    }
    else
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_SetURL(IMTK_PB_HANDLE_T            hHandle,
                                        IMTK_PB_CTRL_URL_TYPE_T     e_url_type,
                                        uint8_t*                    pu1String)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;

    if((pu1String == NULL) || (e_url_type >= IMTK_PB_CTRL_URL_TYPE_MAX))
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (NULL == pt_cmpb_inst)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    if(pt_cmpb_inst->ac_url_list[e_url_type] != NULL)
    {
        CMPB_MEM_FREE(pt_cmpb_inst->ac_url_list[e_url_type]);
        pt_cmpb_inst->ac_url_list[e_url_type] = NULL;
    }
    pt_cmpb_inst->ac_url_list[e_url_type] = (char*)CMPB_MEM_CALLOC(1, sizeof(uint8_t)*(CMPB_STRLEN((CHAR*)pu1String)+1));
    if(pt_cmpb_inst->ac_url_list[e_url_type] == NULL)
    {
        return IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
    }
    CMPB_MEMSET(pt_cmpb_inst->ac_url_list[e_url_type], 0, CMPB_STRLEN((CHAR*)pu1String)+1);
    CMPB_MEMCPY(pt_cmpb_inst->ac_url_list[e_url_type],
                pu1String,
                CMPB_STRLEN((CHAR*)pu1String));
    
    return IMTK_PB_ERROR_CODE_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetTSInfo( IMTK_PB_HANDLE_T                    hHandle,
                                            uint8_t*                            pui1_buf, 
                                            uint32_t                            ui4_buf_len,
                                            IMTK_PB_CTRL_TS_SINGLE_PAT_INFO_T*  ptTSInfo)
{
    BOOL        fg_payload_flag     = FALSE;
    UINT16      ui2_pat_pid         = 0;
    UINT16      ui2_section_len     = 0;/* 12 bit */
    UINT16      ui2_prog_info_len   = 0;/* 12 bit */
    UINT32      ui4_i               = 0;
    UINT32      ui4_cur_pos         = 0;
    UINT32      ui4_pmt_pos         = 0;
        
    
    if((pui1_buf == NULL) || (ui4_buf_len<188) || (ptTSInfo == NULL))
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    x_memset(ptTSInfo, 0, sizeof(IMTK_PB_CTRL_TS_SINGLE_PAT_INFO_T));
    
    /* Search Sync_Byte 0x47 First */
    while(1)
    {
        if(ui4_cur_pos >= ui4_buf_len)
        {
            break;/* Sync byte not found, pat not exist, maybe packet size error */
        }
        if(pui1_buf[ui4_cur_pos] == 0x47)
        {
            for(ui4_i = 0; ui4_i < CHECK_SYNC_BYTE_NUM; ui4_i ++)
            {
                if(pui1_buf[ui4_cur_pos + ui4_i * 188] != 0x47)
                {
                    break;
                }
            }
            if(ui4_i == CHECK_SYNC_BYTE_NUM)
            {
                ptTSInfo->ui2_packet_size = 188;
                break;
            }
        }
        ui4_cur_pos ++;
    }

    if(ui4_cur_pos >= ui4_buf_len)
    {
        ui4_cur_pos = 0;
        while(1)
        {
            if(ui4_cur_pos >= ui4_buf_len)
            {
                return IMTK_PB_ERROR_CODE_NOT_OK;/* Sync byte not found, pat not exist */
            }
            if(pui1_buf[ui4_cur_pos] == 0x47)
            {
                for(ui4_i = 0; ui4_i < CHECK_SYNC_BYTE_NUM; ui4_i ++)
                {
                    if(pui1_buf[ui4_cur_pos + ui4_i * 192] != 0x47)
                    {
                        break;
                    }
                }
                if(ui4_i == CHECK_SYNC_BYTE_NUM)
                {
                    ptTSInfo->ui2_packet_size = 192;
                    break;
                }
            }
            ui4_cur_pos ++;
        }
    }

    while(ui4_cur_pos < ui4_buf_len)
    {
        fg_payload_flag = ((pui1_buf[ui4_cur_pos + 1] & 0x40) != 0); 
        if(fg_payload_flag == TRUE)
        {/* Got Pat Table Start Address */
            ui2_pat_pid     = ((pui1_buf[ui4_cur_pos + 1] & 0x1F)<<8) | pui1_buf[ui4_cur_pos + 2];
            if(ui2_pat_pid == 0)
            {
                break;
            }
        }
        ui4_cur_pos = ui4_cur_pos + ptTSInfo->ui2_packet_size;
        if(ui4_cur_pos >= ui4_buf_len)
        {
            return IMTK_PB_ERROR_CODE_NOT_OK;//Pat table not exist;
        }
    }

    //Pat start address got, start parse pat pid table
    if(ui2_pat_pid == 0)
    {
        ui2_section_len = ((pui1_buf[ui4_cur_pos + 6] & 0x0F)<<8) | pui1_buf[ui4_cur_pos + 7];
        ptTSInfo->ui2_pat_pid = ui2_pat_pid;
        ptTSInfo->ui1_pmt_num = (ui2_section_len - 9)/4;
        if(ptTSInfo->ui1_pmt_num >= SUPPORT_PMT_MAX_NUM)
        {
            ptTSInfo->ui1_pmt_num = SUPPORT_PMT_MAX_NUM;
        }
        for(ui4_i=0; ui4_i<ptTSInfo->ui1_pmt_num; ui4_i++)
        {
            ui4_pmt_pos = ui4_cur_pos + 12 + ui4_i*4 + 3;
            if(ui4_pmt_pos > ui4_buf_len)
            {
                break;
            }
            ptTSInfo->at_pmt_info[ui4_i].ui2_pmt_pid
                = ((pui1_buf[ui4_pmt_pos] & 0x1F)<<8) | pui1_buf[ui4_pmt_pos+1];
        }
    }

    //Reset parse position, as pmt table maybe before pat table
    ui4_cur_pos = ui4_cur_pos%ptTSInfo->ui2_packet_size;
    
    //Parse each pmt detail info
    while(ui4_cur_pos < ui4_buf_len)
    {
        UINT8   ui1_idx = 0;
        UINT16  ui2_tmp_pmt_pid = 0;
        
        fg_payload_flag = ((pui1_buf[ui4_cur_pos + 1] & 0x40) != 0); 
        ui2_tmp_pmt_pid = ((pui1_buf[ui4_cur_pos + 1] & 0x1F)<<8) | pui1_buf[ui4_cur_pos + 2];
        if(fg_payload_flag == TRUE)
        {
            for(ui1_idx=0; ui1_idx<ptTSInfo->ui1_pmt_num; ui1_idx++)
            {
                if(ui1_idx >= SUPPORT_PMT_MAX_NUM)
                {
                    break;
                }
                if(ui2_tmp_pmt_pid == ptTSInfo->at_pmt_info[ui1_idx].ui2_pmt_pid)
                {
                    if(ptTSInfo->at_pmt_info[ui1_idx].fg_init == FALSE)
                    {
                        UINT16 ui2_ES_info_len  = 0;
                        UINT16 ui2_ofst         = 17;
                        
                        ui2_section_len   = ((pui1_buf[ui4_cur_pos + 6] & 0x0F)<<8) | pui1_buf[ui4_cur_pos + 7];
                        ui2_prog_info_len = ((pui1_buf[ui4_cur_pos + 15] & 0x0F)<<8) | pui1_buf[ui4_cur_pos + 16];
                        if(ui2_prog_info_len >= ui2_section_len)
                        {
                            break;/* this pmt is invalid */
                        }
                        ui2_ofst += ui2_prog_info_len;
                        for(ui4_i=0; ;ui4_i++)
                        {
                            if(ui4_i >= SINGLE_PMT_MAX_STREAM_NUM)
                            {
                                break;
                            }
                            ui4_pmt_pos = ui4_cur_pos + ui2_ofst;
                            ui2_ES_info_len = ((pui1_buf[ui4_pmt_pos + 3] & 0x0F)<<8) | pui1_buf[ui4_pmt_pos + 4];;
                            ptTSInfo->at_pmt_info[ui1_idx].at_stream_info_list[ui4_i].ui2_strm_pid 
                                    = ((pui1_buf[ui4_pmt_pos + 1] & 0x1F)<<8) | pui1_buf[ui4_pmt_pos + 2];
                            ptTSInfo->at_pmt_info[ui1_idx].at_stream_info_list[ui4_i].ui2_strm_type
                                    = pui1_buf[ui4_pmt_pos];
                            ptTSInfo->at_pmt_info[ui1_idx].ui1_strm_num++;
                            ui2_ofst += ui2_ES_info_len + 5;
                            if(ui2_ofst >= ui2_section_len)
                            {
                                break;
                            }
                        }
                        ptTSInfo->at_pmt_info[ui1_idx].fg_init = TRUE;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        for(ui1_idx=0; ui1_idx<ptTSInfo->ui1_pmt_num; ui1_idx++)
        {
            if(ui1_idx >= SUPPORT_PMT_MAX_NUM)
            {
                break;
            }
            if(ptTSInfo->at_pmt_info[ui1_idx].fg_init == FALSE)
            {
                break;
            }
        }
        if(ui1_idx == ptTSInfo->ui1_pmt_num)
        {
            break;/* all pmt pid filled */
        }
        ui4_cur_pos     = ui4_cur_pos + ptTSInfo->ui2_packet_size;
    }
    
    return IMTK_PB_ERROR_CODE_OK;
}

#ifdef LINUX_TURNKEY_SOLUTION
static int mem_fd = 0;
#endif
static INT32 IMtkPb_Ctrl_MapBuf(HANDLE_T h_aud_scc)
{
    INT32  i4_ret;
    SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T t_mux_buf_info;

    CMPB_MEMSET(&t_mux_buf_info, 0, sizeof(SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T));
    i4_ret = CMPB_C_SCC_COMP_GET_AUD_BUF_INFO(h_aud_scc, &t_mux_buf_info);
    if(i4_ret == SMR_OK)
    {
        t_mux_info.pv_muxbuf_phy_addr = (VOID*)((t_mux_buf_info.ui4_buffer_start_addr/4096) * 4096);/*4k alliment*/
        t_mux_info.ui4_mux_buf_len    = t_mux_buf_info.ui4_buffer_size;
#ifdef LINUX_TURNKEY_SOLUTION 
        mem_fd = open("/dev/mem", O_RDWR|O_SYNC);
        t_mux_info.pv_muxbuf_vir_addr =  (void*)mmap(0,
                                            (t_mux_info.ui4_mux_buf_len + 4096) & ~4096, /*4k alliment*/
                                            PROT_READ,
                                            MAP_SHARED,
                                            mem_fd,
                                            (UINT32)t_mux_info.pv_muxbuf_phy_addr);
#else
        t_mux_info.pv_muxbuf_vir_addr = t_mux_info.pv_muxbuf_phy_addr;
#endif
    }
    
    return i4_ret;
}
#if 0
static VOID IMtkPb_Ctrl_UnMapBuf()
{
#ifdef LINUX_TURNKEY_SOLUTION 
    munmap(t_mux_info.pv_muxbuf_vir_addr, 
           (SIZE_T)(t_mux_info.ui4_mux_buf_len + 4096) & ~4096);
    mem_fd = 0;
    x_memset(&t_mux_info, 0, sizeof(t_mux_info));
#endif
}
#endif

INT32 _aud_upload_nfy_fct(    
                        VOID*                     pv_nfy_tag,
                        UINT32                    ui4_addr,
                        UINT32                    ui4_size)
{
    UINT32 ui4_vir_addr;
    UINT32 ui4_tmp_size;
    
    if(0 == t_aud_upload_nfy_info.pf_upload_data_nfy)
    {
        //fprintf(stderr,"_aud_upload_nfy_fct(), pf_upload_data_nfy == NULL \n");
        return 0;
    }
#if 0
    fprintf(stderr,"_aud_upload_nfy_fct(), pf_upload_data_nfy() is called,addr is %x, len is %d, tag is %x \n", 
        (unsigned int)(ui4_addr - (uint32_t)t_mux_info.pv_muxbuf_phy_addr + (uint32_t)t_mux_info.pv_muxbuf_vir_addr),
        (int)ui4_size, (unsigned int)pv_nfy_tag);
#endif
    ui4_vir_addr = (ui4_addr - 
                   (uint32_t)t_mux_info.pv_muxbuf_phy_addr + 
                   (uint32_t)t_mux_info.pv_muxbuf_vir_addr);

    if ( ((UINT32)t_mux_info.pv_muxbuf_vir_addr + (UINT32)t_mux_info.ui4_mux_buf_len) - ui4_vir_addr < ui4_size)
    {
        /* wrap around case */
        ui4_tmp_size = ((UINT32)t_mux_info.pv_muxbuf_vir_addr + (UINT32)t_mux_info.ui4_mux_buf_len) -
                     ui4_vir_addr;
        
        t_aud_upload_nfy_info.pf_upload_data_nfy( (const char*)(ui4_addr - 
                                                         (uint32_t)t_mux_info.pv_muxbuf_phy_addr + 
                                                         (uint32_t)t_mux_info.pv_muxbuf_vir_addr), 
                                                         (long)ui4_tmp_size,
                                                         pv_nfy_tag);

        t_aud_upload_nfy_info.pf_upload_data_nfy( (const char*)((uint32_t)t_mux_info.pv_muxbuf_vir_addr), 
                                                         (long)(ui4_size-ui4_tmp_size),
                                                         pv_nfy_tag);

    }
    else
    {
        t_aud_upload_nfy_info.pf_upload_data_nfy( (const char*)(ui4_addr - 
                                                     (uint32_t)t_mux_info.pv_muxbuf_phy_addr + 
                                                     (uint32_t)t_mux_info.pv_muxbuf_vir_addr), 
                                                     (long)ui4_size,
                                                     pv_nfy_tag);
    }

    return 0;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_Capture_Setting(bool      b_on_off,
                                                     uint32_t  ui4_sample_rate,
                                                     uint8_t   ui1_sample_depth,
                                                     uint8_t   ui1_sample_channels)
{
    INT32           i4_ret;
    SM_COMMAND_T    at_sm_cmds[3];
    HANDLE_T        h_aud_scc = NULL_HANDLE;

    //fprintf(stderr, "call IMtkPb_Ctrl_Ext_Capture_Setting(), b_on_off is %d \n", (int)b_on_off);
    at_sm_cmds[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data = SCC_CMD_TYPE_AUDIO;
    at_sm_cmds[1].e_code     = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
    at_sm_cmds[2].e_code     = SM_CMD_CODE_END;
    at_sm_cmds[2].u.ui4_data = 0;
    i4_ret = CMPB_C_SCC_COMP_OPEN(at_sm_cmds, &h_aud_scc);
    if(i4_ret != SMR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    i4_ret = c_scc_aud_set_upload_ctrl(h_aud_scc, b_on_off);
    
    CMPB_C_SCC_COMP_CLOSE(h_aud_scc);
    
    return IMTK_PB_ERROR_CODE_OK;
}



IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_SetBufferSink(IMTK_PB_CTRL_STREAM_TYPE    eStreamType,
                                                   IMtkPb_Ctrl_Ext_Buf_Cb_Fct  pfnCb,
                                                   void*                       pvAppCbTag)
{
    INT32                           i4_ret;
    SM_COMMAND_T                    at_sm_cmds[3];
    HANDLE_T                        h_aud_scc = NULL_HANDLE;
    SCC_AUD_UPLOAD_DATA_NFT_INFO_T  t_nfy_info;

    //fprintf(stderr, "call IMtkPb_Ctrl_Ext_SetBufferSink(), eStreamType = %d \n", (int)eStreamType);

    if(eStreamType != IMTK_PB_CTRL_STREAM_TYPE_AUDIO)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    t_aud_upload_nfy_info.pf_upload_data_nfy = pfnCb;
    t_aud_upload_nfy_info.pv_nfy_tag         = pvAppCbTag;
    
    at_sm_cmds[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data = SCC_CMD_TYPE_AUDIO;
    at_sm_cmds[1].e_code     = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
    at_sm_cmds[2].e_code     = SM_CMD_CODE_END;
    at_sm_cmds[2].u.ui4_data = 0;
    i4_ret = CMPB_C_SCC_COMP_OPEN(at_sm_cmds, &h_aud_scc);
    if(i4_ret != SMR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    if(IMtkPb_Ctrl_MapBuf(h_aud_scc) != SMR_OK)
    {
        CMPB_C_SCC_COMP_CLOSE(h_aud_scc);
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    t_nfy_info.pf_upload_data_nfy = (x_scc_aud_upload_data_nfy_fct)_aud_upload_nfy_fct;
    t_nfy_info.pv_nfy_tag         = pvAppCbTag;
    
    i4_ret = c_scc_aud_set_upload_data_nfy(h_aud_scc, &t_nfy_info);
#if 0    
    CMPB_C_SCC_COMP_CLOSE(h_aud_scc);
#endif
    return IMTK_PB_ERROR_CODE_OK;
}

static IMTK_PB_ERROR_CODE_T _cmpb_cvt_aud_setting(IMTK_PB_CTRL_EXT_ENC_REC_SETTING_T*  prSetting,
                                                       SCC_AUD_UPLOAD_PCM_INFO_T*           pPcmInfo)
{
    if(NULL == prSetting || NULL == pPcmInfo)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    switch(pPcmInfo->ui4_pcm_bits)
    {
    case 0: //16bits
        prSetting->tAudioSetting.u1SampleSize = 16;
        break;
    case 1:
        prSetting->tAudioSetting.u1SampleSize = 24;
        break;
    default:
        prSetting->tAudioSetting.u1SampleSize = 0;
        break;
    }

    switch(pPcmInfo->ui4_pcm_channels)
    {
    case 0: //16bits
        prSetting->tAudioSetting.u1Channels = 2;
        break;
    case 1:
        prSetting->tAudioSetting.u1Channels = 1;
        break;
    case 2:
        prSetting->tAudioSetting.u1Channels = 4;
        break;
    default:
        prSetting->tAudioSetting.u1Channels = 0;
        break;
    }

    switch(pPcmInfo->ui4_pcm_sample_rate)
    {
    case 0: //16bits
        prSetting->tAudioSetting.u4SampleRate = 48000;
        break;
    case 1:
        prSetting->tAudioSetting.u4SampleRate = 44100;
        break;
    case 2:
        prSetting->tAudioSetting.u4SampleRate = 32000;
        break;
    default:
        prSetting->tAudioSetting.u4SampleRate = 0;
        break;
    }
    
    //fprintf(stderr, "call _cmpb_cvt_aud_setting(), sample rate = %d, channels = %d, sample size = %d \n", 
       // prSetting->tAudioSetting.u4SampleRate, prSetting->tAudioSetting.u1Channels, prSetting->tAudioSetting.u1SampleSize );

    return IMTK_PB_ERROR_CODE_OK;

}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Ext_GetSetting(IMTK_PB_CTRL_STREAM_TYPE    eStreamType,
                                                    IMTK_PB_CTRL_EXT_ENC_REC_SETTING_T*  prSetting)
{
    INT32           i4_ret;
    SM_COMMAND_T    at_sm_cmds[3];
    HANDLE_T        h_aud_scc = NULL_HANDLE;
    SCC_AUD_UPLOAD_PCM_INFO_T  t_pcm_info;

    if(NULL == prSetting)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    if(eStreamType != IMTK_PB_CTRL_STREAM_TYPE_AUDIO)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    at_sm_cmds[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data = SCC_CMD_TYPE_AUDIO;
    at_sm_cmds[1].e_code     = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
    at_sm_cmds[2].e_code     = SM_CMD_CODE_END;
    at_sm_cmds[2].u.ui4_data = 0;
    i4_ret = CMPB_C_SCC_COMP_OPEN(at_sm_cmds, &h_aud_scc);
    if(i4_ret != SMR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    CMPB_MEMSET(prSetting, 0, sizeof(IMTK_PB_CTRL_EXT_ENC_REC_SETTING_T));
    CMPB_MEMSET(&t_pcm_info, 0, sizeof(t_pcm_info));
    
    i4_ret = c_scc_aud_get_upload_pcm_info(h_aud_scc, &t_pcm_info);
    if(i4_ret != SMR_OK)
    {
        CMPB_C_SCC_COMP_CLOSE(h_aud_scc); 
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    i4_ret = _cmpb_cvt_aud_setting(prSetting, &t_pcm_info);   
    
    CMPB_C_SCC_COMP_CLOSE(h_aud_scc); 

    return i4_ret;
}

static IMTK_PB_ERROR_CODE_T _set_thumbnail_mode(BOOL OnOff, BOOL IsMain)
{
    static  INT8        i1_cur_mode = 0xff;
    INT32               i4_ret = IMTK_PB_ERROR_CODE_OK;
    SM_COMMAND_T        at_sm_cmds[3];
    HANDLE_T            h_scc_vid = NULL_HANDLE;

    if((OnOff && (i1_cur_mode == 1)) || (!OnOff && (i1_cur_mode == 0)))
    {
        return IMTK_PB_ERROR_CODE_OK;
    }

    /* create the scc handles */
    at_sm_cmds[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data = SCC_CMD_TYPE_VIDEO;
    at_sm_cmds[1].e_code     = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text  = IsMain ? SN_PRES_MAIN_DISPLAY : SN_PRES_SUB_DISPLAY;
    at_sm_cmds[2].e_code     = SM_CMD_CODE_END;
    at_sm_cmds[2].u.ui4_data = 0;
    i4_ret = CMPB_C_SCC_COMP_OPEN(at_sm_cmds, &h_scc_vid);
    if(i4_ret != SMR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    i4_ret = CMPB_C_SCC_VID_SET_SRM_MODE(h_scc_vid, 
                                         OnOff ? SCC_VID_SRM_MODE_THUMBNAIL : SCC_VID_SRM_MODE_VIDEO);
    if (NULL_HANDLE != h_scc_vid)
    {
        CMPB_C_SCC_COMP_CLOSE(h_scc_vid);
        h_scc_vid = NULL_HANDLE;
    }
    
    i1_cur_mode = OnOff ? 1 : 0;
    
    return IMTK_PB_ERROR_CODE_OK;
}

static VOID _cmpb_thumbnail_load_nfy_fct(HANDLE_T                    h_svctx,
                                         SVCTX_COND_T                e_nfy_cond,
                                         SVCTX_NTFY_CODE_T           e_code,
                                         STREAM_TYPE_T               e_stream_type,
                                         VOID*                       pv_nfy_tag)
{
    CMPB_CTRL_INST_T*   pt_cmpb_inst = NULL;

    pt_cmpb_inst = (CMPB_CTRL_INST_T*)pv_nfy_tag;
    if (e_code == SVCTX_NTFY_CODE_MEDIA_EOS)
    {
        pt_cmpb_inst->e_thumbnail_result = IMTK_PB_THUMBNAIL_RESULT_OK;
    }
    else if(e_code == SVCTX_NTFY_CODE_MEDIA_ERROR)
    {
        pt_cmpb_inst->e_thumbnail_result = IMTK_PB_THUMBNAIL_RESULT_FAIL;
    }
    else if((e_code == SVCTX_NTFY_CODE_MEDIA_THUMBNAIL_DONE)/*timeout*/ &&
            (pt_cmpb_inst->e_thumbnail_result != IMTK_PB_THUMBNAIL_RESULT_OK))
    {
        pt_cmpb_inst->e_thumbnail_result = IMTK_PB_THUMBNAIL_RESULT_FAIL;
    }
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetThumbNail(IMTK_PB_HANDLE_T                hHandle,
                                              IMTK_PB_THUMBNAIL_INFO_T*       ptThumbnail)
{
    CMPB_CTRL_INST_T*                   pt_cmpb_inst= NULL;
    INT32                               i4_ret      = 0;
    SIZE_T                              z_size      = 0;
    SVCTX_VIDEO_THUMBNAIL_INFO_T        t_thumb_info;    
    VOID*                               pv_phy_addr = NULL;
    VOID*                               pv_vir_addr = NULL;
    IMTK_PB_ERROR_CODE_T                e_ret       = IMTK_PB_ERROR_CODE_OK;
    UINT8*                              pu1TmpBuf   = NULL;

    if(ptThumbnail == NULL)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    pu1TmpBuf = ptThumbnail->u1CanvasBuffer;
    
    CMPB_MEMSET(&t_thumb_info, 0, sizeof(SVCTX_VIDEO_THUMBNAIL_INFO_T));
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if ((NULL == pt_cmpb_inst) || (pt_cmpb_inst->b_parse_done == FALSE))
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
#ifdef MW_TV_AV_BYPASS_SUPPORT
    CMPB_SVCTX_SET(h_tv_bypass_svctx,
                    SVCTX_COMMON_SET_ENABLE_SVC_SELECT_WITH_ASSOCIATE,
                    &b_tv_scart_enabled,
                    sizeof(BOOL));

    CMPB_SVCTX_SET(h_monitor_bypass_svctx,
                    SVCTX_COMMON_SET_ENABLE_SVC_SELECT_WITH_ASSOCIATE,
                    &b_monitor_scart_enabled,
                    sizeof(BOOL));
#endif

    if(h_sema_thumbnail)
    {
        i4_ret = CMPB_SEMA_LOCK(h_sema_thumbnail, X_SEMA_OPTION_WAIT); 
    }

    pt_cmpb_inst->e_thumbnail_result = IMTK_PB_THUMBNAIL_RESULT_UNKNOWN;
    
    i4_ret = _set_thumbnail_mode(TRUE, (pt_cmpb_inst->ui1_group_mode == 0));
    if(i4_ret != IMTK_PB_ERROR_CODE_OK)
    {
        if(h_sema_thumbnail)
        {
            CMPB_SEMA_UNLOCK(h_sema_thumbnail); 
        }
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    /* Open and play*/    
    t_thumb_info.h_surf                 = NULL_HANDLE;
    t_thumb_info.ui4_thumbnail_x        = 0;
    t_thumb_info.ui4_thumbnail_y        = 0;
    t_thumb_info.ui4_thumbnail_width    = ptThumbnail->u4ThumbnailWidth;
    t_thumb_info.ui4_thumbnail_height   = ptThumbnail->u4ThumbnailHeight;
    t_thumb_info.ui8_pic_idx            = 10;
    t_thumb_info.pf_nfy                 = _cmpb_thumbnail_load_nfy_fct;
    t_thumb_info.pv_tag                 = (VOID*)pt_cmpb_inst;
    
    CMPB_SVCTX_MEDIA_LOAD_THUMBNAIL(pt_cmpb_inst->h_svctx_hdl,
                                    pt_cmpb_inst->h_media_hdl,
                                    &t_thumb_info);
    /* wait for play done */    
    while(1)
    {
        if(pt_cmpb_inst->e_thumbnail_result == IMTK_PB_THUMBNAIL_RESULT_UNKNOWN)
        {
            CMPB_THREAD_DELAY(1);
        }
        else if(pt_cmpb_inst->e_thumbnail_result == IMTK_PB_THUMBNAIL_RESULT_OK)
        {
            break;
        }
        else
        {
            //_set_thumbnail_mode(FALSE, (pt_cmpb_inst->ui1_group_mode == 0));
            if(h_sema_thumbnail)
            {
                CMPB_SEMA_UNLOCK(h_sema_thumbnail); 
            }
            return IMTK_PB_ERROR_CODE_NOT_OK;
        }
    }

    /* prepare temp buffer */
    do
    {
        pv_phy_addr = x_share_mem_alloc((SIZE_T)(ptThumbnail->u4BufLen));
        if(pv_phy_addr == NULL)
        {
            e_ret = IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
            break;
        }
        
        pv_vir_addr = x_mmap_share_mem_rmmgr(pv_phy_addr, (SIZE_T)(ptThumbnail->u4BufLen));
        if(pv_vir_addr == NULL)
        {
            e_ret = IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
            break;
        }
        
        ptThumbnail->u1CanvasBuffer = (UINT8*)pv_phy_addr;
        
        /* get thumbnail data */ 
        z_size = sizeof(IMTK_PB_THUMBNAIL_INFO_T);    
        i4_ret = CMPB_SVCTX_MEDIA_GET_INFO( pt_cmpb_inst->h_svctx_hdl,
                                            pt_cmpb_inst->h_media_hdl,
                                            SVCTX_MM_GET_THUMBNAIL,
                                            (VOID*)ptThumbnail,
                                            z_size);
        if (i4_ret != SVCTXR_OK)
        {
            e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
            break;
        }
    }while(0);
    if(e_ret == IMTK_PB_ERROR_CODE_OK)
    {
        CMPB_MEMCPY(pu1TmpBuf, pv_vir_addr, ptThumbnail->u4BufLen);
    }
    ptThumbnail->u1CanvasBuffer = pu1TmpBuf;
    
    if(pv_phy_addr != NULL)
    {
        x_share_mem_free(pv_phy_addr, (SIZE_T)(ptThumbnail->u4BufLen));
        pv_phy_addr = NULL;
    }
    if(pv_vir_addr != NULL)
    {
        x_munmap_share_mem(pv_vir_addr, (SIZE_T)(ptThumbnail->u4BufLen));
        pv_phy_addr = NULL;
    }
    //_set_thumbnail_mode(FALSE, (pt_cmpb_inst->ui1_group_mode == 0));

    if(h_sema_thumbnail)
    {
        CMPB_SEMA_UNLOCK(h_sema_thumbnail); 
    }

    return e_ret;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Get_BG_Color(IMTK_PB_CTRL_BG_COLOR_T* pt_clr)
{
    SM_COMMAND_T    at_sm_cmds[4];
    INT32           i4_ret;
    HANDLE_T        h_scc_vid = NULL_HANDLE;
    SCC_BG_COLOR_T  t_bg;
    IMTK_PB_ERROR_CODE_T e_ret = IMTK_PB_ERROR_CODE_OK;

    if(NULL == pt_clr)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    /* create the scc handles */
    at_sm_cmds[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data = SCC_CMD_TYPE_VIDEO;
    at_sm_cmds[1].e_code     = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
    at_sm_cmds[2].e_code     = SM_CMD_CODE_END;
    at_sm_cmds[2].u.ui4_data = 0;
    i4_ret = CMPB_C_SCC_COMP_OPEN(at_sm_cmds, &h_scc_vid);
    if(i4_ret != SMR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    i4_ret = CMPB_C_SCC_VID_GET_BG(h_scc_vid, &t_bg);
    if(i4_ret != SMR_OK)
    {
        e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
    }

    pt_clr->ui1_blue  = t_bg.ui1_blue;
    pt_clr->ui1_green = t_bg.ui1_green;
    pt_clr->ui1_red   = t_bg.ui1_red;

    CMPB_C_SCC_COMP_CLOSE(h_scc_vid);
    return e_ret;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Set_BG_Color(IMTK_PB_CTRL_BG_COLOR_T* pt_clr)
{
    SM_COMMAND_T    at_sm_cmds[4];
    INT32           i4_ret;
    HANDLE_T        h_scc_vid = NULL_HANDLE;
    SCC_BG_COLOR_T  t_bg;
    IMTK_PB_ERROR_CODE_T e_ret = IMTK_PB_ERROR_CODE_OK;

    if(NULL == pt_clr)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    /* create the scc handles */
    at_sm_cmds[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data = SCC_CMD_TYPE_VIDEO;
    at_sm_cmds[1].e_code     = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
    at_sm_cmds[2].e_code     = SM_CMD_CODE_END;
    at_sm_cmds[2].u.ui4_data = 0;
    i4_ret = CMPB_C_SCC_COMP_OPEN(at_sm_cmds, &h_scc_vid);
    if(i4_ret != SMR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    t_bg.ui1_blue  = pt_clr->ui1_blue;
    t_bg.ui1_green = pt_clr->ui1_green;
    t_bg.ui1_red   = pt_clr->ui1_red;
    i4_ret = CMPB_C_SCC_VID_SET_BG(h_scc_vid, &t_bg);
    if(i4_ret != SMR_OK)
    {
        e_ret = IMTK_PB_ERROR_CODE_NOT_OK;
    }

    CMPB_C_SCC_COMP_CLOSE(h_scc_vid);
    return e_ret;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_Get_PushBufCnt(IMTK_PB_HANDLE_T    hHandle,
                                                      int32_t*              pi4_cnt)
{
    CMPB_CTRL_INST_T*                   pt_cmpb_inst= NULL;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (NULL == pt_cmpb_inst || !pi4_cnt)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }    

    *pi4_cnt = pt_cmpb_inst->i4_push_buf_cnt;

    return IMTK_PB_ERROR_CODE_OK;
}





IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetAudTrackInfo_Ex(IMTK_PB_HANDLE_T                  hHandle,
                                                 uint32_t                                   u4AudioIndex,
                                                 IMTK_PB_CTRL_GET_AUD_TRACK_DETAIL_INFO_T*  ptInfo)
{
    CMPB_CTRL_INST_T*                   pt_cmpb_inst      = NULL;
    IMTK_PB_CTRL_GET_AUD_TRACK_INFO_T*  pt_aud_track_info = NULL;
    

    if (u4AudioIndex >= MAX_GET_AUD_NUM)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    if(ptInfo == NULL)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);
    if (pt_cmpb_inst)
    {
        pt_aud_track_info = &(pt_cmpb_inst->t_audio_info.at_get_info[u4AudioIndex]);
        ptInfo->eAudEnc = pt_aud_track_info->eAudEnc;
        ptInfo->eChNum  = pt_aud_track_info->eChNum;
        ptInfo->eSampleRate  = pt_aud_track_info->eSampleRate;
        ptInfo->u4BitRate  = pt_aud_track_info->u4BitRate;
        CMPB_STRNCPY((CHAR*)(ptInfo->s_lang), 
                pt_cmpb_inst->t_audio_info.at_get_audio_language[u4AudioIndex], 
                ISO_639_LANG_LEN);
                                    
        return IMTK_PB_ERROR_CODE_OK;
    }
    
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_GetTick(IMTK_PB_HANDLE_T   hHandle, 
                                         uint32_t*          pu4Tick)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    SIZE_T                      z_size;

    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst || (!pt_cmpb_inst->fg_play_normal))
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    
    z_size = sizeof(UINT32);
    
    i4_ret = CMPB_SVCTX_GET(
                    pt_cmpb_inst->h_svctx_hdl,
                    SVCTX_MM_GET_CURRENT_TICK,
                    (VOID*)pu4Tick,
                    &z_size
                    );

    if (i4_ret != SVCTXR_OK)
    {
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    return IMTK_PB_ERROR_CODE_OK;
}
                                                 
IMTK_PB_ERROR_CODE_T IMtkPb_Ctrl_TickSeek(IMTK_PB_HANDLE_T   hHandle, 
                                          uint32_t           u4Tick)
{
    CMPB_CTRL_INST_T*           pt_cmpb_inst = NULL;
    INT32                       i4_ret;
    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
    MM_SVC_POS_INFO_T           t_pos;
    
    if(!_cmpb_ctrl_uop_cab_check(hHandle,CMPB_UOP_TYPE_SEEK))
    {
         return IMTK_PB_ERROR_CODE_NOT_OK;
    } 
    _cmpb_get_inst_by_handle(&pt_cmpb_inst, hHandle);

    if (NULL == pt_cmpb_inst || (!pt_cmpb_inst->fg_play_normal))
    {
        return e_ret_code;
    }

    do
    {
        i4_ret = CMPB_SEMA_LOCK(pt_cmpb_inst->h_sema, X_SEMA_OPTION_WAIT); 
        
        if(OSR_OK != i4_ret)
        {
            break;
        }
                        
        CMPB_MEMSET(&t_pos, 0, sizeof(MM_SVC_POS_INFO_T));

        t_pos.t_pos_type    = MM_SVC_POS_TYPE_ALL_TIME;
        t_pos.ui2_idx       = (UINT16)0;
        t_pos.ui8_pos_val   = (UINT64)u4Tick;
        
        pt_cmpb_inst->fg_during_seek = TRUE;

        i4_ret = CMPB_SVCTX_MEDIA_SEEK(
                        pt_cmpb_inst->h_svctx_hdl,
                        _cmpb_ctrl_media_play_nfy_fct,
                        (VOID*)pt_cmpb_inst,
                        &t_pos
                        );
                        
        if(SVCTXR_OK != i4_ret && SVCTXR_ASYNC_NFY != i4_ret)
        {
            pt_cmpb_inst->fg_during_seek = FALSE;
            CMPB_DBG_ERROR(("CMPB_SVCTX_MEDIA_SEEK() returns %d\n", i4_ret));
            break;
        }

        e_ret_code = IMTK_PB_ERROR_CODE_OK;
    } while(0);

    i4_ret = CMPB_SEMA_UNLOCK(pt_cmpb_inst->h_sema);  
    
    if(OSR_OK != i4_ret)
    {        
    }

    return e_ret_code;
}

