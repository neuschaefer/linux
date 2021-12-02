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
 * $RCSfile:  $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains Record Handler CLI implementations.
 *---------------------------------------------------------------------------*/

#ifdef SYS_RECORD_SUPPORT

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "svctx/rec_hdlr/rec_hdlr_cli.h"
#include "svctx/rec_hdlr/rec_hdlr.h"
#include "svctx/x_svctx.h"
#include "cli/x_cli.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#define DBG_LEVEL_MODULE     rec_hdlr_get_dbg_level()


/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
static INT32 _rec_hdlr_cli_set_dbg_level (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _rec_hdlr_cli_get_dbg_level (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _rec_hdlr_cli_list_rec_svctx (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _rec_hdlr_cli_pause_record (INT32 i4_argc, const CHAR** pps_argv);


/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/
static CLI_EXEC_T at_g_rec_hdlr_cmd_tbl[] = 
{
	{
        CLI_SET_DBG_LVL_STR,
        NULL,
        _rec_hdlr_cli_set_dbg_level,
        NULL,
        CLI_SET_DBG_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
    
 	{
        CLI_GET_DBG_LVL_STR,
        NULL,
        _rec_hdlr_cli_get_dbg_level,
        NULL,
        CLI_GET_DBG_LVL_HELP_STR,
        CLI_SUPERVISOR
    },

    {
        "list",
        "lst",
        _rec_hdlr_cli_list_rec_svctx,
        NULL,
        "list all created rec svctx",
        CLI_SUPERVISOR
    },

    {
        "pause_rec",
        "pause",
        _rec_hdlr_cli_pause_record,
        NULL,
        "pause rec [rec_svctx_handle]  (0: the first rec_svctx in list)",
        CLI_SUPERVISOR
    },
    
	END_OF_CLI_CMD_TBL
};    

/* RECORD HDLR root command table */
static CLI_EXEC_T at_g_rec_hdlr_root_cmd_tbl[] =
{
	{"svctx_rec", NULL, NULL, at_g_rec_hdlr_cmd_tbl, "SVCTX-REC_HDLR commands", CLI_SUPERVISOR},
	
	END_OF_CLI_CMD_TBL
};

/*-----------------------------------------------------------------------------
                    internal functions 
 ----------------------------------------------------------------------------*/
static INT32 _rec_hdlr_cli_set_dbg_level (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_ret;
    UINT16      ui2_dbg_level;
    
	if ((i4_argc != 2) || (pps_argv == NULL))
	{
		x_dbg_stmt("Usage: svctx_rec.sdl [level]\n");
		x_dbg_stmt("                     [level]: n=none/e=error, i=info\n");
		return CLIR_INV_CMD_USAGE;
	}
	
    i4_ret = x_cli_parse_dbg_level (i4_argc, 
                                    pps_argv, 
                                    & ui2_dbg_level);
	if (i4_ret == CLIR_OK)
	{
	    rec_hdlr_set_dbg_level (ui2_dbg_level);

        i4_ret = x_cli_show_dbg_level (rec_hdlr_get_dbg_level ());
	}
	else
	{
		x_dbg_stmt("Usage: svctx_rec.sdl [level]\n");
		x_dbg_stmt("                     [level]: n=none/e=error, i=info\n");
		i4_ret = CLIR_INV_CMD_USAGE;
	}
	
    return i4_ret;
}
    
static INT32 _rec_hdlr_cli_get_dbg_level (INT32 i4_argc, const CHAR** pps_argv)
{
    return x_cli_show_dbg_level (rec_hdlr_get_dbg_level ());
}    

static INT32 _rec_hdlr_cli_list_rec_svctx (INT32 i4_argc, const CHAR** pps_argv)
{
    SVCTX_T*        pt_svctx;
    
	if ((i4_argc != 1) || (pps_argv == NULL))
	{
		x_dbg_stmt("Usage: svctx_rec.list \n");
		return CLIR_INV_CMD_USAGE;
	}
    
    x_dbg_stmt ("=========================\n");

    pt_svctx = svctx_get_using_head ();
    
    while (pt_svctx)
    {
        if (IS_SVCTX_RECORD (pt_svctx))
        {
            REC_SVC_STATE_T     e_state;
        
            if (pt_svctx->pt_running_svc_req)
            {
                REC_SVC_REQ_T*  pt_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
                
                e_state = pt_svc_req->e_state;
            }
            else
            {
                e_state = 0xFF;
            }
            
            x_dbg_stmt ("[%d] \"%s\" (%d)\n", 
                        pt_svctx->h_sys_svctx,
                        pt_svctx->s_name,
                        e_state );
        }

        pt_svctx = DLIST_NEXT (pt_svctx, t_dlink);
    }
    
    x_dbg_stmt ("=========================\n");

    return CLIR_OK;
}

static INT32 _rec_hdlr_cli_pause_record (INT32 i4_argc, const CHAR** pps_argv)
{
    UINT32          ui4_handle;
    HANDLE_T        h_svctx_rec;
    SVCTX_T*        pt_svctx_rec;
    INT32           i4_ret;
    
	if ((i4_argc != 2) || (pps_argv == NULL))
	{
		x_dbg_stmt ("Usage: svctx_rec.pause [rec_svctx_handle] \n");
		x_dbg_stmt ("                        (0: the first rec_svctx in list)\n");
		return CLIR_INV_CMD_USAGE;
	}
    
    ui4_handle = (UINT32) x_strtoull (pps_argv[1],
                                      NULL,
                                      10);
    if (ui4_handle != 0)
    {
        h_svctx_rec = (HANDLE_T) ui4_handle;
    }
    else
    {
        /* find the first record svctx in the using list */
        
        SVCTX_T*    pt_svctx;
        
        h_svctx_rec = NULL_HANDLE;
        pt_svctx    = svctx_get_using_head ();
        
        while (pt_svctx)
        {
            if (IS_SVCTX_RECORD (pt_svctx))
            {
                h_svctx_rec = pt_svctx->h_sys_svctx;
                break;
            }
    
            pt_svctx = DLIST_NEXT (pt_svctx, t_dlink);
        }
        
        if (h_svctx_rec == NULL_HANDLE)
        {
            x_dbg_stmt ("No record svctx found in the using list.\n");
            return CLIR_INV_ARG;
        }
    }
    
    i4_ret = svctx_get_obj (h_svctx_rec, 
                            & pt_svctx_rec );
    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt ("Invalid REC_SVCTX handle.\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    if (pt_svctx_rec->pt_running_svc_req)
    {
        REC_SVC_REQ_T*  pt_svc_req;
        
        pt_svc_req = (REC_SVC_REQ_T *) pt_svctx_rec->pt_running_svc_req;
        
        pt_svc_req->b_inform_paused = TRUE;
    }
    else
    {
        x_dbg_stmt ("No running svc in REC_SVCTX.\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    i4_ret = x_svctx_rec_pause (h_svctx_rec);
                                
    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt ("Fail to pause record (%d).\n", i4_ret);
    }                                
    
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
                    public functions 
 ----------------------------------------------------------------------------*/

INT32 rec_hdlr_cli_attach_cmd_tbl (VOID)
{
    return x_cli_attach_cmd_tbl (at_g_rec_hdlr_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_PIPE);
}    

VOID rec_hdlr_cli_dump_paused_info (
    SVCTX_T*            pt_svctx,
    REC_SVC_REQ_T*      pt_svc_req)
{
    INT32                   i4_ret;
    MM_BUF_RANGE_INFO_T     t_range;
    
    i4_ret = rec_util_get_valid_range (pt_svc_req->h_rec_util,
                                       & t_range );
    if (i4_ret != REC_UTILR_OK)
    {
        x_dbg_stmt ("Fail to get valid range (%d) !\n", i4_ret);
        return;
    }
    
    x_dbg_stmt ("[INFO] Record service is paused. \n");
    x_dbg_stmt ("[INFO] REC_UTIL handle = %d \n", 
                pt_svc_req->h_rec_util);
    x_dbg_stmt ("[INFO] Valid range: [start=%d, end=%d]\n", 
                t_range.ui4_start, 
                t_range.ui4_end );
    
}    

#endif
#endif

