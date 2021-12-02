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
 * $RCSfile: wgl_animation.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 41da94d098a4312ac008252fdf5e6559 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#ifdef WGL_ANIM_SUPPORT 
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "dbg/def_dbg_level_mw.h"
#include "wgl/wgl_sets.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/_wgl.h"
#include "wgl/wgl.h"
#include "wgl/u_wgl.h"
#include "wgl/u_wgl_bdr_common.h"
#include "wgl/wgl_bdr_internal.h"
#include "wgl/animation/x_wgl_animation.h"
#include "wgl/animation/wgl_animation.h"
#include "wgl/animation/wgl_animation_cli.h"
#include "wgl/wgl_anim.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_anim_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

#define MAX_UINT32           ((UINT32) 0xFFFFFFFF)

#define CATCH_FAIL(_catch_expr) if(_catch_expr != WGLR_OK){DBG_ERROR(("<POD> ERR: file = %s, line = %d, reason = %d\n\r",__FILE__,__LINE__,    _catch_expr));    return _catch_expr;}

#define _GET_CUST_INTERVAL(_t_cust_info) (_t_cust_info & 0xFFFF)
#define _GET_CUST_IMG_ID(_t_cust_info) ((_t_cust_info >> 16) & 0x7FFF)
#define _GET_CUST_FLAG(_t_cust_info) (_t_cust_info & 0x80000000)


/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/

 
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _set_img_anim_max_step(
   WGL_ANIM_CONTROL_LOGIC_T*  pt_anim );

static UINT32 _wgl_attr_get_anim_ticks(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT16                          ui2_idx);

static INT32 _wgl_attr_get_total_ticks(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim);

static INT32 _wgl_attr_init_begin_data(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim, 
    WGL_ANIMATION_DATA_T*           pt_anim_data);

static INT32 _wgl_attr_begin_anim(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim, 
    VOID*                           pv_param1,
    UINT32                          ui4_tick);

static INT32 _wgl_anim_attr_ntf_cond(
    WGL_ANIM_CONTROL_LOGIC_T        *pt_anim,
    UINT32                          ui4_tick_seq,
    WGL_ANIM_COND_INF_T*            pt_cond);

static UINT16 _wgl_anim_chk_new_begin(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT32                          ui4_tick_index);

static INT32 _wgl_anim_attr_do_chg_move(
    WGL_ANIM_CONTROL_LOGIC_T        *pt_anim,
    UINT32                          ui4_tick_seq,
    UINT32*                         pui4_count,
    OUT  GL_POINT_T*                pt_position);

static INT32 _wgl_anim_attr_do_chg_size(
    WGL_ANIM_CONTROL_LOGIC_T        *pt_anim,
    UINT32                          ui4_tick_seq,
    UINT32*                         pui4_count,
    OUT  GL_SIZE_T*                 pt_new_size);

static INT32 _wgl_anim_attr_do_chg_scale(
    WGL_ANIM_CONTROL_LOGIC_T        *pt_anim,
    UINT32                          ui4_tick_seq,
    UINT32*                         pui4_count,
    OUT  GL_SIZE_T*                 pt_new_size);

static INT32 _wgl_anim_attr_do_chg_alpha(
    WGL_ANIM_CONTROL_LOGIC_T        *pt_anim,
    UINT32                          ui4_tick_seq,
    UINT32*                         pui4_count,
    OUT  UINT8*                     pui1_alpha);

static INT32 _wgl_anim_attr_do_chg_img(
    WGL_ANIM_CONTROL_LOGIC_T        *pt_anim,
    UINT32                          ui4_tick_seq,
    UINT32*                         pui4_count,
    OUT  WGL_ANIM_IMG_INF_T*        pt_img);

static UINT16 _wgl_anim_attr_get_anim_type(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT16                          ui2_idx,
    WGL_ANIM_BEGIN_DATA_T**         pt_bgn);

static INT32 _wgl_anim_attr_add_anim_bgn(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    WGL_ANIM_BEGIN_DATA_T*          pt_data,
    UINT16                          ui2_bgn_idx);

static INT32 _wgl_anim_attr_hdlr_tick(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT32                          ui4_tick_index);

static INT32 _wgl_anim_attr_set_img_seq(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT8                           ui1_index,
    const WGL_ANIM_IMG_SEQ_T*       pt_img_set);

static INT32 _wgl_anim_attr_set_max_img(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT16                          ui2_index);

static INT32 _wgl_attr_free_img_cust_data(WGL_ANIM_CONTROL_LOGIC_T* pt_anim);

static INT32 _set_attr_img_begin_cust_data(
      WGL_ANIM_CONTROL_LOGIC_T*  pt_anim,
      WGL_IMG_ANIM_T*            pt_img_anim_dst,
      WGL_IMG_ANIM_DATA_T*       pt_img_anim_src);

static INT32 _wgl_attr_abort_anim(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    VOID*                           pv_param1);

static INT32 _wgl_attr_disrupt_anim(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT16                          ui2_type);

static INT32 _wgl_attr_end_anim(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim);

static INT32 _wgl_attr_get_tick_data(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    WGL_ANIM_TICK_DATA*             pt_tick_data);

static INT32 _wgl_attr_get_upd_rgn(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    GL_RECT_T*                      pt_prv_rect,
    WGL_UPD_REGION_T*               pt_upd);

static INT32 _wgl_attr_get_current_rgn(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    GL_RECT_T*                      pt_rect);

static INT32 _wgl_attr_get_anim_state(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT16*                         pui2_state);

static INT32 _wgl_attr_get_anim_info(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    WGL_ANIM_INF_T*                 pt_info);

INT32 wgl_anim_cmd_proc(
    WGL_WIDGET_ANIM_CONTEXT_T   t_anim,
    UINT32                      ui4_cmd_id,
    VOID*                       pv_param1,
    VOID*                       pv_param2);

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*******************************************************************************
 * wgl_anim_pack_img_cust_info
 *      
 * pack custom image information for saving
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/

INT32 wgl_anim_pack_img_cust_info(
    IN ANIM_IMG_CUST_INFO_T*       pt_cust_info,
    IN BOOL                        b_flag,
    IN UINT16                      ui2_img_id,
    IN UINT16                      ui2_interval)
{
    if (NULL == pt_cust_info)
    {
        return WGLR_INV_ARG;
    }
    *pt_cust_info = ((b_flag & 0x1)<<31);
    *pt_cust_info =  *pt_cust_info | ((ui2_img_id & 0x7FFF) << 16);
    *pt_cust_info =  *pt_cust_info | ui2_interval;
    return WGLR_OK;
}


static INT32 _set_img_anim_max_step(
   WGL_ANIM_CONTROL_LOGIC_T*  pt_anim )
{
    WGL_IMG_ANIM_T            t_img_anim;
    WGL_ANIM_IMG_SEQ_T*       pt_img_seq;
    ANIM_IMG_CUST_INFO_T      t_cust_info;  

    t_img_anim = pt_anim->t_data.t_init.t_img;
    pt_img_seq = &(pt_anim->pt_img_seq[t_img_anim.ui1_img_seq_id]);
   
    if(t_img_anim.e_play_mode == ANIM_PLAY_MODE_LOOP)
    {
        if(t_img_anim.ui2_loop_count == WGL_ANIM_INFINITE_VALUE)
        {
            pt_anim->t_data.ui4_max_step = WGL_ANIM_INFINITE_VALUE;
        }
        else
        {
        pt_anim->t_data.ui4_max_step = pt_img_seq->ui2_len * t_img_anim.ui2_loop_count;
    }
    }
    else if(t_img_anim.e_play_mode == ANIM_PLAY_MODE_ONCE)
    {
        pt_anim->t_data.ui4_max_step = pt_img_seq->ui2_len;
    }
    else /*ANIM_PLAY_MODE_CUST*/
    {
        /*check last image custom flag*/
        t_cust_info = *(pt_anim->t_data.t_init.t_img.pt_cust_info + pt_img_seq->ui2_len -1);
        if(_GET_CUST_FLAG(t_cust_info) != 0 )
        {
           pt_anim->t_data.ui4_max_step = 0; 
        }
        else
        {
           pt_anim->t_data.ui4_max_step = t_img_anim.ui2_cust_len;
        }
    }
    return WGLR_OK;

}

/******************************************************************************
* Animation as widget attribute function                                                                                                                              
*******************************************************************************/

INT32 wgl_anim_attr_init(VOID)
{
    INT32                  i4_ret;
    
    i4_ret = wgl_anim_cli_init();
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(ANIM)} ERR: Fail to init ANIM CLI. (%d) at L%d\n\n", i4_ret, __LINE__));
    }
    return i4_ret;
}

INT32 wgl_anim_create(
    WGL_WIDGET_ANIM_CONTEXT_T*       pt_anim)
{
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim_ctr;
    UINT32                          ui4_size;    

    ui4_size = sizeof(WGL_ANIM_CONTROL_LOGIC_T);
    pt_anim_ctr = (WGL_ANIM_CONTROL_LOGIC_T*) WGL_MEM_ALLOC(ui4_size);
    if(NULL == pt_anim_ctr)
    {
        DBG_ERROR(("{WGL(ANIM)} ERR: Fail to alloc memory for control logic. (%d) at L%d\n\n", WGLR_OUT_OF_MEMORY, __LINE__));
        return WGLR_OUT_OF_MEMORY;
    }
    
    
    *pt_anim = (WGL_WIDGET_ANIM_CONTEXT_T)pt_anim_ctr;
    x_memset(pt_anim_ctr, 0, ui4_size);
    pt_anim_ctr->ui4_tick_interval = x_wgl_get_tick_interval();
    pt_anim_ctr->ui2_img_seq_max_len = 0;
    pt_anim_ctr->pt_img_seq = NULL;
    pt_anim_ctr->ui4_start_tick = 0;
    pt_anim_ctr->t_current_data.t_new_tick.t_tick_inf.t_scale.ui4_width= WGL_ANIM_INIT_WIDTH;
    pt_anim_ctr->t_current_data.t_new_tick.t_tick_inf.t_scale.ui4_height = WGL_ANIM_INIT_HEIGHT;

    pt_anim_ctr->t_info.e_state = WGL_ANIM_STATE_NONE;
    WGL_SET_FLAG(pt_anim_ctr->t_info.ui2_done_type,WGL_ANIM_TYPE_NONE);
#ifdef CLI_LVL_ALL    
    pt_anim_ctr->ui4_mem_size = ui4_size;
#endif

    return WGLR_OK;
}

INT32 wgl_anim_free(WGL_WIDGET_ANIM_CONTEXT_T* pt_anim)
{
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim_ctr;

    pt_anim_ctr = (WGL_ANIM_CONTROL_LOGIC_T*)(*pt_anim);
    if(pt_anim_ctr != NULL)
    {
        if(pt_anim_ctr->pt_img_seq != NULL)
        {
            WGL_MEM_FREE(pt_anim_ctr->pt_img_seq);
        }
        WGL_MEM_FREE(pt_anim_ctr);
    }

    *pt_anim = NULL;
    return WGLR_OK;
}

INT32 wgl_anim_data_free(
    WGL_ANIMATION_DATA_T*               pt_begin)
{
    if(pt_begin == NULL)
    {
        return WGLR_INV_ARG;    
    }

    if(pt_begin->pt_anim_data != NULL)
    {
        WGL_MEM_FREE(pt_begin->pt_anim_data);
        pt_begin->pt_anim_data = NULL;
    }

    if(pt_begin->pt_path_mode != NULL)
    {
        WGL_MEM_FREE(pt_begin->pt_path_mode);
        pt_begin->pt_path_mode = NULL;
    }  

    x_memset(&pt_begin->t_path_id_inf,0,sizeof(WGL_ANIM_PATH_ID_INFO_T));

    pt_begin->ui4_flag = 0;
    pt_begin->ui2_count = 0;
    
    return WGLR_OK;    
}

INT32 wgl_anim_get_data_by_handle(
    HANDLE_T                        h_anim,
    WGL_ANIM_CONTROL_LOGIC_T**      ppt_anim)
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_type;

    if(ppt_anim == NULL)
    {
        return WGLR_INV_ARG;
    }
    
    i4_ret    = handle_get_type_obj(h_anim, &e_type, (VOID **)ppt_anim);
    if((i4_ret != HR_OK) || (e_type != HT_WGL_ANIM_HANDLE) || (*ppt_anim == NULL))
    {
        return WGLR_INV_ARG;
    }
    
    return WGLR_OK;
}

static UINT32 _wgl_attr_get_anim_ticks(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT16                          ui2_idx)
{
    UINT32                          ui4_end_tick = 0;
    UINT64                          ui8_time = 0;
    UINT32                          ui4_result;
    UINT16                          ui2_i;
    WGL_ANIM_BEGIN_DATA_T*          pt_begin;
    WGL_IMG_ANIM_DATA_T*            pt_img;
    ANIM_IMG_CUST_INFO_T            t_cust_info;    

    WGL_CHECK_NULL(pt_anim,0);

    pt_begin = pt_anim->pt_begin + ui2_idx;
    if(pt_begin == NULL)
    {
        return 0;
    }

    ui8_time = pt_begin->t_path_mode.ui2_total_steps * pt_begin->t_path_mode.ui2_frame_interval;
    if(WGL_IS_FLAG_TURN_ON(pt_begin->t_path_mode.ui2_anim_type,WGL_ANIM_TYPE_IMG))
    {
        pt_img = &(pt_begin->t_anim_data.u_data.t_img);
        if((pt_img == NULL) || (pt_img->ui1_img_seq_id >= (UINT8)pt_anim->ui2_img_seq_max_len))
        {
            return ui4_end_tick;
        }
        
        if(pt_begin->t_path_mode.e_play_mode == ANIM_PLAY_MODE_LOOP)
        {
            if(pt_begin->t_path_mode.ui2_loop_count == WGL_ANIM_INFINITE_VALUE)
            {
                return WGL_ANIM_INFINITE_VALUE;
            }
            else
            {
            ui8_time = ui8_time * pt_begin->t_path_mode.ui2_loop_count;
        }
        }
        else if(pt_begin->t_path_mode.e_play_mode == ANIM_PLAY_MODE_CUST &&
                pt_img->ui2_cust_len > 0 && pt_img->pt_cust_info != NULL)
        {
            ui8_time = 0;
            for(ui2_i = 0; ui2_i < pt_img->ui2_cust_len; ui2_i ++)
            {
               t_cust_info = *(pt_anim->t_data.t_init.t_img.pt_cust_info + ui2_i);
               ui8_time += _GET_CUST_INTERVAL(t_cust_info);                  
            }
        }
    }

    ui4_result = (UINT32)(ui8_time / pt_anim->ui4_tick_interval);
    if(((UINT64)(ui4_result * pt_anim->ui4_tick_interval)) < ui8_time)
    {
        ui4_result ++;
    }
    
    ui4_end_tick = pt_begin->t_path_mode.ui4_start_tick + ui4_result;
    return ui4_end_tick;
}

