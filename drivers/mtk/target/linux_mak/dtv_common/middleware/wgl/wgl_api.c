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
 * Description: 
 *          This file contains the implementation of exported APIs of the 
 *          Widget Library.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "dbg/dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "inc/common.h"
#include "inc/x_common.h"
#include "os/inc/os.h"
#include "handle/handle.h"
#include "res_mngr/rm.h"
#include "res_mngr/drv/x_irrc_btn_def.h"
#include "io_mngr/x_iom.h"
#include "graphic/gl.h"
#include "graphic/gl_surface.h"
#include "graphic/gl_draw.h"
#include "graphic/gl_plane.h"
#if 0
#include "graphic/util/x_MemPool.h"
#endif

#include "util/x_lnk_list.h"
#include "wgl/x_wgl_msg.h"
#include "wgl/_wgl.h"
#include "wgl/_wgl_cli.h"
#include "wgl/wgl_tools.h"
#include "wgl/animation/u_wgl_animation.h"
#include "wgl/wgl_anim.h"
#include "wgl/wgl.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/wgl_sets.h"

#include "os/inc/x_os.h"
#include "wgl/arabic.h"

#ifdef WGL_COORDINATE_INVERSE
#include "wgl/listbox/u_wgl_lb.h"
#endif

#ifdef CLI_LVL_ALL
#include "graphic/u_gl_nc.h"
#endif

#ifdef TIME_MEASUREMENT
#include "time_msrt/x_time_msrt.h"

#ifdef TMS_MODULE_LEVEL
#undef TMS_MODULE_LEVEL
#endif

#define TMS_MODULE_LEVEL    TMS_LEVEL_OFF_LINE

#endif

#ifndef DRV_SUPPORT_EXTMJC_MODE3
//#define OSD_LINE_SEPARATE_COMPRESSION
#endif

#ifdef LINUX_TURNKEY_SOLUTION
#undef SYS_3D_SUPPORT
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define WGL_ARAB_BUFFER_DEFAULT_SIZE    (2048)
static UTF16_T*           gpw2s_str = NULL;     /* for arabic string after reorder   */
static INT32              gi4_buffer_size = 0;
static HANDLE_T           h_arab_buffer_lock = NULL_HANDLE;
#ifdef TIME_MEASUREMENT
#define PAINT_ALL           "PAINT ALL"
#define PAINT_BACKGROUND    "PAINT BACKGROUND"
#define PAINT_FRONT_TO_BACK "PAINT FRONT to BACK"
#define PAINT_SINGLE_WIDGET "PAINT SINGLE WIDGET"
#define PAINT_COMPOSITION   "PAINT COMPOSITION"
#define PAINT_BACK_TO_FRONT "PAINT BACK TO FRONT"
#endif

#define WM_DEFAULT_NUM_WIDGETS          ((UINT16)   16)

#define WM_DEFAULT_SURFACE_WIDTH        ((UINT32) 1280)
#define WM_DEFAULT_SURFACE_HEIGHT       ((UINT32)  720)
#define WM_DEFAULT_SURFACE_MODE         (GL_COLORMODE_ARGB_D4444)

#define RC_KEY_GROUPS_MASK(kg)          (((UINT64) 1) << ((((UINT32)(kg)) >> 16) - 1))

#define WM_DEFAULT_RC_APP_KEY_GROUPS    KEY_GROUP_NON_FCT_CTRL

#define WM_MAX_FLOATED_WIDGET   ((UINT8) 4)

#define REFERENCE(pc)           ((pc)->pt_ref)
#define SHADOW(pc)              ((pc)->pt_shadow)
#define IS_SINGLE(pc)           ((NULL == REFERENCE(pc)) && (NULL == SHADOW(pc)))
#define IS_REFERENCE(pc)        ((NULL == REFERENCE(pc)) && (NULL != SHADOW(pc)))
#define IS_SHADOW(pc)           ((NULL != REFERENCE(pc)) && (NULL == SHADOW(pc)))

#define PARENT(pw)              ((pw)->pt_parent)
#define SIBLING(pw)             ((pw)->pt_sibling)
#define CHILD(pw)               ((pw)->pt_child)
#define BRANCH(pw)              ((pw)->pt_branch)
#define MSGPROC(pw)             ((pw)->pf_wdgt_proc)
#define DFLT_MSGPROC(pw)        ((pw)->pt_fct_tbl->pf_msg_proc)
#define STATE(pw)               ((pw)->ui2_state)
#define ALPHA(pw)               ((pw)->ui1_alpha)
#define DIRTY(pw)               ((pw)->t_rc_dirty)
#define ANIM(pw)                ((pw)->t_anim)

#define FIND_PREV(prev, obj) \
        while (SIBLING(prev) != (obj)) { (prev) = SIBLING(prev); }
#define FIND_PREV_WITH_STATE(prev, obj, flag) \
        while (SIBLING(prev) != (obj) && !(STATE(SIBLING(prev)) & (flag))) { (prev) = SIBLING(prev); }

#define CALL_MSGPROC(pw, msg, p1, p2)   \
            (MSGPROC(pw)((pw)->h_this, (msg), (p1), (p2)))
#define CALL_DFLT_MSGPROC(pw, msg, p1, p2)   \
            (DFLT_MSGPROC(pw)((pw)->h_this, (msg), (p1), (p2)))

#define ST_VISIBLE(pw)          (STATE(pw) & WS_VISIBLE)
#define ST_ENABLED(pw)          (STATE(pw) & WS_ENABLED)
#define ST_ACTIVATED(pw)        (STATE(pw) & WS_ACTIVATED)
#define ST_DESTROYED(pw)        (STATE(pw) & WS_DESTROYED)
#define ST_IGNORED(pw)          (STATE(pw) & WS_IGNORED)
#define ST_MODIFIED(pw)         (STATE(pw) & WS_MODIFIED)
#define ST_FLOATED(pw)          (STATE(pw) & WS_FLOATED)

#define OWNER(pb)               ((pb)->pt_owner_app)
#define CANVAS(pb)              ((pb)->pt_canvas)
#define FIRST(pb)               ((pb)->pt_first)

#define ACTIVATED(pa)           ((pa)->pt_widget_activated)
#define FOCUSED(pa)             ((pa)->pt_widget_focused)

#define SIBLING_HEAD(pw)        (CHILD(PARENT(pw)))
#define GET_HEAD(pw)            (AT_TOP_LEVEL(pw) ? FIRST(BRANCH(pw)) : SIBLING_HEAD(pw))
#define AT_TOP_LEVEL(pw)        (!PARENT(pw))
#define AT_SIBLING_HEAD(pw)     ((pw) == GET_HEAD(pw))
#define AT_SIBLING_TAIL(pw)     (!SIBLING(pw))

#define GET_HI_WORD(data)       ((UINT16)(((UINT32) (data)) >> 16))
#define GET_LO_WORD(data)       ((UINT16)(((UINT32) (data)) & 0xffff))
#define COMBINE_HI_LO(h, l)     (((h) << 16) | ((l) & 0xffff))

#define RPM_MSG_PAINT           ((UINT32) 0)
#define RPM_MSG_DEL_SINGLE_TREE ((UINT32) 1)
#define RPM_MSG_DEL_MULTI_TREE  ((UINT32) 2)
#define RPM_MSG_DEL_APP         ((UINT32) 3)
#define RPM_MSG_NOTIFY_APP      ((UINT32) 4)
#define RPM_MSG_TIMER           ((UINT32) 5)
#define RPM_MSG_SHADOW_UNLOCK   ((UINT32) 6)

#define TM_ST_STARTED(f)        ((f) & TIMER_STATUS_STARTED)
#define TM_MD_ASYNC(f)          ((f) & TIMER_MODE_ASYNC)

/* memory allocation/deallocation routine */
#define MALLOC_CANVAS           _malloc_canvas
#define MALLOC_WIDGET           _malloc_widget
#define MALLOC_BRANCH           _malloc_branch
#define MALLOC_APP_INFO         _malloc_app_info
#define MALLOC_TIMER            _malloc_timer
#define MALLOC_CANVAS_CALLBACK  _malloc_canvas_callback

#define MFREE_CANVAS(p)             _mfree_canvas(p)
#define MFREE_WIDGET(p)             _mfree_widget(p)
#define MFREE_BRANCH(p)             _mfree_branch(p)
#define MFREE_APP_INFO(p)           _mfree_app_info(p)
#define MFREE_TIMER(p)              _mfree_timer(p)
#define MFREE_CANVAS_CALLBACK(p)    _mfree_canvas_callback(p)

#ifdef CLI_LVL_ALL
#define FLASH_AREA(surf, rc, tm)    _flash_area(surf, rc, tm)
#else
#define FLASH_AREA(surf, rc, tm)
#endif

#define INIT_APP_LIST_ELEMENT(app_element)          \
{                                                   \
    (app_element)->ui4_wdgt_count = 0;              \
    SLIST_INIT(&((app_element)->t_wdgt_list));      \
}

#ifdef WGL_ANIM_SUPPORT    

typedef enum __ANIM_MSG_TYPE_T
{
    ANIM_MSG_UNKNOWN,
    ANIM_MSG_TICK
} _ANIM_MSG_TYPE_T;

typedef struct __ANIM_MSG_T
{
    _ANIM_MSG_TYPE_T    e_type;
} _ANIM_MSG_T;

typedef struct _ANIM_TICK_MSG_DATA_T
{
    UINT32 ui4_tick;
    UINT8 ui1_app_idx;
} _ANIM_TICK_MSG_DATA_T;

typedef struct __ANIM_WDGT_LIST_T
{
    HANDLE_T                            h_widget;
    WGL_ANIM_ATTR_TYPE                  e_anim_attr_type;                  
    GL_RECT_T                           t_rect_scale_rect;
    SLIST_ENTRY_T(__ANIM_WDGT_LIST_T)   t_link;
} _ANIM_WDGT_LIST_T;

typedef struct __ANIM_APP_LIST_T
{
   SLIST_T(__ANIM_WDGT_LIST_T)      t_wdgt_list;
   SLIST_ENTRY_T(__ANIM_APP_LIST_T) t_link;
   UINT32                           ui4_wdgt_count;
   _APP_INFO_T                      *pt_app_info;
   WGL_ANIM_CONTEXT_T               e_anim_context;
   BOOL                             b_been_dispatched;
   BOOL                             b_any_wdgt_anim_running;
   UINT32                           ui4_tick;
} _ANIM_APP_LIST_T;

typedef struct __ANIM_MNGR_T
{
    HANDLE_T                    h_anim_mngr;
    HANDLE_T                    h_anim_msg_q;
    SLIST_T(__ANIM_APP_LIST_T)  t_app_list;
    SLIST_T(__ANIM_WDGT_LIST_T) t_wdgt_list;/*this list for both sync and async animation widget, just in order to store t_rect_scale_rect for every animationed wiget if needs*/
    
    UINT32                      ui4_app_count;

    UINT32                      ui4_tick_interval;
    WGL_TICK_MODE_T             e_tick_mode;

    _CANVAS_T                   *apt_canvas[WGL_MAX_CANVAS_RECORD_NUM];
    UINT32                      ui4_canvas_num;

    GL_RECT_T                   at_update_rect[WGL_MAX_UPDATE_RECT_NUM];
    UINT32                      ui4_update_rect_num;
} _ANIM_MNGR_T;
#endif

typedef struct __REPAINT_MNGR_T
{
    HANDLE_T    h_thread;
    HANDLE_T    h_msg_q;
} _REPAINT_MNGR_T;

typedef struct __RPM_MSG_T
{
    UINT32      ui4_msg;
} _RPM_MSG_T;

typedef struct __DESTROY_MSG_T
{
    UINT32              ui4_msg;
    _CANVAS_T*          pt_canvas;
    _WIDGET_T*          pt_widget;
    BOOL                b_repaint;
} _DESTROY_MSG_T;

typedef struct __DESTROY_APP_MSG_T
{
    UINT32              ui4_msg;
    _APP_INFO_T*        pt_app_info;
 } _DESTROY_APP_MSG_T;

typedef struct __PAING_MSG_T
{
    UINT32      ui4_msg;
    _CANVAS_T*  pt_canvas;
    GL_RECT_T   t_update;
} _PAINT_MSG_T;

typedef struct __APP_NOTIFY_MSG_T
{
    UINT32      ui4_msg;
    _CANVAS_T*  pt_canvas;
    UINT32      ui4_wmsg;
    VOID*       pv_param1;
    VOID*       pv_param2;
} _APP_NOTIFY_MSG_T;

typedef struct __TM_MSG_T
{
    UINT32      ui4_msg;
    HANDLE_T    h_timer;
} _TM_MSG_T;

typedef struct
{
    GL_RECT_T       *pt_rect;
    UINT32          ui4_rect_count;
    HANDLE_T        h_canvas;
    GL_RECT_T       *pt_exclusive_rect;
    UINT32          ui4_exclusive_count;
} _TRANSP_REGION_T;

typedef struct __PAINT_WIDGET_INFO_T
{
    _WIDGET_T*  pt_widget;
    UINT16      ui2_state;
    INT32       i4_x;
    INT32       i4_y;
} _PAINT_WIDGET_INFO_T;

typedef struct __PAINT_INFO_T
{
    GL_HGRAPHICS_T          h_graphics;
    GL_HGRAPHICS_T          h_graphics_target;
    GL_HSURFACE_T           h_surf_target;
    INT32                   i4_trans_x;
    INT32                   i4_trans_y;
    UINT16                  ui2_state;
    UINT8                   ui1_num_floated;
    GL_RECT_T               *at_rc_global;
    _PAINT_WIDGET_INFO_T    *at_pwi;
} _PAINT_INFO_T;

#define RPM_MSG_SIZE                (sizeof(_PAINT_MSG_T))  /* need to be the size of the largest message structure */
#define RPM_MSG_PRIORITY_DESTROY    128
#define RPM_MSG_PRIORITY_PAINT      128
#define RPM_MSG_PRIORITY_NOTIFY     128
#define RPM_MSG_PRIORITY_TIMER      128
#define RPM_MSG_PRIORITY_SHDW_UNLCK 128

#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE            wgl_get_dbg_level()
#else
#define DBG_LEVEL_MODULE            DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

#ifdef OSD_LINE_SEPARATE_COMPRESSION
    #define COMPRESSION_EXTRA_LINES     (10)
#else
    #define COMPRESSION_EXTRA_LINES     (2)
#endif

#ifdef WGL_NPM_WIDGET_SUPPORT
extern INT32 wgl_npm_mem_init(VOID);
#endif
/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/
static HANDLE_T         _h_hrc_lock;
static HANDLE_T         _h_wgl_lock;
static HANDLE_T         _h_inst_lock;

static HANDLE_T         _h_input;

static _APP_INFO_T*     _pt_active_app;

DLIST_APP_T      _t_app_lst;
SLIST_CANVAS_T   _t_canvas_lst;

static _REPAINT_MNGR_T  _t_repaint_mngr;

static INT32            _i4_active_cnt;

/* animation data */
static HANDLE_T         _h_app_list_lock;
static HANDLE_T         _h_app_element_lock;
#ifdef WGL_ANIM_SUPPORT    
static _ANIM_MNGR_T     _t_anim_mngr;
static BOOL             _b_anim_rc_init = FALSE;
#endif
#if 0
static UINT16           _ui2_max_widget;
#endif

#ifdef WGL_COORDINATE_INVERSE
static BOOL             _b_coordinate_reverse = FALSE;
#endif 

static UINT32           _ui4_surf_width       = WM_DEFAULT_SURFACE_WIDTH;
static UINT32           _ui4_surf_height      = WM_DEFAULT_SURFACE_HEIGHT;
static GL_COLORMODE_T   _e_surf_mode          = WM_DEFAULT_SURFACE_MODE;
static BOOL             _b_auto_scale         = TRUE;
static BOOL             _b_inter_compose      = TRUE;
static BOOL             _b_support_compose_anim = FALSE;
static BOOL             _b_compressed_compose = FALSE;
static BOOL             _b_osd_compress       = FALSE;
static GL_COLOR_T       _t_color_key          = {0, {0}, {0}, {0}};
static GL_HPALETTE_T    _h_sys_pal            = NULL_HANDLE;

static UINT8                    _ui1_max_floated_widget = WM_MAX_FLOATED_WIDGET;
static wgl_app_callback_fct     _pf_app_ignore_key_fct  = NULL;
static wgl_widget_callback_fct  _pf_widget_ignore_key_fct  = NULL;

static GL_COMPRESSION_RATIO_T   _e_compression_ratio    = GL_LOSSY_LOW;

static _PAINT_INFO_T    _t_paint_info;

static UINT32           _ui4_shdw_cvs_blk_time = 750;
static HANDLE_T         _h_shdw_cvs_blk_timer = NULL_HANDLE;
    
#ifdef CLI_LVL_ALL
static UINT16           _ui2_dbg_level   = DBG_INIT_LEVEL_MW_WGL_MNGR;
#endif

#ifdef CLI_LVL_ALL
static UINT32           _ui4_flash_time  = 0;
#endif

INT32                   _i4_alloc_mem;
INT32                   _i4_alloc_widget;
INT32                   _i4_alloc_branch;
INT32                   _i4_alloc_canvas;
INT32                   _i4_alloc_app_info;
INT32                   _i4_alloc_timer;
INT32                   _i4_alloc_canvas_callback;

#ifdef WM_DEBUG
static INT32            i4_app_cnt;
static INT32            i4_canvas_cnt;
static INT32            i4_branch_cnt;
static INT32            i4_widget_cnt;
#endif

#if 0 //def CLI_LVL_ALL
#define                 MAX_CANVAS_NUMBER       (3 * 2 * 2)
static UINT32           _aui4_surf_h            [MAX_CANVAS_NUMBER];
static UINT32           _ui4_canvas_count       = 0;
#endif




/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static INT32 _wgl_init_inner(
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_COLORMODE_T      e_colormode,
    BOOL                b_auto_scale,
    BOOL                b_no_inter_compose,
    BOOL                b_support_compose_anim,
    BOOL                b_osd_compress,
    BOOL                b_compressed_compose,
    UINT16              ui2_clut_sz,
    const GL_COLOR_T*   pt_clut,
    const GL_COLOR_T*   pt_color_key,
    UINT16              ui2_num_widget,
    UINT8               ui1_max_floated_widget,
    UINT16              ui2_key_first_repeat_delay,
    UINT16              ui2_key_repeat_delay,
    THREAD_DESCR_T*     pt_thread_desc,
    BOOL                b_need_init_iom);

static BOOL _any_widget_visible(
    const _CANVAS_T*    pt_canvas, 
    BOOL                b_check_ignored);

static VOID _broadcast_single_widget_msg(
    _WIDGET_T*  pt_widget, 
    UINT32      ui4_msg, 
    VOID*       pv_param1,
    VOID*       pv_param2);

static VOID _broadcast_widget_msg(
    _CANVAS_T*  pt_canvas, 
    UINT32      ui4_msg, 
    VOID*       pv_param1,
    VOID*       pv_param2);

static INT32 _wgl_lock(VOID);

static INT32 _wgl_unlock(VOID);

static VOID _paint_widget(
    _PAINT_INFO_T*          pt_paint_info,
    _CANVAS_T*              pt_canvas,
    const GL_RECT_T*        pt_rc_gbl,
    _PAINT_WIDGET_INFO_T*   pt_pwi);

static INT32 _paint_single_tree(
    _PAINT_INFO_T*      pt_paint_info,
    _CANVAS_T*          pt_canvas,
    _WIDGET_T*          pt_widget,
    const GL_RECT_T*    pt_rc_gbl);

static INT32 _paint_canvas(
    _CANVAS_T*          pt_canvas, 
    const GL_RECT_T*    pt_rect);

static VOID _set_flag_recursive(_WIDGET_T* pt_widget, UINT16 ui2_flag);

static BOOL _get_visibility(_WIDGET_T* pt_widget);

static VOID _set_visibility(
    _WIDGET_T*      pt_widget, 
    WGL_SW_CMD_T    e_sw_cmd);

static INT32 _send_msg(
    _WIDGET_T*  pt_widget, 
    UINT32      ui4_msg, 
    VOID*       pv_param1,
    VOID*       pv_param2);

static INT32 _send_canvas_notify(
    _CANVAS_T*  pt_canvas,
    UINT32      ui4_msg,
    VOID*       pv_param1,
    VOID*       pv_param2);

static INT32 _send_canvas_switch_notify(_CANVAS_T* pt_canvas);

static VOID _send_destroyed_recursive(_WIDGET_T* pt_widget);

static BOOL _destroy_single_tree_and_notify(
    _CANVAS_T*  pt_canvas, 
    _WIDGET_T*  pt_widget,
    GL_RECT_T*  pt_rcet);

static VOID _destroy_multi_tree(
    _CANVAS_T*  pt_canvas, 
    _WIDGET_T*  pt_widget,
    GL_RECT_T*  pt_rect);

static BOOL _destroy_multi_tree_and_notify(
    _CANVAS_T*  pt_canvas, 
    _WIDGET_T*  pt_widget,
    GL_RECT_T*  pt_rect);

static INT32 _enable_canvas_callback(
    _CANVAS_T*              pt_canvas, 
    wgl_canvas_callback_fct pf_cvs_nfy_fct, 
    BOOL                    b_enable);

static VOID _widget_timer_callback(
    HANDLE_T  pt_tm_handle,
    VOID*     pv_tag);

static INT32 _proc_tm_msg(HANDLE_T h_timer);
static VOID _delete_timer(_TIMER_T* pt_timer);
static VOID  _delete_widget_timer(SLIST_TIMER_T* pt_tm_lst);
static INT32 _destroy_app_widget(_APP_INFO_T* pt_app_info);
static INT32 _destroy_app_and_notify(_APP_INFO_T* pt_app_info);
static VOID _free_canvas(_CANVAS_T* pt_canvas);

static INT32 _screen_nfy_fct(
    HANDLE_T     h_obj,
    GL_COND_T    e_nfy_cond,
    VOID*        pv_nfy_tag,
    UINT32       ui4_data);
static VOID _setup_active_app(_APP_INFO_T* pt_app_info);
static VOID _cal_translation(_WIDGET_T* pt_widget, INT32* pi4_x, INT32* pi4_y);

static INT32 _cal_exclusive_region(
    GL_RECT_T       *pt_ori_rect,
    GL_RECT_T       *pt_exc_rect,
    GL_RECT_T       *pt_out_rect1,
    GL_RECT_T       *pt_out_rect2,
    GL_RECT_T       *pt_out_rect3,
    GL_RECT_T       *pt_out_rect4);
static VOID _fill_exclusive_region(GL_RECT_T*             pt_ori_rect,
										   INT32                   i4_current_rect,
										   UINT32                 ui4_rect_count,
										   GL_RECT_T*      	     pt_rect,
										   GL_LOCK_INFO_T*    pt_lock_info,
										   GL_COLOR_T*          pt_color);
static BOOL _unify_update_rect(
    const _CANVAS_T*    pt_new, 
    const GL_RECT_T*    pt_rc_new,
    const _CANVAS_T*    pt_old, 
    GL_RECT_T*          pt_rc_old /* in-out */);
static VOID _move_app_to_end(_APP_INFO_T* pt_app_info, UINT16 ui2_flag);

/* memory allocation/deallocatio routine */
static VOID* _malloc_canvas(VOID);
static VOID* _malloc_branch(VOID);
static VOID* _malloc_widget(VOID);
static VOID* _malloc_app_info(VOID);

static VOID _mfree_canvas(VOID* pv_mem);
static VOID _mfree_branch(VOID* pv_mem);
static VOID _mfree_widget(VOID* pv_mem);
static VOID _mfree_app_info(VOID* pv_mem);

#ifdef CLI_LVL_ALL     
static VOID _flash_area(
    GL_HSURFACE_T   h_surf,
    GL_RECT_T*      pt_rc, 
    UINT32          ui4_time);
#endif

/* animation related apis */
#ifdef WGL_ANIM_SUPPORT    
static BOOL _check_widget_is_opaque(_WIDGET_T* pt_widget,UINT16 ui2_state);

static VOID _modify_paint_rect_for_scale(GL_RECT_T* pt_upt_rc);

static VOID _add_paint_rect_for_scale(_WIDGET_T   *pt_widget,WGL_ANIM_ATTR_TYPE e_anim_attr_type);

static VOID _remove_paint_rect_for_scale(_WIDGET_T   *pt_widget,WGL_ANIM_ATTR_TYPE e_anim_attr_type);

static VOID _get_paint_rect_for_scale(_WIDGET_T   *pt_widget,WGL_ANIM_ATTR_TYPE e_anim_attr_type,GL_RECT_T *pt_out_rect);


static BOOL _check_animation_with_affect_child(_WIDGET_T* pt_widget);

static BOOL _check_animation_with_parent_affect_child(_WIDGET_T* pt_widget);

static VOID _paint_widget_with_anim_alpha(
    GL_HSURFACE_T h_surf_src,
    GL_HSURFACE_T h_surf_dst,
    UINT8         ui1_alpha,
    _CANVAS_T*              pt_canvas,
    const GL_RECT_T*        pt_rc_gbl,
    BOOL                    b_need_compose);

static VOID _paint_widget_with_anim(
    _PAINT_INFO_T*          pt_paint_info,
    _CANVAS_T*              pt_canvas,
    const GL_RECT_T*        pt_rc_gbl,
    _PAINT_WIDGET_INFO_T*   pt_pwi,
    BOOL                    b_need_compose);

static BOOL _get_valid_animation_action_type(
    UINT32 event_type,
    WGL_ANIM_ACTION_TYPE_T* pe_action_type);

static INT32 _handle_animation_for_event(
    _WIDGET_T* pt_widget,
    UINT32 event_type);
static BOOL _check_animation_need_scale(_WIDGET_T* pt_widget,WGL_ANIM_TICK_DATA* pt_anim_tick_data);

static INT32 _handle_anim_tick_message(
    HANDLE_T h_widget,
    WGL_ANIM_ATTR_TYPE e_anim_attr_type,
    UINT32 ui4_tick_idx);

static BOOL _check_anim_support(WGL_ANIMATION_DATA_T* pt_anim_data);

static BOOL _check_anim_child_conflict(_WIDGET_T* pt_widget);

static BOOL _check_anim_conflict(
    _WIDGET_T* pt_widget,
    WGL_ANIMATION_DATA_T* pt_anim_data);


static INT32 _get_anim_context(
    HANDLE_T                        h_widget,
    WGL_ANIM_ATTR_TYPE              e_anim_attr,
    WGL_WIDGET_ANIM_CONTEXT_T*      pt_anim);
#endif


HANDLE_T x_wgl_get_canvas_surface_handle(HANDLE_T h_canvas)
{
#ifdef CLI_LVL_ALL
    _CANVAS_T* pt_canvas = NULL;
    HANDLE_TYPE_T   e_type;
	HANDLE_T        h_surface = NULL_HANDLE;
	
    _wgl_lock();
    if (HR_OK == handle_get_type_obj(h_canvas, &e_type, (VOID**) &pt_canvas) &&
           HT_WGL_CANVAS == e_type)
	{
		h_surface = pt_canvas->h_surf_front;
	}
	_wgl_unlock();
	return h_surface;
	
#else
    return NULL_HANDLE;
#endif
}


/*-----------------------------------------------------------------------------
                    functions definitions
 ----------------------------------------------------------------------------*/
static VOID* _malloc_canvas(VOID)
{
    VOID* pt_mem = NULL;
    _i4_alloc_canvas++;
    _i4_alloc_mem += sizeof(_CANVAS_T);
    pt_mem = WGL_MEM_ALLOC(sizeof(_CANVAS_T));
    if(pt_mem != NULL)
    {
        x_memset(pt_mem,(UINT8)0,sizeof(_CANVAS_T));
    }
    return pt_mem;
}

static VOID* _malloc_canvas_callback(VOID)
{
    VOID* pt_mem = NULL;
    _i4_alloc_canvas_callback++;
    _i4_alloc_mem += sizeof(_CANVAS_CALLBACK_T);
    pt_mem = WGL_MEM_ALLOC(sizeof(_CANVAS_CALLBACK_T));
    if(pt_mem != NULL)
    {
        x_memset(pt_mem,(UINT8)0,sizeof(_CANVAS_CALLBACK_T));
    }
    return pt_mem;
}

static VOID* _malloc_branch(VOID)
{
    VOID* pt_mem = NULL;
    _i4_alloc_branch++;
    _i4_alloc_mem += sizeof(_BRANCH_T);
    pt_mem = WGL_MEM_ALLOC(sizeof(_BRANCH_T));
    if(pt_mem != NULL)
    {
        x_memset(pt_mem,(UINT8)0,sizeof(_BRANCH_T));
    }
    
    return pt_mem;
}

static VOID* _malloc_widget(VOID)
{
    VOID* pt_mem = NULL;
    _i4_alloc_widget++;
    _i4_alloc_mem += sizeof(_WIDGET_T);
    pt_mem = WGL_MEM_ALLOC(sizeof(_WIDGET_T));
    if(pt_mem != NULL)
    {
        x_memset(pt_mem,(UINT8)0,sizeof(_WIDGET_T));
    }
    
    return pt_mem;
}

static VOID* _malloc_app_info(VOID)
{
    VOID* pt_mem = NULL;
    _i4_alloc_app_info++;
    _i4_alloc_mem += sizeof(_APP_INFO_T);
    pt_mem = WGL_MEM_ALLOC(sizeof(_APP_INFO_T));
    if(pt_mem != NULL)
    {
        x_memset(pt_mem,(UINT8)0,sizeof(_APP_INFO_T));
    }
    return pt_mem;
}

static VOID* _malloc_timer(VOID)
{
    VOID* pt_mem = NULL;
    _i4_alloc_timer++;
    _i4_alloc_mem += sizeof(_TIMER_T);
    pt_mem = WGL_MEM_ALLOC(sizeof(_TIMER_T));
    if(pt_mem != NULL)
    {
        x_memset(pt_mem,(UINT8)0,sizeof(_TIMER_T));
    }
    return pt_mem;
}

static VOID _mfree_canvas(VOID* pv_mem)
{
    _i4_alloc_canvas--;
    _i4_alloc_mem -= sizeof(_CANVAS_T);
    
    WGL_MEM_FREE(pv_mem);
}

static VOID _mfree_canvas_callback(VOID* pv_mem)
{
    _i4_alloc_canvas_callback--;
    _i4_alloc_mem -= sizeof(_CANVAS_CALLBACK_T);
    
    WGL_MEM_FREE(pv_mem);
}

static VOID _mfree_branch(VOID* pv_mem)
{
    _i4_alloc_branch--;
    _i4_alloc_mem -= sizeof(_BRANCH_T);
    
    WGL_MEM_FREE(pv_mem);
}

static VOID _mfree_widget(VOID* pv_mem)
{
    _i4_alloc_widget--;
    _i4_alloc_mem -= sizeof(_WIDGET_T);
    
    WGL_MEM_FREE(pv_mem);
}

static VOID _mfree_app_info(VOID* pv_mem)
{
    _i4_alloc_app_info--;
    _i4_alloc_mem -= sizeof(_APP_INFO_T);
    
    WGL_MEM_FREE(pv_mem);
}

static VOID _mfree_timer(VOID* pv_mem)
{
    _i4_alloc_timer--;
    _i4_alloc_mem -= sizeof(_TIMER_T);

    WGL_WIDGET_MEM_FREE(((_TIMER_T*) pv_mem)->pt_attached_widget, sizeof(_TIMER_T),pv_mem);
}

static VOID _broadcast_single_widget_msg(
    _WIDGET_T*  pt_widget, 
    UINT32      ui4_msg, 
    VOID*       pv_param1,
    VOID*       pv_param2)
{
    _WIDGET_T*  pt_child;

    HANDLE_T            h_widget = pt_widget->h_this;
    wgl_widget_proc_fct pf_proc  = MSGPROC(pt_widget);
    
    _wgl_unlock();

    (*pf_proc)(h_widget, ui4_msg, pv_param1, pv_param2);

    _wgl_lock();
        
    /* broadcast message to all children of pt_widget */
    for (pt_child = CHILD(pt_widget); pt_child; pt_child = SIBLING(pt_child))
    {
        if (!ST_DESTROYED(pt_child))
        {
            _broadcast_single_widget_msg(
                pt_child, 
                ui4_msg, 
                pv_param1, 
                pv_param2);
        }
    }
}

static VOID _broadcast_widget_msg(
    _CANVAS_T*  pt_canvas, 
    UINT32      ui4_msg, 
    VOID*       pv_param1,
    VOID*       pv_param2)
{
    _BRANCH_T*  pt_branch;
    _WIDGET_T*  pt_widget;
    
    DLIST_FOR_EACH(pt_branch, &(pt_canvas->t_branch_lst), t_link_on_canvas)
    {
        /* process each branch */
        for (pt_widget = FIRST(pt_branch); pt_widget; pt_widget = SIBLING(pt_widget))
        {
            if (!ST_DESTROYED(pt_widget))
            {
                _broadcast_single_widget_msg(
                    pt_widget, 
                    ui4_msg, 
                    pv_param1, 
                    pv_param2);
            }
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: _widget_iom_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  pv_nfy_tag   References.
 *          e_nfy_cond   References.
 *          ui4_evt_code References.
 *          ui4_data     References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _widget_iom_nfy_fct(
    VOID*                   pv_nfy_tag,
    IOM_NFY_COND_T          e_nfy_cond,
    UINT32                  ui4_evt_code,
    UINT32                  ui4_data)
{
    #define _IGNORE             ((UINT32) 0)
    #define _NOTIFY_NEW_APP     ((UINT32) 1)
    #define _CLOSE_OLD_APP      ((UINT32) 2)
    #define _START_NEW_APP      ((UINT32) 4)

    static _APP_INFO_T* s_pt_last_app  = NULL;
    static UINT32       s_ui4_last_msg = WGL_MSG_KEY_UP;
    
    _APP_INFO_T*    pt_app_info;
    CRIT_STATE_T    t_cs;
    UINT32          ui4_msg;
    UINT32          ui4_process = _IGNORE;
    UINT32          ui4_tick = 0;
    
    t_cs = x_crit_start();

    pt_app_info = _pt_active_app;

    /* active app is not changed or last key sequence is complete */
    if (pt_app_info == s_pt_last_app || s_ui4_last_msg == WGL_MSG_KEY_UP)
    {
        if ((_i4_active_cnt >= 0) && pt_app_info)
        {
            ui4_process = _NOTIFY_NEW_APP;
        }
    }
    else
    {
        ui4_process = (pt_app_info && e_nfy_cond == IOM_NFY_COND_BTN_REPEAT) ? 
            (_CLOSE_OLD_APP | _START_NEW_APP) : _CLOSE_OLD_APP;
    }

    ui4_tick = os_get_sys_tick();
    
    x_crit_end(t_cs);
    
    ui4_msg = s_ui4_last_msg;
    
    if (ui4_process & _CLOSE_OLD_APP)
    {
        _APP_INFO_T*    pt_app_tmp;
    
        t_cs = x_crit_start();
        DLIST_FOR_EACH(pt_app_tmp, &_t_app_lst, t_link)
        {
            if (pt_app_tmp == s_pt_last_app)
            {
                break;
            }
        }
        x_crit_end(t_cs);

        if (pt_app_tmp) /* last active app is still alive */
        {
            (pt_app_tmp->pf_callback)(WGL_MSG_KEY_UP, (VOID*) ui4_evt_code, (VOID*) ui4_tick);
        }

        ui4_msg = WGL_MSG_KEY_UP;
    }

#if 0    
    if (ui4_process & _START_NEW_APP)
    {
        (pt_app_info->pf_callback)(
            WGL_MSG_KEY_DOWN, (VOID*) ui4_evt_code, (VOID*) ui4_tick);
        ui4_msg = WGL_MSG_KEY_DOWN;
    }
#endif

    if (ui4_process & _NOTIFY_NEW_APP)
    {
        switch (e_nfy_cond)
        {
            case IOM_NFY_COND_BTN_UP:
                ui4_msg = WGL_MSG_KEY_UP;
                break;

            case IOM_NFY_COND_BTN_DOWN:
                ui4_msg = WGL_MSG_KEY_DOWN;
                break;
                
            case IOM_NFY_COND_BTN_REPEAT:
                ui4_msg = WGL_MSG_KEY_REPEAT;
                break;

            default:
                return;
        }
        
        (pt_app_info->pf_callback)(
            ui4_msg, (VOID*) ui4_evt_code, (VOID*) ui4_tick);
    }

    if (ui4_process == _IGNORE)
    {
        switch (e_nfy_cond)
        {
            case IOM_NFY_COND_BTN_UP:
                ui4_msg = WGL_MSG_KEY_UP;
                break;

            case IOM_NFY_COND_BTN_DOWN:
                ui4_msg = WGL_MSG_KEY_DOWN;
                break;
                
            case IOM_NFY_COND_BTN_REPEAT:
                ui4_msg = WGL_MSG_KEY_REPEAT;
                break;

            default:
                return;
        }
    
        if (_pf_app_ignore_key_fct)
        {
            _pf_app_ignore_key_fct(ui4_msg, (VOID *)ui4_evt_code, (VOID *)ui4_tick);
        }
    }
        
    s_pt_last_app  = pt_app_info;
    s_ui4_last_msg = ui4_msg;
}

/*-----------------------------------------------------------------------------
 * Name: _repaint_and_destroy_mngr_thread_main
 *
 * Description: <function description>
 *
 * Inputs:  *pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _repaint_and_destroy_mngr_thread_main(VOID *pv_arg)
{
    INT32               i4_ret;
    SIZE_T              z_msg_size;
    UINT16              ui2_index;
    _PAINT_MSG_T        t_msg;
    _REPAINT_MNGR_T*    pt_repaint_mngr  = (_REPAINT_MNGR_T*) pv_arg;
    HANDLE_T            h_msg_q          = pt_repaint_mngr->h_msg_q;
    _CANVAS_T*          pt_active_canvas = NULL;
    _CANVAS_T*          pt_curr_canvas   = NULL;
    GL_RECT_T           t_rc_upt, t_rc_tmp;
    GL_RECT_T*          pt_rc_new;
    
    while (1)
    {
        z_msg_size = RPM_MSG_SIZE;
        i4_ret     = x_msg_q_receive(
                        &ui2_index,
                        (VOID*) &t_msg,
                        &z_msg_size,
                        &h_msg_q,
                        1,
                        (pt_active_canvas ? X_MSGQ_OPTION_NOWAIT : X_MSGQ_OPTION_WAIT));

        if (OSR_OK == i4_ret)
        {
            pt_rc_new = NULL;
            
            switch (t_msg.ui4_msg)
            {
                case RPM_MSG_PAINT:
                {
                    pt_curr_canvas = t_msg.pt_canvas;
                    pt_rc_new = &(t_msg.t_update);
                    break;
                }

                case RPM_MSG_DEL_SINGLE_TREE:
                    pt_curr_canvas = ((_DESTROY_MSG_T*) (&t_msg))->pt_canvas;
                    
                    if (_destroy_single_tree_and_notify(
                            pt_curr_canvas,
                            ((_DESTROY_MSG_T*) (&t_msg))->pt_widget,
                            &t_rc_tmp))
                    {
                        if (((_DESTROY_MSG_T*) (&t_msg))->b_repaint)
                        {
                            pt_rc_new = &t_rc_tmp;
                        }
                    }
                    break;

                case RPM_MSG_DEL_MULTI_TREE:
                    pt_curr_canvas = ((_DESTROY_MSG_T*) (&t_msg))->pt_canvas;
                    
                    if (_destroy_multi_tree_and_notify(
                        pt_curr_canvas,
                        ((_DESTROY_MSG_T*) (&t_msg))->pt_widget,
                        &t_rc_tmp))
                    {
                        if (((_DESTROY_MSG_T*) (&t_msg))->b_repaint)
                        {
                            pt_rc_new = &t_rc_tmp;
                        }
                    }
                    break;

                case RPM_MSG_DEL_APP:
                    if (pt_active_canvas)
                    {
                        i4_ret = _paint_canvas(pt_active_canvas, &t_rc_upt);
                        pt_active_canvas = NULL;;
                    }
                    i4_ret = _destroy_app_and_notify(
                        ((_DESTROY_APP_MSG_T*) (&t_msg))->pt_app_info);
                    break;

                case RPM_MSG_NOTIFY_APP:
                {
                    _APP_NOTIFY_MSG_T*  pt_app_nfy = (_APP_NOTIFY_MSG_T*) (&t_msg);
                    _CANVAS_T*          pt_canvas  = pt_app_nfy->pt_canvas;
                    _BRANCH_T*          pt_branch;

                    DLIST_FOR_EACH(pt_branch, &(pt_canvas->t_branch_lst), t_link_on_canvas)
                    {
                        (OWNER(pt_branch)->pf_callback)(
                            pt_app_nfy->ui4_wmsg,
                            pt_app_nfy->pv_param1,
                            pt_app_nfy->pv_param2);
                    }
                    break;
                }
                    
                case RPM_MSG_TIMER:
                {
                    i4_ret = _proc_tm_msg(((_TM_MSG_T*) (&t_msg))->h_timer);

                    if (WGLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL} ERR: Processing timer message failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                    }
                    
                    break;
                }

                case RPM_MSG_SHADOW_UNLOCK:
                {
                    _CANVAS_T   *pt_ref, *pt_shadow;

                    pt_ref    = t_msg.pt_canvas;
                    pt_shadow = SHADOW(pt_ref);

                    /* if still in Shadow-lock state */
                    if (!pt_ref->b_running && !pt_shadow->b_running)
                    {
                        _wgl_lock();
                        
                        pt_ref->b_shadow_locked    = FALSE;
                        pt_shadow->b_shadow_locked = FALSE;

                        pt_ref->b_unlock_to_shadow    = TRUE;
                        pt_shadow->b_unlock_to_shadow = TRUE;

                        pt_curr_canvas = pt_shadow;
                        
                        SET_RECT_BY_SIZE(&t_rc_tmp, 0, 0, (INT32)pt_shadow->ui4_width, (INT32)pt_shadow->ui4_height);

                        pt_rc_new = &t_rc_tmp;

                        _wgl_unlock();
                    }

                    break;
                }
                
                default: /* should not arrive here */
                    break;
            }

            if (pt_rc_new && !RECT_EMPTY(pt_rc_new))
            {
                if (pt_active_canvas)
                {
                    if (!_unify_update_rect(
                        pt_curr_canvas, pt_rc_new, pt_active_canvas, &t_rc_upt))
                    {
                        i4_ret = _paint_canvas(pt_active_canvas, &t_rc_upt);
                        pt_active_canvas = pt_curr_canvas;
                        t_rc_upt = (*pt_rc_new);
                    }
                }
                else
                {
                    pt_active_canvas = pt_curr_canvas;
                    t_rc_upt = (*pt_rc_new);
                }
            }
        }
        else if (OSR_NO_MSG == i4_ret)
        {
            if (pt_active_canvas)
            {
                i4_ret = _paint_canvas(pt_active_canvas, &t_rc_upt);
                pt_active_canvas = NULL;;
            }
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _init_repaint_mngr
 *
 * Description: <function description>
 *
 * Inputs:  pt_thread_desc References.
 *          z_msg_size     References.
 *
 * Outputs: - 
 *
 * Returns: FALSE  Success.
 *          TRUE   Fail.
 *
 ----------------------------------------------------------------------------*/
static BOOL _init_repaint_mngr(
    THREAD_DESCR_T*         pt_thread_desc,
    SIZE_T                  z_msg_size)
{
    HANDLE_T    h_msg_q;
    HANDLE_T    h_thread;
    UINT16      ui2_num_msgs;
    UINT8       ui1_priority;
    SIZE_T      z_stack_size;

    ui2_num_msgs = (pt_thread_desc->ui2_num_msgs == 0) ? WM_NUM_OF_MSGS : pt_thread_desc->ui2_num_msgs;
    
    if (OSR_OK != x_msg_q_create(
                    &h_msg_q, 
                    WM_MSG_Q_NAME,
                    z_msg_size, 
                    ui2_num_msgs))
    {
        return FALSE;
    }
    _t_repaint_mngr.h_msg_q = h_msg_q;
    
    z_stack_size = (pt_thread_desc->z_stack_size == 0) ? WM_THREAD_DEFAULT_STACK_SIZE : pt_thread_desc->z_stack_size;
    ui1_priority = (pt_thread_desc->ui1_priority == 0) ? WM_THREAD_DEFAULT_PRIORITY : pt_thread_desc->ui1_priority;
    
    if (OSR_OK != x_thread_create(
        &h_thread, 
        WM_THREAD_NAME, 
        z_stack_size,
        ui1_priority,
        _repaint_and_destroy_mngr_thread_main,
        sizeof(_t_repaint_mngr),
        &_t_repaint_mngr))
    {
        x_msg_q_delete(h_msg_q);
        _t_repaint_mngr.h_msg_q = NULL_HANDLE;
        return FALSE;
    }

    _t_repaint_mngr.h_thread = h_thread;

    return TRUE;
}

INT32 _app_element_lock(VOID)
{
    return (x_sema_lock(_h_app_element_lock, X_SEMA_OPTION_WAIT) == OSR_OK)? WGLR_OK: WGLR_OS_ERROR;
        
}

INT32 _app_element_unlock(VOID)
{
    return (x_sema_unlock(_h_app_element_lock) == OSR_OK)? WGLR_OK: WGLR_OS_ERROR;
        
}

INT32 _app_list_lock(VOID)
{
    return (x_sema_lock(_h_app_list_lock, X_SEMA_OPTION_WAIT) == OSR_OK)? WGLR_OK: WGLR_OS_ERROR;
        
}

INT32 _app_list_unlock(VOID)
{
    return (x_sema_unlock(_h_app_list_lock) == OSR_OK)? WGLR_OK: WGLR_OS_ERROR;
        
}

/*-----------------------------------------------------------------------------
 * Name: _hrc_lock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _hrc_lock(VOID)
{
    return (x_sema_lock(_h_hrc_lock, X_SEMA_OPTION_WAIT) == OSR_OK ? WGLR_OK : WGLR_OS_ERROR);
}

/*-----------------------------------------------------------------------------
 * Name: _hrc_unlock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _hrc_unlock(VOID)
{
    return (x_sema_unlock(_h_hrc_lock) == OSR_OK ? WGLR_OK : WGLR_OS_ERROR);
}

/*-----------------------------------------------------------------------------
 * Name: _wgl_lock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _wgl_lock(VOID)
{
    return (x_sema_lock(_h_wgl_lock, X_SEMA_OPTION_WAIT) == OSR_OK ? WGLR_OK : WGLR_OS_ERROR);
}

/*-----------------------------------------------------------------------------
 * Name: _wgl_unlock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _wgl_unlock(VOID)
{
    return (x_sema_unlock(_h_wgl_lock) == OSR_OK ? WGLR_OK : WGLR_OS_ERROR);
}

/*-----------------------------------------------------------------------------
 * Name: _handle_free_app
 *
 * Description: <function description>
 *
 * Inputs:  h_handle     References.
 *          e_type       References.
 *          pv_obj       References.
 *          pv_tag       References.
 *          b_req_handle References.
 *
 * Outputs: - 
 *
 * Returns: TRUE   Success.
 *          FALSE  Fail.
 *
 ----------------------------------------------------------------------------*/
static BOOL _handle_free_app (
    HANDLE_T        h_handle,
    HANDLE_TYPE_T   e_type,
    VOID*           pv_obj,
    VOID*           pv_tag,
    BOOL            b_req_handle)
{
    if (!b_req_handle && pv_obj)
    {
        _APP_INFO_T*        pt_app_info = (_APP_INFO_T*) pv_obj;
        _BRANCH_T*          pt_branch;
        CRIT_STATE_T        t_cs;
        _DESTROY_APP_MSG_T  t_msg;

        _wgl_lock();

        if (_pt_active_app == pt_app_info)
        {
            _APP_INFO_T* pt_prev;
            
            /* x_crit_end(t_cs); */
            
            pt_prev = DLIST_PREV(pt_app_info, t_link);
            if (pt_prev)
            {
                /* activate next app */
                _setup_active_app(pt_prev);
            }
            else
            {   /* reset current active app and active count */
                t_cs = x_crit_start();
                _pt_active_app = NULL;
                _i4_active_cnt = 0;
                x_crit_end(t_cs);
            }
        }

        /* mark all widgets of the app as destroyed */
        DLIST_FOR_EACH(pt_branch, &(pt_app_info->t_branch_lst), t_link_on_app)
        {
            if (FIRST(pt_branch))
            {
                _set_flag_recursive(FIRST(pt_branch), WS_DESTROYED);
            }
        }
        
        /* remove pt_app_info from the app-list */
        t_cs = x_crit_start();
        if (pt_app_info != NULL)
        {
            DLIST_REMOVE(pt_app_info, &_t_app_lst, t_link);
        }
        x_crit_end(t_cs);
        
        _wgl_unlock();

        t_msg.ui4_msg     = RPM_MSG_DEL_APP;
        t_msg.pt_app_info = pt_app_info;
        
        x_msg_q_send(
            _t_repaint_mngr.h_msg_q, 
            &t_msg, 
            sizeof(t_msg),
            RPM_MSG_PRIORITY_DESTROY);
        
        
        return TRUE;
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _handle_free_canvas
 *
 * Description: <function description>
 *
 * Inputs:  h_handle     References.
 *          e_type       References.
 *          pv_obj       References.
 *          pv_tag       References.
 *          b_req_handle References.
 *
 * Outputs: - 
 *
 * Returns: FALSE  Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _handle_free_canvas (
    HANDLE_T        h_handle,
    HANDLE_TYPE_T   e_type,
    VOID*           pv_obj,
    VOID*           pv_tag,
    BOOL            b_req_handle)
{
     if (!b_req_handle && pv_obj)
    {
        _CANVAS_T*        pt_canvas = (_CANVAS_T*) pv_obj;
        CRIT_STATE_T        t_cs;
        _TRANSP_LIST_T* pt_list;
        _CANVAS_CALLBACK_T* pt_cvs_cb;
        HANDLE_T h_rgn_list;

        _wgl_lock();
       
        /* remove pt_canvas from the canvas list*/
        t_cs = x_crit_start();
        if (pt_canvas != NULL)
        {
            SLIST_REMOVE(pt_canvas, t_next);
        }
        x_crit_end(t_cs);
        /*remove relationship bewteen pt_canvas and its ref canvas*/
        if(IS_SHADOW(pt_canvas))
        {
           SHADOW(REFERENCE(pt_canvas)) = NULL_HANDLE;
        }
         /* release all transparent list resource of the canvas */
        DLIST_FOR_EACH(pt_list, &pt_canvas->t_transp_list, t_link)
        {
        
           h_rgn_list =pt_list->h_rgn_list;
           DLIST_REMOVE(pt_list, &pt_canvas->t_transp_list, t_link);
           WGL_MEM_FREE(pt_list);
           handle_free(h_rgn_list,TRUE);
        }
         /* release canvas callback function list resource of the canvas*/
        SLIST_FOR_EACH(pt_cvs_cb, &(pt_canvas->t_cvs_cb_lst), t_link)
        {
            SLIST_REMOVE(pt_cvs_cb, t_link);
            MFREE_CANVAS_CALLBACK(pt_cvs_cb);

        }
        _free_canvas(pt_canvas);
        
        _wgl_unlock();
        return TRUE;
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _free_widget
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _free_widget(_WIDGET_T* pt_widget)
{
#ifdef WGL_ANIM_SUPPORT
    UINT8 ui1_idx=0;
    WGL_WIDGET_ANIM_CONTEXT_T t_anim;
#endif
    /* delete attacher timers */
    _delete_widget_timer(&(pt_widget->t_tm_lst));
    
#ifdef WGL_ANIM_SUPPORT
     wgl_end_animation(pt_widget->h_this,WGL_ANIM_ATTR_WIDGET);
     wgl_end_animation(pt_widget->h_this,WGL_ANIM_ATTR_WIDGET_ELEMENT);
     /*delete widget animation handle*/
     t_anim = ANIM(pt_widget);
     wgl_anim_free(&t_anim);
#endif
    /* delete widget instance */
    (pt_widget->pt_fct_tbl->pf_destroy_instance)(pt_widget->t_instance);

#ifdef WGL_ANIM_SUPPORT    
    /*release animation map memory*/
    if(pt_widget->ui1_map_count <= WGL_ANIM_MAX_MAP_CNT )
    {
        for(;ui1_idx<pt_widget->ui1_map_count;ui1_idx++)
        {
            if(pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_anim_data!= NULL)
            {
                 WGL_WIDGET_MEM_FREE(pt_widget,pt_widget->at_anim_map[ui1_idx].t_anim_data.ui2_count*sizeof(WGL_ANIM_DATA_INFO),pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_anim_data);
            }
            if(pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_path_mode!= NULL)
            {
                 WGL_WIDGET_MEM_FREE(pt_widget,pt_widget->at_anim_map[ui1_idx].t_anim_data.ui2_count*sizeof(WGL_ANIM_PATH_DATA_T),pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_path_mode);
            }
        }
    }
#endif
    /* delete widget object */
    MFREE_WIDGET(pt_widget);
}

static BOOL _free_transp_region(
    HANDLE_T            h_handle,
    HANDLE_TYPE_T       e_type,
    VOID*               pv_obj,
    VOID*               pv_tag,
    BOOL                b_req_handle)
{
    _TRANSP_REGION_T    *pt_obj = (_TRANSP_REGION_T *)pv_obj;

    if (pt_obj == NULL)
    {
        return FALSE;
    }

    if (pt_obj->pt_rect)
    {
        x_mem_free(pt_obj->pt_rect);
    }
    if(pt_obj->pt_exclusive_rect)
    {
        x_mem_free(pt_obj->pt_exclusive_rect);
    }
    WGL_MEM_FREE(pt_obj);

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _handle_free_widget
 *
 * Description: <function description>
 *
 * Inputs:  h_handle     References.
 *          e_type       References.
 *          pv_obj       References.
 *          pv_tag       References.
 *          b_req_handle References.
 *
 * Outputs: - 
 *
 * Returns: TRUE   Success.
 *          FALSE  Fail.
 *
 ----------------------------------------------------------------------------*/
static BOOL _handle_free_widget (
    HANDLE_T        h_handle,
    HANDLE_TYPE_T   e_type,
    VOID*           pv_obj,
    VOID*           pv_tag,
    BOOL            b_req_handle)
{
    if (!b_req_handle && pv_obj)
    {
        _free_widget((_WIDGET_T*) pv_obj);
        return TRUE;
    }
    
    return FALSE;
}

static BOOL _handle_free_timer (
    HANDLE_T        h_handle,
    HANDLE_TYPE_T   e_type,
    VOID*           pv_obj,
    VOID*           pv_tag,
    BOOL            b_req_handle)
{
    if (!b_req_handle && pv_obj)
    {
        x_timer_delete(((_TIMER_T*) pv_obj)->h_sys_timer);
        MFREE_TIMER(pv_obj);
        
        return TRUE;
    }

    return FALSE;
}

static BOOL _canvas_cfg_is_valid(const WGL_CANVAS_CFG_T* pt_cfg)
{
    if (pt_cfg)
    {
        if ((pt_cfg->ui4_flag & WGL_CANVAS_CFG_WIDTH) &&
            (0 == pt_cfg->ui4_width))
        {
            return FALSE;
        }
        
        if ((pt_cfg->ui4_flag & WGL_CANVAS_CFG_HEIGHT) &&
            (0 == pt_cfg->ui4_height))
        {
            return FALSE;
        }
        
        if (pt_cfg->ui4_flag & WGL_CANVAS_CFG_PALETTE)
        {
            return (gl_is_valid_obj(pt_cfg->h_palette, GLT_OBJ_PALETTE));
        }
    }
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _find_canvas
 *
 * Description: <function description>
 *
 * Inputs:  h_plane References.
 *
 * Outputs: - 
 *
 * Returns: NULL  Success.
 *
 ----------------------------------------------------------------------------*/
static _CANVAS_T* _find_canvas(HANDLE_T h_plane)
{
    _CANVAS_T*  pt_canvas;
    BOOL        b_cmp;
    
    SLIST_FOR_EACH(pt_canvas, &_t_canvas_lst, t_next)
    {
        if ((!IS_SHADOW(pt_canvas))                                           &&
            (GLR_OK == gl_is_same_plane(pt_canvas->h_plane, h_plane, &b_cmp)) &&
            (TRUE   == b_cmp))
        {
            return (pt_canvas);
        }
    }

    return NULL;
}
static VOID _free_canvas_resource(_CANVAS_T* pt_canvas)
{
#ifdef MW_OSD_COMPRESSION_SHINK
	
	if (pt_canvas->h_view_back != NULL_HANDLE)
	 {
		 x_gl_obj_free(pt_canvas->h_view_back);
		 pt_canvas->h_view_back = NULL_HANDLE;
	 }
#else
	if (pt_canvas->h_view_compress != NULL_HANDLE)
    {
        x_gl_obj_free(pt_canvas->h_view_compress);
        pt_canvas->h_view_compress = NULL_HANDLE;
    }
	if (pt_canvas->h_surf_compress != NULL_HANDLE)
    {
        x_gl_obj_free(pt_canvas->h_surf_compress);
        pt_canvas->h_surf_compress = NULL_HANDLE;
    }
    if (pt_canvas->h_surf_compress_back!= NULL_HANDLE)
    {
        x_gl_obj_free(pt_canvas->h_surf_compress_back);
        pt_canvas->h_surf_compress_back = NULL_HANDLE;
    }
    
#endif
    if (pt_canvas->h_view != NULL_HANDLE)
    {
        x_gl_obj_free(pt_canvas->h_view);
        pt_canvas->h_view = NULL_HANDLE;
    }

    if (pt_canvas->h_palette != NULL_HANDLE)
    {
        x_gl_obj_free(pt_canvas->h_palette);
        pt_canvas->h_palette = NULL_HANDLE;
    }
    if (pt_canvas->h_graphics != NULL_HANDLE)
    {
        x_gl_obj_free(pt_canvas->h_graphics);
        pt_canvas->h_graphics = NULL_HANDLE;
    }
    if (pt_canvas->h_graphics_target != NULL_HANDLE)
    {
        x_gl_obj_free(pt_canvas->h_graphics_target);
        pt_canvas->h_graphics_target = NULL_HANDLE;
    }

    if (pt_canvas->h_surf_front != NULL_HANDLE)
    {
        x_gl_obj_free(pt_canvas->h_surf_front);
        pt_canvas->h_surf_front = NULL_HANDLE;
    }
    
    if (pt_canvas->h_surf_back != NULL_HANDLE)
    {
        x_gl_obj_free(pt_canvas->h_surf_back);
        pt_canvas->h_surf_back = NULL_HANDLE;
    }
    
    if (pt_canvas->h_surf_render != NULL_HANDLE)
    {
        x_gl_obj_free(pt_canvas->h_surf_render);
        pt_canvas->h_surf_render = NULL_HANDLE;
    }
    
    if (pt_canvas->h_surf_anim!= NULL_HANDLE)
    {
        x_gl_obj_free(pt_canvas->h_surf_anim);
        pt_canvas->h_surf_anim = NULL_HANDLE;
    }

    if (pt_canvas->h_plane != NULL_HANDLE)
    {
        x_gl_obj_free(pt_canvas->h_plane);
        pt_canvas->h_plane = NULL_HANDLE;
    }

}

/*-----------------------------------------------------------------------------
 * Name: _free_canvas
 *
 * Description: <function description>
 *
 * Inputs:  pt_canvas References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _free_canvas(_CANVAS_T* pt_canvas)
{
    _free_canvas_resource(pt_canvas);
    MFREE_CANVAS(pt_canvas);
}

static VOID _get_canvas_info(
    const _CANVAS_T*    pt_canvas, 
    WGL_CANVAS_INFO_T*  pt_cvs_info)
{
    if (pt_canvas)
    {
        pt_cvs_info->ui4_width    = pt_canvas->ui4_width;
        pt_cvs_info->ui4_height   = pt_canvas->ui4_height;
        pt_cvs_info->e_clr_mode   = pt_canvas->e_color_mode;
        pt_cvs_info->t_colorkey   = pt_canvas->t_color_key;
        pt_cvs_info->b_auto_scale = pt_canvas->b_auto_scale;
        pt_cvs_info->h_canvas     = pt_canvas->h_this;
    }
    else
    {
        pt_cvs_info->ui4_width    = _ui4_surf_width;
        pt_cvs_info->ui4_height   = _ui4_surf_height;
        pt_cvs_info->e_clr_mode   = _e_surf_mode;
        pt_cvs_info->t_colorkey   = _t_color_key;
        pt_cvs_info->b_auto_scale = _b_auto_scale;
    }
}

static BOOL _create_widget_view(
    UINT32              ui4_width,
    UINT32              ui4_height,
    BOOL                b_auto_scale,
    HANDLE_T            h_surf,
    const GL_COLOR_T*   pt_colorkey,
    HANDLE_T*           ph_view,
    UINT32*             pui4_viewport)
{
    GL_DISPLAY_OPT_T    t_dsp_opt;
    
    if (x_gl_view_create(
        ui4_width, 
        ui4_height, 
        b_auto_scale, 
        ph_view) != GLR_OK)
    {
        return FALSE;
    }

    /* fill GL_DISPLAY_OPT_T structure */
    t_dsp_opt.e_flag                = GL_DISP_OPT_ALL;
    t_dsp_opt.e_blend_option        = GL_BLEND_OPT_PIXEL;
    t_dsp_opt.ui1_alpha             = 255;
    t_dsp_opt.t_colorkey            = *pt_colorkey;
    t_dsp_opt.t_pos.i4_x            = 0;
    t_dsp_opt.t_pos.i4_y            = 0;
    t_dsp_opt.b_decompression       = _b_osd_compress;
#ifdef OSD_LINE_SEPARATE_COMPRESSION
    t_dsp_opt.b_line_separate       = TRUE;
#else
    t_dsp_opt.b_line_separate       = FALSE;
#endif
    t_dsp_opt.ui1_compression_ratio = (UINT8)_e_compression_ratio;
    t_dsp_opt.ui1_extra_line_size   = COMPRESSION_EXTRA_LINES;
    t_dsp_opt.ui4_surf_offset_x     = 0;
    t_dsp_opt.ui4_surf_offset_y     = 0;

    if (_b_osd_compress)
    {
        GL_SURF_INFO_T      t_info;
        
        if (x_gl_surface_get_info(h_surf, &t_info) != GLR_OK)
        {
            x_gl_obj_free(*ph_view);
            return FALSE;
        }

        t_dsp_opt.e_shrink_opt      = GL_SHRINK_MANUAL;
        t_dsp_opt.t_size.ui4_width  = t_info.ui4_width;
        t_dsp_opt.t_size.ui4_height = t_info.ui4_height - COMPRESSION_EXTRA_LINES;
        t_dsp_opt.fx_shrink_x       = (GL_FIX16_T) (((UINT16) 1) << GL_F16_FRAC);
        t_dsp_opt.fx_shrink_y       = (GL_FIX16_T) (((UINT16) 1) << GL_F16_FRAC);

        if (t_info.e_clrmode != GL_COLORMODE_ARGB_D8888 && t_info.e_clrmode != GL_COLORMODE_AYUV_D8888)
        {
            t_dsp_opt.ui1_compression_ratio = (UINT8)GL_LOSSLESS;
        }
    }
    else if (b_auto_scale)
    {
        t_dsp_opt.e_shrink_opt = GL_SHRINK_SURFACE;
    }
    else
    {
        t_dsp_opt.e_shrink_opt      = GL_SHRINK_MANUAL;
        t_dsp_opt.t_size.ui4_width  = ui4_width;
        t_dsp_opt.t_size.ui4_height = ui4_height;
        t_dsp_opt.fx_shrink_x       = (GL_FIX16_T) (((UINT16) 1) << GL_F16_FRAC);
        t_dsp_opt.fx_shrink_y       = (GL_FIX16_T) (((UINT16) 1) << GL_F16_FRAC);
    }
    
    if (x_gl_view_create_viewport(
        *ph_view, 
        h_surf, 
        &t_dsp_opt, 
        pui4_viewport) != GLR_OK)
    {
        x_gl_obj_free(*ph_view);
        return FALSE;
    }
    
    return TRUE;
}

static INT32 _enable_canvas_callback(
    _CANVAS_T*              pt_canvas, 
    wgl_canvas_callback_fct pf_cvs_nfy_fct, 
    BOOL                    b_enable)
{
    _CANVAS_CALLBACK_T* pt_cvs_cb;
    INT32               i4_ret;

    i4_ret = WGLR_OK;
    
    /* look for the specified callback */
    SLIST_FOR_EACH(pt_cvs_cb, &(pt_canvas->t_cvs_cb_lst), t_link)
    {
        if (pf_cvs_nfy_fct == pt_cvs_cb->pf_cvs_nfy_fct)
        {
            if (!b_enable)
            {
                SLIST_REMOVE(pt_cvs_cb, t_link);
                MFREE_CANVAS_CALLBACK(pt_cvs_cb);
            }

            return WGLR_OK;
        }
    }

    /* not found */
    if (b_enable)
    {
        pt_cvs_cb = (_CANVAS_CALLBACK_T*) MALLOC_CANVAS_CALLBACK();

        if (pt_cvs_cb)
        {
            pt_cvs_cb->pf_cvs_nfy_fct = pf_cvs_nfy_fct;
            SLIST_INSERT_HEAD(pt_cvs_cb, &(pt_canvas->t_cvs_cb_lst), t_link);
        }
        else
        {
            i4_ret = WGLR_OUT_OF_MEMORY;
        }
    }

    return (i4_ret);
}

static VOID _fill_canvas_cfg(
    WGL_CANVAS_CFG_T*       pt_cfg_out, 
    const WGL_CANVAS_CFG_T* pt_cfg_in)
{
    UINT32  ui4_flag;
    
    if (pt_cfg_in)
    {
        (*pt_cfg_out) = (*pt_cfg_in);
    }
    else
    {
        pt_cfg_out->ui4_flag = 0;
    }

    ui4_flag = pt_cfg_out->ui4_flag;
    
    if (0 == (ui4_flag & WGL_CANVAS_CFG_WIDTH))
    {
        pt_cfg_out->ui4_width = _ui4_surf_width;
    }
    
    if (0 == (ui4_flag & WGL_CANVAS_CFG_HEIGHT))
    {
        pt_cfg_out->ui4_height = _ui4_surf_height;
    }
    
    if (0 == (ui4_flag & WGL_CANVAS_CFG_COLORMODE))
    {
        pt_cfg_out->e_clr_mode = _e_surf_mode;
    }
    
    if (0 == (ui4_flag & WGL_CANVAS_CFG_AUTOSCALE))
    {
        pt_cfg_out->b_auto_scale = _b_auto_scale;
    }

    if (0 == (ui4_flag & WGL_CANVAS_CFG_COLORKEY))
    {
        pt_cfg_out->t_colorkey = _t_color_key;
    }

	if (0 == (ui4_flag & WGL_CANVAS_CFG_FORCE_NO_INTER_COMPOSE))
	{
		pt_cfg_out->b_force_no_inter_compose = FALSE;
	}
    
    if (GL_IS_PALETTE_MODE(pt_cfg_out->e_clr_mode))
    {
        if (0 == (ui4_flag & WGL_CANVAS_CFG_PALETTE))
        {
            pt_cfg_out->h_palette = _h_sys_pal; /* use system default palette */
        }
    }
    else
    {
        pt_cfg_out->h_palette = NULL_HANDLE;
    }
}

static VOID _set_canvas_colorkey(
    _CANVAS_T*          pt_canvas,
    const GL_COLOR_T*   pt_colorkey)
{
    pt_canvas->t_color_key = (*pt_colorkey);
}

static INT32 _set_canvas_palette(
    _CANVAS_T*  pt_canvas,
    HANDLE_T    h_pal,
    BOOL        b_flip,
    BOOL        b_force_no_clean)
{
    GL_HPALETTE_T   h_dup_pal;
    GL_HVIEW_T      h_view = NULL_HANDLE;
    UINT32          ui4_viewport = 0;
    INT32           i4_ret;
    BOOL            b_new_pal = (h_pal != NULL_HANDLE);
    
    i4_ret = WGLR_INV_COLORMODE;

    if (GL_IS_PALETTE_MODE(pt_canvas->e_color_mode))
    {
        i4_ret = WGLR_OUT_OF_HANDLES;
        
        h_dup_pal = (b_new_pal) ? gl_palette_duplicate_handle(h_pal) : (pt_canvas->h_palette);

        if (NULL_HANDLE != h_dup_pal)
        {
            i4_ret = WGLR_INTERNAL_ERROR;
           
            if (_b_osd_compress || x_gl_view_delete_viewport(
                pt_canvas->h_view, 
                pt_canvas->ui4_viewport) == GLR_OK)
            {
                BOOL    b_in_shadow_lock = (pt_canvas->pt_shadow != NULL && !pt_canvas->b_running && !pt_canvas->pt_shadow->b_running);
                
                if ((b_flip || b_in_shadow_lock) && !b_force_no_clean)
                {
                    x_gl_plane_flip(
                        pt_canvas->h_plane,
                        NULL_HANDLE,
                        TRUE);

                    x_gl_surface_fill(
                        pt_canvas->h_surf_front,
                        &(pt_canvas->t_color_key),
                        GL_SYNC);
                }
            
                if (x_gl_surface_set_palette(
                    pt_canvas->h_surf_front, 
                    h_dup_pal) == GLR_OK)
                {
                    if (_b_osd_compress || _create_widget_view(
                        pt_canvas->ui4_width,
                        pt_canvas->ui4_height,
                        pt_canvas->b_auto_scale,
                        pt_canvas->h_surf_front,
                        &(pt_canvas->t_color_key),
                        &h_view,
                        &ui4_viewport))
                    {
                        if (b_flip || b_in_shadow_lock)
                        {
                            if (x_gl_plane_flip(
                                pt_canvas->h_plane,
                                h_view,
                                TRUE) == GLR_OK)
                            {
                                i4_ret = WGLR_OK;
                            }
                        }
                        else
                        {
                            i4_ret = WGLR_OK;
                        }

                        if (WGLR_OK == i4_ret)
                        {
                            x_gl_obj_free(pt_canvas->h_view);

                            pt_canvas->h_view       = h_view;
                            pt_canvas->ui4_viewport = ui4_viewport;

                            if (b_new_pal)
                            {
                                x_gl_obj_free(pt_canvas->h_palette);
                                pt_canvas->h_palette = h_dup_pal;
                            }
                        }
                        else
                        {
                            x_gl_obj_free(h_view);
                            x_gl_obj_free(h_dup_pal);
                            DBG_ERROR(("{WGL} ERR: Flipping plane failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                        }
                    }
                    else
                    {
                        x_gl_obj_free(h_dup_pal);
                        DBG_ERROR(("{WGL} ERR: Creating widget view failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                    }
                }
                else
                {
                    x_gl_obj_free(h_dup_pal);
                    DBG_ERROR(("{WGL} ERR: Setting palette failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }
            }
        }
        else
        {
            DBG_ERROR(("{WGL} ERR: Out of handles (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    else
    {
        DBG_ERROR(("{WGL} ERR: Canvas is not of palette mode (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }

    return (i4_ret);
}

static INT32 _new_canvas_resource(    
    _CANVAS_T*          pt_canvas,
    GL_HPLANE_T             h_plane,
    const WGL_CANVAS_CFG_T* pt_canvas_cfg,
    const _CANVAS_T*        pt_ref_canvas)
{
    GL_HPLANE_T         h_dup_plane;
    GL_HSCREEN_T        h_screen;
    GL_HVIEW_T          h_view;
    GL_HSURFACE_T       h_surf_front;
    GL_HSURFACE_T       h_surf_back;
    GL_SCREEN_STATUS_T  t_status;
    GL_SURFACE_DESC_T   t_desc;
    UINT32              ui4_viewport = 0;
    UINT32              ui4_view_width, ui4_view_height;
    BOOL                b_new_surf;
    WGL_CANVAS_CFG_T    t_cfg;
     
    INT32 i4_ret = WGLR_OUT_OF_RESOURCE;
    
    pt_canvas->h_screen             = NULL_HANDLE;
    pt_canvas->h_plane              = NULL_HANDLE;
    pt_canvas->h_view               = NULL_HANDLE;
    pt_canvas->h_palette            = NULL_HANDLE;
    pt_canvas->h_surf_front         = NULL_HANDLE;
    pt_canvas->h_surf_back          = NULL_HANDLE;
    pt_canvas->h_surf_render        = NULL_HANDLE;    
    pt_canvas->h_surf_anim          = NULL_HANDLE;
    pt_canvas->h_graphics           = NULL_HANDLE;
    pt_canvas->h_graphics_target    = NULL_HANDLE;
#ifdef MW_OSD_COMPRESSION_SHINK	
    pt_canvas->h_view_back          = NULL_HANDLE;
#else
    pt_canvas->h_surf_compress      = NULL_HANDLE;
    pt_canvas->h_surf_compress_back = NULL_HANDLE;
    pt_canvas->h_view_compress      = NULL_HANDLE;
#endif

    pt_canvas->b_compress_back_running = FALSE;
    
      if (GLR_OK != x_gl_plane_get_linked_screen(
          h_plane, 
          _screen_nfy_fct,
          (VOID*) pt_canvas,
          &h_screen))
      {
          return i4_ret;
      }
      pt_canvas->h_screen = h_screen;
      
      if (GLR_OK != x_gl_screen_get_status(h_screen, &t_status))
      {
          return i4_ret;
      }
    
      h_dup_plane = gl_plane_duplicate_handle(h_plane);
      if (NULL_HANDLE == h_dup_plane)
      {
          return i4_ret;
      }
      pt_canvas->h_plane = h_dup_plane;
    
      /* set up canvas configuration structure */
      _fill_canvas_cfg(&t_cfg, pt_canvas_cfg);
      b_new_surf = 
          (!pt_ref_canvas)                                           || 
          (pt_ref_canvas->ui4_width    != t_cfg.ui4_width)  ||
          (pt_ref_canvas->ui4_height   != t_cfg.ui4_height) ||
          (pt_ref_canvas->e_color_mode != t_cfg.e_clr_mode);
      
      pt_canvas->b_new_surface = b_new_surf;
    
      if (t_cfg.h_palette != NULL_HANDLE)
      {
          pt_canvas->h_palette = gl_palette_duplicate_handle(t_cfg.h_palette);
  
          if (pt_canvas->h_palette == NULL_HANDLE)
          {
              return i4_ret;
          }
      }
      
      if (t_cfg.b_auto_scale)
      {
          ui4_view_width  = t_cfg.ui4_width;
          ui4_view_height = t_cfg.ui4_height;
  
          pt_canvas->ui4_width  = t_cfg.ui4_width;
          pt_canvas->ui4_height = t_cfg.ui4_height;
      }
      else
      {
          ui4_view_width  = (UINT32) t_status.ui2_width;
          ui4_view_height = (UINT32) t_status.ui2_height;
  
          pt_canvas->ui4_width  = (UINT32) t_status.ui2_width;
          pt_canvas->ui4_height = (UINT32) t_status.ui2_height;
      }
      
      if (b_new_surf)
      {
          /* create front buffer */
          t_desc.ui4_width  = t_cfg.ui4_width;
          t_desc.ui4_height = t_cfg.ui4_height;
          t_desc.e_clrmode  = t_cfg.e_clr_mode;
          t_desc.h_palette  = t_cfg.h_palette;
          t_desc.h_clipper  = NULL_HANDLE;
		  
#ifdef MW_OSD_COMPRESSION_SHINK	
          if (_b_osd_compress)
       	  {
			  t_desc.ui4_height = t_cfg.ui4_height + COMPRESSION_EXTRA_LINES;
	   	  }
#endif          
          if (GLR_OK != x_gl_surface_create(&t_desc, &h_surf_front))
          {
              return i4_ret;
          }
          pt_canvas->h_surf_front = h_surf_front;

          x_gl_surface_fill(h_surf_front, &t_cfg.t_colorkey, 0);
          
          /* disable palette setting for internal buffers */
          t_desc.h_palette = NULL_HANDLE;
          
          /* create back buffer */
          if (GLR_OK != x_gl_surface_create(&t_desc, &h_surf_back))
          {
              return i4_ret;
          }
          pt_canvas->h_surf_back = h_surf_back;
  
          x_gl_surface_fill(h_surf_back, &t_cfg.t_colorkey, 0);
          
          if (((t_cfg.b_force_no_inter_compose != TRUE) &&
              (_b_inter_compose) &&
              (t_cfg.e_clr_mode == GL_COLORMODE_ARGB_D8888 ||
               t_cfg.e_clr_mode == GL_COLORMODE_ARGB_D4444 ||
               t_cfg.e_clr_mode == GL_COLORMODE_ARGB_D1555)) ||
               (_b_osd_compress))
          {
              HANDLE_T h_surf_render;
              
              /* create the third buffer (for alpha composition) */
              if (GLR_OK != x_gl_surface_create(&t_desc, &h_surf_render))
              {
                  return i4_ret;
              }
              pt_canvas->h_surf_render = h_surf_render;
          }
          if ((_b_support_compose_anim) &&
          (t_cfg.e_clr_mode == GL_COLORMODE_ARGB_D8888 ||
           t_cfg.e_clr_mode == GL_COLORMODE_ARGB_D4444 ||
           t_cfg.e_clr_mode == GL_COLORMODE_ARGB_D1555))
          {
              HANDLE_T h_surf_anim;
              
              /* create the third buffer (for alpha composition) */
              if (GLR_OK != x_gl_surface_create(&t_desc, &h_surf_anim))
              {
                  return i4_ret;
              }
              pt_canvas->h_surf_anim = h_surf_anim;
          }
#ifndef MW_OSD_COMPRESSION_SHINK	  
          if (_b_osd_compress)
          {
              HANDLE_T    h_surf_compress = NULL_HANDLE;
  
              /* create the buffer for storing compressed data */
              /* height = height + 1 for storing the 4-bit header each line in the worst compression case */
              t_desc.ui4_height = t_desc.ui4_height + COMPRESSION_EXTRA_LINES;
              t_desc.h_palette  = t_cfg.h_palette;
  
              if (x_gl_surface_create(&t_desc, &h_surf_compress) != GLR_OK)
              {
                  return i4_ret;
              }
              pt_canvas->h_surf_compress = h_surf_compress;
  
              /* create the back comrpession buffer */
              if (x_gl_surface_create(&t_desc, &h_surf_compress) != GLR_OK)
              {
                  return i4_ret;
              }
              pt_canvas->h_surf_compress_back = h_surf_compress;
          }
#endif
      }
      else
      {
#ifdef MW_OSD_COMPRESSION_SHINK	
          pt_canvas->h_surf_front = gl_surface_duplicate_handle(pt_ref_canvas->h_surf_front);
#else
          pt_canvas->h_surf_front = gl_surface_duplicate_handle(pt_ref_canvas->h_surf_back);
#endif
          if (NULL_HANDLE == pt_canvas->h_surf_front)
          {
             return i4_ret;
          }
  
          x_gl_surface_set_palette(pt_canvas->h_surf_front, t_cfg.h_palette);
          
#ifdef MW_OSD_COMPRESSION_SHINK	
          pt_canvas->h_surf_back = gl_surface_duplicate_handle(pt_ref_canvas->h_surf_back);
#else
          pt_canvas->h_surf_back = gl_surface_duplicate_handle(pt_ref_canvas->h_surf_front);
#endif
          if (NULL_HANDLE == pt_canvas->h_surf_back)
          {
              return i4_ret;
          }
          
          if (NULL_HANDLE != pt_ref_canvas->h_surf_render)
          {
              pt_canvas->h_surf_render = gl_surface_duplicate_handle(pt_ref_canvas->h_surf_render);
              if (NULL_HANDLE == pt_canvas->h_surf_render)
              {
                  return i4_ret;
              }
          }
          if (NULL_HANDLE != pt_ref_canvas->h_surf_anim)
          {
              pt_canvas->h_surf_anim = gl_surface_duplicate_handle(pt_ref_canvas->h_surf_anim);
              if (NULL_HANDLE == pt_canvas->h_surf_anim)
              {
                  return i4_ret;
              }
          }
#ifndef MW_OSD_COMPRESSION_SHINK	  
          if (pt_ref_canvas->h_surf_compress != NULL_HANDLE)
          {
              pt_canvas->h_surf_compress = gl_surface_duplicate_handle(pt_ref_canvas->h_surf_compress);
  
              if (pt_canvas->h_surf_compress == NULL_HANDLE)
              {
                  return i4_ret;
              }
          }
  
          if (pt_ref_canvas->h_surf_compress_back != NULL_HANDLE)
          {
              pt_canvas->h_surf_compress_back = gl_surface_duplicate_handle(pt_ref_canvas->h_surf_compress_back);
  
              if (pt_canvas->h_surf_compress_back == NULL_HANDLE)
              {
                 return i4_ret;
              }
          }
#endif
      }
      
      if (!_create_widget_view(
          ui4_view_width,
          ui4_view_height,
          t_cfg.b_auto_scale,
#ifndef MW_OSD_COMPRESSION_SHINK          
#ifdef LINUX_TURNKEY_SOLUTION_PHASE2
          (!_b_osd_compress)? pt_canvas->h_surf_back: pt_canvas->h_surf_compress,
#else
          (!_b_osd_compress)? pt_canvas->h_surf_front: pt_canvas->h_surf_compress,
#endif
#else
			pt_canvas->h_surf_front,
#endif
          &(t_cfg.t_colorkey),
          &h_view,
          &ui4_viewport))
      {
          return i4_ret;
      }
  
      pt_canvas->h_view       = h_view;
      pt_canvas->ui4_viewport = ui4_viewport;
  
      if (_b_osd_compress)
      {
          if (!_create_widget_view(
              ui4_view_width,
              ui4_view_height,
              t_cfg.b_auto_scale,
#ifndef MW_OSD_COMPRESSION_SHINK          
              pt_canvas->h_surf_compress_back,
#else
			  pt_canvas->h_surf_render,
#endif
              &(t_cfg.t_colorkey),
              &h_view,
              &ui4_viewport))
          {
              return i4_ret;
          }
  
#ifndef MW_OSD_COMPRESSION_SHINK        
          pt_canvas->h_view_compress       = h_view;
          pt_canvas->ui4_viewport_compress = ui4_viewport;
#else		  
          pt_canvas->h_view_back       = h_view;
          pt_canvas->ui4_viewport_back = ui4_viewport;
#endif
      }

      if (!pt_ref_canvas)
      {
          if (_b_osd_compress)
          {
              if (x_gl_surface_compressed_blt(
#ifndef MW_OSD_COMPRESSION_SHINK 
                  pt_canvas->h_surf_compress,
#else
				  pt_canvas->h_surf_front,
#endif
                  pt_canvas->h_surf_back,
                  0,
                  0,
                  t_cfg.ui4_width,
                  t_cfg.ui4_height,
                  _e_compression_ratio,
        #ifdef OSD_LINE_SEPARATE_COMPRESSION
                  TRUE,
        #else
                  FALSE,
        #endif
                  GL_SYNC) != GLR_OK)
              {
                  return i4_ret;
              }
          }
          
          /* flip the specified plane to the created widget view */
          if (GLR_OK != x_gl_plane_flip(h_plane, h_view, TRUE))
          {
              return i4_ret;
          }
      }
      pt_canvas->t_color_key     = t_cfg.t_colorkey;
      pt_canvas->b_auto_scale    = t_cfg.b_auto_scale;
      pt_canvas->e_color_mode    = t_cfg.e_clr_mode;
      
      return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _new_canvas
 *
 * Description: <function description>
 *
 * Inputs:  h_plane References.
 *
 * Outputs: - 
 *
 * Returns: NULL  Success.
 *
 ----------------------------------------------------------------------------*/
static _CANVAS_T* _new_canvas(
    GL_HPLANE_T             h_plane,
    const WGL_CANVAS_CFG_T* pt_canvas_cfg,
    const _CANVAS_T*        pt_ref_canvas)
{
    _CANVAS_T*          pt_canvas;
    HANDLE_T            h_canvas; 
    GL_COLOR_T          t_bg_color  = {0, {0}, {0}, {0}};

    pt_canvas = (_CANVAS_T*) MALLOC_CANVAS();
    if (!pt_canvas)
    {
        goto _ERR_RET;
    }

    pt_canvas->b_shadow_locked      = FALSE;
    pt_canvas->b_unlock_to_shadow   = FALSE;
    pt_canvas->pf_info_cb           = NULL;
    
    /* allocate canvas handle */
    pt_canvas->h_this = NULL_HANDLE;
    if (HR_OK != handle_alloc(
        HT_WGL_CANVAS, 
        (VOID*) pt_canvas, 
        NULL, 
        _handle_free_canvas, 
        &h_canvas))
    {
        goto _ERR_RET;
    }
    if(WGLR_OK != _new_canvas_resource(pt_canvas,h_plane,pt_canvas_cfg,pt_ref_canvas))
    {
       goto _ERR_RET;
    }
#if 0 //def CLI_LVL_ALL
          _ui4_canvas_count++;
  
          if (_ui4_canvas_count >= MAX_CANVAS_NUMBER)
          {
              _ui4_canvas_count = MAX_CANVAS_NUMBER - 1;
          }
#endif
    pt_canvas->b_running       = (pt_ref_canvas ? FALSE : TRUE);
    pt_canvas->b_with_visibles = FALSE;
    pt_canvas->pt_ref          = (_CANVAS_T*) pt_ref_canvas;
    pt_canvas->pt_shadow       = NULL;
    pt_canvas->h_this          = h_canvas;
    pt_canvas->t_bg_color      = t_bg_color;
    pt_canvas->h_bg_image      = NULL_HANDLE;
    pt_canvas->ui4_bg_width    = 0;
    pt_canvas->ui4_bg_height   = 0;
    pt_canvas->e_bg_mode       = WGL_BG_MAP_CENTERED;
    pt_canvas->t_dflt_color    = t_bg_color;
    
    DLIST_INIT(&(pt_canvas->t_branch_lst));
    SLIST_INIT(&(pt_canvas->t_cvs_cb_lst));
    DLIST_INIT(&(pt_canvas->t_transp_list));
    
    SLIST_INSERT_HEAD(pt_canvas, &_t_canvas_lst, t_next);

#ifdef WM_DEBUG
    i4_canvas_cnt++;
    pt_canvas->i4_branch_cnt = 0;
    pt_canvas->i4_widget_cnt = 0;
#endif

    return (pt_canvas);
    
_ERR_RET:
    if (pt_canvas)
    {
        _free_canvas(pt_canvas);
    }
    
    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: _new_widget
 *
 * Description: <function description>
 *
 * Inputs:  e_type References.
 *          pv_tag References.
 *
 * Outputs: - 
 *
 * Returns: NULL  Success.
 *
 ----------------------------------------------------------------------------*/
static _WIDGET_T* _new_widget(HANDLE_TYPE_T e_type, VOID* pv_tag)
{
    _WIDGET_T*  pt_widget;
    HANDLE_T    h_widget;
    
    pt_widget = (_WIDGET_T*) MALLOC_WIDGET();
    CHECK_VALID(pt_widget, NULL);
#ifdef CLI_LVL_ALL     
    pt_widget->ui4_mem_size = sizeof(_WIDGET_T);
#endif      
    pt_widget->h_this   = NULL_HANDLE;

    if (HR_OK != handle_alloc(
        e_type, 
        (VOID*) pt_widget, 
        pv_tag, 
        _handle_free_widget, 
        &h_widget))
    {
        goto _NEW_WIDGET_RET;
    }

    pt_widget->h_this = h_widget;

    SLIST_INIT(&(pt_widget->t_tm_lst));
    
    return (pt_widget);

_NEW_WIDGET_RET:

    if (pt_widget)
    {
        MFREE_WIDGET(pt_widget);
    }

    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: _find_app_info
 *
 * Description: <function description>
 *
 * Inputs:  h_thread References.
 *
 * Outputs: - 
 *
 * Returns: NULL  Success.
 *
 ----------------------------------------------------------------------------*/
static _APP_INFO_T* _find_app_info(HANDLE_T h_thread)
{
    _APP_INFO_T*    pt_app_info;
    
    DLIST_FOR_EACH(pt_app_info, &_t_app_lst, t_link)
    {
        if (pt_app_info->h_thread == h_thread)
        {
            return (pt_app_info);
        }
    }

    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: _new_app_info
 *
 * Description: <function description>
 *
 * Inputs:  h_thread    References.
 *          pf_callback References.
 *
 * Outputs: - 
 *
 * Returns: NULL  Success.
 *
 ----------------------------------------------------------------------------*/
static _APP_INFO_T* _new_app_info(HANDLE_T h_thread, wgl_app_callback_fct pf_callback)
{
    CRIT_STATE_T    t_cs;
    _APP_INFO_T*    pt_app_info;
    
    pt_app_info = (_APP_INFO_T*) MALLOC_APP_INFO();
    if (!pt_app_info)
    {
        return NULL;
    }
    
    if (HR_OK != handle_alloc(
                    HT_WGL_CLIENT, 
                    pt_app_info, 
                    NULL, 
                    _handle_free_app, 
                    &(pt_app_info->h_this)))
    {
        MFREE_APP_INFO(pt_app_info);
        return NULL;
    }
    
    pt_app_info->h_thread            = h_thread;
    pt_app_info->pf_callback         = pf_callback;
    pt_app_info->pt_widget_focused   = NULL;
    pt_app_info->pt_widget_activated = NULL;
    pt_app_info->ui2_state           = 0;
    pt_app_info->ui4_max_delay       = WGL_NO_KEY_SKIP;
    
    pt_app_info->b_msg_not_processed_by_widget = FALSE;
    pt_app_info->ui4_msg = 0;
    pt_app_info->pv_param1 = NULL;
    pt_app_info->pv_param2 = NULL;
    DLIST_INIT(&(pt_app_info->t_branch_lst));

    t_cs = x_crit_start();
    DLIST_INSERT_HEAD(pt_app_info, &_t_app_lst, t_link);
    x_crit_end(t_cs);
#ifdef WGL_ANIM_SUPPORT
    pt_app_info->ui1_anim_app_idx = 0;
#endif
#ifdef WM_DEBUG
    i4_app_cnt++;
    pt_app_info->i4_branch_cnt = 0;
    pt_app_info->i4_widget_cnt = 0;
#endif

    return (pt_app_info);
}

/*-----------------------------------------------------------------------------
 * Name: _new_branch
 *
 * Description: <function description>
 *
 * Inputs:  pt_app_info References.
 *          pt_canvas   References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static _BRANCH_T* _new_branch(_APP_INFO_T* pt_app_info, _CANVAS_T* pt_canvas)
{
    _BRANCH_T*      pt_branch;
    /* CRIT_STATE_T    t_cs; */
    
    pt_branch = (_BRANCH_T*) MALLOC_BRANCH();
    CHECK_VALID(pt_branch, NULL);

    FIRST(pt_branch)  = NULL;
    CANVAS(pt_branch) = pt_canvas;
    OWNER(pt_branch)  = pt_app_info;
    DLIST_INSERT_HEAD(pt_branch, &(pt_app_info->t_branch_lst), t_link_on_app);

    /* t_cs = x_crit_start(); */
    if (_pt_active_app == pt_app_info)
    {   /* owner app is the current active app */
        DLIST_INSERT_TAIL(pt_branch, &(pt_canvas->t_branch_lst), t_link_on_canvas);
    }
    else
    {
        DLIST_INSERT_HEAD(pt_branch, &(pt_canvas->t_branch_lst), t_link_on_canvas);
    }

    _move_app_to_end(pt_app_info, pt_app_info->ui2_state);

    /* x_crit_end(t_cs); */
    
#ifdef WM_DEBUG
    i4_branch_cnt++;
    pt_branch->i4_widget_cnt = 0;
    pt_app_info->i4_branch_cnt++;
    pt_canvas->i4_branch_cnt++;
#endif

    return (pt_branch);
}

/*-----------------------------------------------------------------------------
 * Name: _screen_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  h_obj       References.
 *          e_nfy_cond  References.
 *          pv_nfy_tag  References.
 *          ui4_data    References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static INT32 _screen_nfy_fct(
    HANDLE_T     h_obj,
    GL_COND_T    e_nfy_cond,
    VOID*        pv_nfy_tag,
    UINT32       ui4_data)
{
    _CANVAS_T*  pt_canvas = (_CANVAS_T*) pv_nfy_tag;

    DBG_INFO(("{WGL} INFO: Screen callback (%d) at L%d\r\n\r\n", __LINE__));
    
    switch (e_nfy_cond)
    {
        case GL_COND_DISPLAY_CHANGED:
            DBG_INFO(("{WGL} INFO: Received GL_COND_DISPLAY_CHANGED at L%d\r\n\r\n", __LINE__));
            if (!_b_auto_scale)
            {
                _APP_NOTIFY_MSG_T   t_msg;
                CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
                pt_canvas->ui4_width  = GET_HI_WORD(ui4_data);
                pt_canvas->ui4_height = GET_LO_WORD(ui4_data);
                CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
                /* sending app-notify message to repaint manager thread */
                t_msg.ui4_msg   = RPM_MSG_NOTIFY_APP;
                t_msg.pt_canvas = pt_canvas;
                t_msg.ui4_wmsg  = WGL_MSG_RESIZE_CANVAS;
                t_msg.pv_param1 = (VOID*) ui4_data;
                t_msg.pv_param2 = NULL;
                
                x_msg_q_send(
                    _t_repaint_mngr.h_msg_q, 
                    &t_msg, 
                    sizeof(t_msg),
                    RPM_MSG_PRIORITY_NOTIFY);
            }
            break;

        default:
            break;
    }
    
    
    return GLR_OK;
}

static INT32 _send_canvas_notify(
    _CANVAS_T*  pt_canvas,
    UINT32      ui4_msg,
    VOID*       pv_param1,
    VOID*       pv_param2)
{
    _CANVAS_CALLBACK_T* pt_cvs_cb;
    
    SLIST_FOR_EACH(pt_cvs_cb, &(pt_canvas->t_cvs_cb_lst), t_link)
    {
        if (pt_cvs_cb->pf_cvs_nfy_fct)
        {
            (*(pt_cvs_cb->pf_cvs_nfy_fct))(
                ui4_msg,
                pt_canvas->h_this,
                pv_param1,
                pv_param2);
        }
    }

    return WGLR_OK;
}

static INT32 _send_canvas_switch_notify(_CANVAS_T* pt_canvas)
{
    BOOL b_visible = FALSE;
    
    if (pt_canvas->b_running)
    {
        b_visible = _any_widget_visible(pt_canvas, FALSE);
        pt_canvas->b_with_visibles = b_visible;
    }
    
    return (_send_canvas_notify(
        pt_canvas, 
        WGL_MSG_SWITCH_CANVAS, 
        WGL_PACK(pt_canvas->b_running), 
        WGL_PACK(b_visible)));
}

static INT32 _send_canvas_update_notify(_CANVAS_T* pt_canvas)
{
    if (pt_canvas->b_running)
    {
        BOOL b_visible = FALSE;
    
        b_visible = _any_widget_visible(pt_canvas, FALSE);

        if (pt_canvas->b_with_visibles != b_visible)
        {
            pt_canvas->b_with_visibles = b_visible;

            return (_send_canvas_notify(
                pt_canvas, 
                WGL_MSG_UPDATE_CANVAS, 
                WGL_PACK(pt_canvas->b_running), 
                WGL_PACK(b_visible)));
        }
    }

    return WGLR_OK;    
}

/*-----------------------------------------------------------------------------
 * Name: _send_msg
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *          ui4_msg   References.
 *          pv_param1 References.
 *          pv_param2 References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static INT32 _send_msg(
    _WIDGET_T*  pt_widget, 
    UINT32      ui4_msg, 
    VOID*       pv_param1,
    VOID*       pv_param2)
{
    return (CALL_MSGPROC(pt_widget, ui4_msg, pv_param1, pv_param2));
}

#ifdef WGL_ANIM_SUPPORT   
static INT32 _send_anim_tick_msg(VOID)
{
    INT32           i4_ret, i4_i;
    _ANIM_MSG_T     t_msg;

    t_msg.e_type = ANIM_MSG_TICK;

    for (i4_i = 0; i4_i < WGL_SEND_MSG_RETRY_TIMES; i4_i++)
    {
        i4_ret = x_msg_q_send(
            _t_anim_mngr.h_anim_msg_q,
            (VOID *)&t_msg,
            sizeof(_ANIM_MSG_T),
            WM_ANIMATION_MSG_PRIORITY);

        if (i4_ret == OSR_OK)
        {
            break;
        }
        else
        {
            DBG_ERROR(("{WGL} send the animation tick message failed. (%d)\n", i4_ret));
            /* delay and re-try */
            x_thread_delay(10);
        }
    }

    return i4_ret;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: _setup_active_app
 *
 * Description: <function description>
 *
 * Inputs:  pt_app_info References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _setup_active_app(_APP_INFO_T* pt_app_info)
{
    CRIT_STATE_T    t_cs;
    
    t_cs = x_crit_start();

    if (_i4_active_cnt < 0)
    {
        _i4_active_cnt++;
    }
    
    if (_pt_active_app)
    {
        _pt_active_app->ui2_state &= (~AIS_ACTIVE);
    }

    _pt_active_app = pt_app_info;
    _pt_active_app->ui2_state |= AIS_ACTIVE;
    
    /* move the apecified _APP_INFO_T to the head of the appinfo list */
    DLIST_REMOVE(pt_app_info, &_t_app_lst, t_link);
    DLIST_INSERT_TAIL(pt_app_info, &_t_app_lst, t_link);

    x_crit_end(t_cs);
    
    /* move all branches belonged to pt_app_info to the tail of their 
         corresponding canvas branch list */
    _move_app_to_end(pt_app_info, pt_app_info->ui2_state);
}

/*-----------------------------------------------------------------------------
 * Name: _activate_app
 *
 * Description: <function description>
 *
 * Inputs:  h_thread References.
 *
 * Outputs: - 
 *
 * Returns: TRUE   Success.
 *          FALSE  Fail.
 *
 ----------------------------------------------------------------------------*/
static BOOL _activate_app(HANDLE_T h_thread)
{
    _APP_INFO_T*    pt_app_info;
    /* CRIT_STATE_T    t_cs; */
    
    /* t_cs = x_crit_start(); */
    if (_pt_active_app && _pt_active_app->h_thread == h_thread)
    {
        /* same app handle as the current active app */
        /* x_crit_end(t_cs); */
        return TRUE;
    }
    /* x_crit_end(t_cs); */
    
    pt_app_info = _find_app_info(h_thread);
    
    if (pt_app_info)
    {
        _setup_active_app(pt_app_info);
        
        return TRUE;
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _remove_from_list
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _remove_from_list(_WIDGET_T* pt_widget)
{
    _WIDGET_T*  pt_first = GET_HEAD(pt_widget);
        
    if (pt_first == pt_widget)
    {
        if (AT_TOP_LEVEL(pt_widget))
        {
            FIRST(BRANCH(pt_widget)) = SIBLING(pt_widget);
        }
        else
        {
            SIBLING_HEAD(pt_widget) = SIBLING(pt_widget);
        }
    }
    else
    {
        FIND_PREV(pt_first, pt_widget);
        SIBLING(pt_first) = SIBLING(pt_widget);
    }
}

static VOID _reorder_widget_with_next(_WIDGET_T* pt_this)
{
    _WIDGET_T* pt_head;
    _WIDGET_T* pt_next = SIBLING(pt_this);

    _remove_from_list(pt_this);
    
    pt_head = GET_HEAD(pt_next);
    
    SIBLING(pt_this) = SIBLING(pt_head);
    SIBLING(pt_head) = pt_this;
}

/*-----------------------------------------------------------------------------
 * Name: _insert_widget_no_ref
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *          e_insert  References.
 *
 * Outputs: - 
 *
 * Returns: FALSE  Success.
 *          TRUE   Fail.
 *
 ----------------------------------------------------------------------------*/
static BOOL _insert_widget_no_ref(_WIDGET_T* pt_widget, WGL_INSERT_MODE_T e_insert)
{
    if (WGL_INSERT_AFTER == e_insert) /* push one level down */
    {
        _WIDGET_T*  pt_tmp;
        
        if (AT_SIBLING_HEAD(pt_widget))
        {   /* already at head */
            return FALSE;
        }
        
        pt_tmp = GET_HEAD(pt_widget);
        
        FIND_PREV(pt_tmp, pt_widget);
        
        _reorder_widget_with_next(pt_tmp);
    }
    else if (WGL_INSERT_BEFORE == e_insert) /* lift one level up */
    {
        if (AT_SIBLING_TAIL(pt_widget))
        {   /* already at tail */
            return FALSE;
        }

        _reorder_widget_with_next(pt_widget);
    }
    else if (WGL_INSERT_TOPMOST == e_insert)
    {
        _WIDGET_T* pt_tmp;
        
        if (AT_SIBLING_TAIL(pt_widget))
        {   /* already at tail */
            return FALSE;
        }
        
        _remove_from_list(pt_widget);
        
        pt_tmp = GET_HEAD(pt_widget);
        
        FIND_PREV(pt_tmp, NULL); /* find the last one in the list */
        SIBLING(pt_tmp)    = pt_widget;
        SIBLING(pt_widget) = NULL;
    }
    else /* WGL_INSERT_BOTTOMMOST == e_insert */
    {
        if (AT_SIBLING_HEAD(pt_widget))
        {   /* already at head */
            return FALSE;
        }
        
        _remove_from_list(pt_widget);
        
        if (AT_TOP_LEVEL(pt_widget))
        {
            SIBLING(pt_widget)       = FIRST(BRANCH(pt_widget));
            FIRST(BRANCH(pt_widget)) = pt_widget;
        }
        else
        {
            SIBLING(pt_widget)      = SIBLING_HEAD(pt_widget);
            SIBLING_HEAD(pt_widget) = pt_widget;
        }
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _insert_tail
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *          pt_head   References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static _WIDGET_T* _insert_tail(_WIDGET_T* pt_widget, _WIDGET_T* pt_head)
{
    _WIDGET_T*  pt_prev;
    
    if (AT_SIBLING_TAIL(pt_widget))
    {
        return (pt_head);
    }

    if (pt_head == pt_widget)
    {
        pt_prev = pt_head = SIBLING(pt_widget);
    }
    else
    {
        pt_prev = pt_head;
        FIND_PREV(pt_prev, pt_widget);
        SIBLING(pt_prev) = SIBLING(pt_widget);
    }

    FIND_PREV(pt_prev, NULL);

    SIBLING(pt_prev)   = pt_widget;
    SIBLING(pt_widget) = NULL;

    return (pt_head);
}

static VOID _move_app_to_end(
    _APP_INFO_T*    pt_app_info, 
    UINT16          ui2_flag)
{
    #define ZFLAGS(f)   ((f) & (AIS_TOPMOST | AIS_ACTIVE | AIS_BOTTOMMOST))
    #define ZSCORE(f)   \
        ((INT32)(((f) & AIS_TOPMOST) ? 2 : 0) + (((f) & AIS_ACTIVE) ? 1 : 0) + (((f) & AIS_BOTTOMMOST) ? -2 : 0))

    _BRANCH_T*  pt_branch;
    _BRANCH_T*  pt_ref_branch;

    if (ZFLAGS(ui2_flag) == (AIS_ACTIVE | AIS_TOPMOST))
    {
        /* move all branches belonged to pt_app_info to the tail of their 
             corresponding canvas branch list */
        DLIST_FOR_EACH(pt_branch, &(pt_app_info->t_branch_lst), t_link_on_app)
        {
            DLIST_REMOVE(
                pt_branch, 
                &(pt_branch->pt_canvas->t_branch_lst), 
                t_link_on_canvas);
            DLIST_INSERT_TAIL(
                pt_branch, 
                &(pt_branch->pt_canvas->t_branch_lst), 
                t_link_on_canvas);
        }
    }
#if 0    
    else if (ZFLAGS(ui2_flag) == AIS_BOTTOMMOST)
    {
        /* move all branches belonged to pt_app_info to the head of their 
             corresponding canvas branch list */
        DLIST_FOR_EACH(pt_branch, &(pt_app_info->t_branch_lst), t_link_on_app)
        {
            DLIST_REMOVE(
                pt_branch, 
                &(pt_branch->pt_canvas->t_branch_lst), 
                t_link_on_canvas);
            DLIST_INSERT_HEAD(
                pt_branch, 
                &(pt_branch->pt_canvas->t_branch_lst), 
                t_link_on_canvas);
        }
    }
#endif    
    else
    {
        DLIST_FOR_EACH(pt_branch, &(pt_app_info->t_branch_lst), t_link_on_app)
        {
            DLIST_REMOVE(
                pt_branch, 
                &(pt_branch->pt_canvas->t_branch_lst), 
                t_link_on_canvas);

            DLIST_FOR_EACH(pt_ref_branch, &(pt_branch->pt_canvas->t_branch_lst), t_link_on_canvas)
            {
                if (ZSCORE(ZFLAGS(pt_ref_branch->pt_owner_app->ui2_state)) > ZSCORE(ZFLAGS(ui2_flag)))
                {
                    break;
                }
            }

            if (pt_ref_branch)
            {
                DLIST_INSERT_BEFORE(
                    pt_branch,
                    pt_ref_branch, 
                    &(pt_branch->pt_canvas->t_branch_lst), 
                    t_link_on_canvas);
            }
            else
            {
                DLIST_INSERT_TAIL(
                    pt_branch, 
                    &(pt_branch->pt_canvas->t_branch_lst), 
                    t_link_on_canvas);
            }
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _remove_widget
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *          pt_rect   References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static BOOL _remove_widget(_WIDGET_T* pt_widget, GL_RECT_T* pt_rect)
{
    _APP_INFO_T*    pt_app_info;
    _BRANCH_T*      pt_branch;
    _WIDGET_T*      pt_tmp;
    _WIDGET_T*      pt_parent;
    INT32           i4_x, i4_y;
    BOOL            b_ret = FALSE;
    
    pt_branch   = BRANCH(pt_widget);
    pt_app_info = OWNER(pt_branch);
    pt_parent   = PARENT(pt_widget);
    
    if (pt_parent)
    {   /* non-top-level widget */
        pt_tmp = CHILD(pt_parent);

        if (pt_tmp == pt_widget)
        {
            CHILD(pt_parent) = SIBLING(pt_widget);
        }
        else
        {
            FIND_PREV(pt_tmp, pt_widget);
            SIBLING(pt_tmp) = SIBLING(pt_widget);
        }

        /* if the current focused widget is pt_widget or one of the children of pt_widget,
             move the focus to pt_widget's parent */
        pt_tmp = FOCUSED(pt_app_info);
        while (pt_tmp)
        {
            if (pt_tmp == pt_widget)
            {
                /* TODO: should check whether pt_parent can be focused or not */
                b_ret = ST_VISIBLE(pt_parent);
                if (b_ret)
                {
                    *pt_rect = pt_parent->t_rect;
                    _cal_translation(PARENT(pt_parent), &i4_x, &i4_y);
                    MOVE_RECT(pt_rect, i4_x, i4_y);
                }
                FOCUSED(pt_app_info) = pt_parent;
                STATE(pt_parent)    |= WS_FOCUSED;
                break;
            }
            pt_tmp = PARENT(pt_tmp);
        }

        PARENT(pt_widget) = NULL;
    }
    else
    {   /* top-level widget */
        pt_tmp = FIRST(pt_branch);

        if (pt_tmp == pt_widget)
        {
            pt_tmp           = SIBLING(pt_widget);
            FIRST(pt_branch) = pt_tmp;
        }
        else
        {
            FIND_PREV(pt_tmp, pt_widget);
            SIBLING(pt_tmp) = SIBLING(pt_widget);
        }

        if (ACTIVATED(pt_app_info) == pt_widget)
        {   /* activated widget is removed, move one node backward */
            ACTIVATED(pt_app_info) = pt_tmp;
            FOCUSED(pt_app_info)   = pt_tmp;
            if (pt_tmp)
            {
                /* TODO: should check whether pt_tmp can be focused or not */
                b_ret = ST_VISIBLE(pt_tmp);
                if (b_ret)
                {
                    *pt_rect = pt_tmp->t_rect;
                    _cal_translation(PARENT(pt_tmp), &i4_x, &i4_y);
                    MOVE_RECT(pt_rect, i4_x, i4_y);
                }
                STATE(pt_tmp) |= (WS_ACTIVATED | WS_FOCUSED);
            }
            else
            {
                b_ret = FALSE;
            }
        }
    }

    SIBLING(pt_widget) = NULL;
    BRANCH(pt_widget)  = NULL;

    return (b_ret);
}

/*-----------------------------------------------------------------------------
 * Name: _delete_widget_group
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _delete_widget_group(_WIDGET_T* pt_widget)
{
    _WIDGET_T* pt_sibling = SIBLING(pt_widget);
    _WIDGET_T* pt_child   = CHILD(pt_widget);
    
    /* reset all outward links */
    BRANCH(pt_widget)  = NULL;
    PARENT(pt_widget)  = NULL;
    SIBLING(pt_widget) = NULL;
    CHILD(pt_widget)   = NULL;

    handle_free(pt_widget->h_this, FALSE);

    if (pt_sibling)
    {
        _delete_widget_group(pt_sibling);
    }
    
    if (pt_child)
    {
        _delete_widget_group(pt_child);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _focusable
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *
 * Outputs: - 
 *
 * Returns: FALSE  Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _focusable(_WIDGET_T* pt_widget)
{
    if (pt_widget == NULL)
    {
        return FALSE;
    }
    
    if (0 == (pt_widget->ui4_attrib & WGL_ATTR_FOCUSABLE))
    {
        return FALSE;
    }

    while (pt_widget && ST_ENABLED(pt_widget))
    {
        pt_widget = PARENT(pt_widget);
    }

    return (!pt_widget);
}

/*-----------------------------------------------------------------------------
 * Name: _check_navigation_key
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *          ui4_msg   References.
 *          ui4_key   References.
 *
 * Outputs: - 
 *
 * Returns: NULL_HANDLE  Success.
 *
 ----------------------------------------------------------------------------*/
static HANDLE_T _check_navigation_key(
    _WIDGET_T*  pt_widget, 
    UINT32      ui4_msg, 
    UINT32      ui4_key)
{
    if (WGL_MSG_KEY_DOWN == ui4_msg && (pt_widget->ui1_num_key_link != 0))
    {
        INT32           i;
        WGL_KEY_LINK_T* pt_link = pt_widget->at_key_lnk;

        for (i = 0; i < pt_widget->ui1_num_key_link; i++)
        {
            if (pt_link[i].ui4_key == ui4_key)
            {
                return (pt_link[i].h_widget);
            }
        }
    }

    return NULL_HANDLE;
}

static VOID _fill_exclusive_region(GL_RECT_T*           pt_ori_rect,
										   INT32                  i4_current_rect,
										   UINT32                ui4_rect_count,
										   GL_RECT_T*      	    pt_rect,
										   GL_LOCK_INFO_T*    pt_lock_info,
										   GL_COLOR_T*          pt_color)
{
	INT32 i4_count = -2;
	GL_RECT_T rect1 = {0}, rect2 = {0}, rect3 = {0}, rect4 = {0};

	i4_count = _cal_exclusive_region(pt_ori_rect, 
								   				  &pt_rect[i4_current_rect],
								   				  &rect1,
								   				  &rect2,
								   				  &rect3,
								   				  &rect4);
	
    if (i4_current_rect+1 == ui4_rect_count)
    {
    	if (i4_count == -1)
    	{
			gl_gpu_fill_rect(pt_lock_info->e_clrmode,
	                                pt_lock_info->at_subplane[0].pv_bits,
	                                pt_lock_info->at_subplane[0].ui4_pitch,
	                                pt_ori_rect->i4_left,
	                                pt_ori_rect->i4_top,
	                                (UINT32)RECT_W(pt_ori_rect),
	                                (UINT32)RECT_H(pt_ori_rect),
	                                gl_compose_packed_color(pt_color, pt_lock_info->e_clrmode));
			return ;
    	}else if (i4_count == 0){
			return ;
		}else if (i4_count == 1){
			gl_gpu_fill_rect(pt_lock_info->e_clrmode,
	                                pt_lock_info->at_subplane[0].pv_bits,
	                                pt_lock_info->at_subplane[0].ui4_pitch,
	                                rect1.i4_left,
	                                rect1.i4_top,
	                                (UINT32)RECT_W(&rect1),
	                                (UINT32)RECT_H(&rect1),
	                                gl_compose_packed_color(pt_color, pt_lock_info->e_clrmode));
			return ;
		}else if (i4_count == 2){
			gl_gpu_fill_rect(pt_lock_info->e_clrmode,
	                                pt_lock_info->at_subplane[0].pv_bits,
	                                pt_lock_info->at_subplane[0].ui4_pitch,
	                                rect1.i4_left,
	                                rect1.i4_top,
	                                (UINT32)RECT_W(&rect1),
	                                (UINT32)RECT_H(&rect1),
	                                gl_compose_packed_color(pt_color, pt_lock_info->e_clrmode));

			gl_gpu_fill_rect(pt_lock_info->e_clrmode,
	                                pt_lock_info->at_subplane[0].pv_bits,
	                                pt_lock_info->at_subplane[0].ui4_pitch,
	                                rect2.i4_left,
	                                rect2.i4_top,
	                                (UINT32)RECT_W(&rect2),
	                                (UINT32)RECT_H(&rect2),
	                                gl_compose_packed_color(pt_color, pt_lock_info->e_clrmode));
			return ;
			
		}else if (i4_count == 3){
			gl_gpu_fill_rect(pt_lock_info->e_clrmode,
	                                pt_lock_info->at_subplane[0].pv_bits,
	                                pt_lock_info->at_subplane[0].ui4_pitch,
	                                rect1.i4_left,
	                                rect1.i4_top,
	                                (UINT32)RECT_W(&rect1),
	                                (UINT32)RECT_H(&rect1),
	                                gl_compose_packed_color(pt_color, pt_lock_info->e_clrmode));

			gl_gpu_fill_rect(pt_lock_info->e_clrmode,
	                                pt_lock_info->at_subplane[0].pv_bits,
	                                pt_lock_info->at_subplane[0].ui4_pitch,
	                                rect2.i4_left,
	                                rect2.i4_top,
	                                (UINT32)RECT_W(&rect2),
	                                (UINT32)RECT_H(&rect2),
	                                gl_compose_packed_color(pt_color, pt_lock_info->e_clrmode));

			gl_gpu_fill_rect(pt_lock_info->e_clrmode,
	                                pt_lock_info->at_subplane[0].pv_bits,
	                                pt_lock_info->at_subplane[0].ui4_pitch,
	                                rect3.i4_left,
	                                rect3.i4_top,
	                                (UINT32)RECT_W(&rect3),
	                                (UINT32)RECT_H(&rect3),
	                                gl_compose_packed_color(pt_color, pt_lock_info->e_clrmode));
			return ;
		}else if (i4_count == 4){
			gl_gpu_fill_rect(pt_lock_info->e_clrmode,
									pt_lock_info->at_subplane[0].pv_bits,
									pt_lock_info->at_subplane[0].ui4_pitch,
									rect1.i4_left,
									rect1.i4_top,
									(UINT32)RECT_W(&rect1),
									(UINT32)RECT_H(&rect1),
									gl_compose_packed_color(pt_color, pt_lock_info->e_clrmode));

			gl_gpu_fill_rect(pt_lock_info->e_clrmode,
									pt_lock_info->at_subplane[0].pv_bits,
									pt_lock_info->at_subplane[0].ui4_pitch,
									rect2.i4_left,
									rect2.i4_top,
									(UINT32)RECT_W(&rect2),
									(UINT32)RECT_H(&rect2),
									gl_compose_packed_color(pt_color, pt_lock_info->e_clrmode));

			gl_gpu_fill_rect(pt_lock_info->e_clrmode,
									pt_lock_info->at_subplane[0].pv_bits,
									pt_lock_info->at_subplane[0].ui4_pitch,
									rect3.i4_left,
									rect3.i4_top,
									(UINT32)RECT_W(&rect3),
									(UINT32)RECT_H(&rect3),
									gl_compose_packed_color(pt_color, pt_lock_info->e_clrmode));

		
			gl_gpu_fill_rect(pt_lock_info->e_clrmode,
	                                pt_lock_info->at_subplane[0].pv_bits,
	                                pt_lock_info->at_subplane[0].ui4_pitch,
	                                rect4.i4_left,
	                                rect4.i4_top,
	                                (UINT32)RECT_W(&rect4),
	                                (UINT32)RECT_H(&rect4),
	                                gl_compose_packed_color(pt_color, pt_lock_info->e_clrmode));
			return ;
		}
		return ;
	}	else{

		if (i4_count == -1){
			_fill_exclusive_region(pt_ori_rect,
											 i4_current_rect + 1,
											 ui4_rect_count,
											 pt_rect,
											 pt_lock_info,
											 pt_color);
    	}else if (i4_count == 0){
			return ;
		}
		else if (i4_count == 1){
			 _fill_exclusive_region(&rect1,
											 i4_current_rect + 1,
											 ui4_rect_count,
											 pt_rect,
											 pt_lock_info,
											 pt_color);
			
		}else if (i4_count == 2){
			 _fill_exclusive_region(&rect1,
											 i4_current_rect + 1,
											 ui4_rect_count,
											 pt_rect,
											 pt_lock_info,
											 pt_color);

			_fill_exclusive_region(&rect2,
											 i4_current_rect + 1,
											 ui4_rect_count,
											 pt_rect,
											 pt_lock_info,
											 pt_color);
			
		}else if (i4_count == 3){
			_fill_exclusive_region(&rect1,
											 i4_current_rect + 1,
											 ui4_rect_count,
											 pt_rect,
											 pt_lock_info,
											 pt_color);

			_fill_exclusive_region(&rect2,
											 i4_current_rect + 1,
											 ui4_rect_count,
											 pt_rect,
											 pt_lock_info,
											 pt_color);

			_fill_exclusive_region(&rect3,
											 i4_current_rect + 1,
											 ui4_rect_count,
											 pt_rect,
											 pt_lock_info,
											 pt_color);
		}else if (i4_count == 4){
			 _fill_exclusive_region(&rect1,
											 i4_current_rect + 1,
											 ui4_rect_count,
											 pt_rect,
											 pt_lock_info,
											 pt_color);

			_fill_exclusive_region(&rect2,
											 i4_current_rect + 1,
											 ui4_rect_count,
											 pt_rect,
											 pt_lock_info,
											 pt_color);

			 _fill_exclusive_region(&rect3,
											 i4_current_rect + 1,
											 ui4_rect_count,
											 pt_rect,
											 pt_lock_info,
											 pt_color);

		
			 _fill_exclusive_region(&rect4,
											 i4_current_rect + 1,
											 ui4_rect_count,
											 pt_rect,
											 pt_lock_info,
											 pt_color);
       }
		return ;
	}
}

static INT32 _cal_exclusive_region(
    GL_RECT_T       *pt_ori_rect,
    GL_RECT_T       *pt_exc_rect,
    GL_RECT_T       *pt_out_rect1,
    GL_RECT_T       *pt_out_rect2,
    GL_RECT_T       *pt_out_rect3,
    GL_RECT_T       *pt_out_rect4)
{
    GL_RECT_T       t_result;

	x_dbg_stmt("{%s %d}new added.\n", __FUNCTION__, __LINE__);
	x_dbg_stmt("{%s %d}new added.\n", __FUNCTION__, __LINE__);	
	x_dbg_stmt("{%s %d}new added.\n", __FUNCTION__, __LINE__);

	if (pt_ori_rect == NULL || pt_exc_rect == NULL || pt_out_rect1 == NULL || pt_out_rect2 == NULL	|| pt_out_rect3 == NULL || pt_out_rect4 == NULL)
	{
		return 0;
	} 

	INTERSECT_RECT(&t_result, pt_ori_rect, pt_exc_rect);

	/* not excluded */
	if (RECT_EMPTY(&t_result))
	{
		SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
		
		return -1;
	}
	else
	{
		/* all excluded */
		if (RECT_INCLUDED(pt_ori_rect, &t_result))
		{
			return 0;
		}
		/* partial excluded */
		else
		{
			if (pt_ori_rect->i4_top >= pt_exc_rect ->i4_top)
			{
				 if(pt_ori_rect->i4_left >= pt_exc_rect->i4_left)
				 {
						if(pt_ori_rect->i4_bottom >= pt_exc_rect->i4_bottom && pt_ori_rect->i4_right > pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_exc_rect->i4_right, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_exc_rect->i4_bottom);
							 SET_RECT(pt_out_rect2, pt_ori_rect->i4_left,  pt_exc_rect->i4_bottom, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
							 return 2;
						}
						if(pt_ori_rect->i4_bottom < pt_exc_rect->i4_bottom && pt_ori_rect->i4_right >= pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_exc_rect->i4_right, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
							 return 1;
						}
						if(pt_ori_rect->i4_bottom >= pt_exc_rect->i4_bottom && pt_ori_rect->i4_right <= pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_exc_rect->i4_bottom, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
							 return 1;
						}
				 }
				 else
				 {
						if(pt_ori_rect->i4_bottom >= pt_exc_rect->i4_bottom && pt_ori_rect->i4_right > pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_exc_rect->i4_left, pt_exc_rect->i4_bottom);
							 SET_RECT(pt_out_rect2, pt_exc_rect->i4_right, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_exc_rect->i4_bottom);
							 SET_RECT(pt_out_rect3, pt_ori_rect->i4_left, pt_exc_rect->i4_bottom, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
							 return 3;
						}
						if(pt_ori_rect->i4_bottom > pt_exc_rect->i4_bottom && pt_ori_rect->i4_right <= pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_exc_rect->i4_left, pt_exc_rect->i4_bottom);
							 SET_RECT(pt_out_rect2, pt_ori_rect->i4_left, pt_exc_rect->i4_bottom, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
							 return 2;
						}
						if(pt_ori_rect->i4_bottom < pt_exc_rect->i4_bottom && pt_ori_rect->i4_right > pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_exc_rect->i4_left, pt_ori_rect->i4_bottom);
							 SET_RECT(pt_out_rect2, pt_exc_rect->i4_right, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
							 return 2;
						}
						if(pt_ori_rect->i4_bottom <= pt_exc_rect->i4_bottom && pt_ori_rect->i4_right <= pt_exc_rect->i4_right)
						{													 
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_exc_rect->i4_left, pt_ori_rect->i4_bottom);
							 return 1;
						}
				 }	
			}
			else
			{
				 if(pt_ori_rect->i4_left >= pt_exc_rect->i4_left)
				 {
						if(pt_ori_rect->i4_bottom >= pt_exc_rect->i4_bottom && pt_ori_rect->i4_right > pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_exc_rect->i4_top);
							 SET_RECT(pt_out_rect2, pt_exc_rect->i4_right,	pt_exc_rect->i4_top, pt_ori_rect->i4_right, pt_exc_rect->i4_bottom);
							 SET_RECT(pt_out_rect3, pt_ori_rect->i4_left,  pt_exc_rect->i4_bottom, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
							 return 3;
						}
						if(pt_ori_rect->i4_bottom > pt_exc_rect->i4_bottom && pt_ori_rect->i4_right <= pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_exc_rect->i4_top);
							 SET_RECT(pt_out_rect2, pt_ori_rect->i4_left,  pt_exc_rect->i4_bottom, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
							 return 2;
						}
						if(pt_ori_rect->i4_bottom < pt_exc_rect->i4_bottom && pt_ori_rect->i4_right > pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_exc_rect->i4_top);
							 SET_RECT(pt_out_rect2, pt_exc_rect->i4_right,	pt_exc_rect->i4_top, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
							 return 2;
						}
						if(pt_ori_rect->i4_bottom <= pt_exc_rect->i4_bottom && pt_ori_rect->i4_right <= pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_exc_rect->i4_top);
							 return 1;
						}
				 }
				 else
				 {
						if(pt_ori_rect->i4_bottom >= pt_exc_rect->i4_bottom && pt_ori_rect->i4_right > pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_exc_rect->i4_top);
							 SET_RECT(pt_out_rect2, pt_ori_rect->i4_left, pt_exc_rect->i4_top, pt_exc_rect->i4_left, pt_exc_rect->i4_bottom);
							 SET_RECT(pt_out_rect3, pt_exc_rect->i4_right,	pt_exc_rect->i4_top, pt_ori_rect->i4_right, pt_exc_rect->i4_bottom);
							 SET_RECT(pt_out_rect4, pt_ori_rect->i4_left,  pt_exc_rect->i4_bottom, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
							 return 4;
						}
						if(pt_ori_rect->i4_bottom > pt_exc_rect->i4_bottom && pt_ori_rect->i4_right <= pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_exc_rect->i4_top);
							 SET_RECT(pt_out_rect2, pt_ori_rect->i4_left, pt_exc_rect->i4_top, pt_exc_rect->i4_left, pt_exc_rect->i4_bottom);
							 SET_RECT(pt_out_rect3, pt_ori_rect->i4_left,  pt_exc_rect->i4_bottom, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
							 return 3;
						}
						if(pt_ori_rect->i4_bottom < pt_exc_rect->i4_bottom && pt_ori_rect->i4_right > pt_exc_rect->i4_right)
						{						
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_exc_rect->i4_top);
							 SET_RECT(pt_out_rect2, pt_ori_rect->i4_left, pt_exc_rect->i4_top, pt_exc_rect->i4_left, pt_ori_rect->i4_bottom);
							 SET_RECT(pt_out_rect3, pt_exc_rect->i4_right,	pt_exc_rect->i4_top, pt_ori_rect->i4_right, pt_ori_rect->i4_bottom);
							 return 3;
						}
						if(pt_ori_rect->i4_bottom <= pt_exc_rect->i4_bottom && pt_ori_rect->i4_right <= pt_exc_rect->i4_right)
						{													 
							 SET_RECT(pt_out_rect1, pt_ori_rect->i4_left, pt_ori_rect->i4_top, pt_ori_rect->i4_right, pt_exc_rect->i4_top);
							 SET_RECT(pt_out_rect2, pt_ori_rect->i4_left, pt_exc_rect->i4_top, pt_exc_rect->i4_left, pt_ori_rect->i4_bottom);
							 return 2;
						}
				 }	
			}	
		}
	}
	return 0;
}

/*-----------------------------------------------------------------------------
 * Name: _cal_translation
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *          pi4_x     References.
 *          pi4_y     References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _cal_translation(_WIDGET_T* pt_widget, INT32* pi4_x, INT32* pi4_y)
{
    INT32 i4_x = 0;
    INT32 i4_y = 0;
    
    while (pt_widget)
    {
        i4_x     += pt_widget->t_rect.i4_left;
        i4_y     += pt_widget->t_rect.i4_top;
        pt_widget = PARENT(pt_widget);
    }

    *pi4_x = i4_x;
    *pi4_y = i4_y;
}

/*-----------------------------------------------------------------------------
 * Name: _cal_update_rect
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *          pt_rect   References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _cal_update_rect(_WIDGET_T* pt_widget, GL_RECT_T* pt_rect)
{
    while (pt_widget)
    {
        MOVE_RECT(pt_rect, pt_widget->t_rect.i4_left, pt_widget->t_rect.i4_top);
        INTERSECT_RECT(pt_rect, pt_rect, &(pt_widget->t_rect));
        pt_widget = PARENT(pt_widget);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _unify_update_rect
 *
 * Description: <function description>
 *
 * Inputs:  pt_new    References.
 *          pt_rc_new References.
 *          pt_old    References.
 *          pt_rc_old References.
 *
 * Outputs: - 
 *
 * Returns: TRUE   Success.
 *          FALSE  Fail.
 *
 ----------------------------------------------------------------------------*/
static BOOL _unify_update_rect(
    const _CANVAS_T*    pt_new, 
    const GL_RECT_T*    pt_rc_new,
    const _CANVAS_T*    pt_old, 
    GL_RECT_T*          pt_rc_old /* in-out */) 
{
    GL_RECT_T   t_union;
    UINT32      ui4_area_union;
    UINT32      ui4_area_new;
    UINT32      ui4_area_old;
    
    if (pt_old == pt_new)
    {
        UNION_RECT(&t_union, pt_rc_new, pt_rc_old);

        ui4_area_new   = (UINT32)(RECT_W(pt_rc_new) * RECT_H(pt_rc_new));
        ui4_area_old   = (UINT32)(RECT_W(pt_rc_old) * RECT_H(pt_rc_old));
        ui4_area_union = (UINT32)(RECT_W(&t_union)  * RECT_H(&t_union));
        
        if (ui4_area_union <= (ui4_area_new + ui4_area_old))
        {
            *pt_rc_old = t_union;
            return TRUE;
        }
    }

    return FALSE;
}

static BOOL _check_need_double_buffer(_WIDGET_T* pt_widget,UINT16 ui2_state)
{

    UINT8 ui1_render_cond = WGL_RENDER_SEMI_TRANS;
#ifdef WGL_ANIM_SUPPORT
     WGL_ANIM_TICK_DATA t_anim_tick_data = {0};
    
     wgl_anim_cmd_proc(ANIM(pt_widget),WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_anim_tick_data));
     
     if((_check_animation_need_scale(pt_widget,&t_anim_tick_data))||
        (t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_BREATHE)||
        (t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_ALPHA))
     {
         return TRUE;
     }
#endif
    if( pt_widget->ui1_alpha >0 && pt_widget->ui1_alpha <255 )
    {
        return TRUE;
    }
    _wgl_unlock();
    
    x_wgl_do_cmd(pt_widget->h_this,WGL_CMD_GL_GET_RENDER_COND,WGL_PACK(ui2_state),WGL_PACK(&ui1_render_cond));

    _wgl_lock();
    
    if(ui1_render_cond == WGL_RENDER_TRANS || ui1_render_cond == WGL_RENDER_TRANS_OPAQUE|| ui1_render_cond == WGL_RENDER_OPAQUE)
    {
        return FALSE;
    }
    return TRUE;
}

#ifdef WGL_ANIM_SUPPORT    
static VOID _cal_anim_update_rect(_WIDGET_T* pt_widget, GL_RECT_T* pt_rect)
{
    /* similar to _cal_update_rect() but without the interaction operation */
    
    pt_widget = PARENT(pt_widget);
    while (pt_widget)
    {
        MOVE_RECT(pt_rect, pt_widget->t_rect.i4_left, pt_widget->t_rect.i4_top);
        pt_widget = PARENT(pt_widget);
    }
}

static BOOL _check_widget_is_opaque(_WIDGET_T* pt_widget,UINT16 ui2_state)
{

    UINT8 ui1_render_cond = WGL_RENDER_SEMI_TRANS;
    if( pt_widget->ui1_alpha != 255 )
    {
        return FALSE;
    }
    _wgl_unlock();
    
    x_wgl_do_cmd(pt_widget->h_this,WGL_CMD_GL_GET_RENDER_COND,WGL_PACK(ui2_state),WGL_PACK(&ui1_render_cond));

    _wgl_lock();
    return (ui1_render_cond == WGL_RENDER_OPAQUE);
}

static VOID _add_paint_rect_for_scale(_WIDGET_T   *pt_widget,WGL_ANIM_ATTR_TYPE e_anim_attr_type)
{
    WGL_ANIM_TICK_DATA t_anim_tick_data = {0};
    GL_RECT_T  t_paint_rect = {0};    
    BOOL            b_widget_found = FALSE;
    _ANIM_WDGT_LIST_T *pt_wdgt_element =NULL;
    
     /*for ended animaiton check the last tick if has scale size*/
    t_anim_tick_data.t_new_tick.t_tick_inf.t_scale.ui4_width = WGL_ANIM_INIT_WIDTH;
    t_anim_tick_data.t_new_tick.t_tick_inf.t_scale.ui4_height= WGL_ANIM_INIT_HEIGHT;
    wgl_anim_cmd_proc(ANIM(pt_widget),WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_anim_tick_data));
    if(t_anim_tick_data.t_new_tick.t_tick_inf.t_scale.ui4_width == WGL_ANIM_INIT_WIDTH &&
       t_anim_tick_data.t_new_tick.t_tick_inf.t_scale.ui4_height== WGL_ANIM_INIT_WIDTH )
    {
        return;
    }
    SLIST_FOR_EACH(pt_wdgt_element, &_t_anim_mngr.t_wdgt_list, t_link)
    {
         if(pt_wdgt_element->h_widget == pt_widget->h_this &&
            pt_wdgt_element->e_anim_attr_type == e_anim_attr_type)
            {
                b_widget_found = TRUE;
                break;
            }
        
    }
    if(!b_widget_found)
    {
        /* add the widget to list */
        pt_wdgt_element = (_ANIM_WDGT_LIST_T *)WGL_MEM_ALLOC(sizeof(_ANIM_WDGT_LIST_T));

        if (pt_wdgt_element)
        {
            pt_wdgt_element->h_widget = pt_widget->h_this;
            pt_wdgt_element->e_anim_attr_type = e_anim_attr_type;
            SLIST_INSERT_HEAD(pt_wdgt_element, &_t_anim_mngr.t_wdgt_list, t_link);
        }
        else
        {
            return;
        }
    }
    wgl_anim_cmd_proc(ANIM(pt_widget),WGL_CMD_ANIM_ATTR_GET_CUR_RRGN,WGL_PACK(&t_paint_rect),NULL);        
    _cal_anim_update_rect(pt_widget, &t_paint_rect);
   WGL_RECT_COPY(&pt_wdgt_element->t_rect_scale_rect,&t_paint_rect);
    
   return;
     
}

static VOID _remove_paint_rect_for_scale(_WIDGET_T   *pt_widget,WGL_ANIM_ATTR_TYPE e_anim_attr_type)
{
     BOOL            b_widget_found = FALSE;
     _ANIM_WDGT_LIST_T *pt_wdgt_element =NULL;
     
     SLIST_FOR_EACH(pt_wdgt_element, &_t_anim_mngr.t_wdgt_list, t_link)
     {
          if(pt_wdgt_element->h_widget == pt_widget->h_this &&
             pt_wdgt_element->e_anim_attr_type == e_anim_attr_type)
             {
                 b_widget_found = TRUE;
                 break;
             }
         
     }
     if(b_widget_found)
     {
         /* remove this widget from the wdgt_list */
        SLIST_REMOVE(pt_wdgt_element, t_link);
        x_memset(pt_wdgt_element, 0, sizeof(_ANIM_WDGT_LIST_T));
        WGL_MEM_FREE(pt_wdgt_element);
     }

}

static VOID _get_paint_rect_for_scale(_WIDGET_T   *pt_widget,WGL_ANIM_ATTR_TYPE e_anim_attr_type,GL_RECT_T *pt_out_rect)
{
     BOOL            b_widget_found = FALSE;
     _ANIM_WDGT_LIST_T *pt_wdgt_element =NULL;
     
     SLIST_FOR_EACH(pt_wdgt_element, &_t_anim_mngr.t_wdgt_list, t_link)
     {
          if(pt_wdgt_element->h_widget == pt_widget->h_this &&
             pt_wdgt_element->e_anim_attr_type == e_anim_attr_type)
             {
                 b_widget_found = TRUE;
                 break;
             }
         
     }
     if(b_widget_found)
     {
        WGL_RECT_COPY(pt_out_rect,&pt_wdgt_element->t_rect_scale_rect);
        
     }

}


static VOID _modify_paint_rect_for_scale(GL_RECT_T* pt_upt_rc)
{
    _ANIM_WDGT_LIST_T   *pt_wdgt_element;
    GL_RECT_T* pt_scale_rect =NULL;
    
    SLIST_FOR_EACH(pt_wdgt_element, &_t_anim_mngr.t_wdgt_list, t_link)
        {
          pt_scale_rect = &pt_wdgt_element->t_rect_scale_rect;
          if(!RECT_EMPTY(pt_scale_rect)&&
             (RECT_OVERLAPPED(pt_scale_rect,pt_upt_rc)||
              RECT_INCLUDED(pt_scale_rect,pt_upt_rc)||
              RECT_INCLUDED(pt_upt_rc,pt_scale_rect)))
    {
                UNION_RECT(pt_upt_rc,pt_upt_rc,pt_scale_rect); 
            }
        }
    }

static BOOL _check_animation_with_affect_child(_WIDGET_T* pt_widget)
{
    WGL_ANIM_INF_T t_anim_info;
    wgl_anim_cmd_proc(ANIM(pt_widget),WGL_CMD_ANIM_ATTR_GET_INFO, WGL_PACK(&t_anim_info),NULL);
    if(pt_widget->b_anim_affect_child &&
        t_anim_info.e_state != WGL_ANIM_STATE_END &&
        (t_anim_info.ui2_done_type &WGL_ANIM_TYPE_SCALE||
         t_anim_info.ui2_done_type  &WGL_ANIM_TYPE_ALPHA||
         t_anim_info.ui2_done_type  &WGL_ANIM_TYPE_BREATHE)
       )
       {
           return TRUE;
       }
    return FALSE;
}

static BOOL _check_animation_with_parent_affect_child(_WIDGET_T* pt_widget)
{
    WGL_ANIM_INF_T t_anim_info;
    while (pt_widget)
    {
        wgl_anim_cmd_proc(ANIM(pt_widget),WGL_CMD_ANIM_ATTR_GET_INFO, WGL_PACK(&t_anim_info),NULL);
        if(pt_widget->b_anim_affect_child &&
             t_anim_info.e_state != WGL_ANIM_STATE_END &&
            (t_anim_info.ui2_done_type &WGL_ANIM_TYPE_SCALE||
             t_anim_info.ui2_done_type  &WGL_ANIM_TYPE_ALPHA||
             t_anim_info.ui2_done_type  &WGL_ANIM_TYPE_BREATHE)
           )
           {
               return TRUE;
           }
        pt_widget = PARENT(pt_widget);
    }
    return FALSE;
}

static VOID _paint_widget_with_anim_alpha(
    GL_HSURFACE_T h_surf_src,
    GL_HSURFACE_T h_surf_dst,
    UINT8         ui1_alpha,
    _CANVAS_T*              pt_canvas,
    const GL_RECT_T*        pt_rc_gbl,
    BOOL                    b_need_compose)
{
    /* last, copy the content to the target buffer (might be front or back) */
    if (NULL_HANDLE != h_surf_src && NULL_HANDLE != h_surf_dst && b_need_compose)
    {
     /* remove the clipper first */
       x_gl_surface_set_clipper(
            h_surf_src, 
            NULL_HANDLE);
       if (pt_canvas->e_color_mode == GL_COLORMODE_ARGB_D8888 ||
           pt_canvas->e_color_mode == GL_COLORMODE_ARGB_D4444 ||
           pt_canvas->e_color_mode == GL_COLORMODE_ARGB_D1555)
       {
           x_gl_surface_compose(
               h_surf_dst,
               h_surf_src,
               pt_rc_gbl->i4_left,
               pt_rc_gbl->i4_top,
               pt_rc_gbl->i4_left,
               pt_rc_gbl->i4_top,
               (UINT32)RECT_W(pt_rc_gbl),
               (UINT32)RECT_H(pt_rc_gbl),
               ui1_alpha,
               GL_SRC_OVER,
               0);
       }
       else if (pt_canvas->e_color_mode == GL_COLORMODE_RGB_D565 ||
                GL_IS_PALETTE_MODE(pt_canvas->e_color_mode))
       {
           x_gl_surface_transp_blt(
               h_surf_dst,
               h_surf_src,
               pt_rc_gbl->i4_left,
               pt_rc_gbl->i4_top,
               pt_rc_gbl->i4_left,
               pt_rc_gbl->i4_top,
               (UINT32)RECT_W(pt_rc_gbl),
               (UINT32)RECT_H(pt_rc_gbl),
               ui1_alpha,
               TRUE,
               &(pt_canvas->t_color_key),
               &(pt_canvas->t_color_key),
               0);
       }
       else
       {
           x_gl_surface_blt(
               h_surf_dst,
               h_surf_src,
               pt_rc_gbl->i4_left,
               pt_rc_gbl->i4_top,
               pt_rc_gbl->i4_left,
               pt_rc_gbl->i4_top,
               (UINT32)RECT_W(pt_rc_gbl),
               (UINT32)RECT_H(pt_rc_gbl),
               ui1_alpha,
               0);
       }
    }

}

static VOID _paint_widget_with_anim_scale(
    GL_HSURFACE_T h_surf_src,
    GL_HSURFACE_T h_surf_dst,
    _CANVAS_T*              pt_canvas,
    const GL_RECT_T*        pt_rc_gbl,
    const GL_RECT_T*        pt_scale_rect)
{
    if (NULL_HANDLE != h_surf_src && NULL_HANDLE != h_surf_dst )
    {

            /* remove the clipper first */
           x_gl_surface_set_clipper(
                h_surf_src, 
                NULL_HANDLE);
            x_gl_surface_set_clipper(
                h_surf_dst, 
                NULL_HANDLE);
           x_gl_surface_stretch_blt(
               h_surf_dst,
               h_surf_src,
               pt_scale_rect->i4_left,
               pt_scale_rect->i4_top,
              (UINT32)RECT_W(pt_scale_rect),
              (UINT32)RECT_H(pt_scale_rect),
               pt_rc_gbl->i4_left,
               pt_rc_gbl->i4_top,
               (UINT32)RECT_W(pt_rc_gbl),
               (UINT32)RECT_H(pt_rc_gbl),
               GL_SYNC);
    }

}

static VOID _paint_widget_with_anim(
    _PAINT_INFO_T*          pt_paint_info,
    _CANVAS_T*              pt_canvas,
    const GL_RECT_T*        pt_rc_gbl,
    _PAINT_WIDGET_INFO_T*   pt_pwi,
    BOOL                    b_need_compose)
{

    _WIDGET_T*  pt_widget  = pt_pwi->pt_widget;    
    WGL_ANIM_TICK_DATA t_anim_tick_data;
    GL_HSURFACE_T h_surf_src,h_surf_dst;
    
    h_surf_src = pt_canvas->h_surf_render;
    h_surf_dst = pt_paint_info->h_surf_target;

    wgl_anim_cmd_proc(ANIM(pt_widget),WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_anim_tick_data));
    
    if(!_check_animation_need_scale(pt_widget,&t_anim_tick_data)&&
       !(t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_BREATHE))
    {
        _paint_widget_with_anim_alpha(h_surf_src,h_surf_dst,ALPHA(pt_widget),pt_canvas,pt_rc_gbl,b_need_compose);
    }
    else if(_check_animation_need_scale(pt_widget,&t_anim_tick_data))
    {
    
        GL_RECT_T t_scale_rect; 
        if(t_anim_tick_data.t_new_tick.t_tick_inf.e_ref_type == ANIM_STD_REF_LEFT_TOP)
        {
     SET_RECT_BY_SIZE(&t_scale_rect,
                     pt_rc_gbl->i4_left,
                     pt_rc_gbl->i4_top,
                     (INT32)t_anim_tick_data.t_new_tick.t_tick_inf.t_scale.ui4_width,
                     (INT32)t_anim_tick_data.t_new_tick.t_tick_inf.t_scale.ui4_height);  
         }else
         {
       INT32 i4_width_dis = 0;
       INT32 i4_height_dis = 0;
       i4_width_dis = RECT_W(pt_rc_gbl) - (INT32)t_anim_tick_data.t_new_tick.t_tick_inf.t_scale.ui4_width;
       i4_height_dis = RECT_H(pt_rc_gbl) - (INT32)t_anim_tick_data.t_new_tick.t_tick_inf.t_scale.ui4_height;
        
     SET_RECT_BY_SIZE(&t_scale_rect,
                     pt_rc_gbl->i4_left+i4_width_dis/2,
                     pt_rc_gbl->i4_top+i4_height_dis/2,
                     (INT32)t_anim_tick_data.t_new_tick.t_tick_inf.t_scale.ui4_width,
                     (INT32)t_anim_tick_data.t_new_tick.t_tick_inf.t_scale.ui4_height);  
         }

        if(t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_ALPHA||
            t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_BREATHE || 
            !_check_widget_is_opaque(pt_widget,(pt_pwi->ui2_state | (STATE(pt_widget) & WS_FOCUSED))))
        {
           /*current animaiton type is Scale and Alpha or Breathe*/
            h_surf_src = pt_canvas->h_surf_render;
            h_surf_dst = pt_canvas->h_surf_anim;
            
            _paint_widget_with_anim_scale(h_surf_src,h_surf_dst,pt_canvas,pt_rc_gbl,&t_scale_rect);

            h_surf_src = pt_canvas->h_surf_anim;
            h_surf_dst = pt_paint_info->h_surf_target;
 
#if 0
            if(t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_BREATHE)
            {
                _paint_widget_with_anim_breathe();
            }
#endif
            if(t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_ALPHA || !_check_widget_is_opaque(pt_widget,(pt_pwi->ui2_state | (STATE(pt_widget) & WS_FOCUSED))))
            {
                 x_gl_surface_set_clipper(
                    h_surf_dst, 
                    NULL_HANDLE);
                 _paint_widget_with_anim_alpha(h_surf_src,h_surf_dst,ALPHA(pt_widget),pt_canvas,&t_scale_rect,TRUE);
            }
        }
        else
        {
            _paint_widget_with_anim_scale(h_surf_src,h_surf_dst,pt_canvas,pt_rc_gbl,&t_scale_rect);
        }
    }
    else
    {
       #if 0
        if(t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_BREATHE)
        {
            _paint_widget_with_anim_breathe();
        }
        #endif
        if(t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_ALPHA)
        {
            _paint_widget_with_anim_alpha(h_surf_src,h_surf_dst,ALPHA(pt_widget),pt_canvas,pt_rc_gbl,b_need_compose);
        }
    }   

}

static BOOL _check_animation_need_scale(_WIDGET_T* pt_widget,WGL_ANIM_TICK_DATA* pt_anim_tick_data)
{
    WGL_ANIM_COND_INF_T          e_anim_cond = {0};

    if(pt_anim_tick_data->ui2_begin_flag&WGL_ANIM_TYPE_SCALE)
    {
        return TRUE;
    }
    if(wgl_anim_cmd_proc(ANIM(pt_widget), WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&e_anim_cond), NULL) != WGLR_OK)
     {
        return FALSE;
     }
  
    return (pt_anim_tick_data->t_new_tick.t_tick_inf.t_scale.ui4_width != WGL_ANIM_INIT_WIDTH || pt_anim_tick_data->t_new_tick.t_tick_inf.t_scale.ui4_height!= WGL_ANIM_INIT_WIDTH );
}
#endif  

static VOID _paint_widget(
    _PAINT_INFO_T*          pt_paint_info,
    _CANVAS_T*              pt_canvas,
    const GL_RECT_T*        pt_rc_gbl,
    _PAINT_WIDGET_INFO_T*   pt_pwi)
{
    HANDLE_T    h_graphics = pt_paint_info->h_graphics;
    _WIDGET_T*  pt_widget  = pt_pwi->pt_widget;
    BOOL        b_need_compose = TRUE;
    
    /* clear the widget area */
    if (NULL_HANDLE != pt_canvas->h_surf_render         
#ifdef WGL_ANIM_SUPPORT    
        && !_check_animation_with_parent_affect_child(pt_widget)
#endif
        )
    {
       if(_check_need_double_buffer(pt_widget,(pt_pwi->ui2_state | (STATE(pt_widget) & WS_FOCUSED))))
        {
        /* remove the clipper first */
        x_gl_surface_set_clipper(
            pt_canvas->h_surf_render, 
            NULL_HANDLE);
        
        x_gl_surface_draw_rect(
            pt_canvas->h_surf_render, 
            pt_rc_gbl, 
            &(pt_canvas->t_color_key), 
            0, 
            GL_STYLE_FILLED, 
            GL_SYNC);
            
            x_gl_translate(pt_paint_info->h_graphics_target, pt_pwi->i4_x, pt_pwi->i4_y);
            x_gl_set_clip(pt_paint_info->h_graphics_target, pt_rc_gbl);

        }else
        {
            /*Although system support double buffer for every widget(pt_canvas->h_surf_render is not NULL),but some widget dose not need 
                       double buffer and can paint directly on pt_canvas->h_surf_target so in this case create graphics on pt_canvas->h_surf_target for widget's paint() function*/
            h_graphics = pt_paint_info->h_graphics_target;
            b_need_compose = FALSE;
        }
    }
    
    /* first, setting the graphics object */
    x_gl_translate(h_graphics, pt_pwi->i4_x, pt_pwi->i4_y);
    x_gl_set_clip(h_graphics, pt_rc_gbl);
    x_gl_set_color(h_graphics, &(pt_canvas->t_dflt_color));

    /* set up dirty region */
    SET_RECT(
        &DIRTY(pt_widget), 
        pt_rc_gbl->i4_left   - pt_pwi->i4_x, 
        pt_rc_gbl->i4_top    - pt_pwi->i4_y,
        pt_rc_gbl->i4_right  - pt_pwi->i4_x, 
        pt_rc_gbl->i4_bottom - pt_pwi->i4_y);
    
    _wgl_unlock();

#ifdef TIME_MEASUREMENT
    TMS_BEGIN(PAINT_SINGLE_WIDGET);
#endif

    /* second, invoking the message procedure with WGL_MSG_PAINT */
    _send_msg(
        pt_widget, 
        WGL_MSG_PAINT, 
        (VOID*) h_graphics, 
        (VOID*) (pt_pwi->ui2_state | (STATE(pt_widget) & WS_FOCUSED)));
    
#ifdef TIME_MEASUREMENT
    TMS_END(PAINT_SINGLE_WIDGET);
#endif

    _wgl_lock();

    /* clear dirty region */
    SET_RECT(&DIRTY(pt_widget), 0, 0, 0, 0);
    
#ifdef TIME_MEASUREMENT
    TMS_BEGIN(PAINT_COMPOSITION);
#endif    

#ifdef WGL_ANIM_SUPPORT    
    if(!_check_animation_with_parent_affect_child(pt_widget))
    {
        _paint_widget_with_anim(pt_paint_info, pt_canvas,pt_rc_gbl,pt_pwi,b_need_compose);
    }
#else
/* last, copy the content to the target buffer (might be front or back) */
  if (NULL_HANDLE != pt_canvas->h_surf_render && b_need_compose )
  {
      if (pt_canvas->e_color_mode == GL_COLORMODE_ARGB_D8888 ||
          pt_canvas->e_color_mode == GL_COLORMODE_ARGB_D4444 ||
          pt_canvas->e_color_mode == GL_COLORMODE_ARGB_D1555)
      {
          x_gl_surface_compose(
              pt_paint_info->h_surf_target,
              pt_canvas->h_surf_render,
              pt_rc_gbl->i4_left,
              pt_rc_gbl->i4_top,
              pt_rc_gbl->i4_left,
              pt_rc_gbl->i4_top,
              (UINT32)RECT_W(pt_rc_gbl),
              (UINT32)RECT_H(pt_rc_gbl),
              ALPHA(pt_widget),
              GL_SRC_OVER,
              0);
      }
      else if (pt_canvas->e_color_mode == GL_COLORMODE_RGB_D565 ||
               GL_IS_PALETTE_MODE(pt_canvas->e_color_mode))
      {
          x_gl_surface_transp_blt(
              pt_paint_info->h_surf_target,
              pt_canvas->h_surf_render,
              pt_rc_gbl->i4_left,
              pt_rc_gbl->i4_top,
              pt_rc_gbl->i4_left,
              pt_rc_gbl->i4_top,
              (UINT32)RECT_W(pt_rc_gbl),
              (UINT32)RECT_H(pt_rc_gbl),
              ALPHA(pt_widget),
              TRUE,
              &(pt_canvas->t_color_key),
              &(pt_canvas->t_color_key),
              0);
      }
      else
      {
          x_gl_surface_blt(
              pt_paint_info->h_surf_target,
              pt_canvas->h_surf_render,
              pt_rc_gbl->i4_left,
              pt_rc_gbl->i4_top,
              pt_rc_gbl->i4_left,
              pt_rc_gbl->i4_top,
              (UINT32)RECT_W(pt_rc_gbl),
              (UINT32)RECT_H(pt_rc_gbl),
              ALPHA(pt_widget),
              0);
      }
  }
#endif

#ifdef TIME_MEASUREMENT
    TMS_END(PAINT_COMPOSITION);
#endif
}

/*-----------------------------------------------------------------------------
 * Name: _paint_single_tree
 *
 * Description: <function description>
 *
 * Inputs:  pt_paint_info References.
 *          pt_canvas     References.
 *          pt_widget     References.
 *          pt_rc_gbl     References.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _paint_single_tree(
    _PAINT_INFO_T*      pt_paint_info,
    _CANVAS_T*          pt_canvas,
    _WIDGET_T*          pt_widget,
    const GL_RECT_T*    pt_rc_gbl)
{
    _PAINT_WIDGET_INFO_T    t_pwi;
    GL_RECT_T               t_rect;
    _WIDGET_T*              pt_child;
#if 0
#ifdef WGL_ANIM_SUPPORT  
    WGL_ANIM_TICK_DATA t_anim_tick_data;
#endif
#endif
    t_pwi.i4_x        = pt_paint_info->i4_trans_x + pt_widget->t_rect.i4_left;
    t_pwi.i4_y        = pt_paint_info->i4_trans_y + pt_widget->t_rect.i4_top;
    t_pwi.ui2_state   = pt_paint_info->ui2_state & STATE(pt_widget);
    t_pwi.pt_widget   = pt_widget;
#ifdef WGL_ANIM_SUPPORT    
    if(_check_animation_with_affect_child(pt_widget))
    {
        if (pt_canvas->h_surf_render != NULL_HANDLE)
        { 
           x_gl_surface_set_clipper(
                pt_canvas->h_surf_render, 
                NULL_HANDLE);
            x_gl_surface_draw_rect(
                pt_canvas->h_surf_render, pt_rc_gbl, &(pt_canvas->t_color_key), 0, GL_STYLE_FILLED, 0);
        }
    }
#endif
    if (pt_widget->ui1_alpha != 0)
    {
        if (ST_FLOATED(pt_widget))
        {
            if (pt_paint_info->ui1_num_floated < _ui1_max_floated_widget)
            {
                pt_paint_info->at_pwi[pt_paint_info->ui1_num_floated] = t_pwi;
                pt_paint_info->at_rc_global[pt_paint_info->ui1_num_floated] = *pt_rc_gbl;
            }

            pt_paint_info->ui1_num_floated++;
        }
        else
        {
            _paint_widget(
                pt_paint_info,
                pt_canvas,
                pt_rc_gbl,
                &t_pwi);
        }
    }
    else
    {
        /* clear dirty region */
        SET_RECT(&DIRTY(pt_widget), 0, 0, 0, 0);
    }
    
    /* paint all children of pt_widget */
    for (pt_child = CHILD(pt_widget); pt_child; pt_child = SIBLING(pt_child))
    {
        t_rect = pt_child->t_rect;
        MOVE_RECT(&t_rect, t_pwi.i4_x, t_pwi.i4_y);
#if 0
 #ifdef WGL_ANIM_SUPPORT  
        wgl_anim_cmd_proc(ANIM(pt_child),WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_anim_tick_data));
        if((t_anim_tick_data.ui2_begin_flag!=WGL_ANIM_TYPE_NONE)&&
           !RECT_INCLUDED(&t_rect,pt_rc_gbl))
        {
            continue;      
        }
#endif
#endif
        INTERSECT_RECT(&t_rect, &t_rect, pt_rc_gbl);

        if (ST_FLOATED(pt_widget) && !ST_VISIBLE(pt_widget) && ST_MODIFIED(pt_widget) && !ST_DESTROYED(pt_widget))
        {
            STATE (pt_widget) &= ~WS_MODIFIED;
        }
        
        if (ST_VISIBLE(pt_child) && !ST_DESTROYED(pt_child) && !RECT_EMPTY(&t_rect))
        {
            pt_paint_info->i4_trans_x = t_pwi.i4_x;
            pt_paint_info->i4_trans_y = t_pwi.i4_y;
            pt_paint_info->ui2_state  = t_pwi.ui2_state;
            
            _paint_single_tree(pt_paint_info, pt_canvas, pt_child, &t_rect);
        }
    }
#ifdef WGL_ANIM_SUPPORT    
    if(_check_animation_with_affect_child(pt_widget))
    {
        HANDLE_T    h_graphics = pt_paint_info->h_graphics_target;
        x_gl_translate(h_graphics, pt_paint_info->i4_trans_x + pt_widget->t_rect.i4_left, pt_paint_info->i4_trans_y + pt_widget->t_rect.i4_top);
        x_gl_set_clip(h_graphics, pt_rc_gbl);
        _paint_widget_with_anim(pt_paint_info, pt_canvas,pt_rc_gbl,&t_pwi,TRUE);
    }
#endif
    return WGLR_OK;
}

    
#ifdef WGL_ANIM_SUPPORT    

static VOID _add_canvas_record(_CANVAS_T *pt_canvas)
{
    UINT32  ui4_i;

    /* return. do nothing */
    if (_t_anim_mngr.ui4_canvas_num >= WGL_MAX_CANVAS_RECORD_NUM)
    {
        DBG_ERROR(("{WGL} canvas record is full.Line: %d\n", __LINE__));
        return;
    }
    
    /* if there is no data, just add it */
    if (_t_anim_mngr.ui4_canvas_num == 0)
    {
        _t_anim_mngr.apt_canvas[_t_anim_mngr.ui4_canvas_num++] = pt_canvas;

        return;
    }

    /* if there is the same data existing already, don't add it */
    for (ui4_i = 0; ui4_i < _t_anim_mngr.ui4_canvas_num; ui4_i++)
    {
        if (_t_anim_mngr.apt_canvas[ui4_i] == pt_canvas)
        {
            return;
        }
    }

    /* or add it as a new record */
    _t_anim_mngr.apt_canvas[_t_anim_mngr.ui4_canvas_num++] = pt_canvas;
}

static VOID _animation_thread_main(VOID *pv_arg)
{
    INT32           i4_ret;
    UINT16          ui2_index;
    _ANIM_MSG_T     t_msg;
    SIZE_T          z_size;

    while (1)
    {
        z_size = sizeof(_ANIM_MSG_T);
        
        i4_ret = x_msg_q_receive(
            &ui2_index,
            &t_msg,
            &z_size,
            &_t_anim_mngr.h_anim_msg_q,
            1,
            X_MSGQ_OPTION_WAIT);

        if (i4_ret == OSR_OK)
        {
            switch (t_msg.e_type)
            {
                case ANIM_MSG_TICK:
                {
                    BOOL    b_continuous_tick, b_one_shot_tick, b_one_shot_tick_drop_frame;

                    b_continuous_tick          = (_t_anim_mngr.e_tick_mode == WGL_TICK_CONTINUOUS);
                    b_one_shot_tick            = (_t_anim_mngr.e_tick_mode == WGL_TICK_ONE_SHOT);
                    b_one_shot_tick_drop_frame = (_t_anim_mngr.e_tick_mode == WGL_TICK_ONE_SHOT_DROP_FRAME);
                    
                    _app_list_lock();

                    if (_t_anim_mngr.ui4_app_count > 0)
                    {
                        _ANIM_APP_LIST_T    *pt_app_element;
                        UINT32              ui4_tick_start, ui4_tick_end, ui4_tick_interval = x_os_get_sys_tick_period();

                        ui4_tick_start = x_os_get_sys_tick();
                        
                        SLIST_FOR_EACH(pt_app_element, &_t_anim_mngr.t_app_list, t_link)
                        {
                            BOOL    b_drop_frame = TRUE;
                            
                            if ((b_continuous_tick ||
                                ((b_one_shot_tick || b_one_shot_tick_drop_frame) &&
                                 (pt_app_element->b_been_dispatched == TRUE)))
                                 &&pt_app_element->b_any_wdgt_anim_running)
                            {
                                /* drawing with app thread */
                                if (pt_app_element->e_anim_context == ANIM_CONTEXT_APP)
                                {
                                    _ANIM_TICK_MSG_DATA_T* pt_msg_data = WGL_MEM_ALLOC(sizeof(_ANIM_TICK_MSG_DATA_T));
                                    if(pt_msg_data != NULL)
                                    {
                                        pt_msg_data->ui1_app_idx = pt_app_element->pt_app_info->ui1_anim_app_idx;
                                        pt_msg_data->ui4_tick = pt_app_element->ui4_tick;
                                    pt_app_element->pt_app_info->pf_callback(
                                        WGL_MSG_ANIMATION_TICK,
                                        (VOID *)pt_app_element,
                                            (VOID *)pt_msg_data);
                                    }
                                }
                            #if 0
                                /* drawing with widget manager */
                                else if (pt_app_element->e_anim_context == ANIM_CONTEXT_WM)
                                {
                                    /* to be implemented */
                                }
                            #endif

                                b_drop_frame = FALSE;
                            
                                /* one-shot mode */
                                if (!b_continuous_tick)
                                {
                                    pt_app_element->b_been_dispatched = FALSE;
                                }
                            }

                            if (!(b_one_shot_tick && b_drop_frame))
                            {
                                pt_app_element->ui4_tick++;
                            }
                        }
                        _app_list_unlock();

                        ui4_tick_end = x_os_get_sys_tick();

                        ui4_tick_end = (ui4_tick_end - ui4_tick_start) * ui4_tick_interval;

                        if (ui4_tick_end < _t_anim_mngr.ui4_tick_interval)
                        {
                            x_thread_delay(_t_anim_mngr.ui4_tick_interval - ui4_tick_end);
                        }

                        _send_anim_tick_msg();
                   }else
                   {

                    _app_list_unlock();
                   }
                    
                    break;
                }

                default:
                    DBG_ERROR(("{WGL} unknown message is received in the animation thread. (%d)\n", t_msg.e_type));
                    break;
            }
        }
    }
}
#endif
/*-----------------------------------------------------------------------------
 * Name: _draw_background
 *
 * Description: <function description>
 *
 * Inputs:  h_graphics References.
 *          pt_canvas  References.
 *          pt_rect    References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _draw_background(
    GL_HSURFACE_T       h_surf,
    _CANVAS_T*          pt_canvas, 
    const GL_RECT_T*    pt_rect)
{
    if (pt_canvas->h_bg_image == NULL_HANDLE)
    {
#if 1
        x_gl_surface_draw_rect(
            h_surf, pt_rect, &(pt_canvas->t_color_key), 0, GL_STYLE_FILLED, 0);
#else
        GL_LOCK_INFO_T  t_lock_info;

        if (GLR_OK == x_gl_surface_lock(h_surf, NULL, 0, &t_lock_info))
        {
            _draw_argb32(
                t_lock_info.at_subplane[0].pv_bits, 
                t_lock_info.at_subplane[0].ui4_width, 
                t_lock_info.at_subplane[0].ui4_height,
                t_lock_info.at_subplane[0].ui4_pitch);

            x_gl_surface_unlock(pt_canvas->h_surf_back, NULL);
        }
#endif
    }
    else
    {
        /* draw background image */
    }
}

static VOID _draw_transparent_list(
    GL_HSURFACE_T       h_surf,
    _CANVAS_T*          pt_canvas,
    GL_RECT_T*          pt_upt_rect)
{
    _TRANSP_LIST_T      *pt_list;
    GL_RECT_T           t_rect;
    GL_LOCK_INFO_T      t_lock_info;
    INT32               i4_ret;

    if (DLIST_IS_EMPTY(&pt_canvas->t_transp_list))
    {
        return;
    }

    i4_ret = x_gl_surface_lock(h_surf, NULL, GL_SYNC, &t_lock_info);

    if (i4_ret == GLR_OK)
    {
        VOID    *pv_surf = NULL;
        
        DLIST_FOR_EACH(pt_list, &pt_canvas->t_transp_list, t_link)
        {
            if (pt_list->b_enalbed)
            {
                HANDLE_TYPE_T       e_type;
                _TRANSP_REGION_T    *pt_region = NULL;

                i4_ret = handle_get_type_obj(pt_list->h_rgn_list, &e_type, (VOID **)&pt_region);

                if (i4_ret == HR_OK && e_type == HT_WGL_TRANP_LIST && pt_region != NULL)
                {
                    UINT32          ui4_i;

                    for (ui4_i = 0; ui4_i < pt_region->ui4_rect_count; ui4_i++)
                    {
                        INTERSECT_RECT(&t_rect, &pt_region->pt_rect[ui4_i], pt_upt_rect);

                        if (!RECT_EMPTY(&t_rect))
                        {
                            if (pt_region->ui4_exclusive_count == 0)
                            {
                                gl_gpu_fill_rect(
                                    t_lock_info.e_clrmode,
                                    t_lock_info.at_subplane[0].pv_bits,
                                    t_lock_info.at_subplane[0].ui4_pitch,
                                    t_rect.i4_left,
                                    t_rect.i4_top,
                                    (UINT32)RECT_W(&t_rect),
                                    (UINT32)RECT_H(&t_rect),
                                    gl_compose_packed_color(&pt_canvas->t_color_key, t_lock_info.e_clrmode));
                            }                           
							else
                            {
                                UINT32      ui4_j;
								UINT32      ui4_rect_count = 0;
								GL_RECT_T*      pt_rect = WGL_MEM_ALLOC(sizeof(GL_RECT_T) * pt_region->ui4_exclusive_count);
								if (pt_rect == NULL)
								{
									x_gl_surface_unlock(h_surf, NULL);
									return ;
								}
								x_memset(pt_rect, 0, sizeof(GL_RECT_T)* pt_region->ui4_exclusive_count);
								
                                for (ui4_j = 0; ui4_j < pt_region->ui4_exclusive_count; ui4_j++)
                            	{
                            	    
									x_dbg_stmt("{%s %d}new added.\n", __FUNCTION__, __LINE__);
                            		INTERSECT_RECT(&pt_rect[ui4_rect_count], &t_rect, &pt_region->pt_exclusive_rect[ui4_j]);
									if (!RECT_EMPTY(&pt_rect[ui4_rect_count]))
									{
										ui4_rect_count ++;
									}
                            	}

								if (ui4_rect_count > 0)
								{
					                _fill_exclusive_region(&t_rect, 
																	0, 
																	ui4_rect_count, 
																	pt_rect,
																	&t_lock_info,
																	&pt_canvas->t_color_key);
								}
								else
								{
                                    
									gl_gpu_fill_rect(
										t_lock_info.e_clrmode,
										t_lock_info.at_subplane[0].pv_bits,
										t_lock_info.at_subplane[0].ui4_pitch,
										t_rect.i4_left,
										t_rect.i4_top,
										(UINT32)RECT_W(&t_rect),
										(UINT32)RECT_H(&t_rect),
										gl_compose_packed_color(&pt_canvas->t_color_key, t_lock_info.e_clrmode));
								}	

								WGL_MEM_FREE(pt_rect);
                            }  
                        }
                    }
                }
            }
        }
        x_gl_surface_unlock(h_surf, NULL);

        if (handle_get_obj(h_surf, (VOID **)&pv_surf) == HR_OK &&
            pv_surf != NULL)
        {
            gl_surface_flush(pv_surf, GL_SYNC);
        }
    }
}

static BOOL _check_single_tree_visibility(
    const _WIDGET_T*    pt_widget,
    BOOL                b_check_ignored)
{
    _WIDGET_T* pt_child;
    
    if ((b_check_ignored && (ST_IGNORED(pt_widget))) ||
        (0 == ST_VISIBLE(pt_widget)))
    {
        return FALSE;
    }

    if (ALPHA(pt_widget) > 0)
    {
        return TRUE;
    }

    /* pt_widget is VISIBLE and ALPHA(pt_widget) == 0, check its children */
    for (pt_child = CHILD(pt_widget); pt_child; pt_child = SIBLING(pt_child))
    {
        if (TRUE == _check_single_tree_visibility(pt_child, b_check_ignored))
        {
            return TRUE;
        }
    }

    return FALSE;
}

static BOOL _any_widget_visible(
    const _CANVAS_T*    pt_canvas, 
    BOOL                b_check_ignored)
{
    _BRANCH_T*  pt_branch;
    _WIDGET_T*  pt_widget;

    DLIST_FOR_EACH(pt_branch, &(pt_canvas->t_branch_lst), t_link_on_canvas)
    {
        /* process each branch */
        for (pt_widget = FIRST(pt_branch); pt_widget; pt_widget = SIBLING(pt_widget))
        {
            if (TRUE == _check_single_tree_visibility(pt_widget, b_check_ignored))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*                 1.
 *  UI canvas ---------+          1. When there is nothing visible on UI canvas, it would go to SHADOW-LOCK state. UI and Shadow
 *    ^   ^            |             canvases would receive WGL_MSG_UPDATE_CANVAS. At the same time, none of the two canvases
 *    |   |            |             are in running states.
 *    |   | 3.         |
 *    |   |            |          2. SHADOW-LOCK state would go to Shadow canvas after _ui4_shdw_cvs_blk_time mili-seconds. UI
 *    |   |            V             and Shadow canvases would receive WGL_MSG_SWITCH_CANVAS. A full-canvas repaint on Shadow
 *  4.|   +----- SHADOW-LOCK         canvas would be triggered. Any update on Shadow canvas in SHADOW-LOCK state would be 
 *    |                |             discarded.
 *    |                |
 *    |                | 2.       3. If there is any update on UI canvas in SHADOW-LOCK, UI canvas would become running again. A
 *    |                |             full-canvas repaint on UI canvas would be triggered.
 *    |                |
 *  Shadow canvas <----+          4. A normal path from Shadow canvas back to UI canvas.
 */
static VOID _shdw_cvs_timeout_cb(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UINT8               ui1_i, ui1_retry_time = 10;
    INT32               i4_ret;
    _DESTROY_MSG_T      t_msg;

    t_msg.ui4_msg   = RPM_MSG_SHADOW_UNLOCK;
    t_msg.pt_canvas = (_CANVAS_T *)pv_tag;
    
    for (ui1_i = 0; ui1_i < ui1_retry_time; ui1_i++)
    {
        i4_ret = x_msg_q_send(
            _t_repaint_mngr.h_msg_q,
            (VOID *)&t_msg,
            sizeof(t_msg),
            RPM_MSG_PRIORITY_SHDW_UNLCK);

        if (i4_ret == OSR_OK)
        {
            break;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _paint_canvas
 *
 * Description: <function description>
 *
 * Inputs:  pt_canvas References.
 *          pt_rect   References.
 *
 * Outputs: - 
 *
 * Returns: WGLR_INTERNAL_ERROR  Success.
 *          WGLR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
#ifdef MW_OSD_COMPRESSION_SHINK

static INT32 _swap_wgl_resource(_CANVAS_T* pt_canvas)
{
	UINT32 ui4_temp;
	if(pt_canvas == NULL)
	{
		return WGLR_OK;
	}
	if (_b_osd_compress)
	{
		ui4_temp = pt_canvas->h_view;
		pt_canvas->h_view = pt_canvas->h_view_back;
		pt_canvas->h_view_back = ui4_temp;

		ui4_temp = pt_canvas->ui4_viewport;
		pt_canvas->ui4_viewport = pt_canvas->ui4_viewport_back;
		pt_canvas->ui4_viewport_back = ui4_temp;
		
		#ifdef SYS_3D_SUPPORT
		ui4_temp = pt_canvas->h_surf_front;
		pt_canvas->h_surf_front = pt_canvas->h_surf_back;
		pt_canvas->h_surf_back  = ui4_temp;
		#else
		ui4_temp = pt_canvas->h_surf_front;
		pt_canvas->h_surf_front = pt_canvas->h_surf_render;
		pt_canvas->h_surf_render  = ui4_temp;
		#endif
	}
	return WGLR_OK;
}

#endif

static INT32 _paint_canvas(_CANVAS_T* pt_canvas, const GL_RECT_T* pt_rect)
{
    _BRANCH_T*      pt_branch;
    _WIDGET_T*      pt_widget;
    GL_HGRAPHICS_T  h_graphics,h_graphics_target;
    _PAINT_INFO_T   *pt_paint_info = &_t_paint_info;
    GL_RECT_T       t_rect, t_upt_rc;
    _CANVAS_T*      pt_ref    = NULL;
    _CANVAS_T*      pt_shadow = NULL;
    BOOL            b_switch  = FALSE;
    BOOL            b_single_cvs_empty = FALSE;
    BOOL            b_full_screen_clean = FALSE;
    BOOL            b_keep_last_ui_canvas = FALSE;
    
#ifdef TIME_MEASUREMENT
    TMS_BEGIN(PAINT_ALL);
#endif

    if (pt_canvas == NULL)
    {
        x_dbg_stmt("\n{WGL} _paint_canvas with NULL canvas pointer. File : %s, Line : %d\n", __FILE__, __LINE__);
        DBG_ABORT(DBG_MOD_WGL);
    }

    _hrc_lock();

    _wgl_lock();

    if (IS_SINGLE(pt_canvas))
    {
        t_upt_rc = *pt_rect;
        _send_canvas_update_notify(pt_canvas);

        if (!_any_widget_visible(pt_canvas, TRUE))
        {
            b_single_cvs_empty = TRUE;
        }
    }
    else
    {
        BOOL b_visible;
        
        if (IS_REFERENCE(pt_canvas))
        {
            pt_ref    = pt_canvas;
            pt_shadow = SHADOW(pt_canvas);
        }
        else /* IS_SHADOW(pt_canvas) */
        {
            pt_ref    = REFERENCE(pt_canvas);
            pt_shadow = pt_canvas;
        }

        b_visible = _any_widget_visible(pt_ref, TRUE);
        b_switch  = (b_visible != pt_ref->b_running);
        
        if (b_switch)
        {
            /* switch to Shadow canvas */
            if (!b_visible && pt_ref->b_running)
            {
                /*Printf("State 1 b_visible (%d) pt_ref->b_running (%d)\n", b_visible, pt_ref->b_running);*/
                /* enter the Shadow-lock state */
                pt_canvas            = pt_ref;
                pt_ref->b_running    = FALSE;
                pt_shadow->b_running = FALSE;

                pt_ref->b_shadow_locked    = TRUE;
                pt_shadow->b_shadow_locked = TRUE;

                x_timer_start(
                    _h_shdw_cvs_blk_timer,
                    _ui4_shdw_cvs_blk_time,
                    X_TIMER_FLAG_ONCE,
                    _shdw_cvs_timeout_cb,
                    (VOID *)pt_ref);

                _send_canvas_update_notify(pt_ref);
                _send_canvas_update_notify(pt_shadow);

                b_switch = FALSE;
                t_upt_rc = *pt_rect;

                b_keep_last_ui_canvas = TRUE;
            }
            else
            {
                /* UI canvas is drawn in Shadow-lock state */
                if (pt_ref->b_shadow_locked)
                {
                    x_timer_stop(_h_shdw_cvs_blk_timer);

                    /* go back to UI canvas */
                    pt_ref->b_shadow_locked    = FALSE;
                    pt_shadow->b_shadow_locked = FALSE;

                    pt_ref->b_running = TRUE;

                    b_switch  = FALSE;
                    pt_canvas = pt_ref;
                    
                    SET_RECT_BY_SIZE(&t_upt_rc, 0, 0, (INT32)pt_canvas->ui4_width, (INT32)pt_canvas->ui4_height);

                    _send_canvas_update_notify(pt_ref);
                    _send_canvas_update_notify(pt_shadow);
                }
                else
                {
                    /* switch to UI canvas */
                    if (!pt_ref->b_unlock_to_shadow)
                    {
                        pt_ref->b_running    = !(pt_ref->b_running);
                        pt_shadow->b_running = !(pt_shadow->b_running);

                        pt_canvas = ((pt_ref->b_running) ? pt_ref : pt_shadow);
                        SET_RECT_BY_SIZE(&t_upt_rc, 0, 0, (INT32)pt_canvas->ui4_width, (INT32)pt_canvas->ui4_height);

                        _send_canvas_switch_notify(pt_ref);
                        _send_canvas_switch_notify(pt_shadow);
                    }
                    /* go back to UI state while in Shadow-unlock state */
                    else
                    {
                        pt_ref->b_unlock_to_shadow    = FALSE;
                        pt_shadow->b_unlock_to_shadow = FALSE;

                        SET_RECT_BY_SIZE(&t_upt_rc, 0, 0, (INT32)pt_canvas->ui4_width, (INT32)pt_canvas->ui4_height);

                        pt_ref->b_running = TRUE;
                        pt_canvas = pt_ref;
                        
                        _send_canvas_update_notify(pt_ref);
                        _send_canvas_update_notify(pt_shadow);                        
                    }
                }
            }
        }
        else
        {
            /* from Shadow-lock state to Shadow canvas */
            if (pt_canvas->b_unlock_to_shadow)
            {
                pt_ref->b_unlock_to_shadow    = FALSE;
                pt_shadow->b_unlock_to_shadow = FALSE;

                t_upt_rc = *pt_rect;

                pt_canvas->b_running = TRUE;
                b_switch = TRUE;
                
                _send_canvas_switch_notify(pt_ref);
                _send_canvas_switch_notify(pt_shadow);
            }
            /* normal update */
            else
            {
                /* UI canvas is drawn in Shadow-lock state */
                if (pt_canvas == pt_ref && pt_ref->b_shadow_locked)
                {
                    /* go back to UI canvas */
                    if (b_visible)
                    {
                        x_timer_stop(_h_shdw_cvs_blk_timer);

                        pt_ref->b_shadow_locked    = FALSE;
                        pt_shadow->b_shadow_locked = FALSE;
                        
                        pt_ref->b_running = TRUE;

                        SET_RECT_BY_SIZE(&t_upt_rc, 0, 0, (INT32)pt_canvas->ui4_width, (INT32)pt_canvas->ui4_height);
                    }
                    /* UI canvas keeps rendering remainder in Shadow-lock state */
                    else
                    {
                        b_keep_last_ui_canvas = TRUE;
                        t_upt_rc = *pt_rect;
                    }
                }
                else
                {
                    t_upt_rc = *pt_rect;
                }

                _send_canvas_update_notify(pt_ref);
                _send_canvas_update_notify(pt_shadow);
            }
        }

        
        if ((UINT32)RECT_W(&t_upt_rc) == pt_canvas->ui4_width  &&
            (UINT32)RECT_H(&t_upt_rc) == pt_canvas->ui4_height &&
            _any_widget_visible(pt_canvas, FALSE) == FALSE)
        {
            b_full_screen_clean = TRUE;
        }
    }
#ifdef WGL_ANIM_SUPPORT    
    _modify_paint_rect_for_scale(&t_upt_rc);
#endif
    /* normal running */
    if (pt_canvas->b_running ||
        /* or keep the last appearance of UI canvas when entering into Shadow-lock state */
        b_keep_last_ui_canvas)
    {
        GL_HSURFACE_T           h_surf_target;
        INT32                   i;
    #ifdef SYS_3D_SUPPORT
        GL_DRV_UPDATE_INFO_T    t_update_info = {
            GL_DRV_UPDATE_NORMAL, pt_canvas->h_surf_back, pt_canvas->h_surf_front, 0, 0, 0, 0, pt_canvas->ui4_width, pt_canvas->ui4_height};
    #endif
#ifdef MW_OSD_COMPRESSION_SHINK
	    if (b_switch && !(pt_shadow->b_new_surface))
    	{
			if (IS_REFERENCE(pt_canvas))
			{
				pt_canvas->h_surf_front = pt_shadow->h_surf_front;
				pt_canvas->h_surf_back = pt_shadow->h_surf_back;
				
				if (NULL_HANDLE != pt_shadow->h_surf_render)
				{
					pt_canvas->h_surf_render = pt_shadow->h_surf_render;
				}

				pt_canvas->h_view            = pt_shadow->h_view;
				pt_canvas->h_view_back       = pt_shadow->h_view_back;
				pt_canvas->ui4_viewport      = pt_shadow->ui4_viewport;
				pt_canvas->ui4_viewport_back = pt_shadow->ui4_viewport_back;
			}
			else /* IS_SHADOW(pt_canvas) */
			{
				pt_canvas->h_surf_front = pt_ref->h_surf_front;
				pt_canvas->h_surf_back = pt_ref->h_surf_back;
				
				if (NULL_HANDLE != pt_ref->h_surf_render)
				{
					pt_canvas->h_surf_render = pt_ref->h_surf_render;
				}

				pt_canvas->h_view            = pt_ref->h_view;
				pt_canvas->h_view_back       = pt_ref->h_view_back;
				pt_canvas->ui4_viewport      = pt_ref->ui4_viewport;
				pt_canvas->ui4_viewport_back = pt_ref->ui4_viewport_back;
			}
    	}

#ifdef SYS_3D_SUPPORT
        t_update_info = {GL_DRV_UPDATE_NORMAL, pt_canvas->h_surf_back, pt_canvas->h_surf_front, 0, 0, 0, 0, pt_canvas->ui4_width, pt_canvas->ui4_height};
#endif
	    h_surf_target = pt_canvas->h_surf_back;

#else

    #ifdef LINUX_TURNKEY_SOLUTION_PHASE2
        h_surf_target = pt_canvas->h_surf_back;
    #else
        h_surf_target = (b_switch) ? pt_canvas->h_surf_front : pt_canvas->h_surf_back;
    #endif
#endif
        if (pt_canvas->h_graphics != NULL_HANDLE)
        {
            h_graphics = pt_canvas->h_graphics;

            i = x_gl_change_surface(
                h_graphics,
                ((NULL_HANDLE == pt_canvas->h_surf_render) ? h_surf_target : pt_canvas->h_surf_render));

            if (i != GLR_OK)
            {
                _wgl_unlock();

                _hrc_unlock();

                return WGLR_INTERNAL_ERROR;
            }

            i = x_gl_set_clip(h_graphics, &t_upt_rc);

            if (i != GLR_OK)
            {
                _wgl_unlock();

                _hrc_unlock();

                return WGLR_INTERNAL_ERROR;
            }
        }
        else
        {
            /* TODO: Should we create a new graphics object each time? */
            if (GLR_OK != x_gl_create_graphics(
                ((NULL_HANDLE == pt_canvas->h_surf_render) ? h_surf_target : pt_canvas->h_surf_render),
                NULL, 
                &t_upt_rc, 
                &h_graphics))
            {
                _wgl_unlock();

                _hrc_unlock();

                return WGLR_INTERNAL_ERROR;
            }

            pt_canvas->h_graphics = h_graphics;
        }
        if (pt_canvas->h_graphics_target != NULL_HANDLE)
        {
            h_graphics_target = pt_canvas->h_graphics_target;

            i = x_gl_change_surface(
                h_graphics_target,
                 h_surf_target);

            if (i != GLR_OK)
            {
                _wgl_unlock();

                _hrc_unlock();

                return WGLR_INTERNAL_ERROR;
            }

            i = x_gl_set_clip(h_graphics_target, &t_upt_rc);

            if (i != GLR_OK)
            {
                _wgl_unlock();

                _hrc_unlock();

                return WGLR_INTERNAL_ERROR;
            }
        }
        else
        {
            /* TODO: Should we create a new graphics object each time? */
            if (GLR_OK != x_gl_create_graphics(
                h_surf_target,
                NULL, 
                &t_upt_rc, 
                &h_graphics_target))
            {
                _wgl_unlock();

                _hrc_unlock();

                return WGLR_INTERNAL_ERROR;
            }

            pt_canvas->h_graphics_target = h_graphics_target;
        }

    #ifdef TIME_MEASUREMENT
        TMS_BEGIN(PAINT_BACKGROUND);
    #endif

        _draw_background(h_surf_target, pt_canvas, &t_upt_rc);

    #ifdef TIME_MEASUREMENT
        TMS_END(PAINT_BACKGROUND);
    #endif

        pt_paint_info->h_graphics      = h_graphics;
        pt_paint_info->h_graphics_target = h_graphics_target;
        pt_paint_info->h_surf_target   = h_surf_target;
        pt_paint_info->ui1_num_floated = 0;
        
        DLIST_FOR_EACH(pt_branch, &(pt_canvas->t_branch_lst), t_link_on_canvas)
        {
            /* process each branch */
            for (pt_widget = FIRST(pt_branch); pt_widget; pt_widget = SIBLING(pt_widget))
            {
                if (ST_FLOATED(pt_widget) && !ST_VISIBLE(pt_widget) && ST_MODIFIED(pt_widget) && !ST_DESTROYED(pt_widget))
                {
                    STATE (pt_widget) &= ~WS_MODIFIED;
                }
                
                if ((ST_VISIBLE(pt_widget)) && 
                    (!ST_DESTROYED(pt_widget)))
                {
                    INTERSECT_RECT(&t_rect, &t_upt_rc, &(pt_widget->t_rect));
                    
                    if (!RECT_EMPTY(&t_rect))
                    {   
                        if (RECT_INCLUDED(&(pt_widget->t_rect), &t_rect))
                        {
                            STATE(pt_widget) &= ~WS_MODIFIED;
                        }
                        
                        /* draw the widget tree of the specified top-level widget */
                        /* draw background on the front buffer */
                        /* draw widget content */
                        pt_paint_info->i4_trans_x    = 0;
                        pt_paint_info->i4_trans_y    = 0;
                        pt_paint_info->ui2_state     = WS_ALL;
                        
                        _paint_single_tree(
                            pt_paint_info,
                            pt_canvas,
                            pt_widget,
                            &t_rect);
                    }
                }
            }
        }

        _draw_transparent_list(h_surf_target, pt_canvas, &t_upt_rc);

        if (pt_paint_info->ui1_num_floated > _ui1_max_floated_widget)
        {
            DBG_ERROR(("{WGL} _paint_canvas: too many floated widgets...\r\n"));
            pt_paint_info->ui1_num_floated = _ui1_max_floated_widget;
        }
    
        /* paint floated widgets */
        for (i = 0; i < pt_paint_info->ui1_num_floated; i++)
        {
            pt_widget = pt_paint_info->at_pwi[i].pt_widget;
            
            if ((ST_VISIBLE(pt_widget)) && 
                (!ST_DESTROYED(pt_widget)))
            {
                _paint_widget(
                    pt_paint_info,
                    pt_canvas,
                    &(pt_paint_info->at_rc_global[i]),
                    &(pt_paint_info->at_pwi[i]));
            }
        }
#ifndef MW_OSD_COMPRESSION_SHINK        
        _wgl_unlock();
#endif        
    #ifdef TIME_MEASUREMENT
        TMS_BEGIN(PAINT_BACK_TO_FRONT);
    #endif

        /*for fix cr: DTV00088208*/
        if(t_upt_rc.i4_left == 0 &&
           t_upt_rc.i4_top == 0 && 
           t_upt_rc.i4_right==(INT32)pt_canvas->ui4_width &&
           t_upt_rc.i4_bottom ==(INT32)pt_canvas->ui4_height)
        {
            x_gl_osd_wait_vdp();
        }

        if (b_switch)
        {
#ifndef MW_OSD_COMPRESSION_SHINK
			if (IS_SHADOW(pt_canvas))
			{
				pt_shadow->b_compress_back_running = pt_ref->b_compress_back_running;
			}
			else
			{
				pt_ref->b_compress_back_running = pt_shadow->b_compress_back_running;
			}
			
            x_gl_surface_set_clipper(
                pt_canvas->h_surf_front,
                NULL_HANDLE);
#else
            x_gl_surface_set_clipper(
                pt_canvas->h_surf_back,
                NULL_HANDLE);
#endif
            if (!_b_osd_compress)
            {
                FLASH_AREA(pt_canvas->h_surf_back, &t_upt_rc, _ui4_flash_time);

            #ifdef SYS_3D_SUPPORT
                t_update_info.e_mode     = GL_DRV_UPDATE_3D_FLIP;
                t_update_info.i4_x       = 0;
                t_update_info.i4_y       = 0;
                t_update_info.ui4_width  = pt_canvas->ui4_width;
                t_update_info.ui4_height = pt_canvas->ui4_height;

                gl_plane_notify_drv_update(&t_update_info);
            #else
                SET_RECT_BY_SIZE(&t_upt_rc, 0, 0, (INT32)pt_canvas->ui4_width, (INT32)pt_canvas->ui4_height);
            #endif
            
                x_gl_plane_flip(
                    pt_canvas->h_plane, 
                    pt_canvas->h_view, 
                    TRUE);
            }
            else
            {
#ifdef MW_OSD_COMPRESSION_SHINK
				
				 x_gl_surface_compressed_blt(pt_canvas->h_surf_render,
											 pt_canvas->h_surf_back,
											 0,
											 0,
											 pt_canvas->ui4_width,
											 pt_canvas->ui4_height,
											 _e_compression_ratio,
											 FALSE,
											 0);
				
				
				x_gl_plane_flip(pt_canvas->h_plane, 
								pt_canvas->h_view_back, 
								TRUE);			

				
				_swap_wgl_resource(pt_canvas);
				
#else
				
                x_gl_surface_compressed_blt(
                    (!pt_canvas->b_compress_back_running)? pt_canvas->h_surf_compress_back: pt_canvas->h_surf_compress,
                    pt_canvas->h_surf_front,
                    0,
                    0,
                    pt_canvas->ui4_width,
                    pt_canvas->ui4_height,
                    _e_compression_ratio,
                #ifdef OSD_LINE_SEPARATE_COMPRESSION
                    TRUE,
                #else
                    FALSE,
                #endif
                    0);

                FLASH_AREA(pt_canvas->h_surf_back, &t_upt_rc, _ui4_flash_time);
                
                x_gl_plane_flip(
                    pt_canvas->h_plane, 
                    (!pt_canvas->b_compress_back_running)? pt_canvas->h_view_compress: pt_canvas->h_view, 
                    TRUE);

            #ifdef OSD_LINE_SEPARATE_COMPRESSION
                /* sync the back buffer */
                x_gl_surface_compressed_blt(
                    (pt_canvas->b_compress_back_running)? pt_canvas->h_surf_compress_back: pt_canvas->h_surf_compress,
                    pt_canvas->h_surf_front,
                    0,
                    0,
                    pt_canvas->ui4_width,
                    pt_canvas->ui4_height,
                    _e_compression_ratio,
                    TRUE,
                    0);
            #endif
#endif

                pt_canvas->b_compress_back_running = !pt_canvas->b_compress_back_running;
            }
        }
        else
        {
            /* reset clipper */
            x_gl_surface_set_clipper(
                pt_canvas->h_surf_back,
                NULL_HANDLE);
            
            FLASH_AREA(pt_canvas->h_surf_front, &t_upt_rc, _ui4_flash_time);

            /* since the single canvas becomes empty, we disable it first to avoid update tearing. [DTV00130984] */
#ifndef LINUX_TURNKEY_SOLUTION
            if (b_single_cvs_empty || b_full_screen_clean)
            {
                x_gl_plane_enable(pt_canvas->h_plane, FALSE);
            }
#endif
#ifndef SYS_3D_SUPPORT
		
	#ifdef MW_OSD_COMPRESSION_SHINK
			if (!_b_osd_compress)
			{
				x_gl_surface_blt(
					pt_canvas->h_surf_front,
					pt_canvas->h_surf_back,
					t_upt_rc.i4_left,
					t_upt_rc.i4_top,
					t_upt_rc.i4_left,
					t_upt_rc.i4_top,
					(UINT32)RECT_W(&t_upt_rc),
					(UINT32)RECT_H(&t_upt_rc),
					0,
					GL_SYNC);
			   
			}
			else
			{
				
				x_gl_surface_compressed_blt(
					pt_canvas->h_surf_render,
					pt_canvas->h_surf_back,
					0,
					0,
					pt_canvas->ui4_width,
					pt_canvas->ui4_height,
					_e_compression_ratio,
					FALSE,
					0);
			
				x_gl_plane_flip(
					pt_canvas->h_plane,
					pt_canvas->h_view_back,
					TRUE);
				
				_swap_wgl_resource(pt_canvas);
			}
	#else
            x_gl_surface_blt(
                pt_canvas->h_surf_front,
                pt_canvas->h_surf_back,
                t_upt_rc.i4_left,
                t_upt_rc.i4_top,
                t_upt_rc.i4_left,
                t_upt_rc.i4_top,
                (UINT32)RECT_W(&t_upt_rc),
                (UINT32)RECT_H(&t_upt_rc),
                0,
            #ifdef LINUX_TURNKEY_SOLUTION_PHASE2
                (pt_canvas->h_surf_compress != NULL_HANDLE)? 0: GL_SYNC | GL_BLT2FRONT);
            #else
                (pt_canvas->h_surf_compress != NULL_HANDLE)? 0: GL_SYNC);
            #endif
	#endif
#else
	#ifdef MW_OSD_COMPRESSION_SHINK
			if (!_b_osd_compress)
			{
	            t_update_info.e_mode     = GL_DRV_UPDATE_3D_BLIT;
	            t_update_info.i4_x       = t_upt_rc.i4_left;
	            t_update_info.i4_y       = t_upt_rc.i4_top;
	            t_update_info.ui4_width  = (UINT32)RECT_W(&t_upt_rc);
	            t_update_info.ui4_height = (UINT32)RECT_H(&t_upt_rc);

	            gl_plane_notify_drv_update(&t_update_info);
			}
			else
			{
				t_update_info.e_mode     = GL_DRV_UPDATE_3D_BLIT;
				t_update_info.h_dst_surf = pt_canvas->h_surf_render;
	            t_update_info.i4_x       = 0;
	            t_update_info.i4_y       = 0;
	            t_update_info.ui4_width  = pt_canvas->ui4_width;
	            t_update_info.ui4_height = pt_canvas->ui4_height;

	            gl_plane_notify_drv_update(&t_update_info);

				x_gl_surface_compressed_blt(
                    pt_canvas->h_surf_back,
                    pt_canvas->h_surf_render,
                    0,
                    0,
                    pt_canvas->ui4_width,
                    pt_canvas->ui4_height,
                    _e_compression_ratio,
                    FALSE,
                    0);

                x_gl_plane_flip(
                    pt_canvas->h_plane,
                    pt_canvas->h_view_back,
                    TRUE);

				_swap_wgl_resource(pt_canvas);
			}
	#else
            t_update_info.e_mode     = GL_DRV_UPDATE_3D_BLIT;
            t_update_info.i4_x       = t_upt_rc.i4_left;
            t_update_info.i4_y       = t_upt_rc.i4_top;
            t_update_info.ui4_width  = (UINT32)RECT_W(&t_upt_rc);
            t_update_info.ui4_height = (UINT32)RECT_H(&t_upt_rc);

            gl_plane_notify_drv_update(&t_update_info);
	#endif
#endif
#ifndef MW_OSD_COMPRESSION_SHINK
            if (pt_canvas->h_surf_compress != NULL_HANDLE)
            {
                x_gl_surface_compressed_blt(
                    (!pt_canvas->b_compress_back_running)? pt_canvas->h_surf_compress_back: pt_canvas->h_surf_compress,
                    pt_canvas->h_surf_front,
                #ifndef OSD_LINE_SEPARATE_COMPRESSION
                    0,
                    0,
                    pt_canvas->ui4_width,
                    pt_canvas->ui4_height,
                    _e_compression_ratio,
                    FALSE,
                #else
                    0,
                    t_upt_rc.i4_top,
                    pt_canvas->ui4_width,
                    (UINT32)RECT_H(&t_upt_rc),
                    _e_compression_ratio,
                    TRUE,
                #endif
                    0);

                x_gl_plane_flip(
                    pt_canvas->h_plane,
                    (!pt_canvas->b_compress_back_running)? pt_canvas->h_view_compress: pt_canvas->h_view,
                    TRUE);

            #ifdef OSD_LINE_SEPARATE_COMPRESSION
                /* sync the back buffer */
                x_gl_surface_compressed_blt(
                    (pt_canvas->b_compress_back_running)? pt_canvas->h_surf_compress_back: pt_canvas->h_surf_compress,
                    pt_canvas->h_surf_front,
                    0,
                    t_upt_rc.i4_top,
                    pt_canvas->ui4_width,
                    (UINT32)RECT_H(&t_upt_rc),
                    _e_compression_ratio,
                    TRUE,
                    0);
            #endif
                pt_canvas->b_compress_back_running = !pt_canvas->b_compress_back_running;
            }
#endif

#ifndef LINUX_TURNKEY_SOLUTION /**/
            /* turn it on again [DTV00130984] */
            if (b_single_cvs_empty || b_full_screen_clean)
            {
                x_gl_plane_enable(pt_canvas->h_plane, TRUE);
            }
#endif
        }
    #ifdef TIME_MEASUREMENT
        TMS_END(PAINT_BACK_TO_FRONT);
    #endif

        gl_plane_notify_drv_update(NULL);

    #if 0
        x_gl_obj_free(h_graphics);
    #endif
    }
#ifndef MW_OSD_COMPRESSION_SHINK
    else
#endif
    {
        _wgl_unlock();
    }

    _hrc_unlock();
    
#ifdef TIME_MEASUREMENT
    TMS_END(PAINT_ALL);
#endif

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _reset_flag_recursive
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *          ui2_flag  References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _reset_flag_recursive(_WIDGET_T* pt_widget, UINT16 ui2_flag)
{
    STATE(pt_widget) &= ~ui2_flag;

    if (CHILD(pt_widget))
    {
        _reset_flag_recursive(CHILD(pt_widget), ui2_flag);
    }

    if (SIBLING(pt_widget))
    {
        _reset_flag_recursive(SIBLING(pt_widget), ui2_flag);
    }
}
/*-----------------------------------------------------------------------------
 * Name: _set_flag_recursive
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *          ui2_flag  References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _set_flag_recursive(_WIDGET_T* pt_widget, UINT16 ui2_flag)
{
    STATE(pt_widget) |= ui2_flag;

    if (CHILD(pt_widget))
    {
        _set_flag_recursive(CHILD(pt_widget), ui2_flag);
    }

    if (SIBLING(pt_widget))
    {
        _set_flag_recursive(SIBLING(pt_widget), ui2_flag);
    }
}

static VOID _set_float(_WIDGET_T *pt_widget, BOOL b_float)
{
    _WIDGET_T       *pt_child;
    
    if (b_float)
    {
        STATE(pt_widget) |= WS_FLOATED;
    }
    else
    {
        STATE(pt_widget) &= ~WS_FLOATED;
    }

    for (pt_child = CHILD(pt_widget); pt_child != NULL; pt_child = SIBLING(pt_child))
    {
        _set_float(pt_child, b_float);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _send_destroyed_recursive
 *
 * Description: <function description>
 *
 * Inputs:  pt_widget References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _send_destroyed_recursive(_WIDGET_T* pt_widget)
{
    _send_msg(pt_widget, WGL_MSG_DESTROY, NULL, NULL);
    
    if (CHILD(pt_widget))
    {
        _send_destroyed_recursive(CHILD(pt_widget));
    }

    if (SIBLING(pt_widget))
    {
        _send_destroyed_recursive(SIBLING(pt_widget));
    }
}

static BOOL _get_visibility(_WIDGET_T* pt_widget)
{
    if (!ST_VISIBLE(pt_widget))
    {
        return FALSE;
    }

    if (PARENT(pt_widget))
    {
        return (_get_visibility(PARENT(pt_widget)));
    }
    else
    {
        return TRUE;
    }
}

static VOID _set_visibility(
    _WIDGET_T*      pt_widget, 
    WGL_SW_CMD_T    e_sw_cmd)
{
    switch (e_sw_cmd)
    {
        case WGL_SW_HIDE_RECURSIVE:
            if (CHILD(pt_widget))
            {
                _reset_flag_recursive(CHILD(pt_widget), WS_VISIBLE);
            }
            
        case WGL_SW_HIDE:
            STATE(pt_widget) &= ~WS_VISIBLE;
            break;

        case WGL_SW_RECURSIVE:
            if (CHILD(pt_widget))
            {
                _set_flag_recursive(CHILD(pt_widget), WS_VISIBLE);
            }
            
        case WGL_SW_NORMAL:
            STATE(pt_widget) |= WS_VISIBLE;
            break;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _destroy_app_widget
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: WGLR_INV_CONTEXT  Success.
 *          WGLR_OK           Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 _destroy_app_widget(_APP_INFO_T* pt_app_info)
{
    _BRANCH_T*      pt_branch;
    _WIDGET_T*      pt_widget;
    _CANVAS_T*      pt_canvas;
    _DESTROY_MSG_T  t_destroy_msg;
    INT32           i4_ret;

    FOCUSED(pt_app_info)   = NULL;
    ACTIVATED(pt_app_info) = NULL;

    DLIST_FOR_EACH(pt_branch, &(pt_app_info->t_branch_lst), t_link_on_app)
    {
        pt_canvas        = CANVAS(pt_branch);
        pt_widget        = FIRST(pt_branch);
        FIRST(pt_branch) = NULL;

        if (pt_widget && !ST_DESTROYED(pt_widget))
        {
            BRANCH(pt_widget) = NULL;

            _set_flag_recursive(pt_widget, WS_DESTROYED);

            t_destroy_msg.ui4_msg   = RPM_MSG_DEL_MULTI_TREE;
            t_destroy_msg.pt_canvas = pt_canvas;
            t_destroy_msg.pt_widget = pt_widget;
            t_destroy_msg.b_repaint = TRUE;
            
            while (1)
            {
                i4_ret = x_msg_q_send(
                    _t_repaint_mngr.h_msg_q, 
                    &t_destroy_msg, 
                    sizeof(t_destroy_msg),
                    RPM_MSG_PRIORITY_DESTROY);

                if (OSR_TOO_MANY != i4_ret)
                {
                    break;
                }
            }
        }
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _destroy_app_and_notify
 *
 * Description: <function description>
 *
 * Inputs:  pt_app_info References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static INT32 _destroy_app_and_notify(_APP_INFO_T* pt_app_info)
{
    #define MAX_NUM_CANVAS      8
    
    _BRANCH_T*  pt_branch;
    _BRANCH_T*  pt_prev;
    _CANVAS_T*  pt_canvas;
    _WIDGET_T*  apt_widget[MAX_NUM_CANVAS];
    _CANVAS_T*  apt_canvas[MAX_NUM_CANVAS];
    INT32       i4_cnt = 0;
    INT32       i;
    GL_RECT_T   t_rect;

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
            
    /* delink all branches belonged to this app from canvases */
    SLIST_FOR_EACH(pt_canvas, &_t_canvas_lst, t_next)
    {
        if (!DLIST_IS_EMPTY(&(pt_canvas->t_branch_lst)))
        {
            pt_branch = DLIST_HEAD(&(pt_canvas->t_branch_lst));
            
            while (1)
            {
                if (!pt_branch)
                {
                    break;
                }
                
                pt_prev   = pt_branch;
                pt_branch = DLIST_NEXT(pt_branch, t_link_on_canvas);

                if (OWNER(pt_prev) == pt_app_info)
                {
                    if (FIRST(pt_prev))
                    {
                        apt_widget[i4_cnt] = FIRST(pt_prev);
                        apt_canvas[i4_cnt] = pt_canvas;
                        i4_cnt++;
                    }
                    
                    DLIST_REMOVE(
                        pt_prev, 
                        &(pt_canvas->t_branch_lst), 
                        t_link_on_canvas);

                    MFREE_BRANCH(pt_prev);
                    
                    break;
                }
            }
        }
    }

    MFREE_APP_INFO(pt_app_info);
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    for (i = 0; i < i4_cnt; i++)
    {
        if (_destroy_multi_tree_and_notify(apt_canvas[i], apt_widget[i], &t_rect))
        {
            _paint_canvas(apt_canvas[i], &t_rect);
        }
    }

    return (WGLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: _destroy_single_tree_and_notify
 *
 * Description: <function description>
 *
 * Inputs:  pt_canvas References.
 *          pt_widget References.
 *          pt_rect   References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static BOOL _destroy_single_tree_and_notify(
    _CANVAS_T*          pt_canvas, 
    _WIDGET_T*          pt_widget,
    GL_RECT_T*          pt_rect)
{
    GL_RECT_T       t_rect_new;
    INT32           i4_x, i4_y;
    BOOL            b_upt_old, b_upt_new;
    _WIDGET_T       *pt_obj = NULL;
    HANDLE_TYPE_T   e_type = INV_HANDLE_TYPE;
    
    _hrc_lock();

    /* check if this widget handle is still valid */
    if (handle_get_type_obj(pt_widget->h_this, &e_type, (VOID **)&pt_obj) != HR_OK ||
        !IS_WIDGET_TYPE(e_type) ||
        pt_obj == NULL)
    {
        _hrc_unlock();
    #if 0
        DBG_ABORT(DBG_MOD_WGL);
    #endif
        return FALSE;
    }
    
    _send_msg(pt_widget, WGL_MSG_DESTROY, NULL, NULL);
    
    if (CHILD(pt_widget))
    {
        _send_destroyed_recursive(CHILD(pt_widget));
    }

    x_sema_lock(_h_inst_lock, X_SEMA_OPTION_WAIT);
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), FALSE);

    b_upt_old = ST_VISIBLE(pt_widget);
    if (b_upt_old)
    {
        *pt_rect = pt_widget->t_rect;
        _cal_translation(PARENT(pt_widget), &i4_x, &i4_y);
        MOVE_RECT(pt_rect, i4_x, i4_y);
    }
    
    /* remove pt_widget from the widget hierarchy */
    b_upt_new = _remove_widget(pt_widget, &t_rect_new);
    if (b_upt_new)
    {
        if (b_upt_old)
        {
            UNION_RECT(pt_rect, pt_rect, &t_rect_new);
        }
        else
        {
            *pt_rect = t_rect_new;
        }
    }
    
    _delete_widget_group(pt_widget);
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), FALSE);

    x_sema_unlock(_h_inst_lock);
    
    _hrc_unlock();
    
    return (b_upt_old || b_upt_new);
}

/*-----------------------------------------------------------------------------
 * Name: _destroy_multi_tree
 *
 * Description: <function description>
 *
 * Inputs:  pt_canvas References.
 *          pt_widget References.
 *          pt_rect   References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _destroy_multi_tree(
    _CANVAS_T*          pt_canvas, 
    _WIDGET_T*          pt_widget,
    GL_RECT_T*          pt_rect)
{
    _WIDGET_T*  pt_tmp;
    
    *pt_rect = pt_widget->t_rect;
    
    while (1)
    {
        pt_tmp = SIBLING(pt_widget);
    
        SIBLING(pt_widget) = NULL;
        
        _delete_widget_group(pt_widget);
        
        if (pt_tmp)
        {
            pt_widget = pt_tmp;
            UNION_RECT(pt_rect, pt_rect, &(pt_widget->t_rect));
        }
        else
        {
            break;
        }
    }
    
}

/*-----------------------------------------------------------------------------
 * Name: _destroy_multi_tree_and_notify
 *
 * Description: <function description>
 *
 * Inputs:  pt_canvas References.
 *          pt_widget References.
 *          pt_rect   References.
 *
 * Outputs: - 
 *
 * Returns: TRUE  Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _destroy_multi_tree_and_notify(
    _CANVAS_T*          pt_canvas, 
    _WIDGET_T*          pt_widget,
    GL_RECT_T*          pt_rect)
{
    _hrc_lock();
    
    _send_destroyed_recursive(pt_widget);
    
    x_sema_lock(_h_inst_lock, X_SEMA_OPTION_WAIT);
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), FALSE);

    _destroy_multi_tree(pt_canvas, pt_widget, pt_rect);
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), FALSE);

    x_sema_unlock(_h_inst_lock);

    _hrc_unlock();
    
    return TRUE;
}

static VOID _widget_timer_callback(
    HANDLE_T  pt_tm_handle,
    VOID*     pv_tag)
{
    _TM_MSG_T   t_msg;

    t_msg.ui4_msg = RPM_MSG_TIMER;
    t_msg.h_timer = (HANDLE_T) pv_tag;
    
    if (x_msg_q_send(
        _t_repaint_mngr.h_msg_q, 
        &t_msg, 
        sizeof(t_msg),
        RPM_MSG_PRIORITY_TIMER) != OSR_OK)
    {
        //DBG_ABORT(DBG_MOD_WGL);
        x_dbg_stmt("{WGL} ERR:widget timer callback sent message failed!\n");
    }
}

static INT32 _proc_tm_msg(HANDLE_T h_timer)
{
    _TIMER_T*       pt_timer;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;
    
    CHECK_VALID(h_timer != NULL_HANDLE, WGLR_INV_ARG);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if ((HR_OK == handle_get_type_obj(h_timer, &e_type, (VOID**) &pt_timer)) && 
        (HT_WGL_TIMER == e_type))
    {
        _WIDGET_T*      pt_widget = pt_timer->pt_attached_widget;
        VOID*           pv_tag    = pt_timer->pv_tag;
        
        if (!ST_DESTROYED(pt_widget))
        {
            if (TM_ST_STARTED(pt_timer->ui4_flag))
            {
                if (TM_MD_ASYNC(pt_timer->ui4_flag))
                {
                    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
                
                    i4_ret = _send_msg(
                        pt_widget,
                        WGL_MSG_TIMER,
                        (VOID*) h_timer,
                        pv_tag);
                    
                    DBG_INFO(("{WGL} INFO: processing ASYNC-timer (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }
                else
                {
                    wgl_app_callback_fct pf_app_cb = OWNER(BRANCH(pt_widget))->pf_callback;
                    
                    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
                
                    i4_ret = ((*pf_app_cb)(WGL_MSG_TIMER, (VOID*) h_timer, pv_tag));

                    DBG_INFO(("{WGL} INFO: processing SYNC-timer (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }

                return (i4_ret);
            }
            else
            {
                i4_ret = WGLR_OK;
                DBG_INFO(("{WGL} INFO: Timer is already stopped (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }
        else
        {
            i4_ret = WGLR_INV_HANDLE;
            DBG_ERROR(("{WGL} ERR: Invalid attached widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget timer handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}

static VOID _delete_timer(_TIMER_T* pt_timer)
{
    SLIST_REMOVE(pt_timer, t_link);
    handle_free(pt_timer->h_this, FALSE);
}

static VOID _delete_widget_timer(SLIST_TIMER_T* pt_tm_lst)
{
    _TIMER_T*   pt_timer;
    
    SLIST_FOR_EACH(pt_timer, pt_tm_lst, t_link)
    {
        _delete_timer(pt_timer);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _handle_autofree
 *
 * Description: <function description>
 *
 * Inputs:  h_handle References.
 *          e_type   References.
 *
 * Outputs: - 
 *
 * Returns: HR_INV_HANDLE_TYPE  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _handle_autofree (
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type)
{
    INT32 i4_ret;
    
    if (e_type < HT_GROUP_WGL || e_type >= HT_WGL_WIDGET_END)
    {
        return HR_INV_HANDLE_TYPE;
    }

    switch (e_type)
    {
        case HT_WGL_CLIENT:
            DBG_INFO(("{WGL} Autofree CLIENT handle at (L%d)\r\n\r\n", __LINE__));
            i4_ret = x_wgl_unregister(h_handle);
            break;

        case HT_WGL_CANVAS:
            DBG_INFO(("{WGL} Autofree CANVAS handle at (L%d), which would be ignored.\r\n\r\n", __LINE__));
            i4_ret = WGLR_OK;
            break;

        case HT_WGL_TIMER:
            DBG_INFO(("{WGL} Autofree TIMER handle at (L%d), which would be ignored.\r\n\r\n", __LINE__));
            i4_ret = WGLR_OK;   /* the destroy of client handle would definitely destroy all its widgets */
            break;
            
        default: /* widget handle types */
        #if 0
            i4_ret = x_wgl_destroy_widget(h_handle);
        #else
            DBG_INFO(("{WGL} Autofree WIDGET handle at (L%d), which would be ignored.\r\n\r\n", __LINE__));
            i4_ret = WGLR_OK;   /* the destroy of client handle would definitely destroy all its widgets */
        #endif
            break;
    }
    
    return (i4_ret == WGLR_OK ? HR_OK : HR_INV_HANDLE);
}

#ifdef CLI_LVL_ALL
static VOID _flash_area(
    GL_HSURFACE_T   h_surf,
    GL_RECT_T*      pt_rect, 
    UINT32          ui4_time)
{
    if (ui4_time > 0)
    {
        GL_COLOR_T t_color = GL_NC_YELLOW;
        
        x_gl_surface_set_clipper(
            h_surf,
            NULL_HANDLE);
            
        x_gl_surface_draw_rect(
            h_surf, 
            pt_rect, 
            &t_color, 
            0, 
            GL_STYLE_FILLED, 
            GL_SYNC);
        
        x_thread_delay(ui4_time);
    }
}
#endif

#ifdef WGL_COORDINATE_INVERSE
static VOID _widget_tree_inverse_x_coordinate_pos(_WIDGET_T*  pt_widget,UINT32 ui4_parent_width,BOOL b_coordinate_reverse)
{
    _WIDGET_T* pt_child;
    
    UINT32 ui4_width = RECT_W(&pt_widget->t_rect); 
    
    pt_widget->t_rect.i4_left -=  pt_widget->i4_x_pos_reverse;
    pt_widget->t_rect.i4_right -= pt_widget->i4_x_pos_reverse;
    if(b_coordinate_reverse && !pt_widget->b_fix_coordinate_dir)
    {
        pt_widget->i4_x_pos_reverse = ui4_parent_width-ui4_width-pt_widget->t_rect.i4_left*2;
        pt_widget->t_rect.i4_left +=  pt_widget->i4_x_pos_reverse;
        pt_widget->t_rect.i4_right += pt_widget->i4_x_pos_reverse;
    }
    else
    {
        pt_widget->i4_x_pos_reverse = 0;
    }
    
    for (pt_child = CHILD(pt_widget); pt_child; pt_child = SIBLING(pt_child))
    {
       _widget_tree_inverse_x_coordinate_pos(pt_child,ui4_width, b_coordinate_reverse);
    }

}
static VOID _widget_tree_notify_move_msg(_WIDGET_T*  pt_widget)
{
    _WIDGET_T* pt_child;    
    wgl_widget_proc_fct pf_proc;
   
    if(!pt_widget->b_fix_coordinate_dir)
    {
        pf_proc = MSGPROC(pt_widget);
        (*pf_proc)(
            pt_widget->h_this, 
            WGL_MSG_MOVE, 
            (VOID*) COMBINE_HI_LO(pt_widget->t_rect.i4_left, pt_widget->t_rect.i4_top),
            0);
    }
    for (pt_child = CHILD(pt_widget); pt_child; pt_child = SIBLING(pt_child))
    {
       _widget_tree_notify_move_msg(pt_child);
    }

}

#endif

/*-----------------------------------------------------------------------------
 * Name: wgl_init
 *
 * Description: This API initializes the widget library and should be called 
 *              during the middleware initialization process.
 *
 * Inputs:  ui4_width                   Contains the width of the frame buffer.
 *          ui4_height                  Contains the height of the frame 
 *                                      buffer.
 *          e_colormode                 Contains the color mode of the frame 
 *                                      buffer.
 *          b_auto_scale                Contains a flag indicating whether the
 *                                      widget view should be scaled 
 *                                      automatically (according to the 
 *                                      resolution change) or not.
 *          ui2_num_widget              Contains the maximum number of widgets.
 *          ui2_key_first_repeat_delay  Contains the time interval between 
 *                                      KEY_DOWN and the first KEY_REPEAT input 
 *                                      event in ms.
 *          ui2_key_repeat_delay        Contains the time interval between two 
 *                                      continuous KEY_REPEAT input event in 
 *                                      ms.
 *          pt_thread_desc              References a THREAD_DESCR_T structure 
 *                                      which describes several attributes 
 *                                      regarding of creating of the Repaint 
 *                                      Manager thread. Note that if values of 
 *                                      these attributes are invalid (e.g. 0), 
 *                                      the default values are used instead.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_ARG        The argument pt_thread_desc is NULL.
 *          WGLR_INIT_FAILED    The initialization of Widget Library failed.
 ----------------------------------------------------------------------------*/
 INT32 wgl_init(
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_COLORMODE_T      e_colormode,
    BOOL                b_auto_scale,
    BOOL                b_no_inter_compose,
    BOOL                b_support_compose_anim,
    BOOL                b_osd_compress,
    BOOL                b_compressed_compose,
    UINT16              ui2_clut_sz,
    const GL_COLOR_T*   pt_clut,
    const GL_COLOR_T*   pt_color_key,
    UINT16              ui2_num_widget,
    UINT8               ui1_max_floated_widget,
    UINT16              ui2_key_first_repeat_delay,
    UINT16              ui2_key_repeat_delay,
    THREAD_DESCR_T*     pt_thread_desc)
{
   return _wgl_init_inner(
                       ui4_width,
                       ui4_height,
                       e_colormode,
                       b_auto_scale,
                       b_no_inter_compose,
                       b_support_compose_anim,
                       b_osd_compress,
                       b_compressed_compose,
                       ui2_clut_sz,
                       pt_clut,
                       pt_color_key,
                       ui2_num_widget,
                       ui1_max_floated_widget,
                       ui2_key_first_repeat_delay,
                       ui2_key_repeat_delay,
                       pt_thread_desc,
                       TRUE);
}

static INT32 _wgl_init_inner(
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_COLORMODE_T      e_colormode,
    BOOL                b_auto_scale,
    BOOL                b_no_inter_compose,
    BOOL                b_support_compose_anim,
    BOOL                b_osd_compress,
    BOOL                b_compressed_compose,
    UINT16              ui2_clut_sz,
    const GL_COLOR_T*   pt_clut,
    const GL_COLOR_T*   pt_color_key,
    UINT16              ui2_num_widget,
    UINT8               ui1_max_floated_widget,
    UINT16              ui2_key_first_repeat_delay,
    UINT16              ui2_key_repeat_delay,
    THREAD_DESCR_T*     pt_thread_desc,
    BOOL                b_need_init_iom)
{
    static handle_autofree_fct apf_autofree_fcts[HT_WGL_WIDGET_END - HT_GROUP_WGL];

    INT32               i,i4_ret=WGLR_OK;
    IRRC_SETTING_T      t_irrc_setting;
    GL_COLOR_T          t_def_color_key = {0, {0}, {0}, {0}};
    GL_COLOR_T*         pt_def_clut;
    UINT16              ui2_size;
    GL_COLORMODE_T      e_plt_mode = GL_COLORMODE_ARGB_CLUT8;
    
    CHECK_VALID(pt_thread_desc, WGLR_INV_ARG);

    _ui4_surf_width       = (ui4_width == 0) ? WM_DEFAULT_SURFACE_WIDTH : ui4_width;
    _ui4_surf_height      = (ui4_height == 0) ? WM_DEFAULT_SURFACE_HEIGHT : ((ui4_height + 1) & ~1);
    _e_surf_mode          = e_colormode;
    _b_auto_scale         = b_auto_scale;
    _b_inter_compose      = !b_no_inter_compose;
    _b_support_compose_anim =b_support_compose_anim;
    _b_osd_compress       = b_osd_compress;
    _b_compressed_compose = b_compressed_compose;
    _t_color_key          = (pt_color_key ? (*pt_color_key) : t_def_color_key);

    _ui1_max_floated_widget = (ui1_max_floated_widget == 0)? WM_MAX_FLOATED_WIDGET: ui1_max_floated_widget;

    _t_paint_info.at_rc_global = (GL_RECT_T *)x_mem_calloc((UINT32)_ui1_max_floated_widget, sizeof(GL_RECT_T));

    if (_t_paint_info.at_rc_global == NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }

    _t_paint_info.at_pwi = (_PAINT_WIDGET_INFO_T *)x_mem_calloc((UINT32)_ui1_max_floated_widget, sizeof(_PAINT_WIDGET_INFO_T));

    if (_t_paint_info.at_pwi == NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }
        
    if ((ui2_clut_sz > 0) && pt_clut)
    {
        pt_def_clut = (GL_COLOR_T*) pt_clut;
        ui2_size    = ui2_clut_sz;
    }
    else
    {
        wgl_get_sys_clut(&pt_def_clut, &ui2_size);
    }

    if (e_colormode == GL_COLORMODE_AYUV_CLUT8)
    {
        e_plt_mode = e_colormode;
    }

    if (GL_FAILED(x_gl_palette_create(
        e_plt_mode, 
        ui2_size, 
        pt_def_clut, 
        &_h_sys_pal)))
    {
        return WGLR_INIT_FAILED;
    }
    
#if 0
    _ui2_max_widget  = (ui2_num_widget == 0) ? WM_DEFAULT_NUM_WIDGETS : ui2_num_widget;
#endif

#ifdef WGL_ANIM_SUPPORT
    if (WGL_FAILED(wgl_anim_attr_init()))
    {
        return WGLR_INIT_FAILED;
    }
#endif
    if (WGL_FAILED(wgl_cli_init()))
    {
        return WGLR_INIT_FAILED;
    }

    for (i = 0; i < (INT32)(HT_WGL_WIDGET_END - HT_GROUP_WGL); i++)
    {
        apf_autofree_fcts[i] = _handle_autofree;
    }
        
    if (handle_set_autofree_tbl(HT_GROUP_WGL, apf_autofree_fcts) != HR_OK)
    {
        return WGLR_INIT_FAILED;
    }
    
    if (!_init_repaint_mngr(pt_thread_desc, RPM_MSG_SIZE))
    {
        return WGLR_INIT_FAILED;
    }

    if (x_timer_create(&_h_shdw_cvs_blk_timer) != OSR_OK)
    {
        return WGLR_INIT_FAILED;
    }
#ifdef WGL_NPM_WIDGET_SUPPORT
    if(wgl_npm_mem_init()!= WGLR_OK)
    {
        return WGLR_INIT_FAILED;
    }
#endif
    if(b_need_init_iom)
    {
        t_irrc_setting.ui8_evt_grp_mask         = WM_DEFAULT_RC_APP_KEY_GROUPS;
        t_irrc_setting.t_rpt_evt_itvl.ui2_1st   = (UINT16)(ui2_key_first_repeat_delay / 10); /* 10 ms per unit*/
        t_irrc_setting.t_rpt_evt_itvl.ui2_other = (UINT16)(ui2_key_repeat_delay / 10);     /* 10 ms per unit*/   
        t_irrc_setting.b_notify_raw_data        = FALSE;
        i4_ret = x_iom_open(
                IOM_DEV_TYPE_IN_IRRC,     /* remote controller */
                ANY_COMP_ID,
                NULL,
                (VOID*) &t_irrc_setting,
                (VOID*) &_h_input,            
                _widget_iom_nfy_fct,
                &_h_input);
        
        
    }

    if( i4_ret == WGLR_OK)
    {
       if (x_sema_create(&_h_hrc_lock, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK)
        {
            if (x_sema_create(&_h_wgl_lock, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK)
            {
                if (x_sema_create(&_h_inst_lock, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK)
                {
                    /* initialize the link list of __CANVAS_T */
                    SLIST_INIT(&_t_canvas_lst);

                    /* initialize the link list of _APP_INFO_T */
                    DLIST_INIT(&_t_app_lst);
                }
                
                return WGLR_OK;
            }
        }
    }
    return WGLR_INIT_FAILED;
}

#ifdef LINUX_TURNKEY_SOLUTION_PHASE2
/* MHF CHUN, We need a version which support initialize WGL without IOM. */
INT32 wgl_init_without_iom(
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_COLORMODE_T      e_colormode,
    BOOL                b_auto_scale,
    BOOL                b_no_inter_compose,
    BOOL                b_support_compose_anim,
    BOOL                b_osd_compress,
    BOOL                b_compressed_compose,
    UINT16              ui2_clut_sz,
    const GL_COLOR_T*   pt_clut,
    const GL_COLOR_T*   pt_color_key,
    UINT16              ui2_num_widget,
    UINT8               ui1_max_floated_widget,
    UINT16              ui2_key_first_repeat_delay,
    UINT16              ui2_key_repeat_delay,
    THREAD_DESCR_T*     pt_thread_desc)
{
    return _wgl_init_inner(
                       ui4_width,
                       ui4_height,
                       e_colormode,
                       b_auto_scale,
                       b_no_inter_compose,
                       b_support_compose_anim,
                       b_osd_compress,
                       b_compressed_compose,
                       ui2_clut_sz,
                       pt_clut,
                       pt_color_key,
                       ui2_num_widget,
                       ui1_max_floated_widget,
                       ui2_key_first_repeat_delay,
                       ui2_key_repeat_delay,
                       pt_thread_desc,
                       FALSE);
}
#endif

/*-----------------------------------------------------------------------------
 * Name: wgl_is_valid_widget
 *
 * Description: This API checks if the given handle is a valid widget handle.
 *
 * Inputs:  h_widget    Contains the handle of the specified widget.
 *
 * Outputs: - 
 *
 * Returns: TRUE    The handle is a valid widget handle..
 *          FALSE   The handle is an invalid widget handle..
 ----------------------------------------------------------------------------*/
BOOL wgl_is_valid_widget(HANDLE_T h_widget)
{
    HANDLE_TYPE_T   e_hdl_type;

    if (HR_OK == x_handle_get_type(h_widget, &e_hdl_type) &&
        IS_WIDGET_TYPE(e_hdl_type))
    {
        return TRUE;
    }

    return FALSE;
}

#ifdef CLI_LVL_ALL
UINT16 wgl_get_dbg_level(VOID)
{
    return (_ui2_dbg_level);
}
#endif

/*-----------------------------------------------------------------------------
 * Name: wgl_get_instance
 *
 * Description: This API returns the instance reference of the specified 
 *              widget. The widget instance is created by the widget itself at 
 *              the time x_wgl_create_widget() is called.
 *
 * Inputs:  h_widget    Contains the handle of the specified widget.
 *          pt_inst     References the widget instance.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK	        The routine was successful.
 *          WGLR_INV_HANDLE The argument h_widget contains an invalid widget 
 *                          handle.
 *          WGLR_INV_ARG    The argument pt_inst is NULL.
 *          WGLR_LOCK_ERROR Locking the instance semaphore failed.
 ----------------------------------------------------------------------------*/
INT32 wgl_get_instance(
    HANDLE_T                        h_widget,
    WGL_WIDGET_CONTEXT_HANDLE_T*    pt_inst)
{
    _WIDGET_T*      pt_widget;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;

    CHECK_VALID(pt_inst, WGLR_INV_ARG);
    
    *pt_inst = NULL;
    i4_ret   = WGLR_LOCK_ERROR;
        
    if (OSR_OK == x_sema_lock(
        _h_inst_lock,
        X_SEMA_OPTION_WAIT))
    {
        if (WGLR_OK == _wgl_lock())
        {
            if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    ||
                !IS_WIDGET_TYPE(e_type)                                                 ||
                ST_DESTROYED(pt_widget))
            {
                _wgl_unlock();
                x_sema_unlock(_h_inst_lock);
                i4_ret = WGLR_INV_HANDLE;
            }
            else
            {
                *pt_inst = pt_widget->t_instance;
                _wgl_unlock();
                i4_ret = WGLR_OK;
            }
        }
    }
    
    return (i4_ret);
}

VOID wgl_get_mem_status(
    INT32* pi4_mem,
    INT32* pi4_widget,
    INT32* pi4_branch,
    INT32* pi4_canvas,
    INT32* pi4_app_info)
{
    *pi4_mem      = _i4_alloc_mem;
    *pi4_widget   = _i4_alloc_widget;
    *pi4_branch   = _i4_alloc_branch;
    *pi4_canvas   = _i4_alloc_canvas;
    *pi4_app_info = _i4_alloc_app_info;
}

INT32 wgl_get_widget_private_tag(
    HANDLE_T    h_widget,
    VOID        **ppv_tag)
{
    INT32           i4_ret;
    _WIDGET_T       *pt_widget = NULL;
    HANDLE_TYPE_T   e_type;

    if (ppv_tag == NULL)
    {
        return WGLR_INV_ARG;
    }

    i4_ret = handle_get_type_obj(h_widget, &e_type, (VOID **)&pt_widget);

    if (i4_ret == HR_OK && IS_WIDGET_TYPE(e_type) && pt_widget != NULL)
    {
        *ppv_tag = pt_widget->pv_private_tag;

        return WGLR_OK;
    }

    return WGLR_INV_HANDLE;
}

#ifdef CLI_LVL_ALL
VOID wgl_show_front_buf_info(VOID)
{
#if 0
    UINT32      ui4_i;
    GL_LOCK_INFO_T t_lock_info;
    for (ui4_i = 0; ui4_i < _ui4_canvas_count; ui4_i++)
    {
        x_gl_surface_lock(_aui4_surf_h[ui4_i], NULL, GL_SYNC, &t_lock_info);
        x_dbg_stmt("\nFB(%d):\n",   ui4_i);
        x_dbg_stmt("Width:  %5d\n", t_lock_info.at_subplane[0].ui4_width);
        x_dbg_stmt("Height: %5d\n", t_lock_info.at_subplane[0].ui4_height);
        x_dbg_stmt("Pitch:  %5d\n", t_lock_info.at_subplane[0].ui4_pitch);
        x_dbg_stmt(
            "\nd.save.binary d:\\front%d.dmp 0x%X--0x%X\n",
            ui4_i,
            t_lock_info.at_subplane[0].pv_bits,
            (UINT32)t_lock_info.at_subplane[0].pv_bits + t_lock_info.at_subplane[0].ui4_height * t_lock_info.at_subplane[0].ui4_pitch);
        x_gl_surface_unlock(_aui4_surf_h[ui4_i], NULL);
    }
#endif



}
#endif

/*-----------------------------------------------------------------------------
 * Name: wgl_release_instance
 *
 * Description: This API releases the instance reference of the specified 
 *              widget and unlocks the internal instance semaphore. Note that 
 *              this API must be called if the caller has ever successfully 
 *              invoked wgl_get_instance().
 *
 * Inputs:  h_widget    Contains the handle of the specified widget.
 *          t_inst      Contains the widget instance returned by the prior call 
 *                      to wgl_get_instance().
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK         The routine was successful.
 *          WGLR_INV_HANDLE The argument h_widget contains an invalid widget 
 *                          handle.
 *          WGLR_INV_ARG    The argument t_inst is not the instance of the 
 *                          widget.
 ----------------------------------------------------------------------------*/
INT32 wgl_release_instance(
    HANDLE_T                    h_widget,
    WGL_WIDGET_CONTEXT_HANDLE_T t_inst)
{
    _WIDGET_T*                  pt_widget;
    HANDLE_TYPE_T               e_type;
    WGL_WIDGET_CONTEXT_HANDLE_T t_winst;
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if (HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget) &&
        IS_WIDGET_TYPE(e_type))
    {
        t_winst = pt_widget->t_instance;
        
        _wgl_unlock();
        
        if (t_inst == t_winst)
        {
            x_sema_unlock(_h_inst_lock);
            return WGLR_OK;
        }
        else
        {
            return WGLR_INV_ARG;
        }
    }
    else
    {
        _wgl_unlock();
        return WGLR_INV_HANDLE;
    }
}

#ifdef CLI_LVL_ALL
VOID wgl_set_dbg_level(UINT16 ui2_dbg_level)
{
    _ui2_dbg_level = ui2_dbg_level;
}
#endif

#ifdef CLI_LVL_ALL
VOID wgl_set_flash_time(UINT32 ui4_flash_time)
{
    _ui4_flash_time = ui4_flash_time;
}
#endif

INT32 wgl_set_widget_private_tag(
    HANDLE_T        h_widget,
    VOID            *pv_tag)
{
    INT32           i4_ret;
    _WIDGET_T       *pt_widget = NULL;
    HANDLE_TYPE_T   e_type;

    i4_ret = handle_get_type_obj(h_widget, &e_type, (VOID **)&pt_widget);

    if (i4_ret == HR_OK && IS_WIDGET_TYPE(e_type) && pt_widget != NULL)
    {
        pt_widget->pv_private_tag = pv_tag;

        return WGLR_OK;
    }

    return WGLR_INV_HANDLE;
}

BOOL wgl_using_compressed_compose(VOID)
{
    return _b_compressed_compose;
}

INT32 wgl_widget_lock(VOID)
{
    return _wgl_lock();
}

INT32 wgl_widget_unlock(VOID)
{
    return _wgl_unlock();
}

BOOL wgl_cur_thread_is_mw_thread(void)
{
     HANDLE_T h_thread = NULL_HANDLE;
     
    if (x_thread_self (&h_thread) != OSR_OK)
    {
        DBG_ABORT(DBG_MOD_WGL);
    }

     return (h_thread == _t_repaint_mngr.h_thread);
}

INT32 wgl_create_widget(
    HANDLE_T                h_container,
    HANDLE_TYPE_T           e_type_widget,
    UINT32                  ui4_type_content,
    UINT32                  ui4_type_border,
    const GL_RECT_T*        pt_rect,
    wgl_widget_proc_fct     pf_wdgt_proc,
    UINT8                   ui1_alpha,
    VOID*                   pv_param,
    VOID*                   pv_tag,
    HANDLE_T *              ph_widget)
{
    HANDLE_T                    h_thread;
    _WIDGET_T*                  pt_widget;
    _WIDGET_T*                  pt_parent;
    WGL_WIDGET_CTRL_INTF_T*     pt_fct_tbl;
    HANDLE_TYPE_T               e_type;
    _CANVAS_T*                  pt_canvas;
    _BRANCH_T*                  pt_branch;
    VOID*                       pv_obj;
    WGL_WIDGET_CONTEXT_HANDLE_T t_instance;
#ifdef WGL_ANIM_SUPPORT
    WGL_WIDGET_ANIM_CONTEXT_T   t_anim = NULL;
#endif
    WGL_WIDGET_CREATE_INPUT_T   t_create_in;
    _APP_INFO_T*                pt_app_info;
    WGL_CREATE_INFO_T           t_create_info;
    HANDLE_T                    h_widget;
    INT32                       i4_ret = WGLR_OK;

    wgl_widget_proc_fct         pf_proc;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_container      = %d\r\n"
         "\te_type_widget    = %d\r\n"
         "\tui4_type_content = %d\r\n"
         "\tui4_type_border  = %d\r\n"
         "\tpt_rect          = 0x%.8x\r\n"
         "\tpf_wdgt_proc     = 0x%.8x\r\n"
         "\tui1_alpha        = %d\r\n"
         "\tpv_param         = 0x%.8x\r\n"
         "\tpv_tag           = 0x%.8x\r\n"
         "\tph_widget        = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_container,
         e_type_widget,
         ui4_type_content,
         ui4_type_border,
         (UINT32) pt_rect,
         (UINT32) pf_wdgt_proc,
         ui1_alpha,
         (UINT32) pv_param,
         (UINT32) pv_tag,
         (UINT32) ph_widget));
    
    CHECK_VALID(pt_rect != NULL && ph_widget != NULL, WGLR_INV_ARG);
    CHECK_VALID(OSR_OK == x_thread_self(&h_thread), WGLR_OS_ERROR);
    CHECK_VALID(
        WGLR_OK == wgl_query_comp_intf(e_type_widget, (VOID**) &pt_fct_tbl),
        WGLR_INV_ARG);
    
    if (HR_OK != handle_get_type_obj(h_container, &e_type, &pv_obj))
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid container handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ALLOC_RET;
    }
    
    /* find the associated _APP_INFO_T structure */
    pt_app_info = _find_app_info(h_thread);
    if (!pt_app_info)
    {   /* no _APP_INFO_T found, implies that no AP callback has been set */
        i4_ret = WGLR_INV_CONTEXT;
        DBG_ERROR(("{WGL} ERR: Cannot find the associated client info (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ALLOC_RET;
    }

    if (IS_WIDGET_TYPE(e_type) && !ST_DESTROYED((_WIDGET_T*) pv_obj))
    {
        /* h_container contains a handle of widget */
        pt_parent = (_WIDGET_T*) pv_obj;
        pt_branch = BRANCH(pt_parent);
        pt_canvas = CANVAS(pt_branch);

        if (pt_branch->pt_owner_app != pt_app_info)
        {   /* h_container is a widget handle, but its owner app is not pt_app_info */
            i4_ret = WGLR_INV_HANDLE;
            DBG_ERROR(("{WGL} ERR: Container and widget belong to different clients (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _ALLOC_RET;
        }
    }
    else if (e_type == HT_WGL_CANVAS)
    {
        pt_canvas = (_CANVAS_T*) pv_obj;
        pt_parent = NULL;
        
        /* finds the branch node of pt_app_info which locates on pt_canvas */
        DLIST_FOR_EACH(pt_branch, &(pt_app_info->t_branch_lst), t_link_on_app)
        {
            if (pt_branch->pt_canvas == pt_canvas)
                break;
        }

        /* not found, create a new branch node on pt_canvas */
        if (!pt_branch)
        {
            pt_branch = _new_branch(pt_app_info, pt_canvas);
            if (!pt_branch)
            {
                i4_ret = WGLR_OUT_OF_MEMORY;
                DBG_ERROR(("{WGL} ERR: Out of memory (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                goto _ALLOC_RET;
            }
        }
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid container handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ALLOC_RET;
    }

    /* create widget instance */
    t_create_in.ui4_type_content = ui4_type_content;
    t_create_in.ui4_type_border  = ui4_type_border;
    t_create_in.pt_rect          = pt_rect;
    t_create_in.pv_param         = pv_param;
    t_create_in.h_palette        = pt_canvas->h_palette;
    _get_canvas_info(pt_canvas, &(t_create_in.t_cvs_info));
    
    i4_ret = (pt_fct_tbl->pf_create_instance)(&t_instance, &t_create_in);
    if (WGL_FAILED(i4_ret))
    {
        DBG_ERROR(("{WGL} ERR: Creating widget instance failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ALLOC_RET;
    }
    
    /* allocate a new widget */
    if (NULL == (pt_widget = _new_widget(e_type_widget, pv_tag)))
    {
        (pt_fct_tbl->pf_destroy_instance)(t_instance);
        i4_ret = WGLR_OUT_OF_MEMORY;
        DBG_ERROR(("{WGL} ERR: Out of memory (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ALLOC_RET;
    }

    /* initialize the widget structure */
    STATE(pt_widget)            = WS_ENABLED;
    ALPHA(pt_widget)            = ui1_alpha;
    BRANCH(pt_widget)           = pt_branch;
    PARENT(pt_widget)           = pt_parent;
    CHILD(pt_widget)            = NULL;
    MSGPROC(pt_widget)          = (pf_wdgt_proc ? pf_wdgt_proc : pt_fct_tbl->pf_msg_proc);
    pt_widget->t_rect           = (*pt_rect);
    pt_widget->ui1_num_key_link = 0;
    pt_widget->ui4_attrib       = pt_fct_tbl->ui4_attribute;
    pt_widget->pt_fct_tbl       = pt_fct_tbl;
    pt_widget->t_instance       = t_instance;
    SET_RECT(&DIRTY(pt_widget), 0, 0, 0, 0);
    pt_widget->pv_private_tag   = NULL;
    
    if (e_type == HT_WGL_CANVAS)
    {
        SIBLING(pt_widget)  = FIRST(pt_branch);
        FIRST(pt_branch)    = pt_widget;
    }
    else
    {
        SIBLING(pt_widget)  = CHILD(pt_parent);
        CHILD(pt_parent)    = pt_widget;
    }

#ifdef WM_DEBUG
    i4_widget_cnt++;
    pt_branch->i4_widget_cnt++;
    pt_app_info->i4_widget_cnt++;
    pt_canvas->i4_widget_cnt++;
#endif

#ifdef WGL_ANIM_SUPPORT
    /*create widget animation handle*/    
    i4_ret = wgl_anim_create(&t_anim);
    if (WGL_FAILED(i4_ret))
    {
            DBG_ERROR(("{WGL} ERR: Creating widget animation instance failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _ALLOC_RET;
    }
    ANIM(pt_widget) = t_anim;
    if(e_type_widget == HT_WGL_WIDGET_FRAME || e_type_widget == HT_WGL_WIDGET_GROUP)
    {
        pt_widget->b_anim_affect_child = ((UINT32)pv_param)&WGL_STL_GL_ANIM_AFFECT_CHILD;
    }
    else
    {
        pt_widget->b_anim_affect_child =FALSE;
    }
    pt_widget->ui1_map_count = 0;
    pt_widget->t_widget_anim_cb.pf_anim_callback = NULL;
    pt_widget->t_widget_anim_cb.pv_tag = NULL;
#endif   
#ifdef WGL_COORDINATE_INVERSE
      if(e_type_widget == HT_WGL_WIDGET_LIST_BOX)
  	  {
        	pt_widget->b_fix_coordinate_dir = (((WGL_LB_INIT_T *)pv_param)->ui4_style)&WGL_STL_GL_FIX_COORDINATE_DIR;
  	  }
   	  else
   	  {
        	pt_widget->b_fix_coordinate_dir = ((UINT32)pv_param)&WGL_STL_GL_FIX_COORDINATE_DIR;
   	  }    
    
     if(_b_coordinate_reverse && !pt_widget->b_fix_coordinate_dir)
     {
        UINT32 ui4_parent_width;
        UINT32 ui4_width = RECT_W(&pt_widget->t_rect); 
        if(pt_parent == NULL)
        {
            pt_canvas = CANVAS(BRANCH(pt_widget));
            ui4_parent_width = pt_canvas->ui4_width;
        }
        else
        {
        
            ui4_parent_width = RECT_W(&pt_parent->t_rect);
        }
        pt_widget->i4_x_pos_reverse = ui4_parent_width-ui4_width-pt_widget->t_rect.i4_left*2;
    }
    else
    {
        pt_widget->i4_x_pos_reverse = 0;
    }
    
    pt_widget->t_rect.i4_left +=  pt_widget->i4_x_pos_reverse;
    pt_widget->t_rect.i4_right += pt_widget->i4_x_pos_reverse;
#endif        
    h_widget = pt_widget->h_this;
    pf_proc  = MSGPROC(pt_widget);
    
    /* send WGL_MSG_CREATE to the widget procedure */
    t_create_info.pt_rect   = (GL_RECT_T*) pt_rect;
    t_create_info.pv_param  = pv_param;

    i4_ret = (*pf_proc)(h_widget, WGL_MSG_CREATE, &t_create_info, NULL);

    if (WGL_FAILED(i4_ret))
    {
        DBG_ERROR(("{WGL} ERR: Calling widget procedure for creation failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        x_wgl_destroy_widget(h_widget);
        return (i4_ret);
    }
    
    *ph_widget = h_widget;

    return WGLR_OK;

_ALLOC_RET:
    return (i4_ret);
}

INT32 x_wgl_add_transparent_list_exclusion(
    HANDLE_T            h_rgn_list,
    GL_RECT_T           at_exclusive_rect[],
    UINT32              ui4_rect_count)
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    _TRANSP_REGION_T    *pt_region = NULL;
    
    if (h_rgn_list == NULL_HANDLE)
    {
        return WGLR_INV_ARG;
    }

    i4_ret = handle_get_type_obj(h_rgn_list, &e_type, (VOID **)&pt_region);

    if (i4_ret != HR_OK || e_type != HT_WGL_TRANP_LIST || pt_region == NULL)
    {
        return WGLR_INV_HANDLE;
    }

    _wgl_lock();

    if (pt_region->ui4_exclusive_count != ui4_rect_count)
    {
        if (ui4_rect_count != 0)
        {
            pt_region->pt_exclusive_rect = x_mem_realloc(pt_region->pt_exclusive_rect, ui4_rect_count * sizeof(GL_RECT_T));

            if (pt_region->pt_exclusive_rect == NULL)
            {
                _wgl_unlock();

                DBG_ERROR(("{WGL} re-allocate transparent list element failed. Line: %d", __LINE__));
                return WGLR_OUT_OF_RESOURCE;
            }
        }
        else
        {
            x_mem_free(pt_region->pt_exclusive_rect);

            pt_region->pt_exclusive_rect = NULL;
        }
    }

    if (pt_region->pt_exclusive_rect != NULL)
    {
        x_memcpy(pt_region->pt_exclusive_rect, at_exclusive_rect, ui4_rect_count * sizeof(GL_RECT_T));
    }

    pt_region->ui4_exclusive_count = ui4_rect_count;
    
    _wgl_unlock();
    
    return WGLR_OK;
}


INT32 x_wgl_broadcast_cmd(
    HANDLE_T            h_container,
    UINT32              ui4_cmd, 
    VOID*               pv_param1,
    VOID*               pv_param2)
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    VOID                *pv_obj = NULL;
    WGL_PACKED_PARAM_T  t_pparam;

    i4_ret = handle_get_type_obj(h_container, &e_type, &pv_obj);

    if (i4_ret != HR_OK && pv_obj == NULL)
    {
        return WGLR_INV_HANDLE;
    }

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    i4_ret = WGLR_OK;

    t_pparam.pv_param1 = pv_param1;
    t_pparam.pv_param2 = pv_param2;

    if (IS_WIDGET_TYPE(e_type) && !ST_DESTROYED((_WIDGET_T*) pv_obj))
    {
        _WIDGET_T       *pt_widget = (_WIDGET_T *)pv_obj;
        
        _broadcast_single_widget_msg(pt_widget, WGL_MSG_COMMAND, (VOID *)ui4_cmd, (VOID *)&t_pparam);
    }
    else if (e_type == HT_WGL_CANVAS)
    {
        _CANVAS_T       *pt_canvas = (_CANVAS_T *)pv_obj;
        
        _broadcast_widget_msg(pt_canvas, WGL_MSG_COMMAND, (VOID *)ui4_cmd, (VOID *)&t_pparam);
    }
    else if (e_type == GLT_OBJ_PLANE)
    {
        _CANVAS_T*  pt_canvas;
        BOOL        b_cmp;
    
        SLIST_FOR_EACH(pt_canvas, &_t_canvas_lst, t_next)
        {
            if ((GLR_OK == gl_is_same_plane(pt_canvas->h_plane, h_container, &b_cmp)) &&
                (TRUE   == b_cmp))
            {
                _broadcast_widget_msg(pt_canvas, WGL_MSG_COMMAND, (VOID *)ui4_cmd, (VOID *)&t_pparam);
            }
        }
    }
    else
    {
        i4_ret = WGLR_INV_TYPE;
        DBG_ERROR(("{WGL} ERR: Invalid container handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return i4_ret;
}

BOOL x_wgl_canvas_with_visibles(HANDLE_T h_canvas)
{
    _CANVAS_T           *pt_canvas;
    HANDLE_TYPE_T       e_type;

    if (HR_OK == handle_get_type_obj(h_canvas, &e_type, (VOID**) &pt_canvas) &&
        HT_WGL_CANVAS == e_type)
    {
        return pt_canvas->b_with_visibles;
    }
    else
    {
        return FALSE;
    }
}

INT32 x_wgl_create_timer(
    HANDLE_T                h_widget,
    WGL_TIMER_SYNC_MODE_T   e_sync_mode,
    HANDLE_T*               ph_timer)
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget    = %d\r\n"
         "\te_sync_mode = %d\r\n"   
         "\tph_timer    = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_widget,
         e_sync_mode,
         (UINT32) ph_timer));
    
    CHECK_VALID(ph_timer != NULL, WGLR_INV_ARG);
    CHECK_VALID(
        (WGL_TIMER_SYNC == e_sync_mode) || (WGL_TIMER_ASYNC == e_sync_mode),
        WGLR_INV_ARG);
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    i4_ret = WGLR_INV_HANDLE;
    
    if (HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget) && 
        IS_WIDGET_TYPE(e_type)                                               &&
        !ST_DESTROYED(pt_widget))
    {
        HANDLE_T h_sys_timer;
        
        i4_ret = x_timer_create(&h_sys_timer);
        
        if (OSR_OK == i4_ret)
        {
            _TIMER_T* pt_timer = (_TIMER_T*) MALLOC_TIMER();

            if (pt_timer)
            {
                HANDLE_T h_this;
                
                i4_ret = handle_alloc(
                    HT_WGL_TIMER, 
                    (VOID*) pt_timer, 
                    NULL, 
                    _handle_free_timer, 
                    &h_this);

                if (HR_OK == i4_ret)
                {
                    pt_timer->h_this             = h_this;
                    pt_timer->h_sys_timer        = h_sys_timer;
                    pt_timer->ui4_flag           = ((e_sync_mode == WGL_TIMER_ASYNC) ? TIMER_MODE_ASYNC : 0);
                    pt_timer->pv_tag             = NULL;
                    pt_timer->pt_attached_widget = pt_widget;
                    SLIST_INSERT_HEAD(pt_timer, &(pt_widget->t_tm_lst), t_link);
                    (*ph_timer) = h_this;
                   #ifdef CLI_LVL_ALL     
                    pt_widget->ui4_mem_size += sizeof(_TIMER_T);
                   #endif   
                    i4_ret = WGLR_OK;
                }
                else
                {
                    i4_ret = WGLR_OUT_OF_HANDLES;
                    x_timer_delete(h_sys_timer);
                    MFREE_TIMER(pt_timer);
                    DBG_ERROR(("{WGL} ERR: Allocate timer structure failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }
            }
            else
            {
                i4_ret = WGLR_OUT_OF_MEMORY;
                x_timer_delete(h_sys_timer);
                DBG_ERROR(("{WGL} ERR: Allocate timer structure failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }
        else
        {
            i4_ret = WGLR_OUT_OF_RESOURCE;
            DBG_ERROR(("{WGL} ERR: Create system timer failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
        
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}

/* create a transparent list. after a list is successfully created, it is DISABLED by default */
INT32 x_wgl_create_transparent_list(
    HANDLE_T                h_canvas,               /* in:  handle of the target canvas*/
    GL_RECT_T               at_rect[],              /* in:  an array of GL_RECT_T */
    UINT32                  ui4_rect_count,         /* in:  rectangle counts of at_rect[] */
    HANDLE_T                *ph_rgn_list)           /* out: handle of the transparent list */
{
    INT32                   i4_ret;
    HANDLE_TYPE_T           e_type;
    _CANVAS_T               *pt_canvas = NULL;
    _TRANSP_REGION_T        *pt_transp_region = NULL;
    _TRANSP_LIST_T          *pt_list = NULL;

    /* check parameters */
    if (h_canvas == NULL_HANDLE || at_rect == NULL || ui4_rect_count == 0 || ph_rgn_list == NULL)
    {
        return WGLR_INV_ARG;
    }

    i4_ret = handle_get_type_obj(h_canvas, &e_type, (VOID **)&pt_canvas);

    if (i4_ret != HR_OK || e_type != HT_WGL_CANVAS || pt_canvas == NULL)
    {
        return WGLR_INV_HANDLE;
    }

    /* allocate memory for handle object */
    pt_transp_region = (_TRANSP_REGION_T *)WGL_MEM_ALLOC(sizeof(_TRANSP_REGION_T));

    if (pt_transp_region == NULL)
    {
        DBG_ERROR(("{WGL} allocate transparent region object failed. Line: %d", __LINE__));
        return WGLR_OUT_OF_RESOURCE;
    }

    x_memset(pt_transp_region, 0, sizeof(_TRANSP_REGION_T));

    /* create a handle */
    i4_ret = handle_alloc(HT_WGL_TRANP_LIST, (VOID *)pt_transp_region, NULL, _free_transp_region, ph_rgn_list);

    if (i4_ret != HR_OK)
    {
        WGL_MEM_FREE(pt_transp_region);

        DBG_ERROR(("{WGL} allocate transparent list handle failed. Line: %d", __LINE__));
        return WGLR_OUT_OF_RESOURCE;
    }

    /* allocate memory for storing rectangles */
    pt_transp_region->pt_rect = (GL_RECT_T *)x_mem_alloc(ui4_rect_count * sizeof(GL_RECT_T));

    if (pt_transp_region->pt_rect == NULL)
    {
        handle_free(*ph_rgn_list, TRUE);
        
        DBG_ERROR(("{WGL} allocate transparent region entry failed. Line: %d", __LINE__));
        return WGLR_OUT_OF_RESOURCE;
    }

    /* read in rectangle data */
    x_memcpy(pt_transp_region->pt_rect, at_rect, ui4_rect_count * sizeof(GL_RECT_T));

    /* add this list to the canvase */
    pt_list = (_TRANSP_LIST_T *)WGL_MEM_ALLOC(sizeof(_TRANSP_LIST_T));

    if (pt_list == NULL)
    {
        handle_free(*ph_rgn_list, TRUE);

        DBG_ERROR(("{WGL} allocate transparent list element failed. Line: %d", __LINE__));
        return WGLR_OUT_OF_RESOURCE;
    }

    pt_transp_region->h_canvas       = h_canvas;
    pt_transp_region->ui4_rect_count = ui4_rect_count;

    _wgl_lock();
    
    DLIST_INSERT_HEAD(pt_list, &pt_canvas->t_transp_list, t_link);

    pt_list->b_enalbed  = FALSE;
    pt_list->h_rgn_list = *ph_rgn_list;

    _wgl_unlock();
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_create_widget
 *
 * Description: This API creates a new widget with type specified in 
 *              e_type_widget. If the creation succeeds, the new widget is 
 *              inserted into the widget hierarchy of the calling thread.
 *
 * Inputs:  h_container         Contains either the handle of the parent widget 
 *                              or the handle of the canvas on which this 
 *                              widget should be displayed. If a canvas handle 
 *                              is present, the created widget will be a 
 *                              top-level widget.
 *          e_type_widget       Contains the handle type of the widget to be 
 *                              created.
 *          ui4_type_content    Contains the content type of the widget to be 
 *                              created.
 *          ui4_type_border     Contains the border type of the widget to be 
 *                              created.
 *          pt_rect             References a GL_RECT_T structure which defines 
 *                              the display area of the widget to be created.
 *          pf_wdgt_proc        Contains the function pointer of the 
 *                              application defined message procedure for the 
 *                              new widget. A NULL value indicates the default 
 *                              widget message procedure will be used.
 *          ui1_alpha           Contains the transparency level of the widget. 
 *                              A completely transparent widget is with value 
 *                              0, while an opaque one is with value 255.
 *          pv_param            References the data used for widget creation 
 *                              and initialization. The real content is 
 *                              dependent on the actual widget type.
 *          pv_tag              Contains an application-defined tag value which
 *                              is attached to the returned widget handle. The 
 *                              application can retrieve this value with the 
 *                              handle API x_handle_get_tag().
 *
 * Outputs: ph_widget   References the handle of the created widget if this 
 *                      routine was successful. In all other cases, this field 
 *                      is undefined.
 *
 * Returns: WGLR_OK         The routine was successful.
 *          WGLR_INV_ARG    The argument pt_rect is NULL or references an empty 
 *                          rectangle. Or ph_widget is NULL.
 *          WGLR_INV_HANDLE The argument h_parent contains an invalid widget 
 *                          handle except NULL_HANDLE.
 *          WGLR_INV_ TYPE  The argument e_type_widget contains an invalid 
 *                          widget type, or the argument ui4_type_content 
 *                          contains either an invalid or incompatible content 
 *                          type, or the argument ui4_type_border contains an 
 *                          invalid border type.
 *
 *          WGLR_OUT_OF_MEMORY  There is not enough memory to complete the 
 *                              operation.
 *          WGLR_OS_ERROR       This API is not called from a thread context.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_create_widget(
    HANDLE_T                h_container,
    HANDLE_TYPE_T           e_type_widget,
    UINT32                  ui4_type_content,
    UINT32                  ui4_type_border,
    const GL_RECT_T*        pt_rect,
    wgl_widget_proc_fct     pf_wdgt_proc,
    UINT8                   ui1_alpha,
    VOID*                   pv_param,
    VOID*                   pv_tag,
    HANDLE_T *              ph_widget)
{
    HANDLE_T                    h_thread;
    _WIDGET_T*                  pt_widget;
    _WIDGET_T*                  pt_parent;
    WGL_WIDGET_CTRL_INTF_T*     pt_fct_tbl;
    HANDLE_TYPE_T               e_type;
    _CANVAS_T*                  pt_canvas;
    _BRANCH_T*                  pt_branch;
    VOID*                       pv_obj;
    WGL_WIDGET_CONTEXT_HANDLE_T t_instance;
#ifdef WGL_ANIM_SUPPORT
    WGL_WIDGET_ANIM_CONTEXT_T   t_anim = NULL;
#endif
    WGL_WIDGET_CREATE_INPUT_T   t_create_in;
    _APP_INFO_T*                pt_app_info;
    WGL_CREATE_INFO_T           t_create_info;
    HANDLE_T                    h_widget;
    INT32                       i4_ret = WGLR_OK;

    wgl_widget_proc_fct         pf_proc;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_container      = %d\r\n"
         "\te_type_widget    = %d\r\n"
         "\tui4_type_content = %d\r\n"
         "\tui4_type_border  = %d\r\n"
         "\tpt_rect          = 0x%.8x\r\n"
         "\tpf_wdgt_proc     = 0x%.8x\r\n"
         "\tui1_alpha        = %d\r\n"
         "\tpv_param         = 0x%.8x\r\n"
         "\tpv_tag           = 0x%.8x\r\n"
         "\tph_widget        = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_container,
         e_type_widget,
         ui4_type_content,
         ui4_type_border,
         (UINT32) pt_rect,
         (UINT32) pf_wdgt_proc,
         ui1_alpha,
         (UINT32) pv_param,
         (UINT32) pv_tag,
         (UINT32) ph_widget));
    
    CHECK_VALID(pt_rect != NULL && ph_widget != NULL, WGLR_INV_ARG);
    CHECK_VALID(OSR_OK == x_thread_self(&h_thread), WGLR_OS_ERROR);
    CHECK_VALID(
        WGLR_OK == wgl_query_comp_intf(e_type_widget, (VOID**) &pt_fct_tbl),
        WGLR_INV_ARG);
    
    _hrc_lock();
    
    /* get handle type of h_container */
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_container, &e_type, &pv_obj))
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid container handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ALLOC_RET;
    }
    
    /* find the associated _APP_INFO_T structure */
    pt_app_info = _find_app_info(h_thread);
    if (!pt_app_info)
    {   /* no _APP_INFO_T found, implies that no AP callback has been set */
        i4_ret = WGLR_INV_CONTEXT;
        DBG_ERROR(("{WGL} ERR: Cannot find the associated client info (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ALLOC_RET;
    }

    if (IS_WIDGET_TYPE(e_type) && !ST_DESTROYED((_WIDGET_T*) pv_obj))
    {
        /* h_container contains a handle of widget */
        pt_parent = (_WIDGET_T*) pv_obj;
        pt_branch = BRANCH(pt_parent);
        pt_canvas = CANVAS(pt_branch);

        if (pt_branch->pt_owner_app != pt_app_info)
        {   /* h_container is a widget handle, but its owner app is not pt_app_info */
            i4_ret = WGLR_INV_HANDLE;
            DBG_ERROR(("{WGL} ERR: Container and widget belong to different clients (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _ALLOC_RET;
        }
    }
    else if (e_type == HT_WGL_CANVAS)
    {
        pt_canvas = (_CANVAS_T*) pv_obj;
        pt_parent = NULL;
        
        /* finds the branch node of pt_app_info which locates on pt_canvas */
        DLIST_FOR_EACH(pt_branch, &(pt_app_info->t_branch_lst), t_link_on_app)
        {
            if (pt_branch->pt_canvas == pt_canvas)
                break;
        }

        /* not found, create a new branch node on pt_canvas */
        if (!pt_branch)
        {
            pt_branch = _new_branch(pt_app_info, pt_canvas);
            if (!pt_branch)
            {
                i4_ret = WGLR_OUT_OF_MEMORY;
                DBG_ERROR(("{WGL} ERR: Out of memory (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                goto _ALLOC_RET;
            }
        }
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid container handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ALLOC_RET;
    }

    /* create widget instance */
    t_create_in.ui4_type_content = ui4_type_content;
    t_create_in.ui4_type_border  = ui4_type_border;
    t_create_in.pt_rect          = pt_rect;
    t_create_in.pv_param         = pv_param;
    t_create_in.h_palette        = pt_canvas->h_palette;
    _get_canvas_info(pt_canvas, &(t_create_in.t_cvs_info));
    
    i4_ret = (pt_fct_tbl->pf_create_instance)(&t_instance, &t_create_in);
    if (WGL_FAILED(i4_ret))
    {
        DBG_ERROR(("{WGL} ERR: Creating widget instance failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ALLOC_RET;
    }
    
    /* allocate a new widget */
    if (NULL == (pt_widget = _new_widget(e_type_widget, pv_tag)))
    {
        (pt_fct_tbl->pf_destroy_instance)(t_instance);
        i4_ret = WGLR_OUT_OF_MEMORY;
        DBG_ERROR(("{WGL} ERR: Out of memory (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ALLOC_RET;
    }

    /* initialize the widget structure */
    STATE(pt_widget)            = WS_ENABLED;
    ALPHA(pt_widget)            = ui1_alpha;
    BRANCH(pt_widget)           = pt_branch;
    PARENT(pt_widget)           = pt_parent;
    CHILD(pt_widget)            = NULL;
    MSGPROC(pt_widget)          = (pf_wdgt_proc ? pf_wdgt_proc : pt_fct_tbl->pf_msg_proc);
    pt_widget->t_rect           = (*pt_rect);
    pt_widget->ui1_num_key_link = 0;
    pt_widget->ui4_attrib       = pt_fct_tbl->ui4_attribute;
    pt_widget->pt_fct_tbl       = pt_fct_tbl;
    pt_widget->t_instance       = t_instance;
    SET_RECT(&DIRTY(pt_widget), 0, 0, 0, 0);
    pt_widget->pv_private_tag   = NULL;
    
    if (e_type == HT_WGL_CANVAS)
    {
        SIBLING(pt_widget)  = FIRST(pt_branch);
        FIRST(pt_branch)    = pt_widget;
    }
    else
    {
        SIBLING(pt_widget)  = CHILD(pt_parent);
        CHILD(pt_parent)    = pt_widget;
    }

#ifdef WM_DEBUG
    i4_widget_cnt++;
    pt_branch->i4_widget_cnt++;
    pt_app_info->i4_widget_cnt++;
    pt_canvas->i4_widget_cnt++;
#endif

#ifdef WGL_ANIM_SUPPORT
    /*create widget animation handle*/    
    i4_ret = wgl_anim_create(&t_anim);
    if (WGL_FAILED(i4_ret))
    {
            DBG_ERROR(("{WGL} ERR: Creating widget animation instance failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _ALLOC_RET;
    }
    ANIM(pt_widget) = t_anim;
    if(e_type_widget == HT_WGL_WIDGET_FRAME || e_type_widget == HT_WGL_WIDGET_GROUP)
    {
        pt_widget->b_anim_affect_child = ((UINT32)pv_param)&WGL_STL_GL_ANIM_AFFECT_CHILD;
    }
    else
    {
        pt_widget->b_anim_affect_child =FALSE;
    }
    pt_widget->ui1_map_count = 0;
    pt_widget->t_widget_anim_cb.pf_anim_callback = NULL;
    pt_widget->t_widget_anim_cb.pv_tag = NULL;
#endif   
#ifdef WGL_COORDINATE_INVERSE
      if(e_type_widget == HT_WGL_WIDGET_LIST_BOX)
  	  {
        	pt_widget->b_fix_coordinate_dir = (((WGL_LB_INIT_T *)pv_param)->ui4_style)&WGL_STL_GL_FIX_COORDINATE_DIR;
  	  }
   	  else
   	  {
        	pt_widget->b_fix_coordinate_dir = ((UINT32)pv_param)&WGL_STL_GL_FIX_COORDINATE_DIR;
   	  }    
    if(_b_coordinate_reverse && !pt_widget->b_fix_coordinate_dir)
    {
        UINT32 ui4_parent_width;
        UINT32 ui4_width = RECT_W(&pt_widget->t_rect); 
        if(pt_parent == NULL)
        {
            pt_canvas = CANVAS(BRANCH(pt_widget));
            ui4_parent_width = pt_canvas->ui4_width;
        }
        else
        {
        
            ui4_parent_width = RECT_W(&pt_parent->t_rect);
        }
        pt_widget->i4_x_pos_reverse = ui4_parent_width-ui4_width-pt_widget->t_rect.i4_left*2;
    }
    else
    {
        pt_widget->i4_x_pos_reverse = 0;
    }
    pt_widget->t_rect.i4_left +=  pt_widget->i4_x_pos_reverse;
    pt_widget->t_rect.i4_right += pt_widget->i4_x_pos_reverse;
#endif                
    h_widget = pt_widget->h_this;
    pf_proc  = MSGPROC(pt_widget);
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    _hrc_unlock();
    
    /* send WGL_MSG_CREATE to the widget procedure */
    t_create_info.pt_rect   = (GL_RECT_T*) pt_rect;
    t_create_info.pv_param  = pv_param;

    i4_ret = (*pf_proc)(h_widget, WGL_MSG_CREATE, &t_create_info, NULL);

    if (WGL_FAILED(i4_ret))
    {
        DBG_ERROR(("{WGL} ERR: Calling widget procedure for creation failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        x_wgl_destroy_widget(h_widget);
        return (i4_ret);
    }
    
    *ph_widget = h_widget;

    return WGLR_OK;

_ALLOC_RET:
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    _hrc_unlock();
    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_default_msg_proc
 *
 * Description: This API calls the default message procedure of of h_widget. 
 *              For instance, if h_widget represents a Button widget, the 
 *              message will be routed to the default message procedure 
 *              provided by Button. The arguments passed to this API are 
 *              identical to those passed to the message procedure.
 *
 * Inputs:  h_widget    Contains the handle to the specified widget.
 *          ui4_msg     Contains  the message id.
 *          pv_param1   Specifies the additional information. The actual 
 *                      content of this argument depends on ui4_msg.
 *          pv_param2   Specifies more additional information. The actual 
 *                      content of this argument depends on ui4_msg.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The function call is successful.
 *          WGLR_INV_HANDLE     The argument h_widget is not a valid widegt 
 *                              handle.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_default_msg_proc(
    HANDLE_T            h_widget,
    UINT32              ui4_msg,
    VOID*               pv_param1,
    VOID*               pv_param2)
{
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget = NULL;
    INT32           i4_ret;
    
    wgl_widget_proc_fct pf_proc;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget  = %d\r\n"
         "\tui4_msg   = %d\r\n"
         "\tpv_param1 = 0x%.8x\r\n"
         "\tpv_param2 = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_widget,
         ui4_msg,
         pv_param1,
         pv_param2));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    || 
        !IS_WIDGET_TYPE(e_type)                                                 ||
        ST_DESTROYED(pt_widget))
    {
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", WGLR_INV_HANDLE, __LINE__));
        CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
        
        return ((pt_widget != NULL && ST_DESTROYED(pt_widget))? WGLR_ALREADY_DESTROYED: WGLR_INV_HANDLE);
    }

    h_widget  = pt_widget->h_this;
    pf_proc   = DFLT_MSGPROC(pt_widget);
            
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    i4_ret = (*pf_proc)(h_widget, ui4_msg, pv_param1, pv_param2);
    if(i4_ret == WGLR_KEY_MSG_NOT_PROCESSED)
    { 
        HANDLE_T           h_thread;  
        UINT32             ui4_msg_not_processed  = 0;
        VOID*              pv_param1_not_processed = NULL;
        WGL_NOTIFY_DATA_T*  pt_nfy_data = NULL;
        CHECK_VALID(OSR_OK == x_thread_self(&h_thread), WGLR_OS_ERROR);
        CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
        if(ui4_msg == WGL_MSG_NOTIFY)
        {
                pt_nfy_data = (WGL_NOTIFY_DATA_T*)pv_param2;
                if( pt_nfy_data != NULL&& 
                   (pt_nfy_data->ui4_nc_code == WGL_NC_GL_KEY_UP ||
                    pt_nfy_data->ui4_nc_code == WGL_NC_GL_KEY_DOWN ||
                    pt_nfy_data->ui4_nc_code == WGL_NC_GL_KEY_REPEAT)
                  )
                {
                    if(pt_nfy_data->u.t_key.ui4_key_state == WGL_KEY_STATE_DOWN)
                    {
                        ui4_msg_not_processed = WGL_MSG_KEY_DOWN;
                    }
                    else if(pt_nfy_data->u.t_key.ui4_key_state == WGL_KEY_STATE_UP)
                    {
                        ui4_msg_not_processed = WGL_MSG_KEY_UP;
                    }
                    else
                    {
                        ui4_msg_not_processed = WGL_MSG_KEY_REPEAT;
                    }
                    pv_param1_not_processed = WGL_PACK(pt_nfy_data->u.t_key.ui4_key_code);
                }
        }
        else if(ui4_msg == WGL_MSG_KEY_DOWN ||
            	ui4_msg == WGL_MSG_KEY_UP ||
            	ui4_msg == WGL_MSG_KEY_REPEAT)
        {
            ui4_msg_not_processed = ui4_msg;
            pv_param1_not_processed = pv_param1;
        }
        if(ui4_msg_not_processed == WGL_MSG_KEY_DOWN ||
           ui4_msg_not_processed == WGL_MSG_KEY_UP ||
           ui4_msg_not_processed == WGL_MSG_KEY_REPEAT
          )
        {
            _APP_INFO_T* pt_app_info = _find_app_info(h_thread);
            if (pt_app_info )
            {
                pt_app_info->b_msg_not_processed_by_widget = TRUE;
                pt_app_info->ui4_msg = ui4_msg_not_processed;
                pt_app_info->pv_param1 = pv_param1_not_processed;
            }
        }
        CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
        /*WGLR_KEY_MSG_NOT_PROCESSED is for inner judge and need not reture outside*/
        i4_ret = WGLR_OK;
    }
    if (WGL_FAILED(i4_ret))
    {
        DBG_ERROR(("{WGL} ERR: Calling default widget procedure failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    return (i4_ret);
}

INT32 x_wgl_delete_timer(HANDLE_T h_timer)
{
    _TIMER_T*       pt_timer;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_timer  = %d\r\n\r\n",
         __FUNCTION__,
         h_timer));
    
    CHECK_VALID(h_timer != NULL_HANDLE, WGLR_INV_ARG);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if ((HR_OK == handle_get_type_obj(h_timer, &e_type, (VOID**) &pt_timer)) && 
        (HT_WGL_TIMER == e_type))
    {
        _delete_timer(pt_timer);
        i4_ret = WGLR_OK;
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget timer handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}

/* delete a transparent list */
INT32 x_wgl_delete_transparent_list(
    HANDLE_T            h_rgn_list)             /* in: handle of a transparent list to be deleted */
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    _TRANSP_REGION_T    *pt_obj = NULL;
    _CANVAS_T           *pt_canvas = NULL;
    _TRANSP_LIST_T      *pt_list;
    BOOL                b_found = FALSE;
    
    if (h_rgn_list == NULL_HANDLE)
    {
        return WGLR_INV_ARG;
    }

    i4_ret = handle_get_type_obj(h_rgn_list, &e_type, (VOID **)&pt_obj);

    if (i4_ret != HR_OK || e_type != HT_WGL_TRANP_LIST || pt_obj == NULL)
    {
        DBG_ERROR(("{WGL} ERR: invalid transparent list handle at L%d\r\n\r\n", __LINE__));
        return WGLR_INV_HANDLE;
    }

    i4_ret = handle_get_type_obj(pt_obj->h_canvas, &e_type, (VOID **)&pt_canvas);

    if (i4_ret != HR_OK || e_type != HT_WGL_CANVAS || pt_canvas == NULL)
    {
        DBG_ERROR(("{WGL} ERR: invalid canvas handle at L%d\r\n\r\n", __LINE__));
        return WGLR_INV_HANDLE;
    }

    _wgl_lock();

    /* remove from list */
    DLIST_FOR_EACH(pt_list, &pt_canvas->t_transp_list, t_link)
    {
        if (pt_list->h_rgn_list == h_rgn_list)
        {
            b_found = TRUE;

            DLIST_REMOVE(pt_list, &pt_canvas->t_transp_list, t_link);
            WGL_MEM_FREE(pt_list);

            break;
        }
    }

    _wgl_unlock();

    if (b_found)
    {
        /* release handle */
        handle_free(h_rgn_list, TRUE);
    
        return WGLR_OK;
    }
    else
    {
        DBG_ERROR(("{WGL} ERR: no such transparent list in associated canvas at L%d\r\n\r\n", __LINE__));
        return WGLR_INV_HANDLE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_destroy_app_widget
 *
 * Description: This API destroys all widgets owned by the calling thread. If 
 *              the calling thread is the thread of the current active 
 *              application, the application behind along the application list 
 *              becomes active.
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_CONTEXT    No widgets are ever created in the calling 
 *                              thread.
 *          WGLR_OS_ERROR       This API is not called from a thread context.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_destroy_app_widget(VOID)
{
    INT32           i4_ret;
    _APP_INFO_T*    pt_app_info;
    HANDLE_T        h_thread;
    
    DBG_API(("{WGL} %s: \r\n\r\n", __FUNCTION__));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    CHECK_VALID(OSR_OK == x_thread_self(&h_thread), WGLR_OS_ERROR);
    
    pt_app_info = _find_app_info(h_thread);
    
    if (pt_app_info)
    {
        i4_ret = _destroy_app_widget(pt_app_info);
        if (WGL_FAILED(i4_ret))
        {
            DBG_ERROR(("{WGL} ERR: Destroying all app widgets failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    else
    {
        i4_ret = WGLR_INV_CONTEXT;
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_destroy_widget
 *
 * Description: This API destroys the widget represent by h_widget. The widget 
 *              and all its child widgets are removed from the widget hierarchy 
 *              immediately. An additional repaint might be induced if 
 *              necessary.
 *
 * Inputs:  h_widget    Contains the handle of the widget to be destroyed.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.

 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_destroy_widget(HANDLE_T h_widget)
{
    return (x_wgl_destroy_widget_ex(h_widget, TRUE));
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_destroy_widget_ex
 *
 * Description: This API destroys the widget represent by h_widget. The widget 
 *              and all its child widgets are removed from the widget hierarchy 
 *              immediately. An additional repaint might be induced if 
 *              necessary.
 *
 * Inputs:  h_widget    Contains the handle of the widget to be destroyed.
 *          b_repaint   Indicate whether or not to repaint the specified widget 
 *                      and its children after being destroyed.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.

 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_destroy_widget_ex(HANDLE_T h_widget, BOOL b_repaint)
{
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget;
    _CANVAS_T*      pt_canvas;
    _DESTROY_MSG_T  t_destroy_msg;
    INT32           i4_ret;

    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget  = %d\r\n",
         __FUNCTION__,
         h_widget));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget) ||
        !IS_WIDGET_TYPE(e_type) ||
        ST_DESTROYED(pt_widget))
    {
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", WGLR_INV_HANDLE, __LINE__));
        CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
        return WGLR_INV_HANDLE;
    }

    STATE(pt_widget) |= WS_DESTROYED;
  
    if (CHILD(pt_widget))
    {
        _set_flag_recursive(CHILD(pt_widget), WS_DESTROYED);
    }

    pt_canvas = CANVAS(BRANCH(pt_widget));

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    t_destroy_msg.ui4_msg   = RPM_MSG_DEL_SINGLE_TREE;
    t_destroy_msg.pt_canvas = pt_canvas;
    t_destroy_msg.pt_widget = pt_widget;
    t_destroy_msg.b_repaint = b_repaint;
    
    if (OSR_OK != x_msg_q_send(
        _t_repaint_mngr.h_msg_q, 
        &t_destroy_msg, 
        sizeof(t_destroy_msg),
        RPM_MSG_PRIORITY_DESTROY))
    {
        INT32       i4_i;
        BOOL        b_sent_ok = FALSE;

        for (i4_i = 0; i4_i < 10; i4_i++)
        {
            if (x_msg_q_send(
                _t_repaint_mngr.h_msg_q, 
                &t_destroy_msg, 
                sizeof(t_destroy_msg),
                RPM_MSG_PRIORITY_DESTROY) == OSR_OK)
            {
                b_sent_ok = TRUE;
                break;
            }
            else
            {
                x_thread_delay(10);
            }
        }

        if (b_sent_ok)
        {
            i4_ret = WGLR_OK;
        }
        else
        {
            i4_ret = WGLR_OS_ERROR;
            DBG_ERROR(("{WGL} ERR: Sending message to Repaint Manager failed (%d), at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    else
    {
        i4_ret = WGLR_OK;
    }

    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_dispatch_event
 *
 * Description: This API retrieves the focused widget of the calling thread and 
 *              dispatched the specified message to the message procedure of 
 *              the widget. Note that this API should be called in the 
 *              application thread context.
 *
 * Inputs:  ui4_msg     Contains  the message id.
 *          pv_param1   Specifies the additional information. The actual 
 *                      content of this argument depends on ui4_msg.
 *          pv_param2   Specifies more additional information. The actual 
 *                      content of this argument depends on ui4_msg.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The function call is successful.
 *          WGLR_INV_CONTEXT    No widgets are ever created in the calling 
 *                              thread.
 *          WGLR_EVENT_IGNORED  The passed message is ignored. That's probably 
 *                              because the delay exceeds the pre-setting delay 
 *                              tolerance.
 *          WGLR_OS_ERROR       This API is not called from a thread context.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 *          WGLR_NO_FOCUSED_WIDGET  There's no focused widget of the calling 
 *                                  thread.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_dispatch_event(
    UINT32              ui4_msg,
    VOID*               pv_param1,
    VOID*               pv_param2)
{
    _WIDGET_T*      pt_widget;
    HANDLE_T        h_thread;
    INT32           i4_ret = WGLR_INV_STATE;
    HANDLE_T        h_target = NULL_HANDLE;

    wgl_widget_proc_fct pf_proc = NULL;
    HANDLE_T            h_widget = NULL_HANDLE;
#ifdef WGL_ANIM_SUPPORT    
    _ANIM_TICK_MSG_DATA_T t_anim_msg_data;

    _ANIM_APP_LIST_T    *pt_app_element = NULL;
    BOOL                b_send_anim_tick_to_widget = FALSE;
#endif    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\tui4_msg   = %d\r\n"
         "\tpv_param1 = 0x%.8x\r\n"
         "\tpv_param2 = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         ui4_msg,
         pv_param1,
         pv_param2));
    
    CHECK_VALID(OSR_OK == x_thread_self(&h_thread), WGLR_OS_ERROR);
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (WGL_MSG_TIMER == ui4_msg)
    {
        HANDLE_TYPE_T   e_type;
        _TIMER_T*       pt_timer;
        
        if ((HR_OK == handle_get_type_obj((HANDLE_T) pv_param1, &e_type, (VOID**) &pt_timer)) && 
            (HT_WGL_TIMER == e_type))
        {
            pt_widget = pt_timer->pt_attached_widget;
            h_widget  = pt_widget->h_this;
            pf_proc   = MSGPROC(pt_widget);
            h_target  = NULL_HANDLE;
        }
        else
        {
            i4_ret = WGLR_INV_ARG;
            DBG_ERROR(("{WGL} ERR: Invalid arguments (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _DISPATCH_EVENT_RET;
        }
    }
#ifdef WGL_ANIM_SUPPORT    
    else if (ui4_msg == WGL_MSG_ANIMATION_TICK)
    {
        _APP_INFO_T         *pt_app_info = _find_app_info(h_thread);
        _ANIM_APP_LIST_T     *pt_app_elem = NULL;
        BOOL                  b_app_found = FALSE;
        i4_ret = WGLR_OK;
        _ANIM_TICK_MSG_DATA_T* pt_msg_data = (_ANIM_TICK_MSG_DATA_T*)pv_param2;

        pt_app_element = (_ANIM_APP_LIST_T *)pv_param1;

        if (pt_msg_data == NULL)
        {
            i4_ret = WGLR_EVENT_IGNORED;
            DBG_ERROR(("{WGL} ERR: invalid element in the application list is passed for the animation tick.\n"));
            goto _DISPATCH_EVENT_RET;
        }
        x_memset(&t_anim_msg_data,0,sizeof(_ANIM_TICK_MSG_DATA_T));
        t_anim_msg_data.ui1_app_idx = pt_msg_data->ui1_app_idx;
        t_anim_msg_data.ui4_tick = pt_msg_data->ui4_tick;
        
        WGL_MEM_FREE(pt_msg_data);
        
         if (pt_app_element == NULL)
        {
            i4_ret = WGLR_EVENT_IGNORED;
            DBG_ERROR(("{WGL} ERR: invalid element in the application list is passed for the animation tick.\n"));
            goto _DISPATCH_EVENT_RET;
        }
        
        if (!pt_app_info)
        {
            i4_ret = WGLR_INV_CONTEXT;
            DBG_ERROR(("{WGL} ERR: Cannot find the associated client info.\n"));
            goto _DISPATCH_EVENT_RET;
        }
        
        CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
        
        CHECK_VALID(WGLR_OK == _app_list_lock(), WGLR_LOCK_ERROR);
        /*in order to judge if the app has been removed from _t_anim_mngr in wgl_end_animation(), 
                thus this app is not valid and memory has been freed */
        SLIST_FOR_EACH(pt_app_elem, &_t_anim_mngr.t_app_list, t_link)
         {
            if(pt_app_elem == pt_app_element)
            {
                b_app_found = TRUE;
                break;
            }
         }        
        CHECK_VALID(WGLR_OK == _app_list_unlock(), WGLR_UNLOCK_ERROR);
        
        CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
        if(!b_app_found)
        {
            goto _DISPATCH_EVENT_RET;
        }
        if (pt_app_element->pt_app_info == pt_app_info && t_anim_msg_data.ui1_app_idx == pt_app_info->ui1_anim_app_idx )
        {
            b_send_anim_tick_to_widget = TRUE;
        }
        else
        {
            if (pt_app_element->pt_app_info == NULL)
            {
                i4_ret = WGLR_EVENT_IGNORED;
                DBG_ERROR(("{WGL} ERR: the app entry for this tick msg has been removed."));
            }
            else
            {
                i4_ret = WGLR_INV_CONTEXT;
                DBG_ERROR(("{WGL} ERR: invalid context for dispatching tick message."));
            }
            
            goto _DISPATCH_EVENT_RET;
        }
    }
#endif
    else
    {
        _APP_INFO_T* pt_app_info = _find_app_info(h_thread);
        
        if (!pt_app_info)
        {
            i4_ret = WGLR_INV_CONTEXT;
            DBG_ERROR(("{WGL} ERR: Cannot find the associated client info (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _DISPATCH_EVENT_RET;
        }

        if (((UINT32) pv_param2) != 0 &&
            (ui4_msg == WGL_MSG_KEY_DOWN || ui4_msg == WGL_MSG_KEY_REPEAT))
        {
            if (pt_app_info->ui4_max_delay != WGL_NO_KEY_SKIP)
            {
                UINT32 ui4_tick_period = x_os_drv_get_tick_period();
                UINT32 ui4_tm_old = ui4_tick_period * (UINT32) pv_param2;
                UINT32 ui4_tm_new = ui4_tick_period * os_get_sys_tick();

                if ((ui4_tm_new - ui4_tm_old) > pt_app_info->ui4_max_delay)
                {
                    /* ignore this key event */
                    i4_ret = WGLR_EVENT_IGNORED;
                    x_dbg_stmt("{WGL} ERR: key [%d] is skipped (%d) at L%d\r\n\r\n", (UINT32) pv_param1, i4_ret, __LINE__);
                    goto _DISPATCH_EVENT_RET;
                }
            }
        }
        
        pt_widget = FOCUSED(pt_app_info);
        
        if (!pt_widget || ST_DESTROYED(pt_widget) || !ST_ENABLED(pt_widget))
        {
            if(_pf_widget_ignore_key_fct != NULL && 
                (ui4_msg==WGL_MSG_KEY_DOWN || ui4_msg==WGL_MSG_KEY_UP|| ui4_msg==WGL_MSG_KEY_REPEAT)
              )
            {
            
                CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
                _pf_widget_ignore_key_fct(h_thread,ui4_msg,(UINT32)pv_param1);
                
                CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
            }
            i4_ret = WGLR_NO_FOCUSED_WIDGET;
            DBG_ERROR(("{WGL} ERR: Cannot find focused widget (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _DISPATCH_EVENT_RET;
        }
        
        h_widget = pt_widget->h_this;
        pf_proc  = MSGPROC(pt_widget);
        
        h_target = _check_navigation_key(pt_widget, ui4_msg, (UINT32) pv_param1);
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    if (ui4_msg != WGL_MSG_ANIMATION_TICK)
    {
        if (h_target == NULL_HANDLE)
        {
            i4_ret = (*pf_proc)(h_widget, ui4_msg, pv_param1, pv_param2);
            if(WGLR_OK == i4_ret )
            {
                 CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
                 _APP_INFO_T* pt_app_info = _find_app_info(h_thread);
                 if (pt_app_info != NULL && 
                     pt_app_info->b_msg_not_processed_by_widget &&
                     pt_app_info->ui4_msg == ui4_msg &&
                     pt_app_info->pv_param1 == pv_param1 )
                    {
                         if(_pf_widget_ignore_key_fct != NULL)
                         {
                         
                           CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
                            _pf_widget_ignore_key_fct(h_thread,ui4_msg,(UINT32)pv_param1);
                            
                            CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
                         }                       
                        pt_app_info->b_msg_not_processed_by_widget = FALSE;
                        pt_app_info->ui4_msg = 0;
                        pt_app_info->pv_param1 = NULL;
                        pt_app_info->pv_param2 = NULL;
                    }
                 CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
            }else if (WGL_FAILED(i4_ret))
            {
                if(i4_ret == WGLR_KEY_MSG_NOT_PROCESSED &&
                   _pf_widget_ignore_key_fct != NULL)
                {
                    _pf_widget_ignore_key_fct(h_thread,ui4_msg,(UINT32)pv_param1);
                }
                if(i4_ret == WGLR_KEY_MSG_NOT_PROCESSED)
                { 
                    /*WGLR_KEY_MSG_NOT_PROCESSED is for inner judge and need not reture outside*/
                    i4_ret = WGLR_OK;
                }else
                {
                     DBG_ERROR(("{WGL} ERR: Sending message(%d) failed (%d) at L%d\r\n\r\n", ui4_msg, i4_ret, __LINE__));
                }
            }
        }
        else
        {
            i4_ret = x_wgl_set_focus(h_target, WGL_SYNC_AUTO_REPAINT);
            if (WGL_FAILED(i4_ret))
            {
                DBG_ERROR(("{WGL} ERR: Setting focus (%d) failed (%d) at L%d\r\n\r\n", ui4_msg, i4_ret, __LINE__));
            }
        }
    }
#ifdef WGL_ANIM_SUPPORT    
    /* WGL_MSG_ANIMATION_TICK */
    else
    {
        if (b_send_anim_tick_to_widget)
        {
            INT32               i4_res;
            UINT32              ui4_i, ui4_j;
            _CANVAS_T           *pt_canvas = NULL;
            HANDLE_TYPE_T       e_type;
            _ANIM_WDGT_LIST_T   *pt_wdgt_element;
            WGL_WIDGET_ANIM_CONTEXT_T             t_anim;            
            WGL_ANIM_COND_INF_T          e_anim_cond;
            BOOL                  b_exist_anim_running = FALSE;
            
             CHECK_VALID(WGLR_OK == _app_list_lock(), WGLR_LOCK_ERROR);
            _t_anim_mngr.ui4_update_rect_num  = 0;
            _t_anim_mngr.ui4_canvas_num       = 0;
            pt_app_element->b_been_dispatched = TRUE;

            /* for each animated widget in this application, process WGL_MSG_ANIMATION_TICK */
            SLIST_FOR_EACH(pt_wdgt_element, &pt_app_element->t_wdgt_list, t_link)
            {
                pt_widget = NULL;
                i4_res    = handle_get_type_obj(pt_wdgt_element->h_widget, &e_type, (VOID **)&pt_widget);

                if (i4_res == HR_OK && pt_widget != NULL && IS_WIDGET_TYPE(e_type))
                {
                    pt_canvas = CANVAS(BRANCH(pt_widget));

                    if (pt_canvas != NULL)
                    {
                        if(_get_anim_context( pt_wdgt_element->h_widget,pt_wdgt_element->e_anim_attr_type,&t_anim)== WGLR_OK)
                        {
                         
                            if(wgl_anim_cmd_proc(t_anim, WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&e_anim_cond),NULL)==WGLR_OK &&
                               e_anim_cond.ui2_end_type != WGL_ANIM_TYPE_ALL)
                            {
                                if(!b_exist_anim_running)
                                {
                                   b_exist_anim_running = TRUE;
                                }
                                _handle_anim_tick_message(pt_wdgt_element->h_widget,pt_wdgt_element->e_anim_attr_type,t_anim_msg_data.ui4_tick);
                        _add_canvas_record(pt_canvas);
                    }
                }
            }
                }
            }

            if(!b_exist_anim_running)
            {
                pt_app_element->b_any_wdgt_anim_running = FALSE;
                CHECK_VALID(WGLR_OK == _app_list_unlock(), WGLR_UNLOCK_ERROR);
                return WGLR_OK;
            }
            

            /* paint all canvas with multiple dirty regions */
            for (ui4_i = 0; ui4_i < _t_anim_mngr.ui4_canvas_num; ui4_i++)
            {
                for (ui4_j = 0; ui4_j < _t_anim_mngr.ui4_update_rect_num; ui4_j++)
                {
                    _paint_canvas(
                        _t_anim_mngr.apt_canvas[ui4_i],
                        &_t_anim_mngr.at_update_rect[ui4_j]);
                }
            }
     
            /* for each animated widget in this application, check its state and make it notify the APP */
            SLIST_FOR_EACH(pt_wdgt_element, &pt_app_element->t_wdgt_list, t_link)
            {
                pt_widget = NULL;
                i4_res    = handle_get_type_obj(pt_wdgt_element->h_widget, &e_type, (VOID **)&pt_widget);

                if (i4_res == HR_OK && pt_widget != NULL && IS_WIDGET_TYPE(e_type))
                {
                   if(_get_anim_context(pt_wdgt_element->h_widget,pt_wdgt_element->e_anim_attr_type,&t_anim) == WGLR_OK)
                    {
                        if(wgl_anim_cmd_proc(t_anim, WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&e_anim_cond), NULL)==WGLR_OK)
                        {
                            if(e_anim_cond.ui2_end_type == WGL_ANIM_TYPE_ALL)
                            {
                            
                                _add_paint_rect_for_scale(pt_widget,pt_wdgt_element->e_anim_attr_type);
                            }
                            
                            /* send animation condition nitification */
                            if(pt_widget->t_widget_anim_cb.pf_anim_callback != NULL &&
                               e_anim_cond.ui2_end_type != WGL_ANIM_TYPE_NONE)
                            {
                                (*pt_widget->t_widget_anim_cb.pf_anim_callback)(pt_wdgt_element->h_widget,pt_wdgt_element->e_anim_attr_type,e_anim_cond,pt_widget->t_widget_anim_cb.pv_tag);
                            }
                        }
                    }
                }
            }
            CHECK_VALID(WGLR_OK == _app_list_unlock(), WGLR_UNLOCK_ERROR);
        }
    }
#endif
    return (i4_ret);
        
_DISPATCH_EVENT_RET:

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}



/*-----------------------------------------------------------------------------
 * Name: x_wgl_do_cmd
 *
 * Description: This API is used to change or obtain the states of the 
 *              specified widget.
 *
 * Inputs:  h_widget    Contains the handle of the specified widget.
 *          ui4_cmd     Contains the command id. This is detailed in the 
 *                      individual widget, content or border design spec.
 *          pv_param1   References the data relevant to the specified command.
 *          pv_param2   References the data relevant to the specified command.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_widget contains an invalid 
 *                              widget handle.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_do_cmd(
    HANDLE_T            h_widget,
    UINT32              ui4_cmd, 
    VOID*               pv_param1,
    VOID*               pv_param2)
{
    WGL_PACKED_PARAM_T  t_pparam;
    INT32               i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget  = %d\r\n"
         "\tui4_cmd   = %d\r\n"
         "\tpv_param1 = 0x%.8x\r\n"
         "\tpv_param2 = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_widget,
         ui4_cmd,
         pv_param1,
         pv_param2));
    
    t_pparam.pv_param1 = pv_param1;
    t_pparam.pv_param2 = pv_param2;

    i4_ret = x_wgl_send_msg(h_widget, WGL_MSG_COMMAND, (VOID*) ui4_cmd, &t_pparam);
    if (WGL_FAILED(i4_ret))
    {
        DBG_ERROR(("{WGL} ERR: Sending WGL_MSG_COMMAND failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_enable
 *
 * Description: This API enables/disables h_widget. A disabled widget does not 
 *              get the input focus and may be presented in a different outlook 
 *              from an enabled one. However, the ultimate appearance depends 
 *              on the implementation of the content and border of h_widget. 
 *              In addition, if h_widget is disabled, all its child widgets 
 *              will be disabled implicitly.
 *
 * Inputs:  h_widget    Contains the handle of the specified widge.
 *          b_enable    Contains TRUE for enabling the widget; FALSE for 
 *                      disabling it.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_widget contains an invalid 
 *                              widget handle.
 *          WGLR_INV_STATE      The argument b_enable is FALSE while h_widget 
 *                              is the current focused widget.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_enable(
    HANDLE_T            h_widget,  
    BOOL                b_enable)
{
    _WIDGET_T*      pt_widget;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;

    wgl_widget_proc_fct pf_proc = NULL;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget  = %d\r\n"
         "\tb_enable  = %d\r\n\r\n",
         __FUNCTION__,
         h_widget,
         b_enable));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    || 
        !IS_WIDGET_TYPE(e_type)                                                 ||
        ST_DESTROYED(pt_widget))
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ENABLE_RET;
    }
    
    i4_ret = WGLR_OK;
    
    if (b_enable)
    {
        STATE(pt_widget) |= WS_ENABLED;
        pf_proc = MSGPROC(pt_widget);
    }
    else
    {
        _APP_INFO_T* pt_app_info = OWNER(BRANCH(pt_widget));

        if (pt_app_info->pt_widget_focused == pt_widget)
        {
            i4_ret = WGLR_INV_STATE;
            DBG_ERROR(("{WGL} ERR: Attempt to disable a focused widget (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
        else
        {
            STATE(pt_widget) &= ~WS_ENABLED;
            pf_proc = MSGPROC(pt_widget);
        }
    }
    
_ENABLE_RET:
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    if (pf_proc)
    {
        i4_ret = (*pf_proc)(h_widget, WGL_MSG_ENABLE, (VOID*) (UINT32) b_enable, 0);
        
        if (WGL_FAILED(i4_ret))
        {
            DBG_ERROR(("{WGL} ERR: Sending WGL_MSG_ENABLE failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    
    return (i4_ret);
}

/* change the state of a transparent list */
INT32 x_wgl_enable_transparent_list(
    HANDLE_T            h_rgn_list,             /* in: handle of a transparent list */
    BOOL                b_enabled,              /* in: enable this list or not */
    BOOL                b_exclusive)            /* in: set its state as exclusive or union */
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    _TRANSP_REGION_T    *pt_obj = NULL;
    _CANVAS_T           *pt_canvas = NULL;
    _TRANSP_LIST_T      *pt_list;
    BOOL                b_found = FALSE;

    if (h_rgn_list == NULL_HANDLE)
    {
        return WGLR_INV_ARG;
    }

    i4_ret = handle_get_type_obj(h_rgn_list, &e_type, (VOID **)&pt_obj);

    if (i4_ret != HR_OK || e_type != HT_WGL_TRANP_LIST || pt_obj == NULL)
    {
        DBG_ERROR(("{WGL} ERR: invalid transparent list handle at L%d\r\n\r\n", __LINE__));
        return WGLR_INV_HANDLE;
    }

    i4_ret = handle_get_type_obj(pt_obj->h_canvas, &e_type, (VOID **)&pt_canvas);

    if (i4_ret != HR_OK || e_type != HT_WGL_CANVAS || pt_canvas == NULL)
    {
        DBG_ERROR(("{WGL} ERR: invalid canvas handle at L%d\r\n\r\n", __LINE__));
        return WGLR_INV_HANDLE;
    }
    
    _wgl_lock();

    DLIST_FOR_EACH(pt_list, &pt_canvas->t_transp_list, t_link)
    {
        if (pt_list->h_rgn_list == h_rgn_list)
        {
            b_found = TRUE;
            break;
        }
    }

    if (b_found)
    {
        /* exclusive enabled */
        if (b_exclusive && b_enabled)
        {
            DLIST_FOR_EACH(pt_list, &pt_canvas->t_transp_list, t_link)
            {
                pt_list->b_enalbed = ((pt_list->h_rgn_list == h_rgn_list)? TRUE: FALSE);
            }
        }
        /* others */
        else
        {
            pt_list->b_enalbed = b_enabled;
        }
        
        _wgl_unlock();


        return WGLR_OK;
    }
    else
    {
        _wgl_unlock();

        DBG_ERROR(("{WGL} ERR: no such transparent list in associated canvas at L%d\r\n\r\n", __LINE__));
        return WGLR_INV_HANDLE;
    }
}


INT32 x_wgl_flip_canvas(HANDLE_T h_canvas)
{
    INT32           i4_ret;
    _CANVAS_T*      pt_canvas = NULL;
    HANDLE_TYPE_T   e_type;
    
    CHECK_VALID(h_canvas, WGLR_INV_ARG);
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    i4_ret = handle_get_type_obj(h_canvas, &e_type, (VOID **)&pt_canvas);

    if (i4_ret == HR_OK && HT_WGL_CANVAS == e_type && pt_canvas != NULL)
    {
#ifdef MW_OSD_COMPRESSION_SHINK
        i4_ret = x_gl_plane_flip(pt_canvas->h_plane, pt_canvas->h_view, TRUE);
#else
        i4_ret = x_gl_plane_flip(
        pt_canvas->h_plane,
        (pt_canvas->b_compress_back_running)? pt_canvas->h_view_compress: pt_canvas->h_view,
        TRUE);
#endif        
        if (i4_ret != GLR_OK)
        {
            i4_ret = WGLR_INTERNAL_ERROR;
            DBG_ERROR(("{WGL} ERR: flip canvas error (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}

INT32 x_wgl_float(
    HANDLE_T            h_widget,
    BOOL                b_float,
    UINT8               ui1_repaint)
{
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget    = %d\r\n"
         "\tb_float     = %d\r\n"
         "\tui1_repaint = %d\r\n\r\n",
         __FUNCTION__,
         h_widget,
         b_float,
         ui1_repaint));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    i4_ret = WGLR_INV_HANDLE;
    
    if (HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget) && 
        IS_WIDGET_TYPE(e_type)                                               &&
        !ST_DESTROYED(pt_widget))
    {
        _set_float(pt_widget, b_float);

        i4_ret = WGLR_OK;
    }

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    if ((WGLR_OK == i4_ret) && (ui1_repaint != WGL_NO_AUTO_REPAINT))
    {
        i4_ret = x_wgl_repaint(h_widget, NULL, (ui1_repaint == WGL_SYNC_AUTO_REPAINT));
    }
    
    return (i4_ret);
}
INT32 x_wgl_get_app_order(HANDLE_T h_thread, UINT8 *pui1_order)
{
    INT32           i4_ret;
    _APP_INFO_T*    pt_app_info = NULL;
    
    CHECK_VALID(h_thread != NULL_HANDLE && pui1_order != NULL, WGLR_INV_ARG);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    pt_app_info = _find_app_info(h_thread);
    
    i4_ret = WGLR_INV_CONTEXT;
    
    if (pt_app_info)
    {
        *pui1_order = WGL_APP_NOT_TO_END;
        
        if (pt_app_info->ui2_state & AIS_TOPMOST)
        {
            *pui1_order = WGL_APP_TOPMOST;
        }
        else if (pt_app_info->ui2_state & AIS_BOTTOMMOST)
        {
            *pui1_order = WGL_APP_BOTTOMMOST;
        }
        
        i4_ret = WGLR_OK;
    }
    else
    {
        DBG_ERROR(("{WGL} ERR: Cannot find associated client info (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name: x_wgl_get_canvas
 *
 * Description: This API retrieves the handle of the canvas (a widget plane) 
 *              associated with the specified OSD plane represented by h_plane.
 *
 * Inputs:  h_plane     Contains the handle of the specified OSD plane.
 *
 * Outputs: ph_canvas   References the handle of the associated canvas if this 
 *                      routine was successful. In all other cases, this field 
 *                      is undetermined.
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_plane contains an invalid OSD plane handle.
 *          WGLR_INV_ARG        The argument ph_canvas is NULL.
 *          WGLR_OUT_OF_MEMORY  There is not enough memory to complete the 
 *                              operation.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_get_canvas(
    GL_HPLANE_T         h_plane,
    HANDLE_T*           ph_canvas)
{
    _CANVAS_T*  pt_canvas;
    INT32       i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_plane   = %d\r\n"
         "\tph_canvas = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_plane,
         ph_canvas));
    
    CHECK_VALID(ph_canvas, WGLR_INV_ARG);
    CHECK_VALID(gl_is_valid_obj(h_plane, GLT_OBJ_PLANE), WGLR_INV_HANDLE);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    i4_ret = WGLR_OK;
    
    pt_canvas = _find_canvas(h_plane);
    if (!pt_canvas)
    {
        pt_canvas = _new_canvas(h_plane, NULL, NULL);
        if (!pt_canvas)
        {
            i4_ret = WGLR_OUT_OF_MEMORY;
            DBG_ERROR(("{WGL} ERR: Out of memory (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _GET_CANVAS_RET;
        }
    }
    *ph_canvas = pt_canvas->h_this;

_GET_CANVAS_RET:
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}


INT32 x_wgl_get_canvas_ex(
    GL_HPLANE_T             h_plane,
    const WGL_CANVAS_CFG_T* pt_cfg,
    HANDLE_T*               ph_canvas)
{
    _CANVAS_T*  pt_ref_canvas;
    INT32       i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_plane   = %d\r\n"
         "\tpt_cfg    = 0x%x\r\n",
         "\tph_canvas = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_plane,
         pt_cfg,
         ph_canvas));
    
    CHECK_VALID(ph_canvas, WGLR_INV_ARG);
    CHECK_VALID(gl_is_valid_obj(h_plane, GLT_OBJ_PLANE), WGLR_INV_HANDLE);
    CHECK_VALID(_canvas_cfg_is_valid(pt_cfg), WGLR_INV_ARG);
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    pt_ref_canvas = _find_canvas(h_plane);
    
    if ((!pt_ref_canvas) || (!SHADOW(pt_ref_canvas)))
    {
        _CANVAS_T* pt_canvas;
        
        pt_canvas = _new_canvas(h_plane, pt_cfg, pt_ref_canvas);
        
        if (pt_canvas)
        {
            i4_ret = WGLR_OK;
            
            if (pt_ref_canvas)
            {
                SHADOW(pt_ref_canvas) = pt_canvas;
            }
            
            *ph_canvas = pt_canvas->h_this;
        }
        else
        {
            i4_ret = WGLR_OUT_OF_MEMORY;
            DBG_ERROR(("{WGL} ERR: Out of memory (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    else
    {
        i4_ret = WGLR_SHADOW_EXIST;
        DBG_ERROR(("{WGL} ERR: Shadow canvas already created (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
        
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}

INT32 x_wgl_get_canvas_info(
    HANDLE_T            h_obj,
    WGL_CANVAS_INFO_T*  pt_cvs_info)
{
    INT32           i4_ret;
    VOID*           pv_obj;
    HANDLE_TYPE_T   e_type;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_obj       = %d\r\n"
         "\tpt_cvs_info = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_obj,
         pt_cvs_info));
    
    CHECK_VALID(pt_cvs_info, WGLR_INV_ARG);
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    i4_ret = WGLR_OK;

    pt_cvs_info->h_canvas = NULL_HANDLE;

    if (NULL_HANDLE == h_obj)
    {
        _get_canvas_info(NULL, pt_cvs_info);
    }
    else if (HR_OK == handle_get_type_obj(h_obj, &e_type, &pv_obj))
    {
        if ((HT_WGL_CANVAS == e_type) || IS_WIDGET_TYPE(e_type))
        {
            _CANVAS_T* pt_canvas = 
                (HT_WGL_CANVAS == e_type) ? (_CANVAS_T*) pv_obj : CANVAS(BRANCH(((_WIDGET_T*) pv_obj)));

            _get_canvas_info(pt_canvas, pt_cvs_info);
        }
        else /* h_obj is a widget handle */
        {
            i4_ret = WGLR_INV_HANDLE;
            DBG_ERROR(("{WGL} ERR: Invalid handle type (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}

INT32 x_wgl_get_canvas_palette(
    HANDLE_T        h_obj,
    GL_HPALETTE_T*  ph_palette)
{
    INT32           i4_ret;
    VOID*           pv_obj;
    HANDLE_TYPE_T   e_type;
    GL_HPALETTE_T   h_pal, h_dup_pal;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_obj      = %d\r\n"
         "\tph_palette = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_obj,
         ph_palette));
    
    CHECK_VALID(ph_palette, WGLR_INV_ARG);
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    i4_ret = WGLR_OK;

    h_pal = NULL_HANDLE;
    
    if (NULL_HANDLE == h_obj)
    {
        h_pal = _h_sys_pal;
    }
    else if (HR_OK == handle_get_type_obj(h_obj, &e_type, &pv_obj))
    {
        if ((HT_WGL_CANVAS == e_type) || IS_WIDGET_TYPE(e_type))
        {
            _CANVAS_T* pt_canvas = 
                (HT_WGL_CANVAS == e_type) ? (_CANVAS_T*) pv_obj : CANVAS(BRANCH(((_WIDGET_T*) pv_obj)));

            h_pal = pt_canvas->h_palette;
        }
    }
    
    if (NULL_HANDLE != h_pal)
    {
        h_dup_pal = gl_palette_duplicate_handle(h_pal);

        if (NULL_HANDLE != h_dup_pal)
        {
            *ph_palette = h_dup_pal;
        }
        else
        {
            i4_ret = WGLR_OUT_OF_HANDLES;
            DBG_ERROR(("{WGL} ERR: Out of handles (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_get_canvas_size
 *
 * Description: This API retrieves the dimension of the specified canvas. Note 
 *              that the size returned is actually the size of the internal 
 *              frame buffer.
 *
 * Inputs:  h_canvas        Contains the handle of the specified canvas.
 *          pui4_width      References the width of the internal frame buffer 
 *                          if this routine was successful. In all other cases, 
 *                          this field is undetermined.
 *          pui4_height     References the height of the internal frame buffer 
 *                          if this routine was successful. In all other cases, 
 *                          this field is undetermined.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_plane contains an invalid OSD 
 *                              plane handle.
 *          WGLR_INV_ARG        The argument ph_canvas is NULL.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_get_canvas_size(
    HANDLE_T    h_canvas,
    UINT32*     pui4_width,
    UINT32*     pui4_height)
{
    _CANVAS_T*      pt_canvas;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_canvas    = %d\r\n"
         "\tpui4_width  = 0x%.8x\r\n"
         "\tpui4_height = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_canvas,
         pui4_width,
         pui4_height));
    
    CHECK_VALID(pui4_width && pui4_height, WGLR_INV_ARG);
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if (HR_OK == handle_get_type_obj(h_canvas, &e_type, (VOID**) &pt_canvas) &&
        HT_WGL_CANVAS == e_type)
    {
        *pui4_width  = pt_canvas->ui4_width;
        *pui4_height = pt_canvas->ui4_height;
        i4_ret       = WGLR_OK;
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}
INT32 x_wgl_get_canvas_plane(
    HANDLE_T       h_canvas,
    GL_HPLANE_T*   ph_plane)
{
    _CANVAS_T*      pt_canvas;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;
    
    CHECK_VALID(ph_plane, WGLR_INV_ARG);
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if (HR_OK == handle_get_type_obj(h_canvas, &e_type, (VOID**) &pt_canvas) &&
        HT_WGL_CANVAS == e_type)
    {
        *ph_plane  = pt_canvas->h_plane;
        i4_ret       = WGLR_OK;
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}
INT32 x_wgl_destroy_canvas(
    HANDLE_T    h_canvas)
{
    _CANVAS_T*      pt_canvas;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret = WGLR_OK;    
    BOOL b_free = FALSE;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_canvas    = %d\r\n",
         __FUNCTION__,
         h_canvas));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if (HR_OK == handle_get_type_obj(h_canvas, &e_type, (VOID**) &pt_canvas) &&
        HT_WGL_CANVAS == e_type)
    {
    
        if( IS_REFERENCE(pt_canvas))
        {
            i4_ret = WGLR_CANVAS_HAS_SHADOW;
            DBG_ERROR(("{WGL} ERR: Shadow has not been destroyed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
        else
        {
            if(DLIST_IS_EMPTY(&(pt_canvas->t_branch_lst)))/*no any APP is linked to this canvas*/
            {
                 b_free = TRUE;
            }
            else
            {
                i4_ret = WGLR_CANVAS_IS_USEING;
                DBG_ERROR(("{WGL} ERR: Some APP has still linked to this canvas (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
       }
        
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    if(b_free)
    {
        i4_ret = (HR_OK == handle_free(h_canvas, FALSE) ? WGLR_OK : WGLR_INTERNAL_ERROR);
         if (WGL_FAILED(i4_ret))
         {
             DBG_ERROR(("{WGL} ERR: Freeing canvas handle failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
         }
    }
    return (i4_ret);
}
INT32 x_wgl_canvas_reconfig(
    HANDLE_T                h_canvas,
    GL_HPLANE_T             h_plane,
    const WGL_CANVAS_CFG_T* pt_cfg)
{
    INT32       i4_ret;
    HANDLE_TYPE_T   e_type;
    _CANVAS_T* pt_canvas = NULL;
    WGL_CVS_CLR_CFG_T   t_cvs_cfg;
    
    CHECK_VALID(h_canvas, WGLR_INV_ARG);
    CHECK_VALID(gl_is_valid_obj(h_plane, GLT_OBJ_PLANE), WGLR_INV_HANDLE);
    CHECK_VALID(_canvas_cfg_is_valid(pt_cfg), WGLR_INV_ARG);
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    if (HR_OK == handle_get_type_obj(h_canvas, &e_type, (VOID**) &pt_canvas) &&
           HT_WGL_CANVAS == e_type)
       {
       
           if( !IS_SINGLE(pt_canvas))
           {
               i4_ret = WGLR_INTERNAL_ERROR;
               DBG_ERROR(("{WGL} ERR: only single canvas can reconfig (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
           }
           else
           {
                _free_canvas_resource(pt_canvas);
                i4_ret = _new_canvas_resource(pt_canvas,h_plane,pt_cfg,NULL);
                if(WGLR_OK != i4_ret)
                {
                   _free_canvas_resource(pt_canvas);
                   DBG_ERROR(("{WGL} ERR: create canvas resource failed(%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }
    t_cvs_cfg.ui4_flag  = WGL_CANVAS_CFG_COLORKEY;
		t_cvs_cfg.t_colorkey = pt_canvas->t_color_key;
		t_cvs_cfg.ui4_flag |= WGL_CANVAS_CFG_PALETTE;
		t_cvs_cfg.h_palette = pt_canvas->h_palette;
		t_cvs_cfg.ui4_flag |= WGL_CANVAS_CFG_COLORMODE;
		t_cvs_cfg.e_clr_mode = pt_canvas->e_color_mode;
		_broadcast_widget_msg(pt_canvas, WGL_MSG_CHANGE_COLOR_CFG, (VOID*) &t_cvs_cfg, NULL);
           }
           
       }
       else
       {
           i4_ret = WGLR_INV_HANDLE;
           DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
       }
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}


INT32 x_wgl_get_canvas_config(HANDLE_T h_canvas,
    							   WGL_CANVAS_CFG_T* pt_cfg)
{
	INT32		i4_ret = WGLR_OK;
	HANDLE_TYPE_T	e_type;
	_CANVAS_T* pt_canvas = NULL;
	
	CHECK_VALID(h_canvas, WGLR_INV_ARG);
	CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
	if (HR_OK == handle_get_type_obj(h_canvas, &e_type, (VOID**) &pt_canvas) &&
		   HT_WGL_CANVAS == e_type)
	   {
	   	   pt_cfg->ui4_flag = WGL_CANVAS_CFG_AUTOSCALE;
	   	   pt_cfg->b_auto_scale = pt_canvas->b_auto_scale;

		   pt_cfg->ui4_flag |= WGL_CANVAS_CFG_FORCE_NO_INTER_COMPOSE;
	   	   pt_cfg->b_force_no_inter_compose = (pt_canvas->h_surf_render == NULL_HANDLE && 
										   		(_b_inter_compose) &&
								              (pt_canvas->e_color_mode == GL_COLORMODE_ARGB_D8888 ||
								               pt_canvas->e_color_mode == GL_COLORMODE_ARGB_D4444 ||
								               pt_canvas->e_color_mode == GL_COLORMODE_ARGB_D1555)) ? FALSE : TRUE;
		   
		   pt_cfg->ui4_flag |= WGL_CANVAS_CFG_COLORMODE;
		   pt_cfg->e_clr_mode = pt_canvas->e_color_mode;
		   
		   if (pt_canvas->h_palette != NULL_HANDLE)
		   {
		   		pt_cfg->ui4_flag |= WGL_CANVAS_CFG_PALETTE;
		   		pt_cfg->h_palette  = gl_palette_duplicate_handle(pt_canvas->h_palette);
		   }
		   else
		   {
		   		pt_cfg->h_palette  =  NULL_HANDLE;
		   }
		   
		   pt_cfg->ui4_flag  |= WGL_CANVAS_CFG_WIDTH;
		   pt_cfg->ui4_width = pt_canvas->ui4_width;

		   pt_cfg->ui4_flag  |= WGL_CANVAS_CFG_HEIGHT;
		   pt_cfg->ui4_height = pt_canvas->ui4_height;

		   pt_cfg->ui4_flag  |= WGL_CANVAS_CFG_COLORKEY;
		   pt_cfg->t_colorkey = pt_canvas->t_color_key;
		   
	   }
	   else
	   {
		   i4_ret = WGLR_INV_HANDLE;
		   DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
	   }
	CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

	return (i4_ret);
}


/*------------------------------------------------------------------*/
/*! @brief  query the children order of a parent
 *  @param  [in]        h_h_parent  - parent widget handle
 *  @param  [out]       ah_children - array to store the children handle in order
 *  @param  [in/out]    pi4_number  - [in] specify the array size [out] children number
 *  @note
 *  @see
 *  @return return the error code
 *  @retval WGLR_OK                 - ok
 *  @retval WGLR_INV_HANDLE         - an invalid handle is passed
 *  @retval WGLR_INV_ARG            - invalid argument is passed
 *  @retval WGLR_BUFFER_NOT_ENOUGH  - children number is larger than the array size
 *  @retval WGLR_LOCK_ERROR         - WGL lock error
 *  @retval WGLR_UNLOCK_ERROR       - WGL unlock error
 */
/*------------------------------------------------------------------*/
INT32 x_wgl_get_children_order(HANDLE_T h_parent, HANDLE_T ah_children[], INT32 *pi4_number)
{
    _WIDGET_T                   *pt_widget, *pt_child;
    HANDLE_TYPE_T               e_type;
    INT32                       i4_ret = WGLR_OK;

    CHECK_VALID(h_parent != NULL_HANDLE && ah_children != NULL && pi4_number != NULL, WGLR_INV_ARG);
    
    _hrc_lock();
    
    /* get handle type of h_container */
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_parent, &e_type, (VOID **)&pt_widget))
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid parent handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ERROR_RET;
    }
    
    if (IS_WIDGET_TYPE(e_type) && !ST_DESTROYED(pt_widget))
    {
        INT32       i4_size = 0;

        /* for each child */
        for (pt_child = CHILD(pt_widget); pt_child != NULL; pt_child = SIBLING(pt_child))
        {
            /* count children number */
            i4_size = i4_size + 1;

            if (i4_size > *pi4_number)
            {
                i4_ret = WGLR_BUFFER_NOT_ENOUGH;
                DBG_ERROR(("{WGL} ERR: children number is larger than the specified array (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                goto _ERROR_RET;
            }

            /* fill child handle */
            ah_children[i4_size - 1] = pt_child->h_this;
        }

        *pi4_number = i4_size;
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid parent type (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ERROR_RET;
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    _hrc_unlock();
    
    return WGLR_OK;

_ERROR_RET:
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    _hrc_unlock();
    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_get_coords
 *
 * Description: This API retrieves the coordinates of the bounding rectangle of 
 *              the specified widget.
 *
 * Inputs:  h_widget    Contains the handle of the specified widget.
 *          e_coords    Contains an enumerated value to indicate whether the 
 *                      coordinates should be in screen coordinates or in the 
 *                      local coordinates of the parent of h_widget.
 *
 * Outputs: pt_rect     References a GL_RECT_T structure which contains the 
 *                      coordinates of the bounding rectangle of h_widget if 
 *                      this routine was successful. In all other cases, this 
 *                      field is undetermined.
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_widget contains an invalid 
 *                              widget handle.
 *          WGLR_INV_ARG        Either the argument e_coords contains an 
 *                              invalid enumerated value or pt_rect is NULL.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
#ifdef WGL_COORDINATE_INVERSE    
INT32 x_wgl_get_coords_for_inverse(
    HANDLE_T            h_widget,
    WGL_COORDS_T        e_coords,
    GL_RECT_T*          pt_rect)
{
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget;
    INT32           i4_x = 0, i4_y = 0;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget = %d\r\n"
         "\te_coords = %d\r\n"
         "\tpt_rect  = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_widget,
         e_coords,
         pt_rect));
    
    CHECK_VALID(pt_rect, WGLR_INV_ARG);

    CHECK_VALID(
        e_coords == WGL_COORDS_LOCAL || e_coords == WGL_COORDS_PARENT || e_coords == WGL_COORDS_SCREEN,
        WGLR_INV_ARG);
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    || 
        !IS_WIDGET_TYPE(e_type)                                                 ||
        ST_DESTROYED(pt_widget))
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _GET_COORDS_RET;
    }

    if (e_coords == WGL_COORDS_LOCAL)
    {
        SET_RECT(pt_rect, 0, 0, RECT_W(&(pt_widget->t_rect)), RECT_H(&(pt_widget->t_rect)));
    }
    else
    {
        *pt_rect = pt_widget->t_rect;
        pt_rect->i4_left -= pt_widget->i4_x_pos_reverse;
        pt_rect->i4_right -= pt_widget->i4_x_pos_reverse;
        
        if (e_coords == WGL_COORDS_SCREEN)
        {
            _WIDGET_T*      pt_parent = PARENT(pt_widget);
            while (pt_parent)
            {
                i4_x     += pt_parent->t_rect.i4_left-pt_parent->i4_x_pos_reverse;
                i4_y     += pt_parent->t_rect.i4_top-pt_parent->i4_x_pos_reverse;
                pt_parent = PARENT(pt_parent);
            }
            MOVE_RECT(pt_rect, i4_x, i4_y);
        }
    }
    
    i4_ret = WGLR_OK;
    
_GET_COORDS_RET:
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    return (i4_ret);
}
#endif
INT32 x_wgl_get_coords(
    HANDLE_T            h_widget,
    WGL_COORDS_T        e_coords,
    GL_RECT_T*          pt_rect)
{
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget;
    INT32           i4_x, i4_y;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget = %d\r\n"
         "\te_coords = %d\r\n"
         "\tpt_rect  = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_widget,
         e_coords,
         pt_rect));
    
    CHECK_VALID(pt_rect, WGLR_INV_ARG);

    CHECK_VALID(
        e_coords == WGL_COORDS_LOCAL || e_coords == WGL_COORDS_PARENT || e_coords == WGL_COORDS_SCREEN,
        WGLR_INV_ARG);
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    || 
        !IS_WIDGET_TYPE(e_type)                                                 ||
        ST_DESTROYED(pt_widget))
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _GET_COORDS_RET;
    }

    if (e_coords == WGL_COORDS_LOCAL)
    {
        SET_RECT(pt_rect, 0, 0, RECT_W(&(pt_widget->t_rect)), RECT_H(&(pt_widget->t_rect)));
    }
    else
    {
        *pt_rect = pt_widget->t_rect;
        
        if (e_coords == WGL_COORDS_SCREEN)
        {
            _cal_translation(PARENT(pt_widget), &i4_x, &i4_y);
            MOVE_RECT(pt_rect, i4_x, i4_y);
        }
    }
    
    i4_ret = WGLR_OK;
    
_GET_COORDS_RET:
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    return (i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name: x_wgl_get_focus
 *
 * Description: This API retrieves the current focused widget of the calling 
 *              thread.
 *
 * Inputs:  -
 *
 * Outputs: ph_widget   References the handle of the current focused widget if 
 *                      this routine was successful. In all other cases, this 
 *                      field is undetermined.
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_ARG        The argument ph_widget is NULL.
 *          WGLR_INV_CONTEXT    No widgets are ever created in the calling 
 *                              thread.
 *          WGLR_OS_ERROR       This API is not called from a thread context.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 *          WGLR_NO_FOCUSED_WIDGET  There's no focused widget of the calling 
 *                                  thread.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_get_focus(HANDLE_T* ph_widget)
{
    HANDLE_T        h_thread;
    _APP_INFO_T*    pt_app_info;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\tph_widget = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         ph_widget));
    
    CHECK_VALID(ph_widget, WGLR_INV_ARG);

    CHECK_VALID(OSR_OK == x_thread_self(&h_thread), WGLR_OS_ERROR);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    /* find the associated _APP_INFO_T structure */
    pt_app_info = _find_app_info(h_thread);

    if (pt_app_info)
    {
        if (FOCUSED(pt_app_info))
        {
            *ph_widget = FOCUSED(pt_app_info)->h_this;
            i4_ret = WGLR_OK;
        }
        else
        {
            i4_ret = WGLR_NO_FOCUSED_WIDGET;
            DBG_ERROR(("{WGL} ERR: Cannot find focused widget (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    else
    {
        i4_ret = WGLR_INV_CONTEXT;
        DBG_ERROR(("{WGL} ERR: Cannot find associated client info (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_get_parent
 *
 * Description: This API retrieves the handle of the parent of the specified 
 *              widget, h_widget. If h_widget is a top-level widget, 
 *              NULL_HANDLE is returned.
 *
 * Inputs:  h_widget    Contains the handle of the specified widget.
 *
 * Outputs: ph_parent   References the parent handle of h_widget if this 
 *                      routine was successful. In all other cases, this field 
 *                      is undetermined.
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_widget contains an invalid 
 *                              widget handle.
 *          WGLR_INV_ARG        The argument ph_parent is NULL.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_get_parent(
    HANDLE_T            h_widget,
    HANDLE_T*           ph_parent)
{
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget  = %d\r\n"
         "\tph_parent = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_widget,
         ph_parent));
    
    CHECK_VALID(ph_parent, WGLR_INV_ARG);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if (HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget) && 
        IS_WIDGET_TYPE(e_type)                                               &&
        !ST_DESTROYED(pt_widget))
    {
        *ph_parent = AT_TOP_LEVEL(pt_widget) ? NULL_HANDLE : PARENT(pt_widget)->h_this;
        i4_ret = WGLR_OK;
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    return (i4_ret);
}

INT32 x_wgl_get_state(
    HANDLE_T        h_widget,
    UINT32*         pui4_state)
{
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget   = %d\r\n"
         "\tpui4_state = 0x%.8x\r\n",
         __FUNCTION__,
         h_widget,
         pui4_state));
    
    CHECK_VALID(pui4_state, WGLR_INV_ARG);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if (HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget) && 
        IS_WIDGET_TYPE(e_type)                                               &&
        !ST_DESTROYED(pt_widget))
    {
        *pui4_state = STATE(pt_widget) & (WS_ENABLED | WS_VISIBLE | WS_ACTIVATED | WS_FOCUSED);
        
        i4_ret = WGLR_OK;
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_get_update_region
 *
 * Description: This API retrieves the invalid region that needs to be updated. 
 *              Note that the region is specified in the local coordinates of 
 *              h_widget.
 *
 * Inputs:  h_widget        Contains the handle of the specified widget.
 *
 * Outputs: pui4_num_rect   References the number of GL_RECT_T entries 
 *                          referenced by pt_upt_rect if this routine was 
 *                          successful. In all other cases, this field is 
 *                          undetermined.
 *          pt_upt_rect     This is an input/output argument. If this is NULL, 
 *                          pui4_num_rect is set with the number of invalid 
 *                          rectangles, which can be used to calculate the 
 *                          appropriate memory size to hold the rectangle 
 *                          structure. If this is not NULL, it references an 
 *                          array of GL_RECT_T which contains the list of 
 *                          invalid rectangles of the widget if this routine 
 *                          was successful. In all other cases, this field is 
 *                          undetermined.
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_widget contains an invalid 
 *                              widget handle.
 *          WGLR_INV_ARG        The argument pui4_num_rect is NULL.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_get_update_region(
    HANDLE_T        h_widget,
    UINT32*         pui4_num_rect,
    GL_RECT_T*      pt_upt_rect)
{
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget      = %d\r\n"
         "\tpui4_num_rect = 0x%.8x\r\n"
         "\tpt_upt_rect   = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_widget,
         pui4_num_rect,
         pt_upt_rect));
    
    CHECK_VALID(pui4_num_rect, WGLR_INV_ARG);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if (HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget) && 
        IS_WIDGET_TYPE(e_type)                                               &&
        !ST_DESTROYED(pt_widget))
    {
        if (pt_upt_rect)
        {
            pt_upt_rect[0] = DIRTY(pt_widget);
        }

        *pui4_num_rect = 1;
        
        i4_ret = WGLR_OK;
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}

INT32 x_wgl_get_visibility(
    HANDLE_T        h_widget,
    BOOL            *pb_visible)
{
    _WIDGET_T*      pt_widget;
    HANDLE_TYPE_T   e_type;

    if (pb_visible == NULL)
    {
        return WGLR_INV_ARG;
    }
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    ||
        !IS_WIDGET_TYPE(e_type)                                                 ||
        ST_DESTROYED(pt_widget))
    {
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", WGLR_INV_HANDLE, __LINE__));
        CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
        return WGLR_INV_HANDLE;
    }

    *pb_visible = _get_visibility(pt_widget);
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return WGLR_OK;
}

INT32 x_wgl_get_widget_proc(
    HANDLE_T                h_widget,
    wgl_widget_proc_fct     *ppf_proc)
{
    INT32           i4_ret;
    _WIDGET_T       *pt_widget = NULL;
    HANDLE_TYPE_T   e_type;

    if (ppf_proc == NULL)
    {
        return WGLR_INV_ARG;
    }

    i4_ret = handle_get_type_obj(h_widget, &e_type, (VOID *)&pt_widget);

    if (i4_ret == HR_OK && IS_WIDGET_TYPE(e_type) && pt_widget != NULL)
    {
        *ppf_proc = pt_widget->pf_wdgt_proc;
    }

    return WGLR_INV_HANDLE;
}

INT32 x_wgl_ignore_visibility_detect(
    HANDLE_T    h_widget,
    BOOL        b_ignore)        
{
    _WIDGET_T*      pt_widget;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget = %d\r\n"
         "\tb_ignore = %d\r\n\r\n",
         __FUNCTION__,
         h_widget,
         b_ignore));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    || 
        !IS_WIDGET_TYPE(e_type)                                                 ||
        ST_DESTROYED(pt_widget))
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    else
    {
        if (b_ignore)
        {
            STATE(pt_widget) |= WS_IGNORED;
        }
        else
        {
            STATE(pt_widget) &= ~WS_IGNORED;
        }
        
        i4_ret = WGLR_OK;
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_insert
 *
 * Description: This API inserts the specified widget before/after the 
 *              reference widget along the z-axis. Note that if h_ref_widget is 
 *              NULL_HANDLE, h_widget is placed at the top/bottom-most level.
 *
 * Inputs:  h_widget        Contains the widget handle.
 *          h_ref_widget    Contains the handle of the reference widget. Note 
 *                          that this argument can be NULL_HANDLE.
 *          e_insert        Contains an enumerated value to specify inserting 
 *                          h_widget before or after h_ref_widget.
 *          ui1_repaint     Contains a flag indicating if 
 *                          synchronous/asynchronous repainting should be 
 *                          performed automatically after the insertion is 
 *                          done.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_widget or h_ref_widget contains 
 *                              an invalid widget handle.
 *          WGLR_INV_ARG        The argument e_insert is invalid.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_insert(
    HANDLE_T            h_widget,
    HANDLE_T            h_ref_widget,
    WGL_INSERT_MODE_T   e_insert,
    UINT8               ui1_repaint)
{
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget;
    _WIDGET_T*      pt_ref_widget;
    BOOL            b_repaint_ref = FALSE;
    BOOL            b_repaint_wgt = FALSE;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget     = %d\r\n"
         "\th_ref_widget = %d\r\n"
         "\te_insert     = %d\r\n"
         "\tui1_repaint  = %d\r\n\r\n",
         __FUNCTION__,
         h_widget,
         h_ref_widget,
         e_insert,
         ui1_repaint));
    
    CHECK_VALID(
        (h_widget != h_ref_widget) &&
        (WGL_INSERT_BEFORE     == e_insert || 
         WGL_INSERT_AFTER      == e_insert || 
         WGL_INSERT_TOPMOST    == e_insert || 
         WGL_INSERT_BOTTOMMOST == e_insert), 
        WGLR_INV_ARG);

    _hrc_lock();
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    i4_ret = WGLR_INV_HANDLE;
    
    if (HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget) && 
        IS_WIDGET_TYPE(e_type)                                               &&
        !ST_DESTROYED(pt_widget))
    {
        if (NULL_HANDLE != h_ref_widget)
        {
            if (HR_OK == handle_get_type_obj(h_ref_widget, &e_type, (VOID**) &pt_ref_widget) && 
                IS_WIDGET_TYPE(e_type)                                                       &&
                !ST_DESTROYED(pt_ref_widget)                                                 &&
                BRANCH(pt_widget) == BRANCH(pt_ref_widget)                                   &&
                PARENT(pt_widget) == PARENT(pt_ref_widget))
            {
                if (WGL_INSERT_BEFORE == e_insert)
                {
                    if (SIBLING(pt_ref_widget) != pt_widget)
                    {
                        /* remove pt_widget */
                        _remove_from_list(pt_widget);
                        
                        /* re-insert pt_widget */
                        SIBLING(pt_widget)     = SIBLING(pt_ref_widget);
                        SIBLING(pt_ref_widget) = pt_widget;

                        b_repaint_ref = ST_VISIBLE(pt_ref_widget);
                        b_repaint_wgt = ST_VISIBLE(pt_widget);
                    }

                    i4_ret = WGLR_OK;
                }
                else if (WGL_INSERT_AFTER == e_insert)
                {
                    if (SIBLING(pt_widget) != pt_ref_widget)
                    {
                        _WIDGET_T* pt_tmp;
                        
                        /* remove pt_widget */
                        _remove_from_list(pt_widget);
                        
                        pt_tmp = (AT_TOP_LEVEL(pt_ref_widget) ? 
                            FIRST(BRANCH(pt_ref_widget)) : SIBLING_HEAD(pt_ref_widget));
                        
                        if (pt_tmp == pt_ref_widget)
                        {
                            if (AT_TOP_LEVEL(pt_ref_widget))
                            {
                                FIRST(BRANCH(pt_ref_widget)) = pt_widget;
                            }
                            else
                            {
                                SIBLING_HEAD(pt_ref_widget) = pt_widget;
                            }
                        }
                        else
                        {
                            FIND_PREV(pt_tmp, pt_ref_widget);
                            SIBLING(pt_tmp) = pt_widget;
                        }
                        SIBLING(pt_widget) = pt_ref_widget;
                        
                        b_repaint_ref = ST_VISIBLE(pt_ref_widget);
                        b_repaint_wgt = ST_VISIBLE(pt_widget);
                    }
                
                    i4_ret = WGLR_OK;
                }
                else /* WGL_INSERT_TOPMOST == e_insert */
                {
                    i4_ret = WGLR_INV_ARG;
                    DBG_ERROR(("{WGL} ERR: Invalid e_insert (%d) value at L%d\r\n\r\n", i4_ret, __LINE__));
                }
            }
            else
            {
                DBG_ERROR(("{WGL} ERR: Invalid reference widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }
        else
        {
            b_repaint_wgt = _insert_widget_no_ref(pt_widget, e_insert);
            i4_ret = WGLR_OK;
        }
    }
    else
    {
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    _hrc_unlock();
    
    /* do automatic repaint if required */
    if (i4_ret == WGLR_OK && ui1_repaint != WGL_NO_AUTO_REPAINT)
    {
        if (WGL_INSERT_BEFORE)
        {
            if (b_repaint_ref)
            {
                i4_ret = x_wgl_repaint(h_ref_widget, NULL, (ui1_repaint == WGL_SYNC_AUTO_REPAINT));
                if (WGL_FAILED(i4_ret))
                {
                    DBG_ERROR(("{WGL} ERR: Repaint h_ref_widget failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }
            }
            if (b_repaint_wgt)
            {
                i4_ret = x_wgl_repaint(h_widget, NULL, (ui1_repaint == WGL_SYNC_AUTO_REPAINT));
                if (WGL_FAILED(i4_ret))
                {
                    DBG_ERROR(("{WGL} ERR: Repaint h_widget failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }
            }
        }
        else
        {
            if (b_repaint_wgt)
            {
                i4_ret = x_wgl_repaint(h_widget, NULL, (ui1_repaint == WGL_SYNC_AUTO_REPAINT));
                if (WGL_FAILED(i4_ret))
                {
                    DBG_ERROR(("{WGL} ERR: Repaint h_widget failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }
            }
            if (b_repaint_ref)
            {
                i4_ret = x_wgl_repaint(h_ref_widget, NULL, (ui1_repaint == WGL_SYNC_AUTO_REPAINT));
                if (WGL_FAILED(i4_ret))
                {
                    DBG_ERROR(("{WGL} ERR: Repaint h_ref_widget failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }
            }
        }
    }
    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_move
 *
 * Description: This API changes the position or size of the specified widget.
 *
 * Inputs:  h_widget    Contains the widget handle.
 *          pt_rect     Reference a GL_RECT_T structure defining the new 
 *                      position and size h_widget. The coordinates are 
 *                      relative to the top-left corner of the parent of 
 *                      h_widget. If h_widget is a top-level widget, this is 
 *                      given in screen coordinates.
 *          ui1_repaint Contains a flag indicating if synchronous/asynchronous 
 *                      repainting should be performed automatically after the 
 *                      move is done.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_widget contains an invalid 
 *                              widget handle.
 *          WGLR_INV_ARG        The argument pt_rect is NULL or pt_rect 
 *                              references an empty rectangle.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
#ifdef WGL_COORDINATE_INVERSE    
INT32 x_wgl_move(
    HANDLE_T            h_widget,
    const GL_RECT_T*    pt_rect,
    UINT8               ui1_repaint)
{
    #define ACT_MOVE            0x1
    #define ACT_RESIZE          0x2
    
    _WIDGET_T*      pt_widget;
    HANDLE_TYPE_T   e_type;
    UINT16          ui2_action = 0;
    GL_RECT_T       t_rc_union;
    _CANVAS_T*      pt_canvas = NULL;
    INT32           i4_ret;
    GL_RECT_T t_widget_rect,t_old_rect;
    wgl_widget_proc_fct pf_proc;
        
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget    = %d\r\n"
         "\tpt_rect     = 0x%.8x\r\n"
         "\tui1_repaint = %d\r\n\r\n",
         __FUNCTION__,
         h_widget,
         pt_rect,
         ui1_repaint));
    
    CHECK_VALID(pt_rect && !RECT_EMPTY(pt_rect), WGLR_INV_ARG);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    ||
        !IS_WIDGET_TYPE(e_type)                                                 ||
        ST_DESTROYED(pt_widget))
    {
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", WGLR_INV_HANDLE, __LINE__));
        CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
        return WGLR_INV_HANDLE;
    }
    t_widget_rect = pt_widget->t_rect;
    t_old_rect = pt_widget->t_rect;
    t_widget_rect.i4_left -= pt_widget->i4_x_pos_reverse;
    t_widget_rect.i4_right-= pt_widget->i4_x_pos_reverse;

    if (RECT_W(&t_widget_rect) != RECT_W(pt_rect) ||
        RECT_H(&t_widget_rect) != RECT_H(pt_rect))
    {
        ui2_action |= ACT_RESIZE;
    }

    if (t_widget_rect.i4_left != pt_rect->i4_left ||
        t_widget_rect.i4_top != pt_rect->i4_top)
    {
        ui2_action |= ACT_MOVE;
    }
    if ( ui2_action != 0)
    {
         pt_widget->t_rect = *pt_rect;
    }
    
    if ( ui2_action != 0  && _b_coordinate_reverse == TRUE && !pt_widget->b_fix_coordinate_dir)
    {
       _WIDGET_T*      pt_parent = PARENT(pt_widget);
       UINT32 ui4_parent_width;
       
       pt_widget->i4_x_pos_reverse = 0;/*reset inverse value,need recalculate again*/
       
       if(pt_parent!=NULL)
       {
            ui4_parent_width = RECT_W(&pt_parent->t_rect);
       }
       else
       {
           pt_canvas = CANVAS(BRANCH(pt_widget));
           ui4_parent_width = pt_canvas->ui4_width;
       }
       _widget_tree_inverse_x_coordinate_pos(pt_widget,ui4_parent_width,_b_coordinate_reverse);
    }
    
    if (ui1_repaint != WGL_NO_AUTO_REPAINT && ui2_action != 0)
    {
        INT32 i4_x, i4_y;
        
        UNION_RECT(&t_rc_union, &(t_old_rect), &(pt_widget->t_rect));
        _cal_translation(PARENT(pt_widget), &i4_x, &i4_y);
        MOVE_RECT(&t_rc_union, i4_x, i4_y);
        pt_canvas = CANVAS(BRANCH(pt_widget));
    }
    
    
    pf_proc = MSGPROC(pt_widget);
    

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    if ((ui2_action & ACT_MOVE)&& pt_widget->b_fix_coordinate_dir)
    {
         
        (*pf_proc)(
            h_widget,
            WGL_MSG_MOVE, 
            (VOID*) COMBINE_HI_LO(pt_widget->t_rect.i4_left, pt_widget->t_rect.i4_top),
            0);
    }

    if (ui2_action & ACT_RESIZE)
    {
        (*pf_proc)(
            h_widget, 
            WGL_MSG_SIZE, 
            (VOID*) COMBINE_HI_LO(RECT_W(pt_rect), RECT_H(pt_rect)),
            0);
    }
    i4_ret = WGLR_OK;

    if (ui2_action != 0)
    {
       _widget_tree_notify_move_msg(pt_widget);

    
        if (ui1_repaint == WGL_SYNC_AUTO_REPAINT)
        {
            i4_ret = _paint_canvas(pt_canvas, &t_rc_union);
            if (WGL_FAILED(i4_ret))
            {
                DBG_ERROR(("{WGL} ERR: Doing sync-repaint failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }
        else if (ui1_repaint == WGL_ASYNC_AUTO_REPAINT)
        {
            i4_ret = x_wgl_repaint(pt_canvas->h_this, &t_rc_union, FALSE);
            if (WGL_FAILED(i4_ret))
            {
                DBG_ERROR(("{WGL} ERR: Doing async-repaint failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }
    }
    
    return (i4_ret);
}
#else
INT32 x_wgl_move(
    HANDLE_T            h_widget,
    const GL_RECT_T*    pt_rect,
    UINT8               ui1_repaint)
{
    #define ACT_MOVE            0x1
    #define ACT_RESIZE          0x2
    
    _WIDGET_T*      pt_widget;
    HANDLE_TYPE_T   e_type;
    UINT16          ui2_action = 0;
    GL_RECT_T       t_rc_union;
    _CANVAS_T*      pt_canvas = NULL;
    INT32           i4_ret;

    wgl_widget_proc_fct pf_proc;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget    = %d\r\n"
         "\tpt_rect     = 0x%.8x\r\n"
         "\tui1_repaint = %d\r\n\r\n",
         __FUNCTION__,
         h_widget,
         pt_rect,
         ui1_repaint));
    
    CHECK_VALID(pt_rect && !RECT_EMPTY(pt_rect), WGLR_INV_ARG);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    ||
        !IS_WIDGET_TYPE(e_type)                                                 ||
        ST_DESTROYED(pt_widget))
    {
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", WGLR_INV_HANDLE, __LINE__));
        CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
        return WGLR_INV_HANDLE;
    }

    if (RECT_W(&(pt_widget->t_rect)) != RECT_W(pt_rect) ||
        RECT_H(&(pt_widget->t_rect)) != RECT_H(pt_rect))
    {
        ui2_action |= ACT_RESIZE;
    }

    if (pt_widget->t_rect.i4_left != pt_rect->i4_left ||
        pt_widget->t_rect.i4_top != pt_rect->i4_top)
    {
        ui2_action |= ACT_MOVE;
    }

    if (ui1_repaint != WGL_NO_AUTO_REPAINT && ui2_action != 0)
    {
        INT32 i4_x, i4_y;
        
        UNION_RECT(&t_rc_union, &(pt_widget->t_rect), pt_rect);
        _cal_translation(PARENT(pt_widget), &i4_x, &i4_y);
        MOVE_RECT(&t_rc_union, i4_x, i4_y);
        pt_canvas = CANVAS(BRANCH(pt_widget));
    }
    
    pt_widget->t_rect = *pt_rect;
    
    pf_proc = MSGPROC(pt_widget);
    

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    /* TODO: make an automatic repaint according the value of b_repaint */
    
    if (ui2_action & ACT_MOVE)
    {
        (*pf_proc)(
            h_widget, 
            WGL_MSG_MOVE, 
            (VOID*) COMBINE_HI_LO(pt_rect->i4_left, pt_rect->i4_top),
            0);
    }

    if (ui2_action & ACT_RESIZE)
    {
        (*pf_proc)(
            h_widget, 
            WGL_MSG_SIZE, 
            (VOID*) COMBINE_HI_LO(RECT_W(pt_rect), RECT_H(pt_rect)),
            0);
    }

    i4_ret = WGLR_OK;
    
    if (ui2_action != 0)
    {
        if (ui1_repaint == WGL_SYNC_AUTO_REPAINT)
        {
            i4_ret = _paint_canvas(pt_canvas, &t_rc_union);
            if (WGL_FAILED(i4_ret))
            {
                DBG_ERROR(("{WGL} ERR: Doing sync-repaint failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }
        else if (ui1_repaint == WGL_ASYNC_AUTO_REPAINT)
        {
            i4_ret = x_wgl_repaint(pt_canvas->h_this, &t_rc_union, FALSE);
            if (WGL_FAILED(i4_ret))
            {
                DBG_ERROR(("{WGL} ERR: Doing async-repaint failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }
    }
    
    return (i4_ret);
}
#endif
INT32 x_wgl_query_canvas_info_callback(
    HANDLE_T            h_widget,
    WGL_CVS_CB_TYPE_T   e_type,
    VOID                *pv_data,
    SIZE_T              *pz_size)
{
    INT32               i4_ret = WGLR_OK;
    HANDLE_TYPE_T       e_handle_type;
    WGL_CANVAS_INFO_T   t_cvs_info = {0};
    _CANVAS_T*          pt_canvas = NULL;
    
    if (h_widget == NULL_HANDLE || pv_data == NULL)
    {
        return WGLR_INV_ARG;
    }

    i4_ret = x_wgl_get_canvas_info(h_widget, &t_cvs_info);

    if (i4_ret != WGLR_OK)
    {
        return WGLR_INV_HANDLE;
    }

    if (handle_get_type_obj(t_cvs_info.h_canvas, &e_handle_type, (VOID **)&pt_canvas) == HR_OK &&
        e_handle_type == HT_WGL_CANVAS &&
        pt_canvas != NULL)
    {
        switch (e_type)
        {
            case WGL_CVS_CB_TXT_DIR:
                if (pz_size == NULL || *pz_size != sizeof(UINT8))
                {
                    return WGLR_INV_ARG;
                }

                if (pt_canvas->pf_info_cb == NULL)
                {
                    return WGLR_FUNC_NOT_IMPL;
                }
                else
                {
                    *pz_size = sizeof(UINT8);
                    *((UINT8 *)pv_data) = pt_canvas->pf_info_cb();

                    return WGLR_OK;
                }
                
            default:
                return WGLR_INV_ARG;
        }
    }
    else
    {
        return WGLR_INV_HANDLE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_register
 *
 * Description: This API registers the calling thread with the Widget Manager 
 *              and returns a corresponding client handle. If this API is 
 *              called the first time in the calling thread context, a new 
 *              client handle is created and returned. Otherwise the client 
 *              handle representing the calling thread is returned. This API 
 *              also sets the callback function of the application. The 
 *              callback function is invoked whenever a user input event 
 *              occurs.
 *
 * Inputs:  pf_callback Contains the application defined callback function.
 *
 * Outputs: ph_client   References the client handle if this routine was 
 *                      successful. In all other cases, this field is 
 *                      undetermined.
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_ARG        Either the argument pf_callback or ph_client 
 *                              is NULL.
 *          WGLR_OS_ERROR       This API is not called from a thread context.
 *          WGLR_OUT_OF_MEMORY  There is not enough memory to complete the 
 *                              operation.
 *          WGLR_ALREADY_REGISTERED
 *                              This API has been called from the calling 
 *                              thread context.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_register(
    wgl_app_callback_fct    pf_callback,
    HANDLE_T*               ph_client)
{
    _APP_INFO_T*    pt_app_info;
    INT32           i4_ret;
    HANDLE_T        h_thread;

    DBG_API(
        ("{WGL} %s:\r\n"
         "\tpf_callback = 0x%.8x\r\n"
         "\tph_client   = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         pf_callback,
         ph_client));
    
    CHECK_VALID(pf_callback && ph_client, WGLR_INV_ARG);
    CHECK_VALID(OSR_OK == x_thread_self(&h_thread), WGLR_OS_ERROR);
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    /* find the associated _APP_INFO_T structure */
    pt_app_info = _find_app_info(h_thread);

    if (pt_app_info)
    {
        pt_app_info->pf_callback = pf_callback;
        *ph_client = pt_app_info->h_this;
        i4_ret     = WGLR_ALREADY_REGISTERED;
        DBG_INFO(("{WGL} This client has already made registration (L%d)\r\n\r\n", __LINE__));
    }
    else
    {
        /* not found, create a new one */
        pt_app_info = _new_app_info(h_thread, pf_callback);
        if (!pt_app_info)
        {
            i4_ret = WGLR_OUT_OF_MEMORY;
            DBG_ERROR(("{WGL} ERR: Out of memory (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
        else
        {
            *ph_client = pt_app_info->h_this;
            i4_ret     = WGLR_OK;
        }
    }

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_repaint
 *
 * Description: This API issues a repaint request to the Repaint Manager. 
 *              Depends on the value of b_sync, this API could execute in an 
 *              asynchronous or a synchronous way. If the asynchronous behavior 
 *              is selected, a painting message is sent to the Repaint 
 *              Manager's thread and this API returns immediately. On the 
 *              contrary, if the synchronous behavior is chosen, this routine 
 *              will not return until the completion of the requested 
 *              repainting or until the occurrence of an error.
 *
 * Inputs:  h_container Contains the widget or the canvas handle being updated.
 *          pt_rect     References a GL_RECT_T structure which gives the area 
 *                      being redrawn. The coordinates specified in this 
 *                      structure are in the coordinates of h_container. A 
 *                      NULL value indicates that whole area of h_container 
 *                      must be updated.
 *          b_sync      If this argument is TRUE, the requested painting 
 *                      executes synchronously. Otherwise it executes 
 *                      asynchronously.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_container contains an invalid 
 *                              handle.
 *          WGLR_INV_ARG        The argument pt_rect is not NULL but the 
 *                              intersection of pt_rect and the widget area is 
 *                              empty.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_repaint(
    HANDLE_T            h_container,
    const GL_RECT_T*    pt_rect,
    BOOL                b_sync)
{
    HANDLE_TYPE_T   e_type;
    VOID*           pv_obj;
    _PAINT_MSG_T    t_paint_msg;
    BOOL            b_paint = TRUE;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_container = %d\r\n"
         "\tpt_rect     = 0x%.8x\r\n"
         "\tb_sync      = %d\r\n\r\n",
         __FUNCTION__,
         h_container,
         pt_rect,
         b_sync));
    
    CHECK_VALID(!pt_rect || (pt_rect && !RECT_EMPTY(pt_rect)), WGLR_INV_ARG);
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    i4_ret = WGLR_INV_HANDLE;
    
    if (HR_OK != handle_get_type_obj(h_container, &e_type, &pv_obj))
    {
        DBG_ERROR(("{WGL} ERR: Invalid container handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _REPAINT_RET;
    }
    
    if (IS_WIDGET_TYPE(e_type) && !ST_DESTROYED((_WIDGET_T*) pv_obj))
    {
        _WIDGET_T* pt_widget = (_WIDGET_T*) pv_obj;

        b_paint = (ST_MODIFIED(pt_widget) || _get_visibility(pt_widget));

        if (b_paint)
        {
            if (pt_rect)
            {
                t_paint_msg.t_update = *pt_rect;
                _cal_update_rect(pt_widget, &(t_paint_msg.t_update));
            }
            else
            {
                t_paint_msg.t_update = pt_widget->t_rect;
                _cal_update_rect(PARENT(pt_widget), &(t_paint_msg.t_update));
            }
            
            if (RECT_EMPTY(&(t_paint_msg.t_update)))
            {
                i4_ret = WGLR_INV_ARG;
                DBG_ERROR(("{WGL} ERR: Empty update rectangle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                goto _REPAINT_RET;
            }
            
            t_paint_msg.pt_canvas = CANVAS(BRANCH(pt_widget));
        }
    }
    else if (e_type == HT_WGL_CANVAS)
    {
        t_paint_msg.pt_canvas = (_CANVAS_T*) pv_obj;
        
        SET_RECT_BY_SIZE(
            &(t_paint_msg.t_update), 
            0, 
            0, 
            (INT32)t_paint_msg.pt_canvas->ui4_width, 
            (INT32)t_paint_msg.pt_canvas->ui4_height);
            
        if (pt_rect)
        {
            INTERSECT_RECT(
                &(t_paint_msg.t_update),
                &(t_paint_msg.t_update),
                pt_rect);
            
            if (RECT_EMPTY(&(t_paint_msg.t_update)))
            {
                i4_ret = WGLR_INV_ARG;
                DBG_ERROR(("{WGL} ERR: Empty update rectangle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                goto _REPAINT_RET;
            }
        }
    }
    else
    {
        DBG_ERROR(("{WGL} ERR: Invalid container handle type (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _REPAINT_RET;
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    if (b_paint)
    {
        if (b_sync)
        {
            i4_ret = _paint_canvas(t_paint_msg.pt_canvas, &(t_paint_msg.t_update));
            if (WGL_FAILED(i4_ret))
            {
                DBG_ERROR(("{WGL} ERR: Repainting update area failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
            return (i4_ret);
        }
        else
        {
            t_paint_msg.ui4_msg = RPM_MSG_PAINT;
            if (OSR_OK != x_msg_q_send(
                _t_repaint_mngr.h_msg_q, 
                &t_paint_msg, 
                sizeof(t_paint_msg),
                RPM_MSG_PRIORITY_PAINT))
            {
                i4_ret = WGLR_OS_ERROR;
                DBG_ERROR(("{WGL} ERR: Sending message to Repaint Manager failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
            else
            {
                i4_ret = WGLR_OK;
            }
            
            return (i4_ret);
        }
    }
    else
    {
        return WGLR_OK;
    }
    
_REPAINT_RET:
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_send_msg
 *
 * Description: This API sends the specified message and its accompanying data 
 *              to h_widget. The routine is synchronous and doesn't return 
 *              until the requested operation is accomplished.
 *
 * Inputs:  h_widget    Contains the widget handle that will receive the 
 *                      message.
 *          ui4_msg     Contains the message id.
 *          pv_param1   Specifies the additional information. The actual 
 *                      content of this argument depends on ui4_msg.
 *          pv_param2   Specifies more additional information. The actual 
 *                      content of this argument depends on ui4_msg.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_widget does not contain a valid 
 *                              widget handle.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_send_msg(
    HANDLE_T    h_widget,
    UINT32      ui4_msg,
    VOID*       pv_param1,
    VOID*       pv_param2)
{
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget;
    HANDLE_T        h_target;
    INT32           i4_ret;
    
    wgl_widget_proc_fct pf_proc;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget  = %d\r\n"
         "\tui4_msg   = %d\r\n"
         "\tpv_param1 = 0x%.8x\r\n"
         "\tpv_param2 = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_widget,
         ui4_msg,
         pv_param1,
         pv_param2));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    ||
        !IS_WIDGET_TYPE(e_type)                                                 ||
        ST_DESTROYED(pt_widget))
    {
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", WGLR_INV_HANDLE, __LINE__));
        CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
        return WGLR_INV_HANDLE;
    }

    h_target = _check_navigation_key(pt_widget, ui4_msg, (UINT32) pv_param1);

    pf_proc = MSGPROC(pt_widget);
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    if (h_target == NULL_HANDLE)
    {
        i4_ret = (*pf_proc)(h_widget, ui4_msg, pv_param1, pv_param2);
        if(i4_ret == WGLR_KEY_MSG_NOT_PROCESSED)
        { 
            /*WGLR_KEY_MSG_NOT_PROCESSED is for inner judge and need not reture outside*/
            i4_ret = WGLR_OK;
        }
        if (WGL_FAILED(i4_ret))
        {
            DBG_ERROR(("{WGL} ERR: Sending message (%d) failed (%d) at L%d\r\n\r\n", ui4_msg, i4_ret, __LINE__));
        }
    }
    else
    {
        i4_ret = x_wgl_set_focus(h_target, WGL_SYNC_AUTO_REPAINT);
        if (WGL_FAILED(i4_ret))
        {
            DBG_ERROR(("{WGL} ERR: Setting focus failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_send_notify
 *
 * Description: <function description>
 *
 * Inputs:  h_parent    References.
 *          ui4_nc_code References.
 *          h_widget    References.
 *          pt_nfy_data References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 x_wgl_send_notify(
    HANDLE_T            h_parent,
    UINT32              ui4_nc_code,
    HANDLE_T            h_widget,
    WGL_NOTIFY_DATA_T*  pt_nfy_data)
{
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget;
    _WIDGET_T*      pt_parent;
    INT32           i4_ret = WGLR_INV_HANDLE;
    
    wgl_widget_proc_fct pf_proc = NULL;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_parent    = %d\r\n"
         "\tui4_nc_code = %d\r\n"
         "\th_widget    = %d\r\n"
         "\tpt_nfy_data = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_parent,
         ui4_nc_code,
         h_widget,
         pt_nfy_data));

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (pt_nfy_data                                                             &&
        HR_OK == handle_get_type_obj(h_parent, &e_type, (VOID**) &pt_parent)    &&
        IS_WIDGET_TYPE(e_type)                                                  &&
        !ST_DESTROYED(pt_parent)                                                &&
        HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    &&
        IS_WIDGET_TYPE(e_type)                                                  &&
        !ST_DESTROYED(pt_widget))
    {
        pf_proc = MSGPROC(pt_parent);
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    if (pf_proc)
    {
        i4_ret = (*pf_proc)(h_parent, WGL_MSG_NOTIFY, (VOID*) h_widget, pt_nfy_data);
        if (WGL_FAILED(i4_ret))
        {
            DBG_ERROR(("{WGL} ERR: Sending notify message failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }

    return (i4_ret);
}
                    
/*-----------------------------------------------------------------------------
 * Name: x_wgl_set_active_app
 *
 * Description: This API sets the application whose main thread is h_thread as 
 *              the current active application. Those widgets of the active 
 *              application are displayed on top of all other application 
 *              widgets. The user input is routed to the active application as 
 *              well.
 *
 * Inputs:  h_thread    Contains the handle of the main thread of the 
 *                      application to be activated.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_thread does not contain a valid 
 *                              thread handle.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_set_active_app(HANDLE_T h_thread)
{
    CRIT_STATE_T    t_cs;
    BOOL            b_done;
    INT32           i4_ret;
   
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_thread = %d\r\n\r\n",
         __FUNCTION__,
         h_thread));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (NULL_HANDLE == h_thread)
    {   /* NULL_HANDLE means deactivating all applications */
        t_cs = x_crit_start();

        _pt_active_app->ui2_state &= (~AIS_ACTIVE);
        _pt_active_app = NULL;
        _i4_active_cnt--;

        x_crit_end(t_cs);

        b_done = TRUE;
    }
    else
    {
        b_done = _activate_app(h_thread);
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    i4_ret = ((b_done == TRUE) ? WGLR_OK :  WGLR_INV_CONTEXT);
    if (WGL_FAILED(i4_ret))
    {
        DBG_ERROR(("{WGL} ERR: Cannot find associated client info (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    return (i4_ret);
}

INT32 x_wgl_set_app_order(HANDLE_T h_thread, UINT8 ui1_order)
{
    INT32           i4_ret;
    _APP_INFO_T*    pt_app_info;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_thread  = %d\r\n"
         "\tui1_order = %d\r\n\r\n",
         __FUNCTION__,
         h_thread,
         ui1_order));

    CHECK_VALID(h_thread != NULL_HANDLE, WGLR_INV_ARG);

    CHECK_VALID(
        (ui1_order == WGL_APP_NOT_TO_END) || 
        (ui1_order == WGL_APP_TOPMOST)    ||
        (ui1_order == WGL_APP_BOTTOMMOST),
        WGLR_INV_ARG);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    pt_app_info = _find_app_info(h_thread);
    
    i4_ret = WGLR_INV_CONTEXT;
    
    if (pt_app_info)
    {
        pt_app_info->ui2_state &= (~(AIS_TOPMOST | AIS_BOTTOMMOST));
        
        if (WGL_APP_TOPMOST == ui1_order)
        {
            pt_app_info->ui2_state |= AIS_TOPMOST;
        }
        else if (WGL_APP_BOTTOMMOST == ui1_order)
        {
            pt_app_info->ui2_state |= AIS_BOTTOMMOST;
        }
        else /* WGL_NOT_TO_END == ui1_order */
        {
            ;
        }

        _move_app_to_end(
            pt_app_info, 
            pt_app_info->ui2_state);
        
        i4_ret = WGLR_OK;
    }
    else
    {
        DBG_ERROR(("{WGL} ERR: Cannot find associated client info (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_set_alpha
 *
 * Description: This API sets the alpha value of the specified widget.
 *
 * Inputs:  h_widget    Contains the widget whose alpha is going to be changed.
 *          ui1_alpha   Contains the alpha value.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The argument h_widget does not contain a valid 
 *                              widget handle.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_set_alpha(
    HANDLE_T            h_widget,
    UINT8               ui1_alpha)
{
    _WIDGET_T*      pt_widget;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget  = %d\r\n"
         "\tui1_alpha = %d\r\n\r\n",
         __FUNCTION__,
         h_widget,
         ui1_alpha));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    || 
        !IS_WIDGET_TYPE(e_type)                                                 ||
        ST_DESTROYED(pt_widget))
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    else
    {
        ALPHA(pt_widget) = ui1_alpha;
        i4_ret = WGLR_OK;
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}

INT32 x_wgl_set_app_ignore_key_fct(wgl_app_callback_fct pf_app_ignore_key_fct)
{
    _pf_app_ignore_key_fct = pf_app_ignore_key_fct;
    
    return WGLR_OK;
}

INT32 x_wgl_set_widget_ignore_key_fct(wgl_widget_callback_fct pf_widget_ignore_key_fct)
{
    _pf_widget_ignore_key_fct = pf_widget_ignore_key_fct;
    return WGLR_OK;
}

INT32 x_wgl_set_canvas_callback(
    HANDLE_T                    h_canvas,
    wgl_canvas_callback_fct     pf_cvs_nfy_fct,
    BOOL                        b_enable)
{
    _CANVAS_T*          pt_canvas;
    HANDLE_TYPE_T       e_type;
    INT32               i4_ret;
    
    DBG_API((
        "{WGL} %s:\r\n"
        "\th_canvas       = %d\r\n"
        "\tpf_cvs_nfy_fct = 0x%.8x\r\n",
        "\tb_enable       = %d\r\n",
        __FUNCTION__,
        h_canvas,
        (UINT32) pf_cvs_nfy_fct,
        b_enable));
    
    CHECK_VALID((pf_cvs_nfy_fct != NULL), WGLR_INV_ARG);

    i4_ret = WGLR_OK;

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK == handle_get_type_obj(h_canvas, &e_type, (VOID**) &pt_canvas) &&
        HT_WGL_CANVAS == e_type)
    {
        i4_ret = _enable_canvas_callback(pt_canvas, pf_cvs_nfy_fct, b_enable);
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}

INT32 x_wgl_set_canvas_color(
    HANDLE_T                    h_canvas,
    const WGL_CVS_CLR_CFG_T*    pt_cvs_clr)
{
    _CANVAS_T*          pt_canvas;
    HANDLE_TYPE_T       e_type;
    WGL_CVS_CLR_CFG_T   t_cvs_cfg;
    INT32               i4_ret;
    BOOL                b_processed = FALSE;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_canvas  = %d\r\n"
         "\tpt_cvs_clr = 0x%.8x\r\n",
         __FUNCTION__,
         h_canvas,
         (UINT32) pt_cvs_clr));
    
    CHECK_VALID((pt_cvs_clr != NULL), WGLR_INV_ARG);

    i4_ret = WGLR_OK;

    _hrc_lock();
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if (HR_OK == handle_get_type_obj(h_canvas, &e_type, (VOID**) &pt_canvas) &&
        HT_WGL_CANVAS == e_type)
    {
        UINT32 ui4_flag = pt_cvs_clr->ui4_flag;
        
        t_cvs_cfg.ui4_flag = 0;
        
        if ((WGLR_OK == i4_ret) && (ui4_flag & WGL_CANVAS_CFG_COLORKEY))
        {
            _set_canvas_colorkey(pt_canvas, &(pt_cvs_clr->t_colorkey));
            t_cvs_cfg.ui4_flag  |= WGL_CANVAS_CFG_COLORKEY;
            t_cvs_cfg.t_colorkey = pt_canvas->t_color_key;
            b_processed = TRUE;
        }

        if ((WGLR_OK == i4_ret) && (ui4_flag & WGL_CANVAS_CFG_PALETTE))
        {
            if (gl_is_valid_obj(pt_cvs_clr->h_palette, GLT_OBJ_PALETTE))
            {
                i4_ret = _set_canvas_palette(
                    pt_canvas, 
                    pt_cvs_clr->h_palette,
                    pt_canvas->b_running,
                    (pt_cvs_clr->ui4_flag & WGL_CANVAS_CFG_CHG_PLT_NO_CLEAN)? TRUE: FALSE);
                
                if (WGLR_OK == i4_ret)
                {
                    t_cvs_cfg.ui4_flag |= WGL_CANVAS_CFG_PALETTE;
                    t_cvs_cfg.h_palette = pt_canvas->h_palette;
                    b_processed = TRUE;
                }
            }
            else
            {
                i4_ret = WGLR_INV_HANDLE;
                DBG_ERROR(("{WGL} ERR: Invalid palette handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }

        if (b_processed)
        {
            _broadcast_widget_msg(pt_canvas, WGL_MSG_CHANGE_COLOR_CFG, (VOID*) &t_cvs_cfg, NULL);
        }
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    _hrc_unlock();
    
    if ((WGLR_OK == i4_ret) && (!b_processed)) /* not processed at all */
    {
        i4_ret = WGLR_INV_ARG;
    }
    
    return (i4_ret);
}

INT32 x_wgl_set_canvas_info_callback(
    HANDLE_T                    h_canvas,
    WGL_CVS_CB_TYPE_T           e_type,
    VOID                        *pv_cb_fct)
{
    _CANVAS_T*          pt_canvas = NULL;
    HANDLE_TYPE_T       e_handle_type;
    INT32               i4_ret = WGLR_OK;
    
    if (HR_OK         == handle_get_type_obj(h_canvas, &e_handle_type, (VOID **)&pt_canvas) &&
        HT_WGL_CANVAS == e_handle_type &&
        pt_canvas     != NULL)
    {
        switch (e_type)
        {
            case WGL_CVS_CB_TXT_DIR:
                pt_canvas->pf_info_cb =(wgl_canvas_info_callback_fct)((UINT32)pv_cb_fct);
                break;

            default:
                i4_ret = WGLR_INV_ARG;
        }
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
    }

    return i4_ret;
}

/*------------------------------------------------------------------*/
/*! @brief  set the children order of a parent
 *  @param  [in]        h_h_parent  - parent widget handle
 *  @param  [in]        ah_children - array to store the children handle in the desired order
 *  @param  [in]        i4_number   - specify the array size
 *  @note
 *  @see
 *  @return return the error code
 *  @retval WGLR_OK                 - ok
 *  @retval WGLR_INV_HANDLE         - an invalid handle is passed
 *  @retval WGLR_INV_ARG            - invalid argument is passed
 *  @retval WGLR_BUFFER_NOT_ENOUGH  - children number is larger than the array size
 *  @retval WGLR_LOCK_ERROR         - WGL lock error
 *  @retval WGLR_UNLOCK_ERROR       - WGL unlock error
 */
/*------------------------------------------------------------------*/
INT32 x_wgl_set_children_order(HANDLE_T h_parent, HANDLE_T ah_children[], INT32 i4_number)
{
    _WIDGET_T                   *pt_widget, *pt_child;
    HANDLE_TYPE_T               e_type;
    INT32                       i4_ret = WGLR_OK;

    CHECK_VALID(h_parent != NULL_HANDLE && ah_children != NULL, WGLR_INV_ARG);
    
    _hrc_lock();
    
    /* get handle type of h_container */
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_parent, &e_type, (VOID **)&pt_widget))
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid parent handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ERROR_RET;
    }
    
    if (IS_WIDGET_TYPE(e_type) && !ST_DESTROYED(pt_widget))
    {
        INT32       i4_size = 0;
        _WIDGET_T   *pt_prev_widget = NULL;

        /* for each child */
        for (pt_child = CHILD(pt_widget); pt_child != NULL; pt_child = SIBLING(pt_child))
        {
            /* count children number */
            i4_size = i4_size + 1;
        }

        /* check if the children number and the array size are matched */
        if (i4_size != i4_number)
        {
            i4_ret = WGLR_INV_ARG;
            DBG_ERROR(("{WGL} ERR: the array size and the children number is not matched (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _ERROR_RET;
        }

        /* for each child in the desired order */
        for (i4_size = 0; i4_size < i4_number; i4_size++)
        {
            pt_child = NULL;
            
            if (handle_get_type_obj(ah_children[i4_size], &e_type, (VOID **)&pt_child) != HR_OK ||
                IS_WIDGET_TYPE(e_type) != TRUE ||
                pt_child == NULL)
            {
                i4_ret = WGLR_INV_HANDLE;
                DBG_ERROR(("{WGL} ERR: the input child widget handle is incorrect (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                goto _ERROR_RET;
            }
            
            if (i4_size == 0)
            {
                CHILD(pt_widget) = pt_child;
            }
            else
            {
                if (pt_prev_widget != NULL)
                {
                    SIBLING(pt_prev_widget) = pt_child;
                }
            }

            SIBLING(pt_child) = NULL;
            pt_prev_widget    = pt_child;
        }
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid parent type (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _ERROR_RET;
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    _hrc_unlock();
    
    return WGLR_OK;

_ERROR_RET:
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    _hrc_unlock();
    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_set_delay_tolerance
 *
 * Description: This API sets the key delay tolerance of the application 
 *              corresponding to the calling thread.
 *
 * Inputs:  ui4_delay   Contains the key delay tolerance in ms. Note that this 
 *                      vlaue represents the maximum delay between the time 
 *                      where the event occurred and where the event is 
 *                      dispatched. If the time difference (lag) exceeds the 
 *                      delay tolerance, the key event is ignored.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_CONTEXT    The calling thread is not a registered 
 *                              application thread.
 *          WGLR_OS_ERROR       This API is not called from a thread context.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_set_delay_tolerance(UINT32 ui4_delay)
{
    HANDLE_T        h_thread;
    _APP_INFO_T*    pt_app_info;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\tui4_delay = %d\r\n\r\n",
         __FUNCTION__,
         ui4_delay));
    
    CHECK_VALID(OSR_OK == x_thread_self(&h_thread), WGLR_OS_ERROR);
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    pt_app_info = _find_app_info(h_thread);
    
    i4_ret = WGLR_INV_CONTEXT;

    if (pt_app_info)
    {
        pt_app_info->ui4_max_delay = ui4_delay;
        i4_ret = WGLR_OK;
    }
    else
    {
        DBG_ERROR(("{WGL} ERR: Cannot find associated client info (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_set_focus
 *
 * Description: This API sets h_widget as the focused widget of its owner 
 *              application. The routine sends a WGL_MSG_LOSE_FOCUS message to 
 *              the widget that loses the focus and a WGL_MSG_GET_FOCUS message 
 *              to the widget that receives the focus. If h_widget is a 
 *              top-level widget, it also receives a WGL_MSG_ACTIVATE message, 
 *              or the top-level parent of h_widget receives it.
 *
 * Inputs:  h_widget    Contains the widget handle that will get the focus.
 *          ui1_repaint Contains a flag indicating if synchronous/asynchronous 
 *                      repainting should be performed automatically after 
 *                      changing the focus.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK  Success.
 *          WGLR_INV_HNADLE The argument h_widget contains an invalid widget 
 *                          handle.
 *          WGLR_CANNOT_BE_FOCUSED  The specified widget h_widget cannot be 
 *                                  focused. A widget can only be focused if 
 *                                  all its parent and ancestors are enabled.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_set_focus(HANDLE_T h_widget, UINT8 ui1_repaint)
{
    HANDLE_TYPE_T   e_type;
    _WIDGET_T*      pt_widget;
    _WIDGET_T*      pt_ancestor;
    _WIDGET_T*      pt_lose;
    _WIDGET_T*      pt_deactive;
    _WIDGET_T*      pt_lost_upt = NULL;
    _WIDGET_T*      pt_got_upt  = NULL;
    _BRANCH_T*      pt_branch;
    _APP_INFO_T*    pt_app_info;
    _CANVAS_T*      pt_canvas;
    HANDLE_T        h_lost = NULL_HANDLE;
    HANDLE_T        h_got  = NULL_HANDLE;
    BOOL            b_curr_active;
    /* CRIT_STATE_T    t_cs; */
    INT32           i4_ret;
    GL_RECT_T       t_rc_lost, t_rc_got;
    INT32           i4_x, i4_y;
    
    HANDLE_T            h_deactive = NULL_HANDLE;
    HANDLE_T            h_ancestor = NULL_HANDLE;
    HANDLE_T            h_lose     = NULL_HANDLE;
    
    wgl_widget_proc_fct pf_proc_deactive = NULL;
    wgl_widget_proc_fct pf_proc_lose     = NULL;
    wgl_widget_proc_fct pf_proc_ancestor = NULL;
    wgl_widget_proc_fct pf_proc_widget   = NULL;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget    = %d\r\n"
         "\tui1_repaint = %d\r\n\r\n",
         __FUNCTION__,
         h_widget,
         ui1_repaint));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget) ||
        !IS_WIDGET_TYPE(e_type)                                             ||
        ST_DESTROYED(pt_widget))
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _SET_FOCUS_RET;
    }
    
    if (!_focusable(pt_widget))
    {
        i4_ret = WGLR_CANNOT_BE_FOCUSED;
        DBG_ERROR(("{WGL} ERR: h_widget cannot be focused (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _SET_FOCUS_RET;
    }
    
    pt_branch   = BRANCH(pt_widget);
    pt_app_info = OWNER(pt_branch);
    pt_canvas   = CANVAS(pt_branch);
    
    /* t_cs = x_crit_start(); */
    
    b_curr_active = (pt_app_info == _pt_active_app);

    /* x_crit_end(t_cs); */
    
    if (pt_app_info->pt_widget_focused == pt_widget)
    {   /* focused widget doesn't really change */
        i4_ret = WGLR_OK;
        goto _SET_FOCUS_RET;
    }

    pt_lose = pt_app_info->pt_widget_focused;
    if (pt_lose)
    {
        if (ST_DESTROYED(pt_lose))
        {
            pt_lose = NULL;
            h_lost  = NULL_HANDLE;
        }
        else
        {
            _WIDGET_T*  pt_tmp = pt_lose;

            while (pt_tmp)
            {
                STATE(pt_tmp) &= ~(WS_FOCUSED | WS_ACTIVATED);
                pt_tmp = PARENT(pt_tmp);
            }

            h_lost       = pt_lose->h_this;
            pt_lost_upt  = pt_lose;
        }
    }
    
    FOCUSED(pt_app_info) = pt_widget;
    STATE(pt_widget)    |= WS_FOCUSED;
    h_got                = pt_widget->h_this;
    pt_got_upt           = pt_widget;
    
    /* move all ancestors of pt_widget to the tail */
    pt_ancestor = pt_widget;
    while (1)
    {
        STATE(pt_ancestor) |= WS_ACTIVATED;
        
        if (AT_TOP_LEVEL(pt_ancestor))
        {   /* top-level widget */
            FIRST(pt_branch) = _insert_tail(pt_ancestor, FIRST(pt_branch));
            break;
        }
        else
        {   /* child widget */
#if 0            
            SIBLING_HEAD(pt_ancestor) = 
                _insert_tail(pt_ancestor, SIBLING_HEAD(pt_ancestor));
#endif
            pt_ancestor = PARENT(pt_ancestor);
        }
    }

    if (ACTIVATED(pt_app_info) == pt_ancestor)
    {   /* same top-level widget, do not send deactivation and activation messages */
        pt_deactive = NULL;
        pt_ancestor = NULL;
    }
    else
    {
        pt_deactive = ACTIVATED(pt_app_info);
        if (pt_deactive)
        {
            if (ST_DESTROYED(pt_deactive))
            {
                pt_deactive = NULL;
            }
            else
            {
                STATE(pt_deactive) &= ~WS_ACTIVATED;
                h_lost              = pt_deactive->h_this;
                pt_lost_upt         = pt_deactive;
            }
        }
        
        ACTIVATED(pt_app_info) = pt_ancestor;
        STATE(pt_ancestor)    |= WS_ACTIVATED;
        h_got                  = pt_ancestor->h_this;
        pt_got_upt             = pt_ancestor;
    }
    
    if (pt_deactive)
    {
        h_deactive       = pt_deactive->h_this;
        pf_proc_deactive = MSGPROC(pt_deactive);
    }
    
    if (pt_lose)
    {
        h_lose       = pt_lose->h_this;
        pf_proc_lose = MSGPROC(pt_lose);
    }
    
    if (pt_ancestor)
    {
        h_ancestor       = pt_ancestor->h_this;
        pf_proc_ancestor = MSGPROC(pt_ancestor);
    }

    pf_proc_widget = MSGPROC(pt_widget);
    
    if (pt_lost_upt)
    {
        t_rc_lost = pt_lost_upt->t_rect;
        _cal_translation(PARENT(pt_lost_upt), &i4_x, &i4_y);
        MOVE_RECT(&t_rc_lost, i4_x, i4_y);
    }

    if (pt_got_upt != NULL)
    {
        t_rc_got = pt_got_upt->t_rect;
        _cal_translation(PARENT(pt_got_upt), &i4_x, &i4_y);
        MOVE_RECT(&t_rc_got, i4_x, i4_y);
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    if (pt_deactive)
    {
        i4_ret = (*pf_proc_deactive)(h_deactive, WGL_MSG_ACTIVATE, (VOID*) FALSE, (VOID*) (UINT32) b_curr_active);
        if (WGL_FAILED(i4_ret))
        {
            DBG_ERROR(("{WGL} ERR: Sending WGL_MSG_ACTIVATE failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }

    if (pt_lose)
    {
        i4_ret = (*pf_proc_lose)(h_lose, WGL_MSG_LOSE_FOCUS, (VOID*) (UINT32) b_curr_active, NULL);
        if (WGL_FAILED(i4_ret))
        {
            DBG_ERROR(("{WGL} ERR: Sending WGL_MSG_LOSE_FOCUS failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
#ifdef WGL_ANIM_SUPPORT
        /*do animation for action*/
        i4_ret = _handle_animation_for_event(pt_widget,WGL_MSG_LOSE_FOCUS);
        if (WGL_FAILED(i4_ret))
        {
            DBG_ERROR(("{WGL} ERR: Handling animaiton map for WGL_MSG_LOSE_FOCUS failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
#endif
    }

    if (pt_ancestor)
    {
        i4_ret = (*pf_proc_ancestor)(h_ancestor, WGL_MSG_ACTIVATE, (VOID*) TRUE, (VOID*) (UINT32) b_curr_active);
        if (WGL_FAILED(i4_ret))
        {
            DBG_ERROR(("{WGL} ERR: Sending WGL_MSG_ACTIVATE failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    
    i4_ret = (*pf_proc_widget)(h_widget, WGL_MSG_GET_FOCUS, (VOID*) (UINT32) b_curr_active, NULL);
    if (WGL_FAILED(i4_ret))
    {
        DBG_ERROR(("{WGL} ERR: Sending WGL_MSG_GET_FOCUS failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
#ifdef WGL_ANIM_SUPPORT
    /*do animation for action*/
    i4_ret = _handle_animation_for_event(pt_widget,WGL_MSG_GET_FOCUS);
    if (WGL_FAILED(i4_ret))
    {
        DBG_ERROR(("{WGL} ERR: Handling animaiton map for WGL_MSG_LOSE_FOCUS failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
#endif
    if (ui1_repaint == WGL_SYNC_AUTO_REPAINT)
    {
        DBG_INFO(("{WGL} INFO: Update area overlapped! Unify the lost and got rects.\r\n\r\n"));
        
        if (pt_lost_upt && pt_got_upt && 
        	  (RECT_OVERLAPPED(&t_rc_got, &t_rc_lost)||
        	  RECT_INCLUDED(&t_rc_got, &t_rc_lost) ||
        	  RECT_INCLUDED(&t_rc_lost,&t_rc_got))
        	 )
        {
            UNION_RECT(&t_rc_got, &t_rc_lost, &t_rc_got);
            i4_ret = _paint_canvas(pt_canvas, &t_rc_got);
            if (WGL_FAILED(i4_ret))
            {
                DBG_ERROR(("{WGL} ERR: Painting update area failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }
        else
        {
            if (pt_lost_upt)
            {
                i4_ret = _paint_canvas(pt_canvas, &t_rc_lost);
                if (WGL_FAILED(i4_ret))
                {
                    DBG_ERROR(("{WGL} ERR: Painting de-focused area failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }
            }
            
            if (pt_got_upt != NULL)
            {
                i4_ret = _paint_canvas(pt_canvas, &t_rc_got);
                if (WGL_FAILED(i4_ret))
                {
                    DBG_ERROR(("{WGL} ERR: Painting focused area failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }
            }
        }
    }
    else if (ui1_repaint == WGL_ASYNC_AUTO_REPAINT)
    {
        if (h_lost != NULL_HANDLE)
        {
            i4_ret = x_wgl_repaint(h_lost, NULL, FALSE);
            if (WGL_FAILED(i4_ret))
            {
                DBG_ERROR(("{WGL} ERR: Painting de-focused area failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }

        if (h_got != NULL_HANDLE)
        {
            i4_ret = x_wgl_repaint(h_got, NULL, FALSE);
            if (WGL_FAILED(i4_ret))
            {
                DBG_ERROR(("{WGL} ERR: Painting focused area failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }
    }

    return WGLR_OK;
    
_SET_FOCUS_RET:

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_set_navigation
 *
 * Description: This API sets the navigation map of h_widget. Once the 
 *              navigation map is set, every time x_wgl_dispatch_event() or 
 *              x_wgl_send_msg() is called, the navigation map of the 
 *              destination widget is checked. If the message being sent is 
 *              WGL_MSG_KEY_DOWN and the key code matches one of the keys in 
 *              the navigation map, the Widget Manager sets the focus to the 
 *              widget corresponding to the key in the navigation map. In this 
 *              situation, the message itself, however, is not routed to the 
 *              message procedure of the original destination widget.
 *
 * Inputs:  h_widget    Contains the widget handle that will get the focus.
 *          ui1_cnt     Contains the number of entries of at_key_lnk. This 
 *                      number must fall in the range (0, WGL_MAX_KEY_LINK).
 *          at_key_lnk  References an array of WGL_KEY_LINK_T structure. Each 
 *                      structure entry defines the mapping of key and target 
 *                      widget handle.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HNADLE     The argument h_widget contains an invalid 
 *                              widget handle.
 *          WGLR_INV_ARG        The argument at_key_lnk is NULL or ui1_cnt is 
 *                              not in the valid range.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_set_navigation(
    HANDLE_T                h_widget,
    UINT8                   ui1_cnt,
    const WGL_KEY_LINK_T    at_key_lnk[])
{
    _WIDGET_T*      pt_widget;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget   = %d\r\n"
         "\tui1_cnt    = %d\r\n"
         "\tat_key_lnk = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         h_widget,
         ui1_cnt,
         at_key_lnk));
    
    CHECK_VALID(
        at_key_lnk && (ui1_cnt <= (UINT8)WGL_MAX_KEY_LINK), 
        WGLR_INV_ARG);
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    &&
        IS_WIDGET_TYPE(e_type)                                                  &&
        !ST_DESTROYED(pt_widget))
    {
        if (ui1_cnt > 0)
        {
            x_memcpy(pt_widget->at_key_lnk, at_key_lnk, sizeof(WGL_KEY_LINK_T) * ui1_cnt);
        }
        
        pt_widget->ui1_num_key_link = ui1_cnt;
        
        i4_ret = WGLR_OK;
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", WGLR_INV_HANDLE, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}

INT32 x_wgl_set_shadow_canvas_block_time(UINT32 ui4_timeout)
{
    if (ui4_timeout)
    {
        _ui4_shdw_cvs_blk_time = ui4_timeout;

        return WGLR_OK;
    }
    else
    {
        return WGLR_INV_ARG;
    }
}

INT32 x_wgl_set_transparent_list(
    HANDLE_T            h_rgn_list,
    GL_RECT_T           at_rect[],
    UINT32              ui4_rect_count)
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    _TRANSP_REGION_T    *pt_region = NULL;
    
    if (h_rgn_list == NULL_HANDLE || at_rect == NULL || ui4_rect_count == 0)
    {
        return WGLR_INV_ARG;
    }

    i4_ret = handle_get_type_obj(h_rgn_list, &e_type, (VOID **)&pt_region);

    if (i4_ret != HR_OK || e_type != HT_WGL_TRANP_LIST || pt_region == NULL)
    {
        return WGLR_INV_HANDLE;
    }

    WGL_ASSERT(pt_region->pt_rect != NULL);
    
    _wgl_lock();

    if (pt_region->ui4_rect_count != ui4_rect_count)
    {
        pt_region->pt_rect = x_mem_realloc(pt_region->pt_rect, ui4_rect_count * sizeof(GL_RECT_T));

        if (pt_region->pt_rect == NULL)
        {
            _wgl_unlock();

            DBG_ERROR(("{WGL} re-allocate transparent list element failed. Line: %d", __LINE__));
            return WGLR_OUT_OF_RESOURCE;
        }
    }

    x_memcpy(pt_region->pt_rect, at_rect, ui4_rect_count * sizeof(GL_RECT_T));

    pt_region->ui4_rect_count = ui4_rect_count;
    
    _wgl_unlock();
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_set_visibility
 *
 * Description: This API is used to alter the visibility flag of the specified 
 *              widget and/or its child widgets.
 *
 * Inputs:  h_widget    Contains the widget handle.
 *          e_sw_cmd    Contains an enumerated value of type WGL_SW_CMD_T, 
 *                      which defines the way to set the visibility flag.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HNADLE     The argument h_widget contains an invalid 
 *                              widget handle.
 *          WGLR_INV_ARG        The argument e_sw_cmd contains an invalid 
 *                              enumerated value.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_set_visibility(
    HANDLE_T            h_widget,
    WGL_SW_CMD_T        e_sw_cmd)
{
    _WIDGET_T*      pt_widget;
    HANDLE_TYPE_T   e_type;
    BOOL            b_old;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget = %d\r\n"
         "\te_sw_cmd = %d\r\n\r\n",
         __FUNCTION__,
         h_widget,
         e_sw_cmd));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    ||
        !IS_WIDGET_TYPE(e_type)                                                 ||
        ST_DESTROYED(pt_widget))
    {
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", WGLR_INV_HANDLE, __LINE__));
        CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
        return WGLR_INV_HANDLE;
    }

    b_old = _get_visibility(pt_widget);
    
    _set_visibility(pt_widget, e_sw_cmd);
    
    if ((b_old) || (_get_visibility(pt_widget)))
    {
        STATE(pt_widget) |= WS_MODIFIED;
    }

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return WGLR_OK;
}

INT32 x_wgl_set_widget_proc(
    HANDLE_T            h_widget,
    wgl_widget_proc_fct pf_wdgt_proc)
{
    _WIDGET_T*      pt_widget;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget  = %d\r\n"
         "\tpf_wdgt_proc = %d\r\n\r\n",
         __FUNCTION__,
         h_widget,
         pf_wdgt_proc));

    if(NULL == pf_wdgt_proc)
    {
        return WGLR_INV_ARG;
    }
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    if (HR_OK != handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    || 
        !IS_WIDGET_TYPE(e_type)                                                 ||
        ST_DESTROYED(pt_widget))
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    else
    {
        MSGPROC(pt_widget) = pf_wdgt_proc;
        i4_ret = WGLR_OK;
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);

}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_show
 *
 * Description: Invocation of this API shows/hides the specified widget and/or 
 *              its child widgets.
 *
 * Inputs:  h_widget    Contains the widget handle.
 *          e_sw_cmd    Contains an enumerated value of type WGL_SW_CMD_T, 
 *                      which defines the way to display the widget.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HNADLE     The argument h_widget contains an invalid 
 *                              widget handle.
 *          WGLR_INV_ARG        The argument e_sw_cmd contains an invalid 
 *                              enumerated value.
 *          WGLR_LOCK_ERROR     Locking the internal protection semaphore 
 *                              failed.
 *          WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore 
 *                              failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_show(
    HANDLE_T            h_widget,
    WGL_SW_CMD_T        e_sw_cmd)
{
    HANDLE_TYPE_T   e_type;
    GL_RECT_T       t_rect;
    _WIDGET_T*      pt_widget;
    INT32           i4_ret;
    _CANVAS_T*      pt_canvas = NULL;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_widget = %d\r\n"
         "\te_sw_cmd = %d\r\n\r\n",
         __FUNCTION__,
         h_widget,
         e_sw_cmd));
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);
    
    i4_ret = WGLR_INV_HANDLE;
    
    if (HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget)    &&
        IS_WIDGET_TYPE(e_type)                                                  &&
        !ST_DESTROYED(pt_widget))
    {
        BOOL b_old;
        
        b_old = _get_visibility(pt_widget);
        
        _set_visibility(pt_widget, e_sw_cmd);
        
        if ((b_old) || (_get_visibility(pt_widget)))
        {
            pt_canvas = CANVAS(BRANCH(pt_widget));
            t_rect = pt_widget->t_rect;
            _cal_update_rect(PARENT(pt_widget), &(t_rect));
        }

        i4_ret = WGLR_OK;
    }
    else
    {
        DBG_ERROR(("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", WGLR_INV_HANDLE, __LINE__));
    }

    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    if (pt_canvas)
    {
        i4_ret = _paint_canvas(pt_canvas, &t_rect);
        if (WGL_FAILED(i4_ret))
        {
            DBG_ERROR(("{WGL} ERR: Repainting update area failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    
    return (i4_ret);
}

INT32 x_wgl_start_timer(
    HANDLE_T            h_timer,
    UINT32              ui4_delay,
    TIMER_FLAG_T        e_flag,
    VOID*               pv_tag)
{
    _TIMER_T*       pt_timer;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_timer    = %d\r\n"
         "\tui4_delay  = %d\r\n"
         "\te_flag     = %d\r\n"
         "\tpv_tag     = %d\r\n\r\n",
         __FUNCTION__,
         h_timer,
         ui4_delay,
         e_flag,
         (UINT32) pv_tag));
    
    CHECK_VALID(h_timer != NULL_HANDLE, WGLR_INV_ARG);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if ((HR_OK == handle_get_type_obj(h_timer, &e_type, (VOID**) &pt_timer)) && 
        (HT_WGL_TIMER == e_type))
    {
        if (x_timer_start(
            pt_timer->h_sys_timer, 
            ui4_delay, 
            e_flag, 
            _widget_timer_callback, 
            (VOID*) h_timer) == OSR_OK)
        {
            pt_timer->pv_tag    = pv_tag;
            pt_timer->ui4_flag |= TIMER_STATUS_STARTED;
            i4_ret = WGLR_OK;
        }
        else
        {
            i4_ret = WGLR_INTERNAL_ERROR;
            DBG_ERROR(("{WGL} ERR: Start timer failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget timer handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}

INT32 x_wgl_stop_timer(HANDLE_T h_timer)
{
    _TIMER_T*       pt_timer;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_timer    = %d\r\n\r\n",
         __FUNCTION__,
         h_timer));
    
    CHECK_VALID(h_timer != NULL_HANDLE, WGLR_INV_ARG);

    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if ((HR_OK == handle_get_type_obj(h_timer, &e_type, (VOID**) &pt_timer)) && 
        (HT_WGL_TIMER == e_type))
    {
        i4_ret = WGLR_OK;
        
        if (TM_ST_STARTED(pt_timer->ui4_flag))
        {
            if (x_timer_stop(pt_timer->h_sys_timer) == OSR_OK)
            {
                pt_timer->ui4_flag &= ~TIMER_STATUS_STARTED;
            }
            else
            {
                i4_ret = WGLR_INTERNAL_ERROR;
                DBG_ERROR(("{WGL} ERR: Stop timer failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }
    }
    else
    {
        i4_ret = WGLR_INV_HANDLE;
        DBG_ERROR(("{WGL} ERR: Invalid widget timer handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_wgl_unregister
 *
 * Description: This API unregisters the client represented by h_client and 
 *              frees this client handle. All widgets ever created in the main 
 *              thread context of h_client will be destroyed as well. If 
 *              h_client is the current active application, the next 
 *              application along the application list becomes active.
 *
 * Inputs:  h_client    Contains the client (application) handle.
 *
 * Outputs: - 
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HNADLE     The argument h_client contains an invalid 
 *                              client handle.
 *          WGLR_INTERNAL_ERROR Freeing the client handle failed.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_unregister(HANDLE_T h_client)
{
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_client = %d\r\n\r\n",
         __FUNCTION__,
         h_client));
    
    CHECK_VALID(
        HR_OK == x_handle_get_type(h_client, &e_type) && HT_WGL_CLIENT == e_type,
        WGLR_INV_HANDLE);
        
    i4_ret = (HR_OK == handle_free(h_client, FALSE) ? WGLR_OK : WGLR_INTERNAL_ERROR);
    if (WGL_FAILED(i4_ret))
    {
        DBG_ERROR(("{WGL} ERR: Freeing client handle failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }
    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name:wgl_arab_buffer_lock
 *
 * Description: API to lock the arab buffer api
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID wgl_arab_buffer_lock(
    VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_lock( h_arab_buffer_lock, X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
        DBG_ERROR(("{WGL} ERR: Cannot Lock Arab buffer%d\r\n", i4_ret, __LINE__));
    }
}


/*-----------------------------------------------------------------------------
 * Name: wgl_arab_buffer_unlock
 *
 * Description: API to unlock the arab buffer api.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID wgl_arab_buffer_unlock(
    VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_unlock( h_arab_buffer_lock );
    if ( i4_ret != OSR_OK )
    {
        DBG_ERROR(("{WGL} ERR: Cannot Unlock Arab buffer%d\r\n", i4_ret, __LINE__));
    }
}


INT32 x_wgl_get_string_size(HFONT_T h_fnt,
            const UTF16_T * w2s_str,
            INT32 i4_length,
            INT32 * pi4_width,
            INT32 * pi4_height)
{
    UTF16_T* w2s_str_new = (UTF16_T*)w2s_str;
    if (!check_exist_arabic(w2s_str_new , (UINT16)i4_length))
    {
        return x_fe_get_string_size(h_fnt, w2s_str, i4_length, pi4_width, pi4_height);
    }
    else
    {
        UINT16 i2_length_out;
        INT32 i4_ret;
        if (!h_arab_buffer_lock)
        {
            i4_ret = x_sema_create( &h_arab_buffer_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
            if ( i4_ret!=OSR_OK || h_arab_buffer_lock==NULL_HANDLE )
            {
                x_sema_delete(h_arab_buffer_lock);
                DBG_ERROR(("{WGL} ERR: Cannot Create lock%d\r\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
        
        wgl_arab_buffer_lock();
        if (gpw2s_str == NULL)
        {
            gpw2s_str = x_mem_calloc(WGL_ARAB_BUFFER_DEFAULT_SIZE , sizeof(UTF16_T));
            gi4_buffer_size = WGL_ARAB_BUFFER_DEFAULT_SIZE;
        }
        
        if (gi4_buffer_size < i4_length)
        {
            gpw2s_str = x_mem_realloc(gpw2s_str , (SIZE_T)((i4_length) * sizeof(UTF16_T)));
            gi4_buffer_size = i4_length;
        }
        
        reorder_arabic(w2s_str_new, (UINT16)i4_length,NULL, gpw2s_str, &i2_length_out);

        i4_ret = x_fe_get_string_size(h_fnt, gpw2s_str, (INT32)i2_length_out, pi4_width, pi4_height);
        wgl_arab_buffer_unlock();
        
        return i4_ret;
    }
}

/**************************************************************************************************
                                                         APIs for animation
******************************************************************************************************/

/*************************************Inner static APIs****************************************************/
#ifdef WGL_ANIM_SUPPORT    
static BOOL _get_valid_animation_action_type(UINT32 event_type,WGL_ANIM_ACTION_TYPE_T* pe_action_type)
{
    switch(event_type)
    {
        case WGL_MSG_GET_FOCUS:
            *pe_action_type = WGL_ANIM_ACTION_FOCUS;
            return TRUE;
        case WGL_MSG_LOSE_FOCUS:
            *pe_action_type = WGL_ANIM_ACTION_UNFOCUS;
            return TRUE;
        default:
            return FALSE;
    }
}
static INT32 _handle_animation_for_event(_WIDGET_T* pt_widget,UINT32 event_type)
{
    WGL_ANIM_ACTION_TYPE_T   e_action_type;
    UINT8 ui1_idx=0;
    
    if(_get_valid_animation_action_type(event_type,&e_action_type) && pt_widget->ui1_map_count <= WGL_ANIM_MAX_MAP_CNT)
    {
        for(ui1_idx=0; ui1_idx<pt_widget->ui1_map_count; ui1_idx++)
        {
            if(pt_widget->at_anim_map[ui1_idx].e_anim_action_type == e_action_type )
            {
                wgl_end_animation(pt_widget->h_this,WGL_ANIM_ATTR_WIDGET);
                wgl_begin_animation(pt_widget->h_this,&pt_widget->at_anim_map[ui1_idx].t_anim_data,WGL_ANIM_ATTR_WIDGET,pt_widget->at_anim_map[ui1_idx].t_anim_context);
            }
        }
    }
    return WGLR_OK;
}

static INT32 _handle_anim_tick_message(HANDLE_T h_widget,WGL_ANIM_ATTR_TYPE e_anim_attr_type,UINT32 ui4_tick_idx)
{
    HANDLE_TYPE_T           e_type;
    _WIDGET_T               *pt_widget = NULL;
    INT32                   i4_ret = WGLR_OK;
    /* check handle of this animation widget */
    i4_ret = handle_get_type_obj(h_widget, &e_type, (VOID **)&pt_widget);

    if (i4_ret != HR_OK || !IS_WIDGET_TYPE(e_type) || ST_DESTROYED(pt_widget))
    {
        DBG_ERROR(("{WGL} invalid widget handle. Line: %d\n", __LINE__));
        i4_ret = WGLR_INV_HANDLE;
    }
    else
    {
       if(e_anim_attr_type == WGL_ANIM_ATTR_WIDGET)
        {
           WGL_ANIM_TICK_DATA t_anim_tick_data;
           UINT8 ui1_idx,ui1_new_idx=0xFF,ui1_old_idx=0xFF;
           GL_RECT_T t_widget_new_rect;
           WGL_UPD_REGION_T t_update_region;
           WGL_IMG_INFO_T t_widget_bk_img_info;
           GL_RECT_T               t_old_rect = {0};
           WGL_ANIM_COND_INF_T          e_anim_cond;           
           
           /* handle tick message */
           i4_ret = wgl_anim_cmd_proc(ANIM(pt_widget),WGL_CMD_ANIM_ATTR_TICK,WGL_PACK(ui4_tick_idx),NULL);
           if(i4_ret == WGLR_OK)
            {
                i4_ret = wgl_anim_cmd_proc(ANIM(pt_widget), WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&e_anim_cond), NULL);
                if(i4_ret != WGLR_OK)
                {
                    return i4_ret;
                }
                if ( e_anim_cond.ui2_end_type == WGL_ANIM_TYPE_ALL)
                {
                    return WGLR_OK;
                }
              
                /*change widget's value*/                
                i4_ret = wgl_anim_cmd_proc(ANIM(pt_widget),WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_anim_tick_data));
                if(i4_ret != WGLR_OK)
                {
                    return i4_ret;
                }

                if(t_anim_tick_data.ui2_cur_rsn&WGL_ANIM_TYPE_NONE)
                {
                   return WGLR_OK;
                }
                if(t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_MOVE)
                {
                    t_old_rect = pt_widget->t_rect;
                    SET_RECT_BY_SIZE(&t_widget_new_rect,
                        t_anim_tick_data.t_new_tick.t_tick_inf.t_point.i4_x,
                        t_anim_tick_data.t_new_tick.t_tick_inf.t_point.i4_y,
                        RECT_W(&(pt_widget->t_rect)),
                        RECT_H(&(pt_widget->t_rect)));
                    x_wgl_move(h_widget,&t_widget_new_rect,WGL_NO_AUTO_REPAINT);
                }
                if(t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_ALPHA)
                {
                    x_wgl_set_alpha(h_widget,t_anim_tick_data.t_new_tick.t_tick_inf.ui1_alpha);
                }
                if(t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_IMG)
                {
                    t_widget_bk_img_info.e_type = WGL_IMG_SET_EXTEND;
                    if(t_anim_tick_data.t_new_tick.t_tick_inf.t_img.pt_img != NULL)
                    {
                        t_widget_bk_img_info.u_img_data.t_extend.t_enable = *t_anim_tick_data.t_new_tick.t_tick_inf.t_img.pt_img;
                        t_widget_bk_img_info.u_img_data.t_extend.t_disable= *t_anim_tick_data.t_new_tick.t_tick_inf.t_img.pt_img;
                        t_widget_bk_img_info.u_img_data.t_extend.t_highlight = *t_anim_tick_data.t_new_tick.t_tick_inf.t_img.pt_img;
                        t_widget_bk_img_info.u_img_data.t_extend.t_highlight_inactive= *t_anim_tick_data.t_new_tick.t_tick_inf.t_img.pt_img;
                        t_widget_bk_img_info.u_img_data.t_extend.t_push= *t_anim_tick_data.t_new_tick.t_tick_inf.t_img.pt_img;
                        t_widget_bk_img_info.u_img_data.t_extend.t_push_unhighlight = *t_anim_tick_data.t_new_tick.t_tick_inf.t_img.pt_img;
                        x_wgl_do_cmd(h_widget,WGL_CMD_GL_SET_IMAGE,WGL_PACK(WGL_IMG_FG),WGL_PACK(&t_widget_bk_img_info));
                    }
                }
                if(t_anim_tick_data.ui2_begin_flag&WGL_ANIM_TYPE_MOVE)
                {
                    wgl_anim_cmd_proc(ANIM(pt_widget),WGL_CMD_ANIM_ATTR_GET_UPD_RGN, WGL_PACK(&t_old_rect),WGL_PACK(&t_update_region));
                }
                else
              	{
                    wgl_anim_cmd_proc(ANIM(pt_widget),WGL_CMD_ANIM_ATTR_GET_UPD_RGN, WGL_PACK(&pt_widget->t_rect),WGL_PACK(&t_update_region));
                }
                if(t_update_region.ui4_update_rect_num <= WGL_MAX_UPDATE_RECT_NUM )
                {
                    for(ui1_idx=0;ui1_idx<(UINT8)(t_update_region.ui4_update_rect_num);ui1_idx++)
                    {
                        /*for the case, animation is scaled down and when the animation is ended need recover widget's init rect*/
                        if((RECT_OVERLAPPED(&t_update_region.at_update_rect[ui1_idx],&pt_widget->t_rect)||
                            RECT_INCLUDED(&t_update_region.at_update_rect[ui1_idx],&pt_widget->t_rect)||
                            RECT_INCLUDED(&pt_widget->t_rect,&t_update_region.at_update_rect[ui1_idx])))
                        {
                            ui1_new_idx = ui1_idx;
                        }                       
                         /*for the case, animation is move type and the animation's start x,y is not equals to widget's rect x,y value*/
                        if( !RECT_EMPTY(&t_old_rect) &&
                            (RECT_OVERLAPPED(&t_update_region.at_update_rect[ui1_idx],&t_old_rect)||
                            RECT_INCLUDED(&t_update_region.at_update_rect[ui1_idx],&t_old_rect)||
                            RECT_INCLUDED(&t_old_rect,&t_update_region.at_update_rect[ui1_idx])))
                        {
                            ui1_old_idx = ui1_idx;
                        }
                    }
                    for(ui1_idx=0;ui1_idx<(UINT8)(t_update_region.ui4_update_rect_num);ui1_idx++)
                    {
                        if(ui1_idx == ui1_new_idx)
                        {
                            UNION_RECT(&t_widget_new_rect, &t_update_region.at_update_rect[ui1_idx], &pt_widget->t_rect);
                        }else
                        {
                            t_widget_new_rect = t_update_region.at_update_rect[ui1_idx];
                        }
                        if(ui1_idx == ui1_old_idx)
                        {
                            UNION_RECT(&t_widget_new_rect,&t_widget_new_rect, &t_old_rect);
                        }
                        wgl_add_updated_region(h_widget,&t_widget_new_rect);
                         
                    }
                }
            }
           
        }else
        {
            wgl_widget_proc_fct     pf_proc = NULL;
            pf_proc   = MSGPROC(pt_widget);
            i4_ret = (*pf_proc)(h_widget, WGL_MSG_ANIMATION_TICK,NULL,WGL_PACK(ui4_tick_idx));
        }
    }
    return i4_ret;
}

static BOOL _check_anim_support(WGL_ANIMATION_DATA_T* pt_anim_data)
{

    UINT16 ui2_anim_type = WGL_ANIM_TYPE_NONE;
    UINT16 ui2_idx=0;
    
    if(pt_anim_data->ui4_flag & WGL_ANIM_DATA_FLAG_PATH_ID_INF)
    {
        //TODO current dose not support path library for widget animation
        return FALSE;
    }
    for(ui2_idx=0; ui2_idx<pt_anim_data->ui2_count;ui2_idx++)
    {
        if(pt_anim_data->ui4_flag & WGL_ANIM_DATA_FLAG_PATH_MODE_INF )
        {
            ui2_anim_type = pt_anim_data->pt_path_mode[ui2_idx].ui2_anim_type;
        }
        if(pt_anim_data->ui4_flag & WGL_ANIM_DATA_FLAG_DATA_INF)
        {
            ui2_anim_type = pt_anim_data->pt_anim_data[ui2_idx].ui2_anim_type;
        }
        if(ui2_anim_type &WGL_ANIM_TYPE_BREATHE ||
            ui2_anim_type &WGL_ANIM_TYPE_SIZE)
        {
            return FALSE;
        }
        if(
        (ui2_anim_type &WGL_ANIM_TYPE_SCALE||
         ui2_anim_type&WGL_ANIM_TYPE_ALPHA||
         ui2_anim_type &WGL_ANIM_TYPE_BREATHE)&&
        (_b_inter_compose == FALSE))
        {
             return FALSE;
        }
        if(ui2_anim_type &WGL_ANIM_TYPE_SCALE&&
          (ui2_anim_type &WGL_ANIM_TYPE_ALPHA||
           ui2_anim_type &WGL_ANIM_TYPE_BREATHE)&&
           (_b_support_compose_anim == FALSE))
        {
             return FALSE;
        }
    }
    return TRUE;
    
}
static BOOL _check_anim_child_conflict(_WIDGET_T* pt_widget)
{
      _WIDGET_T* pt_child;
      WGL_ANIM_TICK_DATA t_anim_tick_data;
       /*if one widget's style is WGL_STL_GL_ANIM_AFFECT_CHILD and with Scale/Alpha/Breathe/Size animation, then it is conflict with all its 
       descendant's animation*/
      wgl_anim_cmd_proc(ANIM(pt_widget),WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_anim_tick_data));
      if(t_anim_tick_data.ui2_begin_flag &WGL_ANIM_TYPE_SCALE||
         t_anim_tick_data.ui2_begin_flag  &WGL_ANIM_TYPE_ALPHA||
         t_anim_tick_data.ui2_begin_flag  &WGL_ANIM_TYPE_BREATHE
        )
        {
           return TRUE;
        }
    
      for (pt_child = CHILD(pt_widget); pt_child; pt_child = SIBLING(pt_child))
      {
          if (TRUE == _check_anim_child_conflict(pt_child))
          {
              return TRUE;
          }
      }
    
      return FALSE;

}
static BOOL _check_anim_conflict(_WIDGET_T* pt_widget,WGL_ANIMATION_DATA_T* pt_anim_data)
{

  /*if one widget's style is WGL_STL_GL_ANIM_AFFECT_CHILD and with Scale/Alpha/Breathe/Size animation, then it is conflict with all its 
       descendant's animation*/
    UINT16 ui2_anim_type = WGL_ANIM_TYPE_NONE;
    UINT16 ui2_idx=0;
    
    if(pt_anim_data->ui4_flag & WGL_ANIM_DATA_FLAG_PATH_ID_INF)
    {
        //TODO maybe path library have already checked support
        return TRUE;
    }
    for(ui2_idx=0; ui2_idx<pt_anim_data->ui2_count;ui2_idx++)        
    {
        if(pt_anim_data->ui4_flag & WGL_ANIM_DATA_FLAG_PATH_MODE_INF )
        {
            ui2_anim_type = pt_anim_data->pt_path_mode[ui2_idx].ui2_anim_type;
        }
        if(pt_anim_data->ui4_flag & WGL_ANIM_DATA_FLAG_DATA_INF)
        {
            ui2_anim_type = pt_anim_data->pt_anim_data[ui2_idx].ui2_anim_type;
        }
        if(ui2_anim_type &WGL_ANIM_TYPE_SCALE||
        ui2_anim_type &WGL_ANIM_TYPE_ALPHA||
        ui2_anim_type &WGL_ANIM_TYPE_BREATHE)
        {
            if(pt_widget->b_anim_affect_child )
            {
                _WIDGET_T* pt_child;
                for (pt_child = CHILD(pt_widget); pt_child; pt_child = SIBLING(pt_child))
                {
                  if (TRUE == _check_anim_child_conflict(pt_child))
                  {
                      return TRUE;
                  }
                }
            }
            else
            {
                WGL_ANIM_TICK_DATA t_anim_tick_data;                
                _WIDGET_T* pt_parent;
                pt_parent = PARENT(pt_widget);
                while (pt_parent)
                {
                    wgl_anim_cmd_proc(ANIM(pt_parent),WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_anim_tick_data));
                    if(pt_parent->b_anim_affect_child &&
                        (t_anim_tick_data.ui2_begin_flag &WGL_ANIM_TYPE_SCALE||
                         t_anim_tick_data.ui2_begin_flag  &WGL_ANIM_TYPE_ALPHA||
                         t_anim_tick_data.ui2_begin_flag  &WGL_ANIM_TYPE_BREATHE)
                       )
                       {
                           return TRUE;
                       }
                    pt_parent = PARENT(pt_parent);
                }
            }
        }
    }
    return FALSE;
}

static INT32 _get_anim_context(
    HANDLE_T                        h_widget,
    WGL_ANIM_ATTR_TYPE              e_anim_attr,
    WGL_WIDGET_ANIM_CONTEXT_T*      pt_anim)
{
    _WIDGET_T*      pt_widget;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret;
    WGL_MTX_OBJ_T*  pt_obj;   

    if(pt_anim == NULL)
    {    
        return WGLR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_widget, &e_type, (VOID **)&pt_widget);
    
    if (i4_ret != HR_OK || !IS_WIDGET_TYPE(e_type) || ST_DESTROYED(pt_widget))
    {
           DBG_ERROR(("{WGL} invalid widget handle. Line: %d\n", __LINE__));
           return WGLR_INV_HANDLE;
    }
    
    if(e_anim_attr == WGL_ANIM_ATTR_WIDGET)
    {
        *pt_anim = pt_widget->t_anim;
    }
    else if(e_anim_attr == WGL_ANIM_ATTR_WIDGET_ELEMENT)
    {
         pt_obj = (WGL_MTX_OBJ_T*)pt_widget->t_instance;
        *pt_anim = pt_obj->t_anim;
    }

    return WGLR_OK;
}

/*************************************Inner APIs****************************************************/
INT32 wgl_add_updated_region(HANDLE_T h_widget, GL_RECT_T *pt_rect)
{
    INT32           i4_ret;
    GL_RECT_T       t_rect;
    HANDLE_TYPE_T   e_type;
    _WIDGET_T       *pt_widget = NULL;
    
    if (h_widget == NULL_HANDLE || pt_rect == NULL)
    {
        return WGLR_INV_ARG;
    }

    i4_ret = handle_get_type_obj(h_widget, &e_type, (VOID **)&pt_widget);

    if (i4_ret != HR_OK || !IS_WIDGET_TYPE(e_type) || pt_widget == NULL)
    {
        return WGLR_INV_HANDLE;
    }

    t_rect = *pt_rect;

    _cal_anim_update_rect(pt_widget, &t_rect);
    
    /* when the update region list is full */
    if (_t_anim_mngr.ui4_update_rect_num == WGL_MAX_UPDATE_RECT_NUM)
    {
        /* naive method. simply union with the last rectangle */
        UNION_RECT(
            &(_t_anim_mngr.at_update_rect[_t_anim_mngr.ui4_update_rect_num - 1]),
            &(_t_anim_mngr.at_update_rect[_t_anim_mngr.ui4_update_rect_num - 1]),
            &t_rect);
    }
    else if (_t_anim_mngr.ui4_update_rect_num > WGL_MAX_UPDATE_RECT_NUM)
    {
        WGL_ASSERT(0);
    }
    else if(_t_anim_mngr.ui4_update_rect_num == 0)
    {
        _t_anim_mngr.at_update_rect[_t_anim_mngr.ui4_update_rect_num++] = t_rect;
    }
    else
    {
       UINT8 ui1_idx =0;
       for(;ui1_idx<_t_anim_mngr.ui4_update_rect_num;ui1_idx++)
       {
            if(RECT_INCLUDED(&_t_anim_mngr.at_update_rect[ui1_idx],&t_rect)||
               RECT_INCLUDED(&t_rect,&_t_anim_mngr.at_update_rect[ui1_idx])
               )
            {
                 UNION_RECT(
                    &(_t_anim_mngr.at_update_rect[ui1_idx]),
                    &(_t_anim_mngr.at_update_rect[ui1_idx]),
                    &t_rect);
                  break;
            }
       }
       if(ui1_idx == _t_anim_mngr.ui4_update_rect_num)
        {
        _t_anim_mngr.at_update_rect[_t_anim_mngr.ui4_update_rect_num++] = t_rect;
        }
    }

    return WGLR_OK;
}

INT32 wgl_begin_animation(HANDLE_T h_widget,  WGL_ANIMATION_DATA_T* pt_anim_data, WGL_ANIM_ATTR_TYPE e_anim_attr_type, WGL_ANIM_CONTEXT_T e_context)
{
    INT32           i4_ret = WGLR_OK;
    HANDLE_T        h_thread;
    _APP_INFO_T     *pt_app_info;
    _WIDGET_T       *pt_widget = NULL;
    HANDLE_TYPE_T           e_type;
    WGL_WIDGET_ANIM_CONTEXT_T    t_anim;    
    
    if (h_widget == NULL_HANDLE || pt_anim_data== NULL)
    {
        return WGLR_INV_ARG;
    }

    if (!_b_anim_rc_init)
    {
        return WGLR_SYS_RC_NOT_INIT;
    }
    
    /*start new animaiton here*/
    i4_ret = handle_get_type_obj(h_widget, &e_type, (VOID **)&pt_widget);

    if (i4_ret != HR_OK || !IS_WIDGET_TYPE(e_type) || ST_DESTROYED(pt_widget))
    {
        DBG_ERROR(("{WGL} invalid widget handle. Line: %d\n", __LINE__));
        return WGLR_INV_HANDLE;
    }
    if(e_anim_attr_type == WGL_ANIM_ATTR_WIDGET)
    {
        if(!_check_anim_support(pt_anim_data))
        {
            DBG_ERROR(("{WGL} animation type not support. Line: %d\n", __LINE__));
            return WGLR_ANIM_TYPE_NOT_SUPPORT;
        }
        if(_check_anim_conflict(pt_widget,pt_anim_data))
        {
            DBG_ERROR(("{WGL} animation type conflict. Line: %d\n", __LINE__));
            return WGLR_ANIM_TYPE_CONFLICT;
        }
    }
    if(_get_anim_context(h_widget,e_anim_attr_type,&t_anim) != WGLR_OK)
    {
        DBG_ERROR(("{WGL} animation handle invalid. Line: %d\n", __LINE__));
        return WGLR_INTERNAL_ERROR;
    }
    /*end previous animation first*/    
    if(wgl_anim_cmd_proc(t_anim, WGL_CMD_ANIM_ATTR_END,NULL, NULL)!= WGLR_OK)
    {
        DBG_ERROR(("{WGL} end previous animation failed. Line: %d\n", __LINE__));
        return WGLR_INTERNAL_ERROR;
    }

    CHECK_VALID(OSR_OK == x_thread_self(&h_thread), WGLR_OS_ERROR);
    
    if (e_context != ANIM_CONTEXT_APP_SYNC)
    {
        CHECK_VALID(WGLR_OK == _app_list_lock(), WGLR_LOCK_ERROR);

        /* find application */
        pt_app_info = _find_app_info(h_thread);

        if (pt_app_info)
        {
            BOOL                b_app_found = FALSE;
            _ANIM_APP_LIST_T    *pt_app_element;
            _ANIM_WDGT_LIST_T   *pt_wdgt_element;
            
            SLIST_FOR_EACH(pt_app_element, &_t_anim_mngr.t_app_list, t_link)
            {
                if (pt_app_element->pt_app_info == pt_app_info)
                {
                    b_app_found = TRUE;
                    break;
                }
            }

            /* NOT found */
            if (!b_app_found)
            {
                /* send the animation starting command to the widget */
                i4_ret = wgl_anim_cmd_proc(t_anim, WGL_CMD_ANIM_ATTR_BEGIN, WGL_PACK(pt_anim_data), WGL_PACK(0));

                if (i4_ret == WGLR_OK)
                {
                    /* add this app to the app_list */
                    pt_app_element = (_ANIM_APP_LIST_T *)WGL_MEM_ALLOC(sizeof(_ANIM_APP_LIST_T));

                    if (pt_app_element)
                    {
                        pt_app_element->e_anim_context    = e_context;
                        pt_app_element->pt_app_info       = pt_app_info;
                        pt_app_element->b_been_dispatched = TRUE;
                        pt_app_element->b_any_wdgt_anim_running = TRUE;
                        pt_app_element->ui4_tick          = 0;
                        pt_app_element->pt_app_info->ui1_anim_app_idx++;
                        
                        INIT_APP_LIST_ELEMENT(pt_app_element);
                        
                        _t_anim_mngr.ui4_app_count++;
                        
                        SLIST_INSERT_HEAD(pt_app_element, &_t_anim_mngr.t_app_list, t_link);

                        /* add the widget to this application */
                        pt_wdgt_element = (_ANIM_WDGT_LIST_T *)WGL_MEM_ALLOC(sizeof(_ANIM_WDGT_LIST_T));

                        if (pt_wdgt_element)
                        {
                            pt_wdgt_element->h_widget = h_widget;
                            pt_wdgt_element->e_anim_attr_type = e_anim_attr_type;
                            pt_app_element->ui4_wdgt_count++;
                            
                            SLIST_INSERT_HEAD(pt_wdgt_element, &pt_app_element->t_wdgt_list, t_link);

                            /* if this is the first app in the app_list, starting the tick engine */
                            if (_t_anim_mngr.ui4_app_count == 1)
                            {
                                _send_anim_tick_msg();
                            }
                        }
                        else
                        {
                            wgl_anim_cmd_proc(t_anim, WGL_CMD_ANIM_ATTR_ABORT,NULL,NULL);
                            _t_anim_mngr.ui4_app_count--;
                            SLIST_REMOVE(pt_app_element, t_link);
                            WGL_MEM_FREE(pt_app_element);
                            
                            i4_ret = WGLR_OUT_OF_MEMORY;
                        }
                    }
                    else
                    {
                        wgl_anim_cmd_proc(t_anim, WGL_CMD_ANIM_ATTR_ABORT,NULL,NULL);
                        i4_ret = WGLR_OUT_OF_MEMORY;
                    }
                }
                else
                {
                    DBG_ERROR(("{WGL} send WGL_CMD_ANIM_ATTR_BEGIN failed %d. Line %d\n", i4_ret, __LINE__));
                }
            }
            /* found */
            else
            {
                BOOL    b_wdgt_found = FALSE;

                pt_app_element->e_anim_context = e_context;
                pt_app_element->b_any_wdgt_anim_running = TRUE;
                
                /* check if the widget is inside of the widget list */
                SLIST_FOR_EACH(pt_wdgt_element, &pt_app_element->t_wdgt_list, t_link)
                {
                    if ( (pt_wdgt_element->h_widget == h_widget) && 
                         (pt_wdgt_element->e_anim_attr_type == e_anim_attr_type)
                       )
                    {
                        b_wdgt_found = TRUE;
                        break;
                    }
                }

                /* if not, add this widget in */
                if (!b_wdgt_found)
                {
                    /* send the animation starting command to the widget */
                    i4_ret = wgl_anim_cmd_proc(t_anim, WGL_CMD_ANIM_ATTR_BEGIN, WGL_PACK(pt_anim_data), WGL_PACK(pt_app_element->ui4_tick));

                    if (i4_ret == WGLR_OK)
                    {
                        pt_wdgt_element = (_ANIM_WDGT_LIST_T *)WGL_MEM_ALLOC(sizeof(_ANIM_WDGT_LIST_T));

                        if (pt_wdgt_element)
                        {
                            pt_wdgt_element->h_widget = h_widget;                            
                            pt_wdgt_element->e_anim_attr_type = e_anim_attr_type;
                            
                            pt_app_element->ui4_wdgt_count++;
                            
                            SLIST_INSERT_HEAD(pt_wdgt_element, &pt_app_element->t_wdgt_list, t_link);
                        }
                        else
                        {
                            wgl_anim_cmd_proc(t_anim, WGL_CMD_ANIM_ATTR_ABORT,NULL,NULL);
                            i4_ret = WGLR_OUT_OF_MEMORY;
                        }
                    }
                    else
                    {
                        DBG_ERROR(("{WGL} send WGL_CMD_ANIM_ATTR_BEGIN failed %d. Line: %d\n", i4_ret, __LINE__));
                    }
                }
                else
                {
                   i4_ret = wgl_anim_cmd_proc(t_anim, WGL_CMD_ANIM_ATTR_BEGIN, WGL_PACK(pt_anim_data), WGL_PACK(pt_app_element->ui4_tick));
                }
            }
        }
        /* not a valid application */
        else
        {
            i4_ret = WGLR_INV_CONTEXT;
        }

        CHECK_VALID(WGLR_OK == _app_list_unlock(), WGLR_UNLOCK_ERROR);
    }
    else if (e_context == ANIM_CONTEXT_APP_SYNC)
    {
        UINT32                  ui4_sys_tick = x_wgl_get_tick_interval();
        wgl_widget_proc_fct     pf_proc = NULL;
        _CANVAS_T               *pt_canvas = NULL;
        WGL_ANIM_COND_INF_T          e_anim_cond;
        UINT32      ui4_i;

        /* send begin animation command */
        i4_ret = wgl_anim_cmd_proc(t_anim, WGL_CMD_ANIM_ATTR_BEGIN, WGL_PACK(pt_anim_data), WGL_PACK(0));

        if (i4_ret == WGLR_OK)
        {
            UINT32      ui4_tick = 0;
            UINT32      ui4_start, ui4_end, ui4_os_tick_period = x_os_get_sys_tick_period(), ui4_total_time;
            
            pt_canvas = CANVAS(BRANCH(pt_widget));
            pf_proc   = MSGPROC(pt_widget);
            
            /* simulate event dispatching */
            while (pt_canvas && pf_proc)
            {
                
                ui4_start = x_os_get_sys_tick();
                
                CHECK_VALID(WGLR_OK == _app_list_lock(), WGLR_LOCK_ERROR);
                /* reset updated rect */
                _t_anim_mngr.ui4_update_rect_num = 0;

                /*Step 1, let aniamtion to handle tick message and cal update region*/
                _handle_anim_tick_message(h_widget,e_anim_attr_type,ui4_tick++);
                
                /*Step 2, paint canvas according to update regions calculated in step 1*/
                /* paint canvas with multiple dirty regions */
                for (ui4_i = 0; ui4_i < _t_anim_mngr.ui4_update_rect_num; ui4_i++)
                {
                    _paint_canvas(
                        pt_canvas,
                        &_t_anim_mngr.at_update_rect[ui4_i]);
                }
                /*Step 3, notify app's animation callback about animation state*/
                
                CHECK_VALID(WGLR_OK == _app_list_unlock(), WGLR_UNLOCK_ERROR);
                
                if(wgl_anim_cmd_proc(t_anim, WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&e_anim_cond), NULL) == WGLR_OK)
                {
                    /*should call _add_paint_rect_for_scale() before notify ap,
                                      to avoid the case that ap's callback will end animation and scale rect is still not added thus will cause scale's rect can not recover*/
                     if ( e_anim_cond.ui2_end_type == WGL_ANIM_TYPE_ALL)
                    {
                        _add_paint_rect_for_scale(pt_widget,e_anim_attr_type);
                    }
                    if(pt_widget->t_widget_anim_cb.pf_anim_callback != NULL
                       && e_anim_cond.ui2_end_type != WGL_ANIM_TYPE_NONE)
                    {
                        (*pt_widget->t_widget_anim_cb.pf_anim_callback)(h_widget,e_anim_attr_type,e_anim_cond,pt_widget->t_widget_anim_cb.pv_tag);
                    }
                    /*Step 4, check if it still needs to send tick message */
                    if ( e_anim_cond.ui2_end_type == WGL_ANIM_TYPE_ALL)
                    {
                        break;
                    }
                }
                ui4_end = x_os_get_sys_tick();

                ui4_total_time = (ui4_end - ui4_start) * ui4_os_tick_period;

                if (ui4_total_time < ui4_sys_tick)
                {
                    /* delay tick time */
                    x_thread_delay(ui4_sys_tick - ui4_total_time);
                }
            }
                           
        }
        else
        {
            DBG_ERROR(("{WGL} send WGL_CMD_ANIM_ATTR_BEGIN failed. (%d) %s L: %d\n", i4_ret, __FILE__, __LINE__));
        }
            }
    else
    {
        DBG_ERROR(("{WGL} invalid context type. Line: %d\n", __LINE__));
        i4_ret = WGLR_INV_ARG;
    }

    return i4_ret;
}

INT32 wgl_end_animation(HANDLE_T h_widget,  WGL_ANIM_ATTR_TYPE e_anim_attr_type)
{
    INT32           i4_ret = WGLR_OK;
    HANDLE_T        h_thread;
    _APP_INFO_T     *pt_app_info;
    WGL_WIDGET_ANIM_CONTEXT_T t_anim;                
    HANDLE_TYPE_T   e_type;
    GL_RECT_T t_update_rect = {0};
    _WIDGET_T*      pt_widget = NULL;    
    _WIDGET_T*      pt_parent = NULL;

    UINT32 ui4_i;

    if (h_widget == NULL_HANDLE ||
        (e_anim_attr_type!= WGL_ANIM_ATTR_WIDGET &&
         e_anim_attr_type != WGL_ANIM_ATTR_WIDGET_ELEMENT)
       )
    {
        return WGLR_INV_ARG;
    }

    if (!_b_anim_rc_init)
    {
        return WGLR_SYS_RC_NOT_INIT;
    }
    
    /* check handle of this animation widget */
    i4_ret = handle_get_type_obj(h_widget, &e_type, (VOID **)&pt_widget);

    if (i4_ret != HR_OK || !IS_WIDGET_TYPE(e_type) || ST_DESTROYED(pt_widget))
    {
        DBG_ERROR(("{WGL} invalid widget handle. Line: %d\n", __LINE__));
        return WGLR_INV_HANDLE;
    }

    /* for scale/breathe animation,need recover region covered by the last step of animation */
    
    _get_anim_context(h_widget,e_anim_attr_type,&t_anim);
     CHECK_VALID(OSR_OK == x_thread_self(&h_thread), WGLR_OS_ERROR);
    
    CHECK_VALID(WGLR_OK == _app_list_lock(), WGLR_LOCK_ERROR);
    
    _t_anim_mngr.ui4_update_rect_num = 0;
    _get_paint_rect_for_scale(pt_widget,e_anim_attr_type,&t_update_rect);
     if(!RECT_EMPTY(&t_update_rect))
     { 
          /*t_upgate_rect is based on screen coordinate, need transfer to widget's parent's coordinate*/    
          pt_parent = PARENT(pt_widget);
          while (pt_parent)
          {
            t_update_rect.i4_left -= pt_parent->t_rect.i4_left;
            t_update_rect.i4_right-= pt_parent->t_rect.i4_left;
            t_update_rect.i4_top-= pt_parent->t_rect.i4_top;
            t_update_rect.i4_bottom-= pt_parent->t_rect.i4_top;
            pt_parent = PARENT(pt_parent);
          }
          UNION_RECT(&t_update_rect, &t_update_rect, &pt_widget->t_rect);
          wgl_add_updated_region(pt_widget->h_this,&t_update_rect);
     }
     /* send the animation ending command to the widget */
      wgl_anim_cmd_proc(t_anim, WGL_CMD_ANIM_ATTR_END,NULL, NULL);
     
     for (ui4_i = 0; ui4_i < _t_anim_mngr.ui4_update_rect_num; ui4_i++)
     {
         _paint_canvas(
             pt_widget->pt_branch->pt_canvas,
             &_t_anim_mngr.at_update_rect[ui4_i]);
     }
     _t_anim_mngr.ui4_update_rect_num = 0;
     
     _remove_paint_rect_for_scale(pt_widget,e_anim_attr_type);
     
    /* find application */
    pt_app_info = _find_app_info(h_thread);

    if (pt_app_info)
    {
        BOOL                b_app_found = FALSE;
        _ANIM_APP_LIST_T    *pt_app_element;
        _ANIM_WDGT_LIST_T   *pt_wdgt_element;
        
        SLIST_FOR_EACH(pt_app_element, &_t_anim_mngr.t_app_list, t_link)
        {
            if (pt_app_element->pt_app_info == pt_app_info)
            {
                b_app_found = TRUE;
                break;
            }
        }

        /* NOT found */
        if (!b_app_found)
        {
            /* there is no animation for this widget already */
            i4_ret = WGLR_OK;
        }
        /* found */
        else
        {
            BOOL    b_wdgt_found = FALSE;
            
            /* check if the widget is inside of the widget list */
            SLIST_FOR_EACH(pt_wdgt_element, &pt_app_element->t_wdgt_list, t_link)
            {
                if ( (pt_wdgt_element->h_widget == h_widget) && 
                     (pt_wdgt_element->e_anim_attr_type == e_anim_attr_type)
                )
                {
                    b_wdgt_found = TRUE;
                    break;
                }
            }

            /* if not, everything is done */
            if (!b_wdgt_found)
            {
                /* there is no animation for this widget already */
                i4_ret = WGLR_OK;
            }
            else
            {
                /* remove this widget from the wdgt_list */
                SLIST_REMOVE(pt_wdgt_element, t_link);
                x_memset(pt_wdgt_element, 0, sizeof(_ANIM_WDGT_LIST_T));
                WGL_MEM_FREE(pt_wdgt_element);
                pt_app_element->ui4_wdgt_count--;

                /* if the wdgt list is empty, remove this app from the app_list */
                if (pt_app_element->ui4_wdgt_count == 0)
                {
                    SLIST_REMOVE(pt_app_element, t_link);
                    x_memset(pt_app_element, 0, sizeof(_ANIM_APP_LIST_T));
                    WGL_MEM_FREE(pt_app_element);
                    _t_anim_mngr.ui4_app_count--;
                }
           }
        }
    }
    /* not a valid application */
    else
    {
        i4_ret = WGLR_INV_CONTEXT;
    }

    CHECK_VALID(WGLR_OK == _app_list_unlock(), WGLR_UNLOCK_ERROR);

    return i4_ret;
}

/*************************************Public APIs****************************************************/
INT32 x_wgl_init_animation_resource(WGL_ANIM_CONFIG_T *pt_config)
{
    INT32       i4_ret;
    
    if (pt_config == NULL                                         ||
        pt_config->e_tick_mode == WGL_TICK_UNKNOWN                ||
        pt_config->ui4_tick_interval < x_os_get_sys_tick_period())
    {
        return WGLR_INV_ARG;
    }

    if(_b_anim_rc_init == TRUE)
    {
        _t_anim_mngr.e_tick_mode         = pt_config->e_tick_mode;
        _t_anim_mngr.ui4_tick_interval   = pt_config->ui4_tick_interval;
        return WGLR_OK;
    }
    /* create the app_list lock */
    i4_ret = x_sema_create(&_h_app_list_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);

    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("{WGL} create app_list lock failed. (%d) Line: %d\n", i4_ret, __LINE__));
        return WGLR_OS_ERROR;
    }

    /* create the app element lock */
    i4_ret = x_sema_create(&_h_app_element_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);

    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("{WGL} create app element lock failed. (%d) Line: %d\n", i4_ret, __LINE__));
        return WGLR_OS_ERROR;
    }

    /* create the animation message queue */
    i4_ret = x_msg_q_create(
        &_t_anim_mngr.h_anim_msg_q,
        WM_ANIMATION_MSG_Q_NAME,
        sizeof(_ANIM_MSG_T),
        WM_ANIMATION_NUM_OF_MSGS);

    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("{WGL} create animation message queue failed. (%d) Line: %d\n", i4_ret, __LINE__));
        return WGLR_OS_ERROR;
    }

    /* create the animation manager thread */
    i4_ret = x_thread_create(
        &_t_anim_mngr.h_anim_mngr,
        WM_ANIMATION_NAME,
        WM_ANIMATION_STACK_SIZE,
        WM_ANIMATION_MSG_PRIORITY,
        _animation_thread_main,
        0,
        NULL);

    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("{WGL} create animation thread failed. (%d) Line: %d\n", i4_ret, __LINE__));
        return WGLR_OS_ERROR;
    }


    /* setup the animation property */
    _t_anim_mngr.e_tick_mode         = pt_config->e_tick_mode;
    _t_anim_mngr.ui4_tick_interval   = pt_config->ui4_tick_interval;
    _t_anim_mngr.ui4_app_count       = 0;
    _t_anim_mngr.ui4_canvas_num      = 0;
    _t_anim_mngr.ui4_update_rect_num = 0;

    x_memset(_t_anim_mngr.apt_canvas, 0, WGL_MAX_CANVAS_RECORD_NUM * sizeof(_CANVAS_T *));
    x_memset(_t_anim_mngr.at_update_rect, 0, WGL_MAX_UPDATE_RECT_NUM * sizeof(GL_RECT_T));

    SLIST_INIT(&_t_anim_mngr.t_app_list);
    SLIST_INIT(&_t_anim_mngr.t_wdgt_list);

    _b_anim_rc_init = TRUE;
    return WGLR_OK;
}

INT32 x_wgl_set_animation_map(
                	HANDLE_T                h_widget,
                    UINT8                   ui1_map_count,     
                	const WGL_ANIM_ACTION_MAP_T*       pt_anim_map)
{
    HANDLE_TYPE_T           e_type;
    _WIDGET_T               *pt_widget = NULL;
    INT32                   i4_ret = WGLR_OK;

    if (h_widget == NULL_HANDLE || ui1_map_count>WGL_ANIM_MAX_MAP_CNT)
    {
        return WGLR_INV_ARG;
    }

    if (!_b_anim_rc_init)
    {
        return WGLR_SYS_RC_NOT_INIT;
    }
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    /* check handle of this animation widget */
    i4_ret = handle_get_type_obj(h_widget, &e_type, (VOID **)&pt_widget);

    if (i4_ret != HR_OK || !IS_WIDGET_TYPE(e_type) || ST_DESTROYED(pt_widget))
    {
        DBG_ERROR(("{WGL} invalid widget handle. Line: %d\n", __LINE__));
        i4_ret = WGLR_INV_HANDLE;
    }
    else
    { 
        /*release old data first*/
        UINT8 ui1_idx=0;
        if(pt_widget->ui1_map_count <= WGL_ANIM_MAX_MAP_CNT)
        {
            for(;ui1_idx<pt_widget->ui1_map_count;ui1_idx++)
            {
                if(pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_anim_data!= NULL)
                {
                     WGL_WIDGET_MEM_FREE(pt_widget,pt_widget->at_anim_map[ui1_idx].t_anim_data.ui2_count*sizeof(WGL_ANIM_DATA_INFO),pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_anim_data);
                }
                if(pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_path_mode!= NULL)
                {
                     WGL_WIDGET_MEM_FREE(pt_widget,pt_widget->at_anim_map[ui1_idx].t_anim_data.ui2_count*sizeof(WGL_ANIM_PATH_DATA_T),pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_path_mode);
                }
            }
            x_memset(pt_widget->at_anim_map,(UINT8)0,pt_widget->ui1_map_count*sizeof(WGL_ANIM_ACTION_MAP_T));
            pt_widget->ui1_map_count = 0;
        }
        if(pt_anim_map != NULL)
        {
            pt_widget->ui1_map_count = ui1_map_count;
            x_memcpy(pt_widget->at_anim_map,pt_anim_map,pt_widget->ui1_map_count*sizeof(WGL_ANIM_ACTION_MAP_T));
            for(ui1_idx=0;ui1_idx<pt_widget->ui1_map_count;ui1_idx++)
            {
                if(pt_widget->at_anim_map[ui1_idx].t_anim_data.ui4_flag & WGL_ANIM_DATA_FLAG_DATA_INF)
                {
                    pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_anim_data = WGL_WIDGET_MEM_ALLOC(pt_widget,pt_widget->at_anim_map[ui1_idx].t_anim_data.ui2_count*sizeof(WGL_ANIM_DATA_INFO));
                    if(pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_anim_data!= NULL)
                    {
                         x_memcpy(pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_anim_data,pt_anim_map->t_anim_data.pt_anim_data,pt_widget->at_anim_map[ui1_idx].t_anim_data.ui2_count*sizeof(WGL_ANIM_DATA_INFO));
                    }
                }
                
                if(pt_widget->at_anim_map[ui1_idx].t_anim_data.ui4_flag & WGL_ANIM_DATA_FLAG_PATH_MODE_INF)  
                {
                    pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_path_mode= WGL_WIDGET_MEM_ALLOC(pt_widget,pt_widget->at_anim_map[ui1_idx].t_anim_data.ui2_count*sizeof(WGL_ANIM_PATH_DATA_T));
                    if(pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_path_mode!= NULL)
                    {
                        x_memcpy(pt_widget->at_anim_map[ui1_idx].t_anim_data.pt_path_mode,pt_anim_map->t_anim_data.pt_path_mode,pt_widget->at_anim_map[ui1_idx].t_anim_data.ui2_count*sizeof(WGL_ANIM_PATH_DATA_T));
                    }
                }
            }
        }
    }
    
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return i4_ret;
}

INT32 x_wgl_start_animation(
        HANDLE_T                h_widget,
        WGL_ANIMATION_DATA_T*   pt_anim_data,
        WGL_ANIM_CONTEXT_T      e_context)
{
    return wgl_begin_animation(h_widget,pt_anim_data,WGL_ANIM_ATTR_WIDGET,e_context);
}

INT32 x_wgl_end_animation( 
                    HANDLE_T            h_widget,
                    VOID                *pv_param1,
                    VOID                *pv_param2)
{
    return wgl_end_animation(h_widget,WGL_ANIM_ATTR_WIDGET);
}

INT32 x_wgl_set_animation_max_img_set(HANDLE_T h_widget,UINT16 ui2_max_img_set)
{
    WGL_WIDGET_ANIM_CONTEXT_T t_anim;
    INT32 i4_ret;

    if (h_widget == NULL_HANDLE)
    {
       return WGLR_INV_ARG;
    }

    if (!_b_anim_rc_init)
    {
       return WGLR_SYS_RC_NOT_INIT;
    }
    i4_ret = _get_anim_context(h_widget,WGL_ANIM_ATTR_WIDGET,&t_anim);
    if(WGLR_OK == i4_ret)
    {
       return wgl_anim_cmd_proc(t_anim,WGL_CMD_ANIM_ATTR_SET_MAX_IMG_SEQ,WGL_PACK(ui2_max_img_set),NULL);
    }
    return i4_ret;

}
INT32 x_wgl_set_animation_img_set(
       HANDLE_T                h_widget,
       UINT8                   ui1_img_set_index,     
       const WGL_ANIM_IMG_SEQ_T*       pt_anim_img_seq)
{
    WGL_WIDGET_ANIM_CONTEXT_T t_anim;
    INT32 i4_ret;
    
    if (h_widget == NULL_HANDLE)
    {
        return WGLR_INV_ARG;
    }

    if (!_b_anim_rc_init)
    {
        return WGLR_SYS_RC_NOT_INIT;
    }
    i4_ret = _get_anim_context(h_widget,WGL_ANIM_ATTR_WIDGET,&t_anim);
    if(WGLR_OK == i4_ret)
    {
        return wgl_anim_cmd_proc(t_anim,WGL_CMD_ANIM_ATTR_SET_IMG_SEQ,WGL_PACK(ui1_img_set_index),WGL_PACK(pt_anim_img_seq));
    }
    return i4_ret;
}

INT32 x_wgl_set_anim_callback(
        HANDLE_T h_widget,
        wgl_app_anim_callback_fct pf_anim_nfy_fct,
        void* pv_private_tag)
{
    _WIDGET_T*      pt_widget = NULL;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret = WGLR_OK;  
    
    if (h_widget == NULL_HANDLE)
    {
        return WGLR_INV_ARG;
    }

    if (!_b_anim_rc_init)
    {
        return WGLR_SYS_RC_NOT_INIT;
    }
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

      if (HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget) && 
        IS_WIDGET_TYPE(e_type)                                               &&
        !ST_DESTROYED(pt_widget))
        {
             pt_widget->t_widget_anim_cb.pf_anim_callback = pf_anim_nfy_fct;
             pt_widget->t_widget_anim_cb.pv_tag = pv_private_tag;
        }
      else
          {
              i4_ret =  WGLR_INV_HANDLE;
              DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
          }
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);

}

INT32 x_wgl_get_anim_callback(
                    HANDLE_T h_widget,
                    WGL_ANIM_CB_T* pt_anim_callback)
{
    _WIDGET_T*      pt_widget = NULL;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret = WGLR_OK;  
    
    if (h_widget == NULL_HANDLE || pt_anim_callback == NULL)
    {
        return WGLR_INV_ARG;
    }

    if (!_b_anim_rc_init)
    {
        return WGLR_SYS_RC_NOT_INIT;
    }
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

      if (HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget) && 
        IS_WIDGET_TYPE(e_type)                                               &&
        !ST_DESTROYED(pt_widget))
        {
             pt_anim_callback->pf_anim_callback = pt_widget->t_widget_anim_cb.pf_anim_callback;
             pt_anim_callback->pv_tag = pt_widget->t_widget_anim_cb.pv_tag ;
        }
      else
          {
              i4_ret =  WGLR_INV_HANDLE;
              DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
          }
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);
}

INT32    x_wgl_get_anim_status(
                    HANDLE_T            h_widget,
                    UINT16              *pui2_anim_type)
{
    _WIDGET_T*      pt_widget = NULL;
    HANDLE_TYPE_T   e_type;
    INT32           i4_ret = WGLR_OK;      
    WGL_ANIM_TICK_DATA t_anim_tick_data;
    
    if (h_widget == NULL_HANDLE || pui2_anim_type==NULL)
    {
        return WGLR_INV_ARG;
    }

    if (!_b_anim_rc_init)
    {
        return WGLR_SYS_RC_NOT_INIT;
    }
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

      if (HR_OK == handle_get_type_obj(h_widget, &e_type, (VOID**) &pt_widget) && 
        IS_WIDGET_TYPE(e_type)                                               &&
        !ST_DESTROYED(pt_widget))
        {
             i4_ret = wgl_anim_cmd_proc(ANIM(pt_widget),WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_anim_tick_data));
             if(i4_ret == WGLR_OK)
             {
                *pui2_anim_type = t_anim_tick_data.ui2_begin_flag;
             }
        }
      else
          {
              i4_ret =  WGLR_INV_HANDLE;
              DBG_ERROR(("{WGL} ERR: Invalid canvas handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
          }
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    
    return (i4_ret);

}

WGL_TICK_MODE_T x_wgl_get_tick_mode(VOID)
{
    return _t_anim_mngr.e_tick_mode;
}


UINT32 x_wgl_get_tick_interval(VOID)
{
    return _t_anim_mngr.ui4_tick_interval;
}

VOID x_wgl_set_tick_interval(UINT32 ui4_data)
{
    _t_anim_mngr.ui4_tick_interval = ui4_data;
}

INT32 x_wgl_config_animation(WGL_ANIM_CONFIG_T *pt_config)
{
    if (pt_config == NULL                                         ||
        pt_config->e_tick_mode == WGL_TICK_UNKNOWN                ||
        pt_config->ui4_tick_interval < x_os_get_sys_tick_period())
    {
        return WGLR_INV_ARG;
    }

    
    return WGLR_OK;
}
#endif

#ifdef WGL_COORDINATE_INVERSE
INT32 x_wgl_set_coordinate_reverse(BOOL b_coordinate_reverse,UINT8 ui1_repaint)
{
    INT32 i4_ret  =WGLR_OK;    
    _CANVAS_T*  pt_canvas;
    BOOL b_change = FALSE;
    
    CHECK_VALID(WGLR_OK == _wgl_lock(), WGLR_LOCK_ERROR);

    if(_b_coordinate_reverse != b_coordinate_reverse)
    {   
        _b_coordinate_reverse = b_coordinate_reverse;
        b_change = TRUE;

        SLIST_FOR_EACH(pt_canvas, &_t_canvas_lst, t_next)
        {
            _BRANCH_T*  pt_branch;
            DLIST_FOR_EACH(pt_branch, &(pt_canvas->t_branch_lst), t_link_on_canvas)
            {
                _WIDGET_T*  pt_widget;
                /* process each branch */
                for (pt_widget = FIRST(pt_branch); pt_widget; pt_widget = SIBLING(pt_widget))
                {
                    if (!ST_DESTROYED(pt_widget))
                    {
                        _widget_tree_inverse_x_coordinate_pos(pt_widget,pt_canvas->ui4_width,_b_coordinate_reverse);
                    }
                }
            }
        }
    }
    CHECK_VALID(WGLR_OK == _wgl_unlock(), WGLR_UNLOCK_ERROR);
    if(b_change)
    {   
        SLIST_FOR_EACH(pt_canvas, &_t_canvas_lst, t_next)
        {
            _BRANCH_T*  pt_branch;
            DLIST_FOR_EACH(pt_branch, &(pt_canvas->t_branch_lst), t_link_on_canvas)
            {
                _WIDGET_T*  pt_widget;
                /* process each branch */
                for (pt_widget = FIRST(pt_branch); pt_widget; pt_widget = SIBLING(pt_widget))
                {
                    if (!ST_DESTROYED(pt_widget))
                    {
                        _widget_tree_notify_move_msg(pt_widget);
                    }
                }
            }
        }
    }
    
    if(b_change &&
       (ui1_repaint == WGL_SYNC_AUTO_REPAINT|| ui1_repaint == WGL_ASYNC_AUTO_REPAINT))
    {
        BOOL b_sync = (ui1_repaint == WGL_SYNC_AUTO_REPAINT)?TRUE:FALSE;
        
        SLIST_FOR_EACH(pt_canvas, &_t_canvas_lst, t_next)
        {
            x_wgl_repaint(pt_canvas->h_this,NULL,b_sync);
        }
    }
    return i4_ret;
}
#endif

