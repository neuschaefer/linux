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
 * $RCSfile: cmpb_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor:  $
 * $MD5HEX: d908c026a37c5be69fa4ecb1520bc593 $
 *
 * Description:
 *         This is the source file for common playback adapter cli
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include <stdbool.h>
#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "os/inc/x_os.h"

#include "cmpb_cli.h"
#include "cmpb_util.h"
#include "IMtkPb_Ctrl_DTV.h"
#include "IMtkPb_ErrorCode.h"
#include "IMtkPb_Ctrl.h"
#ifdef INET_SUPPORT
#include "inet/http/x_httpc_api.h"
#endif
#include "file_mngr/x_fm.h"
#include "svctx/mm_hdlr/u_mm_hdlr.h"

/*-----------------------------------------------------------------------------
 * macros, typedefs, enums
 *---------------------------------------------------------------------------*/

#define FourCC(a,b,c,d)    ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))

#define LINUX_PATH_PREFIX   "/mnt/usb/sda1/cmpb/"

#define NUCLEUS_PATH_PREFIX "/mnt/usb/Mass-000/cmpb/"



/************************ch number convert*************************/
#define CONVERT_CH_ENUM_2_VAL(enum_ch, ui4_ch_val)  \
{                                                   \
    switch(enum_ch)                                 \
    {                                               \
    case IMTK_PB_CTRL_AUD_CH_MONO:                  \
        ui4_ch_val = 1;                             \
        break;                                      \
    case IMTK_PB_CTRL_AUD_CH_STEREO:                \
        ui4_ch_val = 2;                             \
        break;                                      \
    case IMTK_PB_CTRL_AUD_CH_SURROUND_2CH:          \
        ui4_ch_val = 2;                             \
        break;                                      \
    case IMTK_PB_CTRL_AUD_CH_SURROUND:              \
        ui4_ch_val = 2;                             \
        break;                                      \
    case IMTK_PB_CTRL_AUD_CH_3_0:                   \
        ui4_ch_val = 3;                             \
        break;                                      \
    case IMTK_PB_CTRL_AUD_CH_4_0:                   \
        ui4_ch_val = 4;                             \
        break;                                      \
    case IMTK_PB_CTRL_AUD_CH_5_0:                   \
        ui4_ch_val = 5;                             \
        break;                                      \
    case IMTK_PB_CTRL_AUD_CH_5_1:                   \
        ui4_ch_val = 6;                             \
        break;                                      \
    case IMTK_PB_CTRL_AUD_CH_7_1:                   \
        ui4_ch_val = 8;                             \
        break;                                      \
    default:                                        \
        ui4_ch_val = 2;                             \
        break;                                      \
    }                                               \
}

#define CONVERT_CH_VAL_2_ENUM(ui4_ch_val, enum_ch)  \
{                                                   \
    switch(ui4_ch_val)                              \
    {                                               \
    case 1:                                         \
        enum_ch = IMTK_PB_CTRL_AUD_CH_MONO;         \
        break;                                      \
    case 2:                                         \
        enum_ch = IMTK_PB_CTRL_AUD_CH_STEREO;       \
        break;                                      \
    case 3:                                         \
        enum_ch = IMTK_PB_CTRL_AUD_CH_3_0;          \
        break;                                      \
    case 4:                                         \
        enum_ch = IMTK_PB_CTRL_AUD_CH_4_0;          \
        break;                                      \
    case 5:                                         \
        enum_ch = IMTK_PB_CTRL_AUD_CH_5_0;          \
        break;                                      \
    case 6:                                         \
        enum_ch = IMTK_PB_CTRL_AUD_CH_5_1;          \
        break;                                      \
    case 7:                                         \
        enum_ch = IMTK_PB_CTRL_AUD_CH_7_1;          \
        break;                                      \
    default:                                        \
        enum_ch = IMTK_PB_CTRL_AUD_CH_STEREO;       \
        break;                                      \
    }                                               \
}


/************************sample rate convert*************************/   
#define CONVERT_SR_ENUM_2_VAL(enum_sr, ui4_sr_val)  \
{                                                   \
    switch(enum_sr)                                 \
    {                                               \
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_8K:           \
        ui4_sr_val = 8000;                          \
        break;                                      \
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_16K:          \
        ui4_sr_val = 16000;                         \
        break;                                      \
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_32K:          \
        ui4_sr_val = 32000;                         \
        break;                                      \
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_11K:          \
        ui4_sr_val = 11025;                         \
        break;                                      \
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_22K:          \
        ui4_sr_val = 22050;                         \
        break;                                      \
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_44K:          \
        ui4_sr_val = 44100;                         \
        break;                                      \
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_12K:          \
        ui4_sr_val = 12000;                         \
        break;                                      \
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_24K:          \
        ui4_sr_val = 24000;                         \
        break;                                      \
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_48K:          \
        ui4_sr_val = 48000;                         \
        break;                                      \
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_96K:          \
        ui4_sr_val = 96000;                         \
        break;                                      \
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_192K:         \
        ui4_sr_val = 192000;                        \
        break;                                      \
    default:                                        \
        ui4_sr_val = 44100;                         \
        break;                                      \
    }                                               \
}

#define CONVERT_SR_VAL_2_ENUM(ui4_sr_val, enum_sr)  \
{                                                   \
    switch(ui4_sr_val / 1000)                       \
    {                                               \
    case 8:                                         \
        enum_sr = IMTK_PB_CTRL_AUD_SAMPLE_RATE_8K;  \
        break;                                      \
    case 11:                                        \
        enum_sr = IMTK_PB_CTRL_AUD_SAMPLE_RATE_11K; \
        break;                                      \
    case 12:                                        \
        enum_sr = IMTK_PB_CTRL_AUD_SAMPLE_RATE_12K; \
        break;                                      \
    case 16:                                        \
        enum_sr = IMTK_PB_CTRL_AUD_SAMPLE_RATE_16K; \
        break;                                      \
    case 22:                                        \
        enum_sr = IMTK_PB_CTRL_AUD_SAMPLE_RATE_22K; \
        break;                                      \
    case 24:                                        \
        enum_sr = IMTK_PB_CTRL_AUD_SAMPLE_RATE_24K; \
        break;                                      \
    case 32:                                        \
        enum_sr = IMTK_PB_CTRL_AUD_SAMPLE_RATE_32K; \
        break;                                      \
    case 44:                                        \
        enum_sr = IMTK_PB_CTRL_AUD_SAMPLE_RATE_44K; \
        break;                                      \
    case 48:                                        \
        enum_sr = IMTK_PB_CTRL_AUD_SAMPLE_RATE_48K; \
        break;                                      \
    case 96:                                        \
        enum_sr = IMTK_PB_CTRL_AUD_SAMPLE_RATE_96K; \
        break;                                      \
    case 192:                                       \
        enum_sr = IMTK_PB_CTRL_AUD_SAMPLE_RATE_192K;\
        break;                                      \
    default:                                        \
        enum_sr = IMTK_PB_CTRL_AUD_SAMPLE_RATE_44K; \
        break;                                      \
    }                                               \
}

/************************bit depth convert*************************/   
#define CONVERT_BD_ENUM_2_VAL(enum_bd, ui4_bd_val)  \
{                                                   \
    switch(enum_bd)                                 \
    {                                               \
    case IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_8:          \
        ui4_bd_val = 8;                             \
        break;                                      \
    case IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_16:         \
        ui4_bd_val = 16;                            \
        break;                                      \
    case IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_20:         \
        ui4_bd_val = 20;                            \
        break;                                      \
    case IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_24:         \
        ui4_bd_val = 24;                            \
        break;                                      \
    default:                                        \
        ui4_bd_val = 16;                            \
        break;                                      \
    }                                               \
}

#define CONVERT_BD_VAL_2_ENUM(ui4_bd_val, enum_bd)  \
{                                                   \
    switch(ui4_bd_val)                              \
    {                                               \
    case 8:                                         \
        enum_bd = IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_8; \
        break;                                      \
    case 16:                                        \
        enum_bd = IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_16;\
        break;                                      \
    case 20:                                        \
        enum_bd = IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_20;\
        break;                                      \
    case 24:                                        \
        enum_bd = IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_24;\
        break;                                      \
    default:                                        \
        enum_bd = IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_16;\
        break;                                      \
    }                                               \
}

/*-----------------------------------------------------------------------------
 *                   data declarations
 ----------------------------------------------------------------------------*/
#define PULL_SUPPORT
/*#define PULL_INET_TEST*/
//#define TEST_SET_MODE

#ifdef PULL_SUPPORT
CHAR g_aui1_test_text[200]; 
IMtkPb_Ctrl_Pull_Nfy_Fct    g_pfnNotify = NULL;
#endif

#ifdef TEST_SET_MODE
extern BOOL g_is_get_info;
extern BOOL g_is_using_info;
#endif

#ifdef CLI_LVL_ALL
UINT16 g_cmpb_dbg_level = DBG_LEVEL_ERROR;

static INT32 _cmpb_test_replay(VOID);

static INT32 _cmpb_cli_set_dbg_lvl(
                        INT32        i4_argc,
                        const CHAR** pps_argv);

static INT32 _cmpb_cli_get_dbg_lvl(
                        INT32        i4_argc,
                        const CHAR** pps_argv);                        

static INT32 _cmpb_cli_openURI(
                        INT32        i4_argc,
                        const CHAR** pps_argv);   
#ifdef PULL_SUPPORT
static INT32 _cmpb_cli_openPULL(
                        INT32        i4_argc,
                        const CHAR** pps_argv);   
#endif
static INT32 _cmpb_cli_openPUSH(
                        INT32        i4_argc,
                        const CHAR** pps_argv);   

static INT32 _cmpb_cli_close(
                        INT32        i4_argc,
                        const CHAR** pps_argv);   
                        
static INT32 _cmpb_cli_play(
                        INT32        i4_argc,
                        const CHAR** pps_argv);         

static INT32 _cmpb_cli_stop(
                        INT32        i4_argc,
                        const CHAR** pps_argv);

static INT32 _cmpb_cli_pause(
                        INT32        i4_argc,
                        const CHAR** pps_argv);

static INT32 _cmpb_cli_step(
                        INT32        i4_argc,
                        const CHAR** pps_argv);


static INT32 _cmpb_cli_ff(
                        INT32        i4_argc,
                        const CHAR** pps_argv);

static INT32 _cmpb_cli_csub(
                INT32        i4_argc,
                const CHAR** pps_argv);

static INT32 _cmpb_cli_caud(
                INT32        i4_argc,
                const CHAR** pps_argv);

static INT32 _cmpb_cli_fr(
                        INT32        i4_argc,
                        const CHAR** pps_argv);

static INT32 _cmpb_cli_bseek(
                        INT32        i4_argc,
                        const CHAR** pps_argv);

static INT32 _cmpb_cli_tseek(
                        INT32        i4_argc,
                        const CHAR** pps_argv);

static INT32 _cmpb_cli_get_buffer_fullness(
                        INT32        i4_argc,
                        const CHAR** pps_argv); 

static INT32 _cmpb_cli_preload(
                        INT32        i4_argc,
                        const CHAR** pps_argv);                        
                        
static INT32 _cmpb_cli_push_send_buffer(
                        INT32        i4_argc,
                        const CHAR** pps_argv);

static INT32 _cmpb_cli_capture(
                        INT32        i4_argc,
                        const CHAR** pps_argv);
                        
