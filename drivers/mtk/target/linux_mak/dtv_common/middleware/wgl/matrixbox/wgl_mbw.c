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
 *
 * SWAuthor: Chun Zhou, Tinghong Liang
 * Description:
 *
 *---------------------------------------------------------------------------*/

#include "os/inc/x_os.h"
#include "dbg/def_dbg_level_mw.h"
#include "wgl/wgl_sets.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/wgl.h"
#include "wgl/_wgl.h"
#include "wgl/wgl_tools.h"
#include "handle/handle.h"
#include "aee/aee.h"
#include "wgl/u_wgl_bdr_common.h"
#include "wgl/wgl_bdr_internal.h"

#include "wgl/matrixbox/x_wgl_mbw.h"
#include "wgl/matrixbox/wgl_mbw.h"
#include "wgl/matrixbox/wgl_mbw_cli.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_mbw_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

#ifdef DEBUG

#define MBW_CHECK_VALID(_expr, _ret_val)                                                      \
    do {                                                                                      \
        if (!(_expr))                                                                         \
        {                                                                                     \
            DBG_ERROR(("{WGL(MBW)} ERR: %s (%d) at L%d\n\n", __FILE__, _ret_val, __LINE__));  \
            return (_ret_val);                                                                \
        }                                                                                     \
    } while (0)


#define GET_LOCK(_h_mbw, _ppt_ctx)         \
    {                                      \
        INT32  i4_ret_get_lock;            \
        i4_ret_get_lock = wgl_get_instance((_h_mbw), (WGL_WIDGET_CONTEXT_HANDLE_T*)(_ppt_ctx));\
        if(WGL_IS_FAIL(i4_ret_get_lock))   \
        {                                  \
            DBG_ERROR(("{WGL(MBW)} ERR: GET_LOCK()=%d File:%s Line:%d\n", (i4_ret_get_lock), __FILE__, __LINE__));\
            return (i4_ret_get_lock);      \
        }                                  \
    }

#define REL_LOCK(_h_mbw, _pt_ctx)           \
    {                                       \
        INT32   i4_ret_rel_lock;            \
        i4_ret_rel_lock = wgl_release_instance((_h_mbw), (WGL_WIDGET_CONTEXT_HANDLE_T)(_pt_ctx));\
        (_pt_ctx) = NULL;                   \
        if(WGL_IS_FAIL(i4_ret_rel_lock))    \
        {                                   \
            DBG_ERROR(("{WGL(MBW)} ERR: RELEASE_LOCK()=%d File:%s Line:%d\n", i4_ret_rel_lock, __FILE__, __LINE__));\
            return i4_ret_rel_lock;         \
        }                                   \
    }
#else

#define MBW_CHECK_VALID(_expr, _ret_val)   \
    do {                                   \
        if (!(_expr))                      \
        {                                  \
            return (_ret_val);             \
        }                                  \
    } while (0)


#define GET_LOCK(_h_mbw, _ppt_ctx)         \
    {                                      \
        INT32  i4_ret_get_lock;            \
        i4_ret_get_lock = wgl_get_instance((_h_mbw), (WGL_WIDGET_CONTEXT_HANDLE_T*)(_ppt_ctx));\
        if(WGL_IS_FAIL(i4_ret_get_lock))   \
        {                                  \
            return (i4_ret_get_lock);      \
        }                                  \
    }

#define REL_LOCK(_h_mbw, _pt_ctx)          \
    {                                      \
        INT32   i4_ret_rel_lock;           \
        i4_ret_rel_lock = wgl_release_instance((_h_mbw), (WGL_WIDGET_CONTEXT_HANDLE_T)(_pt_ctx));\
        (_pt_ctx) = NULL;                  \
        if(WGL_IS_FAIL(i4_ret_rel_lock))   \
        {                                  \
            return i4_ret_rel_lock;        \
        }                                  \
    }
#endif

/*extra flags*/
#define MBW_VP_BK_CLR_NOT_INIT          ((UINT8)0x01)
#define MBW_VP_BK_IMG_NOT_INIT          ((UINT8)0x02)

/*vp info*/
typedef struct _WGL_MBW_VP_INFO_T
{
    UINT32      ui4_idx_vp_first;      /*first item index in vp, which is based on the total item number*/
    UINT32      ui4_idx_vp_last;       /*last item index in vp, which is based on the total item number*/
} WGL_MBW_VP_INFO_T;

/*event hdlrs union*/
typedef struct _WGL_MBW_EVN_HDLR_DATA_T
{
    WGL_MBW_EVN_HDLR_TYPE_T e_evn_type;
    union
    {
        WGL_MBW_EVN_HDLR_INFO_NVGT_IN_VP_T        t_evn_hdlr_nvgt_in_vp;
        WGL_MBW_EVN_HDLR_INFO_NVGT_WITH_VP_CHG_T  t_evn_hdlr_nvgt_with_vp_chg;
        WGL_MBW_EVN_HDLR_INFO_SELECT_T            t_evn_hdlr_select;
        WGL_MBW_EVN_HDLR_INFO_DESEL_T             t_evn_hdlr_deselect;
    } u_evn_hdlr_data;
} WGL_MBW_EVN_HDLR_DATA_T;

/*record item need repaint dirty region*/
typedef struct _WGL_MBW_ITEM_EX_REPAINT_REGION
{
    BOOL        b_is_dirty;     /*indicates the item if has dirty region*/
    GL_RECT_T   t_dirty_reg;    /*indicates the dirty region,the coordinate is based on MatrixBox*/
} WGL_MBW_ITEM_EX_REPAINT_REGION;


typedef struct _WGL_MBW_CONTROL_LOGIC_T
{
#ifdef CLI_LVL_ALL     
    UINT32                    ui4_mem_size;/*this field must be the first field in the structure*/
#endif   
    HANDLE_T                  h_mbw;                    /*MatrixBox handle*/
    HANDLE_T                  h_mbw_vp;                 /*MatrixBox viewport handle*/
    UINT8                     ui1_extra_flag;           /*Flag*/
    UINT32                    ui4_hlt_idx;              /*Highlight item index, which is based on the total item number*/
    UINT32                    ui4_col_num;              /*column number*/
    UINT32                    ui4_row_num;              /*row number*/
    UINT32                    ui4_style;                /*MatrixBox style*/
    UINT32                    ui4_item_total_num;       /*total item number in data model*/
    UINT32                    ui4_hlt_margin;           /*indicates where to scroll, only valid in SCROLL mode*/
    UINT16                    ui2_mbw_state;            /*MatrixBox's state*/
    BOOL                      b_is_transition;          /*indicates whether MatrixBox is in TRANSTION state*/
    MBW_CTX_T                 pv_mbw_ctx;               /*instance reference of the matrixbox */
    VOID*                     pv_dm_data;               /*data model's private data*/
    WGL_MBW_ITEM_CTNR_INFO_T  t_hlt_item_ctnr_info;     /*Highlight item container handle*/
    BOOL                      b_vp_changed;             /*indicated vp is change, need repaint extend dirty region */
    WGL_MBW_ITEM_EX_REPAINT_REGION* at_need_repaint_ex_region;/*Indicates item's need repaited extra dirty region*/
    WGL_MBW_ITEM_CTNR_INFO_T* at_item_ctnr_info;        /*items' container info array for current viewport*/
    WGL_MBW_ITEM_STATE_T*     aui4_item_state;          /*items' state array*/
    WGL_MBW_ITEM_DATA_T*      at_item_data;             /*items' data array which includes item's own nvgt map presently*/
    WGL_MBW_KEY_MAP_T         t_key_map;                /*MatrixBox's key map*/
    WGL_MBW_VP_INFO_T         t_vp_info;                /*viewport info*/
    WGL_MBW_HLT_ITEM_INFO_T   t_hlt_item_info;          /*highlight item info*/
    WGL_MBW_NVGT_CUSTOM_T     t_nvgt_custom;            /*customized move behavior at boundary of viewport*/
    WGL_MBW_DATA_MODEL_INFO_T t_data_model;             /*data model info*/
    WGL_MBW_KEY_HDLR_INFO_T   t_key_hdlr_info;          /*key handler info*/
    WGL_MBW_EVN_HDLR_INFO_T   t_evn_hdlr_info;          /*event handler info*/
    WGL_MBW_EVN_HDLR_DATA_T   t_evn_hdlr_data;          /*event handler data*/
    WGL_COLOR_STD_SET_T       t_vp_bk_clr;              /*viewport background color info*/
    WGL_IMG_STD_SET_T         t_vp_bk_img;              /*viewport background img info*/
} WGL_MBW_CONTROL_LOGIC_T;


static const WGL_MBW_KEY_MAP_T  MBW_DEF_KEY_MAP =
{
    1,
    BTN_SELECT,
    BTN_CURSOR_LEFT,
    BTN_CURSOR_RIGHT,
    BTN_CURSOR_UP,
    BTN_CURSOR_DOWN,
    0,
    0
};

#define MBW_IS_VALID_ITEM_WIDTH(_vp_width, _item_width, _horiz_dist, _col_num)           \
    ((((_col_num) -1) * (_horiz_dist) + (_col_num) * (_item_width)) <= (_vp_width)) ? TRUE : FALSE


#define MBW_IS_VALID_ITEM_HEIGHT(_vp_height, _item_height, _verti_dist, _row_num)        \
    ((((_row_num) -1) * (_verti_dist) + (_row_num) * (_item_height)) <= (_vp_height)) ? TRUE : FALSE

#define MBW_GET_PROPER_FIRST_ITEM_POS_X(_vp_width, _item_width, _horiz_dist, _col_num)   \
    ((_vp_width) - (((_col_num) -1) * (_horiz_dist) + (_col_num) * (_item_width))) >> 1

#define MBW_GET_PROPER_FIRST_ITEM_POS_Y(_vp_height, _item_height, _verti_dist, _row_num) \
    ((_vp_height) - (((_row_num) -1) * (_verti_dist) + (_row_num) * (_item_height))) >> 1


#define HAS_STYLE(_pt_ctx, _style)  \
    ((_pt_ctx)->ui4_style & (_style) )

#define HAS_STATE(_pt_ctx, _state)  \
    ((_pt_ctx)->ui2_mbw_state & (_state) )

#define HAS_KEY_HANDLER_MASK(_pt_ctx, _key_handler_mask)     \
    ((_pt_ctx)->t_key_hdlr_info.ui4_key_hdlr_mask & (_key_handler_mask))

#define HAS_EVN_HANDLER_MASK(_pt_ctx, _evn_handler_mask)     \
    ((_pt_ctx)->t_evn_hdlr_info.ui4_evn_hdlr_mask & (_evn_handler_mask))

#define MBW_SET_CW_STYLE(_ui4_style, _ui4_cw_style)          \
    do {                                                     \
        if ((_ui4_cw_style) & WGL_MBW_CW_STL_NO_IMG_UI)      \
        {                                                    \
            (_ui4_style) |= WGL_STL_GL_NO_IMG_UI;            \
        }                                                    \
        if ((_ui4_cw_style) & WGL_MBW_CW_STL_NO_BK)          \
        {                                                    \
            (_ui4_style) |= WGL_STL_GL_NO_BK;                \
        }                                                    \
        if ((_ui4_cw_style) & WGL_MBW_CW_STL_USE_SYS_IMG)    \
        {                                                    \
            (_ui4_style) |= WGL_STL_GL_USE_SYS_IMG;          \
        }                                                    \
    } while(0)



#define MBW_IS_VALID_KEY_HDLR_TYPE(_e_key_hdlr_type)                     \
        (((_e_key_hdlr_type) == WGL_MBW_KEY_HDLR_TYPE_SELECT)    ||      \
         ((_e_key_hdlr_type) == WGL_MBW_KEY_HDLR_TYPE_NVGT_LEFT) ||      \
         ((_e_key_hdlr_type) == WGL_MBW_KEY_HDLR_TYPE_NVGT_RIGHT)||      \
         ((_e_key_hdlr_type) == WGL_MBW_KEY_HDLR_TYPE_NVGT_UP)   ||      \
         ((_e_key_hdlr_type) == WGL_MBW_KEY_HDLR_TYPE_NVGT_DOWN) ||      \
         ((_e_key_hdlr_type) == WGL_MBW_KEY_HDLR_TYPE_NVGT_PAGE_UP) ||   \
         ((_e_key_hdlr_type) == WGL_MBW_KEY_HDLR_TYPE_NVGT_PAGE_DOWN)) ? \
         TRUE : FALSE

#define MBW_IS_VALID_EVN_HDLR_TYPE(_e_evn_hdlr_type)                       \
        (((_e_evn_hdlr_type) == WGL_MBW_EVN_HDLR_TYPE_SELECT)    ||        \
         ((_e_evn_hdlr_type) == WGL_MBW_EVN_HDLR_TYPE_DESEL)     ||        \
         ((_e_evn_hdlr_type) == WGL_MBW_EVN_HDLR_TYPE_NVGT_IN_VP)||        \
         ((_e_evn_hdlr_type) == WGL_MBW_EVN_HDLR_TYPE_NVGT_WITH_VP_CHG)) ? \
         TRUE : FALSE

#define MBW_IS_VALID_NVGT_DIR(_e_nvgt_dir)                    \
        (((_e_nvgt_dir) == WGL_MBW_NVGT_DIR_LEFT)     ||      \
         ((_e_nvgt_dir) == WGL_MBW_NVGT_DIR_RIGHT)    ||      \
         ((_e_nvgt_dir) == WGL_MBW_NVGT_DIR_UP)       ||      \
         ((_e_nvgt_dir) == WGL_MBW_NVGT_DIR_DOWN)     ||      \
         ((_e_nvgt_dir) == WGL_MBW_NVGT_DIR_PG_UP)    ||      \
         ((_e_nvgt_dir) == WGL_MBW_NVGT_DIR_PG_DOWN)) ?       \
         TRUE : FALSE

#define MBW_IS_VALID_ITEM_DATA(_t_item_data)                                                                \
        (((_t_item_data).ui1_strct_ver != 1) ||                                                             \
         (((_t_item_data).ui4_data_mask & WGL_MBW_ITEM_DATA_MASK_NVGT_MAP) ?                                \
          (((_t_item_data).t_item_nvgt_map.ui1_strct_ver != 1) ||                                           \
           ((((_t_item_data).t_item_nvgt_map.ui4_item_nvgt_dir_mask & WGL_MBW_NVGT_DIR_MASK_DOWN) &&        \
             ((_t_item_data).t_item_nvgt_map.ui4_item_idx_nvgt_down >= WGL_MBW_MAX_ITEM_NUM)) ||            \
            (((_t_item_data).t_item_nvgt_map.ui4_item_nvgt_dir_mask & WGL_MBW_NVGT_DIR_MASK_UP) &&          \
             ((_t_item_data).t_item_nvgt_map.ui4_item_idx_nvgt_up >= WGL_MBW_MAX_ITEM_NUM)) ||              \
            (((_t_item_data).t_item_nvgt_map.ui4_item_nvgt_dir_mask & WGL_MBW_NVGT_DIR_MASK_LEFT) &&        \
             ((_t_item_data).t_item_nvgt_map.ui4_item_idx_nvgt_left >= WGL_MBW_MAX_ITEM_NUM)) ||            \
            (((_t_item_data).t_item_nvgt_map.ui4_item_nvgt_dir_mask & WGL_MBW_NVGT_DIR_MASK_RIGHT) &&       \
             ((_t_item_data).t_item_nvgt_map.ui4_item_idx_nvgt_right >= WGL_MBW_MAX_ITEM_NUM)) ||           \
            (((_t_item_data).t_item_nvgt_map.ui4_item_nvgt_dir_mask & WGL_MBW_NVGT_DIR_MASK_PG_DOWN) &&     \
             ((_t_item_data).t_item_nvgt_map.ui4_item_idx_nvgt_page_down >= WGL_MBW_MAX_ITEM_NUM)) ||       \
            (((_t_item_data).t_item_nvgt_map.ui4_item_nvgt_dir_mask & WGL_MBW_NVGT_DIR_MASK_PG_UP) &&       \
             ((_t_item_data).t_item_nvgt_map.ui4_item_idx_nvgt_page_up >= WGL_MBW_MAX_ITEM_NUM)))) : (0))) ?\
         FALSE : TRUE

#define MBW_GET_ITEM_DATA(_h_mbw, _pt_ctrl_logic, _data_mask, _ui4_idx_from, _ui4_idx_to, _at_item_data)  \
    do {                                                                 \
        (_pt_ctrl_logic)->t_data_model.t_fct_tbl.pf_get_item_data(       \
                (_pt_ctrl_logic)->pv_dm_data,                            \
                (_h_mbw),                                                \
                (_ui4_idx_from),                                         \
                (_ui4_idx_to),                                           \
                (_data_mask),                                            \
                (_at_item_data));                                        \
    } while(0)

#define MBW_GET_ITEM_STATE(_h_mbw, _pt_ctrl_logic, _ui4_idx_from, _ui4_idx_to, _aui4_item_state)  \
    do {                                                                 \
        (_pt_ctrl_logic)->t_data_model.t_fct_tbl.pf_get_item_state(      \
                (_pt_ctrl_logic)->pv_dm_data,                            \
                (_h_mbw),                                                \
                (_ui4_idx_from),                                         \
                (_ui4_idx_to),                                           \
                (_aui4_item_state));                                     \
    } while(0)


#define MBW_UPDATE_ITEM_CTNR(_h_mbw, _pt_ctrl_logic, _update_mask, _ui4_idx_from, _ui4_idx_to, _at_item_ctnr_info)   \
    do {                                                                 \
        (_pt_ctrl_logic)->t_data_model.t_fct_tbl.pf_update_item_ctnr(    \
                (_pt_ctrl_logic)->pv_dm_data,                            \
                (_h_mbw),                                                \
                (_pt_ctrl_logic)->ui2_mbw_state,                         \
                (_update_mask),                                          \
                (_ui4_idx_from),                                         \
                (_ui4_idx_to),                                           \
                (_at_item_ctnr_info));                                   \
    }while(0)


#define MBW_UPDATE_HLT_ITEM_CTNR(_h_mbw, _pt_ctrl_logic, _update_mask)      \
    do {                                                                    \
        (_pt_ctrl_logic)->t_data_model.t_fct_tbl.pf_update_hlt_item_ctnr(   \
                (_pt_ctrl_logic)->pv_dm_data,                               \
                (_h_mbw),                                                   \
                (_pt_ctrl_logic)->ui2_mbw_state,                            \
                (_update_mask),                                             \
                (_pt_ctrl_logic)->ui4_hlt_idx,                              \
                &((_pt_ctrl_logic)->t_hlt_item_ctnr_info));                 \
    } while(0)


/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/


 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static  INT32  _mbw_on_item_select(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx,
    UINT32                      ui4_item_idx);

static  INT32  _mbw_default_nvgt_key_hdlr(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx,
    WGL_MBW_KEY_HDLR_TYPE_T     e_key_hdlr_type,
    UINT32                      ui4_data);

static  INT32  _mbw_get_normal_nvgt_info(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    UINT32                      ui4_idx_from,
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir,
    WGL_MBW_NVGT_INFO_T*        pt_nvgt_info);

static  INT32  _mbw_get_page_up_down_nvgt_info(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    UINT32                      ui4_idx_from,
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir,
    WGL_MBW_NVGT_INFO_T*        pt_nvgt_info);

static INT32   _mbw_get_page_up_down_custom_nvgt_info (
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    UINT32                      ui4_idx_from,
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir,
    WGL_MBW_NVGT_INFO_T*        pt_nvgt_info);
    
static  INT32  _mbw_gain_new_item_for_page_up_down(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    INT32                       i4_idx_from,
    INT32*                      pi4_idx_to,
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir,
    WGL_MBW_VP_INFO_T           t_vp_info,
    BOOL                        b_in_orig_vp,
    BOOL*                       pb_valid);

static  INT32  _mbw_setup_nvgt_info(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    WGL_MBW_NVGT_INFO_T*        pt_nvgt_info,
    BOOL*                       pb_navigable,
    BOOL*                       pb_vp_changed,
    BOOL*                       pb_wrap_over,
    UINT32                      ui4_idx_orig,
    UINT32                      ui4_idx_new,
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir);

static  INT32  _mbw_gain_new_item_by_map(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    INT32                       i4_idx_from,
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir,
    INT32*                      ui4_idx_to,
    BOOL*                       b_use_item_kmap);

static  INT32  _mbw_calculate_nvgt_step(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    INT32                       i4_idx_from,
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir,
    INT32*                      pi4_nav_step);

static  INT32  _mbw_on_wrap_over(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    INT32*                      pi4_idx_to,
    BOOL*                       pb_wrap_over,
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir);

static  INT32  _mbw_set_vp_bk_image(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic);

static  INT32  _mbw_set_vp_bk_color(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic);

static  INT32  _mbw_get_item_ctnr_info(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    UINT32                      ui4_item_idx,
    WGL_MBW_ITEM_CTNR_INFO_T**  ppt_item_ctnr_info);

static  INT32  _mbw_update_vp_to_show_item(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    UINT32                      ui4_item_idx);

static  INT32  _mbw_show_hlt_item(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic);

static  INT32  _mbw_hide_hlt_item(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic);

static  INT32  _mbw_clear_item_dirty_region(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic);
/*-----------------------------------------------------------------------------
 * Init cli
 *---------------------------------------------------------------------------*/

INT32 x_wgl_mbw_init()
{
    INT32 i4_ret;
    /*init cli*/
    i4_ret = wgl_mbw_cli_init();
    WGL_MBW_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(MBW)} ERR: Fail to init MBW CLI. (%d) at L%d\n\n", i4_ret, __LINE__)));
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/

INT32 x_wgl_mbw_create (
     HANDLE_T                h_container,
     const WGL_MBW_INIT_T*   pt_init_info,
     const GL_RECT_T*        pt_rect,
     wgl_widget_proc_fct     pf_wdgt_proc,
     VOID*                   pv_tag,
     HANDLE_T*               ph_mbw)
{
    /*local variable declaration*/
    INT32                    i4_ret;
    INT32                    i4_item_width;
    INT32                    i4_item_height;
    INT32                    i4_horiz_dist;
    INT32                    i4_verti_dist;
    INT32                    i4_first_item_pos_x;
    INT32                    i4_first_item_pos_y;
    UINT32                   ui4_x;
    UINT32                   ui4_y;
    UINT32                   ui4_col_num;
    UINT32                   ui4_row_num;
    UINT32                   ui4_vp_capacity;
    UINT32                   ui4_cw_style;
    SIZE_T                   z_mem_size;
    GL_RECT_T                t_mbw_vp_rect = {0};
    GL_RECT_T                t_item_cntr_rect = {0};
    GL_RECT_T                t_hlt_item_cntr_rect = {0};
    WGL_MBW_EXTRA_MARGIN_T   t_ex_magin = {0};
    WGL_MBW_DM_INIT_INFO_T   t_mbw_dm_init_info = {0};
    wgl_widget_proc_fct      pf_wgl_mbw_proc;
    WGL_MBW_CONTROL_LOGIC_T* pt_ctrl_logic;

    /*check parameter*/
    MBW_CHECK_VALID((h_container != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_rect != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((!WGL_RECT_IS_EMPTY(pt_rect)), WGLR_INV_ARG);
    MBW_CHECK_VALID((ph_mbw != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->ui1_strct_ver == 1), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->ui4_count_col > 0 && pt_init_info->ui4_count_row > 0), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->i4_width_item > 0 && pt_init_info->i4_height_item > 0), WGLR_INV_ARG);
    MBW_CHECK_VALID(((pt_init_info->ui4_style & WGL_MBW_STL_HORIZ_ARRANGE) ?
                (pt_init_info->ui4_hlt_margin < pt_init_info->ui4_count_col) :
                (pt_init_info->ui4_hlt_margin < pt_init_info->ui4_count_row)),
                WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_hlt_item_info.ui1_strct_ver == 1), WGLR_INV_ARG);
    MBW_CHECK_VALID((!(pt_init_info->t_hlt_item_info.ui4_style & (~((WGL_MBW_HLT_ITEM_STL_LAST_STYLE << 1) - 1)))), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->i4_dist_horiz >= 0 && pt_init_info->i4_dist_verti >= 0), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_data_model.ui1_strct_ver == 1), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_data_model.t_fct_tbl.ui1_strct_ver == 1), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_data_model.t_fct_tbl.pf_dm_init != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_data_model.t_fct_tbl.pf_dm_deinit != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_data_model.t_fct_tbl.pf_enable_item_state != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_data_model.t_fct_tbl.pf_disable_item_state != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_data_model.t_fct_tbl.pf_get_item_data != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_data_model.t_fct_tbl.pf_get_item_state != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_data_model.t_fct_tbl.pf_update_item_ctnr != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_data_model.t_fct_tbl.pf_update_hlt_item_ctnr != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_data_model.t_fct_tbl.pf_ex_get != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_data_model.t_fct_tbl.pf_ex_set != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_evn_hdlr_info.ui1_strct_ver == 1), WGLR_INV_ARG);
    MBW_CHECK_VALID(((pt_init_info->t_evn_hdlr_info.ui4_evn_hdlr_mask != 0) ?
                 (pt_init_info->t_evn_hdlr_info.pf_event_hdlr != NULL) :
                 (1)),
                 WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_key_hdlr_info.ui1_strct_ver == 1), WGLR_INV_ARG);
    MBW_CHECK_VALID(((pt_init_info->t_key_hdlr_info.ui4_key_hdlr_mask != 0) ?
                 (pt_init_info->t_key_hdlr_info.pf_key_hdlr != NULL) :
                 (1)),
                 WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_mbw_ctnr_wgt_style.ui1_strct_ver == 1), WGLR_INV_ARG);
    MBW_CHECK_VALID((wgl_is_legal_inset(pt_rect, (WGL_INSET_T*)(&pt_init_info->t_inset_vp))), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_init_info->t_nvgt_custom.ui1_strct_ver == 1), WGLR_INV_ARG);

    ui4_col_num = pt_init_info->ui4_count_col;
    ui4_row_num = pt_init_info->ui4_count_row;
    ui4_vp_capacity = ui4_col_num * ui4_row_num;
    i4_item_width = pt_init_info->i4_width_item;
    i4_item_height = pt_init_info->i4_height_item;
    i4_horiz_dist = pt_init_info->i4_dist_horiz;
    i4_verti_dist = pt_init_info->i4_dist_verti;

    /*allocate memory for control logic object*/
    z_mem_size = WGL_ALIGN_4(sizeof(WGL_MBW_CONTROL_LOGIC_T)) +
                        WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_CTNR_INFO_T) * ui4_vp_capacity) +
                        WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_DATA_T) * ui4_vp_capacity) +
                        WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_STATE_T) * ui4_vp_capacity) +
                        WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_EX_REPAINT_REGION) * ui4_vp_capacity);

    pt_ctrl_logic = (WGL_MBW_CONTROL_LOGIC_T*)WGL_MEM_ALLOC(z_mem_size);
    if(NULL == pt_ctrl_logic)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Fail to alloc memory for control logic. (%d) at L%d\n\n", WGLR_OUT_OF_MEMORY, __LINE__));
        return WGLR_OUT_OF_MEMORY;
    }
    /*initialize control logic object*/
    x_memset(pt_ctrl_logic, 0, z_mem_size);
