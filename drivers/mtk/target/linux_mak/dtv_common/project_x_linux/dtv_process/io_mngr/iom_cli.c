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
 * $RCSfile: iom_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/4 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: a37d432aa942f00af51dbc0b805f7237 $
 *
 * Description:
 *         This file implements CLI command table for IO Manager.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/common.h"
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "util/x_lnk_list.h"
#include "res_mngr/rm.h"
#include "res_mngr/drv/x_irrc_btn_def.h"
#include "cli/cli.h"
#include "cli/x_cli.h"
#include "io_mngr/_iom.h"
#include "io_mngr/iom.h"
#include "io_mngr/iom_inp_dptr.h"
#include "io_mngr/iom_key_gen.h"
#include "io_mngr/x_iom.h"

#if IOM_SUPPORT_IRRC_DISABLE_KEYS
#include "io_mngr/x_iom.h"
#include "io_mngr/iom_lock.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define IRRC_EVT_REC_NUM            ((UINT32)   512)
#define IRRC_LOWER_BOUND            ((UINT32)   120)
#define IRRC_HIGHER_BOUND           ((UINT32)   3000)

#define IRRC_CTRL_B                 0x02
 
typedef struct _IRRC_BTN_STR_MAP_T
{
    UINT32      ui4_btn_code;
    CHAR*       ps_btn_str;
}   IRRC_BTN_STR_MAP_T;

typedef struct _IRRC_EVT_REC_T
{
    IRRC_BTN_STR_MAP_T  t_btn_str_map;
    UINT32              ui4_loop;
    UINT32              ui4_1st_itvl;
    UINT32              ui4_2nd_itvl;
}   IRRC_EVT_REC_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#if defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST) || defined(CLI_STRESS_MODE)
#if 0
static HANDLE_T             h_fpd_dev = NULL_HANDLE;
static HANDLE_T             h_ind_power_dev = NULL_HANDLE;
#endif
static IRRC_EVT_REC_T       t_irrc_evt_rec[IRRC_EVT_REC_NUM];
static UINT32               ui4_irrc_evt_rec_num = 0;
static UINT32               ui4_irrc_lower_bound = IRRC_LOWER_BOUND;
static UINT32               ui4_irrc_upper_bound = IRRC_HIGHER_BOUND;

static IRRC_BTN_STR_MAP_T   at_btn_to_str[] =
{
    {BTN_DIGIT_0,           "0"},               
    {BTN_DIGIT_1,           "1"},       
    {BTN_DIGIT_2,           "2"},       
    {BTN_DIGIT_3,           "3"},
    {BTN_DIGIT_4,           "4"},
    {BTN_DIGIT_5,           "5"},
    {BTN_DIGIT_6,           "6"},
    {BTN_DIGIT_7,           "7"},
    {BTN_DIGIT_8,           "8"},
    {BTN_DIGIT_9,           "9"},
    {BTN_DIGIT_PLUS_5,      "5+"},
    {BTN_DIGIT_PLUS_10,     "10+"},
    {BTN_DIGIT_PLUS_20,     "20+"},
    {BTN_DIGIT_PLUS_100,    "100+"},
    {BTN_DIGIT_DOT,         "."},
    {BTN_CURSOR_LEFT,       "left"},
    {BTN_CURSOR_RIGHT,      "right"},
    {BTN_CURSOR_UP,         "up"},
    {BTN_CURSOR_DOWN,       "down"},
    {BTN_SELECT,            "select"},
    {BTN_EXIT,              "exit"},
    {BTN_CLEAR,             "clear"},   
    {BTN_ENTER,             "enter"},      
    {BTN_PRG_UP,            "ch+"},
    {BTN_PRG_DOWN,          "ch-"},
    {BTN_PREV_PRG,          "pre_prg"},
    {BTN_CH_LIST,           "ch_list"},
    {BTN_FAV_CH,            "fav_ch"},
    {BTN_FAV_CH_UP,         "fav_ch+"},
    {BTN_FAV_CH_DOWN,       "fav_ch-"},
    {BTN_FAVORITE,          "fav"},        
    {BTN_PIP,               "pip"},
    {BTN_PIP_CH_UP,         "pip_ch+"},
    {BTN_PIP_CH_DOWN,       "pip_ch-"},
    {BTN_PIP_SIZE,          "pip_size"}, 
    {BTN_PIP_POS,           "pip_pos"},
    {BTN_PIP_INPUT_SRC,     "pip_inp_src"},
    {BTN_POP,               "pop"},
    {BTN_PIP_POP,           "pip_pop"}, 
    {BTN_SWAP,              "swap"},
    {BTN_FREEZE,            "freeze"},
    {BTN_ZOOM,              "zoom"},
    {BTN_ZOOM_IN,           "zoom_in"},        
    {BTN_ZOOM_OUT,          "zoom_out"},
    {BTN_ASPECT,            "aspect"},
    {BTN_P_EFFECT,          "p_effect"},
    {BTN_PRG_INFO,          "prg_info"}, 
    {BTN_PRG_DETAIL,        "prg_detail"}, 
    {BTN_CC,                "cc"},
    {BTN_ADD_ERASE,         "add_erase"},
    {BTN_SLEEP,             "sleep"},
    {BTN_TIMER,             "timer"}, 
    {BTN_AUTO_SYNC,         "auto_sync"},
    {BTN_INPUT_SRC,         "inp_src"},
    {BTN_TV,                "tv"},
    {BTN_TV_ANA,            "tv_ana"},
    {BTN_TV_DIG,            "tv_dig"},
    {BTN_TUNER_TER_ANA,     "ter_ana"}, 
    {BTN_TUNER_CAB_ANA,     "cab_ana"},
    {BTN_TUNER_SAT_ANA,     "sat_ana"},
    {BTN_TUNER_TER_DIG,     "ter_dig"},
    {BTN_TUNER_CAB_DIG,     "cab_dig"},        
    {BTN_TUNER_SAT_DIG,     "sat_dig"},
    {BTN_COMPOSITE,         "composite"},
    {BTN_COMPOSITE_1,       "composite1"},     
    {BTN_COMPOSITE_2,       "composite2"},     
    {BTN_COMPOSITE_3,       "composite3"},     
    {BTN_COMPOSITE_4,       "composite4"},         
    {BTN_S_VIDEO,           "s_video"},
    {BTN_S_VIDEO_1,         "s_video1"},
    {BTN_S_VIDEO_2,         "s_video2"},    
    {BTN_S_VIDEO_3,         "s_video3"},
    {BTN_S_VIDEO_4,         "s_video4"},        
    {BTN_COMPONENT,         "component"}, 
    {BTN_COMPONENT_1,       "component1"}, 
    {BTN_COMPONENT_2,       "component2"}, 
    {BTN_COMPONENT_3,       "component3"}, 
    {BTN_COMPONENT_4,       "component4"},                 
    {BTN_VGA,               "vga"},
    {BTN_DVI,               "dvi"},
    {BTN_DVI_1,             "dvi1"},
    {BTN_DVI_2,             "dvi2"},
    {BTN_DVI_3,             "dvi3"},
    {BTN_DVI_4,             "dvi4"},                
    {BTN_HDMI,              "hdmi"},
    {BTN_HDMI_1,            "hdmi1"},    
    {BTN_HDMI_2,            "hdmi2"},    
    {BTN_HDMI_3,            "hdmi3"},
    {BTN_HDMI_4,            "hdmi4"}, 
    {BTN_HDMI_5,            "hdmi5"},           
    {BTN_SCART,             "scart"}, 
    {BTN_SCART_1,           "scart1"}, 
    {BTN_SCART_2,           "scart2"}, 
    {BTN_SCART_3,           "scart3"}, 
    {BTN_SCART_4,           "scart4"},                 
    {BTN_DVD,               "dvd"},
    {BTN_ILINK,             "ilink"}, 
    {BTN_POWER_SAVING ,     "power_saving"},    
    {BTN_DTV_RADIO_ATV,     "dtv_radio_atv"},    
    {BTN_VOL_UP,            "vol+"},
    {BTN_VOL_DOWN,          "vol-"},
    {BTN_MUTE,              "mute"},
    {BTN_MTS,               "mts"},
    {BTN_AUDIO,             "audio"},
    {BTN_MTS_AUDIO,         "mts_audio"},
    {BTN_PIP_AUDIO,         "pip_audio"},        
    {BTN_S_EFFECT,          "s_effect"},
    {BTN_TOOLS,             "tools"},
    {BTN_TTX,               "ttx"},
    {BTN_INDEX,             "index"},
    {BTN_REVEAL,            "reveal"},
    {BTN_MIX,               "mix"},
    {BTN_SUBPAGE,           "subpage"},
    {BTN_SIZE,              "size"},
    {BTN_WIDE_MODE,         "wide"},
    {BTN_POWER,             "power"},
    {BTN_MENU,              "menu"},
    {BTN_EPG,               "epg"},
    {BTN_MEM_CARD,          "mem"},
    {BTN_TEXT,              "text"},
    {BTN_POWER_ON ,         "power_on"},
    {BTN_POWER_OFF,         "power_off"},            
    {BTN_PLAY,              "play"},
    {BTN_PAUSE,             "pause"},
    {BTN_STOP,              "stop"},
    {BTN_RESUME,            "resume"},
    {BTN_RECORD,            "record"},
    {BTN_NEXT,              "next"},
    {BTN_PREV,              "prev"},        
    {BTN_FF,                "ff"},
    {BTN_FR,                "fr"},
    {BTN_SF,                "sf"},
    {BTN_SR,                "sr"},
    {BTN_STEPF,             "stepf"},
    {BTN_STEPR,             "stepr"},
    {BTN_PLAY_PAUSE,        "play_pause"},
    {BTN_PAUSE_STEP,        "pause_step"},
    {BTN_STOP_RESUME,       "stop_resume"},
    {BTN_PLAY_ENTER,        "play_enter"},
    {BTN_EJECT,             "eject"},
    {BTN_TITLE_MENU,        "title_menu"},
    {BTN_ROOT_MENU,         "root_menu"},
    {BTN_GOTO,              "goto"},
    {BTN_MARK,              "mark"},
    {BTN_DIGEST,            "digest"},
    {BTN_PROGRAM,           "program"},
    {BTN_VRMT,              "vrmt"},        
    {BTN_PBC,               "pbc"},
    {BTN_REPEAT,            "repeat"},
    {BTN_REPEAT_A,          "repeat_a"},
    {BTN_REPEAT_B,          "repeat_b"},
    {BTN_REPEAT_A_B,        "repeat_a_b"},
    {BTN_SUB_TITLE,         "subtitle"},
    {BTN_ANGLE,             "angle"},
    {BTN_DISPLAY,           "display"},
    {BTN_RANDOM,            "random"},
    {BTN_PAL_NTSC,          "pal_ntsc"},
    {BTN_SURROUND,          "surround"},
    {BTN_EQUALIZER,         "equalizer"},
    {BTN_HOME_DLIST,        "home_dlist"},
    {BTN_NEXT_DLIST,        "next_dlist"},
    {BTN_PREV_DLIST,        "prev_dlist"},
    {BTN_KARAOKE,           "karaoke"},
    {BTN_AUD_KEY_PLUS,      "aud_key+"},
    {BTN_AUD_KEY_MINUS,     "aud_key-"},
    {BTN_AUD_KEY_RESUME,    "aud_key_resume"},        
    {BTN_ECHO_PLUS,         "echo+"},
    {BTN_ECHO_MINUS,        "echo-"},
    {BTN_VOCAL_ASSIST,      "vocal"},
    {BTN_MENU_PBC,          "menu_pbc"},
    {BTN_TITLE_PBC,         "title_pbc"},
    {BTN_RED,               "red"},
    {BTN_GREEN,             "green"},
    {BTN_YELLOW,            "yellow"},
    {BTN_BLUE,              "blue"},
    {BTN_FACTORY_MODE_1,    "factory1"},
    {BTN_FACTORY_MODE_2,    "factory2"},
    {BTN_FACTORY_MODE_3,    "factory3"},
    {BTN_FACTORY_MODE_4,    "factory4"},
    {BTN_FACTORY_MODE_5,    "factory5"},
    {BTN_FACTORY_MODE_6,    "factory6"},
    {BTN_FACTORY_MODE_7,    "factory7"},
    {BTN_FACTORY_MODE_8,    "factory8"},
    {BTN_FUNCTION_1,        "f1"},
    {BTN_FUNCTION_2,        "f2"},
    {BTN_FUNCTION_3,        "f3"},
    {BTN_FUNCTION_4,        "f4"},
    {BTN_FUNCTION_5,        "f5"},
    {BTN_FUNCTION_6,        "f6"},
    {BTN_FUNCTION_7,        "f7"},
    {BTN_FUNCTION_8,        "f8"},            
    {BTN_FUNCTION_9,        "f9"},
    {BTN_FUNCTION_10,       "f10"},
    {BTN_FUNCTION_11,       "f11"},
    {BTN_FUNCTION_12,       "f12"},    
    {BTN_CUSTOM_1,          "custom1"},
    {BTN_CUSTOM_2,          "custom2"},
    {BTN_CUSTOM_3,          "custom3"},
    {BTN_CUSTOM_4,          "custom4"},
    {BTN_CUSTOM_5,          "custom5"},
    {BTN_CUSTOM_6,          "custom6"},
    {BTN_CUSTOM_7,          "custom7"},
    {BTN_CUSTOM_8,          "custom8"},
    {BTN_CUSTOM_9,          "custom9"},
    {BTN_CUSTOM_10,         "custom10"},
    {BTN_CUSTOM_11,         "custom11"},
    {BTN_CUSTOM_12,         "custom12"},
    {BTN_CUSTOM_13,         "custom13"},
    {BTN_CUSTOM_14,         "custom14"},
    {BTN_CUSTOM_15,         "custom15"},
    {BTN_CUSTOM_16,         "custom16"},            
    {BTN_CUSTOM_17,         "custom17"},
    {BTN_CUSTOM_18,         "custom18"},
    {BTN_CUSTOM_19,         "custom19"},
    {BTN_CUSTOM_20,         "custom20"},       
    {BTN_CUSTOM_21,         "custom21"},
    {BTN_CUSTOM_22,         "custom22"},
    {BTN_CUSTOM_23,         "custom23"},
    {BTN_CUSTOM_24,         "custom24"},
    {BTN_CUSTOM_25,         "custom25"},
    {BTN_CUSTOM_26,         "custom26"},
    {BTN_CUSTOM_27,         "custom27"},
    {BTN_CUSTOM_28,         "custom28"},
    {BTN_CUSTOM_29,         "custom29"},
    {BTN_CUSTOM_30,         "custom30"},
    {BTN_CUSTOM_31,         "custom31"},
    {BTN_CUSTOM_32,         "custom32"},
    {BTN_CUSTOM_33,         "custom33"},
    {BTN_CUSTOM_34,         "custom34"},
    {BTN_CUSTOM_35,         "custom35"},
    {BTN_CUSTOM_36,         "custom36"},            
    {BTN_CUSTOM_37,         "custom37"},
    {BTN_CUSTOM_38,         "custom38"},
    {BTN_CUSTOM_39,         "custom39"},
    {BTN_CUSTOM_40,         "custom40"},      
    {BTN_CUSTOM_41,         "custom41"},         
    {BTN_CUSTOM_42,         "custom42"},         
    {BTN_CUSTOM_43,         "custom43"},         
    {BTN_CUSTOM_44,         "custom44"},         
    {BTN_CUSTOM_45,         "custom45"},         
    {BTN_CUSTOM_46,         "custom46"},         
    {BTN_CUSTOM_47,         "custom47"},         
    {BTN_CUSTOM_48,         "custom48"},         
    {0,                     NULL}
};   

