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
 * $RCSfile: wgl_lb_viewport.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: 23b6643ab8d7c688bca98386aabdd868 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "dbg/def_dbg_level_mw.h"
#include "wgl/listbox/wgl_lb_viewport.h"
#include "wgl/wgl_tools.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/listbox/wgl_lb_dbg.h"


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
#define DBG_MODULE_NAME     "LBVP"


#define _GET_VP_ELEM_NUM(_pt_ctx)                                           \
    ((pt_ctx)->ui2_idx_vp_last-(pt_ctx)->ui2_idx_vp_first+1) 

#define _GET_LAST_ELEM_END_POS(_pt_ctx)                                     \
            ( (_pt_ctx)->i4_offset                                          \
                + (INT32)(((_pt_ctx)->ui2_idx_vp_last-(_pt_ctx)->ui2_idx_vp_first)*((_pt_ctx)->ui2_size_elem+(_pt_ctx)->ui2_dist_elem)) \
                + (INT32)(_pt_ctx)->ui2_size_elem ) 

#define _DEBUG_CHECK_VP_CTX(_pt_ctx)                                        \
    INFORM_INVALID( ((_pt_ctx)->ui2_num_elem <= WGL_VP_MAX_ELEM_NUM)        \
                  && ((_pt_ctx)->ui2_idx_vp_first==WGL_VP_NULL_INDEX || (_pt_ctx)->ui2_idx_vp_first < (_pt_ctx)->ui2_num_elem )     \
                  && ((_pt_ctx)->ui2_idx_vp_last==WGL_VP_NULL_INDEX || (_pt_ctx)->ui2_idx_vp_last < (_pt_ctx)->ui2_num_elem)        \
                  && ((_pt_ctx)->ui2_idx_vp_last>=(_pt_ctx)->ui2_idx_vp_first), WGLR_INTERNAL_ERROR )

#define  _SAFE_SUB(_sub, _x, _y)                                            \
            if( (_x)>(_y) )                                                 \
            {                                                               \
                (_sub) = (_x) - (_y);                                       \
            }                                                               \
            else                                                            \
            {                                                               \
                (_sub) = 0;                                                 \
            }

#define _HALF_ELEM_SIZE(_pt_ctx)  ((_pt_ctx)->ui2_size_elem>>1)



/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static VOID _vp_update_vp_size_base_info (
    IN OUT  WGL_VP_CTX_T                *pt_ctx)
{
    pt_ctx->ui2_vp_elem_num_base = pt_ctx->ui2_size_vp / (pt_ctx->ui2_size_elem + pt_ctx->ui2_dist_elem);
    pt_ctx->ui2_vp_elem_total_height_base = pt_ctx->ui2_vp_elem_num_base * (pt_ctx->ui2_size_elem+pt_ctx->ui2_dist_elem);
    
    if(pt_ctx->ui2_size_vp <= pt_ctx->ui2_vp_elem_total_height_base + _HALF_ELEM_SIZE(pt_ctx))
    {
        /* the visible part of the last elem in VP is smaller than half */
        if (pt_ctx->ui2_vp_elem_num_base > 0)
        {
            pt_ctx->ui2_vp_elem_num_eff = pt_ctx->ui2_vp_elem_num_base;
        }
        else
        {
            pt_ctx->ui2_vp_elem_num_eff = 1;
        }
    }
    else
    {
        /* the visible part of the last elem in VP is larger than half */
        pt_ctx->ui2_vp_elem_num_eff = pt_ctx->ui2_vp_elem_num_base + (UINT16)1;
    }
    
}    
 
