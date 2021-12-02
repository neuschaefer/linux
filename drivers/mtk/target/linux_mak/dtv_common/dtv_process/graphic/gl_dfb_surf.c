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
#include "gl_dfb_util.h"
#include "u_gl.h"
#include "gl.h"
#include "gl_dfb.h"
#include "gl_dfb_clipper.h"
#include "gl_dfb_sw_draw.h"
#include "gl_dfb_sw_blt.h"

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "stdio.h"

#include "cl/x_cl.h"
#include "util/u_lnk_list.h"
#include "gl_surface.h"

#if 0
#include <directfb.h>
#else
#include "mtgfx.h"
#include "mtosd.h"
#include "gl_mem_manager.h"
#endif

#ifdef MW_PHOTO_SHOW_ON_VDP
#include "../../chiling/driver/directfb/MediaTek/mt53xx/mt53xx_fb.h"
#endif


#define GL_DFB_SURF_TRACE_API(...) /*GL_DFB_TRACE*/

#if 0
DFBSurfacePixelFormat gl_dfb_cm_map_get(GL_COLORMODE_T cm)
{
    switch(cm)
    {
        case GL_COLORMODE_AYUV_CLUT2: 
        case GL_COLORMODE_AYUV_CLUT4:     
        case GL_COLORMODE_AYUV_CLUT8:
            /*Fix me*/
            GL_DFB_TRACE("Error cm conv fro gl to dfb\n");
            return DSPF_UNKNOWN;
        case GL_COLORMODE_UYVY_16:
            return DSPF_UYVY;
        case GL_COLORMODE_YUYV_16:
            return DSPF_YUY2;
        case GL_COLORMODE_AYUV_D8888:
            return DSPF_AYUV;
        case GL_COLORMODE_ARGB_CLUT2:
            return DSPF_LUT2;
        case GL_COLORMODE_ARGB_CLUT4:
            return DSPF_UNKNOWN;
        case GL_COLORMODE_ARGB_CLUT8:
            return DSPF_LUT8;
        case GL_COLORMODE_RGB_D565:
            return DSPF_RGB16;
        case GL_COLORMODE_ARGB_D1555:
            return DSPF_ARGB1555;
        case GL_COLORMODE_ARGB_D4444:
            return DSPF_ARGB4444;
        case GL_COLORMODE_ARGB_D8888:
            return DSPF_ARGB;
        case GL_COLORMODE_YUV_420_BLK:
        case GL_COLORMODE_YUV_420_RS:
            /*Fix me*/
            return DSPF_I420;
        case GL_COLORMODE_YUV_422_BLK:
        case GL_COLORMODE_YUV_422_RS:  
            /*Fix me*/
            GL_DFB_TRACE("Error cm conv fro gl to dfb\n");
#ifdef MW_PHOTO_SHOW_ON_VDP
            return DSPF_NV16;
#else
            return DSPF_UNKNOWN;
#endif
        case GL_COLORMODE_YUV_444_BLK:
            /*Fix me*/
            return DSPF_NV12;
        case GL_COLORMODE_YUV_444_RS:
            /*Fix me*/
            return DSPF_NV21;
    }
    return 	 DSPF_ARGB4444;

}

static INT32 _dfb_surface_clear_blit_opts(IDirectFBSurface * p_dfb_surf)
{
    INT32 i4_ret;

    DFBCHECK(p_dfb_surf->SetPorterDuff(p_dfb_surf, DSPD_NONE), &i4_ret); 
    DFBCHECK(p_dfb_surf->SetSrcBlendFunction(p_dfb_surf, DSBF_ZERO), &i4_ret);
    DFBCHECK(p_dfb_surf->SetDstBlendFunction(p_dfb_surf, DSBF_ZERO), &i4_ret);
    DFBCHECK(p_dfb_surf->SetColor(p_dfb_surf, 255, 255, 255, 255), &i4_ret);
    
    DFBCHECK(p_dfb_surf->SetBlittingFlags(p_dfb_surf, DSBLIT_NOFX), &i4_ret);
    return i4_ret;
}
#else

static MTGFX_PD_RULE_T gl_compose_rule_to_mtal(GL_COMPOSE_RULE_T e_rule)
{
	switch (e_rule)
	{		
		case GL_CLEAR:
		case GL_DST_IN:			
		case GL_DST_OUT:
		case GL_DST_OVER:
		case GL_SRC:
		case GL_SRC_IN:			
		case GL_SRC_OUT:
		case GL_SRC_OVER:	
			return (MTGFX_PD_RULE_T) e_rule;
		default :
			return GL_SRC_OVER;
	}
}

#endif

INT32 gl_dfb_surface_rebuild(GL_DFB_SURFACE_T * p_surf, void * dfb_storage, GL_DFB_SURF_STRG_E e_strg)
{
#if 0
    INT32                   i4_ret;

    IDirectFBSurface *      p_new_dfb_surf;
    DFBSurfaceDescription   dfb_desc;
#else
	GL_MEM_POOL*            p_new_dfb_surf;
	UINT32 					ui4_pitch;
#endif

	gl_dfb_obj_lock(p_surf);
    if(e_strg == p_surf->e_strg)
	{
    	GL_DFB_ERR("[GL_DFB_SURF] Warning: same strg of rebuilding\n");
		gl_dfb_obj_unlock(p_surf);
		return GLR_OK;
	}
    switch(e_strg)
    {
        case GL_DFB_SURF_STRG_OFF_SCR:
#ifdef MW_PHOTO_SHOW_ON_VDP
        case GL_DFB_SURF_STRG_OFF_SCR_FROM_FBM:
#endif
#if 0
    		GL_DFB_TRACE("[GL_DFB_SURF]Rebuild p_surf:%x to off-screen\n", p_surf);
            dfb_desc.flags = DSDESC_CAPS|DSDESC_WIDTH|DSDESC_HEIGHT|DSDESC_WIDTH|DSDESC_PIXELFORMAT;    
            dfb_desc.caps  = /*DSCAPS_NONE*//*DSCAPS_SYSTEMONLY*/DSCAPS_VIDEOONLY/*DSCAPS_PREMULTIPLIED*/;
            dfb_desc.width          = p_surf->t_desc.ui4_width;
            dfb_desc.height         = p_surf->t_desc.ui4_height;
            dfb_desc.pixelformat    = gl_dfb_cm_map_get(p_surf->t_desc.e_clrmode);

            /*Create new surface*/
            DFBCHECK(g_p_dfb->CreateSurface(g_p_dfb, &dfb_desc, &p_new_dfb_surf), &i4_ret);
#else
			case GL_DFB_SURF_STRG_OFF_SCR_FROM_FBM:
			{
				ui4_pitch = p_surf->t_desc.ui4_width * (gl_bit_count(p_surf->t_desc.e_clrmode) >> 3);
				if (ui4_pitch &0xF)
				{
					ui4_pitch = ((ui4_pitch + 16) >> 4 ) << 4;
				}
				p_surf->ui4_pitch = ui4_pitch;
				
				p_new_dfb_surf = gl_mem_manager_alloc(p_surf->t_desc.ui4_height *p_surf->ui4_pitch) ;
			}
#endif
            break;
#if 0
#ifndef MW_PHOTO_SHOW_ON_VDP
        case GL_DFB_SURF_STRG_OFF_SCR_FROM_FBM:
    		GL_DFB_TRACE("[GL_DFB_SURF]Rebuild p_surf:%x to off-screen\n", p_surf);
            dfb_desc.flags = DSDESC_CAPS|DSDESC_WIDTH|DSDESC_HEIGHT|DSDESC_WIDTH|DSDESC_PIXELFORMAT;    
            dfb_desc.caps  = DSCAPS_VIDEOONLY|DSCAPS_FROM_FBM;
            dfb_desc.width          = p_surf->t_desc.ui4_width;
            dfb_desc.height         = p_surf->t_desc.ui4_height;
            dfb_desc.pixelformat    = gl_dfb_cm_map_get(p_surf->t_desc.e_clrmode);

			if(dfb_desc.height == 0)
			{
				/*For MMP */
				dfb_desc.height = 1;
			}

            /*Create new surface*/
            DFBCHECK(g_p_dfb->CreateSurface(g_p_dfb, &dfb_desc, &p_new_dfb_surf), &i4_ret);
            break;
#else
        case GL_DFB_SURF_STRG_OFF_SCR_FROM_VDP:
    		GL_DFB_TRACE("[GL_DFB_SURF]Rebuild p_surf:%x to off-screen\n", p_surf);
            dfb_desc.flags = DSDESC_CAPS|DSDESC_WIDTH|DSDESC_HEIGHT|DSDESC_WIDTH|DSDESC_PIXELFORMAT;    
            dfb_desc.caps  = DSCAPS_VIDEOONLY|DSCAPS_FROM_JPEG_VDP;
            dfb_desc.width          = p_surf->t_desc.ui4_width;
            dfb_desc.height         = p_surf->t_desc.ui4_height;
            dfb_desc.pixelformat    = gl_dfb_cm_map_get(p_surf->t_desc.e_clrmode);

			if(dfb_desc.height == 0)
			{
				/*For MMP */
				dfb_desc.height = 1;
			}

            /*Create new surface*/
            DFBCHECK(g_p_dfb->CreateSurface(g_p_dfb, &dfb_desc, &p_new_dfb_surf), &i4_ret);
            break;
#endif

#endif
        case GL_DFB_SURF_STRG_WND:
#if 0
            GL_DFB_TRACE("[GL_DFB_SURF]Rebuild p_surf:%x to window:%x\n", p_surf, dfb_storage);
             
            DFBCHECK(((IDirectFBWindow *)dfb_storage)->GetSurface(dfb_storage, &p_new_dfb_surf), &i4_ret);
            DFBCHECK(p_surf->p_dfb_surf->Clear(p_new_dfb_surf, 0, 0, 0, 0), &i4_ret);
#else
			gl_dfb_subtitle_plane_flip();
			p_new_dfb_surf = gl_dfb_get_layer_pool(&ui4_pitch);
			ui4_pitch = 3840;

#endif
            break;

        case GL_DFB_SURF_STRG_LAYER:
#if 0
            GL_DFB_TRACE("[GL_DFB_SURF]Rebuild p_surf:%x to primary:%x\n", p_surf);
            DFBCHECK(((IDirectFBDisplayLayer *)dfb_storage)->GetSurface(dfb_storage, &p_new_dfb_surf), &i4_ret);
            DFBCHECK(p_surf->p_dfb_surf->Clear(p_new_dfb_surf, 0, 0, 0, 0), &i4_ret);             
#else
			gl_dfb_subtitle_plane_flip();
			p_new_dfb_surf = gl_dfb_get_layer_pool(&ui4_pitch);
			ui4_pitch = 3840;
#endif
            break;

        default:
            GL_DFB_ABORT("How to reach here!");
	}

    
    /*Release old dfb surf and copy the data*/
    if(p_surf->p_dfb_surf != NULL)
#if 0  
    {        
        /*Clear the blending options*/
        _dfb_surface_clear_blit_opts(p_new_dfb_surf);

		/*Blit the old data .*/
        DFBCHECK(p_new_dfb_surf->SetClip(p_new_dfb_surf, 
                                         NULL), 
                 &i4_ret);
        DFBCHECK(p_surf->p_dfb_surf->SetClip(p_surf->p_dfb_surf, 
                                             NULL),
                 &i4_ret);
        DFBCHECK(p_new_dfb_surf->Blit(p_new_dfb_surf, 
                                      p_surf->p_dfb_surf,
                                      NULL,
                                      0,
                                      0), 
                 &i4_ret);
        DFBCHECK(p_new_dfb_surf->ReleaseSource(p_new_dfb_surf), &i4_ret);
        /*For old storage:*/
        switch(p_surf->e_strg)
        {
            case GL_DFB_SURF_STRG_OFF_SCR:
            case GL_DFB_SURF_STRG_OFF_SCR_FROM_FBM:            	
#ifdef MW_PHOTO_SHOW_ON_VDP
            case GL_DFB_SURF_STRG_OFF_SCR_FROM_VDP:
#endif
                DFBCHECK(p_surf->p_dfb_surf->ReleaseSource(p_surf->p_dfb_surf), &i4_ret);
                DFBCHECK(p_surf->p_dfb_surf->Release(p_surf->p_dfb_surf), &i4_ret);
                break;
                
            case GL_DFB_SURF_STRG_LAYER:                 
                DFBCHECK(p_surf->p_dfb_surf->Clear(p_surf->p_dfb_surf, 0, 0, 0, 0), &i4_ret);
                DFBCHECK(p_surf->p_dfb_surf->ReleaseSource(p_surf->p_dfb_surf), &i4_ret);
                DFBCHECK(p_surf->p_dfb_surf->Release(p_surf->p_dfb_surf), &i4_ret);
                break;
                
            case GL_DFB_SURF_STRG_WND:                 
                DFBCHECK(p_surf->p_dfb_surf->Clear(p_surf->p_dfb_surf, 0, 0, 0, 0), &i4_ret);
                DFBCHECK(p_surf->p_dfb_surf->ReleaseSource(p_surf->p_dfb_surf), &i4_ret);
                DFBCHECK(p_surf->p_dfb_surf->Release(p_surf->p_dfb_surf), &i4_ret);
                break;

            default:
               GL_DFB_ABORT("How to reach here!\n");
        }
        DFBCHECK(g_p_dfb->WaitIdle(g_p_dfb), &i4_ret);        
        
        
    }
    if(e_strg != GL_DFB_SURF_STRG_OFF_SCR && e_strg != GL_DFB_SURF_STRG_OFF_SCR_FROM_FBM)
    {
        DFBCHECK(p_new_dfb_surf->Flip(p_new_dfb_surf, NULL, 0), &i4_ret);
    }

    /* Modify the implementor pointer and set new storage flag */
    
    p_surf->e_strg      = e_strg;
    p_surf->p_dfb_surf  = p_new_dfb_surf;
    
 #else
 	{
 		MTGFX_BITBLT_T  blit_arg;
 
	 	blit_arg.u1Alpha       = 0;
		blit_arg.pvSrc         = (VOID*)p_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.i4SrcX        = 0;
		blit_arg.i4SrcY        = 0;
		blit_arg.u4SrcPitch    = p_surf->ui4_pitch;
		blit_arg.eSrcCm        = gl_cm_to_mtal_cm(p_surf->t_desc.e_clrmode);
		
		blit_arg.pvDst         = (VOID*)p_new_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.i4DstX        = 0;
		blit_arg.i4DstY        = 0;
		blit_arg.u4DstPitch    = ui4_pitch;
		blit_arg.eDstCm        = gl_cm_to_mtal_cm(p_surf->t_desc.e_clrmode);

		blit_arg.u4Width       = p_surf->t_desc.ui4_width;
		blit_arg.u4Height      = p_surf->t_desc.ui4_height;
 		MTGFX_Bitblt(&blit_arg);
		
		switch(p_surf->e_strg)
        {
            case GL_DFB_SURF_STRG_OFF_SCR:
            case GL_DFB_SURF_STRG_OFF_SCR_FROM_FBM:
				gl_mem_manager_free(p_surf->p_dfb_surf);
			    break;
			case GL_DFB_SURF_STRG_LAYER:
			case GL_DFB_SURF_STRG_WND:
				break;
				
            default:
               GL_DFB_ABORT("How to reach here!\n");
        }
		MTGFX_Flush();
		MTGFX_Wait(-1);

	
    }
	p_surf->e_strg      = e_strg;
    p_surf->p_dfb_surf  = p_new_dfb_surf;

 #endif
    

    gl_dfb_obj_unlock(p_surf);
	
	return GLR_OK;

}

#if 0
static void _pat_clip_realign(
        GL_DFB_SURFACE_T *  p_surf, 
        INT32               i4_dst_x,
        INT32               i4_dst_y,  
        UINT32              ui4_dst_w,
        UINT32              ui4_dst_h)
{
    DFBRegion dfb_clip;
    INT32 i4_ret;

    
    DFBCHECK(p_surf->p_dfb_surf->GetClip(p_surf->p_dfb_surf, &dfb_clip), &i4_ret);
    
    if(dfb_clip.x2 > i4_dst_x + (INT32)ui4_dst_w)
    {
        dfb_clip.x2 = i4_dst_x + (INT32)ui4_dst_w;
    }
    if(dfb_clip.y2 > i4_dst_y + (INT32)ui4_dst_h)
    {
        dfb_clip.y2 = i4_dst_y + (INT32)ui4_dst_h;
    }

    DFBCHECK(p_surf->p_dfb_surf->SetClip(p_surf->p_dfb_surf, &dfb_clip), &i4_ret);
    
}
#endif

static const UINT8  _s_dbg_ui1_pixel_bytes[] = {
    1, 1, 1, 2, 2, 4, 1, 1, 1, 2, 2, 2, 4 };    
#define _DFB_DBG_SURF_SIZE(p_surf) ((size_t)(p_surf->t_desc.ui4_width * p_surf->t_desc.ui4_height * _s_dbg_ui1_pixel_bytes[p_surf->t_desc.e_clrmode]))
#define _DFB_DBG_SURF_INC(p_surf)\
    do{\
        _z_dfb_dbg_surf_used_mem += _DFB_DBG_SURF_SIZE(p_surf);\
        if(_z_dfb_dbg_surf_used_mem > _z_dfb_dbg_surf_used_mem_top)\
        {\
            _z_dfb_dbg_surf_used_mem_top = _z_dfb_dbg_surf_used_mem;\
        }\
        GL_DFB_TRACE("[GL_DFB_SURF] + len:%d, Surf size is %d/Top: %d\n",_DFB_DBG_SURF_SIZE(p_surf), _z_dfb_dbg_surf_used_mem, _z_dfb_dbg_surf_used_mem_top);\
    }while(0)

#define _DFB_DBG_SURF_DEC(p_surf)\
    do{\
        _z_dfb_dbg_surf_used_mem -= _DFB_DBG_SURF_SIZE(p_surf);\
        GL_DFB_TRACE("[GL_DFB_SURF] - len:%d, Surf size is %d/Top: %d\n",_DFB_DBG_SURF_SIZE(p_surf), _z_dfb_dbg_surf_used_mem, _z_dfb_dbg_surf_used_mem_top);\
    }while(0)

static size_t _z_dfb_dbg_surf_used_mem = 0;
static size_t _z_dfb_dbg_surf_used_mem_top = 0;

#if 0
static DFBRectangle     *_pt_pat_compose_rect  = NULL;
static DFBPoint         *_pt_pat_compose_point = NULL;
static int              _i4_pat_compose_num = 10;

static DFBRectangle     *_pt_pat_transp_rect  = NULL;
static DFBPoint         *_pt_pat_transp_point = NULL;
static int              _i4_pat_transp_num = 10;
#endif

static BOOL _surface_free_fct(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle)
{
	GL_DFB_SURFACE_T *  p_surf = pv_obj;
	//INT32               i4_ret;

	if(pv_obj == NULL)
	{
		return FALSE;
    }
		
    GL_DFB_BEGIN_FREE(h_handle, pv_obj)
         
#if 0
        g_p_dfb->WaitIdle(g_p_dfb);
         

     	DFBCHECK(p_surf->p_dfb_surf->ReleaseSource(p_surf->p_dfb_surf), &i4_ret);
     	DFBCHECK(p_surf->p_dfb_surf->Release(p_surf->p_dfb_surf), &i4_ret);
#else
	MTGFX_Wait(-1);
	gl_mem_manager_free(p_surf->p_dfb_surf);
#endif
        if(p_surf->t_desc.h_clipper != NULL_HANDLE)
        {
            x_gl_obj_free(p_surf->t_desc.h_clipper);
        }
        _DFB_DBG_SURF_DEC(p_surf);  
        #ifndef MW_GL_NO_COMPRESSED_IMG
        if (p_surf->pui1_line_buffer != NULL)
        {
            x_mem_free(p_surf->pui1_line_buffer);
        }
        if(p_surf->h_zip_engine != NULL_HANDLE)
        {
	        x_cl_close(p_surf->h_zip_engine);
	    }
        #endif
       
        gl_dfb_obj_uninit(pv_obj);
        GL_FREE(pv_obj);

        
    GL_DFB_END_FREE(h_handle, pv_obj)
	return FALSE;
}


INT32 
gl_dfb_surface_create(const GL_SURFACE_DESC_T * p_desc, HANDLE_T * ph_surf)
{
    GL_DFB_SURFACE_T *      p_surf;
    /*DFBSurfaceDescription   dfb_desc;*/
    GL_DFB_CLIPPER_T *      p_clipper;
    INT32                   i4_ret;

    p_surf = GL_MALLOC(sizeof(GL_DFB_SURFACE_T));
    GL_MEMSET(p_surf, 0, sizeof(GL_DFB_SURFACE_T));
    gl_dfb_obj_init(GLT_OBJ_SURFACE, p_surf); 
    
#ifndef MW_GL_NO_COMPRESSED_IMG
    p_surf->pui1_line_buffer = NULL;
    p_surf->h_zip_engine     = NULL_HANDLE;
#endif
    *ph_surf = GL_ALLOC_HANDLE(GLT_OBJ_SURFACE, p_surf, NULL, _surface_free_fct); 
    if (*ph_surf == NULL_HANDLE)
    {
        gl_dfb_obj_uninit(p_surf);
        GL_FREE(p_surf);
        return GLR_NO_MEMORY;
    }
    
    p_surf->e_strg = GL_DFB_SURF_STRG_NONE;
    GL_MEMCPY(&(p_surf->t_desc), p_desc, sizeof(GL_SURFACE_DESC_T));

    p_surf->b_from_fbm = FALSE;

    i4_ret = gl_dfb_surface_rebuild(p_surf, NULL, GL_DFB_SURF_STRG_OFF_SCR);
    if (i4_ret != GLR_OK)
    {
        GL_FREE_HANDLE(*ph_surf);
        *ph_surf = NULL_HANDLE;
        return i4_ret;
    }
    SET_RECT_BY_SIZE(&(p_surf->t_clip_rect), 0, 0, p_desc->ui4_width, p_desc->ui4_height);
#if 0
    DFBCHECK(p_surf->p_dfb_surf->Clear(p_surf->p_dfb_surf, 0, 0, 0, 0), &i4_ret);
    if(i4_ret != GLR_OK)
    {
        GL_FREE_HANDLE(*ph_surf);
        *ph_surf = NULL_HANDLE;
        return i4_ret;
    } 
#endif
    if(p_desc->h_clipper != NULL_HANDLE)
    {
        p_clipper = gl_dfb_handle_lock_obj(GLT_OBJ_CLIPPER, p_desc->h_clipper, &i4_ret);/*GL_HANDLE_GET_OBJ(p_desc->h_clipper);*/
        if(i4_ret != GLR_OK)
        {
            GL_FREE_HANDLE(*ph_surf);
            *ph_surf = NULL_HANDLE;
            return i4_ret;
        }    

        gl_dfb_dy_vec_get(p_clipper->p_rect_vec, 0, &(p_surf->t_clip_rect));        
        p_surf->t_desc.h_clipper = gl_clipper_duplicate_handle(p_desc->h_clipper);
        gl_dfb_obj_unlock(p_clipper);
    }

    _DFB_DBG_SURF_INC(p_surf);
    return GLR_OK;
}
#ifdef MW_PHOTO_SHOW_ON_VDP