static CLI_EXEC_T at_cmpb_cmd_tbl[] = 
{
    {CLI_GET_DBG_LVL_STR,   NULL,   _cmpb_cli_get_dbg_lvl,  NULL,   "get debug level",  CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,   _cmpb_cli_set_dbg_lvl,  NULL,   "set debug level",  CLI_SUPERVISOR},       
    {"openuri",             NULL,   _cmpb_cli_openURI,      NULL,   "open URI",         CLI_SUPERVISOR},    
#ifdef PULL_SUPPORT    
    {"openpull",            NULL,   _cmpb_cli_openPULL,     NULL,   "open PULL",        CLI_SUPERVISOR}, 
#endif
    {"openpush",            NULL,   _cmpb_cli_openPUSH,     NULL,   "open PUSH",        CLI_SUPERVISOR},
    {"close",               NULL,   _cmpb_cli_close,        NULL,   "close",            CLI_SUPERVISOR},
    {"preload",             NULL,   _cmpb_cli_preload,      NULL,   "preload test",     CLI_SUPERVISOR},
    {"buffer",              NULL,   _cmpb_cli_get_buffer_fullness,  NULL,   "get buffer fullness",     CLI_SUPERVISOR},
    {"play",                NULL,   _cmpb_cli_play,         NULL,   "play test",        CLI_SUPERVISOR},
    {"stop",                NULL,   _cmpb_cli_stop,         NULL,   "stop test",        CLI_SUPERVISOR},
    {"pause",               NULL,   _cmpb_cli_pause,        NULL,   "pause test",       CLI_SUPERVISOR},
    {"step",                NULL,   _cmpb_cli_step,         NULL,   "step test",        CLI_SUPERVISOR},
    {"ff",                  NULL,   _cmpb_cli_ff,           NULL,   "fast forward test",CLI_SUPERVISOR},
    {"fr",                  NULL,   _cmpb_cli_fr,           NULL,   "fast rewind test", CLI_SUPERVISOR},
    {"csub",                NULL,   _cmpb_cli_csub,         NULL,   "change subtitle",  CLI_SUPERVISOR},
    {"caud",                NULL,   _cmpb_cli_caud,         NULL,   "change audio",     CLI_SUPERVISOR},
    {"bseek",               NULL,   _cmpb_cli_bseek,        NULL,   "byte seek",        CLI_SUPERVISOR},
    {"tseek",               NULL,   _cmpb_cli_tseek,        NULL,   "time seek",        CLI_SUPERVISOR},
    {"sendbuf",             NULL,   _cmpb_cli_push_send_buffer,           NULL,   "push send buffer", CLI_SUPERVISOR},
    {"capture",             NULL,   _cmpb_cli_capture,      NULL,   "capture on/off",  CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};


static CLI_EXEC_T at_cmpb_root_cmd_tbl[] =
{
	  {"cmpb",    NULL,   NULL,   at_cmpb_cmd_tbl,    "common playback Interface",    CLI_SUPERVISOR},
	  END_OF_CLI_CMD_TBL
};
#endif
/*-----------------------------------------------------------------------------
 *                   functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *                   Internal functions implementations
 -----------------------------------------------------------------------------*/

UINT16 cmpb_get_dbg_level(VOID)
{
#ifdef CLI_LVL_ALL	
    return g_cmpb_dbg_level;
#else
	return 0;
#endif    
}

BOOL   cmpb_set_dbg_level(UINT16 ui2_level)
{
#ifdef CLI_LVL_ALL	
    g_cmpb_dbg_level = ui2_level;
    return TRUE;
#else    
   
    return FALSE;
#endif
}

INT32 cmpb_cli_init(VOID)
{

#ifdef CLI_LVL_ALL
    INT32       i4_ret;    
    
    /* Attach Device Manager CLI command table to CLI */
    i4_ret = x_cli_attach_cmd_tbl(at_cmpb_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);
    
    if (i4_ret != CLIR_OK)
    {
        return CMPBR_CLI_ERR;
    }
    
    return CMPBR_OK;
#else
    return CMPBR_OK;
#endif

}

#ifdef CLI_LVL_ALL
static INT32 _cmpb_cli_get_dbg_lvl(
                        INT32        i4_argc,
                        const CHAR** pps_argv)
{   
    if (i4_argc != 1)
    {
        x_dbg_stmt("cmpb.sdl\n\r");
        return CLIR_OK;
    }    

    return x_cli_show_dbg_level(cmpb_get_dbg_level());
}

static INT32 _cmpb_cli_set_dbg_lvl(
                        INT32        i4_argc,
                        const CHAR** pps_argv)
{
    INT32       i4_ret;
    UINT16      ui2_dbg_level;     
   
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("dm.sdl [level]\n\r");
        x_dbg_stmt("[level]: n=none, e=error, a=api, i=info");
        return CLIR_OK;
    }    
	    
    i4_ret = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);
    
    if (i4_ret == CLIR_OK)
    {
        if (cmpb_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_ret = CLIR_CMD_EXEC_ERROR;  
        }
    }
    else
    {
        x_dbg_stmt("cmpb.sdl [level]\n\r");
        x_dbg_stmt("[level]: n=none, e=error, a=api, i=info");
        i4_ret = CLIR_OK;
    }

    x_dbg_stmt("set debug level %s\n\r", (i4_ret == CLIR_OK) ? "successful" : "failed");
    return i4_ret;
}

static IMTK_PB_HANDLE_T hPB_Handle = 0;
static UINT32 ui4_tag = 0x12341234;
BOOL b_pb_info_ready = FALSE;
static BOOL b_print_timecode = FALSE;
static UINT32 ui4_repeat_play_cnt    = 0;
static CHAR g_media_path[256] = {0};
IMTK_PB_CTRL_BUFFERING_MODEL_T g_cur_mode = IMTK_PB_CTRL_BUFFERING_MODEL_PULL;
static BOOL b_is_eos = FALSE;

static char g_meta_str[256] = {0};

static UINT32 _get_fmt_type(const CHAR*  ps_fmt)
{
    UINT32  ui4_fmt_type = 0;
    UINT8   ui1_i        = 0;

    for(ui1_i = 0; ui1_i<4; ui1_i++)
    {
        if(*ps_fmt)
        {
            ui4_fmt_type = (ui4_fmt_type << 8) | *ps_fmt;
            ps_fmt++;
        }
        else
        {
            ui4_fmt_type = (ui4_fmt_type << 8);
        }
    }
    return ui4_fmt_type;
}

static IMTK_PB_CB_ERROR_CODE_T pb_cb(IMTK_PB_CTRL_EVENT_T       eEventType,
                                     void*                      pvTag,
                                     uint32_t                   u4Data)
{
    IMTK_PB_ERROR_CODE_T e_ret = IMTK_PB_ERROR_CODE_OK;
    switch (eEventType)
    {
        /*case IMTK_PB_CTRL_EVENT_BUFFER_READY:
            x_dbg_stmt("received message IMTK_PB_CTRL_EVENT_BUFFER_READY...\n");
            break;*/
        case IMTK_PB_CTRL_EVENT_CUR_TIME_UPDATE:            
            if(b_print_timecode)
            {
                if(0 == u4Data)
                {
                    IMTK_PB_ERROR_CODE_T        e_ret_code = IMTK_PB_ERROR_CODE_NOT_OK;
                    uint32_t  ui4_cur_time;
                    uint64_t  ui8_cur_pos;
                    e_ret_code = IMtkPb_Ctrl_GetCurrentPos(hPB_Handle,
                                              &ui4_cur_time,   
                                              &ui8_cur_pos);

                    x_dbg_stmt("ui4_cur_time is %d, ui8_cur_pos is %d, ret = %d \n", ui4_cur_time, ui8_cur_pos, e_ret_code);
                }
                else
                {
                    x_dbg_stmt("time updated: %d \n", u4Data);
                }
            }
            
            break;

        /*case IMTK_PB_CTRL_EVENT_PRE_PARSE_DONE:
            IMtkPb_Ctrl_GetMediaInfo(hPB_Handle, &(t_media_info));
            b_pb_info_ready = TRUE;
            break;*/

        case IMTK_PB_CTRL_EVENT_BUFFER_UNDERFLOW:            
            x_dbg_stmt("received message IMTK_PB_CTRL_EVENT_BUFFER_UNDERFLOW \n");
            break;

        case IMTK_PB_CTRL_EVENT_TOTAL_TIME_UPDATE:
            break;
            
        case IMTK_PB_CTRL_EVENT_EOS:
            b_is_eos = TRUE;
            break;
        case IMTK_PB_CTRL_EVENT_PLAY_DONE:
        {
            IMTK_PB_CTRL_META_TYPE_T e_type;
            IMTK_PB_CTRL_META_DATA_INFO_T t_meta_info;

            x_dbg_stmt("received message IMTK_PB_CTRL_EVENT_PLAY_DONE \n");
            x_memset(&t_meta_info, 0, sizeof(t_meta_info));
            for(e_type = (IMTK_PB_CTRL_META_TYPE_T)0; e_type < IMTK_PB_CTRL_META_TYPE_NEXT_ARTIST; e_type ++)
            {
                t_meta_info.e_meta_type = e_type;
                t_meta_info.pv_buf      = g_meta_str;
                t_meta_info.ui2_buf_size = 256;
                e_ret = IMtkPb_Ctrl_Get(hPB_Handle,
                                         IMTK_PB_CTRL_GET_TYPE_META_DATA,
                                         &t_meta_info,
                                         sizeof(t_meta_info));
                if(e_ret != IMTK_PB_ERROR_CODE_OK)
                {
                    x_dbg_stmt("get meta info, type is %d, ret is %d \n ", e_type, e_ret);
                }
                else
                {
                    x_dbg_stmt("get meta info, type is %d, string is %s \n ", e_type, g_meta_str);
                }
                g_meta_str[0] = 0;
            }
        }
            break;

        default :
            break;
    }
    
    return IMTK_PB_CB_ERROR_CODE_OK;
}


static INT32 _cmpb_cli_openURI(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    INT32                   i4_ret = CLIR_OK;
    IMTK_PB_ERROR_CODE_T    e_return;
    //IMTK_PB_CTRL_URI_INFO_T t_info;
    IMTK_PB_CTRL_ENGINE_PARAM_T   t_parm;
    
    if (((i4_argc != 2) && (i4_argc != 3)) || (pps_argv == NULL))
    {
        x_dbg_stmt("cmpb.openuri URI [1]\n\r");
        return CLIR_OK;
    }    

#ifdef TEST_SET_MODE
    g_is_using_info = FALSE;
    if(i4_argc == 3)
    {
        if(x_strncasecmp(pps_argv[2], "1", 1) == 0)
        {
            g_is_get_info = TRUE;
            g_is_using_info = TRUE;
        }        
    }
#endif
    b_pb_info_ready = FALSE;
#ifdef TEST_SET_MODE
    if(g_is_using_info)
    {
        e_return = IMtkPb_Ctrl_Open(&hPB_Handle,
                                IMTK_PB_CTRL_BUFFERING_MODEL_URI,
                                IMTK_PB_CTRL_APP_MASTER, NULL);
    }
    else
#endif
    {
        e_return = IMtkPb_Ctrl_Open(&hPB_Handle,
                                IMTK_PB_CTRL_BUFFERING_MODEL_URI,
                                IMTK_PB_CTRL_LIB_MASTER, NULL);
    }
                                
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_Open() Failed !\n");
        return CLIR_INV_ARG;        
    }
    g_cur_mode = IMTK_PB_CTRL_BUFFERING_MODEL_URI;
    t_parm.uBufferModelParam.tUriInfo.u4URI_len = x_strlen(pps_argv[1]);
    if (t_parm.uBufferModelParam.tUriInfo.u4URI_len <=0)
    {
        return CLIR_INV_ARG; 
    }

    x_strcpy(g_media_path, pps_argv[1]);
    e_return = IMtkPb_Ctrl_RegCallback(hPB_Handle,
                            (VOID*)(&ui4_tag),
                            pb_cb
                            );
                            
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_RegCallback() Failed !\n");
        return CLIR_INV_ARG;        
    }                        

    t_parm.u4PlayFlag = IMTK_PB_CTRL_PLAY_FLAG_VIDEO |
                        IMTK_PB_CTRL_PLAY_FLAG_AUDIO;
    t_parm.uBufferModelParam.tUriInfo.pu1URI = (UINT8 *)(pps_argv[1]);
    t_parm.uBufferModelParam.tUriInfo.eBufSizeType = IMTK_PB_CTRL_BUF_SIZE_TYPE_BYTE;
    t_parm.uBufferModelParam.tUriInfo.uBufSize.u4Bytes= 2*1024*1024;
    t_parm.uBufferModelParam.tUriInfo.u4KeepBufThreshold = 2048 * 1024 * 10 / 100;          //10;
    t_parm.uBufferModelParam.tUriInfo.u4ReBufThreshold = 2048 * 1024 * 90 / 100;  //90;
        
    e_return = IMtkPb_Ctrl_SetEngineParam(hPB_Handle,
                                            &t_parm
                                            );

    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_SetEngineParam() Failed !\n");
        return CLIR_INV_ARG;        
    }
    //IMtkPb_Ctrl_GetMediaInfo(hPB_Handle, &(t_media_info));
    b_pb_info_ready = TRUE;

    if(i4_argc == 3)
    {
        b_pb_info_ready = TRUE;
    }

    return i4_ret;
}

#ifdef PULL_SUPPORT
static VOID _fm_async_fct(
                    HANDLE_T        h_req,
                    VOID            *pv_tag,
                    FM_ASYNC_COND_T e_async_cond,
                    UINT32          ui4_data)
{
#if 0
    IMTK_PB_CTRL_PULL_EVENT_T   eEventType;
    void*                       pvTag = pv_tag;
    uint32_t                    u4ReqId = h_req;
    uint32_t                    u4Param = ui4_data;

    switch(e_async_cond)
    {
    case FM_ASYNC_COND_ABORT_FAIL:
        eEventType = IMTK_PB_CTRL_PULL_ABORT_FAIL;
        break;
    case FM_ASYNC_COND_READ_OK:
        eEventType = IMTK_PB_CTRL_PULL_READ_OK;
        break;
    case FM_ASYNC_COND_ABORT_OK:
        eEventType = IMTK_PB_CTRL_PULL_ABORT_OK;
        break;
    case FM_ASYNC_COND_EOF:
        eEventType = IMTK_PB_CTRL_PULL_READ_EOS;
        break;
    default:
        eEventType = IMTK_PB_CTRL_PULL_FAIL;
        break;
    }
    if(g_pfnNotify != NULL)
    {
        g_pfnNotify(eEventType,
                    pvTag,
                    u4ReqId,
                    u4Param);
    }
#endif
}

