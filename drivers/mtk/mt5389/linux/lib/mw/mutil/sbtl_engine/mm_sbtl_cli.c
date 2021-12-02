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
 * $RCSfile: mm_sbtl_cli.c,v $
 * $Revision:
 * $Date:
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 37ebbb2d76fad2e8a90655e5f2bf2c3a $
 *
 * Description:
 *         This file implements CLI APIs for MTP.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "inc/x_common.h"
#include "unicode/x_uc_str.h"
#include "cli/x_cli.h"
#include "dbg/x_dbg.h"

#include "wgl/subtitle/u_wgl_sbtl.h"
#include "wgl/x_wgl.h"
#include "wgl/u_wgl_tools.h"
#include "wgl/u_wgl_obj_type.h"
#include "graphic/x_gl.h"

#include "util/x_lnk_list.h"

#include "mutil/sbtl_engine/sbtl_dbg.h"
#include "mutil/sbtl_engine/sbtl_struct.h"
#include "mutil/sbtl_engine/sbtl_utils.h"
#include "mutil/sbtl_engine/sbtl_register.h"
#include "mutil/sbtl_engine/sbtl_state.h"
#include "mutil/sbtl_engine/sbtl_divx/sbtl_divx_page_inst_mngr.h"
#include "mutil/sbtl_engine/u_mm_sbtl_engine.h"
#include "mutil/sbtl_engine/x_mm_sbtl_engine.h"

#include "inc/x_mm_common.h"
#include "mutil/mm_util_input.h"

#include "file_mngr/u_fm.h"
#include "file_mngr/x_fm.h"

#include "handle/x_handle.h"
#include "inc/u_sys_name.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
#define SRT_TEST_FILE       "/mnt/usb/Mass-000/test.srt"
#define SAMI_TEST_FILE      "/mnt/usb/Mass-000/test.smi"
#define SVSUB_TEST_FILE     "/mnt/usb/Mass-000/test.sub"
#define MPLAYER2_TEST_FILE  "/mnt/usb/Mass-000/test.mpl"
#define SSA_TEST_FILE       "/mnt/usb/Mass-000/test.ssa"
#define MDVDSUB_TEST_FILE   "/mnt/usb/Mass-000/test_mdvdsub.sub"
#define TXT_TEST_FILE       "/mnt/usb/Mass-000/test.txt"
#define PSB_TEST_FILE       "/mnt/usb/Mass-000/test.psb"
#define DVDSS_TEST_FILE     "/mnt/usb/Mass-000/test_dvdss.txt"

#define SRT_W2S_TEST_FILE       "/mnt/usb/Mass-000/test_unicode.srt"
#define SAMI_W2S_TEST_FILE      "/mnt/usb/Mass-000/test_unicode.smi"
#define SVSUB_W2S_TEST_FILE     "/mnt/usb/Mass-000/test_unicode.sub"
#define MPLAYER2_W2S_TEST_FILE  "/mnt/usb/Mass-000/test_unicode.mpl"
#define SSA_W2S_TEST_FILE       "/mnt/usb/Mass-000/test_unicode.ssa"
#define MDVDSUB_W2S_TEST_FILE   "/mnt/usb/Mass-000/test_mdvdsub_unicode.sub"
#define TXT_W2S_TEST_FILE       "/mnt/usb/Mass-000/test_unicode.txt"
#define PSB_W2S_TEST_FILE       "/mnt/usb/Mass-000/test_unicode.psb"
#define DVDSS_W2S_TEST_FILE     "/mnt/usb/Mass-000/test_dvdss_unicode.txt"


#define VOBSUB_SUB_TEST_FILE       "/mnt/usb/Mass-000/VTS_01_0.sub"
#define VOBSUB_IDX_TEST_FILE       "/mnt/usb/Mass-000/VTS_01_0.idx"

#endif
/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL

static INT32 _sbtl_cli_get_dbg_level(INT32              i4_argc, 
                                     const CHAR**       pps_argv);
                                    
static INT32 _sbtl_cli_set_dbg_level(INT32              i4_argc, 
                                     const CHAR**       pps_argv);  

static INT32 _sbtl_cli_divx_open(INT32              i4_argc, 
                                 const CHAR**       pps_argv);  

static INT32 _sbtl_cli_vobsub_open(INT32                i4_argc, 
                                   const CHAR**         pps_argv);

static INT32 _sbtl_cli_srt_open(INT32               i4_argc, 
                                const CHAR**        pps_argv);

static INT32 _sbtl_cli_sami_open(INT32              i4_argc, 
                                 const CHAR**       pps_argv);

static INT32 _sbtl_cli_svsub_open(INT32             i4_argc, 
                                  const CHAR**      pps_argv);

static INT32 _sbtl_cli_mplayer2_open(INT32              i4_argc, 
                                     const CHAR**       pps_argv);

static INT32 _sbtl_cli_ssa_open(INT32              i4_argc, 
                                const CHAR**       pps_argv);

static INT32 _sbtl_cli_mdvdsub_open(INT32           i4_argc, 
                                    const CHAR**    pps_argv);

static INT32 _sbtl_cli_txt_open(INT32           i4_argc, 
                                const CHAR**    pps_argv);

static INT32 _sbtl_cli_psb_open(INT32               i4_argc, 
                                const CHAR**        pps_argv);

static INT32 _sbtl_cli_dvdss_open(INT32               i4_argc, 
                                  const CHAR**        pps_argv);

static INT32 _sbtl_cli_close(INT32              i4_argc, 
                             const CHAR**       pps_argv);  

static INT32 _sbtl_cli_start(INT32              i4_argc, 
                             const CHAR**       pps_argv);  

static INT32 _sbtl_cli_stop(INT32               i4_argc, 
                            const CHAR**        pps_argv); 

static INT32 _sbtl_cli_set_font_info(INT32              i4_argc, 
                                     const CHAR**             pps_argv);

static INT32 _sbtl_cli_set_encoding_info(INT32              i4_argc, 
                                     const CHAR**             pps_argv);

static INT32 _sbtl_cli_set_hlt_fg(INT32              i4_argc, 
                                     const CHAR**             pps_argv);

static INT32 _sbtl_cli_set_hlt_bg(INT32              i4_argc, 
                                     const CHAR**             pps_argv);

static INT32 _sbtl_cli_set_line_gap(INT32              i4_argc, 
                                     const CHAR**             pps_argv);

static INT32 _sbtl_cli_set_disp_page(INT32              i4_argc, 
                                     const CHAR**             pps_argv);

static INT32 _sbtl_cli_set_time_offset(INT32              i4_argc, 
                                     const CHAR**             pps_argv);


#endif

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

#ifdef CLI_LVL_ALL
static GL_COLOR_T           t_color_black = { 255, {   0 }, {   0 }, {   0 } };
static GL_COLOR_T           t_color_white = { 0, { 255 }, { 255 }, { 255 } };

static VOID*                pv_engine = NULL;
static HANDLE_T             h_screen = NULL_HANDLE;
static HANDLE_T             h_plane = NULL_HANDLE;
static HANDLE_T             h_canvas = NULL_HANDLE;
static HANDLE_T             h_subtitle_file = NULL_HANDLE;
static HANDLE_T             h_index_file = NULL_HANDLE;
static MEDIA_FEEDER_T       t_feeder;
static MEDIA_FEEDER_T       t_feeder_idx;

