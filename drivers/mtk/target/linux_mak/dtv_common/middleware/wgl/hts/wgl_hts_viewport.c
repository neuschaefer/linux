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
 * $RCSfile: wgl_hts_viewport.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/6 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: 2c46162b759413f7bb637f26bd65e2f5 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/hts/wgl_hts_viewport.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "HTSVP"

#define IS_VP_SIZE_EFFECTIVE(_pt_ctx)                                       \
            ((_pt_ctx)->ui2_size_vp > 0)

/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _hts_vp_reload(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    OUT     BOOL                        *pb_moved);

static INT32 _hts_vp_go_last_page(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    OUT     BOOL                        *pb_moved);


/*-----------------------------------------------------------------------------
                    public functions
 ----------------------------------------------------------------------------*/
INT32 wgl_hts_vp_init(
    OUT     WGL_HTS_VP_CTX_T            *pt_ctx,
    IN      UINT16                      ui2_attrs,
    IN      UINT16                      ui2_size_vp,
    IN      VOID                        *pv_elem_repository)
{
    
    pt_ctx->ui2_attrs          = ui2_attrs;
    pt_ctx->ui2_size_vp        = ui2_size_vp;
    pt_ctx->pv_elem_repository = pv_elem_repository;
    
    pt_ctx->ui2_first          = 0;
    pt_ctx->ui2_last           = 0;
    pt_ctx->ui2_size_cur       = 0;
    
    return WGLR_OK;
}        
    
INT32 wgl_hts_vp_destroy(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx)
{
    x_memset(pt_ctx, 0, sizeof(WGL_HTS_VP_CTX_T));
    return WGLR_OK;
}        

INT32 wgl_hts_vp_sync(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    OUT     BOOL                        *pb_moved)
{
    return _hts_vp_reload(pt_ctx, pb_moved);
}    

INT32 wgl_hts_vp_set_vp_size(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    IN      UINT16                      ui2_size_vp,
    OUT     BOOL                        *pb_moved)
{
    pt_ctx->ui2_size_vp = ui2_size_vp;
    *pb_moved = FALSE;
    
    return WGLR_OK;
}       

INT32 wgl_hts_vp_page_up(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    OUT     BOOL                        *pb_moved)
{
    INT32                       i4_ret;
    WGL_VP_TPL_ELEM_TYPE_T      *pt_elem, *pt_elem_last, *pt_elem_prev;
    UINT16                      ui2_first, ui2_last, ui2_size_sum, ui2_size_prev;
    UINT16                      ui2_num_elem, ui2_size_first;
    UINT16                      ui2_first_orig, ui2_last_orig;
    
    *pb_moved = FALSE;
    ui2_first_orig = pt_ctx->ui2_first;
    ui2_last_orig  = pt_ctx->ui2_last;
    CATCH_FAIL(_hts_vp_reload(pt_ctx, pb_moved));
    
    if(!IS_VP_SIZE_EFFECTIVE(pt_ctx) || pt_ctx->ui2_first==0 )
    {
        return WGLR_OK;
    }        
    
    ui2_num_elem = WGL_VP_TPL_ELEM_NUM();
    CATCH_FAIL(WGL_VP_TPL_GET_ELEM(pt_ctx->ui2_first, &pt_elem));
    WGL_VP_TPL_GET_ELEM_SIZE(pt_elem, &ui2_size_first);
    pt_elem_prev = WGL_VP_TPL_ELEM_PREV(pt_elem);
    CHECK_VALID(pt_elem_prev != NULL, WGLR_INTERNAL_ERROR);
    WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_prev, &ui2_size_prev);
    
    if(   HAS_FLAG(pt_ctx->ui2_attrs, WGL_VP_ATTR_NEW_PAGE_KEEP_LAST_LINE)
       && pt_ctx->ui2_size_vp >= ui2_size_first+ui2_size_prev )
    {
        ui2_last     = pt_ctx->ui2_first;
        pt_elem_last = pt_elem;
        ui2_size_sum = ui2_size_first;
    }   
    else
    {   /* force page-up if vp size is very small */
        ui2_last     = pt_ctx->ui2_first-(UINT16)1;
        pt_elem_last = WGL_VP_TPL_ELEM_PREV(pt_elem);
        WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_last, &ui2_size_sum);
    }        
    
    /* bottom-up phase: decrease ui2_first */
    ui2_first = ui2_last;
    pt_elem = pt_elem_last;
    while(ui2_first > 0)
    {
        pt_elem_prev = WGL_VP_TPL_ELEM_PREV(pt_elem);
        WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_prev, &ui2_size_prev);
        if(ui2_size_sum + ui2_size_prev > pt_ctx->ui2_size_vp)
        {
            break;
        }
        ui2_size_sum += ui2_size_prev;        
        ui2_first--;
        pt_elem = pt_elem_prev;
    }
    
    /* top-down phase: increase ui2_last */
    if(   ui2_last+(UINT16)1 < ui2_num_elem
       && ui2_size_sum < pt_ctx->ui2_size_vp)
    {
        WGL_VP_TPL_ELEM_TYPE_T      *pt_elem_next;
        UINT16                      ui2_size_next;
        
        pt_elem = pt_elem_last;
        while(ui2_last+(UINT16)1 < ui2_num_elem)
        {
            pt_elem_next = WGL_VP_TPL_ELEM_NEXT(pt_elem);
            WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_next, &ui2_size_next);
            if( ui2_size_sum + ui2_size_next > pt_ctx->ui2_size_vp )
            {
                break;
            }                
            ui2_size_sum += ui2_size_next;
            ui2_last++;
            pt_elem = pt_elem_next;            
        }        
    }    
    
    pt_ctx->ui2_first    = ui2_first;
    pt_ctx->ui2_last     = ui2_last;
    pt_ctx->ui2_size_cur = ui2_size_sum;
    
    if(   pt_ctx->ui2_first != ui2_first_orig 
       || pt_ctx->ui2_last  != ui2_last_orig )  
    {
        *pb_moved = TRUE;
    }       
    
    return WGLR_OK;   
}        

