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
 * $RCSfile: wgl_animation_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/3 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: d812505873b2daf7bb6982376068738e $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#ifdef WGL_ANIM_SUPPORT 
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "wgl/wgl_sets.h"
#include "wgl/u_wgl_rect.h"
#include "dbg/x_dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "cli/x_cli.h"
#include "wgl/wgl.h"
#include "wgl/x_wgl.h"
#include "wgl/_wgl_cli.h"
#include "wgl/wgl_tools.h"
#include "wgl/wgl_sets_cli.h"
#include "wgl/wgl_anim.h"
#include "wgl/animation/wgl_animation_cli.h"
#include "wgl/animation/u_wgl_animation.h"
#include "wgl/animation/x_wgl_animation.h"
#include "dbg/def_dbg_level_mw.h"


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/


 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL

static INT32 _wgl_anim_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv); 
static INT32 _wgl_anim_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_init(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_free(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_test_anim(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_move(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_alpha(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_size(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_scale(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_img(INT32 i4_argc, const CHAR** pps_argv);

/*Path mode */
static INT32 _wgl_anim_cli_set_path_mode(
    WGL_ANIMATION_PATH_MODE_T*              pt_path_mode,
    UINT16                                  ui2_path_count,
    UINT8*                                  pui1_data);

static INT32 _wgl_anim_cli_add_path(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_remove_path(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_get_path(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_get_path_cnt(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_test_path(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_stress_test(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_do_anim_no_check(
    WGL_ANIMATION_DATA_T*               pt_begin,
    UINT32                              ui4_total_ticks);

static INT32 _wgl_anim_cli_bgn_free(
    WGL_ANIMATION_DATA_T*               pt_begin);

static INT32 _wgl_anim_cli_bgn_alloc(
    WGL_ANIMATION_DATA_T*               pt_begin,
    UINT16                              ui2_count,
    UINT32                              ui4_flag);

static INT32 _wgl_anim_cli_set_act_wgl_type(UINT16 ui2_data,HANDLE_TYPE_T* pe_type_widget);

static INT32 _wgl_anim_cli_add_action(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_rmv_action(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _wgl_anim_cli_get_action(INT32 i4_argc, const CHAR** pps_argv);

#endif
/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL

static UINT16 ui2_anim_dbg_level = DBG_INIT_LEVEL_MW_WGL_MNGR;

/*Animation sub module command table */
static CLI_EXEC_T _at_wgl_anim_cmd_tbl[] = 
{
    {CLI_GET_DBG_LVL_STR,   NULL,       _wgl_anim_cli_get_dbg_level,     NULL,       CLI_GET_DBG_LVL_HELP_STR,           CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,       _wgl_anim_cli_set_dbg_level,     NULL,       CLI_SET_DBG_LVL_HELP_STR,           CLI_SUPERVISOR},
    {"init",   NULL,       _wgl_anim_cli_init,     NULL,       "init animation",           CLI_SUPERVISOR},
    {"free",   NULL,       _wgl_anim_cli_free,     NULL,       "free animation",           CLI_SUPERVISOR},
    {"auto",   NULL,       _wgl_anim_cli_test_anim,     NULL,       "Test animation",           CLI_SUPERVISOR},
    {"move",   NULL,       _wgl_anim_cli_move,     NULL,       "Test move animation ",           CLI_SUPERVISOR},
    {"alpha",   NULL,       _wgl_anim_cli_alpha,     NULL,       "Test alpha animation ",           CLI_SUPERVISOR},
    {"size",   NULL,       _wgl_anim_cli_size,     NULL,       "Test size animation ",           CLI_SUPERVISOR},
    {"scale",   NULL,       _wgl_anim_cli_scale,     NULL,       "Test scale animation ",           CLI_SUPERVISOR},
    {"img",   NULL,       _wgl_anim_cli_img,     NULL,       "Test img animation ",           CLI_SUPERVISOR},
    {"add_path",   NULL,  _wgl_anim_cli_add_path,     NULL,       "Test path mode add.",           CLI_SUPERVISOR},
    {"rmv_path",   NULL,  _wgl_anim_cli_remove_path,  NULL,       "Test path mode remove.",           CLI_SUPERVISOR},
    {"get_path",   NULL,  _wgl_anim_cli_get_path,     NULL,       "Test path mode get path inf.",           CLI_SUPERVISOR},
    {"get_cnt",   NULL,  _wgl_anim_cli_get_path_cnt,     NULL,       "Test get path mode count.",           CLI_SUPERVISOR},
    {"path",   NULL,  _wgl_anim_cli_test_path,     NULL,       "Test get path mode count.",           CLI_SUPERVISOR},
    {"stress",   NULL,  _wgl_anim_cli_stress_test,     NULL,       "Animation stress test.",           CLI_SUPERVISOR},
    {"add_act",   NULL,  _wgl_anim_cli_add_action,     NULL,       "Test action map mode add.",           CLI_SUPERVISOR},
    {"rmv_act",   NULL,  _wgl_anim_cli_rmv_action,  NULL,       "Test action map remove.",           CLI_SUPERVISOR},
    {"get_act",   NULL,  _wgl_anim_cli_get_action,     NULL,       "Test action map get path inf.",           CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/* Widget Manager root command table */
#define CLI_IMG_MAX_TPL             10
static WGL_HIMG_TPL_T  _t_cli_imgs[CLI_IMG_MAX_TPL]= 
{
    0x100000,   0x100001,   0x100002,   0x100003,   0x100004,   
    0x100005,   0x100006,   0x100007,   0x100008,   0x100009   
};

static WGL_HIMG_TPL_T  _t_cli_imgs_2[CLI_IMG_MAX_TPL]= 
{
    0x200000,   0x200001,   0x200002,   0x200003,   0x200004,   
    0x200005,   0x200006,   0x200007,   0x200008,   0x200009   
};

static ANIM_IMG_CUST_INFO_T          a_cli_img_cust[CLI_IMG_MAX_TPL]={0};


static CLI_EXEC_T _t_wgl_anim_root_cmd_tbl =
{
    "anim",     NULL,        NULL,      _at_wgl_anim_cmd_tbl,      "Animation commands",   CLI_SUPERVISOR
};

#define END_ANIM_FLG          ((UINT32)0xFFFFFFFF)

#define END_MOVE_D            {0,      0,      0,      0,     0,     NULL}  

/*********************  move test data*********************/
static CLI_ANIM_MOVE_INF gt_move;
static GL_POINT_T _t_move_rst1[5] =
{
    { 0,      0},
    {10,     10},
    {20,     20},
    {30,     30},
    {40,     40}
};

static GL_POINT_T _t_move_rst2[5] =
{
    {100,     50},
    {150,     75},
    {200,     100},
    {250,     125},
    {300,     150}
};

static GL_POINT_T _t_move_rst3[5] =
{
    {100,     300},
    {150,     250},
    {200,     200},
    {250,     150},
    {300,     100}
};

static GL_POINT_T _t_move_rst4[5] =
{
    {300,     150},
    {250,     125},
    {200,     100},
    {150,     75},
    {100,      50}
};

#define CLI_ANIM_MOVE_MAX_NUM           4
static CLI_ANIM_MOVE_D_T _t_anim_move[CLI_ANIM_MOVE_MAX_NUM] =
{
    /*total step  start.x    start.y    end.x     end.y     */
    {   5,      0,      0,      40,     40,     _t_move_rst1},

    /*right down*/
    {   5,     100,     50,      300,    150,   _t_move_rst2},

    /*right up*/
    {   5,     100,     300,      300,   100,   _t_move_rst3},

    /*left up*/
    {   5,     300,     150,     100,     50,   _t_move_rst4}
    
};

/*********************  scale test data*********************/
static CLI_ANIM_SCALE_INF gt_scale;
static GL_SIZE_T    _t_scale_rst1[5]=
{
    {100,     50},
    {125,     75},
    {150,     100},
    {175,     125},
    {200,     150}
};

static GL_SIZE_T    _t_scale_rst2[5]=
{
    {300,     200},
    {375,     250},
    {450,     300},
    {525,     350},
    {600,     400}
};

static GL_SIZE_T    _t_scale_rst3[5]=
{
    {600,     400},
    {525,     350},
    {450,     300},
    {375,     250},
    {300,     200}
};

static GL_SIZE_T    _t_scale_rst4[5]=
{
    {600,     200},
    {525,     250},
    {450,     300},
    {375,     350},
    {300,     400}
};

static GL_SIZE_T    _t_scale_rst5[5]=
{
    {300,     400},
    {375,     350},
    {450,     300},
    {525,     250},
    {600,     200}
};

static GL_SIZE_T    _t_scale_rst6[3]=
{
    {300,     200},
    {450,     300},
    {600,     400}
};

#define CLI_ANIM_SCALE_MAX_NUM           6
static CLI_ANIM_SCALE_D_T _t_anim_scl[CLI_ANIM_SCALE_MAX_NUM]=
{
    /*total step  start.w    start.h    end.w     end.h     */
    /*width zoom in, height zoom in*/
    {   5,      100,    50,     200,     150,   _t_scale_rst1},
    
    /*width zoom in, height zoom in*/
    {   5,      300,    200,    600,     400,   _t_scale_rst2},

    /*width zoom out, height zoom out*/
    {   5,      600,    400,    300,     200,   _t_scale_rst3},

    /*width zoom out, height zoom in*/
    {   5,      600,    200,    300,     400,   _t_scale_rst4},

    /*width zoom in, height zoom out*/
    {   5,      300,    400,    600,     200,   _t_scale_rst5},

    /*width zoom in, height zoom in*/
    {   3,      300,    200,    600,     400,   _t_scale_rst6},
};


/********************  alpha test data*********************/
static CLI_ANIM_ALPHA_INF gt_alpha;

static UINT8 _t_alpha_rst1[5]={0,60,120,180,240};

static UINT8 _t_alpha_rst2[5]={240,180,120,60,0};

#define CLI_ANIM_ALPHA_MAX_NUM           2

static CLI_ANIM_ALPHA_D_T _t_anim_alpha[CLI_ANIM_ALPHA_MAX_NUM]=
{
    /*total step  start.alpha    end.alpha     */
     {  5,      0,         240,     _t_alpha_rst1},

     {  5,      240,         0,     _t_alpha_rst2}
};

/********************  img test data*********************/
static CLI_ANIM_IMG_INF gt_img;

static WGL_ANIM_IMG_INF_T _t_img_1[5]=
{
    {0,&_t_cli_imgs[0]},
    {1,&_t_cli_imgs[1]},
    {2,&_t_cli_imgs[2]},
    {3,&_t_cli_imgs[3]},
    {4,&_t_cli_imgs[4]}
};

static WGL_ANIM_IMG_INF_T _t_img_2[10]=
{
    {0,&_t_cli_imgs[0]},
    {1,&_t_cli_imgs[1]},
    {2,&_t_cli_imgs[2]},
    {3,&_t_cli_imgs[3]},
    {4,&_t_cli_imgs[4]},
    {5,&_t_cli_imgs[5]},
    {6,&_t_cli_imgs[6]},
    {7,&_t_cli_imgs[7]},
    {8,&_t_cli_imgs[8]},
    {9,&_t_cli_imgs[9]},
};

static WGL_ANIM_IMG_INF_T _t_img_3[5]=
{
    {8,&_t_cli_imgs[8]},
    {7,&_t_cli_imgs[7]},
    {6,&_t_cli_imgs[6]},
    {5,&_t_cli_imgs[5]},
    {1,&_t_cli_imgs[1]}
};

/*
typedef struct _CLI_ANIM_IMG_D_T
{
    UINT16                           ui2_total_steps;
    WGL_ANIM_PLAY_MODE_T             e_play_mode;
    UINT16                           ui2_loop_count;
    UINT8                            ui1_img_seq_id;
    UINT16                           ui2_cust_len;
    CLI_ANIM_IMG_CUST                at_cust_inf[5];
} CLI_ANIM_IMG_D_T;
*/
#define CLI_ANIM_IMG_MAX_NUM           3

static CLI_ANIM_IMG_D_T _t_anim_img[CLI_ANIM_IMG_MAX_NUM] =
{
    {5,   ANIM_PLAY_MODE_ONCE, 0, 0, 0,{CLI_CUST_NULL,CLI_CUST_NULL,CLI_CUST_NULL,CLI_CUST_NULL,CLI_CUST_NULL},_t_img_1},
    {5,   ANIM_PLAY_MODE_LOOP, 2, 0, 0,{CLI_CUST_NULL,CLI_CUST_NULL,CLI_CUST_NULL,CLI_CUST_NULL,CLI_CUST_NULL},_t_img_2},
    {5,   ANIM_PLAY_MODE_CUST, 0, 0, 5,{{TRUE,8,50},{TRUE,7,100},{TRUE,6,50},{TRUE,5,50},{TRUE,1,50}},_t_img_3}
};

/*****************************************************/
/*
typedef struct _CLI_ANIM_RGN
{
    UINT32                          ui4_tick;
    GL_RECT_T                       t_ref;
    UINT32                          ui4_update_rect_num;   
    GL_RECT_T                       at_update_rect[2];    
} CLI_ANIM_RGN;
*/

static CLI_ANIM_RGN _t_rgn_1[5]=
{
    { 0,{  0,  0, 300,200}, 1,{{  0,  0, 300,200},CLI_RECT_NULL}},
    { 1,{  0,  0, 300,200}, 1,{{  0,  0, 310,210},CLI_RECT_NULL}},
    { 2,{ 10, 10, 310,210}, 1,{{ 10, 10, 320,220},CLI_RECT_NULL}},
    { 3,{ 20, 20, 320,220}, 1,{{ 20, 20, 330,230},CLI_RECT_NULL}},
    { 4,{ 30, 30, 330,230}, 1,{{ 30, 30, 340,240},CLI_RECT_NULL}}
};

static CLI_ANIM_RGN _t_rgn_2[5]=
{
    { 0,{  0,  0, 300,200}, 1,{{  0,  0, 300,200},CLI_RECT_NULL}},
    { 1,{  0,  0, 300,200}, 1,{{  0,  0, 375,250},CLI_RECT_NULL}},
    { 2,{  0,  0, 310,210}, 1,{{  0,  0, 450,300},CLI_RECT_NULL}},
    { 3,{  0,  0, 320,220}, 1,{{  0,  0, 525,350},CLI_RECT_NULL}},
    { 4,{  0,  0, 330,230}, 1,{{  0,  0, 600,400},CLI_RECT_NULL}}
};

static CLI_ANIM_RGN _t_rgn_3[5]=
{
    { 0,{  0,  0, 600,400}, 1,{{  0,  0, 600,400},CLI_RECT_NULL}},
    { 1,{  0,  0, 600,400}, 1,{{  0,  0, 600,400},CLI_RECT_NULL}},
    { 2,{  0,  0, 525,350}, 1,{{  0,  0, 525,350},CLI_RECT_NULL}},
    { 3,{  0,  0, 450,300}, 1,{{  0,  0, 450,300},CLI_RECT_NULL}},
    { 4,{  0,  0, 375,250}, 1,{{  0,  0, 375,250},CLI_RECT_NULL}}
};

static CLI_ANIM_RGN _t_rgn_4[5]=
{
    { 0,{  0,  0, 300,200}, 1,{{  0,  0, 400,250},CLI_RECT_NULL}},
    { 1,{100, 50, 400,250}, 1,{{100, 50, 525,325},CLI_RECT_NULL}},
    { 2,{150, 75, 525,325}, 1,{{150, 75, 650,400},CLI_RECT_NULL}},
    { 3,{200,100, 650,400}, 1,{{200,100, 775,475},CLI_RECT_NULL}},
    { 4,{250,125, 775,475}, 1,{{250,125, 900,550},CLI_RECT_NULL}}
};

static CLI_ANIM_RGN _t_rgn_6[5]=
{
    { 0,{100, 50, 400,250}, 1,{{100, 50, 400,250},CLI_RECT_NULL}},
    { 3,{100, 50, 400,250}, 1,{{100, 50, 450,275},CLI_RECT_NULL}},
    { 6,{100, 50, 400,250}, 1,{{150, 75, 500,300},CLI_RECT_NULL}},
    { 9,{100, 50, 400,250}, 1,{{200, 100, 550,325},CLI_RECT_NULL}},
    {12,{100, 50, 400,250}, 1,{{250, 125, 600,350},CLI_RECT_NULL}}
};

static CLI_ANIM_RGN _t_rgn_7[5]=
{
    { 0,{100, 50, 140,70}, 1,{{100,50,140,70},CLI_RECT_NULL}},
    { 1,{100, 50, 140,70}, 2,{{100,50,140,70},{150,75,190,95}}},
    { 2,{100, 50, 140,70}, 2,{{150,75,190,95},{200,100,240,120}}},
    { 3,{100, 50, 140,70}, 2,{{200,100,240,120},{250,125,290,145}}},
    { 4,{100, 50, 140,70}, 2,{{250,125,290,145},{300,150,340,170}}}
};
/*
typedef struct _CLI_ANIM_BGN
{
    UINT32                          ui4_flag;
    UINT16                          ui2_id;
    UINT32                          ui4_start_tick;
    UINT16                          ui2_frame;
} CLI_ANIM_BGN;
typedef struct _CLI_ANIM_D
{
    UINT16                          ui2_count;
    CLI_ANIM_BGN                    at_bgn[CLI_BEGIN_NUM];
    UINT16                          ui2_rgn_num;
    CLI_ANIM_RGN*                   pt_rgn;
} CLI_ANIM_D;*/
/*
WGL_ANIM_TYPE_MOVE -> _t_anim_move
WGL_ANIM_TYPE_SCALE -> _t_anim_scl
WGL_ANIM_TYPE_ALPHA-> _t_anim_alpha
WGL_ANIM_TYPE_IMG->_t_anim_img
*/    
#define CLI_ANIM_D_NUM              26 

static CLI_ANIM_D _t_anim_data[CLI_ANIM_D_NUM] =
{
    /*move, fram == 1*interval*/
    { 1, {{WGL_ANIM_TYPE_MOVE,0,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},5, _t_rgn_1},

    { 1, {{WGL_ANIM_TYPE_MOVE,1,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},5, _t_rgn_7},

    { 1, {{WGL_ANIM_TYPE_MOVE,2,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    { 1, {{WGL_ANIM_TYPE_MOVE,3,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    /*scale,zoom out, fram == 1*interval*/
    { 1, {{WGL_ANIM_TYPE_SCALE,1,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},5, _t_rgn_2},

    /*scale,zoom in, fram == 1*interval*/
    { 1, {{WGL_ANIM_TYPE_SCALE,2,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},5, _t_rgn_3},

    { 1, {{WGL_ANIM_TYPE_SCALE,3,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    { 1, {{WGL_ANIM_TYPE_SCALE,4,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    /*alpha*/
    { 1, {{WGL_ANIM_TYPE_ALPHA,0,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    { 1, {{WGL_ANIM_TYPE_ALPHA,1,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

   /*10*/ /*10 Image animtion  */
    { 1, {{WGL_ANIM_TYPE_IMG,0,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    { 1, {{WGL_ANIM_TYPE_IMG,1,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    { 1, {{WGL_ANIM_TYPE_IMG,2,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    /*move + scale at the same time*/ 
    { 2, {{WGL_ANIM_TYPE_MOVE,1,0,50},{WGL_ANIM_TYPE_SCALE,1,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},5, _t_rgn_4},

    /*move + alpha at the same time*/ 
    { 2, {{WGL_ANIM_TYPE_MOVE,1,0,50},{WGL_ANIM_TYPE_ALPHA,0,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    /*move +scale + alpha at the same time*/ 
    { 3, {{WGL_ANIM_TYPE_MOVE,1,0,50},{WGL_ANIM_TYPE_SCALE,1,0,50},{WGL_ANIM_TYPE_ALPHA,0,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    /*move -> scale sequent animation*/ 
    { 2, {{WGL_ANIM_TYPE_MOVE,1,0,50},{WGL_ANIM_TYPE_SCALE,1,7,50},{WGL_ANIM_TYPE_ALPHA,0,0,50},CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    /*move -> alpha */ 
    { 2, {{WGL_ANIM_TYPE_MOVE,1,0,50},{WGL_ANIM_TYPE_ALPHA,0,7,50},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    /*move ->scale -> alpha */ 
    { 3, {{WGL_ANIM_TYPE_MOVE,1,0,50},{WGL_ANIM_TYPE_SCALE,1,7,50},{WGL_ANIM_TYPE_ALPHA,1,13,50},CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    /* move, fram == 3*interval*/
    { 1, {{WGL_ANIM_TYPE_MOVE,1,0,150},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},5, _t_rgn_6},

  /*20*/  /*scale, fram == 2*interval*/
    { 1, {{WGL_ANIM_TYPE_SCALE,3,0,100},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    /* move + scale at the same time, move frame =100,scale frame =150*/ 
    { 2, {{WGL_ANIM_TYPE_MOVE,1,0,100},{WGL_ANIM_TYPE_SCALE,1,0,150},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    /*move + scale at the same time, move frame =100,scale frame =100*/ 
    { 2, {{WGL_ANIM_TYPE_MOVE,1,0,100},{WGL_ANIM_TYPE_SCALE,1,0,100},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},
    
    /*move + scale , move frame =100,scale frame =150*/ 
    { 2, {{WGL_ANIM_TYPE_MOVE,1,0,100},{WGL_ANIM_TYPE_SCALE,1,3,150},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},


    /*move + scale , move frame =100,scale frame =150*/ 
    { 2, {{WGL_ANIM_TYPE_MOVE,1,3,100},{WGL_ANIM_TYPE_SCALE,1,3,150},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL},

    /*move + scale at the same time, move frame =100,scale frame =150*/ 
    { 2, {{WGL_ANIM_TYPE_MOVE,1,0,100},{WGL_ANIM_TYPE_SCALE,5,2,150},CLI_ANIM_NULL,CLI_ANIM_NULL,CLI_ANIM_NULL},0, NULL}

};
/*
typedef struct WGL_ANIM_PATH_DATA_T
{
    UINT16                              ui2_anim_type; 
    UINT16                              ui2_total_steps;          
    UINT16                              ui2_frame_interval;      
    UINT32                              ui4_start_tick;
    WGL_ANIM_PLAY_MODE_T                e_play_mode; 
    UINT16                              ui2_loop_count;
    wgl_app_anim_type_cb_fct            pf_type_cb;  
}WGL_ANIM_PATH_DATA_T;*/
#define CLI_ANIM_PATH_DATA_MAX      27
static WGL_ANIM_PATH_DATA_T _t_path_data[CLI_ANIM_PATH_DATA_MAX]=
{
    /*type                         total   frame  start         e_play_mode   count  cb */
    {WGL_ANIM_TYPE_MOVE,    5,  50,   0,ANIM_PLAY_MODE_ONCE,  0, NULL},/*0*/
    {WGL_ANIM_TYPE_ALPHA,   5,  50,   0,ANIM_PLAY_MODE_ONCE,  0, NULL},/*1*/
    {WGL_ANIM_TYPE_SCALE,   5,  50,   0,ANIM_PLAY_MODE_ONCE,  0, NULL},/*2*/
    {WGL_ANIM_TYPE_IMG,     5,  50,   0,ANIM_PLAY_MODE_ONCE,  0, NULL},/*3*/
    {WGL_ANIM_TYPE_IMG,     5,  50,   0,ANIM_PLAY_MODE_LOOP,  2, NULL},/*4*/
    {WGL_ANIM_TYPE_IMG,     5,  50,   0,ANIM_PLAY_MODE_CUST,  0, NULL},/*5*/
    {WGL_ANIM_TYPE_MOVE,    5,  50,   3,ANIM_PLAY_MODE_ONCE,  0, NULL},/*6*/
    {WGL_ANIM_TYPE_ALPHA,   5,  50,   6,ANIM_PLAY_MODE_ONCE,  0, NULL},/*7*/
    {WGL_ANIM_TYPE_MOVE,  5,  50,  0, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*8*/
    {WGL_ANIM_TYPE_MOVE,  5, 100,  0, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*9*/
    {WGL_ANIM_TYPE_MOVE,  5,  50,  3, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*10*/
    {WGL_ANIM_TYPE_MOVE,  5, 100,  3, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*11*/
    {WGL_ANIM_TYPE_ALPHA, 5,  50,  0, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*12*/
    {WGL_ANIM_TYPE_ALPHA, 5, 100,  0, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*13*/
    {WGL_ANIM_TYPE_ALPHA, 5,  50,  4, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*14*/
    {WGL_ANIM_TYPE_ALPHA, 5, 100,  4, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*15*/
    {WGL_ANIM_TYPE_SCALE, 5,  50,  0, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*16*/
    {WGL_ANIM_TYPE_SCALE, 5, 100,  0, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*17*/
    {WGL_ANIM_TYPE_SCALE, 5,  50,  4, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*18*/
    {WGL_ANIM_TYPE_SCALE, 5, 100,  3, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*19*/
    {WGL_ANIM_TYPE_IMG,   5,  50,  0, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*20*/
    {WGL_ANIM_TYPE_IMG,   5,  50,  0, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*21*/
    {WGL_ANIM_TYPE_IMG,   5, 100,  0, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*22*/
    {WGL_ANIM_TYPE_IMG,   5,  50,  4, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*23*/
    {WGL_ANIM_TYPE_IMG,   5, 100,  4, ANIM_PLAY_MODE_ONCE,  0,  NULL},/*24*/
    {WGL_ANIM_TYPE_IMG,   5, 100,  0, ANIM_PLAY_MODE_LOOP,  2,  NULL},/*25*/
    {WGL_ANIM_TYPE_IMG,   5, 100,  0, ANIM_PLAY_MODE_CUST,  2,  NULL}/*26*/
    
};
/*typedef struct _CLI_ANIM_PATH_MODE
{
    UINT32                          ui4_path_id;
    UINT16                          ui2_count;
    UINT8                           aui1_path_indx[CLI_PATH_DATA_COUNT_MAX];
} CLI_ANIM_PATH_MODE;*/
#define CLI_ANIM_PATH_MODE_MAX      11
static CLI_ANIM_PATH_MODE _t_path_mode[CLI_ANIM_PATH_MODE_MAX]=
{
    {CLI_PATH_MODE_MOVE,  1, {  0,  0,  0,  0,  0}},
    {CLI_PATH_MODE_ALPHA, 1, {  1,  0,  0,  0,  0}},
    {CLI_PATH_MODE_SCALE, 1, {  2,  0,  0,  0,  0}},
    {CLI_PATH_MODE_IMG_1, 1, {  3,  0,  0,  0,  0}},
    {CLI_PATH_MODE_IMG_2, 1, {  4,  0,  0,  0,  0}},
    {CLI_PATH_MODE_7,     1, {  5,  0,  0,  0,  0}},
    {CLI_PATH_MODE_8,     1, {  6,  0,  0,  0,  0}},
    {CLI_PATH_MODE_SCALE, 1, {  2,  0,  0,  0,  0}},
    {CLI_PATH_MODE_10,    2, {  0, 19,  0,  0,  0}},/*move + scale*/
    {CLI_PATH_MODE_11,    2, {  0, 17,  0,  0,  0}},/*move + scale*/
/*10*/    
    {CLI_PATH_MODE_12,    3, {  1, 11, 19,  0,  0}}
};

#endif
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *          wgl_anim_cli_dbg_level_get
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
#ifdef CLI_LVL_ALL
UINT16 wgl_anim_cli_dbg_level_get()
{
    UINT16 ui2_mode = wgl_sets_cli_mode_get();
    
    switch (ui2_mode)
    {
    case WGL_SETS_CLI_MODE_SETS_CTL:
        return ui2_anim_dbg_level;
        
    case WGL_SETS_CLI_MODE_MGR_MASTER:
        return (ui2_anim_dbg_level & wgl_get_dbg_level());
        
    case WGL_SETS_CLI_MODE_BOTH:
        return (ui2_anim_dbg_level | wgl_get_dbg_level());
        
    case WGL_SETS_CLI_MODE_MGR_CTL:
    default:
        return wgl_get_dbg_level();
    }
}
#endif

/*******************************************************************************
 * Name
 *          wgl_anim_cli_dbg_level_set
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
VOID wgl_anim_cli_dbg_level_set(UINT16 ui2_new_level)
{
#ifdef CLI_LVL_ALL
    ui2_anim_dbg_level = ui2_new_level;
#endif
}

/*******************************************************************************
 * Name
 *          wgl_anim_cli_init
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_anim_cli_init()
{
#ifdef CLI_LVL_ALL 
    INT32       i4_ret;    
    
    /* Attach Animation's CLI command table to Widget Manager's CLI table*/
    i4_ret = wgl_cli_reg_sub_table(&_t_wgl_anim_root_cmd_tbl);
    
    if (WGLR_OK != i4_ret)
    {
        return WGLR_INIT_FAILED;
    }
    
    return WGLR_OK;   
#else    
    return WGLR_OK;
#endif      
}

/*-----------------------------------------------------------------------------
 * private VIEW methods implementations
 *---------------------------------------------------------------------------*/ 
/*-----------------------------------------------------------------------------
 * Name: _wgl_anim_cli_get_dbg_level
 *
 * Description: 
 *
 * Inputs:  -
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _wgl_anim_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{     
    INT32       i4_return;
        
    if (i4_argc != 1)
    {
        x_dbg_stmt("{WGL(Anim)} anim.gdl\n\r");
        return CLIR_OK;
    }    
    
    i4_return = x_cli_show_dbg_level(wgl_anim_cli_dbg_level_get());
    
    return (i4_return);
}

/*-----------------------------------------------------------------------------
 * Name: _wgl_anim_cli_set_dbg_level
 *
 * Description: 
 *
 * Inputs:  -
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _wgl_anim_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;     
        
    /* Check arguments */
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("{WGL(Animation)} anim.sdl [level]\n\r");
        x_dbg_stmt("{WGL(Animation)}   [level]: n=none/e=error, a=api, i=info");
        return CLIR_OK;
    }    
    
    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);
    
    if (i4_return == CLIR_OK)
    {
        wgl_anim_cli_dbg_level_set(ui2_dbg_level);
    }
    else
    {
        x_dbg_stmt("{WGL(Animation)} anim.sdl [level]\n\r");
        x_dbg_stmt("{WGL(Animation)}   [level]: n=none/e=error, a=api, i=info");
        i4_return = CLIR_OK;
    }
    
    return (i4_return);
}

static  WGL_WIDGET_ANIM_CONTEXT_T        t_cli_anim;

static INT32 _wgl_anim_cli_init(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                       i4_ret;
    
    x_wgl_set_tick_interval(50);
    i4_ret = wgl_anim_create(&t_cli_anim);
    if(i4_ret == WGLR_OK)
    {
        x_dbg_stmt("[Anim.cli]Create animation OK! Animation=%0x\n\n",t_cli_anim);
    }
    else
    {
        x_dbg_stmt("[Anim.cli]Create animation Fail. ret=%d,Anim=%x!\n\n",i4_ret,t_cli_anim);
    }

    x_wgl_anim_path_init();
    return CLIR_OK;    
}

static INT32 _wgl_anim_cli_free(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                       i4_ret;
    
    i4_ret = wgl_anim_free(&t_cli_anim);
    if(i4_ret == WGLR_OK)
    {
        x_dbg_stmt("[Anim.cli]Create animation OK! Anim=%0x\n\n",t_cli_anim);
    }
    else
    {
        x_dbg_stmt("[Anim.cli]Create animation Fail. ret=%d,Anim=%x!\n\n",i4_ret,t_cli_anim);
    }

    return CLIR_OK;    
}

static INT32 _wgl_anim_set_move_data(
    WGL_ANIMATION_DATA_T*           pt_begin,
    UINT32                          ui4_start_tick,
    UINT16                          ui2_frame_interval,
    UINT16                          ui2_id,
    UINT16                          ui2_index)
{
    CLI_ANIM_MOVE_D_T*                      pt_move;
    WGL_ANIM_DATA_INFO*                     pt_anim_data;
    WGL_ANIM_PATH_DATA_T*                   pt_path_mode;

    if((pt_begin == NULL) || (ui2_index >= pt_begin->ui2_count) || 
       (ui2_id >= CLI_ANIM_MOVE_MAX_NUM))
    {
        return CLIR_INV_ARG;
    }

    pt_move = &_t_anim_move[ui2_id];
    if(pt_move == NULL || pt_move->ui2_total_steps == 0)
    {
        return CLIR_INV_ARG;
    }
    
    if(WGL_IS_FLAG_TURN_ON(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF))
    {
        pt_path_mode = &(pt_begin->pt_path_mode[ui2_index]);
        if(pt_path_mode == NULL)
        {
            return CLIR_INV_ARG;
        }
        WGL_SET_FLAG(pt_path_mode->ui2_anim_type,WGL_ANIM_TYPE_MOVE);
        pt_path_mode->ui2_total_steps = pt_move->ui2_total_steps;
        pt_path_mode->ui4_start_tick = ui4_start_tick;
        pt_path_mode->ui2_frame_interval = ui2_frame_interval;
        pt_path_mode->pf_type_cb = NULL;
    }

    if(WGL_IS_FLAG_TURN_ON(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF))
    {
        pt_anim_data = &(pt_begin->pt_anim_data[ui2_index]);
        if(pt_anim_data == NULL)
        {
            return CLIR_INV_ARG;
        }        
        WGL_SET_FLAG(pt_anim_data->ui2_anim_type,WGL_ANIM_TYPE_MOVE);
        pt_anim_data->u_data.t_move.i2_start_x = pt_move->i2_start_x;
        pt_anim_data->u_data.t_move.i2_start_y = pt_move->i2_start_y;
        pt_anim_data->u_data.t_move.i2_end_x = pt_move->i2_end_x;
        pt_anim_data->u_data.t_move.i2_end_y = pt_move->i2_end_y;
    }

    gt_move.aui4_srt_tick[gt_move.ui2_cnt] = ui4_start_tick;
    gt_move.aui2_frame[gt_move.ui2_cnt] = ui2_frame_interval;
    gt_move.at_move[gt_move.ui2_cnt] = pt_move;
    gt_move.ui2_cnt ++;
    
    return CLIR_OK;
}

static INT32 _wgl_anim_set_scale_data(
    WGL_ANIMATION_DATA_T*           pt_begin,
    UINT32                          ui4_start_tick,
    UINT16                          ui2_frame_interval,
    UINT16                          ui2_id,
    UINT16                          ui2_index)
{
    CLI_ANIM_SCALE_D_T*                      pt_anim;
    WGL_ANIM_DATA_INFO*                     pt_anim_data;
    WGL_ANIM_PATH_DATA_T*                   pt_path_mode;

    if((pt_begin == NULL) || (ui2_index >= pt_begin->ui2_count) || 
       (ui2_id >= CLI_ANIM_SCALE_MAX_NUM))
    {
        return CLIR_INV_ARG;
    }

    pt_anim = &_t_anim_scl[ui2_id];
    if(pt_anim == NULL || pt_anim->ui2_total_steps == 0)
    {
        return CLIR_INV_ARG;
    }
    
    if(WGL_IS_FLAG_TURN_ON(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF))
    {
        pt_path_mode = &(pt_begin->pt_path_mode[ui2_index]);
        if(pt_path_mode == NULL)
        {
            return CLIR_INV_ARG;
        }
        WGL_SET_FLAG(pt_path_mode->ui2_anim_type,WGL_ANIM_TYPE_SCALE);
        pt_path_mode->ui2_total_steps = pt_anim->ui2_total_steps;
        pt_path_mode->ui4_start_tick = ui4_start_tick;
        pt_path_mode->ui2_frame_interval = ui2_frame_interval;
        pt_path_mode->pf_type_cb = NULL;
    }

    if(WGL_IS_FLAG_TURN_ON(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF))
    {
        pt_anim_data = &(pt_begin->pt_anim_data[ui2_index]);
        if(pt_anim_data == NULL)
        {
            return CLIR_INV_ARG;
        }        
        WGL_SET_FLAG(pt_anim_data->ui2_anim_type,WGL_ANIM_TYPE_SCALE);
        pt_anim_data->u_data.t_scale.ui8_start_w = pt_anim->ui8_start_w;
        pt_anim_data->u_data.t_scale.ui8_start_h = pt_anim->ui8_start_h;
        pt_anim_data->u_data.t_scale.ui8_end_w = pt_anim->ui8_end_w;
        pt_anim_data->u_data.t_scale.ui8_end_h = pt_anim->ui8_end_h;
    }

    gt_scale.aui4_srt_tick[gt_scale.ui2_cnt] = ui4_start_tick;
    gt_scale.aui2_frame[gt_scale.ui2_cnt] = ui2_frame_interval;
    gt_scale.at_anim[gt_scale.ui2_cnt] = pt_anim;
    gt_scale.ui2_cnt ++;
    
    return CLIR_OK;
}

static INT32 _wgl_anim_set_alpha_data(
    WGL_ANIMATION_DATA_T*           pt_begin,
    UINT32                          ui4_start_tick,
    UINT16                          ui2_frame_interval,
    UINT16                          ui2_id,
    UINT16                          ui2_index)
{
    CLI_ANIM_ALPHA_D_T*                     pt_anim;
    WGL_ANIM_DATA_INFO*                     pt_anim_data;
    WGL_ANIM_PATH_DATA_T*                   pt_path_mode;

    if((pt_begin == NULL) || (ui2_index >= pt_begin->ui2_count) || 
       (ui2_id >= CLI_ANIM_ALPHA_MAX_NUM))
    {
        return CLIR_INV_ARG;
    }

    pt_anim = &_t_anim_alpha[ui2_id];
    if(pt_anim == NULL || pt_anim->ui2_total_steps == 0)
    {
        return CLIR_INV_ARG;
    }
    
    if(WGL_IS_FLAG_TURN_ON(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF))
    {
        pt_path_mode = &(pt_begin->pt_path_mode[ui2_index]);
        if(pt_path_mode == NULL)
        {
            return CLIR_INV_ARG;
        }
        WGL_SET_FLAG(pt_path_mode->ui2_anim_type,WGL_ANIM_TYPE_ALPHA);
        pt_path_mode->ui2_total_steps = pt_anim->ui2_total_steps;
        pt_path_mode->ui4_start_tick = ui4_start_tick;
        pt_path_mode->ui2_frame_interval = ui2_frame_interval;
        pt_path_mode->pf_type_cb = NULL;
    }

    if(WGL_IS_FLAG_TURN_ON(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF))
    {
        pt_anim_data = &(pt_begin->pt_anim_data[ui2_index]);
        if(pt_anim_data == NULL)
        {
            return CLIR_INV_ARG;
        }        
        WGL_SET_FLAG(pt_anim_data->ui2_anim_type,WGL_ANIM_TYPE_ALPHA);
        pt_anim_data->u_data.t_alpha.ui1_start_alpha = pt_anim->ui1_start;
        pt_anim_data->u_data.t_alpha.ui1_end_alpha = pt_anim->ui1_end;
    }

    gt_alpha.aui4_srt_tick[gt_alpha.ui2_cnt] = ui4_start_tick;
    gt_alpha.aui2_frame[gt_alpha.ui2_cnt] = ui2_frame_interval;
    gt_alpha.at_anim[gt_alpha.ui2_cnt] = pt_anim;
    gt_alpha.ui2_cnt ++;
    
    return CLIR_OK;
}

static INT32 _wgl_anim_set_img_data(
    WGL_ANIMATION_DATA_T*           pt_begin,
    UINT32                          ui4_start_tick,
    UINT16                          ui2_frame_interval,
    UINT16                          ui2_id,
    UINT16                          ui2_index)
{
    CLI_ANIM_IMG_D_T*                       pt_anim;
    WGL_ANIM_DATA_INFO*                     pt_anim_data;
    WGL_ANIM_PATH_DATA_T*                   pt_path_mode;
    UINT16                                  ui2_i;

    if((pt_begin == NULL) || (ui2_index >= pt_begin->ui2_count) || 
       (ui2_id >= CLI_ANIM_IMG_MAX_NUM))
    {
        return CLIR_INV_ARG;
    }

    pt_anim = &_t_anim_img[ui2_id];
    if(pt_anim == NULL || pt_anim->ui2_total_steps == 0)
    {
        return CLIR_INV_ARG;
    }
    
    if(WGL_IS_FLAG_TURN_ON(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF))
    {
        pt_path_mode = &(pt_begin->pt_path_mode[ui2_index]);
        if(pt_path_mode == NULL)
        {
            return CLIR_INV_ARG;
        }
        WGL_SET_FLAG(pt_path_mode->ui2_anim_type,WGL_ANIM_TYPE_IMG);
        pt_path_mode->ui2_total_steps = pt_anim->ui2_total_steps;
        pt_path_mode->ui4_start_tick = ui4_start_tick;
        pt_path_mode->ui2_frame_interval = ui2_frame_interval;
        pt_path_mode->pf_type_cb = NULL;
        pt_path_mode->e_play_mode = pt_anim->e_play_mode;
        pt_path_mode->ui2_loop_count = pt_anim->ui2_loop_count;
    }

    if(WGL_IS_FLAG_TURN_ON(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF))
    {
        pt_anim_data = &(pt_begin->pt_anim_data[ui2_index]);
        if(pt_anim_data == NULL)
        {
            return CLIR_INV_ARG;
        }        
        WGL_SET_FLAG(pt_anim_data->ui2_anim_type,WGL_ANIM_TYPE_IMG);
        pt_anim_data->u_data.t_img.ui1_img_seq_id = pt_anim->ui1_img_seq_id;
        
        if(pt_anim->e_play_mode == ANIM_PLAY_MODE_CUST)
        {
            if(pt_anim->ui2_cust_len > CLI_CUST_IMG_MAX)
            {
                pt_anim_data->u_data.t_img.ui2_cust_len = (UINT16)(CLI_CUST_IMG_MAX);
            }
            else
            {
                pt_anim_data->u_data.t_img.ui2_cust_len = pt_anim->ui2_cust_len;
            }

            for(ui2_i = 0; ui2_i < pt_anim_data->u_data.t_img.ui2_cust_len; ui2_i ++)
            {
                if(ui2_i >= CLI_CUST_IMG_MAX)
                {
                    break;
                }
                wgl_anim_pack_img_cust_info(&(a_cli_img_cust[ui2_i]),
                    pt_anim->at_cust_inf[ui2_i].b_flag,
                    pt_anim->at_cust_inf[ui2_i].ui2_img_id,
                    pt_anim->at_cust_inf[ui2_i].ui2_interval); 
            }

            pt_anim_data->u_data.t_img.pt_cust_info = &(a_cli_img_cust[0]);
        }        

    }

    gt_img.aui4_srt_tick[gt_img.ui2_cnt] = ui4_start_tick;
    gt_img.aui2_frame[gt_img.ui2_cnt] = ui2_frame_interval;
    gt_img.at_anim[gt_img.ui2_cnt] = pt_anim;
    gt_img.ui2_cnt ++;
    
    return CLIR_OK;
}

static UINT16 _wgl_anim_find_move_inf(
    UINT32                              ui4_tick_seq)
{
    UINT16                          ui2_i;                          

    for(ui2_i = 0; ui2_i < gt_move.ui2_cnt; ui2_i ++)
    {
        if(ui2_i == (gt_move.ui2_cnt - 1))
        {
            if(ui4_tick_seq >= gt_move.aui4_srt_tick[ui2_i])
            {
                return ui2_i;
            }
        }
        else if((ui4_tick_seq >= gt_move.aui4_srt_tick[ui2_i]) && 
           (ui4_tick_seq <= gt_move.aui4_srt_tick[(ui2_i + 1)]))
        {
            return ui2_i;
        }
    }

    return 0xFFFF;
}

static UINT16 _wgl_anim_find_scale_inf(
    UINT32                              ui4_tick_seq)
{
    UINT16                          ui2_i;                          

    for(ui2_i = 0; ui2_i < gt_scale.ui2_cnt; ui2_i ++)
    {
        if(ui2_i == (gt_scale.ui2_cnt - 1))
        {
            if(ui4_tick_seq >= gt_scale.aui4_srt_tick[ui2_i])
            {
                return ui2_i;
            }
        }
        else if((ui4_tick_seq >= gt_scale.aui4_srt_tick[ui2_i]) && 
           (ui4_tick_seq <= gt_scale.aui4_srt_tick[(ui2_i + 1)]))
        {
            return ui2_i;
        }
    }

    return 0xFFFF;
}

static UINT16 _wgl_anim_find_img_inf(
    UINT32                              ui4_tick_seq)
{
    UINT16                          ui2_i;                          

    for(ui2_i = 0; ui2_i < gt_img.ui2_cnt; ui2_i ++)
    {
        if(ui2_i == (gt_img.ui2_cnt - 1))
        {
            if(ui4_tick_seq >= gt_img.aui4_srt_tick[ui2_i])
            {
                return ui2_i;
            }
        }
        else if((ui4_tick_seq >= gt_img.aui4_srt_tick[ui2_i]) && 
           (ui4_tick_seq <= gt_img.aui4_srt_tick[(ui2_i + 1)]))
        {
            return ui2_i;
        }
    }

    return 0xFFFF;
}

static UINT16 _wgl_anim_find_alpha_inf(
    UINT32                              ui4_tick_seq)
{
    UINT16                          ui2_i;                          

    for(ui2_i = 0; ui2_i < gt_alpha.ui2_cnt; ui2_i ++)
    {
        if(ui2_i == (gt_alpha.ui2_cnt - 1))
        {
            if(ui4_tick_seq >= gt_alpha.aui4_srt_tick[ui2_i])
            {
                return ui2_i;
            }
        }
        else if((ui4_tick_seq >= gt_alpha.aui4_srt_tick[ui2_i]) && 
           (ui4_tick_seq <= gt_alpha.aui4_srt_tick[(ui2_i + 1)]))
        {
            return ui2_i;
        }
    }

    return 0xFFFF;
}

#define COMP_RECT(rect, rect2)       \
  ( ((rect)->i4_left == (rect2)->i4_left) && \
    ((rect)->i4_top == (rect2)->i4_top) && \
    ((rect)->i4_right == (rect2)->i4_right) && \
    ((rect)->i4_bottom == (rect2)->i4_bottom) )
#if 0    
static INT32 _wgl_anim_get_move_inf(
    GL_POINT_T*                     pt_rst_out,
    UINT16                          ui2_i,
    UINT32                          ui4_tick_seq)
{
    UINT16                          ui2_step;                          
    UINT32                          ui4_start_tick;
    UINT16                          ui2_frame_interval;
    UINT16                          ui2_total_step;
    GL_POINT_T*                     pt_rst;
    
        
    pt_rst = gt_move.at_move[ui2_i]->pt_rst;
    
    if(pt_rst == NULL)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: [%d]Check move fail. File=%s,Line=%d\n\n",ui4_tick_seq,__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }

    ui4_start_tick = gt_move.aui4_srt_tick[ui2_i];
    ui2_frame_interval = gt_move.aui2_frame[ui2_i];
    ui2_total_step = gt_move.at_move[ui2_i]->ui2_total_steps;
    if(ui4_tick_seq >= ui4_start_tick)
    {
        ui2_step = (UINT16)(((ui4_tick_seq - ui4_start_tick) * x_wgl_get_tick_interval())/ui2_frame_interval);
    }
    else
    {
        ui2_step = 0;
    }
    
    /*Ingore check result when the it is after the check total step.*/
    if(ui2_step >= ui2_total_step)
    {
        return CLIR_OK;
    }

    if(pt_rst_out != NULL)
    {
        *pt_rst_out = pt_rst[ui2_step];
    }

    return CLIR_OK;    
}
#endif
    
static INT32 _wgl_anim_check_move(
    WGL_ANIM_TICK_DATA              t_tick_inf,
    UINT16                          ui2_i,
    UINT32                          ui4_tick_seq)
{
    UINT16                          ui2_step;                          
    UINT32                          ui4_start_tick;
    UINT16                          ui2_frame_interval;
    UINT16                          ui2_total_step;
    GL_POINT_T*                     pt_rst;
    
        
    pt_rst = gt_move.at_move[ui2_i]->pt_rst;
    
    if(pt_rst == NULL)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: [%d]Check move fail. File=%s,Line=%d\n\n",ui4_tick_seq,__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }

    ui4_start_tick = gt_move.aui4_srt_tick[ui2_i];
    ui2_frame_interval = gt_move.aui2_frame[ui2_i];
    ui2_total_step = gt_move.at_move[ui2_i]->ui2_total_steps;
    if(ui4_tick_seq >= ui4_start_tick)
    {
        ui2_step = (UINT16)(((ui4_tick_seq - ui4_start_tick) * x_wgl_get_tick_interval())/ui2_frame_interval);
    }
    else
    {
        ui2_step = 0;
    }
    
    /*Ingore check result when the it is after the check total step.*/
    if(ui2_step >= ui2_total_step)
    {
        return CLIR_OK;
    }

    if( (pt_rst[ui2_step].i4_x != t_tick_inf.t_new_tick.t_tick_inf.t_point.i4_x) || 
        (pt_rst[ui2_step].i4_y != t_tick_inf.t_new_tick.t_tick_inf.t_point.i4_y))
    {
        x_dbg_stmt("{ANIM.Cli} ERR: [%d]Check move fail. File=%s,Line=%d\n\n",ui4_tick_seq,__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }

    return CLIR_OK;    
}

static INT32 _wgl_anim_check_scale(
    WGL_ANIM_TICK_DATA              t_tick_inf,
    UINT16                          ui2_i,
    UINT32                          ui4_tick_seq)
{
                              
    UINT16                          ui2_step;                          
    GL_SIZE_T*                      pt_rst = NULL;
    UINT32                          ui4_start_tick;
    UINT16                          ui2_frame_interval;
    UINT16                          ui2_total_step;
    
     pt_rst = gt_scale.at_anim[ui2_i]->pt_rst;
    
    if(pt_rst == NULL)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: [%d]Check move fail. File=%s,Line=%d\n\n",ui4_tick_seq,__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }

    ui4_start_tick = gt_scale.aui4_srt_tick[ui2_i];
    ui2_frame_interval = gt_scale.aui2_frame[ui2_i];
    ui2_total_step = gt_scale.at_anim[ui2_i]->ui2_total_steps;
    ui2_step = (UINT16)(((ui4_tick_seq - ui4_start_tick) * x_wgl_get_tick_interval())/ui2_frame_interval);

    /*Ingore check result when the it is after the check total step.*/
    if(ui2_step >= ui2_total_step)
    {
        return CLIR_OK;
    }

    if( (pt_rst[ui2_step].ui4_width != t_tick_inf.t_new_tick.t_tick_inf.t_scale.ui4_width) || 
        (pt_rst[ui2_step].ui4_height != t_tick_inf.t_new_tick.t_tick_inf.t_scale.ui4_height))
    {
        x_dbg_stmt("{ANIM.Cli} ERR: [%d]Check move fail. File=%s,Line=%d\n\n",ui4_tick_seq,__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }
    
    return CLIR_OK;    
}

static INT32 _wgl_anim_check_alpha(
    WGL_ANIM_TICK_DATA              t_tick_inf,
    UINT16                          ui2_i,
    UINT32                          ui4_tick_seq)
{
                              
    UINT16                          ui2_step;                          
    UINT8*                          pt_rst = NULL;
    UINT32                          ui4_start_tick;
    UINT16                          ui2_frame_interval;
    UINT16                          ui2_total_step;
    
     pt_rst = gt_alpha.at_anim[ui2_i]->pt_rst;
    
    if(pt_rst == NULL)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: [%d]Check alpha fail. File=%s,Line=%d\n\n",ui4_tick_seq,__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }

    ui4_start_tick = gt_alpha.aui4_srt_tick[ui2_i];
    ui2_frame_interval = gt_alpha.aui2_frame[ui2_i];
    ui2_total_step = gt_alpha.at_anim[ui2_i]->ui2_total_steps;
    ui2_step = (UINT16)(((ui4_tick_seq - ui4_start_tick) * x_wgl_get_tick_interval())/ui2_frame_interval);

    /*Ingore check result when the it is after the check total step.*/
    if(ui2_step >= ui2_total_step)
    {
        return CLIR_OK;
    }

    if( (pt_rst[ui2_step]) != t_tick_inf.t_new_tick.t_tick_inf.ui1_alpha)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: [%d]Check alpha fail. File=%s,Line=%d\n\n",ui4_tick_seq,__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }
    
    return CLIR_OK;    
}

static INT32 _wgl_anim_check_img(
    WGL_ANIM_TICK_DATA              t_tick_inf,
    UINT16                          ui2_i,
    UINT32                          ui4_tick_seq)
{
                              
    UINT16                          ui2_step;                          
    WGL_ANIM_IMG_INF_T*             pt_rst = NULL;
    UINT32                          ui4_start_tick;
    UINT16                          ui2_frame_interval;
    UINT16                          ui2_total_step;
    UINT64                          ui8_time; 
    UINT64                          ui8_data=0; 
    
    
     pt_rst = gt_img.at_anim[ui2_i]->pt_rst;
    
    if(pt_rst == NULL)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: [%d]Check img fail. File=%s,Line=%d\n\n",ui4_tick_seq,__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }

    ui4_start_tick = gt_img.aui4_srt_tick[ui2_i];
    ui2_frame_interval = gt_img.aui2_frame[ui2_i];
    ui2_total_step = gt_img.at_anim[ui2_i]->ui2_total_steps;
    
    if(gt_img.at_anim[ui2_i]->e_play_mode == ANIM_PLAY_MODE_CUST)
    {
        for(ui2_step =0; ui2_step < gt_img.at_anim[ui2_i]->ui2_cust_len;ui2_step++)
        {
            ui8_time = ((ui4_tick_seq - ui4_start_tick) * x_wgl_get_tick_interval());
            ui8_data += gt_img.at_anim[ui2_i]->at_cust_inf[ui2_step].ui2_interval;
            if(ui8_time < ui8_data)
            {
                break;
            }
        }
    }
    else
    {
        ui2_step = (UINT16)(((ui4_tick_seq - ui4_start_tick) * x_wgl_get_tick_interval())/ui2_frame_interval);
    }

    /*Ingore check result when the it is after the check total step.*/
    if(ui2_step >= ui2_total_step)
    {
        return CLIR_OK;
    }

    if(pt_rst[ui2_step].ui2_img_id != t_tick_inf.t_new_tick.t_tick_inf.t_img.ui2_img_id) 
    {
        x_dbg_stmt("{ANIM.Cli} ERR: [%d]Check img fail. File=%s,Line=%d\n\n",ui4_tick_seq,__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }

    if((pt_rst[ui2_step].pt_img != NULL) &&
       ((*pt_rst[ui2_step].pt_img) != (*t_tick_inf.t_new_tick.t_tick_inf.t_img.pt_img)))
    {
        x_dbg_stmt("{ANIM.Cli} ERR: [%d]Check img fail. File=%s,Line=%d\n\n",ui4_tick_seq,__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }
    
    return CLIR_OK;    
}


static BOOL _wgl_anim_get_region(
    CLI_ANIM_D*                     pt_anim,
    UINT32                          ui4_tick_seq,
    CLI_ANIM_RGN*                   pt_regin)
{
    UINT16                          ui2_i;
    BOOL                            b_find = FALSE;

    if(pt_anim == NULL)
    {
            x_dbg_stmt("{ANIM.Cli} ERR: Check region para Fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            return FALSE;
    }

    for(ui2_i=0;ui2_i < pt_anim->ui2_rgn_num;ui2_i++)
    {
        if(ui4_tick_seq == pt_anim->pt_rgn[ui2_i].ui4_tick)
        {
            b_find = TRUE;
            break;
        }
        else if(ui4_tick_seq < pt_anim->pt_rgn[ui2_i].ui4_tick)
        {
            break;
        }
    }    

    if(pt_regin != NULL)
    {
        x_memcpy(pt_regin, &(pt_anim->pt_rgn[ui2_i]),sizeof(CLI_ANIM_RGN));
    }
    
    return b_find;
}
static INT32 _wgl_anim_check_region(
    WGL_UPD_REGION_T                t_upd_rgn,
    CLI_ANIM_D*                     pt_anim,
    UINT32                          ui4_tick_seq)
{
    UINT16                          ui2_i;
    UINT16                          ui2_j;
    BOOL                            b_find = FALSE;
    
    if(pt_anim == NULL)
    {
            x_dbg_stmt("{ANIM.Cli} ERR: Check region para Fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            return CLIR_INV_ARG;
    }

    if(t_upd_rgn.ui4_update_rect_num > 2)
    {
            x_dbg_stmt("{ANIM.Cli} ERR: Check region para Fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            return CLIR_INV_ARG;
    }

    /*When the tick seq is large the check region tick num,do not check the region.*/
    if(ui4_tick_seq >= pt_anim->ui2_rgn_num)
    {
        return CLIR_OK; 
    }
    
    for(ui2_i=0;ui2_i < pt_anim->ui2_rgn_num;ui2_i++)
    {
        if(ui4_tick_seq == pt_anim->pt_rgn[ui2_i].ui4_tick)
        {
            b_find = TRUE;
            break;
        }
        else if(ui4_tick_seq < pt_anim->pt_rgn[ui2_i].ui4_tick)
        {
            break;
        }
    }

    if(b_find == TRUE)
    {
        if((t_upd_rgn.ui4_update_rect_num == pt_anim->pt_rgn[ui2_i].ui4_update_rect_num))
        {
            if(t_upd_rgn.ui4_update_rect_num > 0)
            {
                for(ui2_j= 0; ui2_j < t_upd_rgn.ui4_update_rect_num; ui2_j ++)
                {
                    if(COMP_RECT(&(t_upd_rgn.at_update_rect[ui2_j]),
                        &(pt_anim->pt_rgn[ui2_i].at_update_rect[ui2_j])) == FALSE)
                    {
                        x_dbg_stmt("{ANIM.Cli} ERR: Check region Fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
                        return CLIR_INV_ARG;
                    }
                }
            }
        }
        else
        {
            x_dbg_stmt("{ANIM.Cli} ERR: Check region Fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            return CLIR_INV_ARG;
        }
    }
    else if(t_upd_rgn.ui4_update_rect_num != 0)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: Check region Fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }
    
    return CLIR_OK;    
}

static INT32 _wgl_anim_do_anim(
    WGL_ANIMATION_DATA_T*               pt_begin,
    CLI_ANIM_D*                         pt_anim)
{
    INT32                           i4_ret = CLIR_OK;
    UINT32                          ui4_total_ticks = 0;
    UINT32                          ui4_tmp = 0;
    UINT16                          ui2_i;
    UINT32                          ui4_tick;
    UINT16                          ui2_total_steps;          
    UINT16                          ui2_frame_interval;      
//    WGL_ANIM_DATA_INFO*             pt_anim_data;  
    WGL_ANIM_PATH_DATA_T*           pt_path_mode;    
   // WGL_ANIM_PATH_ID_INFO_T         t_path_id_inf; 
    WGL_ANIM_TICK_DATA              t_tick_inf;
    WGL_ANIM_COND_INF_T             t_cond;
    UINT16                          ui2_move_idx;
    UINT16                          ui2_alpha_idx;
    UINT16                          ui2_scale_idx;
    UINT16                          ui2_img_idx;
    GL_RECT_T                       t_rect;
    WGL_UPD_REGION_T                t_update_region;
    CLI_ANIM_RGN                    t_rgn_data;
    

    if(pt_begin == NULL)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: Do animation inv. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }

    for(ui2_i =0; ui2_i < pt_begin->ui2_count; ui2_i ++)
    {
        pt_path_mode = &(pt_begin->pt_path_mode[ui2_i]);
        if(pt_path_mode ==  NULL)
        {
            x_dbg_stmt("{ANIM.Cli} ERR: Do animation begin data Fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            return CLIR_INV_ARG;
        }
        
        ui2_total_steps = pt_path_mode->ui2_total_steps;
        ui2_frame_interval = pt_path_mode->ui2_frame_interval;
        ui4_tmp = (ui2_frame_interval * ui2_total_steps) / x_wgl_get_tick_interval();
        ui4_tmp += pt_path_mode->ui4_start_tick;
        if(ui4_total_ticks < ui4_tmp)
        {
            ui4_total_ticks = ui4_tmp;
        }
        
        pt_path_mode ++;
    }
    
    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_BEGIN, WGL_PACK(pt_begin), NULL);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: BEGIN FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }



    for(ui4_tick =0; ui4_tick <= ui4_total_ticks; ui4_tick ++)
    {
        x_memset(&t_rect,0,sizeof(GL_RECT_T));

        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_TICK,WGL_PACK(ui4_tick), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Tick FAIL. Tick=%d,File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
            return CLIR_CMD_EXEC_ERROR;
        }

        x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_tick_inf));
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Get tick info FAIL. Tick=%d,File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
            return CLIR_CMD_EXEC_ERROR;
        }

        SET_RECT(&t_rect,0,0,300,200); 
        if(TRUE == _wgl_anim_get_region(pt_anim,ui4_tick,&t_rgn_data))
        {
            x_memcpy(&t_rect,&(t_rgn_data.t_ref),sizeof(GL_RECT_T));
        }
        
        i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_UPD_RGN,WGL_PACK(&t_rect),WGL_PACK(&t_update_region));
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Get tick info FAIL. Tick=%d,File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
            return CLIR_CMD_EXEC_ERROR;
        }

        if(WGL_IS_FLAG_TURN_ON(t_tick_inf.ui2_begin_flag,WGL_ANIM_TYPE_MOVE))
        {
            ui2_move_idx = _wgl_anim_find_move_inf(ui4_tick);
            if(ui2_move_idx != 0xFFFF)
            {
                i4_ret = _wgl_anim_check_move(t_tick_inf,ui2_move_idx,ui4_tick);
                if(i4_ret != WGLR_OK)
                {
                 x_dbg_stmt("{ANIM.Cli} ERR: Move check Fail. Tick=%d,F=%s,L=%d\n\n",ui4_tick,__FILE__, __LINE__);
                    return CLIR_CMD_EXEC_ERROR;
                }
            }
        }

        if(WGL_IS_FLAG_TURN_ON(t_tick_inf.ui2_begin_flag,WGL_ANIM_TYPE_ALPHA))
        {
            ui2_alpha_idx = _wgl_anim_find_alpha_inf(ui4_tick);
            if(ui2_alpha_idx != 0xFFFF)
            {
                i4_ret = _wgl_anim_check_alpha(t_tick_inf,ui2_alpha_idx,ui4_tick);
                if(i4_ret != WGLR_OK)
                {
                 x_dbg_stmt("{ANIM.Cli} ERR: ALPHA check Fail.Tick=%d, F=%s,L=%d\n\n",ui4_tick,__FILE__, __LINE__);
                    return CLIR_CMD_EXEC_ERROR;
                }
            }
        }

        if(WGL_IS_FLAG_TURN_ON(t_tick_inf.ui2_begin_flag,WGL_ANIM_TYPE_SCALE))
        {
            ui2_scale_idx = _wgl_anim_find_scale_inf(ui4_tick);
            if(ui2_scale_idx != 0xFFFF)
            {
                i4_ret = _wgl_anim_check_scale(t_tick_inf,ui2_scale_idx,ui4_tick);
                if(i4_ret != WGLR_OK)
                {
                 x_dbg_stmt("{ANIM.Cli} ERR: SCALE Fail.Tick=%d, F=%s,L=%d\n\n",ui4_tick,__FILE__, __LINE__);
                    return CLIR_CMD_EXEC_ERROR;
                }
            }
        }   

        if(WGL_IS_FLAG_TURN_ON(t_tick_inf.ui2_begin_flag,WGL_ANIM_TYPE_IMG))
        {
            ui2_img_idx = _wgl_anim_find_img_inf(ui4_tick);
            if(ui2_img_idx != 0xFFFF)
            {
                i4_ret = _wgl_anim_check_img(t_tick_inf,ui2_img_idx,ui4_tick);
                if(i4_ret != WGLR_OK)
                {
                 x_dbg_stmt("{ANIM.Cli} ERR: IMG check Fail.Tick=%d, F=%s,L=%d\n\n",ui4_tick,__FILE__, __LINE__);
                    return CLIR_CMD_EXEC_ERROR;
                }
            }
        }         

        /*Check the update region information*/
        if((pt_anim->pt_rgn != NULL) && (pt_anim->ui2_rgn_num > 0))
        {
            i4_ret = _wgl_anim_check_region(t_update_region,pt_anim,ui4_tick);
            if(i4_ret != WGLR_OK)
            {
                x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Check upd region FAIL.Tick=%d, File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
                return CLIR_CMD_EXEC_ERROR;
            }
        }

        x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&t_cond), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Condition FAIL.Tick=%d, File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
        }

    }

    
    return CLIR_OK;    
}


static INT32 _wgl_anim_cli_move(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                           i4_ret = CLIR_OK;
    WGL_ANIMATION_DATA_T            t_begin;
    WGL_ANIM_TICK_DATA              t_tick_inf;
    UINT16                          ui2_frame;
    UINT16                          ui2_total;
    UINT32                          ui4_tick;
    UINT32                          ui4_total_ticks;
    WGL_ANIM_COND_INF_T             t_cond;
    GL_RECT_T                       t_rect;
    WGL_UPD_REGION_T                t_update_region;
    
    WGL_ANIMATION_DATA_T*           pt_anim_data;
    UINT32                          ui4_size;
    
    if ((i4_argc != 7) || (pps_argv == NULL))
    {
        x_dbg_stmt("[Anim] Anim.move [frame] [total steps] [Start.x] [Start.y] [End.x] [End.y]\n\r");
        return CLIR_OK;
    }   

    ui4_size = sizeof(WGL_ANIMATION_DATA_T);
    ui4_size = sizeof(&pt_anim_data);
    pt_anim_data = x_mem_alloc(sizeof(WGL_ANIMATION_DATA_T));
    pt_anim_data->pt_path_mode = x_mem_alloc((sizeof(WGL_ANIM_PATH_DATA_T) * 3));
    ui4_size = sizeof(&pt_anim_data);
    if(ui4_size == 0)
    {
        return CLIR_OK;
    }
        
    ui2_frame = (UINT16)x_strtoll(pps_argv[1],NULL,10);
    ui2_total = (UINT16)x_strtoll(pps_argv[2],NULL,10);
    
    x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
    x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
    x_memset(&t_begin,0,sizeof(WGL_ANIMATION_DATA_T));
    x_memset(&t_update_region,0,sizeof(WGL_UPD_REGION_T));
    SET_RECT(&t_rect,10,10,310,210);   
    t_begin.ui2_count = 1;
    t_begin.pt_anim_data = x_mem_alloc((sizeof(WGL_ANIM_DATA_INFO) * t_begin.ui2_count));
    t_begin.pt_path_mode = x_mem_alloc((sizeof(WGL_ANIM_PATH_DATA_T) * t_begin.ui2_count));
    if((t_begin.pt_anim_data == NULL) || (t_begin.pt_path_mode == NULL))
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Alloc memory fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }
    
    x_memset(t_begin.pt_anim_data,0,sizeof(WGL_ANIM_DATA_INFO));
    x_memset(t_begin.pt_path_mode,0,sizeof(WGL_ANIM_PATH_DATA_T));

    WGL_SET_FLAG(t_begin.pt_path_mode->ui2_anim_type,WGL_ANIM_TYPE_MOVE);
    WGL_SET_FLAG(t_begin.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF);
    t_begin.pt_path_mode->ui2_total_steps = ui2_total;
    t_begin.pt_path_mode->ui4_start_tick = 3;
    t_begin.pt_path_mode->ui2_frame_interval = ui2_frame;
    t_begin.pt_path_mode->pf_type_cb = NULL;
    
    WGL_SET_FLAG(t_begin.ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF);
    WGL_SET_FLAG(t_begin.pt_anim_data->ui2_anim_type,WGL_ANIM_TYPE_MOVE);
    t_begin.pt_anim_data->u_data.t_move.i2_start_x = (INT16)x_strtoll(pps_argv[3],NULL,10);
    t_begin.pt_anim_data->u_data.t_move.i2_start_y = (INT16)x_strtoll(pps_argv[4],NULL,10);
    t_begin.pt_anim_data->u_data.t_move.i2_end_x = (INT16)x_strtoll(pps_argv[5],NULL,10);
    t_begin.pt_anim_data->u_data.t_move.i2_end_y = (INT16)x_strtoll(pps_argv[6],NULL,10);

    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_BEGIN, WGL_PACK(&t_begin), NULL);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: BEGIN FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }

    ui4_total_ticks = (t_begin.pt_path_mode->ui2_frame_interval * ui2_total) / x_wgl_get_tick_interval();
    ui4_total_ticks = (t_begin.pt_path_mode->ui4_start_tick + ui4_total_ticks);
    for(ui4_tick = 0; ui4_tick <= ui4_total_ticks; ui4_tick ++)
    {
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_TICK,WGL_PACK(ui4_tick), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Tick FAIL. Tick=%d,File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
        }

        x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_tick_inf));
        if(i4_ret == WGLR_OK)
        {
            x_dbg_stmt("Animation Type:,Get tick:%d. INFO:\n",t_begin.pt_anim_data->ui2_anim_type,ui4_tick);
            x_dbg_stmt("Begin flag=%x,state=%d\n",t_tick_inf.ui2_begin_flag,t_tick_inf.ui2_state);
            x_dbg_stmt("New tick:rsn=%x,tick=%d,Pos[%d,%d]. \n",t_tick_inf.t_new_tick.ui2_inf_rsn,
                                                                t_tick_inf.t_new_tick.aui4_tick[WGL_ANIM_MOVE],
                                                                t_tick_inf.t_new_tick.t_tick_inf.t_point.i4_x,
                                                                t_tick_inf.t_new_tick.t_tick_inf.t_point.i4_y);
        }
        else
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Get tick info FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        }

        i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_UPD_RGN,WGL_PACK(&t_rect),WGL_PACK(&t_update_region));
        if(i4_ret == WGLR_OK)
        {
            UINT16          ui2_upd_num;
            
            SET_RECT_BY_SIZE(&t_rect,
                t_tick_inf.t_new_tick.t_tick_inf.t_point.i4_x,
                t_tick_inf.t_new_tick.t_tick_inf.t_point.i4_y,
                300,
                200);
            for(ui2_upd_num = 0;ui2_upd_num < t_update_region.ui4_update_rect_num;ui2_upd_num ++)
            {
                x_dbg_stmt("->Upd region%d:[%d,%d,%d,%d]",ui2_upd_num,
                                                      t_update_region.at_update_rect[ui2_upd_num].i4_left,
                                                      t_update_region.at_update_rect[ui2_upd_num].i4_top,
                                                      t_update_region.at_update_rect[ui2_upd_num].i4_right,
                                                      t_update_region.at_update_rect[ui2_upd_num].i4_bottom);
            }
        }
        else
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Get upd region FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        }
        
        x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&t_cond), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Condition[%d] FAIL. File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
        }

        if( WGL_IS_FLAG_TURN_ON(t_cond.ui2_end_type,WGL_ANIM_TYPE_MOVE) &&
            (t_cond.e_cond[WGL_ANIM_MOVE] == WGL_ANIM_TYPE_END))
        {
          if(ui4_tick == ui4_total_ticks) 
          {
            x_dbg_stmt("{WGL(ANIM.Cli)} SUCESS: SUCESS!\n");
          }
          else
          {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Condition ERROR. File=%s,Line=%d\n\n",__FILE__, __LINE__);
          }
        }

    }

    x_mem_free(t_begin.pt_anim_data);
    x_mem_free(t_begin.pt_path_mode);
    return CLIR_OK;    
}

static INT32 _wgl_anim_cli_alpha(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                           i4_ret = CLIR_OK;
    WGL_ANIMATION_DATA_T            t_begin;
    WGL_ANIM_TICK_DATA              t_tick_inf;
    UINT16                          ui2_frame;
    UINT16                          ui2_total;
    UINT32                          ui4_tick;
    UINT32                          ui4_total_ticks;
    WGL_ANIM_COND_INF_T             t_cond;
    GL_RECT_T                       t_rect;
    WGL_UPD_REGION_T                t_update_region;

    if ((i4_argc != 5) || (pps_argv == NULL))
    {
        x_dbg_stmt("[Anim] Anim.alpha [frame] [total steps] [Start al] [End al]\n\r");
        return CLIR_OK;
    }   

    ui2_frame = (UINT16)x_strtoll(pps_argv[1],NULL,10);
    ui2_total = (UINT16)x_strtoll(pps_argv[2],NULL,10);

    x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
    x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
    x_memset(&t_begin,0,sizeof(WGL_ANIMATION_DATA_T));
    SET_RECT(&t_rect,10,10,310,210);   
    
    t_begin.ui2_count = 1;
    t_begin.pt_anim_data = x_mem_alloc((sizeof(WGL_ANIM_DATA_INFO) * t_begin.ui2_count));
    t_begin.pt_path_mode = x_mem_alloc((sizeof(WGL_ANIM_PATH_DATA_T) * t_begin.ui2_count));
    if((t_begin.pt_anim_data == NULL) || (t_begin.pt_path_mode == NULL))
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Alloc memory fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }
    
    x_memset(t_begin.pt_anim_data,0,sizeof(WGL_ANIM_DATA_INFO));
    x_memset(t_begin.pt_path_mode,0,sizeof(WGL_ANIM_PATH_DATA_T));

    WGL_SET_FLAG(t_begin.pt_path_mode->ui2_anim_type,WGL_ANIM_TYPE_ALPHA);
    WGL_SET_FLAG(t_begin.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF);
    t_begin.pt_path_mode->ui2_total_steps = ui2_total;
    t_begin.pt_path_mode->ui4_start_tick = 3;
    t_begin.pt_path_mode->ui2_frame_interval = ui2_frame;
    t_begin.pt_path_mode->pf_type_cb = NULL;
    
    WGL_SET_FLAG(t_begin.ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF);
    WGL_SET_FLAG(t_begin.pt_anim_data->ui2_anim_type,WGL_ANIM_TYPE_ALPHA);
    t_begin.pt_anim_data->u_data.t_alpha.ui1_start_alpha = (INT16)x_strtoll(pps_argv[3],NULL,10);
    t_begin.pt_anim_data->u_data.t_alpha.ui1_end_alpha = (INT16)x_strtoll(pps_argv[4],NULL,10);

    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_BEGIN, WGL_PACK(&t_begin), NULL);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: BEGIN FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }

    ui4_total_ticks = (t_begin.pt_path_mode->ui2_frame_interval * ui2_total) / x_wgl_get_tick_interval();
    ui4_total_ticks = (t_begin.pt_path_mode->ui4_start_tick + ui4_total_ticks);
    for(ui4_tick = 0; ui4_tick <= ui4_total_ticks; ui4_tick ++)
    {
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_TICK,WGL_PACK(ui4_tick), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Tick FAIL. Tick=%d,File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
        }

        x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_tick_inf));
        if(i4_ret == WGLR_OK)
        {
            x_dbg_stmt("Animation Type:,Get tick:%d. INFO: ",t_begin.pt_anim_data->ui2_anim_type,ui4_tick);
            x_dbg_stmt("Begin flag=%x,state=%d;",t_tick_inf.ui2_begin_flag,t_tick_inf.ui2_state);
            x_dbg_stmt("New tick:rsn=%x,tick=%d,alpha=%d. \n",t_tick_inf.t_new_tick.ui2_inf_rsn,
                                                                t_tick_inf.t_new_tick.aui4_tick[WGL_ANIM_ALPHA],
                                                                t_tick_inf.t_new_tick.t_tick_inf.ui1_alpha);
        }
        else
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Get tick info FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        }
        
        i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_UPD_RGN,WGL_PACK(&t_rect),WGL_PACK(&t_update_region));
        if(i4_ret == WGLR_OK)
        {
            UINT16          ui2_upd_num;
            
            SET_RECT_BY_SIZE(&t_rect,
                t_rect.i4_left,
                t_rect.i4_top,
                300,
                200);
            
            for(ui2_upd_num = 0;ui2_upd_num < t_update_region.ui4_update_rect_num;ui2_upd_num ++)
            {
                x_dbg_stmt("->Upd region%d:[%d,%d,%d,%d]",ui2_upd_num,
                                                      t_update_region.at_update_rect[ui2_upd_num].i4_left,
                                                      t_update_region.at_update_rect[ui2_upd_num].i4_top,
                                                      t_update_region.at_update_rect[ui2_upd_num].i4_right,
                                                      t_update_region.at_update_rect[ui2_upd_num].i4_bottom);
            }
        }
        else
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Get upd region FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        }
        x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&t_cond), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Condition[%d] FAIL. File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
        }

        if( WGL_IS_FLAG_TURN_ON(t_cond.ui2_end_type,WGL_ANIM_TYPE_ALPHA) &&
            (t_cond.e_cond[WGL_ANIM_ALPHA] == WGL_ANIM_TYPE_END))
        {
          if(ui4_tick == ui4_total_ticks) 
          {
            x_dbg_stmt("{WGL(ANIM.Cli)} SUCESS: SUCESS!\n");
          }
          else
          {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Condition ERROR. File=%s,Line=%d\n\n",__FILE__, __LINE__);
          }
        }

    }

    x_mem_free(t_begin.pt_anim_data);
    x_mem_free(t_begin.pt_path_mode);    
    return CLIR_OK;    
}

static INT32 _wgl_anim_cli_scale(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                           i4_ret = CLIR_OK;
    WGL_ANIMATION_DATA_T            t_begin;
    WGL_ANIM_TICK_DATA              t_tick_inf;
    UINT16                          ui2_frame;
    UINT16                          ui2_total;
    UINT32                          ui4_tick;
    UINT32                          ui4_total_ticks;
    WGL_ANIM_COND_INF_T             t_cond;
    GL_RECT_T                       t_rect;
    WGL_UPD_REGION_T                t_update_region;

    if ((i4_argc != 7) || (pps_argv == NULL))
    {
        x_dbg_stmt("[Anim] Anim.scale [frame] [total steps] [Start.W] [Start.H] [End.W] [End.H]\n\r");
        return CLIR_OK;
    }   

    ui2_frame = (UINT16)x_strtoll(pps_argv[1],NULL,10);
    ui2_total = (UINT16)x_strtoll(pps_argv[2],NULL,10);
    
    x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
    x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
    x_memset(&t_begin,0,sizeof(WGL_ANIMATION_DATA_T));
    x_memset(&t_update_region,0,sizeof(WGL_UPD_REGION_T));
    SET_RECT(&t_rect,10,10,310,210);   

    t_begin.ui2_count = 1;
    t_begin.pt_anim_data = x_mem_alloc((sizeof(WGL_ANIM_DATA_INFO) * t_begin.ui2_count));
    t_begin.pt_path_mode = x_mem_alloc((sizeof(WGL_ANIM_PATH_DATA_T) * t_begin.ui2_count));
    if((t_begin.pt_anim_data == NULL) || (t_begin.pt_path_mode == NULL))
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Alloc memory fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }
    
    x_memset(t_begin.pt_anim_data,0,sizeof(WGL_ANIM_DATA_INFO));
    x_memset(t_begin.pt_path_mode,0,sizeof(WGL_ANIM_PATH_DATA_T));

    WGL_SET_FLAG(t_begin.pt_path_mode->ui2_anim_type,WGL_ANIM_TYPE_SCALE);
    WGL_SET_FLAG(t_begin.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF);
    t_begin.pt_path_mode->ui2_total_steps = ui2_total;
    t_begin.pt_path_mode->ui4_start_tick = 3;
    t_begin.pt_path_mode->ui2_frame_interval = ui2_frame;
    t_begin.pt_path_mode->pf_type_cb = NULL;
    
    WGL_SET_FLAG(t_begin.ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF);
    WGL_SET_FLAG(t_begin.pt_anim_data->ui2_anim_type,WGL_ANIM_TYPE_SCALE);
    t_begin.pt_anim_data->u_data.t_scale.ui8_start_w = (INT16)x_strtoll(pps_argv[3],NULL,10);
    t_begin.pt_anim_data->u_data.t_scale.ui8_start_h = (INT16)x_strtoll(pps_argv[4],NULL,10);
    t_begin.pt_anim_data->u_data.t_scale.ui8_end_w = (INT16)x_strtoll(pps_argv[5],NULL,10);
    t_begin.pt_anim_data->u_data.t_scale.ui8_end_h = (INT16)x_strtoll(pps_argv[6],NULL,10);

    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_BEGIN, WGL_PACK(&t_begin), NULL);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: BEGIN FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }

    ui4_total_ticks = (t_begin.pt_path_mode->ui2_frame_interval * ui2_total) / x_wgl_get_tick_interval();
    ui4_total_ticks = (t_begin.pt_path_mode->ui4_start_tick + ui4_total_ticks);
    for(ui4_tick = 0; ui4_tick <= ui4_total_ticks; ui4_tick ++)
    {
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_TICK,WGL_PACK(ui4_tick), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Tick FAIL. Tick=%d,File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
        }

        x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_tick_inf));
        if(i4_ret == WGLR_OK)
        {
            x_dbg_stmt("Animation Type:,Get tick:%d. INFO:\n",t_begin.pt_anim_data->ui2_anim_type,ui4_tick);
            x_dbg_stmt("Begin flag=%x,state=%d\n",t_tick_inf.ui2_begin_flag,t_tick_inf.ui2_state);
            x_dbg_stmt("New tick:rsn=%x,tick=%d,size[%d,%d]. \n",t_tick_inf.t_new_tick.ui2_inf_rsn,
                                                                t_tick_inf.t_new_tick.aui4_tick[WGL_ANIM_SIZE],
                                                                t_tick_inf.t_new_tick.t_tick_inf.t_scale.ui4_width,
                                                                t_tick_inf.t_new_tick.t_tick_inf.t_scale.ui4_height);
        }
        else
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Get tick info FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        }

        i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_UPD_RGN,WGL_PACK(&t_rect),WGL_PACK(&t_update_region));
        if(i4_ret == WGLR_OK)
        {
            UINT16          ui2_upd_num;
            
            SET_RECT_BY_SIZE(&t_rect,
                t_rect.i4_left,
                t_rect.i4_top,
                300,
                200);
            
            for(ui2_upd_num = 0;ui2_upd_num < t_update_region.ui4_update_rect_num;ui2_upd_num ++)
            {
                x_dbg_stmt("->Upd region%d:[%d,%d,%d,%d]",ui2_upd_num,
                                                      t_update_region.at_update_rect[ui2_upd_num].i4_left,
                                                      t_update_region.at_update_rect[ui2_upd_num].i4_top,
                                                      t_update_region.at_update_rect[ui2_upd_num].i4_right,
                                                      t_update_region.at_update_rect[ui2_upd_num].i4_bottom);
            }
        }
        else
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Get upd region FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        }

        x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&t_cond), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Condition[%d] FAIL. File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
        }


        if( WGL_IS_FLAG_TURN_ON(t_cond.ui2_end_type,WGL_ANIM_TYPE_SCALE) &&
            (t_cond.e_cond[WGL_ANIM_SCALE] == WGL_ANIM_TYPE_END))
        {
          if(ui4_tick == ui4_total_ticks) 
          {
            x_dbg_stmt("{WGL(ANIM.Cli)} SUCESS: SUCESS!\n");
          }
          else
          {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Condition ERROR. File=%s,Line=%d\n\n",__FILE__, __LINE__);
          }
        }

    }

    x_mem_free(t_begin.pt_anim_data);
    x_mem_free(t_begin.pt_path_mode);
    return CLIR_OK;    
}

static INT32 _wgl_anim_cli_size(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                           i4_ret = CLIR_OK;
    WGL_ANIMATION_DATA_T            t_begin;
    WGL_ANIM_TICK_DATA              t_tick_inf;
    UINT16                          ui2_frame;
    UINT16                          ui2_total;
    UINT32                          ui4_tick;
    UINT32                          ui4_total_ticks;
    WGL_ANIM_COND_INF_T             t_cond;

    if ((i4_argc != 7) || (pps_argv == NULL))
    {
        x_dbg_stmt("[Anim] Anim.move [frame] [total steps] [Start.x] [Start.y] [End.x] [End.y]\n\r");
        return CLIR_OK;
    }   

    ui2_frame = (UINT16)x_strtoll(pps_argv[1],NULL,10);
    ui2_total = (UINT16)x_strtoll(pps_argv[2],NULL,10);
    
    x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
    x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
    x_memset(&t_begin,0,sizeof(WGL_ANIMATION_DATA_T));

    t_begin.ui2_count = 1;
    t_begin.pt_anim_data = x_mem_alloc((sizeof(WGL_ANIM_DATA_INFO) * t_begin.ui2_count));
    t_begin.pt_path_mode = x_mem_alloc((sizeof(WGL_ANIM_PATH_DATA_T) * t_begin.ui2_count));
    if((t_begin.pt_anim_data == NULL) || (t_begin.pt_path_mode == NULL))
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Alloc memory fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }
    
    x_memset(t_begin.pt_anim_data,0,sizeof(WGL_ANIM_DATA_INFO));
    x_memset(t_begin.pt_path_mode,0,sizeof(WGL_ANIM_PATH_DATA_T));

    WGL_SET_FLAG(t_begin.pt_path_mode->ui2_anim_type,WGL_ANIM_TYPE_SIZE);
    WGL_SET_FLAG(t_begin.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF);
    t_begin.pt_path_mode->ui2_total_steps = ui2_total;
    t_begin.pt_path_mode->ui4_start_tick = 3;
    t_begin.pt_path_mode->ui2_frame_interval = ui2_frame;
    t_begin.pt_path_mode->pf_type_cb = NULL;
    
    WGL_SET_FLAG(t_begin.ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF);
    WGL_SET_FLAG(t_begin.pt_anim_data->ui2_anim_type,WGL_ANIM_TYPE_SIZE);
    t_begin.pt_anim_data->u_data.t_size.ui8_start_w = (INT16)x_strtoll(pps_argv[3],NULL,10);
    t_begin.pt_anim_data->u_data.t_size.ui8_start_h = (INT16)x_strtoll(pps_argv[4],NULL,10);
    t_begin.pt_anim_data->u_data.t_size.ui8_end_w = (INT16)x_strtoll(pps_argv[5],NULL,10);
    t_begin.pt_anim_data->u_data.t_size.ui8_end_h = (INT16)x_strtoll(pps_argv[6],NULL,10);

    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_BEGIN, WGL_PACK(&t_begin), NULL);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: BEGIN FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }

    ui4_total_ticks = (t_begin.pt_path_mode->ui2_frame_interval * ui2_total) / x_wgl_get_tick_interval();
    ui4_total_ticks = (t_begin.pt_path_mode->ui4_start_tick + ui4_total_ticks);
    for(ui4_tick = 0; ui4_tick <= ui4_total_ticks; ui4_tick ++)
    {
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_TICK,WGL_PACK(ui4_tick), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Tick FAIL. Tick=%d,File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
        }

        x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_tick_inf));
        if(i4_ret == WGLR_OK)
        {
            x_dbg_stmt("Animation Type:,Get tick:%d. INFO:\n",t_begin.pt_anim_data->ui2_anim_type,ui4_tick);
            x_dbg_stmt("Begin flag=%x,state=%d\n",t_tick_inf.ui2_begin_flag,t_tick_inf.ui2_state);
            x_dbg_stmt("New tick:rsn=%x,tick=%d,size[%d,%d]. \n",t_tick_inf.t_new_tick.ui2_inf_rsn,
                                                                t_tick_inf.t_new_tick.aui4_tick[WGL_ANIM_SIZE],
                                                                t_tick_inf.t_new_tick.t_tick_inf.t_size.ui4_width,
                                                                t_tick_inf.t_new_tick.t_tick_inf.t_size.ui4_height);
        }
        else
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Get tick info FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        }

        x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&t_cond), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Condition[%d] FAIL. File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
        }

        if( WGL_IS_FLAG_TURN_ON(t_cond.ui2_end_type,WGL_ANIM_TYPE_SIZE) &&
            (t_cond.e_cond[WGL_ANIM_SIZE] == WGL_ANIM_TYPE_END))
        {
          if(ui4_tick == ui4_total_ticks) 
          {
            x_dbg_stmt("{WGL(ANIM.Cli)} SUCESS: SUCESS!\n");
          }
          else
          {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Condition ERROR. File=%s,Line=%d\n\n",__FILE__, __LINE__);
          }
        }

    }

    x_mem_free(t_begin.pt_anim_data);
    x_mem_free(t_begin.pt_path_mode);
    return CLIR_OK;    
}


static INT32 _wgl_anim_cli_img(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                           i4_ret = CLIR_OK;
    WGL_ANIMATION_DATA_T            t_begin;
    WGL_ANIM_TICK_DATA              t_tick_inf;
    UINT16                          ui2_frame;
    UINT16                          ui2_total;
    UINT16                          ui2_play_mode;
    UINT32                          ui4_tick;
    UINT32                          ui4_total_ticks;
    UINT16                          ui2_i;
    WGL_ANIM_COND_INF_T             t_cond;
    WGL_ANIM_IMG_SEQ_T              t_img_seq;
    UINT16                          ui2_index;

    if (pps_argv == NULL)
    {
        x_dbg_stmt("[Anim] Anim.img No data!\n\r");
        return CLIR_OK;
    }  

    ui2_play_mode = (UINT16)x_strtoll(pps_argv[3],NULL,10);
    if(ui2_play_mode == 0)
    {
        if(i4_argc != 5)
        {
            x_dbg_stmt("[Anim] Anim.img [frame] [total steps] [mode:0] [seq id] \n\r");
            return CLIR_OK;
        }
    }
    else if(ui2_play_mode == 1)
    {
        if(i4_argc != 6)
        {
            x_dbg_stmt("[Anim] Anim.img [frame] [total steps] [mode:1] [seq id] [Loop count] \n\r");
            return CLIR_OK;
        }
    }
    else if(ui2_play_mode == 2)
    {
        if(i4_argc <= 4)
        {
            x_dbg_stmt("[Anim] Anim.img [frame] [total steps] [mode:2] [seq id] [length] [img id,interval] \n\r");
            return CLIR_OK;
        }
    }   
    

    ui2_frame = (UINT16)x_strtoll(pps_argv[1],NULL,10);
    ui2_total = (UINT16)x_strtoll(pps_argv[2],NULL,10);
    
    x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
    x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
    x_memset(&t_begin,0,sizeof(WGL_ANIMATION_DATA_T));

    t_begin.ui2_count = 1;
    t_begin.pt_anim_data = x_mem_alloc((sizeof(WGL_ANIM_DATA_INFO) * t_begin.ui2_count));
    t_begin.pt_path_mode = x_mem_alloc((sizeof(WGL_ANIM_PATH_DATA_T) * t_begin.ui2_count));
    if((t_begin.pt_anim_data == NULL) || (t_begin.pt_path_mode == NULL))
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Alloc memory fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }
    
    x_memset(t_begin.pt_anim_data,0,sizeof(WGL_ANIM_DATA_INFO));
    x_memset(t_begin.pt_path_mode,0,sizeof(WGL_ANIM_PATH_DATA_T));
    WGL_SET_FLAG(t_begin.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF);
    WGL_SET_FLAG(t_begin.ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF);

    WGL_SET_FLAG(t_begin.pt_path_mode->ui2_anim_type,WGL_ANIM_TYPE_IMG);
    t_begin.pt_path_mode->ui2_total_steps = ui2_total;
    t_begin.pt_path_mode->ui4_start_tick = 3;
    t_begin.pt_path_mode->ui2_frame_interval = ui2_frame;
    t_begin.pt_path_mode->pf_type_cb = NULL;
    if(ui2_play_mode == 0)
    {
        t_begin.pt_path_mode->e_play_mode = ANIM_PLAY_MODE_ONCE;
    }
    else if(ui2_play_mode == 1)
    {
        t_begin.pt_path_mode->e_play_mode = ANIM_PLAY_MODE_LOOP;
    }
    else if(ui2_play_mode == 2)
    {
        t_begin.pt_path_mode->e_play_mode = ANIM_PLAY_MODE_CUST;
    }
    
    WGL_SET_FLAG(t_begin.pt_anim_data->ui2_anim_type,WGL_ANIM_TYPE_IMG);
    t_begin.pt_anim_data->u_data.t_img.ui1_img_seq_id = (UINT8)x_strtoll(pps_argv[4],NULL,10);   

    if(ui2_play_mode == 1)
    {
        t_begin.pt_path_mode->ui2_loop_count = (UINT16)x_strtoll(pps_argv[5],NULL,10);
    }
    else if(ui2_play_mode == 2)
    {
        UINT16                  ui2_interval;
        UINT16                  ui2_img_id;
        
        t_begin.pt_anim_data->u_data.t_img.ui2_cust_len = (UINT16)x_strtoll(pps_argv[5],NULL,10);
        if(t_begin.pt_anim_data->u_data.t_img.ui2_cust_len >= CLI_IMG_MAX_TPL)
        {
            t_begin.pt_anim_data->u_data.t_img.ui2_cust_len = (UINT16)(CLI_IMG_MAX_TPL - 1);
        }

        ui2_index = 6;
        for(ui2_i = 0; ui2_i < t_begin.pt_anim_data->u_data.t_img.ui2_cust_len; ui2_i ++)
        {
            if(i4_argc < 7)
            {
                ui2_img_id = ui2_i;
                ui2_interval = 100 + (ui2_i+1)*50; 
            }
            else
            {
                ui2_img_id = (UINT16)x_strtoll(pps_argv[ui2_index ++],NULL,10);
                ui2_interval = (UINT16)x_strtoll(pps_argv[ui2_index ++],NULL,10);
            }
            wgl_anim_pack_img_cust_info(&(a_cli_img_cust[ui2_i]),FALSE,ui2_img_id,ui2_interval); 
        }
        t_begin.pt_anim_data->u_data.t_img.pt_cust_info = &(a_cli_img_cust[0]);
    }
    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_SET_MAX_IMG_SEQ, WGL_PACK(1), NULL);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Set img MAX SEQ FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }
    
    t_img_seq.ui2_len = CLI_IMG_MAX_TPL;
    t_img_seq.pt_img_tpl = &_t_cli_imgs[0];
    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_SET_IMG_SEQ, WGL_PACK(0), WGL_PACK(&t_img_seq));
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Set img seq FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }

    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_BEGIN, WGL_PACK(&t_begin), NULL);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: BEGIN FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }

    ui4_total_ticks = (t_begin.pt_path_mode->ui2_frame_interval * ui2_total) / x_wgl_get_tick_interval();
    if(ui2_play_mode == 1)
    {
        ui4_total_ticks = ui4_total_ticks * t_begin.pt_path_mode->ui2_loop_count;
    }
    else if(ui2_play_mode == 2)
    {
        UINT64                  ui8_time = 0;

        for(ui2_i = 0; ui2_i < t_begin.pt_anim_data->u_data.t_img.ui2_cust_len; ui2_i ++)
        {
            ui8_time += (UINT64)((UINT16)(a_cli_img_cust[ui2_i] & 0xFFFF));
        }
        ui4_total_ticks = ui8_time / x_wgl_get_tick_interval() + 1;
    }
    
    ui4_total_ticks = (t_begin.pt_path_mode->ui4_start_tick + ui4_total_ticks);
    for(ui4_tick = 0; ui4_tick <= ui4_total_ticks; ui4_tick ++)
    {
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_TICK,WGL_PACK(ui4_tick), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Tick FAIL. Tick=%d,File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
        }

        x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_tick_inf));
        if(i4_ret == WGLR_OK)
        {
            x_dbg_stmt("Animation Type:,Get tick:%d. INFO:\n",t_begin.pt_anim_data->ui2_anim_type,ui4_tick);
            x_dbg_stmt("Begin flag=%x,state=%d\n",t_tick_inf.ui2_begin_flag,t_tick_inf.ui2_state);
            x_dbg_stmt("New tick:rsn=%x,tick=%d,img id:%d,  ",t_tick_inf.t_new_tick.ui2_inf_rsn,
                                                                t_tick_inf.t_new_tick.aui4_tick[WGL_ANIM_IMG],
                                                                t_tick_inf.t_new_tick.t_tick_inf.t_img.ui2_img_id);
            if(t_tick_inf.t_new_tick.t_tick_inf.t_img.pt_img != NULL)
            {
                x_dbg_stmt("%x \n",*(t_tick_inf.t_new_tick.t_tick_inf.t_img.pt_img));
            }
            else
            {
                x_dbg_stmt("\n");
            }
        }
        else
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Get tick info FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        }

        x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&t_cond), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Condition[%d] FAIL. File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
        }

        if( WGL_IS_FLAG_TURN_ON(t_cond.ui2_end_type,WGL_ANIM_TYPE_IMG) &&
            (t_cond.e_cond[WGL_ANIM_IMG] == WGL_ANIM_TYPE_END))
        {
          if(ui4_tick == ui4_total_ticks) 
          {
            x_dbg_stmt("{WGL(ANIM.Cli)} SUCESS: SUCESS!\n");
          }
          else
          {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Condition ERROR. File=%s,Line=%d\n\n",__FILE__, __LINE__);
          }
        }

    }

    x_mem_free(t_begin.pt_anim_data);
    x_mem_free(t_begin.pt_path_mode);
    return CLIR_OK;    
}

static INT32 _wgl_anim_cli_bgn_alloc(
    WGL_ANIMATION_DATA_T*               pt_begin,
    UINT16                              ui2_count,
    UINT32                              ui4_flag)
{
    UINT32                              ui4_data_size;
    UINT32                              ui4_path_size;

    if(pt_begin == NULL)
    {
        return CLIR_INV_ARG;
    }
    
    x_memset(pt_begin,0,sizeof(WGL_ANIMATION_DATA_T));
    
    ui4_data_size = sizeof(WGL_ANIM_DATA_INFO) * ui2_count;
    ui4_path_size = sizeof(WGL_ANIM_PATH_DATA_T) * ui2_count;

    if(WGL_IS_FLAG_TURN_ON(ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF))
    {
        pt_begin->pt_path_mode = x_mem_alloc(ui4_path_size);
        if(pt_begin->pt_path_mode == NULL)
        {
            x_dbg_stmt("{ANIM.Cli} ERR: Alloc memory fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            return CLIR_OUT_OF_MEM;
        }
        x_memset(pt_begin->pt_path_mode,0,ui4_path_size);
        WGL_SET_FLAG(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF);
    }

    if(WGL_IS_FLAG_TURN_ON(ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF))
    {
        pt_begin->pt_anim_data = x_mem_alloc(ui4_data_size);
        if(pt_begin->pt_anim_data == NULL)
        {
            x_dbg_stmt("{ANIM.Cli} ERR: Alloc memory fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            return CLIR_OUT_OF_MEM;
        }
        
        x_memset(pt_begin->pt_anim_data,0,ui4_data_size);
    
        WGL_SET_FLAG(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF);
    }    

    if(WGL_IS_FLAG_TURN_ON(ui4_flag,WGL_ANIM_DATA_FLAG_PATH_ID_INF))
    {
        WGL_SET_FLAG(pt_begin->ui4_flag,WGL_ANIM_DATA_FLAG_PATH_ID_INF);
    } 

    pt_begin->ui2_count = ui2_count;
    
    return CLIR_OK;    
}

static INT32 _wgl_anim_cli_bgn_free(
    WGL_ANIMATION_DATA_T*               pt_begin)
{
    if(pt_begin == NULL)
    {
        return CLIR_INV_ARG;    
    }

    if(pt_begin->pt_anim_data != NULL)
    {
        x_mem_free(pt_begin->pt_anim_data);
        pt_begin->pt_anim_data = NULL;
    }

    if(pt_begin->pt_path_mode != NULL)
    {
        x_mem_free(pt_begin->pt_path_mode);
        pt_begin->pt_path_mode = NULL;
    }  

    x_memset(&pt_begin->t_path_id_inf,0,sizeof(WGL_ANIM_PATH_ID_INFO_T));
    return CLIR_OK;    
}

static INT32 _wgl_anim_cli_test_anim(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                                   i4_ret;
//    CLI_ANIM_MOVE_D_T*                      pt_move;
    WGL_ANIMATION_DATA_T                    t_begin;
    UINT32                                  ui4_test;
    UINT16                                  ui2_cnt;
    CLI_ANIM_D*                             pt_anim;
    WGL_ANIM_IMG_SEQ_T                      t_img_seq;
    WGL_ANIM_CONTROL_LOGIC_T*              pt_anim_context;

    pt_anim_context = (WGL_ANIM_CONTROL_LOGIC_T*)t_cli_anim;
    if(pt_anim_context == NULL)
    {
        wgl_anim_free(&t_cli_anim);
    }
    
    x_wgl_set_tick_interval(50);
    i4_ret = wgl_anim_create(&t_cli_anim);
    if(i4_ret == WGLR_OK)
    {
        x_dbg_stmt("[Anim.cli]Create animation OK! Anim=%0x\n\n",t_cli_anim);
    }
    else
    {
        x_dbg_stmt("[Anim.cli]Create animation Fail. ret=%d,Anim=%x!\n\n",i4_ret,t_cli_anim);
    }

    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_SET_MAX_IMG_SEQ, WGL_PACK(2), NULL);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Set img MAX SEQ FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }    

    t_img_seq.ui2_len = CLI_IMG_MAX_TPL;
    t_img_seq.pt_img_tpl = &_t_cli_imgs[0];
    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_SET_IMG_SEQ, WGL_PACK(0), WGL_PACK(&t_img_seq));
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Set img seq FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }

    t_img_seq.ui2_len = CLI_IMG_MAX_TPL;
    t_img_seq.pt_img_tpl = &_t_cli_imgs_2[0];
    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_SET_IMG_SEQ, WGL_PACK(1), WGL_PACK(&t_img_seq));
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Set img seq FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    } 
    
        /*|--move--|*/
    for(ui4_test = 0; ui4_test < CLI_ANIM_D_NUM; ui4_test++)
    {
        x_memset(&gt_move,0,sizeof(CLI_ANIM_MOVE_INF));
        x_memset(&gt_scale,0,sizeof(CLI_ANIM_SCALE_INF));
        x_memset(&gt_alpha,0,sizeof(CLI_ANIM_ALPHA_INF));
        x_memset(&gt_img,0,sizeof(CLI_ANIM_ALPHA_INF));
        
        pt_anim = &(_t_anim_data[ui4_test]);
        if(pt_anim  == NULL)
        {
            x_dbg_stmt("{ANIM.Cli} ERR: data fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            break;
        }
        x_dbg_stmt("{Anim.Cli} Test indxe=%d\n",ui4_test);  
        i4_ret = _wgl_anim_cli_bgn_alloc(&t_begin,pt_anim->ui2_count,(WGL_ANIM_DATA_FLAG_PATH_MODE_INF | WGL_ANIM_DATA_FLAG_DATA_INF));
        if(i4_ret != CLIR_OK)
        {
            x_dbg_stmt("{ANIM.Cli} ERR: Alloc begin data fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            break;
        }

        for(ui2_cnt = 0;ui2_cnt < pt_anim->ui2_count;ui2_cnt++)
        {
            if(WGL_IS_FLAG_TURN_ON(pt_anim->at_bgn[ui2_cnt].ui4_flag,WGL_ANIM_TYPE_MOVE))
            {
                i4_ret = _wgl_anim_set_move_data(&t_begin,
                                                 pt_anim->at_bgn[ui2_cnt].ui4_start_tick,
                                                 pt_anim->at_bgn[ui2_cnt].ui2_frame,
                                                 pt_anim->at_bgn[ui2_cnt].ui2_id,
                                                 ui2_cnt);
                if(i4_ret != CLIR_OK)
                {
                    x_dbg_stmt("{ANIM.Cli} ERR: set move fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
                    break;
                }

            }
            else if(WGL_IS_FLAG_TURN_ON(pt_anim->at_bgn[ui2_cnt].ui4_flag,WGL_ANIM_TYPE_ALPHA))
            {
                i4_ret = _wgl_anim_set_alpha_data(&t_begin,
                                                 pt_anim->at_bgn[ui2_cnt].ui4_start_tick,
                                                 pt_anim->at_bgn[ui2_cnt].ui2_frame,
                                                 pt_anim->at_bgn[ui2_cnt].ui2_id,
                                                 ui2_cnt);
                if(i4_ret != CLIR_OK)
                {
                    x_dbg_stmt("{ANIM.Cli} ERR: set move fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
                    break;
                }

            } 
            else if(WGL_IS_FLAG_TURN_ON(pt_anim->at_bgn[ui2_cnt].ui4_flag,WGL_ANIM_TYPE_SCALE))
            {
                i4_ret = _wgl_anim_set_scale_data(&t_begin,
                                                 pt_anim->at_bgn[ui2_cnt].ui4_start_tick,
                                                 pt_anim->at_bgn[ui2_cnt].ui2_frame,
                                                 pt_anim->at_bgn[ui2_cnt].ui2_id,
                                                 ui2_cnt);
                if(i4_ret != CLIR_OK)
                {
                    x_dbg_stmt("{ANIM.Cli} ERR: set move fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
                    break;
                }

            }                 
            else if(WGL_IS_FLAG_TURN_ON(pt_anim->at_bgn[ui2_cnt].ui4_flag,WGL_ANIM_TYPE_IMG))
            {
                i4_ret = _wgl_anim_set_img_data(&t_begin,
                                                 pt_anim->at_bgn[ui2_cnt].ui4_start_tick,
                                                 pt_anim->at_bgn[ui2_cnt].ui2_frame,
                                                 pt_anim->at_bgn[ui2_cnt].ui2_id,
                                                 ui2_cnt);
                if(i4_ret != CLIR_OK)
                {
                    x_dbg_stmt("{ANIM.Cli} ERR: set move fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
                    break;
                }

            }  
        }

        if(i4_ret != CLIR_OK)
        {
            break;
        }
        
        i4_ret = _wgl_anim_do_anim(&t_begin,pt_anim);
        if(i4_ret != CLIR_OK)
        {
            x_dbg_stmt("{ANIM.Cli} ERR: Do anim fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            break;
        }            

        _wgl_anim_cli_bgn_free(&t_begin);
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_END, NULL, NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{ANIM.Cli} ERR: BEGIN FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        }
        
    }


    if(i4_ret != CLIR_OK)
    {
        _wgl_anim_cli_bgn_free(&t_begin);
        x_dbg_stmt("{ANIM.Cli} Auto Fail!\n\n");
    }
    else
    {
        x_dbg_stmt("{ANIM.Cli} Auto Sucessful!\n\n");
    }
    
    return (CLIR_OK);
}

static INT32 _wgl_anim_cli_set_path_mode(
    WGL_ANIMATION_PATH_MODE_T*              pt_path_mode,
    UINT16                                  ui2_path_count,
    UINT8*                                  pui1_data)
{
    WGL_ANIMATION_PATH_MODE_T*              pt_path;
    UINT8                                   ui1_idx;
    UINT8                                   ui1_data_idx;
    UINT16                                  ui2_i;
    UINT16                                  ui2_j;
    WGL_ANIM_PATH_DATA_T*                   pt_data;

    if((pt_path_mode == NULL) || (ui2_path_count == 0) || (pui1_data == NULL))
    {
        x_dbg_stmt("{ANIM} ERR: Set path mode fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }

    pt_path = pt_path_mode;
    /*Add the path mode information.*/
    for(ui2_i = 0; ui2_i < ui2_path_count; ui2_i ++)
    {
        ui1_idx = *pui1_data;
        if(ui1_idx >= CLI_ANIM_PATH_MODE_MAX)
        {
            x_dbg_stmt("{ANIM} ERR: Set path mode fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            return CLIR_INV_ARG;
        }
        pt_path->ui4_path_id = _t_path_mode[ui1_idx].ui4_path_id;
        pt_path->ui2_count = _t_path_mode[ui1_idx].ui2_count;
        pt_data = (WGL_ANIM_PATH_DATA_T*)x_mem_alloc(sizeof(WGL_ANIM_PATH_DATA_T)*pt_path->ui2_count);
        if(pt_data == NULL)
        {
            x_dbg_stmt("{ANIM} ERR: Set path mode fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            return CLIR_INV_ARG;
        }
            
        pt_path->pt_path_inf = pt_data;

        /*Add the path data for the path mode .*/
        for(ui2_j = 0; ui2_j < pt_path->ui2_count; ui2_j ++)
        {
            ui1_data_idx = _t_path_mode[ui1_idx].aui1_path_indx[ui2_j];
            x_memcpy(pt_data,&_t_path_data[ui1_data_idx],sizeof(WGL_ANIM_PATH_DATA_T));
            pt_data ++;
        }
        
        pui1_data ++;
        pt_path ++;
    }
    
    return (CLIR_OK);
}

static INT32 _wgl_anim_cli_add_path(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT16                                  ui2_i;
    UINT16                                  ui2_count;
    UINT16                                  ui2_added_cnt = 0;
    UINT8*                                  pui1_idx;
    UINT8*                                  pui1_data;
    WGL_ANIMATION_PATH_MODE_T*              pt_path_mode;
    INT32                                   i4_ret;
    
    if ((i4_argc < 3) || (pps_argv == NULL))
    {
        x_dbg_stmt("[Anim] Add path: [count] [path indx array]\n\r");
        return CLIR_OK;
    }   
        
    ui2_count = (UINT16)x_strtoll(pps_argv[1],NULL,10);
    if(ui2_count == 0)
    {
        x_dbg_stmt("[Anim] Add path: count = 0\n\r");
        return CLIR_OK;
    }

    pui1_idx = (UINT8*)x_mem_alloc(sizeof(UINT8)*ui2_count);
    if(pui1_idx == NULL)
    {
        x_dbg_stmt("{ANIM} ERR: Add path memory alloc fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        return CLIR_OK;
    }    

    pt_path_mode = (WGL_ANIMATION_PATH_MODE_T*)x_mem_alloc(sizeof(WGL_ANIMATION_PATH_MODE_T)*ui2_count);
    if(pt_path_mode == NULL)
    {
        x_dbg_stmt("{ANIM} ERR: Add path memory alloc fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        return CLIR_OK;
    }

    x_memset(pui1_idx,0,(sizeof(UINT8)*ui2_count));
    x_memset(pt_path_mode,0,(sizeof(WGL_ANIMATION_PATH_MODE_T)*ui2_count));
    
    pui1_data = pui1_idx;
    for(ui2_i = 0; ui2_i < ui2_count; ui2_i ++)
    {
        *pui1_data = (UINT8)x_strtoll(pps_argv[(2+ui2_i)],NULL,10);
        pui1_data ++;
    }
    
    i4_ret = _wgl_anim_cli_set_path_mode(pt_path_mode,ui2_count,pui1_idx);
    if(i4_ret != CLIR_OK)
    {
        x_dbg_stmt("{ANIM} ERR: Add path memory alloc fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }
    
    x_wgl_anim_lib_add_path(pt_path_mode,ui2_count,&ui2_added_cnt);
    if(ui2_added_cnt != ui2_count)
    {
        x_dbg_stmt("{ANIM} ERR: Add path count = %d,added=%d.\n\n",ui2_count,ui2_added_cnt);
    }

    x_mem_free(pui1_idx);
    if(pt_path_mode->pt_path_inf != NULL)
    {
        x_mem_free(pt_path_mode->pt_path_inf);
    }
    x_mem_free(pt_path_mode);
    return (CLIR_OK);
}



static INT32 _wgl_anim_cli_remove_path(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT16                                  ui2_i;
    UINT16                                  ui2_count;
    UINT32*                                 pui4_idx;
    UINT32*                                 pui4_data;
    INT32                                   i4_ret;

    if ((i4_argc < 3) || (pps_argv == NULL))
    {
        x_dbg_stmt("[Anim] remove path: [count] [path indx array]\n\r");
        return CLIR_OK;
    }   
        
    ui2_count = (UINT16)x_strtoll(pps_argv[1],NULL,10);
    if(ui2_count == 0)
    {
        x_dbg_stmt("[Anim] remove path: count = 0\n\r");
        return CLIR_OK;
    }

    pui4_idx = (UINT32*)x_mem_alloc(sizeof(UINT32)*ui2_count);
    if(pui4_idx == NULL)
    {
        x_dbg_stmt("{ANIM} ERR: remove path memory alloc fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        return CLIR_OK;
    }    
    x_memset(pui4_idx,0,(sizeof(UINT32)*ui2_count));


    pui4_data = pui4_idx;
    for(ui2_i = 0; ui2_i < ui2_count; ui2_i ++)
    {
        *pui4_data = (UINT32)x_strtoll(pps_argv[(2+ui2_i)],NULL,10);
        pui4_data ++;
    }

    i4_ret = wgl_anim_lib_remove_path(pui4_idx,ui2_count);

    if(i4_ret != CLIR_OK)
    {
        x_dbg_stmt("{ANIM} ERR: reomve path fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }

    x_mem_free(pui4_idx);
    
    return (CLIR_OK);
}

static INT32 _wgl_anim_cli_print_path_mode(WGL_ANIMATION_PATH_MODE_T* pt_path)
{
    UINT16                                  ui2_i;
    WGL_ANIM_PATH_DATA_T*                   pt_path_data = NULL;
    
    if(pt_path == NULL)
    {
        x_dbg_stmt("{ANIM} ERR: path mode add is null. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        return (CLIR_OK);
    }

    x_dbg_stmt("PATH MODE: ui4_path_id=%d, ui2_count = %d \n",pt_path->ui4_path_id,pt_path->ui2_count);

    pt_path_data = pt_path->pt_path_inf;
    for(ui2_i = 0; ui2_i < pt_path->ui2_count; ui2_i ++)
    {
        if(pt_path_data == NULL)
        {
            x_dbg_stmt("{ANIM} ERR: PATH DATA[%d] is null. File=%s,Line=%d\n\n",ui2_i,__FILE__, __LINE__);
            return (CLIR_OK);
        }
        
        x_dbg_stmt("[%d]path data:{%x,%d,%d,%d,%x,%d,%x}",ui2_i,
            pt_path_data->ui2_anim_type,
            pt_path_data->ui2_frame_interval,
            pt_path_data->ui2_total_steps,
            pt_path_data->ui4_start_tick,
            pt_path_data->e_play_mode,
            pt_path_data->ui2_loop_count,
            pt_path_data->pf_type_cb);
    }
    
    return (CLIR_OK);
}

static INT32 _wgl_anim_cli_get_path(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT16                      ui2_path_id;
    WGL_ANIMATION_PATH_MODE_T   t_path_mode;
    BOOL                        b_flag;
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("[Anim] get path: [path id] \n\r");
        return CLIR_OK;
    }   
        
    ui2_path_id = (UINT16)x_strtoll(pps_argv[1],NULL,10);

    b_flag = wgl_anim_lib_get_path(ui2_path_id,&t_path_mode);
    if(b_flag != TRUE)
    {
        x_dbg_stmt("{ANIM} ERR: get path fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }

    _wgl_anim_cli_print_path_mode(&t_path_mode);
    
    return (CLIR_OK);
}

static INT32 _wgl_anim_cli_get_path_cnt(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT32    ui4_cnt;
    
    wgl_anim_lib_get_path_cnt(&ui4_cnt);
    x_dbg_stmt("path count =%d\n",ui4_cnt);
    return (CLIR_OK);
}
#define CLI_TST_PATH    8
static INT32 _wgl_anim_cli_test_path(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                       i4_ret;
    WGL_ANIMATION_PATH_MODE_T   at_path_mode[CLI_TST_PATH];
    UINT16                      ui2_added_cnt;
    WGL_ANIMATION_DATA_T        t_begin;
    WGL_ANIM_IMG_SEQ_T          t_img_seq;
    UINT8                       aui1_arry[CLI_TST_PATH] = {0,1,2,3,4,7,8,9};

    x_memset(at_path_mode,0,sizeof(WGL_ANIMATION_PATH_MODE_T)*CLI_TST_PATH);
    
    i4_ret = _wgl_anim_cli_set_path_mode(at_path_mode,CLI_TST_PATH,aui1_arry);
    if(i4_ret != CLIR_OK)
    {
        x_dbg_stmt("{ANIM} ERR: Add path memory alloc fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }
    
    wgl_anim_lib_add_path(at_path_mode,CLI_TST_PATH,&ui2_added_cnt);
    if(ui2_added_cnt != CLI_TST_PATH)
    {
        x_dbg_stmt("{ANIM} ERR: Add path count = %d,added=%d.\n\n",CLI_TST_PATH,ui2_added_cnt);
    }    


    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_SET_MAX_IMG_SEQ, WGL_PACK(2), NULL);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Set img MAX SEQ FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }    

    t_img_seq.ui2_len = CLI_IMG_MAX_TPL;
    t_img_seq.pt_img_tpl = &_t_cli_imgs[0];
    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_SET_IMG_SEQ, WGL_PACK(0), WGL_PACK(&t_img_seq));
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Set img seq FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }

    t_img_seq.ui2_len = CLI_IMG_MAX_TPL;
    t_img_seq.pt_img_tpl = &_t_cli_imgs_2[0];
    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_SET_IMG_SEQ, WGL_PACK(1), WGL_PACK(&t_img_seq));
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Set img seq FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    } 

    x_wgl_set_tick_interval(50);
    

/*********************************************************************
********************    Test one animation type by path mode         ***********
**********************************************************************/
    /*Test move animation*/
    i4_ret = wgl_anim_create(&t_cli_anim);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{ANIM} ERR: Add path memory alloc fail.  ret=%d,File=%s,Line=%d\n\n",i4_ret,__FILE__, __LINE__);
    }
    
    i4_ret = _wgl_anim_cli_bgn_alloc(&t_begin,1,(WGL_ANIM_DATA_FLAG_PATH_ID_INF | WGL_ANIM_DATA_FLAG_DATA_INF));
    if(i4_ret != CLIR_OK)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: Alloc begin data fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }
    
    t_begin.pt_anim_data[0].ui2_anim_type = WGL_ANIM_TYPE_MOVE;
    t_begin.pt_anim_data[0].u_data.t_move.i2_start_x = 100;
    t_begin.pt_anim_data[0].u_data.t_move.i2_start_y = 50;
    t_begin.pt_anim_data[0].u_data.t_move.i2_end_x = 300;
    t_begin.pt_anim_data[0].u_data.t_move.i2_end_y = 150;
    
    t_begin.t_path_id_inf.ui2_path_cnt = 1;
    t_begin.t_path_id_inf.aui4_path_id[0] = CLI_PATH_MODE_MOVE;
    t_begin.t_path_id_inf.aui4_srt_tick[0] = 0;
    t_begin.t_path_id_inf.aui2_start_percent[0] = 0;
    t_begin.t_path_id_inf.aui2_end_percent[0] = 100;

    _wgl_anim_do_anim_no_check(&t_begin,5);

    _wgl_anim_cli_bgn_free(&t_begin);
    wgl_anim_free(&t_cli_anim);

    /*Test move+scale animation,the start time is same,using two path count*/
    i4_ret = wgl_anim_create(&t_cli_anim);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{ANIM} ERR: Add path memory alloc fail.  ret=%d,File=%s,Line=%d\n\n",i4_ret,__FILE__, __LINE__);
    }
    
    i4_ret = _wgl_anim_cli_bgn_alloc(&t_begin,2,
              (WGL_ANIM_DATA_FLAG_PATH_ID_INF | WGL_ANIM_DATA_FLAG_DATA_INF));
    if(i4_ret != CLIR_OK)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: Alloc begin data fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }
    
    t_begin.pt_anim_data[0].ui2_anim_type = WGL_ANIM_TYPE_MOVE;
    t_begin.pt_anim_data[0].u_data.t_move.i2_start_x = 100;
    t_begin.pt_anim_data[0].u_data.t_move.i2_start_y = 50;
    t_begin.pt_anim_data[0].u_data.t_move.i2_end_x = 300;
    t_begin.pt_anim_data[0].u_data.t_move.i2_end_y = 150;
    
    t_begin.pt_anim_data[1].ui2_anim_type = WGL_ANIM_TYPE_SCALE;
    t_begin.pt_anim_data[1].u_data.t_scale.ui8_start_w = 100;
    t_begin.pt_anim_data[1].u_data.t_scale.ui8_end_w = 50;
    t_begin.pt_anim_data[1].u_data.t_scale.ui8_start_h = 300;
    t_begin.pt_anim_data[1].u_data.t_scale.ui8_end_h = 150;
    
    t_begin.t_path_id_inf.ui2_path_cnt = 2;
    t_begin.t_path_id_inf.aui4_path_id[0] = CLI_PATH_MODE_MOVE;
    t_begin.t_path_id_inf.aui4_srt_tick[0] = 0;
    t_begin.t_path_id_inf.aui2_start_percent[0] = 0;
    t_begin.t_path_id_inf.aui2_end_percent[0] = 100;

    t_begin.t_path_id_inf.aui4_path_id[1] = CLI_PATH_MODE_SCALE;
    t_begin.t_path_id_inf.aui4_srt_tick[1] = 0;
    t_begin.t_path_id_inf.aui2_start_percent[1] = 0;
    t_begin.t_path_id_inf.aui2_end_percent[1] = 100;    

    _wgl_anim_do_anim_no_check(&t_begin,5);

    _wgl_anim_cli_bgn_free(&t_begin);
    wgl_anim_free(&t_cli_anim);
    

    /*Test move+scale animation,the start time is same,using one path*/
    i4_ret = wgl_anim_create(&t_cli_anim);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{ANIM} ERR: Add path memory alloc fail.  ret=%d,File=%s,Line=%d\n\n",i4_ret,__FILE__, __LINE__);
    }
    
    i4_ret = _wgl_anim_cli_bgn_alloc(&t_begin,2,
              (WGL_ANIM_DATA_FLAG_PATH_ID_INF | WGL_ANIM_DATA_FLAG_DATA_INF));
    if(i4_ret != CLIR_OK)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: Alloc begin data fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }
    
    t_begin.pt_anim_data[0].ui2_anim_type = WGL_ANIM_TYPE_MOVE;
    t_begin.pt_anim_data[0].u_data.t_move.i2_start_x = 100;
    t_begin.pt_anim_data[0].u_data.t_move.i2_start_y = 50;
    t_begin.pt_anim_data[0].u_data.t_move.i2_end_x = 300;
    t_begin.pt_anim_data[0].u_data.t_move.i2_end_y = 150;
    
    t_begin.pt_anim_data[1].ui2_anim_type = WGL_ANIM_TYPE_SCALE;
    t_begin.pt_anim_data[1].u_data.t_scale.ui8_start_w = 100;
    t_begin.pt_anim_data[1].u_data.t_scale.ui8_end_w = 50;
    t_begin.pt_anim_data[1].u_data.t_scale.ui8_start_h = 300;
    t_begin.pt_anim_data[1].u_data.t_scale.ui8_end_h = 150;
    
    t_begin.t_path_id_inf.ui2_path_cnt = 1;
    t_begin.t_path_id_inf.aui4_path_id[0] = CLI_PATH_MODE_11;
    t_begin.t_path_id_inf.aui4_srt_tick[0] = 0;
    t_begin.t_path_id_inf.aui2_start_percent[0] = 0;
    t_begin.t_path_id_inf.aui2_end_percent[0] = 100;

    _wgl_anim_do_anim_no_check(&t_begin,10);

    _wgl_anim_cli_bgn_free(&t_begin);
    wgl_anim_free(&t_cli_anim);

    /*Test move+scale animation,the start time is not same,using one path*/
    i4_ret = wgl_anim_create(&t_cli_anim);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{ANIM} ERR: Add path memory alloc fail.  ret=%d,File=%s,Line=%d\n\n",i4_ret,__FILE__, __LINE__);
    }
    
    i4_ret = _wgl_anim_cli_bgn_alloc(&t_begin,2,
              (WGL_ANIM_DATA_FLAG_PATH_ID_INF | WGL_ANIM_DATA_FLAG_DATA_INF));
    if(i4_ret != CLIR_OK)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: Alloc begin data fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }
    
    t_begin.pt_anim_data[0].ui2_anim_type = WGL_ANIM_TYPE_MOVE;
    t_begin.pt_anim_data[0].u_data.t_move.i2_start_x = 100;
    t_begin.pt_anim_data[0].u_data.t_move.i2_start_y = 50;
    t_begin.pt_anim_data[0].u_data.t_move.i2_end_x = 300;
    t_begin.pt_anim_data[0].u_data.t_move.i2_end_y = 150;
    
    t_begin.pt_anim_data[1].ui2_anim_type = WGL_ANIM_TYPE_SCALE;
    t_begin.pt_anim_data[1].u_data.t_scale.ui8_start_w = 100;
    t_begin.pt_anim_data[1].u_data.t_scale.ui8_end_w = 50;
    t_begin.pt_anim_data[1].u_data.t_scale.ui8_start_h = 300;
    t_begin.pt_anim_data[1].u_data.t_scale.ui8_end_h = 150;
    
    t_begin.t_path_id_inf.ui2_path_cnt = 1;
    t_begin.t_path_id_inf.aui4_path_id[0] = CLI_PATH_MODE_10;
    t_begin.t_path_id_inf.aui4_srt_tick[0] = 0;
    t_begin.t_path_id_inf.aui2_start_percent[0] = 0;
    t_begin.t_path_id_inf.aui2_end_percent[0] = 100;

    _wgl_anim_do_anim_no_check(&t_begin,13);

    _wgl_anim_cli_bgn_free(&t_begin);
    wgl_anim_free(&t_cli_anim);


    return (CLIR_OK);
}

static INT32 _wgl_anim_do_anim_no_check(
    WGL_ANIMATION_DATA_T*               pt_begin,
    UINT32                              ui4_total_ticks)
{
    INT32                           i4_ret = CLIR_OK;
    UINT16                          ui2_i;
    UINT32                          ui4_tick;
    WGL_ANIM_TICK_DATA              t_tick_inf;
    WGL_ANIM_COND_INF_T             t_cond;
    GL_RECT_T                       t_rect;
    WGL_UPD_REGION_T                t_update_region;
    WGL_ANIMATION_PATH_MODE_T       t_path_mode;
    UINT16                          ui2_total_cnt = 0;  
    BOOL                            b_flag;
    

    if(pt_begin == NULL)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: Do animation inv. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }



    for(ui2_i =0; ui2_i < pt_begin->t_path_id_inf.ui2_path_cnt; ui2_i ++)
    {

        b_flag = wgl_anim_lib_get_path(pt_begin->t_path_id_inf.aui4_path_id[ui2_i],&t_path_mode);
        if(b_flag != TRUE)
        {
            x_dbg_stmt("{ANIM} ERR: get path fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
            return CLIR_INV_ARG;
        }

        ui2_total_cnt +=   t_path_mode.ui2_count;      

    }

    if(ui2_total_cnt != pt_begin->ui2_count)
    {
        x_dbg_stmt("{ANIM} ERR: get path fail. File=%s,Line=%d\n\n",__FILE__, __LINE__);
        return CLIR_INV_ARG;
    }
    
    i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_BEGIN, WGL_PACK(pt_begin), NULL);
    if(i4_ret != WGLR_OK)
    {
        x_dbg_stmt("{ANIM.Cli} ERR: BEGIN FAIL. File=%s,Line=%d\n\n",__FILE__, __LINE__);
    }



    for(ui4_tick =0; ui4_tick <= ui4_total_ticks; ui4_tick ++)
    {
        x_memset(&t_rect,0,sizeof(GL_RECT_T));

        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_TICK,WGL_PACK(ui4_tick), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Tick FAIL. Tick=%d,File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
            return CLIR_CMD_EXEC_ERROR;
        }

        x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_tick_inf));
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Get tick info FAIL. Tick=%d,File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
            return CLIR_CMD_EXEC_ERROR;
        }

        SET_RECT(&t_rect,0,0,300,200); 
        
        i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_UPD_RGN,WGL_PACK(&t_rect),WGL_PACK(&t_update_region));
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Get tick info FAIL. Tick=%d,File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
            return CLIR_CMD_EXEC_ERROR;
        }

        x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&t_cond), NULL);
        if(i4_ret != WGLR_OK)
        {
            x_dbg_stmt("{WGL(ANIM.Cli)} ERR: Condition FAIL.Tick=%d, File=%s,Line=%d\n\n",ui4_tick,__FILE__, __LINE__);
        }

    }

    
    return CLIR_OK;    
}

static INT32 _wgl_anim_cli_stress_test(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT32                          ui4_test;
    UINT32                          ui4_data;
    UINT32                          ui4_data_1;
    WGL_ANIMATION_DATA_T            t_begin;
    INT32                           i4_ret;
    UINT16                          ui2_i;
    WGL_ANIM_DATA_INFO*             pt_data;
    WGL_ANIM_PATH_DATA_T*           pt_path;
    UINT16                          ui2_anim_type;
    BOOL                            b_do_not_care;
    WGL_ANIM_IMG_SEQ_T              t_img_seq;
    //UINT8                           ui1_creat_num;
    UINT8                           ui1_img_mode;
    UINT32                          ui4_index;
    UINT32                          ui4_total_tick;
    UINT32                          ui4_tick;
    UINT16                          ui2_bgn_cnt;
    WGL_ANIM_COND_INF_T             t_cond;
    WGL_ANIM_TICK_DATA              t_tick_inf;
    WGL_ANIM_CONTROL_LOGIC_T*       pt_anim;
    BOOL                            b_not_valid;
    
    ui4_test = 0;
    do
    {
        ui4_data = x_os_get_sys_tick();
        //ui1_creat_num = (UINT8)(ui4_data % 7);

        /*Creat new aniamtion handle*/
       // if(ui1_creat_num == 6 || h_cli_anim == NULL_HANDLE)
        {
            pt_anim = (WGL_ANIM_CONTROL_LOGIC_T*)t_cli_anim;
            if(pt_anim != NULL)
            {
                wgl_anim_free(&t_cli_anim);
            }
            
            x_wgl_set_tick_interval(50);
            i4_ret = wgl_anim_create(&t_cli_anim);
            if(i4_ret != WGLR_OK)
            {
               x_dbg_stmt("{ANIM} ERR: Create anim handle Fail. %d,File=%s,Line=%d\n\n",ui4_test,__FILE__, __LINE__);
              // DBG_ASSERT( 0, DBG_MOD_WGL );
            }
            
        }

        /*Set animation image sequence information*/
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_SET_MAX_IMG_SEQ, WGL_PACK(1), NULL);
        if(i4_ret != WGLR_OK)
        {
               x_dbg_stmt("{ANIM} ERR: Set max img Fail. %d,File=%s,Line=%d\n\n",ui4_test,__FILE__, __LINE__);
      //     DBG_ASSERT( 0, DBG_MOD_WGL );
        }
        
        t_img_seq.ui2_len = CLI_IMG_MAX_TPL;
        t_img_seq.pt_img_tpl = &_t_cli_imgs[0];
        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_SET_IMG_SEQ, WGL_PACK(0), WGL_PACK(&t_img_seq));
        if(i4_ret != WGLR_OK)
        {
           x_dbg_stmt("{ANIM} ERR: Set max img seq Fail. %d,File=%s,Line=%d\n\n",ui4_test,__FILE__, __LINE__);
//           DBG_ASSERT( 0, DBG_MOD_WGL );
        }

        b_do_not_care = FALSE;
        b_not_valid = FALSE;

        ui2_bgn_cnt = ui4_data % 6;
        if(ui2_bgn_cnt == 0 || ui2_bgn_cnt >= 6)
        {
            ui2_bgn_cnt = 1;
        }

        /*Set the begin data*/
        t_begin.pt_anim_data = (WGL_ANIM_DATA_INFO*)x_mem_alloc(sizeof(WGL_ANIM_DATA_INFO)*ui2_bgn_cnt);
        if(t_begin.pt_anim_data == NULL)
        {
           x_dbg_stmt("{ANIM} ERR: Mem alloc Fail. %d,File=%s,Line=%d\n\n",ui4_test,__FILE__, __LINE__);
          // DBG_ASSERT( 0, DBG_MOD_WGL );
        }

        t_begin.pt_path_mode = (WGL_ANIM_PATH_DATA_T*)x_mem_alloc(sizeof(WGL_ANIM_PATH_DATA_T)*ui2_bgn_cnt);
        if(t_begin.pt_path_mode == NULL)
        {
           x_dbg_stmt("{ANIM} ERR: Mem alloc Fail. %d,File=%s,Line=%d\n\n",ui4_test,__FILE__, __LINE__);
         //  DBG_ASSERT( 0, DBG_MOD_WGL );
        }

        pt_data = t_begin.pt_anim_data;
        pt_path = t_begin.pt_path_mode;
        x_memset(pt_data,0,sizeof(WGL_ANIM_DATA_INFO)*ui2_bgn_cnt);
        x_memset(pt_path,0,sizeof(WGL_ANIM_PATH_DATA_T)*ui2_bgn_cnt);
        t_begin.ui2_count = ui2_bgn_cnt;
        WGL_SET_FLAG(t_begin.ui4_flag,WGL_ANIM_DATA_FLAG_DATA_INF);
        WGL_SET_FLAG(t_begin.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_MODE_INF);
        for(ui2_i = 0; ui2_i < ui2_bgn_cnt; ui2_i ++)
        {
            ui4_data_1 = ui4_data % (23 + ui2_i * 11);
            ui2_anim_type = ui4_data_1 % 4;
            
            if(ui2_anim_type == 0)
            {
                pt_data->ui2_anim_type = WGL_ANIM_TYPE_MOVE;
                pt_data->u_data.t_move.i2_start_x = 300;
                pt_data->u_data.t_move.i2_start_y = 200;
                pt_data->u_data.t_move.i2_end_x = 500;
                pt_data->u_data.t_move.i2_end_y = 400;
                pt_data->pf_type_cb = NULL;
                
                pt_path->ui2_anim_type = WGL_ANIM_TYPE_MOVE;
                pt_path->ui2_frame_interval = 100;
                pt_path->ui2_total_steps = 5;
                pt_path->ui4_start_tick = 11 * ui2_i;
                pt_path->pf_type_cb = NULL;
            }
            else if(ui2_anim_type == 1)
            {
                pt_data->ui2_anim_type = WGL_ANIM_TYPE_ALPHA;
                pt_data->u_data.t_alpha.ui1_start_alpha = 10;
                pt_data->u_data.t_alpha.ui1_end_alpha = 10;
                pt_data->pf_type_cb = NULL;
                
                pt_path->ui2_anim_type = WGL_ANIM_TYPE_ALPHA;
                pt_path->ui2_frame_interval = 100;
                pt_path->ui2_total_steps = 5;
                pt_path->ui4_start_tick = 11 * ui2_i;
                pt_path->pf_type_cb = NULL;
            }
            else if(ui2_anim_type == 2)
            {
                ui1_img_mode = ui4_data_1 % 3;
                if(ui1_img_mode == 0 || ((ui2_i + 4) >= ui2_bgn_cnt))
                {/*ANIM_PLAY_MODE_ONCE*/
                    pt_path->ui2_anim_type = WGL_ANIM_TYPE_IMG;
                    pt_path->ui2_frame_interval = 100;
                    pt_path->ui2_total_steps = 5;
                    pt_path->ui4_start_tick = 11 * ui2_i;
                    pt_path->pf_type_cb = NULL;
                    pt_path->e_play_mode = ANIM_PLAY_MODE_ONCE;
                    pt_path->ui2_loop_count = 3;
                    
                    pt_data->ui2_anim_type = WGL_ANIM_TYPE_IMG;
                    pt_data->u_data.t_img.ui1_img_seq_id = 0;
                }
                else if(ui1_img_mode == 1)
                {/*ANIM_PLAY_MODE_LOOP*/
                    pt_path->ui2_anim_type = WGL_ANIM_TYPE_IMG;
                    pt_path->ui2_frame_interval = 100;
                    pt_path->ui2_total_steps = 5;
                    pt_path->ui4_start_tick = 11 * ui2_i;
                    pt_path->pf_type_cb = NULL;
                    pt_path->e_play_mode = ANIM_PLAY_MODE_LOOP;
                    pt_path->ui2_loop_count = 3;
                    
                    pt_data->ui2_anim_type = WGL_ANIM_TYPE_IMG;
                    pt_data->u_data.t_img.ui1_img_seq_id = 0;
                }  
                else if(ui1_img_mode == 2)
                {/*ANIM_PLAY_MODE_LOOP*/
                    pt_path->ui2_anim_type = WGL_ANIM_TYPE_IMG;
                    pt_path->ui2_frame_interval = 100;
                    pt_path->ui2_total_steps = 5;
                    pt_path->ui4_start_tick = 11 * ui2_i;
                    pt_path->pf_type_cb = NULL;
                    pt_path->e_play_mode = ANIM_PLAY_MODE_CUST;
                    pt_path->ui2_loop_count = 3;
                    
                    pt_data->ui2_anim_type = WGL_ANIM_TYPE_IMG;
                    pt_data->u_data.t_img.ui1_img_seq_id = 0;
                    for (ui4_index = 0; ui4_index < 5; ui4_index++)
                    {
                         wgl_anim_pack_img_cust_info(
                            &(a_cli_img_cust[ui4_index]),
                            FALSE,
                            0,
                            100);                     
                    }
                    pt_data->u_data.t_img.ui2_cust_len = 5;
                    pt_data->u_data.t_img.pt_cust_info = &(a_cli_img_cust[0]);
                }                  
                pt_data->pf_type_cb = NULL;

            } 
            else if(ui2_anim_type == 3)
            {
                pt_data->ui2_anim_type = WGL_ANIM_TYPE_SCALE;
                pt_data->u_data.t_scale.ui8_start_w = 300;
                pt_data->u_data.t_scale.ui8_start_h = 200;
                pt_data->u_data.t_scale.ui8_end_w = 500;
                pt_data->u_data.t_scale.ui8_end_h = 450;
                pt_data->pf_type_cb = NULL;
                
                pt_path->ui2_anim_type = WGL_ANIM_TYPE_SCALE;
                pt_path->ui2_frame_interval = 100;
                pt_path->ui2_total_steps = 5;
                pt_path->ui4_start_tick = 11 * ui2_i;
                pt_path->pf_type_cb = NULL;
            } 
#if 0            
            else if(ui2_anim_type == 4)
            {
                pt_data->ui2_anim_type = WGL_ANIM_TYPE_SIZE;
                pt_data->u_data.t_size.ui8_start_w = 300;
                pt_data->u_data.t_size.ui8_start_h = 200;
                pt_data->u_data.t_size.ui8_end_w = 500;
                pt_data->u_data.t_size.ui8_end_h = 450;
                pt_data->pf_type_cb = NULL;
                
                pt_path->ui2_anim_type = WGL_ANIM_TYPE_SIZE;
                pt_path->ui2_frame_interval = 100;
                pt_path->ui2_total_steps = 5;
                pt_path->ui4_start_tick = 11 * ui2_i;
                pt_path->pf_type_cb = NULL;
            }
#endif               
            else if(t_begin.ui2_count > 0) 
            {
                t_begin.ui2_count --;
            }
            else 
            {
                b_do_not_care = TRUE;
                break;
            }
            
            pt_data ++;
            pt_path ++;
        }

        if(t_begin.ui2_count == 0 )
        {
            ui4_test ++;
            x_thread_delay(150);
            continue;
        }
        
        pt_data = t_begin.pt_anim_data;
        pt_path = t_begin.pt_path_mode;
        for(ui2_i = 0; ui2_i < t_begin.ui2_count; ui2_i ++)
        {
            if(pt_data->ui2_anim_type != pt_path->ui2_anim_type)
            {
                b_not_valid = TRUE;
                break;
            }
            
            if(pt_data->ui2_anim_type == 0 || pt_data->ui2_anim_type > 0x3F)
            {
                b_not_valid = TRUE;
                break;
            }
            pt_data ++;
            pt_path ++;
        }

        if(b_not_valid == TRUE)
        {
            ui4_test ++;
            x_thread_delay(150);
            continue;
        }

        pt_path = &(t_begin.pt_path_mode[t_begin.ui2_count - 1]);
        ui4_total_tick = pt_path->ui4_start_tick + 10;
        
        if(b_do_not_care == TRUE)
        {
            ui4_test ++;
            x_thread_delay(150);
            continue;
        }

        i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_BEGIN, WGL_PACK(&t_begin), NULL);
        if(i4_ret != WGLR_OK)
        {
           x_dbg_stmt("{ANIM} ERR: Begin Fail. %d,File=%s,Line=%d\n\n",ui4_test,__FILE__, __LINE__);
//            DBG_ASSERT( 0, DBG_MOD_WGL );
        }

        for(ui4_tick = 0; ui4_tick < ui4_total_tick; ui4_tick ++)
        {
            i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_TICK,WGL_PACK(ui4_tick), NULL);
            if(i4_ret != WGLR_OK)
            {
           x_dbg_stmt("{ANIM} ERR: Mem alloc Fail. [%d,%d]File=%s,Line=%d\n\n",ui4_test,ui4_tick,__FILE__, __LINE__);
            //    DBG_ASSERT( 0, DBG_MOD_WGL );
            }    
            
            i4_ret = wgl_anim_cmd_proc(t_cli_anim,WGL_CMD_ANIM_ATTR_GET_TICK_INF, NULL,WGL_PACK(&t_tick_inf));
            if(i4_ret != WGLR_OK)
            {
           x_dbg_stmt("{ANIM} ERR: Mem alloc Fail. [%d,%d]File=%s,Line=%d\n\n",ui4_test,ui4_tick,__FILE__, __LINE__);
               // DBG_ASSERT( 0, DBG_MOD_WGL );
            } 
            
            i4_ret = wgl_anim_cmd_proc(t_cli_anim, WGL_CMD_ANIM_ATTR_COND,WGL_PACK(&t_cond), NULL);
            if(i4_ret != WGLR_OK)
            {
           x_dbg_stmt("{ANIM} ERR: Mem alloc Fail. [%d,%d]File=%s,Line=%d\n\n",ui4_test,ui4_tick,__FILE__, __LINE__);
             //   DBG_ASSERT( 0, DBG_MOD_WGL );
            } 

            x_memset(&t_tick_inf,0,sizeof(WGL_ANIM_TICK_DATA));
            x_memset(&t_cond,0,sizeof(WGL_ANIM_COND_INF_T));
        }

        x_mem_free(t_begin.pt_anim_data);
        x_mem_free(t_begin.pt_path_mode);
        ui4_test ++;
        x_thread_delay(150);
    }while(ui4_test < 10000);
    
   x_dbg_stmt("Animation Stress test Sucess\n\n");
    
    return (CLIR_OK);
}
static INT32 _wgl_anim_cli_set_act_wgl_type(UINT16 ui2_data,HANDLE_TYPE_T* pe_type_widget)
{
    HANDLE_TYPE_T                       e_type_widget;

    if(pe_type_widget == NULL)
    {
        return CLIR_INV_ARG;
    }
    
    switch(ui2_data)
    {
    case 0x0:
        e_type_widget = HT_WGL_WIDGET_FRAME;
        break;

    case 0x1:
        e_type_widget = HT_WGL_WIDGET_ICON;
        break;

    case 0x2:
        e_type_widget = HT_WGL_WIDGET_TEXT;
        break;

    case 0x3:
        e_type_widget = HT_WGL_WIDGET_BUTTON;
        break;

    case 0x4:
        e_type_widget = HT_WGL_WIDGET_PROGRESS_BAR;
        break;

    case 0x5:
        e_type_widget = HT_WGL_WIDGET_EDIT_BOX;
        break;

    case 0x6:
        e_type_widget = HT_WGL_WIDGET_LIST_BOX;
        break;

    case 0x7:
        e_type_widget = HT_WGL_WIDGET_TKTP;
        break;

    case 0x8:
        e_type_widget = HT_WGL_WIDGET_HTS;
        break;

    case 0x9:
        e_type_widget = HT_WGL_WIDGET_SBTL;
        break;

    case 0xA:
        e_type_widget = HT_WGL_WIDGET_GROUP;
        break;

    case 0xB:
        e_type_widget = HT_WGL_WIDGET_CC_ISDB;
        break;

    default:
        return CLIR_INV_ARG;
    }

    *pe_type_widget = e_type_widget;
    return (CLIR_OK);
}

static INT32 _wgl_anim_cli_add_action(INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_TYPE_T                       e_type_widget;
    WGL_ANIM_ACTION_MAP_T*              pt_action_map;
    WGL_ANIM_ACTION_MAP_T*              pt_action;
    UINT16                              ui2_count;
    UINT16                              ui2_data;
    UINT16                              ui2_i;
    UINT32                              ui4_size;
    INT32                               i4_ret;

    if ((i4_argc < 4) || (pps_argv == NULL))
    {
        x_dbg_stmt("[Anim] Add_act: [widget_type] [count] [action,path id]\n\r");
        return CLIR_OK;
    }  
    
    ui2_data = (UINT16)x_strtoll(pps_argv[1],NULL,10);
    i4_ret = _wgl_anim_cli_set_act_wgl_type(ui2_data,&e_type_widget);
    if(i4_ret != CLIR_OK)
    {
        x_dbg_stmt("[Anim] The widget type is not valid when add action\n");
        return CLIR_INV_ARG;
    }
    
    ui2_count = (UINT16)x_strtoll(pps_argv[2],NULL,10);
    if(ui2_count == 0)
    {
        x_dbg_stmt("[Anim] Add path: count = 0\n\r");
        return CLIR_OK;
    }

    ui4_size = sizeof(WGL_ANIM_ACTION_MAP_T) * ui2_count;
    pt_action_map = (WGL_ANIM_ACTION_MAP_T*)x_mem_alloc(ui4_size);
    if(pt_action_map == NULL)
    {
        return CLIR_OK;
    }

    x_memset(pt_action_map,0,ui4_size);
    pt_action = pt_action_map;
    for(ui2_i = 0; ui2_i < ui2_count; ui2_i ++)
    {
        pt_action->e_anim_action_type = (WGL_ANIM_ACTION_TYPE_T)x_strtoll(pps_argv[(3+(ui2_i*2))],NULL,10);
        pt_action->t_anim_context = ANIM_CONTEXT_APP_SYNC;
        pt_action->t_anim_data.ui2_count = 1;
        WGL_SET_FLAG(pt_action->t_anim_data.ui4_flag,WGL_ANIM_DATA_FLAG_PATH_ID_INF);  
        pt_action->t_anim_data.t_path_id_inf.ui2_path_cnt = 1;
        pt_action->t_anim_data.t_path_id_inf.aui4_path_id[0] = (UINT32)x_strtoll(pps_argv[(4+(ui2_i*2))],NULL,10);
        pt_action->t_anim_data.t_path_id_inf.aui4_srt_tick[0] = 0;
        pt_action->t_anim_data.t_path_id_inf.aui2_start_percent[0] = 0;
        pt_action->t_anim_data.t_path_id_inf.aui2_end_percent[0] = 100;
        pt_action ++;
    }
    
    x_wgl_anim_add_default_actmap(e_type_widget,pt_action_map,ui2_count);
    x_mem_free(pt_action_map);
    return (CLIR_OK);
}

static INT32 _wgl_anim_cli_rmv_action(INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_TYPE_T                       e_type_widget;
    UINT16                              ui2_data;
    WGL_ANIM_ACTION_TYPE_T              e_type ;
    INT32                               i4_ret;

    if ((i4_argc != 3) || (pps_argv == NULL))
    {
        x_dbg_stmt("[Anim] rmv_act: [widget_type] [action type]\n\r");
        return CLIR_OK;
    } 

    ui2_data = (UINT16)x_strtoll(pps_argv[1],NULL,10);
    i4_ret = _wgl_anim_cli_set_act_wgl_type(ui2_data,&e_type_widget);
    if(i4_ret != CLIR_OK)
    {
        x_dbg_stmt("[Anim] The widget type is not valid when add action\n");
        return CLIR_INV_ARG;
    }

    e_type = (WGL_ANIM_ACTION_TYPE_T)x_strtoll(pps_argv[2],NULL,10);
    if(e_type == WGL_ANIM_ACTION_INGORE)
    {
        x_wgl_anim_remove_default_actmap(e_type_widget,TRUE,e_type);
    }
    else
    {
        x_wgl_anim_remove_default_actmap(e_type_widget,FALSE,e_type);
    }
    
    return (CLIR_OK);
}

static INT32 _wgl_anim_cli_get_action(INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_TYPE_T                       e_type_widget;
    UINT16                              ui2_data;
    WGL_ANIM_ACTION_TYPE_T              e_type ;
    INT32                               i4_ret;
    WGL_ANIM_ACTION_MAP_T*              pt_action_map;
        
    if ((i4_argc != 3) || (pps_argv == NULL))
    {
        x_dbg_stmt("[Anim] rmv_act: [widget_type] [action type]\n\r");
        return CLIR_OK;
    } 

    ui2_data = (UINT16)x_strtoll(pps_argv[1],NULL,10);
    i4_ret = _wgl_anim_cli_set_act_wgl_type(ui2_data,&e_type_widget);
    if(i4_ret != CLIR_OK)
    {
        x_dbg_stmt("[Anim] The widget type is not valid when add action\n");
        return CLIR_INV_ARG;
    }

    e_type = (WGL_ANIM_ACTION_TYPE_T)x_strtoll(pps_argv[2],NULL,10);

    x_wgl_anim_get_default_actmap(e_type_widget,e_type, &pt_action_map);
    
    return (CLIR_OK);
}

#endif    

#endif