/* minfo command table */
static CLI_EXEC_T at_mm_sbtl_cmd_tbl[] =
{
    {
        CLI_GET_DBG_LVL_STR,   
        NULL,       
        _sbtl_cli_get_dbg_level,     
        NULL,       
        CLI_GET_DBG_LVL_HELP_STR,                  
        CLI_SUPERVISOR
    },
    {
        CLI_SET_DBG_LVL_STR,   
        NULL,       
        _sbtl_cli_set_dbg_level,     
        NULL,       
        CLI_SET_DBG_LVL_HELP_STR,                  
        CLI_SUPERVISOR
    },
    {
        "dso",   
        NULL,       
        _sbtl_cli_divx_open,     
        NULL,       
        "divx subtitle open and start",                  
        CLI_SUPERVISOR
    },
    
    {
        "vso",   
        NULL,       
        _sbtl_cli_vobsub_open,     
        NULL,       
        "vobsub subtitle open and start",                  
        CLI_SUPERVISOR
    },
    {
        "sso",   
        NULL,       
        _sbtl_cli_srt_open,     
        NULL,       
        "srt subtitle open and start",                  
        CLI_SUPERVISOR
    },
    {
        "smio",   
        NULL,       
        _sbtl_cli_sami_open,     
        NULL,       
        "sami subtitle open and start",                  
        CLI_SUPERVISOR
    },
    {
        "svsubo",   
        NULL,       
        _sbtl_cli_svsub_open,     
        NULL,       
        "svsub subtitle open and start",                  
        CLI_SUPERVISOR
    },
    {
        "mplo",   
        NULL,       
        _sbtl_cli_mplayer2_open,     
        NULL,       
        "mplayer2 subtitle open and start",                  
        CLI_SUPERVISOR
    },
    {
        "ssao",   
        NULL,       
        _sbtl_cli_ssa_open,     
        NULL,       
        "ssa subtitle open and start",                  
        CLI_SUPERVISOR
    },
    {
        "mdso",   
        NULL,       
        _sbtl_cli_mdvdsub_open,     
        NULL,       
        "mdvdsub subtitle open and start",                  
        CLI_SUPERVISOR
    },
    {
        "txto",   
        NULL,       
        _sbtl_cli_txt_open,     
        NULL,       
        "txt subtitle open and start",                  
        CLI_SUPERVISOR
    },
    {
        "psbo",   
        NULL,       
        _sbtl_cli_psb_open,     
        NULL,       
        "power divx subtitle open and start",                  
        CLI_SUPERVISOR
    },
    {
        "dsso",
        NULL,       
        _sbtl_cli_dvdss_open,     
        NULL,       
        "dvd subtitle system open and start",                  
        CLI_SUPERVISOR
    },
    {
        "close",   
        NULL,       
        _sbtl_cli_close,     
        NULL,       
        "subtitle close",                  
        CLI_SUPERVISOR
    },
    {
        "start",   
        NULL,       
        _sbtl_cli_start,     
        NULL,       
        "subtitle start",                  
        CLI_SUPERVISOR
    },
    {
        "stop",   
        NULL,       
        _sbtl_cli_stop,     
        NULL,       
        "subtitle stop",                  
        CLI_SUPERVISOR
    },
    {
        "font",   
        NULL,       
        _sbtl_cli_set_font_info,     
        NULL,       
        "Set subtitle font information",                  
        CLI_SUPERVISOR
    },    
    {
        "encode",   
        NULL,       
        _sbtl_cli_set_encoding_info,     
        NULL,       
        "Set subtitle font information",                  
        CLI_SUPERVISOR
    },     

    {
        "hlt_fg",   
        NULL,       
        _sbtl_cli_set_hlt_fg,     
        NULL,       
        "Set subtitle hlt fontgroud color",                  
        CLI_SUPERVISOR
    }, 

    {
        "hlt_bg",   
        NULL,       
        _sbtl_cli_set_hlt_bg,     
        NULL,       
        "Set subtitle hlt backgroud color",                  
        CLI_SUPERVISOR
    }, 
    {
        "gap",   
        NULL,       
        _sbtl_cli_set_line_gap,     
        NULL,       
        "Set subtitle line gap",                  
        CLI_SUPERVISOR
    }, 
    {
        "page",   
        NULL,       
        _sbtl_cli_set_disp_page,     
        NULL,       
        "Set subtitle display page information",                  
        CLI_SUPERVISOR
    }, 
    {
        "offset",   
        NULL,       
        _sbtl_cli_set_time_offset,     
        NULL,       
        "Set display time offset information",                  
        CLI_SUPERVISOR
    }, 
    
    END_OF_CLI_CMD_TBL
};

/* minfo root command table */
static CLI_EXEC_T at_mm_sbtl_root_cmd_tbl[] =
{
    {
        "mmsbtl",     
        NULL,        
        NULL,       
        at_mm_sbtl_cmd_tbl,      
        "multimedia subtitle commands",   
        CLI_SUPERVISOR
    },
    END_OF_CLI_CMD_TBL
};
#endif
/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/ 
/*-----------------------------------------------------------------------------
 * Name: _mtp_cli_get_dbg_level
 *
 * Description: This API gets the current setting of mtp debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _sbtl_cli_get_dbg_level(INT32              i4_argc, 
                                     const CHAR**       pps_argv)
{
    INT32       i4_return;

    i4_return = x_cli_show_dbg_level(mm_sbtl_get_dbg_level());

    return i4_return;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _mtp_cli_set_dbg_level
 *
 * Description: This API sets the debug level of mtp.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _sbtl_cli_set_dbg_level(INT32              i4_argc, 
                                     const CHAR**       pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        mm_sbtl_set_dbg_level(ui2_dbg_level);
        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }

    x_cli_show_dbg_level(mm_sbtl_get_dbg_level());
    DBG_INFO(("[mm sbtl] set debug level %s\n\r", (i4_return == CLIR_OK) ? "successful" : "failed"));
    return i4_return;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _mtp_cli_set_dbg_level
 *
 * Description: This API sets the debug level of mtp.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL

static INT32 _sbtl_util_finput1bytes_fct(VOID* pv_obj, UINT8* pui1_data)
{
    UINT8               ui1_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    MEDIA_FEEDER_T*     pt_obj;

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
    i4_ret = x_fm_read(pt_obj->h_feeder, (VOID*)&ui1_data, sizeof(ui1_data), &ui4_read);
    *pui1_data = ui1_data;
    if (i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_EOT;
    }
    else
    {
        return MMSBTLR_CORE;
    }
}

static INT32 _sbtl_util_fsetpos_fct(VOID* pv_obj, INT64 i8_offset, UINT8 ui1_whence)
{
    INT32               i4_ret;
    UINT64              ui8_cur_pos;
    MEDIA_FEEDER_T*     pt_obj;

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
    i4_ret = x_fm_lseek(pt_obj->h_feeder, i8_offset, ui1_whence, &ui8_cur_pos);
    if(i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_EOT;
    }
    else
    {
        return MMSBTLR_CORE;
    }
}

static INT32 _sbtl_util_fgetpos_fct(VOID* pv_obj, UINT64* pui8_cur_pos)
{
    UINT64              ui8_cur_pos;
    INT32               i4_ret;
    MEDIA_FEEDER_T*     pt_obj;

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
    i4_ret = x_fm_lseek(pt_obj->h_feeder, 0, FM_SEEK_CUR, &ui8_cur_pos);
    *pui8_cur_pos = ui8_cur_pos;
    
    if(i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_EOT;
    }
    else
    {
        return MMSBTLR_CORE;
    }
}

static INT32 _sbtl_util_fcopybytes_fct(VOID*       pv_obj, 
                                       VOID*       pv_buf, 
                                       SIZE_T      z_buf_leng, 
                                       SIZE_T      z_copy_size,
                                       UINT32*     pui4_size)
{
    UINT32              ui4_read;
    INT32               i4_ret;
    BOOL                b_buf_not_enough;
    SIZE_T              z_copy;
    MEDIA_FEEDER_T*     pt_obj;

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
    if (z_buf_leng < z_copy_size)
    {
        z_copy = z_buf_leng;
        b_buf_not_enough = TRUE;
    }
    else
    {
        z_copy = z_copy_size;
        b_buf_not_enough = FALSE;
    }
    
    i4_ret = x_fm_read(pt_obj->h_feeder, pv_buf, z_copy, &ui4_read);
    *pui4_size = ui4_read;
    if (i4_ret == FMR_OK)
    {
        if(ui4_read < z_copy)
        {
            return MMSBTLR_EOT;
        }
        else
        {
            if(b_buf_not_enough)
            {
                return MMSBTLR_INSUFFICIENT_MEMORY;
            }
            else
            {
                return MMSBTLR_OK;
            }
        }
    }
    else
    {
        return INPUTR_FAIL;
    }
}

VOID _mm_sbtl_state_notify(VOID*        pv_tag,
                           SBTL_COND_T  e_cond,
                           UINT32       e_event,
                           UINT32       ui4_data)
{
    x_dbg_stmt("_mm_sbtl_state_notify cond:%d event:%d\n", e_cond, e_event);
}

static INT32 _sbtl_cli_divx_open(INT32              i4_argc, 
                                 const CHAR**       pps_argv)
{
    SBTL_CFG_T      t_cfg;
    INT32           i4_ret;
    
    /*Open the screen */
    if (NULL_HANDLE == h_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_screen);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open screen\n");
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_plane)
    {
        /*Open the plane */
        i4_ret = x_gl_plane_open (h_screen,
                                  1,
                                  &h_plane);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open plane\n");
            x_handle_free(h_screen);
            return CLIR_OK;
        }
        
        /*Link the plane*/
        i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't link plane\n");
            x_handle_free(h_screen);
            x_handle_free(h_plane);
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_canvas)
    {
        x_wgl_get_canvas(h_plane, &h_canvas);
    }
    
    t_cfg.e_sbtl_type = MM_SBTL_TYPE_DIVX;
    t_cfg.e_divx_sbtl_type = MM_DIVX_SBTL_TITLE_TYPE;
    t_cfg.i4_speed = 100;

    t_cfg.t_subtitle_avi.i4_width = 640;
    t_cfg.t_subtitle_avi.i4_height = 480;
    t_cfg.h_gl_plane = h_canvas;
    
    
    x_sbtl_engine_open(&t_cfg,
                     TRUE,
                     _mm_sbtl_state_notify,
                     NULL,
                     &pv_engine);
    
    return CLIR_OK;
}

