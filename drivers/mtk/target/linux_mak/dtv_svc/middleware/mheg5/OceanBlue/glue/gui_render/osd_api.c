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
                    include files
-----------------------------------------------------------------------------*/
#include "dbg/u_dbg.h"
#include "dbg/x_dbg.h"
#include "cli/u_cli.h"
#include "cli/x_cli.h"
#include "dbg/def_dbg_level_mw.h"
#include "inc/u_common.h"
#include "graphic/x_gl.h"
#include "wgl/x_wgl.h"
#include "handle/x_handle.h"
#include "mheg5/OceanBlue/glue/gui_render/grd.h"
#include "mheg5/OceanBlue/glue/gui_render/gfx_rnd.h"
#include "mheg5/u_mheg5.h"
#include "mheg5/OceanBlue/glue/state_mngr/state_mngr.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_graphics.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_errors.h"
#if 1
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_bitmaps.h"
#include "img/x_img.h"
#include "img/u_img.h"
#endif


/*-----------------------------------------------------------------------------
                    data declarations
----------------------------------------------------------------------------*/
GUI_SURFACE_T g_t_surf =
{
    NULL_HANDLE,
    NULL_HANDLE,
    NULL_HANDLE,
    NULL_HANDLE,
    NULL_HANDLE,
#ifdef MW_MHEG5_ARGB8888_SUPPORT
    GL_COLORMODE_ARGB_D8888,
    GL_COLORMODE_ARGB_D8888,
#else
    GL_COLORMODE_ARGB_D4444,
    GL_COLORMODE_ARGB_D4444,
#endif
    NULL_HANDLE,
    FALSE,
    FALSE,
    FALSE,
    0,
    0,
    NULL_HANDLE
};

#if 1
static BOOL             _b_updated = TRUE;
static GL_RECT_T        _t_update_region = {0, 0, 0, 0};
#endif

#ifdef CLI_LVL_ALL
#ifdef  DBG_LEVEL_MODULE
#undef  DBG_LEVEL_MODULE
#endif

#define DBG_LEVEL_MODULE            mheg5_gui_get_dbg_level()

static UINT32                       _ui4_surface_num = 0;
#endif
/*-----------------------------------------------------------------------------
                    functions prototypes
 ----------------------------------------------------------------------------*/
#if 1
static BOOL _m_get4bytes(UINT8* pui1_data, UINT32 ui4_len, UINT32* pui4_readpos, UINT32* pui4_ret);

static UINT32 _get_png_pHYs(UINT8* pui1_data, UINT32 ui4_len);

static UINT16 _get_jpg_pHYs_ex(UINT8* pui1_data, UINT32 ui4_len, UINT32 z_w, UINT32 z_h);

static UINT16 _get_jpg_pHYs_from_table( UINT32 z_w, UINT32 z_h, UINT8 ui1_unit, UINT16 ui2_y_density );
#endif
/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
#if 1
static BOOL _m_get4bytes(UINT8* pui1_data, UINT32 ui4_len, UINT32* pui4_readpos, UINT32* pui4_ret)
{
    BOOL           b_eof = FALSE;
    UINT32         ui4_ret = 0;

    if (ui4_len >= (*pui4_readpos + 4))
    {
        ui4_ret  = pui1_data[(*pui4_readpos)++] << 24;
        ui4_ret |= pui1_data[(*pui4_readpos)++] << 16;
        ui4_ret |= pui1_data[(*pui4_readpos)++] << 8;
        ui4_ret |= pui1_data[(*pui4_readpos)++];
    }
    else
    {
        if ((*pui4_readpos + 1) <= ui4_len)
        {
            ui4_ret = pui1_data[(*pui4_readpos)++] << 24;
        }
        else
        {
            b_eof = TRUE;
            ui4_ret = 0;
        }

        if ((*pui4_readpos + 1) <= ui4_len)
        {
            ui4_ret |= pui1_data[(*pui4_readpos)++] << 16;
        }
        else
        {
            b_eof = TRUE;
            ui4_ret |= 0;
        }

        if ((*pui4_readpos + 1) <= ui4_len)
        {
            ui4_ret |= pui1_data[(*pui4_readpos)++] << 8;
        }
        else
        {
            b_eof = TRUE;
            ui4_ret |= 0;
        }

        if ((*pui4_readpos + 1) <= ui4_len)
        {
            ui4_ret |= pui1_data[(*pui4_readpos)++];
        }
        else
        {
            b_eof = TRUE;
            ui4_ret |= 0;
        }
    }
    *pui4_ret = ui4_ret;
    return b_eof;
}


static UINT32 _get_png_pHYs(UINT8* pui1_data, UINT32 ui4_len)
{
    BOOL   b_eof;
    UINT32 ui4_chunk_data_len;
    UINT32 ui4_pos = 0;
    UINT32 ui4_pHYs = 0;
    
    ui4_pos = 8;

    while(ui4_pos < ui4_len)
    {
        b_eof = _m_get4bytes(pui1_data, ui4_len, &ui4_pos, &ui4_chunk_data_len);
        if (b_eof)
        {
            break;
        }
        
        if (ui4_pos + 4 > ui4_len)
        {
            /* out of range */
            break;
        }
        else
        {
            /* pHYs */
            if (pui1_data[ui4_pos] == 112 && pui1_data[ui4_pos+1] == 72 &&
                pui1_data[ui4_pos+2] == 89  && pui1_data[ui4_pos+3] == 115)
            {
                ui4_pos += 8;
                
                b_eof = _m_get4bytes(pui1_data, ui4_len, &ui4_pos, &ui4_pHYs);
                if (b_eof || ((ui4_pos < ui4_len) && (pui1_data[ui4_pos] != 1)))
                {
                    ui4_pHYs = 0;
                }
                break;
            }
            /* IDAT */
            else if (pui1_data[ui4_pos] == 73 && pui1_data[ui4_pos+1] == 68 &&
                     pui1_data[ui4_pos+2] == 65  && pui1_data[ui4_pos+3] == 84)
            {
                break;
            }
            /* IEND */
            else if (pui1_data[ui4_pos] == 73 && pui1_data[ui4_pos+1] == 69 &&
                     pui1_data[ui4_pos+2] == 78  && pui1_data[ui4_pos+3] == 68)
            {
                break;
            }
            else
            {
                /* Skip this chunk */
                ui4_pos += (8 + ui4_chunk_data_len);
            }
        }
    }

    return ui4_pHYs;
}

