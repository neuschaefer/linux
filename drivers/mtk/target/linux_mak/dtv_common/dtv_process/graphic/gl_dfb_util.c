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
#else
#include "mtgfx.h"
#endif



/*Dynamic vector functions*/
GL_DFB_DY_VEC_T * gl_dfb_dy_vec_init(size_t z_max_count, size_t z_itm_size)
{   
    GL_DFB_DY_VEC_T * p_vec;
    p_vec = (GL_DFB_DY_VEC_T *)GL_MALLOC(sizeof(GL_DFB_DY_VEC_T));
    p_vec->z_len        = z_max_count * z_itm_size;
    p_vec->z_count      = 0;
    p_vec->z_itm_size   = z_itm_size;
    p_vec->p_data       = GL_MALLOC(p_vec->z_len);

    return p_vec;
}

BOOL gl_dfb_dy_vec_add(GL_DFB_DY_VEC_T * p_vec, VOID * p)
{
    size_t z_used_size;
    z_used_size = p_vec->z_count * p_vec->z_itm_size;

    if(z_used_size >= p_vec->z_len)
    {
        p_vec->z_len += p_vec->z_itm_size * 8;
        p_vec->p_data = GL_REALLOC(p_vec->p_data, p_vec->z_len);
    }
    GL_MEMCPY((char *)(p_vec->p_data) + z_used_size, p, p_vec->z_itm_size);
    p_vec->z_count ++;    

    return TRUE;
}

BOOL gl_dfb_dy_vec_find(GL_DFB_DY_VEC_T * p_vec, VOID * p, UINT32 * pui4_idx)
{
    for(*pui4_idx = 0; *pui4_idx < p_vec->z_count; (*pui4_idx) ++)
    {
        if(GL_MEMCMP(p, ((char *)p_vec->p_data) + p_vec->z_itm_size * (*pui4_idx), p_vec->z_itm_size) == 0)
        {
            return TRUE;
        }
    }    
    return FALSE;    
}

BOOL gl_dfb_dy_vec_remove(GL_DFB_DY_VEC_T * p_vec, UINT32 ui4_idx)
{
    size_t z_offset;
    z_offset = ui4_idx * p_vec->z_itm_size;

    if(p_vec->z_count == 0)
    {
        return FALSE;
    }
    if(ui4_idx < p_vec->z_count - 1)
    {
        GL_MEMCPY(((char *)p_vec->p_data) + z_offset, 
            ((char *)p_vec->p_data) + z_offset + p_vec->z_itm_size, 
            p_vec->z_len - p_vec->z_itm_size - z_offset );
    }
    p_vec->z_count --;  
    
    return TRUE;
}

BOOL gl_dfb_dy_vec_get(GL_DFB_DY_VEC_T * p_vec, UINT32 ui4_idx,  VOID *p)
{
    if(p_vec->z_count <= ui4_idx)
    {
        return FALSE;
    }
    GL_MEMCPY(p, ((char *)p_vec->p_data) + ui4_idx * p_vec->z_itm_size, p_vec->z_itm_size);
    return TRUE;
}


BOOL gl_dfb_dy_vec_set(GL_DFB_DY_VEC_T * p_vec, UINT32 ui4_idx, VOID * p)
{
    if(p_vec->z_count <= ui4_idx)
    {
        return FALSE;
    }
    GL_MEMCPY(((char *)p_vec->p_data) + ui4_idx * p_vec->z_itm_size, p, p_vec->z_itm_size);
    return TRUE;
}

VOID gl_dfb_dy_vec_destroy(GL_DFB_DY_VEC_T * p_vec)
{
    GL_FREE(p_vec->p_data);
    GL_FREE(p_vec);
}