INT32 wgl_hts_vp_page_down(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    OUT     BOOL                        *pb_moved)
{
    INT32                       i4_ret;
    UINT16                      ui2_num_elem;
    UINT16                      ui2_first, ui2_last, ui2_size_last, ui2_size_sum, ui2_size_next;
    UINT16                      ui2_first_orig, ui2_last_orig;
    WGL_VP_TPL_ELEM_TYPE_T      *pt_elem, *pt_elem_first, *pt_elem_next;
    
    *pb_moved = FALSE;
    ui2_first_orig = pt_ctx->ui2_first;
    ui2_last_orig  = pt_ctx->ui2_last;
    CATCH_FAIL(_hts_vp_reload(pt_ctx, pb_moved));
    
    ui2_num_elem = WGL_VP_TPL_ELEM_NUM();
    if(!IS_VP_SIZE_EFFECTIVE(pt_ctx) || pt_ctx->ui2_last + (UINT16)1 >= ui2_num_elem )
    {
        return WGLR_OK;
    }
    
    CATCH_FAIL(WGL_VP_TPL_GET_ELEM(pt_ctx->ui2_last, &pt_elem));
    WGL_VP_TPL_GET_ELEM_SIZE(pt_elem, &ui2_size_last);
    pt_elem_next = WGL_VP_TPL_ELEM_NEXT(pt_elem);
    CHECK_VALID(pt_elem_next != NULL, WGLR_INTERNAL_ERROR);
    WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_next, &ui2_size_next);
    
    if(   HAS_FLAG(pt_ctx->ui2_attrs, WGL_VP_ATTR_NEW_PAGE_KEEP_LAST_LINE) 
       && pt_ctx->ui2_first == pt_ctx->ui2_last
       && pt_ctx->ui2_size_vp >= ui2_size_last + ui2_size_next)
    {
        ui2_first       = pt_ctx->ui2_last;
        pt_elem_first   = pt_elem;
        ui2_size_sum    = ui2_size_last;
    }
    else
    {   /* force page-down if vp_size is very small */
        ui2_first       = pt_ctx->ui2_last + (UINT16)1;
        pt_elem_first   = WGL_VP_TPL_ELEM_NEXT(pt_elem);
        WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_first, &ui2_size_sum);
    }
       
    /* top-down phase: increase ui2_last */       
    ui2_last     = ui2_first;
    pt_elem      = pt_elem_first;
    while(ui2_last+(UINT16)1 < ui2_num_elem)
    {
        pt_elem_next = WGL_VP_TPL_ELEM_NEXT(pt_elem);
        WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_next, &ui2_size_next);
        if(ui2_size_sum + ui2_size_next > pt_ctx->ui2_size_vp)
        {
            break;
        }
        
        ui2_size_sum += ui2_size_next;
        ui2_last ++;        
        pt_elem = pt_elem_next;
    }        
    
    /* bottom-up phase: decrease ui2_first */
    if(   HAS_FLAG(pt_ctx->ui2_attrs, WGL_VP_ATTR_FINAL_PAGE_DOWN_FILL_VP)
       && ui2_first > 0 
       && ui2_size_sum < pt_ctx->ui2_size_vp )
    {
        UINT16                  ui2_size_prev;
        WGL_VP_TPL_ELEM_TYPE_T  *pt_elem_prev;
            
        pt_elem = pt_elem_first;
        while(ui2_first > 0)
        {
            pt_elem_prev = WGL_VP_TPL_ELEM_PREV(pt_elem);
            WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_prev, &ui2_size_prev);
            if(ui2_size_sum + ui2_size_prev > pt_ctx->ui2_size_vp)
            {
                break;
            }
            
            ui2_size_sum += ui2_size_prev;
            ui2_first --;
            pt_elem = pt_elem_prev;
        }
    }
    
    pt_ctx->ui2_first    = ui2_first;
    pt_ctx->ui2_last     = ui2_last;
    pt_ctx->ui2_size_cur = ui2_size_sum;
    
    if(   pt_ctx->ui2_first != ui2_first_orig 
       || pt_ctx->ui2_last  != ui2_last_orig )  
    {
        *pb_moved = TRUE;
    }       
 
    return WGLR_OK;   
}        