/*************************************************************************************** 
*  this function and next function are  extension for CR98825, this is a table about 
*  HD resolution:
*  720*576    28 dots per cm or 72 dots per inch  --> resolution 2835
*  1280*720   35 dots per cm or 90 dots per inch  --> resolution 3543
*  1920*1080  53 dots per cm or 135 dots per inch --> resolution 5315
* 
*  the first step is  to look up above table, 
*  if it looks up fail, then compute the resolution by raw data
****************************************************************************************/
static UINT16 _get_jpg_pHYs_ex(UINT8* pui1_data, UINT32 ui4_len, UINT32 z_w, UINT32 z_h)
{
    UINT8  ui1_unit;
    UINT16 ui2_y_density;
    UINT16 ui2_y_resolution;

    if (ui4_len >= 18 &&
        pui1_data[0] == 0xFF && pui1_data[1] == 0xD8 &&
        pui1_data[2] == 0xFF && pui1_data[3] == 0xE0 &&
        pui1_data[6] == 'J'  && pui1_data[7] == 'F'  && pui1_data[8] == 'I' && pui1_data[9] == 'F')
    {
        ui1_unit      = pui1_data[13];   
        ui2_y_density = ((UINT16)(pui1_data[16] << 8)) | ((UINT16)pui1_data[17]);
        
        ui2_y_resolution = _get_jpg_pHYs_from_table(z_w, z_h, ui1_unit, ui2_y_density);
        
        if( 0 != ui2_y_resolution )
        {
        	return ui2_y_resolution;
        }

        if (ui1_unit == 1)
        {
        	/* inch */
            return (ui2_y_density*10000 + 127)/254;
        }
        else if (ui1_unit == 2)
        {
            /* cm */
            if((ui2_y_density*254)%100 < 50)
            {
                return (((ui2_y_density*254/100)*10000 + 127)/254);
            }
            else 
            {
                return ((((ui2_y_density*254/100) + 1)*10000 + 127)/254);
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
	
static UINT16 _get_jpg_pHYs_from_table( UINT32 z_w, UINT32 z_h, UINT8 ui1_unit, UINT16 ui2_y_density )
{
	if (ui1_unit == 1)
    {   /* inch */
       	if(z_w == 720 && z_h == 576 && ui2_y_density == 72)
     	{
       		return 2835;
       	}
       	if(z_w == 1280 && z_h == 720 && ui2_y_density == 90)
        {
        	return 3543; 
        }
        else if(z_w == 1920 && z_h == 1080 && ui2_y_density == 135)
        {
       		return 5315;
       	}
       	else
       	{
       		return 0;
       	}
	}
	else if (ui1_unit == 2)
    {
    	/* cm */
    	if(z_w == 720 && z_h == 576 && ui2_y_density == 28)
    	{
			return 2835;
		}
		if(z_w == 1280 && z_h == 720 && ui2_y_density == 35)
		{
			return 3543; 
		}
		else if(z_w == 1920 && z_h == 1080 && ui2_y_density == 53)
		{
        	return 5315;
        }
        else
        {
        	return 0;
        }
    }
    else
    {   
        return 0;
    }
}
#endif


void DVP_OSDInit( 
    const OSDColor* palette_ptr, 
    U16BIT          size )
{
    INT32                    i4_ret;

    DBG_INFO(("{MHEG5-OSD} %s, palette_ptr:%p, size:%d\n", __FUNCTION__, palette_ptr, size));
    DBG_ERROR(("{MHEG5-OSD} %s, palette_ptr:%p, size:%d\n", __FUNCTION__, palette_ptr, size));
    DVP_OSDExit();
   
    if (palette_ptr != NULL)
    {
        #if 0
        UINT16 i;
        GL_COLOR_T* at_color = NULL;
        
        MHEG5_ASSERT(size <= 256);
        MHEG5_ASSERT(g_t_surf.h_palette != NULL_HANDLE);

        at_color = (GL_COLOR_T *)x_mem_alloc(188 * sizeof(GL_COLOR_T));
        if (at_color == NULL)
        {
            MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, return error(out of memory)\n",__FUNCTION__, __LINE__));
            return ;
        }
        /* entry map(offset: 64)
                 {full transparent color: 0}
                 {semi-transparent color: 1 ~ 48}
                 {opaque grey level: 49 ~ 52}
                 {opaque color: 53 ~ 187}
             */
        for (i = 0; i < 188; i++)
        {
            at_color[i].a     = (UINT8) (palette_ptr[i+64] >> 24);
            at_color[i].u1.r  = (UINT8) ((palette_ptr[i+64] >> 16) & 0xFF);
            at_color[i].u2.g  = (UINT8) ((palette_ptr[i+64] >> 8) & 0xFF);
            at_color[i].u3.b  = (UINT8) (palette_ptr[i+64] & 0xFF);

            MHEG5_DBG_APP_OSD(("GL_COLOR_DEF(%3d, %3d, %3d, %3d),\t\t/* palette entry\t%d */\n",
                at_color[i].a,at_color[i].u1.r,at_color[i].u2.g,at_color[i].u3.b, i+64));
        }

        grd_update_pal_entry(188, at_color);
        
        i4_ret = x_gl_palette_set_entries(
                    g_t_surf.h_palette, 
                    64, 
                    188, 
                    at_color);

        x_mem_free(at_color);

        if (i4_ret != GLR_OK)
        {
            MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, create palette error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
            return ;
        }
        #endif
    }

    i4_ret = grd_reconfig();
    if (i4_ret != MHEG5R_OK)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, grd_reconfig() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
    }
    
}

void DVP_OSDExit( void )
{
    DBG_INFO(("{MHEG5-OSD} %s\n", __FUNCTION__));

    if (NULL_HANDLE != g_t_surf.h_surf)
    {
        x_gl_obj_free(g_t_surf.h_surf);
        g_t_surf.h_surf = NULL_HANDLE;
    }

    if (NULL_HANDLE != g_t_surf.h_surf_buf)
    {
        x_gl_obj_free(g_t_surf.h_surf_buf);
        g_t_surf.h_surf_buf = NULL_HANDLE;
    }

    /*
    if (NULL_HANDLE != g_t_surf.h_palette)
    {
        x_gl_obj_free(g_t_surf.h_palette);
        g_t_surf.h_palette = NULL_HANDLE;
    }
    */
    
    g_t_surf.b_surf_rendered = FALSE;
    g_t_surf.b_view_rendered = FALSE;
    g_t_surf.ui4_width  = (INT32) 0;
    g_t_surf.ui4_height = (INT32) 0;
}


void* DVP_OSDCreateSurface( 
    U16BIT width, 
    U16BIT height, 
    BOOLEAN init, 
    U32BIT colour )
{
    INT32     i4_ret;
    HANDLE_T  h_surf = NULL_HANDLE;
    GL_SURFACE_DESC_T   t_surf_desc;

    DBG_INFO(("{MHEG5-OSD} %s, width:%d, height:%d, init:%d, colour:0X%X\n", __FUNCTION__, width, height, init, colour));
    
    if (width == 0 || height == 0)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid parameters)\n",__FUNCTION__, __LINE__));
        return NULL;
    }

    t_surf_desc.ui4_width  = (UINT32)width;
    t_surf_desc.ui4_height = (UINT32)height;
    t_surf_desc.e_clrmode  = g_t_surf.e_internal_col_mode;
    t_surf_desc.h_clipper  = NULL_HANDLE;
    t_surf_desc.h_palette  = NULL_HANDLE;/*g_t_surf.h_palette;*/

    i4_ret = x_gl_surface_create(&t_surf_desc, &h_surf);
    if (i4_ret != GLR_OK)
    {        
        DBG_ERROR(("[MHEG5-OSD] %s #%d, create surface error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }

    if (init == TRUE)
    {
        GL_COLOR_T t_out_color;

        if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_D4444)
        {
            UINT8 a,r,g,b;

            a = (UINT8)(((colour & 0x0000F000) >> 12) * 17); 
            r = (UINT8)(((colour & 0x00000F00) >> 8) * 17); 
            g = (UINT8)(((colour & 0x000000F0) >> 4) * 17); 
            b = (UINT8)(((colour & 0x0000000F) >> 0) * 17); 

            colour = GL_COLOR_ARGB8888(a, r, g, b);

            match_color(colour, g_t_surf.e_internal_col_mode, g_t_surf.h_palette, &t_out_color);
        }
        else if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_D8888)
        {
            match_color(colour, g_t_surf.e_internal_col_mode, g_t_surf.h_palette, &t_out_color);
        }
        #if 0
        else if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_CLUT8)
        {
            t_out_color.u3.index = colour;
        }
        #endif
        
        i4_ret = x_gl_surface_fill(h_surf, &t_out_color, GL_SYNC);
        if (i4_ret != GLR_OK)
        {
            x_gl_obj_free(h_surf);

            DBG_ERROR(("[MHEG5-OSD] %s #%d, fill surface error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
            return NULL;
        }
    }
    
    DBG_INFO(("{MHEG5-OSD} %s, created surface:%p\n", __FUNCTION__, (void*)h_surf));
#ifdef CLI_LVL_ALL
    _ui4_surface_num++;
#endif
    return ((void*)h_surf);
}


void* DVP_OSDLockBuffer( void* surface, U32BIT* pitch )
{
    INT32    i4_ret;
    HANDLE_T h_surf;
    GL_LOCK_INFO_T  t_lock_info;

    DBG_INFO(("{MHEG5-OSD} %s, surface:%p, pitch:%p\n", __FUNCTION__, surface, pitch));
    
    if (surface == NULL || pitch == NULL)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid parameters)\n",__FUNCTION__, __LINE__));
        return NULL;
    }

    h_surf = (HANDLE_T)surface;

    if (x_handle_valid(h_surf)) 
    {
        /* lock surface */
        i4_ret = x_gl_surface_lock(h_surf, NULL, GL_SYNC, &t_lock_info);
        if (i4_ret != GLR_OK)
        {
            DBG_ERROR(("[MHEG5-OSD] %s #%d, lock surface error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
            return NULL;
        }
        
        *pitch  = t_lock_info.at_subplane[0].ui4_pitch;

        DBG_INFO(("{MHEG5-OSD} %s, address is %p, pitch is %lu\n", __FUNCTION__, t_lock_info.at_subplane[0].pv_bits, *pitch));
        
        return  (t_lock_info.at_subplane[0].pv_bits);
        
    }
    else
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid surface handle)\n",__FUNCTION__, __LINE__));
        return NULL;
    }
}

void DVP_OSDUnlockBuffer( void* surface )
{
    HANDLE_T h_surf;
    INT32    i4_ret;

    DBG_INFO(("{MHEG5-OSD} %s, surface:%p\n", __FUNCTION__, surface));
    
    if (surface == NULL)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid parameters)\n",__FUNCTION__, __LINE__));
        return ;
    }

    h_surf = (HANDLE_T)surface;

    if (x_handle_valid(h_surf)) 
    {        
        i4_ret = x_gl_surface_unlock(h_surf, NULL);

        if (i4_ret != GLR_OK)
        {
            DBG_ERROR((
                 "[MHEG5-OSD] %s #%d: !!!Panic!!! Unlock surface %lu failed(%ld)\r\n",
                  __FUNCTION__,
                  __LINE__,
                  h_surf,
                  i4_ret));
        }
    }
    else
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid surface handle)\n",__FUNCTION__, __LINE__));
    }
}