IMTK_PB_CB_ERROR_CODE_T  _fm_pull_open(IMTK_PB_HANDLE_T           hHandle,
                                       IMTK_PULL_HANDLE_T*        phPullSrc,
                                       void*                      pvAppTag)
{
    INT32 i4_ret = FMR_OK;
    
    i4_ret = x_fm_open(FM_ROOT_HANDLE,
                       g_aui1_test_text,
                       FM_READ_ONLY,
                       FM_MODE_USR_READ,
                       FALSE,
                       (HANDLE_T*)phPullSrc);
    return ((i4_ret == FMR_OK) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

IMTK_PB_CB_ERROR_CODE_T  _fm_pull_close(IMTK_PULL_HANDLE_T         hPullSrc,
                                        void*                      pvAppTag)
{
    INT32 i4_ret = FMR_OK;
    
    i4_ret = x_fm_close(hPullSrc);
    
    return ((i4_ret == FMR_OK) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

IMTK_PB_CB_ERROR_CODE_T  _fm_pull_read(IMTK_PULL_HANDLE_T     hPullSrc,
                                       void*                  pvAppTag,
                                       uint8_t*               pu1DstBuf,
                                       uint32_t               u4Count, 
                                       uint32_t*              pu4Read)
{
    INT32 i4_ret = FMR_OK;
    
    i4_ret = x_fm_read(hPullSrc, 
                       (UINT8*)pu1DstBuf, 
                       (UINT32)u4Count, 
                       (UINT32*)pu4Read);
    return ((i4_ret == FMR_OK || i4_ret == FMR_EOF) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

IMTK_PB_CB_ERROR_CODE_T _fm_pull_read_async(IMTK_PULL_HANDLE_T         hPullSrc,
                                            void*                      pvAppTag,
                                            uint8_t*                   pu1Dst,
                                            uint32_t                   u4DataLen,
                                            IMtkPb_Ctrl_Pull_Nfy_Fct   pfnNotify,
                                            void*                      pvRdAsyTag,   
                                            uint32_t*                  pu4ReqId)
{
    INT32 i4_ret;
    
    g_pfnNotify = pfnNotify;
    i4_ret = x_fm_read_async(hPullSrc,
                             (UINT8*)pu1Dst,
                             (UINT32)u4DataLen,
                             128,
                             _fm_async_fct,//NULL,//(x_fm_async_fct*)pfnNotify,
                             pvRdAsyTag,
                             (HANDLE_T*)pu4ReqId);
    return ((i4_ret == FMR_OK || i4_ret == FMR_EOF) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

IMTK_PB_CB_ERROR_CODE_T _fm_pull_abort_read_async(IMTK_PULL_HANDLE_T     hPullSrc,
                                                  void*                  pvAppTag,
                                                  uint32_t               u4ReqId)
{
    INT32 i4_ret;
    
    i4_ret = x_fm_abort_async(hPullSrc);
    
    return ((i4_ret == FMR_OK) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

IMTK_PB_CB_ERROR_CODE_T _fm_pull_byteseek(IMTK_PULL_HANDLE_T         hPullSrc,
                                          void*                      pvAppTag,
                                          int64_t                    i8SeekPos,
                                          uint8_t                    u1Whence,
                                          uint64_t*                  pu8CurPos)
{
    INT32 i4_ret;
    
    i4_ret = x_fm_lseek(hPullSrc, 
                        i8SeekPos, 
                        u1Whence, 
                        (UINT64*)pu8CurPos);
    return ((i4_ret == FMR_OK) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

IMTK_PB_CB_ERROR_CODE_T _fm_pull_get_input_len(IMTK_PULL_HANDLE_T  hPullSrc,
                                               void*               pvAppTag,
                                               uint64_t*           pu8Len)
{
    INT32 i4_ret;
    
    i4_ret = x_fm_lseek(hPullSrc, 0, FM_SEEK_END, pu8Len);
    
    return ((i4_ret == FMR_OK) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

#ifdef PULL_INET_TEST
IMTK_PB_CB_ERROR_CODE_T  _httpc_fm_pull_open(IMTK_PB_HANDLE_T           hHandle,
                                             IMTK_PULL_HANDLE_T*        phPullSrc,
                                             void*                      pvAppTag)
{
    INT32 i4_ret = FMR_OK;
    
    i4_ret = x_httpc_fm_open_proxy(g_aui1_test_text,
                                   NULL,
                                   0,
                                   (HANDLE_T*)phPullSrc);
    return ((i4_ret == FMR_OK) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

IMTK_PB_CB_ERROR_CODE_T  _httpc_fm_pull_close(IMTK_PULL_HANDLE_T         hPullSrc,
                                              void*                      pvAppTag)
{
    INT32 i4_ret = FMR_OK;
    
    i4_ret = x_httpc_fm_close(hPullSrc);
    
    return ((i4_ret == FMR_OK) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

IMTK_PB_CB_ERROR_CODE_T  _httpc_fm_pull_read(IMTK_PULL_HANDLE_T     hPullSrc,
                                             void*                  pvAppTag,
                                             uint8_t*               pu1DstBuf,
                                             uint32_t               u4Count,
                                             uint32_t*              pu4Read)
{
    INT32 i4_ret = FMR_OK;
    
    i4_ret = x_httpc_fm_read(hPullSrc, 
                             (UINT8*)pu1DstBuf, 
                             (UINT32)u4Count, 
                             (UINT32*)pu4Read);
    return ((i4_ret == FMR_OK || i4_ret == FMR_EOF) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

IMTK_PB_CB_ERROR_CODE_T _httpc_fm_pull_read_async(IMTK_PULL_HANDLE_T         hPullSrc,
                                                  void*                      pvAppTag,
                                                  uint8_t*                   pu1Dst,
                                                  uint32_t                   u4DataLen,
                                                  IMtkPb_Ctrl_Pull_Nfy_Fct   pfnNotify,
                                                  void*                      pvRdAsyTag,   
                                                  uint32_t*                  pu4ReqId)
{
    INT32 i4_ret;
    g_pfnNotify = pfnNotify;
    i4_ret = x_httpc_fm_read_async(hPullSrc,
                                   (UINT8*)pu1Dst,
                                   (UINT32)u4DataLen,
                                   128,
                                   _fm_async_fct,//NULL,//(x_fm_async_fct*)pfnNotify,
                                   pvRdAsyTag,
                                   (UINT32*)pu4ReqId);
    return ((i4_ret == FMR_OK || i4_ret == FMR_EOF) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

IMTK_PB_CB_ERROR_CODE_T _httpc_fm_pull_abort_read_async(IMTK_PULL_HANDLE_T     hPullSrc,
                                                        void*                  pvAppTag,
                                                        uint32_t               u4ReqId)
{
    INT32 i4_ret;
    
    i4_ret = x_httpc_fm_abort_async(hPullSrc);
    return ((i4_ret == FMR_OK) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

IMTK_PB_CB_ERROR_CODE_T _httpc_fm_pull_byteseek(IMTK_PULL_HANDLE_T         hPullSrc,
                                                void*                      pvAppTag,
                                                int64_t                   i8SeekPos,
                                                uint8_t                    u1Whence,
                                                uint64_t*                  pu8CurPos)
{
    INT32 i4_ret;
    i4_ret = x_httpc_fm_lseek(hPullSrc, 
                              i8SeekPos, 
                              u1Whence, 
                              (UINT64*)pu8CurPos);
    return ((i4_ret == FMR_OK) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}

IMTK_PB_CB_ERROR_CODE_T _httpc_fm_pull_get_input_len(IMTK_PULL_HANDLE_T  hPullSrc,
                                                     void*               pvAppTag,
                                                     uint64_t*           pu8Len)
{
    INT32 i4_ret;
    
    i4_ret = x_httpc_fm_lseek(hPullSrc, 0, FM_SEEK_END, pu8Len);
    
    return ((i4_ret == FMR_OK) ? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
}
#endif

static INT32 _cmpb_cli_openPULL(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
#ifdef PULL_INET_TEST
#ifdef INET_SUPPORT
    BOOL                        b_is_network = FALSE;
#endif
#endif
    INT32                       i4_ret = CLIR_OK;
    IMTK_PB_ERROR_CODE_T        e_return;
    IMTK_PB_CTRL_ENGINE_PARAM_T   t_parm;
    
    if (((i4_argc != 2) && (i4_argc != 3)) || (pps_argv == NULL))
    {
        x_dbg_stmt("cmpb.openpull URI [1]\n\r");
        return CLIR_OK;
    }    

#ifdef TEST_SET_MODE
    g_is_using_info = FALSE;
    if(i4_argc == 3)
    {
        if(x_strncasecmp(pps_argv[2], "1", 1) == 0)
        {
            g_is_get_info = TRUE;
            g_is_using_info = TRUE;
        }        
    }
#endif
#ifdef PULL_INET_TEST
#ifdef INET_SUPPORT
    if(x_strncasecmp(pps_argv[1], "http:", 5) == 0)
    {
        b_is_network = TRUE;
    }
    else
    {
        b_is_network = FALSE;
    }
#endif
#endif
    /*g_aui1_test_text = x_mem_alloc(x_strlen(pps_argv[1]) + 1);*/
    x_memset(g_aui1_test_text, 0, x_strlen(pps_argv[1]));
    x_strcpy(g_aui1_test_text, pps_argv[1]);
    x_strcpy(g_media_path, pps_argv[1]);

    b_pb_info_ready = FALSE;
#ifdef TEST_SET_MODE
    if(g_is_using_info)
    {
        e_return = IMtkPb_Ctrl_Open(&hPB_Handle,
                                IMTK_PB_CTRL_BUFFERING_MODEL_PULL,
                                IMTK_PB_CTRL_APP_MASTER, NULL);
    }
    else    
#endif
    {
        e_return = IMtkPb_Ctrl_Open(&hPB_Handle,
                                IMTK_PB_CTRL_BUFFERING_MODEL_PULL,
                                IMTK_PB_CTRL_LIB_MASTER, NULL);
    }
                                
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_Open() Failed !\n");
        return CLIR_INV_ARG;        
    }
    g_cur_mode = IMTK_PB_CTRL_BUFFERING_MODEL_PULL;
    e_return = IMtkPb_Ctrl_RegCallback(hPB_Handle,
                            (VOID*)(&ui4_tag),
                            pb_cb
                            );
                            
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_RegCallback() Failed !\n");
        return CLIR_INV_ARG;        
    }                        
    t_parm.u4PlayFlag = IMTK_PB_CTRL_PLAY_FLAG_VIDEO | 
                        IMTK_PB_CTRL_PLAY_FLAG_AUDIO;
#ifdef PULL_INET_TEST
#ifdef INET_SUPPORT
    if(b_is_network)
    {
        t_parm.uBufferModelParam.tPullInfo.pvAppTag = (void*)0x12345678;
        t_parm.uBufferModelParam.tPullInfo.pfnOpen = _httpc_fm_pull_open;
        t_parm.uBufferModelParam.tPullInfo.pfnClose = _httpc_fm_pull_close;
        t_parm.uBufferModelParam.tPullInfo.pfnRead = _httpc_fm_pull_read;
        t_parm.uBufferModelParam.tPullInfo.pfnReadAsync = _httpc_fm_pull_read_async;
        t_parm.uBufferModelParam.tPullInfo.pfnAbortReadAsync = _httpc_fm_pull_abort_read_async;
        t_parm.uBufferModelParam.tPullInfo.pfnByteSeek = _httpc_fm_pull_byteseek;
        t_parm.uBufferModelParam.tPullInfo.pfnGetInputLen = _httpc_fm_pull_get_input_len;
    }
    else
#endif
#endif
    {
        t_parm.uBufferModelParam.tPullInfo.pvAppTag = (void*)0x87654321;
        t_parm.uBufferModelParam.tPullInfo.pfnOpen = _fm_pull_open;
        t_parm.uBufferModelParam.tPullInfo.pfnClose = _fm_pull_close;
        t_parm.uBufferModelParam.tPullInfo.pfnRead = _fm_pull_read;
        t_parm.uBufferModelParam.tPullInfo.pfnReadAsync = _fm_pull_read_async;
        t_parm.uBufferModelParam.tPullInfo.pfnAbortReadAsync = _fm_pull_abort_read_async;
        t_parm.uBufferModelParam.tPullInfo.pfnByteSeek = _fm_pull_byteseek;
        t_parm.uBufferModelParam.tPullInfo.pfnGetInputLen = _fm_pull_get_input_len;
    }
        
    e_return = IMtkPb_Ctrl_SetEngineParam(hPB_Handle,
                                            &t_parm
                                            );

    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_SetPullModelParam() Failed !\n");
        return CLIR_INV_ARG;        
    }
    //IMtkPb_Ctrl_GetMediaInfo(hPB_Handle, &(t_media_info));
    b_pb_info_ready = TRUE;
    
    if(i4_argc == 3)
    {
        b_pb_info_ready = TRUE;
    }
    
    return i4_ret;
}
#endif


static INT32 _cmpb_test_replay(VOID)
{
    IMTK_PB_ERROR_CODE_T                e_return;
    IMTK_PB_CTRL_ENGINE_PARAM_T           t_parm;

    x_dbg_stmt("_cmpb_test_replay\n");

    /*stop & close*/
    b_pb_info_ready = FALSE;
    IMtkPb_Ctrl_Stop(hPB_Handle);
    //x_thread_delay(1000);

    IMtkPb_Ctrl_Close(hPB_Handle);
    hPB_Handle = 0;

    //x_thread_delay(1000);

    /*reopen*/
    e_return = IMtkPb_Ctrl_Open(&hPB_Handle,
                                g_cur_mode,
                                IMTK_PB_CTRL_LIB_MASTER, NULL);
                                
    if (e_return != IMTK_PB_ERROR_CODE_OK)
    {
        x_dbg_stmt("IMtkPb_Ctrl_Open() Failed !\n");
        return CLIR_INV_ARG;        
    }
    x_dbg_stmt("IMtkPb_Ctrl_Open() ok !\n");
    //x_thread_delay(1000);

    e_return = IMtkPb_Ctrl_RegCallback(hPB_Handle,
                                (VOID*)(&ui4_tag),
                                pb_cb
                                );
                                
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_RegCallback() Failed !\n");
        return CLIR_INV_ARG;        
    }

    x_dbg_stmt("IMtkPb_Ctrl_RegCallback() ok !\n");
    //x_thread_delay(1000);

    t_parm.u4PlayFlag = IMTK_PB_CTRL_PLAY_FLAG_VIDEO |
                        IMTK_PB_CTRL_PLAY_FLAG_AUDIO;

    if(g_cur_mode == IMTK_PB_CTRL_BUFFERING_MODEL_PULL)
    {                               
#ifdef PULL_INET_TEST
#ifdef INET_SUPPORT
        BOOL  b_is_network;

        if(x_strncasecmp(g_media_path, "http:", 5) == 0)
        {
            b_is_network = TRUE;
        }
        else
        {
            b_is_network = FALSE;
        }

        if(b_is_network)
        {
            t_parm.uBufferModelParam.tPullInfo.pvAppTag = (void*)0x12345678;
            t_parm.uBufferModelParam.tPullInfo.pfnOpen = _httpc_fm_pull_open;
            t_parm.uBufferModelParam.tPullInfo.pfnClose = _httpc_fm_pull_close;
            t_parm.uBufferModelParam.tPullInfo.pfnRead = _httpc_fm_pull_read;
            t_parm.uBufferModelParam.tPullInfo.pfnReadAsync = _httpc_fm_pull_read_async;
            t_parm.uBufferModelParam.tPullInfo.pfnAbortReadAsync = _httpc_fm_pull_abort_read_async;
            t_parm.uBufferModelParam.tPullInfo.pfnByteSeek = _httpc_fm_pull_byteseek;
            t_parm.uBufferModelParam.tPullInfo.pfnGetInputLen = _httpc_fm_pull_get_input_len;
        }
        else
#endif
#endif
        {
            t_parm.uBufferModelParam.tPullInfo.pvAppTag = (void*)0x87654321;
            t_parm.uBufferModelParam.tPullInfo.pfnOpen = _fm_pull_open;
            t_parm.uBufferModelParam.tPullInfo.pfnClose = _fm_pull_close;
            t_parm.uBufferModelParam.tPullInfo.pfnRead = _fm_pull_read;
            t_parm.uBufferModelParam.tPullInfo.pfnReadAsync = _fm_pull_read_async;
            t_parm.uBufferModelParam.tPullInfo.pfnAbortReadAsync = _fm_pull_abort_read_async;
            t_parm.uBufferModelParam.tPullInfo.pfnByteSeek = _fm_pull_byteseek;
            t_parm.uBufferModelParam.tPullInfo.pfnGetInputLen = _fm_pull_get_input_len;
        }          
    }
    else if(g_cur_mode == IMTK_PB_CTRL_BUFFERING_MODEL_URI)
    {
        t_parm.uBufferModelParam.tUriInfo.u4URI_len = x_strlen(g_media_path);
        if (t_parm.uBufferModelParam.tUriInfo.u4URI_len <=0)
        {
            return CLIR_INV_ARG; 
        }
        
        t_parm.uBufferModelParam.tUriInfo.pu1URI = (UINT8 *)(g_media_path);
        t_parm.uBufferModelParam.tUriInfo.eBufSizeType = IMTK_PB_CTRL_BUF_SIZE_TYPE_BYTE;
        t_parm.uBufferModelParam.tUriInfo.uBufSize.u4Bytes= 2*1024*1024;
        t_parm.uBufferModelParam.tUriInfo.u4KeepBufThreshold = 2048 * 1024 * 10 / 100;          //10;
        t_parm.uBufferModelParam.tUriInfo.u4ReBufThreshold = 2048 * 1024 * 90 / 100;  //90;           
    }

    e_return = IMtkPb_Ctrl_SetEngineParam(hPB_Handle,
                                            &t_parm
                                            );
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_SetPullModelParam() Failed !\n");
        return CLIR_INV_ARG;        
    }

    x_dbg_stmt("IMtkPb_Ctrl_SetEngineParam() ok !\n");
    //x_thread_delay(1000);
    b_pb_info_ready = TRUE;

    /*replay*/
    e_return = IMtkPb_Ctrl_Play(hPB_Handle, 0);

    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_Play() Failed !\n");
        return CLIR_INV_ARG;        
    }
    b_is_eos = FALSE;
    x_dbg_stmt("IMtkPb_Ctrl_Play() ok !\n");
    //x_thread_delay(1000);
    
    return CLIR_OK;
}
#if 0
BOOL TEST_AUDIO = TRUE; 
static INT32 _cmpb_cli_openPUSH(
                                INT32        i4_argc,
                                const CHAR** pps_argv)
{
    INT32                               i4_ret = CLIR_OK;
    IMTK_PB_ERROR_CODE_T                e_return;
    IMTK_PB_CTRL_SET_MEDIA_INFO_T       t_media_infor;
    IMTK_PB_CTRL_SETTING_T              t_Setting;
    IMTK_PB_CTRL_ENGINE_PARAM_T           t_parm;

    
    if ((i4_argc > 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("cmpb.openpush [1]\n\r");
        return CLIR_OK;
    }
    else if(i4_argc == 1)
    {
        TEST_AUDIO = FALSE;
    }
    else
    {
        TEST_AUDIO = TRUE;
    }

    b_pb_info_ready = FALSE;

    x_memset(&t_media_infor, 0, sizeof(IMTK_PB_CTRL_SET_MEDIA_INFO_T));
    x_memset(&t_parm, 0, sizeof(IMTK_PB_CTRL_ENGINE_PARAM_T));
    
    if(!TEST_AUDIO)/*------test code ----*/ 
    {
        t_media_infor.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_MTK_P0;
        t_media_infor.u4TotalDuration = 150*90000;
        t_media_infor.u8Size = 14797520;

        t_media_infor.uFormatInfo.tMtkP0Info.tVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_H264;

    }
    else
    {
        t_media_infor.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_MTK_P0;//IMTK_PB_CTRL_MEDIA_TYPE_MP3;
        t_media_infor.u4TotalDuration = 0xFFFFFFFF;//49*90000;
        t_media_infor.u8Size = 0xFFFFFFFF;

        t_media_infor.uFormatInfo.tMtkP0Info.tAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_PCM;
        t_media_infor.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_pcm_info.e_pcm_type = IMTK_PB_CTRL_AUD_PCM_TYPE_NORMAL;
        t_media_infor.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_pcm_info.eNumCh = IMTK_PB_CTRL_AUD_CH_STEREO;
        t_media_infor.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_pcm_info.eSampleRate = IMTK_PB_CTRL_AUD_SAMPLE_RATE_44K;
        t_media_infor.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_pcm_info.u2BlockAlign = 0;
        t_media_infor.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_pcm_info.eBitsPerSample = IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_16;
        t_media_infor.uFormatInfo.tMtkP0Info.tAudInfo.uAudCodecInfo.t_pcm_info.fgBigEndian = FALSE;
    }
    
    e_return = IMtkPb_Ctrl_Open(&hPB_Handle,
                                IMTK_PB_CTRL_BUFFERING_MODEL_PUSH,
                                IMTK_PB_CTRL_APP_MASTER, NULL);
                                
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_Open() Failed !\n");
        return CLIR_INV_ARG;        
    }

    e_return = IMtkPb_Ctrl_RegCallback(hPB_Handle,
                            (VOID*)(&ui4_tag),
                            pb_cb
                            );
                            
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_RegCallback() Failed !\n");
        return CLIR_INV_ARG;        
    }        
    
    t_parm.u4PlayFlag = IMTK_PB_CTRL_PLAY_FLAG_VIDEO |
                        IMTK_PB_CTRL_PLAY_FLAG_AUDIO;

    e_return = IMtkPb_Ctrl_SetEngineParam(hPB_Handle,
                                            &t_parm
                                            );
        
    if (e_return != IMTK_PB_ERROR_CODE_OK)
    {
        x_dbg_stmt("IMtkPb_Ctrl_SetEngineParam() Failed !\n");
        return CLIR_INV_ARG;        
    }

    IMtkPb_Ctrl_SetMediaInfo(hPB_Handle, &t_media_infor);

    x_memset(&t_Setting, 0, sizeof(IMTK_PB_CTRL_SETTING_T));        

    e_return = IMtkPb_Ctrl_Play(hPB_Handle, 0);
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_Play() Failed !\n");
        return i4_ret;        
    }
        
    b_pb_info_ready = TRUE;
    
    return i4_ret;
}
#endif

static INT32 _cmpb_cli_preload(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    INT32                   i4_ret = CLIR_OK;
    IMTK_PB_ERROR_CODE_T    e_return;
    
    if (i4_argc != 1)
    {
        x_dbg_stmt("cmpb.preload\n\r");
        return CLIR_OK;
    }

    if (!b_pb_info_ready)
    {
        x_dbg_stmt(" not ready for preloading\n");
        return i4_ret;
    }

    e_return = IMtkPb_Ctrl_SetSpeed(hPB_Handle, IMTK_PB_CTRL_SPEED_1X);
                             
    /*if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_SetSpeed() Failed !\n");
        return i4_ret;        
    }*/

    /*e_return = IMtkPb_Ctrl_PreLoadData(hPB_Handle);*/

    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_SetSpeed() Failed !\n");
        return i4_ret;        
    }
    
    return i4_ret;
}

#ifdef TEST_SET_MODE
    extern MM_STRM_INFO_T              g_t_strm_info;
#endif
static INT32 _cmpb_cli_play(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    INT32                   i4_ret = CLIR_OK;
    IMTK_PB_ERROR_CODE_T    e_return;
    
    if (i4_argc > 2)
    {
        x_dbg_stmt("cmpb.play\n\r");
        return CLIR_OK;
    }

    if (!b_pb_info_ready)
    {
        x_dbg_stmt(" No media info for playback, please wait pre-processed notify !\n");
        return i4_ret;
    }
    b_print_timecode = FALSE;
    if(i4_argc == 2)
    {
        if(x_strncasecmp(pps_argv[1], "0", 1) == 0)
        {
            b_print_timecode = TRUE;
        }
        else
        {
            UINT32 i;
            UINT32 ui4_len;
            ui4_len = x_strlen(pps_argv[1]);
            i = 0;
            ui4_repeat_play_cnt = 0;
            while(i < ui4_len)
            {
                ui4_repeat_play_cnt = (ui4_repeat_play_cnt * 10 + (pps_argv[1][i] - '0'));
                i ++;
            }
            x_dbg_stmt("repeat play count is %d \n", ui4_repeat_play_cnt);            
        } 
    }
    
    e_return = IMtkPb_Ctrl_SetSpeed(hPB_Handle, IMTK_PB_CTRL_SPEED_1X);
                             
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_SetSpeed() Failed !\n");
        //return i4_ret;        
    }
////////////////////////////////////////////////////
#ifdef TEST_SET_MODE
{
    UINT32                             i;
    IMTK_PB_CTRL_SET_MEDIA_INFO_T      t_media_infor;

    x_memset(&t_media_infor, 0, sizeof(IMTK_PB_CTRL_SET_MEDIA_INFO_T));
    #if 0 /*for asf pull mode of app master*/
    t_media_infor.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_AUDIO_ES;//IMTK_PB_CTRL_MEDIA_TYPE_ASF;

    t_media_infor.uFormatInfo.tAsfInfo.u4DataPacketSize = 4096;//6976;
    t_media_infor.uFormatInfo.tAsfInfo.u8PrerollTime    = 0;//270000;
    t_media_infor.u4TotalDuration                       = 20070000;//60307713;//2790000;
    t_media_infor.u8Size                                = 5361779;//192417842;//1179254;
    #else  /*for mp3 pull mode of app master*/
    t_media_infor.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_AUDIO_ES;

    t_media_infor.uFormatInfo.tAsfInfo.u4DataPacketSize = 4096;
    t_media_infor.uFormatInfo.tAsfInfo.u8PrerollTime    = 0;
    t_media_infor.u4TotalDuration                       = 20070000;
    t_media_infor.u8Size                                = 5361779;
    #endif
    

    for(i = 0; i < g_t_strm_info.ui2_num_stm; i ++)
    {
        if(g_t_strm_info.at_stm_atrbt[i].e_type == ST_VIDEO)
        {
            switch(g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_video_stm_attr.e_enc)
            {
            case MINFO_INFO_VID_ENC_MPEG_1:
            case MINFO_INFO_VID_ENC_MPEG_2:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_MPEG1_2;
                break;
            
            case MINFO_INFO_VID_ENC_MPEG_4:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_MPEG4;
                break;
            case MINFO_INFO_VID_ENC_DIVX_311:
            case MINFO_INFO_VID_ENC_DIVX_4:
            case MINFO_INFO_VID_ENC_DIVX_5:
            case MINFO_INFO_VID_ENC_XVID:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_DIVX311;
                break;

            case MINFO_INFO_VID_ENC_WMV1:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_WMV1;
                break;
            case MINFO_INFO_VID_ENC_WMV2:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_WMV2;
                break;
            case MINFO_INFO_VID_ENC_WMV3:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_WMV3;
                break;
            case MINFO_INFO_VID_ENC_WVC1:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_VC1;
                break;
            case MINFO_INFO_VID_ENC_H264:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_H264;
                break;
            case MINFO_INFO_VID_ENC_H263:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_H263;
                break;
            case MINFO_INFO_VID_ENC_MJPG:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_MJPEG;
                break;
            case MINFO_INFO_VID_ENC_RV8:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_RV8;
                break;
            case MINFO_INFO_VID_ENC_RV9:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_RV9_10;
                break;
            case MINFO_INFO_VID_ENC_SORENSON:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_SORENSON_SPARK;
                break;
            //case MINFO_INFO_VID_ENC_NV12:
                //t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_NV12;
                //break;
            default:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_UNKNOWN;
                break;
            }

            t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.u1StreamIdx = 
                g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.t_stm_id.u.t_asf_stm_id;
            t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.u2CodecPrivInfoLen = 
                g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_video_stm_attr.ui4_codec_info_size;
            if(g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_video_stm_attr.pui1_codec_info &&
                t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.u2CodecPrivInfoLen)
            {
                x_memcpy(t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.au1CodecPrivInfo,
                    g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_video_stm_attr.pui1_codec_info,
                    t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.u2CodecPrivInfoLen);
            }
        }
        else if(g_t_strm_info.at_stm_atrbt[i].e_type == ST_AUDIO)
        {
            switch(g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_audio_stm_attr.e_enc)
            {
            case MINFO_INFO_AUD_ENC_MPEG_1:
            case MINFO_INFO_AUD_ENC_MPEG_2:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_MPEG;
                if(g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_audio_stm_attr.e_layer == 3)
                {
                    t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_MP3;
                }
                t_media_infor.uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_MP3;
                break;
            
            case MINFO_INFO_AUD_ENC_PCM:
            case MINFO_INFO_AUD_ENC_ADPCM:
            case MINFO_INFO_AUD_ENC_LPCM:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_PCM;
                t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_pcm_info.e_pcm_type = IMTK_PB_CTRL_AUD_PCM_TYPE_NORMAL;
                break;
            case MINFO_INFO_AUD_ENC_DTS:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_DTS;
                break;
            case MINFO_INFO_AUD_ENC_AAC:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_AAC;
                break;

            case MINFO_INFO_AUD_ENC_WMA_V1:                
            case MINFO_INFO_AUD_ENC_WMA_V2:                
            case MINFO_INFO_AUD_ENC_WMA_V3:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_WMA;
                break;
            case MINFO_INFO_AUD_ENC_COOK:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_COOK;
                break;
            
            default:
                t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_UNKNOWN;
                break;
            }

            t_media_infor.uFormatInfo.tAsfInfo.tAsfVidInfo.u1StreamIdx = 
                g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.t_stm_id.u.t_asf_stm_id;
            if(t_media_infor.uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc == IMTK_PB_CTRL_AUD_ENC_MP3)
            {
                /*do nothing */
            }
            else if(t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.eAudEnc != IMTK_PB_CTRL_AUD_ENC_PCM)
            {
                CONVERT_CH_VAL_2_ENUM(g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_channels,
                    t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_wma_info.eNumCh);

                CONVERT_SR_VAL_2_ENUM(g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_audio_stm_attr.i4_samples_per_sec,
                    t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_wma_info.eSampleRate);
                
                t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_wma_info.u4AvgBytesPerSec =
                    g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_audio_stm_attr.i4_samples_per_sec * 
                    g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_bits_per_sample / 8;

                 t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_wma_info.u4BlockSz =
                    g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_block_align;
            }
            else
            {
                CONVERT_CH_VAL_2_ENUM(g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_channels,
                    t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_pcm_info.eNumCh);

                CONVERT_SR_VAL_2_ENUM(g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_audio_stm_attr.i4_samples_per_sec,
                    t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_pcm_info.eSampleRate);

                CONVERT_BD_VAL_2_ENUM(g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_bits_per_sample,
                    t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_pcm_info.eBitsPerSample);

                 t_media_infor.uFormatInfo.tAsfInfo.tAsfAudInfo.uAudCodecInfo.t_pcm_info.u2BlockAlign =
                    g_t_strm_info.at_stm_atrbt[i].t_stm_atrbt.u.t_asf_audio_stm_attr.i2_block_align;
            }
        }
    }     
    
    e_return = IMtkPb_Ctrl_SetMediaInfo(hPB_Handle, &t_media_infor);
}
#endif
/////////////////////////////////////////////////////
    e_return = IMtkPb_Ctrl_Play(hPB_Handle, 0);

    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_Play() Failed !\n");
        return i4_ret;        
    }
    b_is_eos = FALSE;

    while(ui4_repeat_play_cnt)
    {
        if(b_is_eos == TRUE)
        {            
            _cmpb_test_replay();
            ui4_repeat_play_cnt --;
        }
        x_thread_delay(1000);
    }
    
    return i4_ret;
}


static INT32 _cmpb_cli_stop(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    INT32       i4_ret = CLIR_OK;
    
    if (i4_argc != 1)
    {
        x_dbg_stmt("cmpb.stop\n\r");
        return CLIR_OK;
    }

    b_print_timecode = FALSE;
    ui4_repeat_play_cnt = 0;
    b_pb_info_ready = FALSE;
    IMtkPb_Ctrl_Stop(hPB_Handle);

    return i4_ret;
}


static INT32 _cmpb_cli_get_buffer_fullness(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    INT32       i4_ret = CLIR_OK;
    UINT32      ui4_percentage;
    IMTK_PB_ERROR_CODE_T e_ret;
    
    if (i4_argc != 1)
    {
        x_dbg_stmt("cmpb.buf\n\r");
        return CLIR_OK;
    }

    e_ret = IMtkPb_Ctrl_GetBufferFullness(hPB_Handle,(uint32_t *)&ui4_percentage);

    if (IMTK_PB_ERROR_CODE_OK == e_ret)
    {
        x_dbg_stmt("buffer fullness = %%d\n", ui4_percentage);
    }
    else
    {
        x_dbg_stmt("IMtkPb_Ctrl_GetBufferFullness() failed, error code=%d\n",e_ret);
    }

    return i4_ret;
}


static INT32 _cmpb_cli_close(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    INT32       i4_ret = CLIR_OK;
    
    if (i4_argc != 1)
    {
        x_dbg_stmt("cmpb.close\n\r");
        return CLIR_OK;
    }

    IMtkPb_Ctrl_Close(hPB_Handle);
    b_pb_info_ready = FALSE;
    hPB_Handle = 0;

    return i4_ret;
}




static INT32 _cmpb_cli_pause(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    INT32       i4_ret = CLIR_OK;
    
    if (i4_argc != 1)
    {
        x_dbg_stmt("cmpb.pause\n\r");
        return CLIR_OK;
    }

    IMtkPb_Ctrl_Pause(hPB_Handle);

    return i4_ret;
}


static INT32 _cmpb_cli_step(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    INT32       i4_ret = CLIR_OK;
    
    if (i4_argc != 1)
    {
        x_dbg_stmt("cmpb.step\n\r");
        return CLIR_OK;
    }

    IMtkPb_Ctrl_Step(hPB_Handle,1);

    return i4_ret;
}


static INT32 _cmpb_cli_ff(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    INT32                   i4_ret = CLIR_OK;
    IMTK_PB_ERROR_CODE_T    e_return;
    IMTK_PB_CTRL_SETTING_T  t_Setting;
    
    if (i4_argc != 1)
    {
        x_dbg_stmt("cmpb.ff\n\r");
        return CLIR_OK;
    }

    e_return = IMtkPb_Ctrl_GetPlayParam(hPB_Handle, 
                             &t_Setting
                             );
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_GetPlayParam() Failed !\n");
        return i4_ret;        
    }
                             
    switch(t_Setting.eSpeed)
    {
        case IMTK_PB_CTRL_SPEED_1X :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FF_2X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_2X :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FF_4X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_4X :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FF_8X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_8X :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FF_16X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_16X :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FF_32X;
            break;
        case IMTK_PB_CTRL_SPEED_FF_32X :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_1X;
            break;
        default :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FF_2X;
            break;
    }
    
    
    /*t_Setting.u4InfoMask = IMTK_PB_CTRL_SETTING_SPEED;*/
    //t_Setting.u2ATrack = 0;
    e_return = IMtkPb_Ctrl_SetSpeed(hPB_Handle, t_Setting.eSpeed);
                             
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_SetSpeed() Failed !\n");
        return i4_ret;        
    }

    return i4_ret;
}


static INT32 _cmpb_cli_csub(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    UINT32 i;
    UINT32 ui4_len;
    UINT32 ui4_track;
        
    if (i4_argc != 2)
    {
        return CLIR_OK;
    }

    ui4_len = x_strlen(pps_argv[1]);
    i = 0;
    ui4_track = 0;
    while(i < ui4_len)
    {
        ui4_track = (ui4_track * 10 + (pps_argv[1][i] - '0'));
        i ++;
    }

    //IMtkPb_Ctrl_SetSubtitleTrack(hPB_Handle, ui4_track);
    
    return CLIR_OK;
}


static INT32 _cmpb_cli_caud(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    UINT32 i;
    UINT32 ui4_len;
    UINT32 ui4_track;
        
    if (i4_argc != 2)
    {
        return CLIR_OK;
    }

    ui4_len = x_strlen(pps_argv[1]);
    i = 0;
    ui4_track = 0;
    while(i < ui4_len)
    {
        ui4_track = (ui4_track * 10 + (pps_argv[1][i] - '0'));
        i ++;
    }

    IMtkPb_Ctrl_SetAudTrack(hPB_Handle, ui4_track);
    return CLIR_OK;
}


static INT32 _cmpb_cli_fr(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    INT32                   i4_ret = CLIR_OK;
    IMTK_PB_ERROR_CODE_T    e_return;
    IMTK_PB_CTRL_SETTING_T  t_Setting;
    
    if (i4_argc != 1)
    {
        x_dbg_stmt("cmpb.fr\n\r");
        return CLIR_OK;
    }

    e_return = IMtkPb_Ctrl_GetPlayParam(hPB_Handle, 
                             &t_Setting
                             );
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_GetPlayParam() Failed !\n");
        return i4_ret;        
    }
                             
    switch(t_Setting.eSpeed)
    {
        /*case IMTK_PB_CTRL_SPEED_FR_1X :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FR_2X;
            break;*/
        case IMTK_PB_CTRL_SPEED_FR_2X :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FR_4X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_4X :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FR_8X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_8X :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FR_16X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_16X :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FR_32X;
            break;
        case IMTK_PB_CTRL_SPEED_FR_32X :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FR_2X;
            break;
        default :
            t_Setting.eSpeed = IMTK_PB_CTRL_SPEED_FR_2X;
            break;
    }
    
    
    /*t_Setting.u4InfoMask = IMTK_PB_CTRL_SETTING_SPEED;*/
    //t_Setting.u2ATrack = 0;
    e_return = IMtkPb_Ctrl_SetSpeed(hPB_Handle, t_Setting.eSpeed);
                             
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_SetSpeed() Failed !\n");
        return i4_ret;        
    }

    return i4_ret;
}

static INT32 _cmpb_cli_bseek(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    #if 0
    INT32                   i4_ret = CLIR_OK;
    IMTK_PB_ERROR_CODE_T    e_return;
    UINT64                  ui8_seek_pos = 0;
    UINT32                  ui4_pos_len = 0;
    UINT32                  i;
    
    if (i4_argc != 2)
    {
        x_dbg_stmt("cmpb.bseek [val]\n\r");
        return CLIR_OK;
    }

    ui4_pos_len = x_strlen(pps_argv[1]);
    //x_dbg_stmt("str len is %d\n", ui4_pos_len);
    i = 0;
    while(i < ui4_pos_len)
    {
        //x_dbg_stmt("cur char is %c - int val is %d\n", pps_argv[1][i], (pps_argv[1][i] - '0'));
        ui8_seek_pos = (ui8_seek_pos * 10 + (pps_argv[1][i] - '0'));
        i ++;
    }
    
    e_return = IMtkPb_Ctrl_ByteSeek(hPB_Handle, ui8_seek_pos);
    x_dbg_stmt("byte seek to %d\n\r", (UINT32)ui8_seek_pos);

                             
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("_cmpb_cli_bseek() Failed, return: %d !\n", e_return);
        return i4_ret;        
    }

    return i4_ret;
    #else
    return IMTK_PB_ERROR_CODE_OK;
    #endif
}

static INT32 _cmpb_cli_tseek(
                INT32        i4_argc,
                const CHAR** pps_argv)
{
    INT32                   i4_ret = CLIR_OK;
    IMTK_PB_ERROR_CODE_T    e_return;
    UINT64                  ui8_seek_time = 0;
    UINT32                  ui4_time_len = 0;
    UINT32                  i;
    
    if (i4_argc != 2)
    {
        x_dbg_stmt("cmpb.tseek [val]\n\r");
        return CLIR_OK;
    }

    ui4_time_len = x_strlen(pps_argv[1]);
    i = 0;
    while(i < ui4_time_len)
    {
        ui8_seek_time = (ui8_seek_time * 10 + (pps_argv[1][i] - '0'));
        i ++;
    }
    
    e_return = IMtkPb_Ctrl_TimeSeek(hPB_Handle, ui8_seek_time);
    x_dbg_stmt("time seek to %d\n\r", (UINT32)ui8_seek_time);
                             
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("_cmpb_cli_tseek() Failed, return: %d !\n", e_return);
        return i4_ret;        
    }

    return i4_ret;
}

static VOID _push_thread_mtka(VOID*   pv_data)
{
    INT32       i4_ret = CLIR_OK;
    HANDLE_T    h_file = NULL_HANDLE;
    
    IMTK_PB_ERROR_CODE_T e_ret;
    
    UINT32      ui4_idx = 0;
    UINT8*      pui1_buf;
    UINT64      ui8_pts;
    UINT32      ui4_strm_size;
    UINT32      ui4_read_cnt;

    /*start push*/
    #ifdef LINUX_TURNKEY_SOLUTION
    i4_ret = x_fm_open( FM_ROOT_HANDLE,
                        LINUX_PATH_PREFIX"ok.pcm",
                        FM_READ_ONLY,
                        0777,
                        FALSE,
                        &h_file);
    #else
    i4_ret = x_fm_open( FM_ROOT_HANDLE,
                        NUCLEUS_PATH_PREFIX"ok.pcm",
                        FM_READ_ONLY,
                        0777,
                        FALSE,
                        &h_file);
    #endif

    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("Open file(/mnt/usb/sda1/cmpb/ok.pcm) Failed !\n");
        return;
    }

    while (1)
    {
        ui4_strm_size = 1764;
        
        e_ret = IMtkPb_Ctrl_GetBuffer ( hPB_Handle, 
                                        CMPB_PRIVATE_CONTAINER_HEADER_SIZE + ui4_strm_size, 
                                        &pui1_buf);

        if (IMTK_PB_ERROR_CODE_OK == e_ret)
        {
            /*reset private container header*/
            /*PTS(8 bytes) Flags(1 byte) Stream_Size(4 bytes)*/
            /*Flags(1 byte): Bit7:Bit6   Bit5   Bit4:Bit3 Bit2:Bit0 */
            /*               Reserved   PktVld  0:3-RIBP  1:3-VAS,Other Reserve */
            x_memset(pui1_buf, 0, 15);
            
            ui8_pts = 0;

            i4_ret = x_fm_read(
                                h_file,
                                pui1_buf + CMPB_PRIVATE_CONTAINER_HEADER_SIZE,
                                ui4_strm_size,
                                &ui4_read_cnt);
            if (FMR_OK != i4_ret)
            {
                break;
            }
            
            if (ui4_read_cnt < ui4_strm_size)
            {
                break;
            }

            x_memcpy(pui1_buf, (VOID*)&ui8_pts, 8);
            x_memcpy(pui1_buf+8, (VOID*)&ui4_strm_size, 4);

            pui1_buf[12] = 0x02;

            IMtkPb_Ctrl_SendData(
                                 hPB_Handle, 
                                 CMPB_PRIVATE_CONTAINER_HEADER_SIZE + ui4_strm_size, 
                                 pui1_buf);

            ui4_idx++;
        }
        else if (IMTK_PB_ERROR_CODE_GET_BUF_PENDING == e_ret)
        {
            x_thread_delay(10);
        }
        else
        {
            x_thread_delay(10);
        }
    }

    IMtkPb_Ctrl_SetPushModelEOS(hPB_Handle);
    
    i4_ret = x_fm_close(h_file);
}

static VOID _push_thread_mtkv(VOID*   pv_data)
{
    INT32       i4_ret = CLIR_OK;
    HANDLE_T    h_file = NULL_HANDLE;
    
    IMTK_PB_ERROR_CODE_T e_ret;
    
    UINT32      ui4_idx = 0;
    UINT8*      pui1_buf;
    UINT64      ui8_pts;
    UINT32      ui4_strm_size;
    UINT32      ui4_read_cnt;

    /*start push*/
    #ifdef LINUX_TURNKEY_SOLUTION
    i4_ret = x_fm_open( FM_ROOT_HANDLE,
                        LINUX_PATH_PREFIX"VideoStream",
                        FM_READ_ONLY,
                        0777,
                        FALSE,
                        &h_file);
    #else
    i4_ret = x_fm_open( FM_ROOT_HANDLE,
                        NUCLEUS_PATH_PREFIX"VideoStream",
                        FM_READ_ONLY,
                        0777,
                        FALSE,
                        &h_file);
    #endif


    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("Open file(/mnt/usb/sda1/VideoStream) Failed !\n");
        return;
    }

    while (1)
    {
        ui4_strm_size = 2048;
        
        e_ret = IMtkPb_Ctrl_GetBuffer ( hPB_Handle, 
                                        CMPB_PRIVATE_CONTAINER_HEADER_SIZE + ui4_strm_size, 
                                        &pui1_buf);

        if (IMTK_PB_ERROR_CODE_OK == e_ret)
        {
            /*reset private container header*/
            /*PTS(8 bytes) Flags(1 byte) Stream_Size(4 bytes)*/
            /*Flags(1 byte): Bit7:Bit6   Bit5   Bit4:Bit3 Bit2:Bit0 */
            /*               Reserved   PktVld  0:3-RIBP  1:3-VAS,Other Reserve */
            x_memset(pui1_buf, 0, 15);
            
            ui8_pts = 0;

            i4_ret = x_fm_read(
                                h_file,
                                pui1_buf + CMPB_PRIVATE_CONTAINER_HEADER_SIZE,
                                ui4_strm_size,
                                &ui4_read_cnt);
            if (FMR_OK != i4_ret)
            {
                break;
            }
            
            if (ui4_read_cnt < ui4_strm_size)
            {
                break;
            }

            x_memcpy(pui1_buf, (VOID*)&ui8_pts, 8);
            x_memcpy(pui1_buf+8, (VOID*)&ui4_strm_size, 4);

            pui1_buf[12] = 0x01;

            IMtkPb_Ctrl_SendData(
                                 hPB_Handle, 
                                 CMPB_PRIVATE_CONTAINER_HEADER_SIZE + ui4_strm_size, 
                                 pui1_buf);

            ui4_idx++;
        }
        else if (IMTK_PB_ERROR_CODE_GET_BUF_PENDING == e_ret)
        {
            x_thread_delay(10);
        }
        else
        {
            x_thread_delay(10);
        }
    }

    IMtkPb_Ctrl_SetPushModelEOS(hPB_Handle);
    
    i4_ret = x_fm_close(h_file);
}

static VOID _push_thread_ps_ts(CHAR    *ps_name)
{
    INT32       i4_ret              = CLIR_OK;
    HANDLE_T    h_file              = NULL_HANDLE;
    UINT8*      pui1_buf            = NULL;
    UINT32      ui4_read_cnt        = 0;    
    CHAR        aui1_path_name[128] = {0};    
    IMTK_PB_ERROR_CODE_T e_ret;
    

    /*start push*/
    #ifdef LINUX_TURNKEY_SOLUTION
    x_strcpy(aui1_path_name, LINUX_PATH_PREFIX);
    #else
    x_strcpy(aui1_path_name, NUCLEUS_PATH_PREFIX);
    #endif
    
    x_strcat(aui1_path_name, ps_name);
    
    i4_ret = x_fm_open( FM_ROOT_HANDLE,
                        aui1_path_name,
                        FM_READ_ONLY,
                        0777,
                        FALSE,
                        &h_file);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("Open file(%s) Failed !\n", aui1_path_name);
        return ;
    }

    while (1)
    {

        e_ret = IMtkPb_Ctrl_GetBuffer ( hPB_Handle, 
                                        CMPB_PRIVATE_PS_BUF_SIZE, 
                                        &pui1_buf);
        if (IMTK_PB_ERROR_CODE_OK == e_ret)
        {
            i4_ret = x_fm_read(
                                h_file,
                                pui1_buf,
                                CMPB_PRIVATE_PS_BUF_SIZE,
                                &ui4_read_cnt);
            if (FMR_OK != i4_ret)
            {
                break;
            }
            
            if (ui4_read_cnt < CMPB_PRIVATE_PS_BUF_SIZE)
            {
                break;
            }

            IMtkPb_Ctrl_SendData(
                                 hPB_Handle, 
                                 CMPB_PRIVATE_PS_BUF_SIZE, 
                                 pui1_buf);

        }
        else if (IMTK_PB_ERROR_CODE_GET_BUF_PENDING == e_ret)
        {
            //IMtkPb_Ctrl_CancelGetBufferPending(hPB_Handle);
            x_thread_delay(10);
        }
        else
        {
            x_thread_delay(10);
        }
    }

    IMtkPb_Ctrl_SetPushModelEOS(hPB_Handle);
    
    i4_ret = x_fm_close(h_file);
}

/* used for avi/wmv/flv etc. that no need specified buffer size*/
static VOID _push_thread_xx(CHAR    *ps_name, INT64 i8_offset, UINT64 ui8_sz)
{
    INT32       i4_ret              = CLIR_OK;
    HANDLE_T    h_file              = NULL_HANDLE;
    UINT8*      pui1_buf            = NULL;
    UINT32      ui4_read_cnt        = 0;    
    CHAR        aui1_path_name[128] = {0};    
    IMTK_PB_ERROR_CODE_T e_ret;
    UINT64      ui8_cur_pos;
    

    /*start push*/
    #ifdef LINUX_TURNKEY_SOLUTION
    x_strcpy(aui1_path_name, LINUX_PATH_PREFIX);
    #else
    x_strcpy(aui1_path_name, NUCLEUS_PATH_PREFIX);
    #endif
    
    x_strcat(aui1_path_name, ps_name);
    
    i4_ret = x_fm_open( FM_ROOT_HANDLE,
                        aui1_path_name,
                        FM_READ_ONLY,
                        0777,
                        FALSE,
                        &h_file);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("Open file(%s) Failed !\n", aui1_path_name);
        return ;
    }

    i4_ret = x_fm_lseek(h_file, i8_offset, FM_SEEK_BGN, &ui8_cur_pos);;
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("cmpb: x_fm_lseek fail!\n");   
    }    

    while (1)
    {
        e_ret = IMtkPb_Ctrl_GetBuffer ( hPB_Handle, 
                                        CMPB_COMMON_BUF_SIZE, 
                                        &pui1_buf);
        if (IMTK_PB_ERROR_CODE_OK == e_ret)
        {
            i4_ret = x_fm_read(
                                h_file,
                                pui1_buf,
                                CMPB_COMMON_BUF_SIZE,
                                &ui4_read_cnt);
            if (FMR_OK != i4_ret)
            {
                break;
            }
            
            if (ui4_read_cnt < CMPB_COMMON_BUF_SIZE)
            {
                break;
            }

            if(ui8_sz != (UINT64)-1)
            {
                i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_CUR, &ui8_cur_pos);;
                if (FMR_OK == i4_ret)
                {
                    if(ui8_cur_pos > (UINT64)i8_offset + ui8_sz)
                    {
                        break; /*ignore the data readed when read size is more than given range size*/
                    }
                }
                else
                {
                    x_dbg_stmt("cmpb: x_fm_lseek fail!\n");  
                }
            }

            IMtkPb_Ctrl_SendData(
                                 hPB_Handle, 
                                 CMPB_COMMON_BUF_SIZE, 
                                 pui1_buf);

        }
        else if (IMTK_PB_ERROR_CODE_GET_BUF_PENDING == e_ret)
        {
            //IMtkPb_Ctrl_CancelGetBufferPending(hPB_Handle);
            x_thread_delay(10);
        }
        else
        {
            x_thread_delay(10);
        }
    }

    IMtkPb_Ctrl_SetPushModelEOS(hPB_Handle);
    
    i4_ret = x_fm_close(h_file);
}

static INT32 _cmpb_cli_get_media_info_by_fmt(
                                UINT32                          ui4_fmt_type,
                                IMTK_PB_CTRL_SET_MEDIA_INFO_T*  pt_media_infor)
{
#if 0
    switch(ui4_fmt_type)
    {
        case FourCC('m', 't', 'k', 'a'):/* ok.pcm */
        {
            IMTK_PB_CTRL_BASIC_AUDIO_INFO_T* ptAudInfo = NULL;

            ptAudInfo = &(pt_media_infor->uFormatInfo.tMtkP0Info.tAudInfo);
            
            pt_media_infor->eMediaType      = IMTK_PB_CTRL_MEDIA_TYPE_MTK_P0;
            pt_media_infor->u4TotalDuration = 0xFFFFFFFF;
            pt_media_infor->u8Size          = 0xFFFFFFFF;

            ptAudInfo->eAudEnc                                  = IMTK_PB_CTRL_AUD_ENC_PCM;
            ptAudInfo->uAudCodecInfo.tPcmInfo.e_pcm_type        = IMTK_PB_CTRL_AUD_PCM_TYPE_NORMAL;
            ptAudInfo->uAudCodecInfo.tPcmInfo.u4NumCh           = 2;
            ptAudInfo->uAudCodecInfo.tPcmInfo.u4SampleRate      = 44100;
            ptAudInfo->uAudCodecInfo.tPcmInfo.u2BlockAlign      = 0;
            ptAudInfo->uAudCodecInfo.tPcmInfo.u2BitsPerSample   = 16;
            ptAudInfo->uAudCodecInfo.tPcmInfo.fgBigEndian       = FALSE;
        }            
            break;
            
        case FourCC('m', 't', 'k', 'v'):/* VideoStream */
        {
            pt_media_infor->eMediaType      = IMTK_PB_CTRL_MEDIA_TYPE_MTK_P0;
            pt_media_infor->u4TotalDuration = 150*90000;
            pt_media_infor->u8Size          = 14797520;

            pt_media_infor->uFormatInfo.tMtkP0Info.tVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_H264;
        }
            break;
            
        case FourCC('p', 's', 0, 0):/*VGB0001_MPEG1_384x288_1071Kbps_MP2_44.1kHz_Monophonic.mpeg*/
        {/* Only need stream id and codec*/
            pt_media_infor->eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_PS;
            pt_media_infor->u4TotalDuration = 0x298902;
            pt_media_infor->u8Size = 0x48c833;
            pt_media_infor->uFormatInfo.tPsInfo.tVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_MPEG1_2;
            pt_media_infor->uFormatInfo.tPsInfo.tVidInfo.tVidStrmIdInfo.u1StrmId = 224;

            pt_media_infor->uFormatInfo.tPsInfo.tAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_MPEG;
            pt_media_infor->uFormatInfo.tPsInfo.tAudInfo.tAudStrmIdInfo.u1StrmId = 192;
        }
            break;
          
        case FourCC('r', 'm', 'v', 'b'):/* Demo_750k_RV10_640x360_30fps_RA_650k_music.rm */
        {
            IMTK_PB_CTRL_RM_VID_INFO_T*     ptRmVidInfo = NULL;
            IMTK_PB_CTRL_RM_AUD_INFO_T*     ptRmAudInfo = NULL;

            ptRmVidInfo = &(pt_media_infor->uFormatInfo.tRmInfo.tRmVidInfo);
            ptRmAudInfo = &(pt_media_infor->uFormatInfo.tRmInfo.tRmAudInfo);

            pt_media_infor->eMediaType      = IMTK_PB_CTRL_MEDIA_TYPE_RM;
            pt_media_infor->u4TotalDuration = 0xFFFFFFFF;
            pt_media_infor->u8Size          = (UINT64)-1;

            ptRmVidInfo->eVidEnc            = IMTK_PB_CTRL_VID_ENC_RV9_10;
            ptRmVidInfo->ui2_frm_width      = 640;
            ptRmVidInfo->ui2_frm_height     = 360;
            ptRmVidInfo->ui4_spo_extra_flags= 0x1481020;
            x_memset(ptRmVidInfo->aui4_specific, 0, sizeof(UINT32)*25);
            ptRmVidInfo->b_is_rm8           = FALSE;

            ptRmVidInfo->uVidCodecInfo.t_rv9_10_info.u4Width    = 640;
            ptRmVidInfo->uVidCodecInfo.t_rv9_10_info.u4Height   = 360;
            ptRmVidInfo->uVidCodecInfo.t_rv9_10_info.eFrmRate   = IMTK_PB_CTRL_FRAME_RATE_29_97;
            ptRmVidInfo->uVidCodecInfo.t_rv9_10_info.eRvBitsVersion= IMTK_PB_CTRL_RV_FID_RV89COMBO;


            ptRmAudInfo->eAudEnc                                = IMTK_PB_CTRL_AUD_ENC_COOK;
            ptRmAudInfo->uAudCodecInfo.t_rm_cook_info.eNumCh       = IMTK_PB_CTRL_AUD_CH_STEREO;
            ptRmAudInfo->uAudCodecInfo.t_rm_cook_info.eSampleRate  = IMTK_PB_CTRL_AUD_SAMPLE_RATE_44K;
            ptRmAudInfo->uAudCodecInfo.t_rm_cook_info.u2SamplePerFrame = 16;
            ptRmAudInfo->uAudCodecInfo.t_rm_cook_info.u4FrameSize  = 280;
            ptRmAudInfo->uAudCodecInfo.t_rm_cook_info.u2RegionNum  = 0x0025;
            ptRmAudInfo->uAudCodecInfo.t_rm_cook_info.u4CplRegionStart = 0x0008;
            ptRmAudInfo->uAudCodecInfo.t_rm_cook_info.u2QBitsNum   = 0x0005;
            ptRmAudInfo->uAudCodecInfo.t_rm_cook_info.u2CookInlvFac = 16;
            ptRmAudInfo->uAudCodecInfo.t_rm_cook_info.u2CookBlockSz = 1400;
            ptRmAudInfo->uAudCodecInfo.t_rm_cook_info.u2FlavorIdx   = 25;
        }
            break;
 
        case FourCC('t', 's', 0, 0):/* Malata_061225Mon-PAT1080P_20061221.ts */
        {
            pt_media_infor->eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS;
            pt_media_infor->u4TotalDuration = 0xFFFFFFFF;
            pt_media_infor->u8Size = (UINT64)-1;

            pt_media_infor->uFormatInfo.tTsInfo.ePacketType = IMTK_PB_CTRL_MPEG2_TS_PACKET_TYPE_188BYTE;

            pt_media_infor->uFormatInfo.tTsInfo.eVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_MPEG1_2;
            pt_media_infor->uFormatInfo.tTsInfo.eVidInfo.u2Pid = 33;
            ////pt_media_infor->uFormatInfo.tTsInfo.eVidInfo.uVidCodecInfo. = ;

            pt_media_infor->uFormatInfo.tTsInfo.eAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_DD;
            pt_media_infor->uFormatInfo.tTsInfo.eAudInfo.u2Pid = 36;
            ////pt_media_infor->uFormatInfo.tTsInfo.eAudInfo.uAudCodecInfo. = ;
        }
            break;

        case FourCC('w', 'm', 'v', 0):
        case FourCC('a', 's', 'f', 0):/* fight.wmv */
        {
            IMTK_PB_CTRL_ASF_VID_INFO_T* ptAsfVidInfo = NULL;
            IMTK_PB_CTRL_ASF_AUD_INFO_T* ptAsfAudInfo = NULL;

            ptAsfVidInfo = &(pt_media_infor->uFormatInfo.tAsfInfo.tAsfVidInfo);
            ptAsfAudInfo = &(pt_media_infor->uFormatInfo.tAsfInfo.tAsfAudInfo);
            
            pt_media_infor->eMediaType      = IMTK_PB_CTRL_MEDIA_TYPE_ASF;
            pt_media_infor->u4TotalDuration = 0xFFFFFFFF;
            pt_media_infor->u8Size          = (UINT64)-1;

            pt_media_infor->uFormatInfo.tAsfInfo.u4DataPacketSize = 5625;
            pt_media_infor->uFormatInfo.tAsfInfo.u8PrerollTime = 450000;
                
            ptAsfVidInfo->eVidEnc           = IMTK_PB_CTRL_VID_ENC_WMV3;
            ptAsfVidInfo->u1StreamIdx       = 2;
            ptAsfVidInfo->u2CodecPrivInfoLen= 5;
            ptAsfVidInfo->u4_Rate           = 30000;
            ptAsfVidInfo->u4_Scale          = 1000;
            ptAsfVidInfo->au1CodecPrivInfo[4] = 0x00;
            ptAsfVidInfo->au1CodecPrivInfo[3] = 0x01;
            ptAsfVidInfo->au1CodecPrivInfo[2] = 0x0A;
            ptAsfVidInfo->au1CodecPrivInfo[1] = 0x69;
            ptAsfVidInfo->au1CodecPrivInfo[0] = 0x4E;
            
            ptAsfVidInfo->uVidCodecInfo.t_wmv3_info.u4Width = 320;
            ptAsfVidInfo->uVidCodecInfo.t_wmv3_info.u4Height = 240;

            ptAsfAudInfo->eAudEnc                           = IMTK_PB_CTRL_AUD_ENC_WMA;
            ptAsfAudInfo->u1StreamIdx                       = 1;
            //ptAsfAudInfo->uAudCodecInfo.tWmaInfo.u2HdrSrc   = 0;
            ptAsfAudInfo->uAudCodecInfo.tWmaInfo.u2NumCh    = 2;
            ptAsfAudInfo->uAudCodecInfo.tWmaInfo.u4SamplePerSec = 44100;
            ptAsfAudInfo->uAudCodecInfo.tWmaInfo.u4BitsPerSec = 8005;
            ptAsfAudInfo->uAudCodecInfo.tWmaInfo.u4BlockSz = 1487;
            ptAsfAudInfo->uAudCodecInfo.tWmaInfo.u2EncoderOpt = 0x0F;
        }
            break;

        case FourCC('a', 'v', 'i', 0):/* h376ShinjoujiShidareSakura10.avi */
        {
            IMTK_PB_CTRL_AVI_VID_INFO_T* ptAviVidInfo = NULL;
            IMTK_PB_CTRL_AVI_AUD_INFO_T* ptAviAudInfo = NULL;

            ptAviVidInfo = &(pt_media_infor->uFormatInfo.tAviInfo.tAviVidInfo);
            ptAviAudInfo = &(pt_media_infor->uFormatInfo.tAviInfo.tAviAudInfo);
            
            pt_media_infor->eMediaType      = IMTK_PB_CTRL_MEDIA_TYPE_AVI;
            pt_media_infor->u4TotalDuration = 0xEF80BA;
            pt_media_infor->u8Size          = (UINT64)-1;
            
            ptAviVidInfo->eVidEnc       = IMTK_PB_CTRL_VID_ENC_DIVX5;
            ptAviVidInfo->u1StreamIdx   = 0;
            x_memcpy(ptAviVidInfo->au1FourCC, "00dc", 4);
            ptAviVidInfo->u4Scale       = 1001;
            ptAviVidInfo->u4Rate        = 30000;
            ptAviVidInfo->u4Width       = 1920;
            ptAviVidInfo->u4Height      = 1080;

            ptAviAudInfo->eAudEnc       = IMTK_PB_CTRL_AUD_ENC_MP3;
            ptAviAudInfo->u1StreamIdx   = 1;
            x_memcpy(ptAviAudInfo->au1FourCC, "01wb", 4);
            ptAviAudInfo->fgVbr         = FALSE;
            ptAviAudInfo->u4Scale       = 1;
            ptAviAudInfo->u4Rate        = 16000;
            ptAviAudInfo->u4Bps         = 16000;
            //ptAviAudInfo->uAudCodecInfo = ;
        }
            break;
            
        case FourCC('m', 'k', 'v', 0):/* VMB0007_DX50_640x352_947Kbps_MP3_44.1kHz_Stereo_CBR.mkv */
        {
            IMTK_PB_CTRL_MKV_VID_INFO_T* ptMkvVidInfo = NULL;
            IMTK_PB_CTRL_MKV_AUD_INFO_T* ptMkvAudInfo = NULL;

            ptMkvVidInfo = &(pt_media_infor->uFormatInfo.tMkvInfo.tMkvVidInfo);
            ptMkvAudInfo = &(pt_media_infor->uFormatInfo.tMkvInfo.tMkvAudInfo);
            
            pt_media_infor->eMediaType      = IMTK_PB_CTRL_MEDIA_TYPE_MKV;
            pt_media_infor->u4TotalDuration = 0xFFFFFFFF;
            pt_media_infor->u8Size          = (UINT64)-1;
            
            ptMkvVidInfo->uVidCodec.eVidEnc     = IMTK_PB_CTRL_VID_ENC_DIVX5;
            ptMkvVidInfo->u8VidTrackNo          = 1;
            ptMkvVidInfo->u8TrackTimeCodeScale  = 1;
            ptMkvVidInfo->u2CodecPrivInfoLen    = 0;
            //ptMkvVidInfo->au1CodecPrivInfo[IMTK_PB_VID_CODEC_PRIV_INFO_LEN] = ;

            ptMkvAudInfo->eAudEnc               = IMTK_PB_CTRL_AUD_ENC_MP3;
            ptMkvAudInfo->u8AudTrackNo          = 2;
            ptMkvAudInfo->uAudCodecInfo.tPcmInfo.u4NumCh = 2;
            ptMkvAudInfo->uAudCodecInfo.tPcmInfo.u4SampleRate = 44100;
            ptMkvAudInfo->uAudCodecInfo.tPcmInfo.u2BitsPerSample = 0;
            ptMkvAudInfo->uAudCodecInfo.tPcmInfo.fgBigEndian = FALSE;
            ptMkvAudInfo->uAudCodecInfo.tPcmInfo.u2BlockAlign = 0;
            //ptMkvAudInfo->u8TrackTimeCodeScale = 1;
            //ptMkvAudInfo->uAudCodecInfo = ;            
        }
            break;

        case FourCC('f', 'l', 'v', 0):/* VMB0321_Superb_30fps_32kbps_22050Hz.flv */
        {
            IMTK_PB_CTRL_FLV_VID_INFO_T* ptFlvVidInfo = NULL;
            IMTK_PB_CTRL_FLV_AUD_INFO_T* ptFlvAudInfo = NULL;

            ptFlvVidInfo = &(pt_media_infor->uFormatInfo.tFlvInfo.tFlvVidInfo);
            ptFlvAudInfo = &(pt_media_infor->uFormatInfo.tFlvInfo.tFlvAudInfo);
            
            pt_media_infor->eMediaType      = IMTK_PB_CTRL_MEDIA_TYPE_FLV;
            pt_media_infor->u4TotalDuration = (UINT32)-1;
            pt_media_infor->u8Size          = (UINT64)-1;
            
            ptFlvVidInfo->eVidEnc       = IMTK_PB_CTRL_VID_ENC_MPEG4;
            ptFlvVidInfo->u1StreamIdx   = 1;
            ptFlvVidInfo->u4Scale       = 1;
            ptFlvVidInfo->u4Rate        = 30;
            ptFlvVidInfo->u4Width       = 0;
            ptFlvVidInfo->u4Height      = 0;

            ptFlvAudInfo->eAudEnc       = IMTK_PB_CTRL_AUD_ENC_MP3;
            ptFlvAudInfo->u1StreamIdx   = 2;
            ptFlvAudInfo->fgVbr         = FALSE;
            ptFlvAudInfo->u4Bps         = 4050;

            ptFlvAudInfo->uAudCodecInfo.tPcmInfo.u4NumCh = 2;
            ptFlvAudInfo->uAudCodecInfo.tPcmInfo.u4SampleRate = 22050;
            ptFlvAudInfo->uAudCodecInfo.tPcmInfo.u2BitsPerSample = 16;
            ptFlvAudInfo->uAudCodecInfo.tPcmInfo.fgBigEndian = FALSE;
        }
            break;
            
        case FourCC('m', 'p', '4', 0):/* VHB0038_mp4v_MPG4_352x192_520Kbps_mp4a_32kHz_Stereo.mp4 */
            break;
            
        default:
            break;
    }
#endif
    return CLIR_OK;
}


static INT32 _cmpb_cli_openPUSH(
                                INT32        i4_argc,
                                const CHAR** pps_argv)
{
    INT32                               i4_ret = CLIR_OK;
    //CHAR*                               ps_name = NULL;
    IMTK_PB_ERROR_CODE_T                e_return;
    IMTK_PB_CTRL_SET_MEDIA_INFO_T       t_media_infor;
    IMTK_PB_CTRL_SETTING_T              t_Setting;
    IMTK_PB_CTRL_ENGINE_PARAM_T         t_parm;

    
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("cmpb.openpush mtka/mtkv/ps/ts/rmvb/asf/avi/mkv/flv/mp4 \n\r");
        return CLIR_OK;
    }

    b_pb_info_ready = FALSE;
    //ps_name = pps_argv[1];
    x_memset(&t_media_infor,    0, sizeof(IMTK_PB_CTRL_SET_MEDIA_INFO_T));
    x_memset(&t_parm,           0, sizeof(IMTK_PB_CTRL_ENGINE_PARAM_T));

    _cmpb_cli_get_media_info_by_fmt(_get_fmt_type(pps_argv[1]), &t_media_infor);
    
    e_return = IMtkPb_Ctrl_Open(&hPB_Handle,
                                IMTK_PB_CTRL_BUFFERING_MODEL_PUSH,
                                IMTK_PB_CTRL_APP_MASTER, 
                                NULL);
                                
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_Open() Failed !\n");
        return CLIR_INV_ARG;        
    }

    e_return = IMtkPb_Ctrl_RegCallback(hPB_Handle,
                            (VOID*)(&ui4_tag),
                            pb_cb
                            );
                            
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_RegCallback() Failed !\n");
        return CLIR_INV_ARG;        
    }        
    
    t_parm.u4PlayFlag = IMTK_PB_CTRL_PLAY_FLAG_VIDEO |
                        IMTK_PB_CTRL_PLAY_FLAG_AUDIO;

    e_return = IMtkPb_Ctrl_SetEngineParam(hPB_Handle,
                                            &t_parm
                                            );
        
    if (e_return != IMTK_PB_ERROR_CODE_OK)
    {
        x_dbg_stmt("IMtkPb_Ctrl_SetEngineParam() Failed !\n");
        return CLIR_INV_ARG;        
    }

    IMtkPb_Ctrl_SetMediaInfo(hPB_Handle, &t_media_infor);

    x_memset(&t_Setting, 0, sizeof(IMTK_PB_CTRL_SETTING_T));        

    e_return = IMtkPb_Ctrl_Play(hPB_Handle, 0);
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        x_dbg_stmt("IMtkPb_Ctrl_Play() Failed !\n");
        return i4_ret;        
    }
        
    b_pb_info_ready = TRUE;
    
    return i4_ret;
}