#ifdef CLI_LVL_ALL
     pt_ctrl_logic->ui4_mem_size = sizeof(z_mem_size);   
#endif

    /*setup control logic object*/
    pt_ctrl_logic->at_item_ctnr_info = (WGL_MBW_ITEM_CTNR_INFO_T*)(((UINT32)pt_ctrl_logic) + WGL_ALIGN_4(sizeof(WGL_MBW_CONTROL_LOGIC_T)));
    pt_ctrl_logic->at_item_data = (WGL_MBW_ITEM_DATA_T*)(((UINT32)(pt_ctrl_logic->at_item_ctnr_info)) + WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_CTNR_INFO_T) * ui4_vp_capacity));
    pt_ctrl_logic->aui4_item_state = (WGL_MBW_ITEM_STATE_T*)(((UINT32)(pt_ctrl_logic->at_item_data)) + WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_DATA_T) * ui4_vp_capacity));
    pt_ctrl_logic->at_need_repaint_ex_region = (WGL_MBW_ITEM_EX_REPAINT_REGION*)(((UINT32)(pt_ctrl_logic->aui4_item_state)) + WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_STATE_T) * ui4_vp_capacity));
    pt_ctrl_logic->ui4_col_num = ui4_col_num;
    pt_ctrl_logic->ui4_row_num = ui4_row_num;
    pt_ctrl_logic->ui4_hlt_idx = WGL_MBW_NULL_INDEX;
    pt_ctrl_logic->ui4_hlt_margin = pt_init_info->ui4_hlt_margin;
    pt_ctrl_logic->ui2_mbw_state = WGL_STATE_ENABLED;
    pt_ctrl_logic->ui4_style = pt_init_info->ui4_style;
    pt_ctrl_logic->b_is_transition = FALSE;
    pt_ctrl_logic->pv_mbw_ctx = NULL;
    pt_ctrl_logic->pv_dm_data = NULL;
    pt_ctrl_logic->ui1_extra_flag = MBW_VP_BK_CLR_NOT_INIT | MBW_VP_BK_IMG_NOT_INIT;
    /*the following attributes will be reset when reload*/
    pt_ctrl_logic->ui4_item_total_num = 0;
    pt_ctrl_logic->t_vp_info.ui4_idx_vp_first = 0;
    pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = 0;

    x_memcpy(&pt_ctrl_logic->t_nvgt_custom, &pt_init_info->t_nvgt_custom, sizeof(WGL_MBW_NVGT_CUSTOM_T));
    x_memcpy(&pt_ctrl_logic->t_key_map, &MBW_DEF_KEY_MAP, sizeof(WGL_MBW_KEY_MAP_T));
    x_memcpy(&pt_ctrl_logic->t_key_hdlr_info, &pt_init_info->t_key_hdlr_info, sizeof(WGL_MBW_KEY_HDLR_INFO_T));
    x_memcpy(&pt_ctrl_logic->t_hlt_item_info, &pt_init_info->t_hlt_item_info, sizeof(WGL_MBW_HLT_ITEM_INFO_T));
    x_memcpy(&pt_ctrl_logic->t_evn_hdlr_info, &pt_init_info->t_evn_hdlr_info, sizeof(WGL_MBW_EVN_HDLR_INFO_T));
    x_memcpy(&pt_ctrl_logic->t_data_model, &pt_init_info->t_data_model, sizeof(WGL_MBW_DATA_MODEL_INFO_T));

    /* create MatrixBox panel */
    pf_wgl_mbw_proc = (pf_wdgt_proc != NULL) ? pf_wdgt_proc : x_wgl_mbw_default_proc;
    ui4_cw_style = 0;
    MBW_SET_CW_STYLE(ui4_cw_style, pt_init_info->t_mbw_ctnr_wgt_style.ui4_cw_style_mbw);

    i4_ret = x_wgl_create_widget(
        h_container,
        HT_WGL_WIDGET_FRAME,
        WGL_CONTENT_FRAME_DEF,
        WGL_BORDER_NULL,
        pt_rect,
        pf_wgl_mbw_proc,
        255,
        (VOID *)(ui4_cw_style),
        pv_tag,
        ph_mbw);

    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Create MBW faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        WGL_MEM_FREE(pt_ctrl_logic);
        pt_ctrl_logic = NULL;
        return i4_ret;
    }
    pt_ctrl_logic->h_mbw = *ph_mbw;
    i4_ret = x_wgl_set_visibility(pt_ctrl_logic->h_mbw, WGL_SW_NORMAL);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the widget's private tag (%d) at L%d\n\n", i4_ret, __LINE__));
        WGL_MEM_FREE(pt_ctrl_logic);
        pt_ctrl_logic = NULL;
        if (WGLR_OK != x_wgl_destroy_widget_ex(*ph_mbw, FALSE))
        {
            DBG_ERROR(("{WGL(MBW)} ERR: Fail to destroy the mbw (%d) at L%d\n\n", i4_ret, __LINE__));
        }
        return i4_ret;
    }

    /* create highlight item container, which is based on the MatrixBox panel*/
    if (pt_ctrl_logic->t_hlt_item_info.ui4_style == 0)
    {
        pt_ctrl_logic->t_hlt_item_ctnr_info.ui1_strct_ver = 1;
        t_ex_magin.ui2_left = 0;
        t_ex_magin.ui2_right = 0;
        t_ex_magin.ui2_top = 0;
        t_ex_magin.ui2_bottom = 0;
        pt_ctrl_logic->t_hlt_item_info.t_ex_margin = t_ex_magin;
    }
    else if (pt_ctrl_logic->t_hlt_item_info.ui4_style & WGL_MBW_HLT_ITEM_STL_ENLRG_SIZE)
    {
        pt_ctrl_logic->t_hlt_item_ctnr_info.ui1_strct_ver = 1;
        t_ex_magin = pt_init_info->t_hlt_item_info.t_ex_margin;
    }

    SET_RECT_BY_SIZE(&t_hlt_item_cntr_rect,
                     0,
                     0,
                     i4_item_width + t_ex_magin.ui2_left + t_ex_magin.ui2_right,
                     i4_item_height + t_ex_magin.ui2_top + t_ex_magin.ui2_bottom);
    ui4_cw_style = 0;
    MBW_SET_CW_STYLE(ui4_cw_style, pt_init_info->t_mbw_ctnr_wgt_style.ui4_cw_style_hlt_item_ctnr);

    i4_ret = x_wgl_create_widget(
                pt_ctrl_logic->h_mbw,
                HT_WGL_WIDGET_FRAME,
                WGL_CONTENT_FRAME_DEF,
                WGL_BORDER_NULL,
                &t_hlt_item_cntr_rect,
                NULL,
                255,
                (VOID*)(ui4_cw_style),
                NULL,
                &pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr);

    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Create MBW hlt item container faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        WGL_MEM_FREE(pt_ctrl_logic);
        pt_ctrl_logic = NULL;
        if (WGLR_OK != x_wgl_destroy_widget_ex(*ph_mbw, FALSE))
        {
            DBG_ERROR(("{WGL(MBW)} ERR: Fail to destroy the mbw (%d) at L%d\n\n", i4_ret, __LINE__));
        }
        return i4_ret;
    }

    i4_ret = x_wgl_set_visibility(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, WGL_SW_HIDE);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
        WGL_MEM_FREE(pt_ctrl_logic);
        pt_ctrl_logic = NULL;
        if (WGLR_OK != x_wgl_destroy_widget_ex(*ph_mbw, FALSE))
        {
            DBG_ERROR(("{WGL(MBW)} ERR: Fail to destroy the mbw (%d) at L%d\n\n", i4_ret, __LINE__));
        }
        return i4_ret;
    }

    SET_RECT(&t_mbw_vp_rect,
             pt_init_info->t_inset_vp.i4_left,
             pt_init_info->t_inset_vp.i4_top,
             RECT_W (pt_rect) - pt_init_info->t_inset_vp.i4_right,
             RECT_H (pt_rect) - pt_init_info->t_inset_vp.i4_bottom);

    ui4_cw_style = 0;
    MBW_SET_CW_STYLE(ui4_cw_style, pt_init_info->t_mbw_ctnr_wgt_style.ui4_cw_style_vp);
    /* create MatrixBox viewport panel */
    i4_ret = x_wgl_create_widget(
        pt_ctrl_logic->h_mbw,
        HT_WGL_WIDGET_FRAME,
        WGL_CONTENT_FRAME_DEF,
        WGL_BORDER_NULL,
        &t_mbw_vp_rect,
        NULL,
        255,
        (VOID*)(ui4_cw_style),
        NULL,
        &pt_ctrl_logic->h_mbw_vp);

    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Create MBW viewport faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        WGL_MEM_FREE(pt_ctrl_logic);
        pt_ctrl_logic = NULL;
        if (WGLR_OK != x_wgl_destroy_widget_ex(*ph_mbw, FALSE))
        {
            DBG_ERROR(("{WGL(MBW)} ERR: Fail to destroy the mbw (%d) at L%d\n\n", i4_ret, __LINE__));
        }
        return i4_ret;
    }

    i4_ret = x_wgl_set_visibility(pt_ctrl_logic->h_mbw_vp, WGL_SW_NORMAL);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
        WGL_MEM_FREE(pt_ctrl_logic);
        pt_ctrl_logic = NULL;
        if (WGLR_OK != x_wgl_destroy_widget_ex(*ph_mbw, FALSE))
        {
            DBG_ERROR(("{WGL(MBW)} ERR: Fail to destroy the mbw (%d) at L%d\n\n", i4_ret, __LINE__));
        }
        return i4_ret;
    }

    i4_first_item_pos_x = 0;
    i4_first_item_pos_y = 0;
    if (!HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_HORIZ_ARRANGE))
    {
        for(ui4_y = 0; ui4_y < ui4_row_num; ui4_y++)
        {
            for(ui4_x = 0; ui4_x < ui4_col_num; ui4_x++)
            {
                pt_ctrl_logic->at_item_ctnr_info[ui4_x + ui4_y * ui4_col_num].ui1_strct_ver = 1;
                SET_RECT_BY_SIZE(&t_item_cntr_rect,
                    i4_first_item_pos_x + ui4_x * (i4_horiz_dist + i4_item_width),
                    i4_first_item_pos_y + ui4_y * (i4_verti_dist + i4_item_height),
                    i4_item_width,
                    i4_item_height);
                ui4_cw_style = 0;
                MBW_SET_CW_STYLE(ui4_cw_style, pt_init_info->t_mbw_ctnr_wgt_style.ui4_cw_style_item_ctnr);
                i4_ret = x_wgl_create_widget(
                    pt_ctrl_logic->h_mbw_vp,
                    HT_WGL_WIDGET_FRAME,
                    WGL_CONTENT_FRAME_DEF,
                    WGL_BORDER_NULL,
                    &t_item_cntr_rect,
                    NULL,
                    255,
                    (VOID*)(ui4_cw_style),
                    NULL,
                    &pt_ctrl_logic->at_item_ctnr_info[ui4_x + ui4_y * ui4_col_num].h_item_ctnr);

                if (WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: Create MBW item containers faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                    WGL_MEM_FREE(pt_ctrl_logic);
                    pt_ctrl_logic = NULL;
                    if (WGLR_OK != x_wgl_destroy_widget_ex(*ph_mbw, FALSE))
                    {
                        DBG_ERROR(("{WGL(MBW)} ERR: Fail to destroy the mbw (%d) at L%d\n\n", i4_ret, __LINE__));
                    }
                    return i4_ret;
                }
                pt_ctrl_logic->at_need_repaint_ex_region[ui4_x + ui4_y * ui4_col_num].b_is_dirty = FALSE;
                SET_RECT(
                    &pt_ctrl_logic->at_need_repaint_ex_region[ui4_x + ui4_y * ui4_col_num].t_dirty_reg,
                    t_item_cntr_rect.i4_left   + t_mbw_vp_rect.i4_left,
                    t_item_cntr_rect.i4_right  + t_mbw_vp_rect.i4_left,
                    t_item_cntr_rect.i4_top    + t_mbw_vp_rect.i4_top,
                    t_item_cntr_rect.i4_bottom + t_mbw_vp_rect.i4_top);
            }
        }
    }
    else
    {
        for(ui4_x = 0; ui4_x < ui4_col_num; ui4_x++)
        {
            for(ui4_y = 0; ui4_y < ui4_row_num; ui4_y++)
            {
                pt_ctrl_logic->at_item_ctnr_info[ui4_y + ui4_x * ui4_row_num].ui1_strct_ver = 1;
                SET_RECT_BY_SIZE(&t_item_cntr_rect,
                    i4_first_item_pos_x + ui4_x * (i4_horiz_dist + i4_item_width),
                    i4_first_item_pos_y + ui4_y * (i4_verti_dist + i4_item_height),
                    i4_item_width,
                    i4_item_height);
                ui4_cw_style = 0;
                MBW_SET_CW_STYLE(ui4_cw_style, pt_init_info->t_mbw_ctnr_wgt_style.ui4_cw_style_item_ctnr);
                i4_ret = x_wgl_create_widget(
                    pt_ctrl_logic->h_mbw_vp,
                    HT_WGL_WIDGET_FRAME,
                    WGL_CONTENT_FRAME_DEF,
                    WGL_BORDER_NULL,
                    &t_item_cntr_rect,
                    NULL,
                    255,
                    (VOID*)(ui4_cw_style),
                    NULL,
                    &pt_ctrl_logic->at_item_ctnr_info[ui4_y + ui4_x * ui4_row_num].h_item_ctnr);

                if (WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: Create MBW containers faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                    WGL_MEM_FREE(pt_ctrl_logic);
                    pt_ctrl_logic = NULL;
                    if (WGLR_OK != x_wgl_destroy_widget_ex(*ph_mbw, FALSE))
                    {
                        DBG_ERROR(("{WGL(MBW)} ERR: Fail to destroy the mbw (%d) at L%d\n\n", i4_ret, __LINE__));
                    }
                    return i4_ret;
                }
                pt_ctrl_logic->at_need_repaint_ex_region[ui4_y + ui4_x * ui4_row_num].b_is_dirty = FALSE;
                SET_RECT(
                    &pt_ctrl_logic->at_need_repaint_ex_region[ui4_y + ui4_x * ui4_row_num].t_dirty_reg,
                    t_item_cntr_rect.i4_left   + t_mbw_vp_rect.i4_left,
                    t_item_cntr_rect.i4_right  + t_mbw_vp_rect.i4_left,
                    t_item_cntr_rect.i4_top    + t_mbw_vp_rect.i4_top,
                    t_item_cntr_rect.i4_bottom + t_mbw_vp_rect.i4_top);
            }
        }
    }
    /*set the pt_ctrl_logic as the widget's private tag*/
    i4_ret = wgl_set_widget_private_tag(*ph_mbw, (VOID*)pt_ctrl_logic);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the widget's private tag (%d) at L%d\n\n", i4_ret, __LINE__));
        WGL_MEM_FREE(pt_ctrl_logic);
        pt_ctrl_logic = NULL;
        if (WGLR_OK != x_wgl_destroy_widget_ex(*ph_mbw, FALSE))
        {
            DBG_ERROR(("{WGL(MBW)} ERR: Fail to destroy the mbw (%d) at L%d\n\n", i4_ret, __LINE__));
        }
        return i4_ret;
    }

    /*init date model,dm must setup item containers and hlt item container in the init procedure*/
    t_mbw_dm_init_info.ui1_strct_ver = 1;
    t_mbw_dm_init_info.ui4_num_item_ctnr = ui4_vp_capacity;
    t_mbw_dm_init_info.at_item_ctnr_info = pt_ctrl_logic->at_item_ctnr_info;
    t_mbw_dm_init_info.pt_hlt_item_ctnr_info = &pt_ctrl_logic->t_hlt_item_ctnr_info;

    if(!(pt_ctrl_logic->t_data_model.t_fct_tbl.pf_dm_init(
                            pt_ctrl_logic->h_mbw,
                            &pt_ctrl_logic->pv_dm_data,
                            &t_mbw_dm_init_info)))
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Fail to init data model (%d) at L%d\n\n", i4_ret, __LINE__));
        WGL_MEM_FREE(pt_ctrl_logic);
        pt_ctrl_logic = NULL;
        if (WGLR_OK != x_wgl_destroy_widget_ex(*ph_mbw, FALSE))
        {
            DBG_ERROR(("{WGL(MBW)} ERR: Fail to destroy the mbw (%d) at L%d\n\n", i4_ret, __LINE__));
        }
        return WGLR_INIT_FAILED;
    }

    return WGLR_OK;
}

INT32 x_wgl_mbw_destroy (
    HANDLE_T                h_mbw)
{
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;
    WGL_MBW_DM_INIT_INFO_T      t_mbw_dm_init_info = {0};
    INT32                       i4_ret;
    UINT32                      ui4_item_ctnr_num;
    GL_RECT_T                   t_mbw_rect;
    HANDLE_T                    h_mbw_parent;
    HANDLE_TYPE_T               e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Destory MBW faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when destroy MatrixBox, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if(pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }

    ui4_item_ctnr_num = pt_ctrl_logic->ui4_col_num * pt_ctrl_logic->ui4_row_num;

    t_mbw_dm_init_info.ui1_strct_ver = 1;
    t_mbw_dm_init_info.ui4_num_item_ctnr = ui4_item_ctnr_num;
    t_mbw_dm_init_info.at_item_ctnr_info = pt_ctrl_logic->at_item_ctnr_info;
    t_mbw_dm_init_info.pt_hlt_item_ctnr_info = &pt_ctrl_logic->t_hlt_item_ctnr_info;

    /*deinit date model*/
    pt_ctrl_logic->t_data_model.t_fct_tbl.pf_dm_deinit(
                            pt_ctrl_logic->pv_dm_data,
                            h_mbw,
                            &t_mbw_dm_init_info);

    /*free control logic object*/
    WGL_MEM_FREE(pt_ctrl_logic);
    pt_ctrl_logic = NULL;

    /*reset the private tag*/
    i4_ret = wgl_set_widget_private_tag(h_mbw, (VOID*)pt_ctrl_logic);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the widget's private tag (%d) at L%d\n\n", i4_ret, __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    i4_ret = x_wgl_get_parent(h_mbw, &h_mbw_parent);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    i4_ret = x_wgl_get_coords(h_mbw, WGL_COORDS_PARENT, &t_mbw_rect);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    REL_LOCK(h_mbw, pv_wgl_ctx);

    /*destroy MatrixBox*/
    i4_ret = x_wgl_destroy_widget_ex(h_mbw, FALSE);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Fail to destroy mbw (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*partially repaint the parent widget*/
    i4_ret = x_wgl_repaint(h_mbw_parent, &t_mbw_rect, FALSE);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: Fail to partially repaint mbw partent(%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    return WGLR_OK;
}

INT32 x_wgl_mbw_default_proc (
    HANDLE_T                h_mbw,
    UINT32                  ui4_msg,
    VOID*                   pv_param1,
    VOID*                   pv_param2)
{
    INT32   i4_ret = WMPR_DONE;
    WGL_MBW_CONTROL_LOGIC_T* pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;
    HANDLE_TYPE_T e_handle_type;

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: default msg procedure faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_INFO(("{WGL(MBW)} Info: when process msg, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return x_wgl_default_msg_proc(h_mbw,
                        ui4_msg,
                        pv_param1,
                        pv_param2);
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    switch (ui4_msg)
    {
    case WGL_MSG_PAINT:
        {
            UINT16 ui2_paint_state = WGL_POINTER_TO_UINT16(pv_param2);

            i4_ret = _mbw_clear_item_dirty_region(pt_ctrl_logic);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: when process msg, clear item diry region failed. (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }

            if ((ui2_paint_state & (WGL_STATE_FOCUSED | WGL_STATE_ACTIVATED | WGL_STATE_ENABLED))
                != pt_ctrl_logic->ui2_mbw_state)
            {
                pt_ctrl_logic->ui2_mbw_state = (ui2_paint_state & (WGL_STATE_FOCUSED | WGL_STATE_ACTIVATED | WGL_STATE_ENABLED));
                MBW_UPDATE_ITEM_CTNR(
                                h_mbw,
                                pt_ctrl_logic,
                                WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE,
                                pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                                pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                                pt_ctrl_logic->at_item_ctnr_info);

                if (pt_ctrl_logic->ui4_hlt_idx != WGL_MBW_NULL_INDEX)
                {
                    MBW_UPDATE_HLT_ITEM_CTNR(
                                h_mbw,
                                pt_ctrl_logic,
                                WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE);
                }

                i4_ret = _mbw_set_vp_bk_image(pt_ctrl_logic);
                if (WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: when process msg, set vp background img faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    return i4_ret;
                }
                i4_ret = _mbw_set_vp_bk_color(pt_ctrl_logic);
                if (WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: when process msg, set vp background img faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    return i4_ret;
                }
            }
            REL_LOCK(h_mbw, pv_wgl_ctx);
        }
    break;

    case WGL_MSG_KEY_DOWN:
        {
            UINT32 ui4_key = (UINT32)pv_param1;
            WGL_MBW_KEY_HDLR_INFO_T t_key_hdlt_info = {0};

            /*should not be allowed in transition state*/
            if(pt_ctrl_logic->b_is_transition == TRUE)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
            }

            if (pt_ctrl_logic->ui4_hlt_idx == WGL_MBW_NULL_INDEX)
            {
                /*ap must enable a item to hlt*/
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return WGLR_OK;
            }

            /*copy the user's key hdlr info*/
            x_memcpy(&t_key_hdlt_info, &pt_ctrl_logic->t_key_hdlr_info, sizeof(WGL_MBW_KEY_HDLR_INFO_T));

            if (ui4_key == pt_ctrl_logic->t_key_map.ui4_key_select)
            {
                if (HAS_KEY_HANDLER_MASK(pt_ctrl_logic, WGL_MBW_KEY_HDLR_MASK_SELECT))
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);

                    t_key_hdlt_info.pf_key_hdlr(
                                      h_mbw,
                                      t_key_hdlt_info.pv_tag_key_hdlr,
                                      WGL_MBW_KEY_HDLR_TYPE_SELECT,
                                      0);
                }
                else
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    i4_ret = x_wgl_mbw_default_key_hdlr(
                                      h_mbw,
                                      WGL_MBW_KEY_HDLR_TYPE_SELECT,
                                      0);

                    if (WGLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(MBW)} ERR: default key_select handler procedure faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                        return i4_ret;
                    }
                }

            }
            else if (ui4_key == pt_ctrl_logic->t_key_map.ui4_key_nvgt_left)
            {
                if (HAS_KEY_HANDLER_MASK(pt_ctrl_logic, WGL_MBW_KEY_HDLR_MASK_NVGT_LEFT))
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);

                    t_key_hdlt_info.pf_key_hdlr(
                                      h_mbw,
                                      t_key_hdlt_info.pv_tag_key_hdlr,
                                      WGL_MBW_KEY_HDLR_TYPE_NVGT_LEFT,
                                      0);
                }
                else
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    i4_ret = x_wgl_mbw_default_key_hdlr(
                                      h_mbw,
                                      WGL_MBW_KEY_HDLR_TYPE_NVGT_LEFT,
                                      0);

                    if (WGLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(MBW)} ERR: default key_left handler procedure faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                        return i4_ret;
                    }
                }

            }
            else if (ui4_key == pt_ctrl_logic->t_key_map.ui4_key_nvgt_right)
            {
                if (HAS_KEY_HANDLER_MASK(pt_ctrl_logic, WGL_MBW_KEY_HDLR_MASK_NVGT_RIGHT))
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);

                    t_key_hdlt_info.pf_key_hdlr(
                                      h_mbw,
                                      t_key_hdlt_info.pv_tag_key_hdlr,
                                      WGL_MBW_KEY_HDLR_TYPE_NVGT_RIGHT,
                                      0);
                }
                else
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    i4_ret = x_wgl_mbw_default_key_hdlr(
                                      h_mbw,
                                      WGL_MBW_KEY_HDLR_TYPE_NVGT_RIGHT,
                                      0);

                    if (WGLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(MBW)} ERR: default key_right handler procedure faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                        return i4_ret;
                    }
                }

            }
            else if (ui4_key == pt_ctrl_logic->t_key_map.ui4_key_nvgt_up)
            {
                if (HAS_KEY_HANDLER_MASK(pt_ctrl_logic, WGL_MBW_KEY_HDLR_MASK_NVGT_UP))
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);

                    t_key_hdlt_info.pf_key_hdlr(
                                      h_mbw,
                                      t_key_hdlt_info.pv_tag_key_hdlr,
                                      WGL_MBW_KEY_HDLR_TYPE_NVGT_UP,
                                      0);
                }
                else
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    i4_ret = x_wgl_mbw_default_key_hdlr(
                                      h_mbw,
                                      WGL_MBW_KEY_HDLR_TYPE_NVGT_UP,
                                      0);

                    if (WGLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(MBW)} ERR: default key_up handler procedure faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                        return i4_ret;
                    }
                }

            }
            else if (ui4_key == pt_ctrl_logic->t_key_map.ui4_key_nvgt_down)
            {
                if (HAS_KEY_HANDLER_MASK(pt_ctrl_logic, WGL_MBW_KEY_HDLR_MASK_NVGT_DOWN))
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);

                    t_key_hdlt_info.pf_key_hdlr(
                                      h_mbw,
                                      t_key_hdlt_info.pv_tag_key_hdlr,
                                      WGL_MBW_KEY_HDLR_TYPE_NVGT_DOWN,
                                      0);
                }
                else
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    i4_ret = x_wgl_mbw_default_key_hdlr(
                                      h_mbw,
                                      WGL_MBW_KEY_HDLR_TYPE_NVGT_DOWN,
                                      0);

                    if (WGLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(MBW)} ERR: default key_down handler procedure faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                        return i4_ret;
                    }
                }

            }
            else if (ui4_key == pt_ctrl_logic->t_key_map.ui4_key_nvgt_page_up)
            {
                if (HAS_KEY_HANDLER_MASK(pt_ctrl_logic, WGL_MBW_KEY_HDLR_MASK_NVGT_PAGE_UP))
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);

                    t_key_hdlt_info.pf_key_hdlr(
                                      h_mbw,
                                      t_key_hdlt_info.pv_tag_key_hdlr,
                                      WGL_MBW_KEY_HDLR_TYPE_NVGT_PAGE_UP,
                                      0);
                }
                else
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    i4_ret = x_wgl_mbw_default_key_hdlr(
                                      h_mbw,
                                      WGL_MBW_KEY_HDLR_TYPE_NVGT_PAGE_UP,
                                      0);
                    if (WGLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(MBW)} ERR: default key_pageup handler procedure faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                        return i4_ret;
                    }
                }

            }
            else if (ui4_key == pt_ctrl_logic->t_key_map.ui4_key_nvgt_page_down)
            {
                if (HAS_KEY_HANDLER_MASK(pt_ctrl_logic, WGL_MBW_KEY_HDLR_MASK_NVGT_PAGE_DOWN))
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);

                    t_key_hdlt_info.pf_key_hdlr(
                                      h_mbw,
                                      t_key_hdlt_info.pv_tag_key_hdlr,
                                      WGL_MBW_KEY_HDLR_TYPE_NVGT_PAGE_DOWN,
                                      0);

                }
                else
                {
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    i4_ret = x_wgl_mbw_default_key_hdlr(
                                      h_mbw,
                                      WGL_MBW_KEY_HDLR_TYPE_NVGT_PAGE_DOWN,
                                      0);
                    if (WGLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(MBW)} ERR: default key_pagedown handler procedure faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                        return i4_ret;
                    }
                }

            }
            else
            {
                REL_LOCK(h_mbw, pv_wgl_ctx);
            }
        }
     break;
     
