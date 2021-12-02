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
 * MW GL object types association:
 *
 * 
 * MW GL                        DirectFB
 * 
 * HScreen                      IDirectFBScreen
 * 
 * HPlane                       IDirectFBLayer
 * 
 * HView/Viewport               DirectFB "visible" surface storage
 *                              (IDirectFBLayer or IDirectFBWindow)
 * 
 * HSurface                     IDirectFBSurface
 * 
 * HClipper                     ...
 * 
 * HPalette                     IDirectFBPalette 
 *
 *
 *
 *
 * 1.   Screen/Layer in DirectFB are static. When system initialized the implementation
 *      enums them and construct MW GL HScreen/HPlane objects. Some MW GL API might lose
 *      its defination like enabling a plane.
 *
 * 2.   The surface in MW GL could be attached to a plane(region) area via "view/viewport"
 *      dynamicly. But in DFB region operations are not visible to client. User can create 
 *      off-screen surface, create primary surface or get surfaces from some objects like 
 *      layers and windows. To implement MW GL interfaces, a GL surface object contains 
 *      a dfb surface pointer. This GL surface is "virtual" and it could be attached with 
 *      different dfb surface dynamicly. Once user flip this surface to a plane, the GL 
 *      surface's dfb surface pointer might be one from layer->GetSurface or window->GetSurface. 
 *      When the surface is not fliped, the pointer is a off-screen surface from 
 *      DirectFB->CreateSurface. The surface pixel data will be copied when the pointer changes.
 *
 *  LOCK order:
 *   Screen > Plane > View > Surface 
 *   Same type of objects will in address order when multiple locked.
 *
 * */
#if 0
#include <directfb.h>
#include <sys/ioctl.h>
#if !defined(__UCLIBC__)
#include <stropts.h>
#endif
#endif
#include <fcntl.h>
#include "pthread.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>


#include "inc/u_common.h"
#include "graphic/u_gl.h"
#include "graphic/gl.h"
#include "graphic/gl_dfb.h"
#include "graphic/gl_dfb_surf.h"

#include "handle/u_handle.h"
#include "stdio.h"
#include "gl_dfb_util.h"

#if 1
#include "mtgfx.h"
#include "mtosd.h"
#include "mtpmx.h"
#include "mtcommon.h"
#include "gl_mem_manager.h"
#endif

#define FBIO_GETIMAGEBUFFER                       0x4629
#define FBIO_GETFRAMEBUFFER                       0x4634


struct mt53fb_imagebuffer
{
    UINT32                   u4Size;
    UINT32                   u4VirtAddr;
    UINT32                   u4PhyAddr;
};

typedef struct mt53fb_imagebuffer mt53fb_imagebuffer;

/*FixMe:
 * These are for some apis calling RM.
 */
#if 0
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_plane_mxr.h"

#ifdef MW_PHOTO_SHOW_ON_VDP
#include "../../chiling/driver/directfb/MediaTek/mt53xx/mt53xx_fb.h"
#endif


IDirectFB *             g_p_dfb;
#endif
static GL_DFB_ROOT_T  * p_gl_dfb_root;

static BOOL             _b_inited = FALSE;

/*Tmp for dbg*/
//static int wcount = 0;

static  pthread_mutex_t g_gl_dfb_mtx;
static  pthread_mutex_t g_gl_dfb_op_mtx;



void 
gl_dfb_global_lock()
{
    if(!_b_inited)return;
    pthread_mutex_lock(&g_gl_dfb_mtx);
       
}
 
void
gl_dfb_global_unlock()
{
    if(!_b_inited)return;
    pthread_mutex_unlock(&g_gl_dfb_mtx);
       
}

/*
 * For Video garbage CRs:
 * We need to decrease the band width. But DirectFB does not support
 * enable/disable planes.
 */
 #if 0
static INT32
_dfb_enable_layer(IDirectFBDisplayLayer * p_dfb_layer, BOOL b_enable)
{
    mw_gl_op_t  t_op;
    int         fd;
    INT32       i4_ret;
    fd = gl_dfb_get_fb_fd();
    if(fd < 0)
    {
        return GLR_DRV_ERROR;
    }
    
    t_op.u4Op = MW_GL_OP_SET_PLANE;
    
    DFBCHECK(p_dfb_layer->GetID(p_dfb_layer, (DFBDisplayLayerID *)(&t_op.u.set_plane.i4_idx)), &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return GLR_DRV_ERROR;    
    }
    
    if(b_enable == TRUE)
    {
        t_op.u.set_plane.i4_enable = 1;
    }
    else
    {
        t_op.u.set_plane.i4_enable = 0;        
    }
    
    ioctl(fd, FBIO_MW_OP, &t_op);

    return GLR_OK;
}
#else
static INT32
_dfb_enable_layer(UINT32 plane_id, BOOL b_enable)
{
    if (MTOSD_PLA_Enable(plane_id,b_enable)!= MTR_OK)
	{
		return GLR_DRV_ERROR;
	}
    return GLR_OK;
}

#endif
#if 0
static INT32
_dfb_enable_osd_scaler(UINT32 ui4_scaler_id, 
							BOOL b_enable, 
							UINT32 ui4_src_width, 
							UINT32 ui4_src_height)
{

#if 0
    mw_gl_op_t  t_op;
    int         fd;
    
    fd = gl_dfb_get_fb_fd();

    if(fd < 0)
    {
        return GLR_DRV_ERROR;
    }
    
    t_op.u4Op = MW_GL_OP_ENABLE_SCALER;
        
    t_op.u.enable_scaler.ui4_enable = ((b_enable == TRUE)? 1: 0);
    t_op.u.enable_scaler.ui4_id     = (unsigned int)ui4_scaler_id;
    t_op.u.enable_scaler.ui4_width  = ui4_src_width;
    t_op.u.enable_scaler.ui4_height = ui4_src_height;
    
    if (ioctl(fd, FBIO_MW_OP, &t_op) < 0)
    {
        return GLR_DRV_ERROR;
    }
    else
    {
        return GLR_OK;
    }
#else
	UINT32 u4DstWidth;
	UINT32 u4DstHeight;

	MTPMX_PANEL_GetResolution(&u4DstWidth,&u4DstHeight);
	MTOSD_SC_Scale(ui4_scaler_id,
				   (b_enable == TRUE)? 1: 0,
				   ui4_src_width,ui4_src_height, u4DstWidth, u4DstHeight);
	return GLR_OK;
#endif
}
#endif

static BOOL
_plane_free_fct(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle)
{
    GL_DFB_PLANE_T * p_plane;
    p_plane = pv_obj;
    GL_DFB_BEGIN_FREE(h_handle, pv_obj)

        gl_dfb_dy_vec_destroy(p_plane->p_hview_vec);
        gl_dfb_obj_uninit(pv_obj);
        GL_FREE(pv_obj);
        
    GL_DFB_END_FREE(h_handle, pv_obj)
}


static HANDLE_T cc_helper_plane;

GL_HPLANE_T 
gl_plane_duplicate_handle(GL_HPLANE_T h_plane)
{
    return (gl_dfb_duplicate_handle(h_plane, GLT_OBJ_PLANE, _plane_free_fct));
}

/*#define PLANE_MW_IDX_2_DFB_IDX(ui2_idx) ui2_idx = ((ui2_idx == 1)?0:1)*/

static BOOL
_screen_free_fct(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle)
{
    GL_DFB_SCREEN_T * p_scr;
    p_scr = pv_obj;
    GL_DFB_BEGIN_FREE(h_handle, pv_obj)

        gl_dfb_dy_vec_destroy(p_scr->p_hplane_vec);        
        gl_dfb_obj_uninit(pv_obj);
        GL_FREE(pv_obj);

    GL_DFB_END_FREE(h_handle, pv_obj)
}


#if 0
static HANDLE_T 
_create_plane(HANDLE_T h_scr, IDirectFBDisplayLayer * p_dfb_layer, BOOL b_use_window, DFBSurfacePixelFormat e_colormode,
                 INT32 i4_width, INT32 i4_height)
{
/*    DFBDisplayLayerConfig dlconfig;*/
    
    GL_DFB_PLANE_T *        p_plane;
    HANDLE_T                handle;
    DFBWindowDescription    t_wdsc;
    INT32                   i4_ret;
    p_plane = (GL_DFB_PLANE_T *)GL_MALLOC(sizeof(GL_DFB_PLANE_T));
    GL_MEMSET(p_plane, 0, sizeof(GL_DFB_PLANE_T));
    gl_dfb_obj_init(GLT_OBJ_PLANE, p_plane);

    p_plane->p_hview_vec    = gl_dfb_dy_vec_init(4, sizeof(HANDLE_T ));
    p_plane->p_dfb_layer    = p_dfb_layer;   
    p_plane->h_scr          = h_scr;
    p_plane->ui1_alpha      = 0xFF;
    p_plane->p_wnd          = NULL;
    
    if(b_use_window)
    {
        t_wdsc.flags = 
                DWDESC_CAPS
            |   DWDESC_SURFACE_CAPS
            |   DWDESC_POSX
            |   DWDESC_POSY
            |   DWDESC_WIDTH
            |   DWDESC_HEIGHT
            |   DWDESC_PIXELFORMAT
            |   DWDESC_OPTIONS;

        t_wdsc.posx         = 0;
        t_wdsc.posy         = 0;
        t_wdsc.width = i4_width;
        t_wdsc.height = i4_height;
        t_wdsc.pixelformat  = e_colormode;
        t_wdsc.caps         = /*DWCAPS_NODECORATION*//*DWCAPS_DOUBLEBUFFER;*/DWCAPS_NONE;
        t_wdsc.surface_caps = /*DSCAPS_NONE*/DSCAPS_VIDEOONLY/*DSCAPS_SYSTEMONLY*//*|DSCAPS_PREMULTIPLIED*/;
        t_wdsc.options      = DWOP_ALPHACHANNEL/*DWOP_COLORKEYING|DWOP_SCALE*/; 

        p_dfb_layer->SetLevel(p_dfb_layer, 0);
        DFBCHECK(p_dfb_layer->CreateWindow(p_plane->p_dfb_layer, 
                                           &t_wdsc, 
                                           &(p_plane->p_wnd)), &i4_ret);
        wcount ++;
        GL_DFB_TRACE("[GL_DFB][CREATED WINDOW]window count:%x\n", wcount);
        
        DFBCHECK(p_plane->p_wnd->SetOpacity(p_plane->p_wnd, 0xFF), &i4_ret);
    }       
    else
    {
        /* For layer exclusived for MW, disable it */
        _dfb_enable_layer(p_plane->p_dfb_layer, FALSE);
    }
    handle = GL_ALLOC_HANDLE(GLT_OBJ_PLANE, p_plane, NULL, _plane_free_fct);

    return handle;
}