void DVP_OSDDestroySurface( void* surface )
{
    HANDLE_T h_surf;

    DBG_INFO(("{MHEG5-OSD} %s, surface:%p\n", __FUNCTION__, surface));
    
    if (surface == NULL)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid surface handle)\n",__FUNCTION__, __LINE__));
        return ;
    }

    h_surf = (HANDLE_T)surface;
    
    if (x_handle_valid(h_surf))
    {
        x_gl_obj_free(h_surf);
    }
#ifdef CLI_LVL_ALL
    _ui4_surface_num--;
#endif    
}

void DVP_OSDBlitBitmap( 
    void* surface, 
    S_RECT* pRect, 
    U32BIT pitch,
    U16BIT screen_x, 
    U16BIT screen_y, 
    E_BLIT_FLAG bflg )
{
    INT32 i4_ret;
    HANDLE_T h_surf;
    
    if (surface == NULL || pRect == NULL)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid surface handle)\n",__FUNCTION__, __LINE__));
        return ;
    }

    DBG_INFO(("{MHEG5-OSD} %s, surface:%p, pRect:%p(L:%ld,T:%ld,W:%lu,H:%lu), pitch:%lu, screen_x:%lu, screen_y:%lu, bflg:%d\n", __FUNCTION__, surface, pRect, pRect->left, pRect->top, pRect->width, pRect->height, pitch, screen_x, screen_y, bflg));

    h_surf = (HANDLE_T)surface;
    
    if (x_handle_valid(h_surf) && x_handle_valid(g_t_surf.h_surf_buf))
    {
        BOOL b_exit_lock = FALSE;
        
        /* unlock the surface first */
        i4_ret = x_gl_surface_unlock(h_surf, NULL);
        if (i4_ret == GLR_OK)
        {
            DBG_INFO(("[MHEG5-OSD] %s #%d, unlock surface first, GL return OK. Lock this surface again when this function exits.\n",__FUNCTION__, __LINE__, i4_ret));
            b_exit_lock = TRUE;
        }
        else if (i4_ret != GLR_SURFACE_NOT_LOCKED)
        {
            DBG_ERROR(("[MHEG5-OSD] %s #%d, unlock surface error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        }
        
        switch (bflg)
        {
            case BLIT_COPY:
                {
                    i4_ret = x_gl_surface_blt(
                                g_t_surf.h_surf_buf,
                                h_surf,
                                (INT32)screen_x,
                                (INT32)screen_y,
                                (INT32)pRect->left,
                                (INT32)pRect->top,
                                (UINT32)pRect->width,
                                (UINT32)pRect->height,
                                255,
                                GL_SYNC);
                    if (i4_ret != GLR_OK)
                    {                        
                        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_blt() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
                    }
                }
                break;

            case BLIT_ALPHA_BLEND:
                {
                    i4_ret = x_gl_surface_compose(
                                g_t_surf.h_surf_buf,
                                h_surf,
                                (INT32)screen_x,
                                (INT32)screen_y,
                                (INT32)pRect->left,
                                (INT32)pRect->top,
                                (UINT32)pRect->width,
                                (UINT32)pRect->height,
                                255,
                                GL_SRC_OVER,
                                GL_SYNC);
                    if (i4_ret != GLR_OK)
                    {                        
                        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_compose() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
                    }
                }
                break;

            default:
                break;
        }

        if (b_exit_lock == TRUE)
        {            
            GL_LOCK_INFO_T  t_lock_info;
            
            i4_ret = x_gl_surface_lock(h_surf, NULL, GL_SYNC, &t_lock_info);
            if (i4_ret != GLR_OK)
            {
                DBG_ERROR(("[MHEG5-OSD] %s #%d, lock surface error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
            }
            else
            {
                DBG_INFO(("[MHEG5-OSD] %s #%d, surface locked again!\n",__FUNCTION__, __LINE__));
            }
        }
    }
    else
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid surface handle)\n",__FUNCTION__, __LINE__));
    }
#if 1
    x_sema_lock(g_t_surf.h_lock, X_SEMA_OPTION_WAIT);

    if (_b_updated)
    {
        SET_RECT_BY_SIZE(
                    &_t_update_region,
                    pRect->left,
                    pRect->top,
                    pRect->width,
                    pRect->height);

        _b_updated = FALSE;
    }
    else
    {
        GL_RECT_T t_rect;
        
        SET_RECT_BY_SIZE(
                    &t_rect,
                    pRect->left,
                    pRect->top,
                    pRect->width,
                    pRect->height);
        
        UNION_RECT(&_t_update_region, &_t_update_region, &t_rect);
    }

    x_sema_unlock(g_t_surf.h_lock);
#endif
    g_t_surf.b_surf_rendered = TRUE;
}

void DVP_OSDDrawRectangle( 
    S_RECT* pRect, 
    U32BIT colour, 
    E_BLIT_FLAG bflg )
{
#if 0
    INT32      i4_ret;
    HANDLE_T   h_rect_surf;
    GL_SURFACE_DESC_T   t_surf_desc;
    GL_COLOR_T t_out_color;

    if (pRect == NULL)
    {
        MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, return error(invalid parameters)\n",__FUNCTION__, __LINE__));
        return;
    }

    MHEG5_DBG_APP_OSD(("{MHEG5-OSD} %s, pRect:%p(L:%ld,T:%ld,W:%lu,H:%lu), colour:0X%X, bflg:%d\n", __FUNCTION__, pRect, pRect->left, pRect->top, pRect->width, pRect->height, colour, bflg));

    t_surf_desc.ui4_width  = (UINT32)pRect->width;
    t_surf_desc.ui4_height = (UINT32)pRect->height;
    t_surf_desc.e_clrmode  = g_t_surf.e_internal_col_mode;
    t_surf_desc.h_clipper  = NULL_HANDLE;
    t_surf_desc.h_palette  = NULL_HANDLE;/*g_t_surf.h_palette;*/

    i4_ret = x_gl_surface_create(&t_surf_desc, &h_rect_surf);
    if (i4_ret != GLR_OK)
    {
        MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, create surface error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return;
    }
    
    if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_D4444)
    {
        UINT8 a,r,g,b;
    
        a = (UINT8)(((colour & 0x0000F000) >> 12) * 17); 
        r = (UINT8)(((colour & 0x00000F00) >> 8) * 17); 
        g = (UINT8)(((colour & 0x000000F0) >> 4) * 17); 
        b = (UINT8)(((colour & 0x0000000F) >> 0) * 17); 
    
        colour = GL_COLOR_ARGB8888(a, r, g, b);
    
        match_color(colour, g_t_surf.e_internal_col_mode, g_t_surf.h_palette, &t_out_color);
    }
    else if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_D8888)
    {
        match_color(colour, g_t_surf.e_internal_col_mode, g_t_surf.h_palette, &t_out_color);
    }
    #if 0
    else if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_CLUT8)
    {
        t_out_color.u3.index = colour;
    }
    #endif
    
    i4_ret = x_gl_surface_fill(h_rect_surf, &t_out_color, GL_SYNC);
    if (i4_ret != GLR_OK)
    {
        x_gl_obj_free(h_rect_surf);
        
        MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, fill surface error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return;
    }

    if (x_handle_valid(g_t_surf.h_surf_buf))
    {
        switch (bflg)
        {
            case BLIT_COPY:
                {
                    i4_ret = x_gl_surface_blt(
                                g_t_surf.h_surf_buf,
                                h_rect_surf,
                                (INT32)pRect->left,
                                (INT32)pRect->top,
                                (INT32)0,
                                (INT32)0,
                                (UINT32)pRect->width,
                                (UINT32)pRect->height,
                                255,
                                GL_SYNC);
                    
                    if (i4_ret != GLR_OK)
                    {                        
                        MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, x_gl_surface_blt() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
                    }
                }
                break;

            case BLIT_ALPHA_BLEND:
                {
                    i4_ret = x_gl_surface_compose(
                                g_t_surf.h_surf_buf,
                                h_rect_surf,
                                (INT32)pRect->left,
                                (INT32)pRect->top,
                                (INT32)0,
                                (INT32)0,
                                (UINT32)pRect->width,
                                (UINT32)pRect->height,
                                255,
                                GL_SRC_OVER,
                                GL_SYNC);
                    
                    if (i4_ret != GLR_OK)
                    {                        
                        MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, x_gl_surface_compose() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
                    }
                }
                break;

            default:
                break;
        }
    }
    else
    {
        MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, return error(invalid surface handle)\n",__FUNCTION__, __LINE__));
    }

    x_gl_obj_free(h_rect_surf);
