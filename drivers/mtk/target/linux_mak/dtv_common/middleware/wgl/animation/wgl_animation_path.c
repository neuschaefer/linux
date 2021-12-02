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
 * $RCSfile: _wgl_animation_path.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Guibiao.Zhou $
 * $MD5HEX: 41da94d098a4312ac008252fdf5e6559 $
 *
 * Description: This file is for the some common algorithm path for animation.
 *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifdef WGL_ANIM_SUPPORT 
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "wgl_animation_path.h"
#include "util/u_lnk_list.h"
#include "x_wgl_animation.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define WGL_ANIM_MAP_MAX_TYPE           ((UINT8)(HT_WGL_WIDGET_END - HT_WGL_WIDGET_START))

static DLIST_PATH_MODE_LINK_T   _t_g_path_mode_q;
static UINT16                   _ui2_anim_path_count;

DLIST_ANIM_MAP_LINK_T   _at_g_anim_map_q[WGL_ANIM_MAP_MAX_TYPE];
static UINT32                   _gui4_wgl_anim_flag = 0;

static VOID* _wgl_anim_lib_new_link(
    WGL_ANIMATION_PATH_MODE_T*          pt_path_mode);

static BOOL _wgl_anim_lib_find_path(
    UINT32                              ui4_path_id,
    WGL_ANIM_PATH_MODE_LINK_T**         ppt_path_link);

static INT32 _anim_path_lib_change_data(
    WGL_ANIMATION_PATH_MODE_T*          pt_path_mode,
    WGL_ANIM_PATH_MODE_LINK_T*          pt_path_link);

static VOID* _wgl_anim_new_actmap(
    WGL_ANIM_ACTION_MAP_T*              pt_actmap);

static BOOL _wgl_anim_find_actmap(
     IN HANDLE_TYPE_T                   e_type_widget,
     IN WGL_ANIM_ACTION_TYPE_T          e_anim_action_type,     
     OUT WGL_ANIM_MAP_NODE_T**          ppt_map_node);


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/