static void 
_create_planes(HANDLE_T h_scr, GL_DFB_SCREEN_T * p_scr, DFBSurfacePixelFormat e_colormode,
                   INT32 i4_width, INT32 i4_height)
{
    IDirectFBDisplayLayer * p_dfb_layer;
    HANDLE_T                h_plane;
    /*DFBDisplayLayerConfig   dfb_cfg;*/
    INT32                   i4_ret;
    DFBDisplayLayerConfig   dfb_layer_cfg;
#ifdef MW_PHOTO_SHOW_ON_VDP
	INT32                   i4_screen_width;
	INT32                   i4_screen_height;
#endif

    /* Plane 0 */
    DFBCHECK(g_p_dfb->GetDisplayLayer(g_p_dfb, 1, &p_dfb_layer), &i4_ret);
    p_dfb_layer->SetCooperativeLevel(p_dfb_layer, DFSCL_EXCLUSIVE);
    
    dfb_layer_cfg.flags       = DLCONF_PIXELFORMAT|DLCONF_BUFFERMODE|DLCONF_OPTIONS;
    dfb_layer_cfg.pixelformat = e_colormode; /*Default pixelformat*/
    dfb_layer_cfg.buffermode  = DLBM_FRONTONLY;
    dfb_layer_cfg.options     = DLOP_ALPHACHANNEL|DLOP_OPACITY;
    DFBCHECK(p_dfb_layer->SetConfiguration(p_dfb_layer, &dfb_layer_cfg), &i4_ret);

    h_plane = _create_plane(h_scr, p_dfb_layer, FALSE, e_colormode, i4_width, i4_height);
    gl_dfb_dy_vec_add(p_scr->p_hplane_vec, &h_plane);

    GL_DFB_INFO("[* GL DFB *] Create plane (ON layer) :%x \n", (unsigned int)h_plane);

    /* Plane 1 */    
    DFBCHECK(g_p_dfb->GetDisplayLayer(g_p_dfb, DLID_PRIMARY, &p_dfb_layer), &i4_ret);
    p_dfb_layer->SetCooperativeLevel(p_dfb_layer, DLSCL_ADMINISTRATIVE);
    h_plane = _create_plane(h_scr, p_dfb_layer, TRUE, e_colormode, i4_width, i4_height);
    gl_dfb_dy_vec_add(p_scr->p_hplane_vec, &h_plane);
    /* Set the plane on window to be CC helper plane */
    cc_helper_plane = h_plane;


#ifdef MW_PHOTO_SHOW_ON_VDP
    /* VDP plane */    
    DFBCHECK(g_p_dfb->GetDisplayLayer(g_p_dfb, 2, &p_dfb_layer), &i4_ret);
    p_dfb_layer->SetCooperativeLevel(p_dfb_layer, DFSCL_EXCLUSIVE);

	DFBCHECK(p_scr->p_dfb_screen->GetSize(p_scr->p_dfb_screen, (int*)&i4_screen_width, (int*)&i4_screen_height), &i4_ret);
	
    dfb_layer_cfg.flags       = DLCONF_PIXELFORMAT|DLCONF_BUFFERMODE| DLCONF_WIDTH | DLCONF_HEIGHT;
    dfb_layer_cfg.pixelformat = DSPF_NV16; /*Default pixelformat*/
    dfb_layer_cfg.buffermode  = DLBM_FRONTONLY;
	dfb_layer_cfg.width = i4_screen_width;
	dfb_layer_cfg.height= i4_screen_height;
    DFBCHECK(p_dfb_layer->SetConfiguration(p_dfb_layer, &dfb_layer_cfg), &i4_ret);
    h_plane = _create_plane(h_scr, p_dfb_layer, FALSE, e_colormode, i4_screen_width, i4_screen_height);
    gl_dfb_dy_vec_add(p_scr->p_hplane_vec, &h_plane);
#endif

    GL_DFB_INFO("[* GL DFB *] Create plane (ON window (Shared)) :%x \n", (unsigned int)h_plane);


}    




/*
 * Do a simple initialization for screen. We just create the
 * main screen.
 */
static HANDLE_T
_create_screens(GL_DFB_ROOT_T * p_root, DFBSurfacePixelFormat e_colormode, INT32 i4_width, INT32 i4_height)
{
    GL_DFB_SCREEN_T *   p_scr;
    HANDLE_T            h_scr;
    INT32               i4_ret;
   
    p_scr = (GL_DFB_SCREEN_T *)GL_MALLOC(sizeof(GL_DFB_SCREEN_T));
    GL_MEMSET(p_scr, 0, sizeof(GL_DFB_SCREEN_T));
    gl_dfb_obj_init(GLT_OBJ_SCREEN, p_scr);
    
    /*The primary surface name used in MW GL is defined SN_PRES_XXXX*/
    x_strcpy(p_scr->s_name, SN_PRES_MAIN_DISPLAY);  

    p_scr->b_enabled    = FALSE;
    p_scr->p_hplane_vec = gl_dfb_dy_vec_init(4, sizeof(HANDLE_T));
    
    DFBCHECK(g_p_dfb->GetScreen(g_p_dfb, 0, &(p_scr->p_dfb_screen)), &i4_ret);
   
    h_scr = GL_ALLOC_HANDLE(GLT_OBJ_SCREEN, p_scr, NULL, _screen_free_fct);

    _create_planes(h_scr, p_scr, e_colormode, i4_width, i4_height);

    gl_dfb_dy_vec_add(p_root->p_hscreen_vec, &h_scr);
    return h_scr;
}

#else

static UINT32 g_subtitle_rgn_list;

static UINT32 g_subtitle_rgn;

static GL_MEM_POOL* osd_rgn_pool = NULL;

static UINT32		osd_rgn_pitch = 0;

static UINT32 screen_width = 0;
static UINT32 screen_height = 0;
static UINT32 ui_width = 0;
static UINT32 ui_height = 0;
static BOOL b_scaler_inited = FALSE;

static INLINE UINT16 gl_dfb_bit_count(GL_COLORMODE_T e_mode)
{
    static const UINT8 s_dfb_aui2_pixel_bits[] = {
        2, 4, 8, 8, 8, 32, 2, 4, 8, 16, 16, 16, 32, 0, 0, 0, 0, 0, 0 };

    return (s_dfb_aui2_pixel_bits[e_mode]);
}

GL_MEM_POOL* gl_dfb_get_layer_pool(UINT32 *ui4_pitch)
{
	*ui4_pitch = osd_rgn_pitch;
	return osd_rgn_pool;
}

UINT32 gl_dfb_get_layer_region_list(void)
{
	return g_subtitle_rgn_list;
}

INT32 gl_dfb_subtitle_plane_flip(void)
{
	UINT32 au4PlaneArray[5] = { MTPMX_OSD2, MTPMX_OSD1, MTPMX_PIP, MTPMX_MAIN, MTPMX_OSD3 };
	MTPMX_SetPlaneOrderArray(au4PlaneArray);
	if (!b_scaler_inited)
	{
	    MTOSD_PLA_Enable(MTOSD_PLANE_1, TRUE);
		MTOSD_SC_Scale(MTOSD_SCALER_1, 
				   TRUE, 
				   ui_width, 
				   ui_height,  
				   screen_width, 
				   screen_height);
	}
	MTOSD_PLA_FlipTo(MTOSD_PLANE_1,g_subtitle_rgn_list);

	if (!b_scaler_inited)
	{
		MTOSD_SC_Scale(MTOSD_SCALER_1, 
					   TRUE, 
					   ui_width, 
					   ui_height,  
					   screen_width, 
					   screen_height);

		b_scaler_inited = TRUE;
		printf("gl_init MTOSD_SC_Scale complete!\n");
	}
	return GLR_OK;
}


static unsigned long
_gl_PageAlign( unsigned long value )
{
     unsigned long mask = sysconf( _SC_PAGESIZE ) - 1;

     return (value + mask) & ~mask;
}

static long
_gl_PageSize( void )
{
     return sysconf( _SC_PAGESIZE );
}

static int
_gl_TryOpen( const char *name1, const char *name2)
{
     int   file;
     file = open( name1, O_RDWR, 660);
     if (file  <= 0) 
	 {
          file = open( name2, O_RDWR, 660);
          if (file <= 0) 
		  {
               printf("[%s:%d]open fb error\n", __FUNCTION__, __LINE__);
          }
     }
	 
     return file;
}


INT32 FrameBuffer_Init(VOID)
{
	int fd;
	mt53fb_imagebuffer imageBuffer;
	VOID* mapmem = NULL;
	
	 fd = _gl_TryOpen("dev/fb0", "/dev/fb/0");

     if (ioctl(fd, FBIO_GETFRAMEBUFFER, &imageBuffer ) < 0)
	 {
          printf("[%s:%d]ioctl fb error\n", __FUNCTION__, __LINE__);
          return GLR_INIT_FAILED;	 
     }

	 mapmem = mmap( (void*)imageBuffer.u4PhyAddr, imageBuffer.u4Size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 0 );

	 
     if (ioctl(fd, FBIO_GETIMAGEBUFFER, &imageBuffer ) < 0)
	 {
          printf("[%s:%d]ioctl fb error\n", __FUNCTION__, __LINE__);
          return GLR_INIT_FAILED;
     }
	 
     mapmem = mmap( (void*)imageBuffer.u4PhyAddr,
                           imageBuffer.u4Size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 
                           _gl_PageAlign( imageBuffer.u4Size ) + _gl_PageSize() + 256 * 1024);
	 
	 if ((int)mapmem == -1)
	 {
	 	printf("[%s:%d]mmap error\n", __FUNCTION__, __LINE__);
		return GLR_INIT_FAILED;
	 }
	 return GLR_OK;
}