#if 1
    x_sema_lock(g_t_surf.h_lock, X_SEMA_OPTION_WAIT);

    if (_b_updated)
    {
        SET_RECT_BY_SIZE(
                    &_t_update_region,
                    pRect->left,
                    pRect->top,
                    pRect->width,
                    pRect->height);

        _b_updated = FALSE;
    }
    else
    {
        GL_RECT_T t_rect;
        
        SET_RECT_BY_SIZE(
                    &t_rect,
                    pRect->left,
                    pRect->top,
                    pRect->width,
                    pRect->height);
        
        UNION_RECT(&_t_update_region, &_t_update_region, &t_rect);
    }

    x_sema_unlock(g_t_surf.h_lock);
#endif
    g_t_surf.b_surf_rendered = TRUE;

    return;

#else
    INT32      i4_ret;
    GL_COLOR_T t_out_color;
    GL_RECT_T t_rect;
    
    if (pRect == NULL)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid parameters)\n",__FUNCTION__, __LINE__));
        return;
    }

    DBG_INFO(("{MHEG5-OSD} %s, pRect:%p(L:%ld,T:%ld,W:%lu,H:%lu), colour:0X%X, bflg:%d\n", __FUNCTION__, pRect, pRect->left, pRect->top, pRect->width, pRect->height, colour, bflg));

    t_rect.i4_left   = (INT32)pRect->left;
    t_rect.i4_top    = (INT32)pRect->top;
    t_rect.i4_right  = (INT32)pRect->left + (INT32)pRect->width;
    t_rect.i4_bottom = (INT32)pRect->top + (INT32)pRect->height;

    if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_D4444)
    {
        UINT8 a,r,g,b;

        a = (UINT8)(((colour & 0x0000F000) >> 12) * 17); 
        r = (UINT8)(((colour & 0x00000F00) >> 8) * 17); 
        g = (UINT8)(((colour & 0x000000F0) >> 4) * 17); 
        b = (UINT8)(((colour & 0x0000000F) >> 0) * 17); 

        colour = GL_COLOR_ARGB8888(a, r, g, b);

        match_color(colour, g_t_surf.e_internal_col_mode, g_t_surf.h_palette, &t_out_color);
    }
    else if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_D8888)
    {
        match_color(colour, g_t_surf.e_internal_col_mode, g_t_surf.h_palette, &t_out_color);
    }
    #if 0
    else if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_CLUT8)
    {
        t_out_color.u3.index = colour;
    }
    #endif

    if (x_handle_valid(g_t_surf.h_surf_buf))
    {
        switch (bflg)
        {
            case BLIT_COPY:
                {
                    i4_ret = x_gl_surface_draw_rect(
                                g_t_surf.h_surf_buf,
                                &t_rect,
                                &t_out_color, 
                                0,
                                GL_STYLE_FILLED, 
                                GL_SYNC);                    
                    if (i4_ret != GLR_OK)
                    {                        
                        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_blt() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
                    }
                }
                break;

            case BLIT_ALPHA_BLEND:
                {
                    i4_ret = x_gl_surface_rect_src_compose(
                        g_t_surf.h_surf_buf,
                        (INT32)pRect->left,
                        (INT32)pRect->top,
                        (UINT32)pRect->width,
                        (UINT32)pRect->height,
                        255,
                        GL_SRC_OVER,
                        g_t_surf.e_internal_col_mode,
                        &t_out_color,
                        GL_SYNC);
                    if (i4_ret != GLR_OK)
                    {                        
                        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_compose() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
                    }
                }
                break;

            default:
                break;
        }
    }
    else
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid surface handle)\n",__FUNCTION__, __LINE__));
    }