/*-----------------------------------------------------------------------------
 * Name: _vp_update_elem_num_residue
 *
 * Description: This api updates the elem num residue related context.
 *
 * Inputs:  pt_ctx  References the viewport context.
 *
 * Outputs: pt_ctx  References the viewport context.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _vp_update_elem_num_residue(
    IN OUT  WGL_VP_CTX_T                *pt_ctx)
{
    if(pt_ctx->ui2_vp_elem_num_base == 0)
    {
        pt_ctx->ui2_num_elem_residue = 1;
    }
    else
    {
        pt_ctx->ui2_num_elem_residue = pt_ctx->ui2_num_elem % pt_ctx->ui2_vp_elem_num_eff;
        if(pt_ctx->ui2_num_elem_residue == 0 && pt_ctx->ui2_num_elem > 0)
        {
           pt_ctx->ui2_num_elem_residue = pt_ctx->ui2_vp_elem_num_eff;
        }
    }
}

static VOID _vp_reset_case_1(
    IN OUT  WGL_VP_CTX_T                *pt_ctx,
    OUT     BOOL                        *pb_vp_scrled)
{
    *pb_vp_scrled = FALSE;
    
    pt_ctx->i4_offset = 0;
    
    if (pt_ctx->ui2_num_elem == 0)
    {
        pt_ctx->ui2_idx_vp_first = WGL_VP_NULL_INDEX;
        pt_ctx->ui2_idx_vp_last  = WGL_VP_NULL_INDEX;
    }
    else
    {
        *pb_vp_scrled = (pt_ctx->ui2_idx_vp_first != 0) ? TRUE : FALSE;
        pt_ctx->ui2_idx_vp_first = 0;
        pt_ctx->ui2_idx_vp_last  = pt_ctx->ui2_num_elem - (UINT16)1;
    }
}

/*-----------------------------------------------------------------------------
                    public functions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: wgl_vp_init
 *
 * Description: This API initializes the vp context.
 *
 * Inputs:  ui2_size_vp    Specifies the pixel size of the viewport.
 *          ui2_size_elem  Specifies the pixel size of each element.
 *          ui2_dist_elem  Specifies the pixel size of the distance between elements.
 *          ui2_num_elem   Specifies the number of elements.
 *
 * Outputs: pt_ctx         Contains the viewport context. 
 *
 * Returns: VPR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
INT32 wgl_vp_init(
    IN      UINT16                      ui2_size_vp,
    IN      UINT16                      ui2_size_elem,
    IN      UINT16                      ui2_dist_elem,
    IN      UINT16                      ui2_num_elem, 
    OUT     WGL_VP_CTX_T                *pt_ctx)
{
    WS_ASSERT(ui2_size_vp <= WGL_VP_MAX_SIZE);
    WS_ASSERT(ui2_num_elem <= WGL_VP_MAX_ELEM_NUM);

    pt_ctx->ui2_size_elem        = ui2_size_elem;
    pt_ctx->ui2_dist_elem        = ui2_dist_elem;
    pt_ctx->ui2_size_vp          = ui2_size_vp;
    pt_ctx->ui2_num_elem         = ui2_num_elem;
    _vp_update_vp_size_base_info(pt_ctx);
    _vp_update_elem_num_residue(pt_ctx);
    pt_ctx->i4_offset            = 0;
    if(pt_ctx->ui2_num_elem == 0)
    {
        pt_ctx->ui2_idx_vp_first = pt_ctx->ui2_idx_vp_last = WGL_VP_NULL_INDEX;
    }
    else
    {
        pt_ctx->ui2_idx_vp_first = 0;
        pt_ctx->ui2_idx_vp_last  = pt_ctx->ui2_idx_vp_first + pt_ctx->ui2_vp_elem_num_base - (UINT16)1;
        if( pt_ctx->ui2_idx_vp_last >= pt_ctx->ui2_num_elem )
        {
            pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_num_elem-(UINT16)1;
        }
        else if( pt_ctx->ui2_size_vp > pt_ctx->ui2_vp_elem_total_height_base )
        {
            if(pt_ctx->ui2_idx_vp_last+ (UINT16)1 < pt_ctx->ui2_num_elem )
            {
                pt_ctx->ui2_idx_vp_last++;
            }
        }
    }

    _DEBUG_CHECK_VP_CTX(pt_ctx);
    return VPR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_vp_set_vp_size
 *
 * Description: This API changes the pixel size of the viewport.
 *
 * Inputs:  pt_ctx    Contains the viewport context.
 *          ui2_size  Specifies the new size.
 *
 * Outputs: pt_ctx    Contains the new viewport context.
 *
 * Returns: VPR_OK    Routine successful.
 ----------------------------------------------------------------------------*/