INT16 wgl_anim_cal_percent(INT16 i2_start,INT16 i2_end, INT16 i2_percent)
{
    INT16               i2_tmp = 0;

    i2_tmp = ((i2_end - i2_start) * i2_percent) / 100;
    i2_tmp += i2_start;

    return i2_tmp;
}
#if 1
INT32 wgl_anim_move_path_jump(
                        UINT16                      ui2_cur_step,
                        VOID*                       pt_init, 
                        OUT WGL_ANIM_TICK_T*        pt_tick)
{
    WGL_ANIM_BEGIN_DATA_T*          pt_anim_data;
    INT16                           i2_tmp = 0;
    UINT16                          ui2_total_steps;          

    if(pt_init == NULL || pt_tick == NULL)
    {
        return WGLR_INV_ARG;
    }
    
    pt_anim_data = (WGL_ANIM_BEGIN_DATA_T*)pt_init;
    ui2_total_steps = pt_anim_data->t_path_mode.ui2_total_steps;
    if(WGL_IS_FLAG_TURN_OFF(pt_anim_data->t_anim_data.ui2_anim_type,WGL_ANIM_TYPE_MOVE) ||
       WGL_IS_FLAG_TURN_OFF(pt_anim_data->t_path_mode.ui2_anim_type,WGL_ANIM_TYPE_MOVE) ||
       (ui2_cur_step > ui2_total_steps) )
    {
        return WGLR_INV_ARG;
    }

    

    if(ui2_cur_step == ui2_total_steps)
    {
        i2_tmp = 100;
    }
    else if(ui2_cur_step == 1) 
    {/*85%*/
        i2_tmp = 85;
    }
    else if(ui2_cur_step == 2)
    { /*95%*/
        i2_tmp = 95;
    }
    else if(ui2_cur_step == 3)
    {/*97%*/
        i2_tmp = 97;
    }
    else if(ui2_cur_step == 4)
    {/*98%*/
        i2_tmp = 98;
    }
    else
    {
        i2_tmp = (INT16)(98 + (2 * (ui2_cur_step - 4) / (ui2_total_steps - 4)));
    }

    pt_tick->t_point.i4_x = (INT32)wgl_anim_cal_percent(
                                            pt_anim_data->t_anim_data.u_data.t_move.i2_start_x,
                                            pt_anim_data->t_anim_data.u_data.t_move.i2_end_x,
                                            i2_tmp);
    
    pt_tick->t_point.i4_y = (INT32)wgl_anim_cal_percent(
                                           pt_anim_data->t_anim_data.u_data.t_move.i2_start_y,
                                           pt_anim_data->t_anim_data.u_data.t_move.i2_end_y,
                                           i2_tmp);   
    
    return WGLR_OK;
}
#else
INT32 wgl_anim_move_path_jump(
                        UINT16                  ui2_cur_step,
                        WGL_MOVE_ANIM_T*        pt_init,
                        OUT GL_POINT_T*         pt_point)
{
    INT16               i2_tmp = 0;

    if((pt_init == NULL) || (pt_point == NULL) || (ui2_cur_step > pt_init->ui2_total_steps))
    {
        return WGLR_INV_ARG;
    }

    if(ui2_cur_step == pt_init->ui2_total_steps)
    {
        i2_tmp = 100;
    }
    else if(ui2_cur_step == 1) 
    {/*85%*/
        i2_tmp = 85;
    }
    else if(ui2_cur_step == 2)
    { /*95%*/
        i2_tmp = 95;
    }
    else if(ui2_cur_step == 3)
    {/*97%*/
        i2_tmp = 97;
    }
    else if(ui2_cur_step == 4)
    {/*98%*/
        i2_tmp = 98;
    }
    else
    {
        i2_tmp = (INT16)(98 + (2 * (ui2_cur_step - 4) / (pt_init->ui2_total_steps - 4)));
    }
    
    pt_point->i4_x = (INT32)wgl_anim_cal_percent(pt_init->i2_start_x,pt_init->i2_end_x,i2_tmp);
    pt_point->i4_y = (INT32)wgl_anim_cal_percent(pt_init->i2_start_y,pt_init->i2_end_y,i2_tmp);   
    
    return WGLR_OK;
}
#endif

static VOID* _wgl_anim_lib_new_link(
    WGL_ANIMATION_PATH_MODE_T*          pt_path_mode)    
{
    WGL_ANIM_PATH_MODE_LINK_T*          pt_link;
    WGL_ANIM_PATH_DATA_T*               pt_path;
    UINT32                              ui4_size;
    
    if((pt_path_mode == NULL) || (pt_path_mode->pt_path_inf == NULL) || 
       (pt_path_mode->ui2_count == 0))
    {
        return NULL;
    }

    pt_link =(WGL_ANIM_PATH_MODE_LINK_T*)WGL_MEM_ALLOC(sizeof(WGL_ANIM_PATH_MODE_LINK_T));
    if(pt_link != NULL) 
    {
        x_memset(pt_link,0,sizeof(WGL_ANIM_PATH_MODE_LINK_T));

        ui4_size = sizeof(WGL_ANIM_PATH_DATA_T) * pt_path_mode->ui2_count;
        pt_path=(WGL_ANIM_PATH_DATA_T*)WGL_MEM_ALLOC(ui4_size);
        if(pt_path == NULL)
        {
            WGL_MEM_FREE(pt_link);
            return NULL;
        }
        x_memset(pt_path,0,ui4_size);
        
        pt_link->t_path.pt_path_inf = pt_path;
        pt_link->t_path.ui4_path_id = pt_path_mode->ui4_path_id;
        pt_link->t_path.ui2_count = pt_path_mode->ui2_count;
        x_memcpy(pt_path, pt_path_mode->pt_path_inf, ui4_size);
    }

    return pt_link;
}


