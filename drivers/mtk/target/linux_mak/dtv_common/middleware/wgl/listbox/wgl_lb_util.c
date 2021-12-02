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
 * $RCSfile: wgl_lb_util.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "wgl/listbox/wgl_lb_util.h"
#include "wgl/wgl_bdr_tools.h"
#include "unicode/x_uc_str.h"

/*****************************************************************************
            Utilities
 *****************************************************************************/
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    public functions
 ----------------------------------------------------------------------------*/
VOID lb_util_w2s_strncpy_wo_ch (
    UTF16_T*        w2s_dst,
    const UTF16_T*  w2s_src,
    UINT32          ui4_len,
    UTF16_T         w2_wo_ch)
{
    UINT32      ui4_src, ui4_dst;
    UTF16_T     w2_ch;
    
    ui4_dst = 0;
    for (ui4_src = 0; ui4_src < ui4_len; ui4_src ++)
    {
        w2_ch = x_uc_w2s_get (w2s_src, ui4_src);
        
        if (w2_ch == ((UTF16_T) 0))
        {
            break;
        }
        
        if (w2_ch != w2_wo_ch)
        {
            x_uc_w2s_set (w2s_dst, ui4_dst, w2_ch);
            ui4_dst ++ ;
        }
    }
    
    x_uc_w2s_set (w2s_dst, ui4_dst, ((UTF16_T) 0));
}
    
INT32 lb_elem_inf_init(
    LB_ELEM_INF_T*                           pt_elem_inf)
{
    if(pt_elem_inf == NULL)
    {
        return WGLR_INV_ARG;
    }

    pt_elem_inf->ui1_state = 0; /*setup as the normal state*/

    #ifdef WGL_ANIM_SUPPORT
    pt_elem_inf->ui4_ani_style = WGL_LB_EA_STL_ONLY_ANIM_DATA;
    #endif

    return WGLR_OK;
}

INT32 lb_init_elem_inf(
    DLIST_LB_ELEM_INF_T*                pt_list, 
    WGL_NPM_CTX_T*                      pt_npm, 
    UINT16                              ui2_num)
{
    INT32                               i4_ret = WGLR_OK;
    UINT16                              ui2_i;
    LB_ELEM_INF_T*                      pt_elem_inf = NULL;

   CHECK_VALID(pt_list && pt_npm, WGLR_INV_ARG);

    /*initial element information list*/
    DLIST_INIT(pt_list);

    for(ui2_i=0; ui2_i<ui2_num; ui2_i++)
    {
        /*alloc one element information and initial*/
        i4_ret = wgl_npm_alloc(pt_npm, (VOID **)&pt_elem_inf);
        if(WGL_IS_FAIL(i4_ret))
        {
            INFORM_ERR(i4_ret);
            wgl_npm_deinit(pt_npm);
            return i4_ret;
        }
        lb_elem_inf_init(pt_elem_inf);

        /*Add to element information list*/
        DLIST_INSERT_TAIL(pt_elem_inf, pt_list, t_link);
    }

    return WGLR_OK;
} 

INT32 lb_deinit_elem_inf(
    DLIST_LB_ELEM_INF_T*                pt_list, 
    WGL_NPM_CTX_T*                      pt_npm)
{
    DLIST_INIT(pt_list);
    wgl_npm_deinit(pt_npm);

    return WGLR_OK;
} 

INT32 lb_get_elem_inf(
    DLIST_LB_ELEM_INF_T*                pt_list, 
    UINT16                              ui2_max_num,
    UINT16                              ui2_idx,
    LB_ELEM_INF_T**                     ppt_inf)
{
    LB_ELEM_INF_T*                      pt_elem_inf;
    UINT16                              ui2_i = 0;

    CHECK_VALID((pt_list && ppt_inf && ui2_idx < ui2_max_num), WGLR_INV_ARG);
    *ppt_inf = NULL;

    DLIST_FOR_EACH( pt_elem_inf, pt_list, t_link )
    {
        if(ui2_i == ui2_idx)
        {
            *ppt_inf = pt_elem_inf;
        }
        
        ui2_i ++;
    }

    return WGLR_OK;
}

