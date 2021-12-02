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
 * $RCSfile: wgl_lb_sb.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/7 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: f615a228487dbf815141dc7f8fdacc09 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "wgl/listbox/wgl_lb_sb.h"
#include "os/inc/x_os.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MUL_DIV_UINT32(_ui4_src, _ui4_numerator, _ui4_denominator)          \
        ((UINT32)(((DOUBLE)(_ui4_src)) * ((DOUBLE)(_ui4_numerator)) / ((DOUBLE)(_ui4_denominator))))
        

/*-----------------------------------------------------------------------------
                    internal functions declarations
 ----------------------------------------------------------------------------*/
static VOID _lb_sb_update_status (
    IN OUT  LB_SB_CTX_T                 *pt_ctx,
    IN      UINT32                      ui4_refz_vp,
    IN      UINT32                      ui4_refz_total,
    IN      UINT32                      ui4_refz_vp_top);

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: wgl_lb_sb_init
 *
 * Description: This API initializes the instance context for 
 *              scroll bar control logic.
 *
 * Inputs:  -
 *
 * Outputs: pt_ctx   Contains the initialized instance context.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID wgl_lb_sb_init(
    OUT     LB_SB_CTX_T                 *pt_ctx)
{
    pt_ctx->ui2_style        = 0;
    pt_ctx->ui2_state        = SBST_DISABLED;
    pt_ctx->ui4_sz_track     = 0;
    pt_ctx->ui4_sz_thumb_min = 0;
    pt_ctx->ui4_sz_elem      = 0;
    pt_ctx->ui4_ds_elem      = 0;
    
    /* AUTO mode at init */
    pt_ctx->u_mode.t_auto.ui4_sz_vp        = 0;
    pt_ctx->u_mode.t_auto.ui2_num_elem     = 0;
    pt_ctx->u_mode.t_auto.ui2_first        = 0;
    pt_ctx->u_mode.t_auto.i4_vp_top_first  = 0;
    
    pt_ctx->ui4_ofs_thumb    = 0;
    pt_ctx->ui4_sz_thumb     = 0;
    
}

/*-----------------------------------------------------------------------------
 * Name: wgl_lb_sb_deinit
 *
 * Description: This API de-initializes the instance context for 
 *              scroll bar control logic.
 *
 * Inputs:  pt_ctx   Contains the instance context.
 *
 * Outputs: pt_ctx   Contains the de-initialized instance context.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID wgl_lb_sb_deinit(
    IN OUT  LB_SB_CTX_T                 *pt_ctx)
{
    x_memset (pt_ctx, 0, sizeof (LB_SB_CTX_T));
}

/*-----------------------------------------------------------------------------
 * Name: wgl_lb_sb_set_attrs
 *
 * Description: This API sets the attributes.
 *
 * Inputs:  pt_ctx    Contains the instance context.
 *          pt_attrs  Contains the set of new attributes.
 *
 * Outputs: pt_ctx    Contains the changed instance context.
 *
 * Returns: WGLR_OK       Routine successful.
 *          WGLR_INV_ARG  Invalid arguments.
 *          < 0           Some other errors.
 ----------------------------------------------------------------------------*/