#if 1
    x_sema_lock(g_t_surf.h_lock, X_SEMA_OPTION_WAIT);

    if (_b_updated)
    {
        SET_RECT_BY_SIZE(
                    &_t_update_region,
                    pRect->left,
                    pRect->top,
                    pRect->width,
                    pRect->height);

        _b_updated = FALSE;
    }
    else
    {
        GL_RECT_T t_rect;
        
        SET_RECT_BY_SIZE(
                    &t_rect,
                    pRect->left,
                    pRect->top,
                    pRect->width,
                    pRect->height);
        
        UNION_RECT(&_t_update_region, &_t_update_region, &t_rect);
    }

    x_sema_unlock(g_t_surf.h_lock);
#endif
    g_t_surf.b_surf_rendered = TRUE;

    return;
#endif
}

void DVP_OSDUpdate( void )
{
    INT32 i4_ret;
    GL_RECT_T t_rect;

    DBG_INFO(("{MHEG5-OSD} %s\n", __FUNCTION__));

    x_sema_lock(g_t_surf.h_lock, X_SEMA_OPTION_WAIT);        

    x_gl_surface_blt(
                g_t_surf.h_surf,
                g_t_surf.h_surf_buf,
                _t_update_region.i4_left,
                 _t_update_region.i4_top,
                _t_update_region.i4_left,
                 _t_update_region.i4_top,
                (UINT32)RECT_W(&_t_update_region),
                (UINT32)RECT_H(&_t_update_region),
                255,
                GL_SYNC);
    
    x_sema_unlock(g_t_surf.h_lock);
    
    if (NULL_HANDLE != g_t_surf.h_surf_bak)
    {       
        SET_RECT_BY_SIZE(&t_rect, 0, 0, g_t_surf.ui4_width, g_t_surf.ui4_height);
 
        x_sema_lock(g_t_surf.h_lock, X_SEMA_OPTION_WAIT);        
        if (MHEG5R_OK != grd_cnv_argb_16b_2_pal_8b(            
            g_t_surf.h_surf_bak,            
            g_t_surf.h_surf_buf,            
            &t_rect,            
            g_t_surf.pui1_pal_cnv_tbl))        
        {            
            x_sema_unlock(g_t_surf.h_lock);            
            return;        
        }                

        x_sema_unlock(g_t_surf.h_lock);
    }

#if 1
    SET_RECT_BY_SIZE(&t_rect, 0, 0, g_t_surf.ui4_width, g_t_surf.ui4_height);
#else
    t_rect.i4_left = _t_update_region.i4_left;
    t_rect.i4_top = _t_update_region.i4_top;
    t_rect.i4_bottom = _t_update_region.i4_bottom;
    t_rect.i4_right = _t_update_region.i4_right;
    
    x_sema_lock(g_t_surf.h_lock, X_SEMA_OPTION_WAIT);
    
    x_memset(&_t_update_region, 0, sizeof(GL_RECT_T));
    _b_updated = TRUE;
    
    x_sema_unlock(g_t_surf.h_lock);
    
#endif

    i4_ret = grd_update( &t_rect, TRUE );
    if (i4_ret != MHEG5R_OK)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
    }
}

