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
#include "inc/u_common.h"
#include "graphic/u_gl.h"
#include "graphic/gl.h"
#include "graphic/gl_dfb.h"
#include "handle/u_handle.h"
#include "stdio.h"

#if 0
#include <directfb.h>

void _mw_2_dfb_entries(const GL_COLOR_T * p_mw_entries, UINT16 ui2_count, DFBColor * p_dfb_entries)
{
    UINT16 i;
    for(i = 0 ; i < ui2_count; i ++)
    {
        p_dfb_entries[i].a = p_mw_entries[i].a;
        p_dfb_entries[i].r = p_mw_entries[i].u1.r;
        p_dfb_entries[i].g = p_mw_entries[i].u2.g;
        p_dfb_entries[i].b = p_mw_entries[i].u3.b;
    }
}

void _dfb_2_mw_entries( DFBColor * p_dfb_entries, UINT16 ui2_count, GL_COLOR_T * p_mw_entries)
{
    UINT16 i;
    for(i = 0 ; i < ui2_count; i ++)
    {
        p_mw_entries[i].a       = p_dfb_entries[i].a;
        p_mw_entries[i].u1.r    = p_dfb_entries[i].r;
        p_mw_entries[i].u2.g    = p_dfb_entries[i].g;
        p_mw_entries[i].u3.b    = p_dfb_entries[i].b;
    }
}
#endif

static BOOL
_pal_free_fct(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle)
{
    GL_DFB_BEGIN_FREE(h_handle, pv_obj)

        gl_dfb_obj_uninit(pv_obj);
        GL_FREE(pv_obj);

    GL_DFB_END_FREE(h_handle, pv_obj)
	return TRUE;
}

INT32 gl_dfb_palette_create(
    GL_COLORMODE_T      e_clrmode, 
    UINT16              ui2_count,
    const GL_COLOR_T    at_color[], 
    GL_HPALETTE_T*      ph_palette)
{
    GL_DFB_PAL_T *          p_pal;
    //INT32 i4_ret;
#if 0
	IDirectFBPalette *		p_dfb_pal;
	DFBPaletteDescription	dfb_pal_desc;
	DFBColor *				p_dfb_entries;
	

    dfb_pal_desc.flags = DPDESC_SIZE|DPDESC_ENTRIES;
    dfb_pal_desc.size = ui2_count;
    
    /*Copy the data from caller*/
    p_dfb_entries = (DFBColor *)GL_MALLOC(ui2_count * sizeof(DFBColor));
    _mw_2_dfb_entries(at_color, ui2_count, p_dfb_entries);
    dfb_pal_desc.entries = p_dfb_entries;

    DFBCHECK(g_p_dfb->CreatePalette (g_p_dfb, &dfb_pal_desc, &p_dfb_pal), &i4_ret);
 #endif
    p_pal = GL_MALLOC(sizeof(GL_DFB_PAL_T));
    GL_MEMSET(p_pal, 0, sizeof(GL_DFB_PAL_T));
    gl_dfb_obj_init(GLT_OBJ_PALETTE, p_pal);

    p_pal->e_clrmode = e_clrmode;
#if 0
    p_pal->p_dfb_pal = p_dfb_pal;
    /*p_pal->p_dfb_entries = p_dfb_entries;*/
    GL_FREE(p_dfb_entries);
#endif

    *ph_palette = GL_ALLOC_HANDLE(GLT_OBJ_PALETTE, p_pal, NULL, _pal_free_fct);
    
    return GLR_OK;
}

GL_HPALETTE_T gl_palette_duplicate_handle(GL_HPALETTE_T h_palette)
{
    return (gl_dfb_duplicate_handle(h_palette, GLT_OBJ_PALETTE, _pal_free_fct));
}

INT32 gl_dfb_palette_set_entries(
    GL_HPALETTE_T       h_palette, 
    UINT16              ui2_base, 
    UINT16              ui2_count, 
    const GL_COLOR_T    at_color[])
{
#if 0
    GL_DFB_PAL_T * p_pal;
    INT32 i4_ret;
	
    DFBColor * p_dfb_entries;
    p_pal = (GL_DFB_PAL_T *)gl_dfb_handle_lock_obj(GLT_OBJ_PALETTE, h_palette, &i4_ret);

    p_dfb_entries = GL_MALLOC(sizeof(DFBColor) * ui2_count);
    _mw_2_dfb_entries(at_color, ui2_count, p_dfb_entries);
    
    DFBCHECK(p_pal->p_dfb_pal->SetEntries (
        p_pal->p_dfb_pal,
     	p_dfb_entries,
    	ui2_count,
     	ui2_base), &i4_ret);
    
    gl_dfb_obj_unlock(p_pal);
#endif
    return GLR_OK;
}