INT32 gl_dfb_surface_create_from_vdp(const GL_SURFACE_DESC_T * p_desc, HANDLE_T * ph_surf)
{
    GL_DFB_SURFACE_T *      p_surf;
    /*DFBSurfaceDescription   dfb_desc;*/
    GL_DFB_CLIPPER_T *      p_clipper;
    INT32                   i4_ret;

    p_surf = GL_MALLOC(sizeof(GL_DFB_SURFACE_T));
    GL_MEMSET(p_surf, 0, sizeof(GL_DFB_SURFACE_T));
    gl_dfb_obj_init(GLT_OBJ_SURFACE, p_surf); 

    p_surf->pui1_line_buffer = NULL;
    p_surf->h_zip_engine     = NULL_HANDLE;
    *ph_surf = GL_ALLOC_HANDLE(GLT_OBJ_SURFACE, p_surf, NULL, _surface_free_fct); 
    if (*ph_surf == NULL_HANDLE)
    {
    gl_dfb_obj_uninit(p_surf);
    GL_FREE(p_surf);
    return GLR_NO_MEMORY;
    }
    

    p_surf->e_strg = GL_DFB_SURF_STRG_NONE;
    GL_MEMCPY(&(p_surf->t_desc), p_desc, sizeof(GL_SURFACE_DESC_T));
    p_surf->b_from_vdp = TRUE;

    i4_ret = gl_dfb_surface_rebuild(p_surf, NULL, GL_DFB_SURF_STRG_OFF_SCR_FROM_VDP);
    if(i4_ret != GLR_OK)
    {
    GL_FREE_HANDLE(*ph_surf);
    *ph_surf = NULL_HANDLE;
    return i4_ret;
    } 
    SET_RECT_BY_SIZE(&(p_surf->t_clip_rect), 0, 0, p_desc->ui4_width, p_desc->ui4_height);
    DFBCHECK(p_surf->p_dfb_surf->Clear(p_surf->p_dfb_surf, 0, 0, 0, 0), &i4_ret);
    if(i4_ret != GLR_OK)
    {
    GL_FREE_HANDLE(*ph_surf);
    *ph_surf = NULL_HANDLE;
    return i4_ret;
    } 

    if(p_desc->h_clipper != NULL_HANDLE)
    {
    p_clipper = gl_dfb_handle_lock_obj(GLT_OBJ_CLIPPER, p_desc->h_clipper, &i4_ret);/*GL_HANDLE_GET_OBJ(p_desc->h_clipper);*/
    if(i4_ret != GLR_OK)
    {
        GL_FREE_HANDLE(*ph_surf);
        *ph_surf = NULL_HANDLE;
        return i4_ret;
    }    

    gl_dfb_dy_vec_get(p_clipper->p_rect_vec, 0, &(p_surf->t_clip_rect));        
    p_surf->t_desc.h_clipper = gl_clipper_duplicate_handle(p_desc->h_clipper);
    gl_dfb_obj_unlock(p_clipper);
    }

    _DFB_DBG_SURF_INC(p_surf);
    return GLR_OK;
}
#endif
INT32 
gl_dfb_surface_create_from_fbm(const GL_SURFACE_DESC_T * p_desc, HANDLE_T * ph_surf)
{
#if 0
    GL_DFB_SURFACE_T *      p_surf;
    /*DFBSurfaceDescription   dfb_desc;*/
    GL_DFB_CLIPPER_T *      p_clipper;
    INT32                   i4_ret;

    p_surf = GL_MALLOC(sizeof(GL_DFB_SURFACE_T));
    GL_MEMSET(p_surf, 0, sizeof(GL_DFB_SURFACE_T));
    gl_dfb_obj_init(GLT_OBJ_SURFACE, p_surf); 
    
#ifndef MW_GL_NO_COMPRESSED_IMG
    p_surf->pui1_line_buffer = NULL;
    p_surf->h_zip_engine     = NULL_HANDLE;
#endif
    *ph_surf = GL_ALLOC_HANDLE(GLT_OBJ_SURFACE, p_surf, NULL, _surface_free_fct); 
    if (*ph_surf == NULL_HANDLE)
    {
        gl_dfb_obj_uninit(p_surf);
        GL_FREE(p_surf);
        return GLR_NO_MEMORY;
    }
    
    p_surf->e_strg = GL_DFB_SURF_STRG_NONE;
    GL_MEMCPY(&(p_surf->t_desc), p_desc, sizeof(GL_SURFACE_DESC_T));

    p_surf->b_from_fbm = TRUE;

    i4_ret = gl_dfb_surface_rebuild(p_surf, NULL, GL_DFB_SURF_STRG_OFF_SCR_FROM_FBM);
    if(i4_ret != GLR_OK)
    {
        GL_FREE_HANDLE(*ph_surf);
        *ph_surf = NULL_HANDLE;
        return i4_ret;
    } 
    SET_RECT_BY_SIZE(&(p_surf->t_clip_rect), 0, 0, p_desc->ui4_width, p_desc->ui4_height);
    DFBCHECK(p_surf->p_dfb_surf->Clear(p_surf->p_dfb_surf, 0, 0, 0, 0), &i4_ret);
    if(i4_ret != GLR_OK)
    {
        GL_FREE_HANDLE(*ph_surf);
        *ph_surf = NULL_HANDLE;
        return i4_ret;
    } 

    if(p_desc->h_clipper != NULL_HANDLE)
    {
        p_clipper = gl_dfb_handle_lock_obj(GLT_OBJ_CLIPPER, p_desc->h_clipper, &i4_ret);/*GL_HANDLE_GET_OBJ(p_desc->h_clipper);*/
        if(i4_ret != GLR_OK)
        {
            GL_FREE_HANDLE(*ph_surf);
            *ph_surf = NULL_HANDLE;
            return i4_ret;
        }    

        gl_dfb_dy_vec_get(p_clipper->p_rect_vec, 0, &(p_surf->t_clip_rect));        
        p_surf->t_desc.h_clipper = gl_clipper_duplicate_handle(p_desc->h_clipper);
        gl_dfb_obj_unlock(p_clipper);
    }

    _DFB_DBG_SURF_INC(p_surf);
#endif
    return GLR_OK;
}

GL_HSURFACE_T gl_surface_duplicate_handle(GL_HSURFACE_T h_surface)
{
    return (gl_dfb_duplicate_handle(h_surface, GLT_OBJ_SURFACE, _surface_free_fct));
}

INT32 gl_dfb_surface_get_info(
    GL_HSURFACE_T       h_surf, 
    GL_SURF_INFO_T*     pt_surf_info)
{
    GL_DFB_SURFACE_T * p_surf;
    INT32 i4_ret;

    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    } 
   
    pt_surf_info->e_clrmode = p_surf->t_desc.e_clrmode;
    pt_surf_info->ui4_width = p_surf->t_desc.ui4_width;
    pt_surf_info->ui4_height = p_surf->t_desc.ui4_height;
    pt_surf_info->h_signal = NULL_HANDLE;
    
    gl_dfb_obj_unlock(p_surf);   

    return GLR_OK;
}

#if 0
#define GL_DFB_SET_COLOR(p_surf, color)\
do{\
    (p_surf)->p_dfb_surf->SetColor(\
            (p_surf)->p_dfb_surf, \
            (color)->u1.r, \
            (color)->u2.g,\
            (color)->u3.b,\
            (color)->a);\
}while(0)

#define GL_RECT_TO_DFB_RECT(p_rect, p_dfb_rect)\
do{\
    (p_dfb_rect)->x1 = (p_rect)->i4_left;\
    (p_dfb_rect)->y1 = (p_rect)->i4_top;\
    (p_dfb_rect)->x2 = (p_rect)->i4_right - 1; \
    (p_dfb_rect)->y2 = (p_rect)->i4_bottom - 1;\
}while(0)


INT32 _prepare_draw_args(GL_DFB_SURFACE_T * p_surf, const GL_COLOR_T * p_color)
{
    DFBRegion tmp_clip_rg; 
    INT32 i4_ret;
    GL_RECT_T t_rect, t_rect1;
    UINT16 ui2_cnt;

    GL_MEMCPY(&t_rect, &(p_surf->t_clip_rect), sizeof(GL_RECT_T));
    
    if (p_surf->t_desc.h_clipper)
    {
        if (GLR_OK == x_gl_clipper_get_clip_list(p_surf->t_desc.h_clipper, &ui2_cnt, &t_rect1))
        {
            INTERSECT_RECT(&t_rect, &t_rect1, &t_rect);
        }
    }
    GL_RECT_TO_DFB_RECT(&t_rect, &tmp_clip_rg);
    
    
    DFBCHECK((p_surf)->p_dfb_surf->SetClip((p_surf)->p_dfb_surf, &tmp_clip_rg), &i4_ret);
    RETURN_RET_IF_FAIL(i4_ret);
    DFBCHECK(p_surf->p_dfb_surf->SetPorterDuff(p_surf->p_dfb_surf, DSPD_NONE), &i4_ret); 
    RETURN_RET_IF_FAIL(i4_ret);
    DFBCHECK(p_surf->p_dfb_surf->SetDrawingFlags(p_surf->p_dfb_surf, DSDRAW_NOFX), &i4_ret); 
    RETURN_RET_IF_FAIL(i4_ret);
    if(p_color != NULL)
    {
        DFBCHECK((p_surf)->p_dfb_surf->SetColor(
            (p_surf)->p_dfb_surf, 
            (p_color)->u1.r, 
            (p_color)->u2.g,
            (p_color)->u3.b,
            (p_color)->a), &i4_ret);
    }
    else
    {
        DFBCHECK((p_surf)->p_dfb_surf->SetColor((p_surf)->p_dfb_surf, 0, 0, 0, 0), &i4_ret);    
    }
    RETURN_RET_IF_FAIL(i4_ret);

    return i4_ret;
}


#define GL_DFB_BLT_ARG_NO           ((UINT32)0)
#define GL_DFB_BLT_ARG_PD           ((UINT32)1)
#define GL_DFB_BLT_ARG_PREALPHA     ((UINT32)2)
#define GL_DFB_BLT_ARG_COLORKEY     ((UINT32)4)
#define GL_DFB_BLT_ARG_ROP          ((UINT32)8)


static INT32 _prepare_blt_args(
        GL_DFB_SURFACE_T *  p_surf,
        UINT32              ui_blt_arg_flag,
        GL_COMPOSE_RULE_T   e_rule, 
        UINT8               ui1_pre_alpha, 
        const GL_COLOR_T *  pt_color_key,
        GL_ROP_TYPE_T       rop)
{
    DFBSurfaceBlittingFlags     dfb_blt_flag = DSBLIT_NOFX;
    DFBSurfacePorterDuffRule    dfb_pd_rule = DSPD_NONE;
    DFBSurfaceBlendFunction     dfb_sb_src_func = DSBF_ZERO; 
    DFBSurfaceBlendFunction     dfb_sb_dst_func = DSBF_ZERO; 
    INT32                       i4_ret;
    DFBRegion                   tmp_clip_rg; 
    GL_RECT_T                   t_rect, t_rect1;
    UINT16                      ui2_cnt;
   
    GL_MEMCPY(&t_rect, &(p_surf->t_clip_rect), sizeof(GL_RECT_T));
    
    if (p_surf->t_desc.h_clipper)
    {
        if (GLR_OK == x_gl_clipper_get_clip_list(p_surf->t_desc.h_clipper, &ui2_cnt, &t_rect1))
        {
            INTERSECT_RECT(&t_rect, &t_rect1, &t_rect);
        }
    }
    GL_RECT_TO_DFB_RECT(&t_rect, &tmp_clip_rg);

    DFBCHECK((p_surf)->p_dfb_surf->SetClip((p_surf)->p_dfb_surf, &tmp_clip_rg), &i4_ret);
	RETURN_RET_IF_FAIL(i4_ret);
    
    /*Clear the blending options*/
    _dfb_surface_clear_blit_opts(p_surf->p_dfb_surf);
    RETURN_RET_IF_FAIL(i4_ret);
    

    if((ui_blt_arg_flag & GL_DFB_BLT_ARG_PD))
    {
        switch(e_rule)
        {
            case GL_CLEAR:
                dfb_pd_rule |= DSPD_CLEAR;
                break;
            case GL_DST_IN:
                dfb_pd_rule |= DSPD_DST_IN;
                break;
            case GL_DST_OUT:
                dfb_pd_rule |= DSPD_DST_OUT;
                break;
            case GL_DST_OVER:
                dfb_pd_rule |= DSPD_DST_OVER;
                break;
            case GL_SRC:
                dfb_pd_rule |= DSPD_SRC;
                DFBCHECK(p_surf->p_dfb_surf->SetColor(p_surf->p_dfb_surf, ui1_pre_alpha, ui1_pre_alpha, ui1_pre_alpha, ui1_pre_alpha), &i4_ret);
                dfb_blt_flag    |= DSBLIT_BLEND_ALPHACHANNEL |DSBLIT_BLEND_COLORALPHA |DSBLIT_SRC_PREMULTIPLY | DSBLIT_DST_PREMULTIPLY | DSBLIT_DEMULTIPLY;
                break;
            case GL_SRC_IN:
                dfb_pd_rule |= DSPD_SRC_IN;
                break;
            case GL_SRC_OUT:
                dfb_pd_rule |= DSPD_SRC_OUT;
                break;
            case GL_SRC_OVER:
                dfb_pd_rule     |= DSPD_SRC_OVER;
                DFBCHECK(p_surf->p_dfb_surf->SetColor(p_surf->p_dfb_surf, ui1_pre_alpha, ui1_pre_alpha, ui1_pre_alpha, ui1_pre_alpha), &i4_ret);
                dfb_blt_flag    |= DSBLIT_BLEND_ALPHACHANNEL| DSBLIT_BLEND_COLORALPHA |DSBLIT_SRC_PREMULTIPLY | DSBLIT_DST_PREMULTIPLY | DSBLIT_DEMULTIPLY;
                break;
        }
        DFBCHECK(p_surf->p_dfb_surf->SetPorterDuff(p_surf->p_dfb_surf, dfb_pd_rule), &i4_ret);    
        RETURN_RET_IF_FAIL(i4_ret);
    }
    
    if((ui_blt_arg_flag & GL_DFB_BLT_ARG_PREALPHA))
    {
        /*Fix me : how does the color works as the alpha channel in DFB - MT53*/
        /*dfb_blt_flag |= (DSBLIT_SRC_PREMULTCOLOR);*/
        DFBCHECK(p_surf->p_dfb_surf->SetColor(p_surf->p_dfb_surf, ui1_pre_alpha, ui1_pre_alpha, ui1_pre_alpha, ui1_pre_alpha), &i4_ret);
        RETURN_RET_IF_FAIL(i4_ret);
        /*
        Orig code
        dfb_blt_flag |= DSBLIT_BLEND_ALPHACHANNEL;        
        DFBCHECK(p_surf->p_dfb_surf->SetColor(p_surf->p_dfb_surf, ui1_pre_alpha, ui1_pre_alpha, ui1_pre_alpha, ui1_pre_alpha), &i4_ret);
        RETURN_RET_IF_FAIL(i4_ret);*/
    }
    if((ui_blt_arg_flag & GL_DFB_BLT_ARG_COLORKEY))
    {
        dfb_blt_flag |= DSBLIT_SRC_COLORKEY;    
        DFBCHECK(p_surf->p_dfb_surf->SetSrcColorKey(p_surf->p_dfb_surf,
            pt_color_key->u1.r,
            pt_color_key->u2.g,
            pt_color_key->u3.b), &i4_ret);
        RETURN_RET_IF_FAIL(i4_ret);
    }

    if((ui_blt_arg_flag & GL_DFB_BLT_ARG_ROP))
    {
        switch(rop)
        {
            case GL_ROP_NOT_SRC:
                dfb_sb_src_func |= DSBF_INVSRCCOLOR;
                break;
            case GL_ROP_NOT_DST:
                dfb_sb_dst_func |= DSBF_INVDESTCOLOR;
                break;                
            case GL_ROP_SRC_XOR_DST:
	            dfb_blt_flag |= DSBLIT_XOR;                
                break;
            case GL_ROP_SRC_XNOR_DST:
                /*Not XNOR directly 
                 *Use A XOR ~B 
                 * */
                dfb_blt_flag |= DSBLIT_XOR;
                dfb_sb_dst_func |= DSBF_INVDESTCOLOR;
                break;
            case GL_ROP_SRC_AND_DST:
            case GL_ROP_NOT_SRC_AND_DST:
            case GL_ROP_SRC_AND_NOT_DST:
            case GL_ROP_NOT_SRC_AND_NOT_DST:
            case GL_ROP_SRC_OR_DST:
            case GL_ROP_NOT_SRC_OR_DST:
            case GL_ROP_SRC_OR_NOT_DST:
            case GL_ROP_NOT_SRC_OR_NOT_DST:
                /**/
                break;

        }
        DFBCHECK(p_surf->p_dfb_surf->SetSrcBlendFunction(p_surf->p_dfb_surf, dfb_sb_src_func), &i4_ret);
        RETURN_RET_IF_FAIL(i4_ret);
        DFBCHECK(p_surf->p_dfb_surf->SetDstBlendFunction(p_surf->p_dfb_surf, dfb_sb_dst_func), &i4_ret);
        RETURN_RET_IF_FAIL(i4_ret);
    }
    
    DFBCHECK(p_surf->p_dfb_surf->SetBlittingFlags(
        p_surf->p_dfb_surf,
        dfb_blt_flag), &i4_ret);
    RETURN_RET_IF_FAIL(i4_ret);
    return i4_ret;
}

/*
The window implementation requires surface flip, but layer implementation does not.
Some issue is DirectFB driver implementation does not suply "Flush Command" .

*/

#ifndef LINUX_TURNKEY_SOLUTION_PHASE2
#define GL_DFB_SURF_FLIP(p_surf)\
do{int __ir_ret;\
  if(p_surf->e_strg != GL_DFB_SURF_STRG_OFF_SCR && p_surf->e_strg != GL_DFB_SURF_STRG_OFF_SCR_FROM_FBM)\
  {\
    DFBCHECK((p_surf)->p_dfb_surf->Flip((p_surf)->p_dfb_surf, NULL, 0), &__ir_ret);\
  }\
  if(ui4_flag == GL_SYNC)\
  {\
         \
        DFBCHECK(g_p_dfb->WaitIdle(g_p_dfb), &__ir_ret);\
         \
  }\
}while(0)
#else
#define GL_DFB_SURF_FLIP(p_surf)\
do{int __ir_ret;\
  if(p_surf->e_strg != GL_DFB_SURF_STRG_OFF_SCR && p_surf->e_strg != GL_DFB_SURF_STRG_OFF_SCR_FROM_FBM)\
  {\
    /*DFBCHECK((p_surf)->p_dfb_surf->Flip((p_surf)->p_dfb_surf, NULL, 0), &__ir_ret);*/\
  }\
  if(ui4_flag == GL_SYNC)\
  {\
         \
        DFBCHECK(g_p_dfb->WaitIdle(g_p_dfb), &__ir_ret);\
         \
  }\
}while(0)
#endif

#define GL_DFB_SURF_FLIP_WITH_REGION(p_surf, pt_region)\
  do{int __ir_ret;\
    DFBCHECK((p_surf)->p_dfb_surf->Flip((p_surf)->p_dfb_surf, pt_region, 0), &__ir_ret);\
    if(ui4_flag == GL_SYNC)\
    {\
           \
          /*DFBCHECK(g_p_dfb->WaitIdle(g_p_dfb), &__ir_ret);*/\
           \
    }\
}while(0)
#else

#define GL_DFB_SURF_FLIP(p_surf)

#define GL_DFB_SURF_FLIP_WITH_REGION(p_surf, pt_region)

#endif