static BOOL _wgl_anim_lib_find_path(
    UINT32                              ui4_path_id,
    WGL_ANIM_PATH_MODE_LINK_T**         ppt_path_link)
{
    WGL_ANIM_PATH_MODE_LINK_T*          pt_tmp_link;

    if(ui4_path_id == WGL_ANIM_NULL_PAHT_ID)
    {
        return FALSE;
    }

    if(ppt_path_link != NULL)
    {
        *ppt_path_link = NULL;
    } 
    
    DLIST_FOR_EACH(pt_tmp_link, &_t_g_path_mode_q, t_link)
    {
        if(pt_tmp_link == NULL)
        {
            break;
        }
        
        if(pt_tmp_link->t_path.ui4_path_id == ui4_path_id)
        {
            if(ppt_path_link != NULL)
            {
                *ppt_path_link = pt_tmp_link;
            }        
            return TRUE;
        }
    }

    return FALSE;
}

static INT32 _anim_path_lib_change_data(
    WGL_ANIMATION_PATH_MODE_T*          pt_path_mode,
    WGL_ANIM_PATH_MODE_LINK_T*          pt_path_link)
{
    WGL_ANIM_PATH_DATA_T*           pt_path_inf;
    UINT32                          ui4_size;
    
    if((pt_path_mode == NULL) || (pt_path_link == NULL) || 
       (pt_path_mode->ui4_path_id == WGL_ANIM_NULL_PAHT_ID))
    {
        return WGLR_INV_ARG;
    }

    ui4_size = sizeof(WGL_ANIM_PATH_DATA_T) * pt_path_mode->ui2_count;
    if(pt_path_link->t_path.ui2_count != pt_path_mode->ui2_count)
    {
        WGL_MEM_FREE(pt_path_link->t_path.pt_path_inf);
        pt_path_inf = (WGL_ANIM_PATH_DATA_T*)WGL_MEM_ALLOC(ui4_size);
        if(pt_path_inf == NULL)
        {
            return WGLR_OUT_OF_MEMORY;
        }
        pt_path_link->t_path.pt_path_inf = pt_path_inf;
    }
    else
    {
        pt_path_inf = pt_path_link->t_path.pt_path_inf;
    }

    x_memcpy(pt_path_inf,pt_path_mode->pt_path_inf,ui4_size);
    pt_path_link->t_path.ui2_count = pt_path_mode->ui2_count;
    pt_path_link->t_path.ui4_path_id = pt_path_mode->ui4_path_id;
    
    return WGLR_OK;
}

INT32 wgl_anim_path_init(VOID)
{
    _ui2_anim_path_count = 0;
    DLIST_INIT(&_t_g_path_mode_q);

    return WGLR_OK;
}

INT32 wgl_anim_lib_add_path(
        IN WGL_ANIMATION_PATH_MODE_T*     pt_path_mode,
        IN UINT16                         ui2_path_cnt,
        OUT UINT16*                       pui2_added_cnt)
{
    UINT16                           ui2_count;
    WGL_ANIMATION_PATH_MODE_T*       pt_path_tmp;
    WGL_ANIM_PATH_MODE_LINK_T*       pt_path_link = NULL;
    INT32                            i4_ret = WGLR_OK;
    

    if(pt_path_mode == NULL || ui2_path_cnt == 0)
    {
        return WGLR_INV_ARG;
    }
    
    pt_path_tmp = pt_path_mode;
    
    for(ui2_count = 0; ui2_count < ui2_path_cnt; ui2_count ++)
    {
        if(pt_path_tmp == NULL)
        {
            break;
        }

        if(TRUE == _wgl_anim_lib_find_path(pt_path_tmp->ui4_path_id,&pt_path_link))
        {
            i4_ret = _anim_path_lib_change_data(pt_path_tmp,pt_path_link);
            if(i4_ret != WGLR_OK)
            {
                break;
            }
        }
        else
        {
            pt_path_link = _wgl_anim_lib_new_link(pt_path_tmp);
            if(pt_path_link == NULL)
            {
                if(pui2_added_cnt != NULL)
                {
                    *pui2_added_cnt = ui2_count;
                }
                return WGLR_INTERNAL_ERROR;
            }
            DLIST_INSERT_TAIL(pt_path_link,&_t_g_path_mode_q,t_link);  
            _ui2_anim_path_count ++;
        }

        pt_path_tmp ++;
    }

    if(pui2_added_cnt != NULL)
    {
        *pui2_added_cnt = ui2_count;
    }   
    
    return i4_ret;
}