static INT32 _wgl_attr_get_total_ticks(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim)
{
    UINT16                          ui2_i;
    UINT32                          ui4_end_tick = 0;
    
    WGL_CHECK_NULL(pt_anim,WGLR_INV_ARG);
    
    for(ui2_i = 0; ui2_i < pt_anim->ui2_anim_total; ui2_i ++)
    {
        ui4_end_tick = _wgl_attr_get_anim_ticks(pt_anim,ui2_i);
        if(ui4_end_tick >= WGL_ANIM_INFINITE_VALUE)
        {
            pt_anim->ui4_total_end_tick = WGL_ANIM_INFINITE_VALUE;
            return WGLR_OK;
        }
        else if(ui4_end_tick > pt_anim->ui4_total_end_tick)
        {
            pt_anim->ui4_total_end_tick = ui4_end_tick;
        }
    }    
    return WGLR_OK;
}

static INT32 _wgl_attr_init_begin_data(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim, 
    WGL_ANIMATION_DATA_T*           pt_anim_data)
{
    UINT16                          ui2_i;
    UINT32                          ui4_least_tick;
    UINT32                          ui4_last_tick;
    UINT16                          ui2_cnt = 0;
    UINT16                          ui2_parse_cnt = 0;
    BOOL                            b_last_tick = FALSE;
    BOOL                            b_alloc = FALSE;
    WGL_ANIMATION_PATH_MODE_T       t_path_mode;
    WGL_ANIM_PATH_DATA_T*           pt_path = NULL;    
    WGL_ANIM_PATH_DATA_T*           pt_path_tmp = NULL;  
    WGL_ANIM_PATH_DATA_T*           pt_path_src = NULL;  
    WGL_ANIM_BEGIN_DATA_T*          pt_begin;
    UINT32                          ui4_size;
    ANIM_IMG_CUST_INFO_T*           pt_cust;
    WGL_IMG_ANIM_DATA_T*            pt_img;
    

    if((pt_anim == NULL) || (pt_anim_data == NULL) || (pt_anim_data->ui2_count == 0) ||
       (!WGL_IS_FLAG_TURN_ON(pt_anim_data->ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF)) || 
       ((!WGL_IS_FLAG_TURN_ON(pt_anim_data->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF))&&
        (!WGL_IS_FLAG_TURN_ON(pt_anim_data->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_ID_INF))) )
    {
        DBG_ERROR(("{WGL(ANIM)} ERR: Invalid NULL arguments. File=%s,Line=%d\n\n", __FILE__, __LINE__));
        return WGLR_INV_ARG;
    }

    if(WGL_IS_FLAG_TURN_ON(pt_anim_data->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF))
    {
        pt_path = pt_anim_data->pt_path_mode;
    }
    else if(WGL_IS_FLAG_TURN_ON(pt_anim_data->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_ID_INF))
    {
        UINT32                  ui4_path_id;
        BOOL                    b_path_find;
        UINT16                  ui2_path_indx = 0;

        
        if(pt_anim_data->t_path_id_inf.ui2_path_cnt >= WGL_ANIM_MAX_PATH_CNT_ONCE)
        {
            DBG_ERROR(("{WGL(ANIM)} ERR: Invalid NULL arguments. File=%s,Line=%d\n\n", __FILE__, __LINE__));
            return WGLR_INV_ARG;
        }
        
        for(ui2_i = 0; ui2_i < pt_anim_data->t_path_id_inf.ui2_path_cnt; ui2_i ++)
        {
            ui4_path_id = pt_anim_data->t_path_id_inf.aui4_path_id[ui2_i];
            if(ui4_path_id == WGL_ANIM_NULL_PAHT_ID)
            {
                continue;
            }
            
            x_memset(&t_path_mode,0,sizeof(WGL_ANIMATION_PATH_MODE_T));
            b_path_find = wgl_anim_lib_get_path(ui4_path_id,&t_path_mode);
            if(b_path_find == TRUE)
            {
                ui2_cnt += t_path_mode.ui2_count;
            }
        }

        if((ui2_cnt == 0) || (ui2_cnt != pt_anim_data->ui2_count))
        {
            DBG_ERROR(("{WGL(ANIM)} ERR: Invalid NULL arguments. File=%s,Line=%d\n\n", __FILE__, __LINE__));
            return WGLR_INV_ARG;
        }

        pt_path = WGL_MEM_ALLOC(sizeof(WGL_ANIM_PATH_DATA_T)*ui2_cnt);
        if(pt_path == NULL)
        {
            DBG_ERROR(("{WGL(ANIM)} ERR: Invalid NULL arguments. File=%s,Line=%d\n\n", __FILE__, __LINE__));
            return WGLR_OUT_OF_MEMORY;
        }
        x_memset(pt_path,0,(sizeof(WGL_ANIM_PATH_DATA_T)*ui2_cnt));
        
        b_alloc = TRUE;
        pt_path_tmp = pt_path;
        ui2_cnt = 0;
        for(ui2_i = 0; ui2_i < pt_anim_data->t_path_id_inf.ui2_path_cnt; ui2_i ++)
        {
            ui4_path_id = pt_anim_data->t_path_id_inf.aui4_path_id[ui2_i];
            if(ui4_path_id == WGL_ANIM_NULL_PAHT_ID)
            {
                continue;
            }
            
            x_memset(&t_path_mode,0,sizeof(WGL_ANIMATION_PATH_MODE_T));
            b_path_find = wgl_anim_lib_get_path(ui4_path_id,&t_path_mode);
            if((b_path_find == TRUE) && (t_path_mode.ui2_count > 0))
            {
                pt_path_src = t_path_mode.pt_path_inf;
                for(ui2_path_indx = 0; ui2_path_indx< t_path_mode.ui2_count; ui2_path_indx ++)
                {
                    if(pt_path_src == NULL)
                    {
                        continue;
                    }
                    x_memcpy(pt_path_tmp,t_path_mode.pt_path_inf,sizeof(WGL_ANIM_PATH_DATA_T));
                    pt_path_tmp->ui4_start_tick += pt_anim_data->t_path_id_inf.aui4_srt_tick[ui2_i];
                    pt_path_tmp ++;
                    pt_path_src ++;
                    ui2_cnt ++;
                }
            }
        }

        if(ui2_cnt != pt_anim_data->ui2_count)
        {
            WGL_MEM_FREE(pt_path);
            DBG_ERROR(("{WGL(ANIM)} ERR: Invalid NULL arguments. File=%s,Line=%d\n\n", __FILE__, __LINE__));
            return WGLR_INV_ARG;
        }
        
    }
            
    if(pt_path == NULL)
    {
        DBG_ERROR(("{WGL(ANIM)} ERR: Invalid NULL arguments. File=%s,Line=%d\n\n", __FILE__, __LINE__));
        return WGLR_INV_ARG;
    }
    
    ui2_cnt = 0;
    pt_anim->pt_begin = WGL_MEM_ALLOC(sizeof(WGL_ANIM_BEGIN_DATA_T)*pt_anim_data->ui2_count);
    if(pt_anim->pt_begin == NULL)
    {
        DBG_ERROR(("{WGL(ANIM)} ERR: Invalid NULL arguments. File=%s,Line=%d\n\n", __FILE__, __LINE__));
        return WGLR_OUT_OF_MEMORY;
    }
    x_memset(pt_anim->pt_begin,0,(sizeof(WGL_ANIM_BEGIN_DATA_T)*pt_anim_data->ui2_count));
    
    pt_begin = pt_anim->pt_begin;
    if(pt_anim_data->ui2_count > 0)
    {
        ui4_least_tick = pt_path[0].ui4_start_tick;
        ui4_last_tick = ui4_least_tick;
        ui2_parse_cnt = 0;
        while(ui2_parse_cnt < pt_anim_data->ui2_count)
        {
            /*Get the least start tick value in the path.*/
            for(ui2_i = 0; ui2_i < pt_anim_data->ui2_count; ui2_i ++)
            {
                if(b_last_tick ==  FALSE)
                {
                    if(pt_path[ui2_i].ui4_start_tick < ui4_least_tick)
                    {
                        ui4_least_tick = pt_path[ui2_i].ui4_start_tick;
                    }

                }
                else if(pt_path[ui2_i].ui4_start_tick > ui4_last_tick)
                {
                    if(ui4_least_tick <= ui4_last_tick)
                    {
                        ui4_least_tick = pt_path[ui2_i].ui4_start_tick;
                    }
                    else if(pt_path[ui2_i].ui4_start_tick < ui4_least_tick)
                    {
                        ui4_least_tick = pt_path[ui2_i].ui4_start_tick;
                    }
                }

            }

            ui4_last_tick = ui4_least_tick;
            b_last_tick = TRUE;
            
            for(ui2_i = 0; ui2_i < pt_anim_data->ui2_count; ui2_i ++)
            {
                if(pt_path[ui2_i].ui4_start_tick == ui4_least_tick) 
                {
                    if(pt_path[ui2_i].ui2_anim_type == pt_anim_data->pt_anim_data[ui2_i].ui2_anim_type)
                {
                    x_memcpy(&pt_begin->t_path_mode,&pt_path[ui2_i],sizeof(WGL_ANIM_PATH_DATA_T));
                    x_memcpy(&pt_begin->t_anim_data,&pt_anim_data->pt_anim_data[ui2_i],sizeof(WGL_ANIM_DATA_INFO));
                    if(WGL_IS_FLAG_TURN_ON(pt_path[ui2_i].ui2_anim_type,WGL_ANIM_TYPE_IMG) &&
                       (pt_path[ui2_i].e_play_mode == ANIM_PLAY_MODE_CUST) &&
                       (pt_anim_data->pt_anim_data[ui2_i].u_data.t_img.ui2_cust_len > 0) &&
                       (pt_anim_data->pt_anim_data[ui2_i].u_data.t_img.pt_cust_info != NULL))
                    {
                        pt_img = &(pt_anim_data->pt_anim_data[ui2_i].u_data.t_img);
                        ui4_size = sizeof(ANIM_IMG_CUST_INFO_T) * pt_img->ui2_cust_len;
                        pt_cust = (ANIM_IMG_CUST_INFO_T*)WGL_MEM_ALLOC(ui4_size);
                        if(pt_cust != NULL)
                        {
                            x_memcpy(pt_cust,pt_img->pt_cust_info,ui4_size);
                            pt_begin->t_anim_data.u_data.t_img.pt_cust_info = pt_cust;
                        #ifdef CLI_LVL_ALL    
                            pt_anim->ui4_mem_size += ui4_size;
                        #endif
                        }
                    }
                    pt_begin ++;
                    ui2_cnt ++;
                }
                    ui2_parse_cnt ++;
                }
            }  

        }
    }

    pt_anim->ui2_anim_total = ui2_cnt;
    pt_anim->ui2_bgn_cur = 0;
    pt_anim->ui2_bgn_last = 0;
    pt_anim->ui4_total_end_tick = 0;

#ifdef CLI_LVL_ALL    
    pt_anim->ui4_mem_size += (UINT32)(sizeof(WGL_ANIM_BEGIN_DATA_T) * pt_anim_data->ui2_count);
#endif
    if(b_alloc == TRUE)
    {
        WGL_MEM_FREE(pt_path);
    }
    
    return WGLR_OK;
}

static INT32 _wgl_attr_begin_anim(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim, 
    VOID*                           pv_param1,
    UINT32                          ui4_tick)
{
    WGL_ANIMATION_DATA_T*           pt_init;
    INT32                           i4_ret;

    pt_init = (WGL_ANIMATION_DATA_T*)pv_param1; 
    
     /*check pv_param1 parameter*/
    if ((pt_init == NULL) || (pt_init->ui2_count == 0))
    {
        DBG_ERROR(("{WGL(ANIM)} ERR: Invalid NULL arguments pv_param1 . File=%s,Line=%d\n\n", __FILE__, __LINE__));
        return WGLR_INV_ARG;
    }

    i4_ret = _wgl_attr_init_begin_data(pt_anim,pt_init);
    if(i4_ret != WGLR_OK)
    {
        return i4_ret;
    }
    
    _wgl_attr_get_total_ticks(pt_anim);
    
    /*update playing state*/
    if (!(pt_anim->ui2_state & ANIM_STATE_PLAYING))
    {
        WGL_UPDATE_STATE(pt_anim->ui2_state, TRUE, ANIM_STATE_PLAYING);
        pt_anim->t_info.e_state = WGL_ANIM_STATE_PLAYING ;
    } 

    x_memset(&pt_anim->t_current_data,0,sizeof(WGL_ANIM_TICK_DATA));
    WGL_SET_FLAG(pt_anim->t_current_data.ui2_begin_flag,WGL_ANIM_TYPE_NONE);
    WGL_SET_FLAG(pt_anim->t_info.ui2_done_type,WGL_ANIM_TYPE_NONE);
    pt_anim->t_current_data.t_new_tick.t_tick_inf.t_scale.ui4_width = WGL_ANIM_INIT_WIDTH;
    pt_anim->t_current_data.t_new_tick.t_tick_inf.t_scale.ui4_height = WGL_ANIM_INIT_HEIGHT;
    pt_anim->t_current_data.t_old_tick.t_tick_inf.t_scale.ui4_width = WGL_ANIM_INIT_WIDTH;
    pt_anim->t_current_data.t_old_tick.t_tick_inf.t_scale.ui4_height = WGL_ANIM_INIT_HEIGHT;
    pt_anim->ui4_start_tick = ui4_tick;
    pt_anim->t_current_data.ui4_tick_seq = ui4_tick;
    return WGLR_OK;
}