INT32 gl_dfb_surface_set_clipper(
    GL_HSURFACE_T       h_surf, 
    GL_HCLIPPER_T       h_clipper)
{
    GL_DFB_SURFACE_T * p_surf;
    GL_DFB_CLIPPER_T * p_clipper;
    /*GL_DFB_CLIPPER_T * p_new_clipper;*/
    GL_RECT_T          t_rect;
    GL_RECT_T          t_surf_rect;
    INT32 i4_ret;
 
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
    RETURN_RET_IF_FAIL(i4_ret);
    
    if(p_surf->t_desc.h_clipper != NULL_HANDLE)
    {
        x_gl_obj_free(p_surf->t_desc.h_clipper);
        p_surf->t_desc.h_clipper = NULL_HANDLE;
    }
       
    if(h_clipper != NULL_HANDLE)
    {
        p_clipper = gl_dfb_handle_lock_obj(GLT_OBJ_CLIPPER, h_clipper, &i4_ret);
        if(i4_ret != GLR_OK)
        {
			gl_dfb_obj_unlock(p_surf);
            return i4_ret;
        } 
        SET_RECT_BY_SIZE(&t_surf_rect, 0, 0, p_surf->t_desc.ui4_width, p_surf->t_desc.ui4_height);

         
        gl_dfb_dy_vec_get(p_clipper->p_rect_vec, 0, &t_rect);
        /*For that bad rectangle*/
        if(t_rect.i4_right < t_rect.i4_left || t_rect.i4_bottom < t_rect.i4_top)
        {
           t_rect.i4_left = t_rect.i4_right = 0;
           t_rect.i4_bottom = t_rect.i4_top = 0;
        }  
        INTERSECT_RECT(&(p_surf->t_clip_rect), &t_rect, &t_surf_rect);
         
        
        GL_DFB_TRACE("[Surf]Set clip as %d,%d,%d,%d\n", 
                p_surf->t_clip_rect.i4_left,
                p_surf->t_clip_rect.i4_top,
                p_surf->t_clip_rect.i4_right,
                p_surf->t_clip_rect.i4_bottom);
        
        gl_dfb_obj_unlock(p_clipper);

    	p_surf->t_desc.h_clipper = gl_clipper_duplicate_handle(h_clipper);        
    }
    else
    {
        p_surf->t_desc.h_clipper = NULL_HANDLE;
         /*For clip protect*/
         
        SET_RECT_BY_SIZE(&(p_surf->t_clip_rect), 0, 0, p_surf->t_desc.ui4_width, p_surf->t_desc.ui4_height);
         
        
        GL_DFB_SURF_TRACE_API("[Surf]Set clip (CLEAN) as %d,%d,%d,%d\n", 
                p_surf->t_clip_rect.i4_left,
                p_surf->t_clip_rect.i4_top,
                p_surf->t_clip_rect.i4_right,
                p_surf->t_clip_rect.i4_bottom);
       
    }

    gl_dfb_obj_unlock(p_surf);
   
    return GLR_OK;
}

INT32 gl_dfb_surface_set_palette(
    GL_HSURFACE_T   h_surf, 
    GL_HPALETTE_T   h_palette)
{
	INT32 i4_ret = GLR_OK;
#if 0
    GL_DFB_SURFACE_T * p_surf;
    GL_DFB_PAL_T * p_pal;

	if(h_palette == NULL_HANDLE)
	{
	    return GLR_OK;
	}
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
	
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }   
    p_pal   = gl_dfb_handle_lock_obj(GLT_OBJ_PALETTE, h_palette, &i4_ret);

	if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    } 
    if(p_surf->t_desc.h_palette != NULL_HANDLE)
    {
        GL_FREE_HANDLE(p_surf->t_desc.h_palette);
    }
    p_surf->t_desc.h_palette = h_palette;

    DFBCHECK(p_surf->p_dfb_surf->SetPalette(p_surf->p_dfb_surf, p_pal->p_dfb_pal), &i4_ret);
    gl_dfb_obj_unlock(p_pal); 
    gl_dfb_obj_unlock(p_surf);
#endif
    return i4_ret;
}

INT32 gl_dfb_surface_lock(
    GL_HSURFACE_T       h_surf, 
    const GL_RECT_T*    pt_rect, 
    UINT32              ui4_flag, 
    GL_LOCK_INFO_T*     pt_lock_info)
{
    GL_DFB_SURFACE_T * p_surf;
    INT32 i4_ret;
    //int i4_pitch;
    
    GL_DFB_TRACE("[GL_DFB_SURFACE]Surface lock :%x\n", h_surf);
	if(h_surf == NULL_HANDLE)
    {
        return GLR_INV_ARG;
    }
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    } 
#if 0
    if (! p_surf->b_surface_locked)
    {
        DFBCHECK(p_surf->p_dfb_surf->Lock(
            p_surf->p_dfb_surf, 
            DSLF_READ|DSLF_WRITE,
            &(pt_lock_info->at_subplane[0].pv_bits),
            &i4_pitch), &i4_ret);

        pt_lock_info->at_subplane[0].ui4_pitch = i4_pitch;
        p_surf->pv_buffer = pt_lock_info->at_subplane[0].pv_bits;
        p_surf->ui4_pitch = (UINT32)i4_pitch;
        p_surf->b_surface_locked = TRUE;
    }
    else
    {
        pt_lock_info->at_subplane[0].ui4_pitch = p_surf->ui4_pitch;
        pt_lock_info->at_subplane[0].pv_bits   = p_surf->pv_buffer;
        i4_ret = GLR_SURFACE_LOCKED;
    }
#else
	if (! p_surf->b_surface_locked)
    {
    	x_sema_lock(p_surf->p_dfb_surf->h_entry_lock,X_SEMA_OPTION_WAIT);
        p_surf->b_surface_locked = TRUE;
	}
	else
	{
		i4_ret = GLR_SURFACE_LOCKED;
	}
	pt_lock_info->at_subplane[0].ui4_pitch = p_surf->ui4_pitch;
    pt_lock_info->at_subplane[0].pv_bits   = (VOID*)p_surf->p_dfb_surf->bmp_buffer.u4VirtAddr;
#endif    
    pt_lock_info->e_clrmode = p_surf->t_desc.e_clrmode;
    pt_lock_info->z_subplane = 1;
    pt_lock_info->at_subplane[0].ui4_width = p_surf->t_desc.ui4_width;
    pt_lock_info->at_subplane[0].ui4_height = p_surf->t_desc.ui4_height;

    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    } 
    
    gl_dfb_obj_unlock(p_surf);
    return GLR_OK;
}

INT32 gl_dfb_surface_unlock(GL_HSURFACE_T h_surf, const GL_RECT_T* pt_rect)
{
    GL_DFB_SURFACE_T * p_surf;
    INT32 i4_ret;
    GL_DFB_TRACE("[GL_DFB_SURFACE]Surface unlock :%x\n", h_surf);	
    if(h_surf == NULL_HANDLE)
    {
        return GLR_INV_ARG;
    }
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }   
    if (p_surf->b_surface_locked)
    {
#if 0
        DFBCHECK(p_surf->p_dfb_surf->Unlock(p_surf->p_dfb_surf), &i4_ret);
#else
		x_sema_unlock(p_surf->p_dfb_surf->h_entry_lock);
#endif
        p_surf->b_surface_locked = FALSE;
    }
    else
    {
        i4_ret = GLR_SURFACE_NOT_LOCKED;
    }
     
    gl_dfb_obj_unlock(p_surf);

    return i4_ret;
}

INT32 gl_dfb_surface_draw_line(
    HANDLE_T            h_surf,
    const GL_POINT_T*   pt_start, 
    const GL_POINT_T*   pt_end,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T * p_surf;
    INT32 i4_ret;
    if(h_surf == NULL_HANDLE)
    {
        return GLR_INV_ARG;
    }

	if ((0 == ui2_line_width))
    {
        return GLR_OK;
    }
#if 0
    if (ui2_line_width == 1)
    {
        p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
        if(i4_ret != GLR_OK)
        {
            return i4_ret;
        }     

         
        i4_ret = _prepare_draw_args(p_surf, pt_color);
        if(i4_ret != GLR_OK)
        {
                 
            gl_dfb_obj_unlock(p_surf);
            return i4_ret;
        } 
        DFBCHECK(p_surf->p_dfb_surf->DrawLine(p_surf->p_dfb_surf, pt_start->i4_x, pt_start->i4_y, pt_end->i4_x, pt_end->i4_y), &i4_ret);
         
        if(i4_ret != GLR_OK)
        {
            gl_dfb_obj_unlock(p_surf);
            return i4_ret;
        } 
        GL_DFB_SURF_FLIP(p_surf);

        gl_dfb_obj_unlock(p_surf);
        return GLR_OK;
    }
    else
#endif
    {
        GL_LOCK_INFO_T     t_lock_info;
        GL_RECT_T          t_dst_clip;
        INT32              x0 = 0, y0 = 0, x1 = 0, y1 = 0;
        UINT32             ui4_color = 0;
        GL_MASK_T*         pt_mask = NULL;
        
        i4_ret = gl_dfb_surface_lock(h_surf, NULL, ui4_flag, &t_lock_info);
        if (GLR_OK != i4_ret)
        {
            return i4_ret;
        }
        
        p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);

        gl_surface_get_clip_area(p_surf, &t_dst_clip);

        x0 = pt_start->i4_x;
        y0 = pt_start->i4_y;
        x1 = pt_end->i4_x;
        y1 = pt_end->i4_y;

        /* make sure that the line width is valid */
        LIMIT_LINE_WIDTH(ui2_line_width);

        if (gl_clip_2d(
            &x0, 
            &y0, 
            &x1, 
            &y1, 
            t_dst_clip.i4_left, 
            t_dst_clip.i4_right - 1, 
            t_dst_clip.i4_top, 
            t_dst_clip.i4_bottom - 1))
        {
            ui4_color = gl_dfb_compose_packed_color(pt_color, t_lock_info.e_clrmode);

            pt_mask = gl_allocate_mask(ui2_line_width);
            if (gl_set_circle_mask(pt_mask) != GLR_OK)
            {
                GL_DFB_TRACE(("{GRAPHIC} ERR: Set circle mask failed at L%d\r\n\r\n", __LINE__));
            }
            
            i4_ret = gl_draw_line(
                t_lock_info.e_clrmode, 
                t_lock_info.at_subplane[0].pv_bits,
                t_lock_info.at_subplane[0].ui4_pitch,
                x0, 
                y0, 
                x1, 
                y1,
                &t_dst_clip,
                ui2_line_width,
                pt_mask,
                ui4_color);
            
            if (i4_ret != GLR_OK)
            {
                GL_DFB_TRACE(("{GRAPHIC} ERR: Software draw-line failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
            
            gl_free_mask(pt_mask);
            
        }
        gl_dfb_obj_unlock(p_surf);
        
        gl_dfb_surface_unlock(h_surf, NULL);
    }
    return i4_ret;
}

INT32 gl_dfb_surface_fill(
    GL_HSURFACE_T       h_surf, 
    const GL_COLOR_T*   pt_color,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T * p_surf;
    INT32 i4_ret;
    GL_RECT_T                   t_rect, t_rect1;
    UINT16                      ui2_cnt;
    

    if(h_surf == NULL_HANDLE)
    {
        return GLR_INV_ARG;
    }
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
   
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }     
#if 0
    if (p_surf->t_desc.e_clrmode == GL_COLORMODE_YUV_422_RS)
    {
        BOOL b_unlock = FALSE;
        if (! p_surf->b_surface_locked)
        {
            DFBCHECK(p_surf->p_dfb_surf->Lock(
                p_surf->p_dfb_surf, 
                DSLF_READ|DSLF_WRITE,
                &p_surf->pv_buffer,
                (int*)&p_surf->ui4_pitch), &i4_ret);
            p_surf->b_surface_locked = TRUE;
            b_unlock = TRUE;
        }

        x_memset(p_surf->pv_buffer, 0, p_surf->ui4_pitch * p_surf->t_desc.ui4_height);
        x_memset(p_surf->pv_buffer + p_surf->ui4_pitch * p_surf->t_desc.ui4_height, 0x80, 
                p_surf->ui4_pitch * p_surf->t_desc.ui4_height);

        if (b_unlock)
        {
            DFBCHECK(p_surf->p_dfb_surf->Unlock(p_surf->p_dfb_surf), &i4_ret);
            p_surf->b_surface_locked = FALSE;
        }
    }
    else
#endif
    {
    GL_MEMCPY(&t_rect, &(p_surf->t_clip_rect), sizeof(GL_RECT_T));
    
    if (p_surf->t_desc.h_clipper)
    {
        if (GLR_OK == x_gl_clipper_get_clip_list(p_surf->t_desc.h_clipper, &ui2_cnt, &t_rect1))
        {
            INTERSECT_RECT(&t_rect, &t_rect1, &t_rect);
        }
    }

    if (RECT_EMPTY(&t_rect))
    {
        GL_DFB_SURF_TRACE_API("[GL] Fill surface rectangle empty %s( %d )\n", __FUNCTION__, __LINE__);
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    }
#if 0
    {
	    DFBRegion                   tmp_clip_rg; 
    GL_RECT_TO_DFB_RECT(&t_rect, &tmp_clip_rg);

    DFBCHECK((p_surf)->p_dfb_surf->SetClip((p_surf)->p_dfb_surf, &tmp_clip_rg), &i4_ret);
    
    DFBCHECK(p_surf->p_dfb_surf->Clear(
                p_surf->p_dfb_surf,                
                pt_color->u1.r,
                pt_color->u2.g,
                pt_color->u3.b,
                pt_color->a), &i4_ret);

    }
#else
	{
		MTGFX_FILL_T  fill_arg;
		
		fill_arg.pvDst      = (VOID*)p_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		fill_arg.i4DstX     = t_rect.i4_left;
		fill_arg.i4DstY     = t_rect.i4_top;
		fill_arg.u4DstPitch = p_surf->ui4_pitch;
		fill_arg.eDstCm     = gl_cm_to_mtal_cm(p_surf->t_desc.e_clrmode);
		fill_arg.u4Width    = RECT_W(&t_rect);
		fill_arg.u4Height   = RECT_H(&t_rect);
		fill_arg.u4Color    = gl_compose_packed_color(pt_color, p_surf->t_desc.e_clrmode);

		
		i4_ret = MTGFX_FillRect(&fill_arg);
		MTGFX_Flush();
		MTGFX_Wait(-1);
	}
#endif
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    } 
    GL_DFB_SURF_FLIP(p_surf);
    }
    gl_dfb_obj_unlock(p_surf);

    return GLR_OK;

}

INT32 gl_dfb_surface_fillrect_blend(
    GL_HSURFACE_T       h_dst_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule,
    GL_COLORMODE_T      e_src_colormode,
    GL_COLOR_T          *pt_src_color,
    UINT32              ui4_flag)
{
#if 0
    GL_DFB_SURFACE_T *          p_surf;
    INT32                       i4_ret;
    DFBRegion                   tmp_clip_rg; 
    DFBRegion                   tmp_clip_rg_bak; 
    GL_RECT_T                   t_rect;
        
    if(h_dst_surf == NULL_HANDLE)
    {
        return GLR_INV_ARG;
    }
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_dst_surf, &i4_ret);
   
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }     
    
    t_rect.i4_left   = i4_dst_x;
    t_rect.i4_top    = i4_dst_y;
    t_rect.i4_right  = i4_dst_x + ui4_width;
    t_rect.i4_bottom = i4_dst_y + ui4_height;
    
    GL_RECT_TO_DFB_RECT(&t_rect, &tmp_clip_rg);
    
    i4_ret =  _prepare_blt_args(p_surf, GL_DFB_BLT_ARG_PD|GL_DFB_BLT_ARG_PREALPHA, e_rule, ui1_pre_alpha, 0, /*Avoid warn*/GL_ROP_NOT_SRC);
    if(i4_ret != GLR_OK)
    {
         
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    }  
    DFBCHECK(p_surf->p_dfb_surf->SetDrawingFlags(p_surf->p_dfb_surf, DSDRAW_BLEND), &i4_ret); 
    DFBCHECK((p_surf)->p_dfb_surf->GetClip((p_surf)->p_dfb_surf, &tmp_clip_rg_bak), &i4_ret);
    DFBCHECK((p_surf)->p_dfb_surf->SetClip((p_surf)->p_dfb_surf, &tmp_clip_rg), &i4_ret);

    
    DFBCHECK(p_surf->p_dfb_surf->SetColor(
                p_surf->p_dfb_surf,                
                pt_src_color->u1.r,
                pt_src_color->u2.g,
                pt_src_color->u3.b,
                pt_src_color->a), &i4_ret);

   DFBCHECK(p_surf->p_dfb_surf->FillRectangle(
            p_surf->p_dfb_surf,
            i4_dst_x,
            i4_dst_y,
            (INT32)ui4_width,
            (INT32)ui4_height), &i4_ret);
   
   DFBCHECK((p_surf)->p_dfb_surf->SetClip((p_surf)->p_dfb_surf, &tmp_clip_rg_bak), &i4_ret);
       

    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    } 
    GL_DFB_SURF_FLIP(p_surf);

    gl_dfb_obj_unlock(p_surf);
#endif
    return GLR_OK;

}

INT32 gl_dfb_surface_draw_rect(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T * p_surf;
    INT32 i4_ret;

    if(h_surf == NULL_HANDLE)
    {
        return GLR_INV_ARG;
    }
    if ((0 == ui2_line_width) && (GL_STYLE_OUTLINED == t_style))
	{
        return GLR_OK;
    }
    
    if (RECT_EMPTY(pt_rect))
    {
        GL_DFB_SURF_TRACE_API("[GL] Draw rectangle empty %s( %d )\n", __FUNCTION__, __LINE__);
        return GLR_OK;
    }

    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
   
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
    
    GL_DFB_SURF_TRACE_API(
		"[GL_DFB_SURF]Draw rect %d,%d,%d,%d with clip [%d,%d,%d,%d] color: [%x,%x,%x,%x] on p_surf:%x\n",
		pt_rect->i4_left,
		pt_rect->i4_top,
		pt_rect->i4_right,
		pt_rect->i4_bottom,
        p_surf->t_clip_rect.i4_left,
        p_surf->t_clip_rect.i4_top,
        p_surf->t_clip_rect.i4_right,
        p_surf->t_clip_rect.i4_bottom,
        pt_color == NULL?0:(UINT32)(pt_color->a),
        pt_color == NULL?0:(UINT32)(pt_color->u1.r),
        pt_color == NULL?0:(UINT32)(pt_color->u2.g),
        pt_color == NULL?0:(UINT32)(pt_color->u3.b),
        p_surf); 
    
 #if 0   
    i4_ret = _prepare_draw_args(p_surf, pt_color);
	
    if(i4_ret != GLR_OK)
    {
         
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    } 
    if(t_style == GL_STYLE_FILLED)
    {
        DFBCHECK(p_surf->p_dfb_surf->FillRectangle(
                p_surf->p_dfb_surf,
                pt_rect->i4_left, 
                pt_rect->i4_top,
                pt_rect->i4_right - pt_rect->i4_left, 
                pt_rect->i4_bottom - pt_rect->i4_top), &i4_ret);
    }
    else
    {
        if (ui2_line_width == 1)
        {
            DFBCHECK(p_surf->p_dfb_surf->DrawRectangle(
                    p_surf->p_dfb_surf, 
                    pt_rect->i4_left,
                    pt_rect->i4_top, 
                    pt_rect->i4_right - pt_rect->i4_left,
                    pt_rect->i4_bottom - pt_rect->i4_top), &i4_ret);
        }
        else
        {
            INT32   i4_line_offset = (INT32)(ui2_line_width / 2);

            /*Draw left */
            DFBCHECK(p_surf->p_dfb_surf->FillRectangle(
                p_surf->p_dfb_surf,
                pt_rect->i4_left - i4_line_offset, 
                pt_rect->i4_top - i4_line_offset,
                (INT32)ui2_line_width, 
                pt_rect->i4_bottom - pt_rect->i4_top + 2*i4_line_offset), &i4_ret);

            /*Draw right */
            DFBCHECK(p_surf->p_dfb_surf->FillRectangle(
                p_surf->p_dfb_surf,
                pt_rect->i4_right - i4_line_offset, 
                pt_rect->i4_top - i4_line_offset,
                (INT32)ui2_line_width, 
                pt_rect->i4_bottom - pt_rect->i4_top + 2*i4_line_offset), &i4_ret);

            /*Draw top*/
            DFBCHECK(p_surf->p_dfb_surf->FillRectangle(
                p_surf->p_dfb_surf,
                pt_rect->i4_left - i4_line_offset, 
                pt_rect->i4_top - i4_line_offset,
                pt_rect->i4_right - pt_rect->i4_left + 2*i4_line_offset, 
                (INT32)ui2_line_width), &i4_ret);

            
            /*Draw bottom*/
            DFBCHECK(p_surf->p_dfb_surf->FillRectangle(
                p_surf->p_dfb_surf,
                pt_rect->i4_left - i4_line_offset, 
                pt_rect->i4_bottom + i4_line_offset - (INT32)ui2_line_width,
                pt_rect->i4_right - pt_rect->i4_left + 2*i4_line_offset, 
                (INT32)ui2_line_width), &i4_ret);
    }
    }
#else
	if(t_style == GL_STYLE_FILLED)
	{
		MTGFX_FILL_T  fill_arg;
		
		fill_arg.pvDst      = (VOID*)p_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		fill_arg.i4DstX     = pt_rect->i4_left;
		fill_arg.i4DstY     = pt_rect->i4_top;
		fill_arg.u4DstPitch = p_surf->ui4_pitch;
		fill_arg.eDstCm     = gl_cm_to_mtal_cm(p_surf->t_desc.e_clrmode);
		fill_arg.u4Width    = RECT_W(pt_rect);
		fill_arg.u4Height   = RECT_H(pt_rect);
		fill_arg.u4Color    = gl_compose_packed_color(pt_color, p_surf->t_desc.e_clrmode);

		
		i4_ret = MTGFX_FillRect(&fill_arg);
		MTGFX_Flush();
		MTGFX_Wait(-1);
	}
	else
	{
		GL_RECT_T       t_work_rect = {0};
		UINT16          ui2_width_idx;
		GL_RECT_T       t_dst_clip;
		UINT32          ui4_color;
        gl_surface_get_clip_area(p_surf, &t_dst_clip);

        ui2_width_idx = 0;

        if (ui2_line_width > 1)
        {
            /* adjust vertices of this rectangle for the variable width */
            t_work_rect.i4_left   = pt_rect->i4_left   - (INT32)(ui2_line_width / 2);
            t_work_rect.i4_right  = pt_rect->i4_right  + (INT32)(ui2_line_width / 2);
            t_work_rect.i4_top    = pt_rect->i4_top    - (INT32)(ui2_line_width / 2);
            t_work_rect.i4_bottom = pt_rect->i4_bottom + (INT32)(ui2_line_width / 2);
        }
        
        do
        {
            INTERSECT_RECT(&t_dst_clip, &t_dst_clip, &t_work_rect);
            
            if (!RECT_EMPTY(&t_dst_clip))
            {
                ui4_color = gl_compose_packed_color(pt_color, p_surf->t_desc.e_clrmode);

				gl_dfb_locked_fill_rect(p_surf->t_desc.e_clrmode,
										(VOID*)p_surf->p_dfb_surf->bmp_buffer.u4VirtAddr,
										p_surf->ui4_pitch,
										t_work_rect.i4_left,
										t_work_rect.i4_top,
										RECT_W(&t_work_rect),
										RECT_H(&t_work_rect),
										ui4_color);
            }
            
            /* Shrink the boundary of this rectangle if the line width is greater than 1 */
            t_work_rect.i4_left   = t_work_rect.i4_left   + 1;
            t_work_rect.i4_right  = t_work_rect.i4_right  - 1;
            t_work_rect.i4_top    = t_work_rect.i4_top    + 1;
            t_work_rect.i4_bottom = t_work_rect.i4_bottom - 1;
            
            ui2_width_idx = (UINT16)(ui2_width_idx + 1);
        } while (ui2_line_width > 1 && ui2_width_idx < ui2_line_width);
        
	}
#endif
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    } 
    GL_DFB_SURF_FLIP(p_surf);

    gl_dfb_obj_unlock(p_surf);

    return GLR_OK;
}
INT32 gl_dfb_surface_flip(GL_HSURFACE_T h_surf, UINT32 ui4_flag)
{
#if 0
	GL_DFB_SURFACE_T * p_surf;  
    INT32       i4_ret;
	
    p_surf = (GL_DFB_SURFACE_T *)gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf,&i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
	
    DFBCHECK((p_surf)->p_dfb_surf->Flip((p_surf)->p_dfb_surf, NULL, 0), &i4_ret);

	gl_dfb_handle_unlock_obj((void*)p_surf);

#endif
	return GLR_OK;
}