#ifdef IOM_CUSTOM_KEY_REPEAT_INTERVAL_SUPPORT
static IOM_CUST_KEY_REP_ITVL_ITEM_T at_cust_key_rep_itvl[MAX_CUSTOM_KEY_REPEAT_ITVL_SETTING_NUM] = {{0}};
static UINT32                       ui4_cust_key_num = 0;
static UINT16                       ui2_unit = 0;
#endif
     
static INT32 _iom_cli_send_evt(INT32 i4_argc, const CHAR** pps_argv);
#ifdef CLI_LVL_ALL
static INT32 _iom_cli_send_evt_single(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _iom_cli_show_info(INT32 i4_argc, const CHAR** pps_argv);
#endif
static INT32 _iom_cli_set_key_mon(INT32 i4_argc, const CHAR** pps_argv);
#ifdef CLI_LVL_ALL
static INT32 _iom_cli_set_key_gen(INT32 i4_argc, const CHAR** pps_argv);
#endif
static INT32 _iom_cli_set_irrc_inp(INT32 i4_argc, const CHAR** pps_argv);
#ifdef CLI_LVL_ALL
static INT32 _iom_cli_show_log(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _iom_cli_clear_log(INT32 i4_argc, const CHAR** pps_argv);
#if 0
static INT32 _iom_cli_show_fpd(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _iom_cli_clear_fpd(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _iom_cli_set_ind_power(INT32 i4_argc, const CHAR** pps_argv);
#endif
static INT32 _iom_cli_set_auto_sleep_timer(INT32 i4_argc, const CHAR** pps_argv);
#endif
static INT32 _iom_cli_prog_irrc(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _iom_cli_channel_change(INT32 i4_argc, const CHAR** pps_argv);
#ifdef CLI_LVL_ALL
static INT32 _iom_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _iom_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
#if IOM_SUPPORT_IRRC_DISABLE_KEYS
static INT32 _iom_cli_irrc_get_handle_by_fltridx(UINT8 ui1_filtr_idx, HANDLE_T  *h_dev);
static INT32 _iom_cli_set_disabledkeys(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _iom_cli_get_disabledkeys_info(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _iom_cli_set_glb_disabledkeys(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _iom_cli_get_glb_disabledkeys(INT32 i4_argc, const CHAR** pps_argv);
#endif
#ifdef IOM_CUSTOM_KEY_REPEAT_INTERVAL_SUPPORT
static INT32 _iom_cli_set_custom_key_rep_itvl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _iom_cli_get_custom_key_rep_itvl(INT32 i4_argc, const CHAR** pps_argv);
#endif
static INT32 _iom_cli_test_factory_mode(INT32 i4_argc, const CHAR** pps_argv);
#endif

/* IO Manager command table */
static CLI_EXEC_T at_iom_cmd_tbl[] =
{
    {"sendevt",             "se",       _iom_cli_send_evt,              NULL,       "Send an IRRC event to IOM",        CLI_GUEST},
#ifdef CLI_LVL_ALL
    {"sendevtsingle",       "ses",      _iom_cli_send_evt_single,       NULL,       "Send an IRRC single event to IOM", CLI_SUPERVISOR},
    {"info",                "info",     _iom_cli_show_info,             NULL,       "Show internal info",               CLI_SUPERVISOR},
#endif
    {"keymon",              "km",       _iom_cli_set_key_mon,           NULL,       "Enable/disable key monitor",       CLI_GUEST},
#ifdef CLI_LVL_ALL
    {"keygen",              "kg",       _iom_cli_set_key_gen,           NULL,       "Enable/disable key generator",     CLI_GUEST},
#endif
    {"irrcinp",             "ii",       _iom_cli_set_irrc_inp,          NULL,       "Enable/disable IRRC input",        CLI_GUEST},
#ifdef CLI_LVL_ALL
    #if IOM_SUPPORT_IRRC_DISABLE_KEYS
    {"irrcdiskey",          "dk",      _iom_cli_set_disabledkeys,       NULL,       "Set disable IRRC keys",            CLI_SUPERVISOR},
    {"irrcdiskeyinfo",      "dki",     _iom_cli_get_disabledkeys_info,  NULL,       "Get disable IRRC keys info",       CLI_SUPERVISOR},
    {"irrcglbdiskeys",      "gdk",     _iom_cli_set_glb_disabledkeys,   NULL,       "Set global disable IRRC keys",     CLI_SUPERVISOR},
    {"irrcglbdiskeysinfo",  "gdki",    _iom_cli_get_glb_disabledkeys,   NULL,       "Get global disable IRRC keys info",CLI_SUPERVISOR},
    #endif
    {"showlog",             "sl",       _iom_cli_show_log,              NULL,       "Show key generator log",           CLI_SUPERVISOR},
    {"clearlog",            "cl",       _iom_cli_clear_log,             NULL,       "Clear key generator log",          CLI_SUPERVISOR},
#if 0
    {"sfpd",                "sfpd",     _iom_cli_show_fpd,              NULL,       "Show characters to FPD",           CLI_SUPERVISOR},
    {"cfpd",                "cfpd",     _iom_cli_clear_fpd,             NULL,       "Clear characters to FPD",          CLI_SUPERVISOR},
    {"spi",                 "spi",      _iom_cli_set_ind_power,         NULL,       "Set power indicator",              CLI_SUPERVISOR},
#endif
#ifdef IOM_CUSTOM_KEY_REPEAT_INTERVAL_SUPPORT
    {"custkeyrepitvl",      "ri",      _iom_cli_set_custom_key_rep_itvl,NULL,       "Set custom key repeat interval",     CLI_SUPERVISOR},
    {"custkeyrepitvlinfo",  "rii",     _iom_cli_get_custom_key_rep_itvl,NULL,       "Get custom key repeat interval info",CLI_SUPERVISOR},
#endif
    {"autosleep",           "as",       _iom_cli_set_auto_sleep_timer,  NULL,       "Configure auto sleep timer",       CLI_SUPERVISOR},
#endif
    {"ch_xng",              "cx",       _iom_cli_channel_change,        NULL,       "Channel change: cx [#iteration #delay_lower_bound #delay_upper_bound [-s major1.minor1 major2.minor2]]", CLI_GUEST},
    {"irrc",                "irrc",     _iom_cli_prog_irrc,             NULL,       "Program IRRC events to IOM",       CLI_GUEST},    
#ifdef CLI_LVL_ALL
    {CLI_GET_DBG_LVL_STR,   NULL,       _iom_cli_get_dbg_level,         NULL,       CLI_GET_DBG_LVL_HELP_STR,           CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,       _iom_cli_set_dbg_level,         NULL,       CLI_SET_DBG_LVL_HELP_STR,           CLI_SUPERVISOR},
    {"testfac",             "tf",       _iom_cli_test_factory_mode,     NULL,       "Test factory mode funtion",        CLI_GUEST},
#endif
    END_OF_CLI_CMD_TBL
};

/* IO Manager root command table */
static CLI_EXEC_T at_iom_root_cmd_tbl[] =
{
    {"iom",     NULL,        NULL,       at_iom_cmd_tbl,      "IO Manager commands",   CLI_GUEST},
    END_OF_CLI_CMD_TBL
};
#endif

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
#if defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST) || defined(CLI_STRESS_MODE)
static INT32 _ascii_to_int(const CHAR* ps);

static UINT32 _gen_rand(VOID);

static VOID _send_btn(UINT32   ui4_evt_code,
                      VOID*    pv_tag);

static VOID _list_btn_str_code(VOID);

static VOID _show_irrc_evt_rec(VOID);

static INT32 _start_sending_irrc_evt(UINT32 ui4_loop);

static INT32 _rec_irrc_evt(const CHAR* ps_irrc_str,
                           UINT32      ui4_loop,
                           UINT32      ui4_1st_itvl,
                           UINT32      ui4_2nd_itvl);

#ifdef CLI_LVL_ALL
static VOID _auto_sleep_cb(IOM_NFY_COND_T e_nfy_cond,  
                           VOID*          pv_nfy_tag);
#endif

extern VOID _dfb_process_irrc_evt(INP_EVT_T* pt_inp_evt);

/*-----------------------------------------------------------------------------
 * Name: _atoi
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _ascii_to_int(const CHAR* ps)
{
    INT32  i4_total;

    i4_total = 0;
    while ( *ps != 0 )
    {
        if ( *ps >= '0' && *ps <= '9' )
        {
            /* Accumulate digit */
            i4_total = 10 * i4_total + (*ps - '0');
            ps++; /* Get next char */
        }
        else /* Not a digit char */
        {
            return (-1);
        }
    }

    return i4_total;
}


/*-----------------------------------------------------------------------------
 * Name: _gen_rand
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static UINT32 _gen_rand(VOID)
{
    static UINT32 ui4_mul = 97;
    static UINT32 ui4_add = 33;
    static UINT32 ui4_seed = 12345;
    
    ui4_seed = ui4_seed * ui4_mul + ui4_add;
    
    return ui4_seed;
}


/*-----------------------------------------------------------------------------
 * Name: _send_btn
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static VOID _send_btn(UINT32   ui4_evt_code,
                      VOID*    pv_tag)
{
    INP_EVT_T   t_inp_evt;

    t_inp_evt.e_evt_type = INP_EVT_TYPE_IRRC;
    t_inp_evt.pv_tag = NULL;
    t_inp_evt.u.s_irrc.ui4_evt_code = ui4_evt_code;
    x_memset(&t_inp_evt.u.s_irrc.t_data, 0 , sizeof(IRRC_DATA_T));

    t_inp_evt.e_cond = IOM_NFY_COND_BTN_DOWN;
    _dfb_process_irrc_evt(&t_inp_evt);

    t_inp_evt.e_cond = IOM_NFY_COND_BTN_UP;
    _dfb_process_irrc_evt(&t_inp_evt);
}


/*-----------------------------------------------------------------------------
 * Name: _list_btn_str_code
 *
 * Description: This API lists all available event string for "iom.irrc"
 *              command.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _list_btn_str_code(VOID)
{
    IRRC_BTN_STR_MAP_T*     pt_btn_str_map = at_btn_to_str;
    
    x_dbg_stmt(" Btn_str        Btn_code        Btn_str        Btn_code\n\r");
    x_dbg_stmt("--------------------------------------------------------\n\r");
    
    while (pt_btn_str_map->ui4_btn_code != 0)
    {
        x_dbg_stmt(" %-14s 0x%x", 
                   pt_btn_str_map->ps_btn_str, 
                   pt_btn_str_map->ui4_btn_code);

        if ((++pt_btn_str_map)->ui4_btn_code == 0)
        {
            x_dbg_stmt("\n\r");
            break;
        }                        
                
        x_dbg_stmt("         %-14s 0x%x\n\r", 
                   pt_btn_str_map->ps_btn_str, 
                   pt_btn_str_map->ui4_btn_code);
                           
        pt_btn_str_map++;
    }
}


/*-----------------------------------------------------------------------------
 * Name: _show_irrc_evt_rec
 *
 * Description: This API shows currently programmed IRRCe events in IRRC event  
 *              record.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _show_irrc_evt_rec(VOID)
{
    UINT32      ui4_idx;
    
    if (ui4_irrc_evt_rec_num == 0)
    {
        x_dbg_stmt("IRRC event record is empty\n\r");
        return;
    }        
        
    x_dbg_stmt("[Num of IRRC events = %d]\n\r", ui4_irrc_evt_rec_num);
    x_dbg_stmt("----------------------------------------------\n\r");
    x_dbg_stmt(" btn_str       loop      1st_itvl    2nd_itvl\n\r");    
    x_dbg_stmt("----------------------------------------------\n\r");
    
    for (ui4_idx = 0; ui4_idx < ui4_irrc_evt_rec_num; ui4_idx++)
    {
        x_dbg_stmt(" %-10s    %-8d  %-8d    %-8d\n\r", 
                   t_irrc_evt_rec[ui4_idx].t_btn_str_map.ps_btn_str,
                   t_irrc_evt_rec[ui4_idx].ui4_loop,
                   t_irrc_evt_rec[ui4_idx].ui4_1st_itvl,
                   t_irrc_evt_rec[ui4_idx].ui4_2nd_itvl);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _start_sending_irrc_evt
 *
 * Description: This API starts sending events recorded in IRRC event record.
 *
 * Inputs:  ui4_loop    Loops of sending event record.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _start_sending_irrc_evt(UINT32 ui4_loop)
{
    BOOL        b_endless;
    BOOL        b_error = FALSE;
    INP_EVT_T   t_inp_evt;   
    UINT32      ui4_go_loop = 0; 
    UINT32      ui4_1st_delay;
    UINT32      ui4_2nd_delay;
    UINT32      ui4_delta;
    UINT32      ui4_i;
    UINT32      ui4_j;        
    
    b_endless = (ui4_loop == 0) ? TRUE : FALSE;
    ui4_delta = ui4_irrc_upper_bound - ui4_irrc_lower_bound;

    while ((ui4_go_loop < ui4_loop) || b_endless)
    {
        x_dbg_stmt(">>>>>>>> Loop %d <<<<<<<<\n\r", ui4_go_loop++);
                        
        /* Send events recorded in event record */
        for (ui4_i = 0; ui4_i < ui4_irrc_evt_rec_num; ui4_i++)
        {
            t_inp_evt.e_evt_type = INP_EVT_TYPE_IRRC;
            t_inp_evt.pv_tag = NULL;
            t_inp_evt.u.s_irrc.ui4_evt_code = t_irrc_evt_rec[ui4_i].t_btn_str_map.ui4_btn_code;
            x_memset(&t_inp_evt.u.s_irrc.t_data, 0 , sizeof(IRRC_DATA_T));

            for (ui4_j = 0; ui4_j < t_irrc_evt_rec[ui4_i].ui4_loop; ui4_j++)
            {
                if (cli_get_char_timeout(50) == IRRC_CTRL_B)
                {
                    x_dbg_stmt("\n\rOperation was interrupted by the user!!!\n\r");
                    return CLIR_OK;   
                }     
                             
                /* Send button-down event */
                t_inp_evt.e_cond = IOM_NFY_COND_BTN_DOWN;        
                
                _dfb_process_irrc_evt(&t_inp_evt);

                if (t_irrc_evt_rec[ui4_i].ui4_1st_itvl == 0)
                {                    
                    ui4_1st_delay = (_gen_rand() % ui4_delta) + ui4_irrc_lower_bound;
                }
                else
                {         
                    ui4_1st_delay = t_irrc_evt_rec[ui4_i].ui4_1st_itvl;           
                }
                
                x_thread_delay(ui4_1st_delay);                
                
                /* Send button-up event */
                t_inp_evt.e_cond = IOM_NFY_COND_BTN_UP;        
                
                _dfb_process_irrc_evt(&t_inp_evt);
                
                if (t_irrc_evt_rec[ui4_i].ui4_2nd_itvl == 0)
                {                    
                    ui4_2nd_delay = (_gen_rand() % ui4_delta) + ui4_irrc_lower_bound;
                }
                else
                {   
                    ui4_2nd_delay = t_irrc_evt_rec[ui4_i].ui4_2nd_itvl;
                }                

                x_dbg_stmt("IRRC btn %s sent [%dms %dms]\n\r", 
                           t_irrc_evt_rec[ui4_i].t_btn_str_map.ps_btn_str,
                           ui4_1st_delay,
                           ui4_2nd_delay);
                           
                x_thread_delay(ui4_2nd_delay);
            }                         
        }
    }     
    
    if (b_error)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    else
    {        
        return CLIR_OK;        
    }        
}

              
/*-----------------------------------------------------------------------------
 * Name: _rec_irrc_evt
 *
 * Description: This API records inputed IRRC event into IRRC event record.
 *
 * Inputs:  ui4_loop        Loops of sending this event.
 *          ui4_1st_itvl    Interval between BTN_DN and BTN_UP.
 *          ui4_2nd_itvl    Interval to next event.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _rec_irrc_evt(const CHAR* ps_irrc_str,
                           UINT32      ui4_loop,
                           UINT32      ui4_1st_itvl,
                           UINT32      ui4_2nd_itvl)
{
    IRRC_BTN_STR_MAP_T*     pt_btn_str_map = at_btn_to_str;    
        
    if (ui4_irrc_evt_rec_num < IRRC_EVT_REC_NUM)
    {
        while (pt_btn_str_map->ui4_btn_code != 0)
        {
            if (x_strcmp(ps_irrc_str, pt_btn_str_map->ps_btn_str) == 0)
            {                                
                t_irrc_evt_rec[ui4_irrc_evt_rec_num].t_btn_str_map.ui4_btn_code = pt_btn_str_map->ui4_btn_code;
                t_irrc_evt_rec[ui4_irrc_evt_rec_num].t_btn_str_map.ps_btn_str = pt_btn_str_map->ps_btn_str;
                t_irrc_evt_rec[ui4_irrc_evt_rec_num].ui4_loop = ui4_loop; 
                t_irrc_evt_rec[ui4_irrc_evt_rec_num].ui4_1st_itvl = ui4_1st_itvl;
                t_irrc_evt_rec[ui4_irrc_evt_rec_num].ui4_2nd_itvl = ui4_2nd_itvl;
                
                ui4_irrc_evt_rec_num++;
                x_dbg_stmt("Num of IRRC events = %d [%d]\n\r", ui4_irrc_evt_rec_num, IRRC_EVT_REC_NUM);
                break;
            }
         
            pt_btn_str_map++;
        }
        
        if (pt_btn_str_map->ui4_btn_code == 0)     
        {
            return CLIR_INV_CMD_USAGE;
        }                           
    }
    else
    {
        x_dbg_stmt("Reach the maximum of IRRC event record [%d]\n\r", IRRC_EVT_REC_NUM);
    }      
    
    return CLIR_OK;    
}


#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: _auto_sleep_cb
 *
 * Description: Auto sleep timer callback function.
 *
 * Inputs:  
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static VOID _auto_sleep_cb(IOM_NFY_COND_T e_nfy_cond,  
                           VOID*          pv_nfy_tag)
{
    switch (e_nfy_cond)
    {
        case IOM_NFY_COND_AUTO_SLEEP_PROMPT:
            x_dbg_stmt("\n\rAuto sleep timer --> IOM_NFY_COND_AUTO_SLEEP_PROMPT\n\r");
            break;

        case IOM_NFY_COND_AUTO_SLEEP_TIMEOUT:
            x_dbg_stmt("\n\rAuto sleep timer --> IOM_NFY_COND_AUTO_SLEEP_TIMEOUT\n\r");
            break;

        default:
            x_dbg_stmt("\n\rAuto sleep timer --> invalid condition\n\r");
    }
}
#endif

                 
/*-----------------------------------------------------------------------------
 * Name: _iom_cli_send_evt
 *
 * Description: This API sends an IRRC event to IO Manager.
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
static INT32 _iom_cli_send_evt(INT32        i4_argc,
                               const CHAR** pps_argv)
{
    /* iom.se [evt_code] [itvl] */

    /* Check arguments */
    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("iom.se [evt_code] [itvl]\n\r");
        x_dbg_stmt("  [itvl]: time interval between button-down and button-up (ms)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (x_iom_send_evt((UINT32)x_strtoull(pps_argv[1], NULL, 0),
                       (UINT32)x_strtoull(pps_argv[2], NULL, 0)) != IOMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    
    return CLIR_OK;
}


#ifdef CLI_LVL_ALL
static INT32 _iom_cli_send_evt_single(INT32 i4_argc, const CHAR** pps_argv)
{  
   BOOL fgBtnDown = TRUE;
 /* iom.se [evt_code] [down/up] */

    /* Check arguments */
    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("iom.ses [evt_code][ctrl]\n\r");
        x_dbg_stmt("  [ctrl]: 0=Button Up, 1= Button Down\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (x_strcmp("0", pps_argv[2]) == 0)
    {
        fgBtnDown = TRUE;
        x_dbg_stmt("send button down\n\r");
    }
    else if (x_strcmp("1", pps_argv[2]) == 0)
    {
        fgBtnDown = FALSE;
        x_dbg_stmt("send button up\n\r");
    }
    else
    {
        return CLIR_INV_ARG;
    }
    
    if (x_iom_send_evt_single((UINT32)x_strtoull(pps_argv[1], NULL, 0),
                               fgBtnDown) != IOMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _iom_cli_show_info
 *
 * Description: This API shows the internal info of IO Manager.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_show_info(INT32        i4_argc,
                                const CHAR** pps_argv)
{
    DEV_INFO_T*         pt_dev;
    EVT_FILTR_T*        pt_evt_filtr;
    UINT16              ui2_dev_idx = 0;
    UINT16              ui2_filtr_idx;
	
#if IOM_SUPPORT_IRRC_DISABLE_KEYS
    SIZE_T              z_disablekey_idx;
    const CHAR*         ps_gdki_arr = "gdki";
	
    /* Get global disable event code information */
	_iom_cli_get_glb_disabledkeys(1, &ps_gdki_arr);

#endif

    /* iom.info */

    pt_dev = iom_op_get_in_dev_info_struct();

    if (pt_dev != NULL)
    {
        while (pt_dev[ui2_dev_idx].e_dev_type != IOM_DEV_TYPE_NONE)
        {
            x_dbg_stmt("[Dev %d] handle=0x%08x, filter_cnt=%d\n\r",
                       ui2_dev_idx,
                       pt_dev[ui2_dev_idx].h_rm_dev,
                       pt_dev[ui2_dev_idx].ui1_evt_filtr_cnt);

            pt_evt_filtr = DLIST_HEAD(&(pt_dev[ui2_dev_idx].t_evt_filtr_list));

            for (ui2_filtr_idx = 0; ui2_filtr_idx < pt_dev[ui2_dev_idx].ui1_evt_filtr_cnt; ui2_filtr_idx++)
            {
                if (pt_evt_filtr != NULL)
                {
                    if (pt_dev[ui2_dev_idx].e_dev_type == IOM_DEV_TYPE_IN_IRRC)
                    {                        
                        x_dbg_stmt("  [Filtr %d] handle=0x%08x, mask=0x%08x%08x\n\r",
                                   ui2_filtr_idx,
                                   pt_evt_filtr->h_filtr,
                                   (UINT32)(pt_evt_filtr->u.s_inp.t_irrc_setting.ui8_evt_grp_mask >> 32),
                                   (UINT32)pt_evt_filtr->u.s_inp.t_irrc_setting.ui8_evt_grp_mask);
    
                        x_dbg_stmt("            1st_itvl=%dms, other_itvl=%dms\n\r",
                                   pt_evt_filtr->u.s_inp.t_irrc_setting.t_rpt_evt_itvl.ui2_1st*10,
                                   pt_evt_filtr->u.s_inp.t_irrc_setting.t_rpt_evt_itvl.ui2_other*10);
#if IOM_SUPPORT_IRRC_DISABLE_KEYS
                       if(pt_evt_filtr->u.s_inp.t_disable_keys.b_enable)
                       {
                            x_dbg_stmt("            filter key feature is enabled\n\r");
                       }
                       else
                       {
                            x_dbg_stmt("            filter key feature is disabled\n\r");
                       }

                       if(pt_evt_filtr->u.s_inp.t_disable_keys.b_positive_filter)
                       {
                           x_dbg_stmt("            keys is disabled\n\r");
                       }
                       else
                       {
                           x_dbg_stmt("            keys is enabled\n\r");
                       }
                            
                       x_dbg_stmt("            keycnt = 0x%08x\n\r", 
                                   pt_evt_filtr->u.s_inp.t_disable_keys.z_evtcode_cnt);
                             
                       if (0 != pt_evt_filtr->u.s_inp.t_disable_keys.z_evtcode_cnt)
                       {
                          if (pt_evt_filtr->u.s_inp.t_disable_keys.b_positive_filter)
                          {
                               x_dbg_stmt("          disable key  list:\n\r");
                          }
                          else
                          {
                               x_dbg_stmt("            enable key   list:\n\r");
                          }

                           for(z_disablekey_idx = 0; z_disablekey_idx < pt_evt_filtr->u.s_inp.t_disable_keys.z_evtcode_cnt; ++ z_disablekey_idx)
                           {
                                x_dbg_stmt("           key %d = 0x%08x\n\r",
                                            z_disablekey_idx,
                                           *(pt_evt_filtr->u.s_inp.t_disable_keys.pui4_evtcode_arr + z_disablekey_idx));
                           }
                        }
#endif
                    }
                    else
                    {
                        x_dbg_stmt("  [Filtr %d] handle=0x%08x\n\r",
                                   ui2_filtr_idx,
                                   pt_evt_filtr->h_filtr);                        
                    }                                                                                   

                    pt_evt_filtr = DLIST_NEXT(pt_evt_filtr, t_link);
                }
            }

            ui2_dev_idx++;
            x_dbg_stmt("\n\r");
        }
    }

    return CLIR_OK;
}
#endif


/*-----------------------------------------------------------------------------
 * Name: _iom_cli_set_key_mon
 *
 * Description: This API enables or disables IO Manager key monitor.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_set_key_mon(INT32        i4_argc,
                                  const CHAR** pps_argv)
{
    /* iom.km [ctrl] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("iom.km [ctrl]\n\r");
        x_dbg_stmt("  [ctrl]: 0=OFF, 1=ON\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (x_strcmp("0", pps_argv[1]) == 0)
    {
        iom_inp_dptr_set_key_mon(FALSE);
        x_dbg_stmt("Key monitor = OFF\n\r");
    }
    else if (x_strcmp("1", pps_argv[1]) == 0)
    {
        iom_inp_dptr_set_key_mon(TRUE);
        x_dbg_stmt("Key monitor = ON\n\r");
    }
    else
    {
        return CLIR_INV_ARG;
    }

    return CLIR_OK;
}


#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: _iom_cli_set_key_gen
 *
 * Description: This API enables or disables IO Manager key generator.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_set_key_gen(INT32        i4_argc,
                                  const CHAR** pps_argv)
{
    /* iom.kg [ctrl] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("iom.kg [ctrl]\n\r");
        x_dbg_stmt("  [ctrl]: 0=OFF, 1=ON\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (x_strcmp("0", pps_argv[1]) == 0)
    {
        iom_key_gen_set_status(FALSE);
        x_dbg_stmt("Key generator = OFF\n\r");
    }
    else if (x_strcmp("1", pps_argv[1]) == 0)
    {
        iom_key_gen_set_status(TRUE);
        x_dbg_stmt("Key generator = ON\n\r");
    }
    else
    {
        return CLIR_INV_ARG;
    }

    return CLIR_OK;
}
#endif


/*-----------------------------------------------------------------------------
 * Name: _iom_cli_set_irrc_inp
 *
 * Description: This API enables or disables IRRC event input path.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_set_irrc_inp(INT32        i4_argc,
                                   const CHAR** pps_argv)
{
    /* iom.ii [ctrl] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("iom.ii [ctrl]\n\r");
        x_dbg_stmt("  [ctrl]: 0=OFF, 1=ON\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (x_strcmp("0", pps_argv[1]) == 0)
    {
        iom_op_set_in_irrc(FALSE);
        x_dbg_stmt("IRRC input = OFF\n\r");
    }
    else if (x_strcmp("1", pps_argv[1]) == 0)
    {
        iom_op_set_in_irrc(TRUE);
        x_dbg_stmt("IRRC input = ON\n\r");
    }
    else
    {
        return CLIR_INV_ARG;
    }

    return CLIR_OK;
}


#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: _iom_cli_show_log
 *
 * Description: This API gets the current setting of IO Manager debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_show_log(INT32        i4_argc,
                               const CHAR** pps_argv)
{
    /* iom.sl */

    iom_key_gen_show_log();

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _iom_cli_clear_log
 *
 * Description: This API gets the current setting of IO Manager debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_clear_log(INT32        i4_argc,
                                const CHAR** pps_argv)
{
    /* iom.cl */

    iom_key_gen_clear_log();

    return CLIR_OK;
}

#if 0

/*-----------------------------------------------------------------------------
 * Name: _iom_cli_show_fpd
 *
 * Description: This API shows characters to front panel display.
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
static INT32 _iom_cli_show_fpd(INT32        i4_argc,
                               const CHAR** pps_argv)
{
    INT32       i4_return;
    IOM_FPD_T   t_fpd_info;
    CHAR        s_char[10];

    /* iom.sfpd [offset] [content] */

    if (i4_argc != 3)
    {
        x_dbg_stmt("iom.sfpd [offset] [content]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    /* Open the device */
    if (h_fpd_dev == NULL_HANDLE)
    {
        i4_return = x_iom_open(IOM_DEV_TYPE_OUT_FPD,
                               ANY_COMP_ID,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               &h_fpd_dev);

        if (i4_return != IOMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }

    /* Perform the operation */
    t_fpd_info.ui2_line_num = 1;
    t_fpd_info.z_offset = (SIZE_T)(x_strtoull(pps_argv[1], NULL, 0));
    t_fpd_info.z_len = x_strlen(pps_argv[2]);
    x_strcpy(s_char, pps_argv[2]);
    t_fpd_info.ps_content = s_char;

    i4_return = x_iom_output(h_fpd_dev, (VOID*)&t_fpd_info);
    if (i4_return != IOMR_OK)
    {
        if (i4_return == IOMR_INV_ARG)
        {
            x_dbg_stmt("iom.sfpd [offset] [content]\n\r");
            return CLIR_INV_CMD_USAGE;
        }
        else
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _iom_cli_clear_fpd
 *
 * Description: This API clear characters from front panel display.
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
static INT32 _iom_cli_clear_fpd(INT32        i4_argc,
                                const CHAR** pps_argv)
{
    INT32       i4_return;
    IOM_FPD_T   t_fpd_info;

    /* iom.cfpd [offset] [len] */

    if (i4_argc != 3)
    {
        x_dbg_stmt("iom.cfpd [offset] [len]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    /* Open the device */
    if (h_fpd_dev == NULL_HANDLE)
    {
        i4_return = x_iom_open(IOM_DEV_TYPE_OUT_FPD,
                               ANY_COMP_ID,
                               NULL,
                               NULL,
                               NULL,
                               NULL,                                   
                               &h_fpd_dev);

        if (i4_return != IOMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }

    /* Perform the operation */
    t_fpd_info.ui2_line_num = 1;
    t_fpd_info.z_offset = (SIZE_T)(x_strtoull(pps_argv[1], NULL, 0));
    t_fpd_info.z_len = (SIZE_T)(x_strtoull(pps_argv[2], NULL, 0));
    t_fpd_info.ps_content = NULL;

    i4_return = x_iom_output(h_fpd_dev, (VOID*)&t_fpd_info);
    if (i4_return != IOMR_OK)
    {
        if (i4_return == IOMR_INV_ARG)
        {
            x_dbg_stmt("iom.cfpd [offset] [len]\n\r");
            return CLIR_INV_CMD_USAGE;
        }
        else
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _iom_cli_set_ind_power
 *
 * Description: This API sets power indicator.
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
static INT32 _iom_cli_set_ind_power(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;
    IOM_IND_T   t_ind_info;

    /* iom.sp [state] [color] */

    if (i4_argc != 3)
    {
        x_dbg_stmt("iom.sp [state] [color]\n\r");
        x_dbg_stmt("  [state]: 0=enable_steady, 1=enable_blink, 2=disable\n\r");
        x_dbg_stmt("  [color]: 0=single, 1=red, 2=green, 3=yellow, 4=blue, 5=amber, 6=white\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    /* Open the device */
    if (h_ind_power_dev == NULL_HANDLE)
    {
        i4_return = x_iom_open(IOM_DEV_TYPE_OUT_IND_POWER,
                               ANY_COMP_ID,
                               NULL,
                               NULL,
                               NULL,
                               NULL,                               
                               &h_ind_power_dev);

        if (i4_return != IOMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }

    /* Perform the operation */
    t_ind_info.e_ind_state = (IOM_IND_STATE_T)(x_strtoull(pps_argv[1], NULL, 0));
    t_ind_info.e_ind_color = (IOM_IND_COLOR_T)(x_strtoull(pps_argv[2], NULL, 0));

    i4_return = x_iom_output(h_ind_power_dev, (VOID*)&t_ind_info);
    if (i4_return != IOMR_OK)
    {
        if (i4_return == IOMR_INV_ARG)
        {
            x_dbg_stmt("iom.sp [state] [color]\n\r");
            x_dbg_stmt("  [state]: 0=enable_steady, 1=enable_blink, 2=disable\n\r");
            x_dbg_stmt("  [color]: 0=single, 1=red, 2=green, 3=yellow, 4=blue, 5=amber, 6=white\n\r");
            return CLIR_INV_CMD_USAGE;
        }
        else
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }

    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _iom_cli_set_auto_sleep_timer
 *
 * Description: This API configures the system sleep timer that will trigger
 *              the system into standby mode when time-out.
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
static INT32 _iom_cli_set_auto_sleep_timer(INT32        i4_argc,
                                           const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_prompt;
    UINT32      ui4_sleep;

    /* iom.as [prompt] [sleep]*/

    /* Check arguments */
    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("iom.as [prompt] [sleep]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    ui4_prompt = (UINT32)x_strtoull(pps_argv[1], NULL, 0);
    ui4_sleep = (UINT32)x_strtoull(pps_argv[2], NULL, 0);

    if (ui4_prompt == 0)
    {
        i4_return = x_iom_set_auto_sleep_timer(ui4_sleep);

        if (i4_return != IOMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        AUTO_SLEEP_TIMER_T  t_auto_sleep_timer;

        t_auto_sleep_timer.ui4_prompt_timeout = ui4_prompt;
        t_auto_sleep_timer.ui4_sleep_timeout = ui4_sleep;
        t_auto_sleep_timer.pf_nfy = _auto_sleep_cb;
        t_auto_sleep_timer.pv_nfy_tag = NULL;
        
        i4_return = x_iom_set_auto_sleep_timer_ex(&t_auto_sleep_timer);

        if (i4_return != IOMR_OK)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }        

    return i4_return;
}
#endif


/*-----------------------------------------------------------------------------
 * Name: _iom_cli_prog_irrc
 *
 * Description: This API programs IRRC events which are planed to be sent to
 *              IO Manager.
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
static INT32 _iom_cli_prog_irrc(INT32        i4_argc,
                                const CHAR** pps_argv)
{
    INT32       i4_return = CLIR_OK;

    /* iom.irrc [cmd] [argvs] / iom.irrc [btn_str] [loop] [1st_itvl] [2nd_itvl] */
    /* Check arguments */
    if (((i4_argc != 2) &&
         (i4_argc != 3) &&
         (i4_argc != 4) &&
         (i4_argc != 5)) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("iom.irrc [cmd] [argvs]\n\r");
        x_dbg_stmt("  [cmd]: list: list available IRRC button strings\n\r");
        x_dbg_stmt("         rec: show programmed IRRC events\n\r");
        x_dbg_stmt("         clear: clear programmed IRRC events\n\r");
        x_dbg_stmt("         bound: config lower and upper bounds for use when interval is random\n\r");        
        x_dbg_stmt("         go: start sending programmed IRRC events (press Ctrl+B to stop)\n\r");
        x_dbg_stmt("  [argvs]: argv1 => loops (for go cmd only)\n\r");
        x_dbg_stmt("                    0=endless loop\n\r");
        x_dbg_stmt("  [argvs]: argv1 & argv2 => lower and upper bounds (ms)(for bound cmd only)\n\r\n\r");      
        x_dbg_stmt("iom.irrc [btn_str] [loop] [1st_itvl] [2nd_itvl]\n\r");      
        x_dbg_stmt("  [btn_str]: button name\n\r");
        x_dbg_stmt("  [loop]: how many loops for the event\n\r");
        x_dbg_stmt("  [1st_itvl]: interval(ms) between BTN_DN and BTN_UP\n\r");
        x_dbg_stmt("              0=1st interval is random\n\r");        
        x_dbg_stmt("  [2nd_itvl]: interval(ms) to next events\n\r");
        x_dbg_stmt("              0=2nd interval is random\n\r");   
        return CLIR_INV_CMD_USAGE;
    }    
    
    if (i4_argc == 2)
    {
        if (x_strcmp("list", pps_argv[1]) == 0)
        {       
            _list_btn_str_code();     
            i4_return = CLIR_OK;
        }           
        else if (x_strcmp("rec", pps_argv[1]) == 0)
        {
            _show_irrc_evt_rec();
            i4_return = CLIR_OK;          
        }               
        else if (x_strcmp("clear", pps_argv[1]) == 0)
        {
            ui4_irrc_evt_rec_num = 0;  
            x_dbg_stmt("All IRRC events have been deleted\n\r");
            i4_return = CLIR_OK;          
        }       
        else if (x_strcmp("go", pps_argv[1]) == 0)
        {
            if (ui4_irrc_evt_rec_num == 0)
            {
                x_dbg_stmt("None of IRRC events have been programmed\n\r");
                i4_return = CLIR_OK; 
            }        
            else
            {                        
                i4_return = _start_sending_irrc_evt(1);            
            }    
        }       
        else if (x_strcmp("bound", pps_argv[1]) == 0)
        {      
            x_dbg_stmt("Lower bound = %d ms\n\r", ui4_irrc_lower_bound);
            x_dbg_stmt("Upper bound = %d ms\n\r", ui4_irrc_upper_bound);
        }    
        else
        {
            i4_return = CLIR_INV_CMD_USAGE;
        }                
    }       
    else if (i4_argc == 3)
    {
        if (x_strcmp("go", pps_argv[1]) == 0)
        {
            if (ui4_irrc_evt_rec_num == 0)
            {
                x_dbg_stmt("None of IRRC events have been programmed\n\r");
                i4_return = CLIR_OK; 
            }        
            else
            {                        
                i4_return = _start_sending_irrc_evt((UINT32)x_strtoull(pps_argv[2], NULL, 0));            
            }
        }            
        else
        {
            i4_return = _rec_irrc_evt(pps_argv[1],
                                      (UINT32)x_strtoull(pps_argv[2], NULL, 0),
                                      0,
                                      0);           
        }                        
    }
    else if (i4_argc == 4)
    {
        if (x_strcmp("bound", pps_argv[1]) == 0)
        {
            if (x_strtoull(pps_argv[2], NULL, 0) < x_strtoull(pps_argv[3], NULL, 0))
            {                
                ui4_irrc_lower_bound = (UINT32)x_strtoull(pps_argv[2], NULL, 0);
                ui4_irrc_upper_bound = (UINT32)x_strtoull(pps_argv[3], NULL, 0);
                x_dbg_stmt("Lower bound = %d ms\n\r", ui4_irrc_lower_bound);
                x_dbg_stmt("Upper bound = %d ms\n\r", ui4_irrc_upper_bound);                    
            }
            else
            {
                x_dbg_stmt("Upper bound must greater than Lower bound\n\r");
                i4_return = CLIR_OK;
            }                                  
        }            
        else
        {
            i4_return = CLIR_INV_CMD_USAGE;
        }  

    }    
    else if (i4_argc == 5)
    {
        i4_return = _rec_irrc_evt(pps_argv[1],
                                  (UINT32)x_strtoull(pps_argv[2], NULL, 0),
                                  (UINT32)x_strtoull(pps_argv[3], NULL, 0),
                                  (UINT32)x_strtoull(pps_argv[4], NULL, 0));
    }   
    else
    {
        i4_return = CLIR_INV_CMD_USAGE;
    }                      
    
    if (i4_return == CLIR_INV_CMD_USAGE)
    {
        x_dbg_stmt("iom.irrc [cmd] [argvs]\n\r");
        x_dbg_stmt("  [cmd]: list: list available IRRC button strings\n\r");
        x_dbg_stmt("         rec: show programmed IRRC events\n\r");
        x_dbg_stmt("         clear: clear programmed IRRC events\n\r");
        x_dbg_stmt("         bound: config lower and upper bounds for use when interval is random\n\r");        
        x_dbg_stmt("         go: start sending programmed IRRC events (press Ctrl+B to stop)\n\r");
        x_dbg_stmt("  [argvs]: argv1 => loops (for go cmd only)\n\r");
        x_dbg_stmt("                    0=endless loop\n\r");
        x_dbg_stmt("  [argvs]: argv1 & argv2 => lower and upper bounds (for bound cmd only)\n\r\n\r");      
        x_dbg_stmt("iom.irrc [btn_str] [loop] [1st_itvl] [2nd_itvl]\n\r");      
        x_dbg_stmt("  [btn_str]: button name\n\r");
        x_dbg_stmt("  [loop]: how many loops for the event\n\r");
        x_dbg_stmt("  [1st_itvl]: interval(ms) between BTN_DN and BTN_UP\n\r");
        x_dbg_stmt("              0=1st interval is random\n\r");        
        x_dbg_stmt("  [2nd_itvl]: interval(ms) to next event\n\r");
        x_dbg_stmt("              0=2nd interval is random\n\r");
    }         
                
    return i4_return;   
}


/*-----------------------------------------------------------------------------
 * Name: _iom_cli_channel_change
 *
 * Description: This API triggers channel change operations.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_channel_change(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    INT32       i4_delay_lower = 3000;
    INT32       i4_delay_upper = 3000;
    INT32       i4_delay_delta = 0;
    INT32       i4_loop        = 10;
    UINT32      ui4_idx        = 0;
    UINT16      ui2_dev        = 0;
    BOOL        b_seq_change   = TRUE;
    static      BOOL b_from    = TRUE;
    UINT32      ui4_delay      = 0;
    BOOL        b_is_random_ch = FALSE;
    VOID*       pv_tag         = NULL;
    DEV_INFO_T* pt_dev = NULL;

    /* iom.cx [#iteration #delay_lower_bound #delay_upper_bound [-s major1.minor1 major2.minor2]] */	
	
    if (i4_argc > 1)
    {
        i4_loop = _ascii_to_int(pps_argv[1]);
        if (i4_loop < 0)
        {
            x_dbg_stmt("Iteration must larger or equal to zero!\r\n");
            return CLIR_OK;
        }
    }

    if (i4_argc > 2)
    {
        i4_delay_lower = _ascii_to_int(pps_argv[2]);
        if (i4_delay_lower == -1 || i4_delay_lower == 0)
        {
            i4_delay_lower = 3000;
        }

        if (i4_delay_lower < 50)
        {
            i4_delay_lower = 50;
        }
    }

    if (i4_argc > 3)
    {
        i4_delay_upper = _ascii_to_int(pps_argv[3]);
        if (i4_delay_upper == -1 || i4_delay_upper == 0)
        {
            i4_delay_upper = 3000;
        }

        if (i4_delay_upper < 50)
        {
            i4_delay_upper = 50;
        }

        if (i4_delay_upper < i4_delay_lower)
        {
            x_dbg_stmt("Upper bound must large or equal to lower bound!\r\n");
            return CLIR_OK;
        }
    }
    else
    {
        i4_delay_upper = i4_delay_lower;
    }

    if (i4_argc > 6)
    {
       if (x_strcmp(pps_argv[4], "-s") == 0)
       {
           b_seq_change = FALSE;
           b_from = TRUE;
       }
    }

    b_is_random_ch = (i4_delay_upper == i4_delay_lower) ? FALSE : TRUE;
    i4_delay_delta = (i4_delay_upper - i4_delay_lower + 1);
    ui4_delay      = (UINT32)i4_delay_lower;

    pt_dev = iom_op_get_in_dev_info_struct();
    
    for (ui2_dev = 0; ui2_dev < (UINT16)IOM_MAX_DEV; ui2_dev++)
    {
        if (pt_dev == NULL)
        {
            x_dbg_stmt("Failed to send key (1)!\r\n");
            return CLIR_OK;
        }

        if (pt_dev->e_dev_type == IOM_DEV_TYPE_IN_IRRC)
        {
            pv_tag = (VOID*)(&(pt_dev->ui4_tag));
            break;
        }
        else if (pt_dev->e_dev_type == IOM_DEV_TYPE_NONE)
        {
            x_dbg_stmt("Failed to send key (2)!\r\n");
            return CLIR_OK;
        }
        
        pt_dev++;
    }

    if (ui2_dev >= (UINT16)IOM_MAX_DEV)
    {
        x_dbg_stmt("Failed to send key (3)!\r\n");
        return CLIR_OK;
    }

    while ((ui4_idx < (UINT32)i4_loop) || (i4_loop == 0))
    {
        if (b_is_random_ch == TRUE)
        {
            ui4_delay = (UINT32)((_gen_rand() % i4_delay_delta) + i4_delay_lower);
        }

        if (b_seq_change)
        {
            
            x_dbg_stmt("\r\n----> %d-th change channel(delay=%d ms)...\r\n", ui4_idx++, ui4_delay);
            
            _send_btn(BTN_PRG_UP, pv_tag);
        }
        else
        {
            CHAR* ps_ch_str = (CHAR*)(b_from? pps_argv[5]: pps_argv[6]);
            CHAR* pch = ps_ch_str;

            x_dbg_stmt("\r\n----> %d-th change channel to %s (delay=%d ms)...\r\n", ui4_idx++, ps_ch_str, ui4_delay);
            while (*pch != '\x0')
            {
                if (*pch >= '0' && *pch <= '9')
                {
                    _send_btn(BTN_DIGIT_0 + (UINT32)(*pch - '0'), pv_tag);
                }
                else if (*pch == '.')
                {
                    _send_btn(BTN_DIGIT_DOT, pv_tag);
                }
                pch++;
                x_thread_delay(300);
            }
            _send_btn(BTN_SELECT, pv_tag);
            b_from = (b_from == TRUE)? FALSE: TRUE;
        }

        x_thread_delay(ui4_delay);
    }

    return CLIR_OK;
}

#ifdef CLI_LVL_ALL
#if IOM_SUPPORT_IRRC_DISABLE_KEYS
/*-----------------------------------------------------------------------------
 * Name: _iom_cli_irrc_get_handle_by_fltridx
 *
 * Description: This API get the event filter handle by the evnet filter index.
 *
 * Inputs:  ui1_filtr_idx         the event filter index in the irrc device. 
 *
 * Outputs: h_dev                the event filter handle 
 *
 * Returns: CLIR_OK                       Routine successful.
 *          CLIR_INV_ARG                   Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_irrc_get_handle_by_fltridx(UINT8 ui1_filtr_idx, 
                                                 HANDLE_T  *h_dev)
{
    DEV_INFO_T*        pt_dev = NULL;
    UINT16             ui2_dev_idx = 0;

    UINT8              ui1_filtr_loop = 0;
    
    EVT_FILTR_T*       pt_evt_filtr = NULL;
    
     iom_lock_db_wr();

     /* get the device */
     pt_dev = iom_op_get_in_dev_info_struct();
     if (pt_dev != NULL)
     {
         while ((pt_dev[ui2_dev_idx].e_dev_type != IOM_DEV_TYPE_NONE) &&
                (pt_dev[ui2_dev_idx].e_dev_type != IOM_DEV_TYPE_IN_IRRC))
        {
          ++ ui2_dev_idx;
        }
     }
    
     /* check the event filter index is exceed the filter count or not */   
     if (pt_dev[ui2_dev_idx].ui1_evt_filtr_cnt <= ui1_filtr_idx)
     {
         x_dbg_stmt("evtfiltridx %d is exceed the filter count %d\n\r", 
                     ui1_filtr_idx, 
                     pt_dev[ui2_dev_idx].ui1_evt_filtr_cnt);
    
         iom_unlock_db();
          
         return CLIR_INV_CMD_USAGE;
     }
    
    /* Get the even filter */    
     pt_evt_filtr = DLIST_HEAD(&(pt_dev[ui2_dev_idx].t_evt_filtr_list)); 
     for (ui1_filtr_loop = 0; ui1_filtr_loop < ui1_filtr_idx; ++ ui1_filtr_loop)
     {
         pt_evt_filtr = DLIST_NEXT(pt_evt_filtr, t_link);
     }
    
     *h_dev = pt_evt_filtr->h_filtr;
    
     iom_unlock_db();

     return CLIR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: _iom_cli_set_disabledkeys
 *
 * Description: This API sets the filter disable keys info.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                       Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_set_disabledkeys(INT32        i4_argc,
                                     const CHAR** pps_argv)
{   
    UINT8              ui1_filtr_idx = 0; 
    
    HANDLE_T           h_dev;
    
    DISABLE_EVTCODE_T  t_disablekeys_info;
    UINT32             ui4_keys_Loop = 0;

    UINT32             ui4_enable_start = 2;

    if (i4_argc < 4 || NULL == pps_argv)
    { 
       x_dbg_stmt("   iom.dk [evtfiltridx] [enable] [evtcode filter][keys] ... \n\r");   
       x_dbg_stmt("     [evtfiltridx]: the event index in the irrc device\n\r");       
       x_dbg_stmt("     [enable]: enable disable key or not: 1:enable; 0: disable\n\r");
       x_dbg_stmt("     [evtcode filter] disable or enable the keys in the keys: 1: disable; 0: enable\n\r");
       x_dbg_stmt("     [keys] ... : disable key code list\n\r\n\r");                  
       return CLIR_INV_CMD_USAGE;   
    }

     /* get evt handle */
    ui1_filtr_idx = (UINT8)x_strtoull(pps_argv[1], NULL, 0);
    if (CLIR_OK != _iom_cli_irrc_get_handle_by_fltridx(ui1_filtr_idx, &h_dev))
    {
        return CLIR_INV_CMD_USAGE;
    }


      /* get  the disable keys info */
      if (1 == (UINT8)x_strtoll(pps_argv[ui4_enable_start], NULL, 0))
      {
           t_disablekeys_info.b_enable = TRUE;
      }
      else if(0 == (UINT8)x_strtoll(pps_argv[ui4_enable_start], NULL, 0))
      {
           t_disablekeys_info.b_enable = FALSE;
      }
      else
      {
          x_dbg_stmt("     [enable]: enable disable key or not: 1:enable; 0: disable\n\r");
          return CLIR_INV_CMD_USAGE;
      }     
        
      if(1 == (UINT8)x_strtoll(pps_argv[ui4_enable_start + 1], NULL, 0))
      {
           t_disablekeys_info.b_positive_filter = TRUE;
      }
      else if( 0 == (UINT8)x_strtoll(pps_argv[ui4_enable_start + 1], NULL, 0))
      {
           t_disablekeys_info.b_positive_filter = FALSE;
      }
      else
      {
           x_dbg_stmt("     [evtcode filter] disable or enable the keys in the keys: 1: disable; 0: enable\n\r");
           return CLIR_INV_CMD_USAGE;
      }
    
      t_disablekeys_info.z_evtcode_cnt = i4_argc - ui4_enable_start - 2;
      t_disablekeys_info.pui4_evtcode_arr = NULL;
     
      if (t_disablekeys_info.z_evtcode_cnt > 0 )
      {
          t_disablekeys_info.pui4_evtcode_arr = (UINT32 *)x_mem_alloc(sizeof(UINT32) * t_disablekeys_info.z_evtcode_cnt);
          if ( NULL != t_disablekeys_info.pui4_evtcode_arr )
          {
              for(ui4_keys_Loop = ui4_enable_start + 2; ui4_keys_Loop < i4_argc; ++ ui4_keys_Loop)
              {
                  *(t_disablekeys_info.pui4_evtcode_arr + ui4_keys_Loop - ui4_enable_start - 2) = (UINT32)x_strtoull(pps_argv[ui4_keys_Loop], NULL, 0);
              }
          }
      }

       /* Set the disable keys info */
      if (IOMR_OK != x_iom_set(h_dev, IOM_SET_IRRC_DISABLE_EVTCODE_INFO, (VOID*)&t_disablekeys_info, sizeof(DISABLE_EVTCODE_T)))
      {
          x_dbg_stmt("set disable key failed\n\r");
      }

      if ( NULL != t_disablekeys_info.pui4_evtcode_arr)
      {
          x_mem_free(t_disablekeys_info.pui4_evtcode_arr);
      }

    return CLIR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: _iom_cli_get_disabledkeys_info
 *
 * Description: This API gets the filter disable keys info.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                       Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_get_disabledkeys_info(INT32 i4_argc, 
                                             const CHAR** pps_argv)
{
    UINT8              ui1_filtr_idx = 0; 
    HANDLE_T           h_dev;
    
    DISABLE_EVTCODE_T  t_disablekeys_info;
    SIZE_T             z_keys_loop = 0;
    
    if (i4_argc != 2 || NULL == pps_argv)
    {
        x_dbg_stmt("iom.dki [evtfiltridx]\n\r");
        x_dbg_stmt("  [evtfiltridx]: the event index in the irrc device\n\r");  
          
        return CLIR_INV_CMD_USAGE;
    }

      /* get evt handle */
     ui1_filtr_idx = (UINT8)x_strtoull(pps_argv[1], NULL, 0);
     if (CLIR_OK != _iom_cli_irrc_get_handle_by_fltridx(ui1_filtr_idx, &h_dev))
     {
         return CLIR_OK;
     }

    t_disablekeys_info.pui4_evtcode_arr = NULL;

     z_keys_loop = sizeof(SIZE_T);
     if (IOMR_OK != x_iom_get(h_dev, IOM_GET_IRRC_DISABLE_EVTCODE_CNT, (VOID*)&(t_disablekeys_info.z_evtcode_cnt), &z_keys_loop))
     {
          x_dbg_stmt("get disable key count failed\n\r");
          return CLIR_CMD_EXEC_ERROR;
     }

     if (0 != t_disablekeys_info.z_evtcode_cnt)
     {
        t_disablekeys_info.pui4_evtcode_arr = (UINT32 *)x_mem_alloc(sizeof(UINT32) * t_disablekeys_info.z_evtcode_cnt);
        if (NULL == t_disablekeys_info.pui4_evtcode_arr ) 
        {
          x_dbg_stmt("allocate memory failed while get disable key info\n\r");
          return CLIR_OUT_OF_MEM;
        }

        z_keys_loop = sizeof(DISABLE_EVTCODE_T);
        if(IOMR_OK != x_iom_get(h_dev, IOM_GET_IRRC_DISABLE_EVTCODE_INFO, (VOID *)&(t_disablekeys_info), &z_keys_loop))
        {
          x_dbg_stmt("get disable keys info failed\n\r");   

          if ( NULL != t_disablekeys_info.pui4_evtcode_arr)
          {
              x_mem_free(t_disablekeys_info.pui4_evtcode_arr);
          }
          return CLIR_CMD_EXEC_ERROR;
        }
     }

     /* output the event filter disable keys info */
      x_dbg_stmt("  [Filtr %d] handle=0x%08x\n\r",
                                   ui1_filtr_idx,
                                   h_dev);
     if(t_disablekeys_info.b_enable)
     {
         x_dbg_stmt("             filter key feature is enabled\n\r");
     }
     else
     {
        x_dbg_stmt("             filter key feature is disabled\n\r");
     }

     if(t_disablekeys_info.b_positive_filter)
     {
        x_dbg_stmt("           keys is disabled\n\r");
     }
     else
     {
         x_dbg_stmt("          keys is enabled\n\r");
     }
                            
      x_dbg_stmt("             keycnt = 0x%08x\n\r", 
                 t_disablekeys_info.z_evtcode_cnt);
      
      if (0 != t_disablekeys_info.z_evtcode_cnt)
      {
          if(t_disablekeys_info.b_positive_filter)
          {
              x_dbg_stmt("             disable key  list:\n\r");
          }
          else
          {
              x_dbg_stmt("             enable key  list:\n\r");
          }

          for(z_keys_loop = 0; z_keys_loop < t_disablekeys_info.z_evtcode_cnt; ++ z_keys_loop)
          {
              x_dbg_stmt("             dkey %d = 0x%08x\n\r", z_keys_loop, *(t_disablekeys_info.pui4_evtcode_arr + z_keys_loop));
          }
      }                    

     /* release the memory */
     if ( NULL != t_disablekeys_info.pui4_evtcode_arr)
     {
         x_mem_free(t_disablekeys_info.pui4_evtcode_arr);
     }
      
     return CLIR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: _iom_cli_set_glb_disabledkeys
 *
 * Description:  This API sets the global disable keys information.
 *
 * Inputs:  i4_argc       Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                       Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_set_glb_disabledkeys(INT32 i4_argc, const CHAR** pps_argv)
{
    
    DISABLE_EVTCODE_T  t_disablekeys_info;
    UINT32             ui4_keys_Loop = 0;

    if (i4_argc < 3 || NULL == pps_argv)
    { 
       x_dbg_stmt("   iom.dk [enable] [evtcode filter][keys] ... \n\r");     
       x_dbg_stmt("     [enable]: enable disable key or not: 1:enable; 0: disable\n\r");
       x_dbg_stmt("     [evtcode filter] disable or enable the keys in the keys: 1: disable; 0: enable\n\r");
       x_dbg_stmt("     [keys] ... : disable key code list\n\r\n\r");                  
       return CLIR_INV_CMD_USAGE;   
    }

    /* get  the disable keys info */
    if (1 == (UINT8)x_strtoll(pps_argv[1], NULL, 0))
    {
        t_disablekeys_info.b_enable = TRUE;
    }
    else if(0 == (UINT8)x_strtoll(pps_argv[1], NULL, 0))
    {
        t_disablekeys_info.b_enable = FALSE;
    }
    else
    {
       x_dbg_stmt("     [enable]: enable disable key or not: 1:enable; 0: disable\n\r");
       return CLIR_INV_CMD_USAGE;
    }
        
    if(1 == (UINT8)x_strtoll(pps_argv[2], NULL, 0))
    {
        t_disablekeys_info.b_positive_filter = TRUE;
    }
    else if(0 == (UINT8)x_strtoll(pps_argv[2], NULL, 0))
    {
        t_disablekeys_info.b_positive_filter = FALSE;
    }
    else
    {
        x_dbg_stmt("     [evtcode filter] disable or enable the keys in the keys: 1: disable; 0: enable\n\r");
        return CLIR_INV_CMD_USAGE;
    }
    
    t_disablekeys_info.z_evtcode_cnt = i4_argc - 3;
    t_disablekeys_info.pui4_evtcode_arr = NULL;
     
    if (t_disablekeys_info.z_evtcode_cnt > 0 )
    {
        t_disablekeys_info.pui4_evtcode_arr = (UINT32 *)x_mem_alloc(sizeof(UINT32) * t_disablekeys_info.z_evtcode_cnt);
        if ( NULL != t_disablekeys_info.pui4_evtcode_arr )
        {
            for(ui4_keys_Loop = 3; ui4_keys_Loop < i4_argc; ++ ui4_keys_Loop)
            {
               *(t_disablekeys_info.pui4_evtcode_arr + ui4_keys_Loop - 3) = (UINT32)x_strtoull(pps_argv[ui4_keys_Loop], NULL, 0);
            }
        }
    }

     /* Set the disable keys info */
    if (IOMR_OK != x_iom_set_global_disable_evtcode(&t_disablekeys_info))
    {
          x_dbg_stmt("set disable key failed\n\r");
     }

    if ( NULL != t_disablekeys_info.pui4_evtcode_arr)
   {
       x_mem_free(t_disablekeys_info.pui4_evtcode_arr);
   }

  return CLIR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: _iom_cli_get_glb_disabledkeys
 *
 * Description:  This API gets the global disable keys information.
 *
 * Inputs:  i4_argc       Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                       Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_get_glb_disabledkeys(INT32 i4_argc, const CHAR** pps_argv)
{
    DISABLE_EVTCODE_T	t_disablekeys_info;
    SIZE_T            z_keys_loop = 0;
     
    if (i4_argc != 1 || NULL == pps_argv)
    {
         x_dbg_stmt("iom.gdki \n\r");          
         return CLIR_INV_CMD_USAGE;
    }
    
    t_disablekeys_info.pui4_evtcode_arr = NULL;
    t_disablekeys_info.z_evtcode_cnt = x_iom_get_global_disable_evtcode_cnt();
    
    if (0 != t_disablekeys_info.z_evtcode_cnt)
    {
         t_disablekeys_info.pui4_evtcode_arr = (UINT32 *)x_mem_alloc(sizeof(UINT32) * t_disablekeys_info.z_evtcode_cnt);
         if (NULL == t_disablekeys_info.pui4_evtcode_arr ) 
         {
             x_dbg_stmt("allocate memory failed while get disable key info\n\r");
             return CLIR_OUT_OF_MEM;
         }
    
         z_keys_loop = sizeof(DISABLE_EVTCODE_T);
         if(IOMR_OK != x_iom_get_global_disable_evtcode(&(t_disablekeys_info)))
         {
             x_dbg_stmt("get disable keys info failed\n\r");    
             if ( NULL != t_disablekeys_info.pui4_evtcode_arr)
             {
                 x_mem_free(t_disablekeys_info.pui4_evtcode_arr);
             }
             return CLIR_CMD_EXEC_ERROR;
         }
      }
    
      /* output the event filter disable keys info */
      if(t_disablekeys_info.b_enable)
      {
           x_dbg_stmt("Global filter key feature is enabled\n\r");
      }
      else
      {
          x_dbg_stmt("Global filter key feature is disabled\n\r");
      }
    
      if(t_disablekeys_info.b_positive_filter)
      {
          x_dbg_stmt("Global keys is disabled\n\r");
      }
      else
      {
          x_dbg_stmt("Global keys is enabled\n\r");
      }
                             
      x_dbg_stmt("Global keycnt = 0x%08x\n\r", 
                  t_disablekeys_info.z_evtcode_cnt);
       
      if (0 != t_disablekeys_info.z_evtcode_cnt)
      {
           if(t_disablekeys_info.b_positive_filter)
           {
               x_dbg_stmt("         disable key  list:\n\r");
           }
           else
           {
               x_dbg_stmt("         enable key  list:\n\r");
           }
    
           for(z_keys_loop = 0; z_keys_loop < t_disablekeys_info.z_evtcode_cnt; ++ z_keys_loop)
           {
               x_dbg_stmt("         key %d = 0x%08x\n\r", z_keys_loop, *(t_disablekeys_info.pui4_evtcode_arr + z_keys_loop));
           }
       }                    
    
      /* release the memory */
      if ( NULL != t_disablekeys_info.pui4_evtcode_arr)
      {
          x_mem_free(t_disablekeys_info.pui4_evtcode_arr);
      }
       
      return CLIR_OK;

}

#endif

#ifdef IOM_CUSTOM_KEY_REPEAT_INTERVAL_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: _iom_cli_set_custom_key_rep_itvl
 *
 * Description:  This API sets the custom key repeat interval.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_set_custom_key_rep_itvl(INT32 i4_argc,
                                              const CHAR** pps_argv)
{
    if (i4_argc < 3 || (i4_argc - 3) % 3 != 0 || NULL == pps_argv)
    {
        x_dbg_stmt("   iom.ri [append] [unit] [[key1] [1st1] [other1] [key2] [1st2] [other2] ...] \n\r");
        x_dbg_stmt("     [append]: append keys to custom settings or not: 1: append; 0: clean\n\r");
        x_dbg_stmt("     [unit]:   unit of follwing 1st & other intervals in milliseconds\n\r");
        x_dbg_stmt("     [key?]:   event code to customize key repeat interval\n\r");
        x_dbg_stmt("     [1st?]:   custom key repeat interval\n\r");
        x_dbg_stmt("     [other?]: event code to customize key repeat interval\n\r");
        x_dbg_stmt("\n\r   Example:\n\r    iom.ri 0 100  (clean custom setting)\n\r    iom.ri 1 100 0x6f000 5 2\n\r");
        x_dbg_stmt("\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    /* get  the append keys info */
    if (1 == (UINT8)x_strtoll(pps_argv[1], NULL, 0))
    {
    }
    else if(0 == (UINT8)x_strtoll(pps_argv[1], NULL, 0))
    {
        ui4_cust_key_num = 0;
        ui2_unit = 0;
        x_memset(at_cust_key_rep_itvl, 0, sizeof(at_cust_key_rep_itvl));
    }
    else
    {
        x_dbg_stmt("     [append]: append keys to custom settings or not: 1: append; 0: clean\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    if (ui2_unit == 0)
    {
        ui2_unit = (UINT16) x_strtoll(pps_argv[2], NULL, 0);
    }
    else if (ui2_unit != (UINT16) x_strtoll(pps_argv[2], NULL, 0))
    {
        x_dbg_stmt("     [unit]:   unit of follwing 1st & other intervals in milliseconds\n\r");
        x_dbg_stmt("last unit settings is %d, shall give the same", ui2_unit);
        return CLIR_INV_CMD_USAGE;
    }

    while (i4_argc > 3)
    {
        at_cust_key_rep_itvl[ui4_cust_key_num][0] = (UINT32) x_strtoull(pps_argv[3], NULL, 0);
        at_cust_key_rep_itvl[ui4_cust_key_num][1] = (UINT32) x_strtoull(pps_argv[4], NULL, 0);
        at_cust_key_rep_itvl[ui4_cust_key_num][2] = (UINT32) x_strtoull(pps_argv[5], NULL, 0);
        i4_argc -= 3;
        pps_argv += 3;
        ui4_cust_key_num++;
    }

    /* Set the disable keys info */
    if (IOMR_OK != x_iom_set_custom_key_repeat_interval(ui2_unit,
                                                        ui4_cust_key_num,
                                                        at_cust_key_rep_itvl))
    {
        x_dbg_stmt("set custom key repeat interval failed\n\r");
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _iom_cli_get_custom_key_rep_itvl
 *
 * Description:  This API gets the custom key repeat interval information.
 *
 * Inputs:  i4_argc       Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                       Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_get_custom_key_rep_itvl(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT32      ui4_i;

    x_dbg_stmt("Number of custom key repeat interval settings: %d\tUnit: %d ms\n\r",
               ui4_cust_key_num, ui2_unit);
    if (ui4_cust_key_num > 0)
    {
        x_dbg_stmt("\n\rKey event code\tFirst interval\tOther interval\n\r");
        for (ui4_i = 0; ui4_i < ui4_cust_key_num; ui4_i++)
        {
            x_dbg_stmt("  0x%08X  \t  %10d  \t  %10d\n\r",
                       at_cust_key_rep_itvl[ui4_i][0],
                       at_cust_key_rep_itvl[ui4_i][1],
                       at_cust_key_rep_itvl[ui4_i][2]);
        }
    }
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _iom_cli_get_dbg_level
 *
 * Description: This API gets the current setting of IO Manager debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_get_dbg_level(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;

    /* iom.gdl */

    i4_return = x_cli_show_dbg_level(iom_op_get_dbg_level());

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _iom_cli_set_dbg_level
 *
 * Description: This API sets the debug level of IO Manager.
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
static INT32 _iom_cli_set_dbg_level(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    /* iom.sdl [level] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("iom.sdl [level]\n\r");
        x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (iom_op_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }
        else
        {
            i4_return = x_cli_show_dbg_level(iom_op_get_dbg_level());
        }
    }
    else
    {
        x_dbg_stmt("iom.sdl [level]\n\r");
        x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return i4_return;
}

static HANDLE_T        h_iom_uart;
static HANDLE_T        h_fact_test_thread;
static BOOL	       opened = FALSE ;

static BOOL _fact_quit_ctrl(BOOL b_set, BOOL b_ctrl)
{
    static BOOL b_quit = FALSE;
    if (b_set)
    {
        b_quit = b_ctrl;
    }
    return b_quit;
}

static VOID _fact_test_thread(VOID* pv_arg)
{
    UINT32          ui4_mode;
	 CHAR            ac_buf[512];
	   IOM_UART_T      t_iom_uart;
	/*ui4_mode = (IOM_UART_OPERATION_MODE_FACTORY | IOM_UART_OPERATION_MODE_TRANSPARENT);
    x_iom_set(h_iom_uart, IOM_SET_UART_OPERATION_MODE, &ui4_mode, sizeof(ui4_mode));
     x_sprintf(ac_buf, "x_iom_output works in factory mode \n\r");

        t_iom_uart.pui1_data = (UINT8*) ac_buf;
        t_iom_uart.z_len     = x_strlen(ac_buf);

        x_iom_output(h_iom_uart, &t_iom_uart);*/
    while (1)
    {
    	if(_fact_quit_ctrl(FALSE, FALSE) == FALSE)
        {
        	sleep(1);
    	}
		else
		{
            x_sprintf(ac_buf, "x_iom_output works in factory mode \n\r");
            t_iom_uart.pui1_data = (UINT8*) ac_buf;
            t_iom_uart.z_len     = x_strlen(ac_buf);
            x_iom_output(h_iom_uart, &t_iom_uart);
            
			ui4_mode = (IOM_UART_OPERATION_MODE_CLI | IOM_UART_OPERATION_MODE_TRANSPARENT);
			x_iom_set(h_iom_uart, IOM_SET_UART_OPERATION_MODE, &ui4_mode, sizeof(ui4_mode));
			_fact_quit_ctrl(TRUE, FALSE);

		}
			
    }

    ui4_mode = (IOM_UART_OPERATION_MODE_CLI | IOM_UART_OPERATION_MODE_TRANSPARENT);
    x_iom_set(h_iom_uart, IOM_SET_UART_OPERATION_MODE, &ui4_mode, sizeof(ui4_mode));

    printf("printf works again.\n");

    x_iom_close(h_iom_uart);

}
static VOID _fact_mode_iom_nfy_fct(
                    VOID*                       pv_nfy_tag,
                    IOM_NFY_COND_T              e_nfy_cond,
                    UINT32                      ui4_evt_code,
                    UINT32                      ui4_data
                    )
{
    switch (e_nfy_cond)
    {
        case IOM_NFY_COND_REC_UART_DATA:
            {
                IOM_UART_T      *pt_iom_uart;
                pt_iom_uart = (IOM_UART_T *) ui4_data;

                if (pt_iom_uart != NULL)
                {
                    FILE *fp;
                    INT32 i4_i;
                    fp = fopen("/dev/ttyMT3", "w");
                    if (fp != NULL)
                    {
                        fprintf(fp, "\rFactory input:");
                        for (i4_i = 0; i4_i < pt_iom_uart->z_len; i4_i++)
                        {
                            fprintf(fp, " %02X", pt_iom_uart->pui1_data[i4_i]);
                        }
                        fprintf(fp, "\n\r");
                        fclose(fp);
                    }

                    if (x_memcmp(pt_iom_uart->pui1_data, "\x31\x32\x33", pt_iom_uart->z_len > 3 ? 3: pt_iom_uart->z_len) == 0)
                    {
                        _fact_quit_ctrl(TRUE, TRUE);
                    }
                }
                else
                {
                    printf("Invalid input data.\n");
                }
            }
            break;

        case IOM_NFY_COND_XMT_COMPLETE:
            printf("Output data completed.\n");
            break;

        default:
            break;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _iom_cli_test_factory_mode
 *
 * Description: This API tests API for factory mode.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _iom_cli_test_factory_mode(INT32        i4_argc,
                                        const CHAR** pps_argv)
{
    INT32           i4_ret = CLIR_OK;
    UART_SETTING_T  t_uart_setting;
    IOM_UART_T      t_iom_uart;
    //UINT32          ui4_mode;
    MAGIC_T         t_magic;
    MAGIC_UNIT_T    at_magic_unit[2];
    UINT8           aui1_magic_char1[1]={0xCA};
    UINT8           aui1_magic_char2[1]={0xCB};
    CHAR            ac_buf[512];
	
    do
    {
        printf("IOM factory mode API test\n\n");
        printf("Opening device...\n");
        t_uart_setting.e_speed    = IOM_UART_SPEED_115200;
        t_uart_setting.e_data_len = IOM_UART_DATA_LEN_8;
        t_uart_setting.e_parity   = IOM_UART_PARITY_NONE;
        t_uart_setting.e_stop_bit = IOM_UART_STOP_BIT_1;
		if(opened==FALSE)
		{
            i4_ret = x_iom_open(IOM_DEV_TYPE_IO_UART, 
                                7,
                                NULL,
                                (VOID*) &t_uart_setting,
                                NULL,
                                _fact_mode_iom_nfy_fct,
                                &h_iom_uart);

            at_magic_unit[0].z_magic_len = 1;          
            at_magic_unit[0].pui1_magic = aui1_magic_char1;
            at_magic_unit[1].z_magic_len = 1;
            at_magic_unit[1].pui1_magic = aui1_magic_char2;
            t_magic.ui1_magic_units_count = 2;         
            t_magic.pt_magic_unit = at_magic_unit;
            i4_ret = x_iom_set(h_iom_uart, IOM_SET_UART_MAGIC_CHAR, &t_magic, sizeof(t_magic));
            if (i4_ret != IOMR_OK)
            {
                FILE *fp = fopen("/dev/ttyMT0", "w");
                if (fp == NULL)
                {
                    fp = stdout;
                }
                fprintf(fp, "set operation mode failed with %d\n", (int)i4_ret);
                break;
            }
		}
        if (i4_ret != IOMR_OK)
        {
            printf(" UART open fails\n");
            break;
        }
        printf("UART opened. printf works\n");
        x_sprintf(ac_buf, "x_iom_output works\n\r");

        t_iom_uart.pui1_data = (UINT8*) ac_buf;
        t_iom_uart.z_len     = x_strlen(ac_buf);

        i4_ret = x_iom_output(h_iom_uart, &t_iom_uart);
        if (i4_ret != IOMR_OK) { break; }

        //printf("Now enter factory mode. Input hex code 31 32 33 to exit.\n");
        _fact_quit_ctrl(TRUE, FALSE);

        //ui4_mode = (IOM_UART_OPERATION_MODE_FACTORY | IOM_UART_OPERATION_MODE_TRANSPARENT);
        //i4_ret = x_iom_set(h_iom_uart, IOM_SET_UART_OPERATION_MODE, &ui4_mode, sizeof(ui4_mode));
        if (i4_ret != IOMR_OK)
        {
            FILE *fp = fopen("/dev/ttyMT0", "w");
            if (fp == NULL)
            {
                fp = stdout;
            }
            fprintf(fp, "set operation mode failed with %d\n", (int)i4_ret);
            break;
        }

        //printf("Here printf does not work.\n");
        //fprintf(stderr, "Here fprintf stderr does not work.\n");
        //x_iom_output(h_iom_uart, &t_iom_uart);
        if(opened==FALSE)
        {
            i4_ret = x_thread_create(&h_fact_test_thread,
                                 "fact_test",
                                 1024,
                                 220,
                                 _fact_test_thread,
                                 0,
                                 NULL);
            opened=TRUE;
        }
    } while (0);

    return i4_ret;
}

#endif    
#endif


/*-----------------------------------------------------------------------------
 * Name: iom_cli_init
 *
 * Description: This API initializes IO Manager CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_CLI_ERROR              CLI operation is failed.
 ----------------------------------------------------------------------------*/
INT32 iom_cli_init(VOID)
{
#if defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST) || defined(CLI_STRESS_MODE)
    INT32       i4_return;

    /* Attach IO Manager CLI command table to CLI */
    i4_return = x_cli_attach_cmd_tbl(at_iom_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);

    if (i4_return != CLIR_OK)
    {
        return IOMR_CLI_ERROR;
    }
    return IOMR_OK;
#else
    return IOMR_OK;
#endif
}