static INT32 _wgl_anim_attr_ntf_cond(
    WGL_ANIM_CONTROL_LOGIC_T        *pt_anim,
    UINT32                          ui4_tick_seq,
    WGL_ANIM_COND_INF_T*            pt_cond)
{
    WGL_CHECK_NULL(pt_anim,WGLR_INV_ARG);

    if (pt_cond == NULL)
    {
        return WGLR_INV_ARG;
    }
    
    x_memset(pt_cond,0,sizeof(WGL_ANIM_COND_INF_T));
    
    if( (pt_anim->ui4_total_end_tick != WGL_ANIM_INFINITE_VALUE) &&
        ((ui4_tick_seq - pt_anim->ui4_start_tick) >= pt_anim->ui4_total_end_tick) )
    {
        pt_cond->ui2_end_type = WGL_ANIM_TYPE_ALL;
        pt_cond->e_cond[WGL_ANIM_TOTAL] = WGL_ANIM_TOTAL_STEP_END;
        WGL_CLEAR_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_NONE);
        WGL_CLEAR_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_NONE);             
        x_memset(&pt_anim->t_data,0,sizeof(WGL_ANIM_DATA_T));
        WGL_UPDATE_STATE(pt_anim->ui2_state, FALSE, ANIM_STATE_PLAYING);  
        pt_anim->t_info.e_state = WGL_ANIM_STATE_COMPLETE_END;
        return WGLR_OK;
    }
   
   if (WGL_IS_FLAG_TURN_ON(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_MOVE))
   {
        if ( pt_anim->t_data.ui2_move_step >= pt_anim->t_data.t_init.t_move.ui2_total_steps
            && pt_anim->t_data.ui2_move_step != 0)
        {
             WGL_SET_FLAG(pt_cond->ui2_end_type,WGL_ANIM_TYPE_MOVE);
             pt_cond->e_cond[WGL_ANIM_MOVE] = WGL_ANIM_TYPE_END;

             WGL_CLEAR_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_MOVE);
             WGL_CLEAR_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_MOVE);             
             pt_anim->t_data.ui2_move_step = 0;
             pt_anim->t_data.ui2_move_idx = 0;
             x_memset(&pt_anim->t_data.t_init.t_move,0,sizeof(WGL_MOVE_ANIM_T));
        }
       
    }

     /*notify size type step end*/           
   if (WGL_IS_FLAG_TURN_ON(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_SIZE))
   {
        /*invoke condition callback*/
        if( pt_anim->t_data.ui2_size_step >= pt_anim->t_data.t_init.t_size.ui2_total_steps
            &&  pt_anim->t_data.ui2_size_step !=0)
        {
           WGL_SET_FLAG(pt_cond->ui2_end_type,WGL_ANIM_TYPE_SIZE);
           pt_cond->e_cond[WGL_ANIM_SIZE] = WGL_ANIM_TYPE_END;
           
           WGL_CLEAR_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_SIZE);
           WGL_CLEAR_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_SIZE);
           pt_anim->t_data.ui2_size_step = 0;
           pt_anim->t_data.ui2_size_idx = 0;
           x_memset(&pt_anim->t_data.t_init.t_size,0,sizeof(WGL_SIZE_ANIM_T));
        }    
   }   

     /*notify size type step end*/           
   if (WGL_IS_FLAG_TURN_ON(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_SCALE))
   {
        /*invoke condition callback*/
        if( pt_anim->t_data.ui2_scale_step >= pt_anim->t_data.t_init.t_scale.ui2_total_steps
            &&  pt_anim->t_data.ui2_scale_step !=0)
        {
           WGL_SET_FLAG(pt_cond->ui2_end_type,WGL_ANIM_TYPE_SCALE);
           pt_cond->e_cond[WGL_ANIM_SCALE] = WGL_ANIM_TYPE_END;
           
           WGL_CLEAR_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_SCALE);
           WGL_CLEAR_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_SCALE);
           pt_anim->t_data.ui2_scale_step = 0;
           pt_anim->t_data.ui2_scale_idx = 0;
           x_memset(&pt_anim->t_data.t_init.t_scale,0,sizeof(WGL_SIZE_ANIM_T));
        }    
   }   
   
   /*notify alpha total step end*/           
   if (WGL_IS_FLAG_TURN_ON(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_ALPHA))
   {
        /*invoke condition callback*/
        if( pt_anim->t_data.ui2_alpha_step >= pt_anim->t_data.t_init.t_alpha.ui2_total_steps
            &&  pt_anim->t_data.ui2_alpha_step !=0)
        {
           WGL_SET_FLAG(pt_cond->ui2_end_type,WGL_ANIM_TYPE_ALPHA);
           pt_cond->e_cond[WGL_ANIM_ALPHA] = WGL_ANIM_TYPE_END;
           
           WGL_CLEAR_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_ALPHA);
           WGL_CLEAR_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_ALPHA);
           pt_anim->t_data.ui2_alpha_step = 0;
           pt_anim->t_data.ui2_alpha_idx = 0;
           x_memset(&pt_anim->t_data.t_init.t_alpha,0,sizeof(WGL_ALPHA_ANIM_T));
        }    
   }
   
     /*notify  image total step end or loop end*/
    if (WGL_IS_FLAG_TURN_ON(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_IMG)
        && pt_anim->t_data.ui2_img_step !=0)
    {        
        if (pt_anim->t_data.t_init.t_img.e_play_mode == ANIM_PLAY_MODE_LOOP 
            && (pt_anim->t_data.ui2_img_step % pt_anim->pt_img_seq[pt_anim->t_data.t_init.t_img.ui1_img_seq_id].ui2_len) == 0
            && !pt_anim->t_data.b_loop_ntf_flg)
        {   
           WGL_SET_FLAG(pt_cond->ui2_end_type,WGL_ANIM_TYPE_IMG);
           pt_cond->e_cond[WGL_ANIM_IMG] = WGL_ANIM_ONE_LOOP_END;
        } 
        
        if ( (pt_anim->ui4_total_end_tick != WGL_ANIM_INFINITE_VALUE) &&
             ((UINT32)pt_anim->t_data.ui2_img_step >= pt_anim->t_data.ui4_max_step))
        {
           WGL_SET_FLAG(pt_cond->ui2_end_type,WGL_ANIM_TYPE_IMG);
           pt_cond->e_cond[WGL_ANIM_IMG] = WGL_ANIM_TYPE_END;
           
           WGL_CLEAR_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_IMG);
           WGL_CLEAR_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_IMG);
           pt_anim->t_data.ui2_img_step = 0;           
           pt_anim->t_data.ui2_img_idx = 0;
           x_memset(&pt_anim->t_data.t_init.t_img,0,sizeof(WGL_ALPHA_ANIM_T));
        } 
    }

   return WGLR_OK;
}

static UINT16 _wgl_anim_chk_new_begin(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT32                          ui4_tick_index)
{
    UINT16                          ui2_bgn_cnt;
    UINT16                          ui2_bgn_idx;
    WGL_ANIM_BEGIN_DATA_T*          pt_bgn_data;

    if((pt_anim == NULL) || (pt_anim->ui2_bgn_cur == pt_anim->ui2_anim_total)) 
    {
        return 0;
    }
    
    ui2_bgn_cnt = 0;
    ui2_bgn_idx = pt_anim->ui2_bgn_cur;
    while(ui2_bgn_idx < pt_anim->ui2_anim_total)
    {
        pt_bgn_data = pt_anim->pt_begin + ui2_bgn_idx;
        if(pt_bgn_data == NULL)
        {
            break;
        }

        if(ui4_tick_index >= pt_bgn_data->t_path_mode.ui4_start_tick)
        {
            ui2_bgn_cnt ++;
        }
        else
        {
            break;
        }
        ui2_bgn_idx ++;
    }

    pt_anim->ui2_bgn_cur = ui2_bgn_idx;
    
    return ui2_bgn_cnt;
}

static INT32 _wgl_anim_attr_do_chg_move(
    WGL_ANIM_CONTROL_LOGIC_T        *pt_anim,
    UINT32                          ui4_tick_seq,
    UINT32*                         pui4_count,
    OUT  GL_POINT_T*                pt_position)
{
    UINT64                          ui8_time;
    UINT64                          ui8_result;
    UINT32                          ui4_adjust;
    INT32                           i4_temp = 0;
    WGL_MOVE_ANIM_T                 t_move;
    GL_POINT_T                      t_new_pos;  
    INT32                           i4_ret; 
    wgl_app_anim_type_cb_fct        pf_type_cb;
    WGL_ANIM_BEGIN_DATA_T*          pt_anim_data;
    WGL_ANIM_TICK_T                 t_anim_tick;
    
    if((pui4_count == NULL) || (pt_position == NULL) ||
       (WGL_IS_FLAG_TURN_OFF(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_MOVE)) ||
       (pt_anim->t_data.ui2_move_idx > pt_anim->ui2_anim_total) ||
       (pt_anim->t_data.ui2_move_step >= pt_anim->t_data.t_init.t_move.ui2_total_steps))
    {          
          return WGLR_OK;  
    } 

    *pui4_count = 0;
    if (ui4_tick_seq != pt_anim->ui4_move_tick_start)
    { 
        if (ui4_tick_seq < pt_anim->ui4_move_tick_start)
        {
            ui4_adjust = MAX_UINT32 - pt_anim->ui4_move_tick_start + 1;            
            ui8_time = (ui4_tick_seq + ui4_adjust) * pt_anim->ui4_tick_interval;              
        }
        else       
        {
            ui8_time = (ui4_tick_seq - pt_anim->ui4_move_tick_start) * pt_anim->ui4_tick_interval;   
        }        
        
        ui8_result = ui8_time / pt_anim->t_data.t_init.t_move.ui2_frame_interval;                 
        if (ui8_result < pt_anim->t_data.ui2_move_step)
        {
            return WGLR_OK;
        } 
       
        if (ui8_result >= pt_anim->t_data.t_init.t_move.ui2_total_steps)
        {
            pt_anim->t_data.ui2_move_step = pt_anim->t_data.t_init.t_move.ui2_total_steps;
        }
        else
        {
             pt_anim->t_data.ui2_move_step = (UINT16)(ui8_result + 1 );
        }
    }
    else
    {
         pt_anim->t_data.ui2_move_step = 1;  
    }    

    x_memcpy(&t_move,&pt_anim->t_data.t_init.t_move,sizeof(WGL_MOVE_ANIM_T));
   
    pf_type_cb = pt_anim->t_data.t_init.t_move.pf_type_cb;
    x_memset(&t_anim_tick,0,sizeof(WGL_ANIM_TICK_T));
    x_memset(&t_new_pos,0,sizeof(GL_POINT_T));
    if (pf_type_cb != NULL) 
    {
        pt_anim_data = pt_anim->pt_begin + pt_anim->t_data.ui2_move_idx;
        if(pt_anim_data == NULL)
        {
            return WGLR_INV_ARG;
        }
        
        i4_ret = pf_type_cb(pt_anim->t_data.ui2_move_step,
                            pt_anim_data,
                            &t_anim_tick);
        CATCH_FAIL(i4_ret);      
        t_new_pos.i4_x = t_anim_tick.t_point.i4_x;
        t_new_pos.i4_y = t_anim_tick.t_point.i4_y;
        
    }
    else if(pt_anim->t_data.ui2_move_step == 1)
    {
        t_new_pos.i4_x = (INT32)t_move.i2_start_x;
        t_new_pos.i4_y = (INT32)t_move.i2_start_y;
    }
    else
    {
        if (t_move.i2_end_x != t_move.i2_start_x)
        {
            i4_temp = (INT32)((t_move.i2_end_x - t_move.i2_start_x) * 
                (pt_anim->t_data.ui2_move_step-1)) / (INT32)(t_move.ui2_total_steps-1); 
            t_new_pos.i4_x = (INT32)t_move.i2_start_x + i4_temp; 
        }
        else
        {
            t_new_pos.i4_x = t_move.i2_start_x; 
        }

        if (t_move.i2_end_y != t_move.i2_start_y)
        {
            i4_temp = (INT32)((t_move.i2_end_y - t_move.i2_start_y) * 
                (pt_anim->t_data.ui2_move_step-1)) / (INT32)(t_move.ui2_total_steps-1);  
            t_new_pos.i4_y = (INT32)t_move.i2_start_y + i4_temp;
        }
        else
        {
            t_new_pos.i4_y = t_move.i2_start_y;
        }            
            
    }

    (*pui4_count) ++;
    x_memcpy(pt_position,&t_new_pos,sizeof(GL_POINT_T));
    
    return WGLR_OK ;
}