INT32 wgl_anim_lib_remove_path(
        IN UINT32*                        pui4_path_id,
        IN UINT16                         ui2_path_cnt)
{
    UINT32*                                 pui4_id;
    UINT16                                  ui2_cnt;
    WGL_ANIM_PATH_MODE_LINK_T*              pt_path_link;

    if(pui4_path_id == NULL)
    {
        return WGLR_INV_ARG;
    }

    pui4_id = pui4_path_id;
    for(ui2_cnt = 0; ui2_cnt < ui2_path_cnt; ui2_cnt ++)
    {
        if(pui4_id == NULL)
        {
            return WGLR_INV_ARG;
        }
        
        if((TRUE == _wgl_anim_lib_find_path(*pui4_id,&pt_path_link))&&
           (pt_path_link != NULL))
        {
            DLIST_REMOVE(pt_path_link,&_t_g_path_mode_q,t_link);    
            _ui2_anim_path_count --;
            
            if(pt_path_link->t_path.pt_path_inf != NULL)
            {
                WGL_MEM_FREE(pt_path_link->t_path.pt_path_inf);
            }
            WGL_MEM_FREE(pt_path_link);
        }
        
        pui4_id ++;
    }
    
    return WGLR_OK;
}


BOOL wgl_anim_lib_get_path(
        IN  UINT32                         ui4_path_id,
        OUT WGL_ANIMATION_PATH_MODE_T*     pt_path_mode)
{
    WGL_ANIM_PATH_MODE_LINK_T*              pt_path_link;
    BOOL                                    b_found;

    if(pt_path_mode == NULL)
    {
        return FALSE;
    }
    
    if(TRUE == _wgl_anim_lib_find_path(ui4_path_id,&pt_path_link))
    {
        if(pt_path_mode != NULL)
        {
            x_memcpy(pt_path_mode,&pt_path_link->t_path,sizeof(WGL_ANIMATION_PATH_MODE_T));
        }
        
        b_found = TRUE;
    }
    else
    {
        b_found = FALSE;
    }

    return b_found;
}

INT32 wgl_anim_lib_get_path_cnt(
        OUT UINT32*                        pui4_count)
{
    if(pui4_count != NULL)
    {
        *pui4_count = _ui2_anim_path_count;
    }

    return WGLR_OK;
}