INT32 gl_dfb_surface_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T * p_dst_surf;            
    GL_DFB_SURFACE_T * p_src_surf;            
	 HANDLE_T           ah_hd[2];
    HANDLE_TYPE_T      ae_type[2];
    void * ap_obj[2];
    INT32       i4_ret;

    ae_type[0]  = GLT_OBJ_SURFACE;
    ae_type[1]  = GLT_OBJ_SURFACE;
    ah_hd[0]    = h_dst_surf;
    ah_hd[1]    = h_src_surf;
	
   
    i4_ret = gl_dfb_mul_handles_lock_objs(ae_type, ah_hd, 2, ap_obj);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }     
    
    p_dst_surf = ap_obj[0];
    p_src_surf = ap_obj[1];
 #if 0
 	{
	    DFBRectangle  	   dfb_rect;

	    if ((ui4_flag & GL_BLT2FRONT) == 0)
	    {   
        dfb_rect.x = i4_src_x;
        dfb_rect.y = i4_src_y;
        dfb_rect.w = ui4_width;
        dfb_rect.h = ui4_height;
     
         
        
        /*MW WGL Calling this with alpha 0 -_-~~!*/
        i4_ret = _prepare_blt_args(p_dst_surf, GL_DFB_BLT_ARG_PREALPHA, 0, 255/*ui1_alpha_assigned*/, 0, /*Avoid warn*/GL_ROP_NOT_SRC);
        if(i4_ret != GLR_OK)
        {
                 
            gl_dfb_mul_objs_unlock(ap_obj, 2);
            return i4_ret;
        }     
        GL_DFB_TRACE_API(
    		"[GL_DFB_SURF] Blt %d,%d,%d,%d,%d,%d\
     with clip[%d,%d,%d,%d] src clip :[%d,%d,%d,%d]from p_surf:%x to p_surf:%x preA:%d\n",
    		i4_dst_x,
    		i4_dst_y,
    		i4_src_x,
    		i4_src_y,
    		ui4_width,
    		ui4_height, 	
            p_dst_surf->t_clip_rect.i4_left,
            p_dst_surf->t_clip_rect.i4_top,
            p_dst_surf->t_clip_rect.i4_right,
            p_dst_surf->t_clip_rect.i4_bottom,                
            p_src_surf->t_clip_rect.i4_left,
            p_src_surf->t_clip_rect.i4_top,
            p_src_surf->t_clip_rect.i4_right,
            p_src_surf->t_clip_rect.i4_bottom,                
            p_src_surf,
            p_dst_surf,
            ui1_alpha_assigned);
    	
    	DFBCHECK(p_src_surf->p_dfb_surf->SetClip(p_src_surf->p_dfb_surf, NULL), &i4_ret);
        DFBCHECK(p_dst_surf->p_dfb_surf->Blit(
            p_dst_surf->p_dfb_surf, 
            p_src_surf->p_dfb_surf,
            &dfb_rect,
            i4_dst_x,
            i4_dst_y), &i4_ret);
        
        DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);

        if(i4_ret != GLR_OK)
        {
            gl_dfb_mul_objs_unlock(ap_obj, 2);
            return i4_ret;
        }     
        
        GL_DFB_SURF_FLIP(p_dst_surf);
    }
    else
    {
        DFBRegion   t_region;

        p_dst_surf = ap_obj[0];
        p_src_surf = ap_obj[1];

        t_region.x1 = i4_src_x;
        t_region.y1 = i4_src_y;
        t_region.x2 = i4_src_x + ui4_width;
        t_region.y2 = i4_src_y + ui4_height;
        
        GL_DFB_SURF_FLIP_WITH_REGION(p_src_surf, &t_region);
    }
 	}
#else
	{
		MTGFX_BITBLT_T  blit_arg;
		blit_arg.u1Alpha       = ui1_alpha_assigned;
		blit_arg.pvSrc         = (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.i4SrcX        = i4_src_x;
		blit_arg.i4SrcY        = i4_src_y;
		blit_arg.u4SrcPitch    = p_src_surf->ui4_pitch;
		blit_arg.eSrcCm        = gl_cm_to_mtal_cm(p_src_surf->t_desc.e_clrmode);
		
		blit_arg.pvDst         = (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.i4DstX        = i4_dst_x;
		blit_arg.i4DstY        = i4_dst_y;
		blit_arg.u4DstPitch    = p_dst_surf->ui4_pitch;
		blit_arg.eDstCm        = gl_cm_to_mtal_cm(p_dst_surf->t_desc.e_clrmode);

		blit_arg.u4Width       = ui4_width;
		blit_arg.u4Height      = ui4_height;

		i4_ret = MTGFX_Bitblt(&blit_arg);
		if (i4_ret < 0)
		{
			gl_dfb_mul_objs_unlock(ap_obj, 2);
			return GLR_DRV_ERROR;
		}
		MTGFX_Flush();
		MTGFX_Wait(-1);
	}
#endif
    gl_dfb_mul_objs_unlock(ap_obj, 2);
    return GLR_OK;
}
INT32 gl_dfb_surface_stretch_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T * p_dst_surf;            
    GL_DFB_SURFACE_T * p_src_surf;            
    
    HANDLE_T           ah_hd[2];
    HANDLE_TYPE_T      ae_type[2];
    void * ap_obj[2];
    INT32 i4_ret;
    
    ae_type[0]  = GLT_OBJ_SURFACE;
    ae_type[1]  = GLT_OBJ_SURFACE;
    ah_hd[0]    = h_dst_surf;
    ah_hd[1]    = h_src_surf;

    i4_ret = gl_dfb_mul_handles_lock_objs(ae_type, ah_hd, 2, ap_obj);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }     
    
    p_dst_surf = ap_obj[0];
    p_src_surf = ap_obj[1];

#if 0
	{
		DFBRectangle  	   dfb_src_rect;
	    DFBRectangle  	   dfb_dst_rect;
	    
    dfb_src_rect.x = i4_src_x;
    dfb_src_rect.y = i4_src_y;
    dfb_src_rect.w = ui4_src_w;
    dfb_src_rect.h = ui4_src_h;

    dfb_dst_rect.x = i4_dst_x;
    dfb_dst_rect.y = i4_dst_y;
    dfb_dst_rect.w = ui4_dst_w;
    dfb_dst_rect.h = ui4_dst_h;

    /*x_dbg_stmt("\r\n[gl_dfb_surface_stretch_blt]>>>>sx %d sy %d sw %d sh %d dx %d dy %d dw %d dh %d\r\n",
    i4_src_x,i4_src_y,ui4_src_w,ui4_src_h,i4_dst_x,i4_dst_y,ui4_dst_w,ui4_dst_h);*/
         
    i4_ret = _prepare_blt_args(p_dst_surf, GL_DFB_BLT_ARG_NO, 0, 0, 0, /*Avoid warn*/GL_ROP_NOT_SRC);
    if(i4_ret != GLR_OK)
    {
             
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }     
    DFBCHECK(p_src_surf->p_dfb_surf->SetClip(p_src_surf->p_dfb_surf, NULL), &i4_ret);
    DFBCHECK(p_dst_surf->p_dfb_surf->StretchBlit(
        p_dst_surf->p_dfb_surf, 
        p_src_surf->p_dfb_surf,
        &dfb_src_rect,
        &dfb_dst_rect), &i4_ret);

    DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);
	}
#else
	{
		MTGFX_SCALER_T     		scaler_arg;
		scaler_arg.pvSrc         = (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		scaler_arg.i4SrcX        = i4_src_x;
		scaler_arg.i4SrcY        = i4_src_y;
		scaler_arg.u4SrcPitch    = p_src_surf->ui4_pitch;
		scaler_arg.eSrcCm        = gl_cm_to_mtal_cm(p_src_surf->t_desc.e_clrmode);
		
		scaler_arg.pvDst         = (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		scaler_arg.i4DstX        = i4_dst_x;
		scaler_arg.i4DstY        = i4_dst_y;
		scaler_arg.u4DstPitch    = p_dst_surf->ui4_pitch;
		scaler_arg.eDstCm        = gl_cm_to_mtal_cm(p_dst_surf->t_desc.e_clrmode);

		scaler_arg.u4Width       = ui4_src_w;
		scaler_arg.u4Height      = ui4_src_h;

		scaler_arg.u4DstWidth    = ui4_dst_w;
		scaler_arg.u4DstHeight   = ui4_dst_h;

		i4_ret = MTGFX_Scaler(&scaler_arg);
	}
#endif
    if(i4_ret != GLR_OK)
    {
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }     
    
    GL_DFB_SURF_FLIP(p_dst_surf);
    
    gl_dfb_mul_objs_unlock(ap_obj, 2);
    return GLR_OK;
}

INT32 gl_dfb_surface_pat_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_alpha_assigned,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T * p_dst_surf;            
    GL_DFB_SURFACE_T * p_src_surf;            
    /*DFBRectangle  	   dfb_dst_rect;*/
    
    //int                dfb_dst_x;
    //int                dfb_dst_y;
    //int                 i4_rgn_count = 0;
    
    HANDLE_T           ah_hd[2];
    HANDLE_TYPE_T      ae_type[2];
    void * ap_obj[2];
    INT32   i4_ret;

    ae_type[0]  = GLT_OBJ_SURFACE;
    ae_type[1]  = GLT_OBJ_SURFACE;
    ah_hd[0]    = h_dst_surf;
    ah_hd[1]    = h_src_surf;
        
    i4_ret = gl_dfb_mul_handles_lock_objs(ae_type, ah_hd, 2, ap_obj);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }     

    p_dst_surf = ap_obj[0];
    p_src_surf = ap_obj[1];
	
#if 0
	{
		        
	    DFBRectangle  	   dfb_src_rect;
	    DFBPoint            pt_point;
		
    if (_pt_pat_compose_point == NULL)
    {
        _pt_pat_compose_point = (DFBPoint *)x_mem_alloc(_i4_pat_compose_num * sizeof(DFBPoint));

        if (_pt_pat_compose_point == NULL)
        {
            return GLR_NO_MEMORY;
        }
    }

    if (_pt_pat_compose_rect == NULL)
    {
        _pt_pat_compose_rect = (DFBRectangle *)x_mem_alloc(_i4_pat_compose_num * sizeof(DFBRectangle));

        if (_pt_pat_compose_rect == NULL)
        {
            return GLR_NO_MEMORY;
        }
    }
		
    dfb_src_rect.x = i4_src_x;
    dfb_src_rect.y = i4_src_y;
    dfb_src_rect.w = ui4_src_w;
    dfb_src_rect.h = ui4_src_h;

    i4_ret = _prepare_blt_args(p_dst_surf, GL_DFB_BLT_ARG_NO, 0, 0, 0, /*Avoid warn*/GL_ROP_NOT_SRC);
    if(i4_ret != GLR_OK)
    {
             
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }
    GL_DFB_SURF_TRACE_API(
		"[GL_DFB_SURF] Pat blt %d,%d,%d,%d, %d,%d,%d,%d\
 with clip[%d,%d,%d,%d] src clip :[%d,%d,%d,%d]from p_surf:%x to p_surf:%x preA:%d\n",
		i4_dst_x,
		i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
		i4_src_x,
		i4_src_y,
		ui4_src_w,
		ui4_src_h, 	
        p_dst_surf->t_clip_rect.i4_left,
        p_dst_surf->t_clip_rect.i4_top,
        p_dst_surf->t_clip_rect.i4_right,
        p_dst_surf->t_clip_rect.i4_bottom,                
        p_src_surf->t_clip_rect.i4_left,
        p_src_surf->t_clip_rect.i4_top,
        p_src_surf->t_clip_rect.i4_right,
        p_src_surf->t_clip_rect.i4_bottom,                
        p_src_surf,
        p_dst_surf,
        ui1_alpha_assigned);

    
    _pat_clip_realign(p_dst_surf, i4_dst_x, i4_dst_y, ui4_dst_w, ui4_dst_h);

    i4_rgn_count = 0;
     
    for(dfb_dst_x = i4_dst_x; (UINT32)(dfb_dst_x - i4_dst_x) < ui4_dst_w; dfb_dst_x += (INT32)ui4_src_w)
    {
        pt_point.x = dfb_dst_x;
        dfb_src_rect.w = ((i4_dst_x + (INT32)ui4_dst_w - dfb_dst_x - dfb_src_rect.w) >= 0 ) ? 
                                                        (INT32)ui4_src_w : (i4_dst_x + (INT32)ui4_dst_w - dfb_dst_x);
        
        for(dfb_dst_y = i4_dst_y; (UINT32)(dfb_dst_y - i4_dst_y) < ui4_dst_h; dfb_dst_y += (INT32)ui4_src_h)
        {
            pt_point.y = dfb_dst_y;
            dfb_src_rect.h = ((i4_dst_y + (INT32)ui4_dst_h - dfb_dst_y - dfb_src_rect.h) >= 0 )? 
                                                        (INT32)ui4_src_h : (i4_dst_y + (INT32)ui4_dst_h - dfb_dst_y);
               
            _pt_pat_compose_rect [i4_rgn_count] = dfb_src_rect;
            _pt_pat_compose_point[i4_rgn_count] = pt_point;

            i4_rgn_count++;

            if (i4_rgn_count >= _i4_pat_compose_num)
            {
                _i4_pat_compose_num += 10;

                _pt_pat_compose_point = (DFBPoint *)x_mem_realloc(_pt_pat_compose_point, _i4_pat_compose_num * sizeof(DFBPoint));

                if (_pt_pat_compose_point == NULL)
                {
                    gl_dfb_mul_objs_unlock(ap_obj, 2);
                    return GLR_NO_MEMORY;
                }

                _pt_pat_compose_rect = (DFBRectangle *)x_mem_realloc(_pt_pat_compose_rect, _i4_pat_compose_num * sizeof(DFBRectangle));

                if (_pt_pat_compose_rect == NULL)
                {
                    gl_dfb_mul_objs_unlock(ap_obj, 2);
                    return GLR_NO_MEMORY;
                }
            }
        }     
    }

    DFBCHECK(p_dst_surf->p_dfb_surf->BatchBlit(
                p_dst_surf->p_dfb_surf, 
                p_src_surf->p_dfb_surf,
                _pt_pat_compose_rect,
                _pt_pat_compose_point,
                i4_rgn_count), &i4_ret);
                       
    if(i4_ret != GLR_OK)
    {
        DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);     
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }
     
     
    DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);
	}
#else
	{
		INT32					x,y;
		MTGFX_BITBLT_T		    blit_arg;
		blit_arg.u1Alpha	   = ui1_alpha_assigned;
		blit_arg.pvSrc		   = (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.i4SrcX 	   = i4_src_x;
		blit_arg.i4SrcY 	   = i4_src_y;
		blit_arg.u4SrcPitch    = p_src_surf->ui4_pitch;
		blit_arg.eSrcCm 	   = gl_cm_to_mtal_cm(p_src_surf->t_desc.e_clrmode);
		
		blit_arg.pvDst		   = (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.u4DstPitch    = p_dst_surf->ui4_pitch;
		blit_arg.eDstCm 	   = gl_cm_to_mtal_cm(p_dst_surf->t_desc.e_clrmode);

		blit_arg.u4Width	   = ui4_src_w;
		blit_arg.u4Height	   = ui4_src_h;
		
		for(x = i4_dst_x; (UINT32)(x - i4_dst_x) < ui4_dst_w; x += (INT32)ui4_src_w)
		{
			blit_arg.i4DstX 	   = x;
			blit_arg.u4Width	   = ((i4_dst_x + (INT32)ui4_dst_w - x - ui4_src_w) >= 0 ) ? 
															(INT32)ui4_src_w : (i4_dst_x + (INT32)ui4_dst_w - x);
			for(y = i4_dst_y; (UINT32)(y - i4_dst_y) < ui4_dst_h; y += (INT32)ui4_src_h)
			{
				blit_arg.i4DstY 	   = y;

				blit_arg.u4Height	   = ((i4_dst_y + (INT32)ui4_dst_h - y - ui4_src_h) >= 0 )? 
															(INT32)ui4_src_h : (i4_dst_y + (INT32)ui4_dst_h - y);
				i4_ret = MTGFX_Bitblt(&blit_arg);

				if (i4_ret < 0)
				{
					gl_dfb_mul_objs_unlock(ap_obj, 2);
					return GLR_DRV_ERROR;
				}
			}	  
		}
		MTGFX_Flush();
		MTGFX_Wait(-1);
	}
#endif
	
    GL_DFB_SURF_FLIP(p_dst_surf);
    
    gl_dfb_mul_objs_unlock(ap_obj, 2);


    return GLR_OK;
}

INT32 gl_dfb_surface_transp_pat_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_alpha_assigned,
    BOOL                b_off_mask,
    const GL_COLOR_T*   pt_color_min,
    const GL_COLOR_T*   pt_color_max,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T *  p_dst_surf;            
    GL_DFB_SURFACE_T *  p_src_surf;     
    /*DFBRectangle  	   dfb_dst_rect;*/
    
    //int                 dfb_dst_x;
    //int                 dfb_dst_y;
    
    HANDLE_T            ah_hd[2];
    HANDLE_TYPE_T       ae_type[2];
    void*               ap_obj[2];
    INT32               i4_ret;

    //int                 i4_rgn_count = 0;
 
    ae_type[0]  = GLT_OBJ_SURFACE;
    ae_type[1]  = GLT_OBJ_SURFACE;
    ah_hd[0]    = h_dst_surf;
    ah_hd[1]    = h_src_surf;
 
    i4_ret = gl_dfb_mul_handles_lock_objs(ae_type, ah_hd, 2, ap_obj);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }     
    
    p_dst_surf = ap_obj[0];
    p_src_surf = ap_obj[1];

    GL_DFB_SURF_TRACE_API(
		"[GL_DFB_SURF] Transp pat blt\
        %d,%d,%d,%d, %d,%d,%d,%d with clip[%d,%d,%d,%d] src clip :[%d,%d,%d,%d]\
        from p_surf:%x to p_surf:%x, preA:%d, off_mask:%d, cmin:%x, cmax:%x\n",
		i4_dst_x,
		i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
		i4_src_x,
		i4_src_y,
		ui4_src_w,
		ui4_src_h, 	
        p_dst_surf->t_clip_rect.i4_left,
        p_dst_surf->t_clip_rect.i4_top,
        p_dst_surf->t_clip_rect.i4_right,
        p_dst_surf->t_clip_rect.i4_bottom,                
        p_src_surf->t_clip_rect.i4_left,
        p_src_surf->t_clip_rect.i4_top,
        p_src_surf->t_clip_rect.i4_right,
        p_src_surf->t_clip_rect.i4_bottom,                
        p_src_surf,
        p_dst_surf,
        ui1_alpha_assigned,
        b_off_mask,
        GL_COLOR_ARGB8888(pt_color_min->a, pt_color_min->u1.r, pt_color_min->u2.g, pt_color_min->u3.b),
        GL_COLOR_ARGB8888(pt_color_max->a, pt_color_max->u1.r, pt_color_max->u2.g, pt_color_max->u3.b));
    