static INT32 _wgl_anim_attr_do_chg_scale(
    WGL_ANIM_CONTROL_LOGIC_T        *pt_anim,
    UINT32                          ui4_tick_seq,
    UINT32*                         pui4_count,
    OUT  GL_SIZE_T*                 pt_new_size)
{
    UINT64                          ui8_time;
    UINT64                          ui8_result;
    UINT32                          ui4_adjust;
    INT32                           i4_temp = 0;
    WGL_SIZE_ANIM_T                 t_size;
    INT32                           i4_ret; 
    GL_SIZE_T                       t_new_size;
    wgl_app_anim_type_cb_fct        pf_type_cb;
    WGL_ANIM_BEGIN_DATA_T*          pt_anim_data;
    WGL_ANIM_TICK_T                 t_anim_tick;

    if((pui4_count == NULL) || (pt_new_size == NULL) ||
       (WGL_IS_FLAG_TURN_OFF(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_SCALE)) ||
       (pt_anim->t_data.ui2_scale_idx > pt_anim->ui2_anim_total) ||
       (pt_anim->t_data.ui2_scale_step >= pt_anim->t_data.t_init.t_scale.ui2_total_steps))
    {          
          return WGLR_OK;  
    } 
    *pui4_count = 0;
    
    if( pt_anim->t_data.ui2_scale_step >= pt_anim->t_data.t_init.t_scale.ui2_total_steps)
    {          
          return WGLR_OK;  
    } 

    if (ui4_tick_seq != pt_anim->ui4_size_tick_start)
    { 
        if (ui4_tick_seq < pt_anim->ui4_size_tick_start)
        {
            ui4_adjust = MAX_UINT32 - pt_anim->ui4_size_tick_start + 1;            
            ui8_time = (ui4_tick_seq + ui4_adjust) * pt_anim->ui4_tick_interval;              
        }
        else       
        {
            ui8_time = (ui4_tick_seq - pt_anim->ui4_size_tick_start) * pt_anim->ui4_tick_interval;   
        }        
        
        ui8_result = ui8_time / pt_anim->t_data.t_init.t_scale.ui2_frame_interval;                 
        if (ui8_result < pt_anim->t_data.ui2_scale_step)
        {
            return WGLR_OK;
        } 
       
        if (ui8_result >= pt_anim->t_data.t_init.t_scale.ui2_total_steps)
        {
            pt_anim->t_data.ui2_scale_step = pt_anim->t_data.t_init.t_scale.ui2_total_steps;
        }
        else
        {
             pt_anim->t_data.ui2_scale_step = (UINT16)(ui8_result + 1 );
        }
    }
    else
    {
         pt_anim->t_data.ui2_scale_step = 1;  
    }    

    x_memcpy(&t_size,&pt_anim->t_data.t_init.t_scale,sizeof(WGL_SIZE_ANIM_T));
    pf_type_cb = t_size.pf_type_cb;
    x_memset(&t_new_size,0,sizeof(GL_SIZE_T));
    x_memset(&t_anim_tick,0,sizeof(WGL_ANIM_TICK_T));
    if (pf_type_cb != NULL)
    {
        pt_anim_data = pt_anim->pt_begin + pt_anim->t_data.ui2_scale_idx;
        if(pt_anim_data == NULL)
        {
            return WGLR_INV_ARG;
        }
        
        i4_ret = pf_type_cb(pt_anim->t_data.ui2_scale_step,pt_anim_data,&t_anim_tick);        
        CATCH_FAIL(i4_ret);
        x_memcpy(&t_new_size,&t_anim_tick.t_scale,sizeof(GL_SIZE_T));
    }
    else if (pt_anim->t_data.ui2_scale_step == 1)    
    {
        t_new_size.ui4_width = pt_anim->t_data.t_init.t_scale.ui2_start_w;
        t_new_size.ui4_height = pt_anim->t_data.t_init.t_scale.ui2_start_h;
    }
    else
    {
        if (t_size.ui2_end_w != t_size.ui2_start_w)
        {
            i4_temp = (INT32)((t_size.ui2_end_w - t_size.ui2_start_w) * (pt_anim->t_data.ui2_scale_step-1))
                   / (INT32)(t_size.ui2_total_steps-1); 
            t_new_size.ui4_width = (UINT32)(t_size.ui2_start_w + i4_temp); 
        } 
        else
        {
             t_new_size.ui4_width = t_size.ui2_start_w; 
        }
        
        if (t_size.ui2_end_h != t_size.ui2_start_h)
        {
            i4_temp = (INT32)((t_size.ui2_end_h - t_size.ui2_start_h) * (pt_anim->t_data.ui2_scale_step-1))
                   / (INT32)(t_size.ui2_total_steps-1); 
            t_new_size.ui4_height = (UINT32)(t_size.ui2_start_h + i4_temp); 
        } 
        else
        {
             t_new_size.ui4_height = t_size.ui2_start_h; 
        }        
    }

    (*pui4_count) ++;
    x_memcpy(pt_new_size,&t_new_size,sizeof(GL_SIZE_T));

    return WGLR_OK ;
}

static INT32 _wgl_anim_attr_do_chg_size(
    WGL_ANIM_CONTROL_LOGIC_T        *pt_anim,
    UINT32                          ui4_tick_seq,
    UINT32*                         pui4_count,
    OUT  GL_SIZE_T*                 pt_new_size)
{
    UINT64                          ui8_time;
    UINT64                          ui8_result;
    UINT32                          ui4_adjust;
    INT32                           i4_temp = 0;
    WGL_SIZE_ANIM_T                 t_size;
    INT32                           i4_ret; 
    GL_SIZE_T                       t_new_size;
    wgl_app_anim_type_cb_fct        pf_type_cb;
    WGL_ANIM_BEGIN_DATA_T*          pt_anim_data;
    WGL_ANIM_TICK_T                 t_anim_tick;

    if((pui4_count == NULL) || (pt_new_size == NULL) ||
       (WGL_IS_FLAG_TURN_OFF(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_SIZE)) ||
       (pt_anim->t_data.ui2_size_idx > pt_anim->ui2_anim_total) ||
       (pt_anim->t_data.ui2_size_step >= pt_anim->t_data.t_init.t_size.ui2_total_steps))
    {          
          return WGLR_OK;  
    } 
    *pui4_count = 0;
    
    if( pt_anim->t_data.ui2_size_step >= pt_anim->t_data.t_init.t_size.ui2_total_steps)
    {          
          return WGLR_OK;  
    } 

    if (ui4_tick_seq != pt_anim->ui4_size_tick_start)
    { 
        if (ui4_tick_seq < pt_anim->ui4_size_tick_start)
        {
            ui4_adjust = MAX_UINT32 - pt_anim->ui4_size_tick_start + 1;            
            ui8_time = (ui4_tick_seq + ui4_adjust) * pt_anim->ui4_tick_interval;              
        }
        else       
        {
            ui8_time = (ui4_tick_seq - pt_anim->ui4_size_tick_start) * pt_anim->ui4_tick_interval;   
        }        
        
        ui8_result = ui8_time / pt_anim->t_data.t_init.t_size.ui2_frame_interval;                 
        if (ui8_result < pt_anim->t_data.ui2_size_step)
        {
            return WGLR_OK;
        } 
       
        if (ui8_result >= pt_anim->t_data.t_init.t_size.ui2_total_steps)
        {
            pt_anim->t_data.ui2_size_step = pt_anim->t_data.t_init.t_size.ui2_total_steps;
        }
        else
        {
             pt_anim->t_data.ui2_size_step = (UINT16)(ui8_result + 1 );
        }
    }
    else
    {
         pt_anim->t_data.ui2_size_step = 1;  
    }    

    x_memcpy(&t_size,&pt_anim->t_data.t_init.t_size,sizeof(WGL_SIZE_ANIM_T));
    pf_type_cb = t_size.pf_type_cb;
    x_memset(&t_new_size,0,sizeof(GL_SIZE_T));
    x_memset(&t_anim_tick,0,sizeof(WGL_ANIM_TICK_T));
    if (pf_type_cb != NULL)
    {
        pt_anim_data = pt_anim->pt_begin + pt_anim->t_data.ui2_size_idx;
        if(pt_anim_data == NULL)
        {
            return WGLR_INV_ARG;
        }
        
        i4_ret = pf_type_cb(pt_anim->t_data.ui2_size_step,pt_anim_data,&t_anim_tick);        
        CATCH_FAIL(i4_ret);
        x_memcpy(&t_new_size,&t_anim_tick.t_size,sizeof(GL_SIZE_T));
    }
    else if (pt_anim->t_data.ui2_size_step == 1)    
    {
        t_new_size.ui4_width = pt_anim->t_data.t_init.t_size.ui2_start_w;
        t_new_size.ui4_height = pt_anim->t_data.t_init.t_size.ui2_start_h;
    }
    else
    {
        if (t_size.ui2_end_w != t_size.ui2_start_w)
        {
            i4_temp = (INT32)((t_size.ui2_end_w - t_size.ui2_start_w) * (pt_anim->t_data.ui2_size_step-1))
                   / (INT32)(t_size.ui2_total_steps-1); 
            t_new_size.ui4_width = (UINT32)(t_size.ui2_start_w + i4_temp); 
        } 
        else
        {
             t_new_size.ui4_width = t_size.ui2_start_w; 
        }
        
        if (t_size.ui2_end_w != t_size.ui2_start_w)
        {
            i4_temp = (INT32)((t_size.ui2_end_w - t_size.ui2_start_w) * (pt_anim->t_data.ui2_size_step-1))
                   / (INT32)(t_size.ui2_total_steps-1); 
            t_new_size.ui4_height = (UINT32)(t_size.ui2_start_w + i4_temp); 
        } 
        else
        {
             t_new_size.ui4_height = t_size.ui2_start_w; 
        }        
    }

    (*pui4_count) ++;
    x_memcpy(pt_new_size,&t_new_size,sizeof(GL_SIZE_T));

    return WGLR_OK ;
}
    
static INT32 _wgl_anim_attr_do_chg_alpha(
    WGL_ANIM_CONTROL_LOGIC_T        *pt_anim,
    UINT32                          ui4_tick_seq,
    UINT32*                         pui4_count,
    OUT  UINT8*                     pui1_alpha)
{
    UINT64                          ui8_time;
    UINT64                          ui8_result;
    UINT32                          ui4_adjust;
    INT32                           i4_temp;
    WGL_ALPHA_ANIM_T                t_alpha;
    INT32                           i4_ret;
    UINT8                           ui1_new_alpha = 255;
    wgl_app_anim_type_cb_fct        pf_type_cb;
    WGL_ANIM_BEGIN_DATA_T*          pt_anim_data;
    WGL_ANIM_TICK_T                 t_anim_tick;
    
    if((pui4_count == NULL) || (pui1_alpha == NULL) ||
       (pt_anim->t_data.ui2_alpha_idx > pt_anim->ui2_anim_total) ||
       (pt_anim->t_data.ui2_alpha_step >= pt_anim->t_data.t_init.t_alpha.ui2_total_steps))
    {          
          return WGLR_OK;  
    } 
    *pui4_count = 0;
    
    if( pt_anim->t_data.ui2_alpha_step >= pt_anim->t_data.t_init.t_alpha.ui2_total_steps)
    {          
          return WGLR_OK;  
    } 

    if (ui4_tick_seq != pt_anim->ui4_alpha_tick_start)
    { 
        if (ui4_tick_seq < pt_anim->ui4_alpha_tick_start)
        {
            ui4_adjust = MAX_UINT32 - pt_anim->ui4_alpha_tick_start + 1;            
            ui8_time = (ui4_tick_seq + ui4_adjust) * pt_anim->ui4_tick_interval;              
        }
        else       
        {
            ui8_time = (ui4_tick_seq - pt_anim->ui4_alpha_tick_start) * pt_anim->ui4_tick_interval;   
        }        
        
        ui8_result = ui8_time / pt_anim->t_data.t_init.t_alpha.ui2_frame_interval;                 
        if (ui8_result < pt_anim->t_data.ui2_alpha_step)
        {
            return WGLR_OK;
        }
        if (ui8_result >= pt_anim->t_data.t_init.t_alpha.ui2_total_steps)
        {
            pt_anim->t_data.ui2_alpha_step =pt_anim->t_data.t_init.t_alpha.ui2_total_steps; 
        }
        else
        {
            pt_anim->t_data.ui2_alpha_step = (UINT16)(ui8_result + 1 );
        }
    }
    else
    {
        pt_anim->t_data.ui2_alpha_step = 1; 
    }
    
    x_memcpy(&t_alpha, &pt_anim->t_data.t_init.t_alpha, sizeof(WGL_ALPHA_ANIM_T));    

    pf_type_cb = t_alpha.pf_type_cb;
    x_memset(&t_anim_tick,0,sizeof(WGL_ANIM_TICK_T));
    if(pf_type_cb != NULL)
    {
        pt_anim_data = pt_anim->pt_begin + pt_anim->t_data.ui2_alpha_idx;
        if(pt_anim_data == NULL)
        {
            return WGLR_INV_ARG;
        }
        
        i4_ret = pf_type_cb(pt_anim->t_data.ui2_alpha_step,pt_anim_data,&t_anim_tick);        
        CATCH_FAIL(i4_ret);
        ui1_new_alpha = t_anim_tick.ui1_alpha;
    }
    else if (pt_anim->t_data.ui2_alpha_step == 1)
    {
        ui1_new_alpha = pt_anim->t_data.t_init.t_alpha.ui1_start_alpha;
    }
    else
    {           
        /* update the alpha value*/
        i4_temp = (INT32)((t_alpha.ui1_end_alpha -t_alpha.ui1_start_alpha) * 
                  (pt_anim->t_data.ui2_alpha_step-1)) / (INT32)(t_alpha.ui2_total_steps-1);
        i4_temp = t_alpha.ui1_start_alpha + i4_temp;
        ui1_new_alpha = (UINT8)i4_temp;
    }

    (*pui4_count) ++;
    *pui1_alpha = ui1_new_alpha;
    return WGLR_OK;
    
}

