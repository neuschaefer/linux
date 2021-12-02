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
 * $RCSfile: sbtl_utils.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         
 *         
 *---------------------------------------------------------------------------*/

#ifndef _SBTL_UTILS_H_
#define _SBTL_UTILS_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#include "mutil/sbtl_engine/sbtl_struct.h"
#include "res_mngr/drv/x_demux.h"
#include "res_mngr/drv/x_stc_ctrl.h"
#else /*__KERNEL__*/
#include "sbtl_struct.h"
#include "x_demux.h"
#include "x_stc_ctrl.h"
#endif /*__KERNEL__*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#ifndef MIN
#define MIN(x, y)           (((x) < (y)) ? (x) : (y))    /**< macro for finding minimum */
#endif

#ifndef MAX
#define MAX(x, y)           (((x) > (y)) ? (x) : (y))    /**< macro for finding maximum */
#endif

#define SBTL_RECT_EMPTY(rect) \
            (((rect)->i4_right <= (rect)->i4_left) || ((rect)->i4_bottom <= (rect)->i4_top))    /**< check if this rectangle is empty */

#define SBTL_RECT_W(rect)    ((rect)->i4_right - (rect)->i4_left)    /**< get rectangle width */
#define SBTL_RECT_H(rect)    ((rect)->i4_bottom - (rect)->i4_top)    /**< get rectangle height */

#define SBTL_SET_RECT(rect, l, t, r, b)  \
    do {                            \
        (rect)->i4_left     = (l);  \
        (rect)->i4_top      = (t);  \
        (rect)->i4_right    = (r);  \
        (rect)->i4_bottom   = (b);  \
    } while (0)    /**< setup a rectangle by coordinates */

#define SBTL_SET_RECT_BY_SIZE(rc, l, t, w, h)        \
    do {                                \
        (rc)->i4_left   = (l);          \
        (rc)->i4_top    = (t);          \
        (rc)->i4_right  = (l) + (w);    \
        (rc)->i4_bottom = (t) + (h);    \
    } while (0)    /**< setup a rectangle by size */

#define SBTL_INTERSECT_RECT(rc_out, rc1, rc2)                                \
    do {                                                                \
        (rc_out)->i4_left   = MAX((rc1)->i4_left, (rc2)->i4_left);      \
        (rc_out)->i4_right  = MIN((rc1)->i4_right, (rc2)->i4_right);    \
        (rc_out)->i4_top    = MAX((rc1)->i4_top, (rc2)->i4_top);        \
        (rc_out)->i4_bottom = MIN((rc1)->i4_bottom, (rc2)->i4_bottom);  \
    } while (0)    /**< intersect two rectangles as rc_out */
    
typedef enum
{
    MM_SBTL_HDLR_MSG_IGNORE = 0,
    MM_SBTL_HDLR_MSG_STATE_EVENT,
    MM_SBTL_HDLR_MSG_DMX_EVENT,
    MM_SBTL_HDLR_MSG_PTS_EVENT,
    MM_SBTL_HDLR_MSG_TIMER_EVENT,
    MM_SBTL_HDLR_MSG_CONFIG_EVENT,
    MM_SBTL_HDLR_MSG_GET_EVENT
} MM_SBTL_HDLR_MSG_TYPE_T;

typedef struct{    
    UINT32 ui4_data_1;    
    UINT32 ui4_data_2;    
    UINT32 ui4_pts;    
    UINT32 ui4_duration;
} MKV_PACK_T;

typedef struct _MM_SBTL_HDLR_MSG_STATE_EVENT_T
{
    UINT32                  ui4_event;
} MM_SBTL_HDLR_MSG_STATE_EVENT_T;

typedef struct _MM_SBTL_HDLR_MSG_DMX_EVENT_T
{
    DEMUX_COND_T            e_nfy_cond;
    UINT32                  ui4_data_1;
    UINT32                  ui4_data_2;
    UINT32                  ui4_pts;    
    UINT32                  ui4_duration;
    PTS_T                   t_stc;
} MM_SBTL_HDLR_MSG_DMX_EVENT_T;

typedef struct _MM_SBTL_HDLR_MSG_PTS_EVENT_T
{
    PTS_T                   t_pts;
    BOOL                    b_cycle;
    UINT32                  ui4_stc;
} MM_SBTL_HDLR_MSG_PTS_EVENT_T;