static INT32 _sbtl_cli_vobsub_open(INT32                i4_argc, 
                                   const CHAR**         pps_argv)
{
    SBTL_CFG_T              t_cfg;
    INT32                   i4_ret;

    /*Open the screen */
    if (NULL_HANDLE == h_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_screen);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open screen\n");
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_plane)
    {
        /*Open the plane */
        i4_ret = x_gl_plane_open (h_screen,
                                  1,
                                  &h_plane);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open plane\n");
            x_handle_free(h_screen);
            return CLIR_OK;
        }
        
        /*Link the plane*/
        i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't link plane\n");
            x_handle_free(h_screen);
            x_handle_free(h_plane);
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_canvas)
    {
        x_wgl_get_canvas(h_plane, &h_canvas);
    }
    
    t_cfg.e_sbtl_type = MM_SBTL_TYPE_VOBSUB;
    t_cfg.i4_speed = 100;
    t_cfg.h_gl_plane = h_canvas;

    if (NULL_HANDLE == h_subtitle_file)
    {
        x_fm_open(FM_ROOT_HANDLE, 
                  VOBSUB_SUB_TEST_FILE,
                  FM_READ_ONLY, 
                  0666, 
                  FALSE, 
                  &h_subtitle_file);
        
        t_feeder.h_feeder = h_subtitle_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;

        t_feeder.t_feeder_le_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;
    }
    t_cfg.pt_feeder[MM_SBTL_FEEDER_SUBTITLE] = &t_feeder;

    if (NULL_HANDLE == h_index_file)
    {
        x_fm_open(FM_ROOT_HANDLE, 
                  VOBSUB_IDX_TEST_FILE,
                  FM_READ_ONLY, 
                  0666, 
                  FALSE, 
                  &h_index_file);
        
        t_feeder_idx.h_feeder = h_index_file;
        t_feeder_idx.t_feeder_be_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder_idx.t_feeder_be_fct_tbl.pf_input4 = NULL;
        t_feeder_idx.t_feeder_be_fct_tbl.pf_input3 = NULL;
        t_feeder_idx.t_feeder_be_fct_tbl.pf_input2 = NULL;
        t_feeder_idx.t_feeder_be_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder_idx.t_feeder_be_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder_idx.t_feeder_be_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;

        t_feeder_idx.t_feeder_le_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder_idx.t_feeder_le_fct_tbl.pf_input4 = NULL;
        t_feeder_idx.t_feeder_le_fct_tbl.pf_input3 = NULL;
        t_feeder_idx.t_feeder_le_fct_tbl.pf_input2 = NULL;
        t_feeder_idx.t_feeder_le_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder_idx.t_feeder_le_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder_idx.t_feeder_le_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;
    }
    t_cfg.pt_feeder[MM_SBTL_FEEDER_INDEX] = &t_feeder_idx;

    x_sbtl_engine_open(&t_cfg,
                     TRUE,
                     _mm_sbtl_state_notify,
                     NULL,
                     &pv_engine);
    
    return CLIR_OK;
}




static INT32 _sbtl_cli_srt_open(INT32               i4_argc, 
                                const CHAR**        pps_argv)
{
    SBTL_CFG_T              t_cfg;
    INT32                   i4_ret;
    UINT8                   ui1_unicode;
    
    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] sso [1=UTF16,0=UTF8 ]\n\r");
        return CLIR_OK;
    }  
    ui1_unicode = (UINT8)x_strtoll(pps_argv[1],NULL,10);

    /*Open the screen */
    if (NULL_HANDLE == h_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_screen);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open screen\n");
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_plane)
    {
        /*Open the plane */
        i4_ret = x_gl_plane_open (h_screen,
                                  1,
                                  &h_plane);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open plane\n");
            x_handle_free(h_screen);
            return CLIR_OK;
        }
        
        /*Link the plane*/
        i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't link plane\n");
            x_handle_free(h_screen);
            x_handle_free(h_plane);
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_canvas)
    {
        x_wgl_get_canvas(h_plane, &h_canvas);
    }
    
    t_cfg.e_sbtl_type = MM_SBTL_TYPE_TEXT_SRT;
    t_cfg.i4_speed = 100;
    t_cfg.h_gl_plane = h_canvas;

    if (NULL_HANDLE == h_subtitle_file)
    {
        if(ui1_unicode == 0)
        {
        x_fm_open(FM_ROOT_HANDLE, 
                  SRT_TEST_FILE,
                  FM_READ_ONLY, 
                  0666, 
                  FALSE, 
                  &h_subtitle_file);
        }
        else
        {
            x_fm_open(FM_ROOT_HANDLE, 
                      SRT_W2S_TEST_FILE,
                      FM_READ_ONLY, 
                      0666, 
                      FALSE, 
                      &h_subtitle_file);
        }
        
        t_feeder.h_feeder = h_subtitle_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;

        t_feeder.t_feeder_le_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;
    }
    t_cfg.pt_feeder[MM_SBTL_FEEDER_SUBTITLE] = &t_feeder;
    
    x_memset(&t_cfg.t_font_info, 0, sizeof(WGL_FONT_INFO_T));
    x_strcpy(t_cfg.t_font_info.a_c_font_name, SN_FONT_DEFAULT);
    t_cfg.t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
    t_cfg.t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
    t_cfg.t_font_info.e_font_cmap  = FE_CMAP_ENC_UNICODE;

    t_cfg.t_bg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    t_cfg.t_fg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    t_cfg.ui1_text_alignment = WGL_AS_CENTER_BOTTOM;
    
    x_sbtl_engine_open(&t_cfg,
                     TRUE,
                     _mm_sbtl_state_notify,
                     NULL,
                     &pv_engine);
    
    return CLIR_OK;
}