static INT32 _wgl_anim_attr_do_chg_img(
    WGL_ANIM_CONTROL_LOGIC_T        *pt_anim,
    UINT32                          ui4_tick_seq,
    UINT32*                         pui4_count,
    OUT  WGL_ANIM_IMG_INF_T*        pt_img)
{
    WGL_IMG_ANIM_T                  t_img_anim;
    WGL_ANIM_IMG_SEQ_T*             pt_img_seq;
    UINT64                          ui8_time;
    UINT64                          ui8_result;
    UINT32                          ui4_adjust;
    BOOL                            b_loop_flag;
    UINT16                          ui2_img_index = 0;
    ANIM_IMG_CUST_INFO_T            t_cust_info;    
    INT32                           i4_ret;
    wgl_app_anim_type_cb_fct        pf_type_cb;
    WGL_ANIM_BEGIN_DATA_T*          pt_anim_data;
    WGL_ANIM_TICK_T                 t_anim_tick;

    if((pui4_count == NULL) || (pt_img == NULL) ||
       (pt_anim->t_data.ui2_img_idx > pt_anim->ui2_anim_total) ||
       (pt_anim->t_data.ui4_max_step!= 0 && 
       (UINT32)pt_anim->t_data.ui2_img_step >= pt_anim->t_data.ui4_max_step) )
    {          
        return WGLR_OK;  
    } 
    
    *pui4_count = 0;
   
    x_memcpy(&t_img_anim,&pt_anim->t_data.t_init.t_img,sizeof(WGL_IMG_ANIM_T));
    if(t_img_anim.ui1_img_seq_id >= (UINT8)pt_anim->ui2_img_seq_max_len)
    {
        return WGLR_INV_ARG;
    }    
    pt_img_seq = &(pt_anim->pt_img_seq[t_img_anim.ui1_img_seq_id]);
    b_loop_flag = (t_img_anim.e_play_mode == ANIM_PLAY_MODE_LOOP);

    if( pt_anim->t_data.ui4_max_step!= 0 && (UINT32)pt_anim->t_data.ui2_img_step == pt_anim->t_data.ui4_max_step)
    {          
        return WGLR_OK;  
    }

    pf_type_cb = t_img_anim.pf_type_cb;
    x_memset(&t_anim_tick,0,sizeof(WGL_ANIM_TICK_T));
    if(pf_type_cb != NULL)
    {
        pt_anim_data = pt_anim->pt_begin + pt_anim->t_data.ui2_img_idx;
        if(pt_anim_data == NULL)
        {
            return WGLR_INV_ARG;
        }
        
        i4_ret = pf_type_cb(pt_anim->t_data.ui2_img_step,pt_anim_data,&t_anim_tick);        
        CATCH_FAIL(i4_ret);
        ui2_img_index = t_anim_tick.t_img.ui2_img_id;
    }
    else if (ui4_tick_seq == pt_anim->ui4_img_tick_start)
    {
       if (t_img_anim.e_play_mode == ANIM_PLAY_MODE_CUST)
       {
           t_cust_info = *(pt_anim->t_data.t_init.t_img.pt_cust_info);
           pt_anim->ui4_cust_tick = _GET_CUST_INTERVAL(t_cust_info);
           if (_GET_CUST_FLAG(t_cust_info)!= 0)
           {
               ui2_img_index = (UINT16)_GET_CUST_IMG_ID(t_cust_info);
           }
           else
           {
               ui2_img_index = pt_anim->t_data.ui2_img_step;
           }           
       }
       else
       {
           ui2_img_index = 0;
       }
       pt_anim->t_data.ui2_img_step = 1;
    }
    else
    {
        if (ui4_tick_seq < pt_anim->ui4_img_tick_start)
        {           
            ui4_adjust = MAX_UINT32 - pt_anim->ui4_img_tick_start + 1;           
            ui8_time = (ui4_tick_seq + ui4_adjust) * pt_anim->ui4_tick_interval;              
        }
        else       
        {
           ui8_time = (ui4_tick_seq - pt_anim->ui4_img_tick_start) * pt_anim->ui4_tick_interval;     
        }      
        
        if (t_img_anim.e_play_mode != ANIM_PLAY_MODE_CUST)
        {        
            /*receive tick not first time*/            
            ui8_result = ui8_time / t_img_anim.ui2_frame_interval;                 
            if (ui8_result < pt_anim->t_data.ui2_img_step)
            {
                return WGLR_OK;
            }  
            
            /*update image step*/
            if(ui8_result >= pt_img_seq->ui2_len && !b_loop_flag)
            {
                pt_anim->t_data.ui2_img_step = pt_img_seq->ui2_len;               
            }   
            else
            {
                pt_anim->t_data.ui2_img_step = (UINT16)(ui8_result + 1);
            }
            if (pt_anim->t_data.b_loop_ntf_flg)
            {
                pt_anim->t_data.b_loop_ntf_flg = FALSE;
            }
            /*get playing image index*/
            if (b_loop_flag)
            {
               ui2_img_index = (UINT16)((pt_anim->t_data.ui2_img_step -1) % pt_img_seq->ui2_len);
                
            }
            else
            {
               ui2_img_index = (UINT16)(pt_anim->t_data.ui2_img_step -1);
            }
                       
        }
        else /*t_img_anim.e_play_mode == ANIM_PLAY_MODE_CUST*/  
        {
           if (ui8_time >= pt_anim->ui4_cust_tick)
           {         
               pt_anim->t_data.ui2_img_step ++;
               t_cust_info = *(pt_anim->t_data.t_init.t_img.pt_cust_info +  pt_anim->t_data.ui2_img_step - 1);
               if (_GET_CUST_FLAG(t_cust_info)!= 0)
               {
                   ui2_img_index = (UINT16)_GET_CUST_IMG_ID(t_cust_info);
               }
               else
               {
                   ui2_img_index = (UINT16)(pt_anim->t_data.ui2_img_step - 1);
               }

               pt_anim->ui4_cust_tick += _GET_CUST_INTERVAL(t_cust_info);                  
           } 
           else
           {
                return WGLR_OK;
           }
        }
    }

    (*pui4_count) ++;
    pt_img->ui2_img_id = ui2_img_index;
    pt_img->pt_img = pt_img_seq->pt_img_tpl + ui2_img_index;
    
    return WGLR_OK;
}

static UINT16 _wgl_anim_attr_get_anim_type(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT16                          ui2_idx,
    WGL_ANIM_BEGIN_DATA_T**         ppt_bgn)
{
    UINT16                          ui2_type = WGL_ANIM_TYPE_NONE;
    WGL_ANIM_BEGIN_DATA_T*          pt_bgn_data ;

    if((pt_anim == NULL) || (ui2_idx >= pt_anim->ui2_anim_total))
    {
        return WGL_ANIM_TYPE_NONE;
    }

    pt_bgn_data = pt_anim->pt_begin + ui2_idx;
    if(pt_bgn_data == NULL)
    {
        return WGL_ANIM_TYPE_NONE;
    }

    ui2_type = pt_bgn_data->t_anim_data.ui2_anim_type;
    if(ui2_type != pt_bgn_data->t_path_mode.ui2_anim_type)
    {
        return WGL_ANIM_TYPE_NONE;
    }

    if(ppt_bgn != NULL)
    {
        *ppt_bgn = pt_bgn_data;
    }
    
    return ui2_type;
}

static INT32 _set_attr_img_begin_cust_data(
      WGL_ANIM_CONTROL_LOGIC_T*  pt_anim,
      WGL_IMG_ANIM_T*            pt_img_anim_dst,
      WGL_IMG_ANIM_DATA_T*       pt_img_anim_src)
{ 
    UINT32                       ui4_size;

    WGL_CHECK_NULL(pt_anim, WGLR_INV_ARG);

    if (NULL == pt_img_anim_dst)
    {
        return WGLR_OK;
    }
    
    if (NULL == pt_img_anim_src)
    {
        if (pt_img_anim_dst->ui2_cust_len != 0)
        {
            WGL_MEM_FREE(pt_img_anim_dst->pt_cust_info );
        }
        pt_img_anim_dst->ui2_cust_len = 0;
        return WGLR_OK;
    }
      
    if (pt_img_anim_dst->ui2_cust_len != pt_img_anim_src->ui2_cust_len)
    {
      if (pt_img_anim_dst->ui2_cust_len != 0)
      {
          WGL_MEM_FREE(pt_img_anim_dst->pt_cust_info );
      }
      ui4_size =(UINT32)(pt_img_anim_src->ui2_cust_len * sizeof(ANIM_IMG_CUST_INFO_T)); 
      pt_img_anim_dst->pt_cust_info  = WGL_MEM_ALLOC( ui4_size );
#ifdef CLI_LVL_ALL    
      pt_anim->ui4_mem_size += ui4_size;
#endif
    }

    if (NULL == pt_img_anim_dst->pt_cust_info )
    {
      DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Fail to alloc memory for pt_tmp_cust_info. (%d) File=%s, Line=%d\n\n", WGLR_OUT_OF_MEMORY,__FILE__, __LINE__));
      return WGLR_OUT_OF_MEMORY;
    }
    pt_img_anim_dst->ui2_cust_len = pt_img_anim_src->ui2_cust_len; 
    x_memcpy(pt_img_anim_dst->pt_cust_info,pt_img_anim_src->pt_cust_info,pt_img_anim_src->ui2_cust_len * sizeof(ANIM_IMG_CUST_INFO_T));       
  
   return WGLR_OK;
   
}


static INT32 _wgl_anim_attr_add_anim_bgn(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    WGL_ANIM_BEGIN_DATA_T*          pt_data,
    UINT16                          ui2_bgn_idx)
{
    UINT16                          ui2_type;
    
    if(pt_anim == NULL)
    {
        return WGLR_INV_ARG;
    }

    ui2_type = pt_data->t_anim_data.ui2_anim_type;
    if(ui2_type != pt_data->t_path_mode.ui2_anim_type)
    {
        return WGLR_INV_ARG;
    }
    
    if(ui2_type & WGL_ANIM_TYPE_MOVE)
    {
        WGL_SET_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_MOVE);
        pt_anim->t_data.t_init.t_move.i2_start_x = pt_data->t_anim_data.u_data.t_move.i2_start_x;
        pt_anim->t_data.t_init.t_move.i2_start_y = pt_data->t_anim_data.u_data.t_move.i2_start_y;
        pt_anim->t_data.t_init.t_move.i2_end_x = pt_data->t_anim_data.u_data.t_move.i2_end_x;
        pt_anim->t_data.t_init.t_move.i2_end_y = pt_data->t_anim_data.u_data.t_move.i2_end_y;
        pt_anim->t_data.t_init.t_move.ui2_total_steps = pt_data->t_path_mode.ui2_total_steps;
        pt_anim->t_data.t_init.t_move.ui2_frame_interval = pt_data->t_path_mode.ui2_frame_interval;
        pt_anim->t_data.ui2_move_idx = ui2_bgn_idx;
        if(pt_data->t_anim_data.pf_type_cb == NULL)
        {
            pt_anim->t_data.t_init.t_move.pf_type_cb = pt_data->t_path_mode.pf_type_cb;
        }
        else
        {
            pt_anim->t_data.t_init.t_move.pf_type_cb = pt_data->t_anim_data.pf_type_cb;
        }
        
    }

    if(ui2_type & WGL_ANIM_TYPE_ALPHA)
    {
        WGL_SET_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_ALPHA);
        pt_anim->t_data.t_init.t_alpha.ui1_start_alpha = pt_data->t_anim_data.u_data.t_alpha.ui1_start_alpha;
        pt_anim->t_data.t_init.t_alpha.ui1_end_alpha = pt_data->t_anim_data.u_data.t_alpha.ui1_end_alpha;
        pt_anim->t_data.t_init.t_alpha.ui2_total_steps = pt_data->t_path_mode.ui2_total_steps;
        pt_anim->t_data.t_init.t_alpha.ui2_frame_interval = pt_data->t_path_mode.ui2_frame_interval;
        pt_anim->t_data.ui2_alpha_idx = ui2_bgn_idx;

        if(pt_data->t_anim_data.pf_type_cb == NULL)
        {
            pt_anim->t_data.t_init.t_alpha.pf_type_cb = pt_data->t_path_mode.pf_type_cb;
        }
        else
        {
            pt_anim->t_data.t_init.t_alpha.pf_type_cb = pt_data->t_anim_data.pf_type_cb;
        }
        
    }    

    if(ui2_type & WGL_ANIM_TYPE_IMG)
    {
        WGL_SET_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_IMG);
        pt_anim->t_data.t_init.t_img.e_play_mode = pt_data->t_path_mode.e_play_mode;
        pt_anim->t_data.t_init.t_img.ui1_img_seq_id = pt_data->t_anim_data.u_data.t_img.ui1_img_seq_id;
        pt_anim->t_data.t_init.t_img.ui2_loop_count = pt_data->t_path_mode.ui2_loop_count;
        pt_anim->t_data.t_init.t_img.ui2_frame_interval = pt_data->t_path_mode.ui2_frame_interval;
        pt_anim->t_data.ui2_img_idx = ui2_bgn_idx;
        if(pt_data->t_anim_data.pf_type_cb == NULL)
        {
            pt_anim->t_data.t_init.t_img.pf_type_cb = pt_data->t_path_mode.pf_type_cb;
        }
        else
        {
            pt_anim->t_data.t_init.t_img.pf_type_cb = pt_data->t_anim_data.pf_type_cb;
        }
        
        if(pt_anim->t_data.t_init.t_img.e_play_mode == ANIM_PLAY_MODE_CUST)
        {
            _set_attr_img_begin_cust_data(pt_anim,
                                          &(pt_anim->t_data.t_init.t_img),
                                          &(pt_data->t_anim_data.u_data.t_img));
        }
        else 
        {
            if(pt_anim->t_data.t_init.t_img.pt_cust_info != NULL)
            {
                WGL_MEM_FREE(pt_anim->t_data.t_init.t_img.pt_cust_info);
            }
            pt_anim->t_data.t_init.t_img.ui2_cust_len = 0;
            pt_anim->t_data.t_init.t_img.pt_cust_info = NULL;
        }
        
        _set_img_anim_max_step(pt_anim);
    }    

    if(ui2_type & WGL_ANIM_TYPE_SCALE)
    {
        WGL_SET_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_SCALE);
        pt_anim->t_data.ui2_scale_idx = ui2_bgn_idx;
        pt_anim->t_data.t_init.t_scale.ui2_start_w = (UINT16)pt_data->t_anim_data.u_data.t_scale.ui8_start_w;
        pt_anim->t_data.t_init.t_scale.ui2_start_h = (UINT16)pt_data->t_anim_data.u_data.t_scale.ui8_start_h;
        pt_anim->t_data.t_init.t_scale.ui2_end_w = (UINT16)pt_data->t_anim_data.u_data.t_scale.ui8_end_w;
        pt_anim->t_data.t_init.t_scale.ui2_end_h = (UINT16)pt_data->t_anim_data.u_data.t_scale.ui8_end_h;
        pt_anim->t_data.t_init.t_scale.ui2_total_steps = pt_data->t_path_mode.ui2_total_steps;
        pt_anim->t_data.t_init.t_scale.ui2_frame_interval = pt_data->t_path_mode.ui2_frame_interval;
        pt_anim->t_data.t_init.t_scale.e_ref_type = pt_data->t_anim_data.u_data.t_scale.e_ref_type;
        if(pt_data->t_anim_data.pf_type_cb == NULL)
        {
            pt_anim->t_data.t_init.t_scale.pf_type_cb = pt_data->t_path_mode.pf_type_cb;
        }
        else
        {
            pt_anim->t_data.t_init.t_scale.pf_type_cb = pt_data->t_anim_data.pf_type_cb;
        }       
    } 
        
    if(ui2_type & WGL_ANIM_TYPE_SIZE && WGL_IS_FLAG_TURN_OFF(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_SCALE))
    {
        WGL_SET_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_SIZE);
        pt_anim->t_data.ui2_size_idx = ui2_bgn_idx;
        pt_anim->t_data.t_init.t_size.ui2_start_w = (UINT16)pt_data->t_anim_data.u_data.t_size.ui8_start_w;
        pt_anim->t_data.t_init.t_size.ui2_start_h = (UINT16)pt_data->t_anim_data.u_data.t_size.ui8_start_h;
        pt_anim->t_data.t_init.t_size.ui2_end_w = (UINT16)pt_data->t_anim_data.u_data.t_size.ui8_end_w;
        pt_anim->t_data.t_init.t_size.ui2_end_h = (UINT16)pt_data->t_anim_data.u_data.t_size.ui8_end_h;
        pt_anim->t_data.t_init.t_size.ui2_total_steps = pt_data->t_path_mode.ui2_total_steps;
        pt_anim->t_data.t_init.t_size.ui2_frame_interval = pt_data->t_path_mode.ui2_frame_interval;
        if(pt_data->t_anim_data.pf_type_cb == NULL)
        {
            pt_anim->t_data.t_init.t_size.pf_type_cb = pt_data->t_path_mode.pf_type_cb;
        }
        else
        {
            pt_anim->t_data.t_init.t_size.pf_type_cb = pt_data->t_anim_data.pf_type_cb;
        }       
    } 

    if(ui2_type & WGL_ANIM_TYPE_BREATHE)
    {
        pt_anim->t_data.ui2_breathe_idx = ui2_bgn_idx;
        WGL_SET_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_BREATHE);
        
    }  
    
    return WGLR_OK;
}