typedef struct _MM_SBTL_HDLR_MSG_CONFIG_EVENT_T
{
    MM_EVN_TYPE_T         e_set_type;
    VOID*                 pv_set_info;
    SIZE_T                z_set_info_len;
} MM_SBTL_HDLR_MSG_CONFIG_EVENT_T;

typedef struct _MM_SBTL_HDLR_MSG_GET_EVENT_T
{
    MM_EVN_TYPE_T         e_get_type;
    VOID*                 pv_get_info;
    SIZE_T                z_get_info_len;
} MM_SBTL_HDLR_MSG_GET_EVENT_T;


typedef struct
{
    VOID*                           pv_mngr;
    VOID*                           pv_sbtl_engine;
    MM_SBTL_HDLR_MSG_TYPE_T         e_msg_type;

    union
    {
        MM_SBTL_HDLR_MSG_STATE_EVENT_T          t_msg_state_event;
        MM_SBTL_HDLR_MSG_DMX_EVENT_T            t_msg_dmx_event;
        MM_SBTL_HDLR_MSG_PTS_EVENT_T            t_msg_pts_event;
        MM_SBTL_HDLR_MSG_CONFIG_EVENT_T         t_msg_config_event;
        MM_SBTL_HDLR_MSG_GET_EVENT_T            t_msg_get_event;
    } u;
} MM_SBTL_HDLR_MSG_T;


#define mm_sbtl_hdlr_send_state_msg(t_msg, pv_engine, mui4_event)   \
            t_msg.e_msg_type = MM_SBTL_HDLR_MSG_STATE_EVENT;        \
            t_msg.pv_sbtl_engine = pv_engine;                       \
            t_msg.u.t_msg_state_event.ui4_event = mui4_event;       \
            mm_sbtl_hdlr_send_msg(&t_msg)

#define mm_sbtl_hdlr_send_dmx_msg(t_msg, pv_engine, me_nfy_cond, mui4_data_1, mui4_data_2, mui4_pts, mui4_duration, mt_stc) \
            t_msg.e_msg_type = MM_SBTL_HDLR_MSG_DMX_EVENT;          \
            t_msg.pv_sbtl_engine = pv_engine;                       \
            t_msg.u.t_msg_dmx_event.e_nfy_cond = me_nfy_cond;       \
            t_msg.u.t_msg_dmx_event.ui4_data_1 = mui4_data_1;       \
            t_msg.u.t_msg_dmx_event.ui4_data_2 = mui4_data_2;       \
            t_msg.u.t_msg_dmx_event.ui4_pts = mui4_pts;             \
            t_msg.u.t_msg_dmx_event.ui4_duration = mui4_duration;   \
            t_msg.u.t_msg_dmx_event.t_stc = mt_stc;                 \
            mm_sbtl_hdlr_send_msg(&t_msg)

#define mm_sbtl_hdlr_send_pts_msg(t_msg, pv_engine, mt_pts, mb_cycle, mui4_stc) \
            t_msg.e_msg_type = MM_SBTL_HDLR_MSG_PTS_EVENT;          \
            t_msg.pv_sbtl_engine = pv_engine;                       \
            t_msg.u.t_msg_pts_event.t_pts = mt_pts;                 \
            t_msg.u.t_msg_pts_event.b_cycle = mb_cycle;             \
            t_msg.u.t_msg_pts_event.ui4_stc = mui4_stc;             \
            mm_sbtl_hdlr_send_msg(&t_msg)

#define mm_sbtl_hdlr_send_timer_msg(t_msg, mpv_mngr) \
            t_msg.e_msg_type = MM_SBTL_HDLR_MSG_TIMER_EVENT;          \
            t_msg.pv_mngr = (VOID*)mpv_mngr;                       \
            mm_sbtl_hdlr_send_msg(&t_msg)

#define mm_sbtl_hdlr_send_config_msg(t_msg, pv_engine, me_set_type, mpv_set_info, mz_set_info_len) \
            t_msg.e_msg_type = MM_SBTL_HDLR_MSG_CONFIG_EVENT;               \
            t_msg.pv_sbtl_engine = pv_engine;                               \
            t_msg.u.t_msg_config_event.e_set_type = me_set_type;            \
            t_msg.u.t_msg_config_event.pv_set_info = mpv_set_info;          \
            t_msg.u.t_msg_config_event.z_set_info_len = mz_set_info_len;    \
            mm_sbtl_hdlr_send_msg(&t_msg)