INT32 lb_add_elem_inf(
    DLIST_LB_ELEM_INF_T*                pt_list, 
    UINT16*                             pui2_elem_num,
    WGL_NPM_CTX_T*                      pt_npm, 
    UINT16                              ui2_idx,
    LB_ELEM_INF_T*                      pt_elem_inf)
{
    INT32                               i4_ret = WGLR_OK;
    LB_ELEM_INF_T*                      pt_cur = NULL;
    LB_ELEM_INF_T*                      pt_inf = NULL;

    CHECK_VALID(pt_list && pt_npm && pui2_elem_num, WGLR_INV_ARG);

    if(*pui2_elem_num == 0 )
    {
        if(!DLIST_IS_EMPTY(pt_list))
        {
            return WGLR_INTERNAL_ERROR;
        }
    }
    else
    {   
        if(ui2_idx > *pui2_elem_num )
        {
            ui2_idx = *pui2_elem_num;
        }

        if(ui2_idx == 0)
        {
            lb_get_elem_inf(pt_list, *pui2_elem_num, 0, &pt_inf);
        }
        else
        {
            lb_get_elem_inf(pt_list, *pui2_elem_num,(UINT16)(ui2_idx - 1), &pt_inf);
        }

        if(pt_inf == NULL)
        {
            return WGLR_INV_ARG;
        }
    }

    /*alloc one element information and initial*/
    i4_ret = wgl_npm_alloc(pt_npm,(VOID **) &pt_cur);
    if(WGL_IS_FAIL(i4_ret))
    {
        return i4_ret;
    }

    if(pt_elem_inf == NULL)
    {
        lb_elem_inf_init(pt_cur);
    }
    else
    {
        pt_cur->ui1_state = pt_elem_inf->ui1_state;

        #ifdef WGL_ANIM_SUPPORT
        pt_cur->ui4_ani_style = pt_elem_inf->ui4_ani_style;
        #endif
    }

    if((*pui2_elem_num) == 0 )
    {
        DLIST_INSERT_TAIL(pt_cur, pt_list, t_link);
    }
    else if(ui2_idx == 0)
    {
        DLIST_INSERT_BEFORE(pt_cur, pt_inf, pt_list, t_link);
    }
    else
    {
        DLIST_INSERT_AFTER(pt_cur, pt_inf, pt_list, t_link);    
    }
    (*pui2_elem_num)++;

    return WGLR_OK;
}

INT32 lb_del_elem_inf(
    DLIST_LB_ELEM_INF_T*                pt_list, 
    UINT16*                             pui2_elem_num,
    WGL_NPM_CTX_T*                      pt_npm, 
    UINT16                              ui2_idx)
{
    LB_ELEM_INF_T*                      pt_inf = NULL;

    CHECK_VALID(pt_list && pt_npm && pui2_elem_num, WGLR_INV_ARG);

    if(ui2_idx > *pui2_elem_num || (*pui2_elem_num) == 0)
    {
        return WGLR_INTERNAL_ERROR;
    }
    
    lb_get_elem_inf(pt_list, *pui2_elem_num, ui2_idx, &pt_inf);

    if(pt_inf == NULL)
    {
        return WGLR_INV_ARG;
    }

    DLIST_REMOVE(pt_inf, pt_list, t_link);
    wgl_npm_free(pt_npm, pt_inf);

    if(*pui2_elem_num == 1)
    {
        DLIST_INIT(pt_list);
    }

    (*pui2_elem_num)--;
    return WGLR_OK;
}