INT32 wgl_hts_vp_scrl_up(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    IN      UINT16                      ui2_scrl_step,
    OUT     BOOL                        *pb_moved)
{
    INT32                       i4_ret;
    UINT16                      ui2_first_orig, ui2_last_orig, ui2_size_next;
    UINT16                      ui2_num_elem, ui2_first, ui2_last, ui2_size_sum;
    WGL_VP_TPL_ELEM_TYPE_T      *pt_elem, *pt_elem_next;
        
    *pb_moved = FALSE;
    ui2_first_orig = pt_ctx->ui2_first;
    ui2_last_orig  = pt_ctx->ui2_last;
    CATCH_FAIL(_hts_vp_reload(pt_ctx, pb_moved));

    if(!IS_VP_SIZE_EFFECTIVE(pt_ctx) || pt_ctx->ui2_first==0 || ui2_scrl_step==0)
    {
        return WGLR_OK;
    }        
    
    ui2_first = pt_ctx->ui2_first > ui2_scrl_step ? pt_ctx->ui2_first - ui2_scrl_step : 0;
    
    /* top-down process: find ui2_last */
    ui2_num_elem = WGL_VP_TPL_ELEM_NUM();
    ui2_last = ui2_first;
    CATCH_FAIL(WGL_VP_TPL_GET_ELEM(ui2_last, &pt_elem));
    WGL_VP_TPL_GET_ELEM_SIZE(pt_elem, &ui2_size_sum);
    while(ui2_last+(UINT16)1 < ui2_num_elem)
    {
        pt_elem_next = WGL_VP_TPL_ELEM_NEXT(pt_elem);
        WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_next, &ui2_size_next);
        if(ui2_size_sum + ui2_size_next > pt_ctx->ui2_size_vp)
        {
            break;
        }
        
        ui2_size_sum += ui2_size_next;
        ui2_last ++;
        pt_elem = pt_elem_next;
    }
        
    pt_ctx->ui2_first    = ui2_first;
    pt_ctx->ui2_last     = ui2_last;
    pt_ctx->ui2_size_cur = ui2_size_sum;
    
    if(   pt_ctx->ui2_first != ui2_first_orig 
       || pt_ctx->ui2_last  != ui2_last_orig )  
    {
        *pb_moved = TRUE;
    }       
            
 
    return WGLR_OK;   
}        


