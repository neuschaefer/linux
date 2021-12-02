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
 * $RCSfile: wgl_cvsst.c,v $
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
#include "wgl/wgl_cvsst.h"
#include "os/inc/x_os.h"
#include "graphic/x_gl.h"
#include "dbg/x_dbg.h"
#include "wgl/wgl_sets_cli.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_sets_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: wgl_cvsst_update
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID wgl_cvsst_update(
    WGL_CANVAS_STATUS_T*            pt_cvsst,
    const WGL_CANVAS_INFO_T*        pt_cvs_info,
    HANDLE_T                        h_palette)
{
    pt_cvsst->e_clr_mode = pt_cvs_info->e_clr_mode;
    x_memcpy(&pt_cvsst->t_colorkey, &pt_cvs_info->t_colorkey, sizeof(GL_COLOR_T));
    pt_cvsst->h_palette  = h_palette;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_cvsst_update_clr_cfg
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID wgl_cvsst_update_clr_cfg(
    WGL_CANVAS_STATUS_T*            pt_cvsst,
    const WGL_CVS_CLR_CFG_T*        pt_clr_cfg)
{
    if (pt_clr_cfg->ui4_flag & WGL_CANVAS_CFG_COLORKEY)
    {
        pt_cvsst->t_colorkey = pt_clr_cfg->t_colorkey;
    }
    
    if (pt_clr_cfg->ui4_flag & WGL_CANVAS_CFG_PALETTE)
    {
        pt_cvsst->h_palette  = pt_clr_cfg->h_palette;
    }
}
    
/*-----------------------------------------------------------------------------
 * Name: wgl_cvsst_copy
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID wgl_cvsst_copy(
    WGL_CANVAS_STATUS_T*            pt_cvsst_dest,
    const WGL_CANVAS_STATUS_T*      pt_cvsst_src)
{
    x_memcpy(pt_cvsst_dest, pt_cvsst_src, sizeof(WGL_CANVAS_STATUS_T));
}

/*-----------------------------------------------------------------------------
 * Name: wgl_cvsst_get_transparent_color
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID wgl_cvsst_get_transparent_color(
    const WGL_CANVAS_STATUS_T*       pt_cvsst,
    GL_COLOR_T*                      pt_clr_transp)
{    
    if(GL_IS_PALETTE_MODE(pt_cvsst->e_clr_mode))
    {
        *pt_clr_transp = pt_cvsst->t_colorkey;
    }
    else
    {
        x_memset(pt_clr_transp, 0, sizeof(GL_COLOR_T));
    }
}

/*-----------------------------------------------------------------------------
 * Name: wgl_cvsst_check_color_transparent
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
BOOL wgl_cvsst_check_color_transparent(
    const WGL_CANVAS_STATUS_T*       pt_cvsst,
    const GL_COLOR_T*                pt_clr)
{
    BOOL    b_clr_transp;
    
    b_clr_transp = FALSE;
    if(GL_IS_PALETTE_MODE(pt_cvsst->e_clr_mode))
    {
        if(pt_clr->u3.index == pt_cvsst->t_colorkey.u3.index)
        {
            b_clr_transp = TRUE;
        }
        else if(GL_IS_COLORMODE_WITH_ALPHA(pt_cvsst->e_clr_mode))
        {
            INT32       i4_ret;
            GL_COLOR_T  t_clr_target = {0};
            
            i4_ret = x_gl_palette_get_entries(
                pt_cvsst->h_palette,
                (UINT16)pt_clr->u3.index,
                1,
                &t_clr_target);
            if( i4_ret == GLR_OK )
            {
                if(t_clr_target.a == 0)
                {
                    b_clr_transp = TRUE;
                }
            }
            else
            {
                DBG_ERROR(("{WGL(TOOL)} ERR: palette get entry error. (%d) at L%d\n\n", i4_ret, __LINE__));
            }
        }
    }
    else
    {
        if( GL_IS_COLORMODE_WITH_ALPHA(pt_cvsst->e_clr_mode) )
        {
            if( pt_clr->a == 0 )
            {
                b_clr_transp = TRUE;
            }
        }
    }
    
    return b_clr_transp;
}    

/*-----------------------------------------------------------------------------
 * Name: wgl_cvsst_get_direct_color
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 wgl_cvsst_get_direct_color(
    const WGL_CANVAS_STATUS_T*       pt_cvsst,
    const GL_COLOR_T*                pt_clr_src,
    GL_COLOR_T*                      pt_dclr_direct)
{
    INT32                            i4_ret;
    
    if(GL_IS_PALETTE_MODE(pt_cvsst->e_clr_mode))
    {
        i4_ret = x_gl_palette_get_entries(
            pt_cvsst->h_palette,
            (UINT16)pt_clr_src->u3.index,
            1,
            pt_dclr_direct);
        if(i4_ret != GLR_OK)
        {
            DBG_ERROR(("{WGL(TOOL)} ERR: palette get entry error. (%d) at L%d\n\n", i4_ret, __LINE__));
            x_memset(pt_dclr_direct, 0, sizeof(GL_COLOR_T));
            return i4_ret;
        }
    }
    else
    {
        x_memcpy(pt_dclr_direct, pt_clr_src, sizeof(GL_COLOR_T));
    }
    
    return WGLR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name: wgl_cvsst_match_color
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 wgl_cvsst_match_color(
    const WGL_CANVAS_STATUS_T*       pt_cvsst,
    BOOL                             b_cmp_alpha,
    const GL_COLOR_T*                pt_clr_src,
    GL_COLOR_T*                      pt_clr_matched)
{
    if(GL_IS_PALETTE_MODE(pt_cvsst->e_clr_mode))
    {
        INT32       i4_ret;
        UINT16      ui2_clr_idx_matched;
        
        i4_ret = x_gl_palette_match_color(
            pt_cvsst->h_palette, 
            b_cmp_alpha,
            pt_clr_src,
            NULL,
            &ui2_clr_idx_matched);
        if(i4_ret != GLR_OK)
        {
            DBG_ERROR(("{WGL(TOOL)} ERR: Match color error. (%d) at L%d\n\n", i4_ret, __LINE__));
            wgl_cvsst_clear_color(pt_cvsst, pt_clr_matched);
            return i4_ret;
        }
        pt_clr_matched->u3.index = (UINT8)ui2_clr_idx_matched;
    }
    else
    {
        *pt_clr_matched = *pt_clr_src;
    }
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_cvsst_clear_color
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID wgl_cvsst_clear_color(
    const WGL_CANVAS_STATUS_T*       pt_cvsst,
    GL_COLOR_T*                      pt_clr)
{
    wgl_cvsst_get_transparent_color(pt_cvsst, pt_clr);
}    

/*-----------------------------------------------------------------------------
 * Name: wgl_cvsst_compare_color
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
BOOL wgl_cvsst_compare_color(
    const WGL_CANVAS_STATUS_T*       pt_cvsst,
    const GL_COLOR_T*                pt_clr_1,
    const GL_COLOR_T*                pt_clr_2)
{
    BOOL    b_same;
    
    if (GL_IS_PALETTE_MODE(pt_cvsst->e_clr_mode))
    {
        b_same = (pt_clr_1->u3.index == pt_clr_2->u3.index) ? TRUE : FALSE;
    }
    else
    {
        b_same = (pt_clr_1->a    == pt_clr_2->a      &&
                  pt_clr_1->u1.r == pt_clr_2->u1.r   &&
                  pt_clr_1->u2.g == pt_clr_2->u2.g   &&
                  pt_clr_1->u3.b == pt_clr_2->u3.b  ) ? TRUE : FALSE;
    }
     
    return b_same;    
}    