static INT32 _sbtl_cli_sami_open(INT32              i4_argc, 
                                 const CHAR**       pps_argv)
{
    SBTL_CFG_T      t_cfg;
    INT32           i4_ret;
    SAMI_STREAM_INFO_T t_stream_info;
    UINT32              ui4_idx;
    UINT8                   ui1_unicode;
    
    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] smio [1=UTF16,0=UTF8 ]\n\r");
        return CLIR_OK;
    } 
    ui1_unicode = (UINT8)x_strtoll(pps_argv[1],NULL,10);
    
    /*Open the screen */
    if (NULL_HANDLE == h_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_screen);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open screen\n");
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_plane)
    {
        /*Open the plane */
        i4_ret = x_gl_plane_open (h_screen,
                                  1,
                                  &h_plane);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open plane\n");
            x_handle_free(h_screen);
            return CLIR_OK;
        }
        
        /*Link the plane*/
        i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't link plane\n");
            x_handle_free(h_screen);
            x_handle_free(h_plane);
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_canvas)
    {
        x_wgl_get_canvas(h_plane, &h_canvas);
    }
    
    t_cfg.e_sbtl_type = MM_SBTL_TYPE_TEXT_SAMI;
    t_cfg.i4_speed = 100;
    t_cfg.h_gl_plane = h_canvas;
    t_cfg.ui4_stream_id = 0;
    if (NULL_HANDLE == h_subtitle_file)
    {
        if(ui1_unicode == 0)
        {
        x_fm_open(FM_ROOT_HANDLE, 
                  SAMI_TEST_FILE,
                  FM_READ_ONLY, 
                  0666, 
                  FALSE, 
                  &h_subtitle_file);
        }
        else
        {
            x_fm_open(FM_ROOT_HANDLE, 
                      SAMI_W2S_TEST_FILE,
                      FM_READ_ONLY, 
                      0666, 
                      FALSE, 
                      &h_subtitle_file);
        }
        
        t_feeder.h_feeder = h_subtitle_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;

        t_feeder.t_feeder_le_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;
    }
    
    t_cfg.pt_feeder[MM_SBTL_FEEDER_SUBTITLE] = &t_feeder;
    
    x_memset(&t_cfg.t_font_info, 0, sizeof(WGL_FONT_INFO_T));
    x_strcpy(t_cfg.t_font_info.a_c_font_name, SN_FONT_DEFAULT);
    t_cfg.t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
    t_cfg.t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
    t_cfg.t_font_info.e_font_cmap  = FE_CMAP_ENC_UNICODE;

    t_cfg.t_bg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    t_cfg.t_fg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    t_cfg.ui1_text_alignment = WGL_AS_CENTER_BOTTOM;
    
    x_sbtl_engine_open(&t_cfg,
                     TRUE,
                     _mm_sbtl_state_notify,
                     NULL,
                     &pv_engine);
    
    x_sbtl_engine_get(pv_engine,
                      EVN_MM_SBTL_GET_STREAM_INFO,
                      &t_stream_info,
                      sizeof(SAMI_STREAM_INFO_T));

    ui4_idx = 0;
    
    x_sbtl_engine_set(pv_engine,
                      EVN_MM_SBTL_SET_STREAM_INDEX,
                      &ui4_idx,
                      4);

    return CLIR_OK;
}


static INT32 _sbtl_cli_svsub_open(INT32               i4_argc, 
                                const CHAR**        pps_argv)
{
    SBTL_CFG_T      t_cfg;
    INT32           i4_ret;
    UINT8                   ui1_unicode;

    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] svsubo [1=UTF16,0=UTF8 ]\n\r");
        return CLIR_OK;
    } 
    ui1_unicode = (UINT8)x_strtoll(pps_argv[1],NULL,10);
    
    /*Open the screen */
    if (NULL_HANDLE == h_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_screen);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open screen\n");
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_plane)
    {
        /*Open the plane */
        i4_ret = x_gl_plane_open (h_screen,
                                  1,
                                  &h_plane);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open plane\n");
            x_handle_free(h_screen);
            return CLIR_OK;
        }
        
        /*Link the plane*/
        i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't link plane\n");
            x_handle_free(h_screen);
            x_handle_free(h_plane);
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_canvas)
    {
        x_wgl_get_canvas(h_plane, &h_canvas);
    }
    
    t_cfg.e_sbtl_type = MM_SBTL_TYPE_TEXT_SVSUB;
    t_cfg.i4_speed = 100;
    t_cfg.h_gl_plane = h_canvas;

    if (NULL_HANDLE == h_subtitle_file)
    {
        if(ui1_unicode == 0)
        {
        x_fm_open(FM_ROOT_HANDLE, 
                  SVSUB_TEST_FILE,
                  FM_READ_ONLY, 
                  0666, 
                  FALSE, 
                  &h_subtitle_file);
        }
        else
        {
            x_fm_open(FM_ROOT_HANDLE, 
                      SVSUB_W2S_TEST_FILE,
                      FM_READ_ONLY, 
                      0666, 
                      FALSE, 
                      &h_subtitle_file);
        }    
        
        t_feeder.h_feeder = h_subtitle_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;

        t_feeder.t_feeder_le_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;
    }
    
    t_cfg.pt_feeder[MM_SBTL_FEEDER_SUBTITLE] = &t_feeder;
    
    x_memset(&t_cfg.t_font_info, 0, sizeof(WGL_FONT_INFO_T));
    x_strcpy(t_cfg.t_font_info.a_c_font_name, SN_FONT_DEFAULT);
    t_cfg.t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
    t_cfg.t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
    t_cfg.t_font_info.e_font_cmap  = FE_CMAP_ENC_UNICODE;

    t_cfg.t_bg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    t_cfg.t_fg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    t_cfg.ui1_text_alignment = WGL_AS_CENTER_BOTTOM;
    
    x_sbtl_engine_open(&t_cfg,
                     TRUE,
                     _mm_sbtl_state_notify,
                     NULL,
                     &pv_engine);
    
    return CLIR_OK;
}