#if 0
	{
		DFBPoint			pt_point;
	    DFBRectangle  	    dfb_src_rect;
		
		 if (_pt_pat_transp_point == NULL)
		 {
			 _pt_pat_transp_point = (DFBPoint *)x_mem_alloc(_i4_pat_transp_num * sizeof(DFBPoint));
		
			 if (_pt_pat_transp_point == NULL)
			 {
				 return GLR_NO_MEMORY;
			 }
		 }
		
		 if (_pt_pat_transp_rect == NULL)
		 {
			 _pt_pat_transp_rect = (DFBRectangle *)x_mem_alloc(_i4_pat_transp_num * sizeof(DFBRectangle));
		
			 if (_pt_pat_transp_rect == NULL)
			 {
				 return GLR_NO_MEMORY;
			 }
		 }
		dfb_src_rect.x = i4_src_x;
	    dfb_src_rect.y = i4_src_y;
	    dfb_src_rect.w = ui4_src_w;
	    dfb_src_rect.h = ui4_src_h;
    if(b_off_mask)
    {
        i4_ret = _prepare_blt_args(p_dst_surf, GL_DFB_BLT_ARG_COLORKEY|GL_DFB_BLT_ARG_PREALPHA, 0, ui1_alpha_assigned, pt_color_min, (GL_ROP_TYPE_T)0);
    }
    else
    {
        i4_ret = _prepare_blt_args(p_dst_surf, GL_DFB_BLT_ARG_PREALPHA, 0, ui1_alpha_assigned, 0, /*Avoid warn*/GL_ROP_NOT_SRC);
    }
    if(i4_ret != GLR_OK)
    {
             
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }
    _pat_clip_realign(p_dst_surf, i4_dst_x, i4_dst_y, ui4_dst_w, ui4_dst_h);

    i4_rgn_count = 0;
    
    for(dfb_dst_x = i4_dst_x; (UINT32)(dfb_dst_x - i4_dst_x) < ui4_dst_w; dfb_dst_x += (INT32)ui4_src_w)
    {
        pt_point.x = dfb_dst_x;
        dfb_src_rect.w = ((i4_dst_x + (INT32)ui4_dst_w - dfb_dst_x - dfb_src_rect.w) >= 0 ) ? 
                                                        (INT32)ui4_src_w : (i4_dst_x + (INT32)ui4_dst_w - dfb_dst_x);
        
        for(dfb_dst_y = i4_dst_y; (UINT32)(dfb_dst_y - i4_dst_y) < ui4_dst_h; dfb_dst_y += (INT32)ui4_src_h)
        {
            pt_point.y = dfb_dst_y;
            dfb_src_rect.h = ((i4_dst_y + (INT32)ui4_dst_h - dfb_dst_y - dfb_src_rect.h) >= 0 )? 
                                                        (INT32)ui4_src_h : (i4_dst_y + (INT32)ui4_dst_h - dfb_dst_y);
            

            _pt_pat_transp_rect [i4_rgn_count] = dfb_src_rect;
            _pt_pat_transp_point[i4_rgn_count] = pt_point;

            i4_rgn_count++;

            if (i4_rgn_count >= _i4_pat_transp_num)
            {
                _i4_pat_transp_num += 10;

                _pt_pat_transp_point = (DFBPoint *)x_mem_realloc(_pt_pat_transp_point, _i4_pat_transp_num * sizeof(DFBPoint));

                if (_pt_pat_transp_point == NULL)
                {
                    gl_dfb_mul_objs_unlock(ap_obj, 2);
                    return GLR_NO_MEMORY;
                }

                _pt_pat_transp_rect = (DFBRectangle *)x_mem_realloc(_pt_pat_transp_rect, _i4_pat_transp_num * sizeof(DFBRectangle));

                if (_pt_pat_transp_rect == NULL)
                {
                    gl_dfb_mul_objs_unlock(ap_obj, 2);
                    return GLR_NO_MEMORY;
                }
            }
        }     
    }

    DFBCHECK(p_dst_surf->p_dfb_surf->BatchBlit(
                p_dst_surf->p_dfb_surf, 
                p_src_surf->p_dfb_surf,
                _pt_pat_transp_rect,
                _pt_pat_transp_point,
                i4_rgn_count), &i4_ret);

    if(i4_ret != GLR_OK)
    {
        DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);         
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }

    DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);
	}
 #else
 	{
 		INT32                       x,y;
    	MTGFX_TRANSPARENT_BITBLT_T blt_arg;

		blt_arg.pvSrc		 = (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blt_arg.i4SrcX		 = i4_src_x;
		blt_arg.i4SrcY		 = i4_src_y;
		blt_arg.u4SrcPitch	 = p_src_surf->ui4_pitch;
		blt_arg.eSrcCm		 = gl_cm_to_mtal_cm(p_src_surf->t_desc.e_clrmode);
		
		blt_arg.pvDst		 = (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blt_arg.i4DstX		 = i4_dst_x;
		blt_arg.i4DstY		 = i4_dst_y;
		blt_arg.u4DstPitch	 = p_dst_surf->ui4_pitch;
		blt_arg.eDstCm		 = gl_cm_to_mtal_cm(p_dst_surf->t_desc.e_clrmode);

		blt_arg.u4Width		 = ui4_src_w;
		blt_arg.u4Height 	 = ui4_src_h;
		
		blt_arg.bInverseMasking	 = !b_off_mask;
		blt_arg.u4ColorSpaceMin	 = gl_compose_packed_color(pt_color_min,p_src_surf->t_desc.e_clrmode);
		blt_arg.u4ColorSpaceMax	 = gl_compose_packed_color(pt_color_max,p_dst_surf->t_desc.e_clrmode);


		for(x = i4_dst_x; (UINT32)(x - i4_dst_x) < ui4_dst_w; x += (INT32)ui4_src_w)
	    {
			blt_arg.i4DstX        = x;
	        blt_arg.u4Width       = ((i4_dst_x + (INT32)ui4_dst_w - x - ui4_src_w) >= 0 ) ? 
	                                                        (INT32)ui4_src_w : (i4_dst_x + (INT32)ui4_dst_w - x);
	        for(y = i4_dst_y; (UINT32)(y - i4_dst_y) < ui4_dst_h; y += (INT32)ui4_src_h)
	        {
				blt_arg.i4DstY        = y;

				blt_arg.u4Height      = ((i4_dst_y + (INT32)ui4_dst_h - y - ui4_src_h) >= 0 )? 
	                                                        (INT32)ui4_src_h : (i4_dst_y + (INT32)ui4_dst_h - y);
	            i4_ret = MTGFX_TransparentBitblt(&blt_arg);

				if (i4_ret < 0)
				{
   					gl_dfb_mul_objs_unlock(ap_obj, 2);
					return GLR_DRV_ERROR;
				}
	        }     
	    }
		
		MTGFX_Flush();
		MTGFX_Wait(-1);
 	}
 #endif
    GL_DFB_SURF_FLIP(p_dst_surf);
    
    gl_dfb_mul_objs_unlock(ap_obj, 2);


    return GLR_OK;

}

INT32 gl_dfb_surface_transp_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    BOOL                b_off_mask,
    const GL_COLOR_T*   pt_color_min,
    const GL_COLOR_T*   pt_color_max,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T *  p_dst_surf;            
    GL_DFB_SURFACE_T *  p_src_surf;            
    
    HANDLE_T            ah_hd[2];
    HANDLE_TYPE_T       ae_type[2];
    void * ap_obj[2];
    INT32               i4_ret;
    ae_type[0]  = GLT_OBJ_SURFACE;
    ae_type[1]  = GLT_OBJ_SURFACE;
    ah_hd[0]    = h_dst_surf;
    ah_hd[1]    = h_src_surf;

    i4_ret = gl_dfb_mul_handles_lock_objs(ae_type, ah_hd, 2, ap_obj);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }     
    
    p_dst_surf = ap_obj[0];
    p_src_surf = ap_obj[1];

#if 0
	{
	    DFBRectangle  	    dfb_rect;

    dfb_rect.x = i4_src_x;
    dfb_rect.y = i4_src_y;
    dfb_rect.w = ui4_width;
    dfb_rect.h = ui4_height;
    
    GL_DFB_TRACE_API(
		"[GL_DFB_SURF] Transp blt\
        %d,%d,   %d,%d,%d,%d with clip[%d,%d,%d,%d] src clip :[%d,%d,%d,%d]\
        from p_surf:%x to p_surf:%x, preA:%d, off_mask:%d, cmin:%x, cmax:%x\n",
		i4_dst_x,
		i4_dst_y,       
		i4_src_x,
		i4_src_y,
		ui4_width,
		ui4_height, 	
        p_dst_surf->t_clip_rect.i4_left,
        p_dst_surf->t_clip_rect.i4_top,
        p_dst_surf->t_clip_rect.i4_right,
        p_dst_surf->t_clip_rect.i4_bottom,                
        p_src_surf->t_clip_rect.i4_left,
        p_src_surf->t_clip_rect.i4_top,
        p_src_surf->t_clip_rect.i4_right,
        p_src_surf->t_clip_rect.i4_bottom,                
        p_src_surf,
        p_dst_surf,
        ui1_alpha_assigned,
        b_off_mask,
        GL_COLOR_ARGB8888(pt_color_min->a, pt_color_min->u1.r, pt_color_min->u2.g, pt_color_min->u3.b),
        GL_COLOR_ARGB8888(pt_color_max->a, pt_color_max->u1.r, pt_color_max->u2.g, pt_color_max->u3.b));

     
    i4_ret = _prepare_blt_args(p_dst_surf, GL_DFB_BLT_ARG_COLORKEY|GL_DFB_BLT_ARG_PREALPHA, 0, ui1_alpha_assigned, pt_color_min, /*Avoid warn*/GL_ROP_NOT_SRC);
    if(i4_ret != GLR_OK)
    {
         
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }     
    
    DFBCHECK(p_dst_surf->p_dfb_surf->Blit(
        p_dst_surf->p_dfb_surf, 
        p_src_surf->p_dfb_surf,
        &dfb_rect,
        i4_dst_x,
        i4_dst_y), &i4_ret);
   
    DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);
		if(i4_ret != GLR_OK)
	    {     
	        gl_dfb_mul_objs_unlock(ap_obj, 2);     
	        return i4_ret;
	    }     
	    
	}
#else
	{
    	MTGFX_TRANSPARENT_BITBLT_T blt_arg;

		blt_arg.pvSrc		 = (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blt_arg.i4SrcX		 = i4_src_x;
		blt_arg.i4SrcY		 = i4_src_y;
		blt_arg.u4SrcPitch	 = p_src_surf->ui4_pitch;
		blt_arg.eSrcCm		 = gl_cm_to_mtal_cm(p_src_surf->t_desc.e_clrmode);
		
		blt_arg.pvDst		 = (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blt_arg.i4DstX		 = i4_dst_x;
		blt_arg.i4DstY		 = i4_dst_y;
		blt_arg.u4DstPitch	 = p_dst_surf->ui4_pitch;
		blt_arg.eDstCm		 = gl_cm_to_mtal_cm(p_dst_surf->t_desc.e_clrmode);

		blt_arg.u4Width		 = ui4_width;
		blt_arg.u4Height 	 = ui4_height;
		
		blt_arg.bInverseMasking	 = !b_off_mask;
		blt_arg.u4ColorSpaceMin	 = gl_compose_packed_color(pt_color_min,p_src_surf->t_desc.e_clrmode);
		blt_arg.u4ColorSpaceMax	 = gl_compose_packed_color(pt_color_max,p_dst_surf->t_desc.e_clrmode);


        i4_ret = MTGFX_TransparentBitblt(&blt_arg);

		if (i4_ret < 0)
		{
				gl_dfb_mul_objs_unlock(ap_obj, 2);
			return GLR_DRV_ERROR;
		}

		MTGFX_Flush();
		MTGFX_Wait(-1);
	}
#endif
	
    GL_DFB_SURF_FLIP(p_dst_surf);
    
    gl_dfb_mul_objs_unlock(ap_obj, 2);

_TIME_END

    return GLR_OK;
}

INT32 gl_dfb_surface_rotate_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    UINT32              ui4_rot_op,
    UINT32              ui4_flag)
{

	GL_DFB_SURFACE_T *  p_dst_surf;            
	GL_DFB_SURFACE_T *  p_src_surf;            
	BOOL                b_flip;
	UINT32              ui4_angle;    
	HANDLE_T            ah_hd[2];
	HANDLE_TYPE_T       ae_type[2];
	void *              ap_obj[2];
	INT32               i4_ret;

	ae_type[0]  = GLT_OBJ_SURFACE;
	ae_type[1]  = GLT_OBJ_SURFACE;
	ah_hd[0]    = h_dst_surf;
	ah_hd[1]    = h_src_surf;

	i4_ret = gl_dfb_mul_handles_lock_objs(ae_type, ah_hd, 2, ap_obj);
	if(i4_ret != GLR_OK)
	{
	    gl_dfb_mul_objs_unlock(ap_obj, 2);
	    return i4_ret;
	}     

	p_dst_surf = ap_obj[0];
	p_src_surf = ap_obj[1];

	b_flip = ((ui4_rot_op & GL_BLT_FLIP) != 0);
	ui4_angle = (ui4_rot_op & GL_BLT_ROTATE_MASK);

	if (ui4_angle == GL_BLT_ROTATE_NONE && !b_flip)
#if 0
	{
	    gl_dfb_surface_blt(
	        h_dst_surf, 
	        h_src_surf, 
	        i4_dst_x,
	        i4_dst_y,
	        i4_src_x,
	        i4_src_y,
	        ui4_width,
	        ui4_height,
	        ui1_alpha_assigned,
	        ui4_flag);
	}
#ifdef MW_PHOTO_SHOW_ON_VDP
	else if (p_dst_surf->t_desc.e_clrmode == GL_COLORMODE_YUV_422_RS)
	{
		DFBSurfaceBlittingFlags 	dfb_blt_flag = DSBLIT_NOFX;
		DFBRectangle  	   dfb_rect;

		dfb_rect.x = i4_src_x;
	    dfb_rect.y = i4_src_y;
	    dfb_rect.w = ui4_width;
	    dfb_rect.h = ui4_height;
		
		/*Clear the blending options*/
		_dfb_surface_clear_blit_opts(p_dst_surf->p_dfb_surf);

		DFBCHECK(p_dst_surf->p_dfb_surf->SetColor(p_dst_surf->p_dfb_surf, ui1_alpha_assigned, ui1_alpha_assigned, ui1_alpha_assigned, ui1_alpha_assigned), &i4_ret);
		RETURN_RET_IF_FAIL(i4_ret);

		switch (ui4_rot_op & GL_BLT_ROTATE_MASK)
		{
			case GL_BLT_ROTATE_CW90:
				 dfb_blt_flag |= DSBLIT_ROTATE90;
				 break;
				 
			case GL_BLT_ROTATE_CW180:
				 dfb_blt_flag |= DSBLIT_ROTATE180;
				 break;
				 
			case GL_BLT_ROTATE_CW270:
				 dfb_blt_flag |= DSBLIT_ROTATE270;
				 break;
				 
				default:
					break;
		}

		if (ui4_rot_op & GL_BLT_FLIP)
		{
			dfb_blt_flag |= DSBLIT_FLIP_HORIZONTAL;
		}
		
		DFBCHECK(p_dst_surf->p_dfb_surf->SetBlittingFlags(p_dst_surf->p_dfb_surf,
														  dfb_blt_flag), &i4_ret);

		DFBCHECK(p_src_surf->p_dfb_surf->SetClip(p_src_surf->p_dfb_surf, NULL), &i4_ret);
	    DFBCHECK(p_dst_surf->p_dfb_surf->Blit(
	        p_dst_surf->p_dfb_surf, 
	        p_src_surf->p_dfb_surf,
	        &dfb_rect,
	        i4_dst_x,
	        i4_dst_y), &i4_ret);
	    

	    if(i4_ret != GLR_OK)
	    {
	        gl_dfb_mul_objs_unlock(ap_obj, 2);
	        return i4_ret;
	    }     
	    DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);
	
	}