static void 
_create_planes(HANDLE_T h_scr, GL_DFB_SCREEN_T * p_scr, GL_COLORMODE_T e_colormode,
                   INT32 i4_width, INT32 i4_height)
{
	GL_POOL_BUF_T 			t_buffer;
	MTGFX_GETBUF_T 			t_gfxbuffer;
	//UINT8 *         		puac_buffer;
	GL_DFB_PLANE_T *		p_plane;
	HANDLE_T				h_plane;
	INT32					i4_ret;
	SIZE_T 					size;
	MTGFX_FILL_T            fill_arg;

	UINT32 au4PlaneArray[5] = { MTPMX_OSD2, MTPMX_OSD1, MTPMX_PIP, MTPMX_MAIN, MTPMX_OSD3 };
	ui_width = (UINT32)i4_width;
	ui_height = (UINT32)i4_height;

	i4_ret = FrameBuffer_Init();
	if (i4_ret != GLR_OK)
	{
		printf("FrameBuffer_Init fail.\n");
		//ASSERT(0);
		return;
	}
	
	printf("gl_init MTAL_Init!\n");
	MTAL_Init();
	printf("gl_init MTAL_Init complete!\n");
	MTGFX_GetBufferPartition(&t_gfxbuffer);
	
	printf("gl_init MTGFX_GetBufferPartition complete!\n");
	t_buffer.u4Addr     = t_gfxbuffer.u4Addr;
	t_buffer.u4PhyAddr  = t_gfxbuffer.u4PhyAddr;
	t_buffer.u4Size     = t_gfxbuffer.u4Size;
	t_buffer.u4VirtAddr = t_gfxbuffer.u4VirtAddr;
	
	gl_mem_manager_init(&t_buffer);
	printf("gl_init gl_mem_manager_init complete!\n");

	
	osd_rgn_pitch = i4_width  * (gl_dfb_bit_count(e_colormode) >> 3);
	size = i4_height * osd_rgn_pitch;
	osd_rgn_pool = gl_mem_manager_alloc(size);

	fill_arg.pvDst		= (VOID*)osd_rgn_pool->bmp_buffer.u4PhyAddr;
	fill_arg.i4DstX 	= 0;
	fill_arg.i4DstY 	= 0;
	fill_arg.u4DstPitch = osd_rgn_pitch;
	fill_arg.eDstCm 	= gl_cm_to_mtal_cm(e_colormode);
	fill_arg.u4Width	= i4_width;
	fill_arg.u4Height	= i4_height;
	fill_arg.u4Color	= 0x0;

	
	i4_ret = MTGFX_FillRect(&fill_arg);
	MTGFX_Flush();
	MTGFX_Wait(-1);
	
	MTOSD_RGN_LIST_Create(&g_subtitle_rgn_list);
	printf("gl_init MTOSD_RGN_LIST_Create complete!\n");
	
	MTOSD_RGN_Create(&g_subtitle_rgn, 
			(UINT32)i4_width, 
			(UINT32)i4_height, 
			(VOID*)osd_rgn_pool->bmp_buffer.u4PhyAddr, 
			gl_cm_to_mtal_cm(e_colormode), 
			osd_rgn_pitch,
			0, 
			0, 
			(UINT32)i4_width, 
			(UINT32)i4_height);
	
	printf("gl_init MTOSD_RGN_Create complete!\n");

	MTOSD_RGN_Insert(g_subtitle_rgn,g_subtitle_rgn_list);

	MTOSD_RGN_Set(g_subtitle_rgn, MTOSD_RGN_MIX_SEL,MTOSD_BM_PIXEL);
	MTPMX_PANEL_GetResolution(&screen_width,&screen_height);

	MTPMX_SetPlaneOrderArray(au4PlaneArray);
	#if 0
	MTOSD_PLA_FlipTo(MTOSD_PLANE_1,g_subtitle_rgn_list);
	
	MTOSD_SC_Scale(MTOSD_SCALER_1, 
				   TRUE, 
				   (UINT32)i4_width, 
				   (UINT32)i4_height,  
				   screen_width, 
				   screen_height);

	printf("gl_init MTOSD_SC_Scale complete!\n");
	#endif
	p_plane = (GL_DFB_PLANE_T *)GL_MALLOC(sizeof(GL_DFB_PLANE_T));
	GL_MEMSET(p_plane, 0, sizeof(GL_DFB_PLANE_T));
	gl_dfb_obj_init(GLT_OBJ_PLANE, p_plane);

	p_plane->p_hview_vec	= gl_dfb_dy_vec_init(4, sizeof(HANDLE_T ));
	p_plane->plane_id	    = MTOSD_PLANE_1;	 
	p_plane->h_scr			= h_scr;
	p_plane->ui1_alpha		= 0xFF;
	h_plane = GL_ALLOC_HANDLE(GLT_OBJ_PLANE, p_plane, NULL, _plane_free_fct);
	
	gl_dfb_dy_vec_add(p_scr->p_hplane_vec, &h_plane);
	
	p_plane = (GL_DFB_PLANE_T *)GL_MALLOC(sizeof(GL_DFB_PLANE_T));
	GL_MEMSET(p_plane, 0, sizeof(GL_DFB_PLANE_T));
	gl_dfb_obj_init(GLT_OBJ_PLANE, p_plane);

	p_plane->p_hview_vec	= gl_dfb_dy_vec_init(4, sizeof(HANDLE_T ));
	p_plane->plane_id	    = MTOSD_PLANE_1;	 
	p_plane->h_scr			= h_scr;
	p_plane->ui1_alpha		= 0xFF;
	h_plane = GL_ALLOC_HANDLE(GLT_OBJ_PLANE, p_plane, NULL, _plane_free_fct);
	
	gl_dfb_dy_vec_add(p_scr->p_hplane_vec, &h_plane);
	
    /* Set the plane on window to be CC helper plane */
    cc_helper_plane = h_plane;
    GL_DFB_INFO("[* GL DFB *] Create plane (ON window (Shared)) :%x \n", (unsigned int)h_plane);

}

static HANDLE_T
_create_screens(GL_DFB_ROOT_T * p_root, GL_COLORMODE_T e_colormode, INT32 i4_width, INT32 i4_height)
{
    GL_DFB_SCREEN_T *   p_scr;
    HANDLE_T            h_scr;
    //INT32               i4_ret;
   
    p_scr = (GL_DFB_SCREEN_T *)GL_MALLOC(sizeof(GL_DFB_SCREEN_T));
    GL_MEMSET(p_scr, 0, sizeof(GL_DFB_SCREEN_T));
    gl_dfb_obj_init(GLT_OBJ_SCREEN, p_scr);
    
    /*The primary surface name used in MW GL is defined SN_PRES_XXXX*/
    x_strcpy(p_scr->s_name, SN_PRES_MAIN_DISPLAY);  

    p_scr->b_enabled    = FALSE;
    p_scr->p_hplane_vec = gl_dfb_dy_vec_init(4, sizeof(HANDLE_T));
    h_scr = GL_ALLOC_HANDLE(GLT_OBJ_SCREEN, p_scr, NULL, _screen_free_fct);

    _create_planes(h_scr, p_scr, e_colormode, i4_width, i4_height);

    gl_dfb_dy_vec_add(p_root->p_hscreen_vec, &h_scr);
    return h_scr;
}


#endif

static INT32 
_handle_autofree (
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type)
{
    if (e_type < GLT_OBJ_START || e_type >= GLT_OBJ_END)
    {
        return HR_INV_HANDLE_TYPE;
    }

    return (x_gl_obj_free(h_handle) == GLR_OK ? HR_OK : HR_INV_HANDLE);
}

/* Get FB file descriptor */
int gl_dfb_get_fb_fd()
{    
    static int fd_fb = 0;

    if(fd_fb == 0)
    {
        fd_fb = open("/dev/fb/0", O_RDWR);
        if(fd_fb <= 0)
        {
            printf("Open FB failed in MW GL\n");
            return -1;
        }
    }
    return fd_fb;   
}


/*Dirty */
//extern int g_main_argc;
//extern char ** g_main_argv;

VOID* gl_dfb_get_native_engine(VOID)
{
    if(! _b_inited)
    {
        return NULL;
    }
#if 0
    return g_p_dfb;
#else
	return NULL;
#endif
}
INT32
gl_dfb_init(GL_COLORMODE_T e_colormode, UINT32 ui4_width, UINT32 ui4_height)
{
    
    static  handle_autofree_fct     apf_autofree_fcts[GLT_OBJ_END - GLT_OBJ_START];
    UINT32                          u_idx;
    pthread_mutexattr_t             mtx_attr;
    
    INT32                           i4_ret;
    if(_b_inited)
    {
        return GLR_OK;
    }        
    /* Init the global mutex lock*/    
    if((i4_ret = pthread_mutexattr_init(&mtx_attr)) != 0)
    {
        GL_DFB_ABORT("Error for pthread init mtx attr\n");
    }
    pthread_mutexattr_settype (&mtx_attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&g_gl_dfb_mtx, &mtx_attr);
    
    if((i4_ret = pthread_mutexattr_init(&mtx_attr)) != 0)
    {
        GL_DFB_ABORT("Error for pthread init mtx attr\n");
    }
    pthread_mutexattr_settype (&mtx_attr, PTHREAD_MUTEX_RECURSIVE_NP);
    if((i4_ret = pthread_mutex_init(&g_gl_dfb_op_mtx, &mtx_attr)) != 0)
    {
        GL_DFB_ABORT("Error for pthread op mtx init\n");
    } 
    
    /* Autofree functions for avoid HANDLE ABORT      */
    for (u_idx = 0; u_idx < GLT_OBJ_END - GLT_OBJ_START; u_idx++)
    {
        apf_autofree_fcts[u_idx] = _handle_autofree;
    }
    if (handle_set_autofree_tbl(HT_GROUP_GL, apf_autofree_fcts) != HR_OK)
    {
        return GLR_INIT_FAILED;
    }
    
    /* init the direct frame buffer */
#if 0
		{
			 /*DFBSurfaceDescription		   surf_desc;*/
			DFBGraphicsDeviceDescription	dfb_dev_desc;
			 
			/* init the direct frame buffer */
#ifdef LINUX_TURNKEY_SOLUTION_PHASE2
			/*DirectFBInit(&g_main_argc, &g_main_argv );*/
#else /* For 1.5 */
		//	  DirectFBInit(&g_main_argc, &g_main_argv );
			DirectFBInit(NULL, NULL);
#endif /* LINUX_SUPPORT_PHASE2 */
			
			/* create pointer to direct frame buffer */
			DirectFBCreate( &g_p_dfb );
	
		   
			/*Get information and show it*/
			g_p_dfb->GetDeviceDescription (g_p_dfb,  &dfb_dev_desc);
			
			GL_DFB_TRACE("\n\n[* GL DFB *] DirectFB for GL inited. Description:\n");
			GL_DFB_TRACE("[* GL DFB *] Acceleration Mask:0x%08x, Blit Flag:0x%08x\n",
					dfb_dev_desc.acceleration_mask,
					dfb_dev_desc.blitting_flags);
			GL_DFB_TRACE("[* GL DFB *] Video memory:%d bytes\n\n",
					dfb_dev_desc.video_memory);
	
			/*Init gl_dfb_root object*/
			p_gl_dfb_root = (GL_DFB_ROOT_T *)GL_MALLOC(sizeof(p_gl_dfb_root));	  
			p_gl_dfb_root->p_hscreen_vec = gl_dfb_dy_vec_init(4, sizeof(HANDLE_T));
	
		}
		
		
		/*
		 * We should configure which plane is a window and which is a layer.
		 */
		
		_create_screens(p_gl_dfb_root, gl_dfb_cm_map_get(e_colormode), (INT32)ui4_width, (INT32)ui4_height);

#else
		{
			/*Init gl_dfb_root object*/
			p_gl_dfb_root = (GL_DFB_ROOT_T *)GL_MALLOC(sizeof(p_gl_dfb_root));	  
			p_gl_dfb_root->p_hscreen_vec = gl_dfb_dy_vec_init(4, sizeof(HANDLE_T));
		}

		_create_screens(p_gl_dfb_root, e_colormode, (INT32)ui4_width, (INT32)ui4_height);

#endif
    
    _b_inited = TRUE;
    return GLR_OK;
}

#if 0
IDirectFBWindow * gl_dfb_get_main_wnd()
{    
    HANDLE_T            h_scr;
    HANDLE_T            h_plane;    
    GL_DFB_SCREEN_T *   p_scr;
    GL_DFB_PLANE_T *    p_plane;
    INT32               i4_ret;
    IDirectFBWindow     *p_wnd = NULL;

    gl_dfb_dy_vec_get(p_gl_dfb_root->p_hscreen_vec, 0, &h_scr);

    p_scr =  gl_dfb_handle_lock_obj(GLT_OBJ_SCREEN, h_scr, &i4_ret);
    
    if(i4_ret != GLR_OK)
    {
        return NULL;
    }
    
    if(gl_dfb_dy_vec_get(p_scr->p_hplane_vec, 1, &h_plane) == TRUE)
    {
        p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
        if(i4_ret != GLR_OK)
        {
            gl_dfb_obj_unlock(p_scr);
            return NULL;
        }
        p_wnd = p_plane->p_wnd;    
        gl_dfb_obj_unlock(p_plane);
    }
    
    gl_dfb_obj_unlock(p_scr);

    return p_wnd;
}    

#endif

INT32
gl_dfb_screen_get_panel_size( 
    GL_HSCREEN_T        h_screen,
    UINT32              *pui4_w,
    UINT32              *pui4_h)
	{
#if 0
		GL_DFB_SCREEN_T *	p_scr;
		INT32				i4_ret;
		int 				i4_dfb_w;
		int 				i4_dfb_h;
	
		/* IF the handle is NULL get a default */
		if(h_screen == NULL_HANDLE)
		{
			if(gl_dfb_dy_vec_get(p_gl_dfb_root->p_hscreen_vec, 0, &h_screen) == FALSE)
			{			 
				return GLR_DRV_ERROR;
			}
		}
		p_scr = gl_dfb_handle_lock_obj(GLT_OBJ_SCREEN, h_screen, &i4_ret);
		RETURN_RET_IF_FAIL(i4_ret);
	
		DFBCHECK(p_scr->p_dfb_screen->GetSize(p_scr->p_dfb_screen, &i4_dfb_w, &i4_dfb_h), &i4_ret);
		*pui4_w = (UINT32)i4_dfb_w;
		*pui4_h = (UINT32)i4_dfb_h;
		
		gl_dfb_obj_unlock(p_scr);
		
		return i4_ret;
	
#else
		if (MTPMX_PANEL_GetResolution(pui4_w,pui4_h) != MTR_OK)
		{
			return GLR_DRV_ERROR;
		}
		return GLR_OK;
#endif
	}


