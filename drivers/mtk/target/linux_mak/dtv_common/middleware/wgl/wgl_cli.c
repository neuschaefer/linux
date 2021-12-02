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
 * $RCSfile: wgl_cli.c,v $
 * $Revision: 
 * $Date: 
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/4 $
 * $SWAuthor: Allen Horng $
 * $MD5HEX: 37a575d022e9c1d5ac1e02f33e84d26b $
 *
 * Description: 
 *         This file implements CLI command table for Widget Manager.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "os/thread/thread_lib.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"

#include "wgl/_wgl.h"
#include "wgl/wgl.h"
#include "wgl/_wgl_cli.h"
#include "font/x_fe.h"
#include "wgl/wgl_sets.h"
#include "wgl/wgl_image.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
    
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL 
static INT32 _wgl_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv); 
static INT32 _wgl_cli_get_mem_status(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _wgl_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _wgl_cli_set_flash_time(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _wgl_cli_show_front_buf(INT32 i4_argc, const CHAR **pps_argv);
static INT32 _wgl_cli_get_string_size(INT32 i4_argc, const CHAR **pps_argv);
static INT32 _wgl_cli_trace_all_image_memory(INT32 i4_argc, const CHAR **pps_argv);
static INT32 _wgl_cli_trace_all_widget_memory(INT32 i4_argc, const CHAR **pps_argv);
static VOID _trace_single_widget_tree_memory(_WIDGET_T*  pt_widget);
static VOID _get_widget_type(_WIDGET_T*  pt_widget,CHAR* pstr_type);

/* Widget Manager command table */
static CLI_EXEC_T _at_wgl_cmd_tbl[] = 
{
    {CLI_GET_DBG_LVL_STR,   NULL,       _wgl_cli_get_dbg_level,     NULL,   CLI_GET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,       _wgl_cli_set_dbg_level,     NULL,   CLI_SET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    {"gms",                 NULL,       _wgl_cli_get_mem_status,    NULL,   "Get Widget Manager memory status",     CLI_SUPERVISOR},
    {"flash",               NULL,       _wgl_cli_set_flash_time,    NULL,   "Set widget update area flashing time", CLI_SUPERVISOR},
    {"sfb",                 NULL,       _wgl_cli_show_front_buf,    NULL,   "Show the front buffer information",    CLI_SUPERVISOR},
    {"gss",                 NULL,       _wgl_cli_get_string_size,   NULL,   "Get the string width",                 CLI_SUPERVISOR},
    {"wms",                 NULL,       _wgl_cli_trace_all_widget_memory,NULL,   "Get all widgets memory status", CLI_SUPERVISOR},
    {"imgms",               NULL,       _wgl_cli_trace_all_image_memory, NULL,   "Get all widgets image handle and surface memory status", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
#ifdef WGL_ANIM_SUPPORT
    END_OF_CLI_CMD_TBL,
#endif
    /* last entry is used as an ending indicator */
    END_OF_CLI_CMD_TBL
};

/* Widget Manager root command table */
static CLI_EXEC_T _at_wgl_root_cmd_tbl[] =
{
    {"wgl",     NULL,        NULL,      _at_wgl_cmd_tbl,      "Widget Manager commands",   CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static const SIZE_T _z_cmd_tbl      = sizeof(_at_wgl_cmd_tbl) / sizeof (CLI_EXEC_T) - 1;
static SIZE_T       _z_last_entry   = 8;

extern VOID wgl_show_front_buf_info(VOID);
#endif

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _wgl_cli_get_dbg_level
 *
 * Description: This API gets the current setting of IO Manager debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _wgl_cli_get_dbg_level(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32       i4_return;
    
    /* wgl.gdl */
    
	if (i4_argc != 1)
	{
        x_dbg_stmt("[WGL] wgl.gdl\n\r");
        return CLIR_OK;
	}    
	    
    i4_return = x_cli_show_dbg_level(wgl_get_dbg_level());

    return (i4_return);
}

static INT32 _wgl_cli_get_mem_status(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32   i4_return = CLIR_OK;
    INT32   i4_mem, i4_widget, i4_branch, i4_canvas, i4_app_info;
    
    /* wgl.gdl */
    
	if (i4_argc != 1)
	{
        x_dbg_stmt("[WGL] wgl.gms\n\r");
        return CLIR_OK;
	}
	    
    wgl_get_mem_status(&i4_mem, &i4_widget, &i4_branch, &i4_canvas, &i4_app_info);
    
    x_dbg_stmt("\n==============================\n");
    x_dbg_stmt("Memory:  %6d bytes\n", i4_mem);
    x_dbg_stmt("Widget:  %6d (%6d bytes)\n", i4_widget,   i4_widget   * sizeof(_WIDGET_T));
    x_dbg_stmt("Branch:  %6d (%6d bytes)\n", i4_branch,   i4_branch   * sizeof(_BRANCH_T));
    x_dbg_stmt("Canvas:  %6d (%6d bytes)\n", i4_canvas,   i4_canvas   * sizeof(_CANVAS_T));
    x_dbg_stmt("Appinfo: %6d (%6d bytes)\n", i4_app_info, i4_app_info * sizeof(_APP_INFO_T));
    x_dbg_stmt("==============================\n\n");

    return (i4_return);
}

/*-----------------------------------------------------------------------------
 * Name: _wgl_cli_set_dbg_level
 *
 * Description: This API sets the debug level of IO Manager.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _wgl_cli_set_dbg_level(
    INT32           i4_argc, 
    const CHAR**    pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;     
    
    /* wgl.sdl [level] */
    
    /* Check arguments */
	if ((i4_argc != 2) || 
	    (pps_argv == NULL))
	{
        x_dbg_stmt("[WGL] wgl.sdl [level]\n\r");
        x_dbg_stmt("[WGL]   [level]: n=none/e=error, a=api, i=info");
		return CLIR_OK;
	}    
        
    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);
    
    if (i4_return == CLIR_OK)
    {
        wgl_set_dbg_level(ui2_dbg_level);
    }
    else
    {
		x_dbg_stmt("[WGL] wgl.sdl [level]\n\r");
		x_dbg_stmt("[WGL]   [level]: n=none/e=error, a=api, i=info");
        i4_return = CLIR_OK;
    }

    return (i4_return);
}

static INT32 _wgl_cli_set_flash_time(
    INT32           i4_argc, 
    const CHAR**    pps_argv)
{
    /* wgl.flash [level] */
    
    /* Check arguments */
	if ((2 == i4_argc) && (NULL != pps_argv))
	{
        UINT32 ui4_flash_time;
        
        ui4_flash_time = (UINT32) x_strtoull(pps_argv[1], NULL, 10);
        wgl_set_flash_time(ui4_flash_time);
        return CLIR_OK;
	}
    
    x_dbg_stmt("[WGL] wgl.flash [time (ms)]\n\r");
    x_dbg_stmt("[WGL]   [time]: 0 = disable, otherwise = last for the specified period");
	return CLIR_INV_ARG;
}

static INT32 _wgl_cli_show_front_buf(INT32 i4_argc, const CHAR **pps_argv)
{
    wgl_show_front_buf_info();

    return CLIR_OK;
}
static INT32 _wgl_cli_get_string_size(INT32 i4_argc, const CHAR **pps_argv)
{
    UTF16_T w2s_str[] = {0x0646,0x0658,0x0644,0x0639,0x0645,0x0625,0x0625,0x0627,0x0648,0x0649,0x0636,0x0648,0x0647,0x0644,0x0644,0x0};
    UTF16_T w2s_str1[] = {0x0042,0x0053,0x0057,0x0044,0x0047,0x0053,0x004A,0x005B,0x0054,0x0059,0x0059,0x0057,0x006A,0x0048,0x004C,0x0044,0x0055,0x0058,0x0053,0x0064,0x0058,0x0};
    INT32 i4_length = sizeof(w2s_str)/sizeof(UTF16_T) - 1;
    INT32 i4_Height=0, i4_Width=0; 
    HFONT_T h_fnt = NULL_HANDLE;
	x_fe_create_font (SN_FONT_DEFAULT, FE_FNT_SIZE_SMALL, FE_FNT_STYLE_REGULAR, FE_CMAP_ENC_UNICODE, & h_fnt);
	
	x_dbg_stmt("[WGL] wgl.gss Source String:0x0646,0x0658,0x0644,0x0639,0x0645,0x0625,0x0625,0x0627,0x0648,0x0649,0x0636,0x0648,0x0647,0x0644,0x0644,0x0 \n\r");
	x_fe_get_string_size(h_fnt, &w2s_str[0], i4_length, &i4_Width, &i4_Height);
	x_dbg_stmt("[WGL] wgl.gss before reorder arabic string width:%d ,height:%d\n\r",i4_Width,i4_Height);
    x_wgl_get_string_size(h_fnt, &w2s_str[0], i4_length, &i4_Width, &i4_Height);
	x_dbg_stmt("[WGL] wgl.gss after reorder arabic string width:%d ,height:%d\n\r",i4_Width,i4_Height);
	
	i4_length = sizeof(w2s_str1)/sizeof(UTF16_T) - 1;
	
	x_dbg_stmt("[WGL] wgl.gss Source String:0x0042,0x0053,0x0057,0x0044,0x0047,0x0053,0x004A,0x005B,0x0054,0x0059,0x0059,0x0057,0x006A,0x0048,0x004C,0x0044,0x0055,0x0058,0x0053,0x0064,0x0058,0x0 \n\r");
	x_fe_get_string_size(h_fnt, &w2s_str1[0], i4_length, &i4_Width, &i4_Height);
	x_dbg_stmt("[WGL] wgl.gss before reorder arabic string width:%d ,height:%d\n\r",i4_Width,i4_Height);
    x_wgl_get_string_size(h_fnt, &w2s_str1[0], i4_length, &i4_Width, &i4_Height);
	x_dbg_stmt("[WGL] wgl.gss after reorder arabic string width:%d ,height:%d\n\r",i4_Width,i4_Height);
    return CLIR_OK;
}


VOID* _wgl_widget_mem_alloc(
    WGL_MTX_OBJ_T* pt_handle,
    SIZE_T s_size
)
{
    VOID *pv_mem = NULL;
    if(pt_handle == NULL)
    {
        x_dbg_stmt("[WGL] ERR: _wgl_widget_mem_alloc() input pt_handle is NULL \n");
        return NULL;
    }
    pv_mem = x_mem_alloc(s_size);
    if(pv_mem != NULL)
    {
        pt_handle->ui4_mem_size +=  s_size;
    }
    return pv_mem;
}

VOID _wgl_widget_mem_free(
    WGL_MTX_OBJ_T* pt_handle,
    SIZE_T s_size,
    VOID* pv_mem)
{
    if(pt_handle == NULL)
    {
           x_dbg_stmt("[WGL] ERR: _wgl_widdget_mem_free() input pt_handle is NULL \n");
           return;
    }
    pt_handle->ui4_mem_size -=  s_size;
    x_mem_free(pv_mem);
}
extern DLIST_APP_T      _t_app_lst;
extern SLIST_CANVAS_T   _t_canvas_lst;

static UINT32 g_ui4_child_layer;
static UINT32 g_ui4_app_total_size;

#define CHILD_PER_LAYER_SPACE_NUM  (UINT32)10

static INT32 _wgl_cli_trace_all_image_memory(INT32 i4_argc, const CHAR **pps_argv)
{
    return wgl_img_print_mem_log();
}
static INT32 _wgl_cli_trace_all_widget_memory(INT32 i4_argc, const CHAR **pps_argv)
{
    _APP_INFO_T*    pt_app_info;
   _CANVAS_T*  pt_canvas;
    GL_LOCK_INFO_T _t_lock_info;
        
    SLIST_FOR_EACH(pt_canvas, &_t_canvas_lst, t_next)
    {
       UINT32 ui4_canvas_surf_mem_size = 0;
       
       x_dbg_stmt("\nCanvas Handle: 0x%x, Plane Handle: 0x%x \n",pt_canvas->h_this,pt_canvas->h_plane);

       if( pt_canvas->pt_ref== NULL)/*single canvas or reference canvas*/
       {
            if(pt_canvas->pt_shadow != NULL)
            {
                x_dbg_stmt("          Reference Canvas with shadow canvas: 0x%x \n",pt_canvas->pt_shadow->h_this);
            }else
            {
                
                x_dbg_stmt("          Reference Canvas with shadow canvas: NULL handle \n");
            }
        }else if(pt_canvas->pt_shadow == NULL)/*shadow canvas*/
        {
            x_dbg_stmt("          Shadow Canvas with reference canvas: 0x%x \n",pt_canvas->pt_ref->h_this);
        }
        if(pt_canvas->b_new_surface)
        {
            if(pt_canvas->h_surf_front != NULL_HANDLE)
            {
               if (x_gl_surface_lock(pt_canvas->h_surf_front, NULL, GL_SYNC, &_t_lock_info) != GLR_OK)
                {
                    x_dbg_stmt("Error: x_gl_surface_lock(pt_canvas->h_surf_front) return failed\n ");
                    return CLIR_OK;
                }
               x_dbg_stmt("          Canvas with font surface bitmap size: %d bytes \n",(_t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height));
               x_gl_surface_unlock(pt_canvas->h_surf_front, NULL);
               ui4_canvas_surf_mem_size += _t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height;
            }
            if(pt_canvas->h_surf_back!= NULL_HANDLE)
            {
               if (x_gl_surface_lock(pt_canvas->h_surf_back, NULL, GL_SYNC, &_t_lock_info) != GLR_OK)
                {
                    x_dbg_stmt("Error: x_gl_surface_lock(pt_canvas->h_surf_back) return failed\n ");
                    return CLIR_OK;
                }
               x_dbg_stmt("          Canvas with back surface bitmap size: %d bytes \n",(_t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height));
               x_gl_surface_unlock(pt_canvas->h_surf_back, NULL);
               ui4_canvas_surf_mem_size += _t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height;
            }
            if(pt_canvas->h_surf_render!= NULL_HANDLE)
            {
               if (x_gl_surface_lock(pt_canvas->h_surf_render, NULL, GL_SYNC, &_t_lock_info) != GLR_OK)
                {
                    x_dbg_stmt("Error: x_gl_surface_lock(pt_canvas->h_surf_render) return failed\n ");
                    return CLIR_OK;
                }
               x_dbg_stmt("          Canvas with render surface bitmap size: %d bytes \n",(_t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height));
               x_gl_surface_unlock(pt_canvas->h_surf_render, NULL);
               ui4_canvas_surf_mem_size += _t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height;
            }
            if(pt_canvas->h_surf_anim!= NULL_HANDLE)
            {
               if (x_gl_surface_lock(pt_canvas->h_surf_anim, NULL, GL_SYNC, &_t_lock_info) != GLR_OK)
                {
                    x_dbg_stmt("Error: x_gl_surface_lock(pt_canvas->h_surf_anim) return failed\n ");
                    return CLIR_OK;
                }
               x_dbg_stmt("          Canvas with anim surface bitmap size: %d bytes \n",(_t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height));
               x_gl_surface_unlock(pt_canvas->h_surf_anim, NULL);
               ui4_canvas_surf_mem_size += _t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height;
            }

#ifndef MW_OSD_COMPRESSION_SHINK
            if(pt_canvas->h_surf_compress!= NULL_HANDLE)
            {
               if (x_gl_surface_lock(pt_canvas->h_surf_compress, NULL, GL_SYNC, &_t_lock_info) != GLR_OK)
                {
                    x_dbg_stmt("Error: x_gl_surface_lock(pt_canvas->h_surf_compress) return failed\n ");
                    return CLIR_OK;
                }
               x_dbg_stmt("          Canvas with compress surface bitmap size: %d bytes \n",(_t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height));
               x_gl_surface_unlock(pt_canvas->h_surf_compress, NULL);
               ui4_canvas_surf_mem_size += _t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height;
            }
            if(pt_canvas->h_surf_compress_back!= NULL_HANDLE)
            {
               if (x_gl_surface_lock(pt_canvas->h_surf_compress_back, NULL, GL_SYNC, &_t_lock_info) != GLR_OK)
                {
                    x_dbg_stmt("Error: x_gl_surface_lock(pt_canvas->h_surf_compress_back) return failed\n ");
                    return CLIR_OK;
                }
               x_dbg_stmt("          Canvas with compress back surface bitmap size: %d bytes \n",(_t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height));
               x_gl_surface_unlock(pt_canvas->h_surf_compress_back, NULL);
               ui4_canvas_surf_mem_size += _t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height;
            }
#endif
            x_dbg_stmt("Canvas with all surfaces bitmap size: %d bytes \n",ui4_canvas_surf_mem_size);
       }
    }
    
    DLIST_FOR_EACH(pt_app_info, &_t_app_lst, t_link)
    {
       OS_THREAD_T     *pt_thread = NULL;
       _BRANCH_T*                  pt_branch;
       
       g_ui4_app_total_size = 0;
        /*1, print per app's thread name*/
        if (handle_get_obj(pt_app_info->h_thread , (VOID **)&pt_thread) == HR_OK)
        {
            if (pt_thread != NULL)
            {
                x_dbg_stmt("APP: %s \n",pt_thread->s_name);
            }
        }
        /*2,print app's canvas and widgets memory info*/
        DLIST_FOR_EACH(pt_branch, &(pt_app_info->t_branch_lst), t_link_on_app)
        {
           _WIDGET_T*  pt_widget;
           _CANVAS_T*  pt_canvas = pt_branch->pt_canvas;
           /*2.1 print app's canvas info*/
           x_dbg_stmt("\nCanvas Handle: 0x%x, Plane Handle: 0x%x \n",pt_canvas->h_this,pt_canvas->h_plane);
           /*2.2 print app's widgets related to one canvas memory info*/
           for (pt_widget = pt_branch->pt_first; pt_widget; pt_widget = pt_widget->pt_sibling)
           {
                if (!(pt_widget->ui2_state& WS_DESTROYED))
                {
                    g_ui4_child_layer = 0;
                    _trace_single_widget_tree_memory(pt_widget);
                }
           }
           
        }
        
        x_dbg_stmt("\nAPP: %s, widgets total memory size %d bytes \n",pt_thread->s_name,g_ui4_app_total_size);

    }
    return CLIR_OK;
}

static VOID _trace_single_widget_tree_memory(
    _WIDGET_T*  pt_widget)
{
    _WIDGET_T*  pt_child;
#ifdef WGL_ANIM_SUPPORT    
    WGL_MTX_OBJ_T*  pt_wgt_mem;
#endif
    UINT32 ui4_wgt_total_mem; 
    UINT32 ui4_wgt_mem = 0,ui4_idx =0;
    CHAR as_widget_type[20];
   
    /*calculate widget totallyl allocated memory size*/
    ui4_wgt_total_mem = pt_widget->ui4_mem_size;
    
    x_wgl_do_cmd(pt_widget->h_this,WGL_CMD_GL_GET_ALLOC_MEM,WGL_PACK(&ui4_wgt_mem),NULL);
    ui4_wgt_total_mem += ui4_wgt_mem;
#ifdef WGL_ANIM_SUPPORT    
    pt_wgt_mem = (WGL_MTX_OBJ_T* )pt_widget->t_anim;
    ui4_wgt_total_mem += pt_wgt_mem->ui4_mem_size;
#endif
    g_ui4_app_total_size += ui4_wgt_total_mem;

    for(;ui4_idx<g_ui4_child_layer*CHILD_PER_LAYER_SPACE_NUM;ui4_idx++)
    {
        x_dbg_stmt(" ");
    }
    x_memset((VOID*)as_widget_type,(UINT8)0,20*sizeof(CHAR));
    _get_widget_type(pt_widget,(CHAR*)as_widget_type);
    x_dbg_stmt("Widget Handle: 0x%x, Type: %s, Memory Size: %d bytes \n",pt_widget->h_this,as_widget_type,ui4_wgt_total_mem);
   
    g_ui4_child_layer++;
    /* get memory info for all children of pt_widget */
    for (pt_child = pt_widget->pt_child; pt_child; pt_child = pt_child->pt_sibling)
    {
        if (!(pt_child->ui2_state& WS_DESTROYED))
        {
            _trace_single_widget_tree_memory(pt_child);
        }
    }
    g_ui4_child_layer--;
}

static VOID _get_widget_type(_WIDGET_T*  pt_widget,CHAR* pstr_type)
{
    HANDLE_TYPE_T   e_type;

    if (HR_OK != handle_get_type_obj(pt_widget->h_this, &e_type, (VOID**) &pt_widget))
    {
        x_dbg_stmt("{WGL} ERR: Invalid widget handle (%d) at L%d\r\n\r\n", WGLR_INV_HANDLE, __LINE__);
    }
    if(HT_WGL_WIDGET_FRAME == e_type )
    {
        x_strcpy(pstr_type,"Frame");
    }else if(HT_WGL_WIDGET_ICON== e_type )
    {
        x_strcpy(pstr_type,"Icon");
    }else if(HT_WGL_WIDGET_TEXT== e_type )
    {
        x_strcpy(pstr_type,"Text");
    }else if(HT_WGL_WIDGET_BUTTON== e_type )
    {
        x_strcpy(pstr_type,"Button");
        
    }else if(HT_WGL_WIDGET_PROGRESS_BAR== e_type )
    {
        x_strcpy(pstr_type,"Progress Bar");
    }else if(HT_WGL_WIDGET_EDIT_BOX== e_type )
    {
        x_strcpy(pstr_type,"Edit Box");
    }else if(HT_WGL_WIDGET_LIST_BOX== e_type )
    {
        x_strcpy(pstr_type,"List Box");
    }else if(HT_WGL_WIDGET_CC== e_type )
    {
        x_strcpy(pstr_type,"CC");
    }else if(HT_WGL_WIDGET_TKTP== e_type )
    {
        x_strcpy(pstr_type,"TKTP");
        
    }else if(HT_WGL_WIDGET_HTS== e_type )
    {
        x_strcpy(pstr_type,"HTS");
        
    }else if(HT_WGL_WIDGET_SBTL== e_type )
    {
        
        x_strcpy(pstr_type,"SBTL");
    }else if(HT_WGL_WIDGET_TTX== e_type )
    {
        x_strcpy(pstr_type,"TTX");
        
    }else if(HT_WGL_WIDGET_GROUP== e_type )
    {
        x_strcpy(pstr_type,"Group");
        
    }else if(HT_WGL_WIDGET_CC_ISDB== e_type )
    {
        x_strcpy(pstr_type,"CC_ISDB");
        
    }else 
    {
        x_strcpy(pstr_type,"Unkown");
    }
}

#endif 

/*-----------------------------------------------------------------------------
 * Name: wgl_cli_init
 *
 * Description: This API initializes IO Manager CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: WGLR_OK                     Routine successful.
 *          WGLR_INIT_FAILED            CLI operation is failed.
 ----------------------------------------------------------------------------*/
INT32 wgl_cli_init(VOID)
{
#ifdef CLI_LVL_ALL 
    INT32       i4_return;    
    
    /* Attach IO Manager CLI command table to CLI */
    i4_return = x_cli_attach_cmd_tbl(_at_wgl_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_GUI);
    
    if (i4_return != CLIR_OK)
    {
        return WGLR_INIT_FAILED;
    }
#endif

    return WGLR_OK;
}

INT32 wgl_cli_reg_sub_table(const CLI_EXEC_T* pt_exec)
{
#ifdef CLI_LVL_ALL    
    if (!pt_exec)
    {
        return WGLR_INV_ARG;
    }

    if (_z_last_entry >= _z_cmd_tbl)
    {
        return WGLR_BUFFER_NOT_ENOUGH;
    }

    _at_wgl_cmd_tbl[_z_last_entry++] = *pt_exec;
#endif

    return WGLR_OK;
}