#ifdef CLI_LVL_ALL
     case WGL_MSG_COMMAND:
        {
            REL_LOCK(h_mbw, pv_wgl_ctx);
            if(NULL == pv_param2)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: The 2nd parameter of WGL_MSG_COMMAND should not be NULL. (%d) at L%d\n\n", i4_ret, __LINE__));
                return WGLR_INV_ARG; 
            }
            else
            {
                if((UINT32)pv_param1 == WGL_CMD_GL_GET_ALLOC_MEM)
                {
                    VOID* pv_mem_size = ((WGL_PACKED_PARAM_T*)pv_param2)->pv_param1;
                    if(pv_mem_size == NULL)
                    {
                         DBG_ERROR(("{WGL(MBW)} ERR: Invalid arguments. For WGL_CMD_GL_GET_ALLOC_MEM, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                         return WGLR_INV_ARG; 
                    }
                    else
                    {
                        *((UINT32*)pv_mem_size) = pt_ctrl_logic->ui4_mem_size;
                    }
                }
            }
        }
        break;
#endif

     default:
        REL_LOCK(h_mbw, pv_wgl_ctx);
        break;
        }

    return x_wgl_default_msg_proc(h_mbw,
                    ui4_msg,
                    pv_param1,
                    pv_param2);

}

INT32 x_wgl_mbw_default_key_hdlr (
    HANDLE_T                        h_mbw,
    WGL_MBW_KEY_HDLR_TYPE_T         e_key_hdlr_type,
    UINT32                          ui4_data)
{
    INT32      i4_ret;
    UINT32     ui4_hlt_idx;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;
    HANDLE_TYPE_T e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((MBW_IS_VALID_KEY_HDLR_TYPE(e_key_hdlr_type)), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: default key handler procedure faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: default key handler, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    ui4_hlt_idx = pt_ctrl_logic->ui4_hlt_idx;

    /*select key default procedure*/
    if (e_key_hdlr_type == WGL_MBW_KEY_HDLR_TYPE_SELECT)
    {
        i4_ret = _mbw_on_item_select(pt_ctrl_logic, pv_wgl_ctx, ui4_hlt_idx);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR:select item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        }
    }
    /*navigation key default procedure*/
    else
    {
        i4_ret = _mbw_default_nvgt_key_hdlr(pt_ctrl_logic, pv_wgl_ctx, e_key_hdlr_type, ui4_data);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR:default key hdlr procedure faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        }
    }
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return i4_ret;
}

INT32 x_wgl_mbw_default_evn_hdlr (
    HANDLE_T                 h_mbw,
    WGL_MBW_EVN_HDLR_TYPE_T  e_evn_hdlr_type,
    UINT32                   ui4_data)
{
    INT32                       i4_ret;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;
    HANDLE_TYPE_T               e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((MBW_IS_VALID_EVN_HDLR_TYPE(e_evn_hdlr_type)), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: default key handler procedure faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if (e_evn_hdlr_type == WGL_MBW_EVN_HDLR_TYPE_NVGT_IN_VP)
    {
        WGL_MBW_EVN_HDLR_INFO_NVGT_IN_VP_T*       pt_mbw_evn_hdlr_nvgt_in_vp;
        pt_mbw_evn_hdlr_nvgt_in_vp = (WGL_MBW_EVN_HDLR_INFO_NVGT_IN_VP_T*)ui4_data;
        MBW_CHECK_VALID((pt_mbw_evn_hdlr_nvgt_in_vp->ui1_strct_ver == 1), WGLR_INV_ARG);
        MBW_CHECK_VALID((pt_mbw_evn_hdlr_nvgt_in_vp->t_nvgt_info.ui1_strct_ver == 1), WGLR_INV_ARG);

        if (pt_mbw_evn_hdlr_nvgt_in_vp->t_nvgt_info.b_navigable)
        {
            i4_ret = x_wgl_mbw_disable_item_state(
                                h_mbw,
                                pt_mbw_evn_hdlr_nvgt_in_vp->t_nvgt_info.ui4_idx_orig,
                                WGL_MBW_ITEM_STATE_HLTED);

            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: disable item state faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }

            i4_ret = x_wgl_mbw_repaint_item(
                                h_mbw,
                                pt_mbw_evn_hdlr_nvgt_in_vp->t_nvgt_info.ui4_idx_orig);

            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: repaint item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }

            i4_ret = x_wgl_mbw_enable_item_state(
                                h_mbw,
                                pt_mbw_evn_hdlr_nvgt_in_vp->t_nvgt_info.ui4_idx_new,
                                WGL_MBW_ITEM_STATE_HLTED);

            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: enable item state faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }

            i4_ret = x_wgl_mbw_repaint_item(
                                h_mbw,
                                pt_mbw_evn_hdlr_nvgt_in_vp->t_nvgt_info.ui4_idx_new);

            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: repaint item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
        }
    }
    else if (e_evn_hdlr_type == WGL_MBW_EVN_HDLR_TYPE_NVGT_WITH_VP_CHG)
    {
        WGL_MBW_EVN_HDLR_INFO_NVGT_WITH_VP_CHG_T* pt_mbw_evn_hdlr_nvgt_with_vp_chg;
        pt_mbw_evn_hdlr_nvgt_with_vp_chg = (WGL_MBW_EVN_HDLR_INFO_NVGT_WITH_VP_CHG_T*)ui4_data;
        MBW_CHECK_VALID((pt_mbw_evn_hdlr_nvgt_with_vp_chg->ui1_strct_ver == 1), WGLR_INV_ARG);
        MBW_CHECK_VALID((pt_mbw_evn_hdlr_nvgt_with_vp_chg->t_nvgt_info.ui1_strct_ver == 1), WGLR_INV_ARG);

        i4_ret = x_wgl_mbw_disable_item_state(
                            h_mbw,
                            pt_mbw_evn_hdlr_nvgt_with_vp_chg->t_nvgt_info.ui4_idx_orig,
                            WGL_MBW_ITEM_STATE_HLTED);

        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: disable item state faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }

        i4_ret = x_wgl_mbw_repaint_item(
                            h_mbw,
                            pt_mbw_evn_hdlr_nvgt_with_vp_chg->t_nvgt_info.ui4_idx_orig);

        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: repaint item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }


        i4_ret = x_wgl_mbw_update_vp(h_mbw,&pt_mbw_evn_hdlr_nvgt_with_vp_chg->t_nvgt_info);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: update viewport faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }

        i4_ret = x_wgl_mbw_enable_item_state(
                    h_mbw,
                    pt_mbw_evn_hdlr_nvgt_with_vp_chg->t_nvgt_info.ui4_idx_new,
                    WGL_MBW_ITEM_STATE_HLTED);

        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: enable item state faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }

        i4_ret = x_wgl_mbw_repaint_vp(h_mbw);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: update viewport faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
    }
    else if (e_evn_hdlr_type == WGL_MBW_EVN_HDLR_TYPE_DESEL)
    {
        WGL_MBW_EVN_HDLR_INFO_DESEL_T*            pt_mbw_evn_hdlr_info_desel;
        pt_mbw_evn_hdlr_info_desel = (WGL_MBW_EVN_HDLR_INFO_DESEL_T*)ui4_data;
        MBW_CHECK_VALID((pt_mbw_evn_hdlr_info_desel->ui1_strct_ver == 1), WGLR_INV_ARG);

        i4_ret = x_wgl_mbw_disable_item_state(
                    h_mbw,
                    pt_mbw_evn_hdlr_info_desel->ui4_idx_desel,
                    WGL_MBW_ITEM_STATE_SELECTED);

        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: disable item state faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }

        i4_ret = x_wgl_mbw_repaint_item(h_mbw, pt_mbw_evn_hdlr_info_desel->ui4_idx_desel);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: repaint item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
    }
    /* e_evn_hdlr_type == WGL_MBW_EVN_HDLR_TYPE_SELECT */
    else
    {
        WGL_MBW_EVN_HDLR_INFO_SELECT_T*           pt_mbw_evn_hdlr_info_select;
        pt_mbw_evn_hdlr_info_select = (WGL_MBW_EVN_HDLR_INFO_SELECT_T*)ui4_data;
        MBW_CHECK_VALID((pt_mbw_evn_hdlr_info_select->ui1_strct_ver == 1), WGLR_INV_ARG);

        i4_ret = x_wgl_mbw_enable_item_state(
                    h_mbw,
                    pt_mbw_evn_hdlr_info_select->ui4_idx_sel,
                    WGL_MBW_ITEM_STATE_SELECTED);

        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: disable item state faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }

        i4_ret = x_wgl_mbw_repaint_item(h_mbw, pt_mbw_evn_hdlr_info_select->ui4_idx_sel);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: repaint item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
    }
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}


INT32 x_wgl_mbw_get_nvgt_info (
    HANDLE_T                h_mbw,
    UINT32                  ui4_idx_from,
    WGL_MBW_NVGT_DIR_T      e_nvgt_dir,
    WGL_MBW_NVGT_INFO_T*    pt_nvgt_info)
{
    INT32                       i4_ret;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;
    HANDLE_TYPE_T               e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((MBW_IS_VALID_NVGT_DIR(e_nvgt_dir)), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_nvgt_info != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_nvgt_info->ui1_strct_ver == 1), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: get nvgt info faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when get nvgt info, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    MBW_CHECK_VALID((ui4_idx_from < pt_ctrl_logic->ui4_item_total_num), WGLR_INV_ARG);

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if (e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_UP || e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_DOWN)
    {
    	 if(pt_ctrl_logic->t_nvgt_custom.t_nvgt_pg_down_custom !=WGL_MBW_NVGT_PG_DOWN_NORMAL ||
           pt_ctrl_logic->t_nvgt_custom.t_nvgt_pg_up_custom!=WGL_MBW_NVGT_PG_UP_NORMAL)
        {
            i4_ret =  _mbw_get_page_up_down_custom_nvgt_info(
                                pt_ctrl_logic,
                                ui4_idx_from,
                                e_nvgt_dir,
                                pt_nvgt_info);
        }
        else
        {
            i4_ret =  _mbw_get_page_up_down_nvgt_info(
                            pt_ctrl_logic,
                            ui4_idx_from,
                            e_nvgt_dir,
                            pt_nvgt_info);
        }
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: get page_up/page_down nvgt key info faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        }
    }
    else
    {
        i4_ret =  _mbw_get_normal_nvgt_info(
                        pt_ctrl_logic,
                        ui4_idx_from,
                        e_nvgt_dir,
                        pt_nvgt_info);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: get normal nvgt key info faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        }
    }
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return i4_ret;

}

INT32 x_wgl_mbw_update_vp (
    HANDLE_T                h_mbw,
    WGL_MBW_NVGT_INFO_T*    pt_nvgt_info)
{
    UINT32                      ui4_page_offset;
    UINT32                      ui4_col_num;
    UINT32                      ui4_row_num;
    UINT32                      ui4_item_ctnr_idx;
    UINT32                      ui4_vp_capacity;
    INT32                       i4_new_vp_idx_first;
    INT32                       i4_new_vp_idx_last;
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_handle_type;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_nvgt_info != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_nvgt_info->ui1_strct_ver == 1), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:update vp  faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when update vp, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    ui4_col_num = pt_ctrl_logic->ui4_col_num;
    ui4_row_num = pt_ctrl_logic->ui4_row_num;
    ui4_vp_capacity = ui4_col_num * ui4_row_num;
    if (pt_nvgt_info->b_vp_changed)
    {
        if (!pt_nvgt_info->b_wrap_over)
        {
            if (pt_nvgt_info->e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_DOWN)
            {
                i4_new_vp_idx_first = (INT32)pt_ctrl_logic->t_vp_info.ui4_idx_vp_last + 1;
                i4_new_vp_idx_last = i4_new_vp_idx_first + ui4_vp_capacity - 1;
                while ((INT32)pt_nvgt_info->ui4_idx_new > i4_new_vp_idx_last)
                {
                    i4_new_vp_idx_first = i4_new_vp_idx_last + 1;
                    i4_new_vp_idx_last = i4_new_vp_idx_first + ui4_vp_capacity - 1;
                }
                /*the last item index in viewport may larger than the total item number*/
                if (i4_new_vp_idx_last >= (INT32)pt_ctrl_logic->ui4_item_total_num)
                {
                    i4_new_vp_idx_last = (INT32)(pt_ctrl_logic->ui4_item_total_num - 1);
                }
            }
            else if (pt_nvgt_info->e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_UP)
            {
                i4_new_vp_idx_first = (INT32)(pt_ctrl_logic->t_vp_info.ui4_idx_vp_first - ui4_vp_capacity);
                i4_new_vp_idx_last = i4_new_vp_idx_first + (INT32)ui4_vp_capacity - 1;
                while ((INT32)pt_nvgt_info->ui4_idx_new < i4_new_vp_idx_first)
                {
                    i4_new_vp_idx_first -= ui4_vp_capacity;
                    i4_new_vp_idx_last = i4_new_vp_idx_first + ui4_vp_capacity - 1;
                }
                if (i4_new_vp_idx_first < 0)
                {
                    i4_new_vp_idx_first = 0;
                    i4_new_vp_idx_last = i4_new_vp_idx_first + ui4_vp_capacity - 1;
                    /*the last item index in viewport may larger than the total item number*/
                    if (i4_new_vp_idx_last >= (INT32)pt_ctrl_logic->ui4_item_total_num)
                    {
                        i4_new_vp_idx_last = (INT32)pt_ctrl_logic->ui4_item_total_num - 1;
                    }
                }
            }
            /*page mode*/
            else if (HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_PAGE_MODE))
            {
                /*get the page which contains the new item*/
                ui4_page_offset = pt_nvgt_info->ui4_idx_new / ui4_vp_capacity;
                i4_new_vp_idx_first = (INT32)(ui4_page_offset * ui4_vp_capacity);
                i4_new_vp_idx_last = i4_new_vp_idx_first + ui4_vp_capacity - 1;
                /*the last item index in viewport may larger than the total item number*/
                if (i4_new_vp_idx_last >= (INT32)pt_ctrl_logic->ui4_item_total_num)
                {
                    i4_new_vp_idx_last = (INT32)pt_ctrl_logic->ui4_item_total_num - 1;
                }
            }
            /*scroll mode*/
            else
            {
                UINT32 ui4_step;
                /*check the arrange mode*/
                if (!HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_HORIZ_ARRANGE))
                {
                    ui4_step = ui4_col_num;
                }
                else
                {
                    ui4_step = ui4_row_num;
                }
                if (pt_nvgt_info->e_nvgt_dir == WGL_MBW_NVGT_DIR_DOWN || pt_nvgt_info->e_nvgt_dir == WGL_MBW_NVGT_DIR_RIGHT)
                {
                    i4_new_vp_idx_last = (INT32)(pt_nvgt_info->ui4_idx_new +
                                            ((ui4_step -1) - pt_nvgt_info->ui4_idx_new % ui4_step) +
                                            pt_ctrl_logic->ui4_hlt_margin * ui4_step);
                    i4_new_vp_idx_first = i4_new_vp_idx_last - ui4_vp_capacity + 1;
                    /*use item nvgt map may cause this situation*/
                    if (i4_new_vp_idx_first < 0)
                    {
                        i4_new_vp_idx_first = 0;
                        i4_new_vp_idx_last = i4_new_vp_idx_first + ui4_vp_capacity - 1;
                    }
                    /*the last item index in viewport may larger than the total item number*/
                    if (i4_new_vp_idx_last >= (INT32)pt_ctrl_logic->ui4_item_total_num)
                    {
                        i4_new_vp_idx_last = (INT32)pt_ctrl_logic->ui4_item_total_num - 1;
                    }

                }
                else
                {
                    i4_new_vp_idx_first = (INT32)(pt_nvgt_info->ui4_idx_new -
                                            pt_nvgt_info->ui4_idx_new % ui4_step -
                                            pt_ctrl_logic->ui4_hlt_margin * ui4_step);
                    if (i4_new_vp_idx_first < 0)
                    {
                        i4_new_vp_idx_first = 0;
                    }
                    i4_new_vp_idx_last = i4_new_vp_idx_first + ui4_vp_capacity - 1;
                    /*the last item index in viewport may larger than the total item number*/
                    if (i4_new_vp_idx_last >= (INT32)pt_ctrl_logic->ui4_item_total_num)
                    {
                        i4_new_vp_idx_last = (INT32)pt_ctrl_logic->ui4_item_total_num - 1;
                    }
                }
            }
        }
        else
        {
            /*get the page which contains the new item*/
            ui4_page_offset = pt_nvgt_info->ui4_idx_new / ui4_vp_capacity;
            i4_new_vp_idx_first = (INT32)(ui4_page_offset * ui4_vp_capacity);
            i4_new_vp_idx_last = i4_new_vp_idx_first + ui4_vp_capacity - 1;
            /*the last item index in viewport may larger than the total item number*/
            if (i4_new_vp_idx_last >= (INT32)pt_ctrl_logic->ui4_item_total_num)
            {
                i4_new_vp_idx_last = (INT32)pt_ctrl_logic->ui4_item_total_num - 1;
            }
        }

        if ((INT32)(pt_ctrl_logic->t_vp_info.ui4_idx_vp_last - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first)
            > (i4_new_vp_idx_last - i4_new_vp_idx_first))
        {
            UINT32  ui4_start_to_hide;
            UINT32  ui4_extra_item_ctnr_num;
            /*hide the excessive item containers*/
            ui4_extra_item_ctnr_num = (pt_ctrl_logic->t_vp_info.ui4_idx_vp_last - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first) -
                                        (i4_new_vp_idx_last - i4_new_vp_idx_first);

            for(ui4_start_to_hide = (UINT32)(i4_new_vp_idx_last + 1);
                ui4_start_to_hide <= (UINT32)i4_new_vp_idx_last + ui4_extra_item_ctnr_num;
                ui4_start_to_hide++)
            {
                x_wgl_set_visibility(
                    pt_ctrl_logic->at_item_ctnr_info[ui4_start_to_hide - i4_new_vp_idx_first].h_item_ctnr,
                    WGL_SW_HIDE);
            }
        }

        if ((INT32)(pt_ctrl_logic->t_vp_info.ui4_idx_vp_last - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first)
            < (i4_new_vp_idx_last - i4_new_vp_idx_first))
        {
            UINT32  ui4_start_to_reshow;
            UINT32  ui4_extra_item_ctnr_num;
            /*reshow the hided item containers*/
            ui4_extra_item_ctnr_num = (UINT32)(i4_new_vp_idx_last - i4_new_vp_idx_first) -
                                        (pt_ctrl_logic->t_vp_info.ui4_idx_vp_last - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first);

            for(ui4_start_to_reshow = pt_ctrl_logic->t_vp_info.ui4_idx_vp_last + 1;
                ui4_start_to_reshow <= pt_ctrl_logic->t_vp_info.ui4_idx_vp_last + ui4_extra_item_ctnr_num;
                ui4_start_to_reshow++)
            {
                x_wgl_set_visibility(
                    pt_ctrl_logic->at_item_ctnr_info[ui4_start_to_reshow - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first].h_item_ctnr,
                    WGL_SW_NORMAL);
            }
        }

        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first = (UINT32)i4_new_vp_idx_first;
        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = (UINT32)i4_new_vp_idx_last;

        x_memset(pt_ctrl_logic->at_item_data, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_DATA_T) * ui4_vp_capacity));
        x_memset(pt_ctrl_logic->aui4_item_state, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_STATE_T) * ui4_vp_capacity));
        x_memset(pt_ctrl_logic->at_need_repaint_ex_region, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_EX_REPAINT_REGION) * ui4_vp_capacity));
        /*update item data*/
        MBW_GET_ITEM_DATA(
                        h_mbw,
                        pt_ctrl_logic,
                        WGL_MBW_ITEM_DATA_MASK_NVGT_MAP,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                        pt_ctrl_logic->at_item_data);
        /*update item state*/
        MBW_GET_ITEM_STATE(
                        h_mbw,
                        pt_ctrl_logic,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                        pt_ctrl_logic->aui4_item_state);
        /*update items ctnr*/
        MBW_UPDATE_ITEM_CTNR(
                        h_mbw,
                        pt_ctrl_logic,
                        WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE | WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATIC,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                        pt_ctrl_logic->at_item_ctnr_info);

        pt_ctrl_logic->b_vp_changed = TRUE;

        /*clear the item ctnr region*/
        for(ui4_item_ctnr_idx = 0; ui4_item_ctnr_idx < ui4_vp_capacity; ui4_item_ctnr_idx++)
        {
            pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].b_is_dirty = FALSE;
            i4_ret = x_wgl_mbw_get_item_ctnr_rect(
                        pt_ctrl_logic->h_mbw,
                        ui4_item_ctnr_idx,
                        &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg));
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR:get item container rect faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
    }
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}


INT32 x_wgl_mbw_reg_key_hdlr (
    HANDLE_T                        h_mbw,
    const WGL_MBW_KEY_HDLR_INFO_T*  pt_key_hdlr_info)
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_handle_type;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_key_hdlr_info != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_key_hdlr_info->ui1_strct_ver == 1), WGLR_INV_ARG);
    MBW_CHECK_VALID(((pt_key_hdlr_info->ui4_key_hdlr_mask != 0) ?
                     (pt_key_hdlr_info->pf_key_hdlr != NULL &&
                      (pt_key_hdlr_info->ui4_key_hdlr_mask &
                       (WGL_MBW_KEY_HDLR_MASK_SELECT |
                        WGL_MBW_KEY_HDLR_MASK_NVGT_LEFT|
                        WGL_MBW_KEY_HDLR_MASK_NVGT_RIGHT|
                        WGL_MBW_KEY_HDLR_MASK_NVGT_UP|
                        WGL_MBW_KEY_HDLR_MASK_NVGT_DOWN|
                        WGL_MBW_KEY_HDLR_MASK_NVGT_PAGE_DOWN|
                        WGL_MBW_KEY_HDLR_MASK_NVGT_PAGE_UP))) : (1)), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:register ker hdlr faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when register ker hdlr, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if(pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }
    else
    {
        x_memcpy(&pt_ctrl_logic->t_key_hdlr_info, pt_key_hdlr_info, sizeof(WGL_MBW_KEY_HDLR_INFO_T));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_OK;
    }
}

INT32 x_wgl_mbw_dm_lock(
    HANDLE_T    h_mbw)
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_handle_type;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:dm lock faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when lock datamodel, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);
    pt_ctrl_logic->pv_mbw_ctx = pv_wgl_ctx;

    return WGLR_OK;
}

INT32 x_wgl_mbw_dm_unlock(
    HANDLE_T    h_mbw)
{
    INT32                     i4_ret;
    HANDLE_TYPE_T             e_handle_type;
    WGL_MBW_CONTROL_LOGIC_T*  pt_ctrl_logic;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:dm unlock faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when unlock datamodel, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    REL_LOCK(h_mbw, pt_ctrl_logic->pv_mbw_ctx);
    pt_ctrl_logic->pv_mbw_ctx = NULL;

    return WGLR_OK;
}