/*widget animation default action map function*/
static VOID* _wgl_anim_new_actmap(
    WGL_ANIM_ACTION_MAP_T*              pt_actmap)
{
    WGL_ANIM_MAP_NODE_T*                pt_link = NULL;
    WGL_ANIM_PATH_DATA_T*               pt_mode = NULL;  
    UINT32                              ui4_size;
    BOOL                                b_set = FALSE;
    
    if((pt_actmap == NULL) || (pt_actmap->e_anim_action_type > WGL_ANIM_ACTION_END) ||
        ((WGL_IS_FLAG_TURN_OFF(pt_actmap->t_anim_data.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_ID_INF)) &&
         (WGL_IS_FLAG_TURN_OFF(pt_actmap->t_anim_data.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF))))
    {
        return NULL;
    }

    pt_link =(WGL_ANIM_MAP_NODE_T*)WGL_MEM_ALLOC(sizeof(WGL_ANIM_MAP_NODE_T));
    if(pt_link != NULL) 
    {
        x_memset(pt_link,0,sizeof(WGL_ANIM_MAP_NODE_T));

        if(WGL_IS_FLAG_TURN_ON(pt_actmap->t_anim_data.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_ID_INF) &&
           (pt_actmap->t_anim_data.t_path_id_inf.ui2_path_cnt > 0 ))
        {
            x_memcpy(&pt_link->t_map.t_anim_data.t_path_id_inf,
                     &pt_actmap->t_anim_data.t_path_id_inf,
                     sizeof(WGL_ANIM_PATH_ID_INFO_T));
            b_set = TRUE;
        }

        if(WGL_IS_FLAG_TURN_ON(pt_actmap->t_anim_data.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF) &&
           (pt_actmap->t_anim_data.ui2_count > 0 ))
        {
            ui4_size = (UINT32)(sizeof(WGL_ANIM_PATH_DATA_T) * pt_actmap->t_anim_data.ui2_count);
            pt_mode =(WGL_ANIM_PATH_DATA_T*)WGL_MEM_ALLOC(ui4_size);
            if(pt_mode != NULL)
            {
                x_memset(pt_mode,0,ui4_size);
                x_memcpy(pt_mode,pt_actmap->t_anim_data.pt_path_mode,ui4_size);
                pt_link->t_map.t_anim_data.pt_path_mode = pt_mode;
                b_set = TRUE;
            }
        }        

        if(b_set == TRUE)
        {
        pt_link->t_map.e_anim_action_type = pt_actmap->e_anim_action_type;
        pt_link->t_map.t_anim_context = pt_actmap->t_anim_context;
            pt_link->t_map.t_anim_data.ui4_flag = pt_actmap->t_anim_data.ui4_flag;
            pt_link->t_map.t_anim_data.ui2_count = pt_actmap->t_anim_data.ui2_count;
        }
        else
        {
            WGL_MEM_FREE(pt_link);
            pt_link = NULL;
        }
    }

    return pt_link;
}

static BOOL _wgl_anim_find_actmap(
     IN HANDLE_TYPE_T                   e_type_widget,
     IN WGL_ANIM_ACTION_TYPE_T          e_anim_action_type,     
     OUT WGL_ANIM_MAP_NODE_T**          ppt_map_node)
{
    WGL_ANIM_MAP_NODE_T*                pt_node;
    UINT8                               ui1_indx;

    if((e_type_widget >= HT_WGL_WIDGET_END) ||
       (e_type_widget < HT_WGL_WIDGET_START) ||
       (e_anim_action_type >= WGL_ANIM_ACTION_END))
    {
        return FALSE;
    }

    ui1_indx = (UINT8)(e_type_widget - HT_WGL_WIDGET_START);
    if(ui1_indx > WGL_ANIM_MAP_MAX_TYPE)
    {
        return FALSE;
    }

    if(ppt_map_node != NULL)
    {
        *ppt_map_node = NULL;
    }
            
    DLIST_FOR_EACH(pt_node, &_at_g_anim_map_q[ui1_indx], t_link)
    {
        if(pt_node == NULL)
        {
            break;
        }
        
        if(pt_node->t_map.e_anim_action_type == e_anim_action_type)
        {
            if(ppt_map_node != NULL)
            {
                *ppt_map_node = pt_node;
            }        
            return TRUE;
        }
    }

    return FALSE;
}

static INT32 _anim_actmap_change_data(
    WGL_ANIM_ACTION_MAP_T*              pt_action_map,
    WGL_ANIM_MAP_NODE_T*                pt_path_link)
{
    if((pt_action_map == NULL) || (pt_path_link == NULL) )
    {
        return WGLR_INV_ARG;
    }
    
    x_memcpy(&(pt_path_link->t_map.t_anim_data.t_path_id_inf),
             &(pt_action_map->t_anim_data.t_path_id_inf),
             sizeof(WGL_ANIM_PATH_ID_INFO_T));
    
    return WGLR_OK;
}

INT32 wgl_anim_actmap_init(VOID)
{
    UINT8                                   ui1_count;

    for(ui1_count = 0; ui1_count < WGL_ANIM_MAP_MAX_TYPE; ui1_count ++)
    {
        DLIST_INIT(&_at_g_anim_map_q[ui1_count]);
    }
    
    return WGLR_OK;
}

