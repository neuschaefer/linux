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
 * $RCSfile: x_mm_sbtl_engine.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         This header file contains Subtitle Stream handler specific 
 *         events.
 *---------------------------------------------------------------------------*/

#ifndef _X_MM_SBTL_ENGINE_H_
#define _X_MM_SBTL_ENGINE_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#include "x_mm_common.h"
#include "u_mm_sbtl_engine.h"

#else /*__KERNEL__*/

#include "x_mm_common.h"
#include "u_mm_sbtl_engine.h"

#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @struct SBTL_CFG_T
 *  @brief This struct defines the subtitle config information
 *  @code
 *  typedef struct _SBTL_CFG_T
 *  {
 *      MM_SBTL_TYPE_T                 e_sbtl_type;
 *      MM_SBTL_DIVX_TYPE_T            e_divx_sbtl_type;
 *      BOOL                        b_divx_hd;
 *      UINT8                       ui1_ovs_top;
 *      UINT8                       ui1_ovs_bottom;
 *      UINT8                       ui1_ovs_left;
 *      UINT8                       ui1_ovs_right;
 *      GL_RECT_T                   t_rect_display;
 *      GL_RECT_T                   t_rect_video;
 *      GL_SIZE_T                   t_video_resolution;
 *      SCDB_SUBTITLE_DIVX_T        t_subtitle_avi;
 *      STREAM_ID_T                 t_stream_id;
 *      UINT16                      ui2_menu_item;
 *      UINT64                      ui8_codec_private_len;
 *      UINT8                       aui1_codec_private[MM_MAX_MKV_CODEC_PRIVATE_LENGTH];
 *      UINT8                       ui1_encoding_count;
 *      SBTL_MKV_CONTENT_ENCODING_T at_content_encoding[MM_MAX_MKV_CONTENT_ENCODING_COUNT];
 *      GL_RECT_T                   t_rect_osd;
 *      BOOL                        b_rect_osd;
 *      WGL_INSET_T                 t_inset;
 *      GL_POINT_T                  t_pos_adjust;
 *      WGL_COLOR_INFO_T            t_fg_color_info;
 *      WGL_COLOR_INFO_T            t_bg_color_info;
 *      WGL_FONT_INFO_T             t_font_info;
 *      UINT8                       ui1_text_alignment;
 *      INT32                       i4_speed;
 *      INT32                       i4_pts_offset;
 *      UINT32                      ui4_video_scale; 
 *      UINT32                      ui4_video_rate; 
 *      UINT32                      ui4_stream_id;
 *      HANDLE_T                    h_connection;
 *      HANDLE_T                    h_source;
 *      HANDLE_T                    h_gl_plane;
 *      HANDLE_T                    h_video;
 *      SBTL_DISPLAY_INFO_CMD_T     t_display_info;
 *      CHAR                        s_prefered_lang_list[MM_SBTL_PREFERED_LANG_LIST_LEN];
 *      MEDIA_FEEDER_T*             pt_feeder[MM_SBTL_FEEDER_MAX];
 *  } SBTL_CFG_T;
 *  @endcode
 *  @li@c  e_sbtl_type                                       -Subtitle type 
 *  @li@c  e_divx_sbtl_type                                  -Divx subtitle type
 *  @li@c  b_divx_hd                                         -Divx subtitle is hd or not
 *  @li@c  ui1_ovs_top                                       -The top of overscan
 *  @li@c  ui1_ovs_bottom                                    -The bottom of overscan
 *  @li@c  ui1_ovs_left                                      -The left of overscan
 *  @li@c  ui1_ovs_right                                     -The right of overscan
 *  @li@c  t_rect_display                                    -The subtitle display rectang
 *  @li@c  t_rect_video                                      -The video retang
 *  @li@c  t_video_resolution                                -The video reolution
 *  @li@c  t_subtitle_avi                                    -The subtitle avi information
 *  @li@c  t_stream_id                                       -The stream id
 *  @li@c  ui2_menu_item                                     -The menu item for menu divx subtitle
 *  @li@c  ui8_codec_private_len                             -The private codec length
 *  @li@c  aui1_codec_private[MM_MAX_MKV_CODEC_PRIVATE_LENGTH]    -The private codec information
 *  @li@c  ui1_encoding_count                                -Encoding count
 *  @li@c  at_content_encoding[MM_MAX_MKV_CONTENT_ENCODING_COUNT]    -The encoding content information
 *  @li@c  t_rect_osd                                        -The osd rectang
 *  @li@c  b_rect_osd                                        -True: the osd rectang is valid;otherwise:not valid
 *  @li@c  t_inset                                           -The inset rectang
 *  @li@c  t_pos_adjust                                      -The subtitle adjust position
 *  @li@c  t_fg_color_info                                   -The foreground color information
 *  @li@c  t_bg_color_info                                   -The background color informaiton
 *  @li@c  t_font_info                                       -The font information
 *  @li@c  ui1_text_alignment                                -The subtitle alignment type
 *  @li@c  i4_speed                                          -The subtitle display speed information
 *  @li@c  i4_pts_offset                                     -The pts offset value
 *  @li@c  ui4_video_scale                                   -The video scale value
 *  @li@c  ui4_video_rate                                    -The video rate value
 *  @li@c  ui4_stream_id                                     -The stream id
 *  @li@c  h_connection                                      -The connection handle
 *  @li@c  h_source                                          -The source handle
 *  @li@c  h_gl_plane                                        -The plane handle
 *  @li@c  h_video                                           -The video handle
 *  @li@c  t_display_info                                    -The display information
 *  @li@c  s_prefered_lang_list[MM_SBTL_PREFERED_LANG_LIST_LEN]    -Prefered language list  information
 *  @li@c  pt_feeder[MM_SBTL_FEEDER_MAX]                     -The subtitle feeder information
 */
