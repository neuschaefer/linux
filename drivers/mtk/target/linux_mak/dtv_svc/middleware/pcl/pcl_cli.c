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
 * $RCSfile: pcl_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: a37d432aa942f00af51dbc0b805f7237 $
 *
 * Description:
 *         This file implements CLI command table for Power Control Library.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/common.h"
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "cli/cli.h"
#include "cli/x_cli.h"
#include "pcl/pcl.h"
#include "pcl/x_pcl.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define PCL_MAX_WAKE_UP_EVT         ((UINT8) 10)

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static UINT16       ui2_wakeup_evt_cnt = 0;
static HANDLE_T     ah_wakup_evt[PCL_MAX_WAKE_UP_EVT];
static HANDLE_T     h_pcl_wakeup_evt = NULL_HANDLE;
static HANDLE_T     h_pcl_nfy = NULL_HANDLE;

static INT32 _pcl_cli_get_wakeup_cap(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_wakeup_setup(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_wakeup_time(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_wakeup_event(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_power_down_time(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_misc_ctrl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_wakeup_reason(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_misc_status(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_gpio(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_sif(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_swsif(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_upgrade_ctrl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_res_count(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_wakeup_setup(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_wakeup_time(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_wakeup_event(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_power_down_time(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_misc_ctrl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_wakeup_reason(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_power_down(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_reboot(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_gpio(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_sif(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_swsif(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_upgrade_ctrl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_res_count(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_reg_dereg_nfy(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_servo_ad(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_show_wakeup_list(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _pcl_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);

/* Power Control Library get command table */
static CLI_EXEC_T at_pcl_get_cmd_tbl[] =
{
    {"cap",             NULL,       _pcl_cli_get_wakeup_cap,        NULL,       "Get wakup-up capability",          CLI_SUPERVISOR},
    {"ws",              NULL,       _pcl_cli_get_wakeup_setup,      NULL,       "Get wakup-up setup",               CLI_SUPERVISOR},
    {"wt",              NULL,       _pcl_cli_get_wakeup_time,       NULL,       "Get wakup-up time",                CLI_SUPERVISOR},
    {"we",              NULL,       _pcl_cli_get_wakeup_event,      NULL,       "Get wakup-up evt",                 CLI_SUPERVISOR},        
    {"pdt",             NULL,       _pcl_cli_get_power_down_time,   NULL,       "Get power-down time",              CLI_SUPERVISOR},
    {"ctrl",            NULL,       _pcl_cli_get_misc_ctrl,         NULL,       "Get the status of misc ctrl",      CLI_SUPERVISOR},
    {"wr",              NULL,       _pcl_cli_get_wakeup_reason,     NULL,       "Get wakup-up reason",              CLI_SUPERVISOR},
    {"sts",             NULL,       _pcl_cli_get_misc_status,       NULL,       "Get misc status",                  CLI_SUPERVISOR},
    {"gpio",            NULL,       _pcl_cli_get_gpio,              NULL,       "Get GPIO status",                  CLI_SUPERVISOR},
    {"sif",             NULL,       _pcl_cli_get_sif,               NULL,       "Get SIF data",                     CLI_SUPERVISOR},
    {"swsif",           NULL,       _pcl_cli_get_swsif,             NULL,       "Get SIF data via GPIO",            CLI_SUPERVISOR},
    {"upc",             NULL,       _pcl_cli_get_upgrade_ctrl,      NULL,       "Get upgrade control",              CLI_SUPERVISOR},
    {"rc",              NULL,       _pcl_cli_get_res_count,         NULL,       "Get resource count",               CLI_SUPERVISOR},
    {"sad",             NULL,       _pcl_cli_get_servo_ad,          NULL,       "Get servo AD status",              CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/* Power Control Library set command table */
static CLI_EXEC_T at_pcl_set_cmd_tbl[] =
{
    {"ws",              NULL,       _pcl_cli_set_wakeup_setup,      NULL,       "Set wakup-up setup",               CLI_SUPERVISOR},
    {"wt",              NULL,       _pcl_cli_set_wakeup_time,       NULL,       "Set wakup-up time",                CLI_SUPERVISOR},
    {"we",              NULL,       _pcl_cli_set_wakeup_event,      NULL,       "Set wakup-up evt",                 CLI_SUPERVISOR},    
    {"pdt",             NULL,       _pcl_cli_set_power_down_time,   NULL,       "Set power-down time",              CLI_SUPERVISOR},
    {"ctrl",            NULL,       _pcl_cli_set_misc_ctrl,         NULL,       "Set misc ctrl",                    CLI_SUPERVISOR},
    {"wr",              NULL,       _pcl_cli_set_wakeup_reason,     NULL,       "Set wakup-up reason",              CLI_SUPERVISOR},    
    {"pd",              NULL,       _pcl_cli_set_power_down,        NULL,       "Power down the system",            CLI_SUPERVISOR},
    {"rb",              NULL,       _pcl_cli_set_reboot,            NULL,       "Reboot the system",                CLI_SUPERVISOR},
    {"gpio",            NULL,       _pcl_cli_set_gpio,              NULL,       "Set GPIO status",                  CLI_SUPERVISOR},
    {"sif",             NULL,       _pcl_cli_set_sif,               NULL,       "Set SIF data",                     CLI_SUPERVISOR},
    {"swsif",           NULL,       _pcl_cli_set_swsif,             NULL,       "Set SIF data via GPIO",            CLI_SUPERVISOR},
    {"upc",             NULL,       _pcl_cli_set_upgrade_ctrl,      NULL,       "Set upgrade control",              CLI_SUPERVISOR},
    {"rc",              NULL,       _pcl_cli_set_res_count,         NULL,       "Set resource count",               CLI_SUPERVISOR},    
    {"reg",             NULL,       _pcl_cli_reg_dereg_nfy,         NULL,       "reg/dereg notify function",        CLI_SUPERVISOR},        
    END_OF_CLI_CMD_TBL
};

/* Power Control Library command table */
static CLI_EXEC_T at_pcl_cmd_tbl[] =
{
    {"get",                 "g",        NULL,                       at_pcl_get_cmd_tbl,     "PCL get commands",         CLI_SUPERVISOR},
    {"set",                 "s",        NULL,                       at_pcl_set_cmd_tbl,     "PCL set commands",         CLI_SUPERVISOR},
    {"wakwuplist",          "wl",       _pcl_cli_show_wakeup_list,  NULL,                       "Show wake-up list",        CLI_SUPERVISOR},    
    {CLI_GET_DBG_LVL_STR,   NULL,       _pcl_cli_get_dbg_level,     NULL,                   CLI_GET_DBG_LVL_HELP_STR,   CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,       _pcl_cli_set_dbg_level,     NULL,                   CLI_SET_DBG_LVL_HELP_STR,   CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/* Power Control Library root command table */
static CLI_EXEC_T at_pcl_root_cmd_tbl[] =
{
    {"pcl",     NULL,        NULL,       at_pcl_cmd_tbl,      "Power Control Library commands",     CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};
#endif

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static VOID _pcl_update_wakeup_evt_array(VOID);

static VOID _pcl_dump_wakeup_reason(PCL_WAKE_UP_REASON_T e_wakeup_reason, 
                                    UINT32               ui4_sub_reason);

static VOID _pcl_nfy_fct(HANDLE_T   h_nfy,
                         VOID*      pv_nfy_tag,
                         PCL_COND_T e_nfy_cond,
                         UINT32     ui4_data);
                         
/*-----------------------------------------------------------------------------
 * Name: _pcl_update_wakeup_evt_array
 *
 * Description: This API updates the wake-up event array according to the info
 *              of wake-up list.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID _pcl_update_wakeup_evt_array(VOID)
{
    UINT16      ui2_idx;
    
    for (ui2_idx = 0; ui2_idx < ui2_wakeup_evt_cnt; ui2_idx++)
    {    
        ah_wakup_evt[ui2_idx] = pcl_get_wakeup_evt_hdl_by_idx(ui2_idx);
    }        
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_dump_wakeup_reason
 *
 * Description: This API dumps the wake-up reasons.
 *
 * Inputs:  e_wakeup_reason     The value of wake-up reason.
 *          ui4_sub_reason      The value of wake-up sub-reason.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID _pcl_dump_wakeup_reason(PCL_WAKE_UP_REASON_T e_wakeup_reason,
                             UINT32               ui4_sub_reason)
{
    x_dbg_stmt("Wake-up reason = ");

    switch (e_wakeup_reason)
    {
        case PCL_WAKE_UP_REASON_VGA:
            x_dbg_stmt("VGA\n\r");
            break;

        case PCL_WAKE_UP_REASON_RTC:
            x_dbg_stmt("RTC\n\r");
            break;

        case PCL_WAKE_UP_REASON_FP:
            x_dbg_stmt("FP\n\r");
            break;

        case PCL_WAKE_UP_REASON_IRRC:
            x_dbg_stmt("IRRC\n\r");
            break;

        case PCL_WAKE_UP_REASON_AC_POWER:
            x_dbg_stmt("AC_POWER\n\r");
            break;

        case PCL_WAKE_UP_REASON_HDMI:
            x_dbg_stmt("HDMI\n\r");
            break;
            
        case PCL_WAKE_UP_REASON_UART:
            x_dbg_stmt("UART\n\r");
            break;

        case PCL_WAKE_UP_REASON_UART_NORMAL:
            x_dbg_stmt("UART normal\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_DIGIT_0:
            x_dbg_stmt("RC digit 0\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_DIGIT_1:
            x_dbg_stmt("RC digit 1\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_DIGIT_2:
            x_dbg_stmt("RC digit 2\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_DIGIT_3:
            x_dbg_stmt("RC digit 3\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_DIGIT_4:
            x_dbg_stmt("RC digit 4\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_DIGIT_5:
            x_dbg_stmt("RC digit 5\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_DIGIT_6:
            x_dbg_stmt("RC digit 6\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_DIGIT_7:
            x_dbg_stmt("RC digit 7\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_DIGIT_8:
            x_dbg_stmt("RC digit 8\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_DIGIT_9:
            x_dbg_stmt("RC digit 9\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_PRG_UP:
            x_dbg_stmt("RC prog_up\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_PRG_DOWN:
            x_dbg_stmt("RC prog_down\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_RC_INP_SRC:
            x_dbg_stmt("RC inp_src\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_FP_PRG_UP:
            x_dbg_stmt("FP prog_up\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_FP_PRG_DOWN:
            x_dbg_stmt("FP prog_down\n\r");
            break;
                        
        case PCL_WAKE_UP_REASON_FP_INP_SRC:
            x_dbg_stmt("FP inp_src\n\r");
            break;

        case PCL_WAKE_UP_REASON_CUSTOM_1:
            x_dbg_stmt("CUSTOM_1\n\r");
            break;

        case PCL_WAKE_UP_REASON_CUSTOM_2:
            x_dbg_stmt("CUSTOM_2\n\r");
            break;

        case PCL_WAKE_UP_REASON_CUSTOM_3:
            x_dbg_stmt("CUSTOM_3\n\r");
            break;

        case PCL_WAKE_UP_REASON_CUSTOM_4:
            x_dbg_stmt("CUSTOM_4\n\r");
            break;
        case PCL_WAKE_UP_REASON_RTC_SPECIAL:
            x_dbg_stmt("RTC Special\n\r");
            break;
        case PCL_WAKE_UP_REASON_UNKNOWN:
        default:
            x_dbg_stmt("unknown\n\r");
            break;
    }
    
    x_dbg_stmt("Wake-up sub-reason = 0x%04x\n\r", ui4_sub_reason);
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_nfy_fct
 *
 * Description: This is Power Control Library notify function.
 *
 * Inputs:  
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _pcl_nfy_fct(HANDLE_T   h_nfy,
                         VOID*      pv_nfy_tag,
                         PCL_COND_T e_nfy_cond,
                         UINT32     ui4_data)
{
    x_dbg_stmt("==== PCL notify ====\n\r");
    switch (e_nfy_cond)
    {
        case PCL_COND_UNKNOWN:
            x_dbg_stmt("nfy_cond = PCL_COND_UNKNOWN\n\r");
            break;
                    
        case PCL_COND_WAKE_UP_EXPIRED:
            x_dbg_stmt("nfy_cond = PCL_COND_WAKE_UP_EXPIRED\n\r");
            break;

        case PCL_COND_RES_CNT_UPDATE_PANEL:
            x_dbg_stmt("nfy_cond = PCL_COND_RES_CNT_UPDATE_PANEL\n\r");
            x_dbg_stmt("res_count = %d\n\r", ui4_data);
            break;
            
        case PCL_COND_RES_CNT_UPDATE_CPU:
            x_dbg_stmt("nfy_cond = PCL_COND_RES_CNT_UPDATE_CPU\n\r");
            x_dbg_stmt("res_count = %d\n\r", ui4_data);
            break;
                        
        case PCL_COND_MONITORING_GPIO_MATCHED:
            x_dbg_stmt("nfy_cond = PCL_COND_MONITORING_GPIO_MATCHED\n\r");
            break;
                        
        case PCL_COND_RES_CNT_UPDATE_CUSTOM_1:
            x_dbg_stmt("nfy_cond = PCL_COND_RES_CNT_UPDATE_CUSTOM_1\n\r");
            x_dbg_stmt("res_count = %d\n\r", ui4_data);
            break;
                        
        case PCL_COND_RES_CNT_UPDATE_CUSTOM_2:
            x_dbg_stmt("nfy_cond = PCL_COND_RES_CNT_UPDATE_CUSTOM_2\n\r");
            x_dbg_stmt("res_count = %d\n\r", ui4_data);
            break;
                        
        case PCL_COND_RES_CNT_UPDATE_CUSTOM_3:
            x_dbg_stmt("nfy_cond = PCL_COND_RES_CNT_UPDATE_CUSTOM_3\n\r");
            x_dbg_stmt("res_count = %d\n\r", ui4_data);
            break;
                        
        case PCL_COND_RES_CNT_UPDATE_CUSTOM_4:                                                
            x_dbg_stmt("nfy_cond = PCL_COND_RES_CNT_UPDATE_CUSTOM_4\n\r");
            x_dbg_stmt("res_count = %d\n\r", ui4_data);
            break;
        default:
            x_dbg_stmt("nfy_cond = %d\n\r", e_nfy_cond);
            break;
    }
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_wakeup_cap
 *
 * Description: This API gets the wake-up capability.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_wakeup_cap(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_wakeup_cap;
    UINT32      ui4_wakeup_ctrl_cap;

    /* pcl.g.cap */

    /* Check arguments */
    if ((i4_argc != 1) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.cap\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_get_wakeup_capability(&ui4_wakeup_cap, &ui4_wakeup_ctrl_cap);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Wake-up capability = 0x%08x\n\r", ui4_wakeup_cap);
    x_dbg_stmt("Wake-up ctrl capability = 0x%08x\n\r", ui4_wakeup_ctrl_cap);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_wakeup_setup
 *
 * Description: This API gets the wake-up setup.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_wakeup_setup(INT32        i4_argc,
                                       const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_wakeup_setup;

    /* pcl.g.ws */

    /* Check arguments */
    if ((i4_argc != 1) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.ws\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_get_wakeup_setup(&ui4_wakeup_setup);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Wake-up setup = 0x%08x\n\r", ui4_wakeup_setup);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_wakeup_time
 *
 * Description: This API gets the wake-up time.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_wakeup_time(INT32        i4_argc,
                                      const CHAR** pps_argv)
{
    INT32       i4_return;
    TIME_T      t_time;

    /* pcl.g.wt */

    /* Check arguments */
    if ((i4_argc != 1) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.wt\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_get_wakeup_time(&t_time);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    if (t_time == NULL_HANDLE)
    {
        x_dbg_stmt("Wake-up time has not been set yet\n\r");
    }
    else
    {                    
        if ((t_time >> 32) > 0)
        {
            x_dbg_stmt("Wake-up time = 0x%08x%08x (UTC seconds)\n\r",
                       (UINT32)(t_time >> 32),
                       (UINT32)(t_time));
        }
        else
        {
            x_dbg_stmt("Wake-up time = 0x%08x (UTC seconds)\n\r", (UINT32)(t_time));
        }    
    }  

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_wakeup_event
 *
 * Description: This API gets a wake-up event.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_wakeup_event(INT32        i4_argc,
                                       const CHAR** pps_argv)
{
    INT32               i4_return;
    UINT8               ui1_idx;
    PCL_WAKE_UP_CTRL_T  t_wakeup_ctrl;

    /* pcl.g.we [index] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.we [index]\n\r");
        return CLIR_INV_CMD_USAGE;
    }
    
    ui1_idx = (UINT8)(x_strtoull(pps_argv[1], NULL, 0)); 
    
    if (ui1_idx < PCL_MAX_WAKE_UP_EVT)
    {
        i4_return = x_pcl_get_wakeup_event(ah_wakup_evt[ui1_idx],
                                           &t_wakeup_ctrl);
        
        if (i4_return != PCLR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }          
        
        if (t_wakeup_ctrl.ui4_misc_ctrl == PCL_MISC_CTRL_IGNORE)
        {            
            x_dbg_stmt("wake-up_time=0x%08x%08x, misc_ctrl=ignore\n\r",
                       (UINT32)(t_wakeup_ctrl.t_time >> 32),
                       (UINT32)t_wakeup_ctrl.t_time);
        }
        else
        {
            x_dbg_stmt("wake-up_time=0x%08x%08x, misc_ctrl=0x%08x\n\r",
                       (UINT32)(t_wakeup_ctrl.t_time >> 32),
                       (UINT32)t_wakeup_ctrl.t_time,
                       t_wakeup_ctrl.ui4_misc_ctrl);            
        }                                   
    }
    else
    {
        x_dbg_stmt("The maximum of wake-up event array is %d!\n\r", PCL_MAX_WAKE_UP_EVT);
        return CLIR_CMD_EXEC_ERROR;
    }                

    return CLIR_OK;    
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_power_down_time
 *
 * Description: This API gets the power-down time.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_power_down_time(INT32        i4_argc,
                                          const CHAR** pps_argv)
{
    INT32       i4_return;
    TIME_T      t_time;

    /* pcl.g.pdt */

    /* Check arguments */
    if ((i4_argc != 1) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.pdt\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_get_power_down_time(&t_time);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    if (t_time == NULL_HANDLE)
    {
        x_dbg_stmt("Power-dow time has not been set yet\n\r");
    }
    else
    {                    
        if ((t_time >> 32) > 0)
        {
            x_dbg_stmt("Power-dow time = 0x%08x%08x (UTC seconds)\n\r",
                       (UINT32)(t_time >> 32),
                       (UINT32)(t_time));
        }
        else
        {
            x_dbg_stmt("Power-dow time = 0x%08x (UTC seconds)\n\r", (UINT32)(t_time));
        }    
    }  

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_misc_ctrl
 *
 * Description: This API gets the status of misc controls.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_misc_ctrl(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_ctrl;

    /* pcl.g.ctrl */

    /* Check arguments */
    if ((i4_argc != 1) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.ctrl\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_get_misc_ctrl(&ui4_ctrl);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    if (ui4_ctrl == PCL_MISC_CTRL_IGNORE)
    {
        x_dbg_stmt("Wake-up control has not been set yet\n\r");
    }
    else
    {     
        x_dbg_stmt("Misc ctrl = 0x%08x\n\r", ui4_ctrl);
    }        

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_wakeup_reason
 *
 * Description: This API gets the wake-up reason.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_wakeup_reason(INT32        i4_argc,
                                        const CHAR** pps_argv)
{
    INT32                   i4_return;
    PCL_WAKE_UP_REASON_T    e_wakeup_reason;
    UINT32                  ui4_sub_reason;

    /* pcl.g.wr */

    /* Check arguments */
    if ((i4_argc != 1) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.wr\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_get_wakeup_reason(&e_wakeup_reason, (VOID*)&ui4_sub_reason);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    _pcl_dump_wakeup_reason(e_wakeup_reason, ui4_sub_reason);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_misc_status
 *
 * Description: This API gets the Power Control driver misc status.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_misc_status(INT32        i4_argc,
                                      const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_status;

    /* pcl.g.sts */

    /* Check arguments */
    if ((i4_argc != 1) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.sts\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_get_misc_status(&ui4_status);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Status = 0x%08x\n\r", ui4_status);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_gpio
 *
 * Description: This API gets a specific GPIO status.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_gpio(INT32        i4_argc,
                               const CHAR** pps_argv)
{
    INT32               i4_return;
    UINT32              ui4_id;
    PCL_GPIO_MODE_T     e_mode;
    PCL_GPIO_STATUS_T   e_status;
    CHAR*               apc_gpio_status[] = {"Unknown", "Low", "High"};

    /* pcl.g.gpio [id] [mode] */

    /* Check arguments */
    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.gpio [id] [mode]\n\r");
        x_dbg_stmt("  [mode]: 0=Output, 1=Input\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    ui4_id = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));
    if (x_strcmp("0", pps_argv[1]) == 0)
    {
        e_mode = PCL_GPIO_MODE_OUTPUT;
    }
    else if (x_strcmp("1", pps_argv[1]) == 0)
    {
        e_mode = PCL_GPIO_MODE_INPUT;
    }
    else
    {
        return CLIR_INV_ARG;
    }

    i4_return = x_pcl_get_gpio(ui4_id, e_mode, &e_status);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("GPIO 0x%x (%s) value is %s\n\r",
               ui4_id,
               (e_mode == PCL_GPIO_MODE_OUTPUT) ? "output mode" : "input mode ",
               apc_gpio_status[e_status]);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_sif
 *
 * Description: This API gets the data of a SIF address.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_sif(INT32        i4_argc,
                              const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_addr;
    UINT32      ui4_offset;
    UINT8       ui1_data;

    /* pcl.g.sif [addr] [offset] */

    /* Check arguments */
    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.sif [addr] [offset]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    ui4_addr = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));
    ui4_offset = (UINT32)(x_strtoull(pps_argv[2], NULL, 0));

    i4_return = x_pcl_get_sif(ui4_addr, ui4_offset, &ui1_data);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("SIF 0x%x (offse=0x%x) value is 0x%x\n\r", ui4_addr, ui4_offset, ui1_data);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_swsif
 *
 * Description: This API gets SIF data via GPIO.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_swsif(INT32        i4_argc,
                                const CHAR** pps_argv)
{
    INT32                       i4_return;
    PCL_SIF_INFO_BY_GPIO_T      t_sif_info;
    UINT8                       aui1_buf[10];

    /* pcl.g.swsif [sda] [scl] [addr] [offset_len] [offset] */

    /* Check arguments */
    if ((i4_argc != 6) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.swsif [sda] [scl] [addr] [offset_len] [offset]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    t_sif_info.ui4_data_gpio_id = (UINT32)(x_strtoull(pps_argv[1], NULL, 0)); /* SDA */
    t_sif_info.ui4_addr_gpio_id = (UINT32)(x_strtoull(pps_argv[2], NULL, 0)); /* SCL */
    t_sif_info.ui4_addr = (UINT32)(x_strtoull(pps_argv[3], NULL, 0));
    t_sif_info.ui1_offset_len = (UINT8)(x_strtoull(pps_argv[4], NULL, 0));
    t_sif_info.ui4_offset = (UINT32)(x_strtoull(pps_argv[5], NULL, 0));
    t_sif_info.ui1_data_len = 1; /* Data length */
    t_sif_info.pui1_data = aui1_buf; /* Data buffer */

    i4_return = x_pcl_get_sif_by_gpio(&t_sif_info);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("swsif 0x%x (offset=0x%x) value is 0x%x\n\r",
               t_sif_info.ui4_addr,
               t_sif_info.ui4_offset,
               aui1_buf[0]);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_upgrade_ctrl
 *
 * Description: This API gets the status of upgrade controls.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_upgrade_ctrl(INT32        i4_argc,
                                       const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_ctrl;

    /* pcl.g.upc */

    /* Check arguments */
    if ((i4_argc != 1) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.upc\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_get_upgrade_ctrl(&ui4_ctrl);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Upgrade ctrl = 0x%08x\n\r", ui4_ctrl);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_res_count
 *
 * Description: This API obtains the count of a specific resource.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_res_count(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32               i4_return;
    PCL_RES_TYPE_T      e_res_type;    
    UINT16              ui2_count = 0;

    /* pcl.g.rc [type] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.rc [type]\n\r");
        return CLIR_INV_CMD_USAGE;
    }
        
    e_res_type = (PCL_RES_TYPE_T)(x_strtoull(pps_argv[1], NULL, 0)); 
           
    if (e_res_type >= PCL_RES_TYPE_MAX)
    {
        x_dbg_stmt("The value of res_type needs to be less than %d\n\r", PCL_RES_TYPE_MAX);
        return CLIR_INV_CMD_USAGE;
    }
               
    i4_return = x_pcl_get_res_count(e_res_type,
                                    &ui2_count);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Count of res_type [%d] = %d\n\r", e_res_type, ui2_count);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_wakeup_setup
 *
 * Description: This API sets the wake-up setup.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_wakeup_setup(INT32        i4_argc,
                                       const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_wakeup_setup;

    /* pcl.s.setup [ctrl] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.s.ws [ctrl]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_set_wakeup_setup((UINT32)x_strtoull(pps_argv[1], NULL, 0));

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_return = x_pcl_get_wakeup_setup(&ui4_wakeup_setup);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Wake-up setup = 0x%08x\n\r", ui4_wakeup_setup);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_wakeup_time
 *
 * Description: This API sets the wake-up time.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_wakeup_time(INT32        i4_argc,
                                      const CHAR** pps_argv)
{
    INT32       i4_return;
    TIME_T      t_time;

    /* pcl.s.wt [time] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.s.wt [time]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_set_wakeup_time((TIME_T)(x_strtoull(pps_argv[1], NULL, 0)));

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    if (h_pcl_wakeup_evt == NULL_HANDLE)
    {
        h_pcl_wakeup_evt = pcl_get_wakeup_event_hdl_pcl();
        ui2_wakeup_evt_cnt++;
    }    

    i4_return = x_pcl_get_wakeup_time(&t_time);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    if ((t_time >> 32) > 0)
    {
        x_dbg_stmt("Wake-up time = 0x%08x%08x (UTC seconds)\n\r",
                   (UINT32)(t_time >> 32),
                   (UINT32)(t_time));
    }
    else
    {
        x_dbg_stmt("Wake-up time = 0x%08x (UTC seconds)\n\r", (UINT32)(t_time));
    }

    _pcl_update_wakeup_evt_array();

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_wakeup_event
 *
 * Description: This API sets a wake-up event.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_wakeup_event(INT32        i4_argc,
                                       const CHAR** pps_argv)
{
    INT32               i4_return;
    UINT8               ui1_idx;
    HANDLE_T            h_wakeup_evt;
    PCL_WAKE_UP_CTRL_T  t_wakeup_ctrl;

    /* pcl.s.we [index] [time] [misc_ctrl] */

    /* Check arguments */
    if (((i4_argc != 4) && (i4_argc != 3)) || 
        ((i4_argc == 3) && (x_strtoull(pps_argv[2], NULL, 0) != 0)) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.s.we [index] [time] [misc_ctrl]\n\r");
        x_dbg_stmt("  [index]: 0xff=adding a new event\n\r");
        x_dbg_stmt("           others=index of an existing event\n\r");
        x_dbg_stmt("  [time]: 0=remove the event indicated by index (misc_ctrl is ignore)\n\r");
        x_dbg_stmt("          others=wake-up time\n\r");
        x_dbg_stmt("  [misc_ctrl]: ignore=ignore it\n\r");
        x_dbg_stmt("               others=misc time\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((x_strtoull(pps_argv[1], NULL, 0)) == 0xFF) /* New event */
    {
        if (ui2_wakeup_evt_cnt <= PCL_MAX_WAKE_UP_EVT)
        {     
            t_wakeup_ctrl.t_time = (TIME_T)(x_strtoull(pps_argv[2], NULL, 0)); 
            t_wakeup_ctrl.ui4_misc_ctrl = (UINT32)(x_strtoull(pps_argv[3], NULL, 0)); 
                 
            i4_return = x_pcl_add_wakeup_event(&t_wakeup_ctrl, &h_wakeup_evt);
            
            if (i4_return != PCLR_OK)
            {
                return CLIR_CMD_EXEC_ERROR;
            }    
        }
        else 
        {
            x_dbg_stmt("Reach the maximum of wake-up event array!\n\r");
            return CLIR_CMD_EXEC_ERROR;
        }    
        
        ui2_wakeup_evt_cnt++;
        _pcl_update_wakeup_evt_array();
        x_dbg_stmt("New wake-up event had been created successfully\n\r");                            
    }
    else /* Existing event */
    {
        ui1_idx = (UINT8)(x_strtoull(pps_argv[1], NULL, 0)); 
        t_wakeup_ctrl.t_time = (TIME_T)(x_strtoull(pps_argv[2], NULL, 0)); 
        
        if (x_strcmp("ignore", pps_argv[3]) == 0)
        {          
            t_wakeup_ctrl.ui4_misc_ctrl = PCL_MISC_CTRL_IGNORE;
        }
        else
        {              
            t_wakeup_ctrl.ui4_misc_ctrl = (UINT32)(x_strtoull(pps_argv[3], NULL, 0));         
        }            
             
        if (t_wakeup_ctrl.t_time == 0) /* Remove it */
        {                       
            if (ah_wakup_evt[ui1_idx] == h_pcl_wakeup_evt)
            {
                x_dbg_stmt("Removing PCL wake-up event is not allowed!!!\n\r");
                return CLIR_OK;
                
            }
                              
            i4_return = x_pcl_remove_wakeup_event(ah_wakup_evt[ui1_idx]);
            
            if (i4_return != PCLR_OK)
            {
                return CLIR_CMD_EXEC_ERROR;
            }  
            
            _pcl_update_wakeup_evt_array();
            x_dbg_stmt("Wake-up event had been removed successfully\n\r");
        }
        else /* Modify it */
        {
            i4_return = x_pcl_set_wakeup_event(ah_wakup_evt[ui1_idx],
                                               &t_wakeup_ctrl);
            
            if (i4_return != PCLR_OK)
            {
                return CLIR_CMD_EXEC_ERROR;
            }              
            
            _pcl_update_wakeup_evt_array();
            x_dbg_stmt("Wake-up event had been changed successfully\n\r");
        }                                                    
    }
                            
    return CLIR_OK;    
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_power_down_time
 *
 * Description: This API sets the power-down time.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_power_down_time(INT32        i4_argc,
                                          const CHAR** pps_argv)
{
    INT32       i4_return;
    TIME_T      t_time;

    /* pcl.s.pdt [time] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.s.pdt [time]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_set_power_down_time((TIME_T)(x_strtoull(pps_argv[1], NULL, 0)));

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_return = x_pcl_get_power_down_time(&t_time);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    if ((t_time >> 32) > 0)
    {
        x_dbg_stmt("Power-down time = 0x%08x%08x (UTC seconds)\n\r",
                   (UINT32)(t_time >> 32),
                   (UINT32)(t_time));
    }
    else
    {
        x_dbg_stmt("Power-down time = 0x%08x (UTC seconds)\n\r", (UINT32)(t_time));
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_misc_ctrl
 *
 * Description: This API sets the misc controls.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_misc_ctrl(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_ctrl;

    /* pcl.s.ctrl [ctrl] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.s.ctrl [ctrl]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_set_misc_ctrl((UINT32)x_strtoull(pps_argv[1], NULL, 0));

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    if (h_pcl_wakeup_evt == NULL_HANDLE)
    {
        h_pcl_wakeup_evt = pcl_get_wakeup_event_hdl_pcl();
        ui2_wakeup_evt_cnt++;
    }  
            
    i4_return = x_pcl_get_misc_ctrl(&ui4_ctrl);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Misc ctrl = 0x%08x\n\r", ui4_ctrl);

    _pcl_update_wakeup_evt_array();
    
    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_wakeup_reason
 *
 * Description: This API sets the wake-up reason.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_wakeup_reason(INT32        i4_argc,
                                        const CHAR** pps_argv)
{
    INT32                   i4_return;
    PCL_WAKE_UP_REASON_T    e_wakeup_reason;
    UINT32                  ui4_sub_reason;

    /* pcl.s.wr [reason] */

    /* Check arguments */
    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.s.wr [reason] [sub-reason]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_wakeup_reason = (PCL_WAKE_UP_REASON_T)(x_strtoull(pps_argv[1], NULL, 0));  
    ui4_sub_reason = (UINT32)(x_strtoull(pps_argv[2], NULL, 0));
                               
    i4_return = x_pcl_set_wakeup_reason(e_wakeup_reason, (VOID*)&ui4_sub_reason);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    
    _pcl_dump_wakeup_reason(e_wakeup_reason, ui4_sub_reason);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_power_down
 *
 * Description: This API sets the system into power-down mode.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_power_down(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    INT32       i4_return;

    /* pcl.s.pd */

    /* Check arguments */
    if ((i4_argc != 1) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.s.pd\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_set_power_down();

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_reboot
 *
 * Description: This API reboots the system.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_reboot(INT32        i4_argc,
                                 const CHAR** pps_argv)
{
    INT32       i4_return;

    /* pcl.s.rb */

    /* Check arguments */
    if ((i4_argc != 1) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.s.rb\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_pcl_set_reboot();

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_gpio
 *
 * Description: This API sets a specific GPIO status.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_gpio(INT32        i4_argc,
                               const CHAR** pps_argv)
{
    INT32               i4_return;
    UINT32              ui4_id;
    PCL_GPIO_STATUS_T   e_ctrl;
    PCL_GPIO_STATUS_T   e_status;
    CHAR*               apc_gpio_status[] = {"Unknown", "Low", "High"};

    /* pcl.g.gpio [id] [ctrl] */

    /* Check arguments */
    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.gpio [id] [ctrl]\n\r");
        x_dbg_stmt("  [ctrl]: 0=Low, 1=High\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    ui4_id = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));

    if (x_strcmp("0", pps_argv[2]) == 0)
    {
        e_ctrl = PCL_GPIO_STATUS_LOW;
    }
    else if (x_strcmp("1", pps_argv[2]) == 0)
    {
        e_ctrl = PCL_GPIO_STATUS_HIGH;
    }
    else
    {
        return CLIR_INV_ARG;
    }

    i4_return = x_pcl_set_gpio(ui4_id, e_ctrl);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_return = x_pcl_get_gpio(ui4_id, PCL_GPIO_MODE_OUTPUT, &e_status);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("GPIO 0x%x (output mode) is set as %s\n\r", ui4_id, apc_gpio_status[e_status]);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_sif
 *
 * Description: This API sets the data of a SIF address.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_sif(INT32        i4_argc,
                              const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_addr;
    UINT32      ui4_offset;
    UINT8       ui1_data_read;
    UINT8       ui1_data_write;

    /* pcl.g.sif [addr] [offset] [data] */

    /* Check arguments */
    if ((i4_argc != 4) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.sif [addr] [offset] [data]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    ui4_addr = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));
    ui4_offset = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));
    ui1_data_write = (UINT8)(x_strtoull(pps_argv[2], NULL, 0));

    i4_return = x_pcl_set_sif(ui4_addr, ui4_offset, ui1_data_write);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_return = x_pcl_get_sif(ui4_addr, ui4_offset, &ui1_data_read);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("SIF 0x%x (offset=0x%x) is set as 0x%x\n\r", ui4_addr, ui4_offset, ui1_data_read);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_swsif
 *
 * Description: This API sets SIF data via GPIO.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_swsif(INT32        i4_argc,
                                const CHAR** pps_argv)
{
    INT32                       i4_return;
    PCL_SIF_INFO_BY_GPIO_T      t_sif_info;
    UINT8                       aui1_buf[10];

    /* pcl.g.swsif [sda] [scl] [addr] [offset_len] [offset] [data] */

    /* Check arguments */
    if ((i4_argc != 7) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.swsif [sda] [scl] [addr] [offset_len] [offset] [data]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    t_sif_info.ui4_data_gpio_id = (UINT32)(x_strtoull(pps_argv[1], NULL, 0)); /* SDA */
    t_sif_info.ui4_addr_gpio_id = (UINT32)(x_strtoull(pps_argv[2], NULL, 0)); /* SCL */
    t_sif_info.ui4_addr = (UINT32)(x_strtoull(pps_argv[3], NULL, 0));
    t_sif_info.ui1_offset_len = (UINT8)(x_strtoull(pps_argv[4], NULL, 0));
    t_sif_info.ui4_offset = (UINT32)(x_strtoull(pps_argv[5], NULL, 0));
    t_sif_info.ui1_data_len = 1; /* Data length */
    aui1_buf[0] = (UINT8)(x_strtoull(pps_argv[6], NULL, 0));
    t_sif_info.pui1_data = aui1_buf; /* Data buffer */

    i4_return = x_pcl_set_sif_by_gpio(&t_sif_info);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("swsif 0x%x (offset=0x%x) is set as 0x%x\n\r",
               t_sif_info.ui4_addr,
               t_sif_info.ui4_offset,
               aui1_buf[0]);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_upgrade_ctrl
 *
 * Description: This API sets the data for upgrade controls.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_upgrade_ctrl(INT32        i4_argc,
                                       const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_ctrl;

    /* pcl.g.upc [ctrl] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.upc [ctrl]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    ui4_ctrl = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));

    i4_return = x_pcl_set_upgrade_ctrl(ui4_ctrl);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Upgrade ctrl = 0x%08x\n\r", ui4_ctrl);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_res_count
 *
 * Description: This API increase the count of a specific resource.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_res_count(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32               i4_return;
    PCL_RES_TYPE_T      e_res_type;    
    BOOL                b_increase;

    /* pcl.s.rc [type] [cal] */

    /* Check arguments */
    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.s.rc [type] [cal]\n\r");
        x_dbg_stmt("  [cal]: 0=decrease, 1=increase\n\r");
        return CLIR_INV_CMD_USAGE;
    }
        
    e_res_type = (PCL_RES_TYPE_T)(x_strtoull(pps_argv[1], NULL, 0)); 
    
    if (e_res_type >= PCL_RES_TYPE_MAX)
    {
        x_dbg_stmt("The value of res_type needs to be less than %d\n\r", PCL_RES_TYPE_MAX);
        return CLIR_INV_CMD_USAGE;
    }
        
    if (x_strcmp("0", pps_argv[2]) == 0)
    {
        b_increase = FALSE;
    }        
    else if (x_strcmp("1", pps_argv[2]) == 0)
    {
        b_increase = TRUE;
    }
    else
    {
        return CLIR_INV_ARG;
    }
   
    i4_return = x_pcl_set_res_count(e_res_type,
                                    b_increase);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_reg_dereg_nfy
 *
 * Description: This API register or de-register an notify function.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_reg_dereg_nfy(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;

    /* pcl.s.reg [ctrl] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.s.reg [ctrl]\n\r");
        x_dbg_stmt("  [ctrl]: 0=dereg, 1=reg\n\r");
        return CLIR_INV_CMD_USAGE;
    }
        
    if (x_strcmp("0", pps_argv[1]) == 0)
    {
        if (h_pcl_nfy != NULL_HANDLE)
        {            
            i4_return = x_pcl_dereg_nfy_fct(h_pcl_nfy);
            
            if (i4_return != PCLR_OK)
            {
                return CLIR_CMD_EXEC_ERROR;
            }   
            
            h_pcl_nfy = NULL_HANDLE;
            x_dbg_stmt("Deregister notify function successfully\n\r");
        }
        else
        {
            x_dbg_stmt("Notify function was registered!!!\n\r");
            return CLIR_CMD_EXEC_ERROR;
        }    
    }
    else if (x_strcmp("1", pps_argv[1]) == 0)
    {
        if (h_pcl_nfy == NULL_HANDLE)
        {            
         i4_return = x_pcl_reg_nfy_fct(NULL, _pcl_nfy_fct, &h_pcl_nfy);
            
            if (i4_return != PCLR_OK)
            {
                return CLIR_CMD_EXEC_ERROR;
            }    
            
          x_dbg_stmt("Register notify function successfully\n\r");
        }
        else
        {
            x_dbg_stmt("Notify function had been registered already!!!\n\r");
            return CLIR_CMD_EXEC_ERROR;
        }       
    }
    else
    {
        return CLIR_INV_ARG;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_servo_ad
 *
 * Description: This API gets a specific servo AD status.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_servo_ad(INT32        i4_argc,
                                   const CHAR** pps_argv)
{
    INT32                   i4_return;
    PCL_SERVO_AD_INFO_T     t_servo_ad_info;

    /* pcl.g.sad [id] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.g.sid [id]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    t_servo_ad_info.ui4_id = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));
    t_servo_ad_info.ui4_data = 0;

    i4_return = x_pcl_get_servo_ad(&t_servo_ad_info);

    if (i4_return != PCLR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Servo AD 0x%x value is %d\n\r",
               t_servo_ad_info.ui4_id,
               t_servo_ad_info.ui4_data);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_show_wakeup_list
 *
 * Description: This API shows the wake-up list.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_show_wakeup_list(INT32        i4_argc,
                                       const CHAR** pps_argv)
{
    /* cecm.wl */

    pcl_show_wakeup_list();

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_get_dbg_level
 *
 * Description: This API gets the current setting of Power Control Library
 *              debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_get_dbg_level(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;

    /* pcl.gdl */

    i4_return = x_cli_show_dbg_level(pcl_get_dbg_level());

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_dbg_level
 *
 * Description: This API sets the debug level of Power Control Library.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_dbg_level(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    /* pcl.sdl [level] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("pcl.sdl [level]\n\r");
        x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (pcl_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }
        else
        {
            i4_return = x_cli_show_dbg_level(pcl_get_dbg_level());
        }
    }
    else
    {
        x_dbg_stmt("pcl.sdl [level]\n\r");
        x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return i4_return;
}
/*-----------------------------------------------------------------------------
 * Name: _pcl_cli_set_log_level
 *
 * Description: 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_cli_set_log_level(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    UINT32  ui4_log_levl;

    ui4_log_levl = pcl_get_log_level();

    x_cli_parse_log_level(i4_argc, pps_argv, &ui4_log_levl);

    pcl_set_log_level(ui4_log_levl);

    return CLIR_OK;
}
#endif


/*-----------------------------------------------------------------------------
 * Name: pcl_cli_init
 *
 * Description: This API initializes Power Control Library CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 Routine successful.
 *          PCLR_CLI_FAILED         CLI operation is failed.
 ----------------------------------------------------------------------------*/
INT32 pcl_cli_init(VOID)
{
#ifdef CLI_LVL_ALL
    INT32       i4_return;
    UINT8       ui1_idx;

    /* Initialize internal variables and data structure */
    for (ui1_idx = 0; ui1_idx < PCL_MAX_WAKE_UP_EVT; ui1_idx++)
    {
        ah_wakup_evt[ui1_idx] = NULL_HANDLE;
    }    

    /* Attach Power Control Library CLI command table to CLI */
    i4_return = x_cli_attach_cmd_tbl(at_pcl_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);
    if (i4_return != CLIR_OK)
    {
        return PCLR_CLI_FAILED;
    }

    i4_return = x_cli_register_log_command("pcl", NULL, _pcl_cli_set_log_level, NULL);
    if(i4_return != CLIR_OK)
    {
        return PCLR_CLI_FAILED;
    }
    return PCLR_OK;
#else
    return PCLR_OK;
#endif
}