INT32 wgl_anim_add_default_actmap(
     IN HANDLE_TYPE_T                   e_type_widget,
     IN WGL_ANIM_ACTION_MAP_T*          pt_action_map,
     IN UINT16                          ui2_count)
{
    UINT16                           ui2_i;
    WGL_ANIM_ACTION_MAP_T*           pt_map_tmp;
    WGL_ANIM_MAP_NODE_T*             pt_link = NULL;
    UINT8                               ui1_indx;
   // INT32                            i4_ret = WGLR_OK;

    if((e_type_widget >= HT_WGL_WIDGET_END) ||
       (e_type_widget < HT_WGL_WIDGET_START) ||
       (pt_action_map == NULL) ||
       (ui2_count == 0))
    {
        return WGLR_INV_ARG;
    }
    
    ui1_indx = (UINT8)(e_type_widget - HT_WGL_WIDGET_START);
    if(ui1_indx > WGL_ANIM_MAP_MAX_TYPE)
    {
        return FALSE;
    }
    
    pt_map_tmp = pt_action_map;
    for(ui2_i = 0; ui2_i < ui2_count; ui2_i ++)
    {
        if(pt_map_tmp == NULL)
        {
            break;
        }

        if(TRUE == _wgl_anim_find_actmap(e_type_widget,pt_map_tmp->e_anim_action_type,&pt_link))
        {
            _anim_actmap_change_data(pt_map_tmp,pt_link);
        }
        else
        {
            pt_link = _wgl_anim_new_actmap(pt_map_tmp);
            if(pt_link == NULL)
            {

                return WGLR_INTERNAL_ERROR;
            }
            DLIST_INSERT_TAIL(pt_link,&_at_g_anim_map_q[ui1_indx],t_link);  
        }
        pt_map_tmp ++;
    }
    
    return WGLR_OK;
}


INT32 wgl_anim_remove_default_actmap(
    IN HANDLE_TYPE_T                   e_type_widget,
    IN BOOL                            b_remove_all,
    IN WGL_ANIM_ACTION_TYPE_T          e_anim_action_type)
{
    WGL_ANIM_MAP_NODE_T*                pt_node;
    UINT8                               ui1_indx;
    WGL_ANIM_ACTION_TYPE_T              e_action_type;
    WGL_ANIM_ACTION_TYPE_T              e_action_s;
    WGL_ANIM_ACTION_TYPE_T              e_action_e;

    if((e_type_widget >= HT_WGL_WIDGET_END) ||
       (e_type_widget < HT_WGL_WIDGET_START) ||
       (e_anim_action_type >= WGL_ANIM_ACTION_END))
    {
        return FALSE;
    }

    ui1_indx = (UINT8)(e_type_widget - HT_WGL_WIDGET_START);
    if(ui1_indx > WGL_ANIM_MAP_MAX_TYPE)
    {
        return FALSE;
    }

    if(b_remove_all == TRUE)
    {
        e_action_s = WGL_ANIM_ACTION_FOCUS;
        e_action_e = WGL_ANIM_ACTION_END;
    }
    else
    {
        e_action_s = e_anim_action_type;
        e_action_e = e_anim_action_type;
    }
    
    for(e_action_type = e_action_s; e_action_type <= e_action_e; e_action_type ++)
    {
        if(TRUE == _wgl_anim_find_actmap(e_type_widget,e_action_type,&pt_node) &&
       (pt_node != NULL))
    {
        DLIST_REMOVE(pt_node,&_at_g_anim_map_q[ui1_indx],t_link);    
            WGL_MEM_FREE(pt_node);
        }
    }
   
    return WGLR_OK;
}