INT32 
gl_dfb_screen_get_name(HANDLE_T h_screen, CHAR ** ps_name)
{
    GL_DFB_SCREEN_T *   p_scr;
    INT32               i4_ret;
    p_scr = gl_dfb_handle_lock_obj(GLT_OBJ_SCREEN, h_screen, &i4_ret);
    RETURN_RET_IF_FAIL(i4_ret);

    *ps_name = p_scr->s_name;
    
    gl_dfb_obj_unlock(p_scr);
    return GLR_OK;
    
}

INT32 
gl_dfb_screen_open(
    const CHAR *        ps_name,
    x_gl_nfy_fct        pf_screen_nfy,
    VOID *              pv_nfy_tag,
    GL_HSCREEN_T *      ph_screen)
{
    UINT32                  ui_idx;
    CHAR *                  s_name;
    HANDLE_T                h_scr = NULL_HANDLE;
    HANDLE_T *              ph_scr;
    /*IDirectFBScreen *       p_dfb_screen;*/
    /*IDirectFBDisplayLayer * p_dfb_layer;*/

    GL_DFB_DY_VEC_FOR_EACH(p_gl_dfb_root->p_hscreen_vec, ui_idx, ph_scr)
    {
        gl_dfb_screen_get_name(*ph_scr, &s_name);       
        GL_DFB_TRACE("s_name is :%s \n", s_name);
            
        if(x_strcmp(s_name, ps_name) == 0)
        {
            h_scr = *ph_scr;
            GL_DFB_TRACE("[gl_dfb_screen_open]Found screen %s \n", s_name);
            break;        
        }
    }    

    if(h_scr == NULL_HANDLE)
    {
        GL_DFB_TRACE("[gl_dfb_screen_open]Not found %s\n", ps_name);            
        return GLR_INV_ARG;
    }

    *ph_screen = GL_CLONE_HANDLE(h_scr, GLT_OBJ_SCREEN, _screen_free_fct);
       
    return GLR_OK;
}


INT32 
gl_dfb_screen_enable(HANDLE_T h_scr, BOOL b_enable)
{
    GL_DFB_SCREEN_T *   p_scr;
    INT32               i4_ret;

    p_scr = gl_dfb_handle_lock_obj(GLT_OBJ_SCREEN, h_scr, &i4_ret);
    RETURN_RET_IF_FAIL(i4_ret);

    if(p_scr->b_enabled == b_enable)
    {
        return GLR_OK;
    }

    p_scr->b_enabled = b_enable;

    if(b_enable)
    {
       /**/
    }    
    gl_dfb_obj_unlock(p_scr);
    return GLR_OK;
}

INT32 
gl_dfb_screen_get_status(
    GL_HSCREEN_T        h_screen, 
    GL_SCREEN_STATUS_T* pt_scr_status)
{
    GL_DFB_SCREEN_T *       p_scr;
#if 0
    int                   w, h;
#else
    UINT32                  w, h;
#endif
    INT32                   i4_ret;
    
    p_scr = gl_dfb_handle_lock_obj(GLT_OBJ_SCREEN, h_screen, &i4_ret); 
    RETURN_RET_IF_FAIL(i4_ret);
        /*GL_HANDLE_GET_OBJ(h_screen);*/

#if 0
	DFBCHECK(p_scr->p_dfb_screen->GetSize(p_scr->p_dfb_screen, &w, &h), &i4_ret);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_scr);
        return i4_ret;
    }
#else
	if (MTPMX_PANEL_GetResolution(&w,&h) != MTR_OK)
	{
		gl_dfb_obj_unlock(p_scr);
		return GLR_DRV_ERROR;
	}
	
#endif
    pt_scr_status->ui2_width  = w;
    pt_scr_status->ui2_height = h;
	
    pt_scr_status->b_progressive = FALSE;

    gl_dfb_obj_unlock(p_scr);
    return GLR_OK;
}

INT32
gl_dfb_plane_num(
    UINT16*             pui2_num)
{
    HANDLE_T            h_scr;
    GL_DFB_SCREEN_T *   p_scr;
    INT32               i4_ret;

    if(pui2_num == NULL)
    {
        return GLR_INV_ARG;
    }    

    gl_dfb_dy_vec_get(p_gl_dfb_root->p_hscreen_vec, 0, &h_scr);

    p_scr =  gl_dfb_handle_lock_obj(GLT_OBJ_SCREEN, h_scr, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }

    *pui2_num = _DY_VEC_COUNT(p_scr->p_hplane_vec);
    
    gl_dfb_obj_unlock(p_scr);

    return GLR_OK;
}    



INT32 
gl_dfb_plane_enum(
    GL_HSCREEN_T        h_screen, 
    UINT16              ui2_idx, 
    GL_PLANE_CAPS_T*    pt_pln_caps)
{
    /*UINT32 ui_idx;*/
    HANDLE_T h_plane;

    GL_DFB_SCREEN_T *   p_scr;
    GL_DFB_PLANE_T *    p_plane;
    
    INT32 i4_ret;

    printf("gl_dfb_plane_enum want plane %d\n", ui2_idx);
    /*PLANE_MW_IDX_2_DFB_IDX(ui2_idx);*/


    p_scr =  gl_dfb_handle_lock_obj(GLT_OBJ_SCREEN, h_screen, &i4_ret);

    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
    gl_dfb_dy_vec_get(p_scr->p_hplane_vec, ui2_idx, &h_plane);
    gl_dfb_obj_unlock(p_scr);

    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);   
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
    
    /*Fix ME: How to get hw zorder mask
     MT539x 0x8 + 0x4 + 0x2 + 0x1
     MT538x 0x7
    */        

    pt_pln_caps->ui4_z_mask = 0x8 + 0x4 + 0x2 + 0x1;

    /*Quering the cm capability interface is not found in DFB*/
    pt_pln_caps->ui4_cm_caps    = GL_CMCAP_ALL;
    pt_pln_caps->ui4_func_caps = 0;
    gl_dfb_obj_unlock(p_plane);

    return GLR_OK;
    
}


INT32 
gl_dfb_plane_open(
    GL_HSCREEN_T        h_screen, 
    UINT16              ui2_idx,
    GL_HPLANE_T*        ph_plane)
{
    GL_DFB_SCREEN_T *   p_scr;
    /*GL_DFB_PLANE_T *    p_plane;*/
    INT32               i4_ret; 
    HANDLE_T            h_plane;


    printf("gl_dfb_plane_open want plane %d\n", ui2_idx);
    /*PLANE_MW_IDX_2_DFB_IDX(ui2_idx);*/


    p_scr = gl_dfb_handle_lock_obj(GLT_OBJ_SCREEN, h_screen, &i4_ret); 
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
  
    gl_dfb_dy_vec_get(p_scr->p_hplane_vec, ui2_idx, &h_plane);

    /* MW will alloc a new handle for a plane */
    *ph_plane = gl_plane_duplicate_handle(h_plane);

    gl_dfb_obj_unlock(p_scr);

    return GLR_OK;
}

INT32 gl_dfb_plane_link(
    GL_HPLANE_T         h_plane,
    GL_HSCREEN_T        h_screen,
    BOOL                b_forced)
{
    /*
     * Cannot change the layer link with the screen.
     * Moving the data from one to other????
     * 
     * */
    return GLR_OK;
}

INT32 gl_dfb_plane_config(
    HANDLE_T            h_plane,
    GL_PLANE_CFG_T*     pt_pln_cfg)
{
    /*
     * todo:How to implement HW scaler in dfb*/
    return GLR_OK;
}


INT32 gl_dfb_plane_reorder(
    GL_HPLANE_T         h_plane, 
    UINT32              e_reorder_type, 
    INT32               i4_param)
{
#if 0
    GL_DFB_PLANE_T *    p_plane;
    int                 i4_old_z;
    INT32               i4_ret;

    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    RETURN_RET_IF_FAIL(i4_ret);

    GL_DFB_TRACE("gl_dfb_plane_reorder:PLANE %x, t:%d, o:%d\n", h_plane, e_reorder_type, i4_param);
    
    DFBCHECK(p_plane->p_dfb_layer->GetLevel(p_plane->p_dfb_layer, (int *)&i4_old_z), &i4_ret); 
   
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_plane);
        if(i4_ret == GLR_NO_IMP)
        {
             return GLR_OK;
        }
        return i4_ret;
    }

    if(e_reorder_type == GL_REORDER_ABSOLUTE)
    {
        if((int)i4_param < 0 )
        {
             i4_param = 0;
        }       
        DFBCHECK(p_plane->p_dfb_layer->SetLevel(p_plane->p_dfb_layer, i4_param), &i4_ret); 
        
        if(i4_ret != GLR_OK)
        {
            gl_dfb_obj_unlock(p_plane);
            return i4_ret;
        }   
    }
    else/*GL_REORDER_RELATIVE*/
    {
       
        DFBCHECK(p_plane->p_dfb_layer->SetLevel(p_plane->p_dfb_layer, i4_old_z + i4_param), &i4_ret);
        
        if(i4_ret != GLR_OK)
        {
            gl_dfb_obj_unlock(p_plane);
            return i4_ret;
        }
   
    }
    /*else
    {
        return GLR_INV_ARG;
    }*/
    gl_dfb_obj_unlock(p_plane);
#endif
    return GLR_OK;
}    