static INT32 _sbtl_cli_mplayer2_open(INT32              i4_argc, 
                                     const CHAR**       pps_argv)
{
    SBTL_CFG_T              t_cfg;
    INT32                   i4_ret;
    UINT8                   ui1_unicode;

    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] mplo [1=UTF16,0=UTF8 ]\n\r");
        return CLIR_OK;
    } 
    ui1_unicode = (UINT8)x_strtoll(pps_argv[1],NULL,10);

    /*Open the screen */
    if (NULL_HANDLE == h_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_screen);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open screen\n");
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_plane)
    {
        /*Open the plane */
        i4_ret = x_gl_plane_open (h_screen,
                                  1,
                                  &h_plane);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open plane\n");
            x_handle_free(h_screen);
            return CLIR_OK;
        }
        
        /*Link the plane*/
        i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't link plane\n");
            x_handle_free(h_screen);
            x_handle_free(h_plane);
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_canvas)
    {
        x_wgl_get_canvas(h_plane, &h_canvas);
    }
    
    t_cfg.e_sbtl_type = MM_SBTL_TYPE_TEXT_MPLAYER2;
    t_cfg.i4_speed = 100;
    t_cfg.h_gl_plane = h_canvas;

    if (NULL_HANDLE == h_subtitle_file)
    {
        if(ui1_unicode == 0)
        {
        x_fm_open(FM_ROOT_HANDLE, 
                  MPLAYER2_TEST_FILE,
                  FM_READ_ONLY, 
                  0666, 
                  FALSE, 
                  &h_subtitle_file);
        }
        else
        {
            x_fm_open(FM_ROOT_HANDLE, 
                      MPLAYER2_W2S_TEST_FILE,
                      FM_READ_ONLY, 
                      0666, 
                      FALSE, 
                      &h_subtitle_file);
        }     
        
        t_feeder.h_feeder = h_subtitle_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;

        t_feeder.t_feeder_le_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;
    }
    t_cfg.pt_feeder[MM_SBTL_FEEDER_SUBTITLE] = &t_feeder;
    
    x_memset(&t_cfg.t_font_info, 0, sizeof(WGL_FONT_INFO_T));
    x_strcpy(t_cfg.t_font_info.a_c_font_name, SN_FONT_DEFAULT);
    t_cfg.t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
    t_cfg.t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
    t_cfg.t_font_info.e_font_cmap  = FE_CMAP_ENC_UNICODE;

    t_cfg.t_bg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    t_cfg.t_fg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    t_cfg.ui1_text_alignment = WGL_AS_CENTER_BOTTOM;
    
    x_sbtl_engine_open(&t_cfg,
                     TRUE,
                     _mm_sbtl_state_notify,
                     NULL,
                     &pv_engine);
    
    return CLIR_OK;
}



static INT32 _sbtl_cli_ssa_open(INT32              i4_argc, 
                                const CHAR**       pps_argv)
{
    SBTL_CFG_T              t_cfg;
    INT32                   i4_ret;
    UINT8                   ui1_unicode;

    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] ssao [1=UTF16,0=UTF8 ]\n\r");
        return CLIR_OK;
    } 
    ui1_unicode = (UINT8)x_strtoll(pps_argv[1],NULL,10);

    /*Open the screen */
    if (NULL_HANDLE == h_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_screen);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open screen\n");
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_plane)
    {
        /*Open the plane */
        i4_ret = x_gl_plane_open (h_screen,
                                  1,
                                  &h_plane);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open plane\n");
            x_handle_free(h_screen);
            return CLIR_OK;
        }
        
        /*Link the plane*/
        i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't link plane\n");
            x_handle_free(h_screen);
            x_handle_free(h_plane);
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_canvas)
    {
        x_wgl_get_canvas(h_plane, &h_canvas);
    }
    
    t_cfg.e_sbtl_type = MM_SBTL_TYPE_TEXT_SSA;
    t_cfg.i4_speed = 100;
    t_cfg.h_gl_plane = h_canvas;

    if (NULL_HANDLE == h_subtitle_file)
    {
        if(ui1_unicode == 0)
        {
        x_fm_open(FM_ROOT_HANDLE, 
                  SSA_TEST_FILE,
                  FM_READ_ONLY, 
                  0666, 
                  FALSE, 
                  &h_subtitle_file);
        }
        else
        {
            x_fm_open(FM_ROOT_HANDLE, 
                      SSA_W2S_TEST_FILE,
                      FM_READ_ONLY, 
                      0666, 
                      FALSE, 
                      &h_subtitle_file);
        }       
        
        t_feeder.h_feeder = h_subtitle_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;

        t_feeder.t_feeder_le_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;
    }
    t_cfg.pt_feeder[MM_SBTL_FEEDER_SUBTITLE] = &t_feeder;
    
    x_memset(&t_cfg.t_font_info, 0, sizeof(WGL_FONT_INFO_T));
    x_strcpy(t_cfg.t_font_info.a_c_font_name, SN_FONT_DEFAULT);
    t_cfg.t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
    t_cfg.t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
    t_cfg.t_font_info.e_font_cmap  = FE_CMAP_ENC_UNICODE;

    t_cfg.t_bg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    t_cfg.t_fg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    t_cfg.ui1_text_alignment = WGL_AS_CENTER_BOTTOM;
    
    x_sbtl_engine_open(&t_cfg,
                     TRUE,
                     _mm_sbtl_state_notify,
                     NULL,
                     &pv_engine);
    
    return CLIR_OK;
}

static INT32 _sbtl_cli_mdvdsub_open(INT32           i4_argc, 
                                    const CHAR**    pps_argv)
{
    SBTL_CFG_T              t_cfg;
    INT32                   i4_ret;
    UINT8                   ui1_unicode;

    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] mdso [1=UTF16,0=UTF8 ]\n\r");
        return CLIR_OK;
    } 
    ui1_unicode = (UINT8)x_strtoll(pps_argv[1],NULL,10);

    /*Open the screen */
    if (NULL_HANDLE == h_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_screen);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open screen\n");
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_plane)
    {
        /*Open the plane */
        i4_ret = x_gl_plane_open (h_screen,
                                  1,
                                  &h_plane);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open plane\n");
            x_handle_free(h_screen);
            return CLIR_OK;
        }
        
        /*Link the plane*/
        i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't link plane\n");
            x_handle_free(h_screen);
            x_handle_free(h_plane);
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_canvas)
    {
        x_wgl_get_canvas(h_plane, &h_canvas);
    }
    
    t_cfg.e_sbtl_type = MM_SBTL_TYPE_TEXT_MDVDSUB;
    t_cfg.i4_speed = 100;
    t_cfg.h_gl_plane = h_canvas;
    t_cfg.ui4_video_rate = 25;
    t_cfg.ui4_video_scale = 1;

    if (NULL_HANDLE == h_subtitle_file)
    {
        if(ui1_unicode == 0)
        {
        x_fm_open(FM_ROOT_HANDLE, 
                  MDVDSUB_TEST_FILE,
                  FM_READ_ONLY,
                  0666, 
                  FALSE, 
                  &h_subtitle_file);
        }
        else
        {
            x_fm_open(FM_ROOT_HANDLE, 
                      MDVDSUB_W2S_TEST_FILE,
                      FM_READ_ONLY, 
                      0666, 
                      FALSE, 
                      &h_subtitle_file);
        }        
        
        t_feeder.h_feeder = h_subtitle_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;

        t_feeder.t_feeder_le_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;
    }
    t_cfg.pt_feeder[MM_SBTL_FEEDER_SUBTITLE] = &t_feeder;
    
    x_memset(&t_cfg.t_font_info, 0, sizeof(WGL_FONT_INFO_T));
    x_strcpy(t_cfg.t_font_info.a_c_font_name, SN_FONT_DEFAULT);
    t_cfg.t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
    t_cfg.t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
    t_cfg.t_font_info.e_font_cmap  = FE_CMAP_ENC_UNICODE;

    t_cfg.t_bg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    t_cfg.t_fg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    t_cfg.ui1_text_alignment = WGL_AS_CENTER_BOTTOM;
    
    x_sbtl_engine_open(&t_cfg,
                     TRUE,
                     _mm_sbtl_state_notify,
                     NULL,
                     &pv_engine);
    
    return CLIR_OK;
}