#if 1
S_IMAGE *DVP_DecodePng( U8BIT *data, U32BIT size )
{
    INT32    i4_ret;
    S_IMAGE* pt_img;
    HANDLE_T h_img;
    SIZE_T   z_w, z_h;
    HANDLE_T h_dec_surf, h_dest_surf;
    GL_SURFACE_DESC_T   t_surf_desc;
    GL_LOCK_INFO_T      t_lock_info;

    DBG_INFO(("{MHEG5-OSD} %s, data:%p, size:%lu\n", __FUNCTION__, data, size));

    if (data == NULL || size == 0)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid parameters)\n",__FUNCTION__, __LINE__));
        return NULL;
    }

    pt_img = (S_IMAGE *)x_mem_alloc(sizeof(S_IMAGE) + sizeof(HANDLE_T));
    if (pt_img == NULL)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(out of memory)\n",__FUNCTION__, __LINE__));
        return NULL;
    }

    i4_ret = x_img_mopen((const VOID *) data,
                         (SIZE_T) size,
                         "png",
                         NULL,
                         NULL,
                         &h_img);
    if (i4_ret != IMGR_OK)
    {
        x_mem_free(pt_img);
        
        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_img_mopen() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }

    i4_ret = x_img_get_dimension( h_img,
                                  INTRA_IMG_PRIMARY_IMG_TYPE,
                                  &z_w,
                                  &z_h);
    if (i4_ret != IMGR_OK)
    {
        x_mem_free(pt_img);
        x_img_close(h_img);

        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_img_get_dimension() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }

    pt_img->width = (U16BIT)z_w;
    pt_img->height = (U16BIT)z_h;

    if (g_t_surf.b_hd_support)
    {
        pt_img->y_resolution = _get_png_pHYs(data, size);
    }
    else
    {
        pt_img->y_resolution = 0;
    }

    pt_img->opaque =    HAS_TRANSPARENCY;

    t_surf_desc.ui4_width  = (UINT32)z_w;
    t_surf_desc.ui4_height = (UINT32)z_h;
    t_surf_desc.h_clipper  = NULL_HANDLE;
    t_surf_desc.h_palette  = NULL_HANDLE;/*g_t_surf.h_palette;*/

    if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_D8888)
    {
        pt_img->pix_bytes = 4;
        t_surf_desc.e_clrmode  = GL_COLORMODE_ARGB_D8888;
    }
    else if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_D4444)
    {
        pt_img->pix_bytes = 2;
        t_surf_desc.e_clrmode  = GL_COLORMODE_ARGB_D4444;
    }
    else
    {
        pt_img->pix_bytes = 1;
        t_surf_desc.e_clrmode  = GL_COLORMODE_ARGB_D4444;
    }

    i4_ret = x_gl_surface_create(&t_surf_desc, &h_dec_surf);
    if (i4_ret != GLR_OK)
    {
        x_mem_free(pt_img);
        x_img_close(h_img);
        
        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_create() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }
	
    i4_ret = x_img_open_eng(PNG_SW_ENG_NAME, NULL, NULL, h_img);
    if (i4_ret != IMGR_OK)
    {
        x_gl_obj_free(h_dec_surf);
        x_img_close(h_img);
        x_mem_free(pt_img);
        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_img_open_eng() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }

    i4_ret = x_img_sync_load(
                            h_img,
                            INTRA_IMG_PRIMARY_IMG_TYPE,
                            NULL,
                            IMG_ROTATE_CLOCKWS_0_OP_TYPE,
                            0,
                            0,
                            z_w,
                            z_h,
                            0,
                            0,
                            z_w,
                            z_h,
                            h_dec_surf);   
    if (i4_ret != IMGR_OK)
    {
        x_mem_free(pt_img);
        x_gl_obj_free(h_dec_surf);
        x_img_close(h_img);
        
        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_img_sync_load() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }

    #if 0
    if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_CLUT8)
    {
        GL_RECT_T t_rect;
        
        t_surf_desc.ui4_width  = (UINT32)z_w;
        t_surf_desc.ui4_height = (UINT32)z_h;
        t_surf_desc.h_clipper  = NULL_HANDLE;
        t_surf_desc.h_palette  = NULL_HANDLE;/*g_t_surf.h_palette;*/
        t_surf_desc.e_clrmode  = GL_COLORMODE_ARGB_CLUT8;
    
        i4_ret = x_gl_surface_create(&t_surf_desc, &h_dest_surf);
        if (i4_ret != GLR_OK)
        {
            x_mem_free(pt_img);
            x_img_close(h_img);
            x_gl_obj_free(h_dec_surf);
            
            MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, x_gl_surface_create() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
            return NULL;
        }

        SET_RECT_BY_SIZE(&t_rect, 0, 0, z_w, z_h);
        i4_ret = grd_cnv_argb_16b_2_pal_8b(h_dest_surf, h_dec_surf, &t_rect, g_t_surf.pui1_pal_cnv_tbl);
        if (i4_ret != GLR_OK)
        {
            x_mem_free(pt_img);
            x_gl_obj_free(h_dec_surf);
            x_gl_obj_free(h_dest_surf);
            x_img_close(h_img);
        
            MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, grd_argb_cnv_32b_2_16b() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
            return NULL;
        }
        
        x_gl_obj_free(h_dec_surf);
    }
    else
    #endif
    {
        h_dest_surf = h_dec_surf;
    }
    
    /* lock surface */
    i4_ret = x_gl_surface_lock(h_dest_surf, NULL, GL_SYNC, &t_lock_info);
    if (i4_ret != GLR_OK)
    {
        x_mem_free(pt_img);
        x_gl_obj_free(h_dest_surf);
        x_img_close(h_img);
        
        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_lock() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }
    #if 0
    if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_CLUT8)
    {
        UINT32 i;
        UINT32 ui4_pitch;
        UINT8* pui1_buf;
        
        pui1_buf = (U8BIT*)(t_lock_info.at_subplane[0].pv_bits);
        ui4_pitch = t_lock_info.at_subplane[0].ui4_pitch;
        
        pt_img->col_buff = x_mem_alloc(z_w * z_h);
        if (pt_img->col_buff == NULL)
        {
            x_mem_free(pt_img);
            x_gl_surface_unlock(h_dest_surf, NULL);
            x_gl_obj_free(h_dest_surf);
            x_img_close(h_img);
            
            MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, alloc mem failed\n",__FUNCTION__, __LINE__));
            return NULL;
        }
        for (i = 0; i < z_h; i++)
        {
            x_memcpy(&(pt_img->col_buff[z_w * i]), &(pui1_buf[i * ui4_pitch]), z_w);
        }
        
        *((HANDLE_T*)(pt_img + 1)) = NULL_HANDLE;
        
        x_gl_surface_unlock(h_dest_surf, NULL);
        x_gl_obj_free(h_dest_surf);
    }
    else
    #endif
    {
        pt_img->col_buff = (U8BIT*)(t_lock_info.at_subplane[0].pv_bits);

        *((HANDLE_T*)(pt_img + 1)) = h_dest_surf;
    }

    x_img_close(h_img);

    DBG_INFO(("{MHEG5-OSD} %s, return S_IMAGE:%p, width:%d, height:%d, y_resolution:%d, pix_bytes:%d, opaque:%d, col_buff:%p\n", __FUNCTION__, pt_img, pt_img->width, pt_img->height, pt_img->y_resolution, pt_img->pix_bytes, pt_img->opaque, pt_img->col_buff));

    return pt_img;
    
}

void DVP_FreePng( S_IMAGE *image )
{
    HANDLE_T h_surf;

    DBG_INFO(("{MHEG5-OSD} %s, image:%p\n", __FUNCTION__, image));
    
    if (image == NULL)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid parameters)\n",__FUNCTION__, __LINE__));
        return ;
    }

    h_surf = *((HANDLE_T*)(image + 1));

    if (h_surf != NULL_HANDLE && x_handle_valid(h_surf)) 
    {
        INT32 i4_ret;
        
        i4_ret = x_gl_surface_unlock(h_surf, NULL);

        if (i4_ret != GLR_OK)
        {
            DBG_ERROR((
                 "[MHEG5-OSD] %s #%d: !!!Panic!!! Unlock surface %lu failed(%ld)\r\n",
                  __FUNCTION__,
                  __LINE__,
                  h_surf,
                  i4_ret));
        }

        x_gl_obj_free(h_surf);
    }
    else
    {
        #if 0
        if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_CLUT8)
        {
            if (image->col_buff != NULL)
            {
                x_mem_free(image->col_buff);
            }
        }
        #endif
    }

    x_mem_free(image);
}

