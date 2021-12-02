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
#include "u_gl.h"
#include "gl.h"
#include "gl_dfb.h"
#include "gl_dfb_util.h"

#include "handle/u_handle.h"
#include "stdio.h"

#if 0
#include <directfb.h>
#endif


static BOOL
_clipper_free_fct(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle)
{
	GL_DFB_CLIPPER_T * p_clipper = pv_obj;   
     
    GL_DFB_BEGIN_FREE(h_handle, pv_obj)
		
		gl_dfb_dy_vec_destroy( p_clipper->p_rect_vec);

        gl_dfb_obj_uninit(pv_obj);
        GL_FREE(pv_obj);

    GL_DFB_END_FREE(h_handle, pv_obj)
	return TRUE;
}

INT32 gl_dfb_clipper_create(
    UINT16              ui2_count, 
    const GL_RECT_T*    pt_rect, 
    GL_HCLIPPER_T*      ph_clipper)
{
    UINT16 i;
    GL_DFB_CLIPPER_T * p_clipper;   
    p_clipper = (GL_DFB_CLIPPER_T *)GL_MALLOC(sizeof(GL_DFB_CLIPPER_T));
    GL_MEMSET(p_clipper, 0, sizeof(GL_DFB_CLIPPER_T));
    gl_dfb_obj_init(GLT_OBJ_CLIPPER, p_clipper);

    p_clipper->p_rect_vec = gl_dfb_dy_vec_init(4, sizeof(GL_RECT_T));

    if(pt_rect != NULL)
    {
        for(i = 0; i < ui2_count; i++)
        {
            gl_dfb_dy_vec_add(p_clipper->p_rect_vec, (VOID *)pt_rect);
        }
    }
    *ph_clipper = GL_ALLOC_HANDLE(GLT_OBJ_CLIPPER, p_clipper, NULL, _clipper_free_fct);

    return GLR_OK;
}

HANDLE_T gl_clipper_duplicate_handle(HANDLE_T h_src)
{
    return gl_dfb_duplicate_handle(h_src, GLT_OBJ_CLIPPER, _clipper_free_fct);
}

INT32 gl_dfb_clipper_set_clip_list(
    GL_HCLIPPER_T       h_clipper, 
    UINT16              ui2_count, 
    const GL_RECT_T*    pt_rect)
{
	GL_DFB_CLIPPER_T * p_clipper;
	INT32 i4_ret;
	
	p_clipper = gl_dfb_handle_lock_obj(GLT_OBJ_CLIPPER, h_clipper, &i4_ret);
    if(i4_ret != GLR_OK)
    {		
        return i4_ret;
    } 
	gl_dfb_dy_vec_set(p_clipper->p_rect_vec, 0, (VOID *)pt_rect);        

	gl_dfb_obj_unlock(p_clipper);
	
	return GLR_OK;
}
INT32 gl_dfb_clipper_get_clip_list(
    GL_HCLIPPER_T       h_clipper, 
    UINT16*             pui2_count, 
    GL_RECT_T*          pt_rect)
{
    GL_DFB_CLIPPER_T * p_clipper;
    INT32 i4_ret;
    
    p_clipper = gl_dfb_handle_lock_obj(GLT_OBJ_CLIPPER, h_clipper, &i4_ret);
    if(i4_ret != GLR_OK)
    {       
        return i4_ret;
    } 

    gl_dfb_dy_vec_get(p_clipper->p_rect_vec, 0, (VOID *)pt_rect);        

    *pui2_count = 1;    
    
    gl_dfb_obj_unlock(p_clipper);
    
    return GLR_OK;

}