#endif /*Show on VDP*/
#else
    {
		MTGFX_BITBLT_T  blit_arg;
		blit_arg.u1Alpha       = ui1_alpha_assigned;
		blit_arg.pvSrc         = (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.i4SrcX        = i4_src_x;
		blit_arg.i4SrcY        = i4_src_y;
		blit_arg.u4SrcPitch    = p_src_surf->ui4_pitch;
		blit_arg.eSrcCm        = gl_cm_to_mtal_cm(p_src_surf->t_desc.e_clrmode);
		
		blit_arg.pvDst         = (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.i4DstX        = i4_dst_x;
		blit_arg.i4DstY        = i4_dst_y;
		blit_arg.u4DstPitch    = p_dst_surf->ui4_pitch;
		blit_arg.eDstCm        = gl_cm_to_mtal_cm(p_dst_surf->t_desc.e_clrmode);

		blit_arg.u4Width       = ui4_width;
		blit_arg.u4Height      = ui4_height;

		i4_ret = MTGFX_Bitblt(&blit_arg);
		if (i4_ret < 0)
		{
			gl_dfb_mul_objs_unlock(ap_obj, 2);
			return GLR_DRV_ERROR;
		}
		MTGFX_Flush();
		MTGFX_Wait(-1);
	}
	else if (ui4_angle == GL_BLT_ROTATE_CW90 && !b_flip)
    {
	   
	   MTGFX_ROTATE_T		   rotate_arg;

	   rotate_arg.pvSrc 		= (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
	   rotate_arg.i4SrcX		= i4_src_x;
	   rotate_arg.i4SrcY		= i4_src_y;
	   rotate_arg.u4SrcPitch	= p_src_surf->ui4_pitch;
	   rotate_arg.eSrcCm		= gl_cm_to_mtal_cm(p_src_surf->t_desc.e_clrmode);
	   
	   rotate_arg.pvDst 		= (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
	   rotate_arg.i4DstX		= i4_dst_x;
	   rotate_arg.i4DstY		= i4_dst_y;
	   rotate_arg.u4DstPitch	= p_dst_surf->ui4_pitch;
	   rotate_arg.eDstCm		= gl_cm_to_mtal_cm(p_dst_surf->t_desc.e_clrmode);

	   rotate_arg.u4Width		= ui4_width;
	   rotate_arg.u4Height		= ui4_height;

	   rotate_arg.eDirection	= MTGFX_CLOCKWISE;
	   
	   i4_ret = MTGFX_Rotate90(&rotate_arg);

	   if (i4_ret < 0)
	   {
		   gl_dfb_mul_objs_unlock(ap_obj, 2);
			return GLR_DRV_ERROR;
	   }
	   
	   MTGFX_Flush();
	   
	   MTGFX_Wait(-1);
		
	   gl_dfb_mul_objs_unlock(ap_obj, 2);
	   
	   return GLR_OK;
   
   }
   else if (ui4_angle == GL_BLT_ROTATE_CW270 && !b_flip)
   {
	   
	   MTGFX_ROTATE_T		   rotate_arg;

	   rotate_arg.pvSrc 		= (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
	   rotate_arg.i4SrcX		= i4_src_x;
	   rotate_arg.i4SrcY		= i4_src_y;
	   rotate_arg.u4SrcPitch	= p_src_surf->ui4_pitch;
	   rotate_arg.eSrcCm		= gl_cm_to_mtal_cm(p_src_surf->t_desc.e_clrmode);
	   
	   rotate_arg.pvDst 		= (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
	   rotate_arg.i4DstX		= i4_dst_x;
	   rotate_arg.i4DstY		= i4_dst_y;
	   rotate_arg.u4DstPitch	= p_dst_surf->ui4_pitch;
	   rotate_arg.eDstCm		= gl_cm_to_mtal_cm(p_dst_surf->t_desc.e_clrmode);

	   rotate_arg.u4Width		= ui4_width;
	   rotate_arg.u4Height		= ui4_height;

	   rotate_arg.eDirection	= MTGFX_C_CLOCKWISE;
	   
	   i4_ret = MTGFX_Rotate90(&rotate_arg);

	   if (i4_ret < 0)
	   {
		   gl_dfb_mul_objs_unlock(ap_obj, 2);
			return GLR_DRV_ERROR;
	   }
	   
	   MTGFX_Flush();
	   MTGFX_Wait(-1);
	   
	   gl_dfb_mul_objs_unlock(ap_obj, 2);
	   
	   return GLR_OK;
   
    }
#endif 
	else
	{
	    gl_sw_blt_surface_with_rotation( 
	        p_dst_surf,
	        p_src_surf,
	        i4_src_x,
	        i4_src_y,
	        i4_dst_x,
	        i4_dst_y,
	        ui4_width,
	        ui4_height,
	        ui1_alpha_assigned,
	        b_flip,
	        ui4_angle,
	        ui4_flag);
	}
#if 0

    DFBCHECK(p_dst_surf->p_dfb_surf->Flip(p_dst_surf->p_dfb_surf, NULL, 0), &i4_ret);
#endif
	GL_DFB_SURF_FLIP(p_dst_surf);

	gl_dfb_mul_objs_unlock(ap_obj, 2);
	return GLR_OK;

}

INT32 gl_dfb_surface_rop_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_ROP_TYPE_T       e_rop_type,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T * p_dst_surf;            
    GL_DFB_SURFACE_T * p_src_surf;            
    
    HANDLE_T           ah_hd[2];
    HANDLE_TYPE_T      ae_type[2];
    void * ap_obj[2];

    INT32 i4_ret;
    
    ae_type[0]  = GLT_OBJ_SURFACE;
    ae_type[1]  = GLT_OBJ_SURFACE;
    ah_hd[0]    = h_dst_surf;
    ah_hd[1]    = h_src_surf;

    i4_ret = gl_dfb_mul_handles_lock_objs(ae_type, ah_hd, 2, ap_obj);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }     
    
    
    p_dst_surf = ap_obj[0];
    p_src_surf = ap_obj[1];

#if 0
	{	       
	    DFBRectangle  	   dfb_rect;
	    dfb_rect.x = i4_src_x;
	    dfb_rect.y = i4_src_y;
	    dfb_rect.w = ui4_width;
	    dfb_rect.h = ui4_height;

	    i4_ret = _prepare_blt_args(p_dst_surf, GL_DFB_BLT_ARG_ROP, 0, 0, 0, e_rop_type);
	    if(i4_ret != GLR_OK)
	    {
	         
	        gl_dfb_mul_objs_unlock(ap_obj, 2);
	        return i4_ret;
	    }     
	    
	    DFBCHECK(p_dst_surf->p_dfb_surf->Blit(
	        p_dst_surf->p_dfb_surf, 
	        p_src_surf->p_dfb_surf,
	        &dfb_rect,
	        i4_dst_x,
	        i4_dst_y), &i4_ret);

	    DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);
	}
#else
	{
		MTGFX_ROP_BITBLT_T	blit_arg;
		blit_arg.pvSrc		   = (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.i4SrcX 	   = i4_src_x;
		blit_arg.i4SrcY 	   = i4_src_y;
		blit_arg.u4SrcPitch    = p_src_surf->ui4_pitch;
		blit_arg.eSrcCm 	   = gl_cm_to_mtal_cm(p_src_surf->t_desc.e_clrmode);
		
		blit_arg.pvDst		   = (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.i4DstX 	   = i4_dst_x;
		blit_arg.i4DstY 	   = i4_dst_y;
		blit_arg.u4DstPitch    = p_dst_surf->ui4_pitch;
		blit_arg.eDstCm 	   = gl_cm_to_mtal_cm(p_dst_surf->t_desc.e_clrmode);

		blit_arg.u4Width	   = ui4_width;
		blit_arg.u4Height	   = ui4_height;

		blit_arg.eRopType      = (MTGFX_ROP_TYPE_T)e_rop_type;
		
		i4_ret = MTGFX_RopBitblt(&blit_arg);
		MTGFX_Flush();
		MTGFX_Wait(-1);
	}
#endif
    if(i4_ret != GLR_OK)
    {
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }     
    
    gl_dfb_mul_objs_unlock(ap_obj, 2);

    GL_DFB_SURF_FLIP(p_dst_surf);

    return GLR_OK;
}

INT32 gl_dfb_surface_compose(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T * p_dst_surf;            
    GL_DFB_SURFACE_T * p_src_surf;            
    
    HANDLE_T           ah_hd[2];
    HANDLE_TYPE_T      ae_type[2];
    void *              ap_obj[2];
    GL_RECT_T                   t_rect, t_rect1;
    UINT16                      ui2_cnt;

    INT32 i4_ret;

    ae_type[0]  = GLT_OBJ_SURFACE;
    ae_type[1]  = GLT_OBJ_SURFACE;
    ah_hd[0]    = h_dst_surf;
    ah_hd[1]    = h_src_surf;

    i4_ret = gl_dfb_mul_handles_lock_objs(ae_type, ah_hd, 2, ap_obj);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }     
     
    p_dst_surf = ap_obj[0];
    p_src_surf = ap_obj[1];

    GL_DFB_SURF_TRACE_API(
		"[GL_DFB_SURF] Compose %d,%d,%d,%d,%d,%d \
 with clip[%d,%d,%d,%d] src clip :[%d,%d,%d,%d]from p_surf:%x to p_surf:%x Rule :%d ,preA:%d\n",
		i4_dst_x,
		i4_dst_y,
		i4_src_x,
		i4_src_y,
		ui4_width,
		ui4_height, 	
        p_dst_surf->t_clip_rect.i4_left,
        p_dst_surf->t_clip_rect.i4_top,
        p_dst_surf->t_clip_rect.i4_right,
        p_dst_surf->t_clip_rect.i4_bottom,                
        p_src_surf->t_clip_rect.i4_left,
        p_src_surf->t_clip_rect.i4_top,
        p_src_surf->t_clip_rect.i4_right,
        p_src_surf->t_clip_rect.i4_bottom,                
        p_src_surf,
        p_dst_surf,
        e_rule,
        ui1_pre_alpha
	);

    
   
    GL_MEMCPY(&t_rect, &(p_src_surf->t_clip_rect), sizeof(GL_RECT_T));
    
    if (p_src_surf->t_desc.h_clipper)
    {
        if (GLR_OK == x_gl_clipper_get_clip_list(p_src_surf->t_desc.h_clipper, &ui2_cnt, &t_rect1))
        {
            INTERSECT_RECT(&t_rect, &t_rect1, &t_rect);
        }
    }
#if 0
	{
		       
	    DFBRectangle  	   dfb_rect;
		DFBRegion                   tmp_clip_rg; 
		dfb_rect.x = i4_src_x;
		dfb_rect.y = i4_src_y;
		dfb_rect.w = ui4_width;
		dfb_rect.h = ui4_height;

    GL_RECT_TO_DFB_RECT(&t_rect, &tmp_clip_rg);

    DFBCHECK((p_src_surf)->p_dfb_surf->SetClip((p_src_surf)->p_dfb_surf, &tmp_clip_rg), &i4_ret);
    
    i4_ret =  _prepare_blt_args(p_dst_surf, GL_DFB_BLT_ARG_PD|GL_DFB_BLT_ARG_PREALPHA, e_rule, ui1_pre_alpha, 0, /*Avoid warn*/GL_ROP_NOT_SRC);
    if(i4_ret != GLR_OK)
    {
         
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }         
    
    DFBCHECK(p_dst_surf->p_dfb_surf->Blit(
        p_dst_surf->p_dfb_surf, 
        p_src_surf->p_dfb_surf,
        &dfb_rect,
        i4_dst_x,
        i4_dst_y), &i4_ret);

    DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);
	}
#else
	{
		MTGFX_ALPHA_COMPOSITION_T         blit_arg;
		blit_arg.u1Alpha       = ui1_pre_alpha;
		blit_arg.pvSrc         = (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.i4SrcX        = i4_src_x;
		blit_arg.i4SrcY        = i4_src_y;
		blit_arg.u4SrcPitch    = p_src_surf->ui4_pitch;
		blit_arg.eSrcCm        = gl_cm_to_mtal_cm(p_src_surf->t_desc.e_clrmode);
		
		blit_arg.pvDst         = (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.i4DstX        = i4_dst_x;
		blit_arg.i4DstY        = i4_dst_y;
		blit_arg.u4DstPitch    = p_dst_surf->ui4_pitch;
		blit_arg.eDstCm        = gl_cm_to_mtal_cm(p_dst_surf->t_desc.e_clrmode);

		blit_arg.u4Width       = ui4_width;
		blit_arg.u4Height      = ui4_height;
		blit_arg.ePDRule       = gl_compose_rule_to_mtal(e_rule);
		blit_arg.u1Alpha       = ui1_pre_alpha;
		blit_arg.bRectSrcOption = FALSE;
		
	    i4_ret = MTGFX_AlphaComposition(&blit_arg);
		MTGFX_Flush();
		MTGFX_Wait(-1);
	}
#endif
    if(i4_ret != GLR_OK)
    {
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }     
    
    GL_DFB_SURF_FLIP(p_dst_surf);
    gl_dfb_mul_objs_unlock(ap_obj, 2);
    return GLR_OK;
}


INT32 gl_dfb_surface_pat_compose(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
     UINT8              ui1_alpha_assigned,
    GL_COMPOSE_RULE_T   e_rule,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T *  p_dst_surf;           
    GL_DFB_SURFACE_T *  p_src_surf;            
   
    HANDLE_T            ah_hd[2];
    HANDLE_TYPE_T       ae_type[2];
    void * ap_obj[2];
    INT32   i4_ret;

    ae_type[0]  = GLT_OBJ_SURFACE;
    ae_type[1]  = GLT_OBJ_SURFACE;
    ah_hd[0]    = h_dst_surf;
    ah_hd[1]    = h_src_surf;
        
    i4_ret = gl_dfb_mul_handles_lock_objs(ae_type, ah_hd, 2, ap_obj);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }     
    
    p_dst_surf = ap_obj[0];
    p_src_surf = ap_obj[1];

    
    
    GL_DFB_SURF_TRACE_API(
		"[GL_DFB_SURF] Pat compose %d,%d,%d,%d, %d,%d,%d,%d\
        with clip[%d,%d,%d,%d] src clip :[%d,%d,%d,%d]\
        from p_surf:%x to p_surf:%x Rule :%d ,preA:%d\n",
		i4_dst_x,
		i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
		i4_src_x,
		i4_src_y,
		ui4_src_w,
		ui4_src_h, 	
        p_dst_surf->t_clip_rect.i4_left,
        p_dst_surf->t_clip_rect.i4_top,
        p_dst_surf->t_clip_rect.i4_right,
        p_dst_surf->t_clip_rect.i4_bottom,                
        p_src_surf->t_clip_rect.i4_left,
        p_src_surf->t_clip_rect.i4_top,
        p_src_surf->t_clip_rect.i4_right,
        p_src_surf->t_clip_rect.i4_bottom,                
        p_src_surf,
        p_dst_surf,
        e_rule,
        ui1_alpha_assigned);

#if 0
	{
		DFBRectangle		dfb_src_rect;
		DFBPoint			pt_point; 
	    int                 dfb_dst_x;
	    int                 dfb_dst_y;
	    int                 i4_rgn_count = 0;
	    
		dfb_src_rect.x = i4_src_x;
	    dfb_src_rect.y = i4_src_y;
	    dfb_src_rect.w = ui4_src_w;
	    dfb_src_rect.h = ui4_src_h;
		
		if (_pt_pat_compose_point == NULL)
		{
			_pt_pat_compose_point = (DFBPoint *)x_mem_alloc(_i4_pat_compose_num * sizeof(DFBPoint));
	
			if (_pt_pat_compose_point == NULL)
			{
				return GLR_NO_MEMORY;
			}
		}
	
		if (_pt_pat_compose_rect == NULL)
		{
			_pt_pat_compose_rect = (DFBRectangle *)x_mem_alloc(_i4_pat_compose_num * sizeof(DFBRectangle));
	
			if (_pt_pat_compose_rect == NULL)
			{
				return GLR_NO_MEMORY;
			}
		}

	    i4_ret = _prepare_blt_args(p_dst_surf, GL_DFB_BLT_ARG_PD|GL_DFB_BLT_ARG_PREALPHA, e_rule, ui1_alpha_assigned, 0, /*Avoid warn*/GL_ROP_NOT_SRC);
	    if(i4_ret != GLR_OK)
	    {
	         
	        gl_dfb_mul_objs_unlock(ap_obj, 2);
	        return i4_ret;
	    }
	    
	    _pat_clip_realign(p_dst_surf, i4_dst_x, i4_dst_y, ui4_dst_w, ui4_dst_h);

	    i4_rgn_count = 0;
	     
	    for(dfb_dst_x = i4_dst_x; (UINT32)(dfb_dst_x - i4_dst_x) < ui4_dst_w; dfb_dst_x += (INT32)ui4_src_w)
	    {
	        pt_point.x = dfb_dst_x;
	        dfb_src_rect.w = ((i4_dst_x + (INT32)ui4_dst_w - dfb_dst_x - dfb_src_rect.w) >= 0 ) ? 
	                                                        (INT32)ui4_src_w : (i4_dst_x + (INT32)ui4_dst_w - dfb_dst_x);
	        
	        for(dfb_dst_y = i4_dst_y; (UINT32)(dfb_dst_y - i4_dst_y) < ui4_dst_h; dfb_dst_y += (INT32)ui4_src_h)
	        {
	            pt_point.y = dfb_dst_y;
	            dfb_src_rect.h = ((i4_dst_y + (INT32)ui4_dst_h - dfb_dst_y - dfb_src_rect.h) >= 0 )? 
	                                                        (INT32)ui4_src_h : (i4_dst_y + (INT32)ui4_dst_h - dfb_dst_y);
	            
	            _pt_pat_compose_rect [i4_rgn_count] = dfb_src_rect;
	            _pt_pat_compose_point[i4_rgn_count] = pt_point;

	            i4_rgn_count++;

	            if (i4_rgn_count >= _i4_pat_compose_num)
	            {
	                _i4_pat_compose_num += 10;

	                _pt_pat_compose_point = (DFBPoint *)x_mem_realloc(_pt_pat_compose_point, _i4_pat_compose_num * sizeof(DFBPoint));

	                if (_pt_pat_compose_point == NULL)
	                {
	                    gl_dfb_mul_objs_unlock(ap_obj, 2);
	                    return GLR_NO_MEMORY;
	                }

	                _pt_pat_compose_rect = (DFBRectangle *)x_mem_realloc(_pt_pat_compose_rect, _i4_pat_compose_num * sizeof(DFBRectangle));

	                if (_pt_pat_compose_rect == NULL)
	                {
	                    gl_dfb_mul_objs_unlock(ap_obj, 2);
	                    return GLR_NO_MEMORY;
	                }
	            }
	        }     
	    }

	    DFBCHECK(p_dst_surf->p_dfb_surf->BatchBlit(
	                p_dst_surf->p_dfb_surf, 
	                p_src_surf->p_dfb_surf,
	                _pt_pat_compose_rect,
	                _pt_pat_compose_point,
	                i4_rgn_count), &i4_ret);
	                       
	    if(i4_ret != GLR_OK)
	    {
	        DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);     
	        gl_dfb_mul_objs_unlock(ap_obj, 2);
	        return i4_ret;
	    }
	     
	     
	    DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);
	}
#else
	{
		INT32                   x,y;
		MTGFX_ALPHA_COMPOSITION_T         blit_arg;
		blit_arg.u1Alpha       = ui1_alpha_assigned;
		blit_arg.pvSrc         = (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.i4SrcX        = i4_src_x;
		blit_arg.i4SrcY        = i4_src_y;
		blit_arg.u4SrcPitch    = p_src_surf->ui4_pitch;
		blit_arg.eSrcCm        = gl_cm_to_mtal_cm(p_src_surf->t_desc.e_clrmode);
		
		blit_arg.pvDst         = (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blit_arg.u4DstPitch    = p_dst_surf->ui4_pitch;
		blit_arg.eDstCm        = gl_cm_to_mtal_cm(p_dst_surf->t_desc.e_clrmode);

		blit_arg.u4Width       = ui4_src_w;
		blit_arg.u4Height      = ui4_src_h;
		blit_arg.ePDRule       = gl_compose_rule_to_mtal(e_rule);
		blit_arg.bRectSrcOption = FALSE;
		
	    for(x = i4_dst_x; (UINT32)(x - i4_dst_x) < ui4_dst_w; x += (INT32)ui4_src_w)
	    {
			blit_arg.i4DstX        = x;
	        blit_arg.u4Width       = ((i4_dst_x + (INT32)ui4_dst_w - x - ui4_src_w) >= 0 ) ? 
	                                                        (INT32)ui4_src_w : (i4_dst_x + (INT32)ui4_dst_w - x);
	        for(y = i4_dst_y; (UINT32)(y - i4_dst_y) < ui4_dst_h; y += (INT32)ui4_src_h)
	        {
				blit_arg.i4DstY        = y;

				blit_arg.u4Height      = ((i4_dst_y + (INT32)ui4_dst_h - y - ui4_src_h) >= 0 )? 
	                                                        (INT32)ui4_src_h : (i4_dst_y + (INT32)ui4_dst_h - y);
	            i4_ret = MTGFX_AlphaComposition(&blit_arg);

				if (i4_ret < 0)
				{
					gl_dfb_mul_objs_unlock(ap_obj, 2);
					return GLR_DRV_ERROR;
				}
	        }     
	    }
		MTGFX_Flush();
		MTGFX_Wait(-1);
	}
#endif
    GL_DFB_SURF_FLIP(p_dst_surf);
    
    gl_dfb_mul_objs_unlock(ap_obj, 2);
    return GLR_OK;
}

#ifdef MW_PHOTO_SHOW_ON_VDP
INT32 gl_dfb_surface_vdp_strech_blend(
    GL_HSURFACE_T       h_surf_dst, 
    GL_HSURFACE_T       h_surf_src_pic, 
    GL_HSURFACE_T       h_surf_src_black,
    UINT8               ui1_alpha)
{
	mt53fb_vdp_stretch_blit blit_args;
    GL_DFB_SURFACE_T * p_dst_surf;            
    GL_DFB_SURFACE_T * p_src_surf_pic;        
    GL_DFB_SURFACE_T * p_src_surf_black;            
    
    HANDLE_T           ah_hd[3];
    HANDLE_TYPE_T      ae_type[3];
    void * ap_obj[3];

    INT32 i4_ret = GLR_OK;

    ae_type[0]  = GLT_OBJ_SURFACE;
    ae_type[1]  = GLT_OBJ_SURFACE;
    ae_type[2]  = GLT_OBJ_SURFACE;
    ah_hd[0]    = h_surf_dst;
    ah_hd[1]    = h_surf_src_pic;
    ah_hd[2]    = h_surf_src_black;

    gl_dfb_mul_handles_lock_objs(ae_type, ah_hd, 3, ap_obj);
    
    p_dst_surf = ap_obj[0];
    p_src_surf_pic = ap_obj[1];	
    p_src_surf_black = ap_obj[2];

	blit_args.composition_level = (INT32)(ui1_alpha >> 1);
	blit_args.dest_buffer_Y_addr = (INT32)p_dst_surf->pv_buffer;
	blit_args.dest_buffer_C_addr = (INT32)p_dst_surf->pv_buffer + p_dst_surf->ui4_pitch * p_dst_surf->t_desc.ui4_height;
	blit_args.dest_buffer_Y_width = p_dst_surf->t_desc.ui4_width;
	blit_args.dest_buffer_Y_height = p_dst_surf->t_desc.ui4_height;
	blit_args.dest_buffer_Y_pitch = p_dst_surf->ui4_pitch;

	blit_args.src1_buffer_Y_addr = (INT32)p_src_surf_pic->pv_buffer;
	blit_args.src1_buffer_C_addr = (INT32)p_src_surf_pic->pv_buffer + p_src_surf_pic->ui4_pitch * p_src_surf_pic->t_desc.ui4_height;
	blit_args.src1_buffer_Y_width = p_src_surf_pic->t_desc.ui4_width;
	blit_args.src1_buffer_Y_height = p_src_surf_pic->t_desc.ui4_height;
	blit_args.src1_buffer_Y_pitch = p_src_surf_pic->ui4_pitch;

	blit_args.src2_buffer_Y_addr = (INT32)p_src_surf_black->pv_buffer;
	blit_args.src2_buffer_C_addr = (INT32)p_src_surf_black->pv_buffer + p_src_surf_black->ui4_pitch * p_src_surf_black->t_desc.ui4_height;

	if (vdpStretchBlit(&blit_args) != 0)
	{
		i4_ret = GLR_DRV_ERROR;
	}
	
	DFBCHECK((p_dst_surf)->p_dfb_surf->Flip((p_dst_surf)->p_dfb_surf, NULL, 0), &i4_ret);
	
    gl_dfb_mul_objs_unlock(ap_obj, 3);
	
    return i4_ret;
}
#endif

INT32 gl_dfb_surface_blend(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T * p_dst_surf;            
    GL_DFB_SURFACE_T * p_src_surf;            
    
    HANDLE_T           ah_hd[2];
    HANDLE_TYPE_T      ae_type[2];
    void * ap_obj[2];

    INT32 i4_ret;

    ae_type[0]  = GLT_OBJ_SURFACE;
    ae_type[1]  = GLT_OBJ_SURFACE;
    ah_hd[0]    = h_dst_surf;
    ah_hd[1]    = h_src_surf;

    gl_dfb_mul_handles_lock_objs(ae_type, ah_hd, 2, ap_obj);
    
    p_dst_surf = ap_obj[0];
    p_src_surf = ap_obj[1];

#if 0
	{
		DFBRectangle  	   dfb_rect;

    dfb_rect.x = i4_src_x;
    dfb_rect.y = i4_src_y;
    dfb_rect.w = ui4_width;
    dfb_rect.h = ui4_height;

     

    i4_ret = _prepare_blt_args(p_dst_surf, GL_DFB_BLT_ARG_PREALPHA, 0, ui1_alpha, 0, /*Avoid warn*/GL_ROP_NOT_SRC);
    if(i4_ret != GLR_OK)
    {
         
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }     
    
    DFBCHECK(p_dst_surf->p_dfb_surf->Blit(
        p_dst_surf->p_dfb_surf, 
        p_src_surf->p_dfb_surf,
        &dfb_rect,
        i4_dst_x,
        i4_dst_y), &i4_ret);

    DFBCHECK(p_dst_surf->p_dfb_surf->ReleaseSource(p_dst_surf->p_dfb_surf),&i4_ret);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_mul_objs_unlock(ap_obj, 2);
        return i4_ret;
    }     
	}
#else
	{
		MTGFX_ALPHA_BLEND_T     blending_arg;
		blending_arg.pvSrc         = (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blending_arg.i4SrcX        = i4_src_x;
		blending_arg.i4SrcY        = i4_src_y;
		blending_arg.u4SrcPitch    = p_src_surf->ui4_pitch;
		blending_arg.eSrcCm        = gl_cm_to_mtal_cm(p_src_surf->t_desc.e_clrmode);
		
		blending_arg.pvDst         = (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
		blending_arg.i4DstX        = i4_dst_x;
		blending_arg.i4DstY        = i4_dst_y;
		blending_arg.u4DstPitch    = p_dst_surf->ui4_pitch;
		blending_arg.eDstCm        = gl_cm_to_mtal_cm(p_dst_surf->t_desc.e_clrmode);

		blending_arg.u4Width       = ui4_width;
		blending_arg.u4Height      = ui4_height;

		blending_arg.u1Alpha       = ui1_alpha;
		
		i4_ret = MTGFX_AlphaBlending(&blending_arg);

		if (i4_ret < 0)
		{
			gl_dfb_mul_objs_unlock(ap_obj, 2);
			return GLR_DRV_ERROR;
		}

		MTGFX_Flush();
		MTGFX_Wait(-1);
	}
#endif  
     
    
    GL_DFB_SURF_FLIP(p_dst_surf);

    gl_dfb_mul_objs_unlock(ap_obj, 2);
    return GLR_OK;
}

static INT32 
_prepare_sw_draw_args(GL_DFB_SURFACE_T * p_surf, VOID ** ppv_buff, UINT32 * pui4_pitch)
{
#if 0
    INT32 i4_ret;
    DFBCHECK(p_surf->p_dfb_surf->Lock(p_surf->p_dfb_surf, DSLF_READ|DSLF_WRITE, ppv_buff, (int*)pui4_pitch), &i4_ret);
    if(i4_ret != GLR_OK)
    {    
        return i4_ret;
    }     
#else
	* ppv_buff = (VOID*)p_surf->p_dfb_surf->bmp_buffer.u4PhyAddr;
	* pui4_pitch = p_surf->ui4_pitch;
#endif
//    gl_draw_set_dfb_surf(p_surf->p_dfb_surf);
    return GLR_OK;
}

#if 0
#define _end_sw_draw(p_surf, pi4_ret)\
do{\
    DFBCHECK(p_surf->p_dfb_surf->Unlock(p_surf->p_dfb_surf), pi4_ret);\
}while(0)
#else
#define _end_sw_draw(p_surf, pi4_ret)
#endif

INT32 gl_dfb_surface_draw_ellipse(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
    GL_DFB_SURFACE_T * p_surf;
    VOID * pv_buff;
    UINT32 ui4_pitch;
    UINT32 ui4_color;

    INT32 i4_ret;
    if ((0 == ui2_line_width) && (GL_STYLE_OUTLINED == t_style))
    {
        return GLR_OK;
    }
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }     
     
    
    ui4_color = gl_dfb_compose_packed_color(pt_color, p_surf->t_desc.e_clrmode);
    
    i4_ret = _prepare_sw_draw_args(p_surf, &pv_buff, &ui4_pitch);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);

        return i4_ret;
    }
    
    if(t_style != GL_STYLE_FILLED)
    {
        gl_draw_ellipse(
            p_surf->t_desc.e_clrmode,
            pv_buff, 
            ui4_pitch,
            pt_rect,
            p_surf->t_clip_rect.i4_left, 
            p_surf->t_clip_rect.i4_right - 1, 
            p_surf->t_clip_rect.i4_top, 
            p_surf->t_clip_rect.i4_bottom - 1,             
            ui2_line_width,
            ui4_color);
    }
    else
    {
         gl_fill_ellipse(
            p_surf->t_desc.e_clrmode,
            pv_buff, 
            ui4_pitch,
            pt_rect,
            p_surf->t_clip_rect.i4_left, 
            p_surf->t_clip_rect.i4_right - 1, 
            p_surf->t_clip_rect.i4_top, 
            p_surf->t_clip_rect.i4_bottom - 1,             
            ui4_color);
    }
    _end_sw_draw(p_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    }

    GL_DFB_SURF_FLIP(p_surf);

    gl_dfb_obj_unlock(p_surf);

    return GLR_OK;
       
}

INT32 gl_dfb_surface_draw_polygon(
    GL_HSURFACE_T       h_surf,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[],
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
	GL_DFB_SURFACE_T * p_surf;
    VOID * pv_buff;
    UINT32 ui4_pitch;
    UINT32 ui4_color;

    INT32 i4_ret;
	if ((0 == ui2_line_width) && (GL_STYLE_OUTLINED == t_style))
    {
        return GLR_OK;
    }
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
    
    ui4_color =gl_dfb_compose_packed_color(pt_color, p_surf->t_desc.e_clrmode);

    i4_ret = _prepare_sw_draw_args(p_surf, &pv_buff, &ui4_pitch);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    }
    if(t_style != GL_STYLE_FILLED)
    {
        gl_draw_polygon(
            p_surf->t_desc.e_clrmode,
            pv_buff, 
            ui4_pitch,
            ui4_count,
            at_vertex,
            p_surf->t_clip_rect.i4_left, 
            p_surf->t_clip_rect.i4_right - 1, 
            p_surf->t_clip_rect.i4_top, 
            p_surf->t_clip_rect.i4_bottom - 1,             
            ui2_line_width,
            ui4_color, 
            TRUE);
    }
    else
    {
         gl_fill_polygon(
            p_surf->t_desc.e_clrmode,
            pv_buff, 
            ui4_pitch,
            ui4_count,
            at_vertex,
            p_surf->t_clip_rect.i4_left, 
            p_surf->t_clip_rect.i4_right - 1, 
            p_surf->t_clip_rect.i4_top, 
            p_surf->t_clip_rect.i4_bottom - 1,             
            ui4_color);
    }
    _end_sw_draw(p_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    }
    GL_DFB_SURF_FLIP(p_surf);

    gl_dfb_obj_unlock(p_surf);

    return GLR_OK;
}	


