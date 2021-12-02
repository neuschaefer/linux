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
 * $RCSfile: x_time_msrt.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/3 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: c69be66ab828e9942d3e0dbf701c3ce7 $
 *
 * Description:
 *         This header file contains specific implementation.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifdef LINUX_TURNKEY_SOLUTION
#ifndef __KERNEL__

#include "os/inc/x_os.h"
#include "cli/x_cli.h"
#include "dbg/x_dbg.h"
//#include "util/ssma.h"
#include "time_msrt/x_time_msrt.h"
#include "time_msrt/u_time_msrt_name.h"
#include "time_msrt/u_time_msrt.h"

/*#include "include/cb_inc/cb_param.h"*/
#include "include/ioctl_inc/ioctl_param.h"
#include "ioctl/adpt_ioctl.h"

extern void* ssma_alloc(size_t size);
extern void ssma_free(void * p);

#ifdef DEBUG_RES_MNGR
#include "dbg/dbg.h"
#endif

typedef struct
{
    CHAR                              s_name[32];
    UINT16                            ui2_level;
    CHAR                              s_comment[16];
    INT32                             i4_ret;
    UINT32                            ui4_flag;
    UINT32                            ui4_tms;
    UINT16                            ui2_common_level;
}PARAM_TMS;

#define RM_DEBUG_INF(A) 
#if 0
extern UINT16 time_msrt_get_level( UINT16 ui2_tms_mdl_level );
#endif

/* TMS selection */
typedef enum
{
    TMS_SEL_INDIVIDUAL = 0,
    TMS_SEL_COMMON,
    TMS_SEL_INDIVIDUAL_COMMON
}   TMS_SEL_T;


extern UINT32 time_msrt_get_flag( VOID );
extern INT32 time_msrt_set_flag(UINT32 ui4_flag);
extern INT32 time_msrt_set_tms_sel( TMS_SEL_T e_tms);
extern INT32 time_msrt_get_tms_sel( TMS_SEL_T* pe_tms);
extern UINT16 time_msrt_get_level( UINT16 ui2_tms_mdl_level );


/* Some CLI definitions. */
#define CLI_TMS_MODULE             "tms"

#define CLI_SET_TMS_LEVEL          CLI_SET_TMS_LVL_STR
#define CLI_GET_TMS_LEVEL          CLI_GET_TMS_LVL_STR
#define CLI_SET_TMS_SEL            "sts"
#define CLI_GET_TMS_SEL            "gts"
#define CLI_SET_TMS_PATH           "stp"
#define CLI_GET_TMS_PATH           "gtp"
#define CLI_DUMP_TMS_REC           "d"
#define CLI_DUMP_TMS_DETAIL_REC    "dd"
#define CLI_CLEAN_TMS_REC          "cl"
#define CLI_TMS_TEST               "test"

#define CLI_SET_TMS_LEVEL_HELP     CLI_SET_TMS_LVL_HELP_STR
#define CLI_GET_TMS_LEVEL_HELP     CLI_GET_TMS_LVL_HELP_STR
#define CLI_SET_TMS_SEL_HELP       "Set TMS select state (i = individual, c = common, b = individual + common)"
#define CLI_GET_TMS_SEL_HELP       "Get TMS select state (i = individual, c = common, b = individual + common)"
#define CLI_SET_TMS_PATH_HELP      "Set TMS main path (n = none, b = boot, c = channel change, s = auto scan)"
#define CLI_GET_TMS_PATH_HELP      "Get TMS main path (n = none, b = boot, c = channel change, s = auto scan)"

/* Common or central time measurement level. */
static UINT16      ui2_common_tms_level = TMS_LEVEL_NONE;



static TMS_SEL_T   e_tms_sel = TMS_SEL_INDIVIDUAL_COMMON;
static BOOL b_tms_init = FALSE;