INT32 gl_dfb_plane_enable(
    GL_HPLANE_T         h_plane,
    BOOL                b_enable)
{
    GL_DFB_PLANE_T *            p_plane = NULL;    
    INT32                       i4_ret;
    
    if (h_plane == NULL_HANDLE)
    {
        return GLR_INV_ARG;
    }
    
    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    gl_dfb_global_lock();
    
    if (p_plane == NULL)
    {
        gl_dfb_global_unlock();
        gl_dfb_handle_unlock_obj(p_plane);
        return GLR_INV_HANDLE;
    }

    if(i4_ret != GLR_OK)
    {
        gl_dfb_global_unlock();
        gl_dfb_handle_unlock_obj(p_plane);
        return i4_ret;
    }
#if 0
    i4_ret =  _dfb_enable_layer(p_plane->p_dfb_layer, b_enable);
#else
	i4_ret =  _dfb_enable_layer(p_plane->plane_id, b_enable);
#endif
    gl_dfb_global_unlock();
    gl_dfb_handle_unlock_obj(p_plane);
    
    return i4_ret;
}
INT32 gl_dfb_plane_clear(GL_HPLANE_T         h_plane)
{
#if 1
	return GLR_OK;
#else
    GL_DFB_PLANE_T *            p_plane = NULL;
    INT32                       i4_ret;
    IDirectFBSurface *          p_dfb_surf;
    if (h_plane == NULL_HANDLE)
    {
        return GLR_INV_ARG;
    }
    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    gl_dfb_global_lock();
    if (p_plane == NULL)
    {
        gl_dfb_global_unlock();
        gl_dfb_handle_unlock_obj(p_plane);
        return GLR_INV_HANDLE;
    }
    if(i4_ret != GLR_OK)
    {
        gl_dfb_global_unlock();
        gl_dfb_handle_unlock_obj(p_plane);
        return i4_ret;
    }
    p_plane->p_dfb_layer->GetSurface(p_plane->p_dfb_layer, &p_dfb_surf);
    DFBCHECK(p_dfb_surf->SetClip(p_dfb_surf, NULL), &i4_ret);
    DFBCHECK(p_dfb_surf->Clear(
                p_dfb_surf,                
                0,
                0,
                0,
                0), &i4_ret);
    DFBCHECK(p_dfb_surf->Flip(p_dfb_surf, NULL, 0), &i4_ret);
    DFBCHECK(p_dfb_surf->ReleaseSource(p_dfb_surf),&i4_ret);
    DFBCHECK(p_dfb_surf->Release(p_dfb_surf),&i4_ret);
    gl_dfb_global_unlock();
    gl_dfb_handle_unlock_obj(p_plane);
    return i4_ret;
#endif
}
INT32 gl_dfb_plane_change_resolution(
        GL_HPLANE_T h_plane,
        UINT32      ui4_width,
        UINT32      ui4_height)
{
#if 1
	return GLR_OK;
#else
    GL_DFB_PLANE_T *            p_plane = NULL;
    INT32                       i4_ret;
    IDirectFBSurface *          p_dfb_surf;
    DFBDisplayLayerConfig       dlconfig;
    DFBDisplayLayerID       dfb_layer_id;
    if (h_plane == NULL_HANDLE)
    {
        return GLR_INV_ARG;
    }
    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    gl_dfb_global_lock();
    if (p_plane == NULL)
    {
        gl_dfb_global_unlock();
        gl_dfb_handle_unlock_obj(p_plane);
        return GLR_INV_HANDLE;
    }
    if(i4_ret != GLR_OK)
    {
        gl_dfb_global_unlock();
        gl_dfb_handle_unlock_obj(p_plane);
        return i4_ret;
    }
    p_plane->p_dfb_layer->GetConfiguration(p_plane->p_dfb_layer, &dlconfig);
    if (dlconfig.height != (INT32)ui4_height ||
        dlconfig.width  != (INT32)ui4_width)
    {
        dlconfig.flags |= DLCONF_WIDTH | DLCONF_HEIGHT;
        dlconfig.height = (INT32)ui4_height;
        dlconfig.width  = (INT32)ui4_width;
        p_plane->p_dfb_layer->SetConfiguration(p_plane->p_dfb_layer, &dlconfig);
        DFBCHECK(p_plane->p_dfb_layer->GetID(p_plane->p_dfb_layer, &dfb_layer_id), &i4_ret);
        DFBCHECK(p_plane->p_dfb_layer->GetSurface(p_plane->p_dfb_layer, &p_dfb_surf), &i4_ret);
        DFBCHECK(p_dfb_surf->SetClip(p_dfb_surf, NULL), &i4_ret);
        DFBCHECK(p_dfb_surf->Clear(p_dfb_surf, 0, 0, 0, 0), &i4_ret);
        DFBCHECK(p_dfb_surf->ReleaseSource(p_dfb_surf),&i4_ret);
        DFBCHECK(p_dfb_surf->Release(p_dfb_surf),&i4_ret);
        _dfb_enable_osd_scaler(dfb_layer_id,
                               TRUE,
                               ui4_width,
                               ui4_height);
    }
    gl_dfb_global_unlock();
    gl_dfb_handle_unlock_obj(p_plane);
    return i4_ret;
#endif
}

INT32 gl_dfb_plane_change_resolution_cm(
        GL_HPLANE_T    h_plane,
        UINT32         ui4_width,
        UINT32         ui4_height,
        GL_COLORMODE_T e_cm)
{
#if 1
	return GLR_OK;
#else
    GL_DFB_PLANE_T *            p_plane = NULL;
    INT32                       i4_ret;
    IDirectFBSurface *          p_dfb_surf;
    DFBDisplayLayerConfig       dlconfig;
    DFBDisplayLayerID           dfb_layer_id;

    DFBSurfacePixelFormat       px_format = gl_dfb_cm_map_get(e_cm);
    
    if (h_plane == NULL_HANDLE)
    {
        return GLR_INV_ARG;
    }
    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    gl_dfb_global_lock();
    if (p_plane == NULL)
    {
        gl_dfb_global_unlock();
        gl_dfb_handle_unlock_obj(p_plane);
        return GLR_INV_HANDLE;
    }
    if(i4_ret != GLR_OK)
    {
        gl_dfb_global_unlock();
        gl_dfb_handle_unlock_obj(p_plane);
        return i4_ret;
    }
    p_plane->p_dfb_layer->GetConfiguration(p_plane->p_dfb_layer, &dlconfig);
    if (dlconfig.height != (INT32)ui4_height ||
        dlconfig.width  != (INT32)ui4_width  ||
        dlconfig.pixelformat != px_format)
    {
    
        dlconfig.flags |= DLCONF_WIDTH | DLCONF_HEIGHT | DLCONF_PIXELFORMAT;
        dlconfig.height = (INT32)ui4_height;
        dlconfig.width  = (INT32)ui4_width;
        dlconfig.pixelformat = px_format;
        
        p_plane->p_dfb_layer->SetConfiguration(p_plane->p_dfb_layer, &dlconfig);
        DFBCHECK(p_plane->p_dfb_layer->GetID(p_plane->p_dfb_layer, &dfb_layer_id), &i4_ret);
        DFBCHECK(p_plane->p_dfb_layer->GetSurface(p_plane->p_dfb_layer, &p_dfb_surf), &i4_ret);
        DFBCHECK(p_dfb_surf->SetClip(p_dfb_surf, NULL), &i4_ret);
        DFBCHECK(p_dfb_surf->Clear(p_dfb_surf, 0, 0, 0, 0), &i4_ret);
        DFBCHECK(p_dfb_surf->ReleaseSource(p_dfb_surf),&i4_ret);
        DFBCHECK(p_dfb_surf->Release(p_dfb_surf),&i4_ret);
        _dfb_enable_osd_scaler(dfb_layer_id,
                               TRUE,
                               ui4_width,
                               ui4_height);
    }
    gl_dfb_global_unlock();
    gl_dfb_handle_unlock_obj(p_plane);
    return i4_ret;
#endif
}




INT32 gl_dfb_plane_get_capability(
    GL_HPLANE_T         h_plane, 
    GL_HSCREEN_T        h_screen,
    GL_PLANE_CAPS_T*    pt_pln_caps)
{
    GL_DFB_PLANE_T *    p_plane;
    /*UINT32  ui4_old_z;*/
    INT32               i4_ret;    

    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    if(i4_ret != GLR_OK)
    {  
        return i4_ret;
    }
    
    /*Fix me :how to get zmask*/
    pt_pln_caps->ui4_z_mask = 0x8 + 0x4 + 0x2 + 0x1;
   
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_plane);
        return i4_ret;
    }    
        
    /*Quering the cm capability interface is not found in DFB*/
    pt_pln_caps->ui4_cm_caps    = GL_CMCAP_ALL;
    pt_pln_caps->ui4_func_caps = 0;
    
    gl_dfb_obj_unlock(p_plane); 
    return GLR_OK;
}

INT32 gl_dfb_plane_get_linked_screen(
    GL_HPLANE_T         h_plane, 
    x_gl_nfy_fct        pf_screen_nfy,
    VOID*               pv_nfy_tag,
    GL_HSCREEN_T*       ph_screen)
{
    GL_DFB_PLANE_T *    p_plane;
    INT32               i4_ret;

    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }

    *ph_screen = p_plane->h_scr;
    
    gl_dfb_obj_unlock(p_plane);

    return GLR_OK;    
}

INT32 gl_dfb_plane_get_order(
    GL_HPLANE_T         h_plane, 
    INT32               *pi4_order)
{
    GL_DFB_PLANE_T *            p_plane = NULL;    
    INT32                       i4_z = 1;
    INT32                       i4_ret;

    if (pi4_order == NULL || h_plane == NULL_HANDLE)
    {
        return GLR_INV_ARG;
    }

    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);

    if (p_plane == NULL)
    {
        return GLR_INV_HANDLE;
    }

    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
#if 0
    DFBCHECK(p_plane->p_dfb_layer->GetLevel(p_plane->p_dfb_layer, (int *)&i4_z), &i4_ret); 
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_plane);
        return i4_ret;
    }
#endif 
    gl_dfb_obj_unlock(p_plane);
    
    *pi4_order = i4_z;
    
    return GLR_OK;
}

INT32 gl_dfb_plane_get_status(
    GL_HPLANE_T         h_plane, 
    GL_PLANE_STATUS_T*  pt_pln_sts)
{
    GL_DFB_PLANE_T *            p_plane;    
    INT32                       i4_ret;

#if 0

    DFBDisplayLayerDescription  dfb_desc;

    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }

    DFBCHECK(p_plane->p_dfb_layer->GetDescription(p_plane->p_dfb_layer, &dfb_desc), &i4_ret);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_plane);
        return i4_ret;
    }
 
    gl_dfb_obj_unlock(p_plane);
    
    x_strcpy(pt_pln_sts->s_screen_name,     dfb_desc.name);
    pt_pln_sts->ui1_z                   = dfb_desc.level;
#else
	p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
	
	gl_dfb_screen_get_name(p_plane->h_scr, (CHAR**)(&(pt_pln_sts->s_screen_name))); 

	gl_dfb_obj_unlock(p_plane);
	
	pt_pln_sts->ui1_z                   = 1;
#endif

    pt_pln_sts->ui1_alpha               = p_plane->ui1_alpha;

    GL_MEMSET(&(pt_pln_sts->t_pln_cfg),  0, sizeof(GL_PLANE_CFG_T));
    
    return GLR_OK;
}

INT32 
gl_dfb_plane_set_alpha(
    GL_HPLANE_T h_plane, 
    UINT8       ui1_alpha)
{
    GL_DFB_PLANE_T *    p_plane;
    INT32               i4_ret;

#if 0
    DFBDisplayLayerConfig   t_config;
    
    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
    GL_DFB_TRACE("[GL_DFB] Plane set alpha %x\n", ui1_alpha); 

    if (p_plane->p_wnd != NULL)
    {
        DFBCHECK(p_plane->p_wnd->SetOpacity(p_plane->p_wnd, ui1_alpha), &i4_ret);
    }
    else
    {
        DFBCHECK(p_plane->p_dfb_layer->GetConfiguration(p_plane->p_dfb_layer, &t_config), &i4_ret);

        if (!(t_config.options & DLOP_OPACITY))
        {
            t_config.options |= DLOP_OPACITY;
            DFBCHECK(p_plane->p_dfb_layer->SetConfiguration(p_plane->p_dfb_layer, &t_config), &i4_ret);
        }
        
        DFBCHECK(p_plane->p_dfb_layer->SetOpacity(p_plane->p_dfb_layer, ui1_alpha), &i4_ret);
    }
    if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_plane);
        return i4_ret;
    }
    p_plane->ui1_alpha = ui1_alpha;
    
    gl_dfb_obj_unlock(p_plane);
#else
	p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }

	if (p_plane->ui1_alpha != ui1_alpha)
	{
		gl_dfb_obj_unlock(p_plane);
        return GLR_OK;
	}
    GL_DFB_TRACE("[GL_DFB] Plane set alpha %x\n", ui1_alpha); 
	
	i4_ret = MTOSD_PLA_SetFading(p_plane->plane_id ,ui1_alpha);
	
	if(i4_ret != GLR_OK)
    {
        gl_dfb_obj_unlock(p_plane);
        return i4_ret;
    }
    p_plane->ui1_alpha = ui1_alpha;
    
    gl_dfb_obj_unlock(p_plane);
#endif
    return GLR_OK;
}