INT32 gl_dfb_surface_draw_polyline(
    GL_HSURFACE_T       h_surf,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[],
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    UINT32              ui4_flag)
{
	GL_DFB_SURFACE_T * p_surf;
    VOID * pv_buff;
    UINT32 ui4_pitch;
    UINT32 ui4_color;

    INT32 i4_ret;
    if ((0 == ui2_line_width))
    {
        return GLR_OK;
    }
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
    
    ui4_color =gl_dfb_compose_packed_color(pt_color, p_surf->t_desc.e_clrmode);

    i4_ret = _prepare_sw_draw_args(p_surf, &pv_buff, &ui4_pitch);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    }

    gl_draw_polygon(
        p_surf->t_desc.e_clrmode,
        pv_buff, 
        ui4_pitch,
        ui4_count,
        at_vertex,
        p_surf->t_clip_rect.i4_left, 
        p_surf->t_clip_rect.i4_right - 1, 
        p_surf->t_clip_rect.i4_top, 
        p_surf->t_clip_rect.i4_bottom - 1,             
        ui2_line_width,
        ui4_color, 
        FALSE);

    _end_sw_draw(p_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    }
    GL_DFB_SURF_FLIP(p_surf);

    gl_dfb_obj_unlock(p_surf);

    return GLR_OK;
}

INT32 gl_dfb_surface_draw_arc(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
	GL_DFB_SURFACE_T * p_surf;
    VOID * pv_buff;
    UINT32 ui4_pitch;
    UINT32 ui4_color;

    INT32 i4_ret;
    if ((0 == ui2_line_width) && (GL_STYLE_OUTLINED == t_style))
    {
        return GLR_OK;
    }
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
    ui4_color = gl_dfb_compose_packed_color(pt_color, p_surf->t_desc.e_clrmode);

    
    i4_ret = _prepare_sw_draw_args(p_surf, &pv_buff, &ui4_pitch);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    }
    
    if(t_style != GL_STYLE_FILLED)
    {
        gl_draw_arc(
            p_surf->t_desc.e_clrmode,
            pv_buff, 
            ui4_pitch,
            pt_rect,
            i4_deg_start,
            i4_deg_end,
            p_surf->t_clip_rect.i4_left, 
            p_surf->t_clip_rect.i4_right - 1, 
            p_surf->t_clip_rect.i4_top, 
            p_surf->t_clip_rect.i4_bottom - 1,             
            ui2_line_width,
            ui4_color,
            TRUE);
    }
    else
    {
         gl_fill_arc(
            p_surf->t_desc.e_clrmode,
            pv_buff, 
            ui4_pitch,
            pt_rect,
            i4_deg_start,
            i4_deg_end,
            p_surf->t_clip_rect.i4_left, 
            p_surf->t_clip_rect.i4_right - 1, 
            p_surf->t_clip_rect.i4_top, 
            p_surf->t_clip_rect.i4_bottom - 1,             
            ui4_color,
            TRUE);
    }
    _end_sw_draw(p_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    }
    GL_DFB_SURF_FLIP(p_surf);

    gl_dfb_obj_unlock(p_surf);
    return GLR_OK;
}	

INT32 gl_dfb_surface_draw_pie(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
	GL_DFB_SURFACE_T * p_surf;
    VOID * pv_buff;
    UINT32 ui4_pitch;
    UINT32 ui4_color;

    INT32 i4_ret;
    if ((0 == ui2_line_width) && (GL_STYLE_OUTLINED == t_style))
    {
        return GLR_OK;
    }
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
    
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
    
    ui4_color = gl_dfb_compose_packed_color(pt_color, p_surf->t_desc.e_clrmode);
    
    i4_ret = _prepare_sw_draw_args(p_surf, &pv_buff, &ui4_pitch);
    
    if(t_style != GL_STYLE_FILLED)
    {
        gl_draw_arc(
            p_surf->t_desc.e_clrmode,
            pv_buff, 
            ui4_pitch,
            pt_rect,
            i4_deg_start,
            i4_deg_end,
            p_surf->t_clip_rect.i4_left, 
            p_surf->t_clip_rect.i4_right - 1, 
            p_surf->t_clip_rect.i4_top, 
            p_surf->t_clip_rect.i4_bottom - 1,             
            ui2_line_width,
            ui4_color,
            TRUE);
    }
    else
    {
         gl_fill_arc(
            p_surf->t_desc.e_clrmode,
            pv_buff, 
            ui4_pitch,
            pt_rect,
            i4_deg_start,
            i4_deg_end,
            p_surf->t_clip_rect.i4_left, 
            p_surf->t_clip_rect.i4_right - 1, 
            p_surf->t_clip_rect.i4_top, 
            p_surf->t_clip_rect.i4_bottom - 1,             
            ui4_color,
            TRUE);
    }
    _end_sw_draw(p_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    }   
    GL_DFB_SURF_FLIP(p_surf);
    
    gl_dfb_obj_unlock(p_surf);
    return GLR_OK;
}

INT32 gl_dfb_surface_draw_round_rect(
    GL_HSURFACE_T           h_surf,
    const GL_RECT_T*        pt_rect,
    const GL_ARC_DESC_T*    pt_arc,
    const GL_COLOR_T*       pt_color,
    UINT16                  ui2_line_width,
    GL_STYLE_T              t_style,
    UINT32                  ui4_flag)
{
    GL_DFB_SURFACE_T * p_surf;
    VOID * pv_buff;
    UINT32 ui4_pitch;
    UINT32 ui4_color;
    GL_RECT_T       t_work_rect;
    GL_RECT_T       at_corner[4];

    INT32 i4_ret;
    if ((0 == ui2_line_width) && (GL_STYLE_OUTLINED == t_style))
    {
        return GLR_OK;
    }
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
    
    ui4_color = gl_dfb_compose_packed_color(pt_color, p_surf->t_desc.e_clrmode);
    
    
    if (ui2_line_width > 1 && t_style == GL_STYLE_OUTLINED)
    {
        t_work_rect.i4_left   = pt_rect->i4_left   - (ui2_line_width - 1);
        t_work_rect.i4_top    = pt_rect->i4_top    - (ui2_line_width - 1);
        t_work_rect.i4_right  = pt_rect->i4_right  + (ui2_line_width - 1);
        t_work_rect.i4_bottom = pt_rect->i4_bottom + (ui2_line_width - 1);
    }
    else
    {
        t_work_rect = *pt_rect;
    }

    gl_set_corner_arc(&t_work_rect, pt_arc, ui2_line_width, at_corner);

    i4_ret = _prepare_sw_draw_args(p_surf, &pv_buff, &ui4_pitch);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    }
    if(t_style != GL_STYLE_FILLED)
    {
       
        gl_draw_round_rect(                    
            p_surf->t_desc.e_clrmode,
            pv_buff, 
            ui4_pitch,
            at_corner,          
            p_surf->t_clip_rect.i4_left, 
            p_surf->t_clip_rect.i4_right - 1, 
            p_surf->t_clip_rect.i4_top, 
            p_surf->t_clip_rect.i4_bottom - 1,
            ui2_line_width,            
            ui4_color);
    }
    else
    {
        gl_fill_round_rect(                    
            p_surf->t_desc.e_clrmode,
            pv_buff, 
            ui4_pitch,
            at_corner,          
            p_surf->t_clip_rect.i4_left, 
            p_surf->t_clip_rect.i4_right - 1, 
            p_surf->t_clip_rect.i4_top, 
            p_surf->t_clip_rect.i4_bottom - 1,
            ui4_color);
    }
    _end_sw_draw(p_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_surf);
        return i4_ret;
    }   
    GL_DFB_SURF_FLIP(p_surf);

    gl_dfb_obj_unlock(p_surf);

    return GLR_OK;

}

INT32 gl_dfb_surface_reconfig(
    GL_HSURFACE_T       h_surf,
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_COLORMODE_T      e_mode)
{
#if 0
    GL_DFB_SURFACE_T * p_surf;
    INT32 i4_ret;
    DFBSurfaceDescription   dfb_desc;
    p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }

    if (p_surf->t_desc.ui4_height == ui4_height && 
        p_surf->t_desc.ui4_width  == ui4_width &&
        p_surf->t_desc.e_clrmode  == e_mode)
    {
        gl_dfb_obj_unlock(p_surf);
        return GLR_OK;
    }
    
    
    DFBCHECK(p_surf->p_dfb_surf->ReleaseSource(p_surf->p_dfb_surf), &i4_ret);
    DFBCHECK(p_surf->p_dfb_surf->Release(p_surf->p_dfb_surf), &i4_ret);

    
	if(ui4_height == 0)
	{
		/*For MMP */
		ui4_height = 1;
	}		
    
    p_surf->t_desc.ui4_height = ui4_height;
    p_surf->t_desc.ui4_width  = ui4_width;
    p_surf->t_desc.e_clrmode  = e_mode;
    
    dfb_desc.width          = p_surf->t_desc.ui4_width;
    dfb_desc.height         = p_surf->t_desc.ui4_height;
    dfb_desc.pixelformat    = gl_dfb_cm_map_get(p_surf->t_desc.e_clrmode);
    
    if ( p_surf->e_strg == GL_DFB_SURF_STRG_OFF_SCR )
    {
		GL_DFB_TRACE("[GL_DFB_SURF]reconfig p_surf:%x to off-screen\n", p_surf);
        dfb_desc.flags = DSDESC_CAPS|DSDESC_WIDTH|DSDESC_HEIGHT|DSDESC_WIDTH|DSDESC_PIXELFORMAT;    
        dfb_desc.caps  = /*DSCAPS_NONE*//*DSCAPS_SYSTEMONLY*/DSCAPS_VIDEOONLY/*DSCAPS_PREMULTIPLIED*/;      
    }
    else
    {
        GL_DFB_TRACE("[GL_DFB_SURF]reconfig p_surf:%x to off-screen\n", p_surf);
        dfb_desc.flags = DSDESC_CAPS|DSDESC_WIDTH|DSDESC_HEIGHT|DSDESC_WIDTH|DSDESC_PIXELFORMAT;    
        dfb_desc.caps  = DSCAPS_VIDEOONLY|DSCAPS_FROM_FBM;
    }
    /*Create new surface*/
    DFBCHECK(g_p_dfb->CreateSurface(g_p_dfb, &dfb_desc, &(p_surf->p_dfb_surf)), &i4_ret);

    p_surf->t_clip_rect.i4_left = 0;
    p_surf->t_clip_rect.i4_top = 0;
    p_surf->t_clip_rect.i4_right = ui4_width;
    p_surf->t_clip_rect.i4_bottom = ui4_height;
    
    gl_dfb_obj_unlock(p_surf);
    
    return i4_ret;
  #else
  	return GLR_OK;
  #endif
}

#ifndef MW_GL_NO_COMPRESSED_IMG

static BOOL  list_init = FALSE;
static HANDLE_T h_img_cache_lock = NULL_HANDLE;

#ifdef MW_ANIMATION_IMG_CACHE

typedef struct _GL_NHV_CATCH_T
{
    DLIST_ENTRY_T(_GL_NHV_CATCH_T) t_img_nhv_cache_entry;
    VOID*                          pv_data;
    HANDLE_T                       h_surf;
    UINT32                         ui4_surface_size;
    UINT32                         ui4_cache_life;
}
GL_NHV_CATCH_T;

DLIST_T(_GL_NHV_CATCH_T) t_img_nhv_cache_list;

#ifndef MW_IMG_CACHE_BUFFER_SIZE
#define MW_IMG_CACHE_BUFFER_SIZE   2*1024*1024
#endif

static UINT32 _ui4_img_cache_size = MW_IMG_CACHE_BUFFER_SIZE;

#endif

static GL_HSURFACE_T    _h_decompression_surf = NULL_HANDLE;
static GL_LOCK_INFO_T   _t_lock_info;
static GL_DFB_SURFACE_T *_pt_decompression_surf = NULL;