INT32 wgl_anim_get_default_actmap(
  IN  HANDLE_TYPE_T                  e_type_widget,
  IN  WGL_ANIM_ACTION_TYPE_T         e_anim_action_type,
  OUT WGL_ANIM_ACTION_MAP_T**        ppt_action_map)
{
    WGL_ANIM_MAP_NODE_T*                pt_node;
    UINT8                               ui1_indx;

    if((e_type_widget >= HT_WGL_WIDGET_END) ||
       (e_type_widget < HT_WGL_WIDGET_START) ||
       (e_anim_action_type >= WGL_ANIM_ACTION_END) ||
       (ppt_action_map == NULL))
    {
        return WGLR_INV_ARG;
    }

    ui1_indx = (UINT8)(e_type_widget - HT_WGL_WIDGET_START);
    if(ui1_indx > WGL_ANIM_MAP_MAX_TYPE)
    {
        return WGLR_INV_ARG;
    }

    if(TRUE == _wgl_anim_find_actmap(e_type_widget,e_anim_action_type,&pt_node) &&
       (pt_node != NULL))
    {
        *ppt_action_map = &(pt_node->t_map);
    }

    return WGLR_OK;
}

INT32 wgl_anim_free_map_tbl(
    WGL_ANIM_ACTION_MAP_T*                  pt_actmap,
    WGL_ANIM_ACTION_TYPE_T                  e_from_type,
    WGL_ANIM_ACTION_TYPE_T                  e_last_type)
    {
    UINT8                                   ui1_indx;

    if (pt_actmap == NULL)
    {
        return WGLR_INV_ARG;
    }

    for(ui1_indx = (UINT8)e_from_type; ui1_indx <= (UINT8)e_last_type; ui1_indx++)
    {
        pt_actmap[ui1_indx].e_anim_action_type = WGL_ANIM_ACTION_INGORE;
        x_memset(&pt_actmap[ui1_indx].t_anim_data,0,sizeof(WGL_ANIMATION_DATA_T));
    }
    
    return WGLR_OK;
}

INT32 wgl_anim_get_default_map_tbl(
    HANDLE_TYPE_T                           e_type_widget,
    WGL_ANIM_ACTION_MAP_T*                  pt_actmap,
    WGL_ANIM_ACTION_TYPE_T                  e_from_type,
    WGL_ANIM_ACTION_TYPE_T                  e_last_type)
{
    UINT8                               ui1_wgl_idx;
    WGL_ANIM_MAP_NODE_T*                pt_node;
    UINT8                               ui1_indx;
    
    if((e_type_widget >= HT_WGL_WIDGET_END) || 
       (e_type_widget < HT_WGL_WIDGET_START) ||
       (pt_actmap == NULL))
    {
        return WGLR_INV_ARG;
    }

    ui1_wgl_idx = (UINT8)(e_type_widget - HT_WGL_WIDGET_START);
    if(ui1_wgl_idx > WGL_ANIM_MAP_MAX_TYPE)
    {
        return WGLR_INV_ARG;
    }
    
    DLIST_FOR_EACH(pt_node, &_at_g_anim_map_q[ui1_wgl_idx], t_link)
    {
        if(pt_node == NULL)
        {
            break;
        }
        
        if(pt_node->t_map.e_anim_action_type <= e_last_type &&
           pt_node->t_map.e_anim_action_type >= e_from_type )
        {
            ui1_indx = (UINT8)pt_node->t_map.e_anim_action_type;
            x_memcpy(&pt_actmap[ui1_indx],&pt_node->t_map,sizeof(WGL_ANIM_ACTION_MAP_T));
        }
    }
    
    return WGLR_OK;
}