INT32 wgl_hts_vp_scrl_down(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    IN      UINT16                      ui2_scrl_step,
    OUT     BOOL                        *pb_moved)
{
    INT32                       i4_ret;
    UINT16                      ui2_first_orig, ui2_last_orig;
    UINT16                      ui2_num_elem, ui2_first, ui2_last, ui2_size_sum;
    UINT16                      ui2_size_prev;
    WGL_VP_TPL_ELEM_TYPE_T      *pt_elem, *pt_elem_prev;
        
    *pb_moved      = FALSE;
    ui2_first_orig = pt_ctx->ui2_first;
    ui2_last_orig  = pt_ctx->ui2_last;
    CATCH_FAIL(_hts_vp_reload(pt_ctx, pb_moved));

    ui2_num_elem = WGL_VP_TPL_ELEM_NUM();
    if(!IS_VP_SIZE_EFFECTIVE(pt_ctx) || pt_ctx->ui2_last+(UINT16)1>=ui2_num_elem || ui2_scrl_step==0 )
    {
        return WGLR_OK;
    }        

    ui2_last = pt_ctx->ui2_last + ui2_scrl_step;
    ui2_last = ui2_last < ui2_num_elem ? ui2_last : (ui2_num_elem-(UINT16)1);
        
    /* bottom-up process: decrease ui2_first */        
    ui2_first = ui2_last;    
    CATCH_FAIL(WGL_VP_TPL_GET_ELEM(ui2_first, &pt_elem));
    WGL_VP_TPL_GET_ELEM_SIZE(pt_elem, &ui2_size_sum);
    while(ui2_first > 0)
    {
        pt_elem_prev = WGL_VP_TPL_ELEM_PREV(pt_elem);
        WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_prev, &ui2_size_prev);
        if(ui2_size_sum + ui2_size_prev > pt_ctx->ui2_size_vp)
        {
            break;
        }
        
        ui2_size_sum += ui2_size_prev;
        ui2_first --;
        pt_elem = pt_elem_prev;
    }
            
    pt_ctx->ui2_first    = ui2_first;
    pt_ctx->ui2_last     = ui2_last;
    pt_ctx->ui2_size_cur = ui2_size_sum;
    
    if(   pt_ctx->ui2_first != ui2_first_orig 
       || pt_ctx->ui2_last  != ui2_last_orig )  
    {
        *pb_moved = TRUE;
    }       
 
    return WGLR_OK;   
}        

INT32 wgl_hts_vp_set_first(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    IN      UINT16                      ui2_first_new,
    OUT     BOOL                        *pb_moved)
{
    INT32                       i4_ret;
    UINT16                      ui2_num_elem, ui2_first, ui2_last, ui2_size_sum;
    UINT16                      ui2_size_next;
    WGL_VP_TPL_ELEM_TYPE_T      *pt_elem, *pt_elem_next;

    *pb_moved      = FALSE;
    CATCH_FAIL(_hts_vp_reload(pt_ctx, pb_moved));
    
    ui2_num_elem = WGL_VP_TPL_ELEM_NUM();
    CHECK_VALID(ui2_first_new < ui2_num_elem, WGLR_INV_ARG);
    if(!IS_VP_SIZE_EFFECTIVE(pt_ctx) || ui2_first_new == pt_ctx->ui2_first)
    {
        return WGLR_OK;
    }
    
    ui2_first = ui2_first_new;
    
    /* top-down process: increase ui2_last */
    ui2_last = ui2_first;
    CATCH_FAIL(WGL_VP_TPL_GET_ELEM(ui2_last, &pt_elem));
    WGL_VP_TPL_GET_ELEM_SIZE(pt_elem, &ui2_size_sum);
    while(ui2_last+(UINT16)1 < ui2_num_elem)
    {
        pt_elem_next = WGL_VP_TPL_ELEM_NEXT(pt_elem);
        WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_next, &ui2_size_next);
        
        if(ui2_size_sum + ui2_size_next > pt_ctx->ui2_size_vp)
        {
            break;
        }
        
        ui2_size_sum += ui2_size_next;
        ui2_last ++;
        pt_elem = pt_elem_next;
    }
    
    pt_ctx->ui2_first    = ui2_first;
    pt_ctx->ui2_last     = ui2_last;
    pt_ctx->ui2_size_cur = ui2_size_sum;
    
    *pb_moved = TRUE;
 
    return WGLR_OK;   
}        