#define mm_sbtl_hdlr_send_get_msg(t_msg, pv_engine, me_get_type, mpv_get_info, mz_get_info_len) \
            t_msg.e_msg_type = MM_SBTL_HDLR_MSG_GET_EVENT;               \
            t_msg.pv_sbtl_engine = pv_engine;                               \
            t_msg.u.t_msg_get_event.e_get_type = me_get_type;            \
            t_msg.u.t_msg_get_event.pv_get_info = mpv_get_info;          \
            t_msg.u.t_msg_get_event.z_get_info_len = mz_get_info_len;    \
            mm_sbtl_hdlr_send_msg(&t_msg)
    
/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
extern VOID mm_sbtl_hdlr_send_msg(const MM_SBTL_HDLR_MSG_T*     pt_msg);

extern INT32 sbtl_dmx_open(SBTL_ENGINE_T*      pt_engine,
                           HANDLE_T            h_source);

extern INT32 sbtl_dmx_close(SBTL_ENGINE_T*     pt_engine);

extern INT32 sbtl_dmx_enable(SBTL_ENGINE_T*    pt_engine);

extern INT32 sbtl_dmx_disable(SBTL_ENGINE_T*   pt_engine);

extern INT32 sbtl_stc_open(SBTL_ENGINE_T*      pt_engine);

extern INT32 sbtl_stc_close(SBTL_ENGINE_T*     pt_engine);

extern INT32 sbtl_stc_pts(SBTL_ENGINE_T*       pt_engine,
                          PTS_T*               pt_pts);

extern INT32 sbtl_stc_type(SBTL_ENGINE_T*      pt_engine,
                             STC_CTRL_TYPE_T*    pt_type);


extern INT32 sbtl_dmx_retrive_data(SBTL_ENGINE_T*      pt_engine,
                                   UINT32              ui4_tag,
                                   UINT32              ui4_length,
                                   UINT8*              aui1_buf);

extern INT32 sbtl_dmx_release_data(SBTL_ENGINE_T*      pt_engine,
                                   UINT32              ui4_tag,
                                   UINT32              ui4_length);

extern INT32 sbtl_dmx_data_que_open(SBTL_ENGINE_T*     pt_engine);

extern INT32 sbtl_dmx_data_que_close(SBTL_ENGINE_T*    pt_engine);

extern INT32 sbtl_dmx_data_que_append(SBTL_ENGINE_T*       pt_engine,
                                      UINT32               ui4_data_1,
                                      UINT32               ui4_data_2,
                                      UINT32               ui4_pts,
                                      UINT32               ui4_duration,
                                      PTS_T                t_stc);

extern BOOL sbtl_dmx_data_que_retrieve(SBTL_ENGINE_T*      pt_engine,
                                       UINT32*             pui4_data_1,
                                       UINT32*             pui4_data_2,
                                       UINT32*             pui4_pts,
                                       UINT32*             pui4_duration,
                                       PTS_T*              pt_stc);

extern BOOL sbtl_dmx_data_que_query(SBTL_ENGINE_T*     pt_engine,
                                    UINT32*            pui4_data_1,
                                    UINT32*            pui4_data_2,
                                    UINT32*            pui4_pts,
                                    UINT32*            pui4_duration);



extern INT32 sbtl_feeder_init(SBTL_ENGINE_T*           pt_engine,
                              MM_SBTL_FEEDER_TYPE_T    e_type);

extern INT32 sbtl_feeder_inp1byte(SBTL_ENGINE_T*           pt_engine,
                                  UINT8*                   pui1_data,
                                  MM_SBTL_FEEDER_TYPE_T    e_type);

extern INT32 sbtl_feeder_inp2byte(SBTL_ENGINE_T*           pt_engine,
                                  UINT16*                  pui2_data,
                                  MM_SBTL_FEEDER_TYPE_T    e_type);


extern INT32 _sbtl_feeder_inp3byte(SBTL_ENGINE_T*           pt_engine,
                                   UINT32*                  pui4_data,
                                   MM_SBTL_FEEDER_TYPE_T    e_type);

extern INT32 sbtl_feeder_inp4byte(SBTL_ENGINE_T*           pt_engine,
                                  UINT32*                  pui4_data,
                                  MM_SBTL_FEEDER_TYPE_T    e_type);