INT32 x_wgl_mbw_begin_transition (
    HANDLE_T    h_mbw)
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_handle_type;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:begin transition faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when begin transition, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if(pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
     }
    pt_ctrl_logic->b_is_transition = TRUE;

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_end_transition (
    HANDLE_T    h_mbw)
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_handle_type;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:end transition faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when end transition, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
       return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    pt_ctrl_logic->b_is_transition = FALSE;
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;

}

INT32 x_wgl_mbw_reload (
    HANDLE_T    h_mbw,
    UINT32      ui4_num_total)
{
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;
    INT32                       i4_ret;
    UINT32                      ui4_item_count;
    UINT32                      ui4_vp_total_num;
    UINT32                      ui4_vp_count;
    UINT32                      ui4_hlt_idx;
    UINT32                      ui4_hlt_item_count;
    UINT32                      ui4_item_ctnr_idx;
    UINT32                      ui4_vp_capacity;
    HANDLE_TYPE_T               e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((ui4_num_total <= WGL_MBW_MAX_ITEM_NUM), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:reload data model faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when reload data model, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if(pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }
    pt_ctrl_logic->ui4_item_total_num = ui4_num_total;
    ui4_vp_capacity = pt_ctrl_logic->ui4_col_num * pt_ctrl_logic->ui4_row_num;
    ui4_hlt_idx = WGL_MBW_NULL_INDEX;

    x_memset(pt_ctrl_logic->at_item_data, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_DATA_T) * ui4_vp_capacity));
    x_memset(pt_ctrl_logic->aui4_item_state, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_STATE_T) * ui4_vp_capacity));
    x_memset(pt_ctrl_logic->at_need_repaint_ex_region, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_EX_REPAINT_REGION) * ui4_vp_capacity));

    if (ui4_num_total == 0)
    {
        for(ui4_item_count = 0; ui4_item_count < ui4_vp_capacity;ui4_item_count++)
        {
            i4_ret = x_wgl_set_visibility(pt_ctrl_logic->at_item_ctnr_info[ui4_item_count].h_item_ctnr, WGL_SW_HIDE);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
        }
        i4_ret = x_wgl_set_visibility(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, WGL_SW_HIDE);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first = WGL_MBW_NULL_INDEX;
        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = WGL_MBW_NULL_INDEX;
        pt_ctrl_logic->ui4_hlt_idx = WGL_MBW_NULL_INDEX;
    }
    else
    {
        /*check whether the state of item in dm is consistent*/
        ui4_hlt_item_count = 0;
        ui4_vp_total_num = ui4_num_total / ui4_vp_capacity + ((ui4_num_total % ui4_vp_capacity) ? 1 : 0);
        for (ui4_vp_count = 0; ui4_vp_count < ui4_vp_total_num; ui4_vp_count++)
        {
            UINT32 ui4_idx_start;
            UINT32 ui4_idx_end;
            ui4_idx_start = ui4_vp_count * ui4_vp_capacity;
            ui4_idx_end = (((ui4_vp_count + 1) * ui4_vp_capacity - 1) >= ui4_num_total) ?
                          (ui4_num_total - 1) : ((ui4_vp_count + 1) * ui4_vp_capacity - 1);

            x_memset(pt_ctrl_logic->aui4_item_state, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_STATE_T) * ui4_vp_capacity));
            x_memset(pt_ctrl_logic->at_item_data, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_DATA_T) * ui4_vp_capacity));

            MBW_GET_ITEM_STATE(h_mbw,
                        pt_ctrl_logic,
                        ui4_idx_start,
                        ui4_idx_end,
                        pt_ctrl_logic->aui4_item_state);

            MBW_GET_ITEM_DATA(h_mbw,
                        pt_ctrl_logic,
                        WGL_MBW_ITEM_DATA_MASK_NVGT_MAP,
                        ui4_idx_start,
                        ui4_idx_end,
                        pt_ctrl_logic->at_item_data);

            for (ui4_item_count = 0; ui4_item_count <= (ui4_idx_end - ui4_idx_start); ui4_item_count++)
            {
                if (pt_ctrl_logic->aui4_item_state[ui4_item_count] & WGL_MBW_ITEM_STATE_HLTED)
                {
                    ui4_hlt_item_count++;
                    ui4_hlt_idx = ui4_item_count + ui4_vp_count * ui4_vp_capacity;
                    if (ui4_hlt_item_count > 1)
                    {
                        DBG_ERROR(("{WGL(MBW)} ERR: item state in dm is not consistent at L%d\n\n", __LINE__));
                        REL_LOCK(h_mbw, pv_wgl_ctx);
                        return WGLR_MBW_DM_INCONSISTENT_ITEM_STATE;
                    }
                }

                if ((pt_ctrl_logic->aui4_item_state[ui4_item_count] & WGL_MBW_ITEM_STATE_HLTED) &&
                    (pt_ctrl_logic->aui4_item_state[ui4_item_count] & WGL_MBW_ITEM_STATE_DISABLED))
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: item state in dm is not consistent at L%d\n\n", __LINE__));
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    return WGLR_MBW_DM_INCONSISTENT_ITEM_STATE;
                }

                if (!MBW_IS_VALID_ITEM_DATA(pt_ctrl_logic->at_item_data[ui4_item_count]))
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: item data in dm is not consistent at L%d\n\n", __LINE__));
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    return WGLR_MBW_DM_INCONSISTENT_ITEM_DATA;
                }
            }
        }
        /*no hlt item in dm*/
        if (ui4_hlt_item_count == 0)
        {
            if (ui4_num_total >= ui4_vp_capacity)
            {
                pt_ctrl_logic->t_vp_info.ui4_idx_vp_first = 0;
                pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = ui4_vp_capacity -1;
            }
            else
            {
                pt_ctrl_logic->t_vp_info.ui4_idx_vp_first = 0;
                pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = ui4_num_total -1;
            }
        }
        /*has one hlt item in dm*/
        else
        {
            UINT32 ui4_page_offset;
            /*get the page which contains the new item*/
            ui4_page_offset = ui4_hlt_idx / ui4_vp_capacity;
            pt_ctrl_logic->t_vp_info.ui4_idx_vp_first = ui4_page_offset * ui4_vp_capacity;
            pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = pt_ctrl_logic->t_vp_info.ui4_idx_vp_first + ui4_vp_capacity - 1;
            /*the last item index in viewport may larger than the total item number*/
            if (pt_ctrl_logic->t_vp_info.ui4_idx_vp_last >= ui4_num_total)
            {
                pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = ui4_num_total - 1;
            }
        }

        /*set or reset the items' visibility*/
        for(ui4_item_count = 0; ui4_item_count < ui4_vp_capacity;ui4_item_count++)
        {
            if (ui4_item_count + pt_ctrl_logic->t_vp_info.ui4_idx_vp_first <= pt_ctrl_logic->t_vp_info.ui4_idx_vp_last)
            {
                i4_ret = x_wgl_set_visibility(pt_ctrl_logic->at_item_ctnr_info[ui4_item_count].h_item_ctnr, WGL_SW_NORMAL);
                if (WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    return i4_ret;
                }
            }
            else
            {
                i4_ret = x_wgl_set_visibility(pt_ctrl_logic->at_item_ctnr_info[ui4_item_count].h_item_ctnr, WGL_SW_HIDE);
                if (WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    return i4_ret;
                }
            }
        }
        x_memset(pt_ctrl_logic->aui4_item_state, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_STATE_T) * ui4_vp_capacity));
        x_memset(pt_ctrl_logic->at_item_data, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_DATA_T) * ui4_vp_capacity));
        /*update catched item data, state, and the item containers*/
        MBW_GET_ITEM_DATA(h_mbw,
                        pt_ctrl_logic,
                        WGL_MBW_ITEM_DATA_MASK_NVGT_MAP,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                        pt_ctrl_logic->at_item_data);
        MBW_GET_ITEM_STATE(h_mbw,
                        pt_ctrl_logic,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                        pt_ctrl_logic->aui4_item_state);
        MBW_UPDATE_ITEM_CTNR(h_mbw,
                        pt_ctrl_logic,
                        WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE | WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATIC,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                        pt_ctrl_logic->at_item_ctnr_info);

        if (ui4_hlt_item_count == 0)
        {
            i4_ret = x_wgl_set_visibility(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, WGL_SW_HIDE);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
            pt_ctrl_logic->ui4_hlt_idx = WGL_MBW_NULL_INDEX;
        }
        else
        {
            pt_ctrl_logic->ui4_hlt_idx = ui4_hlt_idx;
            i4_ret = _mbw_show_hlt_item(pt_ctrl_logic);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: Fail to show hlt item (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
        }
    }
    /*clear the item ctnr region*/
    for(ui4_item_ctnr_idx = 0; ui4_item_ctnr_idx < ui4_vp_capacity; ui4_item_ctnr_idx++)
    {
        pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].b_is_dirty = FALSE;
        i4_ret = x_wgl_mbw_get_item_ctnr_rect(
                    pt_ctrl_logic->h_mbw,
                    ui4_item_ctnr_idx,
                    &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg));
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR:get item container rect faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    pt_ctrl_logic->b_vp_changed = TRUE;
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_reload_with_hlt (
    HANDLE_T    h_mbw,
    UINT32      ui4_num_total,
    UINT32      ui4_hlt_item_idx)
{
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;
    INT32                       i4_ret;
    UINT32                      ui4_item_count;
    UINT32                      ui4_hlt_idx;
    UINT32                      ui4_item_ctnr_idx;
    UINT32                      ui4_vp_capacity;
    UINT32                      ui4_item_state;
    HANDLE_TYPE_T               e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((ui4_num_total <= WGL_MBW_MAX_ITEM_NUM &&
                     (ui4_hlt_item_idx < ui4_num_total ||
                     ui4_hlt_item_idx == WGL_MBW_NULL_INDEX)
                     ), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:reload data model faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when reload data model, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if(pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }
    pt_ctrl_logic->ui4_item_total_num = ui4_num_total;
    ui4_vp_capacity = pt_ctrl_logic->ui4_col_num * pt_ctrl_logic->ui4_row_num;
    ui4_hlt_idx = ui4_hlt_item_idx;

    x_memset(pt_ctrl_logic->at_item_data, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_DATA_T) * ui4_vp_capacity));
    x_memset(pt_ctrl_logic->aui4_item_state, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_STATE_T) * ui4_vp_capacity));
    x_memset(pt_ctrl_logic->at_need_repaint_ex_region, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_EX_REPAINT_REGION) * ui4_vp_capacity));

    if (ui4_num_total == 0)
    {
        for(ui4_item_count = 0; ui4_item_count < ui4_vp_capacity;ui4_item_count++)
        {
            i4_ret = x_wgl_set_visibility(pt_ctrl_logic->at_item_ctnr_info[ui4_item_count].h_item_ctnr, WGL_SW_HIDE);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
        }
        i4_ret = x_wgl_set_visibility(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, WGL_SW_HIDE);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first = WGL_MBW_NULL_INDEX;
        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = WGL_MBW_NULL_INDEX;
        pt_ctrl_logic->ui4_hlt_idx = WGL_MBW_NULL_INDEX;
    }
    else
    {
        /*no hlt item in dm*/
        if (ui4_hlt_idx == WGL_MBW_NULL_INDEX)
        {
            if (ui4_num_total >= ui4_vp_capacity)
            {
                pt_ctrl_logic->t_vp_info.ui4_idx_vp_first = 0;
                pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = ui4_vp_capacity -1;
            }
            else
            {
                pt_ctrl_logic->t_vp_info.ui4_idx_vp_first = 0;
                pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = ui4_num_total -1;
            }
        }
        /*has one hlt item in dm*/
        else
        {
            UINT32 ui4_page_offset;
            /*get the page which contains the new item*/
            ui4_page_offset = ui4_hlt_idx / ui4_vp_capacity;
            pt_ctrl_logic->t_vp_info.ui4_idx_vp_first = ui4_page_offset * ui4_vp_capacity;
            pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = pt_ctrl_logic->t_vp_info.ui4_idx_vp_first + ui4_vp_capacity - 1;
            /*the last item index in viewport may larger than the total item number*/
            if (pt_ctrl_logic->t_vp_info.ui4_idx_vp_last >= ui4_num_total)
            {
                pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = ui4_num_total - 1;
            }
        }

        /*set or reset the items' visibility*/
        for(ui4_item_count = 0; ui4_item_count < ui4_vp_capacity;ui4_item_count++)
        {
            if (ui4_item_count + pt_ctrl_logic->t_vp_info.ui4_idx_vp_first <= pt_ctrl_logic->t_vp_info.ui4_idx_vp_last)
            {
                i4_ret = x_wgl_set_visibility(pt_ctrl_logic->at_item_ctnr_info[ui4_item_count].h_item_ctnr, WGL_SW_NORMAL);
                if (WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    return i4_ret;
                }
            }
            else
            {
                i4_ret = x_wgl_set_visibility(pt_ctrl_logic->at_item_ctnr_info[ui4_item_count].h_item_ctnr, WGL_SW_HIDE);
                if (WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
                    REL_LOCK(h_mbw, pv_wgl_ctx);
                    return i4_ret;
                }
            }
        }
        x_memset(pt_ctrl_logic->aui4_item_state, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_STATE_T) * ui4_vp_capacity));
        x_memset(pt_ctrl_logic->at_item_data, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_DATA_T) * ui4_vp_capacity));
        /*update catched item data, state, and the item containers*/
        MBW_GET_ITEM_DATA(h_mbw,
                        pt_ctrl_logic,
                        WGL_MBW_ITEM_DATA_MASK_NVGT_MAP,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                        pt_ctrl_logic->at_item_data);
        MBW_GET_ITEM_STATE(h_mbw,
                        pt_ctrl_logic,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                        pt_ctrl_logic->aui4_item_state);
        if (ui4_hlt_idx != WGL_MBW_NULL_INDEX)
        {
            /*check input state mask if is contained in current item state and confilict with current state*/
            ui4_item_state = pt_ctrl_logic->aui4_item_state[ui4_hlt_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first];
            if (ui4_item_state & WGL_MBW_ITEM_STATE_DISABLED)
            {
                DBG_ERROR(("{WGL(MBW)} ERR:enable item highlight state faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return WGLR_INV_ARG;
            }
            /* call dm to enable item state */
            pt_ctrl_logic->t_data_model.t_fct_tbl.pf_enable_item_state(
                pt_ctrl_logic->pv_dm_data,
                h_mbw,
                ui4_hlt_idx,
                WGL_MBW_ITEM_STATE_HLTED);
            
            /* update cached  item state */
            pt_ctrl_logic->aui4_item_state[ui4_hlt_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first] |= WGL_MBW_ITEM_STATE_HLTED;
        }
        MBW_UPDATE_ITEM_CTNR(h_mbw,
                        pt_ctrl_logic,
                        WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE | WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATIC,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                        pt_ctrl_logic->at_item_ctnr_info);

        if (ui4_hlt_idx == WGL_MBW_NULL_INDEX)
        {
            i4_ret = x_wgl_set_visibility(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, WGL_SW_HIDE);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
            pt_ctrl_logic->ui4_hlt_idx = WGL_MBW_NULL_INDEX;
        }
        else
        {
            pt_ctrl_logic->ui4_hlt_idx = ui4_hlt_idx;
            i4_ret = _mbw_show_hlt_item(pt_ctrl_logic);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: Fail to show hlt item (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
        }
    }
    /*clear the item ctnr region*/
    for(ui4_item_ctnr_idx = 0; ui4_item_ctnr_idx < ui4_vp_capacity; ui4_item_ctnr_idx++)
    {
        pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].b_is_dirty = FALSE;
        i4_ret = x_wgl_mbw_get_item_ctnr_rect(
                    pt_ctrl_logic->h_mbw,
                    ui4_item_ctnr_idx,
                    &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg));
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR:get item container rect faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    pt_ctrl_logic->b_vp_changed = TRUE;
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_get_total_num (
    HANDLE_T                h_mbw,
    UINT32*                 pui4_num_total)
{
    INT32                        i4_ret;
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;
    HANDLE_TYPE_T                e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pui4_num_total != NULL), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:get total num faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when get total num, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    *pui4_num_total = pt_ctrl_logic->ui4_item_total_num;

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;

}

INT32 x_wgl_mbw_reload_item_static (
    HANDLE_T                h_mbw,
    UINT32                  ui4_idx)
{
    INT32                       i4_ret;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;
    HANDLE_TYPE_T               e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:reload data model faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when reload item, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    MBW_CHECK_VALID((ui4_idx < pt_ctrl_logic->ui4_item_total_num), WGLR_INV_ARG);

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if(pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }

    if (ui4_idx >= pt_ctrl_logic->t_vp_info.ui4_idx_vp_first && ui4_idx <= pt_ctrl_logic->t_vp_info.ui4_idx_vp_last)
    {
        WGL_MBW_ITEM_CTNR_INFO_T* at_item_ctnr_info;
        i4_ret = _mbw_get_item_ctnr_info(pt_ctrl_logic, ui4_idx, &at_item_ctnr_info);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: get item ctnr faild.at L%d\n\n",  __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
        MBW_UPDATE_ITEM_CTNR(h_mbw,
                        pt_ctrl_logic,
                        WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATIC,
                        ui4_idx,
                        ui4_idx,
                        at_item_ctnr_info);

        if (ui4_idx == pt_ctrl_logic->ui4_hlt_idx)
        {
            MBW_UPDATE_HLT_ITEM_CTNR(
                    h_mbw,
                    pt_ctrl_logic,
                    WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATIC);
        }
    }

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_reload_item_data (
    HANDLE_T                h_mbw,
    UINT32                  ui4_idx)
{
    INT32                       i4_ret;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;
    HANDLE_TYPE_T               e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:reload data model faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: when reload item, get attached data faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    MBW_CHECK_VALID((ui4_idx < pt_ctrl_logic->ui4_item_total_num), WGLR_INV_ARG);

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if(pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }

    if (ui4_idx >= pt_ctrl_logic->t_vp_info.ui4_idx_vp_first && ui4_idx <= pt_ctrl_logic->t_vp_info.ui4_idx_vp_last)
    {
        UINT32    ui4_ctnr_idx;
        ui4_ctnr_idx = ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;
        MBW_GET_ITEM_DATA(h_mbw,
                        pt_ctrl_logic,
                        WGL_MBW_ITEM_DATA_MASK_NVGT_MAP,
                        ui4_idx,
                        ui4_idx,
                        &pt_ctrl_logic->at_item_data[ui4_ctnr_idx]);
        if (!MBW_IS_VALID_ITEM_DATA(pt_ctrl_logic->at_item_data[ui4_ctnr_idx]))
        {
            DBG_ERROR(("{WGL(MBW)} ERR: item data in dm is not consistent at L%d\n\n", __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return WGLR_MBW_DM_INCONSISTENT_ITEM_DATA;
        }
    }

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}


INT32 x_wgl_mbw_enable_item_state (
    HANDLE_T                h_mbw,
    UINT32                  ui4_idx,
    UINT32                  ui4_state_mask)
{
    INT32                          i4_ret;
    WGL_MBW_CONTROL_LOGIC_T*       pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T    pv_wgl_ctx;
    HANDLE_TYPE_T                  e_handle_type;
    UINT32                         ui4_item_state;
    UINT32                         ui4_pre_hlt_index;
    GL_RECT_T                      t_vp_rect = {0};
    WGL_MBW_ITEM_EX_REPAINT_REGION t_pre_hlt_dirty_region = {0};

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    ui4_pre_hlt_index = WGL_MBW_NULL_INDEX;
    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:enable item state faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);
    /*check input item if is out of boudary*/
    if(ui4_idx >= pt_ctrl_logic->ui4_item_total_num)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:enable item state faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_INV_ARG;
    }
    /*check input state mask if is contained in current item state and confilict with current state*/
    i4_ret= x_wgl_mbw_get_item_state(h_mbw,ui4_idx,&ui4_item_state);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    ui4_state_mask &= (~ui4_item_state);
    if (!ui4_state_mask)
    {
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_OK;
    }
    if (((ui4_state_mask & WGL_MBW_ITEM_STATE_DISABLED) && (ui4_state_mask & WGL_MBW_ITEM_STATE_HLTED)) ||
        ((ui4_state_mask & WGL_MBW_ITEM_STATE_DISABLED) && (ui4_item_state & WGL_MBW_ITEM_STATE_HLTED)) ||
        ((ui4_state_mask & WGL_MBW_ITEM_STATE_HLTED) && (ui4_item_state & WGL_MBW_ITEM_STATE_DISABLED)))
    {
        DBG_ERROR(("{WGL(MBW)} ERR:enable item state faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_INV_ARG;
    }
    /* call dm to enable item state */
    pt_ctrl_logic->t_data_model.t_fct_tbl.pf_enable_item_state(
        pt_ctrl_logic->pv_dm_data,
        h_mbw,
        ui4_idx,
        ui4_state_mask);

    /* enable item to highlighted state */
    if(ui4_state_mask & WGL_MBW_ITEM_STATE_HLTED)
    {
        if(pt_ctrl_logic->ui4_hlt_idx != WGL_MBW_NULL_INDEX)
        {
            ui4_pre_hlt_index = pt_ctrl_logic->ui4_hlt_idx;
            i4_ret =  x_wgl_mbw_disable_item_state(h_mbw, pt_ctrl_logic->ui4_hlt_idx, WGL_MBW_ITEM_STATE_HLTED);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
            /* record pre-highlighted item dirty region*/
            x_memcpy(
                &t_pre_hlt_dirty_region,
                &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_pre_hlt_index - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first]),
                sizeof(WGL_MBW_ITEM_EX_REPAINT_REGION));
        }
        /* ensure new highlight item is in vp range */
        if(ui4_idx < pt_ctrl_logic->t_vp_info.ui4_idx_vp_first || ui4_idx > pt_ctrl_logic->t_vp_info.ui4_idx_vp_last)
        {
            i4_ret = _mbw_update_vp_to_show_item(pt_ctrl_logic,ui4_idx);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR:show hlt item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
            /*after update vp,let ui4_idx in vp range,need look as whole vp as item dirty region*/
            i4_ret = x_wgl_get_coords(pt_ctrl_logic->h_mbw_vp, WGL_COORDS_PARENT, &t_vp_rect);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
            pt_ctrl_logic->at_need_repaint_ex_region[ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first].b_is_dirty = TRUE;
            UNION_RECT(
                &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first].t_dirty_reg),
                &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first].t_dirty_reg),
                &t_vp_rect);
        }
        /* union pre-highlighted item dirty region */
        if(t_pre_hlt_dirty_region.b_is_dirty)
        {
            pt_ctrl_logic->at_need_repaint_ex_region[ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first].b_is_dirty = TRUE;
            UNION_RECT(
                &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first].t_dirty_reg),
                &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first].t_dirty_reg),
                &t_pre_hlt_dirty_region.t_dirty_reg);
        }
        /* update cached  item state */
        pt_ctrl_logic->aui4_item_state[ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first] |= ui4_state_mask;
        pt_ctrl_logic->ui4_hlt_idx = ui4_idx;
        /* show highlighted item container */
        i4_ret = _mbw_show_hlt_item(pt_ctrl_logic);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR:show hlt item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_OK;
    }

    /*item is not in current vp, do nothing*/
    if(ui4_idx < pt_ctrl_logic->t_vp_info.ui4_idx_vp_first || ui4_idx > pt_ctrl_logic->t_vp_info.ui4_idx_vp_last)
    {
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_OK;
    }
    /*update cached  item state*/
    pt_ctrl_logic->aui4_item_state[ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first] |= ui4_state_mask;

    /*item is in current vp, and the state mask to be enabled does not include HLT*/
    if (ui4_idx == pt_ctrl_logic->ui4_hlt_idx)
    {
        MBW_UPDATE_HLT_ITEM_CTNR(
                            h_mbw,
                            pt_ctrl_logic,
                            WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE);
    }
    else
    {
        WGL_MBW_ITEM_CTNR_INFO_T* at_item_ctnr_info;
        i4_ret = _mbw_get_item_ctnr_info(pt_ctrl_logic, ui4_idx, &at_item_ctnr_info);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: get item ctnr faild.at L%d\n\n",  __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
        MBW_UPDATE_ITEM_CTNR(
                        h_mbw,
                        pt_ctrl_logic,
                        WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE,
                        ui4_idx,
                        ui4_idx,
                        at_item_ctnr_info);
    }
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}


