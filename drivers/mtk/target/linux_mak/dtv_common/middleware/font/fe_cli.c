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
 * $RCSfile: fe_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/7 $
 * $SWAuthor: Ben Tsai $
 * $MD5HEX: a3a3d4cc84f9d41fa9b90270f624d7ca $
 *
 * Description:
 *         This file implements CLI command table for Font Engine.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#include "cli/x_cli.h"
#include "font/fe.h"
#include "font/x_fe.h"
#include "font/cache.h"
#include "graphic/x_gl.h"
#include "handle/handle.h"
#include "graphic/u_gl.h"
#ifndef LINUX_TURNKEY_SOLUTION_PHASE2
#include "svctx/x_svctx.h"
#endif


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL


#define FILE_FONT_PATH_PREFIX     "/mnt/usb/Mass-000/"

static GL_HSURFACE_T        _h_disp_surf    = NULL_HANDLE;
static GL_HSCREEN_T         _h_screen       = NULL_HANDLE;
static GL_HPLANE_T          _h_plane;
static HANDLE_T             _h_view;
static UINT32               _ui4_viewport;
static GL_DISPLAY_OPT_T     _t_disp_opt;
static UINT32               _ui4_panel_width;
static UINT32               _ui4_panel_height;


static INT32 _fe_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fe_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fe_cli_show_font_list(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fe_cli_show_handle_number(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fe_cli_set_resolution(INT32 i4_argc, const CHAR** pps_argv);
static INT32 fe_create_surface(VOID);
static INT32 fe_flip_surface(VOID);
static INT32 fe_test_single_file_font(INT32 i4_argc,const CHAR** pps_argv);
static INT32 fe_test_custom_size(INT32 i4_argc, const CHAR** pps_argv);
static INT32 fe_test_multi_file_font(INT32 i4_argc,const CHAR** pps_argv);
static INT32 file_font_release_resource(INT32 i4_argc, const CHAR** pps_argv);
static INT32 chg_menu_font(INT32 i4_argc, const CHAR** pps_argv);



#ifndef FE_DISABLE_CACHE
static INT32 _fe_cli_get_cache_info(INT32 i4_argc, const CHAR** pps_argv);
#endif /*FE_DISABLE_CACHE*/


/* Font Engine command table */
static CLI_EXEC_T at_fe_cmd_tbl[] =
{
 	{ "gdl",   NULL,  _fe_cli_get_dbg_level,       NULL,  "Get debug level",     CLI_SUPERVISOR},
	{ "sdl",   NULL,  _fe_cli_set_dbg_level,       NULL,  "Set debug level",     CLI_SUPERVISOR},
	{ "sfl",   NULL,  _fe_cli_show_font_list,      NULL,  "Show font list",      CLI_SUPERVISOR},
	{ "shn",   NULL,  _fe_cli_show_handle_number,  NULL,  "Show handle number",  CLI_SUPERVISOR},
#ifndef FE_DISABLE_CACHE
	{ "gci",   NULL,  _fe_cli_get_cache_info,      NULL,  "Get cache info",      CLI_SUPERVISOR},
#endif /*FE_DISABLE_CACHE*/
	{ "sdpi",  NULL,  _fe_cli_set_resolution,      NULL,  "Set resolution",      CLI_SUPERVISOR},
	{ "sff",   NULL,  fe_test_single_file_font,    NULL,  "Single file font",    CLI_SUPERVISOR},
    { "tcs",   NULL,  fe_test_custom_size,         NULL,  "Custom Size Test",    CLI_SUPERVISOR},
    { "mff",   NULL,  fe_test_multi_file_font,     NULL,  "Multi file font",     CLI_SUPERVISOR},
	{ "rfr",   NULL,  file_font_release_resource,  NULL,  "Multi file font",     CLI_SUPERVISOR},
	{ "cmf",   NULL,  chg_menu_font,               NULL,  "Change Menu font",    CLI_SUPERVISOR},
	
    END_OF_CLI_CMD_TBL
};

/* Font Engine root command table */
static CLI_EXEC_T at_fe_root_cmd_tbl[] =
{
	{"fe", NULL, NULL, at_fe_cmd_tbl, "Font Engine commands", CLI_SUPERVISOR},
	END_OF_CLI_CMD_TBL
};


/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _fe_cli_get_dbg_level
 *
 * Description: This API gets the current setting of Font Engine debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _fe_cli_get_dbg_level(INT32        i4_argc,
                                   const CHAR** pps_argv)
{
    INT32       i4_return;

    /* fe.gdl */

	if (i4_argc != 1)
	{
		x_dbg_stmt("fe.gdl\n\r");
		return CLIR_INV_CMD_USAGE;
	}

    i4_return = x_cli_show_dbg_level(fe_get_dbg_level());

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _fe_cli_set_dbg_level
 *
 * Description: This API sets the debug level of Font Engine.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _fe_cli_set_dbg_level(INT32        i4_argc,
                                   const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    /* fe.sdl [level] */

    /* Check arguments */
	if ((i4_argc != 2) ||
	    (pps_argv == NULL))
	{
		x_dbg_stmt("fe.sdl [level]\n\r");
		x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
		return CLIR_INV_CMD_USAGE;
	}

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);
    if (i4_return == CLIR_OK)
    {
        if (fe_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }
        else
        {
            i4_return = x_cli_show_dbg_level(fe_get_dbg_level());
        }
    }
    else
    {
		x_dbg_stmt("fe.sdl [level]\n\r");
		x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
		return CLIR_INV_CMD_USAGE;
    }

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _fe_cli_show_font_list
 *
 * Description: This API shows system font list.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _fe_cli_show_font_list(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    /* fe.sfl */
    FE_FNT_RSC_T *pt_rsc;
    FE_FNT_OBJ_T *pt_obj;

    pt_rsc = fe_get_first_rsc();

    if(pt_rsc != NULL)
    {
        fe_fnt_rsc_lock ();
        do
        {
            INT32   i4_hdl_cnt = 0;

            SLIST_FOR_EACH(pt_obj, &pt_rsc->t_obj_link_head, t_obj_lnk)
            {
                ++i4_hdl_cnt;
            }
            x_dbg_stmt("Font = %s (%d objects associated)\n", pt_rsc->ps_fnt_name, i4_hdl_cnt);
        }while( (pt_rsc=pt_rsc->t_rsc_lnk.pt_next) != NULL );
        fe_fnt_rsc_unlock ();
    }
#ifdef DEBUG
    x_dbg_stmt("Memory usage = %d bytes\n", fe_dbg_get_mem_usage());
#ifndef FE_DISABLE_CACHE
    x_dbg_stmt("Cache usage = %d bytes\n", fe_dbg_get_cache_usage());
#endif
#endif

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _fe_cli_show_handle_number
 *
 * Description: This API shows system font handle number.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _fe_cli_show_handle_number(INT32        i4_argc,
                                        const CHAR** pps_argv)
{
    /* fe.shn */
    x_dbg_stmt("Font handle number = %d\n", fe_fnt_stat_get());

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _fe_cli_get_cache_info
 *
 * Description: This API shows the cache info of Font Engine.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
#ifndef FE_DISABLE_CACHE
static INT32 _fe_cli_get_cache_info(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    /* fe.gci */
    UINT32  ui4_avail_cache;
    UINT32  ui4_accesss,ui4_misss;
    ui4_avail_cache = fe_cache_get_mem_size();
    fe_cache_get_hit_rate(&ui4_accesss, &ui4_misss);

    x_dbg_stmt("Total Cache Memory = %d bytes\n", FNT_CACHE_MEM_SIZE*1024);
	x_dbg_stmt("Free Cache Memory = %d bytes\n", ui4_avail_cache);
    x_dbg_stmt("Cache Access = %d    Cache Miss = %d\n", ui4_accesss, ui4_misss);
    x_dbg_stmt("Cache Member Size = %d\n", sizeof(FE_CACHE_OBJ_T));
    return CLIR_OK;
}

#endif /*FE_DISABLE_CACHE*/


/*----------------------------------------------------------------------------
 * Name:    _atoi
 *
 * Description: Convert a number string to a number.
 *
 * Inputs:  ps     null ('\0') terminated character string.
 *
 *
 * Outputs: -
 *
 * Returns: sum of the number string.
 *
 ----------------------------------------------------------------------------*/
static INT32 _atoi( const CHAR* ps )
{
    INT32  i4_total;

    i4_total = 0;
    while ( *ps != 0 )
    {
        if ( *ps >= '0' && *ps <= '9' )
        {
            /* accumulate digit */
            i4_total = 10 * i4_total + (*ps - '0');
            ps++; /* get next char */
        }
        else /* not a digit char */
        {
            return (-1);
        }
    }

    return i4_total;
}


/*-----------------------------------------------------------------------------
 * Name: _fe_cli_set_resolution
 *
 * Description: This API sets the resolution (dpi).
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _fe_cli_set_resolution(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    /* Check arguments */
	if ((i4_argc != 3) ||
	    (pps_argv == NULL))
	{
		x_dbg_stmt("fe.sdpi [horizontal dpi] [vertical dpi]\n\r");
		return CLIR_INV_CMD_USAGE;
	}

    x_dbg_stmt("Set font resolution to [%u, %u] dpi.\n", _atoi(pps_argv[1]), _atoi(pps_argv[2]));
    fe_set_resolution(_atoi(pps_argv[1]), _atoi(pps_argv[2]));

    return CLIR_OK;
}



/*-----------------------------------------------------------------------------
 * Name: fe_cli_init
 *
 * Description: This API initializes Font Engine CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: FER_OK                     Routine successful.
 *          FER_CLI_ERROR              CLI operation is failed.
 ----------------------------------------------------------------------------*/
INT32 fe_cli_init(VOID)
{
    INT32       i4_return;

    /* Attach Font Engine CLI command table to CLI */
    i4_return = x_cli_attach_cmd_tbl(at_fe_root_cmd_tbl,
                                     CLI_CAT_MW,
                                     CLI_GRP_NONE);

    if (i4_return != CLIR_OK)
    {
        return FER_CLI_ERROR;
    }
    return FER_OK;
}

#if 0

static INT32 _hw_screen_nfy_fct(    
   HANDLE_T                    h_obj,  
   GL_COND_T                   e_nfy_cond, 
   VOID*                       pv_nfy_tag, 
   UINT32                      ui4_data)
{    
   return GLR_OK;
}

#endif

static INT32 fe_create_surface(VOID)
{

    INT32               i4_ret;
    GL_SURFACE_DESC_T   t_desc;
    GL_COLOR_T          t_color;
    #ifndef LINUX_TURNKEY_SOLUTION
    HANDLE_T            h_svctx;
    #endif
    UINT16              ui2_plane_idx = 1;
    GL_PLANE_CAPS_T     t_plane_caps;  
    GL_SCREEN_STATUS_T	scr_status;

    /* stop AV */
    #ifndef LINUX_TURNKEY_SOLUTION
    i4_ret = x_svctx_open("main_svctx", &h_svctx);

    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt("svctx open failed. (%d)\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }
    
    i4_ret = x_svctx_sync_stop_svc(h_svctx, DEFAULT_STOP_SVC_TIMEOUT);

    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt("svctx stop failed. (%d)\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

   
    #endif

    /* open screen related */
    i4_ret = x_gl_screen_open(SN_PRES_MAIN_DISPLAY, NULL, NULL, &_h_screen);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("screen create failed. (%d)\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_gl_screen_get_status(_h_screen, &scr_status);
	x_dbg_stmt("SCR_STATUS:%d\n", scr_status );

    i4_ret = x_gl_screen_get_panel_size(_h_screen, &_ui4_panel_width, &_ui4_panel_height);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("get panel size failed. (%d)\n", i4_ret);

        return CLIR_CMD_EXEC_ERROR;
    }
    
    

    while(1)	
	{	
	    /* enum how many osd plane to screen */
		i4_ret=x_gl_plane_enum(_h_screen, ui2_plane_idx, &t_plane_caps);
		if(i4_ret != GLR_OK)
        {
            x_dbg_stmt("Enum osd planes failed. (%d)\n", i4_ret);
			return i4_ret;
		}
		
		i4_ret = x_gl_plane_open(_h_screen, ui2_plane_idx, &_h_plane);
		
		if(i4_ret == GLR_OK)
		{
			i4_ret = x_gl_plane_link(_h_plane, _h_screen, TRUE);
			
			if(i4_ret == GLR_OK)
			{
                break;
			}
			else
			{
				ui2_plane_idx ++;
				continue;
			}
		}	
		ui2_plane_idx ++;
	}	 


     /* create surfaces */
    t_desc.ui4_width =  800;
	t_desc.ui4_height=  600;
	t_desc.e_clrmode =  GL_COLORMODE_ARGB_D8888;
	t_desc.h_clipper =  NULL_HANDLE;
	t_desc.h_palette =  NULL_HANDLE;
    i4_ret = x_gl_surface_create(&t_desc, &_h_disp_surf);
    
    if (i4_ret != GLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }    

    t_color.u1.r = 0;
    t_color.u2.g = 0;
    t_color.u3.b = 255;
    t_color.a    = 255;

    x_gl_surface_fill(_h_disp_surf, &t_color, GL_SYNC);  

    /* i4_ret = x_gl_view_create(1024, 576, FALSE, &_h_view); */
    i4_ret = x_gl_view_create(_ui4_panel_width, _ui4_panel_height, FALSE, &_h_view);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("view create failed. (%d)\n", i4_ret);
    }

    /* viewport */
    _t_disp_opt.e_flag         = GL_DISP_OPT_POSITION | GL_DISP_OPT_BLEND_OPTION/* | GL_DISP_OPT_RESIZE | GL_DISP_OPT_SURF_OFFSET*/;
    _t_disp_opt.t_pos.i4_x     = 0;
    _t_disp_opt.t_pos.i4_y     = 0;
    _t_disp_opt.e_blend_option = GL_BLEND_OPT_NONE;
    _t_disp_opt.ui1_alpha      = 255;

    i4_ret = x_gl_view_create_viewport(_h_view, _h_disp_surf, &_t_disp_opt, &_ui4_viewport);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("viewport create failed. (%d)\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }  
     

    return CLIR_OK;
}



static INT32 fe_flip_surface(VOID)
{
    INT32  i4_ret;
    
    i4_ret = x_gl_plane_flip(_h_plane, _h_view, TRUE);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("flip failed. (%d)\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }   


    return CLIR_OK;
}


static INT32 fe_test_single_file_font(INT32        i4_argc,
                                   const CHAR** pps_argv)
{
    INT32  i4_ret;
    GL_COLOR_T          t_color;
    CHAR                s_abs_path[64]={0};
    UTF16_T            w2s_text[]={0x69,0x70,0x71,0x72,0x69,0x70,0x71,0x72,0x69,0x70,0x71,0x72,0x00};
    HFONT_T			    h_fnt;

   if (i4_argc < 2)
    {
         x_dbg_stmt("otftest filename \n\r");
    }
    
   file_font_release_resource(0,NULL);

   i4_ret = fe_create_surface();
   if (i4_ret != FER_OK)
    {
        x_dbg_stmt("fe_create_surface fail:(%d)\n", i4_ret);
        return i4_ret;
    }
    
   t_color.u1.y = 0;
   t_color.u2.u = 0;
   t_color.u3.v = 0;
   t_color.a    = 255;

    /* get filename */
   x_sprintf(s_abs_path,"%s%s",FILE_FONT_PATH_PREFIX, pps_argv[1]);

   i4_ret = x_fe_add_file_font(pps_argv[1],s_abs_path,12,24,36);   
   if (i4_ret != FER_OK)
    {
        x_dbg_stmt("Add file font fail:%s. (%d)\n",s_abs_path, i4_ret);
        return i4_ret;
    }
   
   i4_ret = x_fe_create_font(pps_argv[1],
							FE_FNT_SIZE_LARGE,
							FE_FNT_STYLE_REGULAR,
							FE_CMAP_ENC_UNICODE,
							&h_fnt);
    if(i4_ret != GLR_OK){
		x_dbg_stmt("Error create font failed\n");
        return i4_ret;
    }   
   i4_ret = x_gl_surface_draw_ustring(_h_disp_surf,100,50,w2s_text,sizeof(w2s_text)/sizeof(UTF16_T)-1,h_fnt,&t_color,NULL,NULL);
    if (i4_ret != FER_OK)
    {
        x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
        goto Exit;
    }
   i4_ret = fe_flip_surface();
   if (i4_ret != FER_OK)
   {
        x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
        goto Exit;
   } 
   Exit:
   i4_ret = x_fe_delete_font(h_fnt);
   if (i4_ret != FER_OK)
   {
     x_dbg_stmt("Delete Font Error %s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
   }
   i4_ret = x_fe_remove_font(pps_argv[1], X_FE_OPTION_FORCE);
   if (i4_ret != FER_OK)
   {
     x_dbg_stmt("Remove Font Error %s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
   }
   return i4_ret;

}

static INT32 fe_test_custom_size(INT32        i4_argc,
                                   const CHAR** pps_argv)
{
    INT32  i4_ret;
    GL_COLOR_T          t_color;
    CHAR                s_abs_path[64]={0};
    UTF16_T            w2s_text[]={0x69,0x70,0x71,0x72,0x69,0x70,0x71,0x72,0x69,0x70,0x71,0x72,0x00};
    HFONT_T			    h_fnt;

   if (i4_argc < 2)
    {
         x_dbg_stmt("otftest filename \n\r");
    }
    
   file_font_release_resource(0,NULL);

   i4_ret = fe_create_surface();
   if (i4_ret != FER_OK)
    {
        x_dbg_stmt("fe_create_surface fail:(%d)\n", i4_ret);
        return i4_ret;
    }
    
   t_color.u1.y = 0;
   t_color.u2.u = 0;
   t_color.u3.v = 0;
   t_color.a    = 255;

    /* get filename */
   x_sprintf(s_abs_path,"%s%s",FILE_FONT_PATH_PREFIX, pps_argv[1]);

   i4_ret = x_fe_add_file_font(pps_argv[1],s_abs_path,12,24,36);   
   if (i4_ret != FER_OK)
    {
        x_dbg_stmt("Add file font fail:%s. (%d)\n",s_abs_path, i4_ret);
        return i4_ret;
    }
   
   i4_ret = x_fe_create_font(pps_argv[1],
							FE_FNT_SIZE_LARGE,
							FE_FNT_STYLE_REGULAR,
							FE_CMAP_ENC_UNICODE,
							&h_fnt);
    if(i4_ret != GLR_OK){
		x_dbg_stmt("Error create font failed\n");
        goto  Exit;
    }   
   i4_ret = x_gl_surface_draw_ustring(_h_disp_surf,100,50,w2s_text,sizeof(w2s_text)/sizeof(UTF16_T)-1,h_fnt,&t_color,NULL,NULL);
    if (i4_ret != FER_OK)
    {
        x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
        goto Exit;
    }
   i4_ret = fe_flip_surface();
   if (i4_ret != FER_OK)
   {
        x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
        goto Exit;
   } 
   i4_ret = x_fe_set_size(h_fnt , FE_FNT_SIZE_CUSTOM);
   if (i4_ret != FER_OK)
    {
        x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
        goto Exit;
    }
   i4_ret = x_fe_set_custom_size(h_fnt , 28);
   if (i4_ret != FER_OK)
    {
        x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
        goto Exit;
    }
   i4_ret = x_gl_surface_draw_ustring(_h_disp_surf,100,110,w2s_text,sizeof(w2s_text)/sizeof(UTF16_T)-1,h_fnt,&t_color,NULL,NULL);
    if (i4_ret != FER_OK)
    {
        x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
        goto Exit;
    }
   i4_ret = fe_flip_surface();
   if (i4_ret != FER_OK)
   {
        x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
        goto Exit;
   } 
   Exit:
   i4_ret = x_fe_delete_font(h_fnt);
   if (i4_ret != FER_OK)
   {
     x_dbg_stmt("Delete Font Error %s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
   }
   i4_ret = x_fe_remove_font(pps_argv[1], X_FE_OPTION_FORCE);
   if (i4_ret != FER_OK)
   {
     x_dbg_stmt("Remove Font Error %s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
   }
   return i4_ret;

}

static INT32 fe_test_multi_file_font(INT32 i4_argc,const CHAR** pps_argv)
{
    INT32  i4_ret;
    GL_COLOR_T          t_color;    
    CHAR                s_abs_path[3][64]={{0},{0},{0}};
    CHAR               s_file_font_name[]="multifilefont";   
    FE_INIT_FILE       at_init_files[3];
    HFONT_T			    h_fnt;
    UINT8               ui1_idx;

    
   file_font_release_resource(0,NULL);

   i4_ret = fe_create_surface();
   if (i4_ret != FER_OK)
    {
        x_dbg_stmt("fe_create_surface fail:(%d)\n", i4_ret);
        return i4_ret;
    }
    
  
  for (ui1_idx = 0; ui1_idx < 3 ; ui1_idx++)
  {
         /* get filename */
        x_sprintf(s_abs_path[ui1_idx],"%stest%d.ttf",FILE_FONT_PATH_PREFIX,(ui1_idx +1));
        at_init_files[ui1_idx].ps_file = s_abs_path[ui1_idx];       
        at_init_files[ui1_idx].ui1_small = 12;
        at_init_files[ui1_idx].ui1_medium = 24;
        at_init_files[ui1_idx].ui1_large= 36;        
  }   

   

   i4_ret = x_fe_add_multiple_file_font(s_file_font_name,at_init_files,3);   
   if (i4_ret != FER_OK)
    {
        x_dbg_stmt("Add multi file font fail:%s. (%d)\n",s_abs_path, i4_ret);
        return i4_ret;
    }
   
   i4_ret = x_fe_create_font(s_file_font_name,
							FE_FNT_SIZE_LARGE,
							FE_FNT_STYLE_REGULAR,
							FE_CMAP_ENC_UNICODE,
							&h_fnt);
    if(i4_ret != GLR_OK){
		x_dbg_stmt("Error create font failed\n");
    }  
   t_color.u1.y = 0;
   t_color.u2.u = 0;
   t_color.u3.v = 0;
   t_color.a    = 255;
  for (ui1_idx = 0; ui1_idx < 3 ; ui1_idx++)
  {
        i4_ret = x_fe_change_font_resource(s_file_font_name,ui1_idx);
        if (i4_ret != FER_OK)
        {
            x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
            return i4_ret;
        }
        i4_ret = x_gl_surface_draw_ustring(_h_disp_surf,100,50 + ui1_idx * 60,L"Draw text use file font:",54,h_fnt,&t_color,NULL,NULL);
        if (i4_ret != FER_OK)
        {
            x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
            return i4_ret;
        }
    }
   i4_ret = fe_flip_surface();
   if (i4_ret != FER_OK)
   {
        x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
        return i4_ret;
   } 
   return CLIR_OK;
}



static INT32 file_font_release_resource(INT32 i4_argc, const CHAR** pps_argv)
{
    CHAR                s_abs_path[64]={0};
    CHAR               s_file_font_name[]="arabic";
    CHAR               s_multi_file_font_name[]="multifilefont";
   
    if (_h_view != NULL_HANDLE)
    {
        INT32 i4_ret;        
        
        i4_ret = x_gl_view_delete_viewport(_h_view, _ui4_viewport);
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("x_gl_view_delete_viewport failed. (%d)\n", i4_ret);
        }
        
        i4_ret = x_gl_obj_free(_h_view);
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("x_gl_obj_free(_h_view) failed. (%d)\n", i4_ret);
        }
        
        i4_ret = x_gl_obj_free(_h_disp_surf);
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("x_gl_obj_free(_h_disp_surf) failed. (%d)\n", i4_ret);
        } 
         /* get filename */
        x_sprintf(s_abs_path,"%s%s.ttf",FILE_FONT_PATH_PREFIX,s_file_font_name);
        if (x_fe_has_font(s_file_font_name,FE_FNT_STYLE_REGULAR))
        {
            i4_ret = x_fe_remove_font(s_file_font_name,X_FE_OPTION_FORCE);
            if (i4_ret != GLR_OK)
            {
                x_dbg_stmt("x_fe_remove_font failed. (%d)\n", i4_ret);
            }
        }
        x_sprintf(s_abs_path,"%s%s.ttf",FILE_FONT_PATH_PREFIX,s_multi_file_font_name);
        if (x_fe_has_font(s_multi_file_font_name,FE_FNT_STYLE_REGULAR))
        {
            i4_ret = x_fe_remove_font(s_multi_file_font_name,X_FE_OPTION_FORCE);
            if (i4_ret != GLR_OK)
            {
                x_dbg_stmt("x_fe_remove_font failed. (%d)\n", i4_ret);
            }
        }
        return i4_ret;
    }
    else
    {
        return CLIR_OK;
    }
}


static INT32 chg_menu_font(INT32 i4_argc, const CHAR** pps_argv)

{
    INT32   i4_ret;

      i4_ret = x_fe_remove_font(SN_FONT_DEFAULT,X_FE_OPTION_FORCE);
      if (i4_ret != FER_OK)
      {
          x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
          return i4_ret;
      }
      i4_ret = x_fe_remove_font(SN_FONT_DEFAULT_BIG,X_FE_OPTION_FORCE);
      if (i4_ret != FER_OK)
      {
          x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
          return i4_ret;
      }
      i4_ret = x_fe_remove_font("fnt_ttx20_bdf",X_FE_OPTION_FORCE);
      if (i4_ret != FER_OK)
      {
          x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
          return i4_ret;
      }
     i4_ret =  x_fe_add_file_font(SN_FONT_DEFAULT,"/mnt/usb/Mass-000/test3.ttf",26,30,36);
     if (i4_ret != FER_OK)
      {
          x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
          return i4_ret;
      }
     i4_ret =  x_fe_add_file_font(SN_FONT_DEFAULT_BIG,"/mnt/usb/Mass-000/test2.ttf",40,50,60);
     if (i4_ret != FER_OK)
      {
          x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
          return i4_ret;
      }
     i4_ret =  x_fe_add_file_font("fnt_ttx20_bdf","/mnt/usb/Mass-000/test1.ttf",20,20,20);
     if (i4_ret != FER_OK)
      {
          x_dbg_stmt("%s, %s, %d, (%d)\n",__FILE__,__FUNCTION__, __LINE__,i4_ret);
          return i4_ret;
      }

     return CLIR_OK;

}


#endif /*CLI_SUPPORT*/