INT32 lb_find_special_state_elem_inf(
    DLIST_LB_ELEM_INF_T*        pt_list, 
    UINT16                      ui2_max_num,
    UINT8                       ui1_state,
    UINT16                      ui2_start,
    UINT16                      ui2_cnt,
    BOOL                        b_forward,
    BOOL                        b_include,
    UINT16*                     pui2_idx,                   
    LB_ELEM_INF_T**             ppt_inf)
{
    LB_ELEM_INF_T*                      pt_elem_inf;
    LB_ELEM_INF_T*                      pt_inf;
    UINT16                              ui2_i = 0;
    UINT16                              ui2_count = 0;
    
    if(pui2_idx != NULL)
    {
       *pui2_idx = WGL_LB_NULL_INDEX; 
    }

    if(ppt_inf != NULL)
    {
        *ppt_inf = NULL;
    }

    CHECK_VALID((pt_list != NULL && ui2_cnt > 0 && ui2_start < ui2_max_num), WGLR_INV_ARG);
    
    lb_get_elem_inf(pt_list, ui2_max_num, ui2_start, &pt_inf);
    
    pt_elem_inf = pt_inf;
    if(pt_elem_inf == NULL)
    {
        return WGLR_OK;
    }
    ui2_i = ui2_start;
    
    while(pt_inf)
    {
        if( (b_include == TRUE && (pt_elem_inf->ui1_state & ui1_state)) ||
            (b_include == FALSE && (pt_elem_inf->ui1_state & ui1_state) == 0) )
        {
            ui2_count++;
        }

        if(ui2_count >= ui2_cnt)
        {
            break;
        }
        
        if(b_forward)
        {
            pt_elem_inf = DLIST_NEXT(pt_elem_inf, t_link);
            if(ui2_i >= (UINT16)(ui2_max_num - 1))
            {
                break;
            }
            ui2_i ++;
        }
        else
        {
            pt_elem_inf = DLIST_PREV(pt_elem_inf, t_link);
            if(ui2_i == 0)
            {
                break;
            }
            ui2_i --;
        }
    }

    if(ui2_count == ui2_cnt)
    {
        if(pui2_idx != NULL)
        {
           *pui2_idx = ui2_i; 
        }

        if(ppt_inf != NULL)
        {
            *ppt_inf = pt_elem_inf;
        }
    }

    return WGLR_OK;
}

UINT8* lb_get_elem_state_addr(
    DLIST_LB_ELEM_INF_T*                pt_list, 
    UINT16                              ui2_max_num,
    UINT16                              ui2_idx)
{
    LB_ELEM_INF_T*                      pt_elem_inf = NULL;
    
    lb_get_elem_inf(pt_list, ui2_max_num, ui2_idx, &pt_elem_inf);
    if(pt_elem_inf == NULL)
    {
        return NULL;   
    }
    
    return &pt_elem_inf->ui1_state; 
}

UINT8 lb_get_elem_state(
    DLIST_LB_ELEM_INF_T*                pt_list, 
    UINT16                              ui2_max_num,
    UINT16                              ui2_idx)
{
    LB_ELEM_INF_T*                      pt_elem_inf = NULL;

    lb_get_elem_inf(pt_list, ui2_max_num, ui2_idx, &pt_elem_inf);
    if(pt_elem_inf == NULL)
    {
        return 0xFF;
    }
    
    return pt_elem_inf->ui1_state;
}

INT32 lb_get_elem_special_state(
    DLIST_LB_ELEM_INF_T*                pt_list, 
    UINT16                              ui2_max_num,
    UINT8                               ui1_state,
    UINT16                              ui2_max_count,
    UINT16*                             pui2_idx)
{
    LB_ELEM_INF_T*                      pt_elem_inf;
    UINT16                              ui2_i = 0;
    UINT16                              ui2_count = 0;
    
    CHECK_VALID((pt_list != NULL && pui2_idx != NULL && ui2_max_count > 0), WGLR_INV_ARG);

    DLIST_FOR_EACH( pt_elem_inf, pt_list, t_link )
    {
        if(pt_elem_inf->ui1_state & ui1_state)
        {
            pui2_idx[ui2_count++] = ui2_i;
        }

        if(ui2_count >= ui2_max_count)
        {
            break;
        }
        ui2_i ++;
    }

    return WGLR_OK;
}