S_IMAGE *DVP_DecodeJpg( U8BIT *data, U32BIT size )
{
    INT32    i4_ret;
    S_IMAGE* pt_img;
    HANDLE_T h_img;
    SIZE_T   z_w, z_h;
    HANDLE_T h_dest_surf;
    GL_SURFACE_DESC_T   t_surf_desc;
    GL_LOCK_INFO_T      t_lock_info;

    DBG_INFO(("{MHEG5-OSD} %s, data:%p, size:%lu\n", __FUNCTION__, data, size));

    if (data == NULL || size == 0)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid parameters)\n",__FUNCTION__, __LINE__));
        return NULL;
    }

    pt_img = (S_IMAGE *)x_mem_alloc(sizeof(S_IMAGE) + sizeof(HANDLE_T));
    if (pt_img == NULL)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(out of memory)\n",__FUNCTION__, __LINE__));
        return NULL;
    }

    i4_ret = x_img_mopen((const VOID *) data,
                         (SIZE_T) size,
                         "jpg",
                         NULL,
                         NULL,
                         &h_img);
    if (i4_ret != IMGR_OK)
    {
        x_mem_free(pt_img);

        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_img_mopen() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }

    i4_ret = x_img_get_dimension( h_img,
                                  INTRA_IMG_PRIMARY_IMG_TYPE,
                                  &z_w,
                                  &z_h);
    if (i4_ret != IMGR_OK)
    {
        x_mem_free(pt_img);
        x_img_close(h_img);

        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_img_get_dimension() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }
    pt_img->width = (U16BIT)z_w;
    pt_img->height = (U16BIT)z_h;

    if (g_t_surf.b_hd_support)
    {
        pt_img->y_resolution = _get_jpg_pHYs_ex(data, size, z_w, z_h);
    }
    else
    {
        pt_img->y_resolution = 0;
    }
    
    if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_D8888)
    {
        pt_img->pix_bytes = 4;
    }
    else if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_D4444)
    {
        pt_img->pix_bytes = 2;
    }
    else
    {
        pt_img->pix_bytes = 1;
    }

    /* we always decode the JPEG to a AYUV8888 surface */
    pt_img->opaque =    HAS_TRANSPARENCY;

    if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_D8888)
    {
        /* we have to create a new ARGB8888 surface */
        t_surf_desc.ui4_width  = (UINT32)z_w;
        t_surf_desc.ui4_height = (UINT32)z_h;
        t_surf_desc.e_clrmode  = GL_COLORMODE_ARGB_D8888;
        t_surf_desc.h_clipper  = NULL_HANDLE;
        t_surf_desc.h_palette  = NULL_HANDLE;/*g_t_surf.h_palette;*/
    }
    else
    {
        /* we have to create a new ARGB4444 surface */
        t_surf_desc.ui4_width  = (UINT32)z_w;
        t_surf_desc.ui4_height = (UINT32)z_h;
        t_surf_desc.e_clrmode  = GL_COLORMODE_ARGB_D4444;
        t_surf_desc.h_clipper  = NULL_HANDLE;
        t_surf_desc.h_palette  = NULL_HANDLE;/*g_t_surf.h_palette;*/
    }
    
    i4_ret = x_gl_surface_create(&t_surf_desc, &h_dest_surf);
    if (i4_ret != GLR_OK)
    {
        x_img_close(h_img);
        x_mem_free(pt_img);
        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_create() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }

    i4_ret = x_img_open_eng(JPEG_SW_ENG_NAME, NULL, NULL, h_img);
    if (i4_ret != IMGR_OK)
    {
        x_gl_obj_free(h_dest_surf);
        x_img_close(h_img);
        x_mem_free(pt_img);
        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_img_open_eng() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }

    i4_ret = x_img_sync_load(
                            h_img,
                            INTRA_IMG_PRIMARY_IMG_TYPE,
                            NULL,
                            IMG_ROTATE_CLOCKWS_0_OP_TYPE,
                            0,
                            0,
                            z_w,
                            z_h,
                            0,
                            0,
                            z_w,
                            z_h,
                            h_dest_surf);   

    if (i4_ret != IMGR_OK)
    {
        x_gl_obj_free(h_dest_surf);
        x_img_close(h_img);
        x_mem_free(pt_img);
        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_img_eng_sync_load() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }

    #if 0
    if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_CLUT8)
    {
        GL_RECT_T t_rect;
        HANDLE_T  h_pal_surf;
        
        t_surf_desc.ui4_width  = (UINT32)z_w;
        t_surf_desc.ui4_height = (UINT32)z_h;
        t_surf_desc.h_clipper  = NULL_HANDLE;
        t_surf_desc.h_palette  = NULL_HANDLE;/*g_t_surf.h_palette;*/
        t_surf_desc.e_clrmode  = GL_COLORMODE_ARGB_CLUT8;
    
        i4_ret = x_gl_surface_create(&t_surf_desc, &h_pal_surf);
        if (i4_ret != GLR_OK)
        {
            x_mem_free(pt_img);
            x_img_close(h_img);
            x_gl_obj_free(h_dest_surf);
            
            MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, x_gl_surface_create() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
            return NULL;
        }

        SET_RECT_BY_SIZE(&t_rect, 0, 0, z_w, z_h);
        i4_ret = grd_cnv_argb_16b_2_pal_8b(h_pal_surf, h_dest_surf, &t_rect, g_t_surf.pui1_pal_cnv_tbl);
        if (i4_ret != GLR_OK)
        {
            x_mem_free(pt_img);
            x_gl_obj_free(h_pal_surf);
            x_gl_obj_free(h_dest_surf);
            x_img_close(h_img);
        
            MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, grd_argb_cnv_32b_2_16b() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
            return NULL;
        }
        
        x_gl_obj_free(h_dest_surf);

        h_dest_surf = h_pal_surf;
    }
    #endif
    
    /* lock surface */
    i4_ret = x_gl_surface_lock(h_dest_surf, NULL, GL_SYNC, &t_lock_info);
    if (i4_ret != GLR_OK)
    {
        x_mem_free(pt_img);
        /*x_gl_obj_free(h_dec_surf);*/
        x_gl_obj_free(h_dest_surf);
        x_img_close(h_img);
        DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_lock() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
        return NULL;
    }

    #if 0
    if (g_t_surf.e_internal_col_mode == GL_COLORMODE_ARGB_CLUT8)
    {
        UINT32 i;
        UINT32 ui4_pitch;
        UINT8* pui1_buf;
        
        pui1_buf = (U8BIT*)(t_lock_info.at_subplane[0].pv_bits);
        ui4_pitch = t_lock_info.at_subplane[0].ui4_pitch;
        
        pt_img->col_buff = x_mem_alloc(z_w * z_h);
        if (pt_img->col_buff == NULL)
        {
            x_mem_free(pt_img);
            x_gl_surface_unlock(h_dest_surf, NULL);
            x_gl_obj_free(h_dest_surf);
            x_img_close(h_img);
            
            MHEG5_DBG_APP_OSD(("[MHEG5-OSD] %s #%d, alloc mem failed\n",__FUNCTION__, __LINE__));
            return NULL;
        }
        for (i = 0; i < z_h; i++)
        {
            x_memcpy(&(pt_img->col_buff[z_w * i]), &(pui1_buf[i * ui4_pitch]), z_w);
        }
        
        *((HANDLE_T*)(pt_img + 1)) = NULL_HANDLE;
        
        x_gl_surface_unlock(h_dest_surf, NULL);
        x_gl_obj_free(h_dest_surf);
    }
    else
    #endif
    {
        pt_img->col_buff = (U8BIT*)(t_lock_info.at_subplane[0].pv_bits);

        *((HANDLE_T*)(pt_img + 1)) = h_dest_surf;
    }
    x_img_close(h_img);
    DBG_INFO(("{MHEG5-OSD} %s, return S_IMAGE:%p, width:%d, height:%d, y_resolution:%d, pix_bytes:%d, opaque:%d, col_buff:%p\n", __FUNCTION__, pt_img, pt_img->width, pt_img->height, pt_img->y_resolution, pt_img->pix_bytes, pt_img->opaque, pt_img->col_buff));

    return pt_img;
    
}