BOOL wgl_anim_has_dft_map_tbl(
    HANDLE_TYPE_T                           e_type_widget,
    WGL_ANIM_ACTION_TYPE_T                  e_from_type,
    WGL_ANIM_ACTION_TYPE_T                  e_last_type)
{
    UINT8                               ui1_wgl_idx;
    WGL_ANIM_MAP_NODE_T*                pt_node;
    
    if((e_type_widget >= HT_WGL_WIDGET_END) || 
       (e_type_widget < HT_WGL_WIDGET_START))
    {
        return FALSE;
    }

    ui1_wgl_idx = (UINT8)(e_type_widget - HT_WGL_WIDGET_START);
    if(ui1_wgl_idx > WGL_ANIM_MAP_MAX_TYPE)
    {
        return FALSE;
    }
    
    DLIST_FOR_EACH(pt_node, &_at_g_anim_map_q[ui1_wgl_idx], t_link)
    {
        if(pt_node != NULL &&
           pt_node->t_map.e_anim_action_type <= e_last_type &&
           pt_node->t_map.e_anim_action_type >= e_from_type )
        {
            return TRUE;
        }
    }
    
    return FALSE;
}


/*wgl animation path lib api function*/
INT32 x_wgl_anim_path_init(VOID)
{
    return wgl_anim_path_init();
}

INT32 x_wgl_anim_lib_add_path(
    IN WGL_ANIMATION_PATH_MODE_T*     pt_path_mode,
    IN UINT16                         ui2_path_cnt,
    OUT UINT16*                       pui2_added_cnt)
{
    return wgl_anim_lib_add_path(pt_path_mode,ui2_path_cnt,pui2_added_cnt);
}

INT32 x_wgl_anim_lib_remove_path(
    IN UINT32*                        pui4_path_id,
    IN UINT16                         ui2_path_cnt)
{
    return wgl_anim_lib_remove_path(pui4_path_id,ui2_path_cnt);
}

BOOL x_wgl_anim_lib_get_path(
    IN  UINT32                         ui4_path_id,
    OUT WGL_ANIMATION_PATH_MODE_T*     pt_path_mode)
{
    return wgl_anim_lib_get_path(ui4_path_id,pt_path_mode);
}

INT32 x_wgl_anim_lib_get_path_cnt(
        OUT UINT32*                        pui4_count)
{
    return wgl_anim_lib_get_path_cnt(pui4_count);
}

INT32 x_wgl_anim_add_default_actmap(
     IN HANDLE_TYPE_T                       e_type_widget,
     IN WGL_ANIM_ACTION_MAP_T*              pt_action_map,
     IN UINT16                              ui2_count)
{
    return wgl_anim_add_default_actmap(e_type_widget,pt_action_map,ui2_count);
}

INT32 x_wgl_anim_remove_default_actmap(
    IN HANDLE_TYPE_T                        e_type_widget,
    IN BOOL                                 b_remove_all,
    IN WGL_ANIM_ACTION_TYPE_T               e_anim_action_type)
{
    return wgl_anim_remove_default_actmap(e_type_widget,b_remove_all,e_anim_action_type);
}

INT32 x_wgl_anim_get_default_actmap(
  IN  HANDLE_TYPE_T                         e_type_widget,
  IN  WGL_ANIM_ACTION_TYPE_T                e_anim_action_type,
  OUT WGL_ANIM_ACTION_MAP_T**               ppt_action_map)
{
    return wgl_anim_get_default_actmap(e_type_widget,e_anim_action_type,ppt_action_map);
}

INT32 x_wgl_set_lb_default_anim(BOOL b_flag)
{
    if(b_flag == TRUE)
    {
        WGL_SET_FLAG(_gui4_wgl_anim_flag,(UINT32)(1 << (HT_WGL_WIDGET_LIST_BOX - HT_WGL_WIDGET_START)));
    }
    else
    {
        WGL_CLEAR_FLAG(_gui4_wgl_anim_flag,(UINT32)(1 << (HT_WGL_WIDGET_LIST_BOX - HT_WGL_WIDGET_START)));
    }

    return WGLR_OK;
}

BOOL x_wgl_lb_default_anim_en(VOID)
{
    BOOL                    b_flag = FALSE;
    
    if(WGL_IS_FLAG_TURN_ON(_gui4_wgl_anim_flag,(UINT32)(1 << (HT_WGL_WIDGET_LIST_BOX - HT_WGL_WIDGET_START))))
{
        b_flag = TRUE;
}

    return b_flag;
}


#endif