INT32 wgl_vp_set_vp_size(
    IN OUT  WGL_VP_CTX_T                *pt_ctx,
    IN      UINT16                      ui2_size)
{
    _DEBUG_CHECK_VP_CTX(pt_ctx);
    WS_ASSERT(ui2_size <= WGL_VP_MAX_SIZE);

    pt_ctx->ui2_size_vp                   = ui2_size;
    pt_ctx->i4_offset                     = 0;
    _vp_update_vp_size_base_info(pt_ctx);
    _vp_update_elem_num_residue(pt_ctx);

    if( pt_ctx->ui2_num_elem==0 )
    {
        WS_ASSERT(pt_ctx->i4_offset==0 && pt_ctx->ui2_idx_vp_first==WGL_VP_NULL_INDEX && pt_ctx->ui2_idx_vp_last==WGL_VP_NULL_INDEX);
        return VPR_OK;
    }

    /* consider if pt_ctx->ui2_vp_elem_num_base==0*/
    if(pt_ctx->ui2_vp_elem_num_base==0)
    {
        pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_idx_vp_first;
    }
    else if( (pt_ctx->ui2_idx_vp_first+pt_ctx->ui2_vp_elem_num_base)
                >= pt_ctx->ui2_num_elem )
    {   /*need to move down the first visible element*/
     
        pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_num_elem-(UINT16)1;
        if(pt_ctx->ui2_idx_vp_last < pt_ctx->ui2_idx_vp_first)
        {
            pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_idx_vp_first;
        }
        
        _SAFE_SUB(pt_ctx->ui2_idx_vp_first, pt_ctx->ui2_num_elem, pt_ctx->ui2_vp_elem_num_base);
        if( pt_ctx->ui2_idx_vp_first > 0 &&
            pt_ctx->ui2_vp_elem_total_height_base < pt_ctx->ui2_size_vp ) 
        {   /*there is little more spaces for an additional element to be put into the viewport*/
            pt_ctx->ui2_idx_vp_first --;
            pt_ctx->i4_offset = ((INT32)(pt_ctx->ui2_size_vp-pt_ctx->ui2_vp_elem_total_height_base)) 
                                    - ((INT32)pt_ctx->ui2_size_elem);
            if(pt_ctx->i4_offset > 0)
            { /*ensure the first visible element is aligned on the upper boundary of the viewport*/
                pt_ctx->i4_offset = 0;
            }
        }
    }
    else
    {   /*keep the first visible element be put on the upper boundary of the viewport */
        /*Thus the index of the first visible element don't have to change */
        if( pt_ctx->ui2_idx_vp_first + pt_ctx->ui2_vp_elem_num_base > 0 )
        {
            pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_idx_vp_first + pt_ctx->ui2_vp_elem_num_base - (UINT16)1;
        }
        else
        {
            pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_idx_vp_first;
        }
        /*there may be more spaces for the element next to the estimated last element */
        if( pt_ctx->ui2_idx_vp_last >= pt_ctx->ui2_num_elem)
        {
            pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_num_elem - (UINT16)1 ;   
        }
        else if( (pt_ctx->ui2_vp_elem_total_height_base < pt_ctx->ui2_size_vp) &&
                 (pt_ctx->ui2_idx_vp_last + (UINT16)1 < pt_ctx->ui2_num_elem) )
        {
            pt_ctx->ui2_idx_vp_last++;
        }
    }

    _DEBUG_CHECK_VP_CTX(pt_ctx);
    return VPR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_vp_add_elem
 *
 * Description: This API notifies that an element has been added.
 *
 * Inputs:  pt_ctx  Contains the viewport context.
 *
 * Outputs: pt_ctx  Contains the new viewport context.
 *
 * Returns: VPR_OK  Routine successful.
 ----------------------------------------------------------------------------*/
INT32 wgl_vp_add_elem(
    IN OUT  WGL_VP_CTX_T                *pt_ctx)
{
    _DEBUG_CHECK_VP_CTX(pt_ctx);
    
    if( pt_ctx->ui2_num_elem == 0 )
    {
        pt_ctx->ui2_num_elem     = 1;
        pt_ctx->i4_offset        = 0;
        pt_ctx->ui2_idx_vp_first = pt_ctx->ui2_idx_vp_last = 0;
        _vp_update_elem_num_residue(pt_ctx);
    }
    else
    {
        pt_ctx->ui2_num_elem++;
        _vp_update_elem_num_residue(pt_ctx);
    
        if( pt_ctx->ui2_idx_vp_last + (UINT16)2 < pt_ctx->ui2_num_elem )
        {
            _DEBUG_CHECK_VP_CTX(pt_ctx);
            return VPR_OK;
        }
    
        if( ((pt_ctx->ui2_idx_vp_last + (UINT16)1) < pt_ctx->ui2_num_elem ) &&
            ((INT32)pt_ctx->ui2_size_vp > (_GET_LAST_ELEM_END_POS(pt_ctx)+(INT32)pt_ctx->ui2_dist_elem))  )
        {
            pt_ctx->ui2_idx_vp_last++;
        }
    }
    
    _DEBUG_CHECK_VP_CTX(pt_ctx);
    return VPR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_vp_del_elem
 *
 * Description: This API notifies that an element has been removed.
 *
 * Inputs:  pt_ctx  Contains the viewport context.
 *
 * Outputs: pt_ctx  Contains the new viewport context.
 *
 * Returns: VPR_OK       Routine successful.
 *          VPR_NO_ELEM  No element can be removed.
 ----------------------------------------------------------------------------*/
INT32 wgl_vp_del_elem(
    IN OUT  WGL_VP_CTX_T                *pt_ctx)
{
    WS_ASSERT(pt_ctx->ui2_num_elem > 0);
    _DEBUG_CHECK_VP_CTX(pt_ctx);
    if( pt_ctx->ui2_num_elem == 0 )
    {
        return VPR_NO_ELEM;
    }

    if( pt_ctx->ui2_num_elem == 1 )
    {
        pt_ctx->ui2_num_elem     = 0;
        pt_ctx->i4_offset        = 0;
        pt_ctx->ui2_idx_vp_first = pt_ctx->ui2_idx_vp_last = WGL_VP_NULL_INDEX;
        _vp_update_elem_num_residue(pt_ctx);
    }
    else
    {
        pt_ctx->ui2_num_elem --;
        _vp_update_elem_num_residue(pt_ctx);
        if(pt_ctx->ui2_num_elem <= pt_ctx->ui2_vp_elem_num_base)
        {
            pt_ctx->i4_offset = 0;
        }
        
        if( pt_ctx->ui2_idx_vp_last < pt_ctx->ui2_num_elem )
        {
            _DEBUG_CHECK_VP_CTX(pt_ctx);
            return VPR_OK;
        }

        /*pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_num_elem-1;*/
        pt_ctx->ui2_idx_vp_last --;
        WS_ASSERT(pt_ctx->ui2_idx_vp_last < pt_ctx->ui2_num_elem);
        if(pt_ctx->ui2_idx_vp_first > 0)
        {
            pt_ctx->ui2_idx_vp_first --;
        }
    }
    
    _DEBUG_CHECK_VP_CTX(pt_ctx);

    return VPR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_vp_clear
 *
 * Description: This API resets the element number to zero.
 *
 * Inputs:  pt_ctx  Contains the viewport context.
 *
 * Outputs: pt_ctx  Contains the new viewport context.
 *
 * Returns: VPR_OK  Routine successful.
 ----------------------------------------------------------------------------*/
INT32 wgl_vp_clear(
    IN OUT  WGL_VP_CTX_T                *pt_ctx)
{
    pt_ctx->ui2_num_elem     = 0;
    pt_ctx->i4_offset        = 0;
    pt_ctx->ui2_idx_vp_first = pt_ctx->ui2_idx_vp_last = WGL_VP_NULL_INDEX;
    _vp_update_elem_num_residue(pt_ctx);

    return VPR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_vp_get_range
 *
 * Description: This API retrives the current element range of the viewport.
 *
 * Inputs:  pt_ctx       Contains the viewport context.
 *
 * Outputs:  pui2_first  Contains the first element index.
 *           pui2_last   Contains the last element index.
 *           pi4_offset  Contains the pixel offset of the first element.
 *
 * Returns: VPR_OK       Routine successful.
 ----------------------------------------------------------------------------*/
INT32 wgl_vp_get_range( 
    IN      WGL_VP_CTX_T                *pt_ctx,
    OUT     UINT16                      *pui2_first, 
    OUT     UINT16                      *pui2_last, 
    OUT     INT32                       *pi4_offset)
{

    _DEBUG_CHECK_VP_CTX(pt_ctx);

    *pui2_first = pt_ctx->ui2_idx_vp_first;
    *pui2_last  = pt_ctx->ui2_idx_vp_last;
    *pi4_offset = pt_ctx->i4_offset;
    return VPR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_vp_get_visible_range
 *
 * Description: This API gets the current visible element range. Note that 
 *              an element is visible if and only if it's completely visible by the user.
 *
 * Inputs:  pt_ctx      Contains the viewport context.
 *
 * Outputs: pui2_first  Contains the index of the first visible element.
 *          pui2_last   Contains the index of the last visible element.
 *
 * Returns: VPR_OK      Routine successful.
 ----------------------------------------------------------------------------*/
INT32 wgl_vp_get_visible_range(
    IN      WGL_VP_CTX_T                *pt_ctx,
    OUT     UINT16                      *pui2_first,
    OUT     UINT16                      *pui2_last)
{
    UINT16      ui2_first, ui2_last;
    INT32       i4_ret;

    _DEBUG_CHECK_VP_CTX(pt_ctx);
    
    if(pt_ctx->ui2_num_elem == 0)
    {
        *pui2_first = WGL_VP_NULL_INDEX;
        *pui2_last  = WGL_VP_NULL_INDEX;
        return VPR_OK;
    }
    
    if(pt_ctx->ui2_idx_vp_first == pt_ctx->ui2_idx_vp_last)
    {
        *pui2_first = pt_ctx->ui2_idx_vp_first;
        *pui2_last  = pt_ctx->ui2_idx_vp_last;
        return VPR_OK;
    }
    else
    {
        ui2_first = pt_ctx->ui2_idx_vp_first;
        if(pt_ctx->i4_offset < 0 &&
           ((ui2_first+(UINT16)1) < pt_ctx->ui2_num_elem))
        {
            ui2_first ++;
        }
        
        ui2_last = pt_ctx->ui2_idx_vp_last;
        if( (INT32)pt_ctx->ui2_size_vp < _GET_LAST_ELEM_END_POS(pt_ctx) &&
            (ui2_last > 0))
        {
            ui2_last--;
        }
    }
    
    if(!(ui2_first < pt_ctx->ui2_num_elem &&
         ui2_last  < pt_ctx->ui2_num_elem &&
         ui2_first <= ui2_last ))
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        ui2_last = ui2_first;
    }
    *pui2_first  = ui2_first;
    *pui2_last   = ui2_last;

    return VPR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_vp_get_elem_num_base
 *
 * Description: This API gets the element number base.
 *
 * Inputs:  pt_ctx      Contains the viewport context.
 *
 * Outputs: -
 *
 * Returns: The element number base.
 ----------------------------------------------------------------------------*/
UINT16 wgl_vp_get_elem_num_base(
    IN      WGL_VP_CTX_T                *pt_ctx)
{
    return pt_ctx->ui2_vp_elem_num_base;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_vp_get_elem_num_base
 *
 * Description: This API gets the element number base.
 *
 * Inputs:  pt_ctx      Contains the viewport context.
 *
 * Outputs: -
 *
 * Returns: The element number base.
 ----------------------------------------------------------------------------*/
UINT16 wgl_vp_get_elem_num_efft(
    IN      WGL_VP_CTX_T                *pt_ctx)
{
    return pt_ctx->ui2_vp_elem_num_base;
}
/*-----------------------------------------------------------------------------
 * Name: wgl_vp_ensure_visible_prefer_center
 *
 * Description: This API ensures a specified element be visible and 
 *              the element will be put at the center of the viewport if possible.
 *
 * Inputs:  pt_ctx   Contains the viewport context.
 *          ui2_idx  Specifies the target element.
 *
 * Outputs: pt_ctx   Contains the new viewport context.
 *
 * Returns: VPR_OK            Routine successful.
 *          VPR_OUT_OF_RANGE  The ui2_idx is out of range.
 ----------------------------------------------------------------------------*/
INT32 wgl_vp_ensure_visible_prefer_center(
    IN OUT  WGL_VP_CTX_T                *pt_ctx,
    IN      UINT16                      ui2_idx)
{
    INT32   i4_first;
    UINT16  ui2_half_vp_elem_num_base = pt_ctx->ui2_vp_elem_num_base>>1;

    WS_ASSERT(ui2_idx < pt_ctx->ui2_num_elem);
    if( ui2_idx >= pt_ctx->ui2_num_elem )
    {
        return VPR_OUT_OF_RANGE;
    }

    if( pt_ctx->ui2_num_elem <= pt_ctx->ui2_vp_elem_num_base)
    {
        BOOL  b_temp;
        _vp_reset_case_1 (pt_ctx, &b_temp);
        return VPR_OK;
    }

    if( ui2_idx < ui2_half_vp_elem_num_base ||
        ui2_idx > (pt_ctx->ui2_num_elem-ui2_half_vp_elem_num_base) )
    {
        /* perform a minimal move */
        BOOL    b_temp;
        return wgl_vp_check_visibility_minimal_move(pt_ctx, ui2_idx, 0, TRUE, &b_temp,NULL);
    }

    i4_first = (INT32)ui2_idx - (INT32)(ui2_half_vp_elem_num_base);
    
    pt_ctx->i4_offset        = 0;
    pt_ctx->ui2_idx_vp_first = (i4_first<0) ? 0 : ((UINT16)i4_first);
    pt_ctx->ui2_idx_vp_last  = pt_ctx->ui2_idx_vp_first + pt_ctx->ui2_vp_elem_num_base - (UINT16)1 ;
    
    if(pt_ctx->ui2_idx_vp_last >= pt_ctx->ui2_num_elem)
    {
        pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_num_elem-(UINT16)1;
        _SAFE_SUB(pt_ctx->ui2_idx_vp_first, pt_ctx->ui2_idx_vp_last+(UINT16)1, pt_ctx->ui2_vp_elem_num_base);
        if(pt_ctx->ui2_size_vp > pt_ctx->ui2_vp_elem_total_height_base)
        {
            if(pt_ctx->ui2_idx_vp_first>0)
            {
                pt_ctx->ui2_idx_vp_first--;
                pt_ctx->i4_offset = (INT32)(pt_ctx->ui2_size_vp) - (INT32)(pt_ctx->ui2_vp_elem_total_height_base+pt_ctx->ui2_size_elem);
            }
            else
            {
                pt_ctx->i4_offset = 0;
            }
        }
        else
        {
            pt_ctx->i4_offset = (INT32)(pt_ctx->ui2_size_vp) - (INT32)(pt_ctx->ui2_vp_elem_total_height_base-pt_ctx->ui2_dist_elem);
        }

    }
    else if(pt_ctx->ui2_size_vp > pt_ctx->ui2_vp_elem_total_height_base)
    {
        if(pt_ctx->ui2_idx_vp_last+(UINT16)1 < pt_ctx->ui2_num_elem)
        {
            pt_ctx->ui2_idx_vp_last++;
        }
        else if(pt_ctx->ui2_idx_vp_first > 0)
        {
            pt_ctx->ui2_idx_vp_first--;
        }
    }

    _DEBUG_CHECK_VP_CTX(pt_ctx);

    return VPR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_vp_check_visibility_no_scroll
 *
 * Description: This API checks a specified elemen's visibility and ensure it to be visible in the viewport 
 *              with minimal movement by reserving a margin when b_ensure_visible equals TRUE.
 *
 * Inputs:  pt_ctx             Contains the viewport context.
 *          ui2_idx            Specifies the target element.
 *          ui2_margin         Specifies the margin. This argument must be less than ui2_vp_elem_num; 
 *                             otherwise, it would be truncated to (ui2_vp_elem_num - 1).
 *          b_ensure_visible   Specifies whether to ensure the elem to be visible.
 *
 * Outputs: pt_ctx             Contains the new viewport context.
 *          pb_is_vp_scrolled  Contains if the viewport is scrolled after this API.
 *
 * Returns: VPR_OK             Routine successful.
 *          VPR_OUT_OF_RANGE   The ui2_idx is out of range.
 ----------------------------------------------------------------------------*/
INT32 wgl_vp_check_visibility_minimal_move(
    IN OUT  WGL_VP_CTX_T                *pt_ctx,
    IN      UINT16                      ui2_idx,
    IN      UINT16                      ui2_margin,
    IN      BOOL                        b_ensure_visible,
    OUT     BOOL                        *pb_is_vp_scrolled,
    OUT     LB_VP_RANGE_INF_T           *pt_vp_range)
{
    INT32           i4_offset_old;          
    UINT16          ui2_idx_vp_first_old;
    UINT16          ui2_idx_vp_last_old;
    UINT16          ui2_margin_eff;
    
    *pb_is_vp_scrolled   = FALSE;
    
    CHECK_VALID( ui2_idx < pt_ctx->ui2_num_elem, VPR_OUT_OF_RANGE);
    if(pt_vp_range != NULL)
    {
        pt_vp_range->i4_offset = pt_ctx->i4_offset;
        pt_vp_range->ui2_idx_vp_first =  pt_ctx->ui2_idx_vp_first;
        pt_vp_range->ui2_idx_vp_last = pt_ctx->ui2_idx_vp_last;
    }
    
    /* quick return */
    if(pt_ctx->ui2_num_elem <= pt_ctx->ui2_vp_elem_num_base)
    {
        _vp_reset_case_1 (pt_ctx, pb_is_vp_scrolled);
        return VPR_OK;
    }
    
    /* prepare effective margin */
    ui2_margin_eff = (pt_ctx->ui2_vp_elem_num_base > 0) ? 
        MIN(ui2_margin, pt_ctx->ui2_vp_elem_num_base - (UINT16)1) : (UINT16)0;
    
    /* record original viewport context */
    i4_offset_old        = pt_ctx->i4_offset;
    ui2_idx_vp_first_old = pt_ctx->ui2_idx_vp_first;
    ui2_idx_vp_last_old  = pt_ctx->ui2_idx_vp_last;
    
    if( pt_ctx->ui2_vp_elem_num_base == 0 )
    {
        pt_ctx->i4_offset        = 0;
        pt_ctx->ui2_idx_vp_first = pt_ctx->ui2_idx_vp_last = ui2_idx;
    }
    else if( ui2_idx + ui2_margin_eff >= pt_ctx->ui2_idx_vp_last )
    {
        pt_ctx->ui2_idx_vp_last = MIN(ui2_idx + ui2_margin_eff, pt_ctx->ui2_num_elem - (UINT16)1);
        _SAFE_SUB(pt_ctx->ui2_idx_vp_first, (pt_ctx->ui2_idx_vp_last+(UINT16)1), pt_ctx->ui2_vp_elem_num_base);
        if(pt_ctx->ui2_size_vp > pt_ctx->ui2_vp_elem_total_height_base)
        {
            if( pt_ctx->ui2_idx_vp_first > (UINT16)0 )
            {
                pt_ctx->ui2_idx_vp_first --;
                pt_ctx->i4_offset = (INT32)(pt_ctx->ui2_size_vp) - (INT32)(pt_ctx->ui2_vp_elem_total_height_base + pt_ctx->ui2_size_elem);
            }
            else
            {
                pt_ctx->i4_offset = 0;
            }
        }
        else
        {
            pt_ctx->i4_offset = (INT32)(pt_ctx->ui2_size_vp) - (INT32)(pt_ctx->ui2_vp_elem_total_height_base - pt_ctx->ui2_dist_elem);
        }
    }
    else if(ui2_idx <= pt_ctx->ui2_idx_vp_first + ui2_margin_eff)
    {
        pt_ctx->i4_offset        = 0;
        pt_ctx->ui2_idx_vp_first = (ui2_idx > ui2_margin_eff) ? ui2_idx - ui2_margin_eff : (UINT16)0;
        pt_ctx->ui2_idx_vp_last  = pt_ctx->ui2_idx_vp_first + pt_ctx->ui2_vp_elem_num_base - (UINT16)1;
        if( pt_ctx->ui2_idx_vp_last >= pt_ctx->ui2_num_elem )
        {
            pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_num_elem - (UINT16)1;
        }
        else if( pt_ctx->ui2_size_vp > pt_ctx->ui2_vp_elem_total_height_base && 
                 (pt_ctx->ui2_idx_vp_last + (UINT16)1) < pt_ctx->ui2_num_elem)
        {
             pt_ctx->ui2_idx_vp_last ++;
        }
    }

    _DEBUG_CHECK_VP_CTX(pt_ctx);

    /*compare with old viewport context*/
    *pb_is_vp_scrolled = (  pt_ctx->i4_offset!=i4_offset_old                ||
                            pt_ctx->ui2_idx_vp_first!=ui2_idx_vp_first_old  ||
                            pt_ctx->ui2_idx_vp_last!=ui2_idx_vp_last_old) ? TRUE:FALSE;

    if(pt_vp_range != NULL)
    {
        pt_vp_range->i4_offset = pt_ctx->i4_offset;
        pt_vp_range->ui2_idx_vp_first =  pt_ctx->ui2_idx_vp_first;
        pt_vp_range->ui2_idx_vp_last = pt_ctx->ui2_idx_vp_last;
    }
    
    if (!b_ensure_visible)
    {
        pt_ctx->i4_offset = i4_offset_old;
        pt_ctx->ui2_idx_vp_first = ui2_idx_vp_first_old;
        pt_ctx->ui2_idx_vp_last = ui2_idx_vp_last_old;
    }

    return VPR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name: wgl_vp_check_visibility_no_scroll
 *
 * Description: This API ensures a specified element visible in changing page manner 
 *              instead of in scrolling manner.
 *
 * Inputs:  pt_ctx             Contains the viewport context.
 *          ui2_idx            Specifies the target element.
 *          b_ensure_visible   Specifies whether to ensure the elem to be visible
 *
 * Outputs: pt_ctx             Contains the new viewport context.
 *          pb_is_vp_scrolled  Contains if the viewport is scrolled after this API.
 *
 * Returns: VPR_OK             Routine successful.
 *          VPR_OUT_OF_RANGE   The ui2_idx is out of range.
 ----------------------------------------------------------------------------*/
INT32 wgl_vp_check_visibility_no_scroll(
    IN OUT  WGL_VP_CTX_T                *pt_ctx,
    IN      UINT16                      ui2_idx,
    IN      BOOL                        b_ensure_visible,
    OUT     BOOL                        *pb_is_vp_scrolled,
    OUT     LB_VP_RANGE_INF_T           *pt_vp_range)
{
    INT32           i4_offset_old;          
    UINT16          ui2_idx_vp_first_old;
    UINT16          ui2_idx_vp_last_old;
    
    *pb_is_vp_scrolled = FALSE;
    WS_ASSERT(ui2_idx<pt_ctx->ui2_num_elem);
    if(pt_vp_range != NULL)
    {
        pt_vp_range->i4_offset = pt_ctx->i4_offset;
        pt_vp_range->ui2_idx_vp_first =  pt_ctx->ui2_idx_vp_first;
        pt_vp_range->ui2_idx_vp_last = pt_ctx->ui2_idx_vp_last;
    }
    
    if( ui2_idx>=pt_ctx->ui2_num_elem )
    {
        return VPR_OUT_OF_RANGE;
    }
    
    if(pt_ctx->ui2_num_elem == 0)
    {
        return VPR_OK;
    }

    /*record old viewport context*/
    i4_offset_old        = pt_ctx->i4_offset;
    ui2_idx_vp_first_old = pt_ctx->ui2_idx_vp_first;
    ui2_idx_vp_last_old  = pt_ctx->ui2_idx_vp_last;
    
    if( pt_ctx->ui2_vp_elem_num_base==0 )
    {
        pt_ctx->i4_offset        = 0;
        pt_ctx->ui2_idx_vp_first = pt_ctx->ui2_idx_vp_last = ui2_idx;
    }
    else if(ui2_idx >= pt_ctx->ui2_idx_vp_last)
    {
        INT32 i4_le_end_pos = _GET_LAST_ELEM_END_POS(pt_ctx);
        if( ui2_idx > pt_ctx->ui2_idx_vp_last ||
            ( i4_le_end_pos > (INT32)(pt_ctx->ui2_size_vp+_HALF_ELEM_SIZE(pt_ctx))) )
        {   
            /*perform a page-down*/
            
            UINT16  ui2_idx_in_vp = ui2_idx % pt_ctx->ui2_vp_elem_num_eff;
            
            pt_ctx->ui2_idx_vp_first = ui2_idx - ui2_idx_in_vp;
            pt_ctx->ui2_idx_vp_last  = pt_ctx->ui2_idx_vp_first+pt_ctx->ui2_vp_elem_num_base-(UINT16)1;
            pt_ctx->i4_offset        = 0;
        
            if(pt_ctx->ui2_idx_vp_last >= pt_ctx->ui2_num_elem)
            {
                pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_num_elem - (UINT16)1;
            }
            else if(pt_ctx->ui2_size_vp > pt_ctx->ui2_vp_elem_total_height_base &&
                    (pt_ctx->ui2_idx_vp_last+(UINT16)1) < pt_ctx->ui2_num_elem )
            {
                pt_ctx->ui2_idx_vp_last++;
            }
        }
        else if(i4_le_end_pos <= (INT32)pt_ctx->ui2_size_vp)
        {   
            /*do nothing*/
            return WGLR_OK;
        }
        else
        {  /*perform a minimun move*/
            return wgl_vp_check_visibility_minimal_move(pt_ctx, ui2_idx, 0, b_ensure_visible, pb_is_vp_scrolled,NULL);
        }

    }
    else if(ui2_idx <= pt_ctx->ui2_idx_vp_first)
    {
        if( ui2_idx < pt_ctx->ui2_idx_vp_first ||
            (pt_ctx->i4_offset < -((INT32)_HALF_ELEM_SIZE(pt_ctx) )) )
        {   
            /*perform a page-up*/
            if(ui2_idx < pt_ctx->ui2_vp_elem_num_base)
            {
                pt_ctx->ui2_idx_vp_first = 0;
                pt_ctx->ui2_idx_vp_last  = pt_ctx->ui2_idx_vp_first + pt_ctx->ui2_vp_elem_num_base - (UINT16)1;
                pt_ctx->i4_offset        = 0;
                if(pt_ctx->ui2_idx_vp_last >= pt_ctx->ui2_num_elem)
                {
                    pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_num_elem-(UINT16)1;
                }
                else if(pt_ctx->ui2_size_vp > pt_ctx->ui2_vp_elem_total_height_base &&
                        pt_ctx->ui2_idx_vp_last+(UINT16)1 < pt_ctx->ui2_num_elem)
                {
                    pt_ctx->ui2_idx_vp_last++;
                }
            }
            else
            {
                UINT16  ui2_idx_in_vp = ui2_idx % pt_ctx->ui2_vp_elem_num_eff;
                
                pt_ctx->ui2_idx_vp_first = ui2_idx - ui2_idx_in_vp;
                pt_ctx->ui2_idx_vp_last  = pt_ctx->ui2_idx_vp_first+pt_ctx->ui2_vp_elem_num_base-(UINT16)1;
                pt_ctx->i4_offset        = 0;
            
                if(pt_ctx->ui2_idx_vp_last >= pt_ctx->ui2_num_elem)
                {
                    pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_num_elem - (UINT16)1;
                }
                else if(pt_ctx->ui2_size_vp > pt_ctx->ui2_vp_elem_total_height_base &&
                        (pt_ctx->ui2_idx_vp_last+(UINT16)1) < pt_ctx->ui2_num_elem )
                {
                    pt_ctx->ui2_idx_vp_last++;
                }
                
                if (ui2_idx == pt_ctx->ui2_idx_vp_last)
                {
                    /* adjust i4_offset to prevent half visible highlighted elem at the last of viewport */
                    pt_ctx->ui2_idx_vp_first = (pt_ctx->ui2_idx_vp_last+(UINT16)1) - pt_ctx->ui2_vp_elem_num_base;
                    pt_ctx->i4_offset        = 0;
                    
                    if(pt_ctx->ui2_size_vp > pt_ctx->ui2_vp_elem_total_height_base)
                    {
                        if(pt_ctx->ui2_idx_vp_first > 0)
                        {
                            pt_ctx->ui2_idx_vp_first--;
                            pt_ctx->i4_offset = ((INT32)pt_ctx->ui2_size_vp)-((INT32)(pt_ctx->ui2_vp_elem_total_height_base + pt_ctx->ui2_size_elem));
                        }
                    }
                }
            }
        }
        else if(pt_ctx->i4_offset >= 0)
        {
            /*do nothing*/
            return WGLR_OK;
        }
        else 
        {
            /*perform a minimun move*/
            return wgl_vp_check_visibility_minimal_move(pt_ctx, ui2_idx, 0, b_ensure_visible, pb_is_vp_scrolled,NULL);
        }

    }
        
    WS_ASSERT(ui2_idx>=pt_ctx->ui2_idx_vp_first && ui2_idx<=pt_ctx->ui2_idx_vp_last);
    _DEBUG_CHECK_VP_CTX(pt_ctx);

    /*compare with old viewport context*/
    *pb_is_vp_scrolled = (  pt_ctx->i4_offset!=i4_offset_old                ||
                            pt_ctx->ui2_idx_vp_first!=ui2_idx_vp_first_old  ||
                            pt_ctx->ui2_idx_vp_last!=ui2_idx_vp_last_old) ? TRUE:FALSE;
    
    if(pt_vp_range != NULL)
    {
        pt_vp_range->i4_offset = pt_ctx->i4_offset;
        pt_vp_range->ui2_idx_vp_first =  pt_ctx->ui2_idx_vp_first;
        pt_vp_range->ui2_idx_vp_last = pt_ctx->ui2_idx_vp_last;
    }
    
    if (!b_ensure_visible)
    {
        pt_ctx->i4_offset = i4_offset_old;
        pt_ctx->ui2_idx_vp_first = ui2_idx_vp_first_old;
        pt_ctx->ui2_idx_vp_last = ui2_idx_vp_last_old;
    }

    return VPR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: wgl_vp_move_visible
 *
 * Description: This API moves a specified element to be visible at specified position.
 *
 * Inputs:  pt_ctx            Contains the viewport context.
 *          ui2_idx           Specifies the target element.
 *          ui2_idx_new_pos   Specifies the expected visible position index.
 *
 * Outputs: pt_ctx            Contains the new viewport context.
 *
 * Returns: VPR_OK            Routine successful.
 *          VPR_OUT_OF_RANGE  The ui2_idx is out of range.
 ----------------------------------------------------------------------------*/
INT32 wgl_vp_move_visible(
    IN OUT  WGL_VP_CTX_T                *pt_ctx,
    IN      UINT16                      ui2_idx,
    IN      UINT16                      ui2_idx_new_pos)
{

    WS_ASSERT(ui2_idx < pt_ctx->ui2_num_elem);
    if( ui2_idx >= pt_ctx->ui2_num_elem )
    {
        return VPR_OUT_OF_RANGE;
    }
    if(pt_ctx->ui2_num_elem == 0)
    {
        return VPR_OK;
    }

    if(pt_ctx->ui2_vp_elem_num_base == 0)
    {
        pt_ctx->ui2_idx_vp_first = pt_ctx->ui2_idx_vp_last = ui2_idx;
        pt_ctx->i4_offset        = 0;
    }
    else
    {
        INT32  i4_idx_first = ((INT32)ui2_idx)-((INT32)ui2_idx_new_pos);
        if(i4_idx_first < 0)
        {
            i4_idx_first = 0;
        }

        pt_ctx->i4_offset        = 0;
        pt_ctx->ui2_idx_vp_first = (UINT16)i4_idx_first;
        pt_ctx->ui2_idx_vp_last  = pt_ctx->ui2_idx_vp_first + pt_ctx->ui2_vp_elem_num_base - (UINT16)1;
        if(pt_ctx->ui2_idx_vp_last >= pt_ctx->ui2_num_elem)
        {
            pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_num_elem-(UINT16)1;
        }
        else if(pt_ctx->ui2_size_vp > pt_ctx->ui2_vp_elem_total_height_base &&
                (pt_ctx->ui2_idx_vp_last+(UINT16)1 < pt_ctx->ui2_num_elem) )
        {
            pt_ctx->ui2_idx_vp_last ++;
        }
    }

    return VPR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_vp_show_last_page
 *
 * Description: This API makes the viewport to show the last page.
 *
 * Inputs:  pt_ctx             Contains the viewport context.
 *
 * Outputs: pt_ctx             Contains the new viewport context.
 *          pb_is_vp_scrolled  Contains if the viewport is scrolled after this API.
 *
 * Returns: VPR_OK             Routine successful.
 ----------------------------------------------------------------------------*/
INT32 wgl_vp_show_last_page(
    IN OUT  WGL_VP_CTX_T                *pt_ctx,
    OUT     BOOL                        *pb_is_vp_scrolled)
{
    INT32           i4_offset_old;          
    UINT16          ui2_idx_vp_first_old;
    UINT16          ui2_idx_vp_last_old;

    *pb_is_vp_scrolled = FALSE;
    
    if(pt_ctx->ui2_num_elem <= pt_ctx->ui2_vp_elem_num_base)
    {
        _vp_reset_case_1 (pt_ctx, pb_is_vp_scrolled);
        return VPR_OK;
    }

    /*record old viewport context*/
    i4_offset_old           = pt_ctx->i4_offset;
    ui2_idx_vp_first_old    = pt_ctx->ui2_idx_vp_first;
    ui2_idx_vp_last_old     = pt_ctx->ui2_idx_vp_last;

    pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_num_elem-(UINT16)1;
    if(pt_ctx->ui2_num_elem_residue>(UINT16)0)
    {
        _SAFE_SUB(pt_ctx->ui2_idx_vp_first, pt_ctx->ui2_idx_vp_last, pt_ctx->ui2_num_elem_residue-(UINT16)1);
    }
    pt_ctx->i4_offset = 0;
    
    _DEBUG_CHECK_VP_CTX(pt_ctx);
   
    /*compare with old viewport context*/
    *pb_is_vp_scrolled = (  pt_ctx->i4_offset!=i4_offset_old                ||
                            pt_ctx->ui2_idx_vp_first!=ui2_idx_vp_first_old  ||
                            pt_ctx->ui2_idx_vp_last!=ui2_idx_vp_last_old) ? TRUE:FALSE;

    return VPR_OK;
}

INT32 wgl_vp_change_element_size(
    IN OUT  WGL_VP_CTX_T                *pt_ctx,
    IN      BOOL                        b_size_by_num,
    IN      UINT16                      ui2_size,
    IN      UINT16                      ui2_num)
{
    if(b_size_by_num == TRUE)
    {
        pt_ctx->ui2_size_elem = (pt_ctx->ui2_size_vp / ui2_num) - pt_ctx->ui2_dist_elem;
        pt_ctx->i4_offset = (pt_ctx->ui2_size_vp - pt_ctx->ui2_size_elem * ui2_num) >> 1;
    }
    else
    {
        pt_ctx->ui2_size_elem = ui2_size;
       /* pt_ctx->i4_offset = 0;*/
    }

    _vp_update_vp_size_base_info(pt_ctx);
    _vp_update_elem_num_residue(pt_ctx);
    
    pt_ctx->ui2_idx_vp_last  = pt_ctx->ui2_idx_vp_first + pt_ctx->ui2_vp_elem_num_base - (UINT16)1;
    if( pt_ctx->ui2_idx_vp_last >= pt_ctx->ui2_num_elem )
    {
        pt_ctx->ui2_idx_vp_last = pt_ctx->ui2_num_elem-(UINT16)1;
    }
    else if( pt_ctx->ui2_size_vp > pt_ctx->ui2_vp_elem_total_height_base )
    {
        if(pt_ctx->ui2_idx_vp_last+ (UINT16)1 < pt_ctx->ui2_num_elem )
        {
            pt_ctx->ui2_idx_vp_last++;
        }
    }
    
    _DEBUG_CHECK_VP_CTX(pt_ctx);

    return VPR_OK;
}


INT32 wgl_vp_get_vp_size( 
    IN      WGL_VP_CTX_T                *pt_ctx,
    OUT     UINT16                      *pui2_vp_size) 
{

    _DEBUG_CHECK_VP_CTX(pt_ctx);
    if( pui2_vp_size == NULL )
    {
        return VPR_OK;
    }

    *pui2_vp_size = pt_ctx->ui2_size_vp;
    return VPR_OK;
}