INT32 gl_dfb_palette_get_entries(
    GL_HPALETTE_T       h_palette, 
    UINT16              ui2_base, 
    UINT16              ui2_count, 
    GL_COLOR_T          at_color[])
{
#if 0
    GL_DFB_PAL_T * p_pal;
    DFBColor * p_dfb_entries;
    INT32 i4_ret;

    p_pal = (GL_DFB_PAL_T *)gl_dfb_handle_lock_obj(GLT_OBJ_PALETTE, h_palette, &i4_ret);

    p_dfb_entries = GL_MALLOC(sizeof(DFBColor) * ui2_count);

    DFBCHECK(p_pal->p_dfb_pal->GetEntries (
        p_pal->p_dfb_pal,
     	p_dfb_entries,
    	ui2_count,
     	ui2_base), &i4_ret);

    _dfb_2_mw_entries(p_dfb_entries, ui2_count, at_color);

    GL_FREE(p_dfb_entries);
    gl_dfb_obj_unlock(p_pal);
#endif
    return GLR_OK;
}

INT32 gl_dfb_palette_match_color_ex(
    GL_HPALETTE_T       h_palette,
    UINT16              ui2_base,
    UINT16              ui2_count,
    BOOL                b_cmp_alpha,
    const GL_COLOR_T*   pt_to_match,
    GL_COLOR_T*         pt_matched,
    UINT16*             pui2_index)
{
#if 0
    GL_DFB_PAL_T *      p_pal;
    GL_COLOR_T *        p_color;
    UINT16              i;
    INT32               i4_diff;
    INT32               i4_sum_diff = 0;
    INT32               i4_min_sum = 0x7fffffff;
    UINT16              u2_idx = 0;
    INT32 i4_ret;

    p_pal = (GL_DFB_PAL_T *)gl_dfb_handle_lock_obj(GLT_OBJ_PALETTE, h_palette, &i4_ret);
    
    p_color = GL_MALLOC(sizeof(GL_COLOR_T) * ui2_count);

    gl_dfb_palette_get_entries(
        h_palette, 
        ui2_base, 
        ui2_count, 
        p_color);
    
    gl_dfb_obj_unlock(p_pal);

    for(i = 0 ; i < ui2_count; i ++)
    {
        i4_diff = ((INT32)(p_color[i].a) - (INT32)(pt_to_match->a));
        i4_sum_diff += i4_diff * i4_diff;
        i4_diff = ((INT32)(p_color[i].u1.r) - (INT32)(pt_to_match->u1.r));
        i4_sum_diff += i4_diff * i4_diff;
        i4_diff = ((INT32)(p_color[i].u2.g) - (INT32)(pt_to_match->u2.g));
        i4_sum_diff += i4_diff * i4_diff;
        i4_diff = ((INT32)(p_color[i].u3.b) - (INT32)(pt_to_match->u3.b));
        i4_sum_diff += i4_diff * i4_diff;
        
        if(i4_sum_diff < i4_min_sum)
        {
            i4_min_sum = i4_sum_diff;
            u2_idx = i;
        }
    }

    *pui2_index = ui2_base + u2_idx;
    GL_MEMCPY(pt_matched, p_color + u2_idx, sizeof(GL_COLOR_T));
    

    GL_FREE(p_color);
  #endif  
    return GLR_OK;
}


INT32 gl_dfb_palette_match_color(
    GL_HPALETTE_T       h_palette, 
    BOOL                b_cmp_alpha,
    const GL_COLOR_T*   pt_to_match,
    GL_COLOR_T*         pt_matched,
    UINT16*             pui2_index)
{ 
#if 0
    GL_DFB_PAL_T *      p_pal;
    DFBColor            dfb_color;
    unsigned int        ret_idx;
    unsigned int        u_count;
    INT32 i4_ret;

    p_pal = (GL_DFB_PAL_T *)gl_dfb_handle_lock_obj(GLT_OBJ_PALETTE, h_palette, &i4_ret);

    if(b_cmp_alpha)
    {
        DFBCHECK(p_pal->p_dfb_pal->FindBestMatch(
            p_pal->p_dfb_pal,
            pt_to_match->u1.r,
            pt_to_match->u2.g,
            pt_to_match->u3.b,
            pt_to_match->a,
            &ret_idx), &i4_ret);

        DFBCHECK(p_pal->p_dfb_pal->GetEntries(
            p_pal->p_dfb_pal,
            &dfb_color,
            1,
            0), &i4_ret);
        
        gl_dfb_obj_unlock(p_pal);

        *pui2_index = ret_idx;    

        GL_DFB_DFB_COLOR_2_GL_COLOR(&dfb_color, pt_matched);
    }
    else
    {
        DFBCHECK(p_pal->p_dfb_pal->GetSize(p_pal->p_dfb_pal, &u_count), &i4_ret);
        
        i4_ret = gl_dfb_palette_match_color_ex(
                h_palette,
                0,
                u_count,
                b_cmp_alpha,
                pt_to_match,
                pt_matched,
                pui2_index);

        gl_dfb_obj_unlock(p_pal);
    }
#endif
    return GLR_OK;
}