extern INT32 sbtl_feeder_inpnbyte(SBTL_ENGINE_T*           pt_engine,
                                  UINT32                   ui4_len,
                                  UINT8*                   aui_buf,
                                  UINT32*                  pui4_done_bytes,
                                  MM_SBTL_FEEDER_TYPE_T    e_type);

extern INT32 sbtl_feeder_setpos(SBTL_ENGINE_T*             pt_engine,
                                INT64                      i8_offset, 
                                UINT8                      ui1_whence,
                                MM_SBTL_FEEDER_TYPE_T      e_type);

extern INT32 sbtl_feeder_getpos(SBTL_ENGINE_T*             pt_engine,
                                UINT64*                    pui8_offset,
                                MM_SBTL_FEEDER_TYPE_T      e_type);

extern UINT8* sbtl_feeder_get_data_addr(
    SBTL_ENGINE_T*                  pt_engine,
    MM_SBTL_FEEDER_TYPE_T           e_type);

extern INT32 sbtl_feeder_get_file_size(
    SBTL_ENGINE_T*            pt_engine,
    UINT64*                   pui8_offset,
    MM_SBTL_FEEDER_TYPE_T     e_type);

extern INT32 sbtl_feeder_get_cache_endpos(
    SBTL_ENGINE_T*            pt_engine,
    UINT64*                   pui8_offset,
    MM_SBTL_FEEDER_TYPE_T     e_type);


extern INT32 sbtl_feeder_render_new_cache(
    SBTL_ENGINE_T*                  pt_engine,
    MM_SBTL_FEEDER_TYPE_T           e_type);

extern INT32 sbtl_feeder_inpnbyte_p(SBTL_ENGINE_T*          pt_engine,
                                     UINT32                  ui4_len,
                                     UINT8**                 pui1_buf,
                                     UINT32*                 pui4_done_bytes,
                                     MM_SBTL_FEEDER_TYPE_T   e_type);

extern INT32 sbtl_feeder_inpnbyte_addr_p(
    SBTL_ENGINE_T*                  pt_engine,
    UINT64                          ui8_start_pos,
    UINT32                          ui4_len,
    UINT8**                         pui1_buf,
    UINT32*                         pui4_done_bytes,
    MM_SBTL_FEEDER_TYPE_T           e_type);

extern UINT8 sbtl_tag_fmt_check(
    UINT8                           ui1_fmt_cnt,
    SBTL_TAG_FORMAT_T*              pt_tag_inf,
    UINT8*                          pui1_buf,
    UINT8*                          pui1_fmt_len,
    UINT8*                          pui1_fmt_idx,
    UINT32                          ui4_read_len,
    BOOL                            b_UTF16);

extern UINT32 sbtl_get_tag_fmt_max_len(
           UINT8                           ui1_fmt_cnt,
           SBTL_TAG_FORMAT_T*              pt_tag_inf);

extern INT32 sbtl_tag_inf_seek(
    SBTL_ENGINE_T*                  pt_engine,
    UINT64                          ui8_start_pos,
    UINT8                           ui1_fmt_cnt,
    SBTL_TAG_FORMAT_T*              pt_tag_inf,
    MM_SBTL_FEEDER_TYPE_T           e_type,
    BOOL                            b_found_total,
    UINT8**                         ppui1_buf,
    UINT64*                         pui8_seek_len,
    UINT16*                         pui2_count,
    UINT8*                          pui1_fmt_idx,
    BOOL                            b_UTF16);

extern INT32 sbtl_seek_tag_inf(
    SBTL_ENGINE_T*                  pt_engine,
    UINT64                          ui8_start_pos,
    SBTL_TAG_FORMAT_T*              pt_start_tag,
    SBTL_TAG_FORMAT_T*              pt_end_tag,
    UINT8**                         ppui1_buf,
    UINT64*                         pui8_offset,
    UINT64*                         pui8_len,
    MM_SBTL_FEEDER_TYPE_T           e_type,
    BOOL                            b_UTF16);

extern BOOL sbtl_check_tag_inf_by_addr(
    SBTL_ENGINE_T*                  pt_engine,
    UINT64                          ui8_start_pos,
    UINT8                           ui1_fmt_cnt,
    SBTL_TAG_FORMAT_T*              pt_tag_inf,
    MM_SBTL_FEEDER_TYPE_T           e_type,
    UINT8**                         ppui1_buf,
    UINT8*                          pui1_fmt_idx,
    BOOL                            b_UTF16);


