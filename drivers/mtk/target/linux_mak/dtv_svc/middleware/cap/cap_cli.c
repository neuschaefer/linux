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
 * $RCSfile: img_cli.c,v $
 * $Revision:
 * $Date:
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/8 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 36c0b5ff3e590d92a1efcdb6b1e45528 $
 *
 * Description:
 *         This file implements CLI command table for image library.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "handle/handle.h"
#include "u_cap.h"
#include "x_cap.h"
#include "strm_mngr/x_sm.h"
#include "strm_mngr/scc/x_scc.h"
#include "strm_mngr/scc/_scc_vid.h"
#include "strm_mngr/scc/_scc_disp.h"
#include "res_mngr/rm.h"
#include "res_mngr/drv/x_vid_plane.h"

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static UINT16           ui2_cap_dbg_level   = DBG_LEVEL_NONE;
static BOOL             b_is_ready_to_cap   = FALSE;
static HANDLE_T         g_h_cap             = NULL_HANDLE;


static INT32 _cap_cli_get_dbg_level (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cap_cli_set_dbg_level (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cap_cli_open          (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cap_cli_capture       (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cap_cli_save          (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cap_cli_close         (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cap_cli_sync_stop_cap (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cap_cli_sync_stop_save(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cap_cli_async_stop_cap(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cap_cli_async_stop_save(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cap_cli_select_as_bootlogo(INT32 i4_argc, const CHAR** pps_argv);


/* cap library command table */
static CLI_EXEC_T at_cap_cmd_tbl[] =
{
    {CLI_GET_DBG_LVL_STR,   NULL,   _cap_cli_get_dbg_level,     NULL,   CLI_GET_DBG_LVL_HELP_STR,   CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,   _cap_cli_set_dbg_level,     NULL,   CLI_SET_DBG_LVL_HELP_STR,   CLI_SUPERVISOR},
    {"open",                NULL,   _cap_cli_open,              NULL,   "open capture engine",      CLI_SUPERVISOR},
    {"capture",             NULL,   _cap_cli_capture,           NULL,   "Do capture operation",     CLI_SUPERVISOR},
    {"save",                NULL,   _cap_cli_save,              NULL,   "save capture to file",     CLI_SUPERVISOR},
    {"close",               NULL,   _cap_cli_close,             NULL,   "close capture engine",     CLI_SUPERVISOR},
    {"ssc",                 NULL,   _cap_cli_sync_stop_cap,     NULL,   "sync stop capture",        CLI_SUPERVISOR},
    {"sss",                 NULL,   _cap_cli_sync_stop_save,    NULL,   "sync stop save",           CLI_SUPERVISOR},
    {"asc",                 NULL,   _cap_cli_async_stop_cap,    NULL,   "async stop capture",       CLI_SUPERVISOR},        
    {"ass",                 NULL,   _cap_cli_async_stop_save,   NULL,   "async stop save",          CLI_SUPERVISOR},
    {"slogo",               NULL,   _cap_cli_select_as_bootlogo,NULL,   "select one as bootup logo",CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/* IO Manager root command table */
static CLI_EXEC_T at_cap_root_cmd_tbl[] =
{
    {"cap",     NULL,        NULL,       at_cap_cmd_tbl,      "Capture library commands",   CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: cap_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 cap_get_dbg_level(VOID)
{
    return ui2_cap_dbg_level;
}


/*-----------------------------------------------------------------------------
 * Name: cap_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 ----------------------------------------------------------------------------*/
BOOL cap_set_dbg_level(UINT16 ui2_level)
{
    ui2_cap_dbg_level = ui2_level;
    
    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name: _img_cli_get_dbg_level
 *
 * Description: This API gets the current setting of image library debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _cap_cli_get_dbg_level(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;

    i4_return = x_cli_show_dbg_level(cap_get_dbg_level());

    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: _img_cli_set_dbg_level
 *
 * Description: This API sets the debug level of image library.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _cap_cli_set_dbg_level(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (cap_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }

        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }

    x_cli_show_dbg_level(cap_get_dbg_level());
    x_dbg_stmt("[CAP] set debug level %s\n\r", (i4_return == CLIR_OK) ? "successful" : "failed");
    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: cap_cli_init
 *
 * Description: This API initializes capture library CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: IMGR_OK                     Routine successful.
 *          IMGR_CLI_ERROR              CLI operation is failed.
 ----------------------------------------------------------------------------*/
INT32 cap_cli_init(VOID)
{
    INT32       i4_return;

    /* Attach compression library CLI command table to CLI */
    i4_return = x_cli_attach_cmd_tbl(at_cap_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);

    if (i4_return != CLIR_OK)
    {
        return CAPR_CLI_ERROR;
    }

    return CAPR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _x_cap_nfy_fct
 *
 * Description: This API will process msg received from capture engine.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static VOID _x_cap_nfy_fct(
                          HANDLE_T                h_handle,
                          const VOID*             pv_tag,
                          CAP_EVENT_TYPE_T        e_event,
                          const VOID*             pv_data)
{
    switch(e_event)
    {
    case CAP_EVENT_TYPE_OPEN_DONE:
        b_is_ready_to_cap = TRUE;
        break;
    case CAP_EVENT_TYPE_OPEN_ERROR:
        b_is_ready_to_cap = FALSE;
        break;
    case CAP_EVENT_TYPE_CAP_DONE:
        break;
    case CAP_EVENT_TYPE_CAP_ERR:
        break;
    default :
        break;
    }
}

/*-----------------------------------------------------------------------------
 * Name: x_cap_cust_fct
 *
 * Description: This API do custmor event.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static BOOL _x_cap_cust_fct(
                            CAP_SRC_TYPE_T          e_source,
                            CAP_EVENT_TYPE_T        e_event,
                            const VOID*             pv_data)
{
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _cap_cli_open
 *
 * Description: This API opens capture engine
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _cap_cli_open (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32               i4_ret;
    CAP_SRC_TYPE_T      e_source;
    HANDLE_T            h_scc_comp = NULL_HANDLE;
    
    if(i4_argc == 1)
    {
        e_source = CAP_SRC_TYPE_TV_VIDEO;
    }
    else
    {
        e_source = CAP_SRC_TYPE_MM_IMAGE;
    }
    i4_ret = _scc_comp_vid_open( NULL_HANDLE, SN_PRES_MAIN_DISPLAY, &h_scc_comp);
    if (i4_ret != SMR_OK) 
    {
        x_dbg_stmt("[CAP]_scc_comp_vid_open() failed, ret=%d\r\n", i4_ret);
        return CAPR_INTERNAL_ERR;
    }
    
    i4_ret = _scc_comp_vid_set(
                                h_scc_comp,
                                DRVT_VID_PLANE,
                                VID_PLA_SET_TYPE_MODE,
                                (VOID*)(UINT32)VID_PLA_FREEZE,
                                sizeof(VID_PLA_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return CAPR_INTERNAL_ERR;
    }
    i4_ret = x_cap_open(e_source,
                        _x_cap_nfy_fct,
                        NULL,
                        _x_cap_cust_fct,
                        NULL,
                        &g_h_cap
                        );
    if(i4_ret != CAPR_OK)
    {
        return CAPR_CLI_ERROR;
    }
    return CAPR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cap_cli_capture
 *
 * Description: This API starts to capture
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _cap_cli_capture(INT32 i4_argc, const CHAR** pps_argv)
{
    CAP_CAPTURE_INFO_T      t_cap_info;
    if(!b_is_ready_to_cap)
    {
        x_dbg_stmt("----------------Cap engine not ready yet!!! --------\r\n");
        return CAPR_OK;
    }
    if(i4_argc == 2)
    {
        t_cap_info.e_format = CAP_IMG_FMT_TYPE_MPEG;
    }
    else
    {
        t_cap_info.e_format = CAP_IMG_FMT_TYPE_JPEG;
    }
    
    t_cap_info.e_res_type = CAP_OUT_RES_TYPE_SD;
    t_cap_info.ui4_max_size = 512*1024;
    t_cap_info.ui4_quality = 80;
    x_cap_capture ( g_h_cap,
                    &t_cap_info);
    return CAPR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cap_cli_save
 *
 * Description: This API saves the captured image to specified path.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _cap_cli_save(INT32 i4_argc, const CHAR** pps_argv)
{
    CAP_LOGO_SAVE_INFO_T t_save_info;
    
    if(i4_argc == 1)
    {
        t_save_info.e_device_type = CAP_DEVICE_TYPE_INTERNAL;
        t_save_info.u.ui4_logo_id = 0;
    }
    else
    {
        t_save_info.e_device_type = CAP_DEVICE_TYPE_EXTERNAL;
        t_save_info.u.ps_path = NULL;//pps_argv[1];
    }
    
    x_cap_save(g_h_cap, &t_save_info);
    
    return CAPR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cap_cli_close
 *
 * Description: This API closes the captured engine.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _cap_cli_close(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32               i4_ret;
    HANDLE_T            h_scc_comp = NULL_HANDLE;
    
    i4_ret = _scc_comp_vid_open( NULL_HANDLE, SN_PRES_MAIN_DISPLAY, &h_scc_comp);
    if (i4_ret != SMR_OK) 
    {
        x_dbg_stmt("[CAP]_scc_comp_vid_open() failed, ret=%d\r\n", i4_ret);
        return CAPR_INTERNAL_ERR;
    }
    
    i4_ret = _scc_comp_vid_set(
                                h_scc_comp,
                                DRVT_VID_PLANE,
                                VID_PLA_SET_TYPE_MODE,
                                (VOID*)(UINT32)VID_PLA_NORMAL,
                                sizeof(VID_PLA_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return CAPR_INTERNAL_ERR;
    }

    x_cap_close(g_h_cap);
    g_h_cap = NULL_HANDLE;
    
    return CAPR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cap_cli_sync_stop_cap
 *
 * Description: This API sync stops the captured operation.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _cap_cli_sync_stop_cap(INT32 i4_argc, const CHAR** pps_argv)
{
    return x_cap_sync_stop_capture(g_h_cap);
}


/*-----------------------------------------------------------------------------
 * Name: _cap_cli_sync_stop_save
 *
 * Description: This API sync stops the capture saving operation.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _cap_cli_sync_stop_save(INT32 i4_argc, const CHAR** pps_argv)
{
    return x_cap_sync_stop_save(g_h_cap);
}


/*-----------------------------------------------------------------------------
 * Name: _cap_cli_async_stop_cap
 *
 * Description: This API async stops the capture operation.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _cap_cli_async_stop_cap(INT32 i4_argc, const CHAR** pps_argv)
{
    return CAPR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cap_cli_async_stop_save
 *
 * Description: This API async stops the capture saving operation.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _cap_cli_async_stop_save(INT32 i4_argc, const CHAR** pps_argv)
{
    return CAPR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cap_cli_select_as_bootlogo
 *
 * Description: This API select the user specified image as bootup logo.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _cap_cli_select_as_bootlogo(INT32 i4_argc, const CHAR** pps_argv)
{
    CAP_LOGO_SELECT_INFO_T      t_select_info;

    t_select_info.e_device_type = CAP_DEVICE_TYPE_INTERNAL;
    t_select_info.u.ui4_logo_id = 0;
    
    x_cap_select_as_bootlogo (&t_select_info);
    
    return CAPR_OK;
}


#endif