static INT32 _sbtl_cli_txt_open(INT32           i4_argc, 
                                const CHAR**    pps_argv)
{
    SBTL_CFG_T              t_cfg;
    INT32                   i4_ret;
    UINT8                   ui1_unicode;

    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] txto [1=UTF16,0=UTF8 ]\n\r");
        return CLIR_OK;
    } 
    ui1_unicode = (UINT8)x_strtoll(pps_argv[1],NULL,10);

    /*Open the screen */
    if (NULL_HANDLE == h_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_screen);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open screen\n");
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_plane)
    {
        /*Open the plane */
        i4_ret = x_gl_plane_open (h_screen,
                                  1,
                                  &h_plane);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open plane\n");
            x_handle_free(h_screen);
            return CLIR_OK;
        }
        
        /*Link the plane*/
        i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't link plane\n");
            x_handle_free(h_screen);
            x_handle_free(h_plane);
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_canvas)
    {
        x_wgl_get_canvas(h_plane, &h_canvas);
    }
    
    t_cfg.e_sbtl_type = MM_SBTL_TYPE_TEXT_TXT;
    t_cfg.i4_speed = 100;
    t_cfg.h_gl_plane = h_canvas;
    t_cfg.ui4_video_rate = 25;
    t_cfg.ui4_video_scale = 1;

    if (NULL_HANDLE == h_subtitle_file)
    {
        if(ui1_unicode == 0)
        {
        x_fm_open(FM_ROOT_HANDLE, 
                  TXT_TEST_FILE,
                  FM_READ_ONLY,
                  0666, 
                  FALSE, 
                  &h_subtitle_file);
        }
        else
        {
            x_fm_open(FM_ROOT_HANDLE, 
                      TXT_W2S_TEST_FILE,
                      FM_READ_ONLY, 
                      0666, 
                      FALSE, 
                      &h_subtitle_file);
        }     
        
        t_feeder.h_feeder = h_subtitle_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;

        t_feeder.t_feeder_le_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;
    }
    t_cfg.pt_feeder[MM_SBTL_FEEDER_SUBTITLE] = &t_feeder;
    
    x_memset(&t_cfg.t_font_info, 0, sizeof(WGL_FONT_INFO_T));
    x_strcpy(t_cfg.t_font_info.a_c_font_name, SN_FONT_DEFAULT);
    t_cfg.t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
    t_cfg.t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
    t_cfg.t_font_info.e_font_cmap  = FE_CMAP_ENC_UNICODE;

    t_cfg.t_bg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    t_cfg.t_fg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    t_cfg.ui1_text_alignment = WGL_AS_CENTER_BOTTOM;
    
    x_sbtl_engine_open(&t_cfg,
                     TRUE,
                     _mm_sbtl_state_notify,
                     NULL,
                     &pv_engine);
    
    return CLIR_OK;
}


static INT32 _sbtl_cli_psb_open(INT32               i4_argc, 
                                const CHAR**        pps_argv)
{
    SBTL_CFG_T      t_cfg;
    INT32           i4_ret;
    UINT8                   ui1_unicode;

    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] psbo [1=UTF16,0=UTF8 ]\n\r");
        return CLIR_OK;
    } 
    ui1_unicode = (UINT8)x_strtoll(pps_argv[1],NULL,10);
    
    /*Open the screen */
    if (NULL_HANDLE == h_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_screen);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open screen\n");
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_plane)
    {
        /*Open the plane */
        i4_ret = x_gl_plane_open (h_screen,
                                  1,
                                  &h_plane);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open plane\n");
            x_handle_free(h_screen);
            return CLIR_OK;
        }
        
        /*Link the plane*/
        i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't link plane\n");
            x_handle_free(h_screen);
            x_handle_free(h_plane);
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_canvas)
    {
        x_wgl_get_canvas(h_plane, &h_canvas);
    }
    
    t_cfg.e_sbtl_type = MM_SBTL_TYPE_TEXT_POWERDIVX;
    t_cfg.i4_speed = 100;
    t_cfg.h_gl_plane = h_canvas;

    if (NULL_HANDLE == h_subtitle_file)
    {
        if(ui1_unicode == 0)
        {
        x_fm_open(FM_ROOT_HANDLE, 
                  PSB_TEST_FILE,
                  FM_READ_ONLY, 
                  0666, 
                  FALSE, 
                  &h_subtitle_file);
        }
        else
        {
            x_fm_open(FM_ROOT_HANDLE, 
                      PSB_W2S_TEST_FILE,
                      FM_READ_ONLY, 
                      0666, 
                      FALSE, 
                      &h_subtitle_file);
        }       
        
        t_feeder.h_feeder = h_subtitle_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;

        t_feeder.t_feeder_le_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;
    }
    
    t_cfg.pt_feeder[MM_SBTL_FEEDER_SUBTITLE] = &t_feeder;
    
    x_memset(&t_cfg.t_font_info, 0, sizeof(WGL_FONT_INFO_T));
    x_strcpy(t_cfg.t_font_info.a_c_font_name, SN_FONT_DEFAULT);
    t_cfg.t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
    t_cfg.t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
    t_cfg.t_font_info.e_font_cmap  = FE_CMAP_ENC_UNICODE;

    t_cfg.t_bg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    t_cfg.t_fg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    t_cfg.ui1_text_alignment = WGL_AS_CENTER_BOTTOM;
    
    x_sbtl_engine_open(&t_cfg,
                     TRUE,
                     _mm_sbtl_state_notify,
                     NULL,
                     &pv_engine);
    
    return CLIR_OK;
}

static INT32 _sbtl_cli_dvdss_open(INT32               i4_argc, 
                                  const CHAR**        pps_argv)
{
    SBTL_CFG_T      t_cfg;
    INT32           i4_ret;
    UINT8                   ui1_unicode;

    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] dsso [1=UTF16,0=UTF8 ]\n\r");
        return CLIR_OK;
    }
    ui1_unicode = (UINT8)x_strtoll(pps_argv[1],NULL,10);
    
    /*Open the screen */
    if (NULL_HANDLE == h_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_screen);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open screen\n");
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_plane)
    {
        /*Open the plane */
        i4_ret = x_gl_plane_open (h_screen,
                                  1,
                                  &h_plane);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open plane\n");
            x_handle_free(h_screen);
            return CLIR_OK;
        }
        
        /*Link the plane*/
        i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't link plane\n");
            x_handle_free(h_screen);
            x_handle_free(h_plane);
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_canvas)
    {
        x_wgl_get_canvas(h_plane, &h_canvas);
    }
    
    t_cfg.e_sbtl_type = MM_SBTL_TYPE_TEXT_DVDSS;
    t_cfg.i4_speed = 100;
    t_cfg.h_gl_plane = h_canvas;

    if (NULL_HANDLE == h_subtitle_file)
    {
        if(ui1_unicode == 0)
        {
        x_fm_open(FM_ROOT_HANDLE, 
                  DVDSS_TEST_FILE,
                  FM_READ_ONLY, 
                  0666, 
                  FALSE, 
                  &h_subtitle_file);
        }
        else
        {
            x_fm_open(FM_ROOT_HANDLE, 
                      DVDSS_W2S_TEST_FILE,
                      FM_READ_ONLY, 
                      0666, 
                      FALSE, 
                      &h_subtitle_file);
        }     
        
        t_feeder.h_feeder = h_subtitle_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;

        t_feeder.t_feeder_le_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;
    }
    
    t_cfg.pt_feeder[MM_SBTL_FEEDER_SUBTITLE] = &t_feeder;
    
    x_memset(&t_cfg.t_font_info, 0, sizeof(WGL_FONT_INFO_T));
    x_strcpy(t_cfg.t_font_info.a_c_font_name, SN_FONT_DEFAULT);
    t_cfg.t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
    t_cfg.t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
    t_cfg.t_font_info.e_font_cmap  = FE_CMAP_ENC_UNICODE;

    t_cfg.t_bg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    t_cfg.t_fg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    t_cfg.ui1_text_alignment = WGL_AS_CENTER_BOTTOM;
    
    x_sbtl_engine_open(&t_cfg,
                     TRUE,
                     _mm_sbtl_state_notify,
                     NULL,
                     &pv_engine);
    
    return CLIR_OK;

}