static INT32 _cmpb_cli_push_send_buffer (
                                            INT32        i4_argc,
                                            const CHAR** pps_argv) 
{
    UINT32 ui4_fmt_type = 0;

    if (i4_argc != 2)
    {
        x_dbg_stmt("Usage: sendbuf mtk/ps/ts/rmvb/asf/avi/mkv/flv/mp4 \n\r");
        return CLIR_OK;
    }
    
    ui4_fmt_type = _get_fmt_type(pps_argv[1]);
    
    switch(ui4_fmt_type)
    {
        case FourCC('m', 't', 'k', 'a'):/* ok.pcm */
            _push_thread_mtka(NULL);
            break;
            
        case FourCC('m', 't', 'k', 'v'):/* VideoStream */
            _push_thread_mtkv(NULL);
            break;
            
        case FourCC('p', 's', 0, 0):
            _push_thread_ps_ts("VGB0001_MPEG1_384x288_1071Kbps_ MP2_44.1kHz_Monophonic.mpeg");
            break;
            
        case FourCC('t', 's', 0, 0):
            _push_thread_ps_ts("Malata_061225Mon-PAT1080P_20061221.ts");
            break;
            
        case FourCC('r', 'm', 'v', 'b'):
            _push_thread_xx("Demo_750k_RV10_640x360_30fps_RA_650k_music.rm", 0, (UINT64)-1);
            break;
            
        case FourCC('w', 'm', 'v', 0):
        case FourCC('a', 's', 'f', 0):
            _push_thread_xx("fight.wmv", 5264, 79605050);
            break;
            
        case FourCC('a', 'v', 'i', 0):
            //_push_thread_xx("cbr.avi");
            _push_thread_xx("h376ShinjoujiShidareSakura10.avi", 6100, (UINT64)-1);
            break;
            
        case FourCC('m', 'k', 'v', 0):
            _push_thread_xx("VMB0007_DX50_640x352_947Kbps_MP3_44.1kHz_Stereo_CBR.mkv", 5555, (UINT64)-1);
            break;
            
        case FourCC('f', 'l', 'v', 0):
            _push_thread_xx("VMB0321_Superb_30fps_32kbps_22050Hz.flv", 9, (UINT64)-1);
            break;
            
        case FourCC('m', 'p', '4', 0):
            //_push_thread_xx("VHB0003_MP4V_176x144_192Kbps_MPEG-4_44.1kHz_Stereo.mp4");
            _push_thread_xx("VHB0038_mp4v_MPG4_352x192_520Kbps_mp4a_32kHz_Stereo.mp4", 0, (UINT64)-1);
            break;          
                        
        default:
            break;
    }

    return CLIR_OK;
}