INT32 gl_dfb_plane_enable_colorkey(
	GL_HPLANE_T         h_plane,
    BOOL                b_new_enable,
    BOOL*               old_enable_fg)
{
#if 0
	GL_DFB_PLANE_T *	p_plane;
	INT32				i4_ret = GLR_OK;
	DFBDisplayLayerConfig	t_config;
	
	p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
	if(i4_ret != GLR_OK)
	{
		return i4_ret;
	}
	
	GL_DFB_TRACE("[GL_DFB] Plane set alpha %x\n", ui1_alpha); 

	if (p_plane->p_wnd == NULL)
	{
		
		DFBCHECK(p_plane->p_dfb_layer->GetConfiguration(p_plane->p_dfb_layer, &t_config), &i4_ret);

		if (t_config.options & DLOP_SRC_COLORKEY)
		{
			*old_enable_fg = TRUE;
			if (!b_new_enable)
			{
				t_config.options &= ~DLOP_SRC_COLORKEY;
				t_config.flags  = DLCONF_OPTIONS;
				DFBCHECK(p_plane->p_dfb_layer->SetConfiguration(p_plane->p_dfb_layer, &t_config), &i4_ret);
			}
		}
		else
		{
			*old_enable_fg = FALSE;
			if (b_new_enable)
			{
				t_config.options |= DLOP_SRC_COLORKEY;
				t_config.flags  = DLCONF_OPTIONS;
				DFBCHECK(p_plane->p_dfb_layer->SetConfiguration(p_plane->p_dfb_layer, &t_config), &i4_ret);
			}
		}
		
	}
	
	if(i4_ret != GLR_OK)
	{
		gl_dfb_obj_unlock(p_plane);
		return i4_ret;
	}
	
	gl_dfb_obj_unlock(p_plane);
#else
	*old_enable_fg = FALSE;
#endif
	return GLR_OK;
}
#ifdef MW_PHOTO_SHOW_ON_VDP


INT32 gl_dfb_plane_set_vdp_cropping(
    GL_HPLANE_T h_plane, 
    BOOL        b_3d_mode,
    UINT32      ui4_bottom,
    UINT32      ui4_top,
    UINT32      ui4_left,
    UINT32      ui4_right)
{
    GL_DFB_PLANE_T *        p_plane;
    INT32                   i4_ret = GLR_OK;
  //  DFBDisplayLayerConfig   t_config;
    DFBDisplayLayerID       dfb_layer_id;
    mt53fb_vdp_crop_rect    vdp_crop_rect;
    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
    GL_DFB_TRACE("[GL_DFB] Plane set alpha %x\n", ui1_alpha); 

    if (p_plane->p_wnd == NULL)
    {
        DFBCHECK(p_plane->p_dfb_layer->GetID(p_plane->p_dfb_layer, &dfb_layer_id), &i4_ret);
        if (dfb_layer_id == 2)
        {
            //DFBCHECK(p_plane->p_dfb_layer->GetConfiguration(p_plane->p_dfb_layer, &t_config), &i4_ret);
            //t_config.flags |= DLCONF_OPTIONS | DLCONF_3D_CROP_OFFSET ;
            if (b_3d_mode)
            {
                vdp_crop_rect.ui4_3d_mode = DLOP_3D_MODE;
            }
            else
            {
                vdp_crop_rect.ui4_3d_mode = 0;
            }
			
            vdp_crop_rect.ui4_layer          = dfb_layer_id;
            vdp_crop_rect.crop_bottom_offset = ui4_bottom;
            vdp_crop_rect.crop_top_offset    = ui4_top;
            vdp_crop_rect.crop_left_offset   = ui4_left;
            vdp_crop_rect.crop_right_offset  = ui4_right;
            
            vdpSetCropRect(&vdp_crop_rect);
			
        //    DFBCHECK(p_plane->p_dfb_layer->SetConfiguration(p_plane->p_dfb_layer, &t_config), &i4_ret);
        }
    }
    
    gl_dfb_obj_unlock(p_plane);

    return i4_ret;
}


INT32 gl_dfb_plane_get_resource(HANDLE_T h_plane, HANDLE_T h_on_screen_surf)
{
    GL_DFB_PLANE_T *    p_plane;
    INT32               i4_ret = GLR_OK;
    DFBDisplayLayerConfig   t_config;
    GL_DFB_SCREEN_T *       p_scr;
    INT32                   i4_screen_width = 0, i4_screen_height = 0;
    IDirectFBSurface *      p_new_dfb_surf;
    GL_DFB_SURFACE_T *      p_surf;
    
    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }

    if (p_plane->p_wnd == NULL && p_plane->p_dfb_layer == NULL)
    {

        DFBCHECK(g_p_dfb->GetDisplayLayer(g_p_dfb, 2, &p_plane->p_dfb_layer), &i4_ret);
        p_plane->p_dfb_layer->SetCooperativeLevel(p_plane->p_dfb_layer, DFSCL_EXCLUSIVE);

        p_scr = (GL_DFB_SCREEN_T *)gl_dfb_handle_get_obj(GLT_OBJ_SCREEN,p_plane->h_scr);
        
    	DFBCHECK(p_scr->p_dfb_screen->GetSize(p_scr->p_dfb_screen, (int*)&i4_screen_width, (int*)&i4_screen_height), &i4_ret);
    	
        t_config.flags       = DLCONF_PIXELFORMAT|DLCONF_BUFFERMODE| DLCONF_WIDTH | DLCONF_HEIGHT;
        t_config.pixelformat = DSPF_NV16; /*Default pixelformat*/
        t_config.buffermode  = DLBM_FRONTONLY;
    	t_config.width = i4_screen_width;
    	t_config.height= i4_screen_height;
        DFBCHECK(p_plane->p_dfb_layer->SetConfiguration(p_plane->p_dfb_layer, &t_config), &i4_ret);
        DFBCHECK(p_plane->p_dfb_layer->GetSurface(p_plane->p_dfb_layer, &p_new_dfb_surf), &i4_ret);
        DFBCHECK(p_new_dfb_surf->Clear(p_new_dfb_surf, 0, 0, 0, 0), &i4_ret);
        if (h_on_screen_surf == NULL_HANDLE)
        {
            DFBCHECK(p_new_dfb_surf->ReleaseSource(p_new_dfb_surf),&i4_ret);
            DFBCHECK(p_new_dfb_surf->Release(p_new_dfb_surf),&i4_ret);
        }
        else
        {
            p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_on_screen_surf, &i4_ret);
            p_surf->p_dfb_surf = p_new_dfb_surf;
            gl_dfb_handle_unlock_obj(p_surf);
        }
    }
    
    gl_dfb_obj_unlock(p_plane);

    return i4_ret;
}

INT32 gl_dfb_plane_release_resource(HANDLE_T h_plane,HANDLE_T h_on_screen_surf)
{
    GL_DFB_PLANE_T *    p_plane;
    INT32               i4_ret = GLR_OK;
    GL_DFB_SURFACE_T *      p_surf;
    
    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }

    if (p_plane->p_wnd == NULL && p_plane->p_dfb_layer != NULL)
    {
        if (h_on_screen_surf != NULL_HANDLE)
        {
            p_surf  = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_on_screen_surf, &i4_ret);
            DFBCHECK(p_surf->p_dfb_surf->ReleaseSource(p_surf->p_dfb_surf), &i4_ret);
            DFBCHECK(p_surf->p_dfb_surf->Release(p_surf->p_dfb_surf), &i4_ret);
            p_surf->p_dfb_surf = NULL;
            gl_dfb_handle_unlock_obj(p_surf);
        }
        DFBCHECK(p_plane->p_dfb_layer->Release(p_plane->p_dfb_layer), &i4_ret);
        p_plane->p_dfb_layer = NULL;
    }
    
    gl_dfb_obj_unlock(p_plane);

    return i4_ret;
}

#endif
static INT32 
_plane_detach_viewport_strg(GL_DFB_PLANE_T * p_plane, GL_DFB_VIEWPORT_T * p_viewport)
{
   /* DFBSurfaceDescription  t_sdsc;*/
    GL_DFB_SURFACE_T *  p_surf; 
    INT32               i4_ret;
    
    if(p_viewport->h_surf == NULL_HANDLE)
    {
        return GLR_OK;
    }

    /*t_sdsc.flags = DSDESC_WIDTH|DSDESC_HEIGHT;
    t_sdsc.width = p_viewport->t_disp_opt.t_size.ui4_width;
    t_sdsc.width = p_viewport->t_disp_opt.t_size.ui4_height;*/
    
    p_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, p_viewport->h_surf, &i4_ret);/*GL_HANDLE_GET_OBJ(p_viewport->h_surf);*/
    if(i4_ret != GLR_OK)
    {
        return i4_ret;
    }
    
    if(p_surf->b_from_fbm)
    {
        gl_dfb_surface_rebuild(p_surf, NULL, GL_DFB_SURF_STRG_OFF_SCR_FROM_FBM);
    }
#ifdef MW_PHOTO_SHOW_ON_VDP
    else if (p_surf->b_from_vdp)
    {
        gl_dfb_surface_rebuild(p_surf,NULL,GL_DFB_SURF_STRG_OFF_SCR_FROM_VDP);
    }
#endif
    else
    {
        gl_dfb_surface_rebuild(p_surf, NULL, GL_DFB_SURF_STRG_OFF_SCR);
    }

    gl_dfb_obj_unlock(p_surf);
    
    p_viewport->pv_dfb_strg = NULL;
    
    return GLR_OK;
}

static INT32
_plane_invalidate_viewports(GL_DFB_PLANE_T * p_plane)
{
    GL_DFB_VIEW_T * p_view;
    HANDLE_T * ph_view;
    GL_DFB_VIEWPORT_T ** pp_viewport;
    /*GL_DFB_SURFACE_T * p_surf;*/
    UINT32 i, j;
    INT32 i4_ret;

    /*DFBSurfaceDescription  t_sdsc;*/
    GL_DFB_TRACE("Invalidate viewports:%x\n", p_plane);
    GL_DFB_DY_VEC_FOR_EACH(p_plane->p_hview_vec, i, ph_view)
    {    
        p_view = gl_dfb_handle_lock_obj(GLT_OBJ_VIEW, *ph_view, &i4_ret);
        RETURN_RET_IF_FAIL(i4_ret);
        
        p_view->h_plane = NULL_HANDLE;
        GL_DFB_DY_VEC_FOR_EACH(p_view->p_viewport_vec, j, pp_viewport)
        {            
            _plane_detach_viewport_strg(p_plane, *pp_viewport);
        }
        gl_dfb_obj_unlock(p_view);
    }
    while(gl_dfb_dy_vec_remove(p_plane->p_hview_vec, 0) == TRUE);
    
    return GLR_OK;
}