INT32 wgl_hts_vp_get_first(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    OUT     UINT16                      *pui2_first,
    OUT     BOOL                        *pb_moved)
{
    INT32       i4_ret;
    
    CATCH_FAIL(_hts_vp_reload(pt_ctx, pb_moved));
    
    *pui2_first = pt_ctx->ui2_first;

    return WGLR_OK;   
}        

INT32 wgl_hts_vp_get_last(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    OUT     UINT16                      *pui2_last,
    OUT     BOOL                        *pb_moved)
{
    INT32       i4_ret;
    
    CATCH_FAIL(_hts_vp_reload(pt_ctx, pb_moved));
    
    *pui2_last = pt_ctx->ui2_last;

    return WGLR_OK;   
}        

INT32 wgl_hts_vp_get_cur_size(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    OUT     UINT16                      *pui2_cur_size,
    OUT     BOOL                        *pb_moved)
{
    INT32       i4_ret;
    
    CATCH_FAIL(_hts_vp_reload(pt_ctx, pb_moved));
    
    *pui2_cur_size = pt_ctx->ui2_size_cur;
    
    return WGLR_OK;
}        

INT32 wgl_hts_vp_get_state(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    OUT     UINT16                      *pui2_first,
    OUT     UINT16                      *pui2_last,
    OUT     UINT16                      *pui2_cur_size,
    OUT     BOOL                        *pb_moved)
{
    INT32       i4_ret;
    
    CATCH_FAIL(_hts_vp_reload(pt_ctx, pb_moved));
    
    *pui2_first = pt_ctx->ui2_first;
    *pui2_last = pt_ctx->ui2_last;
    *pui2_cur_size = pt_ctx->ui2_size_cur;
    
    return WGLR_OK;
}        


INT32 wgl_hts_vp_get_page_num(
    IN      WGL_HTS_VP_CTX_T            *pt_ctx,
    OUT     UINT16                      *pui2_page_num,
    OUT     BOOL                        *pb_moved)
{
    INT32                       i4_ret;
    WGL_VP_TPL_ELEM_TYPE_T      *pt_elem, *pt_elem_next;
    UINT16                      ui2_i, ui2_num_page;
    UINT16                      ui2_num_elem, ui2_size_sum;
    UINT16                      ui2_size_next;
    
    *pui2_page_num = 0;
    
    /* [Note] consider the case when VP size is very small */
    CATCH_FAIL(_hts_vp_reload(pt_ctx, pb_moved));
    
    ui2_num_elem = WGL_VP_TPL_ELEM_NUM();
    CATCH_FAIL(WGL_VP_TPL_GET_ELEM(0, &pt_elem));
    WGL_VP_TPL_GET_ELEM_SIZE(pt_elem, &ui2_size_sum);
    ui2_num_page = 1;
    ui2_i        = 0;
    while(ui2_i+(UINT16)1 < ui2_num_elem)
    {
        pt_elem_next = WGL_VP_TPL_ELEM_NEXT(pt_elem);
        WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_next, &ui2_size_next);
        if(ui2_size_sum + ui2_size_next > pt_ctx->ui2_size_vp)
        {
            /* one more page is found! */
            ui2_num_page++;
            
            /* context for new page */
            ui2_size_sum = ui2_size_next;
        }
        else
        {
            ui2_size_sum += ui2_size_next;
        }
        
        ui2_i++;
        pt_elem = pt_elem_next;
    }
    
    *pui2_page_num = ui2_num_page;
    
    return WGLR_OK;
    
}        


