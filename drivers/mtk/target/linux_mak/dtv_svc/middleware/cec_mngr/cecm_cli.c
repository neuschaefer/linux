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
 * $RCSfile: cecm_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/4 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: a37d432aa942f00af51dbc0b805f7237 $
 *
 * Description:
 *         This file implements CLI command table for CEC Manager.
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
#include "unicode/x_uc_str.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_cec.h"
#include "cec_mngr/cecm_ctrl.h"
#include "cec_mngr/cecm.h"
#include "cec_mngr/x_cecm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#if (defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST))

#ifdef CLI_LVL_ALL
static HANDLE_T             h_cecm_svc = NULL_HANDLE;
static CHAR                 ac_str[32];
static INT32 _cecm_cli_open(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_close(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_cec_fct(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_get_dev_info_by_la(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_get_comp_id_by_la(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_start_dev_discovery(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_stop_dev_discovery(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_find_tv_la(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_ping_dev(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_give_vndr_id(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_standby(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_req_act_src(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_routing_change(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_stream_path(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_get_cec_version(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_give_physical_address(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_report_physical_address(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_get_menu_lang(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_menu_lang(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_give_deck_status(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_deck_ctrl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_deck_play(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_give_osd_name(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_menu_request(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_user_ctrl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_power_on_dev(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_give_dev_pwr_sts(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_sys_aud_mode_req(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_give_audio_status(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_give_sys_aud_mode_sts(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_request_audio_descriptor(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_request_init_arc(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_request_terminate_arc(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_report_init_arc(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_report_terminate_arc(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_aud_rate(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_clear_timer(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_timer(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_timer_prog_title(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_give_tuner_status(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_select_service(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_tuner_step(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_record_on(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_record_off(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_show_dd_setting(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
#endif

#ifdef CLI_LVL_GUEST
static INT32 _cecm_cli_show_dev_list(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_show_svc_list(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cecm_cli_query_cec_fct(INT32 i4_argc, const CHAR** pps_argv);
#endif


/* CEC Manager command table */
static CLI_EXEC_T at_cecm_cmd_tbl[] =
{
    #ifdef CLI_LVL_GUEST
    {"devlist",             "dl",   _cecm_cli_show_dev_list,            NULL,       "Show dev list",            CLI_GUEST     },
    {"svclist",             "sl",   _cecm_cli_show_svc_list,            NULL,       "Show svc list",            CLI_GUEST     },
    {"status",              "sts",  _cecm_cli_query_cec_fct,            NULL,       "Query CEC function ctrl",  CLI_GUEST     },   
    #endif
    #ifdef CLI_LVL_ALL
    {"devdiscvry",          "dd",   _cecm_cli_show_dd_setting,          NULL,       "Show dev discvry setting", CLI_SUPERVISOR},         
    {"open",                "o",    _cecm_cli_open,                     NULL,       "Open CECM service",        CLI_SUPERVISOR},
    {"close",               "c",    _cecm_cli_close,                    NULL,       "Close CECM service",       CLI_SUPERVISOR},
    {"cecfct",              "cf",   _cecm_cli_set_cec_fct,              NULL,       "Set CEC function ctrl",    CLI_SUPERVISOR},
    {"devinfo",             "di",   _cecm_cli_get_dev_info_by_la,       NULL,       "Get dev info by LA",       CLI_SUPERVISOR},
    {"compid",              "ci",   _cecm_cli_get_comp_id_by_la,        NULL,       "Get comp ID by LA",        CLI_SUPERVISOR},
    {"startdd",             "sdd",  _cecm_cli_start_dev_discovery,      NULL,       "Start device discovery",   CLI_SUPERVISOR},
    {"stopdd",              "tdd",  _cecm_cli_stop_dev_discovery,       NULL,       "Stop device discovery",    CLI_SUPERVISOR},
    {"tvla",                "tvla", _cecm_cli_find_tv_la,               NULL,       "Find TV LA",               CLI_SUPERVISOR},    
    {"ping",                "p",    _cecm_cli_ping_dev,                 NULL,       "Ping a dev",               CLI_SUPERVISOR},
    {"vndrid",              "vid",  _cecm_cli_give_vndr_id,             NULL,       "Give device vendor ID",    CLI_SUPERVISOR},
    {"stby",                "s",    _cecm_cli_set_standby,              NULL,       "Set standby",              CLI_SUPERVISOR},
    {"reqactsrc",           "ras",  _cecm_cli_req_act_src,              NULL,       "Request active source",    CLI_SUPERVISOR},
    {"routingchg",          "rc",   _cecm_cli_routing_change,           NULL,       "Routing change",           CLI_SUPERVISOR},
    {"strmpath",            "sp",   _cecm_cli_set_stream_path,          NULL,       "Set stream path",          CLI_SUPERVISOR},
    {"cecver",              "cv",   _cecm_cli_get_cec_version,          NULL,       "Get CEC ver",              CLI_SUPERVISOR},
    {"givephyaddr",         "gpa",   _cecm_cli_give_physical_address,   NULL,       "Give physical address",    CLI_SUPERVISOR},
    {"rptphyaddr",          "rpa",   _cecm_cli_report_physical_address, NULL,       "Report physical address",  CLI_SUPERVISOR},
    {"getmenulang",         "gml",  _cecm_cli_get_menu_lang,            NULL,       "Get menu lang",            CLI_SUPERVISOR},
    {"setmenulang",         "sml",  _cecm_cli_set_menu_lang,            NULL,       "Set menu lang",            CLI_SUPERVISOR},
    {"decksts",             "ds",   _cecm_cli_give_deck_status,         NULL,       "Give deck status",         CLI_SUPERVISOR},
    {"deckctrl",            "dc",   _cecm_cli_set_deck_ctrl,            NULL,       "Set deck ctrl",            CLI_SUPERVISOR},
    {"deckplay",            "dp",   _cecm_cli_set_deck_play,            NULL,       "Set deck play",            CLI_SUPERVISOR},
    {"osdname",             "osd",  _cecm_cli_give_osd_name,            NULL,       "Give OSD name",            CLI_SUPERVISOR},
    {"menureq",             "mr",   _cecm_cli_set_menu_request,         NULL,       "Set menu request",         CLI_SUPERVISOR},
    {"usrctrl",             "uc",   _cecm_cli_set_user_ctrl,            NULL,       "Set user ctrl",            CLI_SUPERVISOR},
    {"pwrondev",            "pod",  _cecm_cli_power_on_dev,             NULL,       "Power on dev",             CLI_SUPERVISOR},
    {"givedevpwrsts",       "gdps", _cecm_cli_give_dev_pwr_sts,         NULL,       "Give dev power status",    CLI_SUPERVISOR},
    {"setaudmodereq",       "samr", _cecm_cli_set_sys_aud_mode_req,     NULL,       "Set sys aud mode req",     CLI_SUPERVISOR},
    {"audsts",              "as",   _cecm_cli_give_audio_status,        NULL,       "Req amp vol & mute sts",   CLI_SUPERVISOR},
    {"givesysaudmode",      "gsam", _cecm_cli_give_sys_aud_mode_sts,    NULL,       "Req sys aud mode sts",     CLI_SUPERVISOR},
    {"reqauddescriptor",    "rad",  _cecm_cli_request_audio_descriptor, NULL,       "Req aud descriptor",       CLI_SUPERVISOR},
    {"reqinitarc",          "reqia",_cecm_cli_request_init_arc,         NULL,       "Req init arc",             CLI_SUPERVISOR},
    {"reqterminatearc",     "reqta",_cecm_cli_request_terminate_arc,    NULL,       "Req terminate arc",        CLI_SUPERVISOR},
    {"repinitarc",          "repia",_cecm_cli_report_init_arc,          NULL,       "Rep init arc",             CLI_SUPERVISOR},
    {"repterminatearc",     "repta",_cecm_cli_report_terminate_arc,     NULL,       "Rep terminate arc",        CLI_SUPERVISOR},
    {"setaudrate",          "sar",  _cecm_cli_set_aud_rate,             NULL,       "Set aud rate",             CLI_SUPERVISOR},
    {"cleartimer",          "ct",   _cecm_cli_clear_timer,              NULL,       "Clear timer",              CLI_SUPERVISOR},
    {"settimer",            "st",   _cecm_cli_set_timer,                NULL,       "Set timer",                CLI_SUPERVISOR},
    {"settimerprogtitle",   "stpt", _cecm_cli_set_timer_prog_title,     NULL,       "Set timer program title",  CLI_SUPERVISOR},
    {"gettunersts",         "gts",  _cecm_cli_give_tuner_status,        NULL,       "Get tuner status",         CLI_SUPERVISOR},
    {"selectsvc",           "ss",   _cecm_cli_select_service,           NULL,       "Select service",           CLI_SUPERVISOR},
    {"settunerstep",        "stsp", _cecm_cli_set_tuner_step,           NULL,       "Set tuner step",           CLI_SUPERVISOR},
    {"setrecon",            "sro",  _cecm_cli_set_record_on,            NULL,       "Set record on",            CLI_SUPERVISOR},
    {"setrecoff",           "srf",  _cecm_cli_set_record_off,           NULL,       "Set record off",           CLI_SUPERVISOR},
    {CLI_GET_DBG_LVL_STR,   NULL,   _cecm_cli_get_dbg_level,            NULL,       CLI_GET_DBG_LVL_HELP_STR,   CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,   _cecm_cli_set_dbg_level,            NULL,       CLI_SET_DBG_LVL_HELP_STR,   CLI_SUPERVISOR},
    #endif
    END_OF_CLI_CMD_TBL
};

/* CEC Manager root command table */
static CLI_EXEC_T at_cecm_root_cmd_tbl[] =
{
    {"cecm",     NULL,        NULL,       at_cecm_cmd_tbl,      "CEC Manager commands",   CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};
#endif

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
#if (defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST))

#ifdef CLI_LVL_ALL
static CHAR* _convert_opcode_to_str(CECM_OPCODE_T e_opcode);

static BOOL _is_cecm_svc_valid(HANDLE_T h_cecm_svc);

static VOID _cecm_nfy_fct(CECM_NFY_T* pt_nfy_data,
                          VOID*       pv_tag,
                          UINT32      ui4_data);

/*-----------------------------------------------------------------------------
 * Name: _cecm_nfy_fct
 *
 * Description: This is CEC Manager notify function.
 *
 * Inputs:  e_opcode        An opcode.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static CHAR* _convert_opcode_to_str(CECM_OPCODE_T e_opcode)
{
    switch (e_opcode)
    {
        case CECM_OPCODE_FEATURE_ABORT:
            x_sprintf(ac_str, "OPCODE_FEATURE_ABORT (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_IMG_VIEW_ON:
            x_sprintf(ac_str, "OPCODE_IMG_VIEW_ON (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_TUNER_STEP_INCR:
            x_sprintf(ac_str, "OPCODE_TUNER_STEP_INCR (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_TUNER_STEP_DECR:
            x_sprintf(ac_str, "OPCODE_TUNER_STEP_DECR (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_TUNER_DEV_STS:
            x_sprintf(ac_str, "OPCODE_TUNER_DEV_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_GIVE_TUNER_DEV_STS:
            x_sprintf(ac_str, "OPCODE_GIVE_TUNER_DEV_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_REC_ON:
            x_sprintf(ac_str, "OPCODE_REC_ON (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_REC_STS:
            x_sprintf(ac_str, "OPCODE_REC_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_REC_OFF:
            x_sprintf(ac_str, "OPCODE_REC_OFF (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_TXT_VIEW_ON:
            x_sprintf(ac_str, "OPCODE_TXT_VIEW_ON (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_REC_TV_SCREEN:
            x_sprintf(ac_str, "OPCODE_REC_TV_SCREEN (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_GIVE_DECK_STS:
            x_sprintf(ac_str, "OPCODE_GIVE_DECK_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_DECK_STS:
            x_sprintf(ac_str, "OPCODE_DECK_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SET_MENU_LANG:
            x_sprintf(ac_str, "OPCODE_SET_MENU_LANG (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_CLR_ANA_TIMER:
            x_sprintf(ac_str, "OPCODE_CLR_ANA_TIMER (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SET_ANA_TIMER:
            x_sprintf(ac_str, "OPCODE_SET_ANA_TIMER (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_TIMER_STS:
            x_sprintf(ac_str, "OPCODE_TIMER_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_STANDBY:
            x_sprintf(ac_str, "OPCODE_STANDBY (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_PLAY:
            x_sprintf(ac_str, "OPCODE_PLAY (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_DECK_CTRL:
            x_sprintf(ac_str, "OPCODE_DECK_CTRL (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_TIMER_CLRED_STS:
            x_sprintf(ac_str, "OPCODE_TIMER_CLRED_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_USER_CTRL_PRESSED:
            x_sprintf(ac_str, "OPCODE_USER_CTRL_PRESSED (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_USER_CTRL_RELEASED:
            x_sprintf(ac_str, "OPCODE_USER_CTRL_RELEASED (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_GIVE_OSD_NAME:
            x_sprintf(ac_str, "OPCODE_GIVE_OSD_NAME (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SET_OSD_NAME:
            x_sprintf(ac_str, "OPCODE_SET_OSD_NAME (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SET_OSD_STRING:
            x_sprintf(ac_str, "OPCODE_SET_OSD_STRING (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SET_TIMER_PROG_TITLE:
            x_sprintf(ac_str, "OPCODE_SET_TIMER_PROG_TITLE (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SYS_AUD_MOD_REQ:
            x_sprintf(ac_str, "OPCODE_SYS_AUD_MOD_REQ (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_GIVE_AUD_STS:
            x_sprintf(ac_str, "OPCODE_GIVE_AUD_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SET_SYS_AUD_MOD:
            x_sprintf(ac_str, "OPCODE_SET_SYS_AUD_MOD (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_REPORT_AUD_STS:
            x_sprintf(ac_str, "OPCODE_REPORT_AUD_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_GIVE_SYS_AUD_MOD_STS:
            x_sprintf(ac_str, "OPCODE_GIVE_SYS_AUD_MOD_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SYS_AUD_MOD_STS:
            x_sprintf(ac_str, "OPCODE_SYS_AUD_MOD_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_ROUTING_CHG:
            x_sprintf(ac_str, "OPCODE_ROUTING_CHG (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_ROUTING_INFO:
            x_sprintf(ac_str, "OPCODE_ROUTING_INFO (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_ACT_SRC:
            x_sprintf(ac_str, "OPCODE_ACT_SRC (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_GIVE_PHY_ADDR:
            x_sprintf(ac_str, "OPCODE_GIVE_PHY_ADDR (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_REPORT_PHY_ADDR:
            x_sprintf(ac_str, "OPCODE_REPORT_PHY_ADDR (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_REQ_ACT_SRC:
            x_sprintf(ac_str, "OPCODE_REQ_ACT_SRC (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SET_STREAM_PATH:
            x_sprintf(ac_str, "OPCODE_SET_STREAM_PATH (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_DEV_VNDR_ID:
            x_sprintf(ac_str, "OPCODE_DEV_VNDR_ID (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_VNDR_CMD:
            x_sprintf(ac_str, "OPCODE_VNDR_CMD (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_VNDR_REMOTE_BTN_DOWN:
            x_sprintf(ac_str, "OPCODE_VNDR_REMOTE_BTN_DOWN (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_VNDR_REMOTE_BTN_UP:
            x_sprintf(ac_str, "OPCODE_VNDR_REMOTE_BTN_UP (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_GIVE_DEV_VNDR_ID:
            x_sprintf(ac_str, "OPCODE_GIVE_DEV_VNDR_ID (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_MENU_REQ:
            x_sprintf(ac_str, "OPCODE_MENU_REQ (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_MENU_STS:
            x_sprintf(ac_str, "OPCODE_MENU_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_GIVE_DEV_PWR_STS:
            x_sprintf(ac_str, "OPCODE_GIVE_DEV_PWR_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_REPORT_PWR_STS:
            x_sprintf(ac_str, "OPCODE_REPORT_PWR_STS (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_GET_MENU_LANG:
            x_sprintf(ac_str, "OPCODE_GET_MENU_LANG (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SELECT_ANA_SVC:
            x_sprintf(ac_str, "OPCODE_SELECT_ANA_SVC (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SELECT_DIG_SVC:
            x_sprintf(ac_str, "OPCODE_SELECT_DIG_SVC (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SET_DIG_TIMER:
            x_sprintf(ac_str, "OPCODE_SET_DIG_TIMER (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_CLR_DIG_TIMER:
            x_sprintf(ac_str, "OPCODE_CLR_DIG_TIMER (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SET_AUD_RATE:
            x_sprintf(ac_str, "OPCODE_SET_AUD_RATE (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_INACT_SRC:
            x_sprintf(ac_str, "OPCODE_INACT_SRC (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_CEC_VER:
            x_sprintf(ac_str, "OPCODE_CEC_VER (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_GET_CEC_VER:
            x_sprintf(ac_str, "OPCODE_GET_CEC_VER (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_VNDR_CMD_WITH_ID:
            x_sprintf(ac_str, "OPCODE_VNDR_CMD_WITH_ID (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_CLR_EXT_TIMER:
            x_sprintf(ac_str, "OPCODE_CLR_EXT_TIMER (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_SET_EXT_TIMER:
            x_sprintf(ac_str, "OPCODE_SET_EXT_TIMER (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_ABORT:
            x_sprintf(ac_str, "OPCODE_ABORT (0x%x)", e_opcode);
            break;

        case CECM_OPCODE_NONE:
            x_sprintf(ac_str, "OPCODE_NONE (0x%x)", e_opcode);
            break;

        default:
            x_strcpy(ac_str, "Unknown opcode");
            break;
    }

    return (ac_str);
}


/*-----------------------------------------------------------------------------
 * Name: _is_cecm_svc_valid
 *
 * Description: This is CEC Manager notify function.
 *
 * Inputs:  h_cecm_svc      A CECM service handle.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static BOOL _is_cecm_svc_valid(HANDLE_T h_cecm_svc)
{
    return ((h_cecm_svc == NULL_HANDLE) ? FALSE : TRUE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_nfy_fct
 *
 * Description: This is CEC Manager notify function.
 *
 * Inputs:
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_nfy_fct(CECM_NFY_T* pt_nfy_data,
                          VOID*       pv_tag,
                          UINT32      ui4_data)
{
    CHAR        ac_osd_name[CECM_OSD_NAME_SIZE];

    x_dbg_stmt("\n------- cec_nfy (%s)", 
               (pt_nfy_data->e_nfy_type == CECM_NFY_TYPE_OPCODE) ? "opcode" : "msgcode");
    if (pt_nfy_data->e_brdcst_type == CECM_BRDCST_TYPE_NONE)
    {                                
        x_dbg_stmt("(none) -------\n\r");
    }
    else if (pt_nfy_data->e_brdcst_type == CECM_BRDCST_TYPE_DIRECT)
    {                                
        x_dbg_stmt("(directed) -------\n\r");
    }
    else if (pt_nfy_data->e_brdcst_type == CECM_BRDCST_TYPE_BRDCST)
    {                                
        x_dbg_stmt("(brdcasted) -------\n\r");
    }                
    x_dbg_stmt("sent_la = %d\n\r", pt_nfy_data->e_sent_la);
    x_dbg_stmt("sent_opcode = %s\n\r", _convert_opcode_to_str(pt_nfy_data->e_sent_opcode));
    x_dbg_stmt("===========================================\n\r");

    switch (pt_nfy_data->e_nfy_type)
    {
        case CECM_NFY_TYPE_OPCODE:
            x_dbg_stmt("opcode = %s\n\r", _convert_opcode_to_str(pt_nfy_data->u_code.e_opcode));

            switch (pt_nfy_data->u_code.e_opcode)
            {
                case CECM_OPCODE_ACT_SRC:
                    x_dbg_stmt("la = %d\n\r", pt_nfy_data->u_data.t_act_src.e_la);
                    x_dbg_stmt("comp_id = %d\n\r", pt_nfy_data->u_data.t_act_src.ui2_comp_id);
                    break;

                case CECM_OPCODE_INACT_SRC:                    
                case CECM_OPCODE_ROUTING_INFO:
                case CECM_OPCODE_SET_STREAM_PATH:
                    x_dbg_stmt("la = %d\n\r", pt_nfy_data->u_data.e_la);
                    break;

                case CECM_OPCODE_ROUTING_CHG:
                    x_dbg_stmt("orig_pa = 0x%x\n\r", pt_nfy_data->u_data.t_routing_chg_info.ui2_orig_pa);
                    x_dbg_stmt("new_pa = 0x%x\n\r", pt_nfy_data->u_data.t_routing_chg_info.ui2_new_pa);
                    break;
                
                case CECM_OPCODE_SET_OSD_NAME:
                    x_uc_w2s_to_ps((const UTF16_T*)&(pt_nfy_data->u_data.aw2_osd_name)[0],
                                   &ac_osd_name[0],
                                   x_uc_w2s_strlen((const UTF16_T*)&(pt_nfy_data->u_data.aw2_osd_name)[0]));
                    x_dbg_stmt("osd_name = %s\n\r", ac_osd_name);
                    break;

                case CECM_OPCODE_DEV_VNDR_ID:
                    x_dbg_stmt("dev_vndr_id = 0x%02x%02x%02x\n\r", pt_nfy_data->u_data.t_dev_vndr_id[0],
                                                             pt_nfy_data->u_data.t_dev_vndr_id[1],
                                                             pt_nfy_data->u_data.t_dev_vndr_id[2]);
                    break;

                case CECM_OPCODE_REPORT_PWR_STS:
                    x_dbg_stmt("pwr_sts = %d\n\r", pt_nfy_data->u_data.e_pwr_sts);
                    break;

                case CECM_OPCODE_SET_MENU_LANG:
                    x_dbg_stmt("lang = 0x%02x 0x%02x 0x%02x\n\r", pt_nfy_data->u_data.t_menu_lang[0],
                                                                  pt_nfy_data->u_data.t_menu_lang[1],
                                                                  pt_nfy_data->u_data.t_menu_lang[2]);
                    break;

                case CECM_OPCODE_CEC_VER:
                    x_dbg_stmt("cec_ver = 0x%x\n\r", pt_nfy_data->u_data.e_cec_ver);
                    break;

                case CECM_OPCODE_FEATURE_ABORT:
                    x_dbg_stmt("abort_reason = %d\n\r", pt_nfy_data->u_data.e_abort_reason);
                    break;

                case CECM_OPCODE_REPORT_PHY_ADDR:
                    x_dbg_stmt("phy_addr = 0x%x\n\r", pt_nfy_data->u_data.t_report_phy_addr.ui2_pa);
                    x_dbg_stmt("dev_type = 0x%x\n\r", pt_nfy_data->u_data.t_report_phy_addr.e_dev_type);
                    break;

                case CECM_OPCODE_MENU_STS:
                    x_dbg_stmt("menu_state = %d\n\r", pt_nfy_data->u_data.e_menu_req_state);
                    break;

                case CECM_OPCODE_SET_SYS_AUD_MOD:
                case CECM_OPCODE_SYS_AUD_MOD_STS:
                    x_dbg_stmt("sys_aud_sts = %s\n\r", pt_nfy_data->u_data.e_sys_aud_sts ? "ON" : "OFF");
                    break;

                case CECM_OPCODE_REPORT_AUD_STS:
                    x_dbg_stmt("aud_mute = %s, aud_volume = 0x%x\n\r",
                               (pt_nfy_data->u_data.ui1_aud_sts >> 7) ? "ON" : "OFF",
                               (pt_nfy_data->u_data.ui1_aud_sts & 0x7f));
                    break;

                case CECM_OPCODE_TIMER_CLRED_STS:
                    x_dbg_stmt("timer_clred_sts = 0x%x\n\r", pt_nfy_data->u_data.e_timer_clred_sts);
                    break;

                case CECM_OPCODE_TIMER_STS:
                    x_dbg_stmt("timer_sts (overlap) = 0x%x\n\r", pt_nfy_data->u_data.t_timer_sts.e_overlap);
                    x_dbg_stmt("timer_sts (prog_ind) = 0x%x\n\r", pt_nfy_data->u_data.t_timer_sts.e_prog_ind);
                    x_dbg_stmt("timer_sts (prog_info) = 0x%x\n\r", pt_nfy_data->u_data.t_timer_sts.e_prog_info);
                    x_dbg_stmt("timer_sts (not_prog_err_info) = 0x%x\n\r", pt_nfy_data->u_data.t_timer_sts.e_not_prog_err_info);
                    x_dbg_stmt("timer_sts (duration) = 0x%x\n\r", pt_nfy_data->u_data.t_timer_sts.ui2_duration);
                    break;

                case CECM_OPCODE_GIVE_OSD_NAME:
                case CECM_OPCODE_REQ_ACT_SRC:
                case CECM_OPCODE_IMG_VIEW_ON:
                case CECM_OPCODE_TXT_VIEW_ON:
                case CECM_OPCODE_GET_MENU_LANG:
                case CECM_OPCODE_GET_CEC_VER:
                case CECM_OPCODE_ABORT:
                    break;
                default :
                    break;
            }
            break;

        case CECM_NFY_TYPE_MSGCODE:
            switch (pt_nfy_data->u_code.e_msgcode)
            {
                case CECM_MSGCODE_ACK:
                    x_dbg_stmt("CECM_MSGCODE_ACK => ");
                    if (pt_nfy_data->u_data.e_ack_cond == CECM_ACK_COND_OK)
                    {
                        x_dbg_stmt("OK\n\r");
                    }
                    else if (pt_nfy_data->u_data.e_ack_cond == CECM_ACK_COND_NO_RESPONSE)
                    {
                        x_dbg_stmt("NO_RESPONSE\n\r");
                    }
                    else
                    {
                        x_dbg_stmt("UNKNOWN\n\r");
                    }
                    break;

                case CECM_MSGCODE_WAIT_FOR_OPCODE_TIME_OUT:
                    x_dbg_stmt("CECM_MSGCODE_WAIT_FOR_OPCODE_TIME_OUT\n\r");
                    break;

                case CECM_MSGCODE_DEV_LIST_NFY:
                    x_dbg_stmt("CECM_MSGCODE_DEV_LIST_NFY => ");
                    if (pt_nfy_data->u_data.t_dev_lst_nfy.e_dev_lst_sts == CECM_DEV_LIST_STS_NO_CHANGE)
                    {
                        x_dbg_stmt("NO_CHANGE ");
                    }
                    else if (pt_nfy_data->u_data.t_dev_lst_nfy.e_dev_lst_sts == CECM_DEV_LIST_STS_UPDATED)
                    {
                        x_dbg_stmt("UPDATED ");
                    }
                    else
                    {
                        x_dbg_stmt("UNKNOWN ");
                    }
                    
                    x_dbg_stmt("(%d->%d)\n\r", 
                               pt_nfy_data->u_data.t_dev_lst_nfy.e_start_la,
                               pt_nfy_data->u_data.t_dev_lst_nfy.e_stop_la);
                    break;

                case CECM_MSGCODE_ONE_TOUCH_PLAY:
                    x_dbg_stmt("CECM_MSGCODE_ONE_TOUCH_PLAY\n\r");
                    x_dbg_stmt("la = %d\n\r", pt_nfy_data->u_data.t_one_touch_play.e_la);
                    x_dbg_stmt("comp_id = %d\n\r", pt_nfy_data->u_data.t_one_touch_play.ui2_comp_id);
                    x_dbg_stmt("action = %d\n\r", pt_nfy_data->u_data.t_one_touch_play.e_action);
                    break;

                case CECM_MSGCODE_PWR_ON_DEV:
                    x_dbg_stmt("CECM_MSGCODE_PWR_ON_DEV (pa=0x%x)\n\r", pt_nfy_data->u_data.t_pwr_on_dev_sts.ui2_pa);
                        
                    switch (pt_nfy_data->u_data.t_pwr_on_dev_sts.e_pwr_sts)
                    {
                        case CECM_PWR_STS_ON:
                            x_dbg_stmt("pwr_sts = CECM_PWR_STS_ON\n\r");
                            break;

                        case CECM_PWR_STS_STANDBY:
                            x_dbg_stmt("pwr_sts = CECM_PWR_STS_STANDBY\n\r");
                            break;
                            
                        case CECM_PWR_STS_STANDBY_TO_ON:
                            x_dbg_stmt("pwr_sts = CECM_PWR_STS_STANDBY_TO_ON\n\r");
                            break;
                            
                        case CECM_PWR_STS_ON_THEN_ACT_SRC:
                            x_dbg_stmt("pwr_sts = CECM_PWR_STS_ON_THEN_ACT_SRC\n\r");
                            break;
                            
                        case CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_ACK_OK:
                            x_dbg_stmt("pwr_sts = CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_ACK_OK\n\r");
                            break;
                            
                        case CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_NO_ACK:
                            x_dbg_stmt("pwr_sts = CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_NO_ACK\n\r");
                            break;
                            
                        case CECM_PWR_STS_TIMEOUT:
                            x_dbg_stmt("pwr_sts = CECM_PWR_STS_TIMEOUT\n\r");
                            break;   

                        case CECM_PWR_STS_ERROR:
                            x_dbg_stmt("pwr_sts = CECM_PWR_STS_ERROR\n\r");
                            break;                                                                                                                                                                                                   
                        default :
                            break;
                    }
                    break;
                default :
                    break;
            }
            break;
        default :
            break;
        
    }

    x_dbg_stmt("\n\r");
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_open
 *
 * Description: This API opens a CECM service.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_open(INT32        i4_argc,
                            const CHAR** pps_argv)
{
    CECM_SET_CEC_FCT_PARAMS_T   t_set_cec_fct_params;

    /* cecm.o [port_num] */

    /* Check arguments */
    if (_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("CECM service had been opened\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.o [port_num]\n\r");
        x_dbg_stmt("  [port_num]: HMDI port number\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (x_cecm_open_svc(_cecm_nfy_fct, NULL, &h_cecm_svc) != CECMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Open a CECM service successfully (0x%x)\n\r", h_cecm_svc);

    /* Enable CEC function */
    t_set_cec_fct_params.b_enable = TRUE;
    t_set_cec_fct_params.ui1_hdmi_port_num = (UINT8)(x_strtoull(pps_argv[1], NULL, 0));

    if (x_cecm_set_cec_fct(h_cecm_svc,
                           t_set_cec_fct_params) != CECMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Enable CEC function\n\r");

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_close
 *
 * Description: This API closes a CECM service.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_close(INT32        i4_argc,
                             const CHAR** pps_argv)
{
    /* cecm.c */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (x_cecm_close_svc(h_cecm_svc) != CECMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    h_cecm_svc = NULL_HANDLE;

    x_dbg_stmt("Close a CECM service successfully (0x%x)\n\r", h_cecm_svc);

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_cec_fct
 *
 * Description: This API enables/disables CEC function.
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
static INT32 _cecm_cli_set_cec_fct(INT32        i4_argc,
                                   const CHAR** pps_argv)
{
    CECM_SET_CEC_FCT_PARAMS_T   t_set_cec_fct_params = {TRUE, 3};

    /* cecm.cf [ctrl] [port_num] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.cf [ctrl] [port_num]\n\r");
        x_dbg_stmt("  [ctrl]: 0=OFF, 1=ON\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    t_set_cec_fct_params.b_enable = (BOOL)(x_strtoull(pps_argv[1], NULL, 0));
    t_set_cec_fct_params.ui1_hdmi_port_num = (UINT8)(x_strtoull(pps_argv[2], NULL, 0));

    if (x_cecm_set_cec_fct(h_cecm_svc,
                           t_set_cec_fct_params) != CECMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("%s CEC function successfully\n\r", t_set_cec_fct_params.b_enable ? "Enable" : "Disable");

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_get_dev_info_by_la
 *
 * Description: This API gets device info of a logical device.
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
static INT32 _cecm_cli_get_dev_info_by_la(INT32        i4_argc,
                                          const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;
    CECM_DEV_T          t_dev_info;
    CECM_CTRL_T         t_ctrl;
    CHAR                ac_osd_name[CECM_OSD_NAME_SIZE];

    /* cecm.di [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.di [log_addr] [ctrl]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [ctrl]: 0=unsync, 1=sync\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    t_ctrl.b_sync = (BOOL)(x_strtoull(pps_argv[2], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_get_dev_info_by_la(h_cecm_svc,
                                      e_la,
                                      &t_ctrl,
                                      &t_dev_info) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }

        x_uc_w2s_to_ps((const UTF16_T*)&(t_dev_info.aw2_osd_name)[0],
                       &ac_osd_name[0],
                       x_uc_w2s_strlen((const UTF16_T*)&(t_dev_info.aw2_osd_name)));

        x_dbg_stmt("dev_state = %d\n\r", t_dev_info.e_dev_state);
        x_dbg_stmt("act_src   = %d\n\r", (t_dev_info.b_act_src ? TRUE : FALSE));
        x_dbg_stmt("log_addr  = %d\n\r", t_dev_info.e_la);
        x_dbg_stmt("phy_addr  = 0x%x\n\r", t_dev_info.t_report_phy_addr.ui2_pa);  
        x_dbg_stmt("dev_type  = 0x%x\n\r", t_dev_info.t_report_phy_addr.e_dev_type);  
        x_dbg_stmt("osd_name  = %s\n\r", ac_osd_name);
        x_dbg_stmt("vndr_id   = 0x%x%x%x\n\r", t_dev_info.t_dev_vndr_id[0],
                                               t_dev_info.t_dev_vndr_id[1],
                                               t_dev_info.t_dev_vndr_id[2]);
    }
    else
    {
        x_dbg_stmt("cecm.di [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_get_comp_id_by_la
 *
 * Description: This API gets component ID of a logical device.
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
static INT32 _cecm_cli_get_comp_id_by_la(INT32        i4_argc,
                                         const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;
    UINT16              ui2_comp_id;

    /* cecm.ci [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.ci [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_get_comp_id_by_la(h_cecm_svc,
                                     e_la,
                                     &ui2_comp_id) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }

        x_dbg_stmt("comp_id = %d\n\r", ui2_comp_id);
    }
    else
    {
        x_dbg_stmt("cecm.ci [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_start_dev_discovery
 *
 * Description: This API starts the device discovery.
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
static INT32 _cecm_cli_start_dev_discovery(INT32        i4_argc,
                                           const CHAR** pps_argv)
{
    INT32                   i4_return;
    CECM_DEV_DISCOVERY_T    t_dev_discvry;

    /* cecm.sdd [start_la] [stop_la] [itvl] [batch] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 5) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.sdd [start_la] [stop_la] [itvl] [batch]\n\r");
        x_dbg_stmt("  [start_la]: Start logical address\n\r");
        x_dbg_stmt("  [stop_la]: Stop logical address\n\r");
        x_dbg_stmt("  [itvl]: Device discovery interval in second\n\r");
        x_dbg_stmt("  [batch]: Batch polling\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    t_dev_discvry.e_start_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    t_dev_discvry.e_stop_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[2], NULL, 0));
    t_dev_discvry.ui4_itvl = (UINT32)(x_strtoull(pps_argv[3], NULL, 0));
    t_dev_discvry.b_batch_polling = (BOOL)(x_strtoull(pps_argv[4], NULL, 0));
    
    if ((t_dev_discvry.e_start_la > CECM_LOG_ADDR_TV) && (t_dev_discvry.e_start_la < CECM_LOG_ADDR_UNREGED_BRDCST) &&
        (t_dev_discvry.e_stop_la > CECM_LOG_ADDR_TV) && (t_dev_discvry.e_stop_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        i4_return = x_cecm_start_dev_discovery_ex(h_cecm_svc, &t_dev_discvry);
    
        if (i4_return != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.sdd [start_la] [stop_la] [itvl] [batch]\n\r");
        x_dbg_stmt("  [start_la]: Start logical address\n\r");
        x_dbg_stmt("  [stop_la]: Stop logical address\n\r");
        x_dbg_stmt("  [itvl]: Device discovery interval in second\n\r");
        x_dbg_stmt("  [batch]: Batch polling\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_stop_dev_discovery
 *
 * Description: This API stops the device discovery.
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
static INT32 _cecm_cli_stop_dev_discovery(INT32        i4_argc,
                                          const CHAR** pps_argv)
{
    INT32               i4_return;

    /* cecm.tdd */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_cecm_stop_dev_discovery(h_cecm_svc);

    if (i4_return != CECMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_find_tv_la
 *
 * Description: This API finds the TV logical address allocation.
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
static INT32 _cecm_cli_find_tv_la(INT32        i4_argc,
                                  const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;

    /* cecm.tvla */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = x_cecm_find_tv_la_alloc(h_cecm_svc);
    
    if (e_la == CECM_LOG_ADDR_MAX)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    else
    {   
        x_dbg_stmt("TV logical address allocation = %d\n\r", e_la);       
        return CLIR_OK;
    }        
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_ping_dev
 *
 * Description: This API pings a device.
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
static INT32 _cecm_cli_ping_dev(INT32        i4_argc,
                                const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T         e_la;
    BOOL                    b_sync;
    CECM_POLLING_INFO_T     t_polling_info;
    
    /* cecm.p [log_addr] [sync] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (((i4_argc != 2) && (i4_argc != 3)) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.p [log_addr] [sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [sync]: 0=unsync, 1=sync (valid when log_addr = 0)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if (e_la < CECM_LOG_ADDR_UNREGED_BRDCST)
    {
        b_sync = (BOOL)(x_strtoull(pps_argv[2], NULL, 0));

        if (x_cecm_set_polling_message(h_cecm_svc, e_la, b_sync, &t_polling_info) != CECMR_OK)
        {
            x_dbg_stmt("Polling operation is failed (%s)\n\r", (b_sync ? "sync" : "unsync"));
            return CLIR_CMD_EXEC_ERROR;
        }

        if (b_sync)
        {   
            x_dbg_stmt("ack_ok = %d\n\r", t_polling_info.b_ack_ok);                    
        } 
    }
    else
    {
        x_dbg_stmt("cecm.p [log_addr] [sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [sync]: 0=unsync, 1=sync (valid when log_addr = 0)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_give_vndr_id
 *
 * Description: This API requests the vendor ID from a device.
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
static INT32 _cecm_cli_give_vndr_id(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32               i4_return;
    CECM_LOG_ADDR_T     e_la;
    CEC_FRAME_INFO_T    t_frame_info;

    /* cecm.vid [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.vid [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        t_frame_info.ui1_init_addr = (UINT8)CECM_LOG_ADDR_TV;
        t_frame_info.ui1_dest_addr = (UINT8)e_la;
        t_frame_info.ui2_opcode = CECM_OPCODE_GIVE_DEV_VNDR_ID;
        t_frame_info.aui1_operand[0] = 0;
        t_frame_info.z_operand_size = 0;

        i4_return = cecm_ctrl_send_msg(h_cecm_svc, CECM_EVT_TYPE_NORMAL, &t_frame_info, NULL);

        if (i4_return != TRUE)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.vid [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_standby
 *
 * Description: This API turns device(s) into standby.
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
static INT32 _cecm_cli_set_standby(INT32        i4_argc,
                                   const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;
    BOOL                b_sync;
    BOOL                b_error = FALSE;

    /* cecm.s [log_addr] [sync] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (((i4_argc != 2) && (i4_argc != 3)) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.s [log_addr] [sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (0~13)\n\r");
        x_dbg_stmt("              (0 = all devices)\n\r");
        x_dbg_stmt("  [sync]: 0=unsync, 1=sync (valid when log_addr = 0)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    b_sync = (BOOL)(x_strtoull(pps_argv[2], NULL, 0));
            
    if (e_la == CECM_LOG_ADDR_TV)
    {
        if (i4_argc == 3)
        {
            if (x_cecm_set_standby_all(h_cecm_svc, b_sync) != CECMR_OK)
            {
                x_dbg_stmt("Broadcasted standby operation is failed (%s)\n\r", (b_sync ? "sync" : "unsync"));
                return CLIR_CMD_EXEC_ERROR;
            }
            else
            {
                x_dbg_stmt("Broadcasted standby operation is successful (%s)\n\r", (b_sync ? "sync" : "unsync"));
            }
        }
        else
        {
            b_error = TRUE;
        }
    }
    else if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (i4_argc == 2)
        {
            if (x_cecm_set_standby(h_cecm_svc, e_la) != CECMR_OK)
            {
                x_dbg_stmt("Directly standby operation is failed (%s)\n\r", (b_sync ? "sync" : "unsync"));
                return CLIR_CMD_EXEC_ERROR;
            }
        }
        else
        {
            b_error = TRUE;
        }
    }
    else
    {
        b_error = TRUE;
    }

    if (b_error)
    {
        x_dbg_stmt("cecm.s [log_addr] [sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (0~13)\n\r");
        x_dbg_stmt("              (0 = all devices)\n\r");
        x_dbg_stmt("  [sync]: 0=unsync, 1=sync (valid when log_addr = 0)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (e_la == CECM_LOG_ADDR_UNREGED_BRDCST)
    {
        x_dbg_stmt("Broadcasted standby operation is successful\n\r");
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_req_act_src
 *
 * Description: This API requests the active source.
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
static INT32 _cecm_cli_req_act_src(INT32        i4_argc,
                                   const CHAR** pps_argv)
{
    /* cecm.ras */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (x_cecm_request_active_source(h_cecm_svc) != CECMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_routing_change
 *
 * Description: This API issues a routing change.
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
static INT32 _cecm_cli_routing_change(INT32        i4_argc,
                                      const CHAR** pps_argv)
{
    /* cecm.rc [orig_comp_id] [new_comp_id] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.rc [orig_comp_id] [new_comp_id]\n\r");
        x_dbg_stmt("  [orig_comp_id]: orig HDMI comp ID\n\r");
        x_dbg_stmt("  [new_comp_id]: new HDMI comp ID\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (x_cecm_set_routing_change(h_cecm_svc,
                                  (UINT16)(x_strtoull(pps_argv[1], NULL, 0)),
                                  (UINT16)(x_strtoull(pps_argv[2], NULL, 0))) != CECMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_stream_path
 *
 * Description: This API sets stream path
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_set_stream_path(INT32        i4_argc,
                                       const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;

    /* cecm.sp [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.sp [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_set_stream_path(h_cecm_svc, e_la) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.sp [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_get_cec_version
 *
 * Description: This API gets cec version of a device.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_get_cec_version(INT32        i4_argc,
                                       const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;

    /* cecm.cv [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.cv [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_get_cec_version(h_cecm_svc, e_la) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.cv [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_give_physical_address
 *
 * Description: This API requests the physical address of a device.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_give_physical_address(INT32        i4_argc,
                                             const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;

    /* cecm.gpa [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.gpa [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_give_physical_address(h_cecm_svc, e_la) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.gpa [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_report_physical_address
 *
 * Description: This API reports the physical address of a device.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_report_physical_address(INT32        i4_argc,
                                               const CHAR** pps_argv)
{
    CECM_REPORT_PHY_ADDR_T  t_report_phy_addr;

    /* cecm.rpa [dev_type] [phy_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.rpa [dev_type] [pa_addr]\n\r");
        x_dbg_stmt("  [dev_type]: device type (0~5)\n\r");
        x_dbg_stmt("  [phy_addr]: physical address\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    t_report_phy_addr.e_dev_type = (CECM_DEV_TYPE_T)(x_strtoull(pps_argv[1], NULL, 0));
    t_report_phy_addr.ui2_pa = (UINT16)(x_strtoull(pps_argv[2], NULL, 0));

    if (t_report_phy_addr.e_dev_type <= CECM_DEV_TYPE_AUD_SYS)
    {
        if (x_cecm_report_physical_address(h_cecm_svc, &t_report_phy_addr) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.rpa [log_addr] [dev_type] [pa_addr]\n\r");
        x_dbg_stmt("  [dev_type]: device type (0~5)\n\r");
        x_dbg_stmt("  [phy_addr]: physical address\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_get_menu_lang
 *
 * Description: This API gets the menu language of a device.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_get_menu_lang(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    /* cecm.gml */

    CECM_LOG_ADDR_T     e_la;

    /* cecm.gml [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.gml [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_get_menu_lang(h_cecm_svc, e_la) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.gml [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_menu_lang
 *
 * Description: This API sets the menu language of a device.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_set_menu_lang(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    UINT8     aui1_lang[sizeof(ISO_639_LANG_T)];

    /* cecm.sml [lang] */

    /* Check arguments */
    if ((i4_argc != 4) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.sml [lang]\n\r");
        x_dbg_stmt("  [lang]: 3 ASCIIs of ISO 693-2\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    aui1_lang[0] = (UINT8)(x_strtoull(pps_argv[1], NULL, 0));
    aui1_lang[1] = (UINT8)(x_strtoull(pps_argv[2], NULL, 0));
    aui1_lang[2] = (UINT8)(x_strtoull(pps_argv[3], NULL, 0));
    aui1_lang[3] = 0;

    if (x_cecm_set_menu_lang(h_cecm_svc, (ISO_639_LANG_T*)(&aui1_lang[0])) != CECMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_give_deck_status
 *
 * Description: This API queries deck status.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_give_deck_status(INT32        i4_argc,
                                        const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;
    CECM_STS_REQ_T      e_sts_req;

    /* cecm.ds [log_addr] [sts_req] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.ds [log_addr] [sts_req]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [sts_req]: 1=On, 2=Off, 3=Once\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    e_sts_req = (CECM_STS_REQ_T)(x_strtoull(pps_argv[2], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_give_deck_status(h_cecm_svc, e_la, e_sts_req) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.ds [log_addr] [sts_req]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [sts_req]: 1=On, 2=Off, 3=Once\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_deck_ctrl
 *
 * Description: This API sets deck control.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_set_deck_ctrl(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T         e_la;
    CECM_DECK_CTRL_MOD_T    e_deck_ctrl_mod;

    /* cecm.dc [log_addr] [ctrl] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.ds [log_addr] [ctrl]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [ctrl]: 1=skip forward/wind\n\r");
        x_dbg_stmt("          2=skip reverse/rewind\n\r");
        x_dbg_stmt("          3=stop\n\r");
        x_dbg_stmt("          4=eject\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    e_deck_ctrl_mod = (CECM_DECK_CTRL_MOD_T)(x_strtoull(pps_argv[2], NULL, 0));

    if (((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST)) &&
        ((e_deck_ctrl_mod >= CECM_DECK_CTRL_MOD_SKIP_FWR_WIND) &&
        (e_deck_ctrl_mod <= CECM_DECK_CTRL_MOD_EJECT)))
    {
        if (x_cecm_set_deck_ctrl(h_cecm_svc, e_la, e_deck_ctrl_mod) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.ds [log_addr] [ctrl]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [ctrl]: 1=skip forward/wind\n\r");
        x_dbg_stmt("          2=skip reverse/rewind\n\r");
        x_dbg_stmt("          3=stop\n\r");
        x_dbg_stmt("          4=eject\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_deck_play
 *
 * Description: This API sets deck play.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_set_deck_play(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;
    CECM_PLAY_MOD_T     e_play_mod;

    /* cecm.dp [log_addr] [ctrl] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.dp [log_addr] [ctrl]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [ctrl]: 0x24=play forward\n\r");
        x_dbg_stmt("          0x20=play reverse\n\r");
        x_dbg_stmt("          0x25=play still\n\r");
        x_dbg_stmt("          0x05=fast forward min speed\n\r");
        x_dbg_stmt("          0x06=fast forward med speed\n\r");
        x_dbg_stmt("          0x07=fast forward max speed\n\r");
        x_dbg_stmt("          0x09=fast reverse min speed\n\r");
        x_dbg_stmt("          0x0a=fast reverse med speed\n\r");
        x_dbg_stmt("          0x0b=fast reverse max speed\n\r");
        x_dbg_stmt("          0x15=slow forward min speed\n\r");
        x_dbg_stmt("          0x16=slow forward med speed\n\r");
        x_dbg_stmt("          0x17=slow forward max speed\n\r");
        x_dbg_stmt("          0x19=slow reverse min speed\n\r");
        x_dbg_stmt("          0x1a=slow reverse med speed\n\r");
        x_dbg_stmt("          0x1b=slow reverse max speed\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    e_play_mod = (CECM_PLAY_MOD_T)(x_strtoull(pps_argv[2], NULL, 0));

    if (((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST)) &&
        ((e_play_mod == CECM_PLAY_MOD_PLAY_FWR) ||
         (e_play_mod == CECM_PLAY_MOD_PLAY_RVR) ||
         (e_play_mod == CECM_PLAY_MOD_PLAY_STILL) ||
         (e_play_mod == CECM_PLAY_MOD_FAST_FWR_MIN_SPEED) ||
         (e_play_mod == CECM_PLAY_MOD_FAST_FWR_MED_SPEED) ||
         (e_play_mod == CECM_PLAY_MOD_FAST_FWR_MAX_SPEED) ||
         (e_play_mod == CECM_PLAY_MOD_FAST_RVR_MIN_SPEED) ||
         (e_play_mod == CECM_PLAY_MOD_FAST_RVR_MED_SPEED) ||
         (e_play_mod == CECM_PLAY_MOD_FAST_RVR_MAX_SPEED) ||
         (e_play_mod == CECM_PLAY_MOD_SLOW_FWR_MIN_SPEED) ||
         (e_play_mod == CECM_PLAY_MOD_SLOW_FWR_MED_SPEED) ||
         (e_play_mod == CECM_PLAY_MOD_SLOW_FWR_MAX_SPEED) ||
         (e_play_mod == CECM_PLAY_MOD_SLOW_RVR_MIN_SPEED) ||
         (e_play_mod == CECM_PLAY_MOD_SLOW_RVR_MED_SPEED) ||
         (e_play_mod == CECM_PLAY_MOD_SLOW_RVR_MAX_SPEED)))
    {
        if (x_cecm_set_deck_play(h_cecm_svc, e_la, e_play_mod) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.dp [log_addr] [ctrl]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [ctrl]: 0x24=play forward\n\r");
        x_dbg_stmt("          0x20=play reverse\n\r");
        x_dbg_stmt("          0x25=play still\n\r");
        x_dbg_stmt("          0x05=fast forward min speed\n\r");
        x_dbg_stmt("          0x06=fast forward med speed\n\r");
        x_dbg_stmt("          0x07=fast forward max speed\n\r");
        x_dbg_stmt("          0x09=fast reverse min speed\n\r");
        x_dbg_stmt("          0x0a=fast reverse med speed\n\r");
        x_dbg_stmt("          0x0b=fast reverse max speed\n\r");
        x_dbg_stmt("          0x15=slow forward min speed\n\r");
        x_dbg_stmt("          0x16=slow forward med speed\n\r");
        x_dbg_stmt("          0x17=slow forward max speed\n\r");
        x_dbg_stmt("          0x19=slow reverse min speed\n\r");
        x_dbg_stmt("          0x1a=slow reverse med speed\n\r");
        x_dbg_stmt("          0x1b=slow reverse max speed\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_give_osd_name
 *
 * Description: This API queries device's OSD name.
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
static INT32 _cecm_cli_give_osd_name(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;

    /* cecm.osd [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.osd [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_give_osd_name(h_cecm_svc, e_la) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.osd [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_menu_request
 *
 * Description: This API sets menu request.
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
static INT32 _cecm_cli_set_menu_request(INT32        i4_argc,
                                        const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T         e_la;
    CECM_MENU_REQ_STATE_T   e_menu_req_type;

    /* cecm.mr [log_addr] [ctrl] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.mr [log_addr] [ctrl]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [ctrl]: 0=activate\n\r");
        x_dbg_stmt("          1=deactivate\n\r");
        x_dbg_stmt("          2=query\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    e_menu_req_type = (CECM_MENU_REQ_STATE_T)(x_strtoull(pps_argv[2], NULL, 0));

    if (((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST)) &&
        (e_menu_req_type <= CECM_MENU_REQ_STATE_QUERY))
    {
        if (x_cecm_set_menu_request(h_cecm_svc, e_la, e_menu_req_type) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.mr [log_addr] [ctrl]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [ctrl]: 0=activate\n\r");
        x_dbg_stmt("          1=deactivate\n\r");
        x_dbg_stmt("          2=query\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_user_ctrl
 *
 * Description: This API sets menu control (pressed and released).
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
static INT32 _cecm_cli_set_user_ctrl(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T         e_la;
    CECM_USER_CTRL_T        e_usr_ctrl;

    /* cecm.uc [log_addr] [ctrl] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.uc [log_addr] [ctrl]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [ctrl]: user control code\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    e_usr_ctrl = (CECM_USER_CTRL_T)(x_strtoull(pps_argv[2], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_set_user_ctrl_pressed(h_cecm_svc, e_la, e_usr_ctrl) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }

        x_thread_delay(500);
        x_dbg_stmt("\n\r");

        if (x_cecm_set_user_ctrl_released(h_cecm_svc, e_la) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.uc [log_addr] [ctrl]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [ctrl]: user control code\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_power_on_dev
 *
 * Description: This API powers on a device.
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
static INT32 _cecm_cli_power_on_dev(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T         e_la;

    /* cecm.pod [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.pod [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_power_on_dev(h_cecm_svc, e_la) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.pod [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_give_dev_pwr_sts
 *
 * Description: This API queries the current power status of a target device.
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
static INT32 _cecm_cli_give_dev_pwr_sts(INT32        i4_argc,
                                        const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T         e_la;
    BOOL                    b_sync;
    CECM_PWR_STS_T          e_pwr_sts;

    /* cecm.gdps [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.gdps [log_addr] [sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [sync]: 0=unsync, 1=sync\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        b_sync = (BOOL)(x_strtoull(pps_argv[2], NULL, 0));

        if (x_cecm_give_device_power_status(h_cecm_svc, e_la, b_sync, &e_pwr_sts) != CECMR_OK)
        {
            x_dbg_stmt("Give device power status operation is failed (%s)\n\r", (b_sync ? "sync" : "unsync"));
            return CLIR_CMD_EXEC_ERROR;
        }

        if (b_sync)
        {   
            x_dbg_stmt("pwr_sts = %d\n\r", e_pwr_sts);                    
        } 
    }
    else
    {
        x_dbg_stmt("cecm.pod [log_addr] [sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [sync]: 0=unsync, 1=sync\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_sys_aud_mode_req
 *
 * Description: This API sets system audio mode request.
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
static INT32 _cecm_cli_set_sys_aud_mode_req(INT32        i4_argc,
                                            const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T         e_la;
    BOOL                    b_sync;
    CECM_SYS_AUD_CTRL_T     t_sys_aud_ctrl;
    CECM_SYS_AUD_STS_T      e_sys_aud_sts;  

    /* cecm.samr [log_addr] [ctrl] [phy_addr] [sync] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 5) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.samr [log_addr] [ctrl] [phy_addr] [sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [ctrl]: 0=OFF, 1=ON\n\r");
        x_dbg_stmt("  [phy_addr]: physical address\n\r");
        x_dbg_stmt("  [sync]: 0=unsync, 1=sync\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    t_sys_aud_ctrl.e_aud_ctrl = (CECM_SYS_AUD_STS_T)(x_strtoull(pps_argv[2], NULL, 0));
    t_sys_aud_ctrl.ui2_pa = (UINT16)(x_strtoull(pps_argv[3], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        b_sync = (BOOL)(x_strtoull(pps_argv[4], NULL, 0));

        if (x_cecm_set_system_audio_mode_request_ex(h_cecm_svc, &t_sys_aud_ctrl, b_sync, &e_sys_aud_sts) != CECMR_OK)
        {
            x_dbg_stmt("Set system audio mode request operation is failed (%s)\n\r", (b_sync ? "sync" : "unsync"));
            return CLIR_CMD_EXEC_ERROR;
        }
        
        if (b_sync)
        {   
            x_dbg_stmt("sys_aud_sts = %s\n\r",
                       (e_sys_aud_sts == CECM_SYS_AUD_STS_OFF) ? "OFF" : "ON");                    
        }            
    }
    else
    {
        x_dbg_stmt("cecm.samr [log_addr] [ctrl] [phy_addr] [sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [ctrl]: 0=OFF, 1=ON\n\r");
        x_dbg_stmt("  [phy_addr]: physical address\n\r");
        x_dbg_stmt("  [sync]: 0=unsync, 1=sync\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_give_audio_status
 *
 * Description: This API requests an amplifier to send its volume and mute
 *              ststus.
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
static INT32 _cecm_cli_give_audio_status(INT32        i4_argc,
                                         const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;
    BOOL                b_sync;
    UINT8               ui1_aud_sts;

    /* cecm.as [log_addr] [sync] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.as [log_addr] [sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [sync]: 0=unsync, 1=sync\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        b_sync = (BOOL)(x_strtoull(pps_argv[2], NULL, 0));

        if (x_cecm_give_audio_status(h_cecm_svc, e_la, b_sync, &ui1_aud_sts) != CECMR_OK)
        {
            x_dbg_stmt("Give audio status operation is failed (%s)\n\r", (b_sync ? "sync" : "unsync"));
            return CLIR_CMD_EXEC_ERROR;
        }
        
        if (b_sync)
        {   
            x_dbg_stmt("aud_mute = %s, aud_volume = 0x%x\n\r",
                       (ui1_aud_sts >> 7) ? "ON" : "OFF",
                       (ui1_aud_sts & 0x7f));                    
        }            
    }
    else
    {
        x_dbg_stmt("cecm.as [log_addr] [sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [sync]: 0=unsync, 1=sync\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_give_sys_aud_mode_sts
 *
 * Description: This API requests the status of the systam audio mode.
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
static INT32 _cecm_cli_give_sys_aud_mode_sts(INT32        i4_argc,
                                             const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;

    /* cecm.gsam [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.gsam [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_give_system_audio_mode_status(h_cecm_svc, e_la) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.gsam [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}



/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_request_audio_descriptor
 *
 * Description: This API requests the audio descriptor.
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
INT32 _cecm_cli_request_audio_descriptor(INT32 i4_argc, const CHAR** pps_argv)

{
    CECM_LOG_ADDR_T     e_la;
    BOOL                b_sync;

    /* cecm.gsam [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.rad [log_addr] [b_sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [b_sync]: TRUE or FALSE\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    b_sync = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[2], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_request_init_arc(h_cecm_svc, e_la, b_sync) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.riarc [log_addr] [b_sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [b_sync]: TRUE or FALSE\n\r");

        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_request_init_arc
 *
 * Description: This API requests initial audio return channel.
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
INT32 _cecm_cli_request_init_arc(INT32 i4_argc, const CHAR** pps_argv)

{
    CECM_LOG_ADDR_T     e_la;
    BOOL                b_sync;

    /* cecm.gsam [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.riarc [log_addr] [b_sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [b_sync]: TRUE or FALSE\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    b_sync = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[2], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_request_init_arc(h_cecm_svc, e_la, b_sync) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.riarc [log_addr] [b_sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [b_sync]: TRUE or FALSE\n\r");

        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_request_terminate_arc
 *
 * Description: This API requests terminalte audio return channel.
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
INT32 _cecm_cli_request_terminate_arc(INT32 i4_argc, const CHAR** pps_argv)

{
    CECM_LOG_ADDR_T     e_la;
    BOOL                b_sync;

    /* cecm.gsam [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.rtarc [log_addr] [b_sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [b_sync]: TRUE or FALSE\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    b_sync = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[2], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_request_terminate_arc(h_cecm_svc, e_la, b_sync) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.rtarc [log_addr] [b_sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [b_sync]: TRUE or FALSE\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_report_init_arc
 *
 * Description: This API report initial audio return channel.
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
INT32 _cecm_cli_report_init_arc(INT32 i4_argc, const CHAR** pps_argv)

{
    CECM_LOG_ADDR_T     e_la;
    BOOL                b_sync;

    /* cecm.gsam [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.repia [log_addr] [b_sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [b_sync]: TRUE or FALSE\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    b_sync = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[2], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_report_init_arc(h_cecm_svc, e_la, b_sync) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.repia [log_addr] [b_sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [b_sync]: TRUE or FALSE\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_request_terminate_arc
 *
 * Description: This API report terminate audio return channel.
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
INT32 _cecm_cli_report_terminate_arc(INT32 i4_argc, const CHAR** pps_argv)

{
    CECM_LOG_ADDR_T     e_la;
    BOOL                b_sync;

    /* cecm.gsam [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.repta [log_addr] [b_sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [b_sync]: TRUE or FALSE\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    b_sync = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[2], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_report_terminate_arc(h_cecm_svc, e_la, b_sync) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.repta [log_addr] [b_sync]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [b_sync]: TRUE or FALSE\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_aud_rate
 *
 * Description: This API is used to control audio rate from source device.
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
static INT32 _cecm_cli_set_aud_rate(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;
    CECM_AUD_RATE_T     e_aud_rate;

    /* cecm.sar [log_addr] [aud_rate] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.sar [log_addr] [aud_rate]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [aud_rate]: audio rate\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    e_aud_rate = (CECM_AUD_RATE_T)(x_strtoull(pps_argv[2], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_set_audio_rate(h_cecm_svc, e_la, e_aud_rate) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.sar [log_addr] [aud_rate]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [aud_rate]: audio rate\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_clear_timer
 *
 * Description: This API is used to clear a timer block of a device.
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
static INT32 _cecm_cli_clear_timer(INT32        i4_argc,
                                   const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;
    CECM_TIMER_INFO_T   t_timer_info;

    /* cecm.ct [log_addr] [dev_type] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.ct [log_addr] [dev_type]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [dev_type]: 0=analog\n\r");
        x_dbg_stmt("              1=digital\n\r");
        x_dbg_stmt("              2=external\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    t_timer_info.e_dev = (CECM_TIMER_DEV_T)(x_strtoull(pps_argv[2], NULL, 0));
    t_timer_info.ui1_day_of_month = 1;
    t_timer_info.ui1_month_of_year = 1; /* January */
    t_timer_info.ui2_start_time = 0x0100; /* 1:00am */
    t_timer_info.ui2_duration = 0x0300; /* 3 hours */
    t_timer_info.ui1_rec_sequence = 0x1; /* Sunday */

    switch (t_timer_info.e_dev)
    {
        case CECM_TIMER_DEV_ANA:
            t_timer_info.u.t_ana_info.e_ana_brdcst_type = CECM_ANA_BRDCST_CABLE;
            t_timer_info.u.t_ana_info.ui2_freq = 0x10;
            t_timer_info.u.t_ana_info.ui1_brdcst_sys = 0x0;
            break;

        case CECM_TIMER_DEV_DIG:
            x_memset(&t_timer_info.u.aui1_dig_info[0], 0, 7);
            break;

        case CECM_TIMER_DEV_EXT:
            t_timer_info.u.t_ext_info.e_src_specfer = (CECM_TIMER_EXT_SRC_SPECFER_T)0x5; /* external physical address */
            t_timer_info.u.t_ext_info.ui2_phy_addr_or_plug = 0x1000;
            break;

        default:
            x_dbg_stmt("cecm.ct [log_addr] [dev_type]\n\r");
            x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
            x_dbg_stmt("  [dev_type]: 0=analog\n\r");
            x_dbg_stmt("              1=digital\n\r");
            x_dbg_stmt("              2=external\n\r");
            return CLIR_INV_CMD_USAGE;
    }

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_clear_timer(h_cecm_svc, e_la, &t_timer_info) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.ct [log_addr] [dev_type]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [dev_type]: 0=analog\n\r");
        x_dbg_stmt("              1=digital\n\r");
        x_dbg_stmt("              2=external\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_timer
 *
 * Description: This API is used to set a single timer block of a device.
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
static INT32 _cecm_cli_set_timer(INT32        i4_argc,
                                 const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;
    CECM_TIMER_INFO_T   t_timer_info;

    /* cecm.st [log_addr] [dev_type] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.st [log_addr] [dev_type]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [dev_type]: 0=analog\n\r");
        x_dbg_stmt("              1=digital\n\r");
        x_dbg_stmt("              2=external\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    t_timer_info.e_dev = (CECM_TIMER_DEV_T)(x_strtoull(pps_argv[2], NULL, 0));
    t_timer_info.ui1_day_of_month = 1;
    t_timer_info.ui1_month_of_year = 1; /* January */
    t_timer_info.ui2_start_time = 0x0100; /* 1:00am */
    t_timer_info.ui2_duration = 0x0300; /* 3 hours */
    t_timer_info.ui1_rec_sequence = 0x1; /* Sunday */

    switch (t_timer_info.e_dev)
    {
        case CECM_TIMER_DEV_ANA:
            t_timer_info.u.t_ana_info.e_ana_brdcst_type = CECM_ANA_BRDCST_CABLE;
            t_timer_info.u.t_ana_info.ui2_freq = 0x10;
            t_timer_info.u.t_ana_info.ui1_brdcst_sys = 0x0;
            break;

        case CECM_TIMER_DEV_DIG:
            x_memset(&t_timer_info.u.aui1_dig_info[0], 0, 7);
            break;

        case CECM_TIMER_DEV_EXT:
            t_timer_info.u.t_ext_info.e_src_specfer = (CECM_TIMER_EXT_SRC_SPECFER_T)0x5; /* external physical address */
            t_timer_info.u.t_ext_info.ui2_phy_addr_or_plug = 0x1000;
            break;

        default:
            x_dbg_stmt("cecm.st [log_addr] [dev_type]\n\r");
            x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
            x_dbg_stmt("  [dev_type]: 0=analog\n\r");
            x_dbg_stmt("              1=digital\n\r");
            x_dbg_stmt("              2=external\n\r");
            return CLIR_INV_CMD_USAGE;
    }

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_set_timer(h_cecm_svc, e_la, &t_timer_info) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.st [log_addr] [dev_type]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [dev_type]: 0=analog\n\r");
        x_dbg_stmt("              1=digital\n\r");
        x_dbg_stmt("              2=external\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_timer_prog_title
 *
 * Description: This API is used to set the name of a program associated with a
 *              timer block.
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
static INT32 _cecm_cli_set_timer_prog_title(INT32        i4_argc,
                                            const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;
    CECM_PROG_TITLE_T   t_prog_title_str;
    CHAR                ac_buf[] = "Alec";

    /* cecm.stpt [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.stpt [log_addr] \n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    x_memset(&t_prog_title_str, 0, CECM_PROG_TITLE_STRING_SIZE);
    x_uc_ps_to_w2s((const CHAR*)ac_buf, (UTF16_T*)&t_prog_title_str, sizeof(ac_buf));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_set_timer_prog_title(h_cecm_svc, e_la, &t_prog_title_str) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.stpt [log_addr] \n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_give_tuner_status
 *
 * Description: This API is used to request the status of a tuner.
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
static INT32 _cecm_cli_give_tuner_status(INT32        i4_argc,
                                         const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;
    CECM_STS_REQ_T      e_sts_req;

    /* cecm.gts [log_addr] [sts_req] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.gts [log_addr] [dev_type]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [sts_req]: 1=On, 2=Off, 3=Once\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    e_sts_req = (CECM_STS_REQ_T)(x_strtoull(pps_argv[2], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_give_tuner_status(h_cecm_svc, e_la, e_sts_req) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.gts [log_addr] [dev_type]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [sts_req]: 1=On, 2=Off, 3=Once\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_select_service
 *
 * Description: This API is used to directly select an analog TV, digital TV,
 *              radio or data broadcast service.
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
static INT32 _cecm_cli_select_service(INT32        i4_argc,
                                      const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T         e_la;
    CECM_TUNER_SVC_INFO_T   t_tuner_svc_info;

    /* cecm.ss [log_addr] [tuner_svc] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.ss [log_addr] [tuner_svc]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [tuner_svc]: 0=analog\n\r");
        x_dbg_stmt("               1=digital\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    t_tuner_svc_info.e_svc = (CECM_TUNER_SVC_T)(x_strtoull(pps_argv[2], NULL, 0));

    switch (t_tuner_svc_info.e_svc)
    {
        case CECM_TUNER_SVC_ANA:
            t_tuner_svc_info.u.t_ana_info.e_ana_brdcst_type = CECM_ANA_BRDCST_CABLE;
            t_tuner_svc_info.u.t_ana_info.ui2_freq = 0x10;
            t_tuner_svc_info.u.t_ana_info.ui1_brdcst_sys = 0x0;
            break;

        case CECM_TUNER_SVC_DIG:
            t_tuner_svc_info.u.t_dig_info.e_dig_id_method = CECM_DIG_ID_METHOD_CHANNEL;
            t_tuner_svc_info.u.t_dig_info.e_dig_brdcst_sys = CECM_DIG_BRDCST_ATSC_CABLE;
            x_memset(&t_tuner_svc_info.u.t_dig_info.aui1_svc_id[0], 0, 6);
            t_tuner_svc_info.u.t_dig_info.aui1_svc_id[5] = 0x1;
            break;

        default:
            x_dbg_stmt("cecm.ss [log_addr] [tuner_svc]\n\r");
            x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
            x_dbg_stmt("  [tuner_svc]: 0=analog\n\r");
            x_dbg_stmt("               1=digital\n\r");
            return CLIR_INV_CMD_USAGE;
    }

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_select_service(h_cecm_svc, e_la, &t_tuner_svc_info) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.ss [log_addr] [tuner_svc]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [tuner_svc]: 0=analog\n\r");
        x_dbg_stmt("               1=digital\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_tuner_step
 *
 * Description: This API is used to tune to next lowest/highest service in a
 *              tuner's service list.
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
static INT32 _cecm_cli_set_tuner_step(INT32        i4_argc,
                                      const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T     e_la;
    CECM_TUNER_STEP_T   e_tuner_step;

    /* cecm.sts [log_addr] [tuner_step] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.stsp [log_addr] [tuner_step]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [tuner_step]: 0=decrement\n\r");
        x_dbg_stmt("                1=increment\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    e_tuner_step = (CECM_TUNER_STEP_T)(x_strtoull(pps_argv[2], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_set_tuner_step(h_cecm_svc, e_la, e_tuner_step) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.stsp [log_addr] [tuner_step]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [tuner_step]: 0=decrement\n\r");
        x_dbg_stmt("                1=increment\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_record_on
 *
 * Description: This API attempts to record the specific source.
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
static INT32 _cecm_cli_set_record_on(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T         e_la;
    CECM_REC_SRC_T          t_rec_src;

    /* cecm.sro [log_addr] [tuner_svc] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.sro [log_addr] [tuner_svc]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [rec_src]: 1=own source\n\r");
        x_dbg_stmt("             2=digital sevice\n\r");
        x_dbg_stmt("             3=analog sevice\n\r");
        x_dbg_stmt("             4=external plug\n\r");
        x_dbg_stmt("             5=external physical address\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));
    t_rec_src.e_rec_src_type = (CECM_REC_SRC_TYPE_T)(x_strtoull(pps_argv[2], NULL, 0));

    switch (t_rec_src.e_rec_src_type)
    {
        case CECM_REC_SRC_TYPE_OWN_SRC:
            break;

        case CECM_REC_SRC_TYPE_DIG_SVC:
            t_rec_src.u.t_dig_svc.e_dig_id_method = CECM_DIG_ID_METHOD_CHANNEL;
            t_rec_src.u.t_dig_svc.e_dig_brdcst_sys = CECM_DIG_BRDCST_ATSC_CABLE;
            x_memset(&t_rec_src.u.t_dig_svc.aui1_svc_id[0], 0, 6);
            t_rec_src.u.t_dig_svc.aui1_svc_id[5] = 0x1;
            break;

        case CECM_REC_SRC_TYPE_ANA_SVC:
            t_rec_src.u.t_ana_svc.e_ana_brdcst_type = CECM_ANA_BRDCST_CABLE;
            t_rec_src.u.t_ana_svc.ui2_freq = 0x10;
            t_rec_src.u.t_ana_svc.ui1_brdcst_sys = 0x0;
            break;

        case CECM_REC_SRC_TYPE_EXT_PLUG:
            t_rec_src.u.ui1_plug = 0x2;
            break;

        case CECM_REC_SRC_TYPE_EXT_PHY_ADDR:
            t_rec_src.u.ui2_phy_addr = 0x2000;
            break;

        default:
            x_dbg_stmt("cecm.sro [log_addr] [tuner_svc]\n\r");
            x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
            x_dbg_stmt("  [rec_src]: 0=own source\n\r");
            x_dbg_stmt("             1=digital sevice\n\r");
            x_dbg_stmt("             2=analog sevice\n\r");
            x_dbg_stmt("             3=external plug\n\r");
            x_dbg_stmt("             4=external physical address\n\r");
            return CLIR_INV_CMD_USAGE;
    }

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_set_record_on(h_cecm_svc, e_la, &t_rec_src) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.sro [log_addr] [tuner_svc]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        x_dbg_stmt("  [rec_src]: 0=own source\n\r");
        x_dbg_stmt("             1=digital sevice\n\r");
        x_dbg_stmt("             2=analog sevice\n\r");
        x_dbg_stmt("             3=external plug\n\r");
        x_dbg_stmt("             4=external physical address\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_record_off
 *
 * Description: This API requests a device to stop a recording.
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
static INT32 _cecm_cli_set_record_off(INT32        i4_argc,
                                      const CHAR** pps_argv)
{
    CECM_LOG_ADDR_T         e_la;

    /* cecm.srf [log_addr] */

    /* Check arguments */
    if (!_is_cecm_svc_valid(h_cecm_svc))
    {
        x_dbg_stmt("Please open a CECM service first\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.srf [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    e_la = (CECM_LOG_ADDR_T)(x_strtoull(pps_argv[1], NULL, 0));

    if ((e_la > CECM_LOG_ADDR_TV) && (e_la < CECM_LOG_ADDR_UNREGED_BRDCST))
    {
        if (x_cecm_set_record_off(h_cecm_svc, e_la) != CECMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("cecm.srf [log_addr]\n\r");
        x_dbg_stmt("  [log_addr]: logical address (1~13)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}
#endif

#ifdef CLI_LVL_GUEST

/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_show_dev_list
 *
 * Description: This API shows the device list.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_show_dev_list(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    /* cecm.dl */

    cecm_ctrl_show_dev_list();

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_show_svc_list
 *
 * Description: This API shows the service list.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_show_svc_list(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    /* cecm.sl */

    cecm_ctrl_show_svc_list();

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_query_cec_fct
 *
 * Description: This API query the status of CEC function control.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_query_cec_fct(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    /* cecm.sts */

    cecm_ctrl_query_cec_fct();

    return CLIR_OK;
}
#endif

#ifdef CLI_LVL_ALL

/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_show_dd_setting
 *
 * Description: This API shows the current device discovery setting.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_show_dd_setting(INT32        i4_argc,
                                       const CHAR** pps_argv)
{
    /* cecm.dd */

    cecm_ctrl_show_dd_setting();

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_get_dbg_level
 *
 * Description: This API gets the current setting of CEC Manager debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_cli_get_dbg_level(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;

    /* cecm.gdl */

    i4_return = x_cli_show_dbg_level(cecm_get_dbg_level());

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_cli_set_dbg_level
 *
 * Description: This API sets the debug level of CEC Manager.
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
static INT32 _cecm_cli_set_dbg_level(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    /* cecm.sdl [level] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cecm.sdl [level]\n\r");
        x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (cecm_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }
        else
        {
            i4_return = x_cli_show_dbg_level(cecm_get_dbg_level());
        }
    }
    else
    {
        x_dbg_stmt("cecm.sdl [level]\n\r");
        x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return i4_return;
}
#endif 

#endif  


/*-----------------------------------------------------------------------------
 * Name: cecm_cli_init
 *
 * Description: This API initializes CEC Manager CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                     Routine successful.
 *          CECMR_CLI_ERROR              CLI operation is failed.
 ----------------------------------------------------------------------------*/
INT32 cecm_cli_init(VOID)
{
#if (defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST))

    INT32       i4_return;

    /* Attach CEC Manager CLI command table to CLI */
    i4_return = x_cli_attach_cmd_tbl(at_cecm_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);

    if (i4_return != CLIR_OK)
    {
        return CECMR_CLI_ERROR;
    }
    return CECMR_OK;
#else
    return CECMR_OK;
#endif
}