static INT32 _wgl_anim_attr_hdlr_tick(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT32                          ui4_tick_index)
{
    UINT16                          ui2_bgn_cnt;
    UINT16                          ui2_bgn_indx;
    UINT16                          ui2_count;
    UINT16                          ui2_anim_type;
    INT32                           i4_ret;
    UINT32                          ui4_img_count = 0;
    UINT32                          ui4_alpha_count = 0;
    UINT32                          ui4_move_count = 0;
    GL_POINT_T                      t_move_pos;
    UINT32                          ui4_size_count = 0;
    UINT32                          ui4_scl_count = 0;
    GL_SIZE_T                       t_scale;
    GL_SIZE_T                       t_size;
    WGL_ANIM_IMG_INF_T              t_img;
    UINT8                           ui1_alpha;
    WGL_ANIM_BEGIN_DATA_T*          pt_data = NULL; 
    WGL_ANIM_TICK_INFO*             pt_new_tick;
    WGL_ANIM_TICK_INFO*             pt_old_tick;

    if(pt_anim == NULL || ui4_tick_index< pt_anim->ui4_start_tick)
    {
        return WGLR_INV_ARG;
    }
    
    pt_new_tick = &(pt_anim->t_current_data.t_new_tick);
    pt_old_tick = &(pt_anim->t_current_data.t_old_tick);
    ui2_bgn_cnt = _wgl_anim_chk_new_begin(pt_anim,ui4_tick_index);  
    ui2_count = 0;
    WGL_SET_FLAG(pt_anim->t_current_data.ui2_cur_rsn,WGL_ANIM_TYPE_NONE);

    /*Has new animation type to start.*/
    while(ui2_count < ui2_bgn_cnt)
    {
        ui2_bgn_indx = pt_anim->ui2_bgn_last + ui2_count;
        ui2_anim_type = _wgl_anim_attr_get_anim_type(pt_anim,ui2_bgn_indx,&pt_data);
        if((ui2_anim_type == WGL_ANIM_TYPE_NONE) || (pt_data == NULL))
        {
            ui2_bgn_indx ++;
            ui2_count ++;
            continue;
        }

        if (WGL_IS_FLAG_TURN_OFF(pt_anim->ui2_state, ANIM_STATE_PLAYING))
        {
            return WGLR_INV_ARG;
        }

        _wgl_anim_attr_add_anim_bgn(pt_anim,pt_data,ui2_bgn_indx);

        ui2_bgn_indx ++;
        ui2_count ++;
    }

    pt_anim->ui2_bgn_last = pt_anim->ui2_bgn_cur;

    if (WGL_IS_FLAG_TURN_ON(pt_anim->ui2_state, ANIM_STATE_PLAYING))
    {
        if (WGL_ANIM_TYPE_MOVE & pt_anim->t_data.t_init.ui2_type)
        {
            if (WGL_IS_FLAG_TURN_OFF(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_MOVE))
            { 
                pt_anim->ui4_move_tick_start = ui4_tick_index; 
                WGL_SET_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_MOVE);               
                WGL_SET_FLAG(pt_anim->t_info.ui2_done_type,WGL_ANIM_TYPE_MOVE);
            }   
            i4_ret = _wgl_anim_attr_do_chg_move(
                        pt_anim,
                        ui4_tick_index,
                        &ui4_move_count,
                        &t_move_pos);
             CATCH_FAIL(i4_ret); 
             if(ui4_move_count > 0)
             {
                if(WGL_IS_FLAG_TURN_ON(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_MOVE))
                {
                    x_memcpy(&(pt_old_tick->t_tick_inf.t_point),&(pt_new_tick->t_tick_inf.t_point),sizeof(GL_POINT_T));
                    pt_old_tick->aui4_tick[WGL_ANIM_MOVE] = pt_new_tick->aui4_tick[WGL_ANIM_MOVE];
                    WGL_SET_FLAG(pt_old_tick->ui2_inf_rsn,WGL_ANIM_TYPE_MOVE);
                }
                else
                {
                    WGL_SET_FLAG(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_MOVE);
                }
                
                x_memcpy(&pt_new_tick->t_tick_inf.t_point,&t_move_pos,sizeof(GL_POINT_T));                
                pt_new_tick->aui4_tick[WGL_ANIM_MOVE] = ui4_tick_index;
                WGL_SET_FLAG(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_MOVE);
                WGL_SET_FLAG(pt_anim->t_current_data.ui2_cur_rsn,WGL_ANIM_TYPE_MOVE);
             }
        }
        
        if (WGL_ANIM_TYPE_SCALE & pt_anim->t_data.t_init.ui2_type)
        {
            if (WGL_IS_FLAG_TURN_OFF(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_SCALE))
            { 
                pt_anim->ui4_size_tick_start = ui4_tick_index; 
                WGL_SET_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_SCALE);               
                WGL_SET_FLAG(pt_anim->t_info.ui2_done_type,WGL_ANIM_TYPE_SCALE);
            }   
            i4_ret = _wgl_anim_attr_do_chg_scale(
                        pt_anim,
                        ui4_tick_index,
                        &ui4_scl_count,
                        &t_scale);
             CATCH_FAIL(i4_ret);    
             if(ui4_scl_count > 0)
             {
                if(WGL_IS_FLAG_TURN_ON(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_SCALE))
                {
                    x_memcpy(&(pt_old_tick->t_tick_inf.t_scale),&(pt_new_tick->t_tick_inf.t_scale),sizeof(GL_SIZE_T));
                    pt_old_tick->aui4_tick[WGL_ANIM_SCALE] = pt_new_tick->aui4_tick[WGL_ANIM_SCALE];
                    WGL_SET_FLAG(pt_old_tick->ui2_inf_rsn,WGL_ANIM_TYPE_SCALE);
                }
                else
                {
                    WGL_SET_FLAG(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_SCALE);
                }
                
                x_memcpy(&pt_new_tick->t_tick_inf.t_scale,&t_scale,sizeof(GL_SIZE_T));                
                pt_new_tick->aui4_tick[WGL_ANIM_SCALE] = ui4_tick_index;
                WGL_SET_FLAG(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_SCALE);
                pt_new_tick->t_tick_inf.e_ref_type = pt_anim->t_data.t_init.t_scale.e_ref_type;
                pt_old_tick->t_tick_inf.e_ref_type = pt_anim->t_data.t_init.t_scale.e_ref_type;                
                WGL_SET_FLAG(pt_anim->t_current_data.ui2_cur_rsn,WGL_ANIM_TYPE_SCALE);
             }
             
        }        
        else if (WGL_ANIM_TYPE_SIZE & pt_anim->t_data.t_init.ui2_type)
        {
            if (WGL_IS_FLAG_TURN_OFF(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_SIZE))
            { 
                pt_anim->ui4_size_tick_start = ui4_tick_index; 
                WGL_SET_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_SIZE);               
                WGL_SET_FLAG(pt_anim->t_info.ui2_done_type,WGL_ANIM_TYPE_SIZE);
            }   
            i4_ret = _wgl_anim_attr_do_chg_size(
                        pt_anim,
                        ui4_tick_index,
                        &ui4_size_count,
                        &t_size);
             CATCH_FAIL(i4_ret);    
             if(ui4_size_count > 0)
             {
                if(WGL_IS_FLAG_TURN_ON(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_SIZE))
                {
                    x_memcpy(&(pt_old_tick->t_tick_inf.t_size),&(pt_new_tick->t_tick_inf.t_size),sizeof(GL_SIZE_T));
                    pt_old_tick->aui4_tick[WGL_ANIM_SIZE] = pt_new_tick->aui4_tick[WGL_ANIM_SIZE];
                    WGL_SET_FLAG(pt_old_tick->ui2_inf_rsn,WGL_ANIM_TYPE_SIZE);
                }
                else
                {
                    WGL_SET_FLAG(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_SIZE);
                }
                
                x_memcpy(&pt_new_tick->t_tick_inf.t_size,&t_size,sizeof(GL_SIZE_T));                
                pt_new_tick->aui4_tick[WGL_ANIM_SIZE] = ui4_tick_index;
                WGL_SET_FLAG(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_SIZE);
                WGL_SET_FLAG(pt_anim->t_current_data.ui2_cur_rsn,WGL_ANIM_TYPE_SIZE);
             }
             
        }
        
        if (WGL_ANIM_TYPE_ALPHA & pt_anim->t_data.t_init.ui2_type)
        {
            if (WGL_IS_FLAG_TURN_OFF(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_ALPHA))
            { 
                pt_anim->ui4_alpha_tick_start = ui4_tick_index; 
                WGL_SET_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_ALPHA);                
                WGL_SET_FLAG(pt_anim->t_info.ui2_done_type,WGL_ANIM_TYPE_ALPHA);
            }   
            i4_ret = _wgl_anim_attr_do_chg_alpha(
                        pt_anim,
                        ui4_tick_index,
                        &ui4_alpha_count,
                        &ui1_alpha);
             CATCH_FAIL(i4_ret);  
             if(ui4_alpha_count > 0)
             {
                if(WGL_IS_FLAG_TURN_ON(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_ALPHA))
                {
                    pt_old_tick->t_tick_inf.ui1_alpha = pt_new_tick->t_tick_inf.ui1_alpha;
                    pt_old_tick->aui4_tick[WGL_ANIM_ALPHA] = pt_new_tick->aui4_tick[WGL_ANIM_ALPHA];
                    WGL_SET_FLAG(pt_old_tick->ui2_inf_rsn,WGL_ANIM_TYPE_ALPHA);
                }
                else
                {
                    WGL_SET_FLAG(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_ALPHA);
                }
                
                pt_new_tick->t_tick_inf.ui1_alpha = ui1_alpha;
                pt_new_tick->aui4_tick[WGL_ANIM_ALPHA] = ui4_tick_index;
                WGL_SET_FLAG(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_ALPHA);
                WGL_SET_FLAG(pt_anim->t_current_data.ui2_cur_rsn,WGL_ANIM_TYPE_ALPHA);
             }             
        }
        
        if (WGL_ANIM_TYPE_IMG & pt_anim->t_data.t_init.ui2_type)
        {
           if (WGL_IS_FLAG_TURN_OFF(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_IMG))
           { 
               pt_anim->ui4_img_tick_start = ui4_tick_index; 
               WGL_SET_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_IMG);               
               WGL_SET_FLAG(pt_anim->t_info.ui2_done_type,WGL_ANIM_TYPE_IMG);
           }   
           i4_ret = _wgl_anim_attr_do_chg_img(
                        pt_anim,
                        ui4_tick_index,
                        &ui4_img_count,
                        &t_img);
           CATCH_FAIL(i4_ret); 
           if(ui4_img_count > 0)
           {
              if(WGL_IS_FLAG_TURN_ON(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_IMG))
              {
                  x_memcpy(&(pt_old_tick->t_tick_inf.t_img),&(pt_new_tick->t_tick_inf.t_img),sizeof(WGL_ANIM_IMG_INF_T));
                  pt_old_tick->aui4_tick[WGL_ANIM_IMG] = pt_new_tick->aui4_tick[WGL_ANIM_IMG];
                  WGL_SET_FLAG(pt_old_tick->ui2_inf_rsn,WGL_ANIM_TYPE_IMG);
              }
              else
              {
                  WGL_SET_FLAG(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_IMG);
              }
              
              x_memcpy(&pt_new_tick->t_tick_inf.t_img,&t_img,sizeof(WGL_ANIM_IMG_INF_T));                
              pt_new_tick->aui4_tick[WGL_ANIM_IMG] = ui4_tick_index;
              WGL_SET_FLAG(pt_new_tick->ui2_inf_rsn,WGL_ANIM_TYPE_IMG);
              WGL_SET_FLAG(pt_anim->t_current_data.ui2_cur_rsn,WGL_ANIM_TYPE_IMG);
           }
        } 
        
        pt_anim->t_current_data.ui2_begin_flag = pt_anim->ui2_begin_flag;
        pt_anim->t_current_data.ui2_state = pt_anim->ui2_state;
        pt_anim->t_current_data.ui4_tick_seq = ui4_tick_index;
    }
    else
    {
        WGL_SET_FLAG(pt_anim->t_current_data.ui2_begin_flag,WGL_ANIM_TYPE_NONE);
        pt_anim->t_current_data.ui2_state = 0;
        pt_anim->t_current_data.ui4_tick_seq = ui4_tick_index;
    }

    return WGLR_OK;
}