/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _hts_vp_reload(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    OUT     BOOL                        *pb_moved)
{
    INT32                   i4_ret;
    UINT16                  ui2_num_elem, ui2_first, ui2_last;
    BOOL                    b_func_moved;
    UINT16                  ui2_first_orig, ui2_last_orig, ui2_size_orig;
    WGL_VP_TPL_ELEM_TYPE_T  *pt_elem, *pt_elem_next;
    UINT16                  ui2_size_next, ui2_size_sum;
    
    *pb_moved = FALSE;
    
    if(!IS_VP_SIZE_EFFECTIVE(pt_ctx))
    {
        return WGLR_OK;
    }
    
    ui2_first_orig = pt_ctx->ui2_first;
    ui2_last_orig  = pt_ctx->ui2_last;
    ui2_size_orig  = pt_ctx->ui2_size_cur;
    ui2_num_elem   = WGL_VP_TPL_ELEM_NUM();
    
    /* if(ui2_last > elem_num), go_last_page() */
    if(pt_ctx->ui2_last >= ui2_num_elem)
    {
        CATCH_FAIL(_hts_vp_go_last_page(pt_ctx, &b_func_moved));
        *pb_moved = b_func_moved ? TRUE : *pb_moved;
        return WGLR_OK;
    }        
    
    /* recalculate the context */
    ui2_first = pt_ctx->ui2_first;
    ui2_last = ui2_first;
    CATCH_FAIL(WGL_VP_TPL_GET_ELEM(ui2_last, &pt_elem));
    WGL_VP_TPL_GET_ELEM_SIZE(pt_elem, &ui2_size_sum);
    while(ui2_last+(UINT16)1 < ui2_num_elem)
    {
        pt_elem_next = WGL_VP_TPL_ELEM_NEXT(pt_elem);
        WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_next, &ui2_size_next);
        if( ui2_size_sum + ui2_size_next > pt_ctx->ui2_size_vp )
        {
            break;
        }                
        ui2_size_sum += ui2_size_next;
        ui2_last++;
        pt_elem = pt_elem_next;            
    }
    
    pt_ctx->ui2_first    = ui2_first;
    pt_ctx->ui2_last     = ui2_last;
    pt_ctx->ui2_size_cur = ui2_size_sum;
    
    if(   pt_ctx->ui2_first != ui2_first_orig 
       || pt_ctx->ui2_last  != ui2_last_orig 
       || pt_ctx->ui2_size_cur != ui2_size_orig)  
    {
        *pb_moved = TRUE;
    }       
    
    return WGLR_OK;
}        


static INT32 _hts_vp_go_last_page(
    IN OUT  WGL_HTS_VP_CTX_T            *pt_ctx,
    OUT     BOOL                        *pb_moved)
{
    INT32                       i4_ret;
    WGL_VP_TPL_ELEM_TYPE_T      *pt_elem, *pt_elem_prev;
    UINT16                      ui2_num_elem, ui2_size_sum, ui2_first, ui2_last, ui2_size_prev;
    UINT16                      ui2_first_orig, ui2_last_orig;
    
    *pb_moved = FALSE;
    
    if(!IS_VP_SIZE_EFFECTIVE(pt_ctx))
    {
        return WGLR_OK;
    }
    
    ui2_first_orig = pt_ctx->ui2_first;
    ui2_last_orig  = pt_ctx->ui2_last;
    
    ui2_num_elem = WGL_VP_TPL_ELEM_NUM();
    ui2_last = ui2_num_elem - (UINT16)1;
    ui2_first = ui2_last;
    CATCH_FAIL(WGL_VP_TPL_GET_ELEM(ui2_first, &pt_elem));
    WGL_VP_TPL_GET_ELEM_SIZE(pt_elem, &ui2_size_sum);
    while(ui2_first > 0)
    {
        pt_elem_prev = WGL_VP_TPL_ELEM_PREV(pt_elem);
        WGL_VP_TPL_GET_ELEM_SIZE(pt_elem_prev, &ui2_size_prev);

        if(ui2_size_sum + ui2_size_prev > pt_ctx->ui2_size_vp)        
        {
            break;
        }
        
        ui2_first--;
        ui2_size_sum += ui2_size_prev;
        pt_elem = pt_elem_prev;
    }
    
    pt_ctx->ui2_first    = ui2_first;
    pt_ctx->ui2_last     = ui2_last;
    pt_ctx->ui2_size_cur = ui2_size_sum;
    
    if(   pt_ctx->ui2_first != ui2_first_orig 
       || pt_ctx->ui2_last  != ui2_last_orig )  
    {
        *pb_moved = TRUE;
    }       
    
    return WGLR_OK;
}    