extern UINT32 sbtl_asci2decimal(UINT8*     aui1_ary,
                                UINT8      ui1_round);

extern UINT32 sbtl_wasci2decimal(UINT16*     aui2_ary,
                                 UINT8      ui1_round);
                                
extern UINT16 sbtl_asci2cp1252(UINT8    ui1_asci);

extern UINT32 sbtl_get_utf16_string_len(UINT16* pui2_buf, UINT32 ui4_max_len);

extern INT32 sbtl_trans_to_w2s(MM_SBTL_LANG_T          e_language,
                            UINT16                  ui2_data_in,
                            UINT16*                 pui2_data_out);

extern INT32 sbtl_trans_to_w2s_ex(MM_SBTL_ENCODING_T          e_encoding,
                            UINT8*                  pui1_src,
                            UINT32                  ui4_len,
                            UINT16*                 pui2_data_out);

extern INT32 sbtl_record_init(VOID);

extern INT32 sbtl_set_encode_record(
    SBTL_ENGINE_T*                   pt_engine,
    MM_SBTL_ENCODING_T               e_encoding);

extern INT32 sbtl_set_time_ofst_record(
    SBTL_ENGINE_T*                   pt_engine,
    MM_SBTL_TM_OFST_INF*                 pt_time_ofst);

extern INT32 sbtl_set_osd_ofst_record(
    SBTL_ENGINE_T*                   pt_engine,
    GL_POINT_T*                 pt_osd_ofst);

extern INT32 sbtl_set_fg_color_record(
    SBTL_ENGINE_T*                   pt_engine,
    WGL_COLOR_INFO_T*                 pt_fg_color);

extern INT32 sbtl_set_bg_color_record(
    SBTL_ENGINE_T*                   pt_engine,
    WGL_COLOR_INFO_T*                 pt_bg_color);

extern INT32 sbtl_set_font_inf_record(
    SBTL_ENGINE_T*                   pt_engine,
    WGL_FONT_INFO_T*                 pt_font_inf);

extern INT32 sbtl_set_edge_color_record(
    SBTL_ENGINE_T*                   pt_engine,
    WGL_COLOR_INFO_T*                 pt_bg_color);

extern INT32 sbtl_check_record_inf(
    SBTL_ENGINE_T*                   pt_engine);

extern INT32 sbtl_check_encoding_info(
    SBTL_ENGINE_T*                   pt_engine,
    UINT8                            ui1_data_1,
    UINT8                            ui1_data_2,
    BOOL*                            pb_UTF16);

extern INT32 sbtl_set_record(SBTL_ENGINE_T* pt_engine);

extern INT32 sbtl_util_fcopybytes_fct(
                        VOID*       pv_obj, 
                        VOID*       pv_buf, 
                        SIZE_T      z_buf_leng, 
                        SIZE_T      z_copy_size,
                        UINT32*     pui4_size);

extern INT32 sbtl_util_fsetpos_fct(
                        VOID*        pv_obj, 
                        INT64        i8_offset, 
                        UINT8        ui1_whence);

extern INT32 sbtl_util_fgetpos_fct(
                        VOID*       pv_obj, 
                        UINT64*     pui8_cur_pos);

extern INT32 sbtl_util_finput1bytes_fct(
                        VOID*       pv_obj, 
                        UINT8*      pui1_data);

#ifdef INET_SUPPORT
extern INT32 sbtl_util_http_fcopybytes_fct(
                        VOID*       pv_obj, 
                        VOID*       pv_buf, 
                        SIZE_T      z_buf_leng, 
                        SIZE_T      z_copy_size,
                        UINT32*     pui4_size);

extern INT32 sbtl_util_http_fsetpos_fct(
                        VOID*        pv_obj, 
                        INT64        i8_offset, 
                        UINT8        ui1_whence);

extern INT32 sbtl_util_http_fgetpos_fct(
                        VOID*       pv_obj, 
                        UINT64*     pui8_cur_pos);

extern INT32 sbtl_util_http_finput1bytes_fct(
                        VOID*       pv_obj, 
                        UINT8*      pui1_data);
#endif
extern BOOL sbtl_cmp_string(
    UINT16*             pui2_buf,
    UINT16              ui2_len,
    CHAR*               s_string);
#endif /* _SBTL_UTILS_H_ */