static  HANDLE_T    h_file = NULL_HANDLE;

void _Ext_Buf_Cb_Fct(const char*             pc_buf,
                        long                    ui4_data_2,                            
                        void*                     pv_nfy_tag)
{
    INT32       i4_ret;
    UINT32      ui4_write_suc = 0;
    
x_dbg_stmt("======== len:0x%x =========\r\n", ui4_data_2);
    do
    {
        if(h_file == NULL_HANDLE)
        {
            #ifdef LINUX_TURNKEY_SOLUTION
            i4_ret = x_fm_open( FM_ROOT_HANDLE,
                                "/mnt/usb/sda1/test.pcm",
                               FM_WRITE_ONLY | FM_OPEN_CREATE | FM_OPEN_APPEND,
                                0777,
                                FALSE,
                                &h_file);
            #else
            i4_ret = x_fm_open( FM_ROOT_HANDLE,
                                "/mnt/usb/Mass-000/test.pcm",
                               FM_WRITE_ONLY | FM_OPEN_CREATE | FM_OPEN_APPEND,
                                0777,
                                FALSE,
                                &h_file);
            #endif

            if (i4_ret != FMR_OK)
            {
                break;
            }
        }
        i4_ret = x_fm_write(h_file,
                            (VOID*)pc_buf,
                            ui4_data_2,
                            &ui4_write_suc);
        if((i4_ret != FMR_OK) || (ui4_write_suc != ui4_data_2))
        {
            x_dbg_stmt("=====write file failed=== len:0x%x =========\r\n", ui4_write_suc);
            break;
        }        
    }while(0);
x_dbg_stmt("=====suc=== len:0x%x =========\r\n", ui4_data_2);
    //return ((i4_ret == FMR_OK)? IMTK_PB_CB_ERROR_CODE_OK : IMTK_PB_CB_ERROR_CODE_NOT_OK);
 }

static INT32 _cmpb_cli_capture(
                        INT32        i4_argc,
                        const CHAR** pps_argv)
{
    if (i4_argc != 2)
    {
        x_dbg_stmt("cmpb.capture [0/1]\n\r");
        return CLIR_OK;
    }

    if(x_strncasecmp(pps_argv[1], "0", 1) == 0)
    {
        IMtkPb_Ctrl_Ext_Capture_Setting(FALSE, 44100, 16, 2);
        if(h_file)
        {
            x_fm_close(h_file);
            h_file = NULL_HANDLE;
        }        
    }
    else if(x_strncasecmp(pps_argv[1], "1", 1) == 0)
    {
        IMtkPb_Ctrl_Ext_SetBufferSink(IMTK_PB_CTRL_STREAM_TYPE_AUDIO,
                                      _Ext_Buf_Cb_Fct,
                                      (void*)0x12345678);
        IMtkPb_Ctrl_Ext_Capture_Setting(TRUE, 44100, 16, 2);
    }

    return 0;
}

     


#endif /*CLI_LVL_ALL*/