INT32 x_wgl_mbw_disable_item_state (
    HANDLE_T                h_mbw,
    UINT32                  ui4_idx,
    UINT32                  ui4_state_mask)
{
    INT32                       i4_ret;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;
    UINT32                      ui4_item_state;
    HANDLE_TYPE_T               e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:disable item state faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

     /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    /*check input item if is out of boudary*/
    if(ui4_idx >= pt_ctrl_logic->ui4_item_total_num)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:disable item state faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_INV_ARG;
    }
    /*check input state mask if is contained in current item state*/
    i4_ret = x_wgl_mbw_get_item_state(h_mbw,ui4_idx,&ui4_item_state);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    /*if current item state has not input state mask, do nothing. */
    ui4_state_mask &= ui4_item_state;
    if (!ui4_state_mask)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:disable item state faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_INV_ARG;
    }
    /*call dm to disable item state*/
    pt_ctrl_logic->t_data_model.t_fct_tbl.pf_disable_item_state(
        pt_ctrl_logic->pv_dm_data,
        h_mbw,
        ui4_idx,
        ui4_state_mask);

    /*item is not in  view port range */
    if(ui4_idx < pt_ctrl_logic->t_vp_info.ui4_idx_vp_first || ui4_idx > pt_ctrl_logic->t_vp_info.ui4_idx_vp_last)
    {
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_OK;
    }
    /*update cached item state*/
    pt_ctrl_logic->aui4_item_state[ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first] &= (~ui4_state_mask);

    if(!(ui4_state_mask & WGL_MBW_ITEM_STATE_HLTED))
    {
        if(ui4_idx == pt_ctrl_logic->ui4_hlt_idx)
        {
            MBW_UPDATE_HLT_ITEM_CTNR(
                h_mbw,
                pt_ctrl_logic,
                WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE);
        }
        else
        {
            WGL_MBW_ITEM_CTNR_INFO_T* at_item_ctnr_info;
            i4_ret = _mbw_get_item_ctnr_info(pt_ctrl_logic, ui4_idx, &at_item_ctnr_info);
            if (i4_ret != WGLR_OK)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: get item ctnr faild.at L%d\n\n",  __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
            MBW_UPDATE_ITEM_CTNR(
                h_mbw,
                pt_ctrl_logic,
                WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE,
                ui4_idx,
                ui4_idx,
                at_item_ctnr_info);
        }
    }
    else
    {
        if (pt_ctrl_logic->ui4_hlt_idx != WGL_MBW_NULL_INDEX)
        {
            i4_ret = _mbw_hide_hlt_item(pt_ctrl_logic);
            if (i4_ret != WGLR_OK)
            {
                DBG_ERROR(("{WGL(MBW)} ERR:hide hlt item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
        }
        pt_ctrl_logic->ui4_hlt_idx = WGL_MBW_NULL_INDEX;
    }

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_get_item_state (
    HANDLE_T                h_mbw,
    UINT32                  ui4_idx,
    UINT32*                 pui4_state)
{
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pui4_state != NULL), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:get item state faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    /*check input item if is out of boudary*/
    if(ui4_idx >= pt_ctrl_logic->ui4_item_total_num)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:disable item state faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_INV_ARG;
    }

    if(ui4_idx>=pt_ctrl_logic->t_vp_info.ui4_idx_vp_first &&
        ui4_idx<=pt_ctrl_logic->t_vp_info.ui4_idx_vp_last)
    {
       /*item index is in current view port range,get item state from control logic cached state array*/
        *pui4_state = pt_ctrl_logic->aui4_item_state[ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first];
    }
    else
    {
        /*get item state from dm*/
        WGL_MBW_ITEM_STATE_T  at_item_state[1];
        MBW_GET_ITEM_STATE(
                        h_mbw,
                        pt_ctrl_logic,
                        ui4_idx,
                        ui4_idx,
                        at_item_state);

        *pui4_state = at_item_state[0];
    }
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_get_hlt_item (
    HANDLE_T                h_mbw,
    UINT32*                 pui4_idx_hlt)
{
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pui4_idx_hlt != NULL), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:get highlight  item  faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    *pui4_idx_hlt =  pt_ctrl_logic->ui4_hlt_idx;

    if((*pui4_idx_hlt) == WGL_MBW_NULL_INDEX)
    {
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NO_HIGHLIGHT_ITEM;
    }

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}


INT32 x_wgl_mbw_set_key_map (
    HANDLE_T                    h_mbw,
    const WGL_MBW_KEY_MAP_T*    pt_key_map)
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_handle_type;
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_key_map != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_key_map->ui1_strct_ver == 1), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:set key map faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    /*can not allowd in transition state*/
    if(pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }

    x_memcpy(&pt_ctrl_logic->t_key_map,pt_key_map, sizeof(WGL_MBW_KEY_MAP_T));
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_get_visible_range_in_vp (
    HANDLE_T                    h_mbw,
    UINT32*                     pui4_first,
    UINT32*                     pui4_last)
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_handle_type;
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pui4_first != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pui4_last != NULL), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:set key map faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    *pui4_first = pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;
    *pui4_last = pt_ctrl_logic->t_vp_info.ui4_idx_vp_last;
    
    if (pt_ctrl_logic->ui4_item_total_num == 0)
    {
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_INVALID_VP_RANGE;
    }

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}


INT32 x_wgl_mbw_get_item_ctnr_rect (
    HANDLE_T                h_mbw,
    UINT32                  ui4_item_ctnr_idx,
    GL_RECT_T*              pt_rect)
{
    INT32                       i4_ret;
    UINT32                      ui4_item_ctnr_num;
    HANDLE_TYPE_T               e_handle_type;
    GL_RECT_T                   t_vp_rect;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_rect != NULL), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:get item container rect faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    /*check input argument if is valid*/
    ui4_item_ctnr_num = (pt_ctrl_logic->ui4_row_num * pt_ctrl_logic->ui4_col_num);
    if ((ui4_item_ctnr_idx >= ui4_item_ctnr_num))
    {
        DBG_ERROR(("{WGL(MBW)} ERR:get item container rect faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_INV_ARG;
    }

    i4_ret = x_wgl_get_coords(pt_ctrl_logic->at_item_ctnr_info[ui4_item_ctnr_idx].h_item_ctnr, WGL_COORDS_PARENT, pt_rect);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    /*return rect coordinate need to be  based on matrix box but item container created is based on vp container,
       so need do coordinate transfer*/
    i4_ret = x_wgl_get_coords(pt_ctrl_logic->h_mbw_vp,WGL_COORDS_PARENT,&t_vp_rect);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    pt_rect->i4_top += t_vp_rect.i4_top;
    pt_rect->i4_left += t_vp_rect.i4_left;
    pt_rect->i4_bottom += t_vp_rect.i4_top;
    pt_rect->i4_right += t_vp_rect.i4_left;

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_set_image (
    HANDLE_T                h_mbw,
    UINT8                   ui1_target,
    const WGL_IMG_INFO_T*   pt_img_info)
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_handle_type;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((ui1_target == WGL_IMG_MBW_VP_BK), WGLR_INV_ARG);

    /*check pt_img_info*/
    if(pt_img_info && FALSE == wgl_is_legal_img_info(pt_img_info))
    {
        DBG_ERROR(("{WGL(MBW)} ERR: The image info is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:set imge failed. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if (pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }

    /*set input image info to control logic structure*/
    if(pt_img_info)
    {
        COPY_IMG_INFO_TO_STD(&pt_ctrl_logic->t_vp_bk_img, pt_img_info, NULL_HANDLE);
        WGL_CLEAR_FLAG(pt_ctrl_logic->ui1_extra_flag, MBW_VP_BK_IMG_NOT_INIT);
    }
    else
    {
        x_memset(&pt_ctrl_logic->t_vp_bk_img, 0, sizeof(WGL_IMG_STD_SET_T));

        /*reset the flag for bk image*/
        WGL_SET_FLAG(pt_ctrl_logic->ui1_extra_flag, MBW_VP_BK_IMG_NOT_INIT);
    }

    /*update vp background image*/
    i4_ret = _mbw_set_vp_bk_image(pt_ctrl_logic);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_get_image (
    HANDLE_T                h_mbw,
    UINT8                   ui1_target,
    WGL_IMG_INFO_T*         pt_img_info)
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_handle_type;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((ui1_target == WGL_IMG_MBW_VP_BK), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_img_info != NULL), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:set imge failed. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    /*get vp bk image info from control logic structure*/
    SET_STANDARD_TO_IMG_INFO(pt_img_info, &pt_ctrl_logic->t_vp_bk_img);

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_set_color (
    HANDLE_T                h_mbw,
    UINT8                   ui1_target,
    const WGL_COLOR_INFO_T* pt_clr_info)
{
    INT32                       i4_ret;
    GL_COLOR_T                  t_default_color = {0};
    HANDLE_TYPE_T               e_handle_type;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((ui1_target == WGL_CLR_MBW_VP_BK), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:set imge failed. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if (pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }

    /*set input color info to control logic structure*/
    if(pt_clr_info)
    {
        /*copy bk color*/
        COPY_CLR_INFO_TO_STD(&pt_ctrl_logic->t_vp_bk_clr, pt_clr_info, t_default_color);

        /*remove the flag for bk color*/
        WGL_CLEAR_FLAG(pt_ctrl_logic->ui1_extra_flag, MBW_VP_BK_CLR_NOT_INIT);
    }
    else
    {
        x_memset(&pt_ctrl_logic->t_vp_bk_clr, 0, sizeof(WGL_COLOR_STD_SET_T));
        /*reset the flag for bk color*/
        WGL_SET_FLAG(pt_ctrl_logic->ui1_extra_flag, MBW_VP_BK_CLR_NOT_INIT);
    }

    /*update vp background color*/
    i4_ret = _mbw_set_vp_bk_color(pt_ctrl_logic);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_get_color (
    HANDLE_T                h_mbw,
    UINT8                   ui1_target,
    WGL_COLOR_INFO_T*       pt_clr_info)
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_handle_type;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((ui1_target == WGL_CLR_MBW_VP_BK), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_clr_info != NULL), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
         DBG_ERROR(("{WGL(MBW)} ERR:set imge failed. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    /*get color info from control logic structure*/
    SET_STANDARD_TO_COLOR_INFO(pt_clr_info, &pt_ctrl_logic->t_vp_bk_clr);

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}


INT32 x_wgl_mbw_reg_event_hdlr(
    HANDLE_T                       h_mbw,
    const WGL_MBW_EVN_HDLR_INFO_T* pt_evn_hdlr_info)
{
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_evn_hdlr_info != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_evn_hdlr_info->ui1_strct_ver == 1), WGLR_INV_ARG);
    MBW_CHECK_VALID(((pt_evn_hdlr_info->ui4_evn_hdlr_mask != 0) ?
                     (pt_evn_hdlr_info->pf_event_hdlr != NULL &&
                      (pt_evn_hdlr_info->ui4_evn_hdlr_mask &
                       (WGL_MBW_EVN_HDLR_MASK_NVGT_IN_VP |
                        WGL_MBW_EVN_HDLR_MASK_NVGT_WITH_VP_CHG |
                        WGL_MBW_EVN_HDLR_MASK_SELECT|
                        WGL_MBW_EVN_HDLR_MASK_DESEL))) : (1)), WGLR_INV_ARG);

     /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:set event hdlr info failed. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if(pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }

    x_memcpy(&pt_ctrl_logic->t_evn_hdlr_info, pt_evn_hdlr_info, sizeof(WGL_MBW_EVN_HDLR_INFO_T));

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_set_hlt_item_info (
    HANDLE_T                        h_mbw,
    const WGL_MBW_HLT_ITEM_INFO_T*  pt_hlt_info)
{
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;
    INT32                        i4_ret;
    GL_RECT_T                    t_hlt_item_ctnr_rect_old;
    GL_RECT_T                    t_hlt_item_ctnr_rect;
    HANDLE_TYPE_T                e_handle_type;
    UINT32                       ui4_item_ctnr_idx;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_hlt_info != NULL), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_hlt_info->ui1_strct_ver == 1), WGLR_INV_ARG);
    MBW_CHECK_VALID((!(pt_hlt_info->ui4_style & (~((WGL_MBW_HLT_ITEM_STL_LAST_STYLE << 1) - 1)))), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:set highlight item info failed. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if (pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }

    /*update highlight item container rect*/
    i4_ret = x_wgl_get_coords(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, WGL_COORDS_PARENT, &t_hlt_item_ctnr_rect_old);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    t_hlt_item_ctnr_rect.i4_left   = t_hlt_item_ctnr_rect_old.i4_left   + (INT16)pt_ctrl_logic->t_hlt_item_info.t_ex_margin.ui2_left;
    t_hlt_item_ctnr_rect.i4_right  = t_hlt_item_ctnr_rect_old.i4_right  - (INT16)pt_ctrl_logic->t_hlt_item_info.t_ex_margin.ui2_right;
    t_hlt_item_ctnr_rect.i4_top    = t_hlt_item_ctnr_rect_old.i4_top    + (INT16)pt_ctrl_logic->t_hlt_item_info.t_ex_margin.ui2_top;
    t_hlt_item_ctnr_rect.i4_bottom = t_hlt_item_ctnr_rect_old.i4_bottom - (INT16)pt_ctrl_logic->t_hlt_item_info.t_ex_margin.ui2_bottom;

    if (pt_hlt_info->ui4_style == 0)
    {
        pt_ctrl_logic->t_hlt_item_info.ui1_strct_ver = 1;
        pt_ctrl_logic->t_hlt_item_info.ui4_style = 0;
        pt_ctrl_logic->t_hlt_item_info.t_ex_margin.ui2_left   = 0;
        pt_ctrl_logic->t_hlt_item_info.t_ex_margin.ui2_right  = 0;
        pt_ctrl_logic->t_hlt_item_info.t_ex_margin.ui2_top    = 0;
        pt_ctrl_logic->t_hlt_item_info.t_ex_margin.ui2_bottom = 0;
    }
    else if (pt_hlt_info->ui4_style & WGL_MBW_HLT_ITEM_STL_ENLRG_SIZE)
    {
        t_hlt_item_ctnr_rect.i4_left   -= (INT16)pt_hlt_info->t_ex_margin.ui2_left;
        t_hlt_item_ctnr_rect.i4_right  += (INT16)pt_hlt_info->t_ex_margin.ui2_right;
        t_hlt_item_ctnr_rect.i4_top    -= (INT16)pt_hlt_info->t_ex_margin.ui2_top;
        t_hlt_item_ctnr_rect.i4_bottom += (INT16)pt_hlt_info->t_ex_margin.ui2_bottom;
        /*set input hlt info to control logic structure*/
        x_memcpy(&pt_ctrl_logic->t_hlt_item_info, pt_hlt_info, sizeof(WGL_MBW_HLT_ITEM_INFO_T));
    }

    /*move highlight container to new position*/
    i4_ret = x_wgl_move(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, &t_hlt_item_ctnr_rect, WGL_NO_AUTO_REPAINT);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    /*current highlight item in vp range*/
    if (pt_ctrl_logic->ui4_hlt_idx != WGL_MBW_NULL_INDEX &&
        (pt_ctrl_logic->ui4_hlt_idx >= pt_ctrl_logic->t_vp_info.ui4_idx_vp_first &&
        pt_ctrl_logic->ui4_hlt_idx <= pt_ctrl_logic->t_vp_info.ui4_idx_vp_last))
    {
        /*item need repaint extend region, otherwise will occur ui garbage*/
        ui4_item_ctnr_idx = pt_ctrl_logic->ui4_hlt_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;
        pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].b_is_dirty = TRUE;
        UNION_RECT(
            &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg),
            &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg),
            &t_hlt_item_ctnr_rect_old);
    }
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_get_hlt_item_info (
    HANDLE_T                      h_mbw,
    WGL_MBW_HLT_ITEM_INFO_T*      pt_hlt_info)
{
    WGL_MBW_CONTROL_LOGIC_T*      pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T   pv_wgl_ctx;
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_hlt_info != NULL), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:get highlight item info failed. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }

    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    /*return highlight item info*/
    x_memcpy(pt_hlt_info, &pt_ctrl_logic->t_hlt_item_info, sizeof(WGL_MBW_HLT_ITEM_INFO_T));
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_get_hlt_item_ctnr_info(
    HANDLE_T                        h_mbw,
    WGL_MBW_ITEM_CTNR_INFO_T*       pt_hlt_item_ctnr_info)
{
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    MBW_CHECK_VALID((pt_hlt_item_ctnr_info != NULL), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:get highlight item info failed. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);
    /*return highlight item container info*/
    x_memcpy(pt_hlt_item_ctnr_info, &pt_ctrl_logic->t_hlt_item_ctnr_info, sizeof(WGL_MBW_ITEM_CTNR_INFO_T));
    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_move_item_ctnr (
    HANDLE_T                      h_mbw,
    UINT32                        ui4_item_ctnr_idx,
    INT32                         i4_offset_x,
    INT32                         i4_offset_y)
{
    WGL_MBW_CONTROL_LOGIC_T*      pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T   pv_wgl_ctx;
    INT32                         i4_ret;
    UINT32                        ui4_item_idx;
    GL_RECT_T                     t_item_ctnr_rect_old;
    GL_RECT_T                     t_hlt_item_ctnr_rect;
    GL_RECT_T                     t_item_ctnr_rect;
    HANDLE_TYPE_T                 e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:move item container faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
     /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    /*check input argument if is valid*/
    if (ui4_item_ctnr_idx >= pt_ctrl_logic->ui4_row_num * pt_ctrl_logic->ui4_col_num)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, WGLR_INV_ARG));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_INV_ARG;
    }

    /*get the item ctnr rect based on mbw*/
    i4_ret = x_wgl_mbw_get_item_ctnr_rect(h_mbw, ui4_item_ctnr_idx, &t_item_ctnr_rect_old);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    /*move item container*/
    i4_ret = x_wgl_get_coords(pt_ctrl_logic->at_item_ctnr_info[ui4_item_ctnr_idx].h_item_ctnr, WGL_COORDS_PARENT, &t_item_ctnr_rect);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    t_item_ctnr_rect.i4_left   += i4_offset_x;
    t_item_ctnr_rect.i4_right  += i4_offset_x;
    t_item_ctnr_rect.i4_top    += i4_offset_y;
    t_item_ctnr_rect.i4_bottom += i4_offset_y;

    i4_ret = x_wgl_move(pt_ctrl_logic->at_item_ctnr_info[ui4_item_ctnr_idx].h_item_ctnr, &t_item_ctnr_rect, WGL_NO_AUTO_REPAINT);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    ui4_item_idx = ui4_item_ctnr_idx + pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;
    /*item related to item container not in current vp,do nothing*/
    if ((ui4_item_idx > pt_ctrl_logic->t_vp_info.ui4_idx_vp_last))
    {
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_OK;
    }
    /*item need repaint extend region, otherwise will occur ui garbage*/
    pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].b_is_dirty = TRUE;
    UNION_RECT(
        &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg),
        &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg),
        &t_item_ctnr_rect_old);
    /*move hlt item container*/
    if (ui4_item_idx == pt_ctrl_logic->ui4_hlt_idx)
    {
        i4_ret = x_wgl_get_coords(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, WGL_COORDS_PARENT, &t_hlt_item_ctnr_rect);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
        /*union item dirty region with item highlight container rect before moving*/
        UNION_RECT(
            &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg),
            &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg),
            &t_hlt_item_ctnr_rect);

        t_hlt_item_ctnr_rect.i4_left   += i4_offset_x;
        t_hlt_item_ctnr_rect.i4_right  += i4_offset_x;
        t_hlt_item_ctnr_rect.i4_top    += i4_offset_y;
        t_hlt_item_ctnr_rect.i4_bottom += i4_offset_y;

        /*move highlight container to new position*/
        i4_ret = x_wgl_move(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, &t_hlt_item_ctnr_rect, WGL_NO_AUTO_REPAINT);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR:move item container faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
        /*union item dirty region with item highlight container rect after moving*/
        UNION_RECT(
            &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg),
            &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg),
            &t_hlt_item_ctnr_rect);
    }

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}


INT32 x_wgl_mbw_repaint_vp (
    HANDLE_T                    h_mbw)
{
    INT32                       i4_ret;
    GL_RECT_T                   t_paint_region;
    GL_RECT_T                   t_vp_rect;
    GL_RECT_T                   t_hlt_item_rect;
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;
    HANDLE_TYPE_T               e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:repaint vp faild. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);
    /*if vp just changed, maybe need repaint extend region,here just repaint MatrixBox in order to simplify*/
    if(pt_ctrl_logic->b_vp_changed)
    {
        pt_ctrl_logic->b_vp_changed = FALSE;
        i4_ret = x_wgl_repaint(pt_ctrl_logic->h_mbw, NULL ,FALSE);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
    }
    else
    {
        i4_ret = x_wgl_get_coords(pt_ctrl_logic->h_mbw_vp, WGL_COORDS_PARENT, &t_vp_rect);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
        WGL_RECT_COPY(&t_paint_region, &t_vp_rect);

        if (pt_ctrl_logic->ui4_hlt_idx >= pt_ctrl_logic->t_vp_info.ui4_idx_vp_first &&
                pt_ctrl_logic->ui4_hlt_idx <= pt_ctrl_logic->t_vp_info.ui4_idx_vp_last)
        {
            i4_ret = x_wgl_get_coords(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, WGL_COORDS_PARENT, &t_hlt_item_rect);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
            UNION_RECT(&t_paint_region, &t_vp_rect, &t_hlt_item_rect);
        }

        i4_ret = x_wgl_repaint(pt_ctrl_logic->h_mbw, &t_paint_region ,FALSE);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
    }

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_repaint_item (
    HANDLE_T                    h_mbw,
    UINT32                      ui4_idx)
{
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx;
    INT32                       i4_ret;
    GL_RECT_T                   t_paint_region;
    HANDLE_TYPE_T               e_handle_type;
    UINT32                      ui4_item_ctnr_idx;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);
    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:repaint item failed. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    /*check input item if is out of boudary*/
    if ((ui4_idx < pt_ctrl_logic->t_vp_info.ui4_idx_vp_first || ui4_idx > pt_ctrl_logic->t_vp_info.ui4_idx_vp_last))
    {
        DBG_INFO(("{WGL(MBW)} Info:the item is out of vp. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_OK;
    }

    ui4_item_ctnr_idx = ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;

    /*if need not repaint extend dirty region,just repaint item container or highlight item container*/
    if(!(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].b_is_dirty))
    {
        if(pt_ctrl_logic->ui4_hlt_idx != ui4_idx)
        {
            i4_ret = x_wgl_repaint(pt_ctrl_logic->at_item_ctnr_info[ui4_item_ctnr_idx].h_item_ctnr, NULL, FALSE);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
        }
        else
        {
            i4_ret = x_wgl_repaint(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, NULL, FALSE);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return i4_ret;
            }
        }
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_OK;
    }

    /* need repaint extend dirty region*/
    if(pt_ctrl_logic->ui4_hlt_idx != ui4_idx)
    {
        i4_ret = x_wgl_mbw_get_item_ctnr_rect(h_mbw, ui4_item_ctnr_idx, &t_paint_region);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
    }
    else
    {
        i4_ret = x_wgl_get_coords(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, WGL_COORDS_PARENT, &t_paint_region);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
            REL_LOCK(h_mbw, pv_wgl_ctx);
            return i4_ret;
        }
    }

    UNION_RECT(
        &t_paint_region,
        &t_paint_region,
        &pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg);

    i4_ret = x_wgl_repaint(h_mbw, &t_paint_region, FALSE);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return i4_ret;
    }

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}


INT32 x_wgl_mbw_reload_static (
    HANDLE_T                h_mbw)
{
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:repaint item failed. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if (pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }

    MBW_UPDATE_ITEM_CTNR(h_mbw,
        pt_ctrl_logic,
        WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATIC,
        pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
        pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
        pt_ctrl_logic->at_item_ctnr_info);

    if (pt_ctrl_logic->ui4_hlt_idx != WGL_MBW_NULL_INDEX)
    {
        MBW_UPDATE_HLT_ITEM_CTNR(h_mbw, pt_ctrl_logic, WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATIC);
    }

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}

INT32 x_wgl_mbw_reload_data (
    HANDLE_T                h_mbw)
{
    WGL_MBW_CONTROL_LOGIC_T*     pt_ctrl_logic;
    WGL_WIDGET_CONTEXT_HANDLE_T  pv_wgl_ctx;
    INT32                        i4_ret;
    UINT32                       ui4_vp_total_num;
    UINT32                       ui4_num_total;
    UINT32                       ui4_vp_capacity;
    UINT32                       ui4_vp_count;
    UINT32                       ui4_item_count;
    HANDLE_TYPE_T                e_handle_type;

    MBW_CHECK_VALID((h_mbw != NULL_HANDLE), WGLR_INV_ARG);

    /*check mbw handle if is valid*/
    i4_ret = x_handle_get_type(h_mbw, &e_handle_type);
    if (i4_ret != HR_OK || e_handle_type != HT_WGL_WIDGET_FRAME)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:repaint item failed. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    /*get attached data, which contains the control logic object*/
    i4_ret = wgl_get_widget_private_tag(h_mbw, (VOID**)&pt_ctrl_logic);
    if (WGLR_OK != i4_ret || pt_ctrl_logic == NULL)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: file = %s, line = %d, reason = %d\n\r", __FILE__, __LINE__, i4_ret));
        return i4_ret;
    }

    GET_LOCK(h_mbw, &pv_wgl_ctx);

    if (pt_ctrl_logic->b_is_transition == TRUE)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE.at L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE;
    }

    ui4_num_total = pt_ctrl_logic->ui4_item_total_num;
    ui4_vp_capacity = pt_ctrl_logic->ui4_col_num * pt_ctrl_logic->ui4_row_num;
    ui4_vp_total_num = ui4_num_total / ui4_vp_capacity + ((ui4_num_total % ui4_vp_capacity) ? 1 : 0);

    if ((ui4_num_total == 0) ||
        (pt_ctrl_logic->t_vp_info.ui4_idx_vp_first == WGL_MBW_NULL_INDEX) ||
        (pt_ctrl_logic->t_vp_info.ui4_idx_vp_last  == WGL_MBW_NULL_INDEX))
    {
        DBG_INFO(("{WGL(MBW)} no item data will be reloaded L%d\n\n",  __LINE__));
        REL_LOCK(h_mbw, pv_wgl_ctx);
        return WGLR_OK;
    }
    for (ui4_vp_count = 0; ui4_vp_count < ui4_vp_total_num; ui4_vp_count++)
    {
        UINT32 ui4_idx_start;
        UINT32 ui4_idx_end;
        ui4_idx_start = ui4_vp_count * ui4_vp_capacity;
        ui4_idx_end = (((ui4_vp_count + 1) * ui4_vp_capacity - 1) >= ui4_num_total) ?
                      (ui4_num_total - 1) : ((ui4_vp_count + 1) * ui4_vp_capacity - 1);

        x_memset(pt_ctrl_logic->at_item_data, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_DATA_T) * ui4_vp_capacity));

        MBW_GET_ITEM_DATA(h_mbw,
                    pt_ctrl_logic,
                    WGL_MBW_ITEM_DATA_MASK_NVGT_MAP,
                    ui4_idx_start,
                    ui4_idx_end,
                    pt_ctrl_logic->at_item_data);

        for (ui4_item_count = 0; ui4_item_count <= (ui4_idx_end - ui4_idx_start); ui4_item_count++)
        {
            if (!MBW_IS_VALID_ITEM_DATA(pt_ctrl_logic->at_item_data[ui4_item_count]))
            {
                DBG_ERROR(("{WGL(MBW)} ERR: item data in dm is not consistent at L%d\n\n", __LINE__));
                REL_LOCK(h_mbw, pv_wgl_ctx);
                return WGLR_MBW_DM_INCONSISTENT_ITEM_DATA;
            }
        }

    }
    x_memset(pt_ctrl_logic->at_item_data, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_DATA_T) * ui4_vp_capacity));

    MBW_GET_ITEM_DATA(h_mbw,
                    pt_ctrl_logic,
                    WGL_MBW_ITEM_DATA_MASK_NVGT_MAP,
                    pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                    pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                    pt_ctrl_logic->at_item_data);

    REL_LOCK(h_mbw, pv_wgl_ctx);
    return WGLR_OK;
}