void DVP_FreeJpg( S_IMAGE *image )
{
    HANDLE_T h_surf;

    DBG_INFO(("{MHEG5-OSD} %s, image:%p\n", __FUNCTION__, image));

    if (image == NULL)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid parameters)\n",__FUNCTION__, __LINE__));
        return ;
    }

    h_surf = *((HANDLE_T*)(image + 1));

    if (x_handle_valid(h_surf)) 
    {
        INT32 i4_ret;
        
        i4_ret = x_gl_surface_unlock(h_surf, NULL);

        if (i4_ret != GLR_OK)
        {
            DBG_ERROR((
                 "[MHEG5-OSD] %s #%d: !!!Panic!!! Unlock surface %lu failed(%ld)\r\n",
                  __FUNCTION__,
                  __LINE__,
                  h_surf,
                  i4_ret));
        }

        i4_ret = x_gl_obj_free(h_surf);
        if(i4_ret != GLR_OK)
        {
            DBG_ERROR((
                 "[MHEG5-OSD] ERR %s #%d: !!!Panic!!! Free surface %lu failed(%ld)\r\n",
                  __FUNCTION__,
                  __LINE__,
                  h_surf,
                  i4_ret));
        }
    }

    x_mem_free(image);
}



void* DVP_OSDCreateBmpSurf(
    U32BIT   s_width,
    U32BIT   s_height,
    U32BIT   i_width,
    U32BIT   i_height,
    S_IMAGE* image,
    U32BIT*  pitch,
    int*     tiled)
{
    HANDLE_T h_img_surf;
    HANDLE_T h_dest_surf;
    HANDLE_T h_tiling_surf;

    DBG_INFO(("{MHEG5-OSD} %s, s_width:%lu, s_height:%lu, i_width:%lu, i_height:%lu, image:%p, pitch:%p\n", __FUNCTION__, s_width, s_height, i_width, i_height,image, pitch));
    
    if (image == NULL || pitch == NULL || tiled == NULL)
    {
        DBG_ERROR(("[MHEG5-OSD] %s #%d, return error(invalid parameters)\n",__FUNCTION__, __LINE__));
        return NULL;
    }

    h_img_surf = *((HANDLE_T*)(image + 1));
 
    if (x_handle_valid(h_img_surf)) 
    {
        INT32 i4_ret;
        GL_LOCK_INFO_T      t_lock_info;

        /* unlock the surface first */
        i4_ret = x_gl_surface_unlock(h_img_surf, NULL);

        if (i4_ret != GLR_OK && i4_ret != GLR_SURFACE_NOT_LOCKED)
        {
            DBG_ERROR((
                 "{MHEG5 Stack} %s: !!!Panic!!! Unlock surface %lu failed(%ld)\r\n",
                  __FUNCTION__,
                  h_img_surf,
                  i4_ret));

            return NULL;
        }
        
        h_dest_surf = (HANDLE_T)(DVP_OSDCreateSurface((U16BIT)s_width, (U16BIT)s_height, FALSE, 0));

        if (h_dest_surf == NULL_HANDLE)
        {            
            x_gl_surface_lock(h_img_surf, NULL, GL_SYNC, &t_lock_info);

            DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_create() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
            
            return NULL;
        }
        if ((image->width != i_width) || (image->height != i_height))
        {
            h_tiling_surf = (HANDLE_T)(DVP_OSDCreateSurface((U16BIT)i_width, (U16BIT)i_height, FALSE, 0));
            
            if (h_tiling_surf == NULL_HANDLE)
            {
                DVP_OSDDestroySurface((void*)h_dest_surf);
                
                x_gl_surface_lock(h_img_surf, NULL, GL_SYNC, &t_lock_info);
            
                DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_create() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
                
                return NULL;
            }

            i4_ret = x_gl_surface_adv_stretch_blt(
                h_tiling_surf,
                h_img_surf,
                0,
                0,
                (UINT32) i_width,
                (UINT32) i_height,
                0,
                0,
                (UINT32) image->width,
                (UINT32) image->height,
                GL_SYNC);
            
            if (i4_ret != GLR_OK)
            {            
                DVP_OSDDestroySurface((void*)h_dest_surf);
                
                DVP_OSDDestroySurface((void*)h_tiling_surf);
                
                x_gl_surface_lock(h_img_surf, NULL, GL_SYNC, &t_lock_info);
            
                DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_adv_stretch_blt() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));
            
                return NULL;
            }
        }
        else
        {
            h_tiling_surf = h_img_surf;
        }

        i4_ret = x_gl_surface_patblt(
            h_dest_surf,
            h_tiling_surf,
            0,
            0,
            (UINT32) s_width,
            (UINT32) s_height,
            0,
            0,
            (UINT32) i_width,
            (UINT32) i_height,
            255,
            GL_SYNC);
        
        if (h_tiling_surf != h_img_surf)
        {
            /* Yes, you can die now, --! */
            DVP_OSDDestroySurface((void*)h_tiling_surf);
        }

        if (i4_ret != GLR_OK)
        {            
            DVP_OSDDestroySurface((void*)h_dest_surf);
            
            x_gl_surface_lock(h_img_surf, NULL, GL_SYNC, &t_lock_info);

            DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_adv_stretch_blt() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));

            return NULL;
        }

        i4_ret = x_gl_surface_lock(h_dest_surf, NULL, GL_SYNC, &t_lock_info);
        if (i4_ret != GLR_OK)
        {            
            DVP_OSDDestroySurface((void*)h_dest_surf);
            
            x_gl_surface_lock(h_img_surf, NULL, GL_SYNC, &t_lock_info);
            
            DBG_ERROR(("[MHEG5-OSD] %s #%d, x_gl_surface_lock() return error(%d)\n",__FUNCTION__, __LINE__, i4_ret));

            return NULL;
        }

        *pitch = t_lock_info.at_subplane[0].ui4_pitch;
        *tiled = TRUE;

        x_gl_surface_unlock(h_dest_surf, NULL);

        x_gl_surface_lock(h_img_surf, NULL, GL_SYNC, &t_lock_info);

        DBG_INFO(("{MHEG5-OSD} %s, created bmp surface:%p\n", __FUNCTION__, (VOID*)h_dest_surf));

        return ((VOID*)h_dest_surf);

    }
    else
    {
        return NULL;
    }

}
#endif