/*------------------------------------------------------------------*/
typedef struct _SBTL_CFG_T
{
    MM_SBTL_TYPE_T                 e_sbtl_type;
    MM_SBTL_DIVX_TYPE_T            e_divx_sbtl_type;
    MM_SBTL_ENCODING_T             e_encoding;
    UINT32                         ui4_cmd_mask;

    /*for divx subtitle only*/
    BOOL                        b_divx_hd;
    /*overscan*/
    UINT8                       ui1_ovs_top;
    UINT8                       ui1_ovs_bottom;
    UINT8                       ui1_ovs_left;
    UINT8                       ui1_ovs_right;
    GL_RECT_T                   t_rect_display;
    GL_RECT_T                   t_rect_video;
    GL_SIZE_T                   t_video_resolution;
    SCDB_SUBTITLE_DIVX_T        t_subtitle_avi;
    SCDB_SUBTITLE_PS_T          t_subtitle_ps;
    
    SCDB_SUBTITLE_MP4_T         t_subtitle_mp4;
    STREAM_ID_T                 t_stream_id;
    /*for divx menu subtitle only*/
    UINT16                      ui2_menu_item;

    /*for MKV subtitle only*/
    UINT64                      ui8_codec_private_len;
    UINT8                       aui1_codec_private[MM_MAX_MKV_CODEC_PRIVATE_LENGTH];
    UINT8                       ui1_encoding_count;
    SBTL_MKV_CONTENT_ENCODING_T at_content_encoding[MM_MAX_MKV_CONTENT_ENCODING_COUNT];
    
    /*common*/
    GL_RECT_T                   t_rect_osd;
    BOOL                        b_rect_osd;
    WGL_INSET_T                 t_inset;
    GL_POINT_T                  t_pos_adjust;
    WGL_COLOR_INFO_T            t_fg_color_info;
    WGL_COLOR_INFO_T            t_bg_color_info;
    WGL_COLOR_INFO_T            t_edge_color_info;
    WGL_FONT_INFO_T             t_font_info;
    WGL_COLOR_INFO_T            t_font_bg_color;
    UINT8                       ui1_text_alignment;
    MM_SBTL_DISP_PAGE_T         t_disp_page;
    MM_SBTL_TM_OFST_INF         t_time_ofst;
    BOOL                        b_hide;

    /*common*/
    INT32                       i4_speed;
    INT32                       i4_pts_offset;
    UINT32                      ui4_video_scale; 
    UINT32                      ui4_video_rate; 
    BOOL                        b_has_strm_id;  /*1:the ui4_stream_id has been setted;0:not setted*/
    UINT32                      ui4_stream_id;
    
    HANDLE_T                    h_connection;
    HANDLE_T                    h_source;
    HANDLE_T                    h_gl_plane;
    HANDLE_T                    h_video;

    SBTL_DISPLAY_INFO_CMD_T     t_display_info;

    SBTL_LYRIC_INFO_T           t_lyric;

    x_mm_sbtl_pts_cb            pf_pts_cb;
    VOID*                       pv_pts_cb_tag;

    CHAR                        s_prefered_lang_list[MM_SBTL_PREFERED_LANG_LIST_LEN];
    MEDIA_FEEDER_T*             pt_feeder[MM_SBTL_FEEDER_MAX];
    BOOL                        b_is_http_file;
    CHAR                        s_dest_grp_name[RM_NAME_LEN + 1];
} SBTL_CFG_T;



extern INT32 x_sbtl_engine_init(VOID);

extern INT32 x_sbtl_engine_open( SBTL_CFG_T*            pt_cfg,
                                 BOOL                   b_auto_play,
                                 sbtl_state_notify_fct  pf_state_nfy,
                                 VOID*                  pv_sm_nfy_tag,
                                 VOID**                 ppv_engine);

extern INT32 x_sbtl_engine_close( VOID*                 pv_engine);

extern INT32 x_sbtl_engine_set(VOID*                    pt_engine,
                               MM_EVN_TYPE_T            e_event,
                               VOID*                    pv_set_info,
                               SIZE_T                   z_set_info_len);

extern INT32 x_sbtl_engine_get(VOID*                    pv_engine,
                               MM_EVN_TYPE_T            e_event,
                               VOID*                    pv_get_info,
                               SIZE_T                   z_get_info_len);
#endif /* _X_MM_SBTL_ENGINE_H_ */