static INT32 _sbtl_cli_close(INT32              i4_argc, 
                             const CHAR**       pps_argv)
{
    x_sbtl_engine_close(pv_engine);
    pv_engine = NULL;

    if (h_subtitle_file)
    {
        x_fm_close(h_subtitle_file);
        h_subtitle_file = NULL_HANDLE;
    }

    if (h_index_file)
    {
        x_fm_close(h_index_file);
        h_index_file = NULL_HANDLE;
    }

    
    return CLIR_OK;
}

static INT32 _sbtl_cli_start(INT32              i4_argc, 
                             const CHAR**       pps_argv)
{
    x_sbtl_engine_set(pv_engine,
                    EVN_MM_SBTL_APP_PLAY_REQ,
                    NULL,
                    0);
    return CLIR_OK;
}


static INT32 _sbtl_cli_stop(INT32               i4_argc, 
                            const CHAR**        pps_argv)
{
    x_sbtl_engine_set(pv_engine,
                    EVN_MM_SBTL_APP_STOP_REQ,
                    NULL,
                    0);

    return CLIR_OK;
}

#endif
/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: mm_sbtl_cli_init
 *
 * Description: This API initializes multimedia subtitle CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 mm_sbtl_cli_init(VOID)
{
#ifdef CLI_LVL_ALL

    INT32       i4_return;
    
    i4_return = x_cli_attach_cmd_tbl(at_mm_sbtl_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);

    if (i4_return != CLIR_OK)
    {
        return MMSBTLR_CLI_ERROR;
    }

    return MMSBTLR_OK;
#else
    return MMSBTLR_OK;
#endif
}

INT32 mm_sbtl_cli_get_engine(SBTL_ENGINE_T* pt_engine)
{
#ifdef CLI_LVL_ALL
    pv_engine = (VOID*)pt_engine;
    return MMSBTLR_OK;
#else
    return MMSBTLR_OK;
#endif
}

#ifdef CLI_LVL_ALL
static INT32 _sbtl_cli_set_encoding_info(INT32              i4_argc, 
                                     const CHAR**             pps_argv)
{
    UINT8                           ui1_option;
    MM_SBTL_ENCODING_T              e_encoding;
    
    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] encoding [Option](0=Auto,1=ASCI,2=UTF8,3=UTF16,4=GB2312,5=BIG5)\n\r");
        return CLIR_OK;
    } 

    if(pv_engine == NULL)
    {
        x_dbg_stmt("[MM Sbtl] _gpt_engine IS NULL\n\r");
        return CLIR_OK;
    }

    ui1_option = (UINT8)x_strtoll(pps_argv[1],NULL,10);
    if(ui1_option == 0)
    {
        e_encoding = MM_SBTL_ENCODE_AUTO;
    }
    else if(ui1_option == 1)
    {
        e_encoding = MM_SBTL_ENCODE_ASCI;
    }
    else if(ui1_option == 2)
    {
        e_encoding = MM_SBTL_ENCODE_UTF8;
    }
    else if(ui1_option == 3)
    {
        e_encoding = MM_SBTL_ENCODE_UTF16;
    }
    else if(ui1_option == 4)
    {
        e_encoding = MM_SBTL_ENCODE_GB2312;
    }
    else if(ui1_option == 5)
    {
        e_encoding = MM_SBTL_ENCODE_BIG5;
    }
    
    x_sbtl_engine_set(pv_engine,
                    EVN_MM_SBTL_SET_ENCODING,
                    &e_encoding,
                    sizeof(MM_SBTL_ENCODING_T));    
    
    return CLIR_OK;
}
#endif    

#ifdef CLI_LVL_ALL
static INT32 _sbtl_cli_set_font_info(INT32              i4_argc, 
                                     const CHAR**             pps_argv)
{
    UINT8                           ui1_option;
    WGL_FONT_INFO_T                 t_font_info;
    
    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] Font [Option](0=UTF8,1=UTF16,2=Big5,3=GB2312,4=GB large)\n\r");
        return CLIR_OK;
    } 

    if(pv_engine == NULL)
    {
        x_dbg_stmt("[MM Sbtl] _gpt_engine IS NULL\n\r");
        return CLIR_OK;
    }

    ui1_option = (UINT8)x_strtoll(pps_argv[1],NULL,10);
    x_memset(&t_font_info, 0, sizeof(WGL_FONT_INFO_T));
    if(ui1_option == 0)
    {
        x_strcpy(t_font_info.a_c_font_name, SN_FONT_DEFAULT);
        t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
        t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
        t_font_info.e_font_cmap  = FE_CMAP_ENC_UNICODE;
    }
    else if(ui1_option == 1)
    {
        x_strcpy(t_font_info.a_c_font_name, SN_FONT_DEFAULT);
        t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
        t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
        t_font_info.e_font_cmap  = FE_CMAP_ENC_UNICODE;
    }
    else if(ui1_option == 2)
    {
        x_strcpy(t_font_info.a_c_font_name, "fnt_GB");
        t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
        t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
        t_font_info.e_font_cmap  = FE_CMAP_ENC_BIG5;
    }
    else if(ui1_option == 3)
    {
        x_strcpy(t_font_info.a_c_font_name, "fnt_GB");
        t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
        t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
        t_font_info.e_font_cmap  = FE_CMAP_ENC_GB2312;
    }
    else if(ui1_option == 4)
    {
        x_strcpy(t_font_info.a_c_font_name, "fnt_GB");
        t_font_info.e_font_size  = FE_FNT_SIZE_LARGE;
        t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
        t_font_info.e_font_cmap  = FE_CMAP_ENC_GB2312;
    }    
    else if(ui1_option == 5)
    {
        x_strcpy(t_font_info.a_c_font_name, "fnt_GB");
        t_font_info.e_font_size  = FE_FNT_SIZE_CUSTOM;
        t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
        t_font_info.e_font_cmap  = FE_CMAP_ENC_GB2312;
        t_font_info.ui1_custom_size = 40;
    } 
    
    x_sbtl_engine_set(pv_engine,
                    EVN_MM_SBTL_SET_OSD_FONT_INFO,
                    &t_font_info,
                    sizeof(WGL_FONT_INFO_T));   
    return CLIR_OK;
}
#endif
#ifdef CLI_LVL_ALL
static INT32 _sbtl_cli_set_hlt_fg(INT32              i4_argc, 
                                     const CHAR**             pps_argv)
{
    UINT8                           ui1_option;
    GL_COLOR_T                      t_color;
    
    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] hlt_fg [Option](0=black,1=white,2=,)\n\r");
        return CLIR_OK;
    }     
    
    ui1_option = (UINT8)x_strtoll(pps_argv[1],NULL,10);
    x_memset(&t_color, 0, sizeof(GL_COLOR_T));
    
    if(ui1_option == 0)
    {
        t_color.a = 255;
        t_color.u1.r = 0;
        t_color.u2.g = 0;
        t_color.u3.b = 0;
    }
    else if(ui1_option == 1)
    {
        t_color.a = 255;
        t_color.u1.r = 255;
        t_color.u2.g = 255;
        t_color.u3.b = 255;
    }
    else if(ui1_option == 2)
    {
        t_color.a = 255;
        t_color.u1.r = 249;
        t_color.u2.g = 239;
        t_color.u3.b = 60;

    }
    else if(ui1_option == 3)
    {
        t_color.a = 255;
        t_color.u1.r = 86;
        t_color.u2.g = 216;
        t_color.u3.b = 67;
    }

    x_sbtl_engine_set(pv_engine,
                    EVN_MM_SBTL_SET_HLT_FG_COLOR,
                    &t_color,
                    sizeof(GL_COLOR_T));   

    return CLIR_OK;
}
#endif