/*-----------------------------------------------------------------------------
 * private VIEW methods implementations
 *---------------------------------------------------------------------------*/
 static INT32   _mbw_default_nvgt_key_hdlr(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx,
    WGL_MBW_KEY_HDLR_TYPE_T     e_key_hdlr_type,
    UINT32                      ui4_data)
{
    INT32                       i4_ret;
    UINT32                      ui4_hlt_idx;
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir;
    WGL_MBW_NVGT_INFO_T         t_nvgt_info = {0};
    HANDLE_T                    h_temp;
    VOID*                       pv_temp;
    
    t_nvgt_info.ui1_strct_ver = 1;

    if (e_key_hdlr_type == WGL_MBW_KEY_HDLR_TYPE_NVGT_LEFT)
    {
        e_nvgt_dir = WGL_MBW_NVGT_DIR_LEFT;
    }
    else if (e_key_hdlr_type == WGL_MBW_KEY_HDLR_TYPE_NVGT_RIGHT)
    {
        e_nvgt_dir = WGL_MBW_NVGT_DIR_RIGHT;
    }
    else if (e_key_hdlr_type == WGL_MBW_KEY_HDLR_TYPE_NVGT_UP)
    {
        e_nvgt_dir = WGL_MBW_NVGT_DIR_UP;
    }
    else if (e_key_hdlr_type == WGL_MBW_KEY_HDLR_TYPE_NVGT_DOWN)
    {
        e_nvgt_dir = WGL_MBW_NVGT_DIR_DOWN;
    }
    else if (e_key_hdlr_type == WGL_MBW_KEY_HDLR_TYPE_NVGT_PAGE_UP)
    {
        e_nvgt_dir = WGL_MBW_NVGT_DIR_PG_UP;
    }
    else if (e_key_hdlr_type == WGL_MBW_KEY_HDLR_TYPE_NVGT_PAGE_DOWN)
    {
        e_nvgt_dir = WGL_MBW_NVGT_DIR_PG_DOWN;
    }
    else
    {
        DBG_ERROR(("{WGL(MBW)} ERR: default nvgt key handler procedure faild,unknow key type. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    ui4_hlt_idx = pt_ctrl_logic->ui4_hlt_idx;

    i4_ret = x_wgl_mbw_get_nvgt_info(pt_ctrl_logic->h_mbw, ui4_hlt_idx, e_nvgt_dir, &t_nvgt_info);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: get nvgt info faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    if (!t_nvgt_info.b_vp_changed)
    {
        pt_ctrl_logic->t_evn_hdlr_data.e_evn_type = WGL_MBW_EVN_HDLR_TYPE_NVGT_IN_VP;
        pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_nvgt_in_vp.ui1_strct_ver = 1;

        x_memcpy(
            &pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_nvgt_in_vp.t_nvgt_info,
            &t_nvgt_info,
            sizeof(WGL_MBW_NVGT_INFO_T));
        if (HAS_EVN_HANDLER_MASK(pt_ctrl_logic, WGL_MBW_EVN_HDLR_MASK_NVGT_IN_VP))
        {
            WGL_MBW_EVN_HDLR_INFO_NVGT_IN_VP_T t_evn_hdlr_nvgt_in_vp;
            h_temp = pt_ctrl_logic->h_mbw;
            pv_temp = pt_ctrl_logic->t_evn_hdlr_info.pv_tag_evn_hdlr;
            t_evn_hdlr_nvgt_in_vp = pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_nvgt_in_vp;
            
            REL_LOCK(pt_ctrl_logic->h_mbw, pv_wgl_ctx);
            
            pt_ctrl_logic->t_evn_hdlr_info.pf_event_hdlr(
                            h_temp,
                            pv_temp,
                            WGL_MBW_EVN_HDLR_TYPE_NVGT_IN_VP,
                            (UINT32)(&t_evn_hdlr_nvgt_in_vp));

            GET_LOCK(pt_ctrl_logic->h_mbw, &pv_wgl_ctx);
        }
        else
        {
            i4_ret = x_wgl_mbw_default_evn_hdlr(
                            pt_ctrl_logic->h_mbw,
                            WGL_MBW_EVN_HDLR_TYPE_NVGT_IN_VP,
                            (UINT32)(&pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_nvgt_in_vp));
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: default event handler processdure faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
    }
    else
    {
        pt_ctrl_logic->t_evn_hdlr_data.e_evn_type = WGL_MBW_EVN_HDLR_TYPE_NVGT_WITH_VP_CHG;
        pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_nvgt_with_vp_chg.ui1_strct_ver = 1;

        x_memcpy(
            &pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_nvgt_with_vp_chg.t_nvgt_info,
            &t_nvgt_info,
            sizeof(WGL_MBW_NVGT_INFO_T));

        if (HAS_EVN_HANDLER_MASK(pt_ctrl_logic, WGL_MBW_EVN_HDLR_MASK_NVGT_WITH_VP_CHG))
        {
            WGL_MBW_EVN_HDLR_INFO_NVGT_WITH_VP_CHG_T t_evn_hdlr_nvgt_with_vp_chg;
            h_temp = pt_ctrl_logic->h_mbw;
            pv_temp = pt_ctrl_logic->t_evn_hdlr_info.pv_tag_evn_hdlr;
            t_evn_hdlr_nvgt_with_vp_chg = pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_nvgt_with_vp_chg;

            REL_LOCK(pt_ctrl_logic->h_mbw, pv_wgl_ctx);
            
            pt_ctrl_logic->t_evn_hdlr_info.pf_event_hdlr(
                            h_temp,
                            pv_temp,
                            WGL_MBW_EVN_HDLR_TYPE_NVGT_WITH_VP_CHG,
                            (UINT32)(&t_evn_hdlr_nvgt_with_vp_chg));

            GET_LOCK(pt_ctrl_logic->h_mbw, &pv_wgl_ctx);
        }
        else
        {
            i4_ret = x_wgl_mbw_default_evn_hdlr(
                            pt_ctrl_logic->h_mbw,
                            WGL_MBW_EVN_HDLR_TYPE_NVGT_WITH_VP_CHG,
                            (UINT32)(&pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_nvgt_with_vp_chg));
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: default event handler processdure faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
    }
    return WGLR_OK;
}
static INT32  _mbw_on_item_select(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    WGL_WIDGET_CONTEXT_HANDLE_T pv_wgl_ctx,
    UINT32                      ui4_item_idx)
{
    INT32                       i4_ret;
    UINT32                      ui4_item_state;
    HANDLE_T                    h_temp;
    VOID*                       pv_temp;

    i4_ret = x_wgl_mbw_get_item_state(pt_ctrl_logic->h_mbw, ui4_item_idx, &ui4_item_state);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: get item state faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    if (!(ui4_item_state & WGL_MBW_ITEM_STATE_GRAYED))
    {
        /*check whether the item is selected*/
        if ((ui4_item_state & WGL_MBW_ITEM_STATE_SELECTED))
        {
            pt_ctrl_logic->t_evn_hdlr_data.e_evn_type = WGL_MBW_EVN_HDLR_TYPE_DESEL;
            pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_deselect.ui1_strct_ver = 1;
            pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_deselect.ui4_idx_desel = ui4_item_idx;
            pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_deselect.ui4_state_item_orig = ui4_item_state;
            if (HAS_EVN_HANDLER_MASK(pt_ctrl_logic, WGL_MBW_EVN_HDLR_MASK_DESEL))
            {
                WGL_MBW_EVN_HDLR_INFO_DESEL_T t_evn_hdlr_deselect;
                h_temp = pt_ctrl_logic->h_mbw;
                pv_temp = pt_ctrl_logic->t_evn_hdlr_info.pv_tag_evn_hdlr;
                t_evn_hdlr_deselect = pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_deselect;
                
                REL_LOCK(pt_ctrl_logic->h_mbw, pv_wgl_ctx);
                
                pt_ctrl_logic->t_evn_hdlr_info.pf_event_hdlr(
                            h_temp,
                            pv_temp,
                            WGL_MBW_EVN_HDLR_TYPE_DESEL,
                            (UINT32)(&t_evn_hdlr_deselect));
                
                GET_LOCK(pt_ctrl_logic->h_mbw, &pv_wgl_ctx);
            }
            else
            {
                i4_ret = x_wgl_mbw_default_evn_hdlr(
                            pt_ctrl_logic->h_mbw,
                            WGL_MBW_EVN_HDLR_TYPE_DESEL,
                            (UINT32)(&pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_deselect));

                if (WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: default event handler processdure faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                    return i4_ret;
                }
            }
        }
        else
        {
            pt_ctrl_logic->t_evn_hdlr_data.e_evn_type = WGL_MBW_EVN_HDLR_TYPE_SELECT;
            pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_select.ui1_strct_ver = 1;
            pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_select.ui4_idx_sel = ui4_item_idx;
            pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_select.ui4_state_item_orig = ui4_item_state;
            if (HAS_EVN_HANDLER_MASK(pt_ctrl_logic, WGL_MBW_EVN_HDLR_MASK_SELECT))
            {
                WGL_MBW_EVN_HDLR_INFO_SELECT_T t_evn_hdlr_select;
                h_temp = pt_ctrl_logic->h_mbw;
                pv_temp = pt_ctrl_logic->t_evn_hdlr_info.pv_tag_evn_hdlr;
                t_evn_hdlr_select = pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_select;
                
                REL_LOCK(pt_ctrl_logic->h_mbw, pv_wgl_ctx);
                
                pt_ctrl_logic->t_evn_hdlr_info.pf_event_hdlr(
                            h_temp,
                            pv_temp,
                            WGL_MBW_EVN_HDLR_TYPE_SELECT,
                            (UINT32)(&t_evn_hdlr_select));

                GET_LOCK(pt_ctrl_logic->h_mbw, &pv_wgl_ctx);
            }
            else
            {
                i4_ret = x_wgl_mbw_default_evn_hdlr(
                            pt_ctrl_logic->h_mbw,
                            WGL_MBW_EVN_HDLR_TYPE_SELECT,
                            (UINT32)(&pt_ctrl_logic->t_evn_hdlr_data.u_evn_hdlr_data.t_evn_hdlr_select));

                if (WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: default event handler processdure faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                    return i4_ret;
                }
            }
        }
    }
    return WGLR_OK;
}

static INT32   _mbw_get_normal_nvgt_info (
    WGL_MBW_CONTROL_LOGIC_T* pt_ctrl_logic,
    UINT32                   ui4_idx_from,
    WGL_MBW_NVGT_DIR_T       e_nvgt_dir,
    WGL_MBW_NVGT_INFO_T*     pt_nvgt_info)
{
    INT32   i4_ret;
    INT32   i4_idx_to;
    BOOL    b_use_item_kmap = FALSE;
    BOOL    b_wrap_over = FALSE;
    BOOL    b_vp_changed = FALSE;
    BOOL    b_navigable = FALSE;
    UINT32  ui4_idx_vp_first;
    UINT32  ui4_idx_vp_last;
    UINT32  ui4_total_num;
    UINT32  ui4_idx_orig;
    UINT32  ui4_item_state;

    ui4_idx_orig = ui4_idx_from;
    ui4_idx_vp_first = pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;
    ui4_idx_vp_last = pt_ctrl_logic->t_vp_info.ui4_idx_vp_last;
    ui4_total_num = pt_ctrl_logic->ui4_item_total_num;
    while (1)
    {
        /*gain the new item index*/
        i4_ret = _mbw_gain_new_item_by_map(
                pt_ctrl_logic,
                (INT32)ui4_idx_from,
                e_nvgt_dir,
                &i4_idx_to,
                &b_use_item_kmap);
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR: gain new item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        /*if not nvgt by item's own map, calculate the nvgt step*/
        if (b_use_item_kmap == FALSE)
        {
            INT32 i4_nav_step;
            i4_ret = _mbw_calculate_nvgt_step(pt_ctrl_logic, (INT32)ui4_idx_from, e_nvgt_dir, &i4_nav_step);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: calculate nvgt step faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
            else
            {
                i4_idx_to = (INT32)ui4_idx_from + i4_nav_step;
            }
        }

        /*check whether the new item's index isn't out of the boundary of the datamodel*/
        if (i4_idx_to >= (INT32)ui4_total_num || i4_idx_to < 0)
        {
            /*check whether navigated by item map*/
            if (b_use_item_kmap)
            {
                /*not navigable, and vp should not change*/
                b_navigable = FALSE;
                break;
            }
            else
            {
                /*check arrange mode*/
                if (!HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_HORIZ_ARRANGE))
                {
                    /*check wrap_over style*/
                    if (!HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_WRAP_OVER_VERTI))
                    {
                         /*not navigable, and vp should not change*/
                         b_navigable = FALSE;
                         break;
                    }
                    else
                    {
                        /*maybe need wrap_over, recalculate the new item idx*/
                        i4_ret = _mbw_on_wrap_over(
                            pt_ctrl_logic,
                            &i4_idx_to,
                            &b_wrap_over,
                            e_nvgt_dir);
                    }
                }
                else
                {
                    /*check wrap_over style*/
                    if (!HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_WRAP_OVER_HORIZ))
                    {
                         /*not navigable, and vp should not change*/
                         b_navigable = FALSE;
                         break;
                    }
                    else
                    {
                        /*maybe need wrap_over, recalculate the new item idx*/
                        i4_ret = _mbw_on_wrap_over(
                            pt_ctrl_logic,
                            &i4_idx_to,
                            &b_wrap_over,
                            e_nvgt_dir);
                    }
                }
            }
        }

        if (i4_idx_to < (INT32)ui4_total_num && i4_idx_to >= 0)
        {
            if (i4_idx_to == (INT32)ui4_idx_orig)
            {
                /*not navigable, but vp maybe changed*/
                b_navigable = FALSE;
                break;
            }
            /*check item's position*/
            else
            {
                /*out of vp, get item state from data model*/
                if (i4_idx_to > (INT32)ui4_idx_vp_last || i4_idx_to < (INT32)ui4_idx_vp_first)
                {
                    b_vp_changed = TRUE;
                    if (b_use_item_kmap && i4_idx_to > (INT32)ui4_idx_vp_last &&
                        (e_nvgt_dir == WGL_MBW_NVGT_DIR_UP || e_nvgt_dir == WGL_MBW_NVGT_DIR_LEFT))
                    {
                        b_wrap_over = TRUE;
                    }
                    if (b_use_item_kmap && i4_idx_to < (INT32)ui4_idx_vp_first &&
                        (e_nvgt_dir == WGL_MBW_NVGT_DIR_DOWN || e_nvgt_dir == WGL_MBW_NVGT_DIR_RIGHT))
                    {
                        b_wrap_over = TRUE;
                    }
                    i4_ret = x_wgl_mbw_get_item_state(pt_ctrl_logic->h_mbw, (UINT32)i4_idx_to, &ui4_item_state);
                    if (WGLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(MBW)} ERR: get item state faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                        return i4_ret;
                    }
                }
                /*in vp, get item state from catched buffer*/
                else
                {
                    b_vp_changed = FALSE;
                    ui4_item_state = pt_ctrl_logic->aui4_item_state[i4_idx_to - ui4_idx_vp_first];
                }

                /*check whether the item is enabled*/
                if (ui4_item_state & WGL_MBW_ITEM_STATE_DISABLED)
                {
                    /*check whether navigated by item map*/
                    if (b_use_item_kmap)
                    {
                        /*not navigable, and vp should not change, maybe need reset b_vp_changed*/
                        b_navigable = FALSE;
                        break;
                    }
                    else
                    {
                        if ((INT32)ui4_idx_from == i4_idx_to)
                        {
                            b_navigable = FALSE;
                            break;
                        }
                        ui4_idx_from = (UINT32)i4_idx_to;
                        continue;
                    }
                }
                else
                {
                    b_navigable = TRUE;
                    break;
                }
            }
        }
    }

    if (!b_navigable)
    {
        b_wrap_over = FALSE;
        b_vp_changed = FALSE;
        i4_idx_to = (INT32)ui4_idx_orig;
    }
    /*setup nvgt info*/
    i4_ret = _mbw_setup_nvgt_info(
                pt_ctrl_logic,
                pt_nvgt_info,
                &b_navigable,
                &b_vp_changed,
                &b_wrap_over,
                ui4_idx_orig,
                (UINT32)i4_idx_to,
                e_nvgt_dir);

    return i4_ret;
}


static INT32   _mbw_get_page_up_down_nvgt_info (
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    UINT32                      ui4_idx_from,
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir,
    WGL_MBW_NVGT_INFO_T*        pt_nvgt_info)
{
    INT32   i4_ret;
    INT32   i4_idx_to;
    BOOL    b_use_item_kmap = FALSE;
    BOOL    b_wrap_over = FALSE;
    BOOL    b_vp_changed = FALSE;
    BOOL    b_navigable = FALSE;
    BOOL    b_valid = FALSE;
    BOOL    b_first_vp_tried = FALSE;
    BOOL    b_last_vp_tried = FALSE;
    BOOL    b_in_orig_vp = TRUE;
    WGL_MBW_VP_INFO_T t_vp_info;
    INT32   i4_idx_vp_first;
    INT32   i4_idx_vp_last;
    UINT32  ui4_col_num;
    UINT32  ui4_row_num;
    UINT32  ui4_vp_capacity;
    UINT32  ui4_total_num;
    UINT32  ui4_idx_orig;
    UINT32  ui4_item_state;

    ui4_idx_orig = ui4_idx_from;
    i4_idx_vp_first = (INT32)pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;
    i4_idx_vp_last = (INT32)pt_ctrl_logic->t_vp_info.ui4_idx_vp_last;
    ui4_col_num = pt_ctrl_logic->ui4_col_num;
    ui4_row_num = pt_ctrl_logic->ui4_row_num;
    ui4_total_num = pt_ctrl_logic->ui4_item_total_num;
    ui4_vp_capacity = ui4_col_num * ui4_row_num;
    t_vp_info.ui4_idx_vp_first = (UINT32)i4_idx_vp_first;
    t_vp_info.ui4_idx_vp_last = (UINT32)i4_idx_vp_last;

    /*gain the new item index*/
    i4_ret = _mbw_gain_new_item_by_map(
                    pt_ctrl_logic,
                    (INT32)ui4_idx_from,
                    e_nvgt_dir,
                    &i4_idx_to,
                    &b_use_item_kmap);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: gain new item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    /*if nvgt by item's own map, calculate the nvgt step*/
    if (b_use_item_kmap)
    {
        if (i4_idx_to >= (INT32)ui4_total_num || i4_idx_to < 0)
        {
            b_navigable = FALSE;
            b_wrap_over = FALSE;
            b_vp_changed = FALSE;
            i4_idx_to = (INT32)ui4_idx_from;
        }
        else
        {
            if (i4_idx_to > i4_idx_vp_last || i4_idx_to < i4_idx_vp_first)
            {
                b_vp_changed = TRUE;
                if (i4_idx_to < i4_idx_vp_first && e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_DOWN)
                {
                    b_wrap_over = TRUE;
                }
                if (i4_idx_to > i4_idx_vp_last && e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_UP)
                {
                    b_wrap_over = TRUE;
                }
                i4_ret = x_wgl_mbw_get_item_state(pt_ctrl_logic->h_mbw, (UINT32)i4_idx_to, &ui4_item_state);
                if (WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: get item state faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                    return i4_ret;
                }

            }
            /*in vp, get item state from catched buffer*/
            else
            {
                b_vp_changed = FALSE;
                b_wrap_over = FALSE;
                ui4_item_state = pt_ctrl_logic->aui4_item_state[i4_idx_to - i4_idx_vp_first];
            }
            /*check whether the item is enabled*/
            if ((ui4_item_state & WGL_MBW_ITEM_STATE_DISABLED))
            {
                b_navigable = FALSE;
                b_wrap_over = FALSE;
                i4_idx_to = (INT32)ui4_idx_from;
            }
            else
            {
                b_navigable = TRUE;
            }
        }
    }

    /*if not nvgt by item's own map, calculate the nvgt step*/
    if (!b_use_item_kmap)
    {
        while(1)
        {
            i4_ret = _mbw_gain_new_item_for_page_up_down(
                                    pt_ctrl_logic,
                                    (INT32)ui4_idx_from,
                                    &i4_idx_to,
                                    e_nvgt_dir,
                                    t_vp_info,
                                    b_in_orig_vp,
                                    &b_valid);

            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: gain new item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
            if (b_valid == FALSE)
            {
                b_in_orig_vp = FALSE;
                /*actually the b_vp_changed here may incidate incorrectly
                in some special situation, for example, where there is a
                3x3 matrix,the current hlt item idx is 1, and the item 4, 7
                are both disabled, when page_down is pressed, in the first
                loop, no valid item would be found, so it will change the vp,
                and b_vp_changed will be set TRUE; in the second loop, item1
                would be considered as the vaild item, actually the vp do not
                need a change, so b_vp_changed need a reset at last*/
                b_vp_changed = TRUE;

                if (e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_DOWN)
                {
                    i4_idx_vp_first = i4_idx_vp_last + 1;
                    i4_idx_vp_last += ui4_vp_capacity;

                    if (!b_last_vp_tried)
                    {
                        if (i4_idx_vp_first > (INT32)(ui4_total_num -1))
                        {
                            i4_idx_vp_first = 0;
                            i4_idx_vp_last = (INT32)ui4_vp_capacity - 1;
                            if (i4_idx_vp_last > (INT32)ui4_total_num -1)
                            {
                                i4_idx_vp_last = (INT32)ui4_total_num -1;
                            }
                            b_last_vp_tried = TRUE;
                            if ((HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_HORIZ_ARRANGE) &&
                                HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_WRAP_OVER_HORIZ)) ||
                                (!HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_HORIZ_ARRANGE) &&
                                HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_WRAP_OVER_VERTI)))
                            {
                                b_wrap_over = TRUE;
                            }
                            else
                            {
                                b_navigable = FALSE;
                                b_wrap_over = FALSE;
                                b_vp_changed = FALSE;
                                i4_idx_to = (INT32)ui4_idx_from;
                                break;
                            }

                        }
                        else if(i4_idx_vp_last > (INT32)(ui4_total_num -1))
                        {
                            i4_idx_vp_last = (INT32)ui4_total_num -1;
                            b_last_vp_tried = FALSE;
                        }
                        else
                        {
                            b_last_vp_tried = FALSE;
                        }
                    }
                    t_vp_info.ui4_idx_vp_first = (UINT32)i4_idx_vp_first;
                    t_vp_info.ui4_idx_vp_last = (UINT32)i4_idx_vp_last;
                }
                else
                {
                    i4_idx_vp_last = i4_idx_vp_first - 1;
                    i4_idx_vp_first -= ui4_vp_capacity;
                    if (!b_first_vp_tried)
                    {
                        if (i4_idx_vp_last < 0)
                        {
                            UINT32 ui4_page_offset;
                            i4_idx_vp_last = (INT32)ui4_total_num -1;
                            /*get the page which contains the new item*/
                            ui4_page_offset = (UINT32)i4_idx_vp_last / (ui4_col_num * ui4_row_num);
                            i4_idx_vp_first = (INT32)(ui4_page_offset * (ui4_col_num * ui4_row_num));
                            b_first_vp_tried = TRUE;
                            if ((HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_HORIZ_ARRANGE) &&
                                HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_WRAP_OVER_HORIZ)) ||
                                (!HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_HORIZ_ARRANGE) &&
                                HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_WRAP_OVER_VERTI)))
                            {
                                b_wrap_over = TRUE;
                            }
                            else
                            {
                                b_navigable = FALSE;
                                b_wrap_over = FALSE;
                                b_vp_changed = FALSE;
                                i4_idx_to = (INT32)ui4_idx_from;
                                break;
                            }
                        }
                        else if (i4_idx_vp_first < 0)
                        {
                            i4_idx_vp_first = 0;
                            b_first_vp_tried = FALSE;
                        }
                        else
                        {
                            b_first_vp_tried = FALSE;
                        }
                    }
                    t_vp_info.ui4_idx_vp_first = (UINT32)i4_idx_vp_first;
                    t_vp_info.ui4_idx_vp_last = (UINT32)i4_idx_vp_last;
                }
            }
            else
            {
                if (i4_idx_to == (INT32)ui4_idx_orig)
                {
                    b_navigable = FALSE;
                }
                else
                {
                    b_navigable = TRUE;
                }
                break;
            }
        }
    }
    /*setup nvgt info*/
    i4_ret = _mbw_setup_nvgt_info(
                pt_ctrl_logic,
                pt_nvgt_info,
                &b_navigable,
                &b_vp_changed,
                &b_wrap_over,
                ui4_idx_orig,
                (UINT32)i4_idx_to,
                e_nvgt_dir);

    return i4_ret;

}