INT32 lb_find_elem_special_state(
    DLIST_LB_ELEM_INF_T*        pt_list, 
    UINT16                      ui2_max_num,
    UINT8                       ui1_state,
    UINT16                      ui2_start,
    UINT16                      ui2_cnt,
    BOOL                        b_forward,
    BOOL                        b_include,
    UINT16*                     pui2_idx,                   
    UINT8**                     ppui1_state)
{
    LB_ELEM_INF_T*                      pt_elem_inf;
    
     lb_find_special_state_elem_inf(pt_list, 
                                    ui2_max_num, 
                                    ui1_state, 
                                    ui2_start,
                                    ui2_cnt, 
                                    b_forward,
                                    b_include,
                                    pui2_idx, 
                                    &pt_elem_inf);
     if(ppui1_state != NULL)
     {
         if(pt_elem_inf == NULL)
         {
             *ppui1_state = NULL;
         }
         else
         {
             *ppui1_state = &pt_elem_inf->ui1_state;
         }
     } 
     
    return WGLR_OK;
}

INT32 lb_set_elem_state(
    DLIST_LB_ELEM_INF_T*        pt_list, 
    UINT16                      ui2_max_num,
    UINT16                      ui2_idx,
    UINT8                       ui1_state,
    BOOL                        b_flag)
{
    LB_ELEM_INF_T*                      pt_elem_inf = NULL;
    
    lb_get_elem_inf(pt_list, ui2_max_num, ui2_idx, &pt_elem_inf);
    if(pt_elem_inf == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }

    if(b_flag == TRUE)
    {
        WGL_SET_FLAG(pt_elem_inf->ui1_state, ui1_state);
    }
    else
    {
        WGL_CLEAR_FLAG(pt_elem_inf->ui1_state, ui1_state);
    }
    
    return WGLR_OK;
}

BOOL lb_chk_elem_state(
    DLIST_LB_ELEM_INF_T*        pt_list, 
    UINT16                      ui2_max_num,
    UINT16                      ui2_idx,
    UINT8                       ui1_chk_state)
{
    LB_ELEM_INF_T*                      pt_elem_inf = NULL;
    BOOL                                b_flag = FALSE;
    
    lb_get_elem_inf(pt_list, ui2_max_num, ui2_idx, &pt_elem_inf);
    if(pt_elem_inf == NULL)
    {
        return FALSE;
    }
    
    if(pt_elem_inf->ui1_state & ui1_chk_state)
    {
        b_flag = TRUE;
    }

    return b_flag;
}

INT32 lb_do_elems_state_inf(
    DLIST_LB_ELEM_INF_T*        pt_list, 
    UINT16                      ui2_max_num,
    UINT8*                      pui1_state,
    UINT16                      ui2_elem_start,
    UINT16                      ui2_max_count,
    BOOL                        b_set_get)
{
    LB_ELEM_INF_T*                      pt_elem_inf;
    UINT16                              ui2_i = 0;
    UINT16                              ui2_count = 0;
    
    CHECK_VALID((pt_list != NULL && 
                pui1_state!= NULL && 
                ui2_elem_start < ui2_max_num), WGLR_INV_ARG);

    DLIST_FOR_EACH( pt_elem_inf, pt_list, t_link )
    {
        if(ui2_count >= ui2_max_count)
        {
            break;
        }
        
        if(ui2_i >= ui2_elem_start)
        {
            if(b_set_get == TRUE)
            {
                pt_elem_inf->ui1_state = pui1_state[ui2_count++];
            }
            else
            {
                pui1_state[ui2_count++] = pt_elem_inf->ui1_state;
            }
        }

        ui2_i ++;
    }

    return WGLR_OK;
}

INT32 lb_reset_elems_state(
    DLIST_LB_ELEM_INF_T*        pt_list, 
    UINT16                      ui2_max_num,
    UINT16                      ui2_elem_start,
    UINT16                      ui2_max_count)
{
    LB_ELEM_INF_T*                      pt_elem_inf;
    UINT16                              ui2_i = 0;
    UINT16                              ui2_count = 0;
    
    CHECK_VALID((pt_list != NULL && 
                ui2_elem_start < ui2_max_num), WGLR_INV_ARG);

    DLIST_FOR_EACH( pt_elem_inf, pt_list, t_link )
    {
        if(ui2_count >= ui2_max_count)
        {
            break;
        }
        
        if(ui2_i >= ui2_elem_start)
        {
            pt_elem_inf->ui1_state = 0;
        }

        ui2_i ++;
    }

    return WGLR_OK;
}
/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