INT32 gl_surface_sw_nhv_ptn_paint_blt(
    GL_HSURFACE_T       h_surf,
    GL_COLORMODE_T      e_colormode,
    GL_COLOR_T          *pt_colorkey,
    UINT8               *pui1_compressed_img,
    GL_RECT_T           *pt_work_rect)      
{
    INT32               i4_ret;
    GL_DFB_SURFACE_T    *pt_surf;
    GL_LOCK_INFO_T      t_lock_info;
    GL_RECT_T           t_clip;
    register UINT8      *pui1_line_start;
    SIZE_T              z_img_pitch, z_compressed_length;
    HANDLE_T            h_zip_state = NULL_HANDLE;
    INT32               i4_x_pos, i4_y_pos;
    UINT8               *pui1_compressed_img_start;
    register UINT32     ui4_colorkey;
    UINT32              ui4_byte_per_pixel;
    register UINT8      *pui1_line_buffer;
    register INT32      i4_dst_x;
    HANDLE_T            h_zip_engine;

    z_img_pitch               = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pui1_compressed_img);
    z_compressed_length       = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pui1_compressed_img + sizeof(SIZE_T));
    pui1_compressed_img_start = pui1_compressed_img + sizeof(INT32) * 2;

     /* get surface */
    pt_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, & i4_ret);
    if (GL_FAILED(i4_ret))
    {
        GL_DFB_TRACE("{GRAPHIC} ERR: Invalid surface handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__);
        return i4_ret;
    }

    gl_dfb_obj_unlock(pt_surf);
    
    i4_ret = x_gl_surface_lock(h_surf, NULL, 0, &t_lock_info);

    if (i4_ret == GLR_OK)
    {
        /* get clip region */
        gl_surface_get_clip_area(pt_surf, &t_clip);
        INTERSECT_RECT(&t_clip, &t_clip, pt_work_rect);

        do
        {
            if (RECT_EMPTY(&t_clip))
            {
                x_gl_surface_unlock(h_surf,NULL);
                return GLR_OK;
            }

            i4_dst_x = pt_work_rect->i4_left;
            
            /* only two types are supported. 8-bit (colorkey) and 16-bit (1555 and 4444) */
            switch (e_colormode)
            {
                case GL_COLORMODE_ARGB_D4444:
                    ui4_colorkey  = (UINT32)GL_COLOR_ARGB4444(pt_colorkey->a, pt_colorkey->u1.r, pt_colorkey->u2.g, pt_colorkey->u3.b);
                    ui4_byte_per_pixel = 2;
                    break;
                case GL_COLORMODE_ARGB_D1555:
                    ui4_colorkey  = (UINT32)GL_COLOR_ARGB1555(pt_colorkey->a, pt_colorkey->u1.r, pt_colorkey->u2.g, pt_colorkey->u3.b);
                    ui4_byte_per_pixel = 2;
                    break;
                case GL_COLORMODE_ARGB_CLUT8:
                    ui4_colorkey  = (UINT32)pt_colorkey->u3.index;
                    ui4_byte_per_pixel = 1;
                    break;
                case GL_COLORMODE_ARGB_D8888:
                    ui4_colorkey = (UINT32)GL_COLOR_ARGB8888(pt_colorkey->a, pt_colorkey->u1.r, pt_colorkey->u2.g, pt_colorkey->u3.b);
                    ui4_byte_per_pixel = 4;
                    break;
                default:
                    GL_DFB_TRACE("{GRAPHIC} unsupoorted compressed image format. Line: %d\n", __LINE__);
                    x_gl_surface_unlock(h_surf,NULL);
                    return GLR_UNSUPPORTED;
            }
            

            /* allocate a line buffer for decompression */
            if (pt_surf->pui1_line_buffer == NULL)
            {
                pt_surf->pui1_line_buffer = (UINT8 *)x_mem_alloc(pt_surf->t_desc.ui4_width * ui4_byte_per_pixel * sizeof(UINT8));
                /* init the unzip engine */
                if (x_cl_open_eng(SN_CL_ZIP, NULL, NULL, &pt_surf->h_zip_engine) != CLR_OK)
                {
                    GL_DFB_TRACE("{GRAPHIC} unzip engine initialization failed. Line: %d\n", __LINE__);
                    x_gl_surface_unlock(h_surf,NULL);
                    return GLR_FE_ERROR;
                }
            }
            if (pt_surf->pui1_line_buffer == NULL)
            {
                GL_DFB_TRACE("{GRAPHIC} x_mem_alloc failed. Line: %d\n", __LINE__);
                x_gl_surface_unlock(h_surf,NULL);
                return GLR_NO_MEMORY;
            }

            pui1_line_buffer = pt_surf->pui1_line_buffer;
            h_zip_engine     = pt_surf->h_zip_engine;

            /* surface start position */
            pui1_line_start = (UINT8 *)t_lock_info.at_subplane[0].pv_bits + pt_work_rect->i4_top * t_lock_info.at_subplane[0].ui4_pitch;
            
            /* decompress the unused image data (which is outside the clipping region) */
            for (i4_y_pos = pt_work_rect->i4_top; i4_y_pos < t_clip.i4_top; i4_y_pos++)
            {
                z_img_pitch = (SIZE_T)MIN(GET_UINT32_FROM_PTR_LITTLE_END(pui1_compressed_img), t_lock_info.at_subplane[0].ui4_pitch);
                i4_ret = x_cl_decompress(
                    h_zip_engine, 
                    pui1_compressed_img_start, 
                    z_compressed_length, 
                    0,
                    CL_NULL_PARAM_TYPE,
                    NULL,
                    pui1_line_buffer, 
                    &z_img_pitch,
                    &h_zip_state);

                if (i4_ret != CLR_OK)
                {
                    GL_DFB_TRACE("{GRAPHIC} x_cl_decompress failed. (%d) Line: %d\n", i4_ret, __LINE__);
                    x_gl_surface_unlock(h_surf,NULL);
                    return GLR_FE_ERROR;
                }

                pui1_line_start = pui1_line_start + t_lock_info.at_subplane[0].ui4_pitch;
            }

            /* fill the decompressed image into the surface buffer */
            for (i4_y_pos = t_clip.i4_top; i4_y_pos < t_clip.i4_bottom; i4_y_pos++)
            {
                z_img_pitch = (SIZE_T)MIN(GET_UINT32_FROM_PTR_LITTLE_END(pui1_compressed_img), t_lock_info.at_subplane[0].ui4_pitch);
                x_cl_decompress(
                    h_zip_engine,
                    pui1_compressed_img_start,
                    z_compressed_length,
                    0,
                    CL_NULL_PARAM_TYPE,
                    NULL,
                    pui1_line_buffer,
                    &z_img_pitch,
                    &h_zip_state);
            
            
                if (e_colormode == GL_COLORMODE_ARGB_D4444 || e_colormode == GL_COLORMODE_ARGB_D1555)
                {
                    for (i4_x_pos = t_clip.i4_left; i4_x_pos < t_clip.i4_right; i4_x_pos++)
                    {
                        UINT16 ui2_src_pixel = (UINT16)GET_UINT16_FROM_PTR_LITTLE_END(pui1_line_buffer + (i4_x_pos - i4_dst_x) * sizeof(UINT16));
                        if (ui2_src_pixel != (UINT16)ui4_colorkey)
                        {
                            ((UINT16 *)pui1_line_start)[i4_x_pos] = ui2_src_pixel;
                        }
                    }
                }
                else if (e_colormode == GL_COLORMODE_ARGB_D8888)
                {
                    for (i4_x_pos = t_clip.i4_left; i4_x_pos < t_clip.i4_right; i4_x_pos++)
                    {
                        UINT32 ui4_src_pixel = (UINT32)GET_UINT32_FROM_PTR_LITTLE_END(pui1_line_buffer + (i4_x_pos - i4_dst_x) * sizeof(UINT32));
                        if (ui4_src_pixel != (UINT32)ui4_colorkey)
                        {
                            ((UINT32 *)pui1_line_start)[i4_x_pos] = ui4_src_pixel;
                        }
                    }
                }
                else /* if (e_colormode == GL_COLORMODE_ARGB_CLUT8) */
                {
                    for (i4_x_pos = t_clip.i4_left; i4_x_pos < t_clip.i4_right; i4_x_pos++)
                    {
                        UINT8 ui1_src_pixel = pui1_line_buffer[i4_x_pos - i4_dst_x];
                        if (ui1_src_pixel != (UINT8)ui4_colorkey)
                        {
                            pui1_line_start[i4_x_pos] = ui1_src_pixel;
                        }
                    }
                }

                pui1_line_start = pui1_line_start + t_lock_info.at_subplane[0].ui4_pitch;
            }
            x_cl_close(h_zip_state);
        } while (0);
        x_gl_surface_unlock(h_surf, NULL);
    }
    else
    {
        GL_DFB_TRACE("{GRAPHIC} ERR: Locking surface failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__);
    }


    return i4_ret;
}

INT32 gl_surface_sw_nhv_ptn_paint_com(
    GL_HSURFACE_T       h_surf,
    GL_COLORMODE_T      e_colormode,
    GL_COLOR_T          *pt_colorkey,
    UINT8               *pui1_compressed_img,
    GL_RECT_T           *pt_work_rect)      
{
    INT32               i4_ret;
    GL_DFB_SURFACE_T    *pt_surf;
    GL_RECT_T           t_clip;
    SIZE_T              z_img_pitch, z_compressed_length;
    HANDLE_T            h_zip_state;
    UINT8               *pui1_compressed_img_start;
    HANDLE_T            h_zip_engine;

    z_img_pitch               = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pui1_compressed_img);
    z_compressed_length       = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pui1_compressed_img + sizeof(SIZE_T));
    pui1_compressed_img_start = pui1_compressed_img + sizeof(INT32) * 2;

    /* get surface */
    pt_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, & i4_ret);
    if (GL_FAILED(i4_ret))
    {
        GL_DFB_TRACE("{GRAPHIC} ERR: Invalid surface handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__);
        return i4_ret;
    }

    /* get clip region */
    gl_surface_get_clip_area(pt_surf, &t_clip);
    INTERSECT_RECT(&t_clip, &t_clip, pt_work_rect);

    do
    {
        GL_SURF_INFO_T          t_surf_info;
        gl_dfb_obj_unlock(pt_surf);
        if (RECT_EMPTY(&t_clip))
        {
            return GLR_OK;
        }
        i4_ret = x_gl_surface_get_info(h_surf, &t_surf_info);
        if (i4_ret != GLR_OK)
        {
            return i4_ret;
        }
        /* allocate a tmp surface for decompression */
        if (_h_decompression_surf == NULL_HANDLE || 
			t_surf_info.ui4_width > _t_lock_info.at_subplane[0].ui4_width ||
			_t_lock_info.e_clrmode  != e_colormode)
        {
            GL_SURFACE_DESC_T       t_desc;
            if (_h_decompression_surf != NULL_HANDLE)
            {
                x_gl_obj_free(_h_decompression_surf);
                _h_decompression_surf = NULL_HANDLE;
            }
            {
                t_desc.e_clrmode  = e_colormode;
                t_desc.h_clipper  = NULL_HANDLE;
                t_desc.h_palette  = NULL_HANDLE;
                t_desc.ui4_width  = t_surf_info.ui4_width;
                t_desc.ui4_height = 20;

                i4_ret = x_gl_surface_create(&t_desc, &_h_decompression_surf);

                if (_h_decompression_surf == NULL_HANDLE || i4_ret != GLR_OK)
                {
                    GL_DFB_TRACE("{GRAPHIC} allocate decompression surf failed. Line: %d\n", __LINE__);
                    gl_dfb_obj_unlock(pt_surf);
                    return GLR_NO_MEMORY;
                }
                if (_h_decompression_surf != NULL_HANDLE)
                {
                    if (x_gl_surface_lock(_h_decompression_surf, NULL, GL_SYNC, &_t_lock_info) != GLR_OK)
                    {
                        GL_DFB_TRACE("{GRAPHIC} allocate decompression surf failed. Line: %d\n", __LINE__);
                        gl_dfb_obj_unlock(pt_surf);
                        return GLR_SURFACE_BUSY;
                    }

                    x_gl_surface_unlock(_h_decompression_surf, NULL);
                }
                _pt_decompression_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, _h_decompression_surf, & i4_ret);
                
                if (GL_FAILED(i4_ret) || _pt_decompression_surf == NULL)
                {
                    gl_dfb_obj_unlock(pt_surf);
                    return GLR_SURFACE_BUSY;
                }
                gl_dfb_obj_unlock(_pt_decompression_surf);
            }

            /* init the unzip engine */
            if (x_cl_open_eng(SN_CL_ZIP, NULL, NULL, &_pt_decompression_surf->h_zip_engine) != CLR_OK)
            {
                GL_DFB_TRACE("{GRAPHIC} unzip engine initialization failed. Line: %d\n", __LINE__);
                gl_dfb_obj_unlock(pt_surf);
                return GLR_FE_ERROR;
                
            }
        }
        
        {
            UINT32      ui4_cur_line;
            UINT8       *pui1_img_line_start;
            UINT32      ui4_img_height;
            SIZE_T      zdecompressed_length = 0;
            UINT32      ui4_i;
            INT32       i4_y;
            UINT32      ui4_src_y;
            GL_COLOR_T  t_empty_color = {0}; 
            
            h_zip_engine = _pt_decompression_surf->h_zip_engine;
            h_zip_state  = NULL_HANDLE;
            x_cl_get_decompress_len(h_zip_engine,
                                    pui1_compressed_img_start,
                                    z_compressed_length,
                                    CL_NULL_PARAM_TYPE,
                                    NULL,
                                    &zdecompressed_length);
            ui4_img_height = (UINT32)MIN((UINT32)RECT_H(pt_work_rect), zdecompressed_length/z_img_pitch);
 
            i4_y = t_clip.i4_top;
            ui4_src_y = (UINT32)(t_clip.i4_top - pt_work_rect->i4_top);
            
            /* copy operation */
            for (ui4_cur_line = 0; ui4_cur_line < ui4_img_height; )
            {
            		if (x_gl_surface_lock(_h_decompression_surf, NULL, GL_SYNC, &_t_lock_info) != GLR_OK)
		            {
		                GL_DFB_TRACE("{GRAPHIC} allocate decompression surf failed. Line: %d\n", __LINE__);
		                return GLR_SURFACE_BUSY;
		            }
                pui1_img_line_start = (UINT8 *)(_t_lock_info.at_subplane[0].pv_bits);
                
                for (ui4_i = 0; (ui4_i < 20) && (ui4_cur_line < ui4_img_height); ui4_i ++, ui4_cur_line++)
                {
                    z_img_pitch = (SIZE_T)MIN(_t_lock_info.at_subplane[0].ui4_pitch, GET_UINT32_FROM_PTR_LITTLE_END(pui1_compressed_img));
                    /* decompress one line of the zipped image */
                    i4_ret = x_cl_decompress(
                        h_zip_engine,
                        pui1_compressed_img_start,
                        z_compressed_length,
                        0,
                        CL_NULL_PARAM_TYPE,
                        NULL,
                        pui1_img_line_start,
                        &z_img_pitch,
                        &h_zip_state);

                    pui1_img_line_start += _t_lock_info.at_subplane[0].ui4_pitch;
                }
                
			          x_gl_surface_unlock(_h_decompression_surf, NULL);
				if (ui4_cur_line == 0)
				{
					break;
				}
                				
				if (ui4_cur_line % 20 == 0)
				{
                    i4_ret = x_gl_surface_compose(
                        h_surf,
                        _h_decompression_surf,
                        t_clip.i4_left,
                        i4_y,
                        t_clip.i4_left - pt_work_rect->i4_left,
                        ui4_src_y,
                        (UINT32)RECT_W(&t_clip),
                        (UINT32)(20 - ui4_src_y),
                        255,
                        GL_SRC_OVER,
                        GL_SYNC);
                        
                    i4_y += 20 - ui4_src_y;
				}
				else
				{
					i4_ret = x_gl_surface_compose(
                        h_surf,
                        _h_decompression_surf,
                        t_clip.i4_left,
                        i4_y,
                        t_clip.i4_left - pt_work_rect->i4_left,
                        ui4_src_y,
                        (UINT32)RECT_W(&t_clip),
                        (UINT32)(ui4_cur_line % 20 - ui4_src_y),
                        255,
                        GL_SRC_OVER,
                        GL_SYNC);
                    
					i4_y += (ui4_cur_line % 20 - i4_y%20);
				}
				ui4_src_y = 0;
                i4_ret = x_gl_surface_fill(_h_decompression_surf,&t_empty_color, 0);
            }
            
            x_cl_close(h_zip_state);
        }
    } while (0);
        
    return i4_ret;
}

HANDLE_T _nhv_cache_find(GL_COLORMODE_T e_colormode, UINT8 * pui1_compressed_img)
{

#ifndef MW_ANIMATION_IMG_CACHE
        return NULL_HANDLE;
#else
    HANDLE_T h_surf;
    SIZE_T              z_img_pitch, z_compressed_length;
    GL_SURFACE_DESC_T   desc;
    HANDLE_T            h_zip_engine;
    SIZE_T              z_decompressed_length;
    UINT8               *pui1_compressed_img_start;
    GL_COLOR_T          t_ck;
    GL_RECT_T           t_work_rect;
    GL_NHV_CATCH_T      *pt_cache = NULL;
    
    pt_cache = DLIST_HEAD(&(t_img_nhv_cache_list));
    
    while(pt_cache)
    {
        if(pt_cache->pv_data == pui1_compressed_img)
        {
            DLIST_REMOVE(pt_cache, &(t_img_nhv_cache_list), t_img_nhv_cache_entry);
            DLIST_INSERT_HEAD(pt_cache,&(t_img_nhv_cache_list),t_img_nhv_cache_entry);
            
            pt_cache->ui4_cache_life = 20;
            
            return pt_cache->h_surf;
        }
        pt_cache = DLIST_NEXT(pt_cache,t_img_nhv_cache_entry);
    }

    pt_cache = (GL_NHV_CATCH_T*)x_mem_alloc(sizeof(GL_NHV_CATCH_T));
    
    if (pt_cache == NULL)
    {
        return NULL_HANDLE;
    }
     
    z_img_pitch               = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pui1_compressed_img);
    z_compressed_length       = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pui1_compressed_img + sizeof(SIZE_T));
   
    pui1_compressed_img_start = pui1_compressed_img + sizeof(INT32) * 2;
    
    if (x_cl_open_eng(SN_CL_ZIP, NULL, NULL, &h_zip_engine) != CLR_OK)
    {
        GL_DFB_TRACE(("unzip engine initialization failed.\n"));
        x_mem_free(pt_cache);
        return NULL_HANDLE;
    }
    
    x_cl_get_decompress_len (h_zip_engine,   
             pui1_compressed_img_start, 
             z_compressed_length,
             CL_NULL_PARAM_TYPE, 
             0,
             &z_decompressed_length);

    if (z_decompressed_length > MW_IMG_CACHE_BUFFER_SIZE)
    {
        x_mem_free(pt_cache);
        x_cl_close(h_zip_engine);
        return NULL_HANDLE;
    }

    /*Replace Last one*/
    if (_ui4_img_cache_size < z_decompressed_length)
    {
        GL_NHV_CATCH_T*    pt_cache_temp = DLIST_HEAD(&(t_img_nhv_cache_list));
        
        while (pt_cache_temp)
        {
            if (pt_cache_temp->ui4_surface_size > z_decompressed_length)
            {
                pt_cache_temp = DLIST_NEXT(pt_cache_temp, t_img_nhv_cache_entry);
            }
            else
            {
                DLIST_REMOVE(pt_cache_temp, &(t_img_nhv_cache_list), t_img_nhv_cache_entry);
                
                x_gl_obj_free(pt_cache_temp->h_surf);
                
                _ui4_img_cache_size += pt_cache_temp->ui4_surface_size;

                x_mem_free(pt_cache_temp);

                pt_cache_temp = DLIST_HEAD(&(t_img_nhv_cache_list));
                
            }
            if (_ui4_img_cache_size >= z_decompressed_length)
            {
                break;
            }
        }
        if (_ui4_img_cache_size < z_decompressed_length)
        {
            x_mem_free(pt_cache);
            x_cl_close(h_zip_engine);
            return NULL_HANDLE; 
        }
    }

    desc.ui4_width = z_img_pitch/(gl_bit_count(e_colormode) >> 3);
    desc.ui4_height = z_decompressed_length/z_img_pitch;
    desc.e_clrmode = e_colormode;
    desc.h_palette = NULL_HANDLE;
    desc.h_clipper = NULL_HANDLE;

    if(GLR_OK != x_gl_surface_create(&desc, &h_surf))
    {
        GL_DFB_TRACE(("Surf create failed\n"));
        x_mem_free(pt_cache);
        x_cl_close(h_zip_engine);
        return NULL_HANDLE;
    }
    
    x_cl_close(h_zip_engine);
    
    SET_COLOR(&t_ck, 0, 0, 0, 0);
    SET_RECT_BY_SIZE(&t_work_rect, 0, 0, desc.ui4_width,  desc.ui4_height);

    if (GLR_OK != gl_surface_sw_nhv_ptn_paint_blt(h_surf, 
                            e_colormode, 
                            &t_ck, 
                            pui1_compressed_img, 
                            &t_work_rect)
        )
    {
        x_mem_free(pt_cache);
        x_gl_obj_free(h_surf);
        return NULL_HANDLE;
    }
    
    pt_cache->ui4_surface_size = z_decompressed_length;
    pt_cache->ui4_cache_life = 20;
    pt_cache->pv_data = pui1_compressed_img;
    pt_cache->h_surf = h_surf;

    _ui4_img_cache_size -= z_decompressed_length;

    DLIST_INSERT_HEAD(pt_cache,&(t_img_nhv_cache_list),t_img_nhv_cache_entry);
    
    return h_surf;
    
#endif
}

INT32 gl_surface_sw_nhv_ptn_paint(
    GL_HSURFACE_T       h_surf,
    GL_COLORMODE_T      e_colormode,
    GL_COLOR_T          *pt_colorkey,
    UINT8               *pui1_compressed_img,
    GL_RECT_T           *pt_work_rect)    
{
    HANDLE_T            h_cache_surf;
    INT32               i4_ret = GLR_OK;

    if (! list_init)
    {
    #ifdef MW_ANIMATION_IMG_CACHE
        DLIST_INIT(&(t_img_nhv_cache_list));
    #endif
        x_sema_create(&h_img_cache_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
        if (h_img_cache_lock == NULL_HANDLE)
        {
            return NULL_HANDLE;
        }
        
        list_init = TRUE;
    }
    
    x_sema_lock(h_img_cache_lock, X_SEMA_OPTION_WAIT);
    
    h_cache_surf = _nhv_cache_find(e_colormode, pui1_compressed_img);
    if(h_cache_surf == NULL_HANDLE)
    {
        i4_ret = gl_surface_sw_nhv_ptn_paint_blt(
                                h_surf,
                                e_colormode,
                                pt_colorkey,
                                pui1_compressed_img,
                                pt_work_rect);
    }
    else
    {
        GL_DFB_SURFACE_T         *pt_surf;
        
        pt_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, & i4_ret);
        if (GL_FAILED(i4_ret))
        {
            GL_DFB_TRACE("{GRAPHIC} ERR: Invalid surface handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__);
            x_sema_unlock(h_img_cache_lock);
            return i4_ret;
        }

        gl_dfb_obj_unlock(pt_surf);
        
        i4_ret = gl_dfb_surface_transp_blt(
                                h_surf, 
                                h_cache_surf, 
                                pt_work_rect->i4_left,
                                pt_work_rect->i4_top,
                                0,
                                0,
                                pt_surf->t_desc.ui4_width,
                                pt_surf->t_desc.ui4_height,
                                255,
                                TRUE,
                                pt_colorkey,
                                pt_colorkey,
                                GL_SYNC);

    }
    x_sema_unlock(h_img_cache_lock);
    return i4_ret;
}

INT32 gl_surface_sw_nhv_ptn_paint_compose(
    GL_HSURFACE_T       h_surf,
    GL_COLORMODE_T      e_colormode,
    GL_COLOR_T          *pt_colorkey,
    UINT8               *pui1_compressed_img,
    GL_RECT_T           *pt_work_rect)
{
    HANDLE_T            h_cache_surf;
    INT32               i4_ret = GLR_OK;

    if (! list_init)
    {
    #ifdef MW_ANIMATION_IMG_CACHE
        DLIST_INIT(&(t_img_nhv_cache_list));
    #endif
        x_sema_create(&h_img_cache_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
        if (h_img_cache_lock == NULL_HANDLE)
        {
            return NULL_HANDLE;
        }
        
        list_init = TRUE;
    }
    
    x_sema_lock(h_img_cache_lock, X_SEMA_OPTION_WAIT);
    
    h_cache_surf = _nhv_cache_find(e_colormode, pui1_compressed_img);
    if(h_cache_surf == NULL_HANDLE)
    {
        i4_ret = gl_surface_sw_nhv_ptn_paint_com(
                                h_surf,
                                e_colormode,
                                pt_colorkey,
                                pui1_compressed_img,
                                pt_work_rect);
    }
    else
    {
        GL_DFB_SURFACE_T*      pt_surf;
        pt_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, & i4_ret);
        if (GL_FAILED(i4_ret))
        {
            GL_DFB_TRACE("{GRAPHIC} ERR: Invalid surface handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__);
            x_sema_unlock(h_img_cache_lock);       
            return i4_ret;
        }

        gl_dfb_obj_unlock(pt_surf);
                    
        i4_ret = x_gl_surface_compose(
                                h_surf, 
                                h_cache_surf, 
                                pt_work_rect->i4_left,
                                pt_work_rect->i4_top,
                                0,
                                0,
                                RECT_W(pt_work_rect),
                                RECT_H(pt_work_rect),
                                255,
                                GL_SRC_OVER,
                                GL_SYNC);

    }

    x_sema_unlock(h_img_cache_lock);        

    return i4_ret;
}

INT32 gl_nhv_cache_flush(VOID)
{
#ifdef  MW_ANIMATION_IMG_CACHE
    GL_NHV_CATCH_T      *pt_cache = NULL;

    if (! list_init)
    {
        DLIST_INIT(&(t_img_nhv_cache_list));

        x_sema_create(&h_img_cache_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
        if (h_img_cache_lock == NULL_HANDLE)
        {
            return NULL_HANDLE;
        }
        
        list_init = TRUE;
    }

    x_sema_lock(h_img_cache_lock, X_SEMA_OPTION_WAIT);
    
    pt_cache = DLIST_HEAD(&(t_img_nhv_cache_list));
    
    while(pt_cache)
    {
        DLIST_REMOVE(pt_cache, &(t_img_nhv_cache_list), t_img_nhv_cache_entry);
        
        x_gl_obj_free(pt_cache->h_surf);
        
        _ui4_img_cache_size += pt_cache->ui4_surface_size;

        x_mem_free(pt_cache);
         
        pt_cache = DLIST_HEAD(&(t_img_nhv_cache_list));
    }
    
    x_sema_unlock(h_img_cache_lock);
    
#endif

    return GLR_OK;
}

#endif