#if 0
INT32 gl_dfb_ret_2_gl(DFBResult dfb_ret)
{
    switch(dfb_ret)
    {
        case DFB_OK:  	    /*No error occured.*/
            return GLR_OK;
        case DFB_FAILURE:    /*A general or unknown error occured.*/
        case DFB_INIT: 	    /*A general initialization error occured.*/
        case DFB_BUG: 	    /*Internal bug or inconsistency has been detected.*/
        case DFB_DEAD:     /*	Interface has a zero reference counter (available in debug mode).*/            
        case DFB_UNSUPPORTED:     /*	The requested operation or an argument is (currently) not supported.*/
        case DFB_UNIMPLEMENTED:     /*	The requested operation is not implemented, yet.*/
        case DFB_ACCESSDENIED:     /*	Access to the resource is denied.*/
        case DFB_INVARG:     /*	An invalid argument has been specified.*/
            return GLR_DRV_ERROR;
        case DFB_NOSYSTEMMEMORY:     /*	There's not enough system memory.*/
        case DFB_NOVIDEOMEMORY:     /*	There's not enough video memory.*/
            return GLR_NO_MEMORY;
        case DFB_LOCKED:     /*	The resource is (already) locked.*/
            return GLR_SURFACE_LOCKED;
        case DFB_BUFFEREMPTY:     /*	The buffer is empty.*/
        case DFB_FILENOTFOUND:     /*	The specified file has not been found.*/
        case DFB_IO:     /*	A general I/O error occured.*/
            return GLR_DRV_ERROR;
        case DFB_BUSY:     /*	The resource or device is busy.*/
            return GLR_SURFACE_BUSY;/*Fix me: not only surface migth be busy*/
        case DFB_NOIMPL:     /*	No implementation for this interface or content type has been found.*/
            return GLR_NO_IMP;
		case DFB_MISSINGFONT:     /*	No font has been set.*/
        case DFB_TIMEOUT:     /*	The operation timed out.*/
        case DFB_MISSINGIMAGE:     /*	No image has been set.*/
        case DFB_THIZNULL:     /*	'thiz' pointer is NULL.*/
        case DFB_IDNOTFOUND:     /*	No resource has been found by the specified id.*/
			return GLR_DRV_ERROR;
        case DFB_INVAREA:     /*	An invalid area has been specified or detected.*/
			return GLR_OK;			
        case DFB_DESTROYED:     /*	The underlying object (e.g. a window or surface) has been destroyed.*/
        case DFB_FUSION:     /*	Internal fusion error detected, most likely related to IPC resources.*/
        case DFB_BUFFERTOOLARGE:     /*	Buffer is too large.*/
        case DFB_INTERRUPTED:     /*	The operation has been interrupted.*/
        case DFB_NOCONTEXT:     /*	No context available.*/
        case DFB_TEMPUNAVAIL:     /*	Temporarily unavailable.*/
        case DFB_LIMITEXCEEDED:     /*	Attempted to exceed limit, i.e. any kind of maximum size, count etc.*/
        case DFB_NOSUCHMETHOD:     /*	Requested method is not known to remote site.*/
        case DFB_NOSUCHINSTANCE:     /*	Requested instance is not known to remote site.*/
        case DFB_ITEMNOTFOUND:     /*	No such item found.*/
        case DFB_VERSIONMISMATCH:     /*	Some versions didn't match.*/
        case DFB_NOSHAREDMEMORY:     /*	There's not enough shared memory.*/
        case DFB_EOF:     /*	Reached end of file.*/
        case DFB_SUSPENDED:     /*	The requested object is suspended.*/
        case DFB_INCOMPLETE:     /*	The operation has been executed, but not completely.*/
            return GLR_DRV_ERROR;
        default:
            return GLR_DRV_ERROR;
    }
}

#endif