static INT32 _plane_update_viewports(GL_DFB_PLANE_T * p_plane)
{
    GL_DFB_VIEW_T *         p_view;
    GL_DFB_VIEWPORT_T **    pp_viewport;
    GL_DFB_VIEWPORT_T *     p_viewport;
    GL_DFB_SURFACE_T *      p_surf;
    HANDLE_T *              ph_view;
    UINT32                  i, j;
    INT32                   i4_ret; 
    BOOL                    fg_enable_plane = FALSE;
    
#if 0
    DFBDisplayLayerConfig   dfb_cfg;
    DFBDisplayLayerID       dfb_layer_id;
    /* We just disable real layer plane.*/
    if(p_plane->p_wnd == NULL)
    {
        if(p_plane->p_hview_vec->z_count == 0)
        {
            _dfb_enable_layer(p_plane->p_dfb_layer, FALSE);
        }
        else
        {
            fg_enable_plane = TRUE;
        }
    }
    
    GL_DFB_DY_VEC_FOR_EACH(p_plane->p_hview_vec, i, ph_view)
    {   
        p_view = gl_dfb_handle_lock_obj(GLT_OBJ_VIEW, *ph_view, &i4_ret);
        RETURN_RET_IF_FAIL(i4_ret);
        
        GL_DFB_DY_VEC_FOR_EACH(p_view->p_viewport_vec, j, pp_viewport)
        {            
            DFBDisplayLayerConfig   t_config;
            
            p_viewport = *pp_viewport;
            if(p_viewport->h_surf == NULL_HANDLE)
            {
            
                continue;
            }
            p_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, p_viewport->h_surf, &i4_ret);
            if(i4_ret != GLR_OK)
            {
                gl_dfb_obj_unlock(p_view);
                return i4_ret;
            }

            
            dfb_cfg.flags = DLCONF_OPTIONS|DLCONF_PIXELFORMAT|DLCONF_WIDTH|DLCONF_HEIGHT;
            dfb_cfg.options = 0;
            if(p_viewport->t_disp_opt.e_flag & GL_DISP_OPT_COLORKEY)
            {
                 dfb_cfg.options |= DLOP_SRC_COLORKEY;
            }
            if(p_viewport->t_disp_opt.e_flag & GL_DISP_OPT_BLEND_OPTION)
            {
                if(p_viewport->t_disp_opt.e_blend_option == GL_BLEND_OPT_PLANE)
                {
                    dfb_cfg.options |= DLOP_OPACITY;
                }
                else if(p_viewport->t_disp_opt.e_blend_option == GL_BLEND_OPT_PIXEL)
                {
                    dfb_cfg.options |= DLOP_OPACITY | DLOP_ALPHACHANNEL;
                }
                else
                {
                    dfb_cfg.options |= DLOP_NONE;        
                }
            }
            dfb_cfg.pixelformat = gl_dfb_cm_map_get(p_surf->t_desc.e_clrmode);
            dfb_cfg.width       = p_surf->t_desc.ui4_width;
            dfb_cfg.height      = p_surf->t_desc.ui4_height;

            DFBCHECK(p_plane->p_dfb_layer->GetID(p_plane->p_dfb_layer, &dfb_layer_id), &i4_ret);
            
            DFBCHECK(p_plane->p_dfb_layer->GetConfiguration(p_plane->p_dfb_layer, &t_config), &i4_ret);

            if (dfb_layer_id != DLID_PRIMARY && i4_ret == GLR_OK && p_viewport->t_disp_opt.e_flag & GL_DISP_OPT_POSITION)
            {
                DFBCHECK(p_plane->p_dfb_layer->SetScreenPosition(
                    p_plane->p_dfb_layer,              
                    p_viewport->t_disp_opt.t_pos.i4_x,
                    p_viewport->t_disp_opt.t_pos.i4_y), &i4_ret);

                /* layer ID should be the same as the scaler ID */
                _dfb_enable_osd_scaler(
                    (UINT32)dfb_layer_id,
                    p_view->b_auto_scale,
                    (UINT32)p_viewport->t_disp_opt.t_size.ui4_width,
                    (UINT32)p_viewport->t_disp_opt.t_size.ui4_height);
            }

            if ((dfb_layer_id != DLID_PRIMARY) && i4_ret == GLR_OK && 
                (t_config.pixelformat != dfb_cfg.pixelformat ||
                 t_config.width       != dfb_cfg.width       ||
                 t_config.height      != dfb_cfg.height))
            {
                DFBCHECK(p_plane->p_dfb_layer->SetConfiguration(p_plane->p_dfb_layer, &dfb_cfg), &i4_ret);

                if(p_viewport->t_disp_opt.e_flag & GL_DISP_OPT_COLORKEY)
                {
                    printf(" Set color key to [%d,%d,%d]\n", p_viewport->t_disp_opt.t_colorkey.u1.r,
                            p_viewport->t_disp_opt.t_colorkey.u2.g,
                            p_viewport->t_disp_opt.t_colorkey.u3.b);
                    DFBCHECK(p_plane->p_dfb_layer->SetSrcColorKey(
                            p_plane->p_dfb_layer,
                            p_viewport->t_disp_opt.t_colorkey.u1.r,
                            p_viewport->t_disp_opt.t_colorkey.u2.g,
                            p_viewport->t_disp_opt.t_colorkey.u3.b), &i4_ret);
                    if(p_plane->p_wnd != NULL)
                    {
                        DFBCHECK(p_plane->p_wnd->SetColorKey(
                            p_plane->p_wnd,
                            p_viewport->t_disp_opt.t_colorkey.u1.r,
                            p_viewport->t_disp_opt.t_colorkey.u2.g,
                            p_viewport->t_disp_opt.t_colorkey.u3.b), &i4_ret);
                    }
                }
                /*
                 * TODO: For window, we may need to change the size?
                 */
                if((p_viewport->t_disp_opt.e_flag & GL_DISP_OPT_POSITION) &&
                   (p_viewport->t_disp_opt.e_flag & GL_DISP_OPT_RESIZE))
                {            
                    
                    DFBCHECK(p_plane->p_dfb_layer->SetScreenRectangle(
                            p_plane->p_dfb_layer,              
                            p_viewport->t_disp_opt.t_pos.i4_x,
                            p_viewport->t_disp_opt.t_pos.i4_y,
                            p_viewport->t_disp_opt.t_size.ui4_width,
                            p_viewport->t_disp_opt.t_size.ui4_height), &i4_ret);
                    if(p_plane->p_wnd != NULL)
                    {
                        DFBCHECK(p_plane->p_wnd->SetBounds(
                            p_plane->p_wnd,
                            p_viewport->t_disp_opt.t_pos.i4_x,
                            p_viewport->t_disp_opt.t_pos.i4_y,
                            p_surf->t_desc.ui4_width,
                            p_surf->t_desc.ui4_height), &i4_ret);
                    }
                }            
                else if(p_viewport->t_disp_opt.e_flag & GL_DISP_OPT_POSITION)
                {               
                    DFBCHECK(p_plane->p_dfb_layer->SetScreenPosition(
                            p_plane->p_dfb_layer,              
                            p_viewport->t_disp_opt.t_pos.i4_x,
                            p_viewport->t_disp_opt.t_pos.i4_y), &i4_ret);
                    if(p_plane->p_wnd != NULL)
                    {
                        DFBCHECK(p_plane->p_wnd->MoveTo(
                            p_plane->p_wnd,
                            p_viewport->t_disp_opt.t_pos.i4_x,
                            p_viewport->t_disp_opt.t_pos.i4_y), &i4_ret);
                    }
                }            
                else if(p_viewport->t_disp_opt.e_flag & GL_DISP_OPT_RESIZE)
                {               
                    int i4_old_x;
                    int i4_old_y;
                    /*printf(" Set size to [%d,%d]\n", 
                            p_viewport->t_disp_opt.t_size.ui4_width,
                            p_viewport->t_disp_opt.t_size.ui4_height);*/
                    /*FIXME: How to get old layer position?*/
                    i4_old_x = 0;
                    i4_old_y = 0;
                    DFBCHECK(p_plane->p_dfb_layer->SetScreenRectangle(
                            p_plane->p_dfb_layer,              
                            i4_old_x,
                            i4_old_y,
                            p_viewport->t_disp_opt.t_size.ui4_width,
                            p_viewport->t_disp_opt.t_size.ui4_height), &i4_ret);
                    if(p_plane->p_wnd != NULL)
                    {   
                        DFBCHECK(p_plane->p_wnd->GetPosition(
                            p_plane->p_wnd, &i4_old_x, &i4_old_y), &i4_ret);
                        DFBCHECK(p_plane->p_wnd->SetBounds(
                            p_plane->p_wnd,
                            i4_old_x,
                            i4_old_y,
                            p_surf->t_desc.ui4_width,
                            p_surf->t_desc.ui4_height), &i4_ret);
                    }
                }    
            }            
            
            if(p_plane->p_wnd != NULL)
            {            
                p_viewport->pv_dfb_strg = p_plane->p_wnd;
                gl_dfb_surface_rebuild(p_surf, p_viewport->pv_dfb_strg, GL_DFB_SURF_STRG_WND);
            }
            else
            {
                p_viewport->pv_dfb_strg = p_plane->p_dfb_layer;
                gl_dfb_surface_rebuild(p_surf, p_viewport->pv_dfb_strg, GL_DFB_SURF_STRG_LAYER);
          
            }            
            gl_dfb_obj_unlock(p_surf);           
        }
        gl_dfb_obj_unlock(p_view);
    }
#else
	if(p_plane->p_hview_vec->z_count == 0)
    {
        _dfb_enable_layer(p_plane->plane_id, TRUE);
	}
    else
    {
        fg_enable_plane = TRUE;
    }

	GL_DFB_DY_VEC_FOR_EACH(p_plane->p_hview_vec, i, ph_view)
    {   
        p_view = gl_dfb_handle_lock_obj(GLT_OBJ_VIEW, *ph_view, &i4_ret);
        RETURN_RET_IF_FAIL(i4_ret);
        
        GL_DFB_DY_VEC_FOR_EACH(p_view->p_viewport_vec, j, pp_viewport)
        {   
            p_viewport = *pp_viewport;
            if(p_viewport->h_surf == NULL_HANDLE)
            {
                continue;
            }
            p_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, p_viewport->h_surf, &i4_ret);
            if(i4_ret != GLR_OK)
            {
                gl_dfb_obj_unlock(p_view);
                return i4_ret;
            }

            p_viewport->pv_dfb_strg = (VOID*)p_plane->plane_id;
            gl_dfb_surface_rebuild(p_surf, p_viewport->pv_dfb_strg, GL_DFB_SURF_STRG_LAYER);
      
            gl_dfb_obj_unlock(p_surf);           
        }
        gl_dfb_obj_unlock(p_view);
    }
#endif

    if(fg_enable_plane)
    {
#if 0
        _dfb_enable_layer(p_plane->p_dfb_layer, TRUE);
#else
		_dfb_enable_layer(p_plane->plane_id, TRUE);
#endif
    }
    
    return GLR_OK;
}

static BOOL
_view_free_fct(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle)
{
    GL_DFB_VIEW_T *         p_view; 
    GL_DFB_VIEWPORT_T **    pp_viewport;
    GL_DFB_PLANE_T *        p_plane;
    UINT32                  ui4_idx;
    HANDLE_T                h_plane;

    INT32 i4_ret;
    p_view = pv_obj;
    
    GL_DFB_BEGIN_FREE(h_handle, pv_obj)
        h_plane = p_view->h_plane;
        if(h_plane!= NULL_HANDLE)
        {
            p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);            
            GL_DFB_DY_VEC_FOR_EACH(p_view->p_viewport_vec, ui4_idx, pp_viewport)
            {
                _plane_detach_viewport_strg(p_plane, *pp_viewport);
                GL_FREE(*pp_viewport);
            }
            gl_dfb_dy_vec_find(p_plane->p_hview_vec, &h_handle,  &ui4_idx);
            gl_dfb_dy_vec_remove(p_plane->p_hview_vec, ui4_idx);
            gl_dfb_obj_unlock(p_plane);
        }
        gl_dfb_dy_vec_destroy(p_view->p_viewport_vec);
        gl_dfb_obj_uninit(pv_obj);
        GL_FREE(pv_obj);
    GL_DFB_END_FREE(h_handle, pv_obj)
    return TRUE;
}
INT32 gl_dfb_view_create(
    UINT32                  ui4_w,
    UINT32                  ui4_h,
    BOOL                    b_auto_scale,
    GL_HVIEW_T*             ph_view)
{
    GL_DFB_VIEW_T * p_view;


    p_view = (GL_DFB_VIEW_T *)GL_MALLOC(sizeof(GL_DFB_VIEW_T));
    GL_MEMSET(p_view, 0, sizeof(GL_DFB_VIEW_T));
    gl_dfb_obj_init(GLT_OBJ_VIEW, p_view);

    p_view->ui4_w           = ui4_w;
    p_view->ui4_h           = ui4_h;
    p_view->b_auto_scale    = b_auto_scale;
    p_view->p_viewport_vec  = gl_dfb_dy_vec_init(4, sizeof(GL_DFB_VIEWPORT_T *));
    p_view->h_plane         = NULL_HANDLE;
    
    *ph_view = GL_ALLOC_HANDLE(GLT_OBJ_VIEW, p_view, NULL, _view_free_fct);

    GL_DFB_TRACE("Created view :%x\n", *ph_view);

    return GLR_OK;
}    

