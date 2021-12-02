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
 * $RCSfile: wgl_lb.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/7 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: 95c75eb6d2299fa344f9ebf912006ab5 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "dbg/def_dbg_level_mw.h"
#include "unicode/x_uc_str.h"
#include "wgl/wgl.h"
#include "wgl/u_wgl_bdr_common.h"
#include "wgl/wgl_bdr_internal.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/x_wgl_sets.h"

#include "wgl/listbox/x_wgl_lb.h"
#include "wgl/listbox/wgl_lb.h"
#include "wgl/listbox/wgl_lb_cnt_internal.h"
#include "wgl/listbox/wgl_lb_cnt_def.h"
#include "wgl/listbox/wgl_lb_dbg.h"
#include "wgl/arabic.h"
#ifdef WGL_ANIM_SUPPORT
#include "wgl/animation/x_wgl_animation.h"
#endif


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE    wgl_lb_cli_get_eff_dbg_level()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME     "LB"

#define LB_ELEM_INF_POOL_EXP_NUM   100 

#define LB_MAX_UPD_RANGE           2 

#ifdef _CHECK_CONSISTENCY
    
    #define _LB_CHECK_CONSISTENCY(_pt_ctx)                          \
    {                                                               \
        if(! _lb_debug_check_consistency(_pt_ctx) )                 \
        {                                                           \
            DBG_ERROR(("{WGL(%s)} ERR: _LB_CHECK_CONSITENCY() fail! File=%s Line=%d\n", DBG_MODULE_NAME, __FILE__, __LINE__)); \
        }                                                           \
    }

#else
    #define _LB_CHECK_CONSISTENCY(_pt_ctx)                                 
    
#endif    


#ifdef DEBUG

#define GET_LOCK(_h_lb, _ppt_ctx)                                   \
    {                                                               \
        INT32  i4_ret_get_lock;                                     \
        i4_ret_get_lock = wgl_get_instance((_h_lb), (WGL_WIDGET_CONTEXT_HANDLE_T*)(_ppt_ctx));\
        if(WGL_IS_FAIL(i4_ret_get_lock))                            \
        {                                                           \
            DBG_ERROR(("{WGL(%s)} ERR: GET_LOCK()=%d File:%s Line:%d\n", DBG_MODULE_NAME, (i4_ret_get_lock), __FILE__, __LINE__));\
            return (i4_ret_get_lock);                               \
        }                                                           \
    }

#define REL_LOCK(_h_lb, _pt_ctx)                                    \
    {                                                               \
        INT32   i4_ret_rel_lock;                                    \
        i4_ret_rel_lock = wgl_release_instance((_h_lb), (WGL_WIDGET_CONTEXT_HANDLE_T)(_pt_ctx));\
        (_pt_ctx) = NULL;                                           \
        if(WGL_IS_FAIL(i4_ret_rel_lock))                            \
        {                                                           \
            DBG_ERROR(("{WGL(%s)} ERR: RELEASE_LOCK()=%d File:%s Line:%d\n", DBG_MODULE_NAME, i4_ret_rel_lock, __FILE__, __LINE__));\
            return i4_ret_rel_lock;                                 \
        }                                                           \
    }


#else


#define GET_LOCK(_h_lb, _ppt_ctx)                                   \
    {                                                               \
        INT32  i4_ret_get_lock;                                     \
        i4_ret_get_lock = wgl_get_instance((_h_lb), (WGL_WIDGET_CONTEXT_HANDLE_T*)(_ppt_ctx));\
        if(WGL_IS_FAIL(i4_ret_get_lock))                            \
        {                                                           \
            return (i4_ret_get_lock);                               \
        }                                                           \
    }

#define REL_LOCK(_h_lb, _pt_ctx)                                    \
    {                                                               \
        INT32   i4_ret_rel_lock;                                    \
        i4_ret_rel_lock = wgl_release_instance((_h_lb), (WGL_WIDGET_CONTEXT_HANDLE_T)(_pt_ctx));\
        (_pt_ctx) = NULL;                                           \
        if(WGL_IS_FAIL(i4_ret_rel_lock))                            \
        {                                                           \
            return i4_ret_rel_lock;                                 \
        }                                                           \
    }

#endif    



#define _LB_ELEM_FREE_LIST_INS(_pt_elems_free_list, _pt_elem)       \
    do {                                                            \
        (_pt_elem)->pt_prev = NULL;                                 \
        (_pt_elem)->pt_next = (_pt_elems_free_list);                \
        if(_pt_elems_free_list)                                     \
        {                                                           \
            (_pt_elems_free_list)->pt_prev = (_pt_elem);            \
        }                                                           \
        (_pt_elems_free_list) = (_pt_elem);                         \
    } while (FALSE)


#define _IS_IMG_VALID(_h_img)                                       \
    (!((_h_img)!=NULL_HANDLE && !_lb_is_valid_image_handle(_h_img)))
    
#define _IMG_SUIT_COPY(_pt_dst, _pt_src)                            \
    x_memcpy((VOID *)_pt_dst, (VOID *)_pt_src, sizeof(WGL_LB_ITEM_IMG_SUIT_T));
    
#define _HAS_HLT_ELEM(_pt_ctx)                                      \
    ((_pt_ctx)->ui2_idx!=WGL_LB_NULL_INDEX)

#define _LB_VALID_IDX(_pt_ctx,_ui2_idx)                                      \
    ((_ui2_idx < (_pt_ctx)->ui2_elem_num) && (_ui2_idx != WGL_LB_NULL_INDEX))

#define _LB_ELEM_INF_INIT(_pt_elem_inf)                                      \
    lb_elem_inf_init(_pt_elem_inf)

#define _LB_INIT_ELEM_INF(_pt_ctx,_ui2_num)                                      \
    lb_init_elem_inf(&((_pt_ctx)->t_elem_inf_list), &((_pt_ctx)->t_npm_elem_inf),_ui2_num) 

#define _LB_DEINIT_ELEM_INF(_pt_ctx)                                      \
    lb_deinit_elem_inf(&((_pt_ctx)->t_elem_inf_list),&((_pt_ctx)->t_npm_elem_inf)) 

#define _LB_GET_ELEM_INF(_pt_ctx,_ui2_idx, _ppt_inf)                                      \
    lb_get_elem_inf(&((_pt_ctx)->t_elem_inf_list),(_pt_ctx)->ui2_elem_num,  _ui2_idx, _ppt_inf) 

#define _LB_ADD_ELEM_INF(_pt_ctx,_ui2_idx, _pt_inf)                                      \
    lb_add_elem_inf(&((_pt_ctx)->t_elem_inf_list),&((_pt_ctx)->ui2_elem_num),     \
                    &((_pt_ctx)->t_npm_elem_inf), _ui2_idx, _pt_inf) 

#define _LB_DEL_ELEM_INF(_pt_ctx,_ui2_idx)                                      \
    lb_del_elem_inf(&((_pt_ctx)->t_elem_inf_list),&((_pt_ctx)->ui2_elem_num), &((_pt_ctx)->t_npm_elem_inf), _ui2_idx) 

#define _LB_GET_ELEM_STATE(_pt_ctx,_ui2_idx)                                      \
    lb_get_elem_state(&((_pt_ctx)->t_elem_inf_list),(_pt_ctx)->ui2_elem_num,_ui2_idx) 

#define _LB_GET_ELEM_STATE_P(_pt_ctx,_ui2_idx)                                      \
    lb_get_elem_state_addr(&((_pt_ctx)->t_elem_inf_list),(_pt_ctx)->ui2_elem_num,_ui2_idx) 

#define _LB_SET_ELEM_STATE(_pt_ctx,_ui2_idx,_ui1_state)                   \
    lb_set_elem_state(&((_pt_ctx)->t_elem_inf_list),(_pt_ctx)->ui2_elem_num,_ui2_idx,_ui1_state,TRUE) 

#define _LB_CLR_ELEM_STATE(_pt_ctx,_ui2_idx,_ui1_state)                   \
    lb_set_elem_state(&((_pt_ctx)->t_elem_inf_list),(_pt_ctx)->ui2_elem_num,_ui2_idx,_ui1_state,FALSE)

#define _LB_CHK_ELEM_STATE(_pt_ctx,_ui2_idx,_ui1_state)                   \
    lb_chk_elem_state(&((_pt_ctx)->t_elem_inf_list),(_pt_ctx)->ui2_elem_num,_ui2_idx,_ui1_state)

#define _LB_SET_ELEMS_STATE_INF(_pt_ctx,_pui1_state, _ui2_start, _ui2_cnt)             \
    lb_do_elems_state_inf(&((_pt_ctx)->t_elem_inf_list),(_pt_ctx)->ui2_elem_num,\
                          _pui1_state, _ui2_start, _ui2_cnt, TRUE)

#define _LB_GET_ELEMS_STATE_INF(_pt_ctx, _pui1_state,_ui2_start, _ui2_cnt)             \
    lb_do_elems_state_inf(&((_pt_ctx)->t_elem_inf_list),(_pt_ctx)->ui2_elem_num,\
                          _pui1_state, _ui2_start, _ui2_cnt, FALSE)

#define _LB_RST_ELEMS_STATE(_pt_ctx,_ui2_start,_ui2_cnt)             \
    lb_reset_elems_state(&((_pt_ctx)->t_elem_inf_list),(_pt_ctx)->ui2_elem_num,_ui2_start,_ui2_cnt)

#define _LB_GET_ELEM_SPC_ST_IDX(_pt_ctx,_ui1_state,_ui2_cnt,_pui2_idx) \
    lb_get_elem_special_state(&((_pt_ctx)->t_elem_inf_list),(_pt_ctx)->ui2_elem_num, \
                              _ui1_state,_ui2_cnt,_pui2_idx)

#define _LB_FIND_ELEM_ST_INF(_pt_ctx,_ui1_state,_ui2_start,_ui2_cnt,_b_fwd,_pui2_idx,_ppui1_state) \
    lb_find_elem_special_state(&((_pt_ctx)->t_elem_inf_list),(_pt_ctx)->ui2_elem_num, \
                                 _ui1_state,_ui2_start,_ui2_cnt,_b_fwd,TRUE,_pui2_idx,_ppui1_state)
    
#define _LB_FIND_NO_ELEM_ST_INF(_pt_ctx,_ui1_state,_ui2_start,_ui2_cnt,_b_fwd,_pui2_idx,_ppui1_state) \
    lb_find_elem_special_state(&((_pt_ctx)->t_elem_inf_list),(_pt_ctx)->ui2_elem_num, \
                                 _ui1_state,_ui2_start,_ui2_cnt,_b_fwd,FALSE,_pui2_idx,_ppui1_state)

#define HAS_EVN_HANDLER_MASK(_pt_ctx, _evn_handler_mask)            \
    ((_pt_ctx)->t_evn_hdlr_info.ui4_evn_hdlr_mask & (_evn_handler_mask))

#ifdef WGL_ANIM_SUPPORT
#define _LB_ANIM_EN(_pt_ctx)                                      \
(  !HAS_FLAG((_pt_ctx)->ui4_anim_flag,WGL_LB_ANIM_FLAG_DISABLE) && \
    (HAS_STYLE(_pt_ctx,WGL_STL_LB_ANIM_BY_ACT_MAP) || x_wgl_lb_default_anim_en()) && \
     HAS_FLAG((_pt_ctx)->ui1_state, LBST_ANIM_CREATED))

#define _LB_ANIM_CHECK(_pt_ctx,_b_vp_srcl,_ui4_act_flg)                          \
(   !(!HAS_FLAG(_ui4_act_flg,WGL_LB_ACTMAP_SWITCH_PAGE_ANIM) &&    \
       HAS_STYLE(_pt_ctx,WGL_STL_LB_NO_SCROLL) &&          \
       (_b_vp_srcl == TRUE))  )
#endif

#define _LB_SET_MAX_ELEMS_NUM(_pt_ctx, _ui2_total_num)                     \
    ((_pt_ctx)->ui2_max_elem_num =  _ui2_total_num)

#define _LB_GET_MAX_ELEMS_NUM(_pt_ctx)                                      \
    ((_pt_ctx)->ui2_max_elem_num)

#define _LB_ELEMS_EMPTY(_pt_ctx)                                      \
    ((_pt_ctx)->ui2_elem_num == 0)

#define _LB_SET_ELEMS_NUM(_pt_ctx, _ui2_total_num)                     \
    ((_pt_ctx)->ui2_elem_num =  _ui2_total_num)

#define _LB_GET_ELEMS_NUM(_pt_ctx)                                      \
    ((_pt_ctx)->ui2_elem_num)

#define _LB_SET_ELEM_CACHE_NUM(_pt_ctx, _ui2_num)                       \
    ((_pt_ctx)->ui2_cache_num =  _ui2_num)

#define _LB_GET_ELEM_CACHE_NUM(_pt_ctx)                                 \
    ((_pt_ctx)->ui2_cache_num)

#define _LB_INC_ELEM_CACHE_NUM(_pt_ctx)                                 \
    ((_pt_ctx)->ui2_cache_num++)

#define _LB_DEC_ELEM_CACHE_NUM(_pt_ctx)                                 \
    ((_pt_ctx)->ui2_cache_num--)

#define _LB_SET_CACHE_START(_pt_ctx, _ui2_start)                       \
    ((_pt_ctx)->ui2_cache_start =  _ui2_start)

#define _LB_GET_CACHE_START(_pt_ctx)                                 \
    ((_pt_ctx)->ui2_cache_start)

#define _LB_INC_CACHE_START(_pt_ctx)                                 \
    ((_pt_ctx)->ui2_cache_start++)

#define _LB_DEC_CACHE_START(_pt_ctx)                                 \
    ((_pt_ctx)->ui2_cache_start--)
/*-----------------------------------------------------------------------------
                            constants
----------------------------------------------------------------------------*/
/* constants */
static const WGL_LB_KEY_MAP_T LB_DEF_KEY_MAP = 
{
    BTN_CURSOR_DOWN,
    BTN_CURSOR_UP,
    BTN_SELECT,
    0,
    0
};    

static const WGL_LB_ITEM_IMG_SUIT_T NULL_LB_IMG_SUIT = 
{
    NULL_HANDLE,
    NULL_HANDLE,
    NULL_HANDLE,
    NULL_HANDLE,
    NULL_HANDLE,
    NULL_HANDLE,
    NULL_HANDLE,
    NULL_HANDLE,
    NULL_HANDLE,
    NULL_HANDLE
};


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
typedef struct _LB_SEL_NOTIFY_CTX_T
{
    UINT16      ui2_idx_prev_sel;
    UINT16      ui2_idx_new_sel;
} LB_SEL_NOTIFY_CTX_T;


/*-----------------------------------------------------------------------------
                        internal functions
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_cmd_proc(  
    IN      HANDLE_T                    h_lb,
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT32                      ui4_cmd,
    IN OUT  VOID                        *pv_param1,
    IN OUT  VOID                        *pv_param2);
static INT32 _lb_tempunlock_insert_elem(   
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      UINT16                      ui2_idx, 
    IN      WGL_LB_ITEM_T               *at_items, 
    IN      BOOL                        b_is_append);
static INT32 _lb_tempunlock_del_elem( 
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      UINT16                      ui2_idx);
static INT32 _lb_clear(
    IN OUT  LB_CTX_T                    *pt_ctx);

#ifdef WGL_ANIM_SUPPORT
static INT32 _lb_tempunlock_highlight_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_idx_new,
    IN      LB_ELEM_T                   *pt_elem_new,
    IN      BOOL                        b_not_do_nfy,
    IN      WGL_ANIM_ACTION_TYPE_T      e_action_type);
#else
static INT32 _lb_tempunlock_highlight_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_idx_new,
    IN      LB_ELEM_T                   *pt_elem_new,
    IN      BOOL                        b_not_do_nfy);
#endif

static INT32 _lb_tempunlock_unhighlight_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_idx_target,
    IN      BOOL                        b_not_do_nfy);
static INT32 _lb_tempunlock_sel_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      BOOL                        b_raise_notify,
    IN      UINT16                      ui2_idx, 
    OUT     LB_SEL_NOTIFY_CTX_T         *pt_sel_notify_out);
static INT32 _lb_tempunlock_desel_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      BOOL                        b_raise_notify,
    IN      UINT16                      ui2_idx_target);
static INT32 _lb_lose_hlt_elem(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    OUT     BOOL                        *pb_need_notify_hlt, 
    OUT     UINT16                      *pui2_idx_new_hlt);
static INT32 _lb_tempunlock_disable_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_idx);
static INT32 _lb_tempunlock_enable_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_idx);
static INT32 _lb_get_elem(
    IN      LB_CTX_T                    *pt_ctx, 
    IN      UINT16                      ui2_idx, 
    OUT     LB_ELEM_T                   **ppt_elem,
    OUT     LB_ELEM_INF_T               **ppt_elem_inf);
static INT32 _lb_tempunlock_on_elem_pushed(
    IN OUT  LB_CTX_T                    **ppt_ctx);
static INT32 _lb_tempunlock_on_elem_unpushed(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      BOOL                        b_auto_repaint);
static INT32 _lb_tempunlock_on_elem_next_wrap_over(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint);
static INT32 _lb_tempunlock_on_elem_next(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint);
static INT32 _lb_tempunlock_on_elem_prev_no_hlt(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint);
static INT32 _lb_tempunlock_on_elem_prev_wrap_over(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint);
static INT32 _lb_tempunlock_on_elem_prev(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint);
static INT32 _lb_tempunlock_on_page_up(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint);
static INT32 _lb_tempunlock_on_page_down(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint);
static INT32 _lb_tempunlock_on_page_up_prev_page(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      UINT16                      ui2_vi_first,
    IN      UINT16                      ui2_vi_last,
    IN      UINT16                      ui2_num_vp_elem,
    OUT     BOOL                        *pb_need_repaint);
static INT32 _lb_tempunlock_on_page_down_next_page(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      UINT16                      ui2_vi_first,
    IN      UINT16                      ui2_vi_last,
    IN      UINT16                      ui2_num_vp_elem,
    OUT     BOOL                        *pb_need_repaint);
static INT32 _lb_tempunlock_on_page_change_show_first_page(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      UINT16                      ui2_vi_first,
    IN      UINT16                      ui2_num_vp_elem,
    IN      BOOL                        b_search_hlt_from_top,
    OUT     BOOL                        *pb_need_repaint);
static INT32 _lb_tempunlock_on_page_change_show_last_page(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_search_hlt_from_top,
    OUT     BOOL                        *pb_need_repaint);
static VOID _lb_search_highlightable_elem_backward(
    IN      LB_CTX_T                    *pt_ctx,
    IN      UINT16                      ui2_start,
    IN      UINT16                      ui2_stop,
    OUT     UINT16                      *pui2_new_hlt);
static VOID _lb_search_highlightable_elem_forward(
    IN      LB_CTX_T                    *pt_ctx,
    IN      UINT16                      ui2_start,
    IN      UINT16                      ui2_stop,
    OUT     UINT16                      *pui2_new_hlt );
static INT32 _lb_on_paint(
    IN      LB_CTX_T                    *pt_ctx, 
    IN      GL_HGRAPHICS_T              h_g, 
    IN      UINT16                      ui2_state);
static INT32 _lb_update_screen_rect(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN      BOOL                        b_size_fixed);
static INT32 _lb_set_inset(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN      WGL_INSET_T                 *pt_inset);
static INT32 _lb_set_is_bdr_draw_interior(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN      BOOL                        b_is_bdr_draw_interior);
static VOID _lb_tool_safe_w2s_strcpy(
    IN      LB_CTX_T                    *pt_ctx,
    OUT     UTF16_T                     *pw2s_dst, 
    IN      const UTF16_T               *pw2s_src,
    IN      UINT32                      ui4_max_len);
static INT32 _lb_send_notify_hlt(
    IN      HANDLE_T                    h_parent, 
    IN      HANDLE_T                    h_lb, 
    IN      UINT16                      ui2_idx, 
    IN      BOOL                        b_is_hlt);
static INT32 _lb_send_notify_sel(
    IN      HANDLE_T                    h_parent, 
    IN      HANDLE_T                    h_lb, 
    IN      UINT16                      ui2_idx, 
    IN      BOOL                        b_is_sel);
static INT32 _lb_send_notify_pushed(
    IN      HANDLE_T                    h_parent, 
    IN      HANDLE_T                    h_lb, 
    IN      UINT16                      ui2_idx, 
    IN      BOOL                        b_is_pushed);
static INT32 _lb_tempunlock_send_notify_reach_boundary(
    IN      LB_CTX_T                    **ppt_ctx, 
    IN      BOOL                        b_is_top);
static INT32 _lb_check_elem_visibility_after_user_key(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN      UINT16                      ui2_idx,
    IN      BOOL                        b_ensure_elem_visible,
    OUT     BOOL                        *pb_is_vp_scrolled);
static BOOL _lb_is_valid_image_handle(
    IN      HANDLE_T                    h_img);
static BOOL _lb_is_valid_image_suit(
    IN      WGL_LB_ITEM_IMG_SUIT_T      *pt_img_suit);
static BOOL _lb_is_valid_image_info(
    IN      WGL_IMG_INFO_T              *pt_img_info);
static INT32 _lb_req_update_by_hlt_nxt_prv_elem (
    IN OUT  LB_CTX_T*                   pt_ctx,
    IN      BOOL                        b_is_vp_scrolled,
    IN      UINT16                      ui2_hlt_old,
    IN      UINT16                      ui2_hlt_new);
static INT32 _lb_req_update_by_range_elems (
    IN OUT  LB_CTX_T*                   pt_ctx,
    IN      BOOL                        b_is_vp_scrolled,
    IN      UINT16                      ui2_pui_first,
    IN      UINT16                      ui2_pui_last);
static INT32 _lb_tempunlock_repaint_partial_by_item(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_idx,
    IN      UINT8                       ui1_c);
static INT32 _lb_tempunlock_repaint_full (
    IN OUT  LB_CTX_T                    **ppt_ctx);    
static INT32 _lb_tempunlock_repaint_partial_generic(
    IN OUT  LB_CTX_T                    **ppt_ctx);
static INT32 _lb_tempunlock_repaint_region(
    IN      LB_CTX_T                    **ppt_ctx, 
    IN      UINT32                      ui4_tk_start_pu,
    IN      LB_CNT_PU_REGION_T          *pt_pur);
static INT32 _lb_set_elem_data(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN OUT  LB_ELEM_T                   *pt_elem, 
    IN      WGL_LB_ELEM_DATA_T          *pt_elem_data);
static INT32 _lb_get_elem_data(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN      UINT16                      ui2_idx, 
    OUT     WGL_LB_ELEM_DATA_T          *pt_elem_data_out);
static VOID _lb_clear_elem_data(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN      LB_ELEM_T                   *pt_elem);
static INT32 _lb_tempunlock_set_elem_num(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_num_elem);
static INT32 _lb_restore_all_elem_states(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN      UINT8                       *aui1_states);
static VOID _lb_reset_all_elems_states(
    IN OUT  LB_CTX_T                    *pt_ctx);
static INT32 _lb_request_timers_from_cnt(
    IN OUT  LB_CTX_T                    *pt_ctx);
static INT32 _lb_msg_hdlr_on_timer(
    IN      HANDLE_T                    h_lb,
    IN      HANDLE_T                    h_timer,
    IN      VOID*                       pv_tag);
static INT32 _lb_timer_hdr_cnt(
    IN      LB_CTX_T*                   pt_ctx,
    IN      UINT16                      ui2_tr_id);
static BOOL _lb_debug_check_consistency(
    IN      LB_CTX_T                    *pt_ctx);
    
#ifdef WGL_ANIM_SUPPORT
static INT32 _lb_hlt_element_after_anim(
    IN OUT  LB_CTX_T                    *pt_ctx);

static INT32 _lb_begin_animation(
    LB_CTX_T*                       pt_ctx,
    WGL_ANIM_ACTION_TYPE_T          e_action_type);

static INT32 _lb_elem_begin_anim(
    LB_CTX_T**                      ppt_ctx,
    WGL_ANIM_ACTION_TYPE_T          e_action_type);

static INT32 _lb_elem_set_anim_info(
    LB_CTX_T*                       pt_ctx,
    UINT16                          ui2_anim,
    UINT16                          ui2_hlt,
    BOOL*                           pb_do_anim);

static INT32 _lb_create_anim(
    LB_CTX_T*                           pt_ctx,
    BOOL                                b_by_style,
    UINT32                              ui4_style);

static INT32 _lb_destroy_anim(
    LB_CTX_T*                           pt_ctx);


static UINT32 _lb_get_actmap_flag(
    LB_CTX_T*                       pt_ctx,
    WGL_ANIM_ACTION_TYPE_T          e_action_type);
#endif

static INT32 _lb_link_elem_inf(
    LB_CTX_T*                           pt_ctx,
    LB_ELEM_T*                          pt_elem,
    UINT16                              ui2_idx,
    BOOL                                b_init);

static INT32 _lb_chk_items_data(
    IN      LB_CTX_T*                   pt_ctx,
    IN      WGL_LB_ITEM_T               *at_items);

static INT32 _lb_alloc_elem_data_by_addr(
    LB_COL_T*                            at_cols,
    UINT8                                ui1_col_num,
    UINT32                               ui4_cc_size_txt_pool,
    UINT8                                ui1_img_suit_col_num,
    LB_ELEM_T*                           pt_elem);

static INT32 _lb_alloc_elem_data(
    LB_COL_T*                            at_cols,
    UINT8                                ui1_col_num,
    UINT32                               ui4_cc_size_txt_pool,
    UINT8                                ui1_img_suit_col_num,
    UINT32                               ui4_cb_size_elem,
    LB_ELEM_T**                          ppt_elem);

static INT32 _lb_init_elem_data(
    IN      LB_CTX_T*                   pt_ctx,
    IN      WGL_LB_ITEM_T*              at_items, 
    IN OUT  LB_ELEM_T*                  pt_elem);


static INT32 _lb_insert_elem_data(
    IN      LB_CTX_T*                   pt_ctx,
    IN      UINT16                      ui2_idx, 
    IN OUT  LB_ELEM_T*                  pt_elem,
    IN      BOOL                        b_is_append);

static INT32 _lb_check_cache_inf(LB_CTX_T* pt_ctx);

static INT32 _lb_init_col_inf(
    LB_COL_T*                            pt_cols,
    UINT8                                ui1_col_num,
    WGL_LB_COL_INIT_T*                   pt_init_cols);

static INT32 _lb_create_elem_data(
    LB_CTX_T*                          pt_ctx,
    WGL_LB_ITEM_T*                     at_items,
    UINT16                             ui2_idx,
    LB_ELEM_T**                        ppt_elemt,
    BOOL                               b_append);

static INT32 _lb_check_cache_for_vp(LB_CTX_T* pt_ctx);


static INT32 _lb_destroy_elem_data(
    IN      LB_CTX_T*                   pt_ctx,
    IN      UINT16                      ui2_idx,
    IN      LB_ELEM_T**                 ppt_elemt);


static VOID _lb_get_update_range(
        UINT16                       ui2_src_start,
        UINT16                       ui2_src_end,
        UINT16                       ui2_dst_start,
        UINT16                       ui2_dst_end,
        BOOL                         b_include,
        LB_RANGE_INF_T*              pt_upd_range,
        UINT16*                      pui2_count);


/*-----------------------------------------------------------------------------
                        exported functions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: x_wgl_listbox_init
 *
 * Description: This API is called when system initialization to register 
 *              the listbox widget to component repository.
 *
 * Inputs:  ui4_init_type  Specifies the init parameter.
 *
 * Outputs: -
 *
 * Returns: WGLR_OK             Routine successful.
 *          WGLR_INV_ARG        Invalid argument or the widget has been registered.
 *          WGLR_OUT_OF_MEMORY  Out of memory.
 ----------------------------------------------------------------------------*/
INT32 x_wgl_listbox_init(
    UINT32                              ui4_init_type)
{
    UINT32                      ui4_wgl_type = HT_WGL_WIDGET_LIST_BOX;
    WGL_WIDGET_CTRL_INTF_T      t_wgl_intf = { wgl_lb_inst_create, 
                                               wgl_lb_inst_destroy, 
                                               wgl_lb_default_proc,
                                               WGL_ATTR_FOCUSABLE};
    UINT32                      ui4_cnt_type = WGL_CONTENT_LIST_BOX_DEF;
    WGL_CONTENT_CTRL_INTF_T     t_cnt_intf = { wgl_lb_cnt_def_inst_create,
                                               wgl_lb_cnt_def_inst_destroy,
                                               wgl_lb_cnt_def_cmd_proc,
                                               wgl_lb_cnt_def_paint}; 
    INT32                       i4_ret;
    
    if(WGL_LISTBOX_INIT_CTRL_LOGIC & ui4_init_type)
    {
        i4_ret = wgl_register_comp_intf(ui4_wgl_type, &t_wgl_intf);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    if(WGL_LISTBOX_INIT_DEFAULT_CNT & ui4_init_type)
    {
        i4_ret = wgl_register_content_intf(ui4_cnt_type, &t_cnt_intf);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    
    CATCH_FAIL(wgl_lb_cli_init());
    
    return WGLR_OK;
}
 
 
/*-----------------------------------------------------------------------------
                        public functions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: wgl_lb_inst_create
 *
 * Description: This API creates and init an instance of listbox widget.
 *
 * Inputs:  pt_wgt_create   Specifies the init parameters for the new instance. 
 *
 * Outputs: ppv_ctx_wgt  Contains the pointer to the new instance.
 *
 * Returns: WGLR_OK             Routine successful.
 *          WGLR_INV_ARG        Invalid arguments.
 *          WGLR_OUT_OF_MEMORY  Out of memory.
 *          WGLR_INTERNAL_ERROR Internal error.
 *          WGLR_INV_TYPE       Invalid content type or border type.
 ----------------------------------------------------------------------------*/
INT32 wgl_lb_inst_create( 
    WGL_WIDGET_CONTEXT_HANDLE_T         *ppv_ctx_wgt,
    WGL_WIDGET_CREATE_INPUT_T           *pt_wgt_create )
{
    INT32                       i4_ret;
    WGL_LB_INIT_T               *pt_init = (WGL_LB_INIT_T *)pt_wgt_create->pv_param;
    LB_CTX_T                    *pt_ctx = NULL;
    UINT8                       ui1_c;
    WGL_INSET_T                 t_inset;
    BOOL                        b_is_bdr_draw_interior;
    WGL_CONTENT_CREATE_INPUT_T  t_create_cnt;
    LB_CNT_INIT_T               t_init_lb_cnt;
    WGL_CONTENT_CTRL_INTF_T     t_intf_cnt;
    UINT32                      ui4_size_cnt_elem;
    UINT32                      ui4_cc_size_txt_pool;
    UINT8                       ui1_img_suit_col_num;
    UINT32                      ui4_cb_size_elem;
    WGL_CANVAS_STATUS_T         t_cvsst;
    UINT32                      ui4_size;
    
    *ppv_ctx_wgt = NULL;
    
    /*check input data*/    
    if(pt_init->ui4_style & WGL_STL_LB_LBW &&
       (pt_init->ui4_style & WGL_STL_LB_FIXED_ELEM_NUM))
    {
        return WGLR_INV_ARG;
    }

    if((pt_init->ui4_style & (~(WGL_STL_LB_LAST-1))) != 0)
    {
        return WGLR_INV_ARG;
    }
    
    if( pt_init->ui2_max_elem_num > WGL_LB_MAX_ELEM_NUM )
    {
        return WGLR_INV_ARG;
    }
    
    if( (pt_init->ui4_style & WGL_STL_LB_FIXED_ELEM_NUM) 
            && pt_init->ui2_max_elem_num==0 )
    {
        return WGLR_INV_ARG;
    }

    if( (pt_init->ui4_style & WGL_STL_LB_AUTO_SEL) 
        && (pt_init->ui4_style & WGL_STL_LB_MULTI_SEL) )
    {
        return WGLR_INV_ARG;
    }

    if( pt_init->at_cols == NULL                    ||
        pt_init->ui1_col_num == 0                   ||
        pt_init->ui2_elem_size == 0                 ||
        pt_init->ui2_elem_size>WGL_LB_MAX_ELEM_SIZE ||
        pt_init->ui2_elem_distance>WGL_LB_MAX_ELEM_DIST )
    {
        return WGLR_INV_ARG;
    }
    
    /*check input column formats*/
    for(ui1_c=0; ui1_c<pt_init->ui1_col_num; ui1_c++)
    {
        if(!CHECK_VALID_COL_WIDTH(pt_init->at_cols[ui1_c].ui2_width))
        {
            return WGLR_INV_ARG;
        }
        
        if( pt_init->at_cols[ui1_c].e_col_type==LB_COL_TYPE_TEXT &&
            pt_init->at_cols[ui1_c].ui2_max_text_len==0)
        {
            return WGLR_INV_ARG;
        }

        if( !(pt_init->at_cols[ui1_c].e_col_type==LB_COL_TYPE_IMG      ||
              pt_init->at_cols[ui1_c].e_col_type==LB_COL_TYPE_IMG_SUIT ||
              pt_init->at_cols[ui1_c].e_col_type==LB_COL_TYPE_TEXT ) ) 
        {
            return WGLR_INV_ARG;
        }

        if(!WGL_IS_VALID_ALIGN(pt_init->at_cols[ui1_c].ui1_align) )
        {
            return WGLR_INV_ARG;
        }
    }


    /*check the validity of content type id */
    if( !WGL_IS_CONTENT_BELONG_WIDGET(pt_wgt_create->ui4_type_content, HT_WGL_WIDGET_LIST_BOX)  )
    {
        return WGLR_INV_ARG;
    }
    if( !IS_VALID_BORDER_TYPE(pt_wgt_create->ui4_type_border) )
    {
        return WGLR_INV_ARG;
    }

    /*initialize canvas status*/
    wgl_cvsst_update(&t_cvsst, &pt_wgt_create->t_cvs_info, pt_wgt_create->h_palette);

    /*query the control interface of the content*/
    CATCH_FAIL(wgl_query_content_intf(
        pt_wgt_create->ui4_type_content, 
        &t_intf_cnt));
    
    /*Query the Content's Element Size*/
    CATCH_FAIL( t_intf_cnt.pf_content_cmd_proc(
        NULL, 
        WGL_CNT_LB_GET_ELEM_CNT_CTX_SIZE, 
        WGL_PACK(pt_init->ui1_col_num), 
        WGL_PACK(&ui4_size_cnt_elem)) );
    
    /*Calculate the size (in Byte-Count) of an Element*/
    /*LB_ELEM_T + LB_ITEM_T[item_num] + ps_txt_1[max_txt_len_1]+...+ps_txt_k[max_txt_len_k] + pt_img_suit_1+...+pt_img_suit_m */
    ui4_cc_size_txt_pool = 0;
    ui1_img_suit_col_num = 0;
    for(ui1_c=0; ui1_c<pt_init->ui1_col_num; ui1_c++ )
    {
        if(pt_init->at_cols[ui1_c].e_col_type==LB_COL_TYPE_TEXT )
        {
            ui4_cc_size_txt_pool += WGL_ALIGN_4(pt_init->at_cols[ui1_c].ui2_max_text_len+1)*2;
        }
        else if(pt_init->at_cols[ui1_c].e_col_type==LB_COL_TYPE_IMG_SUIT)
        {
            ui1_img_suit_col_num++;
        }
    }

    ui4_cb_size_elem = WGL_ALIGN_4(sizeof(LB_ELEM_T)) +
                           WGL_ALIGN_4(sizeof(LB_ITEM_T)*pt_init->ui1_col_num) + 
                           UTF16_MAX_LEN * ui4_cc_size_txt_pool +
                           WGL_ALIGN_4(sizeof(WGL_LB_ITEM_IMG_SUIT_T)) * ui1_img_suit_col_num +
                           WGL_ALIGN_4( ui4_size_cnt_elem );
    

    /*init LB_CTX_T*/
    if( pt_init->ui4_style & WGL_STL_LB_FIXED_ELEM_NUM )
    {
        /*if in fixed-element-num mode, allocates all elements at once*/
        UINT16          ui2_e;
        BYTE            *pb_next_elem;
        LB_ELEM_T       *pt_elem=NULL;
        LB_ELEM_T       *pt_prev_elem;

        ui4_size = WGL_ALIGN_4(sizeof(LB_CTX_T))
                                + WGL_ALIGN_4(sizeof(LB_COL_T)*pt_init->ui1_col_num)
                                + ui4_cb_size_elem * pt_init->ui2_max_elem_num;
        pt_ctx = WGL_MEM_ALLOC( ui4_size );
        if(pt_ctx==NULL)
        {
            INFORM_ERR(WGLR_OUT_OF_MEMORY);
            return WGLR_OUT_OF_MEMORY;
        }

        x_memset(pt_ctx, 0, ui4_size);
        _LB_SET_ELEMS_NUM(pt_ctx, pt_init->ui2_max_elem_num);
        
        /* init element information poll resource */
        i4_ret = wgl_npm_init (&pt_ctx->t_npm_elem_inf, 
                                        sizeof (LB_ELEM_INF_T), 
                                        pt_init->ui2_max_elem_num, 
                                        LB_ELEM_INF_POOL_EXP_NUM);
        if(WGL_IS_FAIL(i4_ret))
        {
            INFORM_ERR(i4_ret);
            WGL_MEM_FREE(pt_ctx);
            return i4_ret;
        }
        
        /*initial the element list information*/
        _LB_INIT_ELEM_INF(pt_ctx, pt_init->ui2_max_elem_num);
        if(WGL_IS_FAIL(i4_ret))
        {
            INFORM_ERR(i4_ret);
            WGL_MEM_FREE(pt_ctx);
            _LB_DEINIT_ELEM_INF(pt_ctx);
            return i4_ret;
        }

        pt_ctx->at_cols  = (LB_COL_T *)( ((BYTE *)pt_ctx) + WGL_ALIGN_4(sizeof(LB_CTX_T)) );
        pb_next_elem     = ((BYTE *)pt_ctx->at_cols) + WGL_ALIGN_4(sizeof(LB_COL_T)*pt_init->ui1_col_num);
        pt_ctx->pt_elems = (LB_ELEM_T *)pb_next_elem;
        pt_ctx->pt_elems_free_list = NULL;
        pt_prev_elem     = NULL;

        /*init LB_CTX_T->at_cols[]*/
        i4_ret = _lb_init_col_inf(pt_ctx->at_cols, pt_init->ui1_col_num, pt_init->at_cols);
        if(i4_ret != WGLR_OK)
        {
            WS_ASSERT(0);
            WGL_MEM_FREE(pt_ctx);
        }

        for(ui2_e=0; ui2_e<pt_init->ui2_max_elem_num; ui2_e++)
        {
            /*init the element*/
            pt_elem          = (LB_ELEM_T *)pb_next_elem;
            pt_elem->pt_prev = pt_prev_elem;
            if( pt_elem->pt_prev )
            {
                pt_elem->pt_prev->pt_next = pt_elem;
            }
            _lb_alloc_elem_data_by_addr(pt_ctx->at_cols,
                                        pt_init->ui1_col_num,
                                        ui4_cc_size_txt_pool,
                                        ui1_img_suit_col_num,
                                        pt_elem);

            _lb_link_elem_inf(pt_ctx, pt_elem, ui2_e, FALSE);

            for( ui1_c=0; ui1_c<pt_init->ui1_col_num; ui1_c++ )
            {
                switch(pt_init->at_cols[ui1_c].e_col_type) 
                {
                    case LB_COL_TYPE_TEXT:
                    {
                        x_uc_w2s_set(pt_elem->at_items[ui1_c].t_text.pw2s_text, 0, (UTF16_T)0 );
                        x_uc_w2s_set(pt_elem->at_items[ui1_c].t_text.pw2s_raw_text, 0, (UTF16_T)0 );
                    }
                    break;
                    case LB_COL_TYPE_IMG:
                        pt_elem->at_items[ui1_c].h_img = NULL_HANDLE;
                    break;
                    case LB_COL_TYPE_IMG_SUIT:
                        _IMG_SUIT_COPY(pt_elem->at_items[ui1_c].pt_img_suit, &NULL_LB_IMG_SUIT);
                    break;
                }
            }

            pb_next_elem += ui4_cb_size_elem;
            pt_prev_elem  = pt_elem;
        }

        if(pt_elem==NULL)
        {
            WGL_MEM_FREE(pt_ctx);
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            return WGLR_INTERNAL_ERROR;
        }
        pt_elem->pt_next = NULL;

        /*init LB_CTX_T */    
        pt_ctx->ui4_style = pt_init->ui4_style;
        pt_ctx->h_parent  = NULL_HANDLE;
        pt_ctx->h_lb      = NULL_HANDLE;
        pt_ctx->pv_attach = NULL;
        if( !(pt_ctx->ui4_style & WGL_STL_LB_FIXED_ELEM_NUM) && pt_init->ui2_max_elem_num==0 )
        {
            pt_ctx->ui2_max_elem_num = WGL_LB_MAX_ELEM_NUM;
        }
        else
        {
            pt_ctx->ui2_max_elem_num = pt_init->ui2_max_elem_num;
        }
        x_memcpy(&pt_ctx->t_keymap, &LB_DEF_KEY_MAP, sizeof(WGL_LB_KEY_MAP_T));
        pt_ctx->ui2_margin_hlt = 0;
        pt_ctx->t_evn_hdlr_info.ui4_evn_hdlr_mask = 0;
        pt_ctx->t_evn_hdlr_info.pv_tag_evn_hdlr = NULL;
        pt_ctx->t_evn_hdlr_info.pf_event_hdlr = NULL;
        pt_ctx->ui1_col_num    = pt_init->ui1_col_num;
        pt_ctx->ui1_state      = 0;
        _LB_SET_MAX_ELEMS_NUM(pt_ctx, pt_ctx->ui2_max_elem_num);
        _LB_SET_ELEMS_NUM(pt_ctx, pt_ctx->ui2_max_elem_num);
        _LB_SET_CACHE_START(pt_ctx, 0);
        _LB_SET_ELEM_CACHE_NUM(pt_ctx, pt_ctx->ui2_max_elem_num);
        pt_ctx->ui2_sel_num    = 0;
        pt_ctx->t_arab_reorder_attr.b_find_init_level = TRUE;
        pt_ctx->t_arab_reorder_attr.ui1_embedding_level = 0;
        
        /*check force highlighting*/
        if(HAS_STYLE(pt_ctx, WGL_STL_LB_FORCE_HLT))
        {
            /*By default, force the first element to be automatically highlighted*/
            pt_ctx->ui2_idx = 0;
            _LB_SET_ELEM_STATE(pt_ctx, pt_ctx->ui2_idx, LBEST_HLT);
            if(HAS_STYLE(pt_ctx, WGL_STL_LB_AUTO_SEL))
            {
                pt_ctx->ui2_sel_num = 1;
                _LB_SET_ELEM_STATE(pt_ctx, pt_ctx->ui2_idx, LBEST_SEL);
            }
        }
        else
        {
            pt_ctx->ui2_idx = WGL_LB_NULL_INDEX;
        }

        /*check force selecting*/
        if(HAS_STYLE(pt_ctx, WGL_STL_LB_FORCE_SEL) && pt_ctx->ui2_sel_num==0)
        {
            /*By default, force the first element to be automatically selected*/
            pt_ctx->ui2_sel_num = 1;
            _LB_SET_ELEM_STATE(pt_ctx, 0, LBEST_SEL);
        }
        
    }
    else
    {
        ui4_size =  WGL_ALIGN_4(sizeof(LB_CTX_T)) + sizeof(LB_COL_T)*pt_init->ui1_col_num;
        if( NULL==(pt_ctx=WGL_MEM_ALLOC( ui4_size )) )
        {
            return WGLR_OUT_OF_MEMORY;
        }
        
        x_memset(pt_ctx, 0, ui4_size);
        /* init element information poll resource */
        i4_ret = wgl_npm_init (&pt_ctx->t_npm_elem_inf, 
                                        sizeof (LB_ELEM_INF_T), 
                                        pt_init->ui2_max_elem_num, 
                                        LB_ELEM_INF_POOL_EXP_NUM);
        if(WGL_IS_FAIL(i4_ret))
        {
            INFORM_ERR(i4_ret);
            WGL_MEM_FREE(pt_ctx);
            return i4_ret;
        }

        /*initial the element list information*/
        _LB_INIT_ELEM_INF(pt_ctx, 0);
        if(WGL_IS_FAIL(i4_ret))
        {
            INFORM_ERR(i4_ret);
            WGL_MEM_FREE(pt_ctx);
            _LB_DEINIT_ELEM_INF(pt_ctx);
            return i4_ret;
        }

        pt_ctx->at_cols            = (LB_COL_T *)( ((BYTE *)pt_ctx) + WGL_ALIGN_4(sizeof(LB_CTX_T)) );
        pt_ctx->ui4_style          = pt_init->ui4_style;
        pt_ctx->h_parent           = NULL_HANDLE;
        pt_ctx->h_lb               = NULL_HANDLE;
        pt_ctx->pv_attach          = NULL;
        _LB_SET_MAX_ELEMS_NUM(pt_ctx, pt_init->ui2_max_elem_num);
        x_memcpy((VOID *)&pt_ctx->t_keymap, (VOID *)&LB_DEF_KEY_MAP, sizeof(WGL_LB_KEY_MAP_T));
        pt_ctx->ui2_margin_hlt     = 0;
        pt_ctx->ui1_col_num        = pt_init->ui1_col_num;
        pt_ctx->ui1_state          = 0;
        pt_ctx->ui2_idx            = WGL_LB_NULL_INDEX;
        pt_ctx->ui2_sel_num        = 0;
        _LB_SET_ELEMS_NUM(pt_ctx, 0);
        _LB_SET_ELEM_CACHE_NUM(pt_ctx, 0);
        _LB_SET_CACHE_START(pt_ctx, 0);
        pt_ctx->pt_elems           = NULL;
        pt_ctx->pt_elems_free_list = NULL;
        pt_ctx->t_evn_hdlr_info.ui4_evn_hdlr_mask = 0;
        pt_ctx->t_evn_hdlr_info.pv_tag_evn_hdlr   = NULL;
        pt_ctx->t_evn_hdlr_info.pf_event_hdlr     = NULL;
        pt_ctx->t_arab_reorder_attr.b_find_init_level = TRUE;
        pt_ctx->t_arab_reorder_attr.ui1_embedding_level = 0;
 
    }

#ifdef CLI_LVL_ALL     
    pt_ctx->ui4_mem_size = 0;
#endif
    pt_ctx->ui4_cc_size_txt_pool = ui4_cc_size_txt_pool;
    pt_ctx->ui4_cb_size_elem     = ui4_cb_size_elem;
    pt_ctx->ui1_img_suit_col_num = ui1_img_suit_col_num;
    x_memset( & pt_ctx->t_trp, 0, sizeof ( ANM_TRP_CTX_T ) );

    /*init LB_CTX_T->at_cols[]*/
    i4_ret = _lb_init_col_inf(pt_ctx->at_cols, pt_ctx->ui1_col_num, pt_init->at_cols);
    if(i4_ret != WGLR_OK)
    {
        WS_ASSERT(0);
        WGL_MEM_FREE(pt_ctx);
    }

    /*init Border*/
    pt_ctx->t_intf_bdr.pf_border_cmd_proc  = NULL;
    pt_ctx->t_intf_bdr.pf_create_instance  = NULL;
    pt_ctx->t_intf_bdr.pf_destroy_instance = NULL;
    pt_ctx->t_intf_bdr.pf_paint_border     = NULL;
    i4_ret = wgl_query_border_intf( pt_wgt_create->ui4_type_border, &pt_ctx->t_intf_bdr );
    if(WGL_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        WGL_MEM_FREE(pt_ctx);
        return i4_ret;
    }

    if( !IS_NULL_BORDER(pt_ctx) )
    {
        WGL_BORDER_CREATE_INPUT_T   t_bdr_create;
        t_bdr_create.i4_width  = RECT_W(pt_wgt_create->pt_rect);
        t_bdr_create.i4_height = RECT_H(pt_wgt_create->pt_rect);
        t_bdr_create.ui4_style = pt_init->ui4_style;
        t_bdr_create.pt_cvsst = &t_cvsst;
        i4_ret = pt_ctx->t_intf_bdr.pf_create_instance(&pt_ctx->pv_ctx_bdr, &t_bdr_create);
        if(WGL_IS_FAIL(i4_ret))
        {
            WGL_MEM_FREE(pt_ctx);
            return i4_ret;
        }
    }
    
    /*query insets of border*/
    if( !IS_NULL_BORDER(pt_ctx) )
    {
        do 
        {
            DO_BDR_CMD(pt_ctx, WGL_CMD_GL_GET_INSET, &t_inset, NULL, i4_ret);
            if(WGL_IS_FAIL(i4_ret))
            {
                break;
            }
            DO_BDR_CMD(pt_ctx, WGL_CMD_GL_GET_IS_BDR_DRAW_INTERIOR, &b_is_bdr_draw_interior, NULL, i4_ret);
        } while(FALSE);
        
        if( WGL_IS_FAIL(i4_ret) )
        {
            pt_ctx->t_intf_bdr.pf_destroy_instance(pt_ctx->pv_ctx_bdr);
            WGL_MEM_FREE(pt_ctx);
            return i4_ret;
        }
    }
    else
    {
        t_inset.i4_bottom   = t_inset.i4_left = t_inset.i4_right = t_inset.i4_top = 0;
        b_is_bdr_draw_interior = FALSE;
    }

    /*init Content*/
    x_memcpy((VOID *)&pt_ctx->t_intf_cnt, (VOID *)&t_intf_cnt, sizeof(WGL_CONTENT_CTRL_INTF_T));
    SET_RECT_BY_SIZE(&t_create_cnt.t_rect_cnt, 0, 0, RECT_W(pt_wgt_create->pt_rect), RECT_H(pt_wgt_create->pt_rect));
    _RECT_INSET(&t_create_cnt.t_rect_cnt, &t_inset);
    t_create_cnt.pt_cvsst                = &t_cvsst;    
    t_init_lb_cnt.ui4_style              = pt_ctx->ui4_style;
    t_init_lb_cnt.ui1_state              = pt_ctx->ui1_state;
    t_init_lb_cnt.ui1_col_num            = pt_ctx->ui1_col_num;
    t_init_lb_cnt.ui2_elem_num            = pt_ctx->ui2_max_elem_num;
    t_init_lb_cnt.at_cols                = pt_ctx->at_cols;
    t_init_lb_cnt.at_init_cols           = pt_init->at_cols;
    t_init_lb_cnt.ui2_elem_size          = pt_init->ui2_elem_size;
    t_init_lb_cnt.ui2_elem_dist          = pt_init->ui2_elem_distance;
    t_init_lb_cnt.b_is_bdr_draw_interior = b_is_bdr_draw_interior;
    t_create_cnt.pv_init                 = (VOID *)&t_init_lb_cnt;

    /*Create the instance of Content*/
    i4_ret = pt_ctx->t_intf_cnt.pf_create_instance(&pt_ctx->pv_ctx_cnt, &t_create_cnt);
    if( WGL_IS_FAIL(i4_ret) )
    {
        if( !IS_NULL_BORDER(pt_ctx) )
        {
            pt_ctx->t_intf_bdr.pf_destroy_instance(pt_ctx->pv_ctx_bdr);
        }
        WGL_MEM_FREE(pt_ctx);
        return i4_ret;
    }

    if( HAS_STYLE(pt_ctx, WGL_STL_LB_FIXED_ELEM_NUM) )
    {
        /*inform the Content to init each Content Element*/
        LB_ELEM_T *pt_elem_tmp = pt_ctx->pt_elems;
        UINT16     ui2_i;
        LB_ELEM_INF_CONTENT     t_content;

        t_content.pt_elem_inf_list = &(pt_ctx->t_elem_inf_list);
        t_content.pt_elems = pt_ctx->pt_elems;
        t_content.ui2_elem_num = _LB_GET_ELEMS_NUM(pt_ctx);
        t_content.ui2_cache_start = _LB_GET_CACHE_START(pt_ctx);
        t_content.ui2_cache_num = _LB_GET_ELEM_CACHE_NUM(pt_ctx);
        for(ui2_i=0; ui2_i<_LB_GET_ELEMS_NUM(pt_ctx); ui2_i++)
        {
            DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_ADD_ELEM, &t_content, pt_elem_tmp, i4_ret);
            if( WGL_IS_FAIL(i4_ret) )
            {
                INFORM_ERR(i4_ret);
                pt_ctx->t_intf_cnt.pf_destroy_instance(pt_ctx->pv_ctx_cnt);
                if( !IS_NULL_BORDER(pt_ctx) )
                {
                    pt_ctx->t_intf_bdr.pf_destroy_instance(pt_ctx->pv_ctx_bdr);
                }
                WGL_MEM_FREE(pt_ctx);
                return i4_ret;
            }
            pt_elem_tmp = pt_elem_tmp->pt_next;
        }
    }
    
#ifdef WGL_ANIM_SUPPORT
    /*Init the listbox animation information*/
    pt_ctx->ui4_anim_flag = 0;
#ifdef WGL_LB_DEFAULT_NO_ANIMATION
    WGL_SET_FLAG(pt_ctx->ui4_anim_flag,WGL_LB_ANIM_FLAG_DISABLE);
#else
    if(HAS_STYLE(pt_ctx, WGL_STL_LB_ANIM_FORCE_DISABLE))
    {
        WGL_SET_FLAG(pt_ctx->ui4_anim_flag,WGL_LB_ANIM_FLAG_DISABLE);
    }
#endif    
    x_memset(&pt_ctx->t_begin_data,0,sizeof(WGL_ANIMATION_DATA_T));
    pt_ctx->b_path_by_set = FALSE;
    pt_ctx->ui2_anim_idx = 0;
    pt_ctx->ui2_anim_hlt = 0;
    x_memset(pt_ctx->at_actmap,0,sizeof(WGL_ANIM_ACTION_MAP_T)*WGL_LB_ANIM_ACT_NUM);
    x_memset(&pt_ctx->t_anim_s,0,sizeof(GL_RECT_T));
    x_memset(&pt_ctx->t_anim_e,0,sizeof(GL_RECT_T));

    if(HAS_STYLE(pt_ctx, WGL_STL_LB_ANIM_BY_ACT_MAP))
    {
        _lb_create_anim(pt_ctx, TRUE, WGL_STL_LB_ANIM_BY_ACT_MAP);
    }
    else if(HAS_STYLE(pt_ctx, WGL_STL_LB_ANIM_BY_ACT_MAP))
    {
        _lb_create_anim(pt_ctx, TRUE, WGL_STL_LB_ANIM_BY_ACT_MAP);
    }
    else
    {
        _lb_create_anim(pt_ctx, FALSE, 0);
    }
#endif    
    *ppv_ctx_wgt = (WGL_WIDGET_CONTEXT_HANDLE_T)pt_ctx;

#ifdef CLI_LVL_ALL     
    pt_ctx->ui4_mem_size += ui4_size;
#endif  
    return WGLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: wgl_lb_inst_destroy
 *
 * Description: This API destroys the specified listbox instance.
 *
 * Inputs:  pv_ctx_wgt  Specifies the reference to the instance.
 *
 * Outputs: -
 *
 * Returns: WGLR_OK  Routine successful.
 ----------------------------------------------------------------------------*/
INT32 wgl_lb_inst_destroy(
    WGL_WIDGET_CONTEXT_HANDLE_T         pv_ctx_wgt)
{
    INT32      i4_ret;
    LB_CTX_T  *pt_ctx = (LB_CTX_T *)pv_ctx_wgt;

    /*Free the element data information*/
    if(!HAS_STYLE(pt_ctx, WGL_STL_LB_FIXED_ELEM_NUM) )
    {
        LB_ELEM_T   *pt_elem, *pt_elem_next;
        /*clear the element list*/
        INFORM_ERR(_lb_clear((LB_CTX_T *)pv_ctx_wgt));
        /*free the element free list*/
        pt_elem = pt_ctx->pt_elems_free_list;
        while(pt_elem)
        {
            pt_elem_next = pt_elem->pt_next;
            WGL_MEM_FREE(pt_elem);
            pt_elem      = pt_elem_next;
        }
    }

    /*Free the element iformation*/
    _LB_DEINIT_ELEM_INF(pt_ctx);
    _LB_SET_ELEMS_NUM(pt_ctx, 0);

    _LB_SET_CACHE_START(pt_ctx, 0);
    _LB_SET_ELEM_CACHE_NUM(pt_ctx, 0);
#ifdef WGL_ANIM_SUPPORT
    INFORM_ERR(_lb_destroy_anim(pt_ctx));
#endif
    
    /* deinit timer resource pool */
    INFORM_ERR( anm_trp_deinit( & pt_ctx->t_trp ) );

    INFORM_ERR(pt_ctx->t_intf_cnt.pf_destroy_instance(pt_ctx->pv_ctx_cnt) );
    if(!IS_NULL_BORDER(pt_ctx))
    {
        INFORM_ERR(pt_ctx->t_intf_bdr.pf_destroy_instance(pt_ctx->pv_ctx_bdr) );
    }
    
    WGL_MEM_FREE(pv_ctx_wgt);

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_lb_default_proc
 *
 * Description: This API handles all the messages related to listbox.
 *
 * Inputs:  h_lb       Specifies the handle of the listbox.
 *          ui4_msg    Specifies the message id.
 *          pv_param1  Specifies the first parameter if necessary.
 *          pv_param2  Specifies the second parameter if necessary.
 *
 * Outputs: pv_param1  Contains the first return value if necessary.
 *          pv_param2  Contains the second return value if necessary.
 *
 * Returns: WGLR_OK    Routine successful.
 *          < 0        Other errors.
 ----------------------------------------------------------------------------*/
INT32 wgl_lb_default_proc( 
    HANDLE_T                            h_lb,
    UINT32                              ui4_msg,
    VOID                                *pv_param1,
    VOID                                *pv_param2)
{
    LB_CTX_T        *pt_ctx = NULL;
    INT32           i4_ret;
    INT32           i4_ret_fnl;
    
    i4_ret_fnl = WGLR_OK;
    switch(ui4_msg)
    {
        case WGL_MSG_CREATE:
        {
            HANDLE_T    h_parent;

            CATCH_FAIL(x_wgl_get_parent(h_lb, &h_parent));
            
            GET_LOCK(h_lb, &pt_ctx);
            
            /* record the parent handle */
            pt_ctx->h_parent = h_parent;
            pt_ctx->h_lb = h_lb;
            DO_LB_CNT_CMD(pt_ctx, WGL_CMD_LB_SET_HANDLE_TO_CNT, &h_lb, NULL, i4_ret);

            /* init timer resource pool */
            i4_ret_fnl = anm_trp_init( h_lb, & pt_ctx->t_trp );
            
            if (i4_ret_fnl == WGLR_OK)
            {
                /* init the screen rect */
                i4_ret_fnl = _lb_update_screen_rect(pt_ctx, FALSE);
                INFORM_ERR (i4_ret_fnl);
            }
            else
            {
                INFORM_ERR (i4_ret_fnl);
            }
            
            REL_LOCK(h_lb, pt_ctx);
            
            return i4_ret_fnl;
        }
        
        case WGL_MSG_SIZE:
        case WGL_MSG_MOVE:
        {
            GET_LOCK (h_lb, &pt_ctx);
            
            /* update the screen rect */
            i4_ret_fnl = _lb_update_screen_rect(
                    pt_ctx,
                    ui4_msg == WGL_MSG_SIZE ? FALSE : TRUE );
                    
            INFORM_ERR (i4_ret_fnl);
            
            REL_LOCK (h_lb, pt_ctx);
            
            return i4_ret_fnl;
        }

        case WGL_MSG_TIMER:
        {
            return _lb_msg_hdlr_on_timer(
                h_lb, 
                (HANDLE_T) pv_param1, 
                (VOID *) pv_param2);
        }
        case WGL_MSG_NOTIFY:
        {
            WGL_ASSERT(0);  /*pg static should not receive this message*/
            i4_ret = WGLR_INTERNAL_ERROR;
            DBG_ERROR(("{WGL(LB)} ERR: Listbox should not receive any notification. (%d) at L%d\n\n", i4_ret, __LINE__));
        }
        break;
        case WGL_MSG_DESTROY:
        case WGL_MSG_KEY_REPEAT:
        case WGL_MSG_ACTIVATE:
        {    /*do nothing*/
            return WGLR_OK;
        }
    }
    
    GET_LOCK(h_lb, &pt_ctx);
    
    switch(ui4_msg)
    {
        /*Internal Messages*/
        case WGL_MSG_PAINT:
        {
            /*app mybe needs to decide text direction at run time,so the widget query text direction from app's callback
                         and update widget's text direction according to return value*/
            UINT8 ui1_text_dir;
            SIZE_T z_size = sizeof(UINT8);
            /*because x_wgl_query_canvas_info_callback() maybe call app's callback function,so unlock first*/
            REL_LOCK(h_lb,pt_ctx);
            i4_ret_fnl = x_wgl_query_canvas_info_callback(h_lb,WGL_CVS_CB_TXT_DIR,(VOID*)(&ui1_text_dir),&z_size);
            GET_LOCK(h_lb,&pt_ctx);
            if(i4_ret_fnl == WGLR_OK)
            {
                i4_ret_fnl = _lb_tempunlock_cmd_proc(h_lb, &pt_ctx, WGL_CMD_GL_SET_TEXT_DIRECTION,WGL_PACK(ui1_text_dir),NULL);
                if(i4_ret_fnl != WGLR_OK)
                {
                    INFORM_ERR(i4_ret_fnl);
                    break;
                }
            }
            else if(i4_ret_fnl != WGLR_FUNC_NOT_IMPL)
            {
                INFORM_ERR(i4_ret_fnl);
                break;
            }
            i4_ret_fnl = _lb_on_paint(pt_ctx, (GL_HGRAPHICS_T)pv_param1, WGL_POINTER_TO_UINT16(pv_param2));
        }
        break;
    
        case WGL_MSG_COMMAND:
        {
            WGL_PACKED_PARAM_T *pt_params = (WGL_PACKED_PARAM_T *)pv_param2;
            i4_ret_fnl = _lb_tempunlock_cmd_proc(h_lb, &pt_ctx, (UINT32)pv_param1, pt_params->pv_param1, pt_params->pv_param2 );
        }
        break;

        case WGL_MSG_KEY_DOWN:
        {
            UINT32 ui4_key = (UINT32)pv_param1;
            if( ui4_key == pt_ctx->t_keymap.ui4_key_select )
            {
                if(!HAS_FLAG(pt_ctx->ui1_state, LBST_PUSHED) )
                {
                    WGL_SET_FLAG(pt_ctx->ui1_state, LBST_PUSHED);
                    CATCH_FAIL_BREAK(_lb_tempunlock_on_elem_pushed(&pt_ctx), i4_ret_fnl);
                    if(HAS_STYLE(pt_ctx, WGL_STL_LB_ELEM_NO_PUSH))
                    {
                        /* simulate an unpush event immediately in WGL_STL_LB_ELEM_NO_PUSH style*/
                        CATCH_FAIL_BREAK(_lb_tempunlock_on_elem_unpushed(&pt_ctx, TRUE), i4_ret_fnl);
                        WGL_CLEAR_FLAG(pt_ctx->ui1_state, LBST_PUSHED);
                    }
                }
            }
            else if( ui4_key==pt_ctx->t_keymap.ui4_key_next )
            {
                i4_ret_fnl = _lb_tempunlock_on_elem_next(&pt_ctx, TRUE);
            }    
            else if( ui4_key==pt_ctx->t_keymap.ui4_key_prev)
            {
                i4_ret_fnl = _lb_tempunlock_on_elem_prev(&pt_ctx, TRUE);
            }    
            else if( ui4_key==pt_ctx->t_keymap.ui4_key_page_up )
            {
                i4_ret_fnl = _lb_tempunlock_on_page_up(&pt_ctx, TRUE);
            }
            else if( ui4_key==pt_ctx->t_keymap.ui4_key_page_down )
            {
                i4_ret_fnl = _lb_tempunlock_on_page_down(&pt_ctx, TRUE);
            }
            else
            {
                REL_LOCK(h_lb, pt_ctx);
                i4_ret_fnl = wgl_notify_parent_key(h_lb, ui4_key, WGL_KEY_STATE_DOWN);
            }
            INFORM_ERR(i4_ret_fnl);
        }
        break;

        case WGL_MSG_KEY_UP:
        {
            UINT32 ui4_key = (UINT32)pv_param1;
            if( ui4_key == pt_ctx->t_keymap.ui4_key_select )
            {
                if(HAS_FLAG(pt_ctx->ui1_state, LBST_PUSHED))
                {
                    CATCH_FAIL_BREAK(_lb_tempunlock_on_elem_unpushed(&pt_ctx, TRUE), i4_ret_fnl);
                    WGL_CLEAR_FLAG(pt_ctx->ui1_state, LBST_PUSHED);
                }
            }
            else
            {
                REL_LOCK(h_lb, pt_ctx);
                i4_ret_fnl = wgl_notify_parent_key(h_lb, ui4_key, WGL_KEY_STATE_UP);
                INFORM_ERR(i4_ret_fnl);
            }
        }
        break;

        case WGL_MSG_GET_FOCUS:
        {
            if( !(pt_ctx->ui1_state & LBST_FOCUSED) )
            {
                WGL_SET_FLAG(pt_ctx->ui1_state, LBST_FOCUSED);
                DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_SET_STATE, WGL_PACK(pt_ctx->ui1_state), NULL, i4_ret );
                INFORM_ERR(i4_ret);
            }
            REL_LOCK(h_lb, pt_ctx);
            INFORM_ERR(wgl_notify_parent_focus(h_lb, TRUE));
        }
        break;

        case WGL_MSG_LOSE_FOCUS:
        {
            if(HAS_FLAG(pt_ctx->ui1_state, LBST_FOCUSED) )
            {
                WGL_CLEAR_FLAG(pt_ctx->ui1_state, LBST_FOCUSED);
                DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_SET_STATE, WGL_PACK(pt_ctx->ui1_state), NULL, i4_ret );
                INFORM_ERR(i4_ret);
            }

            /*auto generate a dummy unpushed event*/
            if(HAS_FLAG(pt_ctx->ui1_state, LBST_PUSHED))
            {
                CATCH_FAIL_BREAK(_lb_tempunlock_on_elem_unpushed(&pt_ctx, FALSE), i4_ret_fnl);
                WGL_CLEAR_FLAG(pt_ctx->ui1_state, LBST_PUSHED);
            }

            REL_LOCK(h_lb, pt_ctx);
            INFORM_ERR(wgl_notify_parent_focus(h_lb, FALSE));
        }
        break;

        case WGL_MSG_ENABLE:
        {
            BOOL b_is_enable = WGL_POINTER_TO_BOOL(pv_param1);
            if( b_is_enable && pt_ctx->ui1_state & LBST_DISABLED )
            {
                WGL_CLEAR_FLAG(pt_ctx->ui1_state, LBST_DISABLED);
                DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_SET_STATE, WGL_PACK(pt_ctx->ui1_state), NULL, i4_ret_fnl );
            }
            else if(!b_is_enable && !(pt_ctx->ui1_state & LBST_DISABLED))
            {
                WGL_SET_FLAG(pt_ctx->ui1_state, LBST_DISABLED);
                DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_SET_STATE, WGL_PACK(pt_ctx->ui1_state), NULL, i4_ret_fnl );
            }
            INFORM_ERR(i4_ret_fnl);
        }
        break;
        
        case WGL_MSG_CHANGE_COLOR_CFG:
        {
            DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_UPDATE_CVSST, pv_param1, NULL, i4_ret_fnl);
        }
        break;

#ifdef WGL_ANIM_SUPPORT
        case WGL_MSG_ANIMATION_TICK:
        {
            UINT32 ui4_tick_idx = (UINT32)pv_param2;
            WGL_ANIM_TICK_DATA  t_tick_inf;
            WGL_UPD_REGION_T    t_anim_upd;
            WGL_ANIM_COND_INF_T t_anim_cond;

            /*Do the animation tick*/
            i4_ret = wgl_anim_cmd_proc(pt_ctx->t_anim,WGL_CMD_ANIM_ATTR_TICK,WGL_PACK(ui4_tick_idx),NULL);
            INFORM_ERR(i4_ret);
            if(i4_ret != WGLR_OK)
            {
                break;
            }

            /*Get the animation updated region.*/
            x_memset(&t_anim_upd,0,sizeof(WGL_UPD_REGION_T));
            i4_ret = wgl_anim_cmd_proc(pt_ctx->t_anim,
                           WGL_CMD_ANIM_ATTR_GET_UPD_RGN, 
                           WGL_PACK(&pt_ctx->t_anim_s),
                           WGL_PACK(&t_anim_upd));
            INFORM_ERR(i4_ret);
            if(i4_ret != WGLR_OK || t_anim_upd.ui4_update_rect_num == 0)
            {
                break;
            }
            
            /*Get the animation tick then set to listbox content.*/
            x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
            i4_ret = wgl_anim_cmd_proc(pt_ctx->t_anim,WGL_CMD_ANIM_ATTR_GET_TICK_INF,NULL,WGL_PACK(&t_tick_inf));
            INFORM_ERR(i4_ret);
            if(i4_ret != WGLR_OK)
            {
                break;
            }

            if(!HAS_FLAG(t_tick_inf.ui2_cur_rsn,WGL_ANIM_TYPE_NONE))
            {
                DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_SET_ANIM_TICK_INF, WGL_PACK(&t_tick_inf), NULL, i4_ret_fnl );
                INFORM_ERR(i4_ret);
            }
            else
            {
                break;
            }

            x_memset(&t_anim_cond,0,sizeof(WGL_ANIM_COND_INF_T));
            i4_ret = wgl_anim_cmd_proc(pt_ctx->t_anim,WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&t_anim_cond),NULL);
            INFORM_ERR(i4_ret);
            if(i4_ret != WGLR_OK)
            {
                break;
            }

            if(_ANIM_TOTAL_END(&t_anim_cond))
            {
                /*The animation is total end,then highlight the element.*/    
                _lb_hlt_element_after_anim(pt_ctx);
            }           
            
            /*The animation information has change at this tick,so repaint*/
            DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_REQ_UPDATE_ANIM, WGL_PACK(&t_anim_upd), NULL, i4_ret_fnl );
            INFORM_ERR(i4_ret);
            
            CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(&pt_ctx));
            
        }
        break;
#endif        
    }

    if(pt_ctx)
    {
        REL_LOCK(h_lb, pt_ctx);
    }
    
    return i4_ret_fnl;

}

UINT8 wgl_lb_get_state_by_elem(LB_ELEM_T* pt_elem)
{
    if(pt_elem == NULL || pt_elem->pt_elem_inf == NULL)
    {
        return 0;               
    }

    return pt_elem->pt_elem_inf->ui1_state;
}

#ifdef WGL_ANIM_SUPPORT
UINT32 wgl_lb_get_ani_style_by_elem(LB_ELEM_T* pt_elem)
{
    if(pt_elem == NULL || pt_elem->pt_elem_inf == NULL)
    {
        return 0;               
    }

    return pt_elem->pt_elem_inf->ui4_ani_style;
}

static INT32 _lb_hlt_element_after_anim(
    IN OUT  LB_CTX_T                    *pt_ctx)
{
    INT32               i4_ret = WGLR_OK;
    BOOL                b_is_vp_scrolled = FALSE;

    CHECK_NULL(pt_ctx,WGLR_INV_ARG);    

    if(pt_ctx->ui2_anim_idx == WGL_LB_NULL_INDEX)
    {
        return WGLR_INTERNAL_ERROR;
    }
    
    _LB_CLR_ELEM_STATE(pt_ctx, pt_ctx->ui2_anim_idx, LBEST_ANIM);
    pt_ctx->ui2_anim_idx = WGL_LB_NULL_INDEX;

    if(pt_ctx->ui2_anim_hlt != WGL_LB_NULL_INDEX)
    {
        if( _LB_SET_ELEM_STATE(pt_ctx, pt_ctx->ui2_anim_hlt, LBEST_HLT) == WGLR_OK )
        {
            pt_ctx->ui2_idx = pt_ctx->ui2_anim_hlt;
        }
        else
        {
            pt_ctx->ui2_idx = WGL_LB_NULL_INDEX;
        }
        CATCH_FAIL(_lb_check_elem_visibility_after_user_key(pt_ctx, pt_ctx->ui2_anim_hlt, TRUE, &b_is_vp_scrolled));
    }


    /* request minimal update region */
    INFORM_ERR (_lb_req_update_by_hlt_nxt_prv_elem(pt_ctx, 
                                                   b_is_vp_scrolled, 
                                                   WGL_LB_NULL_INDEX, 
                                                   pt_ctx->ui2_idx));

    WGL_CLEAR_FLAG(pt_ctx->ui1_state, LBST_ANIM_DOING);
    return WGLR_OK;
}
#endif

/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_cmd_proc
 *
 * Description: This API hanldes all the commands of listbox.
 *
 * Inputs:  h_lb      Contains the listbox handle.
 *          ppt_ctx   Contains the pointer of the reference to the listbox instance context.
 *          ui4_cmd   Contains the command id.
 *          pv_param1 Contains the first parameter of the command if necessary.
 *          pv_param2 Contains the second parameter of the command if necessary.
 *
 * Outputs: pv_param1 Contains the first return value of the command if necessary.
 *          pv_param2 Contains the second return value of the command if necessary.
 *
 * Returns: WGLR_OK   Routine successful.
 *          < 0       Some errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_cmd_proc(  
    IN      HANDLE_T                    h_lb,
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT32                      ui4_cmd,
    IN OUT  VOID                        *pv_param1,
    IN OUT  VOID                        *pv_param2)
{
    INT32       i4_ret;
    INT32       i4_ret_fnl = WGLR_OK;
    
    /*deliver content or border command */
    if( WGL_IS_CONTENT_CMD(ui4_cmd) )
    {
        DO_LB_CNT_CMD(*ppt_ctx, ui4_cmd, pv_param1, pv_param2, i4_ret);
        switch(i4_ret) 
        {
            case WGL_LBR_NEED_REPAINT_PARTIAL:
                i4_ret = _lb_tempunlock_repaint_partial_generic(ppt_ctx);
            break;
            case WGL_LBR_NEED_REPAINT_FULL:
                i4_ret = _lb_tempunlock_repaint_full (ppt_ctx);
            break;
        }
        return i4_ret;
    }
    else if( WGL_IS_BORDER_CMD(ui4_cmd) && !IS_NULL_BORDER(*ppt_ctx) )
    {
        /*intercept some border commands here*/
        switch(ui4_cmd) 
        {
            case WGL_CMD_GL_SET_INSET:
                CATCH_FAIL(_lb_set_inset(*ppt_ctx, (WGL_INSET_T *)pv_param1));
                break;
            case WGL_CMD_GL_SET_IS_BDR_DRAW_INTERIOR:
                CATCH_FAIL(_lb_set_is_bdr_draw_interior(*ppt_ctx, WGL_POINTER_TO_BOOL(pv_param1) ));
                break;
        }
        
        DO_BDR_CMD(*ppt_ctx, ui4_cmd, pv_param1, pv_param2, i4_ret);
        return i4_ret;
    }
    
    if(! (WGL_IS_WIDGET_CMD(ui4_cmd) && 
            ( WGL_IS_CMD_BELONG_WIDGET(HT_WGL_WIDGET_LIST_BOX, ui4_cmd) 
               || WGL_IS_CMD_GENERAL(ui4_cmd)
            ) 
         )
      )
    {
        return WGLR_INV_CMD;
    }
    
    switch(ui4_cmd)
    {
        /*General Commands*/
        case WGL_CMD_GL_SET_ATTACH_DATA:
        {
            (*ppt_ctx)->pv_attach = pv_param1;
        }    
        break;
        
        case WGL_CMD_GL_GET_ATTACH_DATA:
        {
            *((VOID **)pv_param1) = (*ppt_ctx)->pv_attach;
        }
        break;

        case WGL_CMD_GL_SET_COLOR: /*pv_param1:target-index | pv_param2:WGL_COLOR_INFO_T */
        {
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_SET_COLOR, pv_param1, pv_param2, i4_ret_fnl);
        }
        break;
        
        case WGL_CMD_GL_GET_COLOR:
        {
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_COLOR, pv_param1, pv_param2, i4_ret_fnl);
        }
        break;
        
        case WGL_CMD_GL_SET_FONT:
        {
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_SET_FONT, pv_param1, NULL, i4_ret_fnl);
        }
        break;
        
        case WGL_CMD_GL_GET_FONT:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_FONT, pv_param1, NULL, i4_ret_fnl);
        }
        break;

        case WGL_CMD_GL_SET_FONT_EX:
        {
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_SET_FONT_EX, pv_param1, NULL, i4_ret_fnl);
        }
        break;

        case WGL_CMD_GL_GET_FONT_EX:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_FONT_EX, pv_param1, NULL, i4_ret_fnl);
        }
        break;

        case WGL_CMD_GL_SET_IMAGE:
        {
            UINT8           ui1_target   = WGL_POINTER_TO_UINT8(pv_param1);
            WGL_IMG_INFO_T  *pt_img_info = (WGL_IMG_INFO_T *)pv_param2;

            if((pt_img_info != NULL &&
                !_lb_is_valid_image_info(pt_img_info)) )
            {
                i4_ret_fnl = WGLR_INV_ARG;
                break;
            }
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_SET_IMAGE, ui1_target, pt_img_info, i4_ret_fnl);
        }
        break;
        
        case WGL_CMD_GL_GET_IMAGE:
        { 
            UINT8 ui1_target = WGL_POINTER_TO_UINT8(pv_param1);
            
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_IMAGE, ui1_target, pv_param2, i4_ret_fnl);
        }
        break;

        case WGL_CMD_GL_SET_TEXT_DIRECTION:
        {
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_SET_TEXT_DIRECTION, pv_param1, pv_param2, i4_ret_fnl);
            if(!IS_NULL_BORDER(*ppt_ctx))
            {
                DO_BDR_CMD(*ppt_ctx, WGL_CMD_GL_SET_BDR_TEXT_DIRECTION,pv_param1, pv_param2, i4_ret);
            }
        
        }
        break;

        case WGL_CMD_GL_GET_TEXT_DIRECTION:
        {
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_TEXT_DIRECTION, pv_param1, pv_param2, i4_ret_fnl);
        }
        break;
        
        /*Control Logic Commands - General Attributes*/
        case WGL_CMD_LB_GET_STYLE:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            *((UINT32 *)pv_param1) = (*ppt_ctx)->ui4_style;
        }
        break;
        
        case WGL_CMD_LB_GET_MAX_ELEM_NUM:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            *((UINT16 *)pv_param1) = _LB_GET_MAX_ELEMS_NUM(*ppt_ctx);
        }
        break;
        case WGL_CMD_LB_GET_ALL_ELEM_STATES:
        {
            UINT8       *aui1_states = (UINT8 *)pv_param1;
            UINT16      ui2_cb_size  = WGL_POINTER_TO_UINT16(pv_param2);
            
            CHECK_NULL(aui1_states, WGLR_INV_ARG);
            if(ui2_cb_size != ((UINT16)WGL_LB_ELEM_STATE_BUF_SIZE * _LB_GET_ELEMS_NUM(*ppt_ctx)))
            {
                return WGLR_INV_ARG;
            }
            
             _LB_GET_ELEMS_STATE_INF(*ppt_ctx, aui1_states, 0, _LB_GET_ELEMS_NUM(*ppt_ctx));
        }
        break;
        
        case WGL_CMD_LB_RESTORE_ALL_ELEM_STATES:
        {
            UINT8       *aui1_states = (UINT8 *)pv_param1;
            UINT16      ui2_cb_size  = WGL_POINTER_TO_UINT16(pv_param2);
            CHECK_NULL(aui1_states, WGLR_INV_ARG);
            if(ui2_cb_size != ((UINT16)WGL_LB_ELEM_STATE_BUF_SIZE * _LB_GET_ELEMS_NUM(*ppt_ctx)))
            {
                return WGLR_INV_ARG;
            }
            i4_ret_fnl = _lb_restore_all_elem_states(*ppt_ctx, aui1_states);
        }
        break;

        case WGL_CMD_LB_REG_EVN_HDLR_INFO:
        {
            WGL_LB_EVN_HDLR_INFO_T  *pt_evn_hdlr_info = (WGL_LB_EVN_HDLR_INFO_T *)pv_param1;

            if (pt_evn_hdlr_info == NULL)
            {
                (*ppt_ctx)->t_evn_hdlr_info.ui4_evn_hdlr_mask = 0;
                (*ppt_ctx)->t_evn_hdlr_info.pv_tag_evn_hdlr   = NULL;
                (*ppt_ctx)->t_evn_hdlr_info.pf_event_hdlr     = NULL;
            }
            else
            {
                if (pt_evn_hdlr_info->ui4_evn_hdlr_mask != 0)
                {
                    if (pt_evn_hdlr_info->pf_event_hdlr == NULL)
                    {
                        return WGLR_INV_ARG;
                    }
                    if (!(pt_evn_hdlr_info->ui4_evn_hdlr_mask & WGL_LB_EVN_HDLR_MASK_NVGT_IN_VP))
                    {
                        return WGLR_INV_ARG;
                    }
                    if (HAS_STYLE(*ppt_ctx, WGL_STL_LB_FORCE_HLT) &&
                        (pt_evn_hdlr_info->ui4_evn_hdlr_mask & WGL_LB_EVN_HDLR_MASK_NVGT_IN_VP))
                    {
                        return WGLR_INV_CMD;
                    }
                }  
                x_memcpy(&(*ppt_ctx)->t_evn_hdlr_info, pt_evn_hdlr_info, sizeof(WGL_LB_EVN_HDLR_INFO_T));
            }
        }
        break;

        case WGL_CMD_LB_GET_EVN_HDLR_INFO:
        {
            WGL_LB_EVN_HDLR_INFO_T  *pt_evn_hdlr_info = (WGL_LB_EVN_HDLR_INFO_T *)pv_param1;
            if (pt_evn_hdlr_info == NULL)
            {
                return WGLR_INV_ARG;
            }
            else
            {
                x_memcpy(pt_evn_hdlr_info, &(*ppt_ctx)->t_evn_hdlr_info, sizeof(WGL_LB_EVN_HDLR_INFO_T));
            }
        }
        break;

        /*Control Logic Commands - Element Operations*/
        case WGL_CMD_LB_REPAINT_ELEMS:
        {
            UINT16 ui2_idx_first = WGL_POINTER_TO_UINT16(pv_param1);
            UINT16 ui2_idx_last  = WGL_POINTER_TO_UINT16(pv_param2);
            
            if(ui2_idx_first > ui2_idx_last ||
               ui2_idx_last >= _LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }

            INFORM_ERR (_lb_req_update_by_range_elems (
                *ppt_ctx,
                FALSE,
                ui2_idx_first,
                ui2_idx_last));
            
            CATCH_FAIL (_lb_tempunlock_repaint_partial_generic (ppt_ctx));
            
        }
        break;

        case WGL_CMD_LB_REPAINT_ITEM:
        {
            UINT16  ui2_idx_elem = WGL_GET_HI_WORD(pv_param1);
            UINT8   ui1_idx_col  = WGL_GET_LOW_LOW_BYTE(pv_param1);

            if( ui2_idx_elem >= _LB_GET_ELEMS_NUM(*ppt_ctx) || 
                 ui1_idx_col >= (*ppt_ctx)->ui1_col_num )
            {
                return WGLR_INV_ARG;
            }

            i4_ret_fnl = _lb_tempunlock_repaint_partial_by_item(
                ppt_ctx, 
                ui2_idx_elem,
                ui1_idx_col);
        }
        break;

        case WGL_CMD_LB_APPEND_ELEM:
        {
            i4_ret_fnl = _lb_tempunlock_insert_elem(
                ppt_ctx,
                0, 
                (WGL_LB_ITEM_T *)pv_param1,
                TRUE );
        }
        break;

        case WGL_CMD_LB_INSERT_ELEM:
        {
            i4_ret_fnl = _lb_tempunlock_insert_elem(
                ppt_ctx,
                WGL_POINTER_TO_UINT16(pv_param1),
                (WGL_LB_ITEM_T *)pv_param2,
                FALSE);

        }
        break;

        case WGL_CMD_LB_DEL_ELEM:
        {
            i4_ret_fnl = _lb_tempunlock_del_elem(
                ppt_ctx, 
                WGL_POINTER_TO_UINT16(pv_param1));
        }
        break;

        case WGL_CMD_LB_DEL_ALL:
        {
            if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_FIXED_ELEM_NUM))
            {
                return WGLR_INV_CMD;
            }
            
#if 0
            if(_LB_GET_ELEMS_NUM(*ppt_ctx) == NULL)
            {
                return WGLR_OK;
            }
#endif
            i4_ret_fnl = _lb_clear(*ppt_ctx);
        }
        break;

        case WGL_CMD_LB_SET_ELEM_NUM:
        {
            UINT16  ui2_num_elem = WGL_POINTER_TO_UINT16(pv_param1);

            if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_FIXED_ELEM_NUM))
            {
                return WGLR_INV_CMD;
            }
            if(ui2_num_elem>_LB_GET_MAX_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }
            
            i4_ret_fnl = _lb_tempunlock_set_elem_num(ppt_ctx, ui2_num_elem);
        }
        break;
        
        case WGL_CMD_LB_GET_ELEM_NUM:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            *((UINT16 *)pv_param1) = _LB_GET_ELEMS_NUM(*ppt_ctx);
        }
        break;

        case WGL_CMD_LB_GET_HLT_INDEX:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            *((UINT16 *)pv_param1) = (*ppt_ctx)->ui2_idx;
        }
        break;

        case WGL_CMD_LB_IS_INDEX_HLT:
        {
            UINT16 ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);

            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }

            *((BOOL *)pv_param2) = (ui2_idx==(*ppt_ctx)->ui2_idx) ? TRUE : FALSE;
        }
        break;

        case WGL_CMD_LB_HLT_NEXT_ELEM:
            CATCH_FAIL(_lb_tempunlock_on_elem_next(ppt_ctx, WGL_POINTER_TO_BOOL(pv_param1)));
        break;
        
        case WGL_CMD_LB_HLT_PREV_ELEM:
            CATCH_FAIL(_lb_tempunlock_on_elem_prev(ppt_ctx, WGL_POINTER_TO_BOOL(pv_param1)));
        break;

        case WGL_CMD_LB_HLT_ELEM:
        {
            UINT16 ui2_idx      = WGL_POINTER_TO_UINT16(pv_param1);
            BOOL   b_not_do_nfy = WGL_POINTER_TO_BOOL(pv_param2);
            
            if(ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }
#ifdef WGL_ANIM_SUPPORT
            i4_ret_fnl = _lb_tempunlock_highlight_elem(ppt_ctx, ui2_idx, NULL, b_not_do_nfy, WGL_ANIM_ACTION_HIGHLIGHT);
#else
            i4_ret_fnl = _lb_tempunlock_highlight_elem(ppt_ctx, ui2_idx, NULL, b_not_do_nfy);
#endif
        }
        break;

        case WGL_CMD_LB_HLT_ELEM_VISIBLE:
        {
            UINT16 ui2_idx         = WGL_POINTER_TO_UINT16(pv_param1);
            UINT16 ui2_idx_new_pos = WGL_POINTER_TO_UINT16(pv_param2);
            if(ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }
#ifdef WGL_ANIM_SUPPORT
            CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_idx, NULL, FALSE, WGL_ANIM_ACTION_HIGHLIGHT));
#else
            CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_idx, NULL, FALSE));
#endif
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_MOVE_ELEM_VISIBLE, ui2_idx, ui2_idx_new_pos, i4_ret);
            CATCH_FAIL(i4_ret);
        }
        break;

        case WGL_CMD_LB_UNHLT_ELEM:
        {
            UINT16 ui2_idx      = WGL_POINTER_TO_UINT16(pv_param1);
            BOOL   b_not_do_nfy = WGL_POINTER_TO_BOOL(pv_param2);
            
            if(ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }
            i4_ret_fnl = _lb_tempunlock_unhighlight_elem(ppt_ctx, ui2_idx, b_not_do_nfy);
        }
        break;
        
        case WGL_CMD_LB_GET_SEL_NUM:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            *((UINT16 *)pv_param1) = (*ppt_ctx)->ui2_sel_num;
        }
        break;

        case WGL_CMD_LB_GET_SEL_INDEXES:
        { /*pv_param1: buffer size, pv_parma2: the buffer*/
            UINT16      ui2_i, ui2_buf_idx_num;
            UINT16      *pui2_idxs;

            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            ui2_buf_idx_num = WGL_POINTER_TO_UINT16(pv_param1);
            pui2_idxs       = (UINT16 *)pv_param2;
            if( ui2_buf_idx_num < (*ppt_ctx)->ui2_sel_num )            
            {
                return WGLR_INV_ARG;
            }
            
            if((*ppt_ctx)->ui2_sel_num > 0)
            {
                _LB_GET_ELEM_SPC_ST_IDX(*ppt_ctx,LBEST_SEL,ui2_buf_idx_num,pui2_idxs);
            }
            else
            {
                /* clear the first idx as WGL_LB_NULL_INDEX */
                for(ui2_i = 0; ui2_i < ui2_buf_idx_num; ui2_i ++ )
                {
                    pui2_idxs[ui2_i] = WGL_LB_NULL_INDEX;
                }
            }
        }
        break;

        case WGL_CMD_LB_IS_EMPTY_SEL:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            *((BOOL *)pv_param1) = ((*ppt_ctx)->ui2_sel_num==0)?TRUE:FALSE;
        }
        break;

        case WGL_CMD_LB_IS_INDEX_SEL:
        {
            UINT16 ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            
            *((BOOL *)pv_param2) = _LB_CHK_ELEM_STATE(*ppt_ctx, ui2_idx, LBEST_SEL);

        }
        break;
        
        case WGL_CMD_LB_SET_INDEX_SEL:
        {
            UINT16 ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            CHECK_VALID( ui2_idx < _LB_GET_ELEMS_NUM(*ppt_ctx), WGLR_INV_ARG );
            i4_ret_fnl = _lb_tempunlock_sel_elem(ppt_ctx, FALSE, ui2_idx, NULL );
        }
        break;
        
        case WGL_CMD_LB_SET_INDEX_DESEL:
        {
            UINT16 ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            CHECK_VALID( ui2_idx < _LB_GET_ELEMS_NUM(*ppt_ctx), WGLR_INV_ARG );
            i4_ret_fnl = _lb_tempunlock_desel_elem(ppt_ctx, FALSE, ui2_idx );
        }
        break;

        case WGL_CMD_LB_SEL_ELEM:
        {
            UINT16 ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            CHECK_VALID( ui2_idx < _LB_GET_ELEMS_NUM(*ppt_ctx), WGLR_INV_ARG );
            i4_ret_fnl = _lb_tempunlock_sel_elem(ppt_ctx, TRUE, ui2_idx, NULL );
        }
        break;

        case WGL_CMD_LB_DESEL_ELEM:
        {
            UINT16 ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            CHECK_VALID( ui2_idx < _LB_GET_ELEMS_NUM(*ppt_ctx), WGLR_INV_ARG );
            i4_ret_fnl = _lb_tempunlock_desel_elem(ppt_ctx, TRUE, ui2_idx );
        }
        break;

        case WGL_CMD_LB_DESEL_ALL:
        {
            UINT16      ui2_i;

            if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_FORCE_SEL))
            {
                return WGLR_INV_CMD;
            }

            if((*ppt_ctx)->ui2_sel_num==0)
            {
                return WGLR_OK;
            }

            for(ui2_i=0; ui2_i<_LB_GET_ELEMS_NUM(*ppt_ctx) && !WGL_IS_FAIL(i4_ret_fnl) ; ui2_i++)
            {
                if(_LB_CHK_ELEM_STATE(*ppt_ctx, ui2_i, LBEST_SEL) == TRUE)
                {
                    CATCH_FAIL(_lb_tempunlock_desel_elem(ppt_ctx, TRUE, ui2_i));
                }
            }
        }
        break;

        case WGL_CMD_LB_DISABLE_ELEM:
        {
            UINT16 ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }
            i4_ret_fnl = _lb_tempunlock_disable_elem(ppt_ctx, ui2_idx );
        }
        break;

        case WGL_CMD_LB_ENABLE_ELEM:
        {
            UINT16 ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }
            i4_ret_fnl = _lb_tempunlock_enable_elem(ppt_ctx, ui2_idx );
        }
        break;

        case WGL_CMD_LB_IS_INDEX_ENABLED:
        {
            UINT16  ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            BOOL    b_is_disabled = FALSE;

            if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            
            /**((BOOL *)pv_param1) = ((*ppt_ctx)->pt_elems[ui2_idx].ui1_state&LBEST_SEL)?TRUE:FALSE;*/
            b_is_disabled = _LB_CHK_ELEM_STATE(*ppt_ctx, ui2_idx, LBEST_DISABLED);
            *((BOOL *)pv_param2) = b_is_disabled ? FALSE : TRUE;
        }
        break;

        case WGL_CMD_LB_PAGE_UP:
        {
            CATCH_FAIL(_lb_tempunlock_on_page_up(ppt_ctx, WGL_POINTER_TO_BOOL(pv_param1)));
        }
        break;
        
        case WGL_CMD_LB_PAGE_DOWN:
        {
            CATCH_FAIL(_lb_tempunlock_on_page_down(ppt_ctx, WGL_POINTER_TO_BOOL(pv_param1)));
        }
        break;

        case WGL_CMD_LB_ENSURE_HLT_ELEM_VISIBLE:
        {
            if(_HAS_HLT_ELEM(*ppt_ctx))
            {
                if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_NO_SCROLL))
                {
                    BOOL    b_temp;
                    DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_ENSURE_ELEM_VISIBLE_NO_SCROLL, (*ppt_ctx)->ui2_idx, &b_temp, i4_ret);
                    CATCH_FAIL(i4_ret);
                }
                else
                {
                    DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_ENSURE_ELEM_VISIBLE_PREFER_CENTER, (*ppt_ctx)->ui2_idx, NULL, i4_ret);
                    CATCH_FAIL(i4_ret);
                }
            }
        }
        break;

        case WGL_CMD_LB_ENSURE_ELEM_VISIBLE:
        {
            UINT16                  ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            LB_CNT_EEVMM_PARAM_T    t_param;
            BOOL                    b_temp;
            if( ui2_idx>=_LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }
            t_param.ui2_idx    = ui2_idx;
            t_param.ui2_margin = (*ppt_ctx)->ui2_margin_hlt;
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_ENSURE_ELEM_VISIBLE_MIN_MOVE, &t_param, &b_temp, i4_ret);
            CATCH_FAIL(i4_ret);
        }
        break;

        case WGL_CMD_LB_MOVE_ELEM_VISIBLE:
        {
            UINT16  ui2_idx         = WGL_POINTER_TO_UINT16(pv_param1);
            UINT16  ui2_idx_new_pos = WGL_POINTER_TO_UINT16(pv_param2);
            if( ui2_idx>=_LB_GET_ELEMS_NUM(*ppt_ctx) || ui2_idx_new_pos>=_LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_MOVE_ELEM_VISIBLE, ui2_idx, ui2_idx_new_pos, i4_ret);
            CATCH_FAIL(i4_ret);
        }
        break;
        
        case WGL_CMD_LB_SET_HLT_MARGIN:
        {
            CHECK_VALID(!HAS_FLAG((*ppt_ctx)->ui4_style, WGL_STL_LB_NO_SCROLL), WGLR_INV_CMD);
            (*ppt_ctx)->ui2_margin_hlt = WGL_POINTER_TO_UINT16(pv_param1);
        }
        break;
        
        case WGL_CMD_LB_GET_HLT_MARGIN:
        {
            CHECK_VALID(!HAS_FLAG((*ppt_ctx)->ui4_style, WGL_STL_LB_NO_SCROLL), WGLR_INV_CMD);
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            *((UINT16 *)pv_param1) = (*ppt_ctx)->ui2_margin_hlt;
        }
        break;

        case WGL_CMD_LB_SET_KEY_MAP:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            x_memcpy(&(*ppt_ctx)->t_keymap, pv_param1, sizeof(WGL_LB_KEY_MAP_T));
        }
        break;

        /*Control Logic Commands - Column Operations*/
        case WGL_CMD_LB_GET_COL_NUM:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            *((UINT8 *)pv_param1) = (*ppt_ctx)->ui1_col_num;
        }
        break;
        
        case WGL_CMD_LB_GET_COL_TYPE:
        {
            UINT8 ui1_idx = WGL_POINTER_TO_UINT8(pv_param1);
            if( ui1_idx >= (*ppt_ctx)->ui1_col_num || pv_param2==NULL)
            {
                return WGLR_INV_ARG;
            }
            *((WGL_LB_COL_TYPE_T *)pv_param2) = (*ppt_ctx)->at_cols[ui1_idx].e_col_type;
        }
        break;
        
        /*Control Logic Commands - Data Operations*/
        case WGL_CMD_LB_SET_ELEM_TAG: /* pv_param1: UINT16 ui2_elem_idx, pv_param2: VOID *pv_tag */
        { 
            UINT16              ui2_idx       = WGL_POINTER_TO_UINT16(pv_param1);
            LB_ELEM_T           *pt_elem;
            
            CHECK_VALID(ui2_idx < _LB_GET_ELEMS_NUM(*ppt_ctx), WGLR_INV_ARG);
            _lb_get_elem(*ppt_ctx, ui2_idx, &pt_elem, NULL); 
            if(pt_elem != NULL)
            {
                pt_elem->pv_tag = pv_param2;
            }
        }
        break;
        
        case WGL_CMD_LB_GET_ELEM_TAG: /* pv_param1: UINT16 ui2_elem_idx, pv_param2: VOID **ppv_tag */
        { 
            UINT16              ui2_idx       = WGL_POINTER_TO_UINT16(pv_param1);
            LB_ELEM_T           *pt_elem;
            
            CHECK_VALID(ui2_idx < _LB_GET_ELEMS_NUM(*ppt_ctx) && pv_param2 != NULL, WGLR_INV_ARG);
            _lb_get_elem(*ppt_ctx, ui2_idx, &pt_elem, NULL); 
            if( pt_elem == NULL)
            {
                return WGLR_INV_ARG;
            }
            *((VOID **)pv_param2) = pt_elem->pv_tag;
        }
        break;
        
        case WGL_CMD_LB_SET_ELEM_DATA:
        { 
            UINT16              ui2_idx       = WGL_POINTER_TO_UINT16(pv_param1);
            WGL_LB_ELEM_DATA_T  *pt_elem_data = (WGL_LB_ELEM_DATA_T *)pv_param2;
            LB_ELEM_T           *pt_elem;
            
            if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }
            _lb_get_elem(*ppt_ctx, ui2_idx, &pt_elem, NULL); 
            i4_ret_fnl = _lb_set_elem_data(*ppt_ctx, pt_elem, pt_elem_data);
        }
        break;

        case WGL_CMD_LB_GET_ELEM_DATA:
        {
            UINT16              ui2_idx           = WGL_POINTER_TO_UINT16(pv_param1);
            WGL_LB_ELEM_DATA_T  *pt_elem_data_out = (WGL_LB_ELEM_DATA_T *)pv_param2;
            UINT8       ui1_c;
            if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx) || 
                pt_elem_data_out==NULL ||
                pt_elem_data_out->at_items == NULL)
            {
                return WGLR_INV_ARG;
            }
            for(ui1_c=0; ui1_c<(*ppt_ctx)->ui1_col_num; ui1_c++)
            {
                switch((*ppt_ctx)->at_cols[ui1_c].e_col_type) 
                {
                    case LB_COL_TYPE_TEXT:
                        if(pt_elem_data_out->at_items[ui1_c].pw2s_text == NULL)
                        {
                            return WGLR_INV_ARG;
                        }
                        break;
                    case LB_COL_TYPE_IMG_SUIT:
                        if(pt_elem_data_out->at_items[ui1_c].pt_img_suit== NULL)
                        {
                            return WGLR_INV_ARG;
                        }
                        break;
                    default:
                        break;
                 }
            }

            i4_ret_fnl = _lb_get_elem_data(*ppt_ctx, ui2_idx, pt_elem_data_out);
        }
        break;

        case WGL_CMD_LB_SET_ELEMS_BY_RANGE:
        {
            UINT16              ui2_start    = WGL_GET_HI_WORD(pv_param1);
            UINT16              ui2_end      = WGL_GET_LOW_WORD(pv_param1);
            WGL_LB_ELEM_DATA_T *at_elem_data = (WGL_LB_ELEM_DATA_T *)pv_param2;
            UINT16              ui2_i, ui2_d;
            LB_ELEM_T           *pt_elem;
            
            if(ui2_start > ui2_end              ||
               ui2_end >= _LB_GET_ELEMS_NUM(*ppt_ctx)  ||
               at_elem_data==NULL)
            {
                return WGLR_INV_ARG;
            }
            
            for(ui2_i=ui2_start, ui2_d=0; ui2_i<=ui2_end; ui2_d++, ui2_i++)
            {
                _lb_get_elem(*ppt_ctx, ui2_i, &pt_elem,NULL);
                CATCH_FAIL(_lb_set_elem_data(*ppt_ctx, pt_elem, &at_elem_data[ui2_d]));
            }

        }
        break;

        case WGL_CMD_LB_CLEAR_ELEM_DATA:
        {
            UINT16              ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            LB_ELEM_T           *pt_elem;
            if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx) )
            {
                return WGLR_INV_ARG;
            }
            _lb_get_elem(*ppt_ctx, ui2_idx, &pt_elem, NULL);
            if(pt_elem != NULL)
            {
                _lb_clear_elem_data(*ppt_ctx, pt_elem);
                _LB_CLR_ELEM_STATE(*ppt_ctx, ui2_idx,LBEST_CHACHED);
            }
        }
        break;

        case WGL_CMD_LB_CLEAR_ELEMS_BY_RANGE:
        {
            UINT16              ui2_start = WGL_POINTER_TO_UINT16(pv_param1);
            UINT16              ui2_end   = WGL_POINTER_TO_UINT16(pv_param2);
            UINT16              ui2_i;
            LB_ELEM_T           *pt_elem;
            
            if(ui2_start > ui2_end ||
               ui2_end >= _LB_GET_ELEMS_NUM(*ppt_ctx))
            {
                return WGLR_INV_ARG;
            }
            
            for(ui2_i=ui2_start; ui2_i<=ui2_end; ui2_i++)
            {
                _lb_get_elem(*ppt_ctx, ui2_i, &pt_elem, NULL);
                if(pt_elem != NULL)
                {
                    _lb_clear_elem_data(*ppt_ctx, pt_elem);
                    _LB_CLR_ELEM_STATE(*ppt_ctx, ui2_i,LBEST_CHACHED);
                }
            }
            
        }
        break;

        case WGL_CMD_LB_SET_ITEM_TEXT:
        {
            /*WGL_PACK_2(element-index, item-index)*/
            UINT16      ui2_idx_elem = WGL_GET_HI_WORD(pv_param1);
            UINT8       ui1_idx_col  = WGL_GET_LOW_LOW_BYTE(pv_param1);
            LB_ELEM_T   *pt_elem;

            if( ui2_idx_elem >= _LB_GET_ELEMS_NUM(*ppt_ctx) || 
                 ui1_idx_col >= (*ppt_ctx)->ui1_col_num )
            {
                return WGLR_INV_ARG;
            }
            
            if( !IS_COL_TEXT(*ppt_ctx, ui1_idx_col) )
            {
                return WGLR_INV_CMD;
            }

#ifdef WGL_LB_NO_STORE_TEXT
            if(HAS_FLAG((*ppt_ctx)->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
            {
                return WGLR_OK;
            }
#endif

            _lb_get_elem(*ppt_ctx, ui2_idx_elem, &pt_elem, NULL);
            if(pt_elem == NULL)
            {
                return WGLR_OK;
            }
            
            if( pv_param2 == NULL)
            {   /*sets an empty string*/
                x_uc_w2s_set(pt_elem->at_items[ui1_idx_col].t_text.pw2s_text, 0, WGL_CHAR_NULL);
                x_uc_w2s_set(pt_elem->at_items[ui1_idx_col].t_text.pw2s_raw_text, 0, WGL_CHAR_NULL);
            }
            else
            {
				UINT32     ui4_text_dir;
                VOID* pv_reorder_attr=NULL;
                
				CATCH_FAIL((*ppt_ctx)->t_intf_cnt.pf_content_cmd_proc( (*ppt_ctx)->pv_ctx_cnt, WGL_CNT_LB_GET_TEXT_DIRECTION, WGL_PACK(&ui4_text_dir), NULL));
                if (check_exist_arabic((UTF16_T *)pv_param2,(*ppt_ctx)->at_cols[ui1_idx_col].ui2_max_text_len))
                {

				    UTF16_T*  w2s_buffer;
				    CATCH_FAIL((*ppt_ctx)->t_intf_cnt.pf_content_cmd_proc( (*ppt_ctx)->pv_ctx_cnt, WGL_CNT_LB_GET_BUFFER, WGL_PACK(&w2s_buffer), NULL));
                    x_memset(w2s_buffer,WGL_CHAR_NULL,UTF16_MAX_LEN * (((UINT32)(*ppt_ctx)->at_cols[ui1_idx_col].ui2_max_text_len + 1)));                
                    if((*ppt_ctx)->t_arab_reorder_attr.b_find_init_level == FALSE)
                    {
                        pv_reorder_attr = &(*ppt_ctx)->t_arab_reorder_attr;
                    }

                    i4_ret_fnl = reorder_arabic((UTF16_T *)pv_param2,(*ppt_ctx)->at_cols[ui1_idx_col].ui2_max_text_len,pv_reorder_attr,w2s_buffer,NULL); 
				    if (i4_ret_fnl != WGLR_OK)
			    	{
						return i4_ret_fnl;
			    	}
                    /*reset reorder attr after used for current string content*/
                    (*ppt_ctx)->t_arab_reorder_attr.b_find_init_level = TRUE;
                    (*ppt_ctx)->t_arab_reorder_attr.ui1_embedding_level = 0;
 
                    pt_elem->at_items[ui1_idx_col].t_text.pw2s_text = x_uc_w2s_strcpy(pt_elem->at_items[ui1_idx_col].t_text.pw2s_text,w2s_buffer);                  
                  
                }
                else
                {              
                    _lb_tool_safe_w2s_strcpy(*ppt_ctx, pt_elem->at_items[ui1_idx_col].t_text.pw2s_text, (UTF16_T *)pv_param2, (UINT32)(*ppt_ctx)->at_cols[ui1_idx_col].ui2_max_text_len);
                }
                
                _lb_tool_safe_w2s_strcpy(*ppt_ctx, pt_elem->at_items[ui1_idx_col].t_text.pw2s_raw_text, (UTF16_T *)pv_param2, (UINT32)(*ppt_ctx)->at_cols[ui1_idx_col].ui2_max_text_len);

        }

            _LB_SET_ELEM_STATE(*ppt_ctx, ui2_idx_elem,LBEST_CHACHED);
        }
        break;

        case WGL_CMD_LB_GET_ITEM_TEXT:
        {
            /*WGL_PACK_2(element-index, item-index)*/
            UINT16      ui2_idx_elem = WGL_GET_HI_WORD(pv_param1);
            UINT8       ui1_idx_col  = WGL_GET_LOW_LOW_BYTE(pv_param1);
            LB_ELEM_T   *pt_elem;

            if( ui2_idx_elem >= _LB_GET_ELEMS_NUM(*ppt_ctx) || 
                ui1_idx_col >= (*ppt_ctx)->ui1_col_num   || 
                pv_param2 == NULL)
            {
                return WGLR_INV_ARG;
            }
            
            if( !IS_COL_TEXT(*ppt_ctx, ui1_idx_col) )
            {
                return WGLR_INV_CMD;
            }

#ifdef WGL_LB_NO_STORE_TEXT
            if(HAS_FLAG((*ppt_ctx)->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
            {
                return WGLR_OK;
            }
#endif
            _lb_get_elem(*ppt_ctx, ui2_idx_elem, &pt_elem, NULL);
             
            if(pt_elem != NULL)
            {
                x_uc_w2s_strcpy((UTF16_T *)pv_param2, pt_elem->at_items[ui1_idx_col].t_text.pw2s_raw_text);
            }
        }
        break;
        case WGL_CMD_GL_SET_TEXT_ATTR:
        {
            if(NULL == pv_param1 || ((WGL_TEXT_ATTR_T*)pv_param1)->t_font_attr_type != WGL_TEXT_ATTR_REORDER)
            {
                DBG_ERROR(("{WGL(LB)} ERR: Invalid arguments. For WGL_CMD_GL_SET_TEXT_ATTR, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            else
            {
                if(((WGL_TEXT_ATTR_T*)pv_param1)->pv_attr_data == NULL)
                {
                    /*default value*/
                    (*ppt_ctx)->t_arab_reorder_attr.b_find_init_level = TRUE;
                    (*ppt_ctx)->t_arab_reorder_attr.ui1_embedding_level = 0;
     
                }else
                {
                    (*ppt_ctx)->t_arab_reorder_attr.b_find_init_level = FALSE;
                    (*ppt_ctx)->t_arab_reorder_attr.ui1_embedding_level = *((UINT8*)((WGL_TEXT_ATTR_T*)pv_param1)->pv_attr_data);
                }
            }
            
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CMD_GL_SET_TEXT_ATTR, pv_param1, pv_param2, i4_ret_fnl);
        }
        break;
        case WGL_CMD_GL_GET_TEXT_ATTR:
        {
            if(NULL == pv_param1 || 
              ((WGL_TEXT_ATTR_T*)pv_param1)->t_font_attr_type != WGL_TEXT_ATTR_REORDER ||
              ((WGL_TEXT_ATTR_T*)pv_param1)->pv_attr_data == NULL)
            {
                DBG_ERROR(("{WGL(LB)} ERR: Invalid arguments. For WGL_CMD_GL_GET_TEXT_ARAB_ATTR, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

           *((UINT8*)((WGL_TEXT_ATTR_T*)pv_param1)->pv_attr_data) = (*ppt_ctx)->t_arab_reorder_attr.ui1_embedding_level;
           
           DO_LB_CNT_CMD(*ppt_ctx, WGL_CMD_GL_GET_TEXT_ATTR, pv_param1, pv_param2, i4_ret_fnl);
        }
        break;
        
        case WGL_CMD_LB_SET_ITEM_IMAGE:
        {
            /*WGL_PACK_2(element-index, item-index)*/
            UINT16      ui2_idx_elem = WGL_GET_HI_WORD(pv_param1);
            UINT8       ui1_idx_col  = WGL_GET_LOW_LOW_BYTE(pv_param1);
            LB_ELEM_T   *pt_elem;

            if( ui2_idx_elem >= _LB_GET_ELEMS_NUM(*ppt_ctx) || 
                ui1_idx_col >= (*ppt_ctx)->ui1_col_num )
            {
                return WGLR_INV_ARG;
            }
            
            if( !IS_COL_IMG(*ppt_ctx, ui1_idx_col) )
            {
                return WGLR_INV_CMD;
            }

            _lb_get_elem(*ppt_ctx, ui2_idx_elem, &pt_elem, NULL);
            if(pt_elem == NULL)
            {
                return WGLR_OK;
            }
            
            if( pv_param2 == NULL)
            {   /*sets an empty image*/
                pt_elem->at_items[ui1_idx_col].h_img = NULL_HANDLE;
            }
            else
            {
                HANDLE_T h_img = (HANDLE_T)pv_param2;
                if(!_lb_is_valid_image_handle(h_img))
                {
                    return WGLR_INV_ARG;
                }
                pt_elem->at_items[ui1_idx_col].h_img = h_img;
            }
            _LB_SET_ELEM_STATE(*ppt_ctx, ui2_idx_elem,LBEST_CHACHED);
        }
        break;

        case WGL_CMD_LB_GET_ITEM_IMAGE:
        {
            /*WGL_PACK_2(element-index, item-index)*/
            UINT16      ui2_idx_elem = WGL_GET_HI_WORD(pv_param1);
            UINT8       ui1_idx_col  = WGL_GET_LOW_LOW_BYTE(pv_param1);
            LB_ELEM_T   *pt_elem;

            if( ui2_idx_elem >= _LB_GET_ELEMS_NUM(*ppt_ctx) || 
                ui1_idx_col >= (*ppt_ctx)->ui1_col_num   || 
                pv_param2 == NULL)
            {
                return WGLR_INV_ARG;
            }
            
            if( !IS_COL_IMG(*ppt_ctx, ui1_idx_col) )
            {
                return WGLR_INV_CMD;
            }

            _lb_get_elem(*ppt_ctx, ui2_idx_elem, &pt_elem, NULL);
            if(pt_elem == NULL)
            {
                return WGLR_OUT_OF_CACHED;
            }
             
            *((HANDLE_T *)pv_param2) = pt_elem->at_items[ui1_idx_col].h_img;
        }
        break;

        case WGL_CMD_LB_SET_ITEM_IMAGE_SUIT:
        {
            /*WGL_PACK_2(element-index, item-index)*/
            UINT16      ui2_idx_elem = WGL_GET_HI_WORD(pv_param1);
            UINT8       ui1_idx_col  = WGL_GET_LOW_LOW_BYTE(pv_param1);
            LB_ELEM_T   *pt_elem;
            
            if( ui2_idx_elem >= _LB_GET_ELEMS_NUM(*ppt_ctx) || 
                ui1_idx_col >= (*ppt_ctx)->ui1_col_num )
            {
                return WGLR_INV_ARG;
            }
            
            if( !IS_COL_IMG_SUIT(*ppt_ctx, ui1_idx_col) )
            {
                return WGLR_INV_CMD;
            }
            
            _lb_get_elem(*ppt_ctx, ui2_idx_elem, &pt_elem,NULL);
            if(pt_elem == NULL)
            {
                return WGLR_OUT_OF_CACHED;
            }

            if(pv_param2==NULL)
            {
                _IMG_SUIT_COPY(pt_elem->at_items[ui1_idx_col].pt_img_suit, &NULL_LB_IMG_SUIT);
            }
            else
            {
                WGL_LB_ITEM_IMG_SUIT_T *pt_img_suit = (WGL_LB_ITEM_IMG_SUIT_T *)pv_param2;
                if(!_lb_is_valid_image_suit(pt_img_suit))
                {
                    return WGLR_INV_ARG;
                }
                _IMG_SUIT_COPY(pt_elem->at_items[ui1_idx_col].pt_img_suit, pt_img_suit);
            }
            _LB_SET_ELEM_STATE(*ppt_ctx, ui2_idx_elem,LBEST_CHACHED);
            
        }
        break;

        case WGL_CMD_LB_GET_ITEM_IMAGE_SUIT:
        {
            /*WGL_PACK_2(element-index, item-index)*/
            UINT16      ui2_idx_elem = WGL_GET_HI_WORD(pv_param1);
            UINT8       ui1_idx_col  = WGL_GET_LOW_LOW_BYTE(pv_param1);
            LB_ELEM_T   *pt_elem;
            
            if( ui2_idx_elem >= _LB_GET_ELEMS_NUM(*ppt_ctx) || 
                ui1_idx_col >= (*ppt_ctx)->ui1_col_num   || 
                pv_param2 == NULL)
            {
                return WGLR_INV_ARG;
            }
            
            if( !IS_COL_IMG_SUIT(*ppt_ctx, ui1_idx_col) )
            {
                return WGLR_INV_CMD;
            }

            _lb_get_elem(*ppt_ctx, ui2_idx_elem, &pt_elem, NULL);
            if(pt_elem == NULL)
            {
                return WGLR_OUT_OF_CACHED;
            }

            _IMG_SUIT_COPY(pv_param2, pt_elem->at_items[ui1_idx_col].pt_img_suit);
            
        }
        break;

#ifdef WGL_ANIM_SUPPORT
        case WGL_CMD_LB_CREAT_ANIM:
        {
            UINT32  ui4_style = (UINT32)pv_param1;

            i4_ret_fnl = _lb_create_anim((*ppt_ctx), TRUE, ui4_style);            
        }
        break;

        case WGL_CMD_LB_DESTROY_ANIM:
        {
            i4_ret_fnl = _lb_destroy_anim(*ppt_ctx);
        }
        break;
/*
        case WGL_CMD_LB_SET_ANIM_MARGIN:
        {
            
        }
        break;

        case WGL_CMD_LB_GET_ANIM_MARGIN:
        {
            
        }
        break;
*/
        case WGL_CMD_LB_SET_ANIM_ACTMAP:
        {
            WGL_ANIM_ACTION_MAP_T*  pt_actmap = (WGL_ANIM_ACTION_MAP_T*)pv_param1;
            WGL_ANIM_ACTION_MAP_T*  pt_dst = NULL;
            WGL_ANIM_PATH_DATA_T*   pt_path_data = NULL;
            UINT16                  ui2_cnt;
            UINT32                  ui4_size = 0;

            if((pt_actmap == NULL) || 
               (pt_actmap->e_anim_action_type > WGL_LB_ANIM_ACT_END) )
            {
                return WGLR_INV_ARG;
            }

            if(HAS_FLAG((*ppt_ctx)->ui1_state, LBST_ANIM_CREATED))
            {
                pt_dst = &((*ppt_ctx)->at_actmap[pt_actmap->e_anim_action_type]);

                if(HAS_STYLE((*ppt_ctx), WGL_STL_LB_ANIM_BY_CUST_PATH) &&
                   HAS_FLAG(pt_actmap->t_anim_data.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF))
                {
                    ui2_cnt = pt_actmap->t_anim_data.ui2_count;
                    if(pt_dst->t_anim_data.ui2_count < ui2_cnt)
                    {
                        ui4_size = sizeof(WGL_ANIM_PATH_DATA_T) * ui2_cnt;
                        pt_path_data = (WGL_ANIM_PATH_DATA_T*)WGL_MEM_ALLOC(ui4_size);
                        if(pt_path_data == NULL)
                        {
                            return WGLR_INV_ARG;
                        }
                        if(pt_dst->t_anim_data.pt_path_mode != NULL)
                        {
                            WGL_MEM_FREE(pt_dst->t_anim_data.pt_path_mode);
                            #ifdef CLI_LVL_ALL     
                            (*ppt_ctx)->ui4_mem_size -= sizeof(WGL_ANIM_PATH_DATA_T) * pt_dst->t_anim_data.ui2_count;
                            #endif  
                        }
                        pt_dst->t_anim_data.pt_path_mode = pt_path_data;  
                        pt_dst->t_anim_data.ui2_count = ui2_cnt;
                        #ifdef CLI_LVL_ALL     
                        (*ppt_ctx)->ui4_mem_size += ui4_size;
                        #endif  
                    }
                    x_memcpy(pt_dst->t_anim_data.pt_path_mode,
                             pt_actmap->t_anim_data.pt_anim_data,
                             sizeof(WGL_ANIM_PATH_DATA_T)*ui2_cnt);
                }
                else if(HAS_FLAG(pt_actmap->t_anim_data.ui4_flag , WGL_ANIM_DATA_FLAG_PATH_ID_INF))
                {
                    x_memcpy(pt_dst,pt_actmap,sizeof(WGL_ANIM_ACTION_MAP_T));
                }
                else
                {
                    return WGLR_INV_ARG;
                }
                    
            }
            else
            {
                return WGLR_INV_ARG;
            }
        }
        break;

        case WGL_CMD_LB_GET_ANIM_ACTMAP:
        {
            WGL_ANIM_ACTION_MAP_T*  pt_actmap = (WGL_ANIM_ACTION_MAP_T*)pv_param1;

            if((pt_actmap == NULL) || 
               (pt_actmap->e_anim_action_type > WGL_LB_ANIM_ACT_END) )
            {
                return WGLR_INV_ARG;
            }            

            if(HAS_FLAG((*ppt_ctx)->ui1_state, LBST_ANIM_CREATED))
            {
                pt_actmap = &((*ppt_ctx)->at_actmap[pt_actmap->e_anim_action_type]);
                x_memcpy(pt_actmap,
                         &((*ppt_ctx)->at_actmap[pt_actmap->e_anim_action_type]),
                         sizeof(WGL_ANIM_ACTION_MAP_T));
            }
 
        }
        break;

        case WGL_CMD_LB_ANIM_SET_IMG_SEQ:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            i4_ret = wgl_anim_cmd_proc((*ppt_ctx)->t_anim,
                           WGL_CMD_ANIM_ATTR_SET_IMG_SEQ, 
                           pv_param1,
                           pv_param2);
            INFORM_ERR(i4_ret);
        }
        break;

        case WGL_CMD_LB_ANIM_SET_MAX_IMG_SEQ:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            i4_ret = wgl_anim_cmd_proc((*ppt_ctx)->t_anim,
                           WGL_CMD_ANIM_ATTR_SET_MAX_IMG_SEQ, 
                           pv_param1,
                           NULL);
            INFORM_ERR(i4_ret);

        }
        break;

        case WGL_CMD_LB_ANIM_SET_FLAG:
        {
            BOOL                        b_set = WGL_POINTER_TO_BOOL(pv_param1);
            UINT32                      ui4_flag = (UINT32)pv_param2;
            
            if(!HAS_FLAG(ui4_flag,WGL_LB_ANIM_FLAG_DISABLE))
            {
                return WGLR_INV_ARG;
            }
#ifdef WGL_LB_DEFAULT_NO_ANIMATION
           return WGLR_OK;
#else            
            if(b_set == TRUE)
            {
                WGL_SET_FLAG((*ppt_ctx)->ui4_anim_flag,ui4_flag);
            }
            else
            {
                WGL_CLEAR_FLAG((*ppt_ctx)->ui4_anim_flag,ui4_flag);
            }
#endif            
            
        }
        break;
#endif

        case WGL_CMD_LB_SET_NTF_CB:
        {
            if(pv_param1 == NULL)
            {
                return WGLR_INV_ARG;
            }
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CMD_LB_SET_NTF, pv_param1, NULL, i4_ret_fnl);
        }
        break;

#if 0
        case WGL_CMD_LB_LBW_RLD:
        {
            if(pv_param1 == NULL)
            {
                return WGLR_INV_ARG;
            }
            
            _lb_lbw_reload(*ppt_ctx, (WGL_LB_RLD_INF_T*)pv_param1);            
        }
        break;

        case WGL_CMD_LB_LBW_RLD_PARTIAL:
        {
        }
        break;

        case WGL_CMD_LB_GET_ELEM_TOTAL_NUM:
        {
        }
        break;
#endif        
#ifdef CLI_LVL_ALL
        case WGL_CMD_GL_GET_ALLOC_MEM:
        {
            UINT32      ui4_wgl_mem = (*ppt_ctx)->ui4_mem_size;
            UINT32      ui4_mem_size = 0;
            WGL_MTX_OBJ_T*   pt_wgt_mem ;

            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(LB)} ERR: Invalid arguments for WGL_CMD_GL_GET_ALLOC_MEM. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
            }

#ifdef WGL_ANIM_SUPPORT    
           pt_wgt_mem = (WGL_MTX_OBJ_T*)(*ppt_ctx)->t_anim;
           ui4_wgl_mem += pt_wgt_mem->ui4_mem_size;
#endif

           /*Get the listbox context memory alloc size*/
           DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_CNT_MEM_ALLOC, &ui4_mem_size, NULL, i4_ret_fnl);
           CATCH_FAIL(i4_ret_fnl);
           ui4_wgl_mem += ui4_mem_size;

           /*Get the listbox border memory size*/
           if((*ppt_ctx)->pv_ctx_bdr != NULL)
           {
               pt_wgt_mem = (WGL_MTX_OBJ_T* )(*ppt_ctx)->pv_ctx_bdr;
               ui4_wgl_mem  += pt_wgt_mem->ui4_mem_size;   
           }

           *(UINT32 *)pv_param1 = ui4_wgl_mem ;
        }
       break;
#endif        

        case WGL_CMD_LB_GET_VISIBLE_RANGE:
        {
            if(pv_param1 == NULL)
            {
                return WGLR_INV_ARG;
            }

            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_VISIBLE_RANGE, pv_param1, NULL, i4_ret_fnl);
        }
        break;

        default:
        {
            i4_ret_fnl = WGLR_INV_ARG;
        }

    }
    
    return i4_ret_fnl;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_insert_elem
 *
 * Description: This API inserts an element. 
 *
 * Inputs:  ppt_ctx      Contains the pointer of the reference to the instance context.
 *          ui2_idx      Specifies the element index of the insert position. 
 *                       This argument is valid only when b_is_append is FALSE.
 *          at_items     Specifies the item info array for the new element. 
 *                       If the argument is NULL, an empty element would be inserted.
 *          b_is_append  Specifies if the element is to be appended at the tail or not.
 *
 * Outputs: ppt_ctx      Contains the pointer of the reference to the new instance context.
 *
 * Returns: WGLR_OK             Routine successful.
 *          WGLR_INV_CMD        Invalid command leading to insert the new element
 *          WGLR_INV_ARG        Invalid arguement.
 *          WGLR_OUT_OF_MEMORY  Out of memory.
 *          WGLR_INTERNAL_ERROR Some internal error.
 *          WGLR_INV_HANDLE     The listbox has been destroyed.
 *          < 0                 Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_insert_elem(   
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      UINT16                      ui2_idx, 
    IN      WGL_LB_ITEM_T               *at_items, 
    IN      BOOL                        b_is_append)
{
    INT32                   i4_ret;
    LB_ELEM_T               *pt_elem = NULL;
    BOOL                    b_need_notify_hlt  = FALSE;
    BOOL                    b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
    UINT16                  ui2_idx_notify_hlt = WGL_LB_NULL_INDEX;
    UINT16                  ui2_idx_new;
    LB_SEL_NOTIFY_CTX_T     t_sel_notify = {WGL_LB_NULL_INDEX, WGL_LB_NULL_INDEX};
    HANDLE_T                h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
    UINT16                  ui2_cache_s, ui2_cache_e;
    UINT16                  ui2_num_vp_elem;
    LB_ELEM_INF_CONTENT     t_content;

    if( HAS_STYLE(*ppt_ctx, WGL_STL_LB_FIXED_ELEM_NUM) ||
        (_LB_GET_ELEMS_NUM(*ppt_ctx) >= _LB_GET_MAX_ELEMS_NUM(*ppt_ctx)))
    {
        return WGLR_INV_CMD;
    }
    
    if( !b_is_append)
    { /*only for insertion*/
        if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
        {
            return WGLR_INV_ARG;
        }
    }

    /*check the validation of the items */
    if( at_items != NULL )
    {
        CATCH_FAIL(_lb_chk_items_data(*ppt_ctx, at_items));
    }

    /*Get the insert/append position*/
    if( _LB_GET_ELEMS_NUM(*ppt_ctx) == 0 )
    {
        ui2_idx_new          = 0;
    }
    else if(b_is_append)
    {
        ui2_idx_new      = _LB_GET_ELEMS_NUM(*ppt_ctx);
    }
    else     
    {  
        ui2_idx_new      = ui2_idx;
    }

    ui2_cache_s = _LB_GET_CACHE_START(*ppt_ctx);
    ui2_cache_e = ui2_cache_s + _LB_GET_ELEM_CACHE_NUM(*ppt_ctx); 
    DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_VISIBLE_RANGE, NULL, &ui2_num_vp_elem, i4_ret);
    CATCH_FAIL(i4_ret);

    i4_ret = _LB_ADD_ELEM_INF(*ppt_ctx, ui2_idx_new, NULL);
    if(i4_ret != WGLR_OK)
    {
        return WGLR_INTERNAL_ERROR;
    }

    ui2_num_vp_elem++;

    if( (_LB_GET_ELEMS_NUM(*ppt_ctx) <= ui2_num_vp_elem) ||
        (ui2_idx_new >= ui2_cache_s && ui2_idx_new < ui2_cache_e) ||
        (b_is_append && !HAS_STYLE((*ppt_ctx),WGL_STL_LB_LBW)) ) 
    {
        i4_ret = _lb_create_elem_data(*ppt_ctx,at_items, ui2_idx_new, &pt_elem, b_is_append);
        if(i4_ret != WGLR_OK)
        {
            return WGLR_INTERNAL_ERROR;
        }
    }
    else if(ui2_idx_new < ui2_cache_s)
    {
        _LB_INC_ELEM_CACHE_NUM(*ppt_ctx);
    }

    /*ensure the consistency between LB_CTX_T.ui2_idx and the element state*/
    if( !b_is_append && _HAS_HLT_ELEM(*ppt_ctx) && ui2_idx <= (*ppt_ctx)->ui2_idx )
    {
        (*ppt_ctx)->ui2_idx++;
    }   
    /*if force highlighting mode and no element is highlighted, highlight the element*/
    else if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_FORCE_HLT) && !_HAS_HLT_ELEM(*ppt_ctx)) 
    {
        (*ppt_ctx)->ui2_idx = ui2_idx_new;
        _LB_SET_ELEM_STATE(*ppt_ctx, ui2_idx_new, LBEST_HLT);
        b_need_notify_hlt   = TRUE;
        ui2_idx_notify_hlt  = ui2_idx_new;
    }

    /*if the element is highlighted and auto selection mode is set, then auto select the element*/
    if( b_need_notify_hlt && HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL) )
    {
        CATCH_FAIL(_lb_tempunlock_sel_elem(ppt_ctx, FALSE, (*ppt_ctx)->ui2_idx, &t_sel_notify));
    }
    /*if force selection mode and no element is selected, select the element*/
    else if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_FORCE_SEL) && (*ppt_ctx)->ui2_sel_num==0 )
    {
        _LB_SET_ELEM_STATE(*ppt_ctx, ui2_idx_new, LBEST_SEL);
        (*ppt_ctx)->ui2_sel_num ++;
        t_sel_notify.ui2_idx_new_sel = ui2_idx_new;
    }

    /*inform Content to append an element */
    t_content.pt_elem_inf_list = &((*ppt_ctx)->t_elem_inf_list);
    t_content.pt_elems = (*ppt_ctx)->pt_elems;
    t_content.ui2_elem_num = _LB_GET_ELEMS_NUM(*ppt_ctx);
    t_content.ui2_cache_start = _LB_GET_CACHE_START(*ppt_ctx);
    t_content.ui2_cache_num = _LB_GET_ELEM_CACHE_NUM(*ppt_ctx);
    DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_ADD_ELEM, &t_content, pt_elem, i4_ret);
    CATCH_FAIL(i4_ret);

    /*Ensure the element that is in the viewport should be allocate memory,and
     * free the element data that is not in the vp*/
    _lb_check_cache_for_vp(*ppt_ctx);
  
    if(!b_suppress_notify)
    {
        REL_LOCK(h_lb, *ppt_ctx);
        
        if( b_need_notify_hlt )
        {
            /*send WGL_NC_LB_ELEM_HLT(ui2_idx_notify_hlt, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_idx_notify_hlt, TRUE) );
        }
        if( t_sel_notify.ui2_idx_prev_sel!=WGL_LB_NULL_INDEX)
        {
            /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_prev_sel, FALSE) notification*/
            CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_prev_sel, FALSE) );
        }
        if( t_sel_notify.ui2_idx_new_sel!=WGL_LB_NULL_INDEX)
        {
            /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_new_sel, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_new_sel, TRUE));
        }

        GET_LOCK(h_lb, ppt_ctx);
    }

    _LB_CHECK_CONSISTENCY(*ppt_ctx);

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_del_elem
 *
 * Description: This API deletes an element.
 *
 * Inputs:  ppt_ctx  Contains the pointer of the instance context reference.
 *          ui2_idx  Specifies the index of the element to be deleted.
 *
 * Outputs: ppt_ctx  Contains the pointer of the instance context reference.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_CMD     Invalid commands leading to this API.
 *          WGLR_INV_ARG     Invalid arguments.
 *          WGLR_INV_HANDLE  The listbox has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_del_elem( 
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      UINT16                      ui2_idx)
{
    INT32                   i4_ret;
    LB_ELEM_T               *pt_elem = NULL;
    BOOL                    b_need_notify_hlt = FALSE;
    BOOL                    b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
    UINT16                  ui2_idx_hlt = WGL_LB_NULL_INDEX;
    UINT16                  ui2_idx_sel = WGL_LB_NULL_INDEX;
    HANDLE_T                h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
    LB_SEL_NOTIFY_CTX_T     t_sel_notify = {WGL_LB_NULL_INDEX, WGL_LB_NULL_INDEX};
    UINT16                  ui2_cache_s, ui2_cache_e;
    UINT8                   ui1_state;
    LB_ELEM_INF_CONTENT     t_content;

    if( HAS_STYLE(*ppt_ctx, WGL_STL_LB_FIXED_ELEM_NUM) )
    {
        return WGLR_INV_CMD;
    }
    if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
    {
        return WGLR_INV_ARG;
    }

    ui1_state = _LB_GET_ELEM_STATE(*ppt_ctx, ui2_idx);
    WS_ASSERT( !( ((*ppt_ctx)->ui2_idx!=ui2_idx && HAS_FLAG(ui1_state, LBEST_HLT)) ||
                  ((*ppt_ctx)->ui2_idx==ui2_idx && !HAS_FLAG(ui1_state, LBEST_HLT)) ) );

    ui2_cache_s = _LB_GET_CACHE_START(*ppt_ctx);
    ui2_cache_e = ui2_cache_s + _LB_GET_ELEM_CACHE_NUM(*ppt_ctx); 

    if( (_LB_GET_ELEMS_NUM(*ppt_ctx) > 0 ) && 
        (ui2_idx>= ui2_cache_s && ui2_idx < ui2_cache_e)  )
    {
        _lb_destroy_elem_data(*ppt_ctx, ui2_idx, NULL);
    }
    else if(ui2_idx < ui2_cache_s)
    {
         _LB_DEC_CACHE_START(*ppt_ctx);
    }
    
     _LB_DEL_ELEM_INF(*ppt_ctx, ui2_idx);

    /*if the deleted element is highlighted*/
    if( (*ppt_ctx)->ui2_idx == ui2_idx ) 
    {
        (*ppt_ctx)->ui2_idx = WGL_LB_NULL_INDEX;
        CATCH_FAIL(_lb_lose_hlt_elem(*ppt_ctx, &b_need_notify_hlt, &ui2_idx_hlt));
    }
    /*ensure the consistency between LB_CTX_T.ui2_idx and the element state*/
    else if( _HAS_HLT_ELEM(*ppt_ctx) && ui2_idx<(*ppt_ctx)->ui2_idx )
    {
        (*ppt_ctx)->ui2_idx --;
    }

    /*if the deleted element is selected*/
    if(HAS_FLAG(ui1_state, LBEST_SEL))
    {
        (*ppt_ctx)->ui2_sel_num --;
    }

    if( b_need_notify_hlt && HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL) )
    {
        CATCH_FAIL(_lb_tempunlock_sel_elem(ppt_ctx, FALSE, ui2_idx_hlt, &t_sel_notify));
    }
    else if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_FORCE_SEL) && (*ppt_ctx)->ui2_sel_num==0 )
    {
        /*if in force selection mode and no element is selected now, 
            automatically pick one to be selected*/
        UINT8*                    pui1_tmp;

        ui2_idx_sel = 0;
        _LB_FIND_NO_ELEM_ST_INF((*ppt_ctx),LBEST_SEL,0,1,TRUE,&ui2_idx_sel, &pui1_tmp);
        if( pui1_tmp )
        {
            WGL_SET_FLAG(*pui1_tmp, LBEST_SEL);
            (*ppt_ctx)->ui2_sel_num++;
            t_sel_notify.ui2_idx_new_sel = ui2_idx_sel;
        }
    }

    /*inform Content to delete an element */
    t_content.pt_elem_inf_list = &((*ppt_ctx)->t_elem_inf_list);
    t_content.pt_elems = (*ppt_ctx)->pt_elems;
    t_content.ui2_elem_num = _LB_GET_ELEMS_NUM(*ppt_ctx);
    t_content.ui2_cache_start = _LB_GET_CACHE_START(*ppt_ctx);
    t_content.ui2_cache_num = _LB_GET_ELEM_CACHE_NUM(*ppt_ctx);
    DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_DEL_ELEM, &t_content, pt_elem, i4_ret);
    CATCH_FAIL(i4_ret);
    
    /*Put the element into the element-free-list*/
    if( pt_elem )
    {
        _LB_ELEM_FREE_LIST_INS((*ppt_ctx)->pt_elems_free_list, pt_elem);
    }

    if(!b_suppress_notify)
    {
        REL_LOCK(h_lb, *ppt_ctx); 
        
        if(b_need_notify_hlt )
        {
            /* send WGL_NC_LB_ELEM_HLT(ui2_idx_hlt, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_idx_hlt, TRUE) );
        }
        if(t_sel_notify.ui2_idx_prev_sel != WGL_LB_NULL_INDEX)
        {
            /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_prev_sel, FALSE) notification*/
            CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_prev_sel, FALSE) );
        }
        if(t_sel_notify.ui2_idx_new_sel != WGL_LB_NULL_INDEX)
        {
            /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_new_sel, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_new_sel, TRUE));
        }
        
        GET_LOCK(h_lb, ppt_ctx); 
    }
    
    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_clear
 *
 * Description: This API clears all elements.
 *
 * Inputs:  pt_ctx  Contains the instance context.
 *
 * Outputs: pt_ctx  Contains the instance context.
 *
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    Invalid arguments.
 *          < 0             Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_clear(
    IN OUT  LB_CTX_T                    *pt_ctx)
{
    UINT16          ui2_idx;
    LB_ELEM_T       *pt_elem;
    LB_ELEM_T       *pt_next;
    INT32           i4_ret;

    CHECK_VALID( !HAS_STYLE(pt_ctx, WGL_STL_LB_FIXED_ELEM_NUM), WGLR_INV_ARG );

    DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_CLEAR, NULL, NULL, i4_ret);
    CATCH_FAIL(i4_ret);

    /*Clear the element data information */
    pt_elem = pt_ctx->pt_elems;
    for(ui2_idx=0; ui2_idx<_LB_GET_ELEM_CACHE_NUM(pt_ctx); ui2_idx++)
    {
        pt_next = pt_elem->pt_next;
        /*Put the element into the element-free-list*/
        _LB_ELEM_FREE_LIST_INS(pt_ctx->pt_elems_free_list, pt_elem);
        pt_elem = pt_next;
    }
    pt_ctx->pt_elems     = NULL;
    _LB_SET_CACHE_START(pt_ctx, 0);
    _LB_SET_ELEM_CACHE_NUM(pt_ctx, 0);

    /*Clear the element information list*/
    while(_LB_GET_ELEMS_NUM(pt_ctx) > 0)
    {
        _LB_DEL_ELEM_INF(pt_ctx, 0);
    }

    pt_ctx->ui2_idx      = WGL_LB_NULL_INDEX;
    pt_ctx->ui2_sel_num  = 0;

    _LB_CHECK_CONSISTENCY(pt_ctx);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_highlight_elem
 *
 * Description: This API highlights an element.
 *
 * Inputs:  ppt_ctx      Contains the pointer of the instance context reference
 *          ui2_idx_new  Specifies the index of the element to be highlighted.
 *          pt_elem_new  Contains the info of the element to be highlighted if 
 *                       the argument is not NULL. This argument can be NULL.
 *
 * Outputs: ppt_ctx  Contains the pointer of the instance context reference
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
/*pt_elem_new can be NULL*/
#ifdef WGL_ANIM_SUPPORT
static INT32 _lb_tempunlock_highlight_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_idx_new,
    IN      LB_ELEM_T                   *pt_elem_new,
    IN      BOOL                        b_not_do_nfy,
    IN      WGL_ANIM_ACTION_TYPE_T      e_action_type)
{
    INT32                 i4_ret;
    UINT16                ui2_idx_old;
    HANDLE_T              h_lb, h_parent;
    LB_SEL_NOTIFY_CTX_T   t_sel_notify = {WGL_LB_NULL_INDEX, WGL_LB_NULL_INDEX};
    BOOL                  b_suppress_notify;
    UINT8*                pui2_new_state;
    UINT8*                pui2_old_state;
    
    CHECK_NULL(ppt_ctx,WGLR_INV_ARG);
    CHECK_NULL(*ppt_ctx,WGLR_INV_ARG);
    ui2_idx_old  = (*ppt_ctx)->ui2_idx;
    h_lb = (*ppt_ctx)->h_lb;
    h_parent = (*ppt_ctx)->h_parent;
    b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;

    if(ui2_idx_new == ui2_idx_old)
    {
        return WGLR_OK;
    }

    pui2_new_state = _LB_GET_ELEM_STATE_P(*ppt_ctx, ui2_idx_new);
    if(pui2_new_state == NULL)
    {
        return WGLR_INV_ARG;
    }

    if(HAS_FLAG(*pui2_new_state, LBEST_DISABLED))
    { /* the element is disabled, this command causes no effects*/
        return WGLR_OK;
    }
    
    /* Unhighlight the originally highlighted element*/
    if(_HAS_HLT_ELEM(*ppt_ctx))
    {
        pui2_old_state = _LB_GET_ELEM_STATE_P(*ppt_ctx, (*ppt_ctx)->ui2_idx);

        if(pui2_old_state != NULL )
        {
            WS_ASSERT(HAS_FLAG(*pui2_old_state, LBEST_HLT));
            WGL_CLEAR_FLAG(*pui2_old_state, LBEST_HLT);
        }

        if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE))
        {
            /* inform an element is unhighlighted, */
            /* to ensure the enlarged unhighlighted element will be correctly repainted */
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_REQ_UPDATE_UNHLT_ELEM, ui2_idx_old, NULL, i4_ret);
            CATCH_FAIL(i4_ret);
        }
        
        if(_LB_ANIM_EN(*ppt_ctx) && (e_action_type != WGL_ANIM_ACTION_INGORE))
        {
            _lb_elem_set_anim_info(*ppt_ctx, (*ppt_ctx)->ui2_idx, ui2_idx_new, NULL);
        }

    }
    else if(_LB_ANIM_EN(*ppt_ctx) && (e_action_type != WGL_ANIM_ACTION_INGORE))
    {
        _lb_elem_set_anim_info(*ppt_ctx, ui2_idx_new, ui2_idx_new, NULL);
    }
    
    if(!_LB_ANIM_EN(*ppt_ctx) || (e_action_type == WGL_ANIM_ACTION_INGORE))
    {
        WGL_SET_FLAG(*pui2_new_state, LBEST_HLT);
        (*ppt_ctx)->ui2_idx = ui2_idx_new;
    }
    
    if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL))
    {
        CATCH_FAIL(_lb_tempunlock_sel_elem(ppt_ctx, FALSE, (*ppt_ctx)->ui2_idx, &t_sel_notify));
    }
    
    if(_LB_ANIM_EN(*ppt_ctx) && (e_action_type != WGL_ANIM_ACTION_INGORE))
    {
        _lb_elem_begin_anim(ppt_ctx, e_action_type);   
    }

    CHECK_NULL(ppt_ctx,WGLR_INV_ARG);
    CHECK_NULL(*ppt_ctx,WGLR_INV_ARG);
        
    if((!b_suppress_notify) && (!b_not_do_nfy))
    {
        REL_LOCK(h_lb, *ppt_ctx);
        
        if(ui2_idx_old != WGL_LB_NULL_INDEX )
        {
            /*send WGL_NC_LB_ELEM_HLT(ui2_idx_old, FALSE) notification*/
            CATCH_FAIL(_lb_send_notify_hlt(h_parent, h_lb, ui2_idx_old, FALSE));
        }
        
        /*send WGL_NC_LB_ELEM_HLT(ui2_idx_new, TRUE) notification*/
        CATCH_FAIL(_lb_send_notify_hlt(h_parent, h_lb, ui2_idx_new, TRUE));
        
        if(t_sel_notify.ui2_idx_prev_sel != WGL_LB_NULL_INDEX)
        {
            /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_prev_sel, FALSE) notification*/
            CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_prev_sel, FALSE) );
        }
        if(t_sel_notify.ui2_idx_new_sel != WGL_LB_NULL_INDEX)
        {
            /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_new_sel, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_new_sel, TRUE));
        }
        
        GET_LOCK(h_lb, ppt_ctx);
    }

    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;

}
#else
static INT32 _lb_tempunlock_highlight_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_idx_new,
    IN      LB_ELEM_T                   *pt_elem_new,
    IN      BOOL                        b_not_do_nfy)
{
    INT32                 i4_ret;
    UINT16                ui2_idx_old  = (*ppt_ctx)->ui2_idx;
    HANDLE_T              h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
    LB_SEL_NOTIFY_CTX_T   t_sel_notify = {WGL_LB_NULL_INDEX, WGL_LB_NULL_INDEX};
    BOOL                  b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
    UINT8*                pui1_new_state;
    UINT8*                pui1_old_state;
    
    if(ui2_idx_new == (*ppt_ctx)->ui2_idx)
    {
        return WGLR_OK;
    }
    
    pui1_new_state = _LB_GET_ELEM_STATE_P(*ppt_ctx, ui2_idx_new);
    if(pui1_new_state == NULL)
    {
        return WGLR_INV_ARG;
    }

    if(HAS_FLAG(*pui1_new_state, LBEST_DISABLED))
    { /* the element is disabled, this command causes no effects*/
        return WGLR_OK;
    }
    
    /* Unhighlight the originally highlighted element*/
    if(_HAS_HLT_ELEM(*ppt_ctx))
    {
        pui1_old_state = _LB_GET_ELEM_STATE_P(*ppt_ctx, (*ppt_ctx)->ui2_idx);

        if(pui1_old_state != NULL )
        {
            WS_ASSERT(HAS_FLAG(*pui1_old_state, LBEST_HLT));
            WGL_CLEAR_FLAG(*pui1_old_state, LBEST_HLT);
        }

        if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE))
        {
            /* inform an element is unhighlighted, */
            /* to ensure the enlarged unhighlighted element will be correctly repainted */
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_REQ_UPDATE_UNHLT_ELEM, ui2_idx_old, NULL, i4_ret);
            CATCH_FAIL(i4_ret);
        }
    }
    
    WGL_SET_FLAG(*pui1_new_state, LBEST_HLT);
    (*ppt_ctx)->ui2_idx = ui2_idx_new;
    if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL))
    {
        CATCH_FAIL(_lb_tempunlock_sel_elem(ppt_ctx, FALSE, (*ppt_ctx)->ui2_idx, &t_sel_notify));
    }
    
    if((!b_suppress_notify) && (!b_not_do_nfy))
    {
        REL_LOCK(h_lb, *ppt_ctx);
        
        if(ui2_idx_old != WGL_LB_NULL_INDEX )
        {
            /*send WGL_NC_LB_ELEM_HLT(ui2_idx_old, FALSE) notification*/
            CATCH_FAIL(_lb_send_notify_hlt(h_parent, h_lb, ui2_idx_old, FALSE));
        }
        
        /*send WGL_NC_LB_ELEM_HLT(ui2_idx_new, TRUE) notification*/
        CATCH_FAIL(_lb_send_notify_hlt(h_parent, h_lb, ui2_idx_new, TRUE));
        
        if(t_sel_notify.ui2_idx_prev_sel != WGL_LB_NULL_INDEX)
        {
            /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_prev_sel, FALSE) notification*/
            CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_prev_sel, FALSE) );
        }
        if(t_sel_notify.ui2_idx_new_sel != WGL_LB_NULL_INDEX)
        {
            /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_new_sel, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_new_sel, TRUE));
        }
        
        GET_LOCK(h_lb, ppt_ctx);
    }

    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;

}
#endif

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_unhighlight_elem
 *
 * Description: This API unhighlights an element.
 *
 * Inputs:  ppt_ctx         Contains the pointer of the instance context reference.
 *          ui2_idx_target  Specifies the index of the element to be unhighlighted
 *
 * Outputs: ppt_ctx         Contains the pointer of the instance context reference.
 *
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    Invalid argument.
 *          WGLR_INV_CMD    Invalid commands leading to this API.
 *          WGLR_INV_HANDLE The widget has been destroyed.
 *          < 0             Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_unhighlight_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_idx_target,
    IN      BOOL                        b_not_do_nfy)
{
    HANDLE_T        h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
    BOOL            b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
    INT32           i4_ret;
    UINT8*          pui1_state;
    
    if( ui2_idx_target >= _LB_GET_ELEMS_NUM(*ppt_ctx) )
    {
        return WGLR_INV_ARG;
    }

    if( (*ppt_ctx)->ui2_idx != ui2_idx_target )
    {
        return WGLR_OK; 
    }

    if( HAS_STYLE(*ppt_ctx, WGL_STL_LB_FORCE_HLT) )
    {
        return WGLR_INV_CMD;
    }

    pui1_state = _LB_GET_ELEM_STATE_P(*ppt_ctx, ui2_idx_target);
    if(pui1_state == NULL)
    {
        return WGLR_INV_ARG;
    }

    WS_ASSERT(HAS_FLAG(*pui1_state, LBEST_HLT));
    WGL_CLEAR_FLAG(*pui1_state, LBEST_HLT);
    (*ppt_ctx)->ui2_idx = WGL_LB_NULL_INDEX;
    
    if ((!b_suppress_notify) && (!b_not_do_nfy))
    {
        REL_LOCK(h_lb, *ppt_ctx);
    
        /*send WGL_NC_LB_ELEM_HLT(ui2_idx_target, FALSE) notification*/
        CATCH_FAIL(_lb_send_notify_hlt(h_parent, h_lb, ui2_idx_target, FALSE));
        
        GET_LOCK(h_lb, ppt_ctx);
    }
    
    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_sel_elem
 *
 * Description: This API selects an element.
 *
 * Inputs:  ppt_ctx         Contains the pointer of the instance context reference.
 *          b_raise_notify  Specify whether raise notify or not. Note that 
 *                          if pt_sel_notify_out is NOT NULL, b_raise_notify must be FALSE.
 *          ui2_idx         Specifies the index of the element to be selected.
 *
 * Outputs: ppt_ctx            Contains the pointer of the instance context reference.
 *          pt_sel_notify_out  Contains the notify context.  
 *
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    Invalid arguments
 *          WGLR_INV_HANDLE The widget has been destroyed.
 *          < 0             Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_sel_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      BOOL                        b_raise_notify,
    IN      UINT16                      ui2_idx, 
    OUT     LB_SEL_NOTIFY_CTX_T         *pt_sel_notify_out)
{
    INT32               i4_ret;
    BOOL                b_need_notify_prev_sel = FALSE;
    UINT16              ui2_idx_prev_sel       = WGL_LB_NULL_INDEX;
    LB_ELEM_INF_T               *pt_elem_inf = NULL;

    CHECK_VALID( 
        ( pt_sel_notify_out == NULL) ||
        ((pt_sel_notify_out != NULL) && !b_raise_notify ), WGLR_INV_ARG);
    
    if(pt_sel_notify_out)
    {
        pt_sel_notify_out->ui2_idx_new_sel = pt_sel_notify_out->ui2_idx_prev_sel = WGL_LB_NULL_INDEX;
    }

    if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
    {
        return WGLR_INV_ARG;
    }

    if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL) && ui2_idx != (*ppt_ctx)->ui2_idx)
    {
        return WGLR_OK;
    }

    _lb_get_elem(*ppt_ctx, ui2_idx, NULL, &pt_elem_inf);
    if(pt_elem_inf == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }

    if( HAS_FLAG(pt_elem_inf->ui1_state, LBEST_SEL) )
    {
        return WGLR_OK;
    }
    
    /*check if NOT in multi-selection mode*/
    WS_ASSERT( ! ((!HAS_STYLE(*ppt_ctx, WGL_STL_LB_MULTI_SEL)) && ((*ppt_ctx)->ui2_sel_num>=2) ) );
    
    if( (! HAS_STYLE(*ppt_ctx, WGL_STL_LB_MULTI_SEL)) && (*ppt_ctx)->ui2_sel_num==1 )
    {
        UINT16      ui2_i;
        UINT8*      pui1_state_tmp = NULL;

        _LB_FIND_ELEM_ST_INF(*ppt_ctx, LBEST_SEL,0, 1,TRUE, &ui2_i, &pui1_state_tmp);
        if(ui2_i >= _LB_GET_ELEMS_NUM(*ppt_ctx) || (pui1_state_tmp == NULL) )
        {
            return WGLR_INTERNAL_ERROR;
        }
        
        if(ui2_i==ui2_idx)
        {   /*Do a quick return*/
            return WGLR_OK;
        }

        WGL_CLEAR_FLAG(*pui1_state_tmp, LBEST_SEL);
        (*ppt_ctx)->ui2_sel_num --;
        b_need_notify_prev_sel = TRUE;
        ui2_idx_prev_sel       = ui2_i;
    }
    
    /*select the element*/
    WGL_SET_FLAG(pt_elem_inf->ui1_state, LBEST_SEL);
    (*ppt_ctx)->ui2_sel_num++;
    
    if(pt_sel_notify_out == NULL)
    {
        if(b_raise_notify)
        {
            HANDLE_T    h_lb = (*ppt_ctx)->h_lb, h_parent=(*ppt_ctx)->h_parent;
            BOOL        b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;

            if (!b_suppress_notify)
            {
                REL_LOCK(h_lb, *ppt_ctx);
                
                if( b_need_notify_prev_sel )
                {
                    /*send WGL_NC_LB_ELEM_SEL(ui2_idx_prev_sel, FALSE) notification*/
                    CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, ui2_idx_prev_sel, FALSE) );
                }
            
                /*send WGL_NC_LB_ELEM_SEL(ui2_idx, TRUE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, ui2_idx, TRUE));

                GET_LOCK(h_lb, ppt_ctx);
            }
            
        }
    }
    else
    {
        pt_sel_notify_out->ui2_idx_prev_sel = b_need_notify_prev_sel ? ui2_idx_prev_sel : WGL_LB_NULL_INDEX;
        pt_sel_notify_out->ui2_idx_new_sel  = ui2_idx;
    }
    
    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_desel_elem
 *
 * Description: This API deselects an element.
 *
 * Inputs:  ppt_ctx         Contains the pointer of the instance context reference.
 *          b_raise_notify  Specify whether raise notify or not. 
 *          ui2_idx_target  Specifies the index of the element to be selected.
 *
 * Outputs: ppt_ctx         Contains the pointer of the instance context reference.
 *
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    Invalid arguments
 *          WGLR_INV_HANDLE The widget has been destroyed.
 *          < 0             Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_desel_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      BOOL                        b_raise_notify,
    IN      UINT16                      ui2_idx_target)
{
    INT32           i4_ret;
    HANDLE_T        h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
    BOOL            b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
    LB_ELEM_INF_T               *pt_elem_inf = NULL;
    
    if(ui2_idx_target >= _LB_GET_ELEMS_NUM(*ppt_ctx))
    {
        return WGLR_INV_ARG;
    }

    if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL) && _HAS_HLT_ELEM(*ppt_ctx))
    {
        return WGLR_OK;
    }
    
    _lb_get_elem(*ppt_ctx, ui2_idx_target, NULL, &pt_elem_inf);
    if(pt_elem_inf == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }
    if(!HAS_FLAG(pt_elem_inf->ui1_state, LBEST_SEL))
    {
        return WGLR_OK;
    }
    
    if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_FORCE_SEL) && (*ppt_ctx)->ui2_sel_num==1 )
    { /* The List Box is in force-selection mode and 
          the specified element is the only selected element*/
        return WGLR_INV_CMD;
    }

    WGL_CLEAR_FLAG(pt_elem_inf->ui1_state, LBEST_SEL);
    (*ppt_ctx)->ui2_sel_num --;

    if(!b_suppress_notify && b_raise_notify)
    {
        REL_LOCK(h_lb, *ppt_ctx);
        
        /*send WGL_NC_LB_ELEM_SEL(ui2_idx_target, FALSE) notification*/
        CATCH_FAIL(_lb_send_notify_sel(h_parent, h_lb, ui2_idx_target, FALSE));
        
        GET_LOCK(h_lb, ppt_ctx);
    }

    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_lose_hlt_elem
 *
 * Description: This API hanldes the condition when the highlight element loses hightlight.
 *
 * Inputs:  pt_ctx              Contains the instance context.
 *
 * Outputs: pt_ctx              Contains the changed instance context.
 *          pb_need_notify_hlt  Indicates if need to notify highlight of a new element.
 *
 * Returns: WGLR_OK             Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lb_lose_hlt_elem(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    OUT     BOOL                        *pb_need_notify_hlt, 
    OUT     UINT16                      *pui2_idx_new_hlt)
{
    UINT16                              ui2_tmp;
    UINT8*                              pui1_state = NULL;

    *pb_need_notify_hlt = FALSE;
    *pui2_idx_new_hlt   = WGL_LB_NULL_INDEX;

    if( HAS_STYLE(pt_ctx, WGL_STL_LB_FORCE_HLT) )
    { /*if in force highlighting mode, automatically highlight another*/
        _LB_FIND_NO_ELEM_ST_INF(pt_ctx, LBEST_DISABLED, 0, 1, TRUE, &ui2_tmp, &pui1_state);
        if(pui1_state != NULL && ui2_tmp < _LB_GET_ELEMS_NUM(pt_ctx))
        {
            WGL_SET_FLAG(*pui1_state, LBEST_HLT);
            pt_ctx->ui2_idx      = ui2_tmp;
            *pb_need_notify_hlt = TRUE;
            *pui2_idx_new_hlt   = pt_ctx->ui2_idx;
        }
    }
    return WGLR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_disable_elem
 *
 * Description: This API disables an element.
 *
 * Inputs:  ppt_ctx  Contains the pointer of the instance context reference.
 *          ui2_idx  Specifies the index of the element to be disabled.
 *
 * Outputs: ppt_ctx  Contains the pointer of the instance context reference.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_ARG     Invalid arguments
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_disable_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_idx)
{
    LB_ELEM_T   *pt_elem;
    LB_ELEM_INF_T               *pt_elem_inf = NULL;
    
    if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
    {
        return WGLR_INV_ARG;
    }
    
    _lb_get_elem(*ppt_ctx, ui2_idx, &pt_elem, &pt_elem_inf);
    if(pt_elem_inf == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }
    if(HAS_FLAG(pt_elem_inf->ui1_state, LBEST_DISABLED))
    {
        return WGLR_OK;
    }
    
    WGL_SET_FLAG(pt_elem_inf->ui1_state, LBEST_DISABLED);
    
    /*if this element is highlighted*/
    if( (*ppt_ctx)->ui2_idx == ui2_idx ) 
    {
        INT32                   i4_ret;
        BOOL                    b_need_notify_new_hlt;
        BOOL                    b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
        UINT16                  ui2_idx_new_hlt;
        HANDLE_T                h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
        LB_SEL_NOTIFY_CTX_T     t_sel_notify = {WGL_LB_NULL_INDEX, WGL_LB_NULL_INDEX};
        
        _LB_CLR_ELEM_STATE(*ppt_ctx, (*ppt_ctx)->ui2_idx, LBEST_HLT);
        (*ppt_ctx)->ui2_idx = WGL_LB_NULL_INDEX;
        CATCH_FAIL(_lb_lose_hlt_elem(*ppt_ctx, &b_need_notify_new_hlt, &ui2_idx_new_hlt));
        if(b_need_notify_new_hlt && HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL))
        {
            CATCH_FAIL( _lb_tempunlock_sel_elem(ppt_ctx, FALSE, ui2_idx_new_hlt, &t_sel_notify) );
        }

        if (!b_suppress_notify)
        {
            REL_LOCK(h_lb, *ppt_ctx);
            
            /*send WGL_NC_LB_ELEM_HLT(ui2_idx, FALSE) notification*/
            CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_idx, FALSE) );
            
            /*send WGL_NC_LB_ELEM_HLT(ui2_idx_new_hlt, TRUE) notification*/
            if(b_need_notify_new_hlt)
            {
                CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_idx_new_hlt, TRUE) );
            }
            
            if(t_sel_notify.ui2_idx_prev_sel != WGL_LB_NULL_INDEX)
            {
                /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_prev_sel, FALSE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_prev_sel, FALSE) );
            }
            
            if(t_sel_notify.ui2_idx_new_sel != WGL_LB_NULL_INDEX)
            {
                /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_new_sel, TRUE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_new_sel, TRUE));
            }
            
            GET_LOCK(h_lb, ppt_ctx);
        }
        
    }
    
    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_enable_elem
 *
 * Description: This API enables an element.
 *
 * Inputs:  ppt_ctx  Contains the pointer of the instance context reference.
 *          ui2_idx  Specifies the index of the element to be enabled.
 *
 * Outputs: ppt_ctx  Contains the pointer of the instance context reference.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_ARG     Invalid arguments
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_enable_elem(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_idx)
{
    INT32                   i4_ret;
    LB_ELEM_T               *pt_elem;
    BOOL                    b_need_notify_hlt = FALSE;
    HANDLE_T                h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
    LB_SEL_NOTIFY_CTX_T     t_sel_notify = {WGL_LB_NULL_INDEX, WGL_LB_NULL_INDEX};
    BOOL                    b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
    LB_ELEM_INF_T               *pt_elem_inf = NULL;

    if( ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
    {
        return WGLR_INV_ARG;
    }

    _lb_get_elem(*ppt_ctx, ui2_idx, &pt_elem, &pt_elem_inf);
    if(pt_elem_inf == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }
    if(!HAS_FLAG(pt_elem_inf->ui1_state, LBEST_DISABLED))
    {
        return WGLR_OK;
    }
    
    WGL_CLEAR_FLAG(pt_elem_inf->ui1_state, LBEST_DISABLED);
    
    if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_FORCE_HLT) &&  !_HAS_HLT_ELEM(*ppt_ctx)) 
    {
        WGL_SET_FLAG(pt_elem_inf->ui1_state, LBEST_HLT);
        (*ppt_ctx)->ui2_idx = ui2_idx;
        b_need_notify_hlt   = TRUE;
        if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL))
        {
            CATCH_FAIL( _lb_tempunlock_sel_elem(ppt_ctx, FALSE, (*ppt_ctx)->ui2_idx, &t_sel_notify) );
        }
    }

    if (!b_suppress_notify)
    {
        REL_LOCK(h_lb, *ppt_ctx);
        
        if( b_need_notify_hlt )
        {
            /*send WGL_NC_LB_ELEM_HLT(ui2_idx, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_idx, TRUE) );
        }
        
        if(t_sel_notify.ui2_idx_prev_sel!=WGL_LB_NULL_INDEX)
        {
            /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_prev_sel, FALSE) notification*/
            CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_prev_sel, FALSE) );
        }
        
        if(t_sel_notify.ui2_idx_new_sel!=WGL_LB_NULL_INDEX)
        {
            /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_new_sel, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_new_sel, TRUE));
        }
        
        GET_LOCK(h_lb, ppt_ctx);
    }
    
    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_get_elem
 *
 * Description: This API gets the pointer of a specified element.
 *
 * Inputs:  pt_ctx    Contains the instance context.
 *          ui2_idx   Specifies the element index.
 *
 * Outputs: ppt_elem  Contains the pointer of the specified element.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _lb_get_elem(
    IN      LB_CTX_T                    *pt_ctx, 
    IN      UINT16                      ui2_idx, 
    OUT     LB_ELEM_T                   **ppt_elem,
    OUT     LB_ELEM_INF_T               **ppt_elem_inf)
{
    LB_ELEM_T   *pt_elem = NULL;
    UINT16      ui2_i;
    UINT16      ui2_cache_s;
    UINT16      ui2_cache_e;
    INT32       i4_ret;

    CHECK_NULL(pt_ctx,WGLR_INV_ARG);

    if(ppt_elem != NULL)
    {
        *ppt_elem = NULL;
    }

    if(ppt_elem_inf != NULL)
    {
        *ppt_elem_inf = NULL;
    }

    if(ui2_idx >= _LB_GET_ELEMS_NUM(pt_ctx))
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        if(_LB_ELEMS_EMPTY(pt_ctx))
        {
            return WGLR_INV_ARG;
        }
        ui2_idx = _LB_GET_ELEMS_NUM(pt_ctx) -(UINT16)1;
    }
    
    ui2_cache_s = _LB_GET_CACHE_START(pt_ctx);
    ui2_cache_e = ui2_cache_s + _LB_GET_ELEM_CACHE_NUM(pt_ctx);

    if(ppt_elem != NULL && ui2_idx >= ui2_cache_s && ui2_idx < ui2_cache_e)
    {
        pt_elem = pt_ctx->pt_elems;
        for(ui2_i=ui2_cache_s; ui2_i<ui2_idx; ui2_i++)
        {
            pt_elem = pt_elem->pt_next;
        }
        
        *ppt_elem = pt_elem;
    }

    if(ppt_elem_inf != NULL)
    {
        _LB_GET_ELEM_INF(pt_ctx, ui2_idx, ppt_elem_inf);
    }
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_elem_pushed
 *
 * Description: This API pushes the highlighted element.
 *
 * Inputs:  ppt_ctx  Contains the pointer of the instance context.
 *
 * Outputs: ppt_ctx  Contains the pointer of the instance context.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_elem_pushed(
    IN OUT  LB_CTX_T                    **ppt_ctx)
{
    INT32           i4_ret;
    UINT16          ui2_idx_pushed;    
    HANDLE_T        h_parent = (*ppt_ctx)->h_parent, h_lb = (*ppt_ctx)->h_lb;
    BOOL            b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
    BOOL            b_repaint;
    UINT8           *pui1_state;

    if( _LB_ELEMS_EMPTY(*ppt_ctx) || !_HAS_HLT_ELEM(*ppt_ctx)) 
    {
        return WGLR_OK;
    }

    b_repaint = FALSE;
    
___BLOCK_BEGIN___

    ui2_idx_pushed = (*ppt_ctx)->ui2_idx;
    if( HAS_STYLE(*ppt_ctx, WGL_STL_LB_ELEM_NO_PUSH) )
    {
        GOTO_EOB;
    }

    pui1_state = _LB_GET_ELEM_STATE_P((*ppt_ctx),ui2_idx_pushed);
    if(pui1_state && !HAS_FLAG(*pui1_state, LBEST_PUSHED))
    {
        WGL_SET_FLAG(*pui1_state, LBEST_PUSHED);
        
        b_repaint = TRUE;
        
        /* request minimal update region */
        INFORM_ERR (_lb_req_update_by_range_elems (*ppt_ctx, FALSE, ui2_idx_pushed, ui2_idx_pushed));
        
        /* repaint after notify if NOT in custom mode */
        if( b_repaint && !HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE) )
        {
            CATCH_FAIL (_lb_tempunlock_repaint_partial_generic (ppt_ctx));
        }
        
        _LB_CHECK_CONSISTENCY(*ppt_ctx);
    }

___BLOCK_END___

    if (!b_suppress_notify)
    {
        REL_LOCK(h_lb, *ppt_ctx);
        
        CATCH_FAIL(_lb_send_notify_pushed(h_parent, h_lb, ui2_idx_pushed, TRUE));
        
        GET_LOCK(h_lb, ppt_ctx);
    }
    
    /* repaint after notify in custom mode */
    if( b_repaint && HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE) )
    {
        CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
    }
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_elem_unpushed
 *
 * Description: This element unpushes the highlighted element.
 *
 * Inputs:  ppt_ctx  Contains the pointer of the instance context.
 *
 * Outputs: ppt_ctx  Contains the pointer of the instance context.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_elem_unpushed(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      BOOL                        b_auto_repaint)
{
    INT32           i4_ret;
    BOOL            b_need_notify_sel   = FALSE;
    UINT16          ui2_idx_sel         = WGL_LB_NULL_INDEX;
    BOOL            b_need_notify_desel = FALSE;
    UINT16          ui2_idx_desel       = WGL_LB_NULL_INDEX;
    UINT16          ui2_idx_repaint_1   = WGL_LB_NULL_INDEX, ui2_idx_repaint_2 = WGL_LB_NULL_INDEX;
    HANDLE_T        h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
    BOOL            b_has_no_push_style;
    BOOL            b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
    UINT16          ui2_idx;
    UINT8           *pui1_state;


    if( _LB_ELEMS_EMPTY(*ppt_ctx) || !_HAS_HLT_ELEM(*ppt_ctx)) 
    {
        return WGLR_OK;
    }

    ui2_idx = (*ppt_ctx)->ui2_idx;
    pui1_state =  _LB_GET_ELEM_STATE_P(*ppt_ctx, ui2_idx);
    if(pui1_state == NULL || ui2_idx >= _LB_GET_ELEMS_NUM(*ppt_ctx))
    {
        return WGLR_OK;
    }

    b_has_no_push_style = HAS_STYLE(*ppt_ctx, WGL_STL_LB_ELEM_NO_PUSH) ? TRUE : FALSE;
    if(!b_has_no_push_style)
    {
        WGL_CLEAR_FLAG(*pui1_state, LBEST_PUSHED);
    }

    ui2_idx_repaint_1 = (*ppt_ctx)->ui2_idx;
    /*toggle the selected state of the element*/
    if(HAS_FLAG(*pui1_state, LBEST_SEL))
    {
        CHECK_VALID( !((*ppt_ctx)->ui2_sel_num==0), WGLR_INTERNAL_ERROR );
        if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_FORCE_SEL) && (*ppt_ctx)->ui2_sel_num==1)
        {
            /*none*/
        }
        else if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL))
        {
            /*none*/
        }
        else
        {
            WGL_CLEAR_FLAG(*pui1_state, LBEST_SEL);
            (*ppt_ctx)->ui2_sel_num --;
            b_need_notify_desel = TRUE;
            ui2_idx_desel       = (*ppt_ctx)->ui2_idx;
        }
    }
    else
    {
        CHECK_VALID( !(!HAS_STYLE(*ppt_ctx, WGL_STL_LB_MULTI_SEL) && (*ppt_ctx)->ui2_sel_num>=2), WGLR_INTERNAL_ERROR );
        if( !HAS_STYLE(*ppt_ctx, WGL_STL_LB_MULTI_SEL) && (*ppt_ctx)->ui2_sel_num==1 )
        { /*need to deselect an originally selected element */
            UINT8*     pui1_st_tmp;
            UINT16      ui2_i;

            _LB_FIND_ELEM_ST_INF(*ppt_ctx, LBEST_SEL,0, 1,TRUE, &ui2_i, &pui1_st_tmp);
            
            WS_ASSERT(!(ui2_i == WGL_LB_NULL_INDEX || pui1_st_tmp == NULL) );
            if(pui1_st_tmp)
            {
                WGL_CLEAR_FLAG(*pui1_st_tmp, LBEST_SEL);
            }
            (*ppt_ctx)->ui2_sel_num --;
            b_need_notify_desel = TRUE;
            ui2_idx_desel       = ui2_i;
            ui2_idx_repaint_2   = ui2_idx_desel;
        }

        _LB_SET_ELEM_STATE((*ppt_ctx), ui2_idx, LBEST_SEL);
        (*ppt_ctx)->ui2_sel_num ++;
        b_need_notify_sel = TRUE;
        ui2_idx_sel       = (*ppt_ctx)->ui2_idx;        
    }

    /* request minimal update region */
    if (b_auto_repaint)
    {
        /* request minimal update region for originally selected elem */
        if(ui2_idx_repaint_1 != WGL_LB_NULL_INDEX && 
           ui2_idx_repaint_1 != ui2_idx_repaint_2)
        {
            INFORM_ERR (_lb_req_update_by_range_elems (*ppt_ctx, FALSE, ui2_idx_repaint_1, ui2_idx_repaint_1));
        }
        
        /* request minimal update region for newly selected elem */
        if(ui2_idx_repaint_2 != WGL_LB_NULL_INDEX)
        {
            INFORM_ERR (_lb_req_update_by_range_elems (*ppt_ctx, FALSE, ui2_idx_repaint_2, ui2_idx_repaint_2));
        }
    }

    /* repaint after notify if NOT in custom mode */
    if( b_auto_repaint && !HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE) )
    {
        CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
    }

    if (!b_suppress_notify)
    {
        UINT16 ui2_temp_idx = (*ppt_ctx)->ui2_idx;
        
        REL_LOCK(h_lb, *ppt_ctx);
        
        if( !b_has_no_push_style )
        {
            CATCH_FAIL(_lb_send_notify_pushed(h_parent, h_lb, ui2_temp_idx, FALSE));
        }   
        
        if( b_need_notify_desel )
        {
            /*send WGL_NC_LB_ELEM_SEL(ui2_idx_desel, FALSE) notification*/
            CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, ui2_idx_desel, FALSE) );
        }
        
        if( b_need_notify_sel )
        {
            /*send WGL_NC_LB_ELEM_SEL(ui2_idx_sel, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, ui2_idx_sel, TRUE) );
        }
        
        GET_LOCK(h_lb, ppt_ctx); 
    }
    
    /* repaint after notify in custom mode */
    if( b_auto_repaint && HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE) )
    {
        CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
    }

    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_elem_next_wrap_over
 *
 * Description: This API tries to hightlight the next element at the last element.
 *
 * Inputs:  ppt_ctx    Contains the pointer of the instance context.
 *          b_repaint  Specifies whether to repaint or not.
 *
 * Outputs: ppt_ctx    Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_HANDLE The widget has been destroyed.
 *          < 0             Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_elem_next_wrap_over(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint)
{
    INT32           i4_ret;
    UINT16          ui2_cur;
    UINT16          ui2_nxt;
    UINT8*          pui1_st_nxt = NULL; 
    BOOL            b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;

    if(_LB_GET_ELEMS_NUM(*ppt_ctx) <= 1 )
    {
        return WGLR_OK;
    }

    ui2_cur = (*ppt_ctx)->ui2_idx;
    ui2_nxt = 0;
    
    _LB_FIND_NO_ELEM_ST_INF(*ppt_ctx,LBEST_DISABLED, 0, 1,TRUE, &ui2_nxt, &pui1_st_nxt);

    if(pui1_st_nxt && ui2_nxt!=(*ppt_ctx)->ui2_idx)
    {
        BOOL                 b_is_vp_scrolled;
        BOOL                 b_is_custom_mode = HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE) ? TRUE : FALSE;
        HANDLE_T             h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
        LB_SEL_NOTIFY_CTX_T  t_sel_notify = {WGL_LB_NULL_INDEX, WGL_LB_NULL_INDEX};

        _LB_CLR_ELEM_STATE(*ppt_ctx, (*ppt_ctx)->ui2_idx, LBEST_HLT);
        WGL_SET_FLAG(*pui1_st_nxt, LBEST_HLT);
        (*ppt_ctx)->ui2_idx = ui2_nxt;
        
        if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL))
        {
            CATCH_FAIL(_lb_tempunlock_sel_elem(ppt_ctx, FALSE, (*ppt_ctx)->ui2_idx, &t_sel_notify));
        }

        CATCH_FAIL(_lb_check_elem_visibility_after_user_key(*ppt_ctx, (*ppt_ctx)->ui2_idx, TRUE, &b_is_vp_scrolled));

        /* request minimal update region */
        INFORM_ERR (_lb_req_update_by_hlt_nxt_prv_elem(*ppt_ctx, b_is_vp_scrolled, ui2_cur, ui2_nxt));
        
        /* repaint after notify if NOT in custom mode */
        if( b_repaint && !b_is_custom_mode)
        {
            CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
        }        

        REL_LOCK(h_lb, *ppt_ctx);
        
        if (!b_suppress_notify)
        {
            /*send WGL_NC_LB_ELEM_HLT(ui2_cur, FALSE) notification*/
            CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_cur, FALSE));
            
            /*send WGL_NC_LB_ELEM_HLT(ui2_nxt, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_nxt, TRUE) );      
            
            if(t_sel_notify.ui2_idx_prev_sel != WGL_LB_NULL_INDEX)
            {
                /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_prev_sel, FALSE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_prev_sel, FALSE) );
            }
            
            if(t_sel_notify.ui2_idx_new_sel != WGL_LB_NULL_INDEX)
            {
                /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_new_sel, TRUE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_new_sel, TRUE));
            }
        }
        
        GET_LOCK(h_lb, ppt_ctx);
        
        /* repaint after notify in custom mode */
        if( b_repaint && b_is_custom_mode)
        {
            CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
        }        
    }

    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_elem_next
 *
 * Description: This API tries to hightlight the next element.
 *
 * Inputs:  ppt_ctx    Contains the pointer of the instance context.
 *          b_repaint  Specifies whether to repaint or not.
 *
 * Outputs: ppt_ctx    Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_elem_next(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint)
{
    INT32               i4_ret;
    UINT16              ui2_idx_cur;
    UINT16              ui2_idx_nxt;
#ifdef WGL_ANIM_SUPPORT
    BOOL                b_do_anim = FALSE;
#endif
    
    CHECK_NULL(ppt_ctx,WGLR_INV_ARG);
    CHECK_NULL(*ppt_ctx,WGLR_INV_ARG);
    if(_LB_ELEMS_EMPTY(*ppt_ctx))
    {
        return WGLR_OK;
    }

    /*check if the bottom is reached*/
    if((*ppt_ctx)->ui2_idx==(_LB_GET_ELEMS_NUM(*ppt_ctx)-(UINT16)1))
    {
        if(!HAS_STYLE(*ppt_ctx, WGL_STL_LB_NO_WRAP_OVER))
        {
            if(_LB_GET_ELEMS_NUM(*ppt_ctx) == (UINT16)1)
            {
                return WGLR_OK;
            }
            /*perform the wrap-over*/
            return _lb_tempunlock_on_elem_next_wrap_over(ppt_ctx, b_repaint);
        }
        else
        {   /*Send a notification: the bottom is reached*/
            CATCH_FAIL(_lb_tempunlock_send_notify_reach_boundary(ppt_ctx, FALSE));
            return WGLR_OK;
        }
    }
    
    if(!_HAS_HLT_ELEM(*ppt_ctx) && _LB_CHK_ELEM_STATE(*ppt_ctx, 0, LBEST_DISABLED) == FALSE )
    {
        /*auto highlight the first element*/
        ui2_idx_cur = WGL_LB_NULL_INDEX; /*dummy*/
        ui2_idx_nxt = 0;
    }
    else
    {
        ui2_idx_cur = (_HAS_HLT_ELEM(*ppt_ctx)) ? (*ppt_ctx)->ui2_idx : 0;
        ui2_idx_nxt = ui2_idx_cur+(UINT16)1;
        while(ui2_idx_nxt < _LB_GET_ELEMS_NUM(*ppt_ctx))
        {   /*skip disabled element*/
            if(_LB_CHK_ELEM_STATE(*ppt_ctx, ui2_idx_nxt, LBEST_DISABLED) == FALSE)
            { 
                break;
            }
            ui2_idx_nxt++;
        }

    }
   
    if(_LB_VALID_IDX(*ppt_ctx, ui2_idx_nxt))
    {
        HANDLE_T                h_lb = (*ppt_ctx)->h_lb;
        HANDLE_T                h_parent = (*ppt_ctx)->h_parent;
        BOOL                    b_is_vp_scrolled;
        LB_SEL_NOTIFY_CTX_T     t_sel_notify = {WGL_LB_NULL_INDEX, WGL_LB_NULL_INDEX};
        BOOL                    b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
        WGL_LB_EVN_HDLR_INFO_T             t_evn_hdlr_info;
        WGL_LB_EVN_HDLR_INFO_NVGT_IN_VP_T  t_evn_nvgt_in_vp_info;
#ifdef WGL_ANIM_SUPPORT
        UINT32                             ui4_act_flg = 0;
        
        ui4_act_flg = _lb_get_actmap_flag(*ppt_ctx, WGL_ANIM_ACTION_DOWN);
#endif
        t_evn_nvgt_in_vp_info.b_do_nfy_after_cb = TRUE;

        CATCH_FAIL(_lb_check_elem_visibility_after_user_key(*ppt_ctx, ui2_idx_nxt, FALSE, &b_is_vp_scrolled));

        if (_LB_VALID_IDX((*ppt_ctx),ui2_idx_cur) && 
            (!b_is_vp_scrolled) && HAS_EVN_HANDLER_MASK(*ppt_ctx, WGL_LB_EVN_HDLR_MASK_NVGT_IN_VP))
        {
            t_evn_hdlr_info = (*ppt_ctx)->t_evn_hdlr_info;
            t_evn_nvgt_in_vp_info.ui2_old_hlt = ui2_idx_cur;
            t_evn_nvgt_in_vp_info.ui2_new_hlt = ui2_idx_nxt;
            t_evn_nvgt_in_vp_info.b_sequential = (((ui2_idx_cur - ui2_idx_nxt) == 1) || ((ui2_idx_nxt - ui2_idx_cur) == 1)) ? TRUE : FALSE;
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_HLT_ELEM_AREA, ui2_idx_cur, &t_evn_nvgt_in_vp_info.t_old_rect, i4_ret);
            CATCH_FAIL(i4_ret);
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_HLT_ELEM_AREA, ui2_idx_nxt, &t_evn_nvgt_in_vp_info.t_new_rect, i4_ret);
            CATCH_FAIL(i4_ret);
            
            REL_LOCK(h_lb, *ppt_ctx);

            t_evn_hdlr_info.pf_event_hdlr(h_lb, t_evn_hdlr_info.pv_tag_evn_hdlr, WGL_LB_EVN_HDLR_TYPE_NVGT_IN_VP, (UINT32)&t_evn_nvgt_in_vp_info);

            GET_LOCK(h_lb, ppt_ctx);
        }
        else
        {
            if (_LB_VALID_IDX((*ppt_ctx),ui2_idx_cur))
            {
                _LB_CLR_ELEM_STATE(*ppt_ctx, ui2_idx_cur, LBEST_HLT);
            }
#ifdef WGL_ANIM_SUPPORT
            if(_LB_ANIM_EN(*ppt_ctx) && _LB_ANIM_CHECK(*ppt_ctx,b_is_vp_scrolled, ui4_act_flg))
            {
                _lb_elem_set_anim_info(*ppt_ctx, ui2_idx_cur, ui2_idx_nxt, &b_do_anim);
            }           
            else
#endif
            {
                _LB_SET_ELEM_STATE(*ppt_ctx, ui2_idx_nxt, LBEST_HLT);
                (*ppt_ctx)->ui2_idx = ui2_idx_nxt;
            }

            
            if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL))
            {
                CATCH_FAIL(_lb_tempunlock_sel_elem(ppt_ctx, FALSE, (*ppt_ctx)->ui2_idx, &t_sel_notify));
            }
            
            CATCH_FAIL(_lb_check_elem_visibility_after_user_key(*ppt_ctx, ui2_idx_nxt, TRUE, &b_is_vp_scrolled));

            /* request minimal update region */
#ifdef WGL_ANIM_SUPPORT
            if(b_do_anim == FALSE)
            {
            INFORM_ERR (_lb_req_update_by_hlt_nxt_prv_elem(*ppt_ctx, b_is_vp_scrolled, ui2_idx_cur, ui2_idx_nxt));
            }
            else
            {
                INFORM_ERR (_lb_req_update_by_hlt_nxt_prv_elem(*ppt_ctx, b_is_vp_scrolled, ui2_idx_cur, WGL_LB_NULL_INDEX));
            }

            if(_LB_ANIM_EN(*ppt_ctx) && _LB_ANIM_CHECK(*ppt_ctx,b_is_vp_scrolled, ui4_act_flg))
            {
                _lb_elem_begin_anim(ppt_ctx,WGL_ANIM_ACTION_DOWN);   
            }

            CHECK_NULL(ppt_ctx,WGLR_INV_ARG);
            CHECK_NULL(*ppt_ctx,WGLR_INV_ARG);
#else
            INFORM_ERR (_lb_req_update_by_hlt_nxt_prv_elem(*ppt_ctx, b_is_vp_scrolled, ui2_idx_cur, ui2_idx_nxt));
#endif
            
            /* repaint after notify if NOT in custom mode */
            if( b_repaint && !HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE) )
            {
                CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
            }
        }

        if ((!b_suppress_notify) && t_evn_nvgt_in_vp_info.b_do_nfy_after_cb)
        {
            REL_LOCK(h_lb, *ppt_ctx);
            
            if( ui2_idx_cur!=WGL_LB_NULL_INDEX )
            {   /*send WGL_NC_LB_ELEM_HLT(ui2_idx_cur, FALSE) notification*/
                CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_idx_cur, FALSE) );
            }
            
            /*send WGL_NC_LB_ELEM_HLT(ui2_idx_nxt, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_idx_nxt, TRUE) );
    
            if(t_sel_notify.ui2_idx_prev_sel!=WGL_LB_NULL_INDEX)
            {
                /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_prev_sel, FALSE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_prev_sel, FALSE) );
            }
    
            if(t_sel_notify.ui2_idx_new_sel!=WGL_LB_NULL_INDEX)
            {
                /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_new_sel, TRUE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_new_sel, TRUE));
            }
            
            GET_LOCK(h_lb, ppt_ctx); 
        }
        
        /* repaint after notify in custom mode */
        if (!(_LB_VALID_IDX(*ppt_ctx,ui2_idx_cur) && (!b_is_vp_scrolled) && 
              HAS_EVN_HANDLER_MASK(*ppt_ctx, WGL_LB_EVN_HDLR_MASK_NVGT_IN_VP)))
        {
            if (b_repaint && HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE) )
            {
                CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
            }
        }
        
    }
    else if(!HAS_STYLE(*ppt_ctx, WGL_STL_LB_NO_WRAP_OVER))
    {   /*try to wrap over*/
        return _lb_tempunlock_on_elem_next_wrap_over(ppt_ctx, b_repaint);
    }
    else
    {
       /*Send a notification: the bottom is reached*/
        CATCH_FAIL(_lb_tempunlock_send_notify_reach_boundary(ppt_ctx, FALSE));
        return WGLR_OK;
    }

    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_elem_prev_no_hlt
 *
 * Description: This API tries to hightlight an element when no element is highlighted 
 *              for the request to highlight the previous element.
 *
 * Inputs:  ppt_ctx    Contains the pointer of the instance context.
 *          b_repaint  Specifies whether to repaint or not.
 *
 * Outputs: ppt_ctx    Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_elem_prev_no_hlt(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint)
{
    INT32           i4_ret;
    UINT16          ui2_tmp;
    UINT8*          pui1_state_tmp = NULL;
    
    if(_LB_ELEMS_EMPTY(*ppt_ctx))
    {
        return WGLR_OK;
    }

    ui2_tmp = _LB_GET_ELEMS_NUM(*ppt_ctx) - (UINT16)1;
    _LB_FIND_NO_ELEM_ST_INF(*ppt_ctx, LBEST_DISABLED, ui2_tmp, 1,FALSE, &ui2_tmp, &pui1_state_tmp);

    if( (ui2_tmp != WGL_LB_NULL_INDEX) && (pui1_state_tmp != NULL) )
    {
        HANDLE_T                h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
        BOOL                    b_is_vp_scrolled;
        BOOL                    b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
        LB_SEL_NOTIFY_CTX_T     t_sel_notify = {WGL_LB_NULL_INDEX, WGL_LB_NULL_INDEX};
        
        (*ppt_ctx)->ui2_idx = ui2_tmp;
        WGL_SET_FLAG(*pui1_state_tmp, LBEST_HLT);
       
        if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL))
        {
            CATCH_FAIL(_lb_tempunlock_sel_elem(ppt_ctx, FALSE, (*ppt_ctx)->ui2_idx, &t_sel_notify));
        }

        CATCH_FAIL(_lb_check_elem_visibility_after_user_key(*ppt_ctx, (*ppt_ctx)->ui2_idx, TRUE, &b_is_vp_scrolled));

        /* request minimal update region */
        INFORM_ERR (_lb_req_update_by_hlt_nxt_prv_elem(*ppt_ctx, b_is_vp_scrolled, WGL_LB_NULL_INDEX, ui2_tmp));

        /* repaint after notify if NOT in custom mode */
        if( b_repaint && !HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE) )
        {
            CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
        }

        if (!b_suppress_notify)
        {
            REL_LOCK(h_lb, *ppt_ctx);
            
            /*send WGL_NC_LB_ELEM_HLT(ui2_tmp, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_tmp, TRUE) );
            
            if(t_sel_notify.ui2_idx_prev_sel!=WGL_LB_NULL_INDEX)
            {
                /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_prev_sel, FALSE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_prev_sel, FALSE) );
            }
            
            if(t_sel_notify.ui2_idx_new_sel!=WGL_LB_NULL_INDEX)
            {
                /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_new_sel, TRUE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_new_sel, TRUE));
            }

            GET_LOCK(h_lb, ppt_ctx);
        }
        
        /* repaint after notify in custom mode */
        if( b_repaint && HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE) )
        {
            CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
        }
        
    }

    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_elem_prev_wrap_over
 *
 * Description: This API tries to hightlight the previous element at the first element.
 *
 * Inputs:  ppt_ctx    Contains the pointer of the instance context.
 *          b_repaint  Specifies whether to repaint or not.
 *
 * Outputs: ppt_ctx    Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_elem_prev_wrap_over(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint)
{
    INT32           i4_ret;
    UINT16          ui2_cur;
    UINT16          ui2_pre;
    UINT8*          pui1_state_prev = NULL;

    if(_LB_GET_ELEMS_NUM(*ppt_ctx) <= 1)
    {
        return WGLR_OK;
    }

    ui2_cur = (*ppt_ctx)->ui2_idx;
    ui2_pre = _LB_GET_ELEMS_NUM(*ppt_ctx)-(UINT16)1;
    _LB_FIND_NO_ELEM_ST_INF(*ppt_ctx, LBEST_DISABLED,ui2_pre, 1,FALSE, &ui2_pre, &pui1_state_prev);
    
    if ( _LB_VALID_IDX(*ppt_ctx, ui2_pre) && (pui1_state_prev != NULL) ) 
    {
        HANDLE_T             h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
        BOOL                 b_is_vp_scrolled;
        BOOL                 b_is_custom_mode = HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE) ? TRUE : FALSE;
        BOOL                 b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
        LB_SEL_NOTIFY_CTX_T  t_sel_notify = {WGL_LB_NULL_INDEX, WGL_LB_NULL_INDEX};

        _LB_CLR_ELEM_STATE(*ppt_ctx, ui2_cur, LBEST_HLT);
        WGL_SET_FLAG(*pui1_state_prev, LBEST_HLT);
        (*ppt_ctx)->ui2_idx = ui2_pre;
        if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL))
        {
            CATCH_FAIL(_lb_tempunlock_sel_elem(ppt_ctx, FALSE, (*ppt_ctx)->ui2_idx, &t_sel_notify));
        }

        CATCH_FAIL(_lb_check_elem_visibility_after_user_key(*ppt_ctx, (*ppt_ctx)->ui2_idx, TRUE, &b_is_vp_scrolled));
        
        /* request minimal update region */
        INFORM_ERR (_lb_req_update_by_hlt_nxt_prv_elem(*ppt_ctx, b_is_vp_scrolled, ui2_cur, ui2_pre));

        /* repaint after notify if NOT in custom mode */
        if( b_repaint && !b_is_custom_mode)
        {
            CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
        }
        
        REL_LOCK(h_lb, *ppt_ctx);
        
        if (!b_suppress_notify)
        {
            /*send WGL_NC_LB_ELEM_HLT(ui2_cur, FALSE) notification*/
            CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_cur, FALSE) );
            
            /*send WGL_NC_LB_ELEM_HLT(ui2_pre, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_pre, TRUE) );
            
            if(t_sel_notify.ui2_idx_prev_sel!=WGL_LB_NULL_INDEX)
            {
                /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_prev_sel, FALSE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_prev_sel, FALSE) );
            }
            
            if(t_sel_notify.ui2_idx_new_sel!=WGL_LB_NULL_INDEX)
            {
                /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_new_sel, TRUE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_new_sel, TRUE));
            }
        }
        
        GET_LOCK(h_lb, ppt_ctx);
        
        /* repaint after notify in custom mode */
        if( b_repaint && b_is_custom_mode)
        {
            CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
        }        
        
    }

    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_elem_prev
 *
 * Description: This API tries to hightlight the previous element.
 *
 * Inputs:  ppt_ctx    Contains the pointer of the instance context.
 *          b_repaint  Specifies whether to repaint or not.
 *
 * Outputs: ppt_ctx    Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_elem_prev(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint)
{
    INT32       i4_ret;
    UINT16      ui2_cur;
    UINT16      ui2_pre;
    UINT8*      pui1_st_prev = NULL;
#ifdef WGL_ANIM_SUPPORT
    BOOL        b_do_anim = FALSE;
#endif

    CHECK_NULL(ppt_ctx,WGLR_INV_ARG);
    CHECK_NULL(*ppt_ctx,WGLR_INV_ARG);
    if(!_HAS_HLT_ELEM(*ppt_ctx))
    {
        return _lb_tempunlock_on_elem_prev_no_hlt(ppt_ctx, b_repaint);
    }
    else if( !HAS_STYLE(*ppt_ctx, WGL_STL_LB_NO_WRAP_OVER) && (*ppt_ctx)->ui2_idx==0 )
    {
        return _lb_tempunlock_on_elem_prev_wrap_over(ppt_ctx, b_repaint);
    }
    else if((HAS_STYLE(*ppt_ctx, WGL_STL_LB_NO_WRAP_OVER) && (*ppt_ctx)->ui2_idx==0))
    {
        /*Send a notification: the top boundary is reached*/
        return _lb_tempunlock_send_notify_reach_boundary(ppt_ctx, TRUE);
    }
    
    if( _LB_GET_ELEMS_NUM(*ppt_ctx) ==0 || (*ppt_ctx)->ui2_idx == 0) 
    {
        return WGLR_OK;
    }

    ui2_cur = (*ppt_ctx)->ui2_idx;
    ui2_pre = ui2_cur - (UINT16)1;

    _LB_FIND_NO_ELEM_ST_INF(*ppt_ctx, LBEST_DISABLED, ui2_pre, 1,FALSE, &ui2_pre, &pui1_st_prev);

    CHECK_VALID( 
        !( (pui1_st_prev!=NULL && !(ui2_pre<_LB_GET_ELEMS_NUM(*ppt_ctx)) ) ||
           (pui1_st_prev==NULL && (ui2_pre<_LB_GET_ELEMS_NUM(*ppt_ctx)) )), WGLR_INTERNAL_ERROR );

    if( pui1_st_prev ) 
    {
        BOOL                    b_is_vp_scrolled;
        HANDLE_T                h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
        LB_SEL_NOTIFY_CTX_T     t_sel_notify = {WGL_LB_NULL_INDEX, WGL_LB_NULL_INDEX};
        BOOL                    b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
        WGL_LB_EVN_HDLR_INFO_T             t_evn_hdlr_info;
        WGL_LB_EVN_HDLR_INFO_NVGT_IN_VP_T  t_evn_nvgt_in_vp_info;
#ifdef WGL_ANIM_SUPPORT 
        UINT32                             ui4_act_flg = 0;
        
        ui4_act_flg = _lb_get_actmap_flag(*ppt_ctx, WGL_ANIM_ACTION_UP);
#endif        
        t_evn_nvgt_in_vp_info.b_do_nfy_after_cb = TRUE;
        
        CATCH_FAIL(_lb_check_elem_visibility_after_user_key(*ppt_ctx, ui2_pre, FALSE, &b_is_vp_scrolled));

        if ((!b_is_vp_scrolled) && HAS_EVN_HANDLER_MASK(*ppt_ctx, WGL_LB_EVN_HDLR_MASK_NVGT_IN_VP))
        {
            t_evn_hdlr_info = (*ppt_ctx)->t_evn_hdlr_info;
            t_evn_nvgt_in_vp_info.ui2_old_hlt = ui2_cur;
            t_evn_nvgt_in_vp_info.ui2_new_hlt = ui2_pre;
            t_evn_nvgt_in_vp_info.b_sequential = (((ui2_cur - ui2_pre) == 1) || ((ui2_pre- ui2_cur) == 1)) ? TRUE : FALSE;
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_HLT_ELEM_AREA, ui2_cur, &t_evn_nvgt_in_vp_info.t_old_rect, i4_ret);
            CATCH_FAIL(i4_ret);
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_HLT_ELEM_AREA, ui2_pre, &t_evn_nvgt_in_vp_info.t_new_rect, i4_ret);
            CATCH_FAIL(i4_ret);
            
            REL_LOCK(h_lb, *ppt_ctx);

            t_evn_hdlr_info.pf_event_hdlr(h_lb, t_evn_hdlr_info.pv_tag_evn_hdlr, WGL_LB_EVN_HDLR_TYPE_NVGT_IN_VP, (UINT32)&t_evn_nvgt_in_vp_info);

            GET_LOCK(h_lb, ppt_ctx);
        }
        else
        {
            _LB_CLR_ELEM_STATE(*ppt_ctx, ui2_cur, LBEST_HLT);
#ifdef WGL_ANIM_SUPPORT
            if(_LB_ANIM_EN(*ppt_ctx) && _LB_ANIM_CHECK(*ppt_ctx,b_is_vp_scrolled,ui4_act_flg))
            {
                _lb_elem_set_anim_info(*ppt_ctx, ui2_cur, ui2_pre, &b_do_anim);
            } 
            else
#endif
            {
                _LB_SET_ELEM_STATE(*ppt_ctx, ui2_pre, LBEST_HLT);
                (*ppt_ctx)->ui2_idx = ui2_pre;
            }
                       
            if(HAS_STYLE(*ppt_ctx, WGL_STL_LB_AUTO_SEL))
            {
                CATCH_FAIL(_lb_tempunlock_sel_elem(ppt_ctx, FALSE, (*ppt_ctx)->ui2_idx, &t_sel_notify));
            }

            CATCH_FAIL(_lb_check_elem_visibility_after_user_key(*ppt_ctx, ui2_pre, TRUE, &b_is_vp_scrolled));

            /* request minimal update region */
#ifdef WGL_ANIM_SUPPORT
            if(b_do_anim == FALSE)
            {
            INFORM_ERR (_lb_req_update_by_hlt_nxt_prv_elem(*ppt_ctx, b_is_vp_scrolled, ui2_cur, ui2_pre));
            }
            else
            {
                INFORM_ERR (_lb_req_update_by_hlt_nxt_prv_elem(*ppt_ctx, b_is_vp_scrolled, ui2_cur, WGL_LB_NULL_INDEX));
            }

            if(_LB_ANIM_EN(*ppt_ctx) && _LB_ANIM_CHECK(*ppt_ctx,b_is_vp_scrolled,ui4_act_flg))
            {
                _lb_elem_begin_anim(ppt_ctx,WGL_ANIM_ACTION_UP);   
            }

            CHECK_NULL(ppt_ctx,WGLR_INV_ARG);
            CHECK_NULL(*ppt_ctx,WGLR_INV_ARG);
#else
            INFORM_ERR (_lb_req_update_by_hlt_nxt_prv_elem(*ppt_ctx, b_is_vp_scrolled, ui2_cur, ui2_pre));
#endif
            
            /* repaint after notify if NOT in custom mode */
            if( b_repaint && !HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE) )
            {
                CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
            }
        }

        if ((!b_suppress_notify) && t_evn_nvgt_in_vp_info.b_do_nfy_after_cb)
        {
            REL_LOCK(h_lb, *ppt_ctx);
            
            /*send WGL_NC_LB_ELEM_HLT(ui2_cur, FALSE) notification*/
            CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_cur, FALSE) );
            
            /*send WGL_NC_LB_ELEM_HLT(ui2_pre, TRUE) notification*/
            CATCH_FAIL( _lb_send_notify_hlt(h_parent, h_lb, ui2_pre, TRUE) );
            
            if(t_sel_notify.ui2_idx_prev_sel!=WGL_LB_NULL_INDEX)
            {
                /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_prev_sel, FALSE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_prev_sel, FALSE) );
            }
            
            if(t_sel_notify.ui2_idx_new_sel!=WGL_LB_NULL_INDEX)
            {
                /*send WGL_NC_LB_ELEM_SEL(t_sel_notify.ui2_idx_new_sel, TRUE) notification*/
                CATCH_FAIL( _lb_send_notify_sel(h_parent, h_lb, t_sel_notify.ui2_idx_new_sel, TRUE));
            }
            
            GET_LOCK(h_lb, ppt_ctx);
        }
        
        /* repaint after notify in custom mode */
        if (!((!b_is_vp_scrolled) && HAS_EVN_HANDLER_MASK(*ppt_ctx, WGL_LB_EVN_HDLR_MASK_NVGT_IN_VP)))
        {
            if ( b_repaint && HAS_STYLE(*ppt_ctx, WGL_STL_LB_CUSTOM_MODE) )
            {
                CATCH_FAIL(_lb_tempunlock_repaint_partial_generic(ppt_ctx));
            }
        }
        
    }
    else if(!HAS_STYLE(*ppt_ctx, WGL_STL_LB_NO_WRAP_OVER))
    {
        return _lb_tempunlock_on_elem_prev_wrap_over(ppt_ctx, b_repaint);
    }
    else
    {
        /*Send a notification: the top boundary is reached*/
        return _lb_tempunlock_send_notify_reach_boundary(ppt_ctx, TRUE);
    }
    
    _LB_CHECK_CONSISTENCY(*ppt_ctx);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_page_up
 *
 * Description: This API handles the page-up request.
 *
 * Inputs:  ppt_ctx    Contains the pointer of the instance context.
 *          b_repaint  Specifies whether to repaint or not.
 *
 * Outputs: ppt_ctx    Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_page_up(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint)
{
    UINT16      ui2_first, ui2_last, ui2_num_vp_elem;
    UINT32      ui4_range;
    INT32       i4_ret;
    BOOL        b_need_repaint;
    
    b_need_repaint = FALSE;
    if(_LB_GET_ELEMS_NUM(*ppt_ctx)==0)
    {
        return WGLR_OK;
    }

    DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_VISIBLE_RANGE, &ui4_range, &ui2_num_vp_elem, i4_ret);
    CATCH_FAIL(i4_ret);
    ui2_first = WGL_GET_HI_WORD(ui4_range);
    ui2_last  = WGL_GET_LOW_WORD(ui4_range);
    if(ui2_first >= _LB_GET_ELEMS_NUM(*ppt_ctx)||
       ui2_last  >= _LB_GET_ELEMS_NUM(*ppt_ctx)||
       ui2_first > ui2_last)
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        return WGLR_OK;
    }

    if(ui2_num_vp_elem == 0)
    {
        return _lb_tempunlock_on_elem_prev(ppt_ctx, b_repaint);
    }
    
___BLOCK_BEGIN___
    if(_HAS_HLT_ELEM(*ppt_ctx))
    {
        UINT16      ui2_new_hlt;

        if( _LB_GET_ELEMS_NUM(*ppt_ctx)==1 ||
            (ui2_first==0 && (ui2_last+(UINT16)1)==_LB_GET_ELEMS_NUM(*ppt_ctx) && (*ppt_ctx)->ui2_idx==0))
        {
            return WGLR_OK;
        }
        else if( (*ppt_ctx)->ui2_idx == 0 )
        { /* if current highlight element is at the first page */
            if(!HAS_STYLE(*ppt_ctx, WGL_STL_LB_NO_WRAP_OVER))
            {
                CATCH_FAIL(_lb_tempunlock_on_page_change_show_last_page(ppt_ctx, TRUE, &b_need_repaint));
                GOTO_EOB;
            }
            else
            {
                BOOL    b_is_vp_scrolled;
                CATCH_FAIL(_lb_check_elem_visibility_after_user_key(*ppt_ctx, (*ppt_ctx)->ui2_idx, TRUE, &b_is_vp_scrolled));
                b_need_repaint = b_is_vp_scrolled;
                GOTO_EOB;
            }
        }

        ui2_new_hlt     = WGL_LB_NULL_INDEX;
        if((*ppt_ctx)->ui2_idx != ui2_first)
        {
            UINT16  ui2_stop = (*ppt_ctx)->ui2_idx > ui2_first ? MIN((*ppt_ctx)->ui2_idx-(UINT16)1, ui2_last) : ui2_last;
            _lb_search_highlightable_elem_forward(*ppt_ctx, ui2_first, ui2_stop, &ui2_new_hlt);
        }
    
        if((*ppt_ctx)->ui2_idx == ui2_first || !_LB_VALID_IDX(*ppt_ctx, ui2_new_hlt))
        { /* show the previous page and try to auto highlight an element from the top of the viewport */
            CATCH_FAIL(_lb_tempunlock_on_page_up_prev_page(ppt_ctx, ui2_first, ui2_last, ui2_num_vp_elem, &b_need_repaint));
            GOTO_EOB;
        }
        else
        { /* highlight the highlightable element that is nearest to the top of the viewport */
        #ifdef WGL_ANIM_SUPPORT
            CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE, WGL_ANIM_ACTION_PAGE_UP));
        #else
            CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt,NULL, FALSE));        
        #endif
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_MOVE_ELEM_VISIBLE, ui2_new_hlt, ui2_new_hlt-ui2_first, i4_ret);
            CATCH_FAIL(i4_ret);
            b_need_repaint = TRUE;
            GOTO_EOB;
        }
    }
    else
    {
        UINT16          ui2_new_hlt;
        
        _lb_search_highlightable_elem_forward(*ppt_ctx, ui2_first, ui2_last, &ui2_new_hlt);
        if(_LB_VALID_IDX(*ppt_ctx, ui2_new_hlt))
        {
#ifdef WGL_ANIM_SUPPORT
            CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE, WGL_ANIM_ACTION_PAGE_UP));
#else
            CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt,NULL, FALSE));
#endif
            b_need_repaint = TRUE;
            /* [NOTE] don't need to move viewport because ui2_new_hlt must be in the range [ui2_first, ui2_last] */
            GOTO_EOB;
        }
        else
        {   /* show next page and try to auto highlight an element from the bottom of the viewport */
            CATCH_FAIL(_lb_tempunlock_on_page_up_prev_page(ppt_ctx, ui2_first, ui2_last, ui2_num_vp_elem, &b_need_repaint));
            GOTO_EOB;
        }
    }
___BLOCK_END___

    if(b_need_repaint && b_repaint)
    {
        CATCH_FAIL (_lb_tempunlock_repaint_full(ppt_ctx));
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_page_down
 *
 * Description: This API handles the page-down request.
 *
 * Inputs:  ppt_ctx    Contains the pointer of the instance context.
 *          b_repaint  Specifies whether to repaint or not.
 *
 * Outputs: ppt_ctx    Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_page_down(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_repaint)
{
    UINT16      ui2_first, ui2_last, ui2_num_vp_elem;
    UINT32      ui4_range;
    INT32       i4_ret;
    BOOL        b_need_repaint;

    b_need_repaint = FALSE;
    if(_LB_GET_ELEMS_NUM(*ppt_ctx) ==0)
    {
        return WGLR_OK;
    }

    DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_VISIBLE_RANGE, &ui4_range, &ui2_num_vp_elem, i4_ret);
    CATCH_FAIL(i4_ret);
    ui2_first = WGL_GET_HI_WORD(ui4_range);
    ui2_last  = WGL_GET_LOW_WORD(ui4_range);
    if( ui2_first >= _LB_GET_ELEMS_NUM(*ppt_ctx) ||
        ui2_last  >= _LB_GET_ELEMS_NUM(*ppt_ctx)||
        ui2_first > ui2_last)
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        return WGLR_OK;
    }

    if(ui2_num_vp_elem == 0)
    {
        return _lb_tempunlock_on_elem_next(ppt_ctx, b_repaint);
    }

___BLOCK_BEGIN___
    if(_HAS_HLT_ELEM(*ppt_ctx))
    {   
        UINT16      ui2_new_hlt;

        if(_LB_GET_ELEMS_NUM(*ppt_ctx) == 1 ||
          (ui2_first==0 && (ui2_last+(UINT16)1)==_LB_GET_ELEMS_NUM(*ppt_ctx)&& (*ppt_ctx)->ui2_idx==ui2_last) )
        {
            return WGLR_OK;
        }
        else if(((*ppt_ctx)->ui2_idx+(UINT16)1) == _LB_GET_ELEMS_NUM(*ppt_ctx))
        { /* if current highlight element is at the last page */
            if(!HAS_STYLE(*ppt_ctx, WGL_STL_LB_NO_WRAP_OVER))
            {
                CATCH_FAIL(_lb_tempunlock_on_page_change_show_first_page(ppt_ctx, ui2_first, ui2_num_vp_elem, FALSE, &b_need_repaint));
                GOTO_EOB;
            }
            else
            {
                BOOL    b_is_vp_scrolled;
                CATCH_FAIL(_lb_check_elem_visibility_after_user_key(*ppt_ctx, (*ppt_ctx)->ui2_idx, TRUE, &b_is_vp_scrolled));
                b_need_repaint = b_is_vp_scrolled;
                GOTO_EOB;
            }
        }
        
        ui2_new_hlt     = WGL_LB_NULL_INDEX;
        if((*ppt_ctx)->ui2_idx != ui2_last)
        {
            UINT16  ui2_stop = (*ppt_ctx)->ui2_idx < ui2_last ? MAX((*ppt_ctx)->ui2_idx+(UINT16)1, ui2_first) : ui2_first;
            _lb_search_highlightable_elem_backward(*ppt_ctx, ui2_last, ui2_stop, &ui2_new_hlt);
        }

        if((*ppt_ctx)->ui2_idx==ui2_last || !_LB_VALID_IDX(*ppt_ctx, ui2_new_hlt))
        {   /* show the next page and search another highlightable element from bottom of viewport */
            CATCH_FAIL(_lb_tempunlock_on_page_down_next_page(ppt_ctx, ui2_first, ui2_last, ui2_num_vp_elem, &b_need_repaint));
            GOTO_EOB;
        }
        else
        {   /* highlight ui2_new_hlt at some position of the viewport */
#ifdef WGL_ANIM_SUPPORT
            CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE, WGL_ANIM_ACTION_PAGE_DOWN));
#else
            CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE));
#endif
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_MOVE_ELEM_VISIBLE, ui2_new_hlt, ui2_new_hlt-ui2_first, i4_ret);
            CATCH_FAIL(i4_ret);
            b_need_repaint = TRUE;
            GOTO_EOB;
        }

    }
    else
    {   /* try to auto highlight an element from the bottom of the viewport,
           if there's no highlightable element, show next page and try to auto highlight an element again*/
        UINT16      ui2_new_hlt;

        _lb_search_highlightable_elem_backward(*ppt_ctx, ui2_last, ui2_first, &ui2_new_hlt);
        if(_LB_VALID_IDX(*ppt_ctx, ui2_new_hlt))
        {
#ifdef WGL_ANIM_SUPPORT
            CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE, WGL_ANIM_ACTION_PAGE_DOWN));
#else
            CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE));
#endif
            b_need_repaint = TRUE;
            /* [NOTE] don't need to move viewport because ui2_new_hlt must be in the range [ui2_first, ui2_last] */
            GOTO_EOB;
        }
        else
        {   /* show next page and try to auto highlight an element from the bottom of the viewport */
            CATCH_FAIL(_lb_tempunlock_on_page_down_next_page(ppt_ctx, ui2_first, ui2_last, ui2_num_vp_elem, &b_need_repaint));
            GOTO_EOB;
        }

    }
___BLOCK_END___

    if(b_need_repaint && b_repaint)
    {
        CATCH_FAIL (_lb_tempunlock_repaint_full (ppt_ctx));
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_page_up_prev_page
 *
 * Description: This API shows the previous page and try to auto highlight 
 *              an element from the top of the viewport
 *
 * Inputs:  ppt_ctx          Contains the pointer of the instance context.
 *          ui2_vi_first     Specifies the first visible element.
 *          ui2_vi_last      Specifies the last visible element.
 *          ui2_num_vp_elem  Specifies the element number in the viewport
 *
 * Outputs: ppt_ctx          Contains the pointer of the changed instance context.
 *          pb_need_repaint  Indicates if need to repaint.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_ARG     Invalid arguments
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_page_up_prev_page(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      UINT16                      ui2_vi_first,
    IN      UINT16                      ui2_vi_last,
    IN      UINT16                      ui2_num_vp_elem,
    OUT     BOOL                        *pb_need_repaint)
{
    INT32       i4_ret;
    
    *pb_need_repaint = FALSE;
    CHECK_VALID(ui2_num_vp_elem>0, WGLR_INV_ARG);

    if(ui2_vi_first == 0)
    { /* this page is the first page, so try to show the last page if warp-over mode is on */
        if(!HAS_STYLE(*ppt_ctx, WGL_STL_LB_NO_WRAP_OVER))
        {
            return _lb_tempunlock_on_page_change_show_last_page(ppt_ctx, TRUE, pb_need_repaint);
        }
        else
        {
            return WGLR_OK;
        }
    }
    else
    {
        if(ui2_vi_first < ui2_num_vp_elem)
        {   /* show the first page */
            return _lb_tempunlock_on_page_change_show_first_page(ppt_ctx, ui2_vi_first, ui2_num_vp_elem, TRUE, pb_need_repaint);
        }
        else
        {   /* show the previous page */
            UINT32      ui4_new_prev_page_range;
            UINT16      ui2_prev_page_first = ui2_vi_first - ui2_num_vp_elem;
            UINT16      ui2_new_prev_page_first, ui2_new_prev_page_last, ui2_temp, ui2_new_hlt;

            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_MOVE_ELEM_VISIBLE, ui2_prev_page_first, 0, i4_ret);
            CATCH_FAIL(i4_ret);
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_VISIBLE_RANGE, &ui4_new_prev_page_range, &ui2_temp, i4_ret);
            CATCH_FAIL(i4_ret);
            ui2_new_prev_page_first = WGL_GET_HI_WORD(ui4_new_prev_page_range);
            ui2_new_prev_page_last  = WGL_GET_LOW_WORD(ui4_new_prev_page_range);
            
            _lb_search_highlightable_elem_forward(*ppt_ctx, ui2_new_prev_page_first, ui2_new_prev_page_last, &ui2_new_hlt); 
            if(_LB_VALID_IDX(*ppt_ctx, ui2_new_hlt))
            {
#ifdef WGL_ANIM_SUPPORT
                CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE, WGL_ANIM_ACTION_INGORE));
#else
                CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE));
#endif
            }
        }
    }

    *pb_need_repaint = TRUE;
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_page_down_next_page
 *
 * Description: This API shows the next page and try to auto highlight 
 *              an element from the bottom of the viewport
 *
 * Inputs:  ppt_ctx          Contains the pointer of the instance context.
 *          ui2_vi_first     Specifies the first visible element.
 *          ui2_vi_last      Specifies the last visible element.
 *          ui2_num_vp_elem  Specifies the element number in the viewport
 *
 * Outputs: ppt_ctx          Contains the pointer of the changed instance context.
 *          pb_need_repaint  Indicates if need to repaint.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_ARG     Invalid arguments
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_page_down_next_page(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      UINT16                      ui2_vi_first,
    IN      UINT16                      ui2_vi_last,
    IN      UINT16                      ui2_num_vp_elem,
    OUT     BOOL                        *pb_need_repaint)
{
    INT32       i4_ret;

    *pb_need_repaint = FALSE;
    CHECK_VALID(ui2_num_vp_elem>0, WGLR_INV_ARG);

    if((ui2_vi_last+(UINT16)1) == _LB_GET_ELEMS_NUM(*ppt_ctx))
    { /* this page is the last page, so try to show the first page if wrap-over mode is on*/
        if(!HAS_STYLE(*ppt_ctx, WGL_STL_LB_NO_WRAP_OVER))
        {
            return _lb_tempunlock_on_page_change_show_first_page(ppt_ctx, ui2_vi_first, ui2_num_vp_elem, FALSE, pb_need_repaint);
        }
        else
        {
            return WGLR_OK;
        }                
    }
    else
    {            
        UINT16  ui2_next_page_first = ui2_vi_first + ui2_num_vp_elem;
        UINT16  ui2_next_page_last  = ui2_vi_last + ui2_num_vp_elem;
        
        if(ui2_next_page_last >= _LB_GET_ELEMS_NUM(*ppt_ctx))
        {   /* show the last page */
            return _lb_tempunlock_on_page_change_show_last_page(ppt_ctx, FALSE, pb_need_repaint);
        }
        else
        {   /* show the next page */
            UINT32      ui4_new_next_page_range;
            UINT16      ui2_new_next_page_first, ui2_new_next_page_last, ui2_temp, ui2_new_hlt;

            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_MOVE_ELEM_VISIBLE, ui2_next_page_first, 0, i4_ret);
            CATCH_FAIL(i4_ret);
            DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_VISIBLE_RANGE, &ui4_new_next_page_range, &ui2_temp, i4_ret);
            CATCH_FAIL(i4_ret);
            ui2_new_next_page_first = WGL_GET_HI_WORD(ui4_new_next_page_range);
            ui2_new_next_page_last  = WGL_GET_LOW_WORD(ui4_new_next_page_range);

            _lb_search_highlightable_elem_backward(*ppt_ctx, ui2_new_next_page_last, ui2_new_next_page_first, &ui2_new_hlt);
            if(_LB_VALID_IDX(*ppt_ctx, ui2_new_hlt))
            {
#ifdef WGL_ANIM_SUPPORT
                CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE, WGL_ANIM_ACTION_INGORE));
#else
                CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE));
#endif
            }
        }
    }
    
    *pb_need_repaint = TRUE;
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_page_change_show_first_page
 *
 * Description: This API shows the first page and try to auto highlight 
 *              an element.
 *
 * Inputs:  ppt_ctx                Contains the pointer of the instance context.
 *          ui2_vi_first           Specifies the first visible element.
 *          ui2_num_vp_elem        Specifies the element number in the viewport.
 *          b_search_hlt_from_top  Specifies the rule for searching an highlight element.
 *
 * Outputs: ppt_ctx          Contains the pointer of the changed instance context.
 *          pb_need_repaint  Indicates if need to repaint.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_ARG     Invalid arguments
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_page_change_show_first_page(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      UINT16                      ui2_vi_first,
    IN      UINT16                      ui2_num_vp_elem,
    IN      BOOL                        b_search_hlt_from_top,
    OUT     BOOL                        *pb_need_repaint)
{
    UINT16          ui2_new_hlt;
    INT32           i4_ret;
        
    *pb_need_repaint = FALSE;
    CHECK_VALID(ui2_num_vp_elem > 0, WGLR_INV_ARG);
    
    if(_LB_GET_ELEMS_NUM(*ppt_ctx)==0)
    {
        return WGLR_OK;
    }

    if(b_search_hlt_from_top)
    {
        _lb_search_highlightable_elem_forward(*ppt_ctx, 0, (UINT16)MIN(ui2_num_vp_elem-1, _LB_GET_ELEMS_NUM(*ppt_ctx)-1), &ui2_new_hlt);
    }
    else
    {
        _lb_search_highlightable_elem_backward(*ppt_ctx, (UINT16)MIN(ui2_num_vp_elem-1, _LB_GET_ELEMS_NUM(*ppt_ctx)-1), 0, &ui2_new_hlt);
    }
    
    if(_LB_VALID_IDX(*ppt_ctx, ui2_new_hlt))
    {
#ifdef WGL_ANIM_SUPPORT
        CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE, WGL_ANIM_ACTION_INGORE));
#else
        CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt,NULL, FALSE));
#endif
    }

    DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_MOVE_ELEM_VISIBLE, 0, 0, i4_ret);
    CATCH_FAIL(i4_ret);
    *pb_need_repaint = TRUE;

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_on_page_change_show_last_page
 *
 * Description: This API shows the last page and try to auto highlight 
 *              an element.
 *
 * Inputs:  ppt_ctx                Contains the pointer of the instance context.
 *          b_search_hlt_from_top  Specifies the rule for searching an highlight element.
 *
 * Outputs: ppt_ctx          Contains the pointer of the changed instance context.
 *          pb_need_repaint  Indicates if need to repaint.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_on_page_change_show_last_page(
    IN OUT  LB_CTX_T                    **ppt_ctx,
    IN      BOOL                        b_search_hlt_from_top,
    OUT     BOOL                        *pb_need_repaint)
{
    BOOL            b_is_vp_scrolled;
    UINT16          ui2_new_vi_first, ui2_new_vi_last, ui2_num_vp_elem, ui2_new_hlt;
    UINT32          ui4_new_vi_range;
    INT32           i4_ret;

    *pb_need_repaint = FALSE;

    DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_SHOW_LAST_PAGE, &b_is_vp_scrolled, NULL, i4_ret);
    CATCH_FAIL(i4_ret);

    DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_VISIBLE_RANGE, &ui4_new_vi_range, &ui2_num_vp_elem, i4_ret);
    CATCH_FAIL(i4_ret);
    ui2_new_vi_first = WGL_GET_HI_WORD(ui4_new_vi_range);
    ui2_new_vi_last  = WGL_GET_LOW_WORD(ui4_new_vi_range);

    if(b_search_hlt_from_top)
    {
        _lb_search_highlightable_elem_forward(*ppt_ctx, ui2_new_vi_first, ui2_new_vi_last, &ui2_new_hlt);
    }
    else
    {
        _lb_search_highlightable_elem_backward(*ppt_ctx, ui2_new_vi_last, ui2_new_vi_first, &ui2_new_hlt);
    }
    
    if(_LB_VALID_IDX(*ppt_ctx, ui2_new_hlt))
    {
#ifdef WGL_ANIM_SUPPORT
        CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE,WGL_ANIM_ACTION_INGORE));
#else
        CATCH_FAIL(_lb_tempunlock_highlight_elem(ppt_ctx, ui2_new_hlt, NULL, FALSE));
#endif
    }

    *pb_need_repaint = TRUE;

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_search_highlightable_elem_backward
 *
 * Description: This API searches an highlightable element in a range by backward direction.
 *
 * Inputs:  pt_ctx     Contains the instance context.
 *          ui2_start  Specifies the start of the search range.
 *          ui2_stop   Specifies the end of the search range.
 *
 * Outputs: pui2_new_hlt      Contains the index of the found element.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lb_search_highlightable_elem_backward(
    IN      LB_CTX_T                    *pt_ctx,
    IN      UINT16                      ui2_start,
    IN      UINT16                      ui2_stop,
    OUT     UINT16                      *pui2_new_hlt)
{
    UINT16          ui2_i;
    INT32           i4_ret;

    if(pui2_new_hlt != NULL)
    {
        *pui2_new_hlt     = WGL_LB_NULL_INDEX;
    }

    if(!(ui2_start<_LB_GET_ELEMS_NUM(pt_ctx) && ui2_stop<_LB_GET_ELEMS_NUM(pt_ctx)&& ui2_start>=ui2_stop))
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        return;
    }

    _LB_FIND_NO_ELEM_ST_INF(pt_ctx, 
                         LBEST_DISABLED,
                         ui2_start, 
                         1,
                         FALSE, 
                         &ui2_i, 
                         NULL);
    if(_LB_VALID_IDX(pt_ctx, ui2_i) && (ui2_i >=ui2_stop))
    {
        if(pui2_new_hlt != NULL)
        {
            *pui2_new_hlt     = ui2_i;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lb_search_highlightable_elem_backward
 *
 * Description: This API searches an highlightable element in a range by forward direction.
 *
 * Inputs:  pt_ctx     Contains the instance context.
 *          ui2_start  Specifies the start of the search range.
 *          ui2_stop   Specifies the end of the search range.
 *
 * Outputs: pui2_new_hlt      Contains the index of the found element.
 *          ppt_elem_new_hlt  Contains the pointer of the found element.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lb_search_highlightable_elem_forward(
    IN      LB_CTX_T                    *pt_ctx,
    IN      UINT16                      ui2_start,
    IN      UINT16                      ui2_stop,
    OUT     UINT16                      *pui2_new_hlt)
{
    UINT16          ui2_i;
    INT32           i4_ret;

    if(pui2_new_hlt != NULL)
    {
        *pui2_new_hlt     = WGL_LB_NULL_INDEX;
    }

    if(!( ui2_start<_LB_GET_ELEMS_NUM(pt_ctx) && ui2_stop<_LB_GET_ELEMS_NUM(pt_ctx) && ui2_start<=ui2_stop))
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        return;
    }

    _LB_FIND_NO_ELEM_ST_INF(pt_ctx, 
                         LBEST_DISABLED,
                         ui2_start, 
                         1,
                         TRUE, 
                         &ui2_i, 
                         NULL);
    if(_LB_VALID_IDX(pt_ctx, ui2_i) && (ui2_i <=ui2_stop))
    {
        if(pui2_new_hlt != NULL)
        {
            *pui2_new_hlt     = ui2_i;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lb_on_paint
 *
 * Description: This API handles WGL_MSG_PAINT message.
 *
 * Inputs:  pt_ctx     Contains the instance context.
 *          h_g        Contains the handle of the graphic context.
 *          ui2_state  Specifies the current visual state of the widget.
 *
 * Outputs: h_g        Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK         Routine successful.
 *          < 0             Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_on_paint(
    IN      LB_CTX_T                    *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      UINT16                      ui2_state)
{
    INT32           i4_ret;
    UINT32          ui4_num_rc_update;
    GL_RECT_T       t_rc_update;
    
    if( WGLR_OK != (i4_ret=x_wgl_get_update_region(pt_ctx->h_lb, &ui4_num_rc_update, &t_rc_update)) ||
        ui4_num_rc_update != 1 )
    {
        INFORM_ERR(i4_ret);
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        WGL_RECT_EMPTY(&t_rc_update);
    }

    INFORM_ERR (_lb_update_screen_rect (pt_ctx, TRUE));
    
    if( !IS_NULL_BORDER(pt_ctx) )
    {
        INFORM_ERR( pt_ctx->t_intf_bdr.pf_paint_border(pt_ctx->pv_ctx_bdr, h_g, 1, &t_rc_update, ui2_state) );
    }

    _lb_check_cache_inf(pt_ctx);

    i4_ret = pt_ctx->t_intf_cnt.pf_paint_content(pt_ctx->pv_ctx_cnt, h_g, 1, &t_rc_update, ui2_state);

    /* queck if content request timer */
    if( i4_ret == WGL_LBR_REQ_TIMER )
    {
        CATCH_FAIL( _lb_request_timers_from_cnt(pt_ctx) );
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_update_screen_rect
 *
 * Description: This API updates internal data when the size or location of listbox is changed.
 *
 * Inputs:  pt_ctx        Contains the instance context.
 *          b_size_fixed  Specifies if size was not changed. In the case of TRUE, 
 *                        the function process in a simple and efficient way.
 *
 * Outputs: pt_ctx  Contains the changed instance context.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_update_screen_rect(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN      BOOL                        b_size_fixed)
{
    GL_RECT_T       t_rc_scrn_lb;
    WGL_INSET_T     t_inset_bdr;
    INT32           i4_ret;

    CATCH_FAIL(x_wgl_get_coords(pt_ctx->h_lb, WGL_COORDS_SCREEN, &t_rc_scrn_lb));
    
    if( !b_size_fixed )
    {
        if( !IS_NULL_BORDER(pt_ctx) )
        {
            DO_BDR_CMD(pt_ctx, WGL_BDR_GL_SET_SIZE, RECT_W(&t_rc_scrn_lb), RECT_H(&t_rc_scrn_lb), i4_ret);
            CATCH_FAIL(i4_ret);
            DO_BDR_CMD(pt_ctx, WGL_CMD_GL_GET_INSET, &t_inset_bdr, NULL, i4_ret);
            CATCH_FAIL(i4_ret);
        }
        else
        {
            _INSET_CLEAR(&t_inset_bdr);
        }
    }
    
    DO_LB_CNT_CMD(pt_ctx, 
                  WGL_CNT_LB_SET_SCREEN_RECT, 
                  &t_rc_scrn_lb, 
                  b_size_fixed ? NULL : &t_inset_bdr, 
                  i4_ret);
                      
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_set_inset
 *
 * Description: This API updates the insets between content and border.
 *
 * Inputs:  pt_ctx   Contains the instance context.
 *          pt_inset Contains the new insets.
 *
 * Outputs: pt_ctx   Contains the changed instance context.
 *
 * Returns: WGLR_OK  Routine successful.
 *          < 0      Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_set_inset(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN      WGL_INSET_T                 *pt_inset)
{
    INT32           i4_ret;

    if(!CHECK_VALID_INSET(pt_inset))
    {
        return WGLR_INV_ARG;
    }

    DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_SET_BDR_INSET, WGL_PACK(pt_inset), NULL, i4_ret );

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_set_is_bdr_draw_interior
 *
 * Description: This API sets the flag indicating whether the border should draw 
 *              the background of the content area or not.
 *
 * Inputs:  pt_ctx                  Contains the instance context.
 *          b_is_bdr_draw_interior  Specifies the BOOL flag.
 *
 * Outputs: pt_ctx                  Contains the changed instance context.
 *
 * Returns: WGLR_OK                 Routine successful.
 *          < 0                     Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_set_is_bdr_draw_interior(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN      BOOL                        b_is_bdr_draw_interior)
{
    INT32       i4_ret;

    DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_SET_IS_BDR_DRAW_INTERIOR, WGL_PACK(b_is_bdr_draw_interior), NULL, i4_ret );
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tool_safe_w2s_strcpy
 *
 * Description: This API safely copies a UTF16_T string into a limited size 
 *              string buffer.
 *
 * Inputs:  pw2s_src     Contains the source string.
 *          ui4_max_len  Specifies the size of the destination string buffer 
 *                       in UTF16_T character count.
 *
 * Outputs: pw2s_dst     Contains the destination string buffer.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lb_tool_safe_w2s_strcpy(
    IN      LB_CTX_T                    *pt_ctx,
    OUT     UTF16_T                     *pw2s_dst, 
    IN      const UTF16_T               *pw2s_src,
    IN      UINT32                      ui4_max_len)
{
    /*TODO: optimize*/
    UINT32  ui4_src_len;
    UINT32  ui4_safe_len;

    if(ui4_max_len == 0)
    {
        x_uc_w2s_set(pw2s_dst, 0, WGL_CHAR_NULL);
        return;
    }

    ui4_src_len  = (UINT32)x_uc_w2s_strlen(pw2s_src);
    ui4_safe_len = (ui4_src_len > (ui4_max_len-1)) ? (ui4_max_len-1) : ui4_src_len;
    if(ui4_safe_len == 0)
    {
        x_uc_w2s_set(pw2s_dst, 0, WGL_CHAR_NULL);
        return;
    }

    x_uc_w2s_strncpy(pw2s_dst, pw2s_src, ui4_safe_len);
    x_uc_w2s_set(pw2s_dst, ui4_safe_len, WGL_CHAR_NULL);

    if(HAS_STYLE(pt_ctx, WGL_STL_LB_IGNORE_NEWLINE))
    {
        UTF16_T         w2s_force_break[] = WGL_FORCE_BREAK_CHARS;
        UINT32          ui4_len;
        UTF16_T         w2_ch;

        ui4_len = 0;

        do 
        {
            w2_ch = x_uc_w2s_get (pw2s_dst, ui4_len);
            
            if (w2_ch == ((UTF16_T) 0) )
            {
                break;
            }
            
            if (x_uc_w2s_chr (w2s_force_break, w2_ch))
            {
                x_uc_w2s_strcpy((UTF16_T*)(pw2s_dst + ui4_len), (UTF16_T*)(pw2s_dst + ui4_len + 1));
                if (ui4_len != 0)
                {
                    ui4_len --;
                }
            }
            
            ui4_len ++;
            
        } while (w2_ch != ((UTF16_T) 0));
    }

}

/*-----------------------------------------------------------------------------
 * Name: _lb_send_notify_hlt
 *
 * Description: This API sends a WGL_NC_LB_ELEM_HLT notification to the parent.
 *
 * Inputs:  h_parent  Specifies the handle of the parent.
 *          h_lb      Specifies the handle of the listbox.
 *          ui2_idx   Specifies the index of the highlighted or unhighlighted element.
 *          b_is_hlt  Specifies whether the element is being highlighted or unhighlighted.
 *
 * Outputs: -
 *
 * Returns: WGLR_OK   Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lb_send_notify_hlt(
    IN      HANDLE_T                    h_parent, 
    IN      HANDLE_T                    h_lb, 
    IN      UINT16                      ui2_idx, 
    IN      BOOL                        b_is_hlt)
{
    WGL_NOTIFY_DATA_T   t_notify_data;
    INT32               i4_ret;

    t_notify_data.ui4_nc_code                      = WGL_NC_LB_ELEM_HLT;
    t_notify_data.u.t_lb_elem_hlt.b_is_highlighted = b_is_hlt;
    t_notify_data.u.t_lb_elem_hlt.ui2_elem_idx     = ui2_idx;
    INFORM_ERR(x_wgl_send_notify(h_parent, WGL_NC_LB_ELEM_HLT, h_lb, &t_notify_data));
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_send_notify_sel
 *
 * Description: This API sends a WGL_NC_LB_ELEM_SEL notification to the parent.
 *
 * Inputs:  h_parent  Specifies the handle of the parent.
 *          h_lb      Specifies the handle of the listbox.
 *          ui2_idx   Specifies the index of the selected or deselected element.
 *          b_is_sel  Specifies whether the element is being selected or deselected.
 *
 * Outputs: -
 *
 * Returns: WGLR_OK   Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lb_send_notify_sel(
    IN      HANDLE_T                    h_parent, 
    IN      HANDLE_T                    h_lb, 
    IN      UINT16                      ui2_idx, 
    IN      BOOL                        b_is_sel)
{
    WGL_NOTIFY_DATA_T   t_notify_data;
    INT32               i4_ret;

    t_notify_data.ui4_nc_code                   = WGL_NC_LB_ELEM_SEL;
    t_notify_data.u.t_lb_elem_sel.b_is_selected = b_is_sel;
    t_notify_data.u.t_lb_elem_sel.ui2_elem_idx  = ui2_idx;
    INFORM_ERR(x_wgl_send_notify(h_parent, WGL_NC_LB_ELEM_SEL, h_lb, &t_notify_data));
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_send_notify_pushed
 *
 * Description: This API sends a WGL_NC_LB_ELEM_PUSH notification to the parent.
 *
 * Inputs:  h_parent     Specifies the handle of the parent.
 *          h_lb         Specifies the handle of the listbox.
 *          ui2_idx      Specifies the index of the pushed or unpushed element.
 *          b_is_pushed  Specifies whether the element is being pushed or unpushed.
 *
 * Outputs: -
 *
 * Returns: WGLR_OK   Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lb_send_notify_pushed(
    IN      HANDLE_T                    h_parent, 
    IN      HANDLE_T                    h_lb, 
    IN      UINT16                      ui2_idx, 
    IN      BOOL                        b_is_pushed)
{
    WGL_NOTIFY_DATA_T   t_notify_data;
    INT32               i4_ret;

    t_notify_data.ui4_nc_code                   = WGL_NC_LB_ELEM_PUSH;
    t_notify_data.u.t_lb_elem_push.b_is_pushed  = b_is_pushed;
    t_notify_data.u.t_lb_elem_push.ui2_elem_idx = ui2_idx;
    INFORM_ERR(x_wgl_send_notify(h_parent, WGL_NC_LB_ELEM_PUSH, h_lb, &t_notify_data));
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_send_notify_reach_boundary
 *
 * Description: This API sends a notification WGL_NC_LB_REACH_TOP or WGL_NC_LB_REACH_BOTTOM
 *              to the parent.
 *
 * Inputs:  ppt_ctx   Contains the pointer of the instance context.
 *          b_is_top  Specifies the top or the bottom is reached.
 *
 * Outputs: -
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_send_notify_reach_boundary(
    IN      LB_CTX_T                    **ppt_ctx, 
    IN      BOOL                        b_is_top)
{
    WGL_NOTIFY_DATA_T   t_notify_data;
    INT32               i4_ret;
    HANDLE_T            h_lb = (*ppt_ctx)->h_lb, h_parent = (*ppt_ctx)->h_parent;
    BOOL                b_suppress_notify = HAS_STYLE (*ppt_ctx, WGL_STL_LB_SUPPRESS_NFY) ? TRUE : FALSE;
    
    if (!b_suppress_notify)
    {
        REL_LOCK(h_lb, *ppt_ctx);
        
        t_notify_data.ui4_nc_code = b_is_top ? WGL_NC_LB_REACH_TOP : WGL_NC_LB_REACH_BOTTOM;
        INFORM_ERR(x_wgl_send_notify(h_parent, t_notify_data.ui4_nc_code, h_lb, &t_notify_data));
        
        GET_LOCK(h_lb, ppt_ctx);
    }
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_check_elem_visibility_after_user_key
 *
 * Description: This API checks an element's visibility after user changes 
 *              the highlighted element.And ensure the elem to be visible if 
 *              b_ensure_elem_visible equals TRUE.
 *
 * Inputs:  pt_ctx                  Contains the instance context.
 *          ui2_idx                 Specifies index of the new highlighted element.
 *          b_ensure_elem_visible   Specifies whether to ensure the elem to be visible.
 *
 * Outputs: pt_ctx             Contains the changed instance context.
 *          pb_is_vp_scrolled  Indicates the viewport was scrolled or not.
 *
 * Returns: WGLR_OK            Routine successful.
 *          < 0                 Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_check_elem_visibility_after_user_key(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN      UINT16                      ui2_idx,
    IN      BOOL                        b_ensure_elem_visible,
    OUT     BOOL                        *pb_is_vp_scrolled)
{
    INT32   i4_ret;

    if(HAS_STYLE(pt_ctx, WGL_STL_LB_NO_SCROLL))
    {
        if (b_ensure_elem_visible)
        {
            DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_ENSURE_ELEM_VISIBLE_NO_SCROLL, ui2_idx, pb_is_vp_scrolled, i4_ret);
            CATCH_FAIL(i4_ret);
        }
        else
        {
            LB_CNT_CEV_PARAM_T t_param = {0};
            DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_CHECK_ELEM_VISIBILITY_NO_SCROLL, ui2_idx, &t_param, i4_ret);
            CATCH_FAIL(i4_ret);
            *pb_is_vp_scrolled = t_param.b_is_vp_scrolled;
        } 
    }
    else
    {
        LB_CNT_EEVMM_PARAM_T    t_param1;
        t_param1.ui2_idx    = ui2_idx;
        t_param1.ui2_margin = pt_ctx->ui2_margin_hlt;
        if (b_ensure_elem_visible)
        {
            DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_ENSURE_ELEM_VISIBLE_MIN_MOVE, &t_param1, pb_is_vp_scrolled, i4_ret);
            CATCH_FAIL(i4_ret);
        }
        else
        {
            LB_CNT_CEV_PARAM_T t_param2 = {0};
            DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_CHECK_ELEM_VISIBILITY_MIN_MOVE, &t_param1, &t_param2, i4_ret);
            CATCH_FAIL(i4_ret);
            *pb_is_vp_scrolled = t_param2.b_is_vp_scrolled;
        }  
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_is_valid_image_handle
 *
 * Description: This API checks the validity of an image handle.
 *
 * Inputs:  h_img  Specifies the image hanlde to be checked.
 *
 * Outputs: -
 *
 * Returns: TRUE   If the image handle is valid.
 *          FALSE  Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lb_is_valid_image_handle(
    IN      HANDLE_T                    h_img)
{
    UINT32  ui4_tmp_w, ui4_tmp_h;
    if(WGLR_OK != x_wgl_img_tpl_get_size(h_img, &ui4_tmp_w, &ui4_tmp_h))
    {
        return FALSE;
    }
    
    return TRUE;
}    

/*-----------------------------------------------------------------------------
 * Name: _lb_is_valid_image_suit
 *
 * Description: This API checks the validity of an image suit.
 *
 * Inputs:  pt_img_suit  Specifies the image suit.
 *
 * Outputs: -
 *
 * Returns: TRUE   If the image suit is valid.
 *          FALSE  Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lb_is_valid_image_suit(
    IN      WGL_LB_ITEM_IMG_SUIT_T      *pt_img_suit)
{
    HANDLE_T    ah_img[10];
    UINT16      ui2_i;

    if(pt_img_suit==NULL)
    {
        return FALSE;   
    }
    ah_img[0] = pt_img_suit->h_normal;
    ah_img[1] = pt_img_suit->h_disable;
    ah_img[2] = pt_img_suit->h_highlight;
    ah_img[3] = pt_img_suit->h_highlight_unfocus;
    ah_img[4] = pt_img_suit->h_pushed;
    
    ah_img[5] = pt_img_suit->h_selected;
    ah_img[6] = pt_img_suit->h_selected_disable;
    ah_img[7] = pt_img_suit->h_selected_highlight;
    ah_img[8] = pt_img_suit->h_selected_highlight_unfocus;
    ah_img[9] = pt_img_suit->h_selected_pushed;
        
    for(ui2_i=0; ui2_i<10; ui2_i++)
    {
        if(!_IS_IMG_VALID(ah_img[ui2_i]))
        {
            return FALSE;
        }
    }
    
    return TRUE;    
}    
    
/*-----------------------------------------------------------------------------
 * Name: _lb_is_valid_image_info
 *
 * Description: This API checks the validity of an image info.
 *
 * Inputs:  pt_img_info  Specifies the image info.
 *
 * Outputs: -
 *
 * Returns: TRUE   If the image info is valid.
 *          FALSE  Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lb_is_valid_image_info(
    IN      WGL_IMG_INFO_T              *pt_img_info)
{
    HANDLE_T ah_img[5];
    UINT16 ui2_num_img = 0, ui2_i;
    
    if(!IS_LEGAL_IMG_SET_TYPE(pt_img_info->e_type))
    {
        return FALSE;
    }
    
    switch(pt_img_info->e_type) 
    {
        case WGL_IMG_SET_BASIC:
            ah_img[0] = pt_img_info->u_img_data.t_basic.t_disable;
            ah_img[1] = pt_img_info->u_img_data.t_basic.t_enable;
            ui2_num_img = 2;
            break;
        case WGL_IMG_SET_STANDARD:
            ah_img[0] = pt_img_info->u_img_data.t_standard.t_disable;
            ah_img[1] = pt_img_info->u_img_data.t_standard.t_enable;
            ah_img[2] = pt_img_info->u_img_data.t_standard.t_highlight;
            ui2_num_img = 3;
            break;
        case WGL_IMG_SET_EXTEND:
            ah_img[0] = pt_img_info->u_img_data.t_extend.t_disable;
            ah_img[1] = pt_img_info->u_img_data.t_extend.t_enable;
            ah_img[2] = pt_img_info->u_img_data.t_extend.t_highlight;
            ah_img[3] = pt_img_info->u_img_data.t_extend.t_highlight_inactive;
            ah_img[4] = pt_img_info->u_img_data.t_extend.t_push;
            ui2_num_img = 5;
            break;
    }

    for(ui2_i=0; ui2_i<ui2_num_img; ui2_i++)
    {
        if(!_IS_IMG_VALID(ah_img[ui2_i]))
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_req_update_by_hlt_nxt_prv_elem 
 *
 * Description: This function requests to update some regions of listbox after the 
 *              action for "next" or "previous" element highlighting is performed.
 *
 * Inputs:  pt_ctx            Contains the pointer of the instance context.
 *          b_is_vp_scrolled  Specifies if the viewport was ever scrolled or not.
 *          ui2_hlt_old       Specifies the index of the original highlighted element.
 *                            If no element is highlighted originally, it can be
 *                            specified as WGL_LB_NULL_INDEX.
 *          ui2_hlt_new       Specifies the index of the new highlighted element.
 *                            This argument cannot be equal to one of the following values: 
 *                              - WGL_LB_NULL_INDEX, or
 *                              - ui2_old_hlt.
 *
 * Outputs: pt_ctx            Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK           Routine successful.
 *          WGLR_INV_ARG      Invalid argument.
 *          < 0               Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_req_update_by_hlt_nxt_prv_elem (
    IN OUT  LB_CTX_T*                   pt_ctx,
    IN      BOOL                        b_is_vp_scrolled,
    IN      UINT16                      ui2_hlt_old,
    IN      UINT16                      ui2_hlt_new)
{
    INT32                       i4_ret;
#if 0    
    CHECK_VALID(
        ui2_hlt_new != WGL_LB_NULL_INDEX &&
        ui2_hlt_new != ui2_hlt_old, WGLR_INV_ARG);
#endif        
    if(b_is_vp_scrolled)
    {
        DO_LB_CNT_CMD (pt_ctx, WGL_CNT_LB_REQ_UPDATE_FULL, NULL, NULL, i4_ret);
        INFORM_ERR (i4_ret);
    }
    else
    {
        
        /* append an update rect to clear the old highlight element */
        if(ui2_hlt_old != WGL_LB_NULL_INDEX)
        {
            /* inform an element is unhighlighted, */
            /* to ensure the enlarged unhighlighted element is correctly repainted */
            DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_REQ_UPDATE_UNHLT_ELEM, ui2_hlt_old, NULL, i4_ret);
            INFORM_ERR(i4_ret);
        }        
        
        if (ui2_hlt_new != WGL_LB_NULL_INDEX)
        {
        /* append an update rect to repaint the new highlight element */
        DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_REQ_UPDATE_HLT_ELEM, ui2_hlt_new, NULL, i4_ret);
        INFORM_ERR(i4_ret);
        }
        
        /* append an update rect to update any viewport inidicators if necessary */
        DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_REQ_UPDATE_VP_INDRS, NULL, NULL, i4_ret);
        INFORM_ERR(i4_ret);
        
    }
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_req_update_by_range_elems
 *
 * Description: This function requests to update a region of listbox by
 *              an element range.
 *
 * Inputs:  pt_ctx            Contains the pointer of the instance context.
 *          b_is_vp_scrolled  Specifies if the viewport was ever scrolled or not.
 *          ui2_pui_first     Specifies the index of the first element.
 *          ui2_pui_first     Specifies the index of the last element.
 *
 * Outputs: pt_ctx            Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK           Routine successful.
 *          WGLR_INV_ARG      Invalid argument.
 *          < 0               Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_req_update_by_range_elems (
    IN OUT  LB_CTX_T*                   pt_ctx,
    IN      BOOL                        b_is_vp_scrolled,
    IN      UINT16                      ui2_pui_first,
    IN      UINT16                      ui2_pui_last)
{
    INT32                       i4_ret;
    
    CHECK_VALID(ui2_pui_last >= ui2_pui_first, WGLR_INV_ARG);

    if (b_is_vp_scrolled)
    {
        DO_LB_CNT_CMD (pt_ctx, WGL_CNT_LB_REQ_UPDATE_FULL, NULL, NULL, i4_ret);
        INFORM_ERR (i4_ret);
    }
    else
    {
        LB_CNT_PU_ELEM_RANGE_T      t_pu_elem_range;
        
        t_pu_elem_range.ui2_pui_first = ui2_pui_first;
        t_pu_elem_range.ui2_pui_last  = ui2_pui_last;
        
        DO_LB_CNT_CMD (pt_ctx, WGL_CNT_LB_REQ_UPDATE_RANGE_ELEMS, &t_pu_elem_range, NULL, i4_ret);
        INFORM_ERR (i4_ret);
    }
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_repaint_partial_by_item
 *
 * Description: This API partially repaints an area of the listbox by specifying 
 *              an item.
 *
 * Inputs:  ppt_ctx           Contains the pointer of the instance context.
 *          ui2_idx           Specifies the element index of the item.
 *          ui2_c             Specifies the column index of the item.
 *
 * Outputs: ppt_ctx           Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK           Routine successful.
 *          WGLR_INV_HANDLE   The widget has been destroyed.
 *          < 0               Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_repaint_partial_by_item(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_idx,
    IN      UINT8                       ui1_c)
{
    LB_CNT_PU_REGION_T      t_pu_region;
    LB_CNT_GURBI_PARAM_T    t_param;
    UINT32                  ui4_tk_start_pu;
    INT32                   i4_ret;

    ui4_tk_start_pu = os_get_sys_tick();
    
    CATCH_FAIL (x_wgl_get_state ((*ppt_ctx)->h_lb, &t_param.ui4_state_wgt));
    t_param.ui2_idx_elem = ui2_idx;
    t_param.ui1_idx_col  = ui1_c;
    
    DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_UPDATE_REGION_BY_ITEM, &t_param, &t_pu_region, i4_ret);
    CATCH_FAIL(i4_ret);
    
    CATCH_FAIL(_lb_tempunlock_repaint_region(ppt_ctx, ui4_tk_start_pu, &t_pu_region));

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_repaint_full 
 *
 * Description: This function repaints the whole listbox.
 *
 * Inputs:  ppt_ctx           Contains the pointer of the instance context.
 *
 * Outputs: ppt_ctx           Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK           Routine successful.
 *          WGLR_INV_HANDLE   The widget has been destroyed.
 *          < 0               Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_repaint_full (
    IN OUT  LB_CTX_T                    **ppt_ctx)
{
    INT32   i4_ret;
    
    DO_LB_CNT_CMD (*ppt_ctx, WGL_CNT_LB_REQ_UPDATE_FULL, NULL, NULL, i4_ret);
    INFORM_ERR (i4_ret);
    
    CATCH_FAIL (_lb_tempunlock_repaint_partial_generic (ppt_ctx));
    
    return WGLR_OK;
}
        
/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_repaint_partial_generic
 *
 * Description: This API partially repaints the areas that are needed to updated.
 *
 * Inputs:  ppt_ctx           Contains the pointer of the instance context.
 *
 * Outputs: ppt_ctx           Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK           Routine successful.
 *          WGLR_INV_HANDLE   The widget has been destroyed.
 *          < 0               Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_repaint_partial_generic(
    IN OUT  LB_CTX_T                    **ppt_ctx)
{
    LB_CNT_PU_REGION_T      t_pu_region;
    UINT32                  ui4_tk_start_pu;
    INT32                   i4_ret;

    ui4_tk_start_pu = os_get_sys_tick();
    
    DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_UPDATE_REGION_GENERIC, &t_pu_region, NULL, i4_ret);
    CATCH_FAIL(i4_ret);

    CATCH_FAIL(_lb_tempunlock_repaint_region(ppt_ctx, ui4_tk_start_pu, &t_pu_region));

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_repaint_region
 *
 * Description: This API repaints a set of areas.
 *
 * Inputs:  ppt_ctx          Contains the pointer of the instance context.
 *          ui4_tk_start_pu  Specifies the time moment at which 
 *                           the transaction for dirty regions repaints starts.
 *
 * Outputs: -
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_repaint_region(
    IN      LB_CTX_T                    **ppt_ctx, 
    IN      UINT32                      ui4_tk_start_pu,
    IN      LB_CNT_PU_REGION_T          *pt_pur)
{
    UINT16                  ui2_i;
    HANDLE_T                h_lb = (*ppt_ctx)->h_lb;
    INT32                   i4_ret;
    UINT32                  ui4_tk_last_fu;

    for(ui2_i=0; (ui2_i<pt_pur->ui2_num_rect) && (ui2_i<LB_CNT_PU_REGION_MAX_NUM_RECT); ui2_i++)
    {
        DO_LB_CNT_CMD(*ppt_ctx, WGL_CNT_LB_GET_LAST_FULL_UPDATE_TICK, &ui4_tk_last_fu, NULL, i4_ret);
        CATCH_FAIL(i4_ret);
        if(ui4_tk_start_pu < ui4_tk_last_fu)
        {   /* optimization: quick return if some full update occurs between the partial updates*/
            break;
        }
        REL_LOCK(h_lb, *ppt_ctx);
        
        INFORM_ERR(x_wgl_repaint(h_lb, &pt_pur->at_rcs[ui2_i], TRUE));
        
        GET_LOCK(h_lb, ppt_ctx);
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_set_elem_data
 *
 * Description: This API updates the data of an element.
 *
 * Inputs:  pt_ctx        Contains the instance context.
 *          pt_elem_data  Contains the new element data.
 *
 * Outputs: pt_ctx        Contains the changed instance context.
 *          pt_elem       Contains the changed element.
 *
 * Returns: WGLR_OK  Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lb_set_elem_data(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN OUT  LB_ELEM_T                   *pt_elem, 
    IN      WGL_LB_ELEM_DATA_T          *pt_elem_data)
{
    UINT8       ui1_c;
    INT32       i4_ret;

    if(pt_elem == NULL || pt_elem->pt_elem_inf == NULL)
    {
        return WGLR_OK;
    }

    if(pt_elem_data==NULL || pt_elem_data->at_items==NULL)
    {
        for(ui1_c=0; ui1_c<pt_ctx->ui1_col_num; ui1_c++)
        {
            switch(pt_ctx->at_cols[ui1_c].e_col_type) 
            {
                case LB_COL_TYPE_TEXT:
                {
#ifdef WGL_LB_NO_STORE_TEXT
                    if(!HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
#endif                        
                    {
                    x_uc_w2s_set(pt_elem->at_items[ui1_c].t_text.pw2s_text, 0, (UTF16_T)0);
                    x_uc_w2s_set(pt_elem->at_items[ui1_c].t_text.pw2s_raw_text, 0, (UTF16_T)0);
                }
                }
                break;
                case LB_COL_TYPE_IMG:
                    pt_elem->at_items[ui1_c].h_img = NULL_HANDLE;
                break;
                case LB_COL_TYPE_IMG_SUIT:
                    _IMG_SUIT_COPY(pt_elem->at_items[ui1_c].pt_img_suit, &NULL_LB_IMG_SUIT);
                break;
            }
        }
    }
    else
    {
        for(ui1_c=0; ui1_c<pt_ctx->ui1_col_num; ui1_c++)
        {
            switch(pt_ctx->at_cols[ui1_c].e_col_type) 
            {
                case LB_COL_TYPE_TEXT:
#ifdef WGL_LB_NO_STORE_TEXT
                    if(!HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
#endif                        
                    {
                        UTF16_T       *pw2s_dst = NULL;
                        UTF16_T       *pw2s_raw_data = NULL;
                        UTF16_T       *pw2s_src = NULL;
                        UINT16        ui2_max_len = 0;
                        VOID*         pv_reorder_attr=NULL;

                        pw2s_dst = pt_elem->at_items[ui1_c].t_text.pw2s_text;
                        pw2s_raw_data = pt_elem->at_items[ui1_c].t_text.pw2s_raw_text;
                        pw2s_src = pt_elem_data->at_items[ui1_c].pw2s_text;
                        ui2_max_len = pt_ctx->at_cols[ui1_c].ui2_max_text_len;

                        if(pt_elem_data->at_items[ui1_c].pw2s_text)
                        {
                            if (check_exist_arabic(pw2s_src, ui2_max_len))
                            {
                                UTF16_T*  w2s_buffer;

                                CATCH_FAIL(pt_ctx->t_intf_cnt.pf_content_cmd_proc(
                                                                                  pt_ctx->pv_ctx_cnt, 
                                                                                  WGL_CNT_LB_GET_BUFFER, 
                                                                                  WGL_PACK(&w2s_buffer), 
                                                                                  NULL));
                                x_memset(w2s_buffer,WGL_CHAR_NULL,(SIZE_T)(UTF16_MAX_LEN * ui2_max_len));                
                                if(pt_ctx->t_arab_reorder_attr.b_find_init_level == FALSE)
                                {
                                    pv_reorder_attr = &(pt_ctx->t_arab_reorder_attr);
                                }

                                i4_ret = reorder_arabic(pw2s_src, ui2_max_len, pv_reorder_attr,w2s_buffer,NULL); 
                                if (i4_ret != WGLR_OK)
                                {
                                    return i4_ret;
                                }

                                /*reset reorder attr after used for current string content*/
                                pt_ctx->t_arab_reorder_attr.b_find_init_level = TRUE;
                                pt_ctx->t_arab_reorder_attr.ui1_embedding_level = 0;

                                pw2s_dst  = x_uc_w2s_strcpy(pw2s_dst ,w2s_buffer);                  
                            }
                            else
                            {
                                _lb_tool_safe_w2s_strcpy(pt_ctx, pw2s_dst , pw2s_src, ui2_max_len );
                                _lb_tool_safe_w2s_strcpy(pt_ctx, pw2s_raw_data, pw2s_src,ui2_max_len);
                            }
                        }
                        else
                        {
                            x_uc_w2s_set(pw2s_dst, 0, (UTF16_T)0);
                            x_uc_w2s_set(pw2s_raw_data, 0, (UTF16_T)0);
                        }
                    }
                break;
                case LB_COL_TYPE_IMG:
                    pt_elem->at_items[ui1_c].h_img = pt_elem_data->at_items[ui1_c].h_img;
                break;
                case LB_COL_TYPE_IMG_SUIT:
                    if(pt_elem_data->at_items[ui1_c].pt_img_suit)
                    {
                        _IMG_SUIT_COPY(pt_elem->at_items[ui1_c].pt_img_suit, pt_elem_data->at_items[ui1_c].pt_img_suit);
                    }
                    else
                    {
                        _IMG_SUIT_COPY(pt_elem->at_items[ui1_c].pt_img_suit, &NULL_LB_IMG_SUIT);
                    }
                break;
            }
        }
    }

    WGL_SET_FLAG(pt_elem->pt_elem_inf->ui1_state, LBEST_CHACHED);
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_get_elem_data
 *
 * Description: This API retrieves the data of an element.
 *
 * Inputs:  pt_ctx            Contains the instance context.
 *          ui2_idx           Specifies the element index.
 *
 * Outputs: pt_elem_data_out  Contains the element data.
 *
 * Returns: WGLR_OK           Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lb_get_elem_data(
    IN      LB_CTX_T                    *pt_ctx, 
    IN      UINT16                      ui2_idx, 
    OUT     WGL_LB_ELEM_DATA_T          *pt_elem_data_out)
{
    LB_ELEM_T   *pt_elem = NULL;
    UINT8       ui1_c;

    if(!_LB_VALID_IDX(pt_ctx, ui2_idx) )
    {
         return WGLR_INV_ARG;
    }

    _lb_get_elem(pt_ctx, ui2_idx, &pt_elem, NULL);
    if (pt_elem != NULL)
    {
        for(ui1_c=0; ui1_c<pt_ctx->ui1_col_num; ui1_c++)
        {
            switch(pt_ctx->at_cols[ui1_c].e_col_type) 
            {
                case LB_COL_TYPE_TEXT:
#ifdef WGL_LB_NO_STORE_TEXT
                    if(!HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
#endif                        
                    {
                        _lb_tool_safe_w2s_strcpy(pt_ctx, 
                            pt_elem_data_out->at_items[ui1_c].pw2s_text, 
                            pt_elem->at_items[ui1_c].t_text.pw2s_raw_text, 
                            pt_ctx->at_cols[ui1_c].ui2_max_text_len);
                    }
                break;
                
                case LB_COL_TYPE_IMG:
                    pt_elem_data_out->at_items[ui1_c].h_img = pt_elem->at_items[ui1_c].h_img;
                break;
                case LB_COL_TYPE_IMG_SUIT:
                    _IMG_SUIT_COPY(pt_elem_data_out->at_items[ui1_c].pt_img_suit, pt_elem->at_items[ui1_c].pt_img_suit);
                break;
            }
        }
    
        return WGLR_OK;
    }
    else
    {
        WS_ASSERT(0);
        return WGLR_INTERNAL_ERROR;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lb_clear_elem_data
 *
 * Description: This API clears an element.
 *
 * Inputs:  pt_ctx   Contains the instance context.
 *          pt_elem  Contains the element data.
 *
 * Outputs: pt_ctx   Contains the changed instance context.
 *          pt_elem  Contains the changed element data.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lb_clear_elem_data(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN OUT  LB_ELEM_T                   *pt_elem)
{
    UINT8       ui1_c;

    if(pt_elem == NULL)
    {
        return;
    }

    for( ui1_c=0; ui1_c<pt_ctx->ui1_col_num; ui1_c++ )
    {
        switch(pt_ctx->at_cols[ui1_c].e_col_type) 
        {
            case LB_COL_TYPE_TEXT:
            {
                x_uc_w2s_set(pt_elem->at_items[ui1_c].t_text.pw2s_text, 0, (UTF16_T)0 );
                x_uc_w2s_set(pt_elem->at_items[ui1_c].t_text.pw2s_raw_text, 0, (UTF16_T)0 );
            }
            break;
            case LB_COL_TYPE_IMG:
                pt_elem->at_items[ui1_c].h_img = NULL_HANDLE;
            break;
            case LB_COL_TYPE_IMG_SUIT:
                _IMG_SUIT_COPY(pt_elem->at_items[ui1_c].pt_img_suit, &NULL_LB_IMG_SUIT);
            break;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lb_tempunlock_set_elem_num
 *
 * Description: This API changes the number of elements.
 *
 * Inputs:  ppt_ctx          Contains the pointer of the instance context.
 *          ui2_num_elem     Specifies the new element number.
 *
 * Outputs: ppt_ctx          Contains the pointer of the changed instance context.
 *
 * Returns: WGLR_OK          Routine successful.
 *          WGLR_INV_HANDLE  The widget has been destroyed.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_tempunlock_set_elem_num(
    IN OUT  LB_CTX_T                    **ppt_ctx, 
    IN      UINT16                      ui2_num_elem)
{
    UINT16      ui2_i;
    INT32       i4_ret;

    if(ui2_num_elem==0 && _LB_GET_ELEMS_NUM(*ppt_ctx)> 0)
    {
        CATCH_FAIL(_lb_clear(*ppt_ctx));
        /*Note: don't need to send any notification*/
    }
    else if(ui2_num_elem > _LB_GET_ELEMS_NUM(*ppt_ctx))
    {
        for(ui2_i=_LB_GET_ELEMS_NUM(*ppt_ctx); ui2_i<ui2_num_elem; ui2_i++)
        {
            CATCH_FAIL(_lb_tempunlock_insert_elem(ppt_ctx, 0, NULL, TRUE));
        }
    }
    else if(ui2_num_elem < _LB_GET_ELEMS_NUM(*ppt_ctx))
    {
        for(ui2_i=_LB_GET_ELEMS_NUM(*ppt_ctx); ui2_i>ui2_num_elem; ui2_i--)
        {
            CATCH_FAIL(_lb_tempunlock_del_elem(ppt_ctx, (UINT16)(ui2_i-1)));
        }
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_restore_all_elem_states
 *
 * Description: This API restores the states of all elements.
 *
 * Inputs:  pt_ctx        Contains the instance context.
 *          aui1_states   Contains the states of all elements.
 *
 * Outputs: pt_ctx        Contains the changed instance context.
 *
 * Returns: WGLR_OK       Routine successful.
 *          WGLR_INV_ARG  Invalid arguments.
 *          < 0           Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lb_restore_all_elem_states(
    IN OUT  LB_CTX_T                    *pt_ctx, 
    IN      UINT8                       *aui1_states)
{
    UINT16          ui2_i;
    UINT8*          pui1_state;
    INT32           i4_ret;

    if(aui1_states == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }
    
    i4_ret = _LB_SET_ELEMS_STATE_INF(pt_ctx, aui1_states, 0, _LB_GET_ELEMS_NUM(pt_ctx));
    if(i4_ret != WGLR_OK)
    {
        return WGLR_INTERNAL_ERROR;
    }

    pui1_state = aui1_states;
    pt_ctx->ui2_sel_num = 0;
    for(ui2_i=0; ui2_i < _LB_GET_ELEMS_NUM(pt_ctx); ui2_i++)
    {
        if(HAS_FLAG(*pui1_state, LBEST_HLT))
        {
            pt_ctx->ui2_idx = ui2_i;
        }
        if(HAS_FLAG(*pui1_state, LBEST_SEL))
        {
            pt_ctx->ui2_sel_num++;
        }
        pui1_state ++;
    }
    
    if(!_lb_debug_check_consistency(pt_ctx))
    {
        /*reset the state of all elements*/
        _lb_reset_all_elems_states(pt_ctx);
        return WGLR_INV_ARG;
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_reset_all_elems_states
 *
 * Description: This API resets the states of all the elements.
 *
 * Inputs:  pt_ctx   Contains the instance context.
 *
 * Outputs: pt_ctx   Contains the changed instance context.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lb_reset_all_elems_states(
    IN OUT  LB_CTX_T                    *pt_ctx)
{
    if(_LB_ELEMS_EMPTY(pt_ctx))
    {
        pt_ctx->ui2_idx     = WGL_LB_NULL_INDEX;
        pt_ctx->ui2_sel_num = 0;
        return;
    }

    _LB_RST_ELEMS_STATE(pt_ctx, 0, _LB_GET_ELEMS_NUM(pt_ctx));  
    pt_ctx->ui2_idx     = WGL_LB_NULL_INDEX;
    pt_ctx->ui2_sel_num = 0;

    if(HAS_STYLE(pt_ctx, WGL_STL_LB_FORCE_HLT))
    {
        _LB_SET_ELEM_STATE(pt_ctx, 0, LBEST_HLT);
        pt_ctx->ui2_idx = 0;
        if(HAS_STYLE(pt_ctx, WGL_STL_LB_AUTO_SEL))
        {
            _LB_SET_ELEM_STATE(pt_ctx, 0, LBEST_SEL);
            pt_ctx->ui2_sel_num = 1;
        }
    }
    if(HAS_STYLE(pt_ctx, WGL_STL_LB_FORCE_SEL) && pt_ctx->ui2_sel_num==0)
    {
        _LB_SET_ELEM_STATE(pt_ctx, 0, LBEST_SEL);
        pt_ctx->ui2_sel_num = 1;
    }
    
    _LB_CHECK_CONSISTENCY(pt_ctx);

}

/*-----------------------------------------------------------------------------
 * Name: _lb_request_timers_from_cnt
 *
 * Description: This API retrieves timer requests from content.
 *
 * Inputs:  pt_ctx   Contains the instance context.
 *
 * Outputs: pt_ctx   Contains the instance context.
 *
 * Returns: WGLR_OK  Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lb_request_timers_from_cnt(
    IN OUT  LB_CTX_T                    *pt_ctx)
{
    INT32                   i4_ret;
    LB_CNT_TIMER_REQ_T      t_timer_req;
    BOOL                    b_more_req;
    
    b_more_req = TRUE;
    do
    {
        DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_GET_REQ_TIMER, &t_timer_req, NULL, i4_ret);
        
        if( i4_ret == WGLR_OK )
        {
            HANDLE_T    h_timer;
            
            i4_ret = anm_trp_alloc( &pt_ctx->t_trp, t_timer_req.e_mode, &h_timer);
            
            if( WGL_IS_FAIL(i4_ret) )
            {
                INFORM_ERR(i4_ret);
                continue;
            }
            
            INFORM_ERR( x_wgl_start_timer( 
                h_timer, 
                t_timer_req.ui4_ms_delay, 
                X_TIMER_FLAG_ONCE,
                MAKE_TIMER_TAG(TIMER_TARGET_CNT, t_timer_req.ui2_id) ) );
        }
        else
        {
            INFORM_ERR( i4_ret );
            b_more_req = FALSE;
        }
    
    } while( b_more_req );
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _lb_msg_hdlr_on_timer
 *
 * Description: This API handles the WGL_MSG_TIMER.
 *
 * Inputs:  h_lb     Specifies the widget handle.
 *          h_timer  Specifies the timers handle.
 *          pv_tag   Specifies the timer tag.
 *          
 * Outputs: -
 *
 * Returns: WGLR_OK  Routine successful.
 *          < 0      Some error occurs.
 ----------------------------------------------------------------------------*/
static INT32 _lb_msg_hdlr_on_timer(
    IN      HANDLE_T                    h_lb,
    IN      HANDLE_T                    h_timer,
    IN      VOID*                       pv_tag)
{
    INT32           i4_ret, i4_ret_timer, i4_ret_fnl;
    LB_CTX_T*       pt_ctx = NULL;
    
    GET_LOCK(h_lb, &pt_ctx);
    
    i4_ret_timer = WGLR_OK;
    
    switch( TIMER_TAG_GET_TARGET(pv_tag) )
    {
        case TIMER_TARGET_CL:
            /* do nothing */
        break;
        
        case TIMER_TARGET_CNT:
            i4_ret_timer = _lb_timer_hdr_cnt(pt_ctx, TIMER_TAG_GET_REQ_ID(pv_tag) );
        break;
        
        case TIMER_TARGET_BDR:
            /* do nothing */
        break;
        
        default:
            INFORM_ERR( WGLR_INTERNAL_ERROR );
            i4_ret_timer = WGLR_INTERNAL_ERROR;
    }        
    
    /* free the timer resource */
    i4_ret_fnl = anm_trp_free( &pt_ctx->t_trp, h_timer );

    if (i4_ret_fnl == WGLR_OK)
    {
        /* post processing [PART-1] */
        switch( i4_ret_timer )
        {
            case WGL_LBR_NEED_REPAINT_PARTIAL:
                i4_ret_fnl = _lb_tempunlock_repaint_partial_generic (&pt_ctx);
                INFORM_ERR (i4_ret_fnl);
            break;
            
            case WGL_LBR_NEED_REPAINT_FULL:
                i4_ret_fnl = _lb_tempunlock_repaint_full (&pt_ctx);
                INFORM_ERR (i4_ret_fnl);
            break;
        }
    }
    else
    {
        INFORM_ERR (i4_ret_fnl);
    }
    
    /* release the instance */
    if( pt_ctx )
    {
        REL_LOCK(h_lb, pt_ctx);
    }
    
    /* post processing [PART-2] */
    if (i4_ret_fnl == WGLR_OK)
    {
        /* do nothing in current version */
    }
    
    return i4_ret_fnl;
}        

/*-----------------------------------------------------------------------------
 * Name: _lb_timer_hdr_cnt
 *
 * Description: This API delivers timer message to content.
 *
 * Inputs:  pt_ctx     Contains the instance context.
 *          ui2_tr_id  Specifies the timer request id.
 *
 * Outputs: -
 *
 * Returns: WGLR_OK  Routine successful.
 *          > 0      Some message returned.
 *          < 0      Some error occurs.
 ----------------------------------------------------------------------------*/
static INT32 _lb_timer_hdr_cnt(
    IN      LB_CTX_T*                   pt_ctx,
    IN      UINT16                      ui2_tr_id)
{
    INT32                       i4_ret, i4_ret_timer;
    LB_CNT_DO_TIMER_PARAM_T     t_param;
    
    CATCH_FAIL (x_wgl_get_state (pt_ctx->h_lb, &t_param.ui4_state_wgt));
    t_param.ui2_id_tr = ui2_tr_id;
    
    DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_DO_TIMER, &t_param, NULL, i4_ret_timer);
    INFORM_ERR(i4_ret_timer);
    
    return i4_ret_timer;
}    

/*-----------------------------------------------------------------------------
 * Name: _lb_debug_check_consistency
 *
 * Description: This API checks the consistency of the instance context.
 *
 * Inputs:  pt_ctx  Contains the instance context.
 *
 * Outputs: -
 *
 * Returns: TRUE    If passes the check.
 *          FALSE   Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lb_debug_check_consistency(
    IN      LB_CTX_T                    *pt_ctx)
{
    UINT16          ui2_i;
    UINT16          ui2_num_sel, ui2_num_hlt, ui2_num_dis;
    INT32           i4_ret;
    UINT16          ui2_cache_s, ui2_cache_e;
    LB_ELEM_INF_T*  pt_inf = NULL;
    LB_ELEM_INF_T*  pt_inf_prev = NULL;

    
    if(_LB_ELEMS_EMPTY(pt_ctx))
    {
        if(     _HAS_HLT_ELEM(pt_ctx) 
            ||  (pt_ctx->ui2_sel_num!=0 )
            ||  (pt_ctx->pt_elems!=NULL)
          )
        {
            return FALSE;
        }

        return TRUE;
    }

    ui2_num_sel = ui2_num_hlt = ui2_num_dis = 0;
    ui2_cache_s = _LB_GET_CACHE_START(pt_ctx);
    if( _LB_GET_ELEM_CACHE_NUM(pt_ctx) == 0)
    {
        ui2_cache_e = ui2_cache_s;
    }
    else
    {
        ui2_cache_e = ui2_cache_s + _LB_GET_ELEM_CACHE_NUM(pt_ctx) - (UINT16)1;
    }

    if((ui2_cache_e - ui2_cache_s) >= _LB_GET_ELEMS_NUM(pt_ctx))
    {
        return FALSE;
    }
    
    pt_inf = DLIST_HEAD(&pt_ctx->t_elem_inf_list);
    if(pt_inf == NULL)
    {
        return FALSE;
    } 

    for(ui2_i = 0; ui2_i<=_LB_GET_ELEMS_NUM(pt_ctx); ui2_i++)
    {
        if(DLIST_PREV(pt_inf, t_link) != pt_inf_prev  || 
           (HAS_FLAG(pt_inf->ui1_state, LBEST_PUSHED) && 
            !HAS_FLAG(pt_inf->ui1_state, LBEST_HLT)))
        {
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            return FALSE;
        }

        if(HAS_FLAG(pt_inf->ui1_state, LBEST_HLT))
        {
            if(    (ui2_i != pt_ctx->ui2_idx)
                || HAS_FLAG(pt_inf->ui1_state, LBEST_DISABLED)
                || (HAS_STYLE(pt_ctx, WGL_STL_LB_AUTO_SEL) 
                     && !HAS_FLAG(pt_inf->ui1_state, LBEST_SEL))
              )
            {
                INFORM_ERR(WGLR_INTERNAL_ERROR);
                return FALSE;
            }
            ui2_num_hlt++;
        }
        if(HAS_FLAG(pt_inf->ui1_state, LBEST_SEL))
        {
            ui2_num_sel++;
        }
        if(HAS_FLAG(pt_inf->ui1_state, LBEST_DISABLED))
        {
            ui2_num_dis++;
        }

        pt_inf_prev = pt_inf;
        pt_inf = DLIST_NEXT(pt_inf, t_link);
    }

    if(     (!_HAS_HLT_ELEM(pt_ctx) && ui2_num_hlt!=0)
        ||  (_HAS_HLT_ELEM(pt_ctx)  && ui2_num_hlt!=1) 
        ||  (ui2_num_dis == _LB_GET_ELEMS_NUM(pt_ctx) && ui2_num_hlt!=0)
        ||  (ui2_num_sel != pt_ctx->ui2_sel_num)
        )
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        return FALSE;
    }

    if(HAS_STYLE(pt_ctx, WGL_STL_LB_FORCE_HLT))
    {
        if(ui2_num_dis < _LB_GET_ELEMS_NUM(pt_ctx)
            && ui2_num_hlt==0)
        {
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            return FALSE;
        }
    }
    
    if(HAS_STYLE(pt_ctx, WGL_STL_LB_FORCE_SEL))
    {
        if(ui2_num_sel==0)
        {
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            return FALSE;
        }
    }

    if( ! HAS_STYLE(pt_ctx, WGL_STL_LB_MULTI_SEL))
    {
        if(ui2_num_sel >= 2)
        {
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            return FALSE;
        }
    }    

    if( HAS_STYLE(pt_ctx, WGL_STL_LB_AUTO_SEL) )
    {
        if(  ui2_num_sel >= 2
            || (_HAS_HLT_ELEM(pt_ctx) && ui2_num_sel!=1)
          )
        {
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            return FALSE;
        }
    }

    return TRUE;
}

#ifdef WGL_ANIM_SUPPORT
static INT32 _wgl_lb_anim_get_begin_data(
    LB_CTX_T*                       pt_ctx,
    GL_RECT_T                       t_from_rect,
    GL_RECT_T                       t_end_rect,
    UINT8                           ui1_alpha_s,
    UINT8                           ui1_alpha_e,
    WGL_ANIMATION_DATA_T*           pt_begin)
{
    UINT16                          ui2_count = 0;
    UINT16                          ui2_i ;
    UINT16                          ui2_path_idx ;
    WGL_ANIMATION_PATH_MODE_T       t_path;
    BOOL                            b_find;
    BOOL                            b_free_mem = FALSE;
    WGL_ANIM_PATH_DATA_T*           pt_path_data;
    WGL_ANIM_PATH_DATA_T*           pt_path_tmp;
    WGL_ANIM_DATA_INFO*             pt_data;
    UINT16                          aui2_count[WGL_ANIM_MAX_PATH_CNT_ONCE];
    UINT16                          ui2_end ;
    UINT16                          ui2_perc_s;
    UINT16                          ui2_perc_e;
    INT32                           i4_tmp;
    UINT32                          ui4_size;
    
    if( (pt_ctx == NULL) || (pt_begin == NULL) || RECT_EMPTY(&t_from_rect) ||RECT_EMPTY(&t_end_rect))
    {
        return WGLR_INV_ARG;
    }

    x_memset(aui2_count,0,sizeof(UINT16)*WGL_ANIM_MAX_PATH_CNT_ONCE);
    if(pt_begin->t_path_id_inf.ui2_path_cnt >= WGL_ANIM_MAX_PATH_CNT_ONCE)
    {
        pt_begin->t_path_id_inf.ui2_path_cnt =(UINT16)(WGL_ANIM_MAX_PATH_CNT_ONCE - 1);
    }
        
    if(HAS_FLAG(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF))
    {
        if(HAS_FLAG(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_ID_INF))
        {
            return WGLR_INV_ARG;
        }
        ui2_count = pt_begin->ui2_count;
        pt_path_data = pt_begin->pt_path_mode;
    }
    else if(HAS_FLAG(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_ID_INF))
    {
        for(ui2_i = 0; ui2_i < pt_begin->t_path_id_inf.ui2_path_cnt; ui2_i ++)
        {
            b_find = wgl_anim_lib_get_path(pt_begin->t_path_id_inf.aui4_path_id[ui2_i],&t_path);
            if(b_find) 
            {
                ui2_count += t_path.ui2_count;
            }
            else
            {
                return WGLR_INV_ARG;
            }            
        }

        /*Alloc memory for the path data information*/
        if(ui2_count == 0)
        {
           return WGLR_INTERNAL_ERROR;
        }        
        pt_path_data = (WGL_ANIM_PATH_DATA_T*)WGL_MEM_ALLOC(sizeof(WGL_ANIM_PATH_DATA_T)*ui2_count);
        if(pt_path_data == NULL)
        {
            return WGLR_OUT_OF_MEMORY;
        }
        x_memset(pt_path_data,0,sizeof(WGL_ANIM_PATH_DATA_T)*ui2_count);
        
        pt_path_tmp = pt_path_data;
        
        for(ui2_i = 0; ui2_i < pt_begin->t_path_id_inf.ui2_path_cnt; ui2_i ++)
        {
            b_find = wgl_anim_lib_get_path(pt_begin->t_path_id_inf.aui4_path_id[ui2_i],&t_path);
            if(b_find) 
            {
                x_memcpy(pt_path_tmp,t_path.pt_path_inf,sizeof(WGL_ANIM_PATH_DATA_T)*t_path.ui2_count);  
                pt_path_tmp += t_path.ui2_count;
                aui2_count[ui2_i]= t_path.ui2_count;
            }
        } 
        pt_begin->ui2_count = ui2_count;
        b_free_mem = TRUE;
    }
    else
    {
       return WGLR_INV_ARG;
    }

    if(ui2_count == 0 || pt_path_data == NULL)
    {
       return WGLR_INTERNAL_ERROR;
    }

    /*Set the data information for begin data*/
    ui4_size = sizeof(WGL_ANIM_DATA_INFO)*ui2_count;
    pt_data = (WGL_ANIM_DATA_INFO*)WGL_MEM_ALLOC(ui4_size);
    if(pt_data == NULL)
    {
        if(b_free_mem == TRUE)
        {
            WGL_MEM_FREE(pt_path_data);
        }    
        return WGLR_OUT_OF_MEMORY;
    }
#ifdef CLI_LVL_ALL     
    pt_ctx->ui4_mem_size += ui4_size;
#endif  
    x_memset(pt_data,0,sizeof(WGL_ANIM_DATA_INFO)*ui2_count);
    pt_begin->pt_anim_data = pt_data;
    
    pt_path_tmp = pt_path_data;  
    ui2_end = 0;
    
    for(ui2_i = 0; ui2_i < ui2_count; ui2_i ++)
    {
        for(ui2_path_idx=0; ui2_path_idx < pt_begin->t_path_id_inf.ui2_path_cnt; ui2_path_idx ++)
        {
            ui2_end += aui2_count[ui2_path_idx];
            if(ui2_i < ui2_end)
            {
                break;
            }
        }

        ui2_perc_s = pt_begin->t_path_id_inf.aui2_start_percent[ui2_path_idx];
        ui2_perc_e = pt_begin->t_path_id_inf.aui2_end_percent[ui2_path_idx];
        
        switch(pt_path_tmp->ui2_anim_type)
        {
        case WGL_ANIM_TYPE_MOVE:
            if(RECT_INCLUDED(&t_from_rect,&t_end_rect) && RECT_INCLUDED(&t_end_rect,&t_from_rect))
            {
                return WGLR_INV_ARG;
            }
            i4_tmp = t_from_rect.i4_left + (t_end_rect.i4_left - t_from_rect.i4_left)*ui2_perc_s/100;
            pt_data->u_data.t_move.i2_start_x = (INT16)i4_tmp;

            i4_tmp = t_from_rect.i4_left + (t_end_rect.i4_left - t_from_rect.i4_left)*ui2_perc_e/100;            
            pt_data->u_data.t_move.i2_end_x = (INT16)i4_tmp;

            i4_tmp = t_from_rect.i4_top + (t_end_rect.i4_top - t_from_rect.i4_top)*ui2_perc_s/100;
            pt_data->u_data.t_move.i2_start_y = (INT16)i4_tmp;

            i4_tmp = t_from_rect.i4_top + (t_end_rect.i4_top - t_from_rect.i4_top)*ui2_perc_e/100;
            pt_data->u_data.t_move.i2_end_y = (INT16)i4_tmp;
            break;

        case WGL_ANIM_TYPE_ALPHA:
            i4_tmp = ui1_alpha_s + ((INT32)(ui1_alpha_e - ui1_alpha_s))*ui2_perc_s / 100;
            if(i4_tmp >= 0)
            {
                pt_data->u_data.t_alpha.ui1_start_alpha = (UINT8)i4_tmp;
            }
            else
            {
                pt_data->u_data.t_alpha.ui1_start_alpha = 0;
            }
            
            i4_tmp = ui1_alpha_s + ((INT32)(ui1_alpha_e - ui1_alpha_s))*ui2_perc_e / 100;
            if(i4_tmp >= 0)
            {
                pt_data->u_data.t_alpha.ui1_end_alpha = (UINT8)i4_tmp;
            }
            else
            {
                pt_data->u_data.t_alpha.ui1_end_alpha = 0;
            }            
            break;

        case WGL_ANIM_TYPE_SCALE:
            break;

        case WGL_ANIM_TYPE_IMG:
            break;
            
        default :
            break;
        }

        pt_data->ui2_anim_type = pt_path_tmp->ui2_anim_type;

        pt_data ++;
        pt_path_tmp ++;
    }

    WGL_SET_FLAG(pt_begin->ui4_flag, WGL_ANIM_DATA_FLAG_DATA_INF);

    if(b_free_mem == TRUE)
    {
        WGL_MEM_FREE(pt_path_data);
    }
    
    return WGLR_OK;
}

static UINT32 _lb_get_actmap_flag(
    LB_CTX_T*                       pt_ctx,
    WGL_ANIM_ACTION_TYPE_T          e_action_type)
{
    CHECK_VALID((pt_ctx != NULL), 0);

    if(e_action_type > WGL_LB_ANIM_ACT_END)
    {
        return 0;
    } 

    return pt_ctx->at_actmap[e_action_type].ui4_flag;
}

static INT32 _lb_begin_animation(
    LB_CTX_T*                       pt_ctx,
    WGL_ANIM_ACTION_TYPE_T          e_action_type)
{
    WGL_ANIMATION_DATA_T*           pt_anim_data;
    WGL_ANIM_PATH_DATA_T*           pt_path;
    UINT32                          ui4_size;
    UINT8                           ui1_alpha_s;
    UINT8                           ui1_alpha_e;
    INT32                           i4_ret;
    
    CHECK_VALID((pt_ctx != NULL), WGLR_INV_ARG);

    if(e_action_type > WGL_LB_ANIM_ACT_END)
    {
        return WGLR_INV_ARG;
    }

    pt_anim_data = &pt_ctx->at_actmap[e_action_type].t_anim_data;
    if(pt_anim_data->ui2_count == 0)
    {
        return WGLR_INV_ARG;
    }

    wgl_anim_data_free(&pt_ctx->t_begin_data);

    if(_LB_ANIM_EN(pt_ctx))
    {
        x_memcpy(&pt_ctx->t_begin_data.t_path_id_inf,
                 &pt_anim_data->t_path_id_inf,
                 sizeof(WGL_ANIM_PATH_ID_INFO_T));
        WGL_SET_FLAG(pt_ctx->t_begin_data.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_ID_INF);
    }
    else if(HAS_STYLE(pt_ctx,WGL_STL_LB_ANIM_BY_CUST_PATH) &&
       HAS_FLAG(pt_anim_data->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF))
    {
        ui4_size = (UINT32)(sizeof(WGL_ANIM_PATH_DATA_T)*pt_anim_data->ui2_count);
        pt_path = (WGL_ANIM_PATH_DATA_T*)WGL_MEM_ALLOC(ui4_size);
        if(pt_path == NULL)
        {
            return WGLR_OUT_OF_MEMORY;
        }
        x_memcpy(pt_path,pt_anim_data->pt_path_mode,ui4_size);
        pt_ctx->t_begin_data.pt_path_mode = pt_path;
        pt_ctx->t_begin_data.ui2_count = pt_anim_data->ui2_count;
        WGL_SET_FLAG(pt_ctx->t_begin_data.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF);
#ifdef CLI_LVL_ALL     
        pt_ctx->ui4_mem_size += ui4_size;
#endif  

    }

    DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_GET_ANIM_ALPHA_INF, &ui1_alpha_s, &ui1_alpha_e, i4_ret);

    i4_ret = _wgl_lb_anim_get_begin_data(pt_ctx,
                                pt_ctx->t_anim_s,
                                pt_ctx->t_anim_e,
                                ui1_alpha_s,
                                ui1_alpha_e,
                                &pt_ctx->t_begin_data);
    if(i4_ret != WGLR_OK)
    {
        return WGLR_INTERNAL_ERROR;
    }    
        
    return WGLR_OK;
}

static INT32 _lb_elem_begin_anim(
    LB_CTX_T**                      ppt_ctx,
    WGL_ANIM_ACTION_TYPE_T          e_action_type)
{
    INT32                           i4_ret;
    LB_ELEM_T                       *pt_anim_elem = NULL, *pt_hlt_elem = NULL;
    LB_ELEM_INF_T               *pt_elem_inf = NULL;
    
    CHECK_VALID((ppt_ctx != NULL), WGLR_INV_ARG);

    if(!HAS_FLAG((*ppt_ctx)->ui1_state, LBST_ANIM_CREATED))
    {
        return WGLR_INV_ARG;
    }
    
    i4_ret = _lb_begin_animation((*ppt_ctx),e_action_type);
    
    if(i4_ret == WGLR_OK)
    {
        HANDLE_T                        h_lb;
        WGL_ANIMATION_DATA_T            t_begin_data;
        WGL_ANIM_CONTEXT_T              t_anim_context;
        
        h_lb = (*ppt_ctx)->h_lb;
        x_memcpy(&t_begin_data, &(*ppt_ctx)->t_begin_data, sizeof(WGL_ANIMATION_DATA_T));
        t_anim_context = (*ppt_ctx)->at_actmap[e_action_type].t_anim_context;

        REL_LOCK(h_lb, *ppt_ctx);
        i4_ret = wgl_begin_animation(h_lb,
                            &t_begin_data,
                            WGL_ANIM_ATTR_WIDGET_ELEMENT,
                            t_anim_context);
        GET_LOCK(h_lb, ppt_ctx);
    }

    CHECK_NULL(ppt_ctx,WGLR_INV_ARG);
    CHECK_NULL(*ppt_ctx,WGLR_INV_ARG);
    
    if(i4_ret == WGLR_OK)
    {
        WGL_SET_FLAG((*ppt_ctx)->ui1_state, LBST_ANIM_DOING);
    }
    else
    {
        if((*ppt_ctx)->ui2_anim_idx != WGL_LB_NULL_INDEX)
        {
            _lb_get_elem((*ppt_ctx), (*ppt_ctx)->ui2_anim_idx, &pt_anim_elem,&pt_elem_inf);
            if(pt_elem_inf == NULL)
            {
                return WGLR_INTERNAL_ERROR;
            }
            if(pt_elem_inf)
            {
                WGL_CLEAR_FLAG(pt_elem_inf->ui1_state, LBEST_ANIM);
                (*ppt_ctx)->ui2_anim_idx = WGL_LB_NULL_INDEX;
            }
        }

        if((*ppt_ctx)->ui2_anim_hlt != WGL_LB_NULL_INDEX)
        {
            _lb_get_elem((*ppt_ctx), (*ppt_ctx)->ui2_anim_hlt, &pt_hlt_elem, &pt_elem_inf);
            if(pt_elem_inf)
            {
                WGL_SET_FLAG(pt_elem_inf->ui1_state, LBEST_HLT);
                (*ppt_ctx)->ui2_idx = (*ppt_ctx)->ui2_anim_hlt;
                (*ppt_ctx)->ui2_anim_hlt = WGL_LB_NULL_INDEX;
            }
        }
    }
    
    return WGLR_OK;
}

static INT32 _lb_elem_set_anim_info(
    LB_CTX_T*                       pt_ctx,
    UINT16                          ui2_anim,
    UINT16                          ui2_hlt,
    BOOL*                           pb_do_anim)
{
    INT32                           i4_ret;
    LB_ELEM_T                       *pt_anim_elem = NULL, *pt_hlt_elem = NULL;
    LB_ELEM_INF_T                   *pt_elem_inf = NULL;
    LB_ELEM_INF_T                   *pt_hlt_inf = NULL;
    LB_CNT_EEVMM_PARAM_T            t_param1;

    if(pb_do_anim)
    {
        *pb_do_anim = FALSE;
    }

    CHECK_VALID((pt_ctx != NULL), WGLR_INV_ARG);
    
    if(ui2_anim != WGL_LB_NULL_INDEX)
    {
        _lb_get_elem(pt_ctx, ui2_anim, &pt_anim_elem, &pt_elem_inf);
    }

    if(ui2_hlt != WGL_LB_NULL_INDEX)
    {
        _lb_get_elem(pt_ctx, ui2_hlt, &pt_hlt_elem, &pt_hlt_inf);
    }
    
    if( _LB_ANIM_EN(pt_ctx))
    {
        if(pt_anim_elem && pt_elem_inf)
        {
            pt_ctx->ui2_anim_idx = ui2_anim;
            WGL_SET_FLAG(pt_elem_inf->ui1_state, LBEST_ANIM);
        }
        else if(pt_hlt_elem && pt_hlt_inf)
        {
            pt_ctx->ui2_anim_idx = ui2_hlt;
            WGL_SET_FLAG(pt_hlt_inf->ui1_state, LBEST_ANIM);
        }
        else
        {
            return WGLR_OK;
        }
        
        pt_ctx->ui2_anim_hlt = ui2_hlt;
        
        /*If the new hlt element is to do animation,not highlight first,it repain the animation.*/
        t_param1.ui2_idx    = pt_ctx->ui2_anim_idx;
        t_param1.ui2_margin = pt_ctx->ui2_margin_hlt;
        DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_GET_ANIM_ELEM_AREA, &t_param1, &pt_ctx->t_anim_s, i4_ret);
        INFORM_ERR(i4_ret);
        t_param1.ui2_idx    = pt_ctx->ui2_anim_hlt;
        DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_GET_ANIM_ELEM_AREA, &t_param1, &pt_ctx->t_anim_e, i4_ret);
        INFORM_ERR(i4_ret);
        if(pb_do_anim)
        {
            *pb_do_anim = TRUE;
        }        
    }
    else if(pt_hlt_elem != NULL && pt_hlt_inf)
    {
        WGL_SET_FLAG(pt_hlt_inf->ui1_state, LBEST_HLT);
        pt_ctx->ui2_idx = ui2_hlt;
    }
    
    return WGLR_OK;
}

static INT32 _lb_create_anim(
    LB_CTX_T*                           pt_ctx,
    BOOL                                b_by_style,
    UINT32                              ui4_style)
{
    INT32                                   i4_ret = WGLR_OK;
    BOOL                                    b_has_dft;
    
    CHECK_VALID((pt_ctx != NULL), WGLR_INV_ARG);

    b_has_dft = wgl_anim_has_dft_map_tbl(HT_WGL_WIDGET_LIST_BOX,WGL_LB_ANIM_ACT_START,WGL_LB_ANIM_ACT_END);
    if(b_has_dft == TRUE)
    {
        if(HAS_FLAG(pt_ctx->ui1_state, LBST_ANIM_CREATED))
        {
            wgl_anim_free(&pt_ctx->t_anim);
        }
        
        i4_ret = wgl_anim_create(&pt_ctx->t_anim);
        
        if(i4_ret != WGLR_OK)
        {
            WGL_CLEAR_FLAG(pt_ctx->ui1_state, LBST_ANIM_CREATED);
            return WGLR_INTERNAL_ERROR;
        }
        
        wgl_anim_free_map_tbl(pt_ctx->at_actmap, WGL_LB_ANIM_ACT_START, WGL_LB_ANIM_ACT_END);
        i4_ret = wgl_anim_get_default_map_tbl(HT_WGL_WIDGET_LIST_BOX,
                                 pt_ctx->at_actmap,
                                 WGL_LB_ANIM_ACT_START,
                                 WGL_LB_ANIM_ACT_END);  
        if(i4_ret != WGLR_OK)
        {
            wgl_anim_free(&pt_ctx->t_anim);
        }
        
        WGL_SET_FLAG(pt_ctx->ui1_state, LBST_ANIM_CREATED);
        
        if( b_by_style == TRUE && HAS_FLAG(ui4_style,WGL_STL_LB_ANIM_BY_ACT_MAP))
        {            
            WGL_SET_FLAG(pt_ctx->ui4_style,WGL_STL_LB_ANIM_BY_ACT_MAP);
        }
        
    }

    return i4_ret;
}

static INT32 _lb_destroy_anim(
    LB_CTX_T*                           pt_ctx)
{
    INT32                                   i4_ret = WGLR_OK;

    if(HAS_FLAG(pt_ctx->ui1_state, LBST_ANIM_CREATED))
    {
        i4_ret = wgl_anim_free(&pt_ctx->t_anim);
        if(i4_ret == WGLR_OK)
        {
            wgl_anim_free_map_tbl(pt_ctx->at_actmap,WGL_LB_ANIM_ACT_START,WGL_LB_ANIM_ACT_END);
            WGL_CLEAR_FLAG(pt_ctx->ui1_state, LBST_ANIM_CREATED);
            WGL_CLEAR_FLAG(pt_ctx->ui4_style,WGL_STL_LB_ANIM_BY_ACT_MAP);
        }
        else
        {
            i4_ret = WGLR_INV_ARG;
        }
    }

    return i4_ret; 
}

#endif

#if 0
static INT32 _lb_lbw_reload(
    LB_CTX_T*                           pt_ctx,
    WGL_LB_RLD_INF_T*                   pt_rld_inf)
{
    INT32                               i4_ret = WGLR_OK;
    BOOL                                b_pass = TRUE;
    UINT16                              ui2_total_num;
    UINT32                              ui4_rangle;
    UINT16                              ui2_base_num_eff;
    
    CHECK_VALID((pt_ctx != NULL), WGLR_INV_ARG);
    CHECK_VALID((pt_rld_inf != NULL), WGLR_INV_ARG);

    if(pt_rld_inf->ui2_total_elems > WGL_LB_MAX_ELEM_NUM)
    {
        return WGLR_INV_ARG;
    }
    
    ui2_total_num = pt_rld_inf->ui2_total_elems;
    DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_GET_VISIBLE_RANGE, &ui4_rangle, &ui2_base_num_eff, i4_ret);
    CATCH_FAIL(i4_ret);            
    
    switch (pt_rld_inf->e_rld_hlt)
    {
        case LBW_RLD_HLT_OPT_KEEP_IDX:
        break;
        
        case LBW_RLD_HLT_OPT_RESET_IDX:
            if (pt_rld_inf->ui2_hlt_elem >= ui2_total_num)
            {
                b_pass = FALSE;
            }
        break;
        
        case LBW_RLD_HLT_OPT_RESET_POS:
            if (pt_rld_inf->ui2_hlt_elem >= ui2_total_num || 
                pt_rld_inf->ui2_hlt_pos >= ui2_base_num_eff)
            {
                b_pass = FALSE;
            }
        break;
        
        default:
            b_pass = FALSE;
    }  

    if (!b_pass)
    {
        return WGLR_INV_ARG;
    }

        
    return i4_ret; 
}
#endif

static INT32 _lb_link_elem_inf(
    LB_CTX_T*                           pt_ctx,
    LB_ELEM_T*                          pt_elem,
    UINT16                              ui2_idx,
    BOOL                                b_init)
{
    LB_ELEM_INF_T*                      pt_elem_inf = NULL;

    CHECK_NULL(pt_ctx, WGLR_INV_ARG);
    CHECK_NULL(pt_elem, WGLR_INV_ARG);
    _LB_GET_ELEM_INF(pt_ctx, ui2_idx, &pt_elem_inf);

    if(pt_elem_inf == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }

    if(b_init == TRUE)
    {
        _LB_ELEM_INF_INIT(pt_elem_inf);
    }
    pt_elem->pt_elem_inf = pt_elem_inf;

    return WGLR_OK;
}

static INT32 _lb_chk_items_data(
    IN      LB_CTX_T*                   pt_ctx,
    IN      WGL_LB_ITEM_T               *at_items)
{
    UINT8                               ui1_c;

    CHECK_NULL(pt_ctx , WGLR_INV_ARG);
    CHECK_NULL(at_items, WGLR_INV_ARG);

    for(ui1_c=0; ui1_c<pt_ctx->ui1_col_num; ui1_c++ )
    {
        if(at_items[ui1_c].e_col_type != pt_ctx->at_cols[ui1_c].e_col_type)
        {
            return WGLR_INV_ARG;
        }
        
        switch(at_items[ui1_c].e_col_type)
        {
            case LB_COL_TYPE_IMG:
                if( at_items[ui1_c].data.h_img!=NULL_HANDLE &&
                    !_lb_is_valid_image_handle(at_items[ui1_c].data.h_img))
                {
                    return WGLR_INV_ARG;
                }
                break;
            case LB_COL_TYPE_IMG_SUIT:
                if( at_items[ui1_c].data.pt_img_suit!=NULL &&
                    !_lb_is_valid_image_suit(at_items[ui1_c].data.pt_img_suit))
                {
                    return WGLR_INV_ARG;
                }
                break;
            case LB_COL_TYPE_TEXT:
                /*TODO: check the validity of the text*/
                break;
            default:
                return WGLR_INV_ARG;
        }
    }

    return WGLR_OK;
}

static INT32 _lb_alloc_elem_data_by_addr(
    LB_COL_T*                            at_cols,
    UINT8                                ui1_col_num,
    UINT32                               ui4_cc_size_txt_pool,
    UINT8                                ui1_img_suit_col_num,
    LB_ELEM_T*                           pt_elem)
{
    UTF16_T                 *pw2s_next_txt;
    BYTE                    *pb_next_img_suit;
    UINT8                   ui1_c;

    CHECK_NULL(at_cols , WGLR_INV_ARG);
    CHECK_NULL(pt_elem, WGLR_INV_ARG);

    pt_elem->pt_elem_inf = NULL;
    pt_elem->pv_tag    = NULL;
    pt_elem->pt_prev = NULL;
    pt_elem->pt_next = NULL;
    pt_elem->at_items    = (LB_ITEM_T *)(((BYTE *)pt_elem) + WGL_ALIGN_4(sizeof(LB_ELEM_T)) );
    pw2s_next_txt        = (UTF16_T *) ( ((BYTE *)pt_elem->at_items) + 
                                WGL_ALIGN_4(sizeof(LB_ITEM_T) * ui1_col_num) );
    pb_next_img_suit     = ((BYTE *)pw2s_next_txt) + UTF16_MAX_LEN * ui4_cc_size_txt_pool;
    pt_elem->pv_cnt_elem = (VOID *) ( pb_next_img_suit + 
                              (WGL_ALIGN_4(sizeof(WGL_LB_ITEM_IMG_SUIT_T)) * ui1_img_suit_col_num) );
    for(ui1_c = 0; ui1_c < ui1_col_num; ui1_c++ )
    {
        if( at_cols[ui1_c].e_col_type==LB_COL_TYPE_TEXT )
        {
            pt_elem->at_items[ui1_c].t_text.pw2s_text = pw2s_next_txt;
            pt_elem->at_items[ui1_c].t_text.pw2s_raw_text = pw2s_next_txt+WGL_ALIGN_4(at_cols[ui1_c].ui2_max_text_len);
            pw2s_next_txt += WGL_ALIGN_4(at_cols[ui1_c].ui2_max_text_len)*2;
        }
        else if(at_cols[ui1_c].e_col_type==LB_COL_TYPE_IMG_SUIT)
        {
            pt_elem->at_items[ui1_c].pt_img_suit = (WGL_LB_ITEM_IMG_SUIT_T *)pb_next_img_suit;
            pb_next_img_suit += WGL_ALIGN_4(sizeof(WGL_LB_ITEM_IMG_SUIT_T));
        }
    }

    return WGLR_OK;
}

static INT32 _lb_alloc_elem_data(
    LB_COL_T*                            at_cols,
    UINT8                                ui1_col_num,
    UINT32                               ui4_cc_size_txt_pool,
    UINT8                                ui1_img_suit_col_num,
    UINT32                               ui4_cb_size_elem,
    LB_ELEM_T**                          ppt_elem)
{
    INT32                               i4_ret;
    LB_ELEM_T*                          pt_tmp;

    CHECK_NULL(at_cols, WGLR_INV_ARG);
    if(ppt_elem == NULL)
    {
        return WGLR_INV_ARG;
    }

    *ppt_elem = NULL;

    pt_tmp = WGL_MEM_ALLOC( ui4_cb_size_elem );
    if(pt_tmp == NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }

    /*init the element*/
    i4_ret = _lb_alloc_elem_data_by_addr(at_cols,
                                         ui1_col_num,
                                         ui4_cc_size_txt_pool,
                                         ui1_img_suit_col_num,
                                         pt_tmp);
    if(i4_ret != WGLR_OK)
    {
        WGL_MEM_FREE(pt_tmp);
        return i4_ret;
    }
    
    *ppt_elem = pt_tmp;

    return WGLR_OK;
}

static INT32 _lb_insert_elem_data(
    IN      LB_CTX_T*                   pt_ctx,
    IN      UINT16                      ui2_idx, 
    IN OUT  LB_ELEM_T*                  pt_elem,
    IN      BOOL                        b_is_append)
{
    INT32                   i4_ret = WGLR_OK;
    UINT16                  ui2_cache_s, ui2_cache_e;
    UINT16                  ui2_num_vp_elem;

    ui2_cache_s = _LB_GET_CACHE_START(pt_ctx);
    ui2_cache_e = ui2_cache_s + _LB_GET_ELEM_CACHE_NUM(pt_ctx); 

    if( (_LB_GET_ELEM_CACHE_NUM(pt_ctx) == 0) && (pt_ctx->pt_elems != NULL) )
    {
        return WGLR_INTERNAL_ERROR; 
    }

    DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_GET_VISIBLE_RANGE, NULL, &ui2_num_vp_elem, i4_ret);
    CATCH_FAIL(i4_ret);

    ui2_num_vp_elem ++;/*The last one element of the viewport may not complete,so the number should vp+1 */
    if( (_LB_GET_ELEM_CACHE_NUM(pt_ctx) <= ui2_num_vp_elem) || 
        (ui2_idx >= ui2_cache_s && ui2_idx < ui2_cache_e) ||
        (b_is_append && !HAS_STYLE(pt_ctx,WGL_STL_LB_LBW)) )
    {
        /*insert the element into the element-list*/
        if( pt_ctx->pt_elems == NULL )
        {
            pt_elem->pt_next     = NULL;
            pt_elem->pt_prev     = NULL;
            pt_ctx->pt_elems = pt_elem;
            _LB_SET_CACHE_START(pt_ctx, ui2_idx);
        }
        else if(b_is_append)
        {
            LB_ELEM_T   *pt_end;
            
            _lb_get_elem(pt_ctx, (UINT16)(ui2_cache_e - 1), &pt_end, NULL);
            if (!pt_end)
            {
                WS_ASSERT(0);
                return WGLR_INTERNAL_ERROR;
            }

            pt_elem->pt_next = NULL;
            pt_elem->pt_prev = pt_end;
            pt_end->pt_next  = pt_elem;
        }
        else if( ui2_idx == ui2_cache_s )
        {   /*insert before the first element*/
            pt_elem->pt_next              = pt_ctx->pt_elems;
            pt_elem->pt_prev              = NULL;
            pt_ctx->pt_elems->pt_prev = pt_elem;
            pt_ctx->pt_elems          = pt_elem;
        }
        else if( (ui2_idx +(UINT16)1) == ui2_cache_s && (_LB_GET_ELEM_CACHE_NUM(pt_ctx) <= ui2_num_vp_elem))
        {   /*insert before the first cache element*/
            pt_elem->pt_next              = pt_ctx->pt_elems;
            pt_elem->pt_prev              = NULL;
            pt_ctx->pt_elems->pt_prev = pt_elem;
            pt_ctx->pt_elems          = pt_elem;
            _LB_SET_CACHE_START(pt_ctx, ui2_idx);
        }
        else if( ui2_idx > ui2_cache_s || ui2_idx <= ui2_cache_e )
        {   /*insert before some element except the first one*/
            UINT16      ui2_prev;
            UINT16      ui2_i;
            LB_ELEM_T   *pt_prev = pt_ctx->pt_elems;
            
            ui2_prev = ui2_idx-(UINT16)1;
            for(ui2_i=ui2_cache_s ; ui2_i<ui2_prev && pt_prev; ui2_i++)
            {
                pt_prev = pt_prev->pt_next; 
            }

            if(pt_prev==NULL)
            {
                INFORM_ERR(WGLR_INTERNAL_ERROR);
                pt_prev = pt_ctx->pt_elems;
            }

            if(pt_prev->pt_next)
            {
                pt_prev->pt_next->pt_prev = pt_elem;
            }
            pt_elem->pt_next = pt_prev->pt_next;
            pt_elem->pt_prev = pt_prev;
            pt_prev->pt_next = pt_elem;
        }
        else 
        {
            i4_ret = WGLR_INTERNAL_ERROR ;
        }
    }
    _LB_INC_ELEM_CACHE_NUM(pt_ctx);

    return i4_ret;
}


static INT32 _lb_init_elem_data(
    IN      LB_CTX_T*                   pt_ctx,
    IN      WGL_LB_ITEM_T*              at_items, 
    IN OUT  LB_ELEM_T*                  pt_elem)
{
    INT32                   i4_ret = WGLR_OK;
    UINT8                   ui1_c;

    CHECK_NULL(pt_ctx , WGLR_INV_ARG);
    CHECK_NULL(pt_elem, WGLR_INV_ARG);

    if(at_items==NULL)
    {
        for( ui1_c=0; ui1_c<pt_ctx->ui1_col_num; ui1_c++ )
        {
            switch(pt_ctx->at_cols[ui1_c].e_col_type) 
            {
                case LB_COL_TYPE_IMG:
                    pt_elem->at_items[ui1_c].h_img = NULL_HANDLE;
                    break;
                case LB_COL_TYPE_IMG_SUIT:
                    _IMG_SUIT_COPY(pt_elem->at_items[ui1_c].pt_img_suit, &NULL_LB_IMG_SUIT);
                    break;
                case LB_COL_TYPE_TEXT:
                {
                    x_uc_w2s_set(pt_elem->at_items[ui1_c].t_text.pw2s_text, 0, WGL_CHAR_NULL );
                    x_uc_w2s_set(pt_elem->at_items[ui1_c].t_text.pw2s_raw_text, 0, WGL_CHAR_NULL );
                }
                    break;
                default:
                    WS_ASSERT(0);
                    return WGLR_INTERNAL_ERROR;
            }
        }
    }
    else 
    {
        for( ui1_c=0; ui1_c<pt_ctx->ui1_col_num; ui1_c++ )
        {
            switch(pt_ctx->at_cols[ui1_c].e_col_type) 
            {
                case LB_COL_TYPE_IMG:
                    pt_elem->at_items[ui1_c].h_img = at_items[ui1_c].data.h_img;
                    break;
                case LB_COL_TYPE_IMG_SUIT:
                    if(at_items[ui1_c].data.pt_img_suit==NULL)
                    {
                        _IMG_SUIT_COPY(pt_elem->at_items[ui1_c].pt_img_suit, &NULL_LB_IMG_SUIT);
                    }
                    else
                    {
                        _IMG_SUIT_COPY(pt_elem->at_items[ui1_c].pt_img_suit, at_items[ui1_c].data.pt_img_suit);
                    }
                    break;
                case LB_COL_TYPE_TEXT:
					{
					 UINT32     ui4_text_dir;

#ifdef WGL_LB_NO_STORE_TEXT
                     if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
                     {
                        break;
                     }
#endif                     
  					 CATCH_FAIL(pt_ctx->t_intf_cnt.pf_content_cmd_proc( pt_ctx->pv_ctx_cnt, WGL_CNT_LB_GET_TEXT_DIRECTION, WGL_PACK(&ui4_text_dir), NULL));  	                 
					 if (check_exist_arabic(at_items[ui1_c].data.pw2s_text,pt_ctx->at_cols[ui1_c].ui2_max_text_len))
  	                 {  
  	                     UTF16_T*  w2s_buffer;
                         VOID* pv_reorder_attr=NULL;
                         
				    	 CATCH_FAIL(pt_ctx->t_intf_cnt.pf_content_cmd_proc( pt_ctx->pv_ctx_cnt, WGL_CNT_LB_GET_BUFFER, WGL_PACK(&w2s_buffer), NULL));
                   		 x_memset(w2s_buffer,WGL_CHAR_NULL,(SIZE_T)(UTF16_MAX_LEN * (pt_ctx->at_cols[ui1_c].ui2_max_text_len + 1)));                
                         if(pt_ctx->t_arab_reorder_attr.b_find_init_level == FALSE)
                         {
                             pv_reorder_attr = &(pt_ctx->t_arab_reorder_attr);
                         }

                         i4_ret = reorder_arabic( at_items[ui1_c].data.pw2s_text,pt_ctx->at_cols[ui1_c].ui2_max_text_len,pv_reorder_attr,w2s_buffer,NULL);
						 if (i4_ret != WGLR_OK)
					 	 {
							return i4_ret;
					 	 }
						  pt_elem->at_items[ui1_c].t_text.pw2s_text = x_uc_w2s_strcpy(pt_elem->at_items[ui1_c].t_text.pw2s_text,w2s_buffer);                  						 							 
				 	 }
					 else
					 {
                        _lb_tool_safe_w2s_strcpy(pt_ctx, pt_elem->at_items[ui1_c].t_text.pw2s_text, at_items[ui1_c].data.pw2s_text, (UINT32)pt_ctx->at_cols[ui1_c].ui2_max_text_len);
					 }
                     
                     _lb_tool_safe_w2s_strcpy(pt_ctx, pt_elem->at_items[ui1_c].t_text.pw2s_raw_text, at_items[ui1_c].data.pw2s_text, (UINT32)pt_ctx->at_cols[ui1_c].ui2_max_text_len);
                	}
                    break;
                default:
                    WS_ASSERT(0);
                    return WGLR_INTERNAL_ERROR;
            }
        }
        /*reset reorder attr after used for current string content*/
        pt_ctx->t_arab_reorder_attr.b_find_init_level = TRUE;
        pt_ctx->t_arab_reorder_attr.ui1_embedding_level = 0;
    }

    return WGLR_OK;
}

static INT32 _lb_create_elem_data(
    LB_CTX_T*                          pt_ctx,
    WGL_LB_ITEM_T*                     at_items,
    UINT16                             ui2_idx,
    LB_ELEM_T**                        ppt_elemt,
    BOOL                               b_append)
{
    INT32                   i4_ret = WGLR_OK;
    LB_ELEM_T               *pt_elem = NULL;
    BOOL                    b_alloc = FALSE;

    CHECK_NULL(pt_ctx , WGLR_INV_ARG);

    /*Alloc the element data memory*/
    if(pt_ctx->pt_elems_free_list==NULL)
    {
        i4_ret = _lb_alloc_elem_data(pt_ctx->at_cols,
                                     pt_ctx->ui1_col_num,
                                     pt_ctx->ui4_cc_size_txt_pool,
                                     pt_ctx->ui1_img_suit_col_num,
                                     pt_ctx->ui4_cb_size_elem,
                                     &pt_elem);
        if(i4_ret != WGLR_OK)
        {
            return i4_ret;
        }

        b_alloc = TRUE;
    }
    else
    {   /*retrieve a free element from the free-element-list*/
        pt_elem = pt_ctx->pt_elems_free_list;
        if(pt_elem->pt_next)
        {
            pt_elem->pt_next->pt_prev = NULL;
        }
        pt_ctx->pt_elems_free_list = pt_elem->pt_next;
    }

    if( pt_elem == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }

    i4_ret = _lb_init_elem_data(pt_ctx, at_items, pt_elem);
    if(i4_ret != WGLR_OK)
    {
        if(b_alloc == TRUE )
        {
            WGL_MEM_FREE(pt_elem);
        }
        return i4_ret ; 
    }

    pt_elem->pv_tag    = NULL;

    i4_ret =   _lb_insert_elem_data(pt_ctx, ui2_idx, pt_elem, b_append);
    if(i4_ret != WGLR_OK)
    {
        if(b_alloc == TRUE )
        {
            WGL_MEM_FREE(pt_elem);
        }
        return i4_ret ; 
    }

    i4_ret = _lb_link_elem_inf(pt_ctx, pt_elem, ui2_idx, FALSE);
    if(i4_ret != WGLR_OK)
    {
        if( b_alloc == TRUE )
        {
            WGL_MEM_FREE(pt_elem);
        }
        return i4_ret ; 
    }


    if(ppt_elemt != NULL)
    {
        *ppt_elemt = pt_elem;
    }

#ifdef CLI_LVL_ALL     
    if( b_alloc == TRUE )
    {
        pt_ctx->ui4_mem_size += pt_ctx->ui4_cb_size_elem;
    }
#endif  
    return WGLR_OK;
}

static INT32 _lb_destroy_elem_data(
    IN      LB_CTX_T*                   pt_ctx,
    IN      UINT16                      ui2_idx,
    IN      LB_ELEM_T**                 ppt_elemt)
{
    INT32                   i4_ret = WGLR_OK;
    UINT16                  ui2_cache_s, ui2_cache_e;
    LB_ELEM_T*              pt_elem = NULL;

    ui2_cache_s = _LB_GET_CACHE_START(pt_ctx);
    if( _LB_GET_ELEMS_NUM(pt_ctx) == 0)
    {
        ui2_cache_e = ui2_cache_s;
    }
    else
    {
        ui2_cache_e = ui2_cache_s + _LB_GET_ELEM_CACHE_NUM(pt_ctx) - (UINT16)1;
    }

    /*If the cache number is 0 or out of the cache range, not do the delete action*/
    if((_LB_GET_ELEM_CACHE_NUM(pt_ctx) == 0) || ui2_idx < ui2_cache_s || ui2_idx > ui2_cache_e)
    {
        return WGLR_OK;
    }

    if( ui2_idx==ui2_cache_s)
    {
        pt_elem              = pt_ctx->pt_elems;
        pt_ctx->pt_elems = pt_ctx->pt_elems->pt_next;
        if( pt_ctx->pt_elems )
        {
            pt_ctx->pt_elems->pt_prev = NULL;
        }
        /*the cache start is wrong when it delete the first cache element*/
        _LB_INC_CACHE_START(pt_ctx); 
    }
    else
    {
        UINT16      ui2_i;
        LB_ELEM_T   *pt_prev = pt_ctx->pt_elems;

        for( ui2_i=ui2_cache_s; (ui2_i+(UINT16)1)<ui2_idx; ui2_i++ )
        {
            pt_prev = pt_prev->pt_next;
        }
        
        if(!(pt_prev!=NULL && pt_prev->pt_next!=NULL))
        {
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            pt_prev = pt_ctx->pt_elems;
        }
        pt_elem          = pt_prev->pt_next;
        pt_prev->pt_next = pt_elem->pt_next;
        if(pt_elem->pt_next)
        {
            pt_elem->pt_next->pt_prev = pt_prev;
        }
    }

     _LB_DEC_ELEM_CACHE_NUM(pt_ctx);
    if(pt_elem && pt_elem->pt_elem_inf)
    {
        WGL_CLEAR_FLAG(pt_elem->pt_elem_inf->ui1_state, LBEST_CHACHED); 
        pt_elem->pt_elem_inf = NULL;
        _LB_ELEM_FREE_LIST_INS(pt_ctx->pt_elems_free_list, pt_elem);
        if(ppt_elemt != NULL)
        {
            *ppt_elemt = pt_elem;
        }
    }

    return WGLR_OK;
}

/*ensure the element in the viewport has been allocate element data, it only for
 * the LBW mode */
static INT32 _lb_check_cache_for_vp(LB_CTX_T* pt_ctx)
{
    UINT32                  ui4_range;
    UINT16                  ui2_vp_first, ui2_vp_last ;
    UINT16                  ui2_cache_s, ui2_cache_e;
    LB_RANGE_INF_T          at_free_range[LB_MAX_UPD_RANGE];
    LB_RANGE_INF_T          at_alloc_range[LB_MAX_UPD_RANGE];
    UINT16                  ui2_free_cnt = 0;
    UINT16                  ui2_alloc_cnt = 0;
    UINT16                  ui2_i, ui2_idx;
    INT32                   i4_ret;
    LB_ELEM_INF_CONTENT     t_content;
    LB_ELEM_T*              pt_elem = NULL;


    if(!HAS_STYLE(pt_ctx,WGL_STL_LB_LBW))
    {
        return WGLR_OK; 
    }

    /*Get the viewport information*/
    DO_LB_CNT_CMD(pt_ctx, WGL_CMD_LB_GET_RANGE, &ui4_range, NULL, i4_ret);
    CATCH_FAIL(i4_ret);

    ui2_vp_first = WGL_GET_HI_WORD(ui4_range);
    ui2_vp_last  = WGL_GET_LOW_WORD(ui4_range);

    ui2_cache_s = _LB_GET_CACHE_START(pt_ctx);
    if( _LB_GET_ELEM_CACHE_NUM(pt_ctx) == 0)
    {
        ui2_cache_e = ui2_cache_s;
    }
    else
    {
        ui2_cache_e = ui2_cache_s + _LB_GET_ELEM_CACHE_NUM(pt_ctx) - (UINT16)1;
    }

    x_memset(at_free_range, 0, sizeof(LB_RANGE_INF_T)*LB_MAX_UPD_RANGE);
    _lb_get_update_range(ui2_cache_s, ui2_cache_e, ui2_vp_first, ui2_vp_last, FALSE, at_free_range, &ui2_free_cnt);

    x_memset(at_alloc_range, 0, sizeof(LB_RANGE_INF_T)*LB_MAX_UPD_RANGE);
    _lb_get_update_range(ui2_cache_s, ui2_cache_e, ui2_vp_first, ui2_vp_last, TRUE, at_alloc_range, &ui2_alloc_cnt);

    /*Get the element data that is not in now vp and free the memory*/
    for(ui2_i = 0; ui2_i < ui2_free_cnt; ui2_i ++)
    {
        if(ui2_i >= (UINT16)LB_MAX_UPD_RANGE)
        {
            break;
        }

        for(ui2_idx =at_free_range[ui2_i].ui2_start; ui2_idx <= at_free_range[ui2_i].ui2_end; ui2_idx ++)
        {
            pt_elem = NULL;
            i4_ret = _lb_destroy_elem_data(pt_ctx, ui2_idx, &pt_elem);
            if(i4_ret != WGLR_OK)
            {
                return WGLR_INTERNAL_ERROR;
            }
            t_content.pt_elem_inf_list = &(pt_ctx->t_elem_inf_list);
            t_content.pt_elems = pt_ctx->pt_elems;
            t_content.ui2_elem_num = _LB_GET_ELEMS_NUM(pt_ctx);
            t_content.ui2_cache_start = _LB_GET_CACHE_START(pt_ctx);
            t_content.ui2_cache_num = _LB_GET_ELEM_CACHE_NUM(pt_ctx);
            DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_FREE_ELEM, &t_content, pt_elem, i4_ret);
            if(i4_ret != WGLR_OK)
            {
                return WGLR_INTERNAL_ERROR;
            }
        } 
    }

    /*Check the elements that in viewport is allocate memory or not, if
     * not,allocate the memeory*/
    ui2_cache_s = _LB_GET_CACHE_START(pt_ctx);
    if( _LB_GET_ELEM_CACHE_NUM(pt_ctx) == 0)
    {
        ui2_cache_e = ui2_cache_s;
    }
    else
    {
        ui2_cache_e = ui2_cache_s + _LB_GET_ELEM_CACHE_NUM(pt_ctx) - (UINT16)1;
    }

    for(ui2_i = 0; ui2_i < ui2_alloc_cnt; ui2_i ++)
    {
        if(ui2_i >= (UINT16)LB_MAX_UPD_RANGE)
        {
            break;
        }

        for(ui2_idx = at_alloc_range[ui2_i].ui2_start; ui2_idx <= at_alloc_range[ui2_i].ui2_end; ui2_idx++)
        {
            pt_elem = NULL;
            i4_ret = _lb_create_elem_data(pt_ctx, NULL, ui2_idx, &pt_elem, FALSE);
            if(i4_ret != WGLR_OK)
            {
                return WGLR_INTERNAL_ERROR;
            }

            t_content.pt_elem_inf_list = &(pt_ctx->t_elem_inf_list);
            t_content.pt_elems = pt_ctx->pt_elems;
            t_content.ui2_elem_num = _LB_GET_ELEMS_NUM(pt_ctx);
            t_content.ui2_cache_start = _LB_GET_CACHE_START(pt_ctx);
            t_content.ui2_cache_num = _LB_GET_ELEM_CACHE_NUM(pt_ctx);
            DO_LB_CNT_CMD(pt_ctx, WGL_CNT_LB_CREATE_ELEM, &t_content, pt_elem, i4_ret);
            if(i4_ret != WGLR_OK)
            {
                return WGLR_INTERNAL_ERROR;
            }
        }
    } 

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_check_cache_inf
 *
 * Description: 1.Check the elements in the viewport are allocate data memory.
 *              2.To get the elements data information when that does not
 *              cached.
 *              This function is only for LBW style.
 * Inputs:  pt_ctx         Contains the instance context.
 *         
 *
 * Outputs: Null.
 *
 * Returns: WGLR_OK           Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lb_check_cache_inf(LB_CTX_T* pt_ctx)
{
    INT32                   i4_ret = WGLR_OK;
    UINT32                  ui4_range;
    UINT16                  ui2_vp_first, ui2_vp_last ;
    UINT16                  ui2_i;
    LB_ELEM_T*              pt_elem;
    LB_ELEM_INF_T*          pt_elem_inf;               
    WGL_LB_ELEM_DATA_INFO_T     t_elem_data;
    WGL_PACKED_PARAM_T      t_param;
    WGL_LB_NFY_TYPE_T      e_type;
    WGL_LB_COL_INIT_T*      pt_cols_fmt = NULL;

    if(!HAS_STYLE(pt_ctx,WGL_STL_LB_LBW))
    {
        return WGLR_OK; 
    }

    _lb_check_cache_for_vp(pt_ctx);

    /*Get the viewport information*/
    DO_LB_CNT_CMD(pt_ctx, WGL_CMD_LB_GET_RANGE, &ui4_range, NULL, i4_ret);
    CATCH_FAIL(i4_ret);

    ui2_vp_first = WGL_GET_HI_WORD(ui4_range);
    ui2_vp_last  = WGL_GET_LOW_WORD(ui4_range);

    x_memset(&t_elem_data, 0, sizeof(WGL_LB_ELEM_DATA_INFO_T));

    t_elem_data.at_cols_fmt = WGL_MEM_ALLOC(sizeof(WGL_LB_COL_INIT_T)*pt_ctx->ui1_col_num);
    if(t_elem_data.at_cols_fmt == NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }

    x_memset(t_elem_data.at_cols_fmt, 0, sizeof(WGL_LB_COL_INIT_T)*pt_ctx->ui1_col_num);
    pt_cols_fmt = t_elem_data.at_cols_fmt;
    for( ui2_i = 0; ui2_i < pt_ctx->ui1_col_num; ui2_i ++)
    {
        pt_cols_fmt->e_col_type = pt_ctx->at_cols[ui2_i].e_col_type;
        pt_cols_fmt->ui2_max_text_len = pt_ctx->at_cols[ui2_i].ui2_max_text_len;
        pt_cols_fmt ++;
    }

    t_elem_data.ui1_col_num = pt_ctx->ui1_col_num;
    t_elem_data.ui2_elem_num = _LB_GET_ELEMS_NUM(pt_ctx);
    t_param.pv_param1 = NULL;
    t_param.pv_param2 = (VOID *)&t_elem_data;
    e_type = WGL_LB_NFY_REQ_ELEM_DATA;

    for( ui2_i = ui2_vp_first; ui2_i <= ui2_vp_last ;ui2_i ++)
    {
        _lb_get_elem(pt_ctx, ui2_i, &pt_elem, &pt_elem_inf);

        if(pt_elem && pt_elem_inf && !HAS_FLAG(pt_elem_inf->ui1_state, LBEST_CHACHED))
        { /*If the element not cached, to inform app to get data*/
            t_elem_data.ui2_elem_start = ui2_i;
            t_elem_data.ui2_elem_end = ui2_i;
            DO_LB_CNT_CMD(pt_ctx, WGL_CMD_LB_DO_NTF, &e_type, &t_param, i4_ret);
            if(i4_ret == WGLR_OK)
            {
                WGL_SET_FLAG(pt_elem_inf->ui1_state, LBEST_CHACHED);
            }
        }
    } 

    WGL_MEM_FREE(t_elem_data.at_cols_fmt);
    return WGLR_OK;
}

static VOID _lb_get_update_range(
        UINT16                       ui2_src_start,
        UINT16                       ui2_src_end,
        UINT16                       ui2_dst_start,
        UINT16                       ui2_dst_end,
        BOOL                         b_include,
        LB_RANGE_INF_T*              pt_upd_range,
        UINT16*                      pui2_count)
{
    UINT16                           ui2_start, ui2_end;
    UINT16                           ui2_cnt = 0;

    if( (pt_upd_range == NULL) || (pui2_count == NULL) )
    {
        return ;
    }

    *pui2_count = 0;

    ___BLOCK_BEGIN___
        if(b_include == TRUE)
        {
            if( ui2_dst_start == ui2_dst_end )
            {
                if(ui2_dst_start < ui2_src_start || ui2_dst_start > ui2_src_end)
                {
                    pt_upd_range[ui2_cnt].ui2_start = ui2_dst_start ;
                    pt_upd_range[ui2_cnt].ui2_end = ui2_dst_start;
                    ui2_cnt ++;
                }
                GOTO_EOB;
            }

            ui2_start = MIN(ui2_src_start,ui2_dst_start); 
            ui2_end = MIN(ui2_src_start, ui2_dst_end);
            if(ui2_start < ui2_end)
            {
                if(ui2_end >= ui2_src_start)
                {
                    ui2_end --;
                }
                pt_upd_range[ui2_cnt].ui2_start = ui2_start;
                pt_upd_range[ui2_cnt].ui2_end = ui2_end;
                ui2_cnt ++;
            }

            ui2_start = MAX(ui2_src_end, ui2_dst_start); 
            ui2_end = MAX(ui2_src_end, ui2_dst_end);
            if( ui2_start < ui2_end )
            {
                if(ui2_start <= ui2_src_end)
                {
                    ui2_start ++;
                }
                pt_upd_range[ui2_cnt].ui2_start = ui2_start;
                pt_upd_range[ui2_cnt].ui2_end = ui2_end;
                ui2_cnt ++;
            }
        }
        else
        {
            if( ui2_src_start == ui2_src_end )
            {
                if(ui2_src_start < ui2_dst_start || ui2_src_start > ui2_dst_end)
                {
                    pt_upd_range[ui2_cnt].ui2_start = ui2_src_start ;
                    pt_upd_range[ui2_cnt].ui2_end = ui2_src_start;
                    ui2_cnt ++;
                }
                GOTO_EOB;
            }

            ui2_start = MIN(ui2_src_start,ui2_dst_start); 
            ui2_end = MIN(ui2_src_end, ui2_dst_start);
            if(ui2_start < ui2_end)
            {
                if(ui2_end >= ui2_dst_start)
                {
                    ui2_end --;
                }
                pt_upd_range[ui2_cnt].ui2_start = ui2_start;
                pt_upd_range[ui2_cnt].ui2_end = ui2_end;
                ui2_cnt ++;
            }

            ui2_start = MAX(ui2_src_start, ui2_dst_end); 
            ui2_end = MAX(ui2_src_end, ui2_dst_end);
            if(ui2_start < ui2_end)
            {
                if(ui2_start <= ui2_dst_end)
                {
                    ui2_start ++;
                }
                pt_upd_range[ui2_cnt].ui2_start = ui2_start;
                pt_upd_range[ui2_cnt].ui2_end = ui2_end;
                ui2_cnt ++;
            }
        }
 
    ___BLOCK_END___

    *pui2_count = ui2_cnt;
}


static INT32 _lb_init_col_inf(
    LB_COL_T*                            pt_cols,
    UINT8                                ui1_col_num,
    WGL_LB_COL_INIT_T*                   pt_init_cols)
{
    UINT8                                ui1_c;

    CHECK_VALID(pt_cols && pt_init_cols && (ui1_col_num > 0), WGLR_INV_ARG);

    for(ui1_c=0; ui1_c<ui1_col_num; ui1_c++)
    {
        pt_cols[ui1_c].e_col_type = pt_init_cols[ui1_c].e_col_type;
        
        switch(pt_init_cols[ui1_c].e_col_type) 
        {
            case LB_COL_TYPE_TEXT:
                pt_cols[ui1_c].ui2_max_text_len = pt_init_cols[ui1_c].ui2_max_text_len+(UINT16)1;
                break;
            case LB_COL_TYPE_IMG:
                pt_cols[ui1_c].ui2_max_text_len = (UINT16)0;
                break;
            case LB_COL_TYPE_IMG_SUIT:
                pt_cols[ui1_c].ui2_max_text_len = (UINT16)0;
                break;
            default:
                return WGLR_INTERNAL_ERROR; 
        }

    }
    return WGLR_OK; 
}


