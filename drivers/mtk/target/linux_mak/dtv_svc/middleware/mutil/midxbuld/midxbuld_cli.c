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
 * $RCSfile: midxbuld_cli.c,v $
 * $Revision:
 * $Date:
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 37ebbb2d76fad2e8a90655e5f2bf2c3a $
 *
 * Description:
 *         This file implements get/set debug level APIs for Midxbuld.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/x_common.h"
#include "midxbuld_dbg.h"
#include "mutil/midxbuld/x_midxbuld.h"
#include "mutil/midxbuld/midxbuld.h"
#include "mutil/midxbuld/midxbuld_cli.h"
#include "cli/x_cli.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "mutil/midxbuld/avi_idxbuld/avi_midxbuld_cli.h"
#include "mutil/midxbuld/asf_idxbuld/asf_midxbuld_cli.h"
#include "mutil/midxbuld/mp4_idxbuld/mp4_midxbuld_cli.h"
#include "mutil/midxbuld/mkv_idxbuld/mkv_midxbuld_cli.h"
#include "mutil/midxbuld/rm_idxbuld/rm_midxbuld_cli.h"

#else

#include "x_common.h"
#include "x_midxbuld.h"
#include "midxbuld.h"
#include "midxbuld_cli.h"
#include "x_cli.h"
#include "x_dbg.h"

#include "avi_idxbuld/avi_midxbuld_cli.h"
#include "asf_idxbuld/asf_midxbuld_cli.h"
#include "mp4_idxbuld/mp4_midxbuld_cli.h"
#include "mkv_idxbuld/mkv_midxbuld_cli.h"
#include "rm_idxbuld/rm_midxbuld_cli.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL

static INT32 midxbuld_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);


