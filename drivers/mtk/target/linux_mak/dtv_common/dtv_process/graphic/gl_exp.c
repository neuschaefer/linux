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
/*
 * THis file contains the functions in MW exported to other modules due some
 *  #$@%#%$$ reasons.
 *
 * */
#include "graphic/u_gl.h"
#include "graphic/gl.h"
#include "graphic/gl_dfb.h"



#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "stdio.h"

#include "graphic/_gl_cli.h"

INT32 gl_init(GL_COLORMODE_T e_colormode, UINT32 ui4_width, UINT32 ui4_height)
{
#if 0
    gl_cli_init();
#endif    
    return gl_dfb_init(e_colormode, ui4_width, ui4_height);
}

HANDLE_T gl_get_gpu()
{
    /*My god!!!*/
	GL_DFB_TRACE("gl_get_gpu\n");
	
    return NULL_HANDLE;
}
INT32 _flush_cmd(
    UINT8 ui1_num, const HANDLE_T* ph_signal, UINT32 ui4_flag)
{
    GL_DFB_TRACE("_flush_cmd\n");
	#ifdef ANDORID
	MTGFX_Flush();
	#endif
    return GLR_OK;
}
void gl_surface_get_clip_area(GL_DFB_SURFACE_T * p_surf, GL_RECT_T * p_rect)
{
    UINT16      ui2_cnt;
    GL_RECT_T   t_rect;

    GL_DFB_TRACE("gl_surface_get_clip_area\n");
    
    GL_MEMCPY(p_rect, &(p_surf->t_clip_rect), sizeof(GL_RECT_T));
    
    
    if (p_surf->t_desc.h_clipper)
    {
        if (GLR_OK == x_gl_clipper_get_clip_list(p_surf->t_desc.h_clipper, &ui2_cnt, &t_rect))
        {
            INTERSECT_RECT(p_rect, &t_rect, p_rect);
        }
    }
}

INT32 gl_is_same_plane(
        GL_HPLANE_T h_plane1,
        GL_HPLANE_T h_plane2, 
        BOOL* pb_cmp)
{
    GL_DFB_PLANE_T * p_plane1;
    GL_DFB_PLANE_T * p_plane2;

	GL_DFB_TRACE("gl_is_same_plane\n");
	

    p_plane1 = GL_HANDLE_GET_OBJ(GLT_OBJ_PLANE, h_plane1);
    p_plane2 = GL_HANDLE_GET_OBJ(GLT_OBJ_PLANE, h_plane2);
    
    *pb_cmp = (p_plane1 == p_plane2);


    return GLR_OK;
}

BOOL gl_is_valid_obj(GL_HOBJ_T h_obj, HANDLE_TYPE_T e_type)
{
    HANDLE_TYPE_T   e_hdl_type;
    GL_DFB_TRACE("gl_is_valid_obj\n");
	
    CHECK_VALID(h_obj != NULL_HANDLE, FALSE);
    CHECK_VALID(
        HR_OK == x_handle_get_type(h_obj, &e_hdl_type),
        FALSE);
    CHECK_VALID(e_type == e_hdl_type, FALSE);

    return TRUE;
}


VOID gl_surface_get_size(VOID* pv_surf, UINT32* pui4_width, UINT32* pui4_height)
{
	GL_DFB_TRACE("gl_surface_get_size\n");
	
    *pui4_width = ((GL_DFB_SURFACE_T*) pv_surf)->t_desc.ui4_width;
    *pui4_height = ((GL_DFB_SURFACE_T*) pv_surf)->t_desc.ui4_height;
}

GL_COLORMODE_T gl_surface_get_colormode(VOID* pv_surf)
{
	GL_DFB_TRACE("gl_surface_get_colormode\n");
	
    return (((GL_DFB_SURFACE_T*) pv_surf)->t_desc.e_clrmode);
}

HANDLE_T gl_palette_get_pal_handle(VOID* pv_palette)
{
	GL_DFB_TRACE("gl_palette_get_pal_handle\n");
	
    /*My god*/
    return NULL_HANDLE;
}