static INT32   _mbw_get_page_up_down_custom_nvgt_info (
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    UINT32                      ui4_idx_from,
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir,
    WGL_MBW_NVGT_INFO_T*        pt_nvgt_info)
{
    INT32   i4_ret;
    INT32   i4_idx_to;
    BOOL    b_use_item_kmap = FALSE;
    BOOL    b_wrap_over = FALSE;
    BOOL    b_vp_changed = FALSE;
    BOOL    b_navigable = FALSE;
    INT32   i4_idx_vp_first;
    INT32   i4_idx_vp_last;
    UINT32  ui4_col_num;
    UINT32  ui4_row_num;
    UINT32  ui4_vp_capacity;
    UINT32  ui4_total_num;
    UINT32  ui4_idx_orig;
    UINT32  ui4_item_state;

    ui4_idx_orig = ui4_idx_from;
    i4_idx_vp_first = (INT32)pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;
    i4_idx_vp_last = (INT32)pt_ctrl_logic->t_vp_info.ui4_idx_vp_last;
    ui4_col_num = pt_ctrl_logic->ui4_col_num;
    ui4_row_num = pt_ctrl_logic->ui4_row_num;
    ui4_total_num = pt_ctrl_logic->ui4_item_total_num;
    ui4_vp_capacity = ui4_col_num * ui4_row_num;

    /*gain the new item index*/
    i4_ret = _mbw_gain_new_item_by_map(
                    pt_ctrl_logic,
                    (INT32)ui4_idx_from,
                    e_nvgt_dir,
                    &i4_idx_to,
                    &b_use_item_kmap);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: gain new item faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    /*if nvgt by item's own map, calculate the nvgt step*/
    if (b_use_item_kmap)
    {
        if (i4_idx_to >= (INT32)ui4_total_num || i4_idx_to < 0)
        {
            b_navigable = FALSE;
            b_wrap_over = FALSE;
            b_vp_changed = FALSE;
            i4_idx_to = (INT32)ui4_idx_from;
        }
        else
        {
            if (i4_idx_to > i4_idx_vp_last || i4_idx_to < i4_idx_vp_first)
            {
                b_vp_changed = TRUE;
                if (i4_idx_to < i4_idx_vp_first && e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_DOWN)
                {
                    b_wrap_over = TRUE;
                }
                if (i4_idx_to > i4_idx_vp_last && e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_UP)
                {
                    b_wrap_over = TRUE;
                }
                i4_ret = x_wgl_mbw_get_item_state(pt_ctrl_logic->h_mbw, (UINT32)i4_idx_to, &ui4_item_state);
                if (WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(MBW)} ERR: get item state faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                    return i4_ret;
                }

            }
            /*in vp, get item state from catched buffer*/
            else
            {
                b_vp_changed = FALSE;
                b_wrap_over = FALSE;
                ui4_item_state = pt_ctrl_logic->aui4_item_state[i4_idx_to - i4_idx_vp_first];
            }
            /*check whether the item is enabled*/
            if ((ui4_item_state & WGL_MBW_ITEM_STATE_DISABLED))
            {
                b_navigable = FALSE;
                b_wrap_over = FALSE;
                i4_idx_to = (INT32)ui4_idx_from;
            }
            else
            {
                b_navigable = TRUE;
            }
        }
    }

    /*if not nvgt by item's own map, calculate the nvgt step*/
    if (!b_use_item_kmap)
    {
        if (e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_DOWN)
        {
            b_wrap_over = TRUE;
            b_navigable = TRUE;
            if(i4_idx_vp_last == (INT32)ui4_total_num -1)/*already be the last page*/
            {
                if(i4_idx_to == i4_idx_vp_last)
                {
                    b_navigable = FALSE;
                }
                i4_idx_to = i4_idx_vp_last;                
                b_vp_changed = FALSE;
            }
            else
            {
                i4_idx_vp_first = i4_idx_vp_last + 1;
                i4_idx_vp_last += ui4_vp_capacity;
                if(i4_idx_vp_last > (INT32)(ui4_total_num -1))
                {
                    i4_idx_vp_last = (INT32)ui4_total_num -1;
                }
                i4_idx_to = i4_idx_vp_first;
                b_vp_changed = TRUE;
            }
        }
        else
        {
            b_wrap_over = TRUE;
            b_navigable = TRUE;
            if(i4_idx_vp_first==0)/*already be the first page*/
            {
                if(i4_idx_to == i4_idx_vp_first)
                {
                    b_navigable = FALSE;
                }
                i4_idx_to = i4_idx_vp_first;                
                b_vp_changed = FALSE;
            }
            else
            {
                i4_idx_vp_last = i4_idx_vp_first-1;
                i4_idx_vp_first -= (INT32)ui4_vp_capacity;
                i4_idx_to = i4_idx_vp_first;
                b_vp_changed = TRUE;
            }

        }
    }
    /*setup nvgt info*/
    i4_ret = _mbw_setup_nvgt_info(
                pt_ctrl_logic,
                pt_nvgt_info,
                &b_navigable,
                &b_vp_changed,
                &b_wrap_over,
                ui4_idx_orig,
                (UINT32)i4_idx_to,
                e_nvgt_dir);

    return i4_ret;

}

static INT32    _mbw_gain_new_item_by_map(
    WGL_MBW_CONTROL_LOGIC_T*  pt_ctrl_logic,
    INT32                     i4_idx_from,
    WGL_MBW_NVGT_DIR_T        e_nvgt_dir,
    INT32*                    pi4_idx_to,
    BOOL*                     pb_use_item_kmap)
{
    UINT32                  ui4_idx_vp_first;
    UINT32                  ui4_idx_vp_last;
    UINT32                  ui4_item_nvgt_dir_mask;
    UINT32                  ui4_item_data_mask;
    WGL_MBW_ITEM_DATA_T     at_mbw_item_data[1];

    if (i4_idx_from >= (INT32)pt_ctrl_logic->ui4_item_total_num ||i4_idx_from < 0)
    {
        DBG_ERROR(("{WGL(MBW)} ERR: the idx of the item is invalid. at L%d\n\n",  __LINE__));
        return WGLR_INV_ARG;
    }
    ui4_idx_vp_first = pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;
    ui4_idx_vp_last = pt_ctrl_logic->t_vp_info.ui4_idx_vp_last;

    /*check whether the old item is in vp*/
    if (i4_idx_from >= (INT32)ui4_idx_vp_first && i4_idx_from <= (INT32)ui4_idx_vp_last)
    {
        x_memcpy(at_mbw_item_data, &pt_ctrl_logic->at_item_data[i4_idx_from - ui4_idx_vp_first], sizeof(WGL_MBW_ITEM_DATA_T));
        ui4_item_data_mask = at_mbw_item_data[0].ui4_data_mask;
        ui4_item_nvgt_dir_mask = at_mbw_item_data[0].t_item_nvgt_map.ui4_item_nvgt_dir_mask;
    }
    else
    {
        MBW_GET_ITEM_DATA(
                        pt_ctrl_logic->h_mbw,
                        pt_ctrl_logic,
                        WGL_MBW_ITEM_DATA_MASK_NVGT_MAP,
                        (UINT32)i4_idx_from,
                        (UINT32)i4_idx_from,
                        at_mbw_item_data);

        ui4_item_data_mask = at_mbw_item_data[0].ui4_data_mask;
        ui4_item_nvgt_dir_mask = at_mbw_item_data[0].t_item_nvgt_map.ui4_item_nvgt_dir_mask;
    }
    /*check whether the item has its own nvgt map*/
    if (ui4_item_data_mask & WGL_MBW_ITEM_DATA_MASK_NVGT_MAP)
    {
        /*check whether the nvgt direction has been masked in the map*/
         if (ui4_item_nvgt_dir_mask & WGL_MBW_MAKE_NVGT_DIR_MASK(e_nvgt_dir))
         {
            *pb_use_item_kmap = TRUE;
            if (e_nvgt_dir == WGL_MBW_NVGT_DIR_LEFT)
            {
                *pi4_idx_to = (INT32)(at_mbw_item_data[0].t_item_nvgt_map.ui4_item_idx_nvgt_left);
            }
            else if (e_nvgt_dir == WGL_MBW_NVGT_DIR_RIGHT)
            {
                *pi4_idx_to = (INT32)(at_mbw_item_data[0].t_item_nvgt_map.ui4_item_idx_nvgt_right);
            }
            else if (e_nvgt_dir == WGL_MBW_NVGT_DIR_UP)
            {
                *pi4_idx_to = (INT32)(at_mbw_item_data[0].t_item_nvgt_map.ui4_item_idx_nvgt_up);
            }
            else if (e_nvgt_dir == WGL_MBW_NVGT_DIR_DOWN)
            {
                *pi4_idx_to = (INT32)(at_mbw_item_data[0].t_item_nvgt_map.ui4_item_idx_nvgt_down);
            }
            else if (e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_UP)
            {
                *pi4_idx_to = (INT32)(at_mbw_item_data[0].t_item_nvgt_map.ui4_item_idx_nvgt_page_up);
            }
            else if (e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_DOWN)
            {
                *pi4_idx_to = (INT32)(at_mbw_item_data[0].t_item_nvgt_map.ui4_item_idx_nvgt_page_down);
            }
            else
            {
                DBG_ERROR(("{WGL(MBW)} ERR: the nvgt direction is invalid. at L%d\n\n",  __LINE__));
                return WGLR_INV_ARG;
            }
        }
        else
        {
            *pb_use_item_kmap = FALSE;
        }
    }
    else
    {
        *pb_use_item_kmap = FALSE;
    }

    return WGLR_OK;
}


static INT32   _mbw_calculate_nvgt_step(
    WGL_MBW_CONTROL_LOGIC_T*  pt_ctrl_logic,
    INT32                     i4_idx_from,
    WGL_MBW_NVGT_DIR_T        e_nvgt_dir,
    INT32*                    pi4_nav_step)
{
    UINT32 ui4_col_num;
    UINT32 ui4_row_num;
    BOOL b_nav_dir_out_bound;
    UINT32 ui4_vp_capacity;
    UINT32 ui4_vp_num;

    ui4_col_num = pt_ctrl_logic->ui4_col_num;
    ui4_row_num = pt_ctrl_logic->ui4_row_num;
    ui4_vp_capacity = ui4_col_num * ui4_row_num;
    b_nav_dir_out_bound = FALSE;
    ui4_vp_num = pt_ctrl_logic->ui4_item_total_num/ui4_vp_capacity+(pt_ctrl_logic->ui4_item_total_num%ui4_vp_capacity>0?1:0);
    /*vertical arrange mode*/
    if (!HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_HORIZ_ARRANGE))
    {
        if (e_nvgt_dir == WGL_MBW_NVGT_DIR_LEFT)
        {
            b_nav_dir_out_bound = (i4_idx_from % ui4_col_num == 0);
           
            /*horizontal move in same row*/
            if (b_nav_dir_out_bound)
            {
                if (HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_WRAP_OVER_HORIZ))
                {
                    if (pt_ctrl_logic->t_nvgt_custom.t_nvgt_horiz_custom == WGL_MBW_NVGT_HORIZ_LOOP_IN_ONE_PAGE)
                    {
                        if ((UINT32)i4_idx_from+ui4_col_num > pt_ctrl_logic->ui4_item_total_num)
                        {
                            ui4_col_num = pt_ctrl_logic->ui4_item_total_num-(UINT32)i4_idx_from;
                        }
                        *pi4_nav_step = (INT32)ui4_col_num - 1;
                    }
                    else if (pt_ctrl_logic->t_nvgt_custom.t_nvgt_horiz_custom == WGL_MBW_NVGT_HORIZ_LOOP_CROSS_PAGES)
                    {
                        if(pt_ctrl_logic->t_vp_info.ui4_idx_vp_first== 0)/*already be in the first page and need rollback to the last page*/
                        {
                            UINT32 ui4_col_num_tmp = ui4_col_num;
                            if(i4_idx_from+(INT32)((ui4_vp_num-1)*ui4_vp_capacity+(ui4_col_num_tmp-1))>=(INT32)pt_ctrl_logic->ui4_item_total_num - 1)
                            {
                                *pi4_nav_step = (INT32)pt_ctrl_logic->ui4_item_total_num - 1 - i4_idx_from;
                            }
                            else
                            {
                                *pi4_nav_step = (INT32)((ui4_vp_num-1)*ui4_vp_capacity+(ui4_col_num_tmp-1));
                            }
                        }
                        else
                        {
                            *pi4_nav_step = -(INT32)(ui4_col_num *(ui4_row_num-1)+1);
                        }
                    }
                    else
                    {
                        *pi4_nav_step = -1;
                    }
                }
                else
                {
                    *pi4_nav_step = 0;
                }
            }
            else
            {
                *pi4_nav_step = -1;
            }
        }
        else if (e_nvgt_dir == WGL_MBW_NVGT_DIR_RIGHT)
        {
            if (i4_idx_from < (INT32)pt_ctrl_logic->ui4_item_total_num - 1)
            {
                b_nav_dir_out_bound = ((UINT32)i4_idx_from % ui4_col_num == ui4_col_num - (UINT32)1);
            }
            else
            {
                b_nav_dir_out_bound = TRUE;
            }

            /*horizontal move in same row*/
            if (b_nav_dir_out_bound)
            {
                if (HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_WRAP_OVER_HORIZ))
                {
                    if (pt_ctrl_logic->t_nvgt_custom.t_nvgt_horiz_custom == WGL_MBW_NVGT_HORIZ_LOOP_IN_ONE_PAGE)
                    {
                        *pi4_nav_step = - (INT32)(i4_idx_from % ui4_col_num);
                    }
                    else if (pt_ctrl_logic->t_nvgt_custom.t_nvgt_horiz_custom == WGL_MBW_NVGT_HORIZ_LOOP_CROSS_PAGES)
                    {
                        if(pt_ctrl_logic->t_vp_info.ui4_idx_vp_last == pt_ctrl_logic->ui4_item_total_num-1)/*already be in the last page and need rollback to the first page*/
                        {
                            if(i4_idx_from == pt_ctrl_logic->ui4_item_total_num-1)/*the last item on the last row*/
                            {
                                 if(i4_idx_from-ui4_col_num <= pt_ctrl_logic->t_vp_info.ui4_idx_vp_first)/*the last viewport ony has one row*/
                                 {
                                    *pi4_nav_step = -i4_idx_from;
                                 }
                                 else
                                 {
                                    *pi4_nav_step = -(INT32)((i4_idx_from%ui4_col_num)+1);/*go to the last item on the previous row*/
                                 }
                            }
                            else
                            {
                                *pi4_nav_step = -(INT32)(pt_ctrl_logic->t_vp_info.ui4_idx_vp_first+(ui4_col_num-1));
                            }
                        }
                        else
                        {
                            while(i4_idx_from+(INT32)(ui4_col_num *(ui4_row_num-1)+1)>(INT32)pt_ctrl_logic->ui4_item_total_num - 1)
                            {
                                ui4_row_num--;
                            }
                            *pi4_nav_step = (INT32)(ui4_col_num *(ui4_row_num-1)+1);
                        }
                    }
                    else
                    {
                        *pi4_nav_step = 1;
                    }
                }
                else
                {
                    *pi4_nav_step = 0;
                }
            }
            else
            {
                *pi4_nav_step = 1;
            }
        }
        else if (e_nvgt_dir == WGL_MBW_NVGT_DIR_UP)
        {
            b_nav_dir_out_bound = ((((UINT32)i4_idx_from % ui4_col_num) +pt_ctrl_logic->t_vp_info.ui4_idx_vp_first)== (UINT32)i4_idx_from);
            while ((UINT32)i4_idx_from+ui4_col_num*(ui4_row_num-1) > pt_ctrl_logic->ui4_item_total_num-1)
            {
                ui4_row_num -= 1;
            }

            /*vertical move in same column*/
            if (b_nav_dir_out_bound)
            {
                if (pt_ctrl_logic->t_nvgt_custom.t_nvgt_verti_custom== WGL_MBW_NVGT_VERTI_LOOP_IN_ONE_PAGE)
                {
                    *pi4_nav_step = (INT32)ui4_col_num*(ui4_row_num-1);
                }
                else
                {
                    *pi4_nav_step = (INT32)(-ui4_col_num);
                }             
            }
            else
            {
                *pi4_nav_step = (INT32)(-ui4_col_num);
            }
        }
        else if (e_nvgt_dir == WGL_MBW_NVGT_DIR_DOWN)
        {
           if ((UINT32)i4_idx_from+ui4_col_num >= pt_ctrl_logic->ui4_item_total_num)
           {
                b_nav_dir_out_bound = TRUE;
                ui4_row_num = ((UINT32)i4_idx_from - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first)/ui4_col_num+1;
           }
           else
           {
                b_nav_dir_out_bound = ((pt_ctrl_logic->t_vp_info.ui4_idx_vp_last-(ui4_col_num-((UINT32)i4_idx_from % ui4_col_num)-1 ))== (UINT32)i4_idx_from);
           }
            /*vertical move in same column*/
            if (b_nav_dir_out_bound)
            {
               if (pt_ctrl_logic->t_nvgt_custom.t_nvgt_verti_custom== WGL_MBW_NVGT_VERTI_LOOP_IN_ONE_PAGE)
                {
                    *pi4_nav_step = (-(INT32)(ui4_col_num*(ui4_row_num-1)));
                }
                else
                {
                    /* <Modify by chunzhou,20090115> 
                                       Change the default behavior when the current hlt item is in the next to 
                                       last row, and it does't have item which is directly below it */
                    if ((((UINT32)i4_idx_from / ui4_col_num + 1) == ((pt_ctrl_logic->ui4_item_total_num - 1) / ui4_col_num)) &&
                        (((UINT32)i4_idx_from % ui4_col_num) > (pt_ctrl_logic->ui4_item_total_num - 1) % ui4_col_num))
                    {
                        *pi4_nav_step = (INT32)pt_ctrl_logic->ui4_item_total_num - 1 - i4_idx_from; 
                    }
                    else
                    {
                        *pi4_nav_step = (INT32)ui4_col_num;
                    }
                }
                
            }
            else
            {
                if ((((UINT32)i4_idx_from / ui4_col_num + 1) == ((pt_ctrl_logic->ui4_item_total_num - 1) / ui4_col_num)) &&
                    (((UINT32)i4_idx_from % ui4_col_num) > (pt_ctrl_logic->ui4_item_total_num - 1) % ui4_col_num))
                {
                    *pi4_nav_step = (INT32)pt_ctrl_logic->ui4_item_total_num - 1 - i4_idx_from; 
                }
                else
                {
                    *pi4_nav_step = (INT32)ui4_col_num;
                }
            }
        }
        else
        {
            DBG_ERROR(("{WGL(MBW)} ERR: the nvgt direction is invalid. at L%d\n\n",  __LINE__));
            return WGLR_INV_ARG;
        }
    }
    else
    {
        if (e_nvgt_dir == WGL_MBW_NVGT_DIR_LEFT)
        {
            *pi4_nav_step = (INT32)(-ui4_row_num);
        }
        else if (e_nvgt_dir == WGL_MBW_NVGT_DIR_RIGHT)
        {
            /* <Modify by chunzhou,20090115> */
            if ((((UINT32)i4_idx_from / ui4_row_num + 1) == ((pt_ctrl_logic->ui4_item_total_num - 1) / ui4_row_num)) &&
                 ((UINT32)i4_idx_from % ui4_row_num) > ((pt_ctrl_logic->ui4_item_total_num - 1) % ui4_row_num))
            {
                *pi4_nav_step = (INT32)pt_ctrl_logic->ui4_item_total_num - 1 - i4_idx_from; 
            }
            else
            {
                *pi4_nav_step = (INT32)ui4_row_num;
            }
        }
        else if (e_nvgt_dir == WGL_MBW_NVGT_DIR_UP)
        {
            b_nav_dir_out_bound = (i4_idx_from % ui4_row_num == 0);
            /*vertical move in same column*/
            if (b_nav_dir_out_bound)
            {
                if (HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_WRAP_OVER_VERTI))
                {
                    if (pt_ctrl_logic->t_nvgt_custom.t_nvgt_horiz_custom == WGL_MBW_NVGT_HORIZ_LOOP_IN_ONE_PAGE)
                    {
                        *pi4_nav_step = (INT32)ui4_row_num - 1;
                    }
                    else
                    {
                        *pi4_nav_step = -1;
                    }
                }
                else
                {
                    *pi4_nav_step = 0;
                }
            }
            else
            {
                *pi4_nav_step = -1;
            }
        }
        else if (e_nvgt_dir == WGL_MBW_NVGT_DIR_DOWN)
        {
            if (i4_idx_from < (INT32)pt_ctrl_logic->ui4_item_total_num - 1)
            {
                b_nav_dir_out_bound = ((UINT32)i4_idx_from % ui4_row_num == ui4_row_num - (UINT32)1);
            }
            else
            {
                b_nav_dir_out_bound = TRUE;
                ui4_row_num = (UINT32)i4_idx_from % ui4_row_num + (UINT32)1;
            }

            /*vertical move in same column*/
            if (b_nav_dir_out_bound)
            {
                if (HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_WRAP_OVER_VERTI))
                {
                    if (pt_ctrl_logic->t_nvgt_custom.t_nvgt_horiz_custom == WGL_MBW_NVGT_HORIZ_LOOP_IN_ONE_PAGE)
                    {
                        *pi4_nav_step = 1 - (INT32)ui4_row_num;
                    }
                    else
                    {
                        *pi4_nav_step = 1;
                    }
                }
                else
                {
                    *pi4_nav_step = 0;
                }
            }
            else
            {
                *pi4_nav_step = 1;
            }
        }
        else
        {
            DBG_ERROR(("{WGL(MBW)} ERR: the nvgt direction is invalid. at L%d\n\n",  __LINE__));
            return WGLR_INV_ARG;
        }
    }
    return WGLR_OK;

}

static INT32  _mbw_on_wrap_over(
    WGL_MBW_CONTROL_LOGIC_T* pt_ctrl_logic,
    INT32*                   pi4_idx_to,
    BOOL*                    pb_wrap_over,
    WGL_MBW_NVGT_DIR_T       e_nvgt_dir)
{
    INT32 i4_total_num;
    INT32 i4_col_num;
    INT32 i4_row_num;

    i4_total_num = (INT32)pt_ctrl_logic->ui4_item_total_num;
    i4_col_num = (INT32)pt_ctrl_logic->ui4_col_num;
    i4_row_num = (INT32)pt_ctrl_logic->ui4_row_num;
    *pb_wrap_over = TRUE;
    if(!HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_HORIZ_ARRANGE))
    {
        /*in verti arrange mode, right, down or left nvgt direction may lead to this situation*/
        if (*pi4_idx_to >= i4_total_num)
        {
            /*keep alignment between the old and new hlt item*/
            if (e_nvgt_dir == WGL_MBW_NVGT_DIR_RIGHT)
            {
                *pi4_idx_to = *pi4_idx_to - i4_total_num;
            }
            if (e_nvgt_dir == WGL_MBW_NVGT_DIR_DOWN)
            {
                *pi4_idx_to = *pi4_idx_to % i4_col_num;
            }
            if (e_nvgt_dir == WGL_MBW_NVGT_DIR_LEFT)
            {
                *pi4_idx_to = i4_total_num - 1;
                /*reset wrap_over attribute*/
                *pb_wrap_over = FALSE;
            }
        }
        /*in verti arrange mode, only left or up nvgt direction may lead to this situation*/
        if (*pi4_idx_to < 0)
        {
            if (e_nvgt_dir == WGL_MBW_NVGT_DIR_LEFT)
            {
                *pi4_idx_to = *pi4_idx_to + i4_total_num;
            }
            if (e_nvgt_dir == WGL_MBW_NVGT_DIR_UP)
            {
                INT32 i4_total_row_num;
                i4_total_row_num = (i4_total_num % i4_col_num == 0) ? (i4_total_num / i4_col_num) : (i4_total_num / i4_col_num + 1);
                *pi4_idx_to += i4_total_row_num * i4_col_num;
                if (*pi4_idx_to >= i4_total_num)
                {
                    *pi4_idx_to -= i4_col_num;
                }
            }
        }
    }
    else
    {
        /*in horiz arrange mode, right, down or up nvgt direction may lead to this situation*/
        if (*pi4_idx_to >= i4_total_num)
        {
            if (e_nvgt_dir == WGL_MBW_NVGT_DIR_RIGHT)
            {
                *pi4_idx_to = *pi4_idx_to % i4_row_num;
            }
            if (e_nvgt_dir == WGL_MBW_NVGT_DIR_DOWN)
            {
                *pi4_idx_to = *pi4_idx_to - i4_total_num;
            }
            if (e_nvgt_dir == WGL_MBW_NVGT_DIR_UP)
            {
                *pi4_idx_to = i4_total_num - 1;
                /*reset wrap_over attribute*/
                *pb_wrap_over = FALSE;
            }
        }
        /*in horiz arrange mode, only left or up nvgt direction may lead to this situation*/
        if (*pi4_idx_to < 0)
        {
            if (e_nvgt_dir == WGL_MBW_NVGT_DIR_LEFT)
            {
                UINT32 ui4_total_col_num;
                ui4_total_col_num = (UINT32)((i4_total_num % i4_row_num == 0) ? (i4_total_num / i4_row_num) : (i4_total_num / i4_row_num + 1));
                *pi4_idx_to += ui4_total_col_num * i4_row_num;
                if (*pi4_idx_to >= i4_total_num)
                {
                    *pi4_idx_to -= i4_row_num;
                }
            }
            if (e_nvgt_dir == WGL_MBW_NVGT_DIR_UP)
            {
                *pi4_idx_to = *pi4_idx_to + i4_total_num;
            }
        }
    }
    return WGLR_OK;
}