/* minfo command table */
static CLI_EXEC_T at_midxbuld_cmd_tbl[] =
{
    {CLI_GET_DBG_LVL_STR,   NULL,  midxbuld_cli_get_dbg_level,  NULL,  CLI_GET_DBG_LVL_HELP_STR, CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,  midxbuld_cli_set_dbg_level,  NULL,  CLI_SET_DBG_LVL_HELP_STR, CLI_SUPERVISOR},
    {"avi" ,                NULL,                        NULL,  NULL,   "avi midxbuld commands", CLI_SUPERVISOR},
    {"asf" ,                NULL,                        NULL,  NULL,   "asf midxbuld commands", CLI_SUPERVISOR},
    {"mp4" ,                NULL,                        NULL,  NULL,   "mp4 midxbuld commands", CLI_SUPERVISOR},
    {"mkv" ,                NULL,                        NULL,  NULL,   "mkv midxbuld commands", CLI_SUPERVISOR},
    {"rm" ,                 NULL,                        NULL,  NULL,   "rm midxbuld commands", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/* minfo root command table */
static CLI_EXEC_T at_midxbuld_root_cmd_tbl[] =
{
    {"midxbuld",     NULL,        NULL,       at_midxbuld_cmd_tbl,      "midxbuld commands",   CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

#endif

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_get_dbg_level
 *
 * Description: This API gets the current setting of minfo debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_get_dbg_level(INT32        i4_argc,
                                        const CHAR** pps_argv)
{
    INT32       i4_return;

    i4_return = x_cli_show_dbg_level(midxbuld_get_dbg_level());

    return i4_return;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_set_dbg_level
 *
 * Description: This API sets the debug level of minfo.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_set_dbg_level(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (midxbuld_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }

        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }

    x_cli_show_dbg_level(midxbuld_get_dbg_level());
    DBG_INFO(("[midxbuld] set debug level %s\n\r", (i4_return == CLIR_OK) ? "successful" : "failed"));
    return i4_return;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_init
 *
 * Description: This API initializes mfmtrecg CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 midxbuld_cli_init(VOID)
{
#ifdef CLI_LVL_ALL

    INT32       i4_return;

#ifdef VIDEO_PLAYBACK_SUPPORT

    CLI_EXEC_T  *pt_cmd_array = NULL;

#if 1
    extern SLIST_T(_MIDXBULD_HANDLER_REG_T) gt_midxbuld_handler_reg_list;
    MIDXBULD_HANDLER_REG_T*    pt_reg;

    SLIST_FOR_EACH(pt_reg, &gt_midxbuld_handler_reg_list, t_link)
    {
        if (pt_reg->t_handler_fct_tbl.pf_midxbuld_cli_init != NULL)
        {
            pt_reg->t_handler_fct_tbl.pf_midxbuld_cli_init(&pt_cmd_array);
        }
        else
        {
            return MIDXBULDR_OK;
        }
        if(pt_reg->t_favor_media_format.e_media_type == MEDIA_TYPE_CONTAINER)
        {
            switch(pt_reg->t_favor_media_format.t_media_subtype.u.e_contnr_subtype)
            {
            case MEDIA_CONTNR_SUBTYPE_AVI:
                at_midxbuld_cmd_tbl[2].pt_next_level = pt_cmd_array;
                break;
            case MEDIA_CONTNR_SUBTYPE_ASF:
                at_midxbuld_cmd_tbl[3].pt_next_level = pt_cmd_array;
                break;
            case MEDIA_CONTNR_SUBTYPE_MP4:
                at_midxbuld_cmd_tbl[4].pt_next_level = pt_cmd_array;
                break;
            case MEDIA_CONTNR_SUBTYPE_MATROSKA:
                at_midxbuld_cmd_tbl[5].pt_next_level = pt_cmd_array;
                break;
            case MEDIA_CONTNR_SUBTYPE_RM:
                at_midxbuld_cmd_tbl[6].pt_next_level = pt_cmd_array;
                break;
            default:
                return MIDXBULDR_FMT_NOT_SUPPORT;
            }
            break;
        }
        else if(pt_reg->t_favor_media_format.e_media_type == MEDIA_TYPE_PS)
        {
            switch(pt_reg->t_favor_media_format.t_media_subtype.u.e_ps_subtype)
            {
            case MEDIA_PS_SUBTYPE_MPG:
                break;
            case MEDIA_PS_SUBTYPE_VOB:
                /*at_midxbuld_cmd_tbl[6].pt_next_level = pt_cmd_array;*/
                break;
            case MEDIA_PS_SUBTYPE_DAT:
                break;
            default:
                return MIDXBULDR_FMT_NOT_SUPPORT;
            }
            break;
        }
        /*break;only execute once*/
    }
#else  
    /* Attatch AVI command array */
    avi_midxbuld_cli_init(&pt_cmd_array);
    at_midxbuld_cmd_tbl[2].pt_next_level = pt_cmd_array;

    /* Attatch ASF command array */
    asf_midxbuld_cli_init(&pt_cmd_array);
    at_midxbuld_cmd_tbl[3].pt_next_level = pt_cmd_array;

    /* Attatch MP4 command array */
    mp4_midxbuld_cli_init(&pt_cmd_array);
    at_midxbuld_cmd_tbl[4].pt_next_level = pt_cmd_array;
    
    /* Attatch MKV command array*/
    mkv_midxbuld_cli_init(&pt_cmd_array);
    at_midxbuld_cmd_tbl[5].pt_next_level = pt_cmd_array;

    /* Attatch RM command array*/
    rm_midxbuld_cli_init(&pt_cmd_array);
    at_midxbuld_cmd_tbl[6].pt_next_level = pt_cmd_array;
#endif
#endif
    
    /* Attach msvctx library CLI command table to CLI */
    i4_return = x_cli_attach_cmd_tbl(at_midxbuld_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);

    if (i4_return != CLIR_OK)
    {
        return MIDXBULDR_CLI_ERROR;
    }

    return MIDXBULDR_OK;
#else
    return MIDXBULDR_CLI_ERROR;
#endif
}