static INT32 _wgl_anim_attr_set_max_img(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT16                          ui2_index)
{
    WGL_CHECK_NULL(pt_anim,0);

    if((ui2_index == 0) || (pt_anim->ui2_img_seq_max_len != 0) || (pt_anim->pt_img_seq != NULL))
    {
         DBG_ERROR(("{WGL(ANIM)} ERR: Invalid IMG arguments .File=%s,Line=%d\n\n",  __FILE__,__LINE__));
         return WGLR_INV_ARG;
    }

    pt_anim->pt_img_seq = WGL_MEM_ALLOC(sizeof(WGL_ANIM_IMG_SEQ_T)*ui2_index);
    if(pt_anim->pt_img_seq == NULL)
    {
         return WGLR_OUT_OF_MEMORY;
    }        

    x_memset(pt_anim->pt_img_seq,0,sizeof(WGL_ANIM_IMG_SEQ_T)*ui2_index);
    pt_anim->ui2_img_seq_max_len = ui2_index;
    
#ifdef CLI_LVL_ALL    
      pt_anim->ui4_mem_size += (UINT32)(sizeof(WGL_ANIM_IMG_SEQ_T) * ui2_index);
#endif
    return WGLR_OK;
}

static INT32 _wgl_anim_attr_set_img_seq(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT8                           ui1_index,
    const WGL_ANIM_IMG_SEQ_T*       pt_img_set)
{
    WGL_ANIM_IMG_SEQ_T*             pt_img_seq_check;
    UINT32                          ui4_size = 0;    
  
    /*NULL point check of pt_img_set*/
    if (NULL == pt_img_set )
    {
         DBG_ERROR(("{WGL(ANIM)} ERR: Invalid NULL arguments pt_img_set.File=%s,Line=%d\n\n",  __FILE__,__LINE__));
         return WGLR_INV_ARG;
    }
    /*ui1_index check */
    if ((UINT16)ui1_index >= pt_anim->ui2_img_seq_max_len)
    {
        DBG_ERROR(("{WGL(ANIM)} ERR: Invalid arguments ui1_index(%d) >= ui2_max_img_set_len,File=%s,Line=%d\n\n", ui1_index, __FILE__, __LINE__));
        return WGLR_INV_ARG;
    }
    /*check if playing image sequence equals ui1_index*/
    if (WGL_IS_FLAG_TURN_ON(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_IMG)
        && pt_anim->t_data.t_init.t_img.ui1_img_seq_id == ui1_index)
    {
        DBG_ERROR(("{WGL(ANIM)} ERR: ui1_index=%d image sequence is playing now,can't be set.File=%s,Line=%d\n\n", ui1_index, __FILE__, __LINE__));
        return WGLR_INV_STATE;
    }
    
    /*image sequence len check*/
    if (pt_img_set->ui2_len == 0)
    {
        DBG_ERROR(("{WGL(ANIM)} ERR: Invalid arguments pt_img_set->ui2_len=0,File=%s,Line=%d\n\n", __FILE__, __LINE__));
        return WGLR_INV_ARG;
    } 
    /*NULL image sequence temlate check*/
    if (NULL == pt_img_set->pt_img_tpl)
    {
        DBG_ERROR(("{WGL(ANIM)} ERR: Invalid NULL arguments pt_img_set->pt_img_tpl,File=%s,Line=%d\n\n",  __FILE__, __LINE__));
        return WGLR_INV_ARG;
    }  
    pt_img_seq_check = (WGL_ANIM_IMG_SEQ_T*)(pt_anim->pt_img_seq + ui1_index);
    if (pt_img_seq_check->ui2_len != pt_img_set->ui2_len)
    {
        if ((pt_img_seq_check->ui2_len !=0) && (pt_img_seq_check->pt_img_tpl != NULL))
        {
            WGL_MEM_FREE(pt_img_seq_check->pt_img_tpl);
        }
        pt_img_seq_check->ui2_len = pt_img_set->ui2_len;
        ui4_size = (UINT32)(pt_img_set->ui2_len * sizeof(WGL_HIMG_TPL_T*)); 
        pt_img_seq_check->pt_img_tpl = WGL_MEM_ALLOC(ui4_size);
        if(NULL == pt_img_seq_check->pt_img_tpl)
        {
            DBG_ERROR(("{WGL(ANIM)} ERR: Fail to alloc memory for pt_img_seq_check->pt_img_tpl. (%d) File=%s, Line=%d\n\n", WGLR_OUT_OF_MEMORY,__FILE__, __LINE__));
            return WGLR_OUT_OF_MEMORY;
        }
    }
    
#ifdef CLI_LVL_ALL    
      pt_anim->ui4_mem_size += ui4_size;
#endif
    x_memcpy(pt_img_seq_check->pt_img_tpl,pt_img_set->pt_img_tpl,pt_img_set->ui2_len * sizeof(WGL_HIMG_TPL_T*));
    return WGLR_OK;
    
}

static INT32 _wgl_attr_abort_anim(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    VOID*                           pv_param1)
{    
    return WGLR_OK;
}

static INT32 _wgl_attr_disrupt_anim(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT16                          ui2_type)
{
    if (WGL_IS_FLAG_TURN_OFF(pt_anim->ui2_state,ANIM_STATE_PLAYING))
    {
         return WGLR_OK;
    }    
    if (WGL_IS_FLAG_TURN_ON(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_MOVE)
        && WGL_IS_FLAG_TURN_ON(ui2_type,WGL_ANIM_TYPE_MOVE))
    {
        WGL_CLEAR_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_MOVE);
        WGL_CLEAR_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_MOVE);
        x_memset(&(pt_anim->t_data.t_init.t_move),0,sizeof(WGL_MOVE_ANIM_T));        
        pt_anim->t_data.t_init.t_move.pf_type_cb = NULL;
        pt_anim->t_data.ui2_move_step = 0;
        pt_anim->ui4_move_tick_start = 0; 
    }

    if (WGL_IS_FLAG_TURN_ON(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_SIZE)
        && WGL_IS_FLAG_TURN_ON(ui2_type,WGL_ANIM_TYPE_SIZE))
    {
        WGL_CLEAR_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_SIZE);
        WGL_CLEAR_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_SIZE);
        x_memset(&(pt_anim->t_data.t_init.t_size),0,sizeof(WGL_SIZE_ANIM_T));   
        pt_anim->t_data.ui2_size_step = 0;
        pt_anim->ui4_size_tick_start = 0;
        pt_anim->t_data.t_init.t_size.pf_type_cb = NULL;
    }
    
    if (WGL_IS_FLAG_TURN_ON(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_ALPHA)
        && WGL_IS_FLAG_TURN_ON(ui2_type,WGL_ANIM_TYPE_ALPHA))
    {
        WGL_CLEAR_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_ALPHA);
        WGL_CLEAR_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_ALPHA);
        x_memset(&(pt_anim->t_data.t_init.t_alpha),0,sizeof(WGL_ALPHA_ANIM_T));   
        pt_anim->t_data.t_init.t_alpha.pf_type_cb = NULL;
        pt_anim->t_data.ui2_alpha_step = 0;
        pt_anim->ui4_alpha_tick_start = 0;           
    }
  
    if (WGL_IS_FLAG_TURN_ON(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_IMG) && 
        WGL_IS_FLAG_TURN_ON(ui2_type,WGL_ANIM_TYPE_IMG))
    {
        WGL_CLEAR_FLAG(pt_anim->t_data.t_init.ui2_type,WGL_ANIM_TYPE_IMG);  
        WGL_CLEAR_FLAG(pt_anim->ui2_begin_flag,WGL_ANIM_TYPE_IMG);
        pt_anim->t_data.ui2_img_step = 0;
        pt_anim->t_data.ui4_max_step = 0;
        pt_anim->ui4_img_tick_start = 0;  
        pt_anim->ui4_cust_tick = 0;
    }
   
    /*update playing state*/
    if (pt_anim->ui2_begin_flag == 0)
    {
         WGL_UPDATE_STATE(pt_anim->ui2_state, FALSE, ANIM_STATE_PLAYING);
        pt_anim->t_info.e_state = WGL_ANIM_STATE_END;
    }  
    
    return WGLR_OK;
}  

static INT32 _wgl_attr_free_img_cust_data(WGL_ANIM_CONTROL_LOGIC_T*       pt_anim)
{
    UINT16                                  ui2_i;
    WGL_ANIM_BEGIN_DATA_T*                  pt_bgn;
    if(pt_anim == NULL)
    {
        return WGLR_INV_ARG;
    }    

    pt_bgn = pt_anim->pt_begin;
    for(ui2_i = 0; ui2_i < pt_anim->ui2_anim_total; ui2_i ++)
    {
        if(pt_bgn == NULL)
        {
            break;
        }

        if(WGL_IS_FLAG_TURN_ON(pt_bgn->t_anim_data.ui2_anim_type,WGL_ANIM_TYPE_IMG) &&
           (pt_bgn->t_anim_data.u_data.t_img.pt_cust_info != NULL))
        {
            WGL_MEM_FREE(pt_bgn->t_anim_data.u_data.t_img.pt_cust_info);
            pt_bgn->t_anim_data.u_data.t_img.pt_cust_info = NULL;
        }
    }
    
    return WGLR_OK;
}

static INT32 _wgl_attr_end_anim(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim)
{
              
    if((pt_anim == NULL))
    {
         return WGLR_OK;
    } 
    

    if (WGL_IS_FLAG_TURN_ON(pt_anim->t_data.t_init.t_img.e_play_mode,ANIM_PLAY_MODE_CUST) &&
        pt_anim->t_data.t_init.t_img.pt_cust_info != NULL)
    {
        WGL_MEM_FREE(pt_anim->t_data.t_init.t_img.pt_cust_info);
        pt_anim->t_data.t_init.t_img.pt_cust_info = NULL;
    }  

    if(pt_anim->pt_begin != NULL)
    {
        _wgl_attr_free_img_cust_data(pt_anim);
        WGL_MEM_FREE(pt_anim->pt_begin);    
        pt_anim->pt_begin = NULL;
    }
    pt_anim->ui2_anim_total = 0;
    pt_anim->ui2_bgn_cur = 0;
    pt_anim->ui2_bgn_last = 0;
    x_memset(&(pt_anim->t_current_data),0,sizeof(WGL_ANIM_TICK_DATA));
    pt_anim->t_current_data.t_new_tick.t_tick_inf.t_scale.ui4_width = WGL_ANIM_INIT_WIDTH;
    pt_anim->t_current_data.t_new_tick.t_tick_inf.t_scale.ui4_height = WGL_ANIM_INIT_HEIGHT;
    pt_anim->t_current_data.t_old_tick.t_tick_inf.t_scale.ui4_width = WGL_ANIM_INIT_WIDTH;
    pt_anim->t_current_data.t_old_tick.t_tick_inf.t_scale.ui4_height = WGL_ANIM_INIT_HEIGHT;
    
    pt_anim->ui2_begin_flag = 0;
    x_memset(&(pt_anim->t_data),0,sizeof(WGL_ANIM_DATA_T));     
    pt_anim->ui4_size_tick_start = 0; 
    pt_anim->ui4_img_tick_start = 0;  
    pt_anim->ui4_cust_tick = 0;
    pt_anim->ui4_alpha_tick_start = 0;    
    pt_anim->ui4_start_tick = 0;
    WGL_UPDATE_STATE(pt_anim->ui2_state, FALSE, ANIM_STATE_PLAYING);  
    pt_anim->t_info.e_state = WGL_ANIM_STATE_END;
    return WGLR_OK;
}

static INT32 _wgl_attr_get_tick_data(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    WGL_ANIM_TICK_DATA*             pt_tick_data)
{
    WGL_CHECK_NULL(pt_anim,WGLR_INV_ARG);
    if(pt_tick_data == NULL)
    {
        return WGLR_INV_ARG;
    }

    x_memcpy(pt_tick_data,&(pt_anim->t_current_data),sizeof(WGL_ANIM_TICK_DATA));
    return WGLR_OK;
}

static INT32 _wgl_attr_get_current_rgn(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    GL_RECT_T*                      pt_rect)
{
    if((pt_anim == NULL) || (pt_rect == NULL))
    {
        return WGLR_INV_ARG;
    }

    if(RECT_EMPTY(&(pt_anim->t_current_data.t_cur_rect)))
    {
        x_memset(pt_rect,0,sizeof(GL_RECT_T));
    }
    else
    {
        x_memcpy(pt_rect,&(pt_anim->t_current_data.t_cur_rect),sizeof(GL_RECT_T));
    }
        
    return WGLR_OK;
}

static INT32 _wgl_attr_get_anim_state(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    UINT16*                         pui2_state)
{
    if((pt_anim == NULL) || (pui2_state == NULL))
    {
        return WGLR_INV_ARG;
    }

    *pui2_state = pt_anim->ui2_state;
        
    return WGLR_OK;
}

static INT32 _wgl_attr_get_anim_info(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    WGL_ANIM_INF_T*                 pt_info)
{
    if((pt_anim == NULL) || (pt_info == NULL))
    {
        return WGLR_INV_ARG;
    }

    x_memcpy(pt_info, &pt_anim->t_info, sizeof(WGL_ANIM_INF_T));
        
    return WGLR_OK;
}