INT32 gl_dfb_view_create_viewport(
    GL_HVIEW_T              h_view, 
    GL_HSURFACE_T           h_surface, 
    const GL_DISPLAY_OPT_T* pt_disp_opt,
    UINT32*                 pui4_id)
{
    GL_DFB_VIEW_T *     p_view;
    GL_DFB_VIEWPORT_T * p_viewport;
    GL_DFB_PLANE_T *    p_plane;
    INT32               i4_ret;
    HANDLE_T            h_plane;   
    UINT32              ui4_idx;
    
    GL_DFB_TRACE_API("[GL_DFB]Create viewport with surf %x\n", h_surface);
            
    p_view = gl_dfb_handle_lock_obj(GLT_OBJ_VIEW, h_view, &i4_ret);
    RETURN_RET_IF_FAIL(i4_ret);
 
    h_plane = p_view->h_plane;
    gl_dfb_obj_unlock(p_view);

    
    p_viewport = (GL_DFB_VIEWPORT_T *)GL_MALLOC(sizeof(GL_DFB_VIEWPORT_T));
    GL_MEMSET(p_viewport, 0, sizeof(GL_DFB_VIEWPORT_T));
    
    p_viewport->h_view      = h_view;
    p_viewport->h_surf      = h_surface;
    p_viewport->pv_dfb_strg = NULL;

    GL_MEMCPY(&(p_viewport->t_disp_opt), pt_disp_opt, sizeof(GL_DISPLAY_OPT_T));

    GL_DFB_TRACE("***********Create viewport disp opt:w %d,h %d, sopt:%d\n", 
                p_viewport->t_disp_opt.t_size.ui4_width, 
                p_viewport->t_disp_opt.t_size.ui4_height,
                pt_disp_opt->e_shrink_opt);

    /*
     * In MW GL the shrink opts are depending on HW driver.
     */
    if(pt_disp_opt->e_shrink_opt == GL_SHRINK_SURFACE)
    {        
        GL_SURF_INFO_T  t_info;

        if (gl_dfb_surface_get_info(h_surface, &t_info) == GLR_OK)
        {
            p_viewport->t_disp_opt.t_size.ui4_width  = t_info.ui4_width;
            p_viewport->t_disp_opt.t_size.ui4_height = t_info.ui4_height;
        }
    }
        
    if(h_plane != NULL_HANDLE)
    {
        p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
        RETURN_RET_IF_FAIL(i4_ret);
        
        p_view = gl_dfb_handle_lock_obj(GLT_OBJ_VIEW, h_view, &i4_ret);
        if(i4_ret != GLR_OK)
        {
            gl_dfb_obj_unlock(p_plane);
            return i4_ret;
        }
        
        if(gl_dfb_dy_vec_find(p_plane->p_hview_vec, &h_view, &ui4_idx) != FALSE)
        {
            /* Detach current views */
            _plane_invalidate_viewports(p_plane);
            
            gl_dfb_dy_vec_add(p_view->p_viewport_vec, &p_viewport);                
            
            _plane_update_viewports(p_plane);            
        }
        else
        {
            gl_dfb_dy_vec_add(p_view->p_viewport_vec, &p_viewport);
        }
        gl_dfb_obj_unlock(p_view);
        gl_dfb_obj_unlock(p_plane);
    }
    else
    {
        p_view = gl_dfb_handle_lock_obj(GLT_OBJ_VIEW, h_view, &i4_ret);
        RETURN_RET_IF_FAIL(i4_ret);
        gl_dfb_dy_vec_add(p_view->p_viewport_vec, &p_viewport);
        gl_dfb_obj_unlock(p_view);        
    }
    GL_DFB_TRACE("Created viewport :%x at view:%x\n", p_viewport, h_view);
    return GLR_OK;
}    

    
INT32 gl_dfb_view_delete_viewport(
    GL_HVIEW_T      h_view,
    UINT32          ui4_id)
{
    GL_DFB_VIEW_T *     p_view;
    GL_DFB_VIEWPORT_T * p_viewport = NULL;    
    GL_DFB_PLANE_T  *   p_plane;
    HANDLE_T            h_plane;
    UINT32              ui4_idx;
    INT32               i4_ret;
    
    GL_DFB_TRACE("[GL_DFB]Delete viewport  %x, id:%d\n", h_view,ui4_id);
    
    p_view = gl_dfb_handle_lock_obj(GLT_OBJ_VIEW, h_view, &i4_ret);
    RETURN_RET_IF_FAIL(i4_ret);

    if(gl_dfb_dy_vec_get(p_view->p_viewport_vec, ui4_id, &p_viewport) == FALSE)
    {
         gl_dfb_obj_unlock(p_view);
         return GLR_INV_ARG;
    }    
    h_plane = p_view->h_plane;
    gl_dfb_obj_unlock(p_view);

    if(h_plane != NULL_HANDLE)
    {
        p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
        RETURN_RET_IF_FAIL(i4_ret);
        
        p_view = gl_dfb_handle_lock_obj(GLT_OBJ_VIEW, h_view, &i4_ret);
        if(i4_ret != GLR_OK)
        {
            gl_dfb_obj_unlock(p_plane);
            return i4_ret;
        }
        
        if(gl_dfb_dy_vec_find(p_plane->p_hview_vec, &h_view, &ui4_idx) != FALSE)
        {
            /* Detach current views */
            _plane_invalidate_viewports(p_plane);
            
            gl_dfb_dy_vec_remove(p_view->p_viewport_vec, ui4_id);
            
            _plane_update_viewports(p_plane);            
        }
        else
        {
            gl_dfb_dy_vec_remove(p_view->p_viewport_vec, ui4_id);
        }
        gl_dfb_obj_unlock(p_view);
        gl_dfb_obj_unlock(p_plane);
    }
    else
    {
        p_view = gl_dfb_handle_lock_obj(GLT_OBJ_VIEW, h_view, &i4_ret);
        RETURN_RET_IF_FAIL(i4_ret);
        gl_dfb_dy_vec_remove(p_view->p_viewport_vec, ui4_id);
        gl_dfb_obj_unlock(p_view);        
    }
    if(p_viewport) 
        GL_FREE(p_viewport);
    GL_DFB_TRACE("Delete viewport :%x at view:%x\n", p_viewport, h_view);    

    return GLR_OK;
}    

INT32 gl_dfb_view_reconfig_viewport(
    GL_HPLANE_T     h_plane,
    GL_HVIEW_T      h_view,
    UINT32          ui4_viewport_id,
    INT32           i4_view_x,
    INT32           i4_view_y,
    UINT32          ui4_width,
    UINT32          ui4_height,
    UINT32          ui4_surf_offset_x,
    UINT32          ui4_surf_offset_y,
    GL_FIX16_T      fx_shrink_x,
    GL_FIX16_T      fx_shrink_y)
{
    return GLR_OK;
}    


INT32 
gl_dfb_plane_flip(
    GL_HPLANE_T     h_plane,
    GL_HVIEW_T      h_view,
    BOOL            b_forced)
{
    HANDLE_T            h_old_plane;
    GL_DFB_PLANE_T *    p_plane;
    GL_DFB_VIEW_T *     p_view;
   
    GL_DFB_PLANE_T *    p_old_plane = NULL_HANDLE;
        
    VOID *              ap_locked_obj[2];
    HANDLE_T            ah_handle[2];
    HANDLE_TYPE_T       ae_type[2];
    
    UINT32              ui4_idx;
    INT32               i4_ret;

    GL_DFB_TRACE("Flip view :%x to plane:%x\n", h_view, h_plane);    
    
    /* Lock the plane */
    p_plane = gl_dfb_handle_lock_obj(GLT_OBJ_PLANE, h_plane, &i4_ret);
    /*
     * How to remove the current views is depending HW defination in MW.
     * But this cannot be very smartly simulated. Just remove all.
     * Currently we just support one view/viewport on a plane.
     */
    _plane_invalidate_viewports(p_plane);    
    if(h_view == NULL_HANDLE)
    {        
        i4_ret = _plane_update_viewports(p_plane);        
        gl_dfb_obj_unlock(p_plane);  
        return i4_ret;
    }
    
    /* Lock the view */
    p_view = gl_dfb_handle_lock_obj(GLT_OBJ_VIEW, h_view, &i4_ret);    
    if(i4_ret != GLR_OK)
    {   
        gl_dfb_obj_unlock(p_plane);
        return GLR_DRV_ERROR;
    }
    h_old_plane = p_view->h_plane;
    gl_dfb_obj_unlock(p_view);
    gl_dfb_obj_unlock(p_plane);

    /* 
     * Re-lock planes in correct order, but some thing might be changed here, 
     * we need to check and give an error if something changed.
     */
    ae_type[0] = GLT_OBJ_PLANE; ah_handle[0] = h_plane;    
    ae_type[1] = GLT_OBJ_PLANE; ah_handle[1] = h_old_plane;
    
    /* Lock the planes */
    i4_ret = gl_dfb_mul_handles_lock_objs(ae_type, ah_handle, 2, ap_locked_obj);
    RETURN_RET_IF_FAIL(i4_ret);

    p_plane     = ap_locked_obj[0];
    p_old_plane = ap_locked_obj[1];   

    /* Lock the view */
    p_view = gl_dfb_handle_lock_obj(GLT_OBJ_VIEW, h_view, &i4_ret);
    if(i4_ret != GLR_OK)
    {
        gl_dfb_mul_objs_unlock(ap_locked_obj, 2);
        return GLR_FLIP_FAILED;    
    }
    
    p_view->h_plane = h_plane;    
    
    gl_dfb_obj_unlock(p_view);
    
    if(h_old_plane != NULL_HANDLE)
    {        
        GL_DFB_TRACE("Remove view %x from plane:%x\n", h_view, h_old_plane);
        if(gl_dfb_dy_vec_find(p_old_plane->p_hview_vec, &h_view, &ui4_idx))
        {               
            _plane_invalidate_viewports(p_old_plane);
            if(p_plane != p_old_plane)
            {                
                _plane_update_viewports(p_old_plane);    
            }
        }
        else
        {
            /* TODO: Rollback? */
            gl_dfb_mul_objs_unlock(ap_locked_obj, 2);
            return GLR_FLIP_FAILED;
        }     
    }  
    
    gl_dfb_dy_vec_add(p_plane->p_hview_vec, &h_view);       
    
    i4_ret = _plane_update_viewports(p_plane);
    
    gl_dfb_mul_objs_unlock(ap_locked_obj, 2);

    return i4_ret;
}

HANDLE_T gl_dfb_cc_helper_get_plane()
{
    return cc_helper_plane;
}