#if 1
UINT8 gl_cm_to_mtal_cm(GL_COLORMODE_T e_cm)
{
	switch(e_cm)
	{
		case GL_COLORMODE_AYUV_CLUT2:
			return MTGFX_CM_YCBCR_CLUT2;
		case GL_COLORMODE_AYUV_CLUT4:
			return MTGFX_CM_YCBCR_CLUT4;
		case GL_COLORMODE_AYUV_CLUT8:
			return MTGFx_CM_YCBCR_CLUT8;
		case GL_COLORMODE_UYVY_16:
			return MTGFX_CM_CBYCRY422_DIRECT16;
		case GL_COLORMODE_YUYV_16:
			return MTGFX_CM_YCBYCR422_DIRECT16;
		case GL_COLORMODE_AYUV_D8888:
			return MTGFX_CM_AYCBCR8888_DIRECT32;
		case GL_COLORMODE_ARGB_CLUT2:
			return MTGFX_CM_RGB_CLUT2;
		case GL_COLORMODE_ARGB_CLUT4:
			return MTGFX_CM_RGB_CLUT4;
		case GL_COLORMODE_ARGB_CLUT8:
			return MTGFX_CM_RGB_CLUT8;
		case GL_COLORMODE_RGB_D565:
			return MTGFX_CM_RGB565_DIRECT16;
		case GL_COLORMODE_ARGB_D1555:
			return MTGFX_CM_ARGB1555_DIRECT16;
		case GL_COLORMODE_ARGB_D4444:
			return MTGFX_CM_ARGB4444_DIRECT16;
		case GL_COLORMODE_ARGB_D8888:
			return MTGFX_CM_ARGB8888_DIRECT32;
		default:
			return MTGFX_CM_ARGB8888_DIRECT32;
	}
}
#endif


#define GFX_COLOR_RGB565(r, g, b)           (((((UINT16) (r)) & 0xf8) << 8) | ((((UINT16) (g)) & 0xfc) << 3) | ((((UINT16) (b)) & 0xf8) >> 3))
#define GFX_COLOR_ARGB1555(a, r, g, b)      (((((UINT16) (a)) > 0) ? (UINT16) 0x8000 : 0) | ((((UINT16) (r)) & 0xf8) << 7) | ((((UINT16) (g)) & 0xf8) << 2) | ((((UINT16) (b)) & 0xf8) >> 3))
#define GFX_COLOR_ARGB4444(a, r, g, b)      (((((UINT16) (a)) & 0xf0) << 8) | ((((UINT16) (r)) & 0xf0) << 4) | (((UINT16) (g)) & 0xf0) | ((((UINT16) (b)) & 0xf0) >> 4))
#define GFX_COLOR_ARGB8888(a, r, g, b)      (((((UINT32) (a)) & 0xff) << 24) | ((((UINT32) (r)) & 0xff) << 16) | ((((UINT32) (g)) & 0xff) << 8) | (((UINT32) (b)) & 0xff))


UINT32 gl_dfb_compose_packed_color(const GL_COLOR_T* pt_color, GL_COLORMODE_T e_mode)
{
    switch (e_mode)
    {
        case GL_COLORMODE_ARGB_CLUT2:
        case GL_COLORMODE_AYUV_CLUT2:
            return ((pt_color->u3.index) & 0x3);
            
        case GL_COLORMODE_ARGB_CLUT4:
        case GL_COLORMODE_AYUV_CLUT4:
            return ((pt_color->u3.index) & 0xf);

        case GL_COLORMODE_ARGB_CLUT8:
        case GL_COLORMODE_AYUV_CLUT8:
            return (pt_color->u3.index);

        case GL_COLORMODE_YUYV_16:
            return (GFX_COLOR_ARGB8888(pt_color->u1.y, pt_color->u2.u, pt_color->u1.y, pt_color->u3.v));
            
        case GL_COLORMODE_UYVY_16:
            return (GFX_COLOR_ARGB8888(pt_color->u2.u, pt_color->u1.y, pt_color->u3.v, pt_color->u1.y));

        case GL_COLORMODE_RGB_D565:
            return (GFX_COLOR_RGB565(pt_color->u1.r, pt_color->u2.g, pt_color->u3.b));
            
        case GL_COLORMODE_ARGB_D1555:
            return (GFX_COLOR_ARGB1555(pt_color->a, pt_color->u1.r, pt_color->u2.g, pt_color->u3.b));

        case GL_COLORMODE_ARGB_D4444:
            return (GFX_COLOR_ARGB4444(pt_color->a, pt_color->u1.r, pt_color->u2.g, pt_color->u3.b));

        case GL_COLORMODE_ARGB_D8888:
            return (GFX_COLOR_ARGB8888(pt_color->a, pt_color->u1.r, pt_color->u2.g, pt_color->u3.b));

        case GL_COLORMODE_AYUV_D8888:
            return (GFX_COLOR_ARGB8888(pt_color->a, pt_color->u1.y, pt_color->u2.u, pt_color->u3.v));
            
        default:
            return 0;
    }
}