static INT32 _wgl_attr_get_upd_rgn(
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim,
    GL_RECT_T*                      pt_prv_rect,
    WGL_UPD_REGION_T*               pt_upd)
{
    UINT16                          ui2_flag;
    BOOL                            b_size_or_scl = FALSE;
    GL_POINT_T*                     pt_point;
    GL_SIZE_T*                      pt_size = NULL;
    INT32                           i4_w;
    INT32                           i4_h;
    GL_RECT_T                       t_new_rect;
    GL_RECT_T                       t_old_rect;
    WGL_ANIM_TICK_INFO*             pt_tick;
    UINT32                          ui4_tick = 0;
    UINT32                          ui4_tick_seq;
    
    if((pt_anim == NULL) || (pt_prv_rect == NULL) || (pt_upd == NULL) || RECT_EMPTY(pt_prv_rect))
    {
        return WGLR_INV_ARG;
    }

    if(RECT_EMPTY(&(pt_anim->t_current_data.t_cur_rect)))
    {
    i4_w = RECT_W(pt_prv_rect);
    i4_h = RECT_H(pt_prv_rect);
    }
    else
    {
        i4_w = RECT_W(&(pt_anim->t_current_data.t_cur_rect));
        i4_h = RECT_H(&(pt_anim->t_current_data.t_cur_rect));        
    }
    
    pt_tick = &(pt_anim->t_current_data.t_new_tick);
    x_memset(&t_new_rect,0,sizeof(GL_RECT_T));
    x_memset(pt_upd,0,sizeof(WGL_UPD_REGION_T));  

    ui2_flag = pt_tick->ui2_inf_rsn;
    if(WGL_IS_FLAG_TURN_ON(ui2_flag,WGL_ANIM_TYPE_NONE))
    {
        return WGLR_OK;
    }

    ui4_tick_seq = pt_anim->t_current_data.ui4_tick_seq;

    if(ui4_tick_seq >= (pt_anim->ui4_start_tick + pt_anim->ui4_total_end_tick))
    {
        x_memcpy(&(pt_upd->at_update_rect[pt_upd->ui4_update_rect_num]),
                 &(pt_anim->t_current_data.t_cur_rect),
                 sizeof(GL_RECT_T));
        pt_upd->ui4_update_rect_num ++;
        return WGLR_OK;
    }
    
    if(WGL_IS_FLAG_TURN_ON(ui2_flag,WGL_ANIM_TYPE_SCALE) && 
       WGL_IS_FLAG_TURN_ON(ui2_flag,WGL_ANIM_TYPE_SIZE))
    {
        b_size_or_scl = TRUE;
        pt_size = &(pt_tick->t_tick_inf.t_scale);
        ui4_tick = pt_tick->aui4_tick[WGL_ANIM_SCALE];
        WGL_CLEAR_FLAG(ui2_flag,WGL_ANIM_TYPE_SIZE);
    }
    else if(WGL_IS_FLAG_TURN_ON(ui2_flag,WGL_ANIM_TYPE_SCALE) && 
        WGL_IS_FLAG_TURN_OFF(ui2_flag,WGL_ANIM_TYPE_SIZE))
    {
        b_size_or_scl = TRUE;
        pt_size = &(pt_tick->t_tick_inf.t_scale);
        ui4_tick = pt_tick->aui4_tick[WGL_ANIM_SCALE];
    }
    else if(WGL_IS_FLAG_TURN_OFF(ui2_flag,WGL_ANIM_TYPE_SCALE) && 
        WGL_IS_FLAG_TURN_ON(ui2_flag,WGL_ANIM_TYPE_SIZE))
    {
        b_size_or_scl = TRUE;
        pt_size = &(pt_tick->t_tick_inf.t_size);
        ui4_tick = pt_tick->aui4_tick[WGL_ANIM_SIZE];
    }

    pt_point = &(pt_tick->t_tick_inf.t_point);
    if(WGL_IS_FLAG_TURN_ON(ui2_flag,WGL_ANIM_TYPE_MOVE) && !b_size_or_scl &&
       (ui4_tick_seq <= pt_tick->aui4_tick[WGL_ANIM_MOVE]))
    {
        SET_RECT_BY_SIZE(&t_new_rect,pt_point->i4_x,pt_point->i4_y,i4_w,i4_h);
    }
    else if(WGL_IS_FLAG_TURN_OFF(ui2_flag,WGL_ANIM_TYPE_MOVE) && b_size_or_scl &&
        (ui4_tick_seq <= ui4_tick))
    {
        if (pt_anim->t_current_data.t_new_tick.t_tick_inf.e_ref_type  == ANIM_STD_REF_LEFT_TOP)
        {
        SET_RECT_BY_SIZE(&t_new_rect,
                         pt_prv_rect->i4_left,
                         pt_prv_rect->i4_top,
                         (INT32)pt_size->ui4_width,
                         (INT32)pt_size->ui4_height);
        }
        else
        {
             INT32 i4_width_dis,i4_height_dis;
             i4_width_dis = RECT_W(pt_prv_rect) - pt_size->ui4_width;
             i4_height_dis = RECT_H(pt_prv_rect) - pt_size->ui4_height;
             
             SET_RECT_BY_SIZE(&t_new_rect,
                             pt_prv_rect->i4_left+i4_width_dis/2,
                             pt_prv_rect->i4_top+i4_height_dis/2,
                             (INT32)pt_size->ui4_width,
                             (INT32)pt_size->ui4_height); 
        }
#if 0        
        ui2_old_flag = pt_anim->t_current_data.t_old_tick.ui2_inf_rsn;
        if(WGL_IS_FLAG_TURN_ON(ui2_old_flag,WGL_ANIM_TYPE_SCALE))
        {
            pt_old_size = &(pt_anim->t_current_data.t_old_tick.t_tick_inf.t_scale);
            SET_RECT_BY_SIZE(&t_old_rect,
                             pt_prv_rect->i4_left,
                             pt_prv_rect->i4_top,
                             (INT32)pt_old_size->ui4_width,
                             (INT32)pt_old_size->ui4_height);
        }
        else if(WGL_IS_FLAG_TURN_ON(ui2_old_flag,WGL_ANIM_TYPE_SIZE))
        {
            pt_old_size = & (pt_anim->t_current_data.t_old_tick.t_tick_inf.t_size);
            
            SET_RECT_BY_SIZE(&t_old_rect,
                             pt_prv_rect->i4_left,
                             pt_prv_rect->i4_top,
                             (INT32)pt_old_size->ui4_width,
                             (INT32)pt_old_size->ui4_height);

        }
#endif        
    }
    else if(WGL_IS_FLAG_TURN_ON(ui2_flag,WGL_ANIM_TYPE_MOVE) && b_size_or_scl &&
        ((ui4_tick_seq <= pt_tick->aui4_tick[WGL_ANIM_MOVE]) || (ui4_tick_seq <= ui4_tick)) )
    {
        if (pt_anim->t_current_data.t_new_tick.t_tick_inf.e_ref_type== ANIM_STD_REF_LEFT_TOP)
        {
        SET_RECT_BY_SIZE(&t_new_rect,
                         pt_point->i4_x,
                         pt_point->i4_y,
                         (INT32)pt_size->ui4_width,
                         (INT32)pt_size->ui4_height);
        }
        else
        {
             INT32 i4_width_dis,i4_height_dis;
             i4_width_dis = RECT_W(pt_prv_rect) - pt_size->ui4_width;
             i4_height_dis = RECT_H(pt_prv_rect) - pt_size->ui4_height;
             
             SET_RECT_BY_SIZE(&t_new_rect,
                             pt_point->i4_x+i4_width_dis/2,
                             pt_point->i4_y+i4_height_dis/2,
                             (INT32)pt_size->ui4_width,
                             (INT32)pt_size->ui4_height);     
        }

    }

    if(((WGL_IS_FLAG_TURN_ON(ui2_flag,WGL_ANIM_TYPE_ALPHA) && 
          (ui4_tick_seq <= pt_tick->aui4_tick[WGL_ANIM_ALPHA])) ||
         (WGL_IS_FLAG_TURN_ON(ui2_flag,WGL_ANIM_TYPE_BREATHE) && 
          (ui4_tick_seq <= pt_tick->aui4_tick[WGL_ANIM_BRTH]))  ||
         (WGL_IS_FLAG_TURN_ON(ui2_flag,WGL_ANIM_TYPE_IMG) && 
          (ui4_tick_seq <= pt_tick->aui4_tick[WGL_ANIM_IMG]))) &&
         RECT_EMPTY(&t_new_rect))
    {
        if(RECT_EMPTY(&(pt_anim->t_current_data.t_cur_rect)))
        {
            x_memcpy(&t_new_rect,pt_prv_rect,sizeof(GL_RECT_T));
        }
        else
        {
            x_memcpy(&t_new_rect,&(pt_anim->t_current_data.t_cur_rect),sizeof(GL_RECT_T));
        }    
    } 
    
    if(!RECT_EMPTY(&t_new_rect))
    {
        x_memset(&t_old_rect,0,sizeof(GL_RECT_T));

        if(RECT_EMPTY(&(pt_anim->t_current_data.t_cur_rect)))
        {
            x_memcpy(&t_old_rect,pt_prv_rect,sizeof(GL_RECT_T));
        }
        else
        {
            x_memcpy(&t_old_rect,&(pt_anim->t_current_data.t_cur_rect),sizeof(GL_RECT_T));
        }
        
        x_memcpy(&(pt_anim->t_current_data.t_cur_rect),&t_new_rect,sizeof(GL_RECT_T));

        if(RECT_OVERLAPPED(&t_new_rect,&t_old_rect)||
            RECT_INCLUDED(&t_new_rect,&t_old_rect)||
            RECT_INCLUDED(&t_old_rect,&t_new_rect))
        {
            UNION_RECT(&t_new_rect,&t_new_rect,&t_old_rect); 
        }
        else 
        {
            x_memcpy(&(pt_upd->at_update_rect[pt_upd->ui4_update_rect_num]),
                     &t_old_rect,
                     sizeof(GL_RECT_T));
            pt_upd->ui4_update_rect_num ++;
        }

        x_memcpy(&(pt_upd->at_update_rect[pt_upd->ui4_update_rect_num]),
                 &t_new_rect,
                 sizeof(GL_RECT_T));
        pt_upd->ui4_update_rect_num ++;
    }
    else if(!RECT_EMPTY(&pt_anim->t_current_data.t_cur_rect))
    {
        x_memcpy(&(pt_upd->at_update_rect[pt_upd->ui4_update_rect_num]),
                 &(pt_anim->t_current_data.t_cur_rect),
                 sizeof(GL_RECT_T));
        pt_upd->ui4_update_rect_num ++;
    }

    return WGLR_OK;
}

INT32 wgl_anim_cmd_proc(
    WGL_WIDGET_ANIM_CONTEXT_T   t_anim,
    UINT32                      ui4_cmd_id,
    VOID*                       pv_param1,
    VOID*                       pv_param2)
{
    INT32                       i4_ret ;
    WGL_ANIM_CONTROL_LOGIC_T*   pt_anim = NULL;

    pt_anim = (WGL_ANIM_CONTROL_LOGIC_T*)t_anim;
    if(pt_anim == NULL)
    {
        return WGLR_INV_ARG;
    }
    
    switch (ui4_cmd_id)
    {
    case WGL_CMD_ANIM_ATTR_COND:        
        i4_ret = _wgl_anim_attr_ntf_cond(pt_anim,
                                pt_anim->t_current_data.ui4_tick_seq,
                               (WGL_ANIM_COND_INF_T*)pv_param1);
        break;  
        
    case WGL_CMD_ANIM_ATTR_TICK:  
        i4_ret = _wgl_anim_attr_hdlr_tick(pt_anim,(UINT32)pv_param1);
        break;
        
    case WGL_CMD_ANIM_ATTR_SET_IMG_SEQ:
        i4_ret = _wgl_anim_attr_set_img_seq(pt_anim,WGL_POINTER_TO_UINT8(pv_param1),(WGL_ANIM_IMG_SEQ_T*)pv_param2);        
        break;
        
    case WGL_CMD_ANIM_ATTR_BEGIN:   
        i4_ret = _wgl_attr_begin_anim(pt_anim,pv_param1,(UINT32)pv_param2);   
        break;

    case WGL_CMD_ANIM_ATTR_ABORT: 
        i4_ret = _wgl_attr_abort_anim(pt_anim,pv_param1);
        break;

    case WGL_CMD_ANIM_ATTR_DISRUPT: 
        i4_ret = _wgl_attr_disrupt_anim(pt_anim,WGL_POINTER_TO_UINT16(pv_param1));
        break;

    case WGL_CMD_ANIM_ATTR_END: 
        i4_ret = _wgl_attr_end_anim(pt_anim);
        break;

    case WGL_CMD_ANIM_ATTR_GET_TICK_INF:
        i4_ret = _wgl_attr_get_tick_data(pt_anim,(WGL_ANIM_TICK_DATA*)pv_param2);
        break;

    case WGL_CMD_ANIM_ATTR_GET_UPD_RGN:
        i4_ret = _wgl_attr_get_upd_rgn(pt_anim,(GL_RECT_T*)pv_param1,(WGL_UPD_REGION_T*)pv_param2);
        break;

    case WGL_CMD_ANIM_ATTR_SET_MAX_IMG_SEQ:
        i4_ret = _wgl_anim_attr_set_max_img(pt_anim,WGL_POINTER_TO_UINT16(pv_param1));
        break;

    case WGL_CMD_ANIM_ATTR_GET_CUR_RRGN:
        i4_ret = _wgl_attr_get_current_rgn(pt_anim,(GL_RECT_T*)pv_param1);
        break;
        
    case WGL_CMD_ANIM_ATTR_GET_STATE:
        i4_ret = _wgl_attr_get_anim_state(pt_anim,(UINT16*)pv_param1);
        break;
        
    case WGL_CMD_ANIM_ATTR_GET_INFO:
        i4_ret = _wgl_attr_get_anim_info(pt_anim,(WGL_ANIM_INF_T*)pv_param1);
        break;

    default:
        i4_ret = WGLR_INV_MSG;
        DBG_ERROR(("{WGL(ANIM)} ERR: Unknown message type. (%d) at L%d\n\n", i4_ret, __LINE__));
        break;
    }

    return i4_ret;
}

#endif    /*WGL_ANIM_SUPPORT*/