INT32 wgl_lb_sb_set_attrs(
    IN OUT  LB_SB_CTX_T                 *pt_ctx,
    IN      LB_SB_ATTRS_T               *pt_attrs)
{
    CHECK_VALID(
        !(pt_attrs->ui2_style & (~((SB_STL_LAST_STYLE << 1) - 1))) &&
        pt_attrs->ui4_sz_track > 0                                 &&
        pt_attrs->ui4_sz_thumb_min > 0                             &&
        pt_attrs->ui4_sz_thumb_min < pt_attrs->ui4_sz_track        &&
        pt_attrs->ui4_sz_elem > 0, WGLR_INV_ARG);
    
    if(pt_attrs->ui2_style == pt_ctx->ui2_style               &&
       pt_attrs->ui4_sz_track == pt_ctx->ui4_sz_track         &&
       pt_attrs->ui4_sz_thumb_min == pt_ctx->ui4_sz_thumb_min &&
       pt_attrs->ui4_sz_elem == pt_ctx->ui4_sz_elem           &&
       pt_attrs->ui4_ds_elem == pt_ctx->ui4_ds_elem )
    {
        return WGLR_OK;
    }
    
    /* update the attrs */
    pt_ctx->ui2_style        = (UINT16)pt_attrs->ui2_style;
    pt_ctx->ui4_sz_track     = pt_attrs->ui4_sz_track;
    pt_ctx->ui4_sz_thumb_min = pt_attrs->ui4_sz_thumb_min;
    pt_ctx->ui4_sz_elem      = pt_attrs->ui4_sz_elem;
    pt_ctx->ui4_ds_elem      = pt_attrs->ui4_ds_elem;
    
    WGL_SET_FLAG(pt_ctx->ui2_state, SBST_DIRTY);
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_lb_sb_update_vp
 *
 * Description: This API updates the vp related parameters.
 *
 * Inputs:  pt_ctx           Contains the instance context.
 *          ui4_sz_vp        Specifies the size of the viewport
 *          ui2_num_elem     Specifies the element number.
 *          ui2_first        Specifies the index of the first visible element.
 *          i4_vp_top_first  Specifies the offset of the first visible element's top 
 *                           in vp coords. This argument should be less than or equal to zero.
 *
 * Outputs: pt_ctx           Contains the changed instance context.
 *
 * Returns: WGLR_OK       Routine successful.
 *          WGLR_INV_ARG  Invalid arguments.
 *          < 0           Some other errors.
 ----------------------------------------------------------------------------*/
INT32 wgl_lb_sb_update_vp(
    IN OUT  LB_SB_CTX_T                 *pt_ctx,
    IN      UINT32                      ui4_sz_vp,
    IN      UINT16                      ui2_num_elem,
    IN      UINT16                      ui2_first,
    IN      INT32                       i4_vp_top_first)
{
    /* check if in AUTO mode */
    CHECK_VALID(!HAS_FLAG(pt_ctx->ui2_style, SB_STL_MANUAL_MODE), WGLR_INV_CMD);
    
    /* check parameters */
    CHECK_VALID(
        ui4_sz_vp > 0            &&
        ui2_first < ui2_num_elem &&
        i4_vp_top_first <= 0, WGLR_INV_ARG);

    if( ui4_sz_vp == pt_ctx->u_mode.t_auto.ui4_sz_vp           &&
        ui2_num_elem == pt_ctx->u_mode.t_auto.ui2_num_elem     &&
        ui2_first == pt_ctx->u_mode.t_auto.ui2_first           &&
        i4_vp_top_first == pt_ctx->u_mode.t_auto.i4_vp_top_first)
    {
        return WGLR_OK;
    }
    
    /* update the vp */
    pt_ctx->u_mode.t_auto.ui4_sz_vp       = ui4_sz_vp;
    pt_ctx->u_mode.t_auto.ui2_num_elem    = ui2_num_elem;
    pt_ctx->u_mode.t_auto.ui2_first       = ui2_first;
    pt_ctx->u_mode.t_auto.i4_vp_top_first = i4_vp_top_first;
    
    WGL_SET_FLAG(pt_ctx->ui2_state, SBST_DIRTY);
    
    return WGLR_OK;
} 


/*-----------------------------------------------------------------------------
 * Name: wgl_lb_sb_set_status
 *
 * Description: This API update/override the status of the scroll bar.
 *
 * Inputs:  pt_ctx           Contains the instance context.
 *          ui4_refz_vp      Contains the reference size of vp
 *          ui4_refz_total   Contains the reference size of total elements
 *          ui4_refz_vp_top  Contains the reference position of the vp's top.
 *
 * Outputs: pt_ctx          Contains the changed instance context.
 *
 * Returns: WGLR_OK              Routine successful.
 *          < 0                  Some other errors.
 ----------------------------------------------------------------------------*/
INT32 wgl_lb_sb_set_status(
    IN OUT  LB_SB_CTX_T                 *pt_ctx,
    IN      UINT32                      ui4_refz_vp,
    IN      UINT32                      ui4_refz_total,
    IN      UINT32                      ui4_refz_vp_top)
{
    /* check if in MANUAL mode */
    CHECK_VALID(HAS_FLAG(pt_ctx->ui2_style, SB_STL_MANUAL_MODE), WGLR_INV_CMD);

    /* check for quick return */
    if(pt_ctx->ui4_sz_track == 0)
    {
        return WGLR_OK;
    }

    pt_ctx->u_mode.t_manual.ui4_ref_sz_vp     = ui4_refz_vp;
    pt_ctx->u_mode.t_manual.ui4_ref_sz_total  = ui4_refz_total;
    pt_ctx->u_mode.t_manual.ui4_ref_sz_vp_top = ui4_refz_vp_top;

    WGL_SET_FLAG(pt_ctx->ui2_state, SBST_DIRTY);
    
    return WGLR_OK;
}
        
/*-----------------------------------------------------------------------------
 * Name: wgl_lb_sb_get_thumb
 *
 * Description: This API gets current location & size of the thumb.
 *
 * Inputs:  pt_ctx   Contains the instance context.
 *
 * Outputs: pt_ctx          Contains the changed instance context.
 *          pb_disabled     Contains whether the scroll bar should be disabled or not.
 *          pui4_ofs_thumb  Contains the offset of the thumb. 
 *                          The offset is zero if the scroll bar is disabled.
 *          pui4_sz_thumb   Contains the size of the thumb.
 *                          The size is zero if the scroll bar is disabled.
 *
 * Returns: WGLR_OK              Routine successful.
 *          WGLR_INTERNAL_ERROR  Internal errors indicating that something should not happen.
 *          < 0                  Some other errors.
 ----------------------------------------------------------------------------*/
INT32 wgl_lb_sb_get_thumb(
    IN      LB_SB_CTX_T                 *pt_ctx,
    OUT     BOOL                        *pb_disabled,
    OUT     UINT32                      *pui4_ofs_thumb,
    OUT     UINT32                      *pui4_sz_thumb)
{
    
    *pb_disabled    = TRUE;
    *pui4_ofs_thumb = 0;
    *pui4_sz_thumb  = 0;
    
    if(pt_ctx->ui4_sz_track == 0)
    {
        return WGLR_OK;
    }
    
    if(HAS_FLAG(pt_ctx->ui2_state, SBST_DIRTY))
    {
        if (HAS_FLAG (pt_ctx->ui2_style, SB_STL_MANUAL_MODE))
        {
            _lb_sb_update_status (pt_ctx, 
                                  pt_ctx->u_mode.t_manual.ui4_ref_sz_vp,  
                                  pt_ctx->u_mode.t_manual.ui4_ref_sz_total,  
                                  pt_ctx->u_mode.t_manual.ui4_ref_sz_vp_top);
        }
        else
        {
            UINT32  ui4_sz_total;
            INT32   i4_top_vp_logical;
            
            ui4_sz_total = pt_ctx->u_mode.t_auto.ui2_num_elem > 0 ?
                ((UINT32)pt_ctx->u_mode.t_auto.ui2_num_elem) * (pt_ctx->ui4_sz_elem + pt_ctx->ui4_ds_elem) - pt_ctx->ui4_ds_elem : 0;
            
            if(pt_ctx->u_mode.t_auto.ui2_first > 0)
            {
                i4_top_vp_logical = (((INT32)pt_ctx->u_mode.t_auto.ui2_first) * ((INT32)(pt_ctx->ui4_sz_elem + pt_ctx->ui4_ds_elem))) - pt_ctx->u_mode.t_auto.i4_vp_top_first;
            }
            else
            {
                i4_top_vp_logical = - pt_ctx->u_mode.t_auto.i4_vp_top_first;
            }
            
            CHECK_VALID(i4_top_vp_logical >= 0, WGLR_INTERNAL_ERROR);
            
            _lb_sb_update_status (pt_ctx, 
                                  pt_ctx->u_mode.t_auto.ui4_sz_vp,  
                                  ui4_sz_total,
                                  (UINT32) i4_top_vp_logical );
        }
        
        WGL_CLEAR_FLAG(pt_ctx->ui2_state, SBST_DIRTY);
    }
    
    *pb_disabled    = HAS_FLAG(pt_ctx->ui2_state, SBST_DISABLED) ? TRUE : FALSE;
    *pui4_ofs_thumb = pt_ctx->ui4_ofs_thumb;
    *pui4_sz_thumb  = pt_ctx->ui4_sz_thumb;
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lb_sb_update_status
 *
 * Description: This API update/override the status of the scroll bar.
 *
 * Inputs:  pt_ctx           Contains the instance context.
 *          ui4_refz_vp      Contains the reference size of vp
 *          ui4_refz_total   Contains the reference size of total elements. It may be zero
 *          ui4_refz_vp_top  Contains the reference position of the vp's top. 
 *                           If ui4_refz_total == 0, ui4_refz_vp_top must be 0.
 *
 * Outputs: pt_ctx          Contains the changed instance context.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lb_sb_update_status (
    IN OUT  LB_SB_CTX_T                 *pt_ctx,
    IN      UINT32                      ui4_refz_vp,
    IN      UINT32                      ui4_refz_total,
    IN      UINT32                      ui4_refz_vp_top)
{
    BOOL    b_disabled;
    UINT32  ui4_ofs_thumb, ui4_sz_thumb;
    INT32   i4_ret;
    
    if (!(ui4_refz_total > 0 || ui4_refz_vp_top == 0))
    {
        INFORM_ERR (WGLR_INV_ARG);   
    }
    
    if (ui4_refz_total > ui4_refz_vp)
    {
        /* calculate the size of thumb */
        if(HAS_FLAG(pt_ctx->ui2_style, SB_STL_FIXED_THUMB_SIZE))
        {
            ui4_sz_thumb = pt_ctx->ui4_sz_thumb_min;
        }
        else
        {
            ui4_sz_thumb = MUL_DIV_UINT32(pt_ctx->ui4_sz_track, ui4_refz_vp, ui4_refz_total);
            ui4_sz_thumb = MAX(ui4_sz_thumb, pt_ctx->ui4_sz_thumb_min);
        }
        
        b_disabled = ui4_sz_thumb < pt_ctx->ui4_sz_track ? FALSE : TRUE;
    }
    else
    {
        b_disabled = TRUE;
    }
    
    if(!b_disabled)
    {
        /* calculate ui4_ofs_thumb */
        if (ui4_refz_vp_top + ui4_refz_vp >= ui4_refz_total)
        {
            /* put the thumb at the very bottom */
            ui4_ofs_thumb = pt_ctx->ui4_sz_track - ui4_sz_thumb;
        }
        else if (ui4_refz_vp_top == 0)
        {
            /* put the thumb at the very top */
            ui4_ofs_thumb = 0;
        }
        else
        {
            /* condition - 1: pt_ctx->ui4_sz_track > ui4_sz_thumb */
            /* condition - 2: ui4_refz_total > ui4_refz_vp */
            UINT32  ui4_sz_movable_sb      = pt_ctx->ui4_sz_track - ui4_sz_thumb;
            UINT32  ui4_sz_movable_logical = ui4_refz_total - ui4_refz_vp;
            
            ui4_ofs_thumb = MUL_DIV_UINT32(ui4_sz_movable_sb, ui4_refz_vp_top, ui4_sz_movable_logical);
            
            /* make sure the thumb would not go out of the bottom */
            ui4_ofs_thumb = MIN(ui4_ofs_thumb, pt_ctx->ui4_sz_track - ui4_sz_thumb);
        }
        
    }
    else
    {
        ui4_ofs_thumb = 0;
        ui4_sz_thumb  = 0;
    }
 
    /* update the state */
    if(b_disabled)
    {
        WGL_SET_FLAG(pt_ctx->ui2_state, SBST_DISABLED);
    }
    else
    {
        WGL_CLEAR_FLAG(pt_ctx->ui2_state, SBST_DISABLED);
    }
    
    pt_ctx->ui4_ofs_thumb = ui4_ofs_thumb;
    pt_ctx->ui4_sz_thumb  = ui4_sz_thumb;
    
}