#ifdef CLI_LVL_ALL
static INT32 _sbtl_cli_set_hlt_bg(INT32              i4_argc, 
                                     const CHAR**             pps_argv)
{
    UINT8                           ui1_option;
    GL_COLOR_T                      t_color;

    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] hlt_bg [Option](0=black,1=white,2=yellow,3=green)\n\r");
        return CLIR_OK;
    }    
    ui1_option = (UINT8)x_strtoll(pps_argv[1],NULL,10);
    x_memset(&t_color, 0, sizeof(GL_COLOR_T));
    
    if(ui1_option == 0)
    {
        t_color.a = 255;
        t_color.u1.r = 0;
        t_color.u2.g = 0;
        t_color.u3.b = 0;
    }
    else if(ui1_option == 1)
    {
        t_color.a = 255;
        t_color.u1.r = 255;
        t_color.u2.g = 255;
        t_color.u3.b = 255;
    }
    else if(ui1_option == 2)
    {
        t_color.a = 255;
        t_color.u1.r = 249;
        t_color.u2.g = 239;
        t_color.u3.b = 60;

    }
    else if(ui1_option == 3)
    {
        t_color.a = 255;
        t_color.u1.r = 86;
        t_color.u2.g = 216;
        t_color.u3.b = 67;
    }
    
    x_sbtl_engine_set(pv_engine,
                    EVN_MM_SBTL_SET_HLT_BG_COLOR,
                    &t_color,
                    sizeof(GL_COLOR_T));   

    return CLIR_OK;
}
#endif

#ifdef CLI_LVL_ALL
static INT32 _sbtl_cli_set_line_gap(INT32              i4_argc, 
                                     const CHAR**             pps_argv)
{
    UINT8                           ui1_option;
    INT32                      i4_data;
    
    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] gap [Option](0=0,1=10,2=20)\n\r");
        return CLIR_OK;
    }    
    ui1_option = (UINT8)x_strtoll(pps_argv[1],NULL,10);
    
    if(ui1_option == 0)
    {
        i4_data = 0;
    }
    else if(ui1_option == 1)
    {
        i4_data = 10;
    }
    else if(ui1_option == 2)
    {
        i4_data = 15;

    }

    x_sbtl_engine_set(pv_engine,
                    EVN_MM_SBTL_SET_LINE_GAP,
                    (VOID*)&i4_data,
                    sizeof(INT32));   

    return CLIR_OK;
}
#endif


#ifdef CLI_LVL_ALL
static INT32 _sbtl_cli_set_disp_page(INT32              i4_argc, 
                                     const CHAR**             pps_argv)
{
    UINT8                           ui1_option;
    MM_SBTL_DISP_PAGE_T                 t_disp_page;

    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] page [Option(mode,page line,interval line,hlt fix line,gap)](0=(fix,5,0,2,10),1=(fix,10,1,5,10),2=(fix,5,0,2,2)  \n");
        x_dbg_stmt(" 3=(fix,10,2,6,10), 4=(scroll line,10,2,6,10), 5=(fix,1,0,0,5))\n\r");
        return CLIR_OK;
    }    
    ui1_option = (UINT8)x_strtoll(pps_argv[1],NULL,10);
    x_memset(&t_disp_page, 0, sizeof(MM_SBTL_DISP_PAGE_T));

    if(ui1_option == 0)
    {
        t_disp_page.e_page_mode = MM_SBTL_PAGE_HLT_FIXED;
        t_disp_page.ui2_page_line_num = 5;
        t_disp_page.ui2_interval_line = 0;
        t_disp_page.ui2_hlt_fix_line = 2;
        t_disp_page.i4_line_gap = 10;
    }
    else if(ui1_option == 1)
    {
        t_disp_page.e_page_mode = MM_SBTL_PAGE_HLT_FIXED;
        t_disp_page.ui2_page_line_num = 10;
        t_disp_page.ui2_interval_line = 1;
        t_disp_page.ui2_hlt_fix_line = 2;
        t_disp_page.i4_line_gap = 10;
    }
    else if(ui1_option == 2)
    {
        t_disp_page.e_page_mode = MM_SBTL_PAGE_HLT_FIXED;
        t_disp_page.ui2_page_line_num = 5;
        t_disp_page.ui2_interval_line = 0;
        t_disp_page.ui2_hlt_fix_line = 2;
        t_disp_page.i4_line_gap = 2;
    }
    else if(ui1_option == 3)
    {
        t_disp_page.e_page_mode = MM_SBTL_PAGE_HLT_FIXED;
        t_disp_page.ui2_page_line_num = 10;
        t_disp_page.ui2_interval_line = 2;
        t_disp_page.ui2_hlt_fix_line = 6;
        t_disp_page.i4_line_gap = 10;
    }    
    else if(ui1_option == 4)
    {
        t_disp_page.e_page_mode = MM_SBTL_PAGE_SCROLL_LINE;
        t_disp_page.ui2_page_line_num = 10;
        t_disp_page.ui2_interval_line = 1;
        t_disp_page.ui2_hlt_fix_line = 2;
        t_disp_page.i4_line_gap = 10;
    } 
    else if(ui1_option == 5)
    {
        t_disp_page.e_page_mode = MM_SBTL_PAGE_HLT_FIXED;
        t_disp_page.ui2_page_line_num = 1;
        t_disp_page.ui2_interval_line = 0;
        t_disp_page.ui2_hlt_fix_line = 0;
        t_disp_page.i4_line_gap = 5;
    } 

    x_sbtl_engine_set(pv_engine,
                    EVN_MM_SBTL_SET_PAGE_INF,
                    &t_disp_page,
                    sizeof(MM_SBTL_DISP_PAGE_T));   
    
    return CLIR_OK;
}
#endif

#ifdef CLI_LVL_ALL
static INT32 _sbtl_cli_set_time_offset(INT32              i4_argc, 
                                     const CHAR**             pps_argv)
{
    UINT8                           ui1_option;
    MM_SBTL_TM_OFST_INF             t_time_ofst;

    if (i4_argc != 2)
    {
        x_dbg_stmt("[MM Sbtl] offset [Option](0=off,1=auto,2=user(2s),3= user(-2s))\n\r");
        return CLIR_OK;
    }    
    ui1_option = (UINT8)x_strtoll(pps_argv[1],NULL,10);
    x_memset(&t_time_ofst,0,sizeof(MM_SBTL_TM_OFST_INF));
    
    if(ui1_option == 0)
    {
        t_time_ofst.e_ofst_mode = MM_SBTL_TM_OFST_MODE_OFF;
    }
    else if(ui1_option == 1)
    {
        t_time_ofst.e_ofst_mode = MM_SBTL_TM_OFST_MODE_AUTO;
    }
    else if(ui1_option == 2)
    {
        t_time_ofst.e_ofst_mode = MM_SBTL_TM_OFST_MODE_USER;
        t_time_ofst.i8_time_ofst = 20;
    }
    else if(ui1_option == 3)
    {
        t_time_ofst.e_ofst_mode = MM_SBTL_TM_OFST_MODE_USER;
        t_time_ofst.i8_time_ofst = -20;
    }
    
    x_sbtl_engine_set(pv_engine,
                    EVN_MM_SBTL_SET_ADJUST_TIME,
                    (VOID*)&t_time_ofst,
                    sizeof(MM_SBTL_TM_OFST_INF));   

    return CLIR_OK;
}
#endif


#endif /*__KERNEL__*/