static INT32   _mbw_setup_nvgt_info(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    WGL_MBW_NVGT_INFO_T*        pt_nvgt_info,
    BOOL*                       pb_navigable,
    BOOL*                       pb_vp_changed,
    BOOL*                       pb_wrap_over,
    UINT32                      ui4_idx_orig,
    UINT32                      ui4_idx_new,
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir)
{
    UINT32      ui4_col_num;
    UINT32      ui4_row_num;
    UINT32      ui4_idx_vp_first;
    UINT32      ui4_idx_vp_last;
    ui4_col_num = pt_ctrl_logic->ui4_col_num;
    ui4_row_num = pt_ctrl_logic->ui4_row_num;
    ui4_idx_vp_first = pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;
    ui4_idx_vp_last = pt_ctrl_logic->t_vp_info.ui4_idx_vp_last;

    if (e_nvgt_dir != WGL_MBW_NVGT_DIR_PG_UP && e_nvgt_dir != WGL_MBW_NVGT_DIR_PG_DOWN)
    {
        if (*pb_navigable && !(*pb_vp_changed) && !HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_PAGE_MODE))
        {
            /*check hlt margin*/
            if(HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_HORIZ_ARRANGE))
            {
                if ((ui4_idx_vp_first != 0 &&
                     (e_nvgt_dir == WGL_MBW_NVGT_DIR_UP || e_nvgt_dir == WGL_MBW_NVGT_DIR_LEFT) &&
                     ((ui4_idx_new -ui4_idx_vp_first) / ui4_row_num < pt_ctrl_logic->ui4_hlt_margin) &&
                     ((ui4_idx_orig -ui4_idx_vp_first) / ui4_row_num >= pt_ctrl_logic->ui4_hlt_margin)) ||
                    ((ui4_idx_vp_last != pt_ctrl_logic->ui4_item_total_num - 1) &&
                      (e_nvgt_dir == WGL_MBW_NVGT_DIR_DOWN || e_nvgt_dir == WGL_MBW_NVGT_DIR_RIGHT) &&
                      ((ui4_idx_new -ui4_idx_vp_first) / ui4_row_num > (ui4_idx_vp_last -ui4_idx_vp_first) / ui4_row_num - pt_ctrl_logic->ui4_hlt_margin) &&
                      ((ui4_idx_orig -ui4_idx_vp_first) / ui4_row_num <= (ui4_idx_vp_last -ui4_idx_vp_first) / ui4_row_num - pt_ctrl_logic->ui4_hlt_margin)))
                {
                        *pb_vp_changed = TRUE;
                }
            }
            else
            {
                if ((ui4_idx_vp_first != 0 &&
                     (e_nvgt_dir == WGL_MBW_NVGT_DIR_UP || e_nvgt_dir == WGL_MBW_NVGT_DIR_LEFT) &&
                     ((ui4_idx_new -ui4_idx_vp_first) / ui4_col_num < pt_ctrl_logic->ui4_hlt_margin) &&
                     ((ui4_idx_orig -ui4_idx_vp_first) / ui4_col_num >= pt_ctrl_logic->ui4_hlt_margin)) ||
                    ((ui4_idx_vp_last != pt_ctrl_logic->ui4_item_total_num - 1) &&
                      (e_nvgt_dir == WGL_MBW_NVGT_DIR_DOWN || e_nvgt_dir == WGL_MBW_NVGT_DIR_RIGHT) &&
                      ((ui4_idx_new -ui4_idx_vp_first) / ui4_col_num > (ui4_idx_vp_last -ui4_idx_vp_first) / ui4_col_num - pt_ctrl_logic->ui4_hlt_margin) &&
                      ((ui4_idx_orig -ui4_idx_vp_first) / ui4_col_num <= (ui4_idx_vp_last -ui4_idx_vp_first) / ui4_col_num - pt_ctrl_logic->ui4_hlt_margin)))
                {
                    *pb_vp_changed = TRUE;
                }
            }
        }
        pt_nvgt_info->ui1_strct_ver = 1;
        pt_nvgt_info->b_navigable   = *pb_navigable;
        pt_nvgt_info->b_vp_changed  = *pb_vp_changed;
        pt_nvgt_info->b_wrap_over   = *pb_wrap_over;
        pt_nvgt_info->e_nvgt_dir    = e_nvgt_dir;
        pt_nvgt_info->ui4_idx_new   = ui4_idx_new;
        pt_nvgt_info->ui4_idx_orig  = ui4_idx_orig;

    }
    else
    {
        /*set nvgt info,maybe something need change*/
        if (!*pb_navigable)
        {
            if (*pb_wrap_over)
            {
                UINT32 ui4_page_offset;
                UINT32 ui4_new_vp_idx_first;
                UINT32 ui4_new_vp_idx_last;
                /*get the page which contains the new item*/
                ui4_page_offset = ui4_idx_new / (ui4_col_num * ui4_row_num);
                ui4_new_vp_idx_first = ui4_page_offset * (ui4_col_num * ui4_row_num);
                ui4_new_vp_idx_last = ui4_new_vp_idx_first + ui4_col_num * ui4_row_num - 1;
                /*the last item index in viewport can't larger than the total item number*/
                if (ui4_new_vp_idx_last >= pt_ctrl_logic->ui4_item_total_num)
                {
                    ui4_new_vp_idx_last = pt_ctrl_logic->ui4_item_total_num - 1;
                }
                /*the b_vp_changed should be reset here*/
                if (ui4_new_vp_idx_first != ui4_idx_vp_first || ui4_new_vp_idx_last != ui4_idx_vp_last)
                {
                    *pb_vp_changed = TRUE;
                }
                else
                {
                    *pb_vp_changed = FALSE;
                }
            }
            else
            {
                *pb_vp_changed = FALSE;
            }
        }
        pt_nvgt_info->ui1_strct_ver = 1;
        pt_nvgt_info->b_navigable   = *pb_navigable;
        pt_nvgt_info->b_vp_changed  = *pb_vp_changed;
        pt_nvgt_info->b_wrap_over   = *pb_wrap_over;
        pt_nvgt_info->e_nvgt_dir    = e_nvgt_dir;
        pt_nvgt_info->ui4_idx_new   = ui4_idx_new;
        pt_nvgt_info->ui4_idx_orig  = ui4_idx_orig;
    }

    return WGLR_OK;
}

static INT32   _mbw_gain_new_item_for_page_up_down(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    INT32                       i4_idx_from,
    INT32*                      pi4_idx_to,
    WGL_MBW_NVGT_DIR_T          e_nvgt_dir,
    WGL_MBW_VP_INFO_T           t_vp_info,
    BOOL                        b_in_orig_vp,
    BOOL*                       pb_valid)
{
    UINT32 ui4_idx_vp_first;
    UINT32 ui4_idx_vp_last;
    UINT32 ui4_step;
    INT32  i4_ret;
    UINT32 ui4_item_state;

    ui4_idx_vp_first = t_vp_info.ui4_idx_vp_first;
    ui4_idx_vp_last = t_vp_info.ui4_idx_vp_last;

    if(HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_HORIZ_ARRANGE))
    {
        ui4_step = pt_ctrl_logic->ui4_row_num;
    }
    else
    {
        ui4_step = pt_ctrl_logic->ui4_col_num;
    }

    if (e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_DOWN)
    {
        /*last col or row may not be filled full, keep alignment between the old and new item*/
        if ((UINT32)i4_idx_from % ui4_step <= ui4_idx_vp_last % ui4_step)
        {
            *pi4_idx_to = (INT32)(ui4_idx_vp_last - (ui4_idx_vp_last % ui4_step - i4_idx_from % ui4_step));
        }
        else
        {
            *pi4_idx_to = (INT32)((ui4_idx_vp_last - ui4_step) + (i4_idx_from % ui4_step - ui4_idx_vp_last % ui4_step));
        }
        /*the item for ending the traverse in original vp is different from that in other vp*/
        if (!b_in_orig_vp)
        {
            i4_idx_from = (INT32)(ui4_idx_vp_first + i4_idx_from % ui4_step - ui4_step);
        }

        while(*pi4_idx_to > i4_idx_from)
        {
            /*get item state*/
            i4_ret = x_wgl_mbw_get_item_state(pt_ctrl_logic->h_mbw, (UINT32)(*pi4_idx_to), &ui4_item_state);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: get item state faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
            /*check whether the item is enabled*/
            if ((ui4_item_state & WGL_MBW_ITEM_STATE_DISABLED))
            {
                *pi4_idx_to -= ui4_step;
            }
            else
            {
                *pb_valid = TRUE;
                return WGLR_OK;
            }
        }
        *pb_valid = FALSE;
        return WGLR_OK;

    }

    if (e_nvgt_dir == WGL_MBW_NVGT_DIR_PG_UP)
    {
        *pi4_idx_to = (INT32)(ui4_idx_vp_first + i4_idx_from % ui4_step);

        if (!b_in_orig_vp)
        {
            if ((UINT32)i4_idx_from % ui4_step <= ui4_idx_vp_last % ui4_step)
            {
                i4_idx_from = (INT32)(ui4_idx_vp_last - (ui4_idx_vp_last % ui4_step - i4_idx_from % ui4_step));
            }
            else
            {
                i4_idx_from = (INT32)((ui4_idx_vp_last - ui4_step) + (i4_idx_from % ui4_step - ui4_idx_vp_last % ui4_step));
            }
            i4_idx_from += ui4_step;
        }

        while(*pi4_idx_to < i4_idx_from)
        {
            /*get item state*/
            i4_ret = x_wgl_mbw_get_item_state(pt_ctrl_logic->h_mbw, (UINT32)(*pi4_idx_to), &ui4_item_state);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: get item state faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }

            /*check whether the item is enabled*/
            if ((ui4_item_state & WGL_MBW_ITEM_STATE_DISABLED))
            {
                *pi4_idx_to += ui4_step;
            }
            else
            {
                *pb_valid = TRUE;
                return WGLR_OK;
            }
        }
        *pb_valid = FALSE;
        return WGLR_OK;

    }
    return WGLR_OK;
}

static  INT32 _mbw_set_vp_bk_image(WGL_MBW_CONTROL_LOGIC_T* pt_ctrl_logic)
{
    UINT16         ui2_mbw_state;
    WGL_IMG_INFO_T t_img_info;
    WGL_HIMG_TPL_T h_img_tpl = NULL_HANDLE;

    ui2_mbw_state = pt_ctrl_logic->ui2_mbw_state;

    if (WGL_IS_FLAG_TURN_OFF(pt_ctrl_logic->ui1_extra_flag, MBW_VP_BK_IMG_NOT_INIT))
    {
        /*select image*/
        if (IS_STATE_ACTIVATED(ui2_mbw_state) && IS_STATE_FOCUSED(ui2_mbw_state))
        {
            h_img_tpl = pt_ctrl_logic->t_vp_bk_img.t_highlight;
        }
        else if (IS_STATE_ENABLED(ui2_mbw_state))
        {
            h_img_tpl = pt_ctrl_logic->t_vp_bk_img.t_enable;
        }
        else
        {
            h_img_tpl = pt_ctrl_logic->t_vp_bk_img.t_disable;
        }
    }
    /*image handle is valid,because vp container is always in normal state.
    so change its bk image according to mbw state*/
    t_img_info.e_type = WGL_IMG_SET_STANDARD;
    t_img_info.u_img_data.t_standard.t_enable    = h_img_tpl;
    t_img_info.u_img_data.t_standard.t_disable   = h_img_tpl;
    t_img_info.u_img_data.t_standard.t_highlight = h_img_tpl;
    return x_wgl_do_cmd(
                    pt_ctrl_logic->h_mbw_vp,
                    WGL_CMD_GL_SET_IMAGE,
                    WGL_PACK(WGL_IMG_BK),
                    WGL_PACK(&t_img_info));
}

static  INT32 _mbw_set_vp_bk_color(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic)
{
    UINT16           ui2_mbw_state;
    WGL_COLOR_INFO_T t_clr_info;
    GL_COLOR_T       t_vpbk_clr = {0};

    ui2_mbw_state = pt_ctrl_logic->ui2_mbw_state;

    if (WGL_IS_FLAG_TURN_OFF(pt_ctrl_logic->ui1_extra_flag, MBW_VP_BK_CLR_NOT_INIT))
    {
        /*select color*/
        if (IS_STATE_ACTIVATED(ui2_mbw_state) && IS_STATE_FOCUSED(ui2_mbw_state))
        {
            t_vpbk_clr = pt_ctrl_logic->t_vp_bk_clr.t_highlight;
        }
        else if (IS_STATE_ENABLED(ui2_mbw_state))
        {
            t_vpbk_clr = pt_ctrl_logic->t_vp_bk_clr.t_enable;
        }
        else
        {
            t_vpbk_clr = pt_ctrl_logic->t_vp_bk_clr.t_disable;
        }
    }
    t_clr_info.e_type = WGL_COLOR_SET_STANDARD;
    t_clr_info.u_color_data.t_standard.t_enable = t_vpbk_clr;
    t_clr_info.u_color_data.t_standard.t_disable = t_vpbk_clr;
    t_clr_info.u_color_data.t_standard.t_highlight = t_vpbk_clr;

    return x_wgl_do_cmd(
            pt_ctrl_logic->h_mbw_vp,
            WGL_CMD_GL_SET_COLOR,
            WGL_PACK(WGL_CLR_BK),
            WGL_PACK(&t_clr_info));
}


static INT32 _mbw_get_item_ctnr_info(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    UINT32                      ui4_item_idx,
    WGL_MBW_ITEM_CTNR_INFO_T**  ppt_item_ctnr_info)
{
    if(pt_ctrl_logic== NULL ||
        (ui4_item_idx < pt_ctrl_logic->t_vp_info.ui4_idx_vp_first || ui4_item_idx > pt_ctrl_logic->t_vp_info.ui4_idx_vp_last))
    {
        DBG_ERROR(("{WGL(MBW)} ERR:get item container faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    *ppt_item_ctnr_info = &pt_ctrl_logic->at_item_ctnr_info[ui4_item_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first];
    return WGLR_OK;
}

static INT32 _mbw_update_vp_to_show_item(
    WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic,
    UINT32                      ui4_item_idx)
{
    INT32      i4_ret;
    INT32      i4_new_vp_idx_first;
    INT32      i4_new_vp_idx_last;
    UINT32     ui4_page_offset;
    UINT32     ui4_item_count;
    UINT32     ui4_item_ctnr_idx;
    UINT32     ui4_vp_capacity;

    MBW_CHECK_VALID((pt_ctrl_logic != NULL), WGLR_INV_ARG);

    ui4_vp_capacity = pt_ctrl_logic->ui4_col_num * pt_ctrl_logic->ui4_row_num;
    i4_new_vp_idx_first = (INT32)pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;
    i4_new_vp_idx_last = (INT32)pt_ctrl_logic->t_vp_info.ui4_idx_vp_last;

    if (HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_PAGE_MODE))
    {
        /*get the page which contains the new item*/
        ui4_page_offset = ui4_item_idx / ui4_vp_capacity;
        i4_new_vp_idx_first = (INT32)(ui4_page_offset * ui4_vp_capacity);
        i4_new_vp_idx_last = i4_new_vp_idx_first + (INT32)ui4_vp_capacity - 1;
        /*the last item index in viewport may larger than the total item number*/
        if (i4_new_vp_idx_last >= (INT32)pt_ctrl_logic->ui4_item_total_num)
        {
            i4_new_vp_idx_last = (INT32)pt_ctrl_logic->ui4_item_total_num - 1;
        }
    }
    /*scroll mode*/
    else
    {
        UINT32 ui4_step;
        /*check the arrange mode*/
        if (!HAS_STYLE(pt_ctrl_logic, WGL_MBW_STL_HORIZ_ARRANGE))
        {
            ui4_step = pt_ctrl_logic->ui4_col_num;
        }
        else
        {
            ui4_step = pt_ctrl_logic->ui4_row_num;
        }

        if (ui4_item_idx > pt_ctrl_logic->t_vp_info.ui4_idx_vp_last)
        {
            i4_new_vp_idx_last = (INT32)(ui4_item_idx +
                                    ((ui4_step -1) - ui4_item_idx % ui4_step) +
                                    pt_ctrl_logic->ui4_hlt_margin * ui4_step);
            i4_new_vp_idx_first = i4_new_vp_idx_last - (INT32)ui4_vp_capacity + 1;

            if (i4_new_vp_idx_first < 0)
            {
                i4_new_vp_idx_first = 0;
                i4_new_vp_idx_last = i4_new_vp_idx_first + ui4_vp_capacity - 1;
            }
            /*the last item index in viewport may larger than the total item number*/
            if (i4_new_vp_idx_last >= (INT32)pt_ctrl_logic->ui4_item_total_num)
            {
                i4_new_vp_idx_last = (INT32)pt_ctrl_logic->ui4_item_total_num - 1;
            }
        }

        if (ui4_item_idx < pt_ctrl_logic->t_vp_info.ui4_idx_vp_first)
        {
            i4_new_vp_idx_first = (INT32)(ui4_item_idx -
                                    ui4_item_idx % ui4_step -
                                    pt_ctrl_logic->ui4_hlt_margin * ui4_step);
            if (i4_new_vp_idx_first < 0)
            {
                i4_new_vp_idx_first = 0;
            }
            i4_new_vp_idx_last = i4_new_vp_idx_first + ui4_vp_capacity - 1;
            /*the last item index in viewport may larger than the total item number*/
            if (i4_new_vp_idx_last >= (INT32)pt_ctrl_logic->ui4_item_total_num)
            {
                i4_new_vp_idx_last = (INT32)pt_ctrl_logic->ui4_item_total_num - 1;
            }
        }
    }
    pt_ctrl_logic->t_vp_info.ui4_idx_vp_first = (UINT32)i4_new_vp_idx_first;
    pt_ctrl_logic->t_vp_info.ui4_idx_vp_last = (UINT32)i4_new_vp_idx_last;

    /*set or reset the items' visibility*/
    for(ui4_item_count = 0; ui4_item_count < ui4_vp_capacity;ui4_item_count++)
    {
        if (ui4_item_count + pt_ctrl_logic->t_vp_info.ui4_idx_vp_first <= pt_ctrl_logic->t_vp_info.ui4_idx_vp_last)
        {
            i4_ret = x_wgl_set_visibility(pt_ctrl_logic->at_item_ctnr_info[ui4_item_count].h_item_ctnr, WGL_SW_NORMAL);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
        else
        {
            i4_ret = x_wgl_set_visibility(pt_ctrl_logic->at_item_ctnr_info[ui4_item_count].h_item_ctnr, WGL_SW_HIDE);
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR: Fail to set the item's visibility (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
    }

    x_memset(pt_ctrl_logic->at_item_data, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_DATA_T) * ui4_vp_capacity));
    x_memset(pt_ctrl_logic->aui4_item_state, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_STATE_T) * ui4_vp_capacity));
    x_memset(pt_ctrl_logic->at_need_repaint_ex_region, 0, WGL_ALIGN_4(sizeof(WGL_MBW_ITEM_EX_REPAINT_REGION) * ui4_vp_capacity));
    /*update item data*/
    MBW_GET_ITEM_DATA(
                    pt_ctrl_logic->h_mbw,
                    pt_ctrl_logic,
                    WGL_MBW_ITEM_DATA_MASK_NVGT_MAP,
                    pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                    pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                    pt_ctrl_logic->at_item_data);
    /*update item state*/
    MBW_GET_ITEM_STATE(
                    pt_ctrl_logic->h_mbw,
                    pt_ctrl_logic,
                    pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                    pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                    pt_ctrl_logic->aui4_item_state);
    /*update items ctnr*/
    MBW_UPDATE_ITEM_CTNR(
                    pt_ctrl_logic->h_mbw,
                    pt_ctrl_logic,
                    WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE | WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATIC,
                    pt_ctrl_logic->t_vp_info.ui4_idx_vp_first,
                    pt_ctrl_logic->t_vp_info.ui4_idx_vp_last,
                    pt_ctrl_logic->at_item_ctnr_info);

    pt_ctrl_logic->b_vp_changed = TRUE;
    /*clear the item ctnr region*/
    for(ui4_item_ctnr_idx = 0; ui4_item_ctnr_idx < ui4_vp_capacity; ui4_item_ctnr_idx++)
    {
        pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].b_is_dirty = FALSE;
        i4_ret = x_wgl_mbw_get_item_ctnr_rect(
                    pt_ctrl_logic->h_mbw,
                    ui4_item_ctnr_idx,
                    &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg));
        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(MBW)} ERR:get item container rect faild. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    return WGLR_OK;
}

static INT32 _mbw_show_hlt_item(WGL_MBW_CONTROL_LOGIC_T* pt_ctrl_logic)
{
    GL_RECT_T t_item_ctnr_rect;
    GL_RECT_T t_hlt_item_ctnr_rect;
    UINT32    ui4_idx;
    INT32     i4_ret;

    ui4_idx = pt_ctrl_logic->ui4_hlt_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;

    /*call dm to request highlight item data*/
    MBW_UPDATE_HLT_ITEM_CTNR(
                    pt_ctrl_logic->h_mbw,
                    pt_ctrl_logic,
                    WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE | WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATIC);

    /*calcualte highlight container new position*/
    i4_ret = x_wgl_mbw_get_item_ctnr_rect(pt_ctrl_logic->h_mbw, ui4_idx, &t_item_ctnr_rect);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:get item container rect faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return i4_ret;
    }
    t_hlt_item_ctnr_rect.i4_left = t_item_ctnr_rect.i4_left - pt_ctrl_logic->t_hlt_item_info.t_ex_margin.ui2_left;
    t_hlt_item_ctnr_rect.i4_right = t_item_ctnr_rect.i4_right + pt_ctrl_logic->t_hlt_item_info.t_ex_margin.ui2_right;
    t_hlt_item_ctnr_rect.i4_top = t_item_ctnr_rect.i4_top - pt_ctrl_logic->t_hlt_item_info.t_ex_margin.ui2_top;
    t_hlt_item_ctnr_rect.i4_bottom = t_item_ctnr_rect.i4_bottom + pt_ctrl_logic->t_hlt_item_info.t_ex_margin.ui2_bottom;
    /*move highlight container to new position*/
    i4_ret = x_wgl_move(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, &t_hlt_item_ctnr_rect, WGL_NO_AUTO_REPAINT);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:move item container faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return i4_ret;
    }
    /*set highlight item container is visible*/
    i4_ret = x_wgl_set_visibility(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, WGL_SW_NORMAL);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:set highlight item container visibility faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return i4_ret;
    }
    /*set the item container underside the highlight item container is invisible*/
    i4_ret = x_wgl_set_visibility(pt_ctrl_logic->at_item_ctnr_info[ui4_idx].h_item_ctnr, WGL_SW_HIDE);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:set item container visibility faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return i4_ret;
    }
    return WGLR_OK;
}


static INT32 _mbw_hide_hlt_item(WGL_MBW_CONTROL_LOGIC_T* pt_ctrl_logic)
{
    INT32                     i4_ret;
    WGL_MBW_ITEM_CTNR_INFO_T* at_item_ctnr_info;
    UINT32                    ui4_idx;
    UINT32                    ui4_ctnr_idx;
    GL_RECT_T                 t_hlt_ctnr_rect;

    ui4_idx = pt_ctrl_logic->ui4_hlt_idx;
    i4_ret = _mbw_get_item_ctnr_info(pt_ctrl_logic, ui4_idx, &at_item_ctnr_info);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:get item container info faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return i4_ret;
    }
    /*let item container hide below hightlight item contaienr update its data */
    MBW_UPDATE_ITEM_CTNR(
                   pt_ctrl_logic->h_mbw,
                   pt_ctrl_logic,
                   WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE,
                   ui4_idx,
                   ui4_idx,
                   at_item_ctnr_info);

    /*set highlight item container is hide*/
    i4_ret = x_wgl_set_visibility(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, WGL_SW_HIDE);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:set hlt item container visibility faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return i4_ret;
    }
    /*set the item container underside the highlight item container is visible*/
    i4_ret = x_wgl_set_visibility(at_item_ctnr_info[0].h_item_ctnr, WGL_SW_NORMAL);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:set item container visibility faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return i4_ret;
    }
    /*need to repaint extra hlt margin*/
    i4_ret = x_wgl_get_coords(pt_ctrl_logic->t_hlt_item_ctnr_info.h_item_ctnr, WGL_COORDS_PARENT, &t_hlt_ctnr_rect);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:get item container rect faild. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return i4_ret;
    }
    /*item need repaint extend region, otherwise will occur ui garbage*/
    ui4_ctnr_idx = ui4_idx - pt_ctrl_logic->t_vp_info.ui4_idx_vp_first;
    pt_ctrl_logic->at_need_repaint_ex_region[ui4_ctnr_idx].b_is_dirty = TRUE;
    UNION_RECT(
        &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_ctnr_idx].t_dirty_reg),
        &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_ctnr_idx].t_dirty_reg),
        &t_hlt_ctnr_rect);

    return WGLR_OK;
}

static  INT32  _mbw_clear_item_dirty_region(WGL_MBW_CONTROL_LOGIC_T*    pt_ctrl_logic)
{
    GL_RECT_T    t_update_rect;
    UINT32       ui4_num_update_rect;
    UINT32       ui4_vp_capacity;
    UINT32       ui4_item_ctnr_idx;
    INT32        i4_ret;

    ui4_vp_capacity = pt_ctrl_logic->ui4_col_num * pt_ctrl_logic->ui4_row_num;
    i4_ret = x_wgl_get_update_region(pt_ctrl_logic->h_mbw, &ui4_num_update_rect, &t_update_rect);
    if (WGLR_OK != i4_ret || ui4_num_update_rect != 1)
    {
        DBG_ERROR(("{WGL(MBW)} ERR:get update region faild. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    for(ui4_item_ctnr_idx = 0; ui4_item_ctnr_idx < ui4_vp_capacity; ui4_item_ctnr_idx++)
    {
        if(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].b_is_dirty &&
            RECT_INCLUDED(&t_update_rect, &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg)))
        {
            pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].b_is_dirty = FALSE;
            i4_ret = x_wgl_mbw_get_item_ctnr_rect(
                        pt_ctrl_logic->h_mbw,
                        ui4_item_ctnr_idx,
                        &(pt_ctrl_logic->at_need_repaint_ex_region[ui4_item_ctnr_idx].t_dirty_reg));
            if (WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(MBW)} ERR:get item container rect faild. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
    }

    return WGLR_OK;
}