/* The following is CLI related stuff. */
#if ((defined CLI_LVL_ALL)||(defined CLI_LVL_GUEST))
static INT32 cli_get_common_tms_level (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_set_common_tms_level (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_get_tms_path (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_set_tms_path (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_get_tms_sel (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_set_tms_sel (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_dump_tms_rec (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_dump_detail_tms_rec (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_clean_tms_rec (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_tms_test (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static CLI_EXEC_T at_cli_tms_cmd [] =
{
    {CLI_GET_TMS_LEVEL, NULL, cli_get_common_tms_level, NULL, CLI_GET_TMS_LEVEL_HELP, CLI_SUPERVISOR},
    {CLI_SET_TMS_LEVEL, NULL, cli_set_common_tms_level, NULL, CLI_SET_TMS_LEVEL_HELP, CLI_SUPERVISOR},
    {CLI_GET_TMS_PATH, NULL, cli_get_tms_path, NULL, CLI_GET_TMS_PATH_HELP, CLI_SUPERVISOR},
    {CLI_SET_TMS_PATH, NULL, cli_set_tms_path, NULL, CLI_SET_TMS_PATH_HELP, CLI_SUPERVISOR},
    {CLI_GET_TMS_SEL,   NULL, cli_get_tms_sel,   NULL, CLI_GET_TMS_SEL_HELP,   CLI_SUPERVISOR},
    {CLI_SET_TMS_SEL,   NULL, cli_set_tms_sel,   NULL, CLI_SET_TMS_SEL_HELP,   CLI_SUPERVISOR},
    {CLI_DUMP_TMS_REC,  NULL, cli_dump_tms_rec,  NULL, "d [name], Dump brief TMS records", CLI_SUPERVISOR},
    {CLI_DUMP_TMS_DETAIL_REC,  NULL, cli_dump_detail_tms_rec,  NULL, "dd [name], Dump detail TMS records", CLI_SUPERVISOR},
    {CLI_CLEAN_TMS_REC, NULL, cli_clean_tms_rec,  NULL, "cl [name], Clean up TMS records", CLI_SUPERVISOR},
    {CLI_TMS_TEST,      NULL, cli_tms_test,       NULL, "test", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_tms [] =
{
    {CLI_TMS_MODULE, NULL, NULL, &(at_cli_tms_cmd [0]), "Time measurement commands", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};
#endif /* CLI_LVL_ALL */

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

#if ((defined CLI_LVL_ALL)||(defined CLI_LVL_GUEST))

static INT32 cli_tms_test(
    INT32         i4_argc,
    const CHAR**  pps_argv)
{
    x_time_msrt_begin("tms", ui2_common_tms_level, "test start");
    x_thread_delay(900);
    x_time_msrt_end("tms", ui2_common_tms_level , "test end");
    x_time_msrt_dump("tms");
    return TMSR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: cli_dump_tms_rec
 *
 * Description: This function is called by the CLI and dumps the TMS records.
 *
 * Inputs:  i4_argc  References.
 *          pps_argv References.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_dump_tms_rec (
    INT32         i4_argc,
    const CHAR**  pps_argv)
{
    if ( (i4_argc >= 1) &&
         (pps_argv != NULL) &&
         ((pps_argv[1]) != NULL))
    {
        x_time_msrt_dump( pps_argv[1] );
    }
    else if ( i4_argc == 0 )
    {
        x_time_msrt_dump( NULL );
    }
    else
    {
        x_dbg_stmt( "Invalid argument !!!\r\n" );
    }

    return (CLIR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: cli_dump_detail_tms_rec
 *
 * Description: This function is called by the CLI and handles the TMS level
 *              get command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_dump_detail_tms_rec (
    INT32         i4_argc,
    const CHAR**  pps_argv)
{
    if ( (i4_argc >= 1) &&
         (pps_argv != NULL) &&
         ((pps_argv[1]) != NULL))
    {
        x_time_msrt_dump_detail( pps_argv[1] );
    }
    else if ( i4_argc == 0 )
    {
        x_time_msrt_dump_detail( NULL );
    }
    else
    {
        x_dbg_stmt( "Invalid argument !!!\r\n" );
    }

    return (CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: cli_clean_tms_rec
 *
 * Description: This function is called by the CLI and handles the TMS level
 *              get command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_clean_tms_rec (
    INT32         i4_argc,
    const CHAR**  pps_argv)
{
    if ( (i4_argc >= 1) &&
         (pps_argv != NULL) &&
         ((pps_argv[1]) != NULL))
    {
        x_time_msrt_clean( pps_argv[1] );
    }
    else if ( i4_argc == 0 )
    {
        x_time_msrt_clean( NULL );
    }
    else
    {
        x_dbg_stmt( "Invalid argument !!!\r\n" );
    }

    return (CLIR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: cli_get_tms_level
 *
 * Description: This function is called by the CLI and handles the TMS level
 *              get command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_get_common_tms_level (
    INT32         i4_argc,
    const CHAR**  pps_argv)
{

    PARAM_TMS* pt_tms;
    INT32 i4_ret;
    UINT16 ui2_level = 0;
    RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

    pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
    if (pt_tms == NULL)
    {
        return -1;
    }
    pt_tms->ui2_common_level = ui2_common_tms_level;
    i4_ret = ADPT_IOCTL(IOCTL_TMS_GET_COMMON_LEVEL, pt_tms);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        ui2_level = pt_tms->ui2_level;
    }

    ui2_common_tms_level = pt_tms->ui2_common_level;
    
    ssma_free(pt_tms);

    x_cli_show_tms_level (ui2_common_tms_level);

    return (CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: cli_set_tms_level
 *
 * Description: This function is called by the CLI and handles the TMS level
 *              set command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_set_common_tms_level (
    INT32         i4_argc,
    const CHAR**  pps_argv)
{
    UINT16 ui2_tms_level;


    if (x_cli_parse_tms_level (i4_argc, pps_argv, &ui2_tms_level) == CLIR_OK)
    {
        PARAM_TMS* pt_tms;
        INT32 i4_ret;
        UINT16 ui2_level = 0;
        RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

        pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
        if (pt_tms == NULL)
        {
            return -1;
        }
        pt_tms->ui2_common_level = ui2_tms_level;
        i4_ret = ADPT_IOCTL(IOCTL_TMS_SET_COMMON_LEVEL, pt_tms);
        if (i4_ret >= 0) /* ioctl successfully */
        {
            ui2_level = pt_tms->ui2_level;
        }

        ui2_common_tms_level = pt_tms->ui2_common_level;
        
        ssma_free(pt_tms);
        
        ui2_common_tms_level = ui2_tms_level;
    }

    x_cli_show_tms_level (ui2_common_tms_level);

    return (CLIR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: cli_get_tms_path
 *
 * Description: This function is called by the CLI and handles the TMS path
 *              get command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_get_tms_path (INT32         i4_argc,
                               const CHAR**  pps_argv)
{
    x_dbg_stmt ("TMS path: ");
    UINT32 ui4_flag;
    ui4_flag = time_msrt_get_flag();
    switch (ui4_flag)
    {
    case TMS_FLAG_NONE:
        x_dbg_stmt ("n\n");
        break;

    case TMS_FLAG_BOOT:
        x_dbg_stmt ("b\n");
        break;

    case TMS_FLAG_CHG_CHL:
        x_dbg_stmt ("c\n");
        break;

    case TMS_FLAG_AUTO_SCN:
        x_dbg_stmt ("s\n");
        break;

    case TMS_FLAG_INP_CHG:
        x_dbg_stmt ("i\n");
        break;        

    default:
        x_dbg_stmt ("unknown\n");
        break;
    }

    return (CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: cli_set_tms_sel
 *
 * Description: This function is called by the CLI and handles the TMS path
 *              set command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK       Successful
 *          CLIR_INV_ARG  One or more invalid arguments
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_set_tms_path (INT32         i4_argc,
                               const CHAR**  pps_argv)
{
    INT32       i4_return = CLIR_INV_ARG;
    const CHAR* ps_tms_sel;
    UINT32 ui4_flag = TMS_FLAG_NONE;

    if ( (i4_argc >= 1) &&
         (pps_argv != NULL) &&
         ((ps_tms_sel = pps_argv[1]) != NULL))
    {
        i4_return = CLIR_OK;

        if (x_strlen (ps_tms_sel) == 1)
        {
            switch (*ps_tms_sel)
            {
                case 'N':
                case 'n':
                    ui4_flag = TMS_FLAG_NONE;
                    break;

                case 'S':
                case 's':
                    ui4_flag = TMS_FLAG_AUTO_SCN;
                    break;

                case 'C':
                case 'c':
                    ui4_flag = TMS_FLAG_CHG_CHL;
                    break;

                case 'B':
                case 'b':
                    ui4_flag = TMS_FLAG_BOOT;
                    break;

                case 'I':
                case 'i':
                    ui4_flag = TMS_FLAG_INP_CHG;
                    break;

                default:
                    x_dbg_stmt ("Invalid TMS path.\n");
                    break;
            }
        }
    }
    
    time_msrt_set_flag(ui4_flag);
    
    cli_get_tms_path( 0, NULL );

    return (i4_return);
}

/*-----------------------------------------------------------------------------
 * Name: cli_get_tms_sel
 *
 * Description: This function is called by the CLI and handles the TMS select
 *              get command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_get_tms_sel (INT32         i4_argc,
                              const CHAR**  pps_argv)
{
    x_dbg_stmt ("TMS select: ");

    time_msrt_get_tms_sel(&e_tms_sel);

    switch (e_tms_sel)
    {
    case TMS_SEL_INDIVIDUAL:
        x_dbg_stmt ("i\n");
        break;

    case TMS_SEL_COMMON:
        x_dbg_stmt ("c\n");
        break;

    case TMS_SEL_INDIVIDUAL_COMMON:
        x_dbg_stmt ("b\n");
        break;

    default:
        x_dbg_stmt ("unknown\n");
        break;
    }

    return (CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: cli_set_tms_sel
 *
 * Description: This function is called by the CLI and handles the debug select
 *              set command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK       Successful
 *          CLIR_INV_ARG  One or more invalid arguments
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_set_tms_sel (INT32         i4_argc,
                              const CHAR**  pps_argv)
{
    BOOL        b_tms_sel_set;
    INT32       i4_return = CLIR_INV_ARG;
    const CHAR* ps_tms_sel;


    if ( (i4_argc >= 1) &&
         (pps_argv != NULL) &&
         ((ps_tms_sel = pps_argv[1]) != NULL))
    {
        i4_return = CLIR_OK;

        b_tms_sel_set = FALSE;

        if (x_strlen (ps_tms_sel) == 1)
        {
            switch (*ps_tms_sel)
            {
                case 'I':
                case 'i':
                    b_tms_sel_set = TRUE;

                    e_tms_sel = TMS_SEL_INDIVIDUAL;
                    break;

                case 'C':
                case 'c':
                    b_tms_sel_set = TRUE;

                    e_tms_sel = TMS_SEL_COMMON;
                    break;

                case 'B':
                case 'b':
                    b_tms_sel_set = TRUE;

                    e_tms_sel = TMS_SEL_INDIVIDUAL_COMMON;
                    break;

                default:
                    break;
            }
        }

        /* Output error message if not successful. */
        if (! (b_tms_sel_set))
        {
            x_dbg_stmt ("Invalid TMS selection. %s\n", CLI_SET_TMS_SEL_HELP);
        }
    }
    
    time_msrt_set_tms_sel(e_tms_sel);
        
    cli_get_tms_sel( 0, NULL );

    return (i4_return);
}

#endif /* CLI_LVL_ALL */


/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: time_msrt_get_level
 *
 * Description: This funuction combines the module and common TMS level and
 *              returns the final level according to TMS selection.
 *
 * Inputs:  ui2_tms_mdl_level Contains the module level given by caller.
 *
 * Outputs: -
 *
 * Returns: ui2_tms_level  The final TMS level.
 *
 ----------------------------------------------------------------------------*/
UINT16 time_msrt_get_level( UINT16 ui2_tms_mdl_level )
{
    PARAM_TMS* pt_tms;
    INT32 i4_ret;
    UINT16 ui2_level = 0;
    RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

    pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
    if (pt_tms == NULL)
    {
        return -1;
    }
    pt_tms->ui2_level = ui2_tms_mdl_level;
    i4_ret = ADPT_IOCTL(IOCTL_TMS_GET_LEVEL_FLAG, pt_tms);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        ui2_level = pt_tms->ui2_level;
    }
    ssma_free(pt_tms);

    return ui2_level;
}

/*-----------------------------------------------------------------------------
 * Name: time_msrt_get_flag
 *
 * Description: This funuction retrieves the TMS path flag
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: ui4_flag  The TMS path.
 *
 ----------------------------------------------------------------------------*/
UINT32 time_msrt_get_flag( VOID )
{
    PARAM_TMS* pt_tms;
    INT32 i4_ret;
    UINT32 ui4_flag = 0;
    RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

    pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
    if (pt_tms == NULL)
    {
        return -1;
    }
    i4_ret = ADPT_IOCTL(IOCTL_TMS_GET_FLAG, pt_tms);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        ui4_flag = pt_tms->ui4_flag;
    }
    ssma_free(pt_tms);

    return ui4_flag;
}
/*-----------------------------------------------------------------------------
 * Name: time_msrt_set_flag
 *
 * Description: This funuction retrieves the TMS path flag
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: ui4_flag  The TMS path.
 *
 ----------------------------------------------------------------------------*/
INT32 time_msrt_set_flag( UINT32 ui4_flag )
{
    PARAM_TMS* pt_tms;
    INT32 i4_ret;
    RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

    pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
    if (pt_tms == NULL)
    {
        return -1;
    }
    pt_tms->ui4_flag = ui4_flag;
    i4_ret = ADPT_IOCTL(IOCTL_TMS_SET_FLAG, pt_tms);
  
    ssma_free(pt_tms);

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: time_msrt_set_tms_sel
 *
 * Description: This funuction retrieves the TMS path flag
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: ui4_flag  The TMS path.
 *
 ----------------------------------------------------------------------------*/
INT32 time_msrt_set_tms_sel( TMS_SEL_T e_tms)
{
    PARAM_TMS* pt_tms;
    INT32 i4_ret;
    RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

    pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
    if (pt_tms == NULL)
    {
        return -1;
    }
    pt_tms->ui4_tms = (UINT32)e_tms;
    i4_ret = ADPT_IOCTL(IOCTL_TMS_SET_TMS_SEL, pt_tms);
    ssma_free(pt_tms);
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: time_msrt_get_tms_sel
 *
 * Description: This funuction retrieves the TMS path flag
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: ui4_flag  The TMS path.
 *
 ----------------------------------------------------------------------------*/
INT32 time_msrt_get_tms_sel( TMS_SEL_T* pe_tms)
{
    PARAM_TMS* pt_tms;
    INT32 i4_ret;
    RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

    pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
    if (pt_tms == NULL)
    {
        return -1;
    }
    i4_ret = ADPT_IOCTL(IOCTL_TMS_SET_TMS_SEL, pt_tms);
    (*pe_tms) = (TMS_SEL_T)pt_tms->ui4_tms;
    ssma_free(pt_tms);
    return i4_ret;
}




/*-----------------------------------------------------------------------------
 * Name: time_msrt_init
 *
 * Description: Time measurement initialization function.
 *
 * Inputs:  ui4_max_num_records  Max TMS records.
 *
 * Outputs: -
 *
 * Returns: TMSR_FAIL  Success.
 *          TMSR_OK    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 time_msrt_init( SIZE_T z_max_num_records )
{
    if (!b_tms_init)
    {
        CRIT_STATE_T   t_crit_state;
        if (ADPT_IOCTL(IOCTL_TMS_INIT, NULL) >= 0)
        {
            b_tms_init = TRUE;
        }
        

        t_crit_state = x_crit_start();
        
#if ((defined CLI_LVL_ALL)||(defined CLI_LVL_GUEST))
        /* And register the TMS library with the CLI module. */
        if (x_cli_attach_cmd_tbl (&(at_cli_tms [0]), CLI_CAT_MW, CLI_GRP_NONE) != CLIR_OK)
        {
            x_crit_end( t_crit_state );
            DBG_ABORT (0);
        }
#endif

#ifdef TIME_MEASUREMENT
        TMS_BEGIN (TMS_COOL_BOOT_TIME);
        TMS_BEGIN_EX (TMS_FLAG_BOOT, TMS_COOL_BOOT_TIME, "time_msrt_init");
#endif
        x_crit_end( t_crit_state );
    }

    return TMSR_OK;
    
}


/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_spot
 *
 * Description: This function create the SPOT TMS record.
 *
 * Inputs:  s_name      Name of the SPOT TMS record.
 *          ui2_tms_lvl The TMS level of the record.
 *
 * Outputs: -
 *
 * Returns: TMSR_FAIL                 Success.
 *          TMSR_FAIL                 Fail.
 *          TMSR_INSUFFICIENT_MEMORY  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_time_msrt_spot(
    const CHAR*           s_name,
    UINT16                ui2_tms_lvl,
    const CHAR*           s_comment )
{
    if (b_tms_init)
    {
        PARAM_TMS* pt_tms;
        INT32 i4_ret;
        UINT32 ui4_lenth;
        RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

        pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
        if (pt_tms == NULL)
        {
            return -1;
        }
        
        ui4_lenth = x_strlen(s_name);
        if(ui4_lenth >= 32)
        {
            ui4_lenth = 31;
        }
        x_memset(pt_tms->s_name, 0, 32);
        x_memcpy(pt_tms->s_name,s_name,ui4_lenth);
        
        x_dbg_stmt("s_name, %s, length, %d\n", s_name, ui4_lenth);
        
        ui4_lenth = x_strlen(s_comment);
        if(ui4_lenth >= 16)
        {
            ui4_lenth = 15;
        }
        x_memset(pt_tms->s_comment, 0, 16);
        x_memcpy(pt_tms->s_comment,s_comment,ui4_lenth);
        
        pt_tms->ui2_level = ui2_tms_lvl;
        i4_ret = ADPT_IOCTL(IOCTL_TMS_SPOT, pt_tms);
        if (i4_ret >= 0) /* ioctl successfully */
        {
            i4_ret = pt_tms->i4_ret;
        }
        ssma_free(pt_tms);

        return i4_ret;
    }
    return TMSR_FAIL;
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_diff
 *
 * Description: This function create the SPOT TMS record.
 *
 * Inputs:  s_name      Name of the SPOT TMS record.
 *          ui2_tms_lvl The TMS level of the record.
 *
 * Outputs: -
 *
 * Returns: TMSR_FAIL                 Success.
 *          TMSR_FAIL                 Fail.
 *          TMSR_INSUFFICIENT_MEMORY  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_time_msrt_diff(
    const CHAR*           s_name,
    UINT16                ui2_tms_lvl,
    const CHAR*           s_comment )
{
    if (b_tms_init)
    {
        PARAM_TMS* pt_tms;
        INT32 i4_ret;
        UINT32 ui4_lenth;
        RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

        pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
        if (pt_tms == NULL)
        {
            return -1;
        }
        pt_tms->ui2_level = ui2_tms_lvl;
        
        ui4_lenth = x_strlen(s_name);
        if(ui4_lenth >= 32)
        {
            ui4_lenth = 31;
        }
        x_memset(pt_tms->s_name, 0, 32);
        x_memcpy(pt_tms->s_name,s_name,ui4_lenth);
        
        x_dbg_stmt("s_name, %s, length, %d\n", s_name, ui4_lenth);
        
        ui4_lenth = x_strlen(s_comment);
        if(ui4_lenth >= 16)
        {
            ui4_lenth = 15;
        }
        x_memset(pt_tms->s_comment, 0, 16);
        x_memcpy(pt_tms->s_comment,s_comment,ui4_lenth);
                
        i4_ret = ADPT_IOCTL(IOCTL_TMS_DIFF, pt_tms);
        if (i4_ret >= 0) /* ioctl successfully */
        {
            i4_ret = pt_tms->i4_ret;
        }
        ssma_free(pt_tms);

        return i4_ret;
    }
    return TMSR_FAIL;
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_begin
 *
 * Description: This function begins the BEGIN-END TMS record.
 *
 * Inputs:  s_name      Name of the SPOT TMS record.
 *          ui2_avg_cnt The MAX count of the TMS records.
 *          ui2_tms_lvl The TMS level of the record.
 *
 * Outputs: -
 *
 * Returns: TMSR_FAIL                 Success.
 *          TMSR_FAIL                 Fail.
 *          TMSR_INSUFFICIENT_MEMORY  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_time_msrt_begin(
    const CHAR*           s_name,
    UINT16                ui2_tms_lvl,
    const CHAR*           s_comment )
{
    if (b_tms_init)
    {
        PARAM_TMS* pt_tms;
        INT32 i4_ret;
        UINT32 ui4_lenth;
        RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

        pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
        if (pt_tms == NULL)
        {
            return -1;
        }
        pt_tms->ui2_level = ui2_tms_lvl;
        ui4_lenth = x_strlen(s_name);
        if(ui4_lenth >= 32)
        {
            ui4_lenth = 31;
        }
        x_memset(pt_tms->s_name, 0, 32);
        x_memcpy(pt_tms->s_name,s_name,ui4_lenth);
        
        x_dbg_stmt("s_name, %s, length, %d\n", s_name, ui4_lenth);
        
        ui4_lenth = x_strlen(s_comment);
        if(ui4_lenth >= 16)
        {
            ui4_lenth = 15;
        }
        x_memset(pt_tms->s_comment, 0, 16);
        x_memcpy(pt_tms->s_comment,s_comment,ui4_lenth);
        
        i4_ret = ADPT_IOCTL(IOCTL_TMS_BEGIN, pt_tms);
        if (i4_ret >= 0) /* ioctl successfully */
        {
            i4_ret = pt_tms->i4_ret;
        }
        ssma_free(pt_tms);

        return i4_ret;
    }
    return TMSR_FAIL;
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_end
 *
 * Description: This function ends the BEGIN-END TMS record.
 *
 * Inputs:  s_name      Name of the SPOT TMS record.
 *          ui2_tms_lvl The TMS level of the record.
 *
 * Outputs: -
 *
 * Returns: TMSR_FAIL                 Success.
 *          TMSR_FAIL                 Fail.
 *          TMSR_INSUFFICIENT_MEMORY  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_time_msrt_end(
    const CHAR*           s_name,
    UINT16                ui2_tms_lvl,
    const CHAR*           s_comment )
{
    if (b_tms_init)
    {
        PARAM_TMS* pt_tms;
        INT32 i4_ret;
        UINT32 ui4_lenth;
        RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

        pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
        if (pt_tms == NULL)
        {
            return -1;
        }
        pt_tms->ui2_level = ui2_tms_lvl; 

        ui4_lenth = x_strlen(s_name);
       if(ui4_lenth >= 32)
        if(ui4_lenth >= 32)
        {
            ui4_lenth = 31;
        }
        x_memset(pt_tms->s_name, 0, 32);
        x_memcpy(pt_tms->s_name,s_name,ui4_lenth);
        
        x_dbg_stmt("s_name, %s, length, %d\n", s_name, ui4_lenth);
        
        ui4_lenth = x_strlen(s_comment);
        if(ui4_lenth >= 16)
        {
            ui4_lenth = 15;
        }
        x_memset(pt_tms->s_comment, 0, 16);
        x_memcpy(pt_tms->s_comment,s_comment,ui4_lenth);
        
        i4_ret = ADPT_IOCTL(IOCTL_TMS_END, pt_tms);
        if (i4_ret >= 0) /* ioctl successfully */
        {
            i4_ret = pt_tms->i4_ret;
        }
        ssma_free(pt_tms);

        return i4_ret;
    }
    return TMSR_FAIL;
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_dump
 *
 * Description: This function dumps all records which matchs the name specified
 *              by caller.
 *
 * Inputs:  s_name Caller specified record name to be dump. Set it to NULL to
 *                 dump all.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID x_time_msrt_dump( const CHAR* s_name )
{
    if (b_tms_init)
    {
        PARAM_TMS* pt_tms;
        UINT32 ui4_lenth;
        RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

        pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
        if (pt_tms == NULL)
        {
            return;
        }
        ui4_lenth = x_strlen(s_name);
        if(ui4_lenth >= 32)
        {
            ui4_lenth = 31;
        }
        x_memset(pt_tms->s_name, 0, 32);
        x_memcpy(pt_tms->s_name,s_name,ui4_lenth);
        
        x_dbg_stmt("s_name, %s, length, %d\n", s_name, ui4_lenth);
        
        
        ADPT_IOCTL(IOCTL_TMS_DUMP, pt_tms);
        ssma_free(pt_tms);
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_dump_detail
 *
 * Description: This function dumps details of all records which matchs the
 *              name specified by caller.
 *
 * Inputs:  s_name Caller specified record name to be dump. Set it to NULL to
 *                 dump all.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID x_time_msrt_dump_detail( const CHAR* s_name )
{
    if (b_tms_init)
    {
        PARAM_TMS* pt_tms;
        UINT32 ui4_lenth;
        RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

        pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
        if (pt_tms == NULL)
        {
            return;
        }
        ui4_lenth = x_strlen(s_name);
        if(ui4_lenth >= 32)
        {
            ui4_lenth = 31;
        }
        x_memset(pt_tms->s_name, 0, 32);
        x_memcpy(pt_tms->s_name,s_name,ui4_lenth);
        ADPT_IOCTL(IOCTL_TMS_DUMP_DETAIL, pt_tms);
        ssma_free(pt_tms);
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_clean
 *
 * Description: This function clean up all records which matchs the
 *              name specified by caller.
 *
 * Inputs:  s_name Caller specified record name to be clean. Set it to NULL to
 *                 clean all.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID x_time_msrt_clean( const CHAR* s_name )
{
    if (b_tms_init)
    {
        PARAM_TMS* pt_tms;
        UINT32 ui4_lenth;
        RM_DEBUG_INF("[MD:Time Measurement] tms spot-0\n\n");

        pt_tms = (PARAM_TMS *)ssma_alloc(sizeof(PARAM_TMS));
        if (pt_tms == NULL)
        {
            return;
        }
        
        ui4_lenth = x_strlen(s_name);
        if(ui4_lenth >= 32)
        {
            ui4_lenth = 31;
        }
        x_memset(pt_tms->s_name, 0, 32);
        x_memcpy(pt_tms->s_name,s_name,ui4_lenth);
        
        ADPT_IOCTL(IOCTL_TMS_CLEAN, pt_tms);
        ssma_free(pt_tms);
    }
    return;
}

#endif
#else

#include "os/inc/x_os.h"
#include "cli/x_cli.h"
#include "dbg/x_dbg.h"
#include "time_msrt/x_time_msrt.h"
#include "hash/hash_api.h"

#include "time_msrt/u_time_msrt_name.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define TMS_LOCK( h_lock )                                         \
{                                                                  \
    INT32  i4_ret = x_sema_lock( h_lock, X_SEMA_OPTION_WAIT);      \
    if ( i4_ret != OSR_OK )                                        \
    {                                                              \
        DBG_ABORT( 0 );                                            \
    }                                                              \
}

#define TMS_UNLOCK( h_lock )                                       \
{                                                                  \
    INT32  i4_ret = x_sema_unlock( h_lock );                       \
    if ( i4_ret != OSR_OK )                                        \
    {                                                              \
        DBG_ABORT( 0 );                                            \
    }                                                              \
}

#define TMS_INIT_MAX_NUM_RECORDS     ((SIZE_T)128)

#define TMS_IS_INITIALIZED     ( h_tms_free_list_lock != NULL_HANDLE &&       \
                                 h_time_msrt_db       != NULL_HANDLE )

/* Determine the default common or central TMS level. */
#ifdef TMS_INIT_LEVEL_COMMON
#define TMS_INIT_LEVEL  TMS_INIT_LEVEL_COMMON

#else /* TMS_INIT_LEVEL_COMMON */
#define TMS_INIT_LEVEL  TMS_LEVEL_NONE

#endif /* TMS_INIT_LEVEL_COMMON */

#define TMS_MIN( a, b )     ((a<=b)?a:b)

/* Some CLI definitions. */
#define CLI_TMS_MODULE             "tms"

#define CLI_SET_TMS_LEVEL          CLI_SET_TMS_LVL_STR
#define CLI_GET_TMS_LEVEL          CLI_GET_TMS_LVL_STR
#define CLI_SET_TMS_SEL            "sts"
#define CLI_GET_TMS_SEL            "gts"
#define CLI_SET_TMS_PATH           "stp"
#define CLI_GET_TMS_PATH           "gtp"
#define CLI_DUMP_TMS_REC           "d"
#define CLI_DUMP_TMS_DETAIL_REC    "dd"
#define CLI_CLEAN_TMS_REC          "cl"

#define CLI_SET_TMS_LEVEL_HELP     CLI_SET_TMS_LVL_HELP_STR
#define CLI_GET_TMS_LEVEL_HELP     CLI_GET_TMS_LVL_HELP_STR
#define CLI_SET_TMS_SEL_HELP       "Set TMS select state (i = individual, c = common, b = individual + common)"
#define CLI_GET_TMS_SEL_HELP       "Get TMS select state (i = individual, c = common, b = individual + common)"
#define CLI_SET_TMS_PATH_HELP      "Set TMS main path (n = none, b = boot, c = channel change, s = auto scan)"
#define CLI_GET_TMS_PATH_HELP      "Get TMS main path (n = none, b = boot, c = channel change, s = auto scan)"

/* TMS selection */
typedef enum
{
    TMS_SEL_INDIVIDUAL = 0,
    TMS_SEL_COMMON,
    TMS_SEL_INDIVIDUAL_COMMON
}   TMS_SEL_T;

typedef enum
{
    TMS_TYPE_SPOT,
    TMS_TYPE_DIFF,
    TMS_TYPE_BEGIN,
    TMS_TYPE_END
} TMS_TYPE_T;

typedef struct _TMS_BEGIN_END_TS_T
{
    UINT32              ui4_begin_ts;
    UINT32              ui4_end_ts;
    CHAR                s_begin_cmt[TMS_COMMENT_MAX_LENGTH+1];
    CHAR                s_end_cmt[TMS_COMMENT_MAX_LENGTH+1];
} TMS_BEGIN_END_TS_T;

typedef struct _TMS_SPOT_TS_T
{
    TMS_TYPE_T          e_type;
    UINT32              ui4_spot_ts;
    CHAR                s_spot_cmt[TMS_COMMENT_MAX_LENGTH+1];
} TMS_SPOT_TS_T;

typedef struct _TMS_REC_T
{
    struct _TMS_REC_T*  pt_next;
    TMS_TYPE_T          e_type;
    CHAR                s_name[TMS_NAME_MAX_LENGTH+1];
    UINT16              ui2_avg_cnt;
    UINT16              ui2_curr_cnt;
    UINT16              ui2_curr_spot_cnt;
    TMS_BEGIN_END_TS_T  at_ts[TMS_REC_CNT_MAX];
    TMS_SPOT_TS_T       at_spot_ts[TMS_SPOT_REC_CNT_MAX];
} TMS_REC_T;

#define CHECK_VALID_TMS_MEM_ADDR( _addr )                                     \
{                                                                             \
    if ( (UINT32)_addr < (UINT32)pv_mem_start ||                              \
         (UINT32)_addr > (UINT32)pv_mem_end )                                 \
    {                                                                         \
        DBG_ABORT( 0 );                                                       \
    }                                                                         \
}

#define CHECK_VALID_TMS_REC_MEM( _tms_rec_addr, _addr )                       \
{                                                                             \
    CHECK_VALID_TMS_MEM_ADDR( _tms_rec_addr )                                 \
    if ( (UINT32)_addr < (UINT32)_tms_rec_addr ||                             \
         (UINT32)_addr > (((UINT32)_tms_rec_addr)+z_per_tms_rec_size) )       \
    {                                                                         \
        DBG_ABORT( 0 );                                                       \
    }                                                                         \
}

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static TMS_REC_T*  pt_tms_rec_free_lst = NULL;
static INT32       i4_tms_max_num_records = 0;
static INT32       i4_tms_num_free_records = 0;

/* memory checking variables */
static VOID*       pv_mem_start = NULL;
static VOID*       pv_mem_end = NULL;
static SIZE_T      z_per_tms_rec_size = 0;

static HANDLE_T    h_tms_free_list_lock = NULL_HANDLE;

static HANDLE_T    h_time_msrt_db = NULL_HANDLE;

/* Common or central time measurement level. */
static UINT16      ui2_common_tms_level = TMS_INIT_LEVEL;

static TMS_SEL_T   e_tms_sel = TMS_SEL_INDIVIDUAL_COMMON;

static UINT32      ui4_flag = TMS_FLAG_BOOT;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
extern UINT16 time_msrt_get_level( UINT16 ui2_tms_mdl_level );
extern UINT32 time_msrt_get_flag( VOID );

/* The following is CLI related stuff. */
#if ((defined CLI_LVL_ALL)||(defined CLI_LVL_GUEST))
static INT32 cli_get_common_tms_level (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_set_common_tms_level (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_get_tms_path (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_set_tms_path (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_get_tms_sel (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_set_tms_sel (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_dump_tms_rec (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_dump_detail_tms_rec (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static INT32 cli_clean_tms_rec (
    INT32         i4_argc,
    const CHAR**  pps_argv);

static CLI_EXEC_T at_cli_tms_cmd [] =
{
    {CLI_GET_TMS_LEVEL, NULL, cli_get_common_tms_level, NULL, CLI_GET_TMS_LEVEL_HELP, CLI_SUPERVISOR},
    {CLI_SET_TMS_LEVEL, NULL, cli_set_common_tms_level, NULL, CLI_SET_TMS_LEVEL_HELP, CLI_SUPERVISOR},
    {CLI_GET_TMS_PATH, NULL, cli_get_tms_path, NULL, CLI_GET_TMS_PATH_HELP, CLI_SUPERVISOR},
    {CLI_SET_TMS_PATH, NULL, cli_set_tms_path, NULL, CLI_SET_TMS_PATH_HELP, CLI_SUPERVISOR},
    {CLI_GET_TMS_SEL,   NULL, cli_get_tms_sel,   NULL, CLI_GET_TMS_SEL_HELP,   CLI_SUPERVISOR},
    {CLI_SET_TMS_SEL,   NULL, cli_set_tms_sel,   NULL, CLI_SET_TMS_SEL_HELP,   CLI_SUPERVISOR},
    {CLI_DUMP_TMS_REC,  NULL, cli_dump_tms_rec,  NULL, "d [name], Dump brief TMS records", CLI_SUPERVISOR},
    {CLI_DUMP_TMS_DETAIL_REC,  NULL, cli_dump_detail_tms_rec,  NULL, "dd [name], Dump detail TMS records", CLI_SUPERVISOR},
    {CLI_CLEAN_TMS_REC, NULL, cli_clean_tms_rec,  NULL, "cl [name], Clean up TMS records", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_tms [] =
{
    {CLI_TMS_MODULE, NULL, NULL, &(at_cli_tms_cmd [0]), "Time measurement commands", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};
#endif /* CLI_LVL_ALL */

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

#if ((defined CLI_LVL_ALL)||(defined CLI_LVL_GUEST))
/*-----------------------------------------------------------------------------
 * Name: cli_dump_tms_rec
 *
 * Description: This function is called by the CLI and dumps the TMS records.
 *
 * Inputs:  i4_argc  References.
 *          pps_argv References.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_dump_tms_rec (
    INT32         i4_argc,
    const CHAR**  pps_argv)
{
    if ( (i4_argc >= 1) &&
         (pps_argv != NULL) &&
         ((pps_argv[1]) != NULL))
    {
        x_time_msrt_dump( pps_argv[1] );
    }
    else if ( i4_argc == 0 )
    {
        x_time_msrt_dump( NULL );
    }
    else
    {
        x_dbg_stmt( "Invalid argument !!!\r\n" );
    }

    return (CLIR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: cli_dump_detail_tms_rec
 *
 * Description: This function is called by the CLI and handles the TMS level
 *              get command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_dump_detail_tms_rec (
    INT32         i4_argc,
    const CHAR**  pps_argv)
{
    if ( (i4_argc >= 1) &&
         (pps_argv != NULL) &&
         ((pps_argv[1]) != NULL))
    {
        x_time_msrt_dump_detail( pps_argv[1] );
    }
    else if ( i4_argc == 0 )
    {
        x_time_msrt_dump_detail( NULL );
    }
    else
    {
        x_dbg_stmt( "Invalid argument !!!\r\n" );
    }

    return (CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: cli_clean_tms_rec
 *
 * Description: This function is called by the CLI and handles the TMS level
 *              get command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_clean_tms_rec (
    INT32         i4_argc,
    const CHAR**  pps_argv)
{
    if ( (i4_argc >= 1) &&
         (pps_argv != NULL) &&
         ((pps_argv[1]) != NULL))
    {
        x_time_msrt_clean( pps_argv[1] );
    }
    else if ( i4_argc == 0 )
    {
        x_time_msrt_clean( NULL );
    }
    else
    {
        x_dbg_stmt( "Invalid argument !!!\r\n" );
    }

    return (CLIR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: cli_get_tms_level
 *
 * Description: This function is called by the CLI and handles the TMS level
 *              get command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_get_common_tms_level (
    INT32         i4_argc,
    const CHAR**  pps_argv)
{
    x_cli_show_tms_level (ui2_common_tms_level);

    return (CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: cli_set_tms_level
 *
 * Description: This function is called by the CLI and handles the TMS level
 *              set command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_set_common_tms_level (
    INT32         i4_argc,
    const CHAR**  pps_argv)
{
    UINT16 ui2_tms_level;


    if (x_cli_parse_tms_level (i4_argc, pps_argv, &ui2_tms_level) == CLIR_OK)
    {
        ui2_common_tms_level = ui2_tms_level;
    }

    x_cli_show_tms_level (ui2_common_tms_level);

    return (CLIR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: cli_get_tms_path
 *
 * Description: This function is called by the CLI and handles the TMS path
 *              get command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_get_tms_path (INT32         i4_argc,
                               const CHAR**  pps_argv)
{
    x_dbg_stmt ("TMS path: ");

    switch (ui4_flag)
    {
    case TMS_FLAG_NONE:
        x_dbg_stmt ("n\n");
        break;

    case TMS_FLAG_BOOT:
        x_dbg_stmt ("b\n");
        break;

    case TMS_FLAG_CHG_CHL:
        x_dbg_stmt ("c\n");
        break;

    case TMS_FLAG_AUTO_SCN:
        x_dbg_stmt ("s\n");
        break;

    case TMS_FLAG_INP_CHG:
        x_dbg_stmt ("i\n");
        break;        

    default:
        x_dbg_stmt ("unknown\n");
        break;
    }

    return (CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: cli_set_tms_sel
 *
 * Description: This function is called by the CLI and handles the TMS path
 *              set command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK       Successful
 *          CLIR_INV_ARG  One or more invalid arguments
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_set_tms_path (INT32         i4_argc,
                               const CHAR**  pps_argv)
{
    INT32       i4_return = CLIR_INV_ARG;
    const CHAR* ps_tms_sel;


    if ( (i4_argc >= 1) &&
         (pps_argv != NULL) &&
         ((ps_tms_sel = pps_argv[1]) != NULL))
    {
        i4_return = CLIR_OK;

        if (x_strlen (ps_tms_sel) == 1)
        {
            switch (*ps_tms_sel)
            {
                case 'N':
                case 'n':
                    ui4_flag = TMS_FLAG_NONE;
                    break;

                case 'S':
                case 's':
                    ui4_flag = TMS_FLAG_AUTO_SCN;
                    break;

                case 'C':
                case 'c':
                    ui4_flag = TMS_FLAG_CHG_CHL;
                    break;

                case 'B':
                case 'b':
                    ui4_flag = TMS_FLAG_BOOT;
                    break;

                case 'I':
                case 'i':
                    ui4_flag = TMS_FLAG_INP_CHG;
                    break;

                default:
                    x_dbg_stmt ("Invalid TMS path.\n");
                    break;
            }
        }
    }

    cli_get_tms_path( 0, NULL );

    return (i4_return);
}

/*-----------------------------------------------------------------------------
 * Name: cli_get_tms_sel
 *
 * Description: This function is called by the CLI and handles the TMS select
 *              get command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_get_tms_sel (INT32         i4_argc,
                              const CHAR**  pps_argv)
{
    x_dbg_stmt ("TMS select: ");

    switch (e_tms_sel)
    {
    case TMS_SEL_INDIVIDUAL:
        x_dbg_stmt ("i\n");
        break;

    case TMS_SEL_COMMON:
        x_dbg_stmt ("c\n");
        break;

    case TMS_SEL_INDIVIDUAL_COMMON:
        x_dbg_stmt ("b\n");
        break;

    default:
        x_dbg_stmt ("unknown\n");
        break;
    }

    return (CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: cli_set_tms_sel
 *
 * Description: This function is called by the CLI and handles the debug select
 *              set command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK       Successful
 *          CLIR_INV_ARG  One or more invalid arguments
 *
 ----------------------------------------------------------------------------*/
static INT32 cli_set_tms_sel (INT32         i4_argc,
                              const CHAR**  pps_argv)
{
    BOOL        b_tms_sel_set;
    INT32       i4_return = CLIR_INV_ARG;
    const CHAR* ps_tms_sel;


    if ( (i4_argc >= 1) &&
         (pps_argv != NULL) &&
         ((ps_tms_sel = pps_argv[1]) != NULL))
    {
        i4_return = CLIR_OK;

        b_tms_sel_set = FALSE;

        if (x_strlen (ps_tms_sel) == 1)
        {
            switch (*ps_tms_sel)
            {
                case 'I':
                case 'i':
                    b_tms_sel_set = TRUE;

                    e_tms_sel = TMS_SEL_INDIVIDUAL;
                    break;

                case 'C':
                case 'c':
                    b_tms_sel_set = TRUE;

                    e_tms_sel = TMS_SEL_COMMON;
                    break;

                case 'B':
                case 'b':
                    b_tms_sel_set = TRUE;

                    e_tms_sel = TMS_SEL_INDIVIDUAL_COMMON;
                    break;

                default:
                    break;
            }
        }

        /* Output error message if not successful. */
        if (! (b_tms_sel_set))
        {
            x_dbg_stmt ("Invalid TMS selection. %s\n", CLI_SET_TMS_SEL_HELP);
        }
    }

    cli_get_tms_sel( 0, NULL );

    return (i4_return);
}

#endif /* CLI_LVL_ALL */


/*-----------------------------------------------------------------------------
 * Name: _tms_rec_free
 *
 * Description: This function puts the pt_tms_rec into free list for future
 *              use.
 *
 * Inputs:  pt_tms_rec The TMS record to be free.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _tms_rec_free( TMS_REC_T* pt_tms_rec )
{
    x_memset( pt_tms_rec->at_ts,
              0,
              (sizeof( TMS_BEGIN_END_TS_T ) * TMS_REC_CNT_MAX) );

    x_memset( pt_tms_rec->at_spot_ts,
              0,
              (sizeof( TMS_SPOT_TS_T ) * TMS_SPOT_REC_CNT_MAX) );

    pt_tms_rec->s_name[0] = 0;
    pt_tms_rec->ui2_curr_cnt = 0;
    pt_tms_rec->ui2_curr_spot_cnt = 0;

    TMS_LOCK( h_tms_free_list_lock );

    pt_tms_rec->pt_next = pt_tms_rec_free_lst;
    pt_tms_rec_free_lst = pt_tms_rec;
    ++i4_tms_num_free_records;

    TMS_UNLOCK( h_tms_free_list_lock );
}

/*-----------------------------------------------------------------------------
 * Name: _tms_rec_alloc
 *
 * Description: This function allocates TMS record structure from free list.
 *              If free list run out of free blocks, it will call x_mem_alloc
 *              to allocate a new block of memory.
 *
 * Inputs:  e_type       References.
 *          ui4_begin_ts References.
 *          s_name       References.
 *          ui2_avg_cnt  References.
 *
 * Outputs: -
 *
 * Returns: NULL        Success.
 *          pt_tms_rec  Fail.
 *
 ----------------------------------------------------------------------------*/
static TMS_REC_T* _tms_rec_alloc(
    TMS_TYPE_T          e_type,
    UINT32              ui4_begin_ts,
    const CHAR*         s_name,
    UINT16              ui2_avg_cnt,
    const CHAR*         s_comment )
{
    TMS_REC_T*   pt_tms_rec = NULL;
    SIZE_T       z_size = 0;

    if ( !pt_tms_rec_free_lst )
    {
        return NULL;
    }
    else
    {
        TMS_LOCK( h_tms_free_list_lock );

        pt_tms_rec = pt_tms_rec_free_lst;
        pt_tms_rec_free_lst = pt_tms_rec_free_lst->pt_next;
        --i4_tms_num_free_records;

        CHECK_VALID_TMS_MEM_ADDR(pt_tms_rec);

        TMS_UNLOCK( h_tms_free_list_lock );

        pt_tms_rec->pt_next = NULL;
        pt_tms_rec->s_name[0] = 0;
    }

    pt_tms_rec->e_type = e_type;
    if (e_type == TMS_TYPE_BEGIN)
    {
        pt_tms_rec->ui2_avg_cnt = ui2_avg_cnt;
        pt_tms_rec->ui2_curr_cnt = 0;
        pt_tms_rec->at_ts[0].ui4_begin_ts = ui4_begin_ts;
        if (s_comment)
        {
            x_strncpy(pt_tms_rec->at_ts[0].s_begin_cmt,
                      s_comment,
                      TMS_COMMENT_MAX_LENGTH);
            x_strncpy(pt_tms_rec->at_spot_ts[0].s_spot_cmt,
                      s_comment,
                      TMS_COMMENT_MAX_LENGTH);
        }
        else
        {
            pt_tms_rec->at_ts[0].s_begin_cmt[0] = 0;
            pt_tms_rec->at_spot_ts[0].s_spot_cmt[0] = 0;
        }
        pt_tms_rec->at_ts[0].ui4_end_ts = 0;
    }

    pt_tms_rec->at_spot_ts[0].e_type = e_type;
    pt_tms_rec->at_spot_ts[0].ui4_spot_ts = ui4_begin_ts;
    if (s_comment)
    {
        x_strncpy(pt_tms_rec->at_spot_ts[0].s_spot_cmt,
                  s_comment,
                  TMS_COMMENT_MAX_LENGTH);
    }
    else
    {
        pt_tms_rec->at_spot_ts[0].s_spot_cmt[0] = 0;
    }
    pt_tms_rec->ui2_curr_spot_cnt = 1;

    z_size = x_strlen(s_name);
    if ( z_size > TMS_NAME_MAX_LENGTH )
    {
        x_memcpy( pt_tms_rec->s_name, s_name, TMS_NAME_MAX_LENGTH);
        pt_tms_rec->s_name[TMS_NAME_MAX_LENGTH] = 0;
    }
    else
    {
        x_strcpy( pt_tms_rec->s_name, s_name );
    }

    return pt_tms_rec;
}

/*-----------------------------------------------------------------------------
 * Name: _time_msrt_spot_print
 *
 * Description: This function print the spot point of time measurement.
 *
 * Inputs:  s_name         Name of the spot point.
 *          ui4_time_stamp Time stamp of the spot point.
 *          ui4_no         The serial-no of the spot point. If ui4_no is equal
 *                         to zero, no serial-no is printed out.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _time_msrt_spot_print(
    const CHAR*           s_name,
    UINT32                ui4_sys_tick,
    UINT32                ui4_no,
    const CHAR*           s_comment )
{
    UINT64  ui8_time_stamp = ui4_sys_tick * x_os_get_sys_tick_period();

    if ( !s_name )
    {
        return;
    }
    if ( ui4_no != 0 )
    {
        x_dbg_stmt("[TIME_MSRT] #%.2u, %-32s, %-16s, SPOT at  %8llu ms\r\n",
                   ui4_no, s_name, (s_comment) ? s_comment : "", ui8_time_stamp);
    }
    else
    {
        x_dbg_stmt("[TIME_MSRT] ###, %-32s, %-16s, SPOT at  %8llu ms\r\n",
                   s_name, (s_comment) ? s_comment : "", ui8_time_stamp);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _time_msrt_diff_print
 *
 * Description: This function print the spot point of time measurement.
 *
 * Inputs:  s_name         Name of the spot point.
 *          ui4_diff_tick  Time stamp difference from the start point.
 *          ui4_no         The serial-no of the spot point. If ui4_no is equal
 *                         to zero, no serial-no is printed out.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _time_msrt_diff_print(
    const CHAR*           s_name,
    UINT32                ui4_diff_tick,
    UINT32                ui4_no,
    const CHAR*           s_comment )
{
    UINT64  ui8_time_stamp = ui4_diff_tick * x_os_get_sys_tick_period();

    if ( !s_name )
    {
        return;
    }
    if ( ui4_no != 0 )
    {
        x_dbg_stmt("[TIME_MSRT] #%.2u, %-32s, %-16s, DIFF is         %8llu ms\r\n",
                   ui4_no, s_name, (s_comment) ? s_comment : "", ui8_time_stamp);
    }
    else
    {
        x_dbg_stmt("[TIME_MSRT] ###, %-32s, %-16s, DIFF spot at    %8llu ms\r\n",
                   s_name, (s_comment) ? s_comment : "", ui8_time_stamp);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _time_msrt_begin2end_print
 *
 * Description: This function print the begin-end point of time measurement.
 *
 * Inputs:  s_name         Name of the begin-end point of time measurement.
 *          ui4_delta_time The delta time between begin and end point.
 *          ui4_no         The serial-no of the begin-end point. If ui4_no is
 *                         equal to zero, no serial-no is printed out.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _time_msrt_begin2end_print(
    const CHAR*           s_name,
    UINT32                ui4_delta_time,
    UINT32                ui4_no,
    const CHAR*           s_comment)
{
    UINT64  ui8_time_stamp = ui4_delta_time * x_os_get_sys_tick_period();

    if ( !s_name )
    {
        return;
    }
    if ( ui4_no != 0 )
    {
        x_dbg_stmt("[TIME_MSRT] #%.2u, %-32s, %-16s BEGIN-END costs %8llu ms\r\n",
                   ui4_no, s_name, s_comment, ui8_time_stamp );
    }
    else
    {
        x_dbg_stmt("[TIME_MSRT] ###, %-32s, %-16s, %-16s BEGIN-END costs %8llu ms\r\n",
                   s_name, s_comment, ui8_time_stamp );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _time_msrt_begin2end_avg_print
 *
 * Description: This function print the average delta time of begin-end point
 *              of time measurement.
 *
 * Inputs:  s_name             Name of the begin-end point of time measurement.
 *          ui4_avg_delta_time The avarage delta time between begin and end
 *                             point.
 *          ui4_avg_cnt        The total count which of the avarage time is
 *                             calculated.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _time_msrt_begin2end_avg_print(
    const CHAR*           s_name,
    UINT32                ui4_avg_delta_time,
    UINT32                ui4_avg_cnt )
{
    UINT64  ui8_time_stamp = ui4_avg_delta_time * x_os_get_sys_tick_period();

    if ( !s_name )
    {
        return;
    }
    x_dbg_stmt("[TIME_MSRT] ###, %-32s, BEGIN-END AVG %5llu ms/ %3u times\r\n",
               s_name, ui8_time_stamp, ui4_avg_cnt );
}


/*-----------------------------------------------------------------------------
 * Name: _time_msrt_begin_print
 *
 * Description: This function print the begin time of begin-end point
 *              of time measurement.
 *
 * Inputs:  s_name       Name of the begin-end point of time measurement.
 *          ui4_begin_ts The time stamp of the begin point.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _time_msrt_begin_print(
    const CHAR*           s_name,
    UINT32                ui4_begin_ts,
    const CHAR*           s_comment )
{
    UINT64  ui8_time_stamp = ui4_begin_ts * x_os_get_sys_tick_period();

    if ( !s_name )
    {
        return;
    }
    x_dbg_stmt("[TIME_MSRT] ###, %-32s, %-16s, BEGIN at time   %8llu ms\r\n",
               s_name, (s_comment) ? s_comment : "", ui8_time_stamp );
}
#if 0
/*-----------------------------------------------------------------------------
 * Name: _time_msrt_end_print
 *
 * Description: This function print the end time of begin-end point
 *              of time measurement.
 *
 * Inputs:  s_name       Name of the begin-end point of time measurement.
 *          ui4_begin_ts The time stamp of the begin point.
 *          ui4_end_ts   The time stamp of the end point.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _time_msrt_end_print(
    const CHAR*           s_name,
    UINT32                ui4_begin_ts,
    UINT32                ui4_end_ts,
    const CHAR*           s_comment )
{
    UINT64  ui8_time_stamp = ui4_end_ts * x_os_get_sys_tick_period();

    if ( !s_name )
    {
        return;
    }

    x_dbg_stmt("[TIME_MSRT] ###, %-32s, %-16s, END at time     %8llu ms\r\n",
               s_name, (s_comment) ? s_comment : "", ui8_time_stamp );

    _time_msrt_begin2end_print( s_name, (ui4_end_ts-ui4_begin_ts), 0 ,s_comment);
}
#endif
/*-----------------------------------------------------------------------------
 * Name: _time_msrt_off_line
 *
 * Description: _time_msrt_off_line
 *
 * Inputs:  e_type       The type of the TMS record.
 *          s_name       The name of the TMS record.
 *          ui4_avg_cnt  User specified MAX average count of this TMS record.
 *          ui4_sys_time Current system time stamp.
 *
 * Outputs: -
 *
 * Returns: TMSR_OK                   Success.
 *          TMSR_FAIL                 Fail.
 *          TMSR_INSUFFICIENT_MEMORY  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 _time_msrt_off_line(
    TMS_TYPE_T            e_type,
    const CHAR*           s_name,
    UINT16                ui4_avg_cnt,
    UINT32                ui4_sys_time,
    const CHAR*           s_comment )
{
    INT32      i4_ret;
    TMS_REC_T* pt_tms_rec = NULL;
    SIZE_T     z_key_size = x_strlen(s_name) + 1;

    if ( z_key_size > (TMS_NAME_MAX_LENGTH+1) )
    {
        z_key_size = TMS_NAME_MAX_LENGTH + 1;
    }

    hash_lock(h_time_msrt_db);

    if ( hash_is_key_exist( h_time_msrt_db,
                            (const INT8*)s_name,
                            z_key_size ) )
    {
        i4_ret = hash_get_obj(
            h_time_msrt_db,
            (const INT8*)s_name,
            (SIZE_T)z_key_size,
            (VOID**)&pt_tms_rec );
        if ( i4_ret != HASH_OK || !pt_tms_rec )
        {
            hash_unlock(h_time_msrt_db);
            return TMSR_INSUFFICIENT_MEMORY;
        }

        if (e_type == TMS_TYPE_BEGIN)
        {
            UINT16     ui2_idx = 0;
            
            pt_tms_rec->ui2_curr_spot_cnt = 0;
            pt_tms_rec->ui2_curr_cnt = pt_tms_rec->ui2_curr_cnt + (UINT16)1;
			
			ui2_idx = pt_tms_rec->ui2_curr_cnt % pt_tms_rec->ui2_avg_cnt;
			if ( ui2_idx >= TMS_REC_CNT_MAX )
			{
                hash_unlock(h_time_msrt_db);
                return TMSR_FAIL;
            }
			
            CHECK_VALID_TMS_REC_MEM(
                pt_tms_rec,
                &pt_tms_rec->at_ts[ui2_idx] );

            pt_tms_rec->at_ts[ui2_idx].ui4_begin_ts =
                ui4_sys_time;
            if (s_comment)
            {
                x_strncpy(pt_tms_rec->at_ts[ui2_idx].s_begin_cmt,
                          s_comment,
                          TMS_COMMENT_MAX_LENGTH);
            }
            else
            {
                pt_tms_rec->at_ts[ui2_idx].s_begin_cmt[0] = 0;
            }

            pt_tms_rec->at_ts[ui2_idx].ui4_end_ts = 0;
            pt_tms_rec->e_type = e_type;

            pt_tms_rec->at_spot_ts[0].e_type = e_type;
            pt_tms_rec->at_spot_ts[0].ui4_spot_ts = ui4_sys_time;
            if (s_comment)
            {
                x_strncpy(pt_tms_rec->at_spot_ts[0].s_spot_cmt,
                          s_comment,
                          TMS_COMMENT_MAX_LENGTH);
            }
            else
            {
                pt_tms_rec->at_spot_ts[0].s_spot_cmt[0] = 0;
            }

            pt_tms_rec->at_spot_ts[0].e_type = e_type;
            pt_tms_rec->at_spot_ts[0].ui4_spot_ts = ui4_sys_time;
            if (s_comment)
            {
                x_strncpy(pt_tms_rec->at_spot_ts[0].s_spot_cmt,
                          s_comment,
                          TMS_COMMENT_MAX_LENGTH);
            }
            else
            {
                pt_tms_rec->at_spot_ts[0].s_spot_cmt[0] = 0;
            }

            pt_tms_rec->ui2_curr_spot_cnt++;
        }
        else if ((e_type == TMS_TYPE_SPOT) ||
                 (e_type == TMS_TYPE_DIFF))
        {
            if (pt_tms_rec->ui2_curr_spot_cnt >= TMS_SPOT_REC_CNT_MAX)
            {
                hash_unlock(h_time_msrt_db);
                return TMSR_FAIL;
            }

            pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt].e_type = e_type;
            pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt].ui4_spot_ts = ui4_sys_time;
            if (s_comment)
            {
                x_strncpy(pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt].s_spot_cmt,
                          s_comment,
                          TMS_COMMENT_MAX_LENGTH);
            }
            else
            {
                pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt].s_spot_cmt[0] = 0;
            }

            pt_tms_rec->ui2_curr_spot_cnt++;
        }
        else
        {
            UINT16     ui2_idx = 0;

            if ( pt_tms_rec->e_type != TMS_TYPE_BEGIN )
            {
                hash_unlock(h_time_msrt_db);
                return TMSR_FAIL;
            }
            
            ui2_idx = pt_tms_rec->ui2_curr_cnt%pt_tms_rec->ui2_avg_cnt;
            if ( ui2_idx >= TMS_REC_CNT_MAX )
			{
                hash_unlock(h_time_msrt_db);
                return TMSR_FAIL;
            }
            
            CHECK_VALID_TMS_REC_MEM(
                pt_tms_rec,
                &pt_tms_rec->at_ts[ui2_idx] );

            pt_tms_rec->at_ts[ui2_idx].ui4_end_ts =
                ui4_sys_time;
            if (s_comment)
            {
                x_strncpy(pt_tms_rec->at_ts[ui2_idx].s_end_cmt,
                          s_comment,
                          TMS_COMMENT_MAX_LENGTH);
            }
            else
            {
                pt_tms_rec->at_ts[ui2_idx].s_end_cmt[0] = 0;
            }
        }
    }
    else
    {
        if ( e_type == TMS_TYPE_END )
        {
            /* ignore */
            hash_unlock(h_time_msrt_db);
            return TMSR_FAIL;
        }

        pt_tms_rec = _tms_rec_alloc(
            e_type,
            ui4_sys_time,
            s_name,
            ui4_avg_cnt,
            s_comment );
        if ( !pt_tms_rec )
        {
            hash_unlock(h_time_msrt_db);
            return TMSR_INSUFFICIENT_MEMORY;
        }

        i4_ret = hash_add_obj(
            h_time_msrt_db,
            (const INT8*)s_name,
            (SIZE_T)z_key_size,
            (VOID*)pt_tms_rec,
            (VOID**)NULL );
        if ( i4_ret != HASH_OK || !pt_tms_rec )
        {
            _tms_rec_free( pt_tms_rec );
            hash_unlock(h_time_msrt_db);
            return TMSR_INSUFFICIENT_MEMORY;
        }
    }

    hash_unlock(h_time_msrt_db);
    return TMSR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _time_msrt_dump_rec
 *
 * Description: This fucntion dump the brief of a specified TMS record.
 *
 * Inputs:  pt_tms_rec Then TMS record to be print out.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _time_msrt_dump_rec( TMS_REC_T* pt_tms_rec )
{
    UINT16     ui2_i = 0;

    if ( !pt_tms_rec )
    {
        return;
    }

    if ( pt_tms_rec->e_type == TMS_TYPE_BEGIN )
    {
        UINT32    ui4_avg_time = 0;
        UINT32    ui4_total = 0;

        for ( ui2_i = 0; ui2_i < pt_tms_rec->ui2_avg_cnt; ++ui2_i )
        {
            UINT16 ui2_cnt = 0;
            INT32  i4_delta = 0;

            if ( pt_tms_rec->ui2_curr_cnt > pt_tms_rec->ui2_avg_cnt )
            {
                ui2_cnt = (UINT16)( pt_tms_rec->ui2_curr_cnt - 
                            (pt_tms_rec->ui2_avg_cnt - 1) + ui2_i);
            }
            else
            {
                ui2_cnt = ui2_i;
            }

            if ( ui2_cnt <= pt_tms_rec->ui2_curr_cnt )
            {
                ui2_cnt %= pt_tms_rec->ui2_avg_cnt;
                if ( ui2_cnt >= TMS_REC_CNT_MAX )
                {
                    continue;
                }
                i4_delta = (INT32)( pt_tms_rec->at_ts[ui2_cnt].ui4_end_ts -
                             pt_tms_rec->at_ts[ui2_cnt].ui4_begin_ts );
                if ( i4_delta < 0 )
                {
                    continue;
                }

                ++ui4_total;
                ui4_avg_time += (UINT32)i4_delta;
                continue;
            }
            break;
        }

        if ( ui4_total != 0 )
        {
            ui4_avg_time /= ui4_total;

            _time_msrt_begin2end_avg_print(
                pt_tms_rec->s_name,
                ui4_avg_time,
                ui4_total );
        }
    }
    else if (pt_tms_rec->ui2_curr_spot_cnt &&
             (pt_tms_rec->ui2_curr_spot_cnt < TMS_SPOT_REC_CNT_MAX) )
    {
        if ((pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt-1].e_type == TMS_TYPE_SPOT) ||
            ((pt_tms_rec->ui2_curr_spot_cnt == 1) &&
             (pt_tms_rec->at_spot_ts[0].e_type == TMS_TYPE_DIFF)))
        {
            _time_msrt_spot_print(
                pt_tms_rec->s_name,
                pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt-1].ui4_spot_ts,
                pt_tms_rec->ui2_curr_spot_cnt,
                pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt-1].s_spot_cmt );
        }
        else if (pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt-1].e_type == TMS_TYPE_DIFF)
        {
            UINT32     ui4_delta_time;

            if ( pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt-1].ui4_spot_ts >
                 pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt].ui4_spot_ts )
            {
                if ( pt_tms_rec->ui2_curr_spot_cnt > (UINT16)1 )
                {
                    ui4_delta_time = ( 0xFFFFFFFF -
                                       pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt-2].ui4_spot_ts ) +
                                     pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt-1].ui4_spot_ts;
                }
                else
                {
                    ui4_delta_time = 0;
                }
            }
            else
            {
                if ( pt_tms_rec->ui2_curr_spot_cnt > (UINT16)1 )
                {
                    ui4_delta_time = pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt-1].ui4_spot_ts -
                                     pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt-2].ui4_spot_ts;
                }
                else
                {
                    ui4_delta_time = 0;
                }
            }

            _time_msrt_diff_print(
                pt_tms_rec->s_name,
                ui4_delta_time,
                pt_tms_rec->ui2_curr_spot_cnt,
                pt_tms_rec->at_spot_ts[pt_tms_rec->ui2_curr_spot_cnt-1].s_spot_cmt );
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _time_msrt_dump_detail_rec
 *
 * Description: This fucntion dump the details of a specified TMS record.
 *
 * Inputs:  pt_tms_rec Then TMS record to be print out.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _time_msrt_dump_detail_rec( TMS_REC_T* pt_tms_rec )
{
    UINT16     ui2_i = 0;

    if ( pt_tms_rec->e_type == TMS_TYPE_BEGIN )
    {
        for ( ui2_i = 0; ui2_i < pt_tms_rec->ui2_avg_cnt; ++ui2_i )
        {
            UINT16 ui2_cnt = 0;
            INT32  i4_delta = 0;

            if ( pt_tms_rec->ui2_curr_cnt > pt_tms_rec->ui2_avg_cnt )
            {
                ui2_cnt = (UINT16)( pt_tms_rec->ui2_curr_cnt -
                            (pt_tms_rec->ui2_avg_cnt - 1) + ui2_i);
            }
            else
            {
                ui2_cnt = ui2_i;
            }

            if ( ui2_cnt <= pt_tms_rec->ui2_curr_cnt )
            {
                ui2_cnt %= pt_tms_rec->ui2_avg_cnt;
                if ( ui2_cnt >= TMS_REC_CNT_MAX )
                {
                    continue;
                }
                i4_delta = (INT32)( pt_tms_rec->at_ts[ui2_cnt].ui4_end_ts -
                             pt_tms_rec->at_ts[ui2_cnt].ui4_begin_ts );
                if ( i4_delta < 0 )
                {
                    continue;
                }
                _time_msrt_begin2end_print(
                    pt_tms_rec->s_name,
                    (UINT32)i4_delta,
                    (UINT32)(ui2_i + 1),
                    pt_tms_rec->at_ts[ui2_cnt].s_end_cmt);

                continue;
            }
            break;
        }
    }

    if (pt_tms_rec->ui2_curr_spot_cnt &&
        (pt_tms_rec->ui2_curr_spot_cnt < TMS_SPOT_REC_CNT_MAX))
    {
        for (ui2_i = 0; ui2_i < pt_tms_rec->ui2_curr_spot_cnt; ui2_i++)
        {
            if ((pt_tms_rec->ui2_curr_spot_cnt == 1) &&
                (pt_tms_rec->at_spot_ts[0].e_type == TMS_TYPE_BEGIN))
            {
                break;
            }

            if (pt_tms_rec->at_spot_ts[ui2_i].e_type == TMS_TYPE_BEGIN)
            {
                x_dbg_stmt("\n\n");
                _time_msrt_begin_print(
                    pt_tms_rec->s_name,
                    pt_tms_rec->at_spot_ts[ui2_i].ui4_spot_ts,
                    pt_tms_rec->at_spot_ts[ui2_i].s_spot_cmt );
            }
            else if ((pt_tms_rec->at_spot_ts[ui2_i].e_type == TMS_TYPE_SPOT) ||
                     ((ui2_i == 0) && (pt_tms_rec->at_spot_ts[ui2_i].e_type == TMS_TYPE_DIFF)))
            {
                _time_msrt_spot_print(
                    pt_tms_rec->s_name,
                    pt_tms_rec->at_spot_ts[ui2_i].ui4_spot_ts,
                    (UINT32)ui2_i,
                    pt_tms_rec->at_spot_ts[ui2_i].s_spot_cmt );
            }
            else if (pt_tms_rec->at_spot_ts[ui2_i].e_type == TMS_TYPE_DIFF &&
                     ui2_i > 0 )
            {
                UINT32     ui4_delta_time;

                if ( pt_tms_rec->at_spot_ts[ui2_i-1].ui4_spot_ts >
                     pt_tms_rec->at_spot_ts[ui2_i].ui4_spot_ts )
                {
                    ui4_delta_time = ( 0xFFFFFFFF - pt_tms_rec->at_spot_ts[ui2_i-1].ui4_spot_ts ) +
                                     pt_tms_rec->at_spot_ts[ui2_i].ui4_spot_ts;
                }
                else
                {
                    ui4_delta_time = pt_tms_rec->at_spot_ts[ui2_i].ui4_spot_ts -
                                     pt_tms_rec->at_spot_ts[ui2_i-1].ui4_spot_ts;
                }

                _time_msrt_diff_print(
                    pt_tms_rec->s_name,
                    ui4_delta_time,
                    (UINT32)ui2_i,
                    pt_tms_rec->at_spot_ts[ui2_i].s_spot_cmt );
            }
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _time_msrt_dump_fct
 *
 * Description: This function is the callback funtion of hash_parse(). It calls
 *              _time_msrt_dump_rec() to dump the brief of each TMS record.
 *
 * Inputs:  h_hash       The handle of the hash.
 *          ui4_counter  Indicate this is the n-th count of records.
 *          pv_obj       References to the TMS record.
 *          pv_parse_tag Not used here.
 *
 * Outputs: -
 *
 * Returns: HASH_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _time_msrt_dump_fct(
    HANDLE_T          h_hash,
    UINT32            ui4_counter,
    VOID*             pv_obj,
    VOID*             pv_parse_tag )
{
    TMS_REC_T* pt_tms_rec = (TMS_REC_T*)pv_obj;

    if ( !pv_obj )
    {
        return HASH_OK;
    }

    _time_msrt_dump_rec( pt_tms_rec );

    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _time_msrt_dump_detail_fct
 *
 * Description: This function is the callback funtion of hash_parse(). It calls
 *              _time_msrt_dump_detail_rec() to dump the detail of each TMS
 *              record.
 *
 * Inputs:  h_hash       The handle of the hash.
 *          ui4_counter  Indicate this is the n-th count of records.
 *          pv_obj       References to the TMS record.
 *          pv_parse_tag Not used here.
 *
 * Outputs: -
 *
 * Returns: HASH_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _time_msrt_dump_detail_fct(
    HANDLE_T          h_hash,
    UINT32            ui4_counter,
    VOID*             pv_obj,
    VOID*             pv_parse_tag )
{
    TMS_REC_T* pt_tms_rec = (TMS_REC_T*)pv_obj;

    if ( !pv_obj )
    {
        return HASH_OK;
    }

    _time_msrt_dump_detail_rec( pt_tms_rec );

    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _time_msrt_dump_choosed_fct
 *
 * Description: This function is the callback funtion of hash_parse(). It calls
 *              _time_msrt_dump_rec() to dump the brief of each TMS
 *              record which matches the name specified by user.
 *
 * Inputs:  h_hash       The handle of the hash.
 *          ui4_counter  Indicate this is the n-th count of records.
 *          pv_obj       References to the TMS record.
 *          pv_parse_tag References to the name specified by user.
 *
 * Outputs: -
 *
 * Returns: HASH_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _time_msrt_dump_choosed_fct(
    HANDLE_T          h_hash,
    UINT32            ui4_counter,
    VOID*             pv_obj,
    VOID*             pv_parse_tag )
{
    TMS_REC_T* pt_tms_rec = (TMS_REC_T*)pv_obj;
    CHAR*      s_name = (CHAR*)pv_parse_tag;
    SIZE_T     z_name_size;

    if ( !pv_obj || !pv_parse_tag )
    {
        return HASH_OK;
    }
    z_name_size = x_strlen(s_name);
    z_name_size = TMS_MIN( z_name_size, TMS_NAME_MAX_LENGTH );

    if ( z_name_size <= x_strlen(pt_tms_rec->s_name) )
    {
        if ( x_memcmp( s_name, pt_tms_rec->s_name, z_name_size ) == 0 )
        {
            _time_msrt_dump_rec( pt_tms_rec );
        }
    }

    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _time_msrt_dump_choosed_detail_fct
 *
 * Description: This function is the callback funtion of hash_parse(). It calls
 *              _time_msrt_dump_detail_rec() to dump the detail of each TMS
 *              record which matches the name specified by user.
 *
 * Inputs:  h_hash       The handle of the hash.
 *          ui4_counter  Indicate this is the n-th count of records.
 *          pv_obj       References to the TMS record.
 *          pv_parse_tag References to the name specified by user.
 *
 * Outputs: -
 *
 * Returns: HASH_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _time_msrt_dump_choosed_detail_fct(
    HANDLE_T          h_hash,
    UINT32            ui4_counter,
    VOID*             pv_obj,
    VOID*             pv_parse_tag )
{
    TMS_REC_T* pt_tms_rec = (TMS_REC_T*)pv_obj;
    CHAR*      s_name = (CHAR*)pv_parse_tag;
    SIZE_T     z_name_size;

    if ( !pv_obj || !pv_parse_tag )
    {
        return HASH_OK;
    }
    z_name_size = x_strlen(s_name);
    z_name_size = TMS_MIN( z_name_size, TMS_NAME_MAX_LENGTH );

    if ( z_name_size <= x_strlen(pt_tms_rec->s_name) )
    {
        if ( x_memcmp( s_name, pt_tms_rec->s_name, z_name_size ) == 0 )
        {
            _time_msrt_dump_detail_rec( pt_tms_rec );
        }
    }

    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _time_msrt_clean_cond_fct
 *
 * Description: This is the clean condition callback function of
 *              hash_del_obj_on_cond().
 *
 * Inputs:  pi1_key    References to the key of the object.
 *          z_key_size Contains the size of the key.
 *          pv_obj     References to the object itself.
 *          pv_tag     References to the user specified name of object to be
 *                     delete.
 *
 * Outputs: -
 *
 * Returns: TRUE   Success.
 *          FALSE  Fail.
 *
 ----------------------------------------------------------------------------*/
static BOOL _time_msrt_clean_cond_fct(
    const INT8*       pi1_key,
    SIZE_T            z_key_size,
    VOID*             pv_obj,
    VOID*             pv_tag )
{
    CHAR*      s_name = (CHAR*)pv_tag;
    SIZE_T     z_name_size;

    if ( !s_name )
    {
        return TRUE;
    }

    z_name_size = x_strlen(s_name);
    z_name_size = TMS_MIN( z_name_size, TMS_NAME_MAX_LENGTH );

    if ( z_name_size < z_key_size )
    {
        if ( x_memcmp( s_name, pi1_key, z_name_size ) == 0 )
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _time_msrt_get_key
 *
 * Description: This function uses to get the key string.
 *
 * Inputs:  s_name
 *
 * Outputs: -
 *
 * Returns: String of Key
 *
 ----------------------------------------------------------------------------*/
static CHAR* _time_msrt_get_key(
    const CHAR* s_name )
{
    if (s_name)
    {
        return (CHAR*) s_name;
    }

    switch (ui4_flag)
    {
        case TMS_FLAG_BOOT:
            return TMS_COOL_BOOT_TIME;

        case TMS_FLAG_CHG_CHL:
            return TMS_CHL_CHE_TIME;

        case TMS_FLAG_AUTO_SCN:
            return TMS_NAME_AUTO_SCAN;

        default:
            return NULL;
    }
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: time_msrt_get_level
 *
 * Description: This funuction combines the module and common TMS level and
 *              returns the final level according to TMS selection.
 *
 * Inputs:  ui2_tms_mdl_level Contains the module level given by caller.
 *
 * Outputs: -
 *
 * Returns: ui2_tms_level  The final TMS level.
 *
 ----------------------------------------------------------------------------*/
UINT16 time_msrt_get_level( UINT16 ui2_tms_mdl_level )
{
    UINT16 ui2_tms_level;

    if ( e_tms_sel == TMS_SEL_INDIVIDUAL )
    {
        ui2_tms_level = ui2_tms_mdl_level;
    }
    else if ( e_tms_sel == TMS_SEL_COMMON )
    {
        ui2_tms_level = ui2_common_tms_level;
    }
    else /* TMS_SEL_INDIVIDUAL_COMMON */
    {
        ui2_tms_level = (ui2_common_tms_level|ui2_tms_mdl_level);
    }

    return ui2_tms_level;
}

/*-----------------------------------------------------------------------------
 * Name: time_msrt_get_flag
 *
 * Description: This funuction retrieves the TMS path flag
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: ui4_flag  The TMS path.
 *
 ----------------------------------------------------------------------------*/
UINT32 time_msrt_get_flag( VOID )
{
    return ui4_flag;
}

/*-----------------------------------------------------------------------------
 * Name: time_msrt_init
 *
 * Description: Time measurement initialization function.
 *
 * Inputs:  ui4_max_num_records  Max TMS records.
 *
 * Outputs: -
 *
 * Returns: TMSR_FAIL  Success.
 *          TMSR_OK    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 time_msrt_init( SIZE_T z_max_num_records )
{
    INT32          i4_ret, i4_i;
    CRIT_STATE_T   t_crit_state;
    TMS_REC_T*     pt_tms_recs = NULL;
    TMS_REC_T*     pt_tms_rec_i = NULL;
    SIZE_T         z_tms_rec_size = 0;
    SIZE_T         z_alloc_size = 0;

    t_crit_state = x_crit_start();

    if ( h_time_msrt_db == NULL_HANDLE )
    {
        i4_ret = hash_create( 256, &h_time_msrt_db );
        if ( i4_ret != HASH_OK )
        {
            x_crit_end( t_crit_state );
            DBG_ABORT (0);
        }
    }

    if ( h_tms_free_list_lock == NULL_HANDLE )
    {
        i4_ret = x_sema_create( &h_tms_free_list_lock,
                                X_SEMA_TYPE_MUTEX,
                                X_SEMA_STATE_UNLOCK );
        if ( i4_ret != OSR_OK )
        {
            x_crit_end( t_crit_state );
            DBG_ABORT(0);
        }
    }

    if ( z_max_num_records == 0 )
    {
        z_max_num_records = TMS_INIT_MAX_NUM_RECORDS;
    }

    /* Each TMS record size should be, size of struct TMS_REC_T + size of
       TMS_BEGIN_END_TS_T * (TMS_REC_CNT_MAX-1) */
    z_tms_rec_size = sizeof(TMS_REC_T);

    /* Set total allocated size */
    z_alloc_size = z_tms_rec_size * z_max_num_records;

    pt_tms_recs = (TMS_REC_T*)x_mem_alloc( z_alloc_size );
    if ( !pt_tms_recs )
    {
        x_crit_end( t_crit_state );
        DBG_ABORT(0);
    }

    /* Initialize these values for memory chacking */
    pv_mem_start = (VOID*) pt_tms_recs;
    pv_mem_end = (VOID*)((UINT32)pv_mem_start + z_alloc_size );
    z_per_tms_rec_size = z_tms_rec_size;

    i4_tms_max_num_records = i4_tms_num_free_records = (INT32)z_max_num_records;
    x_memset( pt_tms_recs, 0, z_alloc_size );

    pt_tms_rec_i = pt_tms_recs;
    for( i4_i = 0; i4_i < (i4_tms_max_num_records-1); ++i4_i )
    {
        TMS_REC_T* pt_next_tms_recs =
            (TMS_REC_T*)(((CHAR*)pt_tms_rec_i) + z_tms_rec_size);

        pt_tms_rec_i->pt_next = pt_next_tms_recs;
        pt_tms_rec_i = pt_next_tms_recs;
    }
    pt_tms_rec_i->pt_next = NULL;
    pt_tms_rec_free_lst = pt_tms_recs;

#if ((defined CLI_LVL_ALL)||(defined CLI_LVL_GUEST))
    /* And register the TMS library with the CLI module. */
    if (x_cli_attach_cmd_tbl (&(at_cli_tms [0]), CLI_CAT_MW, CLI_GRP_NONE) != CLIR_OK)
    {
        x_crit_end( t_crit_state );
        DBG_ABORT (0);
    }
#endif

    x_crit_end( t_crit_state );

#ifdef TIME_MEASUREMENT
    TMS_BEGIN (TMS_COOL_BOOT_TIME);
    TMS_BEGIN_EX (TMS_FLAG_BOOT, TMS_COOL_BOOT_TIME, "time_msrt_init");
#endif

    return TMSR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_spot
 *
 * Description: This function create the SPOT TMS record.
 *
 * Inputs:  s_name      Name of the SPOT TMS record.
 *          ui2_tms_lvl The TMS level of the record.
 *
 * Outputs: -
 *
 * Returns: TMSR_FAIL                 Success.
 *          TMSR_FAIL                 Fail.
 *          TMSR_INSUFFICIENT_MEMORY  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_time_msrt_spot(
    const CHAR*           s_name,
    UINT16                ui2_tms_lvl,
    const CHAR*           s_comment )
{
    INT32   i4_ret = TMSR_OK;
    UINT32  ui4_sys_time = x_os_get_sys_tick();
    CHAR*   s_key;

    if ( !TMS_IS_INITIALIZED )
    {
        return TMSR_FAIL;
    }

    s_key = _time_msrt_get_key(s_name);
    if (!s_key)
    {
        return TMSR_FAIL;
    }

    if ( ui2_tms_lvl & TMS_LEVEL_REAL_TIME )
    {
        _time_msrt_spot_print( s_key, ui4_sys_time, 0, s_comment );
    }
    if ( ui2_tms_lvl & TMS_LEVEL_OFF_LINE )
    {
        i4_ret = _time_msrt_off_line(
            TMS_TYPE_SPOT,
            s_key,
            TMS_REC_CNT_MAX,
            ui4_sys_time,
            s_comment );
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_diff
 *
 * Description: This function create the SPOT TMS record.
 *
 * Inputs:  s_name      Name of the SPOT TMS record.
 *          ui2_tms_lvl The TMS level of the record.
 *
 * Outputs: -
 *
 * Returns: TMSR_FAIL                 Success.
 *          TMSR_FAIL                 Fail.
 *          TMSR_INSUFFICIENT_MEMORY  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_time_msrt_diff(
    const CHAR*           s_name,
    UINT16                ui2_tms_lvl,
    const CHAR*           s_comment )
{
    INT32      i4_ret = TMSR_OK;
    UINT32     ui4_sys_time = x_os_get_sys_tick();
    CHAR*      s_key;

    if ( !TMS_IS_INITIALIZED )
    {
        return TMSR_FAIL;
    }

    s_key = _time_msrt_get_key(s_name);
    if (!s_key)
    {
        return TMSR_FAIL;
    }

    if ( ui2_tms_lvl & TMS_LEVEL_REAL_TIME )
    {
        _time_msrt_diff_print(
            s_key,
            ui4_sys_time,
            0,
            s_comment );
    }
    if ( ui2_tms_lvl & TMS_LEVEL_OFF_LINE )
    {
        i4_ret = _time_msrt_off_line(
            TMS_TYPE_DIFF,
            s_key,
            TMS_REC_CNT_MAX,
            ui4_sys_time,
            s_comment );
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_begin
 *
 * Description: This function begins the BEGIN-END TMS record.
 *
 * Inputs:  s_name      Name of the SPOT TMS record.
 *          ui2_avg_cnt The MAX count of the TMS records.
 *          ui2_tms_lvl The TMS level of the record.
 *
 * Outputs: -
 *
 * Returns: TMSR_FAIL                 Success.
 *          TMSR_FAIL                 Fail.
 *          TMSR_INSUFFICIENT_MEMORY  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_time_msrt_begin(
    const CHAR*           s_name,
    UINT16                ui2_tms_lvl,
    const CHAR*           s_comment )
{
    INT32   i4_ret = TMSR_OK;
    UINT32  ui4_sys_time = x_os_get_sys_tick();
    CHAR*   s_key;

    if ( !TMS_IS_INITIALIZED )
    {
        return TMSR_FAIL;
    }

    s_key = _time_msrt_get_key(s_name);
    if (!s_key)
    {
        return TMSR_FAIL;
    }

    if ( ui2_tms_lvl & TMS_LEVEL_REAL_TIME )
    {
        _time_msrt_begin_print( s_key, ui4_sys_time, s_comment );
    }
    if ( ui2_tms_lvl & TMS_LEVEL_OFF_LINE )
    {
        i4_ret = _time_msrt_off_line(
            TMS_TYPE_BEGIN,
            s_key,
            TMS_REC_CNT_MAX,
            ui4_sys_time,
            s_comment );
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_end
 *
 * Description: This function ends the BEGIN-END TMS record.
 *
 * Inputs:  s_name      Name of the SPOT TMS record.
 *          ui2_tms_lvl The TMS level of the record.
 *
 * Outputs: -
 *
 * Returns: TMSR_FAIL                 Success.
 *          TMSR_FAIL                 Fail.
 *          TMSR_INSUFFICIENT_MEMORY  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_time_msrt_end(
    const CHAR*           s_name,
    UINT16                ui2_tms_lvl,
    const CHAR*           s_comment )
{
    INT32      i4_ret = TMSR_OK;
    UINT32     ui4_sys_time = x_os_get_sys_tick();
    SIZE_T     z_key_size;
    CHAR*      s_key;

    if ( !TMS_IS_INITIALIZED )
    {
        return TMSR_FAIL;
    }

    s_key = _time_msrt_get_key(s_name);
    if (!s_key)
    {
        return TMSR_FAIL;
    }
    z_key_size = x_strlen(s_key) + 1;

    if ( z_key_size > (TMS_NAME_MAX_LENGTH+1) )
    {
        z_key_size = TMS_NAME_MAX_LENGTH + 1;
    }

    if ( ui2_tms_lvl & TMS_LEVEL_REAL_TIME )
    {
        _time_msrt_begin_print( s_key, ui4_sys_time, s_comment );
    }
    if ( ui2_tms_lvl & TMS_LEVEL_OFF_LINE )
    {
        i4_ret = _time_msrt_off_line(
            TMS_TYPE_END,
            s_key,
            0,
            ui4_sys_time,
            s_comment );
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_dump
 *
 * Description: This function dumps all records which matchs the name specified
 *              by caller.
 *
 * Inputs:  s_name Caller specified record name to be dump. Set it to NULL to
 *                 dump all.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID x_time_msrt_dump( const CHAR* s_name )
{
    hash_parse_fct   pf_dump_fct = NULL;

    if ( !TMS_IS_INITIALIZED )
    {
        return;
    }

    if ( s_name && s_name[0] != '\0' )
    {
        pf_dump_fct = _time_msrt_dump_choosed_fct;
    }
    else
    {
        pf_dump_fct = _time_msrt_dump_fct;
    }

    hash_lock(h_time_msrt_db);

    x_dbg_stmt( "\r\n" );
    x_dbg_stmt( "[TIME_MSRT] DUMP RECORDS >>>>START>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
    hash_parse( h_time_msrt_db,
                pf_dump_fct,
                (VOID*)s_name );
    x_dbg_stmt( "[TIME_MSRT] DUMP RECORDS <<<<END<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\r\n");
    x_dbg_stmt( "\r\n" );

    hash_unlock(h_time_msrt_db);
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_dump_detail
 *
 * Description: This function dumps details of all records which matchs the
 *              name specified by caller.
 *
 * Inputs:  s_name Caller specified record name to be dump. Set it to NULL to
 *                 dump all.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID x_time_msrt_dump_detail( const CHAR* s_name )
{
    hash_parse_fct   pf_dump_fct = NULL;

    if ( !TMS_IS_INITIALIZED )
    {
        return;
    }

    if ( s_name && s_name[0] != '\0' )
    {
        pf_dump_fct = _time_msrt_dump_choosed_detail_fct;
    }
    else
    {
        pf_dump_fct = _time_msrt_dump_detail_fct;
    }

    hash_lock(h_time_msrt_db);

    x_dbg_stmt( "\r\n" );
    x_dbg_stmt( "[TIME_MSRT] DUMP DETAIL RECORDS >>>>START>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
    hash_parse( h_time_msrt_db,
                pf_dump_fct,
                (VOID*)s_name );
    x_dbg_stmt( "[TIME_MSRT] DUMP DETAIL RECORDS <<<<END<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\r\n");
    x_dbg_stmt( "\r\n" );

    hash_unlock(h_time_msrt_db);
}

/*-----------------------------------------------------------------------------
 * Name: x_time_msrt_clean
 *
 * Description: This function clean up all records which matchs the
 *              name specified by caller.
 *
 * Inputs:  s_name Caller specified record name to be clean. Set it to NULL to
 *                 clean all.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID x_time_msrt_clean( const CHAR* s_name )
{
    if ( !TMS_IS_INITIALIZED )
    {
        return;
    }

    hash_lock(h_time_msrt_db);

    hash_del_obj_on_cond(
        h_time_msrt_db,
        _time_msrt_clean_cond_fct,
        (hash_obj_free_fct)_tms_rec_free,
        (VOID*)s_name );

    hash_unlock(h_time_msrt_db);

    return;
}

#endif
