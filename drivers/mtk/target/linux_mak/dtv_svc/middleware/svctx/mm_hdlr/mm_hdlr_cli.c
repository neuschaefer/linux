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
 * $RCSfile: mm_hdlr_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/5 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 631271d39cbc47b5a9231693a0fbf6ae $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "cli/x_cli.h"
#include "os/inc/x_os.h"

#include "inc/common.h"
#include "inc/u_sys_name.h"
#include "svctx/svctx.h"
#include "svctx/svctx_dbg.h"
#include "svctx/svctx_cli.h"
#include "svctx/mm_hdlr/mm_svc_req.h"
#include "svctx/mm_hdlr/u_mm_hdlr.h"
#include "dbg/x_dbg_drv.h"

#include "strm_mngr/mm_sbtl_hdlr/u_sm_mm_sbtl_hdlr.h"
#include "wgl/subtitle/u_wgl_sbtl.h"
#include "wgl/x_wgl.h"
#include "wgl/u_wgl_tools.h"
#include "wgl/u_wgl_obj_type.h"
#include "graphic/x_gl.h"

#include "file_mngr/x_fm_mfw.h"
#include "file_mngr/x_fm.h"

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_open_media (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_close_media(INT32 i4_argc, const CHAR** pps_argv);
#ifdef MM_TIME_SHIFT_UNIT_TEST
static INT32 _mm_hdlr_update_valid_range(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_open_time_shift(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_close_time_shift(INT32 i4_argc, const CHAR** pps_argv);
#endif /* MM_TIME_SHIFT_UNIT_TEST */
static INT32 _mm_hdlr_stop_media (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_get_tc (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_get_dur(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_media_select_svc (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_media_select_stream (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_media_trick (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_media_seek (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_get_raw_data(INT32 i4_argc, const CHAR** pps_argv);
#ifdef TIME_MEASUREMENT
static INT32 _mm_hdlr_cli_set_tms_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_cli_get_tms_level(INT32 i4_argc, const CHAR** pps_argv);
#endif /* TIME_MEASUREMENT */

static INT32 _mm_hdlr_open_drm_media (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_close_drm_media (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_getinfo_drm_media (INT32 i4_argc, const CHAR** pps_argv);
#if 0
static INT32 _mm_hdlr_setinfo_drm_media (INT32 i4_argc, const CHAR** pps_argv);
#endif
static INT32 _mm_hdlr_getinfo_generic_drm_media (INT32 i4_argc, const CHAR** pps_argv);

static INT32 _mm_hdlr_create_surface(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_delete_surface(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_load_thumbnail(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_load_thumbnail_stress_test(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_sync_cancel_load_thumbnail(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_load_preview_thumbnail(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_sync_cancel_load_preview_thumbnail(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_set_thumbnail_region_transprent (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_divx_test (INT32 i4_argc, const CHAR** pps_argv);
static INT32 svctx_cli_open_timecode(INT32 i4_argc, const CHAR** pps_argv);

extern BOOL gb_tm_on_off;

#ifdef MULTIMEDIA_AUTOTEST_SUPPORT
// _cmpb_cli_push_send_buffer_cli

typedef enum
{
    SCPT_BASIC_DATA_TYPE_NONE = 0x0,
    SCPT_BASIC_DATA_TYPE_NUM,
    SCPT_BASIC_DATA_TYPE_STR,
    SCPT_BASIC_DATA_TYPE_VAL,
    SCPT_BASIC_DATA_TYPE_MAX
} SCPT_BASIC_DATA_TYPE_T;

typedef enum
{
    SCPT_PARSER_NONE = 0x0,
    SCPT_PARSER_USELESS_CHAR,
    SCPT_PARSER_GLB_U8,
    SCPT_PARSER_GLB_STRING,
    SCPT_PARSER_TEST_CASE,
    SCPT_PARSER_ERROR,
    SCPT_PARSER_MAX
} SCPT_PARSER_STATE_T;

typedef enum
{
    TEST_CASE_NONE = 0x0,
    TEST_CASE_NAME,
    TEST_CASE_START,
    TEST_CASE_CMD,
    TEST_CASE_SEQ,
    TEST_CASE_LOOP,
    TEST_CASE_END,
    TEST_CASE_ERROR,
    TEST_CASE_MAX
} SCPT_PARSER_TEST_CASE_STATE_T;

typedef enum
{
    TEST_CASE_CMD_NONE = 0x0,
    TEST_CASE_CMD_SEQ,
    TEST_CASE_CMD_LOOP,
    TEST_CASE_CMD_END,
    TEST_CASE_CMD_ERR,
    TEST_CASE_CMD_MAX
} SCPT_PARSER_CMD_STATE_T;


typedef enum
{
    SCPT_CODE_TYPE_NONE = 0x0,
    SCPT_CODE_TYPE_SENTENCE,
    SCPT_CODE_TYPE_FUNC,
    SCPT_CODE_TYPE_LOOP,
    SCPT_CODE_TYPE_MAX
} SCPT_CODE_TYPE_T;

#define AUTOTEST_GETGOLDEN_TIME 1
#define SCPT_KEY_WORD_LEN             0x10 
#define SCPT_BASIC_DATA_TYPE_MAX      0x10 
#define SCPT_FUNC_PARA_LEN            0xFF
#define SCPT_FUNC_PARA_CNT            0x10
#define SCPT_CALL_STACK_MAX           0x20

#define SCPT_U8_DATA_STR_LEN          0x10

#define SCPT_U4_INVALID               0xFFFFFFFF

/* varaible name max length  */
#define SCPT_VARA_NAME_MAX            0x20
#define SCPT_PARA_CNT_MAX             0x10

#define SCPT_CHAR_CR                         0xd    /* new line character */
#define SCPT_CHAR_LF                         0xa    /* new line character */
#define SCPT_CHAR_SPACE                      0x20   /* space */
#define SCPT_CHAR_TAB                        0x09   /* tab */
#define SCPT_CHAR_ZERO                    0x0  /* \0 */
#define SCPT_CHAR_COLON                   0x3a  /* : */


#define SCPT_CHAR_SLASH                      0x2f   /* / */
#define SCPT_CHAR_ASTERISK                   0x2a   /* * */
#define SCPT_CHAR_EM                         '='
#define SCPT_CHAR_LEFT_MID_BRACKET           '['
#define SCPT_CHAR_RIGHT_MID_BRACKET          ']'
#define SCPT_CHAR_LEFT_BRACKET               '('
#define SCPT_CHAR_RIGHT_BRACKET              ')'
#define SCPT_CHAR_RIGHT_BIG_BRACKET          '}'
#define SCPT_CHAR_LEFT_BIG_BRACKET           '{'
#define SCPT_CHAR_SEMICOLON                  ';'
#define SCPT_CHAR_COMMA                      ','

#define SCPT_NUM_LEN                         11


#define SCPT_CHAR_DQM                        '"'   /* double quotation mark */


#define SCPT_BASIC_TYPE_U8                   "UINT4"
#define SCPT_BASIC_TYPE_STR                  "STRING"
#define SCPT_CHECK                           "c"

#define SCPT_BASIC_TYPE_HEX_STR              "0x"

#define SCPT_KEY_START                      "START:"
#define SCPT_KEY_END                        "END:"
#define SCPT_KEY_LOOP                       "LOOP"
#define SCPT_CMD_MO                         "mo"
#define SCPT_CMD_GGV                        "ggv"


#define SCPT_TEST_CASE_FG                    "[MTK_MM_TEST_CASE_"

#define AUTOTEST_MEMLEAK_CHK

typedef struct _SCPT_CODE_NODE_T
{   
    SCPT_CODE_TYPE_T              e_type;
    UCHAR*                        puc_code;
    struct _SCPT_CODE_NODE_T*    pt_next;
} SCPT_CODE_NODE_T;

typedef struct
{
    UINT32 ui4_top;
    SCPT_CODE_NODE_T*  pat_node[SCPT_CALL_STACK_MAX];
} SCPT_CALL_STACK_T;

typedef struct
{
    HANDLE_T                        h_file;
    UINT32                          ui4_size;
    CHAR*                           puc_start;   /* current cursor  */
    CHAR*                           puc_crnt;    /* current cursor  */
} SCPT_TEXT_T;

typedef struct _SCPT_U8_TYPE_T
{
    CHAR*                                   pac_val;
    CHAR*                                   pac_vara;
    struct _SCPT_U8_TYPE_T*                pt_next;
} SCPT_U8_TYPE_T;

typedef struct _SCPT_U8_TYPE_LIST_T
{   
    UINT32                                  ui4_len;
    SCPT_U8_TYPE_T*                         pt_head;
} SCPT_U8_TYPE_LIST_T;

typedef struct _SCPT_STRING_CLASS_T
{
    UINT32                                  ui4_len;
    CHAR*                                   pc_str;
} SCPT_STRING_CLASS_T;

typedef struct _SCPT_STRING_ARRY_TYPE_T
{
    UINT32                                  ui4_cnt;
    UINT32                                  ui4_idx;
    UINT32                                  ui4_len;
    CHAR*                                   pac_vals;
    CHAR*                                   pac_str_arry_name;
    struct _SCPT_STRING_ARRY_TYPE_T*       pt_next;
} SCPT_STRING_ARRY_TYPE_T;

typedef struct _SCPT_STRING_TYPE_LIST_T
{   
    UINT32                                  ui4_len;
    SCPT_STRING_ARRY_TYPE_T*                pt_head;
} SCPT_STRING_TYPE_LIST_T;

typedef struct _SCPT_TEST_CASE_T
{   
    UINT32                                  ui4_failed;
    UINT32                                  ui4_tc_sum;
    SCPT_PARSER_CMD_STATE_T                 e_cmd_stt;
    SCPT_PARSER_TEST_CASE_STATE_T           e_stt;
    BOOL                                    b_check;
} SCPT_TEST_CASE_T;

typedef struct
{
    SCPT_PARSER_STATE_T                     e_stt;
    SCPT_U8_TYPE_LIST_T                     t_b8_list;
    SCPT_STRING_TYPE_LIST_T                 t_str_arry_type_list;
    SCPT_TEST_CASE_T                        t_tc;           // test case
    SCPT_TEXT_T                             t_file;
} SCPT_DATA_T;
#define AUTOTEST_MAXCASE_INDEX (30)
#define AUTOTEST_MAX_FILE (30)

typedef struct
{
  UINT32 u4FileIndex;
  UINT32 u4LasFileIndex;
  UINT32 u4LoopIndex;
  UINT32 u4CurId;
  UINT32 u4CurType;
  HANDLE_T hCaseSema;
  BOOL fgResult;
  BOOL fgWaitCallback;
  BOOL fgCaseResult;                  //test case fail or pass
  BOOL afgIsFail[AUTOTEST_MAX_FILE][AUTOTEST_MAXCASE_INDEX];
  char u1FailMessage[AUTOTEST_MAXCASE_INDEX][1024];
}SCPT_INFO_T;

typedef struct
{
  UINT32 u4GoldenTime;
  UINT32 u4Diff;
}SCPT_GODLEN_INFO_T;

static SCPT_INFO_T rScptInfo={0};
static BOOL rScptLock = FALSE;
#if AUTOTEST_GETGOLDEN_TIME
#define GOLDEN_CHECKTIME (20)
UINT32 _grGoldenTime_cur[AUTOTEST_MAXCASE_INDEX] = {0};
UINT32 _grGoldenTime_Max[AUTOTEST_MAXCASE_INDEX] = {0};
UINT32 _grGoldenTime_Min[AUTOTEST_MAXCASE_INDEX] = {0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
    0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
    0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
    0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff};
#endif

SCPT_GODLEN_INFO_T _grGoldenInfo[AUTOTEST_MAX_FILE][AUTOTEST_MAXCASE_INDEX]=
{
  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}, // File 1
  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}, // File 2
  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}  // File 3
};

#ifdef VDEC_TIME_PROFILE
BOOL fgAutoPlayRecordFileName = FALSE;
CHAR ucAutoPlayCurFileDir[512];
CHAR ucAutoPlayCurFileName[256];
#endif

#ifdef AUTOTEST_MEMLEAK_CHK
typedef struct
{
    CHAR uc_name[30];
    INT32 i4_active_objs;
    INT32 i4_num_objs;
    INT32 i4_obj_size;
    INT32 i4_obj_per_slab;
    INT32 i4_pages_per_slab;
}SLAB_OBJ_INFO_T;

typedef struct
{
    INT32 i4_batch_count;
    INT32 i4_limit;
    INT32 i4_shared_factor;
}SLAB_TUNABLE_INFO_T;

typedef struct
{
    INT32 i4_active_slabs;
    INT32 i4_num_slabs;
    INT32 i4_shared_avail;
}SLAB_DATA_INFO_T;

typedef struct _SLAB_DETAIL_INFO_T
{
    SLAB_OBJ_INFO_T* 	pt_obj_info;
    SLAB_TUNABLE_INFO_T* pt_tunable_info;
    SLAB_DATA_INFO_T* 	pt_data_info;
    struct _SLAB_DETAIL_INFO_T* pt_next;
}SLAB_DETAIL_INFO_T;

typedef struct
{
    BOOL 	b_first_set;
    BOOL 	b_mem_leak;
#ifndef LINUX_TURNKEY_SOLUTION
    INT32 	i4_delta_mem;
    INT32 	i4_last_avail_mem;
    INT32 	i4_avail_mem;
#else
    UINT32 ui4_threshold;
    SLAB_DETAIL_INFO_T* pt_last_slab_list;
    SLAB_DETAIL_INFO_T* pt_slab_list;
    SLAB_DETAIL_INFO_T* pt_delta_list;
#endif
}MEM_MONITOR_INFO_T;
static MEM_MONITOR_INFO_T rMemInfo;
#endif //!#ifdef AUTOTEST_MEMLEAK_CHK

#endif

#ifdef MULTIMEDIA_AUTOTEST_SUPPORT
static INT32 _mm_hdlr_mm_auto_test_loop (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_mm_to_drv_pipe(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_mm_thread_delay(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_mm_get_golden_values(INT32 i4_argc, const CHAR** pps_argv);
static CHAR* _scpt_filter_useless_char( CHAR* puc_str ,     UINT32 ui4_len );
static BOOL _scpt_is_word_char( CHAR*                       puc_str );
static BOOL _scpt_is_number_char( CHAR*                     puc_str );
static UINT32 _scpt_query_str_len( CHAR* puc_str ,          UINT32 ui4_len );
static INT32 _scpt_get_crnt_buf_size( SCPT_TEXT_T*          pt_file );
static void _scpt_u8_list_init( SCPT_U8_TYPE_LIST_T*        pt_u8_list );
static INT32 _scpt_u8_list_add(   SCPT_U8_TYPE_LIST_T*       pt_u8_list,
                                    SCPT_U8_TYPE_T*         pt_node );
static SCPT_U8_TYPE_T* _scpt_query_u8_node( SCPT_U8_TYPE_LIST_T*     pt_list,
                                            CHAR*                    pc_str );

static SCPT_U8_TYPE_T* _scpt_get_u8_list_tail(SCPT_U8_TYPE_LIST_T*  pt_u8_list);
static SCPT_U8_TYPE_T* _scpt_u8_list_get_node(SCPT_U8_TYPE_LIST_T*  pt_u8_list,
                                              UINT32                u8_cnt);
static INT32 _scpt_u8_list_rm_node(  SCPT_U8_TYPE_LIST_T*           pt_u8_list,
                                         UINT32                     u8_rm_cnt);
static void _scpt_u8_list_free( SCPT_U8_TYPE_LIST_T*                pt_u8_list);
static INT32 _scpt_check_number_str(    CHAR*   puc_str ,          
                                        UINT32  ui4_len );
static INT32 _scpt_parser_all_glb_u8_vara( SCPT_DATA_T*         pt_scpt_data );
static INT32 _scpt_parser_one_glb_u8_vara( SCPT_DATA_T*         pt_scpt_data );
static INT32 _scpt_parser_all_str_array( SCPT_DATA_T*           pt_scpt_data );
static INT32 _scpt_parser_one_str_array( SCPT_DATA_T*           pt_scpt_data );
static SCPT_STRING_ARRY_TYPE_T* _scpt_get_str_arry_type_list_tail(
                                     SCPT_STRING_TYPE_LIST_T*   pt_list);
static SCPT_STRING_ARRY_TYPE_T* _scpt_get_str_arry_type_list_node(
                                    SCPT_STRING_TYPE_LIST_T*    pt_list,
                                    UINT32                      u8_cnt);
static INT32 _scpt_parser_init(SCPT_DATA_T*                     pt_scpt_data);
static INT32 _scpt_parser_deinit(SCPT_DATA_T*                   pt_scpt_data);
static INT32 _scpt_parser_query_big_bracket_str_len(CHAR* pb_str,UINT32 ui4_len);
static INT32 _scpt_str_arry_type_list_add( SCPT_STRING_TYPE_LIST_T*   pt_list,
                                           SCPT_STRING_ARRY_TYPE_T*   pt_node );
static UINT32 _scpt_parser_get_str_dqm_cnt( CHAR*  pt_str, UINT32 u4_len);
static INT32 _scpt_parser_one_str_array_vals(
                                SCPT_STRING_ARRY_TYPE_T*  pt_str_arry_type);
static INT32 _scpt_parser_query_dq_str_len( CHAR*   pb_str,     UINT32 ui4_len);
static INT32 _scpt_parser_glb_u8_stt_chg( SCPT_DATA_T*          pt_scpt_data );
static INT32 _scpt_parser_all_str_array_stt_chg( SCPT_DATA_T*   pt_scpt_data );
static INT32 _scpt_parser(SCPT_DATA_T*      pt_scpt_data);
static BOOL _scpt_filter_is_comment_start( CHAR*                    puc_str);
static BOOL _scpt_filter_is_comment_end( CHAR*                      puc_str);
static CHAR* _scpt_filter_comment( CHAR* puc_str ,          UINT32 ui4_len );
static BOOL _scpt_is_nl( CHAR*                 puc_str);
static BOOL _scpt_is_left_mid_bracket( CHAR*                 puc_str);
static BOOL _scpt_is_right_mid_bracket( CHAR*                 puc_str);
static CHAR* _scpt_filter_nl( CHAR* puc_str , UINT32 ui4_len );
static BOOL _scpt_is_space( CHAR*                 puc_str);
static CHAR* _scpt_filter_space( CHAR* puc_str , UINT32 ui4_len );
static BOOL _scpt_is_number( CHAR       uc_char );
static UINT32 _scpt_query_number_str_len( CHAR* puc_str ,    UINT32 ui4_len );
static CHAR*  _scpt_parser_filter_and_check_semicolon(CHAR* pb_str,UINT32 ui4_len);
static CHAR* _scpt_parser_filter_to_semicolon(CHAR* pb_str,UINT32 ui4_len);
static INT32 _scpt_parser_test_case_stt_chg( SCPT_DATA_T*      pt_scpt_data );
static UINT32 _scpt_parser_query_mid_bracket_str_len(CHAR* puc_str,UINT32 ui4_len);
static UINT32 _scpt_parser_check_test_case_cmd( char*        pc_str );
static INT32 _scpt_parser_test_case_eng( SCPT_DATA_T*      pt_scpt_data );
static void _scpt_parser_tc_cmd_stt_chg( SCPT_DATA_T*      pt_scpt_data );
static INT32 _scpt_parser_do_cmd(SCPT_DATA_T*  pt_scpt_data,UINT32 ui4_cmd_idx );
static BOOL _scpt_is_para_divide_char( CHAR* puc_str );
static SCPT_STRING_ARRY_TYPE_T* _scpt_query_str_node( 
                        SCPT_STRING_TYPE_LIST_T*   pt_list,
                        CHAR*                      pc_str );
static INT32 _scpt_get_str_array_val( 
                SCPT_STRING_ARRY_TYPE_T*   pt_str_arry,
                SCPT_STRING_CLASS_T*       pt_str_obj );
static SCPT_STRING_ARRY_TYPE_T* _scpt_parser_query_path_arry(
                                SCPT_DATA_T*        pt_scpt_data,
                                CHAR*               pc_path );
static INT32 _scpt_parser_one_test_case( SCPT_DATA_T*      pt_scpt_data );
static INT32 _scpt_parser_get_one_gldn_val(
                            SCPT_GODLEN_INFO_T*         pt_gldn_val, 
                            SCPT_STRING_CLASS_T*        pt_str_ojb);
static INT32 _scpt_parser_set_golden_values(
                            SCPT_GODLEN_INFO_T*         pa_gldn_vals, 
                            SCPT_STRING_CLASS_T*        pt_str_ojb);
static INT32 _scpt_parser_get_golden_values(
                            SCPT_GODLEN_INFO_T         pa_gldn_vals[][AUTOTEST_MAXCASE_INDEX], 
                            SCPT_STRING_ARRY_TYPE_T*    pt_str_arry);

#ifdef AUTOTEST_MEMLEAK_CHK
#ifdef LINUX_TURNKEY_SOLUTION
static INT32 _scpt_update_slab_list( SLAB_DETAIL_INFO_T* pt_slab_list_head, 
						BOOL 	b_update_ref );
static SLAB_DETAIL_INFO_T* _scpt_alloc_slab_info( SLAB_DETAIL_INFO_T** ppt_head );
static UINT32 _scpt_query_slab_list_length( SLAB_DETAIL_INFO_T* pt_head );
static INT32 _scpt_free_slab_list( SLAB_DETAIL_INFO_T** ppr_unit );
static SLAB_DETAIL_INFO_T*  _scpt_filter_delta_slab_info( SLAB_DETAIL_INFO_T* pt_delta_info );
static SLAB_DETAIL_INFO_T* _scpt_calc_delta_slab_info( 
			SLAB_DETAIL_INFO_T* 	pt_slab_last,  SLAB_DETAIL_INFO_T* pt_slab_current );
static SLAB_DETAIL_INFO_T* _scpt_dup_slab_list( SLAB_DETAIL_INFO_T* pt_src_list );
static INT32 _scpt_copy_slab_info( SLAB_DETAIL_INFO_T* pt_dst_info, 
						SLAB_DETAIL_INFO_T* pt_src_info );
static INT32 _scpt_copy_slab_obj_info( SLAB_OBJ_INFO_T* pt_dst_info, 
						SLAB_OBJ_INFO_T* pt_src_info );
static INT32 _scpt_copy_slab_tunable_info( SLAB_TUNABLE_INFO_T* pt_dst_info, 
						SLAB_TUNABLE_INFO_T* pt_src_info );
static INT32 _scpt_copy_slab_data_info( SLAB_DATA_INFO_T* pt_dst_info, 
						SLAB_DATA_INFO_T* pt_src_info );
static INT32  _scpt_init_slab_info( SLAB_DETAIL_INFO_T* pt_detail_info	);
static INT32  _scpt_init_slab_obj_info( SLAB_OBJ_INFO_T* pt_obj_info	);
static INT32  _scpt_init_slab_tunable_info( SLAB_TUNABLE_INFO_T* pt_tunable_info	);
static INT32  _scpt_init_slab_data_info( SLAB_DATA_INFO_T*  pt_data_info	);
static UINT32 _scpt_parser_query_proc( CHAR** ppt_read_buf, UINT32 ui4_len, 
															CHAR* ptr_key );
static VOID _scpt_parser_release_proc( CHAR** ppt_read_buf );
static INT32 _scpt_parser_read_slab_info( CHAR* puc_str, UINT32 ui4_len, 
						SLAB_DETAIL_INFO_T* pt_detail_info );
static INT32 _scpt_parser_read_obj_info( CHAR* puc_str, UINT32 ui4_len, 
						SLAB_OBJ_INFO_T* pt_obj_info );
static INT32 _scpt_parser_read_tunable_info( CHAR* puc_str, UINT32 ui4_len, 
						SLAB_TUNABLE_INFO_T* pt_tunable_info );
static INT32 _scpt_parser_read_data_info( CHAR* puc_str, UINT32 ui4_len, 
						SLAB_DATA_INFO_T* pt_data_info	);
static SLAB_DETAIL_INFO_T* _scpt_parser_gen_slab_detail_list( 
			CHAR** ppt_read_buf , UINT32 ui4_len );
static UINT32 _scpt_find_first_char( CHAR* puc_str , UINT32 ui4_len , CHAR uc_char );
static BOOL _scpt_is_word_char_ex( CHAR* puc_str );
static UINT32 _scpt_query_str_len_ex( CHAR* puc_str , UINT32 ui4_len );
#else
static INT32 _scpt_update_memory_leakage(INT32 i4_count, BOOL b_update_ref);
//only call this api in nucleus mode
extern VOID os_mem_cli_stats(HANDLE_T h_part_hdl, SIZE_T *z_totalfree, SIZE_T *z_bigfree,
                 					SIZE_T *z_freeobj, SIZE_T *z_allocatedobj, SIZE_T *z_totalsize,
                 					SIZE_T *z_allocsize, CHAR *ps_name);
//only call this api in nucleus mode
extern INT32 os_mem_scan_mem(HANDLE_T h_part_hdl);

#endif //!#ifdef LINUX_TURNKEY_SOLUTION
static INT32 _mm_hdlr_mm_monitor_memory(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _mm_hdlr_mm_reset_meminfo( INT32 i4_argc, const CHAR** pps_argv );
static MEM_MONITOR_INFO_T* _scpt_get_meminfo( VOID );
static INT32 _mm_hdlr_mm_show_meminfo( INT32 i4_argc, const CHAR** pps_argv );

#endif //!#ifdef AUTOTEST_MEMLEAK_CHK
#endif


/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
HANDLE_T h_g_main_svctx        = NULL_HANDLE;
static HANDLE_T h_g_media             = NULL_HANDLE;
static VSH_REGION_INFO_T t_g_disp_region;
static BOOL b_g_thumbnail_previewing  = FALSE;

static CLI_EXEC_T at_mm_hdlr_cmd_tbl[] =
{
    {   "media open",
        "mo",
        _mm_hdlr_open_media,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "media close",
        "mc",
        _mm_hdlr_close_media,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
#ifdef MM_TIME_SHIFT_UNIT_TEST
    {   "update valid range",
        "uvr",
        _mm_hdlr_update_valid_range,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "open time shift",
        "ots",
        _mm_hdlr_open_time_shift,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "close time shift",
        "cts",
        _mm_hdlr_close_time_shift,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
#endif /* MM_TIME_SHIFT_UNIT_TEST */
    {   "media svc select",
        "mss",
         _mm_hdlr_media_select_svc,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "media stream select",
        "msst",
         _mm_hdlr_media_select_stream,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "media trick",
        "mtk",
        _mm_hdlr_media_trick,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "media seek",
        "msk",
        _mm_hdlr_media_seek,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "media stop",
        "mst",
        _mm_hdlr_stop_media,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {
        "media_get_raw_data",
        "mgrd",
        _mm_hdlr_get_raw_data,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "time code get",
        "gtc",
        _mm_hdlr_get_tc,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "time code dur",
        "gdr",
        _mm_hdlr_get_dur,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "stop",
        "st",
        svctx_cli_sync_stop_svctx,
        NULL,
        "usage: st",
        CLI_SUPERVISOR
    },
    {   "close",
        "cl",
        svctx_cli_close_svctx,
        NULL,
        "usage: cl",
        CLI_SUPERVISOR
    },
    {   "scdb",
        "scdb",
        svctx_cli_dump_scdb_data,
        NULL,
        "usage: scdb",
        CLI_SUPERVISOR
    },
    {
        "timecode",
        "tm",
        svctx_cli_open_timecode,
        NULL,
        "usage: tm on/off",
        CLI_SUPERVISOR
    },
    {
        CLI_GET_DBG_LVL_STR,
        NULL,
        _mm_hdlr_cli_get_dbg_level,
        NULL,
        CLI_GET_DBG_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
    {
        CLI_SET_DBG_LVL_STR,
        NULL,
        _mm_hdlr_cli_set_dbg_level,
        NULL,
        CLI_SET_DBG_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
#ifdef TIME_MEASUREMENT
    {
        CLI_SET_TMS_LVL_STR,
        NULL,
        _mm_hdlr_cli_set_tms_level,
        NULL,
        CLI_SET_TMS_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
    {
        CLI_GET_TMS_LVL_STR,
        NULL,
        _mm_hdlr_cli_get_tms_level,
        NULL,
        CLI_GET_TMS_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
#endif /* TIME_MEASUREMENT */

    {   "drm media open",
        "dmo",
        _mm_hdlr_open_drm_media,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "drm media close",
        "dmc",
        _mm_hdlr_close_drm_media,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "drm media getinfo",
        "dmg",
        _mm_hdlr_getinfo_drm_media,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
#if 0
    {   "drm media setinfo",
        "dms",
        _mm_hdlr_setinfo_drm_media,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
#endif
    {   "drm media getinfo2",
        "dmgg",
        _mm_hdlr_getinfo_generic_drm_media,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "create surface",
        "cs",
        _mm_hdlr_create_surface,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "delete surface",
        "ds",
        _mm_hdlr_delete_surface,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "load thumbnail",
        "lt",
        _mm_hdlr_load_thumbnail,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "load thumbnail stress test",
        "ltst",
        _mm_hdlr_load_thumbnail_stress_test,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "sync cancel load thumbnail",
        "sclt",
        _mm_hdlr_sync_cancel_load_thumbnail,
        NULL,
        "usage: ",
        CLI_SUPERVISOR
    },
    {   "load preview thumbnail",
        "lpt",
        _mm_hdlr_load_preview_thumbnail,
        NULL,
        "Usage: lpt ui4_x ui4_y ui4_width ui4_height",
        CLI_SUPERVISOR
    },
    {   "set thumbnail region transparent",
        "strt",
        _mm_hdlr_set_thumbnail_region_transprent,
        NULL,
        "usage:strt 0/1 ",
        CLI_SUPERVISOR
    },
        
    {   "sync cancel load preview_thumbnail",
        "sclpt",
        _mm_hdlr_sync_cancel_load_preview_thumbnail,
        NULL,
        "usage:sclpt ",
        CLI_SUPERVISOR
    },
    {   "MKV DivXPlus test",
        "divx",
        _mm_hdlr_divx_test,
        NULL,
        "usage:divx pl_id ch_id",
        CLI_SUPERVISOR
    },
#ifdef MULTIMEDIA_AUTOTEST_SUPPORT
    {   "MW to Driver",
        "m2d",
        _mm_hdlr_mm_to_drv_pipe,
        NULL,
        "usage:mm auto test case mw to driver pipe",
        CLI_SUPERVISOR
    },
    {   "Get golden value",
        "ggv",
        _mm_hdlr_mm_get_golden_values,
        NULL,
        "usage: Get golden value",
        CLI_SUPERVISOR
    },
    {   "CLI thread delay",
        "dly",
        _mm_hdlr_mm_thread_delay,
        NULL,
        "usage: thread delay",
        CLI_SUPERVISOR
    },
    {   "MM auto test",
        "mat",
        _mm_hdlr_mm_auto_test_loop,
        NULL,
        "usage: paser auto test case script",
        CLI_SUPERVISOR
    },
#ifdef AUTOTEST_MEMLEAK_CHK
    {   "run mem leakage",
        "rml",
        _mm_hdlr_mm_monitor_memory,
        NULL,
        "usage: rml",
        CLI_SUPERVISOR 
   },
   {   "clear mem leakage",
        "cml",
        _mm_hdlr_mm_reset_meminfo,
        NULL,
        "usage: cml",
        CLI_SUPERVISOR 
   },
   {   "get mem leakage",
        "gml",
        _mm_hdlr_mm_show_meminfo,
        NULL,
        "usage: gml",
        CLI_SUPERVISOR 
   },
#endif

#endif
    END_OF_CLI_CMD_TBL
};


/* SVL Builder root command table */
static CLI_EXEC_T at_mm_hdlr_root_cmd_tbl[] =
{
    {"svctx_mm", NULL, NULL, at_mm_hdlr_cmd_tbl, "SVCTX-MM_HDLR commands",   CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Name:    svctx_cli_attach_cmd_tbl
 * Description:
 * Input arguments:  -
 * Output arguments: -
 * Returns:
 *----------------------------------------------------------------------------*/
INT32 mm_hdlr_cli_attach_cmd_tbl(VOID)
{
    return (x_cli_attach_cmd_tbl(at_mm_hdlr_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_PIPE));
}
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Name:    _atoi
 *
 * Description: Convert a number string to a number.
 *
 * Inputs:  ps     null ('\0') terminated character string.
 *
 *
 * Outputs: -
 *
 * Returns: sum of the number string.
 *
 ----------------------------------------------------------------------------*/
static INT32 _atoi( const CHAR* ps )
{
    INT32  i4_total;

    i4_total = 0;
    while ( *ps != 0 )
    {
        if ( *ps >= '0' && *ps <= '9' )
        {
            /* accumulate digit */
            i4_total = 10 * i4_total + (*ps - '0');
            ps++; /* get next char */
        }
        else /* not a digit char */
        {
            return (-1);
        }
    }

    return i4_total;
}
/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_cli_get_dbg_level
 *
 * Description: This API gets the current debug level of Service Context.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_cli_get_dbg_level (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32  i4_ret;

    i4_ret = x_cli_show_dbg_level(svctx_get_dbg_level());

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_cli_set_dbg_level
 *
 * Description: This API sets the debug level of Service Context.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_cli_set_dbg_level (INT32 i4_argc, const CHAR** pps_argv)
{
    UINT16 ui2_dbg_level;
    INT32  i4_ret;

    i4_ret = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_ret == CLIR_OK){
        svctx_set_dbg_level(ui2_dbg_level);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_open_media_nfy
 *
 * Description:
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _mm_hdlr_open_media_nfy(
                    HANDLE_T                    h_svctx,
                    SVCTX_COND_T                e_nfy_cond,
                    SVCTX_NTFY_CODE_T           e_code,
                    STREAM_TYPE_T               e_stream_type,
                    VOID*                       pv_nfy_tag,
                    VOID*                       pv_data
                    )
{
    x_dbg_stmt("%s() stream_type=%s, e_code=%s, e_nfy_cond=%s\r\n",
                    __func__,
                    s_g_strm_types[e_stream_type],
                    s_g_svctx_ntfy_codes[e_code],
                    s_g_state_strings[e_nfy_cond]
                    );
}

/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_media_select_svc_nfy
 *
 * Description:
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _mm_hdlr_media_select_svc_nfy(
                    HANDLE_T                    h_svctx,
                    SVCTX_COND_T                e_nfy_cond,
                    SVCTX_NTFY_CODE_T           e_code,
                    STREAM_TYPE_T               e_stream_type,
                    VOID*                       pv_nfy_tag
                    )
{
    INT32 i4_ret;
    SVCTX_T * pt_svctx;
    i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK) {
               return  /* SVCTXR_FAIL */ ;
            }
#ifndef MULTIMEDIA_AUTOTEST_SUPPORT

    x_dbg_stmt("%s() stream_type=%s, e_code=%s, e_nfy_cond=%s\r\n",
                    __func__,
                    s_g_strm_types[e_stream_type],
                    s_g_svctx_ntfy_codes[e_code],
                    s_g_state_strings[e_nfy_cond]
                    );
#endif 
    svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
}


/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_open_media
 *
 * Description: This API open a multimedia object.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
INT32 _mm_hdlr_open_media(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32               i4_ret      = CLIR_OK;
    MM_SVC_COMMAND_T    t_cmd[6];
    CHAR*               ps_name     = NULL;
    CHAR*               paui1_text = NULL;

    ps_name = "main_svctx";

    paui1_text = x_mem_alloc(x_strlen(pps_argv[1]) + 1);    
    x_memset(paui1_text, 0, x_strlen(pps_argv[1]));
    x_strcpy(paui1_text, pps_argv[1]);


    if (x_handle_valid(h_g_main_svctx) == TRUE) {
        x_svctx_close(h_g_main_svctx);
    }

    i4_ret = x_svctx_open(ps_name, &h_g_main_svctx);
    if (i4_ret != SVCTXR_OK) 
    {
        x_dbg_stmt("x_svctx_open(%s) failed, i4_ret = %d\r\n", ps_name, i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    t_cmd[0].e_code = MM_SVC_CMD_CODE_SVL_NAME;
    t_cmd[0].u.ps_text = "MEDIA_SVC";
    t_cmd[1].e_code = MM_SVC_CMD_CODE_SVL_ID;
    t_cmd[1].u.ui2_data = 1;
    t_cmd[2].e_code = MM_SVC_CMD_CODE_SRC_TYPE;
    t_cmd[2].u.ui1_data = SRC_TYPE_MEDIA_STORGE;
    t_cmd[3].e_code = MM_SVC_CMD_CODE_MEDIA_PATH;
    t_cmd[3].u.ps_text = paui1_text;

    t_cmd[4].e_code = MM_SVC_CMD_CODE_END;
    i4_ret = x_svctx_media_open(
                h_g_main_svctx,
                t_cmd,
                _mm_hdlr_open_media_nfy,
                NULL,
                &h_g_media
                );
    x_mem_free(paui1_text);
    paui1_text = NULL;
    if(i4_ret !=  SVCTXR_OK)
    {
      return CLIR_CMD_EXEC_ERROR;
    }
    return CLIR_OK;
}

static INT32 _mm_hdlr_close_media (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret  = CLIR_OK;

    if (x_handle_valid(h_g_main_svctx) && x_handle_valid(h_g_media))
    {
        i4_ret = x_svctx_media_close(
                    h_g_main_svctx,
                    h_g_media
                    );
        if (i4_ret !=  SVCTXR_OK)
        {
            x_dbg_stmt("x_svctx_media_close failed, i4_ret = %d\r\n", i4_ret);
            return CLIR_CMD_EXEC_ERROR;
        }

        i4_ret = x_svctx_close(h_g_main_svctx);
        if (i4_ret !=  SVCTXR_OK)
        {
            x_dbg_stmt("x_svctx_close failed, i4_ret = %d\r\n", i4_ret);
            return CLIR_CMD_EXEC_ERROR;
        }
    }

    return CLIR_OK;
}

#ifdef MM_TIME_SHIFT_UNIT_TEST
static CHAR gc_time_shift_file_set[3][64] =
{
    {"/mnt/usb/Mass-000/PVR/timeshift_01.pvr"},
    {"/mnt/usb/Mass-000/PVR/timeshift_01.tdtl"},
    {"/mnt/usb/Mass-000/PVR/timeshift_01.tidx"}
};
static HANDLE_T gh_time_shift_tick_detail    = NULL_HANDLE;
static HANDLE_T gh_time_shift_tick_index     = NULL_HANDLE;
static UINT8*   gpui1_time_shift_tick_detail = NULL;
static UINT8*   gpui1_time_shift_tick_index  = NULL;
static UINT32   gui4_time_shift_tick_idx_total = 0;

INT32 _mm_hdlr_update_valid_range(INT32 i4_argc, const CHAR** pps_argv)
{
    MM_BUF_RANGE_INFO_T t_range = {0};
    HANDLE_T            h_svctx = NULL_HANDLE;

    if (i4_argc == 4)
    {
        h_svctx = (HANDLE_T)_atoi(pps_argv[1]);
        t_range.ui4_start = (UINT32)_atoi(pps_argv[2]);
        t_range.ui4_end = (UINT32)_atoi(pps_argv[3]);
    }
    else
    {
        if (h_g_main_svctx == NULL_HANDLE ||
            h_g_media == NULL_HANDLE ||
            gui4_time_shift_tick_idx_total == 0)
        {
            return CLIR_CMD_EXEC_ERROR;
        }

        h_svctx = h_g_main_svctx;
        if (i4_argc == 2)
        {
            if ( x_strcmp( pps_argv[1], "all" ) == 0 )
            {
                t_range.ui4_start = 0;
                t_range.ui4_end = gui4_time_shift_tick_idx_total - 1;
            }
            else
            {
                x_dbg_stmt("\r\nUsage: uvr [all]\r\n");
                return CLIR_CMD_EXEC_ERROR;
            }
        }
        else if (i4_argc == 3)
        {
            t_range.ui4_start = (UINT32)_atoi(pps_argv[1]);
            t_range.ui4_end = (UINT32)_atoi(pps_argv[2]);
        }
    }

    x_svctx_time_shift_update_valid_range(h_svctx, &t_range);

    return CLIR_OK;
}

static VOID _time_shift_free(VOID)
{
    if (gpui1_time_shift_tick_detail)
    {
        x_mem_free(gpui1_time_shift_tick_detail);
        gpui1_time_shift_tick_detail = NULL;
    }
    if (gpui1_time_shift_tick_index)
    {
        x_mem_free(gpui1_time_shift_tick_index);
        gpui1_time_shift_tick_index = NULL;
    }
    if (x_handle_valid(gh_time_shift_tick_detail) == TRUE)
    {
        x_fm_mfw_close(gh_time_shift_tick_detail);
        gh_time_shift_tick_detail = NULL_HANDLE;
    }
    if (x_handle_valid(gh_time_shift_tick_index) == TRUE)
    {
        x_fm_mfw_close(gh_time_shift_tick_index);
        gh_time_shift_tick_index = NULL_HANDLE;
    }
    gui4_time_shift_tick_idx_total = 0;
}

INT32 _mm_hdlr_open_time_shift(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32               i4_ret  = CLIR_OK;
    MM_SVC_COMMAND_T    t_cmd[4];
    CHAR*               ps_name;
    PVR_TICK_CTRL_BLK_T t_tick_cb = {0};
    FM_FILE_INFO_T      t_file_info = {0};
    UINT32              ui4_read = 0;
    HANDLE_T            h_file = NULL_HANDLE;
    UINT8*              pui1_tick_detail;
    UINT8*              pui1_tick_index;
    UINT32              ui4_i = 0;
    PVR_TICK_INDEX_T*   pt_tick_idx = NULL;
    UINT32              ui4_temp = 0;

    ps_name = "main_svctx";

    if (x_handle_valid(h_g_main_svctx) == TRUE)
    {
        x_svctx_close(h_g_main_svctx);
    }

    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE,
                           gc_time_shift_file_set[1],
                           FM_READ_ONLY,
                           FM_MODE_USR_READ,
                           FALSE,
                           &gh_time_shift_tick_detail);
    if (i4_ret != FMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    else
    {
        i4_ret = x_fm_mfw_get_info_by_handle(gh_time_shift_tick_detail, &t_file_info);
        if (i4_ret != FMR_OK || t_file_info.ui8_size == 0)
        {
            _time_shift_free();
            return CLIR_CMD_EXEC_ERROR;
        }
        gpui1_time_shift_tick_detail =  x_mem_alloc(t_file_info.ui8_size/* + 31*/);
        /*pui1_tick_detail = (UINT8*)(((UINT32)gpui1_time_shift_tick_detail + 31) & 0xFFFFFFE0);*/
        pui1_tick_detail = gpui1_time_shift_tick_detail;
        if(pui1_tick_detail == NULL)
        {
           _time_shift_free();
           return CLIR_CMD_EXEC_ERROR;
        }
        i4_ret = x_fm_mfw_read(gh_time_shift_tick_detail,
                               pui1_tick_detail,
                               (UINT32)t_file_info.ui8_size,
                               &ui4_read);
        if (i4_ret != FMR_OK || ui4_read != (UINT32)t_file_info.ui8_size)
        {
            _time_shift_free();
            return CLIR_CMD_EXEC_ERROR;
        }
        x_fm_mfw_close(gh_time_shift_tick_detail);
        gh_time_shift_tick_detail = NULL_HANDLE;
    }

    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE,
                           gc_time_shift_file_set[2],
                           FM_READ_ONLY,
                           FM_MODE_USR_READ,
                           FALSE,
                           &gh_time_shift_tick_index);
    if (i4_ret != FMR_OK)
    {
        _time_shift_free();
        return CLIR_CMD_EXEC_ERROR;
    }
    else
    {
        i4_ret = x_fm_mfw_get_info_by_handle(gh_time_shift_tick_index, &t_file_info);
        if (i4_ret != FMR_OK || t_file_info.ui8_size == 0)
        {
            _time_shift_free();
            return CLIR_CMD_EXEC_ERROR;
        }
        gpui1_time_shift_tick_index =  x_mem_alloc(t_file_info.ui8_size/* + 31*/);
        /*pui1_tick_index = (UINT8*)(((UINT32)gpui1_time_shift_tick_index + 31) & 0xFFFFFFE0);*/
        pui1_tick_index = gpui1_time_shift_tick_index;
        if(pui1_tick_index == NULL)
        {
           _time_shift_free();
           return CLIR_CMD_EXEC_ERROR;
        }
        i4_ret = x_fm_mfw_read(gh_time_shift_tick_index,
                               pui1_tick_index,
                               (UINT32)t_file_info.ui8_size,
                               &ui4_read);
        if (i4_ret != FMR_OK || ui4_read != (UINT32)t_file_info.ui8_size)
        {
            _time_shift_free();
            return CLIR_CMD_EXEC_ERROR;
        }
        x_fm_mfw_close(gh_time_shift_tick_index);
        gh_time_shift_tick_index = NULL_HANDLE;

        t_tick_cb.pv_start_address = (VOID*)pui1_tick_index;
        t_tick_cb.pv_end_address = (VOID*)((UINT32)t_tick_cb.pv_start_address +
                                           (UINT32)t_file_info.ui8_size -
                                           (UINT32)sizeof(PVR_TICK_INDEX_T));
        t_tick_cb.ui4_entry_num = (UINT32)t_file_info.ui8_size / sizeof(PVR_TICK_INDEX_T);
        gui4_time_shift_tick_idx_total = t_tick_cb.ui4_entry_num;
        x_dbg_stmt("[Timeshift MM] tick index total number is %d\r\n", gui4_time_shift_tick_idx_total);

        pt_tick_idx = (PVR_TICK_INDEX_T*)pui1_tick_index;
        while (ui4_i < t_tick_cb.ui4_entry_num)
        {
            ui4_temp = (UINT32)pt_tick_idx->pt_tick_detail + (UINT32)pui1_tick_detail;
            pt_tick_idx->pt_tick_detail = (PVR_TICK_DETAIL_T*)ui4_temp;
            pt_tick_idx += ++ui4_i;
        }
    }

    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE,
                           gc_time_shift_file_set[0],
                           FM_READ_ONLY,
                           FM_MODE_USR_READ,
                           FALSE,
                           &h_file);
    if (i4_ret != FMR_OK)
    {
        _time_shift_free();
        return CLIR_CMD_EXEC_ERROR;
    }
    else
    {
        i4_ret = x_fm_mfw_get_info_by_handle(h_file, &t_file_info);
        t_tick_cb.ui4_fifo_pkt_num = (UINT32)t_file_info.ui8_size / 192;
        x_fm_mfw_close(h_file);
        h_file = NULL_HANDLE;
        if (i4_ret != FMR_OK || t_file_info.ui8_size == 0)
        {
            _time_shift_free();
            return CLIR_CMD_EXEC_ERROR;
        }
    }

    t_tick_cb.ui4_entry_size = 16;
    t_tick_cb.ui4_tick_period = 500;
    t_tick_cb.z_dev_access_unit = (SIZE_T)2048;

    i4_ret = x_svctx_open(ps_name, &h_g_main_svctx);
    if (i4_ret != SVCTXR_OK) {
        _time_shift_free();
        x_dbg_stmt("x_svctx_open(%s) failed, i4_ret = %d\r\n", ps_name, i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    t_cmd[0].e_code = MM_SVC_CMD_CODE_SRC_TYPE;
    t_cmd[0].u.ui1_data = SRC_TYPE_MEDIA_TIME_SHIFT;
    t_cmd[1].e_code = MM_SVC_CMD_CODE_MEDIA_PATH;
    t_cmd[1].u.ps_text = gc_time_shift_file_set[0];
    t_cmd[2].e_code = MM_SVC_CMD_CODE_TIME_SHIFT_UNIT_TEST;
    t_cmd[2].u.pv_data = &t_tick_cb;

    t_cmd[3].e_code = MM_SVC_CMD_CODE_END;
    i4_ret = x_svctx_media_open(
                 h_g_main_svctx,
                 t_cmd,
                 _mm_hdlr_open_media_nfy,
                 NULL,
                 &h_g_media
                 );
    if(i4_ret !=  SVCTXR_OK)
    {
        _time_shift_free();
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}

INT32 _mm_hdlr_close_time_shift(INT32 i4_argc, const CHAR** pps_argv)
{
    _time_shift_free();
    return _mm_hdlr_close_media(i4_argc, pps_argv);
}
#endif /* MM_TIME_SHIFT_UNIT_TEST */

/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_stop_media
 *
 * Description: This API open a multimedia object.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
INT32 _mm_hdlr_stop_media(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;

    x_dbg_stmt("%s()\r\n", __func__);

    if (x_handle_valid(h_g_main_svctx) == FALSE) {
        i4_ret = x_svctx_open("main_svctx", &h_g_main_svctx);
        if (i4_ret != SVCTXR_OK) {
            x_dbg_stmt("x_svctx_open(main_svctx) failed, i4_ret = %d\r\n", i4_ret);
            return CLIR_OK;
        }
    }

    i4_ret = x_svctx_sync_stop_svc(h_g_main_svctx, 0);
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_sync_stop_svc(main_svctx) failed, i4_ret = %d\r\n", i4_ret);
    }


    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_get_raw_data
 *
 * Description: This API get minfo raw data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get_raw_data(INT32 i4_argc, const CHAR** pps_argv)
{
    MINFO_TYPE_RAW_DATA_T       t_minfo_raw_data;
    SIZE_T                      ui4_size = 0;
    INT32                       i4_ret = 0;
    UINT32                      ui4_i = 0;
    UCHAR*                      pt_buf = NULL;

    ui4_size = sizeof(MINFO_TYPE_RAW_DATA_T);
    x_memset(&t_minfo_raw_data, 0, sizeof(MINFO_TYPE_RAW_DATA_T));
    t_minfo_raw_data.ui8_seek_pos = 5;
    t_minfo_raw_data.z_read_data_size = 10;
    t_minfo_raw_data.pv_get_info = x_mem_alloc(5);
    if (t_minfo_raw_data.pv_get_info == NULL)
    {
        return CLIR_OUT_OF_MEM;
    }
    
    i4_ret = x_svctx_media_get_info(h_g_main_svctx, h_g_media, SVCTX_MM_GET_TYPE_RAW_DATA, (VOID*)&t_minfo_raw_data, ui4_size);
    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt("\n------ _mm_hdlr_get_raw_data error! ------\n");
        return i4_ret;
    }
    pt_buf = t_minfo_raw_data.pv_get_info;
    for (ui4_i = 0; ui4_i < t_minfo_raw_data.z_read_data_size; ui4_i ++)
    {
        x_dbg_stmt("%02x ", pt_buf[ui4_i]);
    }

    x_mem_free(t_minfo_raw_data.pv_get_info);
    
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_get_tc
 *
 * Description: This API open a multimedia object.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
INT32 _mm_hdlr_get_tc(INT32 i4_argc, const CHAR** pps_argv)
{
    MM_SVC_TIME_CODE_INFO_T t_tc_info;
    SIZE_T                  z_size;

    t_tc_info.t_tm_code_type = MM_SVC_TM_CODE_TYPE_ALL;
    t_tc_info.ui2_idx = 0;
    z_size = sizeof(t_tc_info);
    x_svctx_get(h_g_main_svctx,
                SVCTX_MM_GET_TYPE_TIME_CODE,
                &t_tc_info,
                &z_size);

    x_dbg_stmt("TIME CODE, i4_ret = %d\r\n", t_tc_info.ui8_time);
    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_get_dur
 *
 * Description: This API open a multimedia object.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
INT32 _mm_hdlr_get_dur(INT32 i4_argc, const CHAR** pps_argv)
{
    MM_SVC_DUR_INFO_T t_dur_info;
    SIZE_T            z_size;
    t_dur_info.t_dur_type = MM_SVC_DUR_TYPE_ALL;
    t_dur_info.ui2_idx = 0;
    z_size =  sizeof(t_dur_info);
    x_svctx_get(h_g_main_svctx,
                SVCTX_MM_GET_TYPE_DUR,
                &t_dur_info,
                &z_size);
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_media_select_svc
 *
 * Description: This API open a multimedia object.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
INT32 _mm_hdlr_media_select_svc(INT32 i4_argc, const CHAR** pps_argv)
{
    SRC_DESC_T          t_src_desc;
    SNK_DESC_T          t_snk_desc;
    INT32               i4_ret;
    HANDLE_T            h_screen        = NULL_HANDLE;
    HANDLE_T            h_plane         = NULL_HANDLE;
    HANDLE_T            h_canvas        = NULL_HANDLE;
    BOOL                b_with_st       = FALSE;
    UINT32              ui4_stream_mode = ST_MASK_AUDIO|ST_MASK_VIDEO;
    VSH_REGION_INFO_T   t_video_region  = {0};
    VSH_REGION_INFO_T   t_disp_region   = {0};

    if (h_g_media == NULL_HANDLE )
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    if (i4_argc > 1)
    {
        if ( x_strcmp( pps_argv[1], "st" ) == 0 )
        {
            b_with_st = TRUE;
        }
        else
        {
            x_dbg_stmt("\r\nUsage: mss [st]\r\n");
        }
    }
    x_memset(&t_src_desc, 0, sizeof(SRC_DESC_T));
    x_memset(&t_snk_desc, 0, sizeof(SNK_DESC_T));
    
    t_src_desc.ui2_svc_lst_id        = 0;/*don't care in case of non-TS file*/
    t_src_desc.ui2_svl_rec_id        = 0;/*don't care in case of non-TS file*/
    t_src_desc.e_type                = SRC_TYPE_MEDIA_STORGE;
    t_src_desc.ps_conn_src_type      = NULL;
    t_src_desc.ps_conn_src_name      = SN_MAIN_SVL;
    t_src_desc.u_info.t_mm.h_media   = h_g_media;

    t_video_region.ui4_x = 0;
    t_video_region.ui4_y = 0;
    t_video_region.ui4_width  = VSH_REGION_MAX_WIDTH;
    t_video_region.ui4_height = VSH_REGION_MAX_HEIGHT;

    t_disp_region.ui4_x = 0;
    t_disp_region.ui4_y = 0;
    t_disp_region.ui4_width  = VSH_REGION_MAX_WIDTH;
    t_disp_region.ui4_height = VSH_REGION_MAX_HEIGHT;

    t_snk_desc.pt_video_region      = &t_video_region;
    t_snk_desc.pt_disp_region       = &t_disp_region;
    t_snk_desc.pt_video_plane_order = NULL;
    t_snk_desc.ps_snk_grp_name      = SN_PRES_MAIN_DISPLAY;
    t_snk_desc.pt_audio_strm_fltr_fct = NULL;
    t_snk_desc.pv_audio_strm_fltr_tag = NULL;
    t_snk_desc.pt_video_strm_fltr_fct = NULL;
    t_snk_desc.pv_video_strm_fltr_tag = NULL;

    if (b_with_st)
    {
#ifndef ANDROID
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
                return CLIR_CMD_EXEC_ERROR;
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
               return CLIR_CMD_EXEC_ERROR;
           }
           /*Link the plane*/
           i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
           if (GL_FAILED(i4_ret))
           {
               x_dbg_stmt("Can't link plane\n");
               x_handle_free(h_screen);
               x_handle_free(h_plane);
               return CLIR_CMD_EXEC_ERROR;
           }
        }
        if (NULL_HANDLE == h_canvas)
        {
            x_wgl_get_canvas(h_plane, &h_canvas);
        }
#endif        
        t_snk_desc.u_text_strm.t_subtitle.h_gl_plane = h_canvas;
        ui4_stream_mode |= ST_MASK_SUBTITLE;
    }

    i4_ret = x_svctx_select_svc(
                 h_g_main_svctx,
                 ui4_stream_mode,
                 &t_src_desc,
                 &t_snk_desc,
                 _mm_hdlr_media_select_svc_nfy,
                 (VOID*)h_g_media);
    if (i4_ret != SVCTXR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}

INT32 _mm_hdlr_media_select_stream(INT32 i4_argc, const CHAR** pps_argv)
{

     STREAM_COMP_ID_T         t_comp_id;
     UINT8                    ui1_idx = 0;
     INT32                    i4_ret;
     SVCTX_T*                 pt_svctx = NULL;
     UINT32                   ui4_db_version     = SCDB_NULL_VER_ID;
     UINT16                   ui2_num_recs       = 0;
     MM_SVC_REQ_T*            pt_running_svc_req;
     STREAM_TYPE_T            e_strm_type = ST_AUDIO;
     STREAM_COMP_ID_T*        pt_comp_id = NULL;
     SCDB_REC_T               t_scdb_rec;

     i4_ret = svctx_get_obj(h_g_main_svctx, &pt_svctx);

     if (i4_ret != SVCTXR_OK) {
           return CLIR_CMD_EXEC_ERROR;
            }

     if (x_strcmp("1", pps_argv[1]) == 0) {
       ui1_idx = 1;
     }
     if (x_strcmp("2", pps_argv[1]) == 0) {
       ui1_idx  = 2;
     }
     if (x_strcmp("3", pps_argv[1]) == 0) {
        ui1_idx = 3;
     }
     if (x_strcmp("4", pps_argv[1]) == 0) {
        ui1_idx = 4;
     }
     if (x_strcmp("5", pps_argv[1]) == 0) {
        ui1_idx = 5;
     }
     if (x_strcmp("6", pps_argv[1]) == 0) {
        ui1_idx = 6;
     }
     if (x_strcmp("7", pps_argv[1]) == 0) {
        ui1_idx = 7;
     }
     if (x_strcmp("8", pps_argv[1]) == 0) {
        ui1_idx = 8;
     }

     if (x_strcmp("a", pps_argv[2]) == 0) {
       e_strm_type = ST_AUDIO;
     }
     else
     if (x_strcmp("s", pps_argv[2]) == 0) {
       e_strm_type = ST_SUBTITLE;
     }



     pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
     i4_ret = x_scdb_get_num_recs(
                   pt_running_svc_req->pt_player->h_scdb,
                   e_strm_type,
                   &ui2_num_recs,
                   &ui4_db_version
                   );

     if (i4_ret != SCDBR_OK) {
         DBG_ERROR((_ERROR_HEADER"x_scdb_get_num_recs() failed, return = %d\r\n", i4_ret));
         return CLIR_CMD_EXEC_ERROR;
     }

     i4_ret = x_scdb_get_rec_by_idx(
                     pt_running_svc_req->pt_player->h_scdb,
                     e_strm_type,
                     ui1_idx,
                     &t_comp_id,
                     &t_scdb_rec,
                     &ui4_db_version
                     );
     if (i4_ret == SCDBR_OK) {
           x_dbg_stmt("PlayStream , i4_ret = %d\r\n", (INT32)pt_comp_id->pv_stream_tag);
     }

#if 0
     if (x_strcmp("1", pps_argv[1]) == 0) {
       ui1_idx = 1;
     }
     if (x_strcmp("2", pps_argv[1]) == 0) {
       ui1_idx  = 2;
     }
     if (x_strcmp("3", pps_argv[1]) == 0) {
        ui1_idx = 3;
     }
     if (x_strcmp("4", pps_argv[1]) == 0) {
        ui1_idx = 4;
     }
     if (x_strcmp("5", pps_argv[1]) == 0) {
        ui1_idx = 5;
     }
     if (x_strcmp("6", pps_argv[1]) == 0) {
        ui1_idx = 6;
     }
     if (x_strcmp("7", pps_argv[1]) == 0) {
        ui1_idx = 7;
     }
     if (x_strcmp("8", pps_argv[1]) == 0) {
        ui1_idx = 8;
     }
     t_comp_id.e_type = ST_AUDIO;
     t_comp_id.pv_stream_tag = (VOID*)ui1_idx;
#endif
     i4_ret = x_svctx_select_stream(
              h_g_main_svctx,
              &t_comp_id,
              _mm_hdlr_media_select_svc_nfy,
             (VOID*)h_g_media);

     if (i4_ret != SVCTXR_OK)
     {
        return CLIR_CMD_EXEC_ERROR;
     }
    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_media_select_svc
 *
 * Description: This API open a multimedia object.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
INT32 _mm_hdlr_media_trick(INT32 i4_argc, const CHAR** pps_argv)
{
    MM_SVC_TRICK_INFO_T t_trk_info;
    HANDLE_T            h_svctx = h_g_main_svctx;

    if (i4_argc < 2) {
        x_dbg_stmt("Syntax Error!\r\n");
        return CLIR_OK;
    }
    else if (i4_argc == 3)
    {
        h_svctx = (HANDLE_T)_atoi(pps_argv[2]);
    }

    /*support pause & resume*/
    if (x_strcmp("p", pps_argv[1]) == 0) {
      t_trk_info.e_speed_type = MM_SPEED_TYPE_PAUSE;
    } else if (x_strcmp("f1x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
    } else if (x_strcmp("f2x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_FORWARD_2X;
    } else if (x_strcmp("f4x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_FORWARD_4X;
    }else if (x_strcmp("f8x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_FORWARD_8X;
    }else if (x_strcmp("f16x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_FORWARD_16X;
    }else if (x_strcmp("f32x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_FORWARD_32X;
    }else if (x_strcmp("b2x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_REWIND_2X;
    }else if (x_strcmp("b4x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_REWIND_4X;
    }else if (x_strcmp("b8x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_REWIND_8X;
    }else if (x_strcmp("b16x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_REWIND_16X;
    }else if (x_strcmp("b32x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_REWIND_32X;
    }else if (x_strcmp("sf", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_STEP;
    }else if (x_strcmp("f1/2x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1_DIV_2X;
    }else if (x_strcmp("f1/4x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1_DIV_4X;
    }else if (x_strcmp("f1/8x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1_DIV_8X;
    }else if (x_strcmp("f1/16x", pps_argv[1]) == 0) {
       t_trk_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1_DIV_16X;
    }
    else
    {
        x_dbg_stmt("Syntax Error! Stream type not support \r\n");
        return CLIR_OK;
    }

    x_svctx_set(h_svctx,
                SVCTX_MM_SET_TYPE_TRICK,
                &t_trk_info,
                sizeof(t_trk_info));
    return CLIR_OK;
}

INT32 _mm_hdlr_media_seek(INT32 i4_argc, const CHAR** pps_argv)
{

     MM_SVC_POS_INFO_T t_pos_info;
     const CHAR* s_str;
     CHAR* s_endptr;
     INT32 val;

     s_str = pps_argv[1];
     val = x_strtoll(s_str,&s_endptr,10);
 #if 1
     MM_SVC_DUR_INFO_T t_dur_info;
     SIZE_T            z_size;
     t_dur_info.t_dur_type = MM_SVC_DUR_TYPE_ALL;
     t_dur_info.ui2_idx = 0;
     t_dur_info.ui8_duration = 0;
     z_size =  sizeof(t_dur_info);
     x_svctx_get(h_g_main_svctx,
                     SVCTX_MM_GET_TYPE_DUR,
                     &t_dur_info,
                     &z_size);
     if((t_dur_info.ui8_duration == 0) ||
        (val >= (INT32)t_dur_info.ui8_duration) ||
        (val < 0))
     {
         x_dbg_stmt("Syntax Error! Make sure 0 <= %d(seek time) < %d(total play time)\n",
             val, (INT32)t_dur_info.ui8_duration);
         return CLIR_INV_ARG;
         //return CLIR_OK;
     }
 #endif
     if (s_endptr == s_str)
     {
        return CLIR_INV_ARG;

     }
     t_pos_info.t_pos_type = MM_SVC_POS_TYPE_ALL_TIME;
     t_pos_info.ui2_idx = 0;
     t_pos_info.ui8_pos_val = val;
     x_svctx_media_seek(h_g_main_svctx,
                       NULL,
                       NULL,
                       &t_pos_info
                       );
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_media_stress1
 *
 * Description: stress tese case 1.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
INT32 _mm_hdlr_media_stress_1(INT32 i4_argc, const CHAR** pps_argv)
{

    return CLIR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_media_stop_svc
 *
 * Description: This API open a multimedia object.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
INT32 _mm_hdlr_media_stop_svc(INT32 i4_argc, const CHAR** pps_argv)
{


    return CLIR_OK;

}
#ifdef TIME_MEASUREMENT
/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_cli_set_tms_level
 *
 * Description: This API sets the time measurement level of Service Context.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_cli_set_tms_level(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT32 i4_ret;
    UINT16 ui2_tms_level;

    i4_ret = x_cli_parse_tms_level(i4_argc, pps_argv, &ui2_tms_level);
    if ( i4_ret == CLIR_OK ) {
        svctx_set_tms_level(ui2_tms_level);
    }

    x_cli_show_tms_level( svctx_get_tms_level() );

    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_cli_get_tms_level
 *
 * Description: This API gets the current time measurement level of Service Context.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_cli_get_tms_level(INT32 i4_argc, const CHAR** pps_argv)
{
    return x_cli_show_tms_level(svctx_get_tms_level());
}
#endif /* TIME_MEASUREMENT */

static CHAR* gc_drm_test_file_tbl[10] =
{
    /* owner guard: 91 90 8f */
    "/mnt/usb/Mass-000/test/V05_HT_user1_type4_key5_serial0_slot0.avi",
    "/mnt/usb/Mass-000/test/V201_base.avi",
    "/mnt/usb/Mass-000/test/V202_purchase_user1.avi",
    "/mnt/usb/Mass-000/test/V203_rental_user1.avi",
    "/mnt/usb/Mass-000/test/V204_purchase_user2.avi",
    "/mnt/usb/Mass-000/test/V205_base_metadata_2.avi",
    "/mnt/usb/Mass-000/test/V212_HD_purchase_user1.divx",
    "/mnt/usb/Mass-000/test/V213_HD_rental_user1.divx",
    "/mnt/usb/Mass-000/test/V214_HD_purchase_user2.divx"
};

static INT32 _mm_hdlr_open_drm_media (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret  = CLIR_OK;
    UINT32 ui4_idx;
    MM_SVC_COMMAND_T  t_cmd[5];
    CHAR* ps_name = "main_svctx";

    ui4_idx = (UINT32)x_strtoll(pps_argv[1], NULL, 10);
    if (i4_argc != 2)
    {
        return CLIR_INV_ARG;
    }

    if (x_handle_valid(h_g_main_svctx) == TRUE)
    {
        x_svctx_close(h_g_main_svctx);
    }

    i4_ret = x_svctx_open(ps_name, &h_g_main_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt("x_svctx_open(%s) failed, i4_ret = %d\r\n", ps_name, i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    t_cmd[0].e_code = MM_SVC_CMD_CODE_SVL_NAME;
    t_cmd[0].u.ps_text = "MEDIA_SVC";
    t_cmd[1].e_code = MM_SVC_CMD_CODE_SVL_ID;
    t_cmd[1].u.ui2_data = 1;
    t_cmd[2].e_code = MM_SVC_CMD_CODE_SRC_TYPE;
    t_cmd[2].u.ui1_data = SRC_TYPE_MEDIA_STORGE;
    t_cmd[3].e_code = MM_SVC_CMD_CODE_MEDIA_PATH;
    t_cmd[3].u.ps_text = gc_drm_test_file_tbl[ui4_idx];
    t_cmd[4].e_code = MM_SVC_CMD_CODE_END;

    i4_ret = x_svctx_media_open(
                h_g_main_svctx,
                t_cmd,
                _mm_hdlr_open_media_nfy,
                NULL,
                &h_g_media
                );
    if (i4_ret !=  SVCTXR_OK)
    {
        x_dbg_stmt("x_svctx_media_open failed, i4_ret = %d\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}

static INT32 _mm_hdlr_close_drm_media (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret  = CLIR_OK;

    if (x_handle_valid(h_g_main_svctx)  && x_handle_valid(h_g_media))
    {
        i4_ret = x_svctx_media_close(
                    h_g_main_svctx,
                    h_g_media
                    );
        if (i4_ret !=  SVCTXR_OK)
        {
            x_dbg_stmt("x_svctx_media_close failed, i4_ret = %d\r\n", i4_ret);
            return CLIR_CMD_EXEC_ERROR;
        }

        i4_ret = x_svctx_close(h_g_main_svctx);
        if (i4_ret !=  SVCTXR_OK)
        {
            x_dbg_stmt("x_svctx_close failed, i4_ret = %d\r\n", i4_ret);
            return CLIR_CMD_EXEC_ERROR;
        }
    }

    return CLIR_OK;
}

static INT32 _mm_hdlr_getinfo_drm_media (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret  = CLIR_OK;
    DRM_INFO_SVCTX_T t_drm_info_svctx;

    i4_ret = x_svctx_media_get_info(
                        h_g_main_svctx,
                        h_g_media,
                        SVCTX_DRM_GET_TYPE_INFO,
                        &t_drm_info_svctx,
                        sizeof(DRM_INFO_SVCTX_T)
                        );
    if (i4_ret !=  SVCTXR_OK)
    {
        x_dbg_stmt("x_svctx_media_get_info failed, i4_ret = %d\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}

#if 0
static INT32 _mm_hdlr_setinfo_drm_media (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret  = CLIR_OK;

    i4_ret = x_svctx_media_set_info(
                        h_g_main_svctx,
                        h_g_media,
                        SVCTX_DRM_SET_TYPE_PLAYBACK,
                        NULL,
                        0
                        );
    if (i4_ret !=  SVCTXR_OK)
    {
        x_dbg_stmt("x_svctx_media_set_info failed, i4_ret = %d\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}
#endif

static INT32 _mm_hdlr_getinfo_generic_drm_media (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret  = CLIR_OK;
    CHAR ac_reg_code[11];

    i4_ret = x_svctx_generic_get_info(
                        h_g_main_svctx,
                        h_g_media,
                        SVCTX_GENGRIC_GET_TYPE_DIVX_DRM_REGISTRATION_CODE,
                        ac_reg_code,
                        11
                        );
    if (i4_ret !=  SVCTXR_OK)
    {
        x_dbg_stmt("x_svctx_generic_get_info failed, i4_ret = %d\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}

static GL_HSCREEN_T        _h_screen         = NULL_HANDLE;
static GL_HSURFACE_T       _h_disp_surf      = NULL_HANDLE;
static GL_HPLANE_T         _h_plane          = NULL_HANDLE;
static HANDLE_T            _h_view           = NULL_HANDLE;
static UINT32              _ui4_viewport     = 0;
static GL_DISPLAY_OPT_T    _t_disp_opt;
static UINT32              _ui4_panel_width  = 0;
static UINT32              _ui4_panel_height = 0;

static BOOL                _b_thumbnail_stress_done = TRUE;
static UINT32              _ui4_thumbnail_stress_window = 0;
HANDLE_T                   _h_canvas;
HANDLE_T                   _h_rgn_list;   

static VOID _gl_obj_free(VOID)
{
#ifndef ANDROID
    INT32   i4_ret;

    if (_h_rgn_list)
    {
        x_wgl_delete_transparent_list(_h_rgn_list);
        _h_rgn_list = NULL_HANDLE;
    }
    
    if (_h_plane)
    {
        i4_ret = x_gl_plane_flip(_h_plane, NULL_HANDLE, TRUE);

        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("flip failed. (%d)\r\n", i4_ret);
        }

        _h_plane = NULL_HANDLE;
    }

    if (_h_view)
    {
        i4_ret = x_gl_view_delete_viewport(_h_view, _ui4_viewport);

        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("viewport delete failed. (%d)\r\n", i4_ret);
        }

        i4_ret = x_gl_obj_free(_h_view);

        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("view delete failed. (%d)\r\n", i4_ret);
        }

        _h_view = NULL_HANDLE;
    }

    if (_h_disp_surf)
    {
        i4_ret = x_gl_obj_free(_h_disp_surf);

        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("surface delete failed. (%d)\r\n", i4_ret);
        }

        _h_disp_surf = NULL_HANDLE;
    }

    if (_h_plane)
    {
        i4_ret = x_gl_obj_free(_h_plane);

        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("plane delete failed. (%d)\r\n", i4_ret);
        }

        _h_plane = NULL_HANDLE;
    }

    if (_h_screen)
    {
        i4_ret = x_gl_obj_free(_h_screen);

        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("screen delete failed. (%d)\r\n", i4_ret);
        }

        _h_screen = NULL_HANDLE;
    }
#endif    
}

static INT32 _mm_hdlr_create_surface (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret = 0;
    UINT16 ui2_plane_idx = 2;
#ifndef ANDROID
    GL_COLORMODE_T  e_clrmode = GL_COLORMODE_AYUV_D8888;
    GL_SURFACE_DESC_T t_desc;
    GL_COLOR_T t_color;

    if (i4_argc == 2 || i4_argc == 3)
    {
        e_clrmode = (GL_COLORMODE_T)(UINT32)_atoi(pps_argv[1]);

        if (i4_argc == 3)
        {
            ui2_plane_idx = (UINT16)_atoi(pps_argv[2]);
        }
    }
    else if (i4_argc != 1)
    {
        x_dbg_stmt("\r\nUsage: cs [color mode] [plane index]\r\n");

        return CLIR_INV_ARG;
    }

    /* open screen related */
    i4_ret = x_gl_screen_open(SN_PRES_MAIN_DISPLAY, NULL, NULL, &_h_screen);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("screen create failed. (%d)\r\n", i4_ret);

        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_gl_screen_get_panel_size(_h_screen, &_ui4_panel_width, &_ui4_panel_height);
    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("panel size get failed. (%d)\r\n", i4_ret);
        _gl_obj_free();

        return CLIR_CMD_EXEC_ERROR;
    }

    t_desc.e_clrmode  = e_clrmode;
    t_desc.h_clipper  = NULL_HANDLE;
    t_desc.h_palette  = NULL_HANDLE;
    t_desc.ui4_width  = _ui4_panel_width;
    t_desc.ui4_height = _ui4_panel_height;

    i4_ret = x_gl_surface_create(&t_desc, &_h_disp_surf);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("surface create failed. (%d)\r\n", i4_ret);
        _gl_obj_free();

        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_gl_plane_open(_h_screen, ui2_plane_idx, &_h_plane);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("plane create failed. (%d)\r\n", i4_ret);
        _gl_obj_free();

        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_gl_plane_link(_h_plane, _h_screen, TRUE);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("plane link failed. (%d)\r\n", i4_ret);
        _gl_obj_free();

        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_gl_view_create(_ui4_panel_width, _ui4_panel_height, FALSE, &_h_view);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("view create failed. (%d)\r\n", i4_ret);
        _gl_obj_free();

        return CLIR_CMD_EXEC_ERROR;
    }

    /* viewport */
    _t_disp_opt.e_flag         = GL_DISP_OPT_POSITION | GL_DISP_OPT_BLEND_OPTION/* | GL_DISP_OPT_RESIZE | GL_DISP_OPT_SURF_OFFSET*/;
    _t_disp_opt.t_pos.i4_x     = 0;
    _t_disp_opt.t_pos.i4_y     = 0;
    _t_disp_opt.e_blend_option = GL_BLEND_OPT_NONE;
    _t_disp_opt.ui1_alpha      = 255;

    i4_ret = x_gl_view_create_viewport(_h_view, _h_disp_surf, &_t_disp_opt, &_ui4_viewport);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("viewport create failed. (%d)\r\n", i4_ret);
        _gl_obj_free();

        return CLIR_CMD_EXEC_ERROR;
    }

    t_color.u1.y = 128;
    t_color.u2.u = 128;
    t_color.u3.v = 128;
    t_color.a    = 255;

    x_gl_surface_fill(_h_disp_surf, &t_color, GL_SYNC);

    i4_ret = x_gl_plane_flip(_h_plane, _h_view, TRUE);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("flip failed. (%d)\r\n", i4_ret);
        _gl_obj_free();

        return CLIR_CMD_EXEC_ERROR;
    }
#endif
    return CLIR_OK;
}

static INT32 _mm_hdlr_delete_surface (INT32 i4_argc, const CHAR** pps_argv)
{
    _gl_obj_free();

    return CLIR_OK;
}

static VOID _mm_hdlr_media_load_thumbnail_nfy_fct(
                    HANDLE_T                    h_svctx,
                    SVCTX_COND_T                e_nfy_cond,
                    SVCTX_NTFY_CODE_T           e_code,
                    STREAM_TYPE_T               e_stream_type,
                    VOID*                       pv_nfy_tag
                    )
{
     x_dbg_stmt("\r\n%s() stream_type=%s, e_code=%s, e_nfy_cond=%s\r\n",
                     __func__,
                     s_g_strm_types[e_stream_type],
                     s_g_svctx_ntfy_codes[e_code],
                     s_g_state_strings[e_nfy_cond]
                     );
     if (e_code == SVCTX_NTFY_CODE_MEDIA_THUMBNAIL_DONE)
     {
         _b_thumbnail_stress_done = TRUE;
     }
}

static INT32 _mm_hdlr_load_thumbnail (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret = 0;
    SVCTX_VIDEO_THUMBNAIL_INFO_T t_thumbnail_info = {0};
    UINT64 ui8_pic_idx_dft = 10;
    UINT32 ui4_thumbnail_x_dft = 50;
    UINT32 ui4_thumbnail_y_dft = 50;
    UINT32 ui4_thumbnail_width_dft = 288;
    UINT32 ui4_thumbnail_height_dft = 192;
    UINT32 ui4_window_idx = 1;
    UINT32 ui4_window_idx_c = 1;
    UINT32 ui4_window_idx_r = 1;
    UINT32 ui4_max_window_per_row = 4;

    if (i4_argc == 2)
    {
        ui8_pic_idx_dft = (UINT64)_atoi(pps_argv[1]);
    }
    else if (i4_argc == 3)
    {
        ui8_pic_idx_dft = (UINT64)_atoi(pps_argv[1]);
        ui4_window_idx = (UINT32)_atoi(pps_argv[2]);
        ui4_window_idx_r = (ui4_window_idx - 1) / ui4_max_window_per_row + 1;
        ui4_window_idx_c = ui4_window_idx - ui4_max_window_per_row * (ui4_window_idx_r - 1);
        if (ui4_window_idx >= 1 && ui4_window_idx <= 12)
        {
            ui4_thumbnail_x_dft = ui4_thumbnail_x_dft * ui4_window_idx_c +
                                  ui4_thumbnail_width_dft * (ui4_window_idx_c - 1);
            ui4_thumbnail_y_dft = ui4_thumbnail_y_dft * ui4_window_idx_r +
                                  ui4_thumbnail_height_dft * (ui4_window_idx_r - 1);
        }
        else
        {
            x_dbg_stmt("\r\nUsage: lt [picture index] [window index]\r\n");
            x_dbg_stmt("\r\ninvalid argument! [window index] should between 1~12\r\n");

            return CLIR_INV_ARG;
        }
    }
    else if (i4_argc == 6)
    {

        ui8_pic_idx_dft = (UINT64)_atoi(pps_argv[1]);
        ui4_thumbnail_x_dft = (UINT32)_atoi(pps_argv[2]);
        ui4_thumbnail_y_dft = (UINT32)_atoi(pps_argv[3]);
        ui4_thumbnail_width_dft = (UINT32)_atoi(pps_argv[4]);
        ui4_thumbnail_height_dft = (UINT32)_atoi(pps_argv[5]);
    }
    else if (i4_argc != 1)
    {
        x_dbg_stmt("\r\nUsage: lt [picture index] [window index]/[x] [y] [width] [height]\r\n");

        return CLIR_INV_ARG;
    }

    t_thumbnail_info.ui8_pic_idx = ui8_pic_idx_dft;
    t_thumbnail_info.ui4_thumbnail_x = ui4_thumbnail_x_dft;
    t_thumbnail_info.ui4_thumbnail_y = ui4_thumbnail_y_dft;
    t_thumbnail_info.ui4_thumbnail_width = ui4_thumbnail_width_dft;
    t_thumbnail_info.ui4_thumbnail_height = ui4_thumbnail_height_dft;
    if ((t_thumbnail_info.ui4_thumbnail_x + t_thumbnail_info.ui4_thumbnail_width) >= (_ui4_panel_width) ||
        (t_thumbnail_info.ui4_thumbnail_y + t_thumbnail_info.ui4_thumbnail_height) >= (_ui4_panel_height))
    {
        x_dbg_stmt("invalid thumbnail [x]/[y]/[width]/[height].\r\n");

        return CLIR_INV_ARG;
    }

    t_thumbnail_info.h_surf = _h_disp_surf;
    t_thumbnail_info.pf_nfy = _mm_hdlr_media_load_thumbnail_nfy_fct;
    t_thumbnail_info.pv_tag = NULL;

    x_svctx_media_load_thumbnail(h_g_main_svctx, h_g_media, &t_thumbnail_info);
    if (i4_ret != SVCTXR_ASYNC_NFY && i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt("load thumbnail failed. (%d)\r\n", i4_ret);
        _gl_obj_free();

        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}

static INT32 _mm_hdlr_load_thumbnail_stress_test (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret = 0;
    INT32 i4_argc_l = 3;
    CHAR* pps_argv_l[3];

    if (!_b_thumbnail_stress_done)
    {
        return CLIR_OK;
    }

    switch (++_ui4_thumbnail_stress_window)
    {
    case 1:
        i4_ret = _mm_hdlr_create_surface(1, NULL);
        if (i4_ret != CLIR_OK)
        {
            _ui4_thumbnail_stress_window = 0;
            return CLIR_CMD_EXEC_ERROR;
        }
        i4_argc_l = 1;
        break;
    case 2:
        pps_argv_l[1] = "30";
        pps_argv_l[2] = "2";
        break;
    case 3:
        pps_argv_l[1] = "60";
        pps_argv_l[2] = "3";
        break;
    case 4:
        pps_argv_l[1] = "120";
        pps_argv_l[2] = "4";
        break;
    case 5:
        pps_argv_l[1] = "240";
        pps_argv_l[2] = "5";
        break;
    case 6:
        pps_argv_l[1] = "480";
        pps_argv_l[2] = "6";
        break;
    case 7:
        pps_argv_l[1] = "600";
        pps_argv_l[2] = "7";
        break;
    case 8:
        pps_argv_l[1] = "720";
        pps_argv_l[2] = "8";
        break;
    case 9:
        pps_argv_l[1] = "900";
        pps_argv_l[2] = "9";
        break;
    case 10:
        pps_argv_l[1] = "1050";
        pps_argv_l[2] = "10";
        break;
    case 11:
        pps_argv_l[1] = "1200";
        pps_argv_l[2] = "11";
        break;
    case 12:
        pps_argv_l[1] = "1500";
        pps_argv_l[2] = "12";
        break;
    default:
        _mm_hdlr_delete_surface(1, NULL);
        _ui4_thumbnail_stress_window = 0;
        return CLIR_OK;
    }

    _b_thumbnail_stress_done = FALSE;
    i4_ret = _mm_hdlr_load_thumbnail(i4_argc_l, (const CHAR**)pps_argv_l);
    if (i4_ret != CLIR_OK)
    {
        _b_thumbnail_stress_done = TRUE;
    }
    return i4_ret;
}

static INT32 _mm_hdlr_sync_cancel_load_thumbnail (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret = 0;

    if (h_g_main_svctx != NULL_HANDLE && h_g_media != NULL_HANDLE)
    {
        i4_ret = x_svctx_media_sync_cancel_load_thumbnail(h_g_main_svctx, h_g_media);
        if (i4_ret != SVCTXR_OK)
        {
            x_dbg_stmt("sync cancel load thumbnail failed. (%d)\r\n", i4_ret);

            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("no valid svctx & media handles exist.\r\n");
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mm_hdlr_load_preview_thumbnail
 *
 * Description: This function loading preview thumbnail
 *
 * Inputs:  i4_argc  
 *          pps_argv     
 *
 * Returns: 

 * Author : lianming lin(mcnmtk40234)
 *
 * History:
    (1)2009-7-29 : initial
 ----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_load_preview_thumbnail (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                         i4_ret         = 0;
    VSH_REGION_INFO_T             t_video_region = {0};
    VSH_REGION_INFO_T             t_disp_region  = {0};
    VSH_SET_PLANE_ORDER_INFO_T    t_video_plane_order;
    SRC_DESC_T                    t_src_desc     = {0};
    SNK_DESC_T                    t_snk_desc     = {0};
    SRC_MM_INFO_T                 t_mm           = {0};

    x_dbg_stmt("%s()\r\n", __func__);
    
    if (h_g_media == NULL_HANDLE)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    if ((i4_argc != 5) && (i4_argc != 1))
    {
        x_dbg_stmt("Usage: lpt ui4_x ui4_y ui4_width ui4_height\r\n");
        return CLIR_INV_ARG;
    }
    else if (i4_argc == 5)
    {
        t_g_disp_region.ui4_x      =  _atoi(pps_argv[1]);
        t_g_disp_region.ui4_y      =  _atoi(pps_argv[2]);
        t_g_disp_region.ui4_width  =  _atoi(pps_argv[3]);
        t_g_disp_region.ui4_height =  _atoi(pps_argv[4]);
    }
    else
    {
        t_g_disp_region.ui4_x      =  500;
        t_g_disp_region.ui4_y      =  500;
        t_g_disp_region.ui4_width  =  2000;
        t_g_disp_region.ui4_height =  2000;
    }
    
    t_video_region.ui4_x      = 0;
    t_video_region.ui4_y      = 0;
    t_video_region.ui4_width  = VSH_REGION_MAX_WIDTH;
    t_video_region.ui4_height = VSH_REGION_MAX_HEIGHT;

    t_disp_region.ui4_x      = t_g_disp_region.ui4_x;
    t_disp_region.ui4_y      = t_g_disp_region.ui4_y;
    t_disp_region.ui4_width  = t_g_disp_region.ui4_width;
    t_disp_region.ui4_height = t_g_disp_region.ui4_height;

    t_video_plane_order.e_order_ctrl     = VSH_PLANE_ORDER_CTRL_BOTTOM;
    t_video_plane_order.u.ui1_num_layers = 0;

    t_mm.h_media                = h_g_media;
    t_mm.t_speed                = MM_SPEED_TYPE_FORWARD_1X;
    t_mm.t_pos_info.t_pos_type  = MM_SVC_POS_TYPE_ALL_TIME;
    t_mm.t_pos_info.ui2_idx     = (UINT16)0;
    t_mm.t_pos_info.ui8_pos_val = 0;

    t_src_desc.ui2_svc_lst_id        = 0; /*don't care in case of non-TS file*/
    t_src_desc.ui2_svl_rec_id        = 0; /*don't care in case of non-TS file*/
    t_src_desc.e_type                = SRC_TYPE_MEDIA_STORGE;
    t_src_desc.ps_conn_src_type      = NULL;
    t_src_desc.ps_conn_src_name      = SN_MAIN_SVL;
    t_src_desc.u_info.t_mm           = t_mm;

    t_snk_desc.pt_video_region        = &t_video_region;
    t_snk_desc.pt_disp_region         = &t_disp_region;
    t_snk_desc.pt_video_plane_order   = &t_video_plane_order;
    t_snk_desc.pt_video_strm_fltr_fct = NULL;
    t_snk_desc.pv_video_strm_fltr_tag = NULL;
    t_snk_desc.pt_audio_strm_fltr_fct = NULL;
    t_snk_desc.pv_audio_strm_fltr_tag = NULL;
    t_snk_desc.ps_snk_grp_name        = SN_PRES_MAIN_DISPLAY;

    i4_ret = x_svctx_select_svc(h_g_main_svctx,
                                ST_MASK_VIDEO,
                                &t_src_desc,
                                &t_snk_desc,
                                _mm_hdlr_media_select_svc_nfy,
                                (VOID*)h_g_media);
    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt("select service error(%d)\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }
    b_g_thumbnail_previewing = TRUE;

    if (i4_ret == CLIR_OK)
    {
        x_dbg_stmt("loading preview thumbnail...\r\n");
    }
    
    return i4_ret;
}

UINT16 _get_default_osd_plane_index(VOID)
{
/* hard-code for temp solution. Weider Chang 7/8/2006*/
#ifdef APP_537X_SUPPORT
    return 1;
#else
    return 0;
#endif
}   

/*-----------------------------------------------------------------------------
 * Name:  _mm_hdlr_set_thumbnail_region_transprent
 *
 * Description: This function set thumbnail overlapped region transparent or not
 *
 * Inputs:  i4_argc  
 *          pps_argv     
 *
 * Returns: 

 * Author : lianming lin(mcnmtk40234)
 *
 * History:
    (1)2009-7-29 : initial
 ----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_set_thumbnail_region_transprent (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32              i4_ret;
    BOOL               b_transparent = TRUE;
    UINT16             ui2_plane_idx = 2;
#ifndef ANDROID    
    #if 0
    GL_COLORMODE_T     e_clrmode     = GL_COLORMODE_AYUV_D8888;
    GL_SURFACE_DESC_T  t_desc;
    GL_COLOR_T         t_color;
    #endif
    
    GL_RECT_T          at_rect[1];
    
    if (!b_g_thumbnail_previewing)
    { 
        x_dbg_stmt("not loading preview thumbnail...\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    if (i4_argc != 2)
    {
        x_dbg_stmt("Usage: strt 0/1\r\n");
        return CLIR_INV_ARG;
    }
    
    if (0 == x_strncmp(pps_argv[1], "0", (SIZE_T)1))
    {
        b_transparent = FALSE;
    }

    #if 1
    /*open screen*/
    if (NULL_HANDLE == _h_screen)
    {
        i4_ret = x_gl_screen_open(SN_PRES_MAIN_DISPLAY, NULL, NULL, &_h_screen);
        if(GLR_OK != i4_ret)
        {
            x_dbg_stmt("screen create failed. (%d)\r\n", i4_ret);
            
            return CLIR_CMD_EXEC_ERROR;
        }

        i4_ret = CLIR_OK;
        do
        {
            ui2_plane_idx = _get_default_osd_plane_index();
        
            /*init plane*/
            i4_ret = x_gl_plane_open(_h_screen, (UINT16)ui2_plane_idx, &_h_plane);
            if(GLR_OK != i4_ret)
            {
                x_dbg_stmt("plane create failed. (%d)\r\n", i4_ret);
                i4_ret = CLIR_CMD_EXEC_ERROR;
                break;
            }
        
            /*canvas*/
            i4_ret = x_wgl_get_canvas(_h_plane, &_h_canvas);
            if(WGLR_OK != i4_ret)
            {
                x_dbg_stmt("get canvas create failed. (%d)\r\n", i4_ret);
                i4_ret = CLIR_CMD_EXEC_ERROR;
                break;
            }
            
            at_rect[0].i4_top    = 0;
            at_rect[0].i4_left   = 0;
            i4_ret = x_wgl_get_canvas_size(_h_canvas, 
                                           (UINT32*)&at_rect[0].i4_right, 
                                           (UINT32*)&at_rect[0].i4_bottom);
            if(WGLR_OK != i4_ret)
            {
                x_dbg_stmt("get canvas size failed. (%d)\r\n", i4_ret);
                i4_ret = CLIR_CMD_EXEC_ERROR;
                break;
            }
           
            i4_ret = x_wgl_create_transparent_list(_h_canvas, 
                                                   at_rect,
                                                   1,
                                                   &_h_rgn_list); 
            if (CLIR_OK != i4_ret)
            {
                break;
            }
        
        } while(0);
        
        if (i4_ret != CLIR_OK)
        {
            _gl_obj_free();
        }
    }
    
    if (b_transparent)
    {
        x_wgl_enable_transparent_list(_h_rgn_list,
                                      TRUE,
                                      TRUE);  
    }
    else
    {
        x_wgl_enable_transparent_list(_h_rgn_list,
                                      FALSE,
                                      FALSE); 
    }
    
    /*i4_ret = x_wgl_show(_h_canvas, WGL_SW_NORMAL);*/
    i4_ret = x_wgl_flip_canvas(_h_canvas);

    return i4_ret;
    
    #else
    if (NULL_HANDLE == _h_screen)
    {   
        /* open screen related */
        i4_ret = x_gl_screen_open(SN_PRES_MAIN_DISPLAY, NULL, NULL, &_h_screen);
        
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("screen create failed. (%d)\r\n", i4_ret);
        
            return CLIR_CMD_EXEC_ERROR;
        }
        
        i4_ret = x_gl_screen_get_panel_size(_h_screen, 
                                            &_ui4_panel_width, 
                                            &_ui4_panel_height);
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("panel size get failed. (%d)\r\n", i4_ret);
            _gl_obj_free();
        
            return CLIR_CMD_EXEC_ERROR;
        }
        
        t_desc.e_clrmode  = e_clrmode;
        t_desc.h_clipper  = NULL_HANDLE;
        t_desc.h_palette  = NULL_HANDLE;
        t_desc.ui4_width  = _ui4_panel_width; 
        t_desc.ui4_height = _ui4_panel_height;
        
        i4_ret = x_gl_surface_create(&t_desc, &_h_disp_surf);
        
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("surface create failed. (%d)\r\n", i4_ret);
            _gl_obj_free();
        
            return CLIR_CMD_EXEC_ERROR;
        }
        
        i4_ret = x_gl_plane_open(_h_screen, ui2_plane_idx, &_h_plane);
        
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("plane create failed. (%d)\r\n", i4_ret);
            _gl_obj_free();
        
            return CLIR_CMD_EXEC_ERROR;
        }
        
        i4_ret = x_gl_plane_link(_h_plane, _h_screen, TRUE);
        
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("plane link failed. (%d)\r\n", i4_ret);
            _gl_obj_free();
        
            return CLIR_CMD_EXEC_ERROR;
        }

        
        i4_ret = x_gl_view_create(_ui4_panel_width,
                                  _ui4_panel_height, 
                                  FALSE,
                                  &_h_view);
        
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("view create failed. (%d)\r\n", i4_ret);
            _gl_obj_free();
        
            return CLIR_CMD_EXEC_ERROR;
        }
        
        /* viewport */
        _t_disp_opt.e_flag         = GL_DISP_OPT_POSITION | GL_DISP_OPT_BLEND_OPTION/* | GL_DISP_OPT_RESIZE | GL_DISP_OPT_SURF_OFFSET*/;
        _t_disp_opt.t_pos.i4_x     = 0;
        _t_disp_opt.t_pos.i4_y     = 0;
        _t_disp_opt.e_blend_option = GL_BLEND_OPT_NONE;
        _t_disp_opt.ui1_alpha      = 255;
        
        i4_ret = x_gl_view_create_viewport(_h_view, 
                                           _h_disp_surf, 
                                           &_t_disp_opt, 
                                           &_ui4_viewport);
        
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("viewport create failed. (%d)\r\n", i4_ret);
            _gl_obj_free();
        
            return CLIR_CMD_EXEC_ERROR;
        }
    }

    if (b_transparent)
    {
        t_color.u1.y = 0;
        t_color.u2.u = 0;
        t_color.u3.v = 0;
        t_color.a    = 0;
    }
    else
    {
        t_color.u1.y = 128;
        t_color.u2.u = 128;
        t_color.u3.v = 128;
        t_color.a    = 255;
    }

    x_gl_surface_fill(_h_disp_surf, &t_color, GL_SYNC);

    i4_ret = x_gl_plane_flip(_h_plane, _h_view, TRUE);

    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("flip failed. (%d)\r\n", i4_ret);
        _gl_obj_free();

        return CLIR_CMD_EXEC_ERROR;
    }
    
    return i4_ret;
    #endif
 #endif
    return 0;   
}

/* Name:  _mm_hdlr_sync_cancel_load_preview_thumbnail
 *
 * Description: This function sync cancel loading preview thumbnail
 *
 * Inputs:  i4_argc  
 *          pps_argv     
 *
 * Returns: 

 * Author : lianming lin(mcnmtk40234)
 *
 * History:
    (1)2009-7-29 : initial
 ----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_sync_cancel_load_preview_thumbnail (INT32         i4_argc, 
                                                          const CHAR**  pps_argv)
{
    INT32          i4_ret;

    x_dbg_stmt("%s()\r\n", __func__);

    if (x_handle_valid(h_g_main_svctx) == FALSE) 
    {
        i4_ret = x_svctx_open("main_svctx", &h_g_main_svctx);
        if (i4_ret != SVCTXR_OK) 
        {
            x_dbg_stmt("x_svctx_open(main_svctx) failed, i4_ret = %d\r\n", 
                       i4_ret);
            return i4_ret;
        }
    }

    i4_ret = x_svctx_sync_stop_svc(h_g_main_svctx, 0);
    if (i4_ret != SVCTXR_OK) 
    {
        x_dbg_stmt("x_svctx_sync_stop_svc(main_svctx) failed, i4_ret = %d\r\n",
                   i4_ret);
    }
    
    b_g_thumbnail_previewing   = FALSE;
    
    t_g_disp_region.ui4_x      =  500;
    t_g_disp_region.ui4_y      =  500;
    t_g_disp_region.ui4_width  =  2000;
    t_g_disp_region.ui4_height =  2000;
    
    if (i4_ret == CLIR_OK)
    {
        x_dbg_stmt("cancel loading preview thumbnail ok!\r\n");
    }
    return i4_ret;
}

#ifdef MULTIMEDIA_AUTOTEST_SUPPORT

/*-----------------------------------------------------------------------------
 * Name: _scpt_filter_is_comment_start
 *
 * Description: check is comment start flag
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static BOOL _scpt_filter_is_comment_start( CHAR*                 puc_str)
{
    if( NULL == puc_str )
    {
        x_dbg_stmt("_scpt_filter_is_comment_start null  !\n");
        return FALSE;
    }

    if( (SCPT_CHAR_SLASH == puc_str[0]) && (SCPT_CHAR_ASTERISK == puc_str[1]) )
    {
        return TRUE;
    }
    
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_filter_is_comment_end
 *
 * Description: check is comment end flag
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static BOOL _scpt_filter_is_comment_end( CHAR*                 puc_str)
{
    if( NULL == puc_str )
    {
        return FALSE;
    }

    if( (SCPT_CHAR_ASTERISK == puc_str[0]) && (SCPT_CHAR_SLASH == puc_str[1]) )
    {
        return TRUE;
    }
    
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_filter_comment
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static CHAR* _scpt_filter_comment( CHAR* puc_str , UINT32 ui4_len )
{
    CHAR* puc_filter        = puc_str;
    CHAR* puc_filter_end    = (puc_str + ui4_len);
    
    if( NULL == puc_str )
    {
        return puc_filter;
    }
    
    if( 0x0 == ui4_len )
    {
        return puc_filter;
    }
    
        
    if( TRUE != _scpt_filter_is_comment_start(puc_str) )
    {
        return puc_filter;
    }
    
    puc_filter +=0x2;
    
    while( ( FALSE == _scpt_filter_is_comment_end(puc_filter) ) && 
            ( puc_filter < puc_filter_end) )
    {
        puc_filter++;
    }
    
    puc_filter +=0x2;
    
    return puc_filter;
}


/*-----------------------------------------------------------------------------
 * Name: _scpt_is_nl
 *
 * Description: check is new line character or not, the ascii  0xd&oxa.
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static BOOL _scpt_is_nl( CHAR*                 puc_str)
{
    if( NULL == puc_str )
    {
        return FALSE;
    }

    if( ( SCPT_CHAR_CR == puc_str[0] ) || ( SCPT_CHAR_LF == puc_str[0] ) )
    {
        return TRUE;
    }
    
    return FALSE;
}

static BOOL _scpt_is_left_mid_bracket( CHAR*                 puc_str)
{
    if( NULL == puc_str )
    {
        return FALSE;
    }

    if( SCPT_CHAR_LEFT_MID_BRACKET == puc_str[0] )
    {
        return TRUE;
    }
    
    return FALSE;
}

static BOOL _scpt_is_right_mid_bracket( CHAR*                 puc_str)
{
    if( NULL == puc_str )
    {
        return FALSE;
    }

    if( SCPT_CHAR_RIGHT_MID_BRACKET == puc_str[0] )
    {
        return TRUE;
    }
    
    return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name: _scpt_filter_nl
 *
 * Description: filter new line character, the ascii  0xd&oxa.
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static CHAR* _scpt_filter_nl( CHAR* puc_str , UINT32 ui4_len )
{
    CHAR* puc_filter        = puc_str;
    CHAR* puc_filter_end    = (puc_str + ui4_len);
    
    if( NULL == puc_str )
    {
        return puc_filter;
    }
    
    if( 0x0 == ui4_len )
    {
        return puc_filter;
    }
    
    while( (TRUE == _scpt_is_nl(puc_filter)) && (puc_filter < puc_filter_end) )
    {
        puc_filter++;
    }
    
    return puc_filter;
}


/*-----------------------------------------------------------------------------
 * Name: _scpt_is_space
 *
 * Description: check is space, the ascii  0x20.
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static BOOL _scpt_is_space( CHAR*                 puc_str)
{
    if( NULL == puc_str )
    {
        return FALSE;
    }
    
    if( ( SCPT_CHAR_SPACE == puc_str[0] ) || ( SCPT_CHAR_TAB == puc_str[0] ) )
    {
        return TRUE;
    }
    
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_filter_space
 *
 * Description: filter space character, the ascii  0x20.
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static CHAR* _scpt_filter_space( CHAR* puc_str , UINT32 ui4_len )
{
    CHAR* puc_filter        = puc_str;
    CHAR* puc_filter_end    = (puc_str + ui4_len);
    
    if( NULL == puc_str )
    {
        return puc_filter;
    }
    
    if( 0x0 == ui4_len )
    {
        return puc_filter;
    }
    
    while( (TRUE == _scpt_is_space(puc_filter)) && (puc_filter < puc_filter_end) )
    {
        puc_filter++;
    }
    
    return puc_filter;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_filter_useless_char
 *
 * Description: filter space ,new line,comment.
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static CHAR* _scpt_filter_useless_char( CHAR* puc_str , UINT32 ui4_len )
{
    CHAR* puc_filter        = puc_str;
    CHAR* puc_last_filter   = puc_str;
    UINT32 ui4_filter_len   = ui4_len;

    do
    {
        /* 1.  filter space */
        puc_last_filter     = puc_filter;
        puc_filter          = _scpt_filter_space(puc_filter,ui4_filter_len);
        ui4_filter_len      = (puc_last_filter + ui4_filter_len - puc_filter);
        
        /*  2. filter new line char */
        puc_last_filter     = puc_filter;
        puc_filter          = _scpt_filter_nl(puc_filter,ui4_filter_len);
        ui4_filter_len      = (puc_last_filter + ui4_filter_len - puc_filter);
        
        /*  3. filter comment */
        puc_last_filter     = puc_filter;
        puc_filter          = _scpt_filter_comment(puc_filter,ui4_filter_len);
        ui4_filter_len      = (puc_last_filter + ui4_filter_len - puc_filter);

        if( 0x0 == ui4_filter_len )
        {
            break;
        }
    }while(_scpt_is_space(puc_filter)||
            _scpt_is_nl(puc_filter)||
            _scpt_filter_is_comment_start(puc_filter));

    return puc_filter;
}

static BOOL _scpt_is_number( CHAR       uc_char )
{
    /* 0~9 */
    if( (  uc_char >= '0' ) && (  uc_char <= '9' ) )
    {
        return TRUE;
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_is_number_char
 *
 * Description: check is word char
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static BOOL _scpt_is_number_char( CHAR*       puc_str )
{
    /* 0~9 */
    if( (  puc_str[0] >= '0' ) && (  puc_str[0] <= '9' ) )
    {
        return TRUE;
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_is_word_char
 *
 * Description: check is word char
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static BOOL _scpt_is_word_char( CHAR*       puc_str )
{
    if( NULL == puc_str )
    {
        return FALSE;
    }
    
    /* a~z */
    if( (  puc_str[0] >= 'a' ) && (  puc_str[0] <= 'z' ) )
    {
        return TRUE;
    }
    
    /* A~Z */
    if( (  puc_str[0] >= 'A' ) && (  puc_str[0] <= 'Z' ) )
    {
        return TRUE;
    }
    
    /* 0~9 */
    if( (  puc_str[0] >= '0' ) && (  puc_str[0] <= '9' ) )
    {
        return TRUE;
    }
    
    /* "_" */
    if( '_' == puc_str[0] )
    {
        return TRUE;
    }
    
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_check_number_str
 *
 * Description: check the string is number or not
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _scpt_check_number_str( CHAR* puc_str ,          UINT32 ui4_len )
{
    UINT32 ui4_loop         = 0x0;

    if( (NULL == puc_str) || (0x0 == ui4_len) )
    {
        return FALSE;
    }
    
    /* 2.  filter new line char */
    while((TRUE==_scpt_is_number(puc_str[ui4_loop]))&&(ui4_loop<ui4_len))
    {
        ui4_loop++;
    }

    if(ui4_loop == ui4_len)
    {
        return TRUE;
    }
    
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_is_para_divide_char
 *
 * Description: check is parameter divide char
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static BOOL _scpt_is_para_divide_char( CHAR* puc_str )
{
    if( NULL == puc_str )
    {
        return FALSE;
    }
    
    if( TRUE == _scpt_is_space(puc_str) )
    {
        return TRUE;
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_query_number_str_len
 *
 * Description: query number string len.
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static UINT32 _scpt_query_number_str_len( CHAR* puc_str ,    UINT32 ui4_len )
{
    UINT32 ui4_loop         = 0x0;
    UINT32 ui4_loop_len     = ui4_len;
    CHAR* puc_filter        = puc_str;

    if(NULL == puc_str)
    {
        return 0x0;
    }
    
    /* 1.  check is ward  char or not */

    if( TRUE != _scpt_is_number_char(puc_str) )
    {
        return 0x0;
    }
    
    /* 2.  filter new line char */
    ui4_loop_len = ( ( puc_str + ui4_len) - puc_filter );
    while( (TRUE == _scpt_is_number_char(puc_str)) && (ui4_loop<ui4_loop_len) )
    {
        puc_str++;
        ui4_loop++;
    }
    
    return ui4_loop;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_query_str_len
 *
 * Description: query word string len.
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static UINT32 _scpt_query_str_len( CHAR* puc_str ,          UINT32 ui4_len )
{
    UINT32 ui4_loop         = 0x0;
    UINT32 ui4_loop_len     = ui4_len;
    CHAR* puc_filter        = puc_str;

    if(NULL == puc_str)
    {
        return 0x0;
    }
    
    /* 1.  check is ward  char or not */

    if( TRUE != _scpt_is_word_char(puc_str) )
    {
        return 0x0;
    }
    
    /* 2.  filter new line char */
    ui4_loop_len = ( ( puc_str + ui4_len) - puc_filter );
    while( (TRUE == _scpt_is_word_char(puc_str)) && (ui4_loop<ui4_loop_len) )
    {
        puc_str++;
        ui4_loop++;
    }
    
    return ui4_loop;
}


static INT32 _scpt_get_crnt_buf_size( SCPT_TEXT_T*        pt_file )
{
    return (pt_file->ui4_size - (pt_file->puc_crnt - pt_file->puc_start) );
}

/* UINT64_TYPE_LIST list funcs */
static void _scpt_u8_list_init( SCPT_U8_TYPE_LIST_T*      pt_u8_list )
{
    if(NULL == pt_u8_list)
    {
        return;
    }
    
    pt_u8_list->ui4_len = 0x0;
    pt_u8_list->pt_head = NULL;
        
    return ;
}

static void _scpt_str_arry_type_list_init(
                    SCPT_STRING_TYPE_LIST_T*        pt_str_arry_type_list )
{
    if(NULL == pt_str_arry_type_list)
    {
        return;
    }
    
    pt_str_arry_type_list->ui4_len              = 0x0;
    pt_str_arry_type_list->pt_head              = NULL;
    return ;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_str_arry_type_list_add
 *
 * Description: add str array list  node
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _scpt_str_arry_type_list_add(  SCPT_STRING_TYPE_LIST_T*   pt_list,
                                            SCPT_STRING_ARRY_TYPE_T*   pt_node )
{
    SCPT_STRING_ARRY_TYPE_T*   pt_tail_node = NULL;
    
    if( (NULL == pt_list) || (NULL == pt_node) )
    {
        return CLIR_NOT_INIT;
    }
    
    pt_tail_node = _scpt_get_str_arry_type_list_tail(pt_list);
    if(NULL == pt_tail_node)
    {
        if( 0x0 != pt_list->ui4_len )
        {
            return CLIR_NOT_INIT;
        }
        
        pt_list->pt_head    = pt_node;
    }
    else
    {
        pt_tail_node->pt_next   = pt_node;
    }

    pt_node->pt_next        = NULL;
    pt_list->ui4_len++;
        
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_str_arry_type_list_rm_node
 *
 * Description: rm one str array list  node
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _scpt_str_arry_type_list_rm_node(  
        SCPT_STRING_TYPE_LIST_T*            pt_list,
        UINT32                              u4_rm_idx)
{
    SCPT_STRING_ARRY_TYPE_T*     pt_rm_node  = NULL;
    SCPT_STRING_ARRY_TYPE_T*     pt_node     = NULL;
    
    if( NULL == pt_list )
    {
        x_dbg_stmt("_scpt_str_arry_type_list_rm_node null  !\n");
        return CLIR_NOT_INIT;
    }
    
    if( u4_rm_idx > pt_list->ui4_len )
    {
        x_dbg_stmt("_scpt_str_arry_type_list_rm_node fail  !\n");
        return CLIR_NOT_INIT;
    }
    
    if( ( 0x0 == pt_list->ui4_len ) || ( 0x0 == u4_rm_idx ) )
    {
        x_dbg_stmt("_scpt_str_arry_type_list_rm_node 0 len(idx)  !\n");
        return CLIR_NOT_INIT;
    }
     
    if( 0x1 == u4_rm_idx )
    {
        pt_rm_node          = pt_list->pt_head;
        pt_list->pt_head    = pt_rm_node->pt_next;
    }
    else
    {
        pt_node = _scpt_get_str_arry_type_list_node(pt_list, u4_rm_idx - 1 );
        if(NULL == pt_node)
        {
            x_dbg_stmt("_scpt_str_arry_type_list_rm_node get pre node fail!\n");
            return CLIR_NOT_INIT;
        }
        
        pt_rm_node   = pt_node->pt_next;
        if(NULL == pt_rm_node)
        {
            x_dbg_stmt("_scpt_str_arry_type_list_rm_node no node !\n");
            return CLIR_NOT_INIT;
        }
        
        pt_node->pt_next    = pt_rm_node->pt_next;
    }
    
    /* free u8 vara */
    x_mem_free(pt_rm_node->pac_vals);
    x_mem_free(pt_rm_node);
    pt_list->ui4_len--;
        
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_str_arry_type_list_free
 *
 * Description: free str array type list.
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static void _scpt_str_arry_type_list_free( SCPT_STRING_TYPE_LIST_T*  pt_list)
{
    INT32             i4_ret        = CLIR_OK;  

    if( NULL == pt_list)
    {
        x_dbg_stmt("_scpt_str_arry_type_list_free null  !\n");
        return;
    }
    
    while(pt_list->ui4_len)
    {
        i4_ret = _scpt_str_arry_type_list_rm_node(pt_list,pt_list->ui4_len);
        if( CLIR_OK != i4_ret )
        {
            x_dbg_stmt("_scpt_str_arry_type_list_free fail !\n");
        }
    }
        
    return ;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_get_str_arry_type_list_tail
 *
 * Description: get str array list tail node
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static SCPT_STRING_ARRY_TYPE_T* _scpt_get_str_arry_type_list_tail(
                                            SCPT_STRING_TYPE_LIST_T*   pt_list)
{
    SCPT_STRING_ARRY_TYPE_T*   pt_node = NULL;
    if( NULL == pt_list )
    {
        x_dbg_stmt("_scpt_get_str_arry_type_list_tail null  !\n");
        return NULL;
    }
    
    pt_node = _scpt_get_str_arry_type_list_node(pt_list,pt_list->ui4_len);
        
    return pt_node;
}

/*-----------------------------------------------------------------------------
 * Name: _scpt_get_str_arry_type_list_node
 *
 * Description: get str array list  node
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static SCPT_STRING_ARRY_TYPE_T* _scpt_get_str_arry_type_list_node(
                                    SCPT_STRING_TYPE_LIST_T*    pt_list,
                                    UINT32                      u8_cnt)
{
    SCPT_STRING_ARRY_TYPE_T*        pt_node = NULL;
    UINT32                          u8_idx = 0x1;
    
    if( NULL == pt_list )
    {
        return NULL;
    }
    
    if( ( u8_cnt > pt_list->ui4_len ) || ( 0x0 == u8_cnt ) )
    {
        return NULL;
    }
    
    pt_node = pt_list->pt_head;
    
    while( u8_idx < u8_cnt )
    {
        if(pt_node)
        {
            pt_node = pt_node->pt_next;
        }
        else
        {
            break;
        }
        
        u8_idx++;
    }

    if( u8_idx < u8_cnt )
    {
        x_dbg_stmt("_scpt_get_str_arry_type_list_node error  !\n");
        return NULL;
    }
    
    return pt_node;
}


static SCPT_U8_TYPE_T* _scpt_get_u8_list_tail(SCPT_U8_TYPE_LIST_T*  pt_u8_list )
{
    SCPT_U8_TYPE_T*   pt_u8_node = NULL;
    if( NULL == pt_u8_list )
    {
        x_dbg_stmt("_scpt_get_u8_list_tail null  !\n");
        return NULL;
    }
    
    pt_u8_node = pt_u8_list->pt_head;
    
    if( NULL == pt_u8_node)
    {
        return NULL;
    }
    
    while(NULL != pt_u8_node->pt_next)
    {
        pt_u8_node = pt_u8_node->pt_next;
    }
        
    return pt_u8_node;
}

static INT32 _scpt_get_str_array_val( 
                        SCPT_STRING_ARRY_TYPE_T*   pt_str_arry,
                        SCPT_STRING_CLASS_T*       pt_str_obj )
{
    UINT32      ui4_dq_cnt  = 0x0;
    UINT32      ui4_str_len = 0x0;
    CHAR*       pac_vals    = NULL;
    CHAR*       pac_end     = NULL;
    
    if( NULL == pt_str_arry )
    {
        return CLIR_NOT_INIT;
    }

    if( pt_str_arry->ui4_idx >= pt_str_arry->ui4_cnt )
    {
        return CLIR_NOT_INIT;
    }
    
    pac_vals = pt_str_arry->pac_vals;
    pac_end  = (pac_vals + pt_str_arry->ui4_len);
    
    if( NULL == pac_vals )
    {
        return CLIR_NOT_INIT;
    }

    do
    {
        if( pac_vals >= pac_end )
        {
            return CLIR_NOT_INIT;
        }
        
        if( SCPT_CHAR_DQM == pac_vals[0] )
        {
            ui4_dq_cnt++;
        }

        if( ui4_dq_cnt >= (2*pt_str_arry->ui4_idx + 1 ) )
        {
            break;
        }
        
    }while(pac_vals++);

    ui4_str_len =_scpt_parser_query_dq_str_len(pac_vals,(pac_end - pac_vals));
    if( ui4_str_len <= 0x2 )
    {
        return CLIR_NOT_INIT;
    }

    pt_str_obj->pc_str  = (pac_vals+1);
    pt_str_obj->ui4_len = (ui4_str_len - 0x2);

    return CLIR_OK;
}

static SCPT_STRING_ARRY_TYPE_T* _scpt_query_str_node( 
                        SCPT_STRING_TYPE_LIST_T*   pt_list,
                        CHAR*                      pc_str )
{
    SCPT_STRING_ARRY_TYPE_T* pt_node = NULL;

    if( NULL == pt_list )
    {
        return NULL;
    }

    pt_node = pt_list->pt_head;

    while(pt_node)
    {
        if(0x0 == x_memcmp( pc_str,
                            pt_node->pac_str_arry_name,
                            x_strlen(pt_node->pac_str_arry_name) ) )
        {
            break;
        }
        
        pt_node = pt_node->pt_next;
    }

    return pt_node;
}

static SCPT_U8_TYPE_T* _scpt_query_u8_node( SCPT_U8_TYPE_LIST_T*     pt_list,
                                            CHAR*                    pc_str )
{
    SCPT_U8_TYPE_T* pt_node = NULL;

    if( NULL == pt_list )
    {
        return NULL;
    }

    pt_node = pt_list->pt_head;

    while(pt_node)
    {
        if(0x0 == x_memcmp(pc_str,pt_node->pac_vara,x_strlen(pt_node->pac_vara)))
        {
            break;
        }
        
        pt_node = pt_node->pt_next;
    }

    return pt_node;
}

static INT32 _scpt_u8_list_add(  SCPT_U8_TYPE_LIST_T*               pt_list,
                                 SCPT_U8_TYPE_T*                    pt_node )
{
    SCPT_U8_TYPE_T*   pt_tail_node = NULL;
    
    if( (NULL == pt_list) || (NULL == pt_node) )
    {
        x_dbg_stmt("_scpt_uint8_list_add null  !\n");
        return CLIR_NOT_INIT;
    }
    
    pt_tail_node = _scpt_get_u8_list_tail(pt_list);
    if(NULL == pt_tail_node)
    {
        if( 0x0 != pt_list->ui4_len )
        {
            x_dbg_stmt("_scpt_uint8_list_add ui4_len fail !\n");
            return CLIR_NOT_INIT;
        }
        
        pt_list->pt_head = pt_node;
    }
    else
    {
        pt_tail_node->pt_next   = pt_node;
    }
    
    pt_node->pt_next = NULL;
    pt_list->ui4_len++;
        
    return CLIR_OK;
}

static SCPT_U8_TYPE_T* _scpt_u8_list_get_node(SCPT_U8_TYPE_LIST_T*  pt_u8_list,
                                              UINT32                u8_cnt)
{
    SCPT_U8_TYPE_T*     pt_node = NULL;
    UINT32              u8_idx = 0x1;
    
    if( NULL == pt_u8_list )
    {
        x_dbg_stmt("_scpt_u8_list_get_node null  !\n");
        return NULL;
    }
    
    if( u8_cnt > pt_u8_list->ui4_len )
    {
        x_dbg_stmt("_scpt_u8_list_get_node fail  !\n");
        return NULL;
    }
    
    pt_node = pt_u8_list->pt_head;
    
    while( u8_idx < u8_cnt )
    {
        if(pt_node)
        {
            pt_node = pt_node->pt_next;
        }
        else
        {
            break;
        }
        
        u8_idx++;
    }

    if( u8_idx < u8_cnt )
    {
        x_dbg_stmt("_scpt_u8_list_get_node error  !\n");
        return NULL;
    }
    
    return pt_node;
}

static INT32 _scpt_u8_list_rm_node(  SCPT_U8_TYPE_LIST_T*           pt_u8_list,
                                         UINT32                     u8_rm_cnt)
{
    SCPT_U8_TYPE_T*     pt_rm_node  = NULL;
    SCPT_U8_TYPE_T*     pt_node     = NULL;
    
    if( NULL == pt_u8_list )
    {
        x_dbg_stmt("_scpt_u8_list_remove_node null  !\n");
        return CLIR_NOT_INIT;
    }
    
    if( u8_rm_cnt > pt_u8_list->ui4_len )
    {
        x_dbg_stmt("_scpt_u8_list_remove_node fail  !\n");
        return CLIR_NOT_INIT;
    }
    
    if( 0x0 == pt_u8_list->ui4_len )
    {
        x_dbg_stmt("_scpt_u8_list_remove_node 0 len  !\n");
        return CLIR_NOT_INIT;
    }
     
    if( 0x1 == u8_rm_cnt )
    {
        pt_rm_node          = pt_u8_list->pt_head;
        pt_u8_list->pt_head = pt_rm_node->pt_next;
    }
    else
    {
        pt_node = _scpt_u8_list_get_node(pt_u8_list, u8_rm_cnt - 1 );
        if(NULL == pt_node)
        {
            x_dbg_stmt("_scpt_u8_list_remove_node get node fail  !\n");
            return CLIR_NOT_INIT;
        }
        
        pt_rm_node          = pt_node->pt_next;
        if(NULL == pt_rm_node)
        {
            x_dbg_stmt("_scpt_u8_list_remove_node no  !\n");
            return CLIR_NOT_INIT;
        }
        
        pt_node->pt_next    = pt_rm_node->pt_next;
    }
    
    /* free u8 vara */
    x_mem_free(pt_rm_node->pac_val);
    x_mem_free(pt_rm_node->pac_vara);
    x_mem_free(pt_rm_node);
    pt_u8_list->ui4_len--;
        
    return CLIR_OK;
}


static void _scpt_u8_list_free( SCPT_U8_TYPE_LIST_T*                pt_u8_list)
{
    INT32             i4_ret        = CLIR_OK;  

    if( NULL == pt_u8_list)
    {
        x_dbg_stmt("_scpt_uint8_list_free null  !\n");
        return;
    }
    
    while(pt_u8_list->ui4_len)
    {
        i4_ret = _scpt_u8_list_rm_node(pt_u8_list,pt_u8_list->ui4_len);
        if( CLIR_OK != i4_ret )
        {
            x_dbg_stmt("_scpt_uint8_list_free fail(%d) !\n",pt_u8_list->ui4_len);
        }
    }
        
    return ;
}


/* */
static INT32 _scpt_parser_one_glb_u8_vara( SCPT_DATA_T*      pt_scpt_data )
{
    CHAR*               puc_vara                = NULL;
    UINT32              ui4_vara_len            = 0x0;
    SCPT_U8_TYPE_T*     pt_u8_data              = NULL;
    INT32               i4_ret                  = CLIR_OK;
    
    if(NULL == pt_scpt_data)
    {
        return CLIR_INV_ARG;
    }
    
    /* Filter */
    /* 1. check u8 key word */
    if(0x0 != x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_BASIC_TYPE_U8,
                        x_strlen(SCPT_BASIC_TYPE_U8)) )
    {
        x_dbg_stmt("not u8 type \n");
        return CLIR_INV_ARG;
    }

    pt_scpt_data->t_file.puc_crnt += x_strlen(SCPT_BASIC_TYPE_U8);
    
    do
    {
        pt_u8_data = x_mem_alloc(sizeof(SCPT_U8_TYPE_T));
        if(NULL == pt_u8_data)
        {
            x_dbg_stmt("u8 data malloc fail \n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        pt_scpt_data->t_file.puc_crnt = 
                        _scpt_filter_useless_char(
                        pt_scpt_data->t_file.puc_crnt,
                        _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
        
        /* 2.  query  word strling  len*/
        ui4_vara_len = _scpt_query_str_len(pt_scpt_data->t_file.puc_crnt,
                            _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
        puc_vara = x_mem_alloc(ui4_vara_len + 1 );
        if( NULL == puc_vara )
        {
            x_dbg_stmt("u8 vara malloc fail \n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        /* 2.  get  vara word string */
        x_memset(puc_vara, 0, (ui4_vara_len + 1 ) );
        x_memcpy(puc_vara, pt_scpt_data->t_file.puc_crnt, ui4_vara_len);
        
        pt_u8_data->pac_vara            = puc_vara;
        pt_scpt_data->t_file.puc_crnt   += ui4_vara_len;
        
        /* 3.  get  vara value */
        /* filter useless char */
        
        pt_scpt_data->t_file.puc_crnt = 
                        _scpt_filter_useless_char(
                        pt_scpt_data->t_file.puc_crnt,
                        _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
            
        if( SCPT_CHAR_EM != pt_scpt_data->t_file.puc_crnt[0] )
        {
            x_dbg_stmt("equal mark fail !\n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        pt_scpt_data->t_file.puc_crnt += 1;
        
        pt_scpt_data->t_file.puc_crnt = _scpt_filter_useless_char(
                        pt_scpt_data->t_file.puc_crnt,
                        _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
        
        /*  query  u8 value  len*/
        ui4_vara_len = _scpt_query_number_str_len(pt_scpt_data->t_file.puc_crnt,
                            _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
        
        if(TRUE !=_scpt_check_number_str(pt_scpt_data->t_file.puc_crnt,ui4_vara_len))
        {
            x_dbg_stmt("u8 value is not int type !\n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }

        if( ui4_vara_len > 0x10 )
        {
            x_dbg_stmt("u8 value very large !\n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        pt_u8_data->pac_val = x_mem_alloc( ui4_vara_len + 1 );
        if( NULL == pt_u8_data->pac_val )
        {
            x_dbg_stmt("u8 data val malloc fail \n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        x_memset(pt_u8_data->pac_val, 0, ( ui4_vara_len + 1 ) );
        x_memcpy(pt_u8_data->pac_val,pt_scpt_data->t_file.puc_crnt,ui4_vara_len);
        
        pt_scpt_data->t_file.puc_crnt += ui4_vara_len;
        
        pt_scpt_data->t_file.puc_crnt = _scpt_filter_useless_char(
                        pt_scpt_data->t_file.puc_crnt,
                        _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
        
        if( SCPT_CHAR_SEMICOLON != pt_scpt_data->t_file.puc_crnt[0] )
        {
            x_dbg_stmt("semicolon mark fail !\n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        pt_scpt_data->t_file.puc_crnt +=1;
        
        pt_u8_data->pt_next = NULL;
        
        i4_ret = _scpt_u8_list_add(&pt_scpt_data->t_b8_list,pt_u8_data);
        if( CLIR_OK != i4_ret )
        {
            x_dbg_stmt("_scpt_u8_list_add fail \n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        return CLIR_OK;
    }while(0);

    x_mem_free(puc_vara);
    x_mem_free(pt_u8_data);
    
    return i4_ret;
}

static UINT32 _scpt_parser_query_mid_bracket_str_len(CHAR* puc_str,UINT32 ui4_len)
{
    CHAR* puc_filter        = puc_str;
    CHAR* puc_filter_end    = (puc_str + ui4_len);
    
    if( TRUE != _scpt_is_left_mid_bracket(puc_filter) )
    {
        x_dbg_stmt("_scpt_parser_query_mid_bracket_str_len invalid \n");
        return 0x0;
    }

    puc_filter++;
    
    while(  (   TRUE != _scpt_is_right_mid_bracket( puc_filter ) )&&
             (   puc_filter < puc_filter_end ) )
    {
        puc_filter++;
    }

    if( puc_filter >= puc_filter_end)
    {
        return 0x0;
    }
    
    puc_filter++;
        
    return (puc_filter - puc_str);
}

static CHAR* _scpt_parser_check_and_filter_array_key(CHAR* puc_str,UINT32 ui4_len)
{
    CHAR* puc_filter        = puc_str;
    CHAR* puc_last_filter   = puc_str;
    UINT32 ui4_filter_len   = ui4_len;
    UINT32 ui4_vara_len     = 0x0;
    
    puc_filter          = _scpt_filter_useless_char(puc_filter,ui4_filter_len );
    ui4_filter_len      = (puc_last_filter + ui4_filter_len - puc_filter);
    
    if( TRUE != _scpt_is_left_mid_bracket(puc_filter) )
    {
        x_dbg_stmt("str_array left_mid_bracket invalid \n");
        return NULL;
    }
    
    puc_filter      +=  0x1;
    ui4_filter_len  -=  0x1;
    
    puc_filter      = _scpt_filter_useless_char(puc_filter,ui4_filter_len );
    ui4_filter_len  = (puc_last_filter + ui4_filter_len - puc_filter);

    ui4_vara_len    = _scpt_query_str_len(puc_filter,ui4_filter_len);
    puc_filter      += ui4_vara_len;
    ui4_filter_len  -= ui4_vara_len;

    puc_filter      = _scpt_filter_useless_char(puc_filter,ui4_filter_len );
    ui4_filter_len  = (puc_last_filter + ui4_filter_len - puc_filter);
    
    if( TRUE != _scpt_is_right_mid_bracket(puc_filter) )
    {
        x_dbg_stmt("str_array right_mid_bracket invalid \n");
        return NULL;
    }
    
    puc_filter      +=  0x1;
    ui4_filter_len  -=  0x1;
    
    puc_filter          = _scpt_filter_useless_char(puc_filter,ui4_filter_len );
    ui4_filter_len      = (puc_last_filter + ui4_filter_len - puc_filter);

    if( SCPT_CHAR_EM != puc_filter[0] )
    {
        x_dbg_stmt("str_array equal invalid \n");
        return NULL;
    }
    
    puc_filter      +=1;
    ui4_filter_len  -=1;
    
    puc_filter      = _scpt_filter_useless_char(puc_filter,ui4_filter_len );
    ui4_filter_len  = (puc_last_filter + ui4_filter_len - puc_filter);
    
    if( SCPT_CHAR_LEFT_BIG_BRACKET != puc_filter[0] )
    {
        x_dbg_stmt("str_array invalid LEFT_BIG_BRACKET \n");
        return NULL;
    }
    
    return puc_filter;
}

static INT32 _scpt_parser_one_str_array( SCPT_DATA_T*           pt_scpt_data )
{
    UINT32                      ui4_vara_len            = 0x0;
    UINT32                      ui4_str_arry_vals_len   = 0x0;
    SCPT_STRING_ARRY_TYPE_T*    pt_str_array            = NULL;
    INT32                       i4_ret                  = CLIR_OK;
    SCPT_TEXT_T*                pt_file                 = NULL;;
    
    if(NULL == pt_scpt_data)
    {
        return CLIR_INV_ARG;
    }

    pt_file = &pt_scpt_data->t_file;
    
    /* Filter */
    /* 1. check string key word */
    if(0x0 != x_memcmp( pt_file->puc_crnt,
                        SCPT_BASIC_TYPE_STR,
                        x_strlen(SCPT_BASIC_TYPE_STR)) )
    {
        x_dbg_stmt("not str_array type \n");
        return CLIR_INV_ARG;
    }

    /* filter string key word */
    pt_file->puc_crnt += x_strlen(SCPT_BASIC_TYPE_STR);
    
    do
    {
        pt_str_array = x_mem_alloc(sizeof(SCPT_STRING_ARRY_TYPE_T));
        if(NULL == pt_str_array)
        {
            x_dbg_stmt("str_array malloc fail \n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        pt_file->puc_crnt = _scpt_filter_useless_char(pt_file->puc_crnt,
                                        _scpt_get_crnt_buf_size(pt_file) );
        
        /* 2.  query  string array name  len*/
        ui4_vara_len = _scpt_query_str_len(pt_file->puc_crnt,
                                        _scpt_get_crnt_buf_size(pt_file));
        if( 0x0 == ui4_vara_len )
        {
            x_dbg_stmt("str_array name null \n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        pt_str_array->pac_str_arry_name = x_mem_alloc(ui4_vara_len + 1 );
        if( NULL == pt_str_array->pac_str_arry_name )
        {
            x_dbg_stmt("str_array malloc fail \n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        /* 2.  get  vara word string */
        x_memset(pt_str_array->pac_str_arry_name, 0, (ui4_vara_len + 1 ) );
        x_memcpy(pt_str_array->pac_str_arry_name,pt_file->puc_crnt,ui4_vara_len);
        
        pt_file->puc_crnt += ui4_vara_len;
        
        /* 3.  get  vara value */
        
        /* check string array flag [...] = {...} */

        pt_file->puc_crnt =_scpt_parser_check_and_filter_array_key(
                                pt_file->puc_crnt,
                                _scpt_get_crnt_buf_size(pt_file));
        if(NULL == pt_file->puc_crnt)
        {
            x_dbg_stmt("_scpt_parser_check_and_filter_array_key  fail \n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        ui4_str_arry_vals_len = _scpt_parser_query_big_bracket_str_len(
                                pt_file->puc_crnt,
                                _scpt_get_crnt_buf_size(pt_file));
        
        pt_str_array->pac_vals = x_mem_alloc(ui4_str_arry_vals_len + 1 );
        pt_str_array->ui4_len  = (ui4_str_arry_vals_len + 1 );
        
        if( NULL == pt_str_array->pac_vals )
        {
            x_dbg_stmt("str_array malloc fail \n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        x_memset(pt_str_array->pac_vals,0,(ui4_str_arry_vals_len + 1));
        x_memcpy(pt_str_array->pac_vals,pt_file->puc_crnt,ui4_str_arry_vals_len);
        
        pt_file->puc_crnt += ui4_str_arry_vals_len;
        
        /* parser get  array strings */
        i4_ret = _scpt_parser_one_str_array_vals(pt_str_array);
        if( CLIR_OK != i4_ret )
        {
            x_dbg_stmt("_scpt_parser_one_str_array_vals fail !\n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }

        pt_scpt_data->t_file.puc_crnt =
                    _scpt_parser_filter_and_check_semicolon(
                    pt_scpt_data->t_file.puc_crnt,
                    _scpt_get_crnt_buf_size(&pt_scpt_data->t_file) );
        
        if( NULL == pt_scpt_data->t_file.puc_crnt)
        {
            x_dbg_stmt("semicolon mark fail !\n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        /* add one str  array to list */
        i4_ret = _scpt_str_arry_type_list_add(
                                &pt_scpt_data->t_str_arry_type_list,
                                pt_str_array);
        if( CLIR_OK != i4_ret )
        {
            x_dbg_stmt("_scpt_str_arry_type_list_add fail !\n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        
        return CLIR_OK;
    }while(0);

    x_mem_free(pt_str_array->pac_str_arry_name);
    x_mem_free(pt_str_array->pac_vals);
    x_mem_free(pt_str_array);
    
    return i4_ret;
}

static INT32 _scpt_parser_one_str_array_vals(SCPT_STRING_ARRY_TYPE_T*  pt_str_arry_type)
{
    if( NULL == pt_str_arry_type )
    {
        x_dbg_stmt("_scpt_parser_one_str_array_vals null !\n");
        return  CLIR_INV_ARG;
    }
    
    if( SCPT_CHAR_LEFT_BIG_BRACKET != pt_str_arry_type->pac_vals[0] )
    {
        x_dbg_stmt("_scpt_parser_one_str_array_vals invalid !\n");
        return  CLIR_INV_ARG;
    }
    
    pt_str_arry_type->ui4_cnt = _scpt_parser_get_str_dqm_cnt(
                                    pt_str_arry_type->pac_vals,
                                    pt_str_arry_type->ui4_len);
    
    if( pt_str_arry_type->ui4_cnt % 2 )
    {
        x_dbg_stmt("_scpt_parser_one_str_array_vals fail !\n");
        return  CLIR_INV_ARG;
    }
        
    pt_str_arry_type->ui4_cnt = (pt_str_arry_type->ui4_cnt/2);
    
    return CLIR_OK;
}

static INT32 _scpt_parser_query_dq_str_len( CHAR*   pb_str,     UINT32 ui4_len )
{
    CHAR*   puc_fltr        = pb_str;
    CHAR*   puc_fltr_end    = (pb_str + ui4_len);
    
    if( SCPT_CHAR_DQM != puc_fltr[0] )
    {
        x_dbg_stmt("_scpt_parser_query_dq_str_len invalid !\n");
        return  0x0;
    }
    
    puc_fltr +=1;
    
    while( (SCPT_CHAR_DQM != puc_fltr[0]) && ( puc_fltr < puc_fltr_end ) )
    {
        puc_fltr++;
    }

    if( puc_fltr >= puc_fltr_end )
    {
        x_dbg_stmt("_scpt_parser_query_dq_str_len fail !\n");
        return  0x0;
    }
    puc_fltr +=1;
    
    return (puc_fltr - pb_str);
}

static CHAR* _scpt_parser_filter_to_semicolon(CHAR* pb_str,UINT32 ui4_len)
{
    CHAR*   puc_fltr        = pb_str;
    CHAR*   puc_end         = ( pb_str + ui4_len );

    while( puc_fltr < puc_end )
    {
        if( SCPT_CHAR_SEMICOLON == puc_fltr[0] )
        {
            break;
        }
        puc_fltr++;
    }
    
    if( puc_fltr >= puc_end )
    {
        return NULL;
    }
    
    return puc_fltr;
}


static CHAR* _scpt_parser_filter_and_check_semicolon(CHAR* pb_str,UINT32 ui4_len)
{
    CHAR*   puc_fltr        = pb_str;
    CHAR*   puc_last_filter = pb_str;
    UINT32  ui4_filter_len  = ui4_len;
    
    puc_last_filter =   pb_str;
    puc_fltr        =   _scpt_filter_useless_char(puc_fltr,ui4_filter_len);
    ui4_filter_len  =   (puc_last_filter + ui4_filter_len - puc_fltr);
    
    if( 0x0 == ui4_filter_len )
    {
        x_dbg_stmt("_scpt_parser_filter_and_check_semicolon null !\n");
        return  NULL;
    }
    
    if( SCPT_CHAR_SEMICOLON != puc_fltr[0] )
    {
        x_dbg_stmt("_scpt_parser_filter_and_check_semicolon failed !\n");
        return  NULL;
    }
    
    puc_fltr +=1;
    
    return puc_fltr;
}


static INT32 _scpt_parser_query_big_bracket_str_len(CHAR* pb_str,UINT32 ui4_len)
{
    CHAR*   puc_fltr        = pb_str;
    CHAR*   puc_fltr_end    = (pb_str + ui4_len);
    
    if( SCPT_CHAR_LEFT_BIG_BRACKET != puc_fltr[0] )
    {
        x_dbg_stmt("_scpt_parser_query_big_bracket_str_len invalid !\n");
        return  0x0;
    }
    
    puc_fltr +=1;
    
    while((SCPT_CHAR_RIGHT_BIG_BRACKET!=puc_fltr[0])&&(puc_fltr<puc_fltr_end))
    {
        puc_fltr++;
    }

    if( puc_fltr >= puc_fltr_end )
    {
        x_dbg_stmt("_scpt_parser_query_big_bracket_str_len fail !\n");
        return  0x0;
    }
    puc_fltr +=1;
    
    return (puc_fltr - pb_str);
}



static UINT32 _scpt_parser_get_str_dqm_cnt( CHAR*  pb_str, UINT32 ui4_len)
{
    UINT32  ui4_str_vals_cnt    = 0x0;
    CHAR*   puc_fltr            = pb_str;
    CHAR*   puc_fltr_end        = (pb_str + ui4_len);
    
    if( NULL == pb_str )
    {
        x_dbg_stmt("_scpt_parser_get_str_array_vals_cnt null !\n");
        return  0x0;
    }
   
    while( puc_fltr < puc_fltr_end )
    {
        if( SCPT_CHAR_DQM == puc_fltr[0] )
        {
            ui4_str_vals_cnt++;
        }
        
        puc_fltr++;
    }
    
    return ui4_str_vals_cnt;
}
   
static UINT32 _scpt_parser_check_test_case_cmd( char*        pc_str )
{
    UINT32  ui4_lp      = 0x0;
    UINT32  ui4_cmd_cnt = (sizeof(at_mm_hdlr_cmd_tbl)/sizeof(CLI_EXEC_T));
    
    for(ui4_lp = 0x0; ui4_lp < ui4_cmd_cnt; ui4_lp++ )
    {
        if(0x0 == x_memcmp( pc_str,
                        at_mm_hdlr_cmd_tbl[ui4_lp].ps_cmd_abbr_str,
                        x_strlen(at_mm_hdlr_cmd_tbl[ui4_lp].ps_cmd_abbr_str)) )
        {
            if( x_strlen(at_mm_hdlr_cmd_tbl[ui4_lp].ps_cmd_abbr_str) > 0x0 )
            {
                return ui4_lp;
            }
        }
    }

    return SCPT_U4_INVALID;
}

static INT32 _scpt_parser_stt_chg( SCPT_DATA_T*      pt_scpt_data )
{
    INT32               i4_ret                  = CLIR_NOT_INIT;
    
    if(0x0 == x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_BASIC_TYPE_STR,
                        x_strlen(SCPT_BASIC_TYPE_STR)) )
    {
        pt_scpt_data->e_stt = SCPT_PARSER_GLB_STRING;
        i4_ret = CLIR_OK;
    }
    else if(0x0 == x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_BASIC_TYPE_U8,
                        x_strlen(SCPT_BASIC_TYPE_U8)) )
    {
        pt_scpt_data->e_stt = SCPT_PARSER_GLB_U8;
        i4_ret = CLIR_OK;
    }
    else if(0x0 == x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_TEST_CASE_FG,
                        x_strlen(SCPT_TEST_CASE_FG)) )
    {
        pt_scpt_data->e_stt = SCPT_PARSER_TEST_CASE;
        i4_ret = CLIR_OK;
    }
    else
    {
        pt_scpt_data->e_stt = SCPT_PARSER_USELESS_CHAR;
        i4_ret = CLIR_OK;
    }

    return i4_ret;
}


static INT32 _scpt_parser_glb_u8_stt_chg( SCPT_DATA_T*      pt_scpt_data )
{
    INT32               i4_ret                  = CLIR_NOT_INIT;

    pt_scpt_data->t_file.puc_crnt = 
                        _scpt_filter_useless_char(
                        pt_scpt_data->t_file.puc_crnt,
                        _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
    
    if(0x0 == x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_BASIC_TYPE_U8,
                        x_strlen(SCPT_BASIC_TYPE_U8)) )
    {
        pt_scpt_data->e_stt = SCPT_PARSER_GLB_STRING;
        i4_ret = CLIR_OK;
    }
    else if(0x0 == x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_BASIC_TYPE_STR,
                        x_strlen(SCPT_BASIC_TYPE_STR)) )
    {
        pt_scpt_data->e_stt = SCPT_PARSER_GLB_STRING;
        i4_ret = CLIR_OK;
    }
    else if(0x0 == x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_TEST_CASE_FG,
                        x_strlen(SCPT_TEST_CASE_FG)) )
    {
        pt_scpt_data->e_stt = SCPT_PARSER_TEST_CASE;
        i4_ret = CLIR_OK;
    }

    return i4_ret;
}

static SCPT_STRING_ARRY_TYPE_T* _scpt_parser_query_path_arry(
                                SCPT_DATA_T*        pt_scpt_data,
                                CHAR*               pc_path )
{
    CHAR*                       pc_fltr             = pc_path;
    CHAR*                       pc_end              = NULL;
    SCPT_STRING_ARRY_TYPE_T*    pt_str_arry_node    = NULL;
    
    if( NULL == pt_scpt_data )
    {
        return NULL;
    }
    
    /* must mo cmd*/
    if(0x0 != x_memcmp(SCPT_CMD_MO,pc_path,x_strlen(SCPT_CMD_MO) ) )
    {
        return NULL;
    }
    
    pc_end  = (pt_scpt_data->t_file.puc_start + pt_scpt_data->t_file.ui4_size);
    pc_fltr += x_strlen(SCPT_CMD_MO);
    
    pc_fltr = _scpt_filter_useless_char(pc_fltr,(pc_end - pc_fltr));
    
    pt_str_arry_node =  _scpt_query_str_node(
                                &pt_scpt_data->t_str_arry_type_list,
                                 pc_fltr);
    
    return pt_str_arry_node;
}

static INT32 _scpt_parser_get_one_gldn_val(
                            SCPT_GODLEN_INFO_T*         pt_gldn_val, 
                            SCPT_STRING_CLASS_T*        pt_str_ojb)
{
    CHAR*       puc_fltr    = pt_str_ojb->pc_str; 
    CHAR        ac_num[SCPT_NUM_LEN];
    CHAR*       puc_end     = (pt_str_ojb->pc_str + pt_str_ojb->ui4_len); 
    UINT32      ui4_len     =0x0;
    puc_fltr    = _scpt_filter_useless_char(puc_fltr,(puc_end - puc_fltr));
    
    if( SCPT_CHAR_LEFT_BRACKET != puc_fltr[0] )
    {
        x_dbg_stmt("one golden values invalid LEFT_BIG_BRACKET \n");
        return CLIR_INV_ARG;
    }
    puc_fltr++;
    
    puc_fltr    = _scpt_filter_useless_char(puc_fltr,(puc_end - puc_fltr));
    ui4_len     = _scpt_query_number_str_len(puc_fltr,(puc_end - puc_fltr));
    if( (0x0 == ui4_len) || (ui4_len >= SCPT_NUM_LEN) )
    {
        x_dbg_stmt("Golden Time invalid \n");
    }

    x_memset( ac_num, 0, SCPT_NUM_LEN);
    x_strncpy(ac_num,puc_fltr,ui4_len);
    
    pt_gldn_val->u4GoldenTime =  x_strtoll(ac_num,NULL,10);
    
    puc_fltr +=ui4_len; 
    puc_fltr    = _scpt_filter_useless_char(puc_fltr,(puc_end - puc_fltr));
    
    if( SCPT_CHAR_COMMA != puc_fltr[0] )
    {
        x_dbg_stmt("Golden Time invalid COMMA \n");
        return CLIR_INV_ARG;
    }

    puc_fltr++; 
    puc_fltr = _scpt_filter_useless_char(puc_fltr,(puc_end - puc_fltr));

    ui4_len     = _scpt_query_number_str_len(puc_fltr,(puc_end - puc_fltr));
    if( (0x0 == ui4_len) || (ui4_len >= SCPT_NUM_LEN) )
    {
        x_dbg_stmt("Golden Time invalid \n");
    }

    x_memset( ac_num, 0, SCPT_NUM_LEN);
    x_strncpy(ac_num,puc_fltr,ui4_len);
    
    pt_gldn_val->u4Diff =  x_strtoll(ac_num,NULL,10);
    
    puc_fltr +=ui4_len; 
    puc_fltr = _scpt_filter_useless_char(puc_fltr,(puc_end - puc_fltr));

    if( SCPT_CHAR_RIGHT_BRACKET != puc_fltr[0] )
    {
        x_dbg_stmt("one golden values invalid RIGHT_BIG_BRACKET \n");
        return CLIR_INV_ARG;
    }
    puc_fltr++;

    if(puc_fltr > puc_end)
    {
        x_dbg_stmt("one golden values out of memory \n");
        return CLIR_INV_ARG;
    }
    
    pt_str_ojb->pc_str  =puc_fltr;
    pt_str_ojb->ui4_len =(puc_end - puc_fltr);

    return CLIR_OK;
}

static INT32 _scpt_parser_set_golden_values(
                            SCPT_GODLEN_INFO_T*         pa_gldn_vals, 
                            SCPT_STRING_CLASS_T*        pt_str_ojb)
{
    INT32                   i4_ret  = CLIR_OK;
    UINT32                  ui4_idx = 0x0;
    CHAR*                   puc_end = (pt_str_ojb->pc_str + pt_str_ojb->ui4_len); 
    
    while(ui4_idx < AUTOTEST_MAXCASE_INDEX)
    {
        i4_ret = _scpt_parser_get_one_gldn_val(&pa_gldn_vals[ui4_idx], pt_str_ojb);
        if(CLIR_OK != i4_ret)
        {
            x_dbg_stmt("_scpt_parser_get_one_gldn_val fail\n");
            break;
        }
        
        pt_str_ojb->pc_str    = _scpt_filter_useless_char(
                                    pt_str_ojb->pc_str,
                                    (puc_end - pt_str_ojb->pc_str));
        pt_str_ojb->ui4_len   = (puc_end - pt_str_ojb->pc_str);
        if(pt_str_ojb->ui4_len < 5 )
        {
            break;
        }
        
        if( SCPT_CHAR_COMMA != pt_str_ojb->pc_str[0] )
        {
            x_dbg_stmt("_scpt_parser_get_one_gldn_val invalid COMMA \n");
            return CLIR_INV_ARG;
        }
        
        pt_str_ojb->pc_str++;
        ui4_idx++;
    }

    return i4_ret;
}

static INT32 _scpt_parser_get_golden_values(
                            SCPT_GODLEN_INFO_T          pa_gldn_vals[][AUTOTEST_MAXCASE_INDEX], 
                            SCPT_STRING_ARRY_TYPE_T*    pt_str_arry)
{
    INT32                   i4_ret              = CLIR_OK;
    SCPT_STRING_CLASS_T     t_str_ojb;
    
    pt_str_arry->ui4_idx = 0x0;
    
    while( pt_str_arry->ui4_idx < pt_str_arry->ui4_cnt )
    {
        i4_ret=_scpt_get_str_array_val(pt_str_arry,&t_str_ojb);
        if( CLIR_OK != i4_ret )
        {
            x_dbg_stmt("golden_values null \n");
            break;
        }
        
        i4_ret=_scpt_parser_set_golden_values(pa_gldn_vals[pt_str_arry->ui4_idx],&t_str_ojb);
        if( CLIR_OK != i4_ret )
        {
            x_dbg_stmt(" set golden_values fail \n");
            break;
        }
        
        pt_str_arry->ui4_idx++;
    }

    return CLIR_OK;
}

static INT32 _scpt_parser_do_cmd(SCPT_DATA_T*  pt_scpt_data,UINT32 ui4_cmd_idx )
{
    INT32                       i4_ret              = CLIR_OK;
    INT32                       i4_argc             = 0x0;
    SCPT_BASIC_DATA_TYPE_T      e_basic_data_type   = SCPT_BASIC_DATA_TYPE_NONE;
    UINT32                      ui4_len             = 0x0;
    UINT32                      ui4_cli_len         = 0x0;
    UINT32                      ui4_fltr_len        = 0x0;
    CHAR*                       pc_str              = NULL;
    CHAR*                       pc_fltr             = NULL;
    CHAR*                       puc_end             = NULL;
    CHAR*                       pps_argv[SCPT_PARA_CNT_MAX];
    SCPT_U8_TYPE_T*             pt_u8_node          = NULL;
    SCPT_STRING_ARRY_TYPE_T*    pt_str_arry_node    = NULL;
    SCPT_STRING_CLASS_T         t_str_ojb;
                
    if( NULL == pt_scpt_data )
    {
        return CLIR_NOT_INIT;
    }

    
    
    ui4_len         = _scpt_get_crnt_buf_size(&pt_scpt_data->t_file);
    ui4_fltr_len    = ui4_len;
    pc_str          = pt_scpt_data->t_file.puc_crnt;
    pc_fltr         = pc_str;
    puc_end         = ( pc_str + ui4_len );

    ui4_cli_len = x_strlen(at_mm_hdlr_cmd_tbl[ui4_cmd_idx].ps_cmd_abbr_str);
    if( 0x0 == ui4_cli_len )
    {
        x_dbg_stmt("cmd(%d) len is 0 (%s)!\n",
            ui4_cmd_idx,
            at_mm_hdlr_cmd_tbl[ui4_cmd_idx].ps_cmd_abbr_str);
        return CLIR_NOT_INIT;
    }

    if(0x0 != x_memcmp(
                pc_fltr,
                at_mm_hdlr_cmd_tbl[ui4_cmd_idx].ps_cmd_abbr_str,
                ui4_cli_len ) )
    {
        x_dbg_stmt("not found cmd(%s)\n",
                at_mm_hdlr_cmd_tbl[ui4_cmd_idx].ps_cmd_abbr_str);
        return CLIR_NOT_INIT;
    }


    /* rebuild cli cmd */
    
    i4_argc             = 0x0;
    pps_argv[i4_argc]   = x_mem_alloc( ui4_cli_len + 1 ); 
    if( NULL == pps_argv[i4_argc] )
    {
        x_dbg_stmt("rebuild cli alloc fail !\n");
        return CLIR_NOT_INIT;
    }
    
    x_memset( pps_argv[i4_argc], 0, ( ui4_cli_len + 1 ) );
    x_strncpy(pps_argv[i4_argc],pc_fltr,ui4_cli_len);
    x_dbg_stmt("+++++++cmd: %s\n",pps_argv[i4_argc]);
    i4_argc++;
            
    pc_fltr += x_strlen(at_mm_hdlr_cmd_tbl[ui4_cmd_idx].ps_cmd_abbr_str);
    
    /* parser parameter thorugh space  */
    while( (pc_fltr < puc_end) && (SCPT_CHAR_SEMICOLON != pc_fltr[0]) )
    {
        pc_fltr         = _scpt_filter_useless_char(pc_fltr,ui4_fltr_len);
        ui4_fltr_len    = (puc_end - pc_fltr);

        if( SCPT_CHAR_SEMICOLON == pc_fltr[0])
        {
            x_dbg_stmt("cmd para success \n");
            i4_ret =  CLIR_OK;
            break;
        }
        
        e_basic_data_type = SCPT_BASIC_DATA_TYPE_NONE;
        x_memset( &t_str_ojb, 0, sizeof(SCPT_STRING_CLASS_T) );
        /* search vara list */
        
        pt_u8_node = _scpt_query_u8_node(&pt_scpt_data->t_b8_list,pc_fltr);
            
        if(NULL == pt_u8_node)
        {
            /* 2. search str list */
            pt_str_arry_node =  _scpt_query_str_node(
                                    &pt_scpt_data->t_str_arry_type_list,
                                    pc_fltr);
            if( NULL != pt_str_arry_node )
            {
                if( 0x0 == x_memcmp(
                            SCPT_CMD_GGV,
                            at_mm_hdlr_cmd_tbl[ui4_cmd_idx].ps_cmd_abbr_str,
                            x_strlen(SCPT_CMD_GGV) ) )
                {
                    i4_ret = _scpt_parser_get_golden_values(
                                                            _grGoldenInfo,
                                                            pt_str_arry_node);
                }
                else
                {
                    i4_ret=_scpt_get_str_array_val(pt_str_arry_node,&t_str_ojb);
#ifdef VDEC_TIME_PROFILE
                    fgAutoPlayRecordFileName = TRUE;
#endif
                }
                
                if( CLIR_OK != i4_ret )
                {
                    break;
                }
                
                e_basic_data_type = SCPT_BASIC_DATA_TYPE_STR;
                pc_fltr += x_strlen(pt_str_arry_node->pac_str_arry_name);
                ui4_fltr_len      = ( puc_end - pc_fltr );
            } 
        }
        else
        {
            t_str_ojb.pc_str    = pt_u8_node->pac_val;
            t_str_ojb.ui4_len   = x_strlen(pt_u8_node->pac_val);
            e_basic_data_type   = SCPT_BASIC_DATA_TYPE_NUM;
            pc_fltr            += x_strlen(pt_u8_node->pac_vara);
            ui4_fltr_len        = ( puc_end - pc_fltr );
        }
            
        if( SCPT_BASIC_DATA_TYPE_NONE == e_basic_data_type )
        {
            ui4_fltr_len    = (puc_end - pc_fltr);
            
            if( SCPT_CHAR_DQM == pc_fltr[0] )
            {
                ui4_fltr_len    = (puc_end - pc_fltr);
                
                ui4_len         = _scpt_parser_query_dq_str_len(pc_fltr,ui4_fltr_len);

                if( ui4_len <= 0x2 )
                {
                    x_dbg_stmt("query_dq_str_len null !\n");
                    i4_ret =  CLIR_NOT_INIT;
                    break;
                }
                
                e_basic_data_type = SCPT_BASIC_DATA_TYPE_VAL;
                t_str_ojb.ui4_len = ( ui4_len - 0x2 );
                t_str_ojb.pc_str  = ( pc_fltr + 0x1 );
                
            }
            else if( TRUE == _scpt_is_number_char(pc_fltr) )
            {
                ui4_len     = _scpt_query_str_len(pc_fltr,ui4_fltr_len);
                
                if( 0x0 == ui4_len )
                {
                    x_dbg_stmt("cmd para num_str len null !\n");
                    i4_ret =  CLIR_NOT_INIT;
                    break;
                }
                
                e_basic_data_type = SCPT_BASIC_DATA_TYPE_VAL;
                t_str_ojb.ui4_len = ui4_len;
                t_str_ojb.pc_str  = pc_fltr;
            }
            else
            {
                x_dbg_stmt("cmd para invalid !\n");
                i4_ret =  CLIR_NOT_INIT;
                break;
            }
            
            pc_fltr         += ui4_len;
            ui4_fltr_len    =  ( puc_end - pc_fltr );
        }
        else
        {
        }

        if(t_str_ojb.ui4_len && t_str_ojb.pc_str)
        {
            pps_argv[i4_argc] = x_mem_alloc( t_str_ojb.ui4_len + 1 ); 
            if(NULL == pps_argv[i4_argc])
            {
                i4_ret =  CLIR_NOT_INIT;
                break;
            }
            
            x_memset( pps_argv[i4_argc], 0, ( t_str_ojb.ui4_len + 1 ) );
            x_strncpy(pps_argv[i4_argc],t_str_ojb.pc_str,t_str_ojb.ui4_len);
            x_dbg_stmt("para(%d):%s\n",i4_argc,pps_argv[i4_argc]);

#ifdef VDEC_TIME_PROFILE
            {
                INT32 i = 0;

                if ( fgAutoPlayRecordFileName )
                {
                    x_memset(ucAutoPlayCurFileDir, 0x0, sizeof(ucAutoPlayCurFileDir));
                    x_memset(ucAutoPlayCurFileName, 0x0, sizeof(ucAutoPlayCurFileName));

                    for ( i = t_str_ojb.ui4_len; i > 0; i-- )
                    {
                        if ( t_str_ojb.pc_str[i] == '/' )
                        {
                            break;
                        }
                    }

                    x_strncpy(ucAutoPlayCurFileDir, t_str_ojb.pc_str + 16, i - 16); //Ignore the pre string "mnt/usb/Mass-000/"
                    x_strncpy(ucAutoPlayCurFileName, t_str_ojb.pc_str + i + 1, t_str_ojb.ui4_len - i - 1);

                    for ( i=0; i<x_strlen((CHAR *)ucAutoPlayCurFileDir); i++ )
                    {
                        if ( ucAutoPlayCurFileDir[i] == '/' )
                        {
                            ucAutoPlayCurFileDir[i] = '\\';
                        }
                    }

                    fgAutoPlayRecordFileName = FALSE;
                }
            }
#endif

            i4_argc++;
        }
        
        if( SCPT_CHAR_SEMICOLON == pc_fltr[0] )
        {
            i4_ret =  CLIR_OK;
            break;
        }
        
        if( SCPT_BASIC_DATA_TYPE_NONE != e_basic_data_type )
        {
            if(TRUE != _scpt_is_para_divide_char(pc_fltr))
            {
                x_dbg_stmt("cmd para divide invalid !\n");
                i4_ret =  CLIR_NOT_INIT;
                break;
            }
        }
    }
    
    if( CLIR_OK == i4_ret )
    {
        if( SCPT_CHAR_SEMICOLON == pc_fltr[0] )
        {
            if( TRUE != pt_scpt_data->t_tc.b_check )
            {
                i4_ret = at_mm_hdlr_cmd_tbl[ui4_cmd_idx].pf_exec_fct(
                            i4_argc,
                            (const CHAR**)pps_argv);
                #if 0
                x_dbg_stmt(
                            "do cmd (%s) i4_ret(%d)\n",
                            at_mm_hdlr_cmd_tbl[ui4_cmd_idx].ps_cmd_abbr_str,
                            i4_ret );
                #endif 
                x_thread_delay(10);
            }
        }
        else
        {
            x_dbg_stmt("no do invalid cmd:(%s)\n",
                    at_mm_hdlr_cmd_tbl[ui4_cmd_idx].ps_cmd_abbr_str);
        }
    }
    else
    {
        x_dbg_stmt("no do cmd:(%s)\n",
                    at_mm_hdlr_cmd_tbl[ui4_cmd_idx].ps_cmd_abbr_str);
    }
    
    /* free cmd memory */
    while( i4_argc > 0x0 )
    {
        x_mem_free(pps_argv[i4_argc-1]);
        pps_argv[i4_argc-1] = NULL;
        i4_argc--;
    };
    
    return i4_ret;
}

static INT32 _scpt_parser_one_cmd( SCPT_DATA_T*      pt_scpt_data )
{
    UINT32                      ui4_cmd             = 0x0;
    INT32                       i4_ret              = CLIR_OK;

    pt_scpt_data->t_file.puc_crnt = 
                    _scpt_filter_useless_char(
                    pt_scpt_data->t_file.puc_crnt,
                    _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
    
    ui4_cmd=_scpt_parser_check_test_case_cmd(pt_scpt_data->t_file.puc_crnt);
    
    if( SCPT_U4_INVALID != ui4_cmd )
    {
        i4_ret = _scpt_parser_do_cmd(pt_scpt_data,ui4_cmd);
        if(CLIR_OK != i4_ret)
        {
            x_dbg_stmt("cmd do fail \n");
            return i4_ret;
        }
        
        pt_scpt_data->t_file.puc_crnt = 
                _scpt_parser_filter_to_semicolon(
                pt_scpt_data->t_file.puc_crnt,
                _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
        
        if(NULL == pt_scpt_data->t_file.puc_crnt)
        {
            x_dbg_stmt("cmd end invalid \n");
            i4_ret = CLIR_NOT_INIT;
        }
        else
        {
            pt_scpt_data->t_file.puc_crnt += 0x1;
        }
    }

     return i4_ret;
}

static INT64 _scpt_parser_get_bracket_num( SCPT_DATA_T*      pt_scpt_data )
{
    CHAR*               pc_fltr         = NULL;
    CHAR*               pc_end          = NULL;
    SCPT_U8_TYPE_T*     pt_u8_node      = NULL;
    UINT32              ui4_num_len     = 0x0;
    CHAR*               pc_num_str      = NULL;
    INT64               i8_val          = 0x0;
    SCPT_STRING_CLASS_T t_str_obj       = {0};
    
    if( NULL == pt_scpt_data )
    {
        return SCPT_U4_INVALID;
    }
    
    pc_fltr = pt_scpt_data->t_file.puc_crnt;
    pc_end  = (pt_scpt_data->t_file.puc_start + pt_scpt_data->t_file.ui4_size);
    
    pc_fltr = _scpt_filter_useless_char( pc_fltr , (pc_end - pc_fltr) );

    if( SCPT_CHAR_LEFT_BRACKET != pc_fltr[0])
    {
        x_dbg_stmt("loop_cmd left bracket invalid \n");
        return SCPT_U4_INVALID;
    }
    
    pc_fltr++;
    
    pc_fltr = _scpt_filter_useless_char( pc_fltr , (pc_end - pc_fltr) );

    /* query u8 list */
    pt_u8_node = _scpt_query_u8_node(&pt_scpt_data->t_b8_list,pc_fltr);

    if( NULL == pt_u8_node )
    {
        /* check is number str */
        ui4_num_len = _scpt_query_number_str_len(pc_fltr,(pc_end - pc_fltr));
        if( 0x0 == ui4_num_len )
        {
            return SCPT_U4_INVALID;
        }

        t_str_obj.pc_str    = pc_fltr;
        t_str_obj.ui4_len   = ui4_num_len;
        pc_fltr             += ui4_num_len;
    }
    else
    {
        t_str_obj.pc_str    = pt_u8_node->pac_val;
        t_str_obj.ui4_len   = x_strlen(pt_u8_node->pac_val);
        pc_fltr             += x_strlen(pt_u8_node->pac_vara);;
    }
    
    pc_fltr = _scpt_filter_useless_char( pc_fltr , (pc_end - pc_fltr) );
    if( SCPT_CHAR_RIGHT_BRACKET != pc_fltr[0] )
    {
        x_dbg_stmt("loop_cmd right bracket invalid \n");
        return SCPT_U4_INVALID;
    }

    /* get loop count */
    pc_num_str = x_mem_alloc( t_str_obj.ui4_len + 1 ); 
    if(NULL == pc_num_str)
    {
        x_dbg_stmt("loop malloc fail !\n");
        return SCPT_U4_INVALID;
    }
    
    x_memset( pc_num_str, 0, ( t_str_obj.ui4_len + 1 ) );
    x_strncpy(pc_num_str,t_str_obj.pc_str,t_str_obj.ui4_len);

    i8_val = x_strtoll(pc_num_str,NULL,10);

    x_mem_free(pc_num_str);
    pc_num_str = NULL;
    
    return i8_val;
}

static CHAR* _scpt_parser_fltr_big_bracket( CHAR* puc_str , UINT32 ui4_len )
{
    CHAR*               pc_fltr         = puc_str;
    CHAR*               pc_end          = (puc_str + ui4_len);

    while( (SCPT_CHAR_LEFT_BIG_BRACKET != pc_fltr[0]) && (pc_fltr < pc_end) )
    {
        pc_fltr++;
    }

    if(pc_fltr >= pc_end)
    {
        return NULL;    
    }
    
    return pc_fltr;    
}

static INT32 _scpt_parser_multi_cmd( SCPT_DATA_T*      pt_scpt_data )
{
    INT32               i4_ret          = CLIR_OK;
    
    if( NULL == pt_scpt_data )
    {
        return CLIR_NOT_INIT;
    }
    
    if(SCPT_CHAR_LEFT_BIG_BRACKET != pt_scpt_data->t_file.puc_crnt[0])
    {
        x_dbg_stmt("loop multi cmd invalid \n");
        return CLIR_NOT_INIT;
    }

    pt_scpt_data->t_file.puc_crnt +=0x1;

    do
    {
        pt_scpt_data->t_file.puc_crnt = 
                    _scpt_filter_useless_char(
                    pt_scpt_data->t_file.puc_crnt,
                    _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
        
        if(SCPT_CHAR_RIGHT_BIG_BRACKET == pt_scpt_data->t_file.puc_crnt[0])
        {
            i4_ret = CLIR_OK;
            break;
        }
        
        i4_ret = _scpt_parser_one_cmd(pt_scpt_data);
        if(CLIR_OK != i4_ret)
        {
            x_dbg_stmt("_scpt_parser_loop_cmd fail(%d)\n",i4_ret);
            return i4_ret;
        }
    }while(1);
            
    pt_scpt_data->t_file.puc_crnt +=0x1;
    return CLIR_OK;
}

static INT32 _scpt_parser_loop_cmd( SCPT_DATA_T*      pt_scpt_data )
{
    CHAR*               pc_fltr         = NULL;
    CHAR*               pc_end          = NULL;
    UINT32              ui4_num_val     = 0x0;
    UINT32              i               = 0x0;
    INT32               i4_ret          = CLIR_NOT_INIT;
    
    if( NULL == pt_scpt_data )
    {
        return CLIR_NOT_INIT;
    }
    
    
    pc_end  = (pt_scpt_data->t_file.puc_start + pt_scpt_data->t_file.ui4_size);
    
    if(0x0 != x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_KEY_LOOP,
                        x_strlen(SCPT_KEY_LOOP)))
    {
        return CLIR_NOT_INIT;
    }
    
    pt_scpt_data->t_file.puc_crnt += x_strlen(SCPT_KEY_LOOP);
    
    ui4_num_val = (UINT32)_scpt_parser_get_bracket_num(pt_scpt_data);
    if( SCPT_U4_INVALID == ui4_num_val )
    {
        x_dbg_stmt("loop num invalid \n");
        return CLIR_NOT_INIT;
    }
    
    pc_fltr = pt_scpt_data->t_file.puc_crnt;
    pc_fltr = _scpt_parser_fltr_big_bracket(pc_fltr,(pc_end - pc_fltr));
    if( NULL == pc_fltr )
    {
        x_dbg_stmt("loop big bracket invalid \n");
        return CLIR_NOT_INIT;
    }
    
    for(i = 0x0; i < ui4_num_val; i++)
    {
        pt_scpt_data->t_file.puc_crnt = pc_fltr;
        
        i4_ret = _scpt_parser_multi_cmd(pt_scpt_data);
        if(CLIR_OK != i4_ret)
        {
            x_dbg_stmt("_scpt_parser_loop_cmd fail(%d)\n",i);
            return i4_ret;
        }
    }
    
    return CLIR_OK;
}

static INT32 _scpt_parser_one_test_case( SCPT_DATA_T*      pt_scpt_data )
{
    INT32                       i4_ret              = CLIR_OK;
    
    do
    {
        pt_scpt_data->t_file.puc_crnt = 
                    _scpt_filter_useless_char(
                    pt_scpt_data->t_file.puc_crnt,
                    _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
        
        _scpt_parser_tc_cmd_stt_chg(pt_scpt_data);
        
        switch(pt_scpt_data->t_tc.e_cmd_stt)
        {
            case TEST_CASE_CMD_SEQ:
                i4_ret = _scpt_parser_one_cmd(pt_scpt_data);
                break;
            case TEST_CASE_CMD_LOOP:
                i4_ret = _scpt_parser_loop_cmd(pt_scpt_data);
                break;
            case TEST_CASE_CMD_END:
                return CLIR_OK;
             default:
                i4_ret =  CLIR_NOT_INIT;
        }
        
        if(CLIR_OK != i4_ret)
        {
            break;
        }
    }while(1);

    return i4_ret;
}

static INT32 _scpt_parser_test_case_eng( SCPT_DATA_T*      pt_scpt_data )
{
    UINT32                      ui4_fp              = 0x0;
    UINT32                      i                   = 0x0;
    INT32                       i4_ret              = CLIR_NOT_INIT;
    CHAR*                       pc_tc_st            = NULL;
    SCPT_STRING_ARRY_TYPE_T*    pt_str_arry_node    = NULL;
    
    /* save test case start address to multi test every path of string-array-path */
    UINT8 u1FileIdx = 0;
    UINT8 u1TestCase = 0;
    pt_scpt_data->t_file.puc_crnt = 
                    _scpt_filter_useless_char(
                    pt_scpt_data->t_file.puc_crnt,
                    _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
    pc_tc_st = pt_scpt_data->t_file.puc_crnt;
    
    if(0x0 != x_memcmp(SCPT_CMD_MO,pc_tc_st,x_strlen(SCPT_CMD_MO) ) )
    {
        return CLIR_NOT_INIT;
    }
    
    pt_str_arry_node = _scpt_parser_query_path_arry(pt_scpt_data,pc_tc_st);
    
    if(NULL != pt_str_arry_node)
    {
        ui4_fp = pt_str_arry_node->ui4_cnt;
    }
    else
    {
        ui4_fp = 0x1;
    }
    for(u1FileIdx =0; u1FileIdx<AUTOTEST_MAX_FILE;u1FileIdx++)
    {
        for(u1TestCase =0; u1TestCase<AUTOTEST_MAXCASE_INDEX;u1TestCase++)
        {
            rScptInfo.afgIsFail[u1FileIdx][u1TestCase] = 0;
        }
    }
    for( i = 0x0; i < ui4_fp; i++ )
    {
        pt_scpt_data->t_file.puc_crnt = pc_tc_st;
        
        if(NULL != pt_str_arry_node)
        {
            /* update str array read index */
            pt_str_arry_node->ui4_idx = i;
            rScptInfo.u4FileIndex=i;
        }
        
        i4_ret = _scpt_parser_one_test_case(pt_scpt_data);
        
        pt_scpt_data->t_tc.ui4_tc_sum++;
        if((CLIR_OK != i4_ret) || (!rScptInfo.fgCaseResult))
        {
            pt_scpt_data->t_tc.ui4_failed++;
            x_dbg_stmt("TC(%d) Failed Count(%d)\n",rScptInfo.u4CurId,pt_scpt_data->t_tc.ui4_failed);
        }
        else
        {
            x_dbg_stmt("TC(%d) Pass\n",rScptInfo.u4CurId);
        }
    }

    return i4_ret;
}

static void _scpt_parser_tc_cmd_stt_chg( SCPT_DATA_T*      pt_scpt_data )
{
    pt_scpt_data->t_file.puc_crnt = 
                _scpt_filter_useless_char(
                pt_scpt_data->t_file.puc_crnt,
                _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
    
    if(0x0 == x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_KEY_LOOP,
                        x_strlen(SCPT_KEY_LOOP)) )
    {
        pt_scpt_data->t_tc.e_cmd_stt = TEST_CASE_CMD_LOOP;
    }
    else if(0x0 == x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_KEY_END,
                        x_strlen(SCPT_KEY_END)) )
    {
        pt_scpt_data->t_tc.e_cmd_stt = TEST_CASE_CMD_END;
    }
    else
    {
        pt_scpt_data->t_tc.e_cmd_stt = TEST_CASE_CMD_SEQ;
    }
    
    return ;
}

static INT32 _scpt_parser_test_case_stt_chg( SCPT_DATA_T*      pt_scpt_data )
{
    INT32               i4_ret                  = CLIR_NOT_INIT;
    
    
    pt_scpt_data->t_file.puc_crnt = 
                        _scpt_filter_useless_char(
                        pt_scpt_data->t_file.puc_crnt,
                        _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
    
    if(0x0 == x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_KEY_START,
                        x_strlen(SCPT_KEY_START)) )
    {
        pt_scpt_data->t_tc.e_stt = TEST_CASE_START;
        i4_ret = CLIR_OK;
    }
    else if(0x0 == x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_KEY_LOOP,
                        x_strlen(SCPT_KEY_LOOP)) )
    {
        pt_scpt_data->t_tc.e_stt = TEST_CASE_LOOP;
        i4_ret = CLIR_OK;
    }
    else if(0x0 == x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_KEY_END,
                        x_strlen(SCPT_KEY_END)) )
    {
        pt_scpt_data->t_tc.e_stt = TEST_CASE_END;
        i4_ret = CLIR_OK;
    }
    else if(0x0 == x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_TEST_CASE_FG,
                        x_strlen(SCPT_TEST_CASE_FG)) )
    {
        pt_scpt_data->t_tc.e_stt = TEST_CASE_NAME;
        i4_ret = CLIR_OK;
    }
    else
    {

    }
    
    return i4_ret;
}


static INT32 _scpt_parser_all_str_array_stt_chg( SCPT_DATA_T*      pt_scpt_data )
{
    INT32               i4_ret;
    
    i4_ret = _scpt_parser_glb_u8_stt_chg(pt_scpt_data);

    return i4_ret;
}

static INT32 _scpt_parser_test_case( SCPT_DATA_T*           pt_scpt_data )
{
    INT32               i4_ret                  = CLIR_OK;
    UINT32              ui4_len                 = 0x0;
    CHAR*               pc_tc_name              = NULL;
    
        
    if( NULL == pt_scpt_data )
    {
        return CLIR_NOT_INIT;
    }
    
    if(0x0 != x_memcmp( pt_scpt_data->t_file.puc_crnt,
                        SCPT_TEST_CASE_FG,
                        x_strlen(SCPT_TEST_CASE_FG)) )
    {
        x_dbg_stmt("_scpt_parser_test_case null !\n");
        return CLIR_NOT_INIT;
    }
    
    do
    {
        _scpt_parser_test_case_stt_chg(pt_scpt_data);
        
        switch( pt_scpt_data->t_tc.e_stt )
        {
            case TEST_CASE_NAME:
                if(NULL != pc_tc_name)
                {
                    x_dbg_stmt("_scpt_parser_test_case tested !\n");
                    i4_ret = CLIR_NOT_INIT;
                    break;
                }
                //get name strlen
                ui4_len = _scpt_parser_query_mid_bracket_str_len(
                            pt_scpt_data->t_file.puc_crnt,
                            _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
                
                if( ui4_len <= (x_strlen(SCPT_TEST_CASE_FG) + 0x2) )
                {
                    x_dbg_stmt("_scpt_parser_test_case name invalid !\n");
                    i4_ret = CLIR_NOT_INIT;
                    break;
                }
                
                pc_tc_name = x_mem_alloc( ui4_len + 1 ); 
                if(NULL == pc_tc_name)
                {
                    x_dbg_stmt("_scpt_parser_test_case name malloc fail !\n");
                    i4_ret = CLIR_NOT_INIT;
                    break;
                }
                
                x_memset( pc_tc_name, 0, ( ui4_len + 1 ) );
                x_strncpy(pc_tc_name,pt_scpt_data->t_file.puc_crnt,ui4_len);
                
                // filter test case name
                pt_scpt_data->t_file.puc_crnt += ui4_len;
                i4_ret = CLIR_OK;
                break;
            case TEST_CASE_START:
                pt_scpt_data->t_file.puc_crnt += x_strlen(SCPT_KEY_START);
                i4_ret = _scpt_parser_test_case_eng(pt_scpt_data);
                break;
            case TEST_CASE_END:
                pt_scpt_data->t_file.puc_crnt += x_strlen(SCPT_KEY_END);
                i4_ret = CLIR_OK;
                break;
            case TEST_CASE_ERROR:
            default:
                break;
        }

        if( CLIR_OK != i4_ret )
        {
            x_dbg_stmt("### test case: (%s) fail ### \n",pc_tc_name);
            break;
        }

        if( TEST_CASE_END == pt_scpt_data->t_tc.e_stt)
        {
            x_dbg_stmt("$$$ test case: (%s) OK $$$ \n",pc_tc_name);
            break;
        }
    }while(1);
    
    return i4_ret;
}



static INT32 _scpt_parser_all_str_array( SCPT_DATA_T*           pt_scpt_data )
{
    INT32               i4_ret                  = CLIR_OK;
    INT32               i4_ret_cmp              = CLIR_OK;
    
    do
    {
        /* 1. filter useless char */
        
        pt_scpt_data->t_file.puc_crnt = 
                        _scpt_filter_useless_char(
                        pt_scpt_data->t_file.puc_crnt,
                        _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
        
        i4_ret_cmp =   x_memcmp(pt_scpt_data->t_file.puc_crnt,
                                SCPT_BASIC_TYPE_STR,
                                x_strlen(SCPT_BASIC_TYPE_STR) );
        x_dbg_stmt("_scpt_parser_all_str_array i4_ret_cmp %d \n",i4_ret_cmp);
        
        if( 0x0 == i4_ret_cmp )
        {
            /* 3. parser one u8 vara */
            i4_ret = _scpt_parser_one_str_array(pt_scpt_data);
            if(CLIR_OK != i4_ret)
            {
                x_dbg_stmt("_scpt_parser_one_str_array one Failed !\n");
                break;
            }
        }
        else
        {
            break;
        }
        
    }while(0);
    
    return i4_ret;
}


static INT32 _scpt_parser_all_glb_u8_vara( SCPT_DATA_T*      pt_scpt_data )
{
    INT32               i4_ret                  = CLIR_OK;
    INT32               i4_ret_cmp              = CLIR_OK;
    
    do
    {
        /* 1. filter useless char */
        
        // SCPT_FILTER_USELESS_CHAR(pt_scpt_data);
        pt_scpt_data->t_file.puc_crnt = 
                _scpt_filter_useless_char(
                        pt_scpt_data->t_file.puc_crnt,
                        _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
        
        i4_ret_cmp =   x_memcmp(pt_scpt_data->t_file.puc_crnt,
                                SCPT_BASIC_TYPE_U8,
                                x_strlen(SCPT_BASIC_TYPE_U8) );
        
        x_dbg_stmt("_scpt_parser_all_glb_u8_vara i4_ret_cmp %d \n",i4_ret_cmp);
        
        if( 0x0 == i4_ret_cmp )
        {
            /* 3. parser one u8 vara */
            i4_ret = _scpt_parser_one_glb_u8_vara(pt_scpt_data);
            if(CLIR_OK != i4_ret)
            {
                x_dbg_stmt("_scpt_parser_all_glb_u8_vara one Failed !\n");
                break;
            }
        }
        else
        {
            break;
        }
        
    }while(0);
    
    return i4_ret;
}

static INT32 _scpt_parser(SCPT_DATA_T*      pt_scpt_data)
{
    INT32               i4_ret                  = CLIR_OK;
    
    do
    {   
        _scpt_parser_stt_chg(pt_scpt_data);
    
        switch(pt_scpt_data->e_stt)
        {
            case SCPT_PARSER_USELESS_CHAR:
                pt_scpt_data->t_file.puc_crnt = 
                        _scpt_filter_useless_char(
                        pt_scpt_data->t_file.puc_crnt,
                        _scpt_get_crnt_buf_size(&pt_scpt_data->t_file));
                break;
            case SCPT_PARSER_GLB_U8:
                /* parser u8 data */
                i4_ret = _scpt_parser_all_glb_u8_vara(pt_scpt_data);
                if(CLIR_OK != i4_ret)
                {
                    x_dbg_stmt("_scpt_parser all_glb_u8_vara Failed !\n");
                    break;
                }
                
                i4_ret = _scpt_parser_glb_u8_stt_chg(pt_scpt_data);
                if(CLIR_OK != i4_ret)
                {
                    x_dbg_stmt("_scpt_parser_glb_u8_stt_chg Failed !\n");
                    break;
                }
                break;
            case SCPT_PARSER_GLB_STRING:
                /* parser string array */
                i4_ret = _scpt_parser_all_str_array(pt_scpt_data);
                if(CLIR_OK != i4_ret)
                {
                    x_dbg_stmt("_scpt_parser _scpt_parser_all_str_array Failed !\n");
                    break;
                }
                
                i4_ret = _scpt_parser_all_str_array_stt_chg(pt_scpt_data);
                if(CLIR_OK != i4_ret)
                {
                    x_dbg_stmt("_scpt_parser_all_str_array_stt_chg Failed !\n");
                }
                break;
            case SCPT_PARSER_TEST_CASE:
                i4_ret = _scpt_parser_test_case(pt_scpt_data);
                if(CLIR_OK != i4_ret)
                {
                    x_dbg_stmt("tc Failed !\n");
                }
                break;
            case SCPT_PARSER_ERROR:
            default:
                x_dbg_stmt("_scpt_parser ERROR !\n");
                break;
        }

        if( 0x0 == _scpt_get_crnt_buf_size(&pt_scpt_data->t_file) )
        {
            x_dbg_stmt("_scpt_parser Finished !\n");
            i4_ret = CLIR_OK;
            break;
        }
        
        if(CLIR_OK != i4_ret)
        {
            x_dbg_stmt("_scpt_parser ERROR !\n");
            break;
        }
        
        /* parser test case */
    }while(1);

    return i4_ret;
}

static INT32 _scpt_parser_init(SCPT_DATA_T*         pt_scpt_data)
{
    _scpt_u8_list_init(&pt_scpt_data->t_b8_list);
    _scpt_str_arry_type_list_init(&pt_scpt_data->t_str_arry_type_list);
    
    return CLIR_OK;
}

static INT32 _scpt_parser_deinit(SCPT_DATA_T*       pt_scpt_data)
{
    UINT32 u4CurId =0;
    UINT32 u4CurFileId =0;
    BOOL fgIsFail =FALSE;
    x_dbg_stmt("TC Report:\n   Total Count(%d)\n   Failed Count(%d)\n",
                pt_scpt_data->t_tc.ui4_tc_sum,
                pt_scpt_data->t_tc.ui4_failed);
    for(u4CurFileId = 0; u4CurFileId <AUTOTEST_MAX_FILE; u4CurFileId++)
    {
        fgIsFail =FALSE;
        for(u4CurId = 0; u4CurId <AUTOTEST_MAXCASE_INDEX; u4CurId++)
        {
            if(rScptInfo.afgIsFail[u4CurFileId][u4CurId])
            {
                fgIsFail =TRUE;
            }
        }
    
        if(fgIsFail)
        {
            x_dbg_stmt("\nFile index:(%d)\n",
                u4CurFileId);
            for(u4CurId = 0; u4CurId <AUTOTEST_MAXCASE_INDEX; u4CurId++)
            {
                if(rScptInfo.afgIsFail[u4CurFileId][u4CurId])
                {
                    x_dbg_stmt("Test index:(%d)\n  %s\n",
                    u4CurId,
                    rScptInfo.u1FailMessage[u4CurId]);
                }
            }
        }
    }
    /* free b8 list  */
    _scpt_u8_list_free(&pt_scpt_data->t_b8_list);
     
    /* free string array list  */
    _scpt_str_arry_type_list_free(&pt_scpt_data->t_str_arry_type_list);
    
    return CLIR_OK;
}

static VOID _mm_hdlr_mm_autotest_callback(VOID* pv_nfy_tag,
   MM_AUTOTEST_COND_T  e_nfy_cond,
   UINT32              ui4_data_1,
   UINT32              ui4_data_2)
{
   SCPT_INFO_T *prScptInfo=(SCPT_INFO_T *)pv_nfy_tag;
   if(prScptInfo==NULL ||prScptInfo->hCaseSema==0)
   {
       x_dbg_stmt("Autotest not inited\n");
       return;
   }

   if(prScptInfo->u4CurId!=ui4_data_1)
   {
       x_dbg_stmt("Test Id not match:(%d,%d)\n",prScptInfo->u4CurId,ui4_data_1);
       e_nfy_cond=MM_AUTOTEST_COND_INVALID;
   }

   if(e_nfy_cond==MM_AUTOTEST_COND_CASE_DONE)
   {
      rScptInfo.fgResult=(BOOL)ui4_data_2;
      if(rScptInfo.fgResult)
      {
        x_dbg_stmt("<<<<Finish test id: %d pass\n",prScptInfo->u4CurId);
      }
      else
      {
          x_dbg_stmt("<<<<Finish test id: %d fail\n",prScptInfo->u4CurId);
          rScptInfo.afgIsFail[rScptInfo.u4FileIndex][prScptInfo->u4CurId] = TRUE;
      }
   }
   else if(e_nfy_cond==MM_AUTOTEST_COND_GOLDEN_DONE)
   {
      x_dbg_stmt("<<<<Index(%d),Golden Value = %u\n",rScptInfo.u4CurId,ui4_data_2);
      rScptInfo.fgResult=TRUE;
#if AUTOTEST_GETGOLDEN_TIME
      //_grGoldenTime[rScptInfo.u4LoopIndex][prScptInfo->u4CurId]=ui4_data_2;

      _grGoldenTime_cur[prScptInfo->u4CurId] = ui4_data_2;
      
      if(ui4_data_2 > _grGoldenTime_Max[prScptInfo->u4CurId])
      {
          _grGoldenTime_Max[prScptInfo->u4CurId] = ui4_data_2;
      }
      if((ui4_data_2 < _grGoldenTime_Min[prScptInfo->u4CurId])||(_grGoldenTime_Min[prScptInfo->u4CurId] == 0xffffffff))
      {
          _grGoldenTime_Min[prScptInfo->u4CurId] = ui4_data_2;
      }

      
#endif
   }
   else
   {
      rScptInfo.fgResult=FALSE;
      x_dbg_stmt("Invalid Callback Type %d\n",e_nfy_cond);
   }
   rScptInfo.fgWaitCallback=FALSE;
   rScptInfo.fgCaseResult &= rScptInfo.fgResult; 
   if(rScptLock)
   {
       rScptLock = FALSE;
       if(x_sema_unlock(prScptInfo->hCaseSema) != OSR_OK)
       {
           x_dbg_stmt("Sema Unlock Error\n");
           rScptLock = TRUE;
       }
   }   
}
 
static INT32 _mm_hdlr_mm_to_drv_pipe(INT32 i4_argc, const CHAR** pps_argv)
{
    MM_AUTOTEST_INFO_T rAutoTestInfo;
    MM_AUTOTEST_TYPE_T e_cmd_type;
    INT32 i4_ret;
    UINT32 u4Param;
    BOOL fgFirsInit=FALSE;
    //x_dbg_stmt("mm_to_drv_pipe \n");
    // 1-->type 2-->id  3-->duration 4-->Goldentime, 5-->diftime
    
    u4Param = x_strtoll(pps_argv[1],NULL,10);
    e_cmd_type=(MM_AUTOTEST_TYPE_T)u4Param;
    rAutoTestInfo.e_cmd_type=e_cmd_type;
    rScptInfo.u4CurType=e_cmd_type;
    u4Param = x_strtoll(pps_argv[2],NULL,10);
    rScptInfo.u4CurId=u4Param;
    if(e_cmd_type==MM_AUTOTEST_MAKEGOLDEN
        || e_cmd_type==MM_AUTOTEST_TIMECASE
        || e_cmd_type==MM_AUTOTEST_CRCCHECK
        ||e_cmd_type==MM_AUTOTEST_EOSCASE)
    {
       x_memcpy( rScptInfo.u1FailMessage[rScptInfo.u4CurId],
                        pps_argv[4],
                        x_strlen(pps_argv[4]));
    }
    if(e_cmd_type==MM_AUTOTEST_INIT)
    {
       if(rScptInfo.hCaseSema==0)
       {   
           if(x_sema_create(&rScptInfo.hCaseSema,X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK)!=OSR_OK)
           {
               x_dbg_stmt("Sema Create Error\n");
           }
           rScptInfo.u4LasFileIndex=0;
           rScptInfo.u4LoopIndex=0;
           fgFirsInit=TRUE;
           x_dbg_stmt("FileIndex=%d,Loop Index=%d\n",rScptInfo.u4LasFileIndex,rScptInfo.u4LoopIndex);
       }
       rAutoTestInfo.u4rParam[0]=(UINT32)_mm_hdlr_mm_autotest_callback;
       rAutoTestInfo.u4rParam[1]=(UINT32)(&rScptInfo);
       rScptInfo.fgResult=TRUE;
       rScptInfo.fgCaseResult = TRUE;
       rScptInfo.fgWaitCallback=FALSE;
#if AUTOTEST_GETGOLDEN_TIME
       if(rScptInfo.u4LasFileIndex!=rScptInfo.u4FileIndex)
       {
           x_dbg_stmt("\n!!!!!!!!!!!NextFile!!!!!!!!!!!!!!!!\n");
           rScptInfo.u4LoopIndex=0;
           rScptInfo.u4LasFileIndex=rScptInfo.u4FileIndex;
           x_dbg_stmt("FileIndex=%d,Loop Index=%d\n",rScptInfo.u4LasFileIndex,rScptInfo.u4LoopIndex);
       }
       else if(fgFirsInit==FALSE)
       {
           rScptInfo.u4LoopIndex++;
           x_dbg_stmt("FileIndex=%d,Loop Index=%d\n",rScptInfo.u4LasFileIndex,rScptInfo.u4LoopIndex);
       }
#endif
#ifdef VDEC_TIME_PROFILE
        x_strncpy(&(rAutoTestInfo.ucAutoPlayCurFileDir[0]), &(ucAutoPlayCurFileDir[0]), sizeof(rAutoTestInfo.ucAutoPlayCurFileDir));
        x_strncpy(&(rAutoTestInfo.ucAutoPlayCurFileName[0]), &(ucAutoPlayCurFileName[0]), sizeof(rAutoTestInfo.ucAutoPlayCurFileName));
#endif
    }
    else if(e_cmd_type==MM_AUTOTEST_TIMECASE
            || e_cmd_type==MM_AUTOTEST_EOSCASE
            || e_cmd_type==MM_AUTOTEST_CRCCHECK)
    {
    
       u4Param = x_strtoll(pps_argv[2],NULL,10);
       rAutoTestInfo.u4rParam[0]=u4Param;
       u4Param = x_strtoll(pps_argv[3],NULL,10);
       rAutoTestInfo.u4rParam[1]=u4Param*1000;
       if(rScptInfo.u4FileIndex<AUTOTEST_MAX_FILE&&rScptInfo.u4CurId<AUTOTEST_MAXCASE_INDEX)
       {
           u4Param =_grGoldenInfo[rScptInfo.u4FileIndex][rScptInfo.u4CurId].u4GoldenTime;//x_strtoll(pps_argv[4],NULL,10);
           rAutoTestInfo.u4rParam[2]=u4Param;
           u4Param = _grGoldenInfo[rScptInfo.u4FileIndex][rScptInfo.u4CurId].u4Diff;//x_strtoll(pps_argv[5],NULL,10);
           rAutoTestInfo.u4rParam[3]=u4Param;
       }
       rScptInfo.fgWaitCallback=TRUE;
    }
    else if(e_cmd_type==MM_AUTOTEST_MAKEGOLDEN 
        || e_cmd_type==MM_AUTOTEST_GETEOSTIME
        || e_cmd_type==MM_AUTOTEST_PLAYSTOP
        || e_cmd_type==MM_AUTOTEST_GETCRCGOLDEN)
    {
       u4Param = x_strtoll(pps_argv[2],NULL,10);
       rAutoTestInfo.u4rParam[0]=u4Param;
       u4Param = x_strtoll(pps_argv[3],NULL,10);
       rAutoTestInfo.u4rParam[1]=u4Param*1000;
       rScptInfo.fgWaitCallback=TRUE;
    }
    else if(e_cmd_type==MM_AUTOTEST_RESET)
    {
#if AUTOTEST_GETGOLDEN_TIME
       u4Param = x_strtoll(pps_argv[3],NULL,10);
       if(u4Param==0)
       {
           UINT32 u4Index=0;
           x_dbg_stmt("\n<<<<<<<<<<<Loop:%d,GoldenValueRecords(%d)>>>>>>>>>>\n",rScptInfo.u4LoopIndex,rScptInfo.u4CurId);

           for(u4Index=0;u4Index<=rScptInfo.u4CurId;u4Index++)
           {
             x_dbg_stmt("(%d:%u) ",u4Index,_grGoldenTime_cur[u4Index]);

           }
           x_dbg_stmt("\n<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
       }
       else if(u4Param==1)
       {

           UINT32 u4CaseIndex=0;
           UINT32 u4DifTime =0;
           UINT64 u8AverageTime=0;
           
           x_dbg_stmt("\n!!!!file:(%d)!!!!!!!LoopTimes(%d)!!!!!!!!!!!!!!!!\n",rScptInfo.u4FileIndex, rScptInfo.u4LoopIndex+1);
           for(u4CaseIndex=0;u4CaseIndex<=rScptInfo.u4CurId;u4CaseIndex++)
           {
              #if 0
              u8AverageTime=0;
              u4DifTime=900;           //default  different value is 90000  (90k/sec) 
              u4GoldenTime=0;
              u4DeltTime=0;
              for(u4LoopIndex=0;u4LoopIndex<rScptInfo.u4LoopIndex+1;u4LoopIndex++)
              {
                 u8AverageTime +=(UINT64)_grGoldenTime[u4LoopIndex][u4CaseIndex];
              }
              
              u8AverageTime = u8AverageTime/(rScptInfo.u4LoopIndex+1);
              for(u4LoopIndex=0;u4LoopIndex<rScptInfo.u4LoopIndex+1;u4LoopIndex++)
              {
                 u4GoldenTime=_grGoldenTime[u4LoopIndex][u4CaseIndex];
                 u4DeltTime= u4GoldenTime >= (UINT32)u8AverageTime ? (u4GoldenTime -(UINT32)u8AverageTime) : ((UINT32)u8AverageTime-u4GoldenTime);
                 u4DifTime= u4DifTime>u4DeltTime ? u4DifTime : u4DeltTime;
              }

              #endif 
               if(_grGoldenTime_Min[u4CaseIndex] ==0xffffffff)
              {
                _grGoldenTime_Min[u4CaseIndex] = 0;
              }
              u8AverageTime = ((UINT64)_grGoldenTime_Max[u4CaseIndex] + (UINT64)_grGoldenTime_Min[u4CaseIndex])/2;
              u4DifTime = _grGoldenTime_Max[u4CaseIndex] - (UINT32)u8AverageTime;
              if( u4DifTime <9000)
              {
                  u4DifTime = 9000;

              }
              {
                x_dbg_stmt("(%u,%u),\n",(UINT32)u8AverageTime,u4DifTime);
              }
           }
           
           if(x_sema_delete(rScptInfo.hCaseSema) != OSR_OK)
           {
              x_dbg_stmt("x_sema_delete ScptInfo.hCaseSema fail !!!\n");
           }
           else
           {
             rScptInfo.hCaseSema = 0;
           }
           for(u4CaseIndex=0;u4CaseIndex<=rScptInfo.u4CurId;u4CaseIndex++)
           {
               _grGoldenTime_Max[u4CaseIndex] = 0;
               _grGoldenTime_Min[u4CaseIndex] = 0xffffffff;

           }
           rScptInfo.u4LasFileIndex=0;
           rScptInfo.u4LoopIndex=0;
       }
#endif
       return CLIR_OK;
    }
    
    i4_ret = x_svctx_set(
                    h_g_main_svctx,
                    SVCTX_MM_SET_TYPE_AUTOTEST_INFO,
                    (VOID*)&rAutoTestInfo,
                     sizeof(MM_AUTOTEST_INFO_T)
                    );
    
    if(SVCTXR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    return CLIR_OK;
}

static INT32 _mm_hdlr_mm_thread_delay(INT32 i4_argc, const CHAR** pps_argv)
{
    if(i4_argc>=2)
    {
       UINT32 u4Param;
       u4Param = x_strtoll(pps_argv[1],NULL,10);
       x_thread_delay(u4Param*1000);
    }
    else
    {
        if(rScptInfo.u4CurType!=MM_AUTOTEST_INIT && rScptInfo.hCaseSema && !rScptLock)
        {
            rScptLock = TRUE;
            if(x_sema_lock(rScptInfo.hCaseSema,X_SEMA_OPTION_WAIT)!=OSR_OK)
            {
                x_dbg_stmt("Sema Lock Error\n");
                rScptLock = FALSE;
            }           
        }
    }    
    return CLIR_OK;
}


static INT32 _mm_hdlr_mm_get_golden_values(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT32 i,j;

    for(i = 0; i<AUTOTEST_MAX_FILE; i++)
    {
        //x_dbg_stmt("Golden Info[%d]\n",i);
        for(j = 0; j<AUTOTEST_MAXCASE_INDEX; j++)
        {
            //x_dbg_stmt("{%d,%d}",_grGoldenInfo[i][j].u4GoldenTime,_grGoldenInfo[i][j].u4Diff);

            if(j && (0x0 == (j%10)))
            {
          //      x_dbg_stmt("\n");
            }
        }
       // x_dbg_stmt("\n");
    }
    
    return CLIR_OK;
}

static INT32 _mm_hdlr_mm_auto_test (INT32 i4_argc, const CHAR** pps_argv, const CHAR* file_path)
{
    INT32               i4_ret                  = CLIR_OK;
    UINT32              ui4_file_path_len       = 0x0;
    CHAR*               pac_file_path           = NULL;
    UINT32              ui4_read_cnt            = 0x0;
    BOOL                b_check                 = FALSE;
    FM_FILE_INFO_T      t_info;
    SCPT_DATA_T         t_scpt_data;
    SCPT_DATA_T*        pt_scpt_data = &t_scpt_data;;
    
    #ifdef LINUX_TURNKEY_SOLUTION
    const CHAR ac_file_ppath[]= "/mnt/usb/sda1/";
    #else
    const CHAR ac_file_ppath[]= "/mnt/usb/Mass-000/";
    #endif

    if( i4_argc < 2 )
    {
        x_dbg_stmt("para invalid !\n");
        return CLIR_INV_ARG;
    }
    else if( 0x3 == i4_argc )
    {
        if(0x0 == x_memcmp( pps_argv[2],
                        SCPT_CHECK,
                        x_strlen(SCPT_CHECK)) )
        {
            b_check = TRUE;
        }
    }
    
    x_memset(pt_scpt_data, 0, sizeof(t_scpt_data));
    x_memset(&t_info, 0, sizeof(t_info));

    /* Get test_script file path */
    do
    {
        ui4_file_path_len = (x_strlen((CHAR*)ac_file_ppath)+ x_strlen(file_path) + 1);
        pac_file_path = x_mem_alloc(ui4_file_path_len); 
        if( NULL == pac_file_path )
        {
            i4_ret = CLIR_INV_ARG;
            break;
        }
        
        x_memset(pac_file_path, 0, ui4_file_path_len);
        x_strcpy(pac_file_path, ac_file_ppath);
        x_strcpy(pac_file_path + x_strlen(ac_file_ppath), file_path);

        /* open script file */
        
        x_fm_open( FM_ROOT_HANDLE,
                            pac_file_path,
                            FM_READ_ONLY,
                            FM_MODE_USR_READ,
                            FALSE,
                            &pt_scpt_data->t_file.h_file); 
        
        /* get file info  */
        
        i4_ret = x_fm_get_info_by_handle(pt_scpt_data->t_file.h_file, &t_info);
        
        if( FMR_OK != i4_ret )
        {
            x_dbg_stmt("get info(%d) Failed!\n",i4_ret);

            i4_ret = CLIR_INV_ARG;
            break;
        }
        
        if( 0x0 == t_info.ui8_size )
        {
            x_dbg_stmt(" script file is zero !\n");

            i4_ret = CLIR_INV_ARG;
            break;
        }
        
        /* copy  to memory */
        
        pt_scpt_data->t_file.ui4_size = t_info.ui8_size;
        pt_scpt_data->t_file.puc_start = x_mem_alloc( pt_scpt_data->t_file.ui4_size + 1 ); 
        if( NULL == pt_scpt_data->t_file.puc_start )
        {
            x_dbg_stmt("malloc memory Failed!\n");

            i4_ret = CLIR_INV_ARG;
            break;
        }
        
        i4_ret = x_fm_read(
                    pt_scpt_data->t_file.h_file,
                    pt_scpt_data->t_file.puc_start,
                    pt_scpt_data->t_file.ui4_size,
                    &ui4_read_cnt);
        if (FMR_OK != i4_ret)
        {
            break;
        }
        
        pt_scpt_data->t_file.puc_crnt = pt_scpt_data->t_file.puc_start;
        
        /* paser script  */
        i4_ret = _scpt_parser_init(pt_scpt_data);
        if (CLIR_OK != i4_ret)
        {
            x_dbg_stmt("_scpt_parser_init Failed!\n");
            break;
        }
        
        pt_scpt_data->t_tc.b_check = b_check;
        i4_ret = _scpt_parser(pt_scpt_data);
        if (CLIR_OK != i4_ret)
        {
            x_dbg_stmt("_scpt_parser Failed!\n");
            break;
        }
    }while(0);
    

    _scpt_parser_deinit(pt_scpt_data);
    
    /* free memory  */
    x_mem_free(pt_scpt_data->t_file.puc_start);
    pt_scpt_data->t_file.puc_start = NULL;
    
    x_mem_free(pac_file_path);
    pac_file_path = NULL;

    x_fm_close(pt_scpt_data->t_file.h_file);
    
    if(i4_ret !=  SVCTXR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    
    return CLIR_OK;
}

static INT32 _mm_hdlr_mm_auto_test_loop (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32               i4_ret                  = CLIR_OK;
    UINT32              ui4_file_path_len       = 0x0;
    CHAR*               pac_file_path           = NULL;
    UINT32              ui4_read_cnt            = 0x0;
    FM_FILE_INFO_T      t_info;
    SCPT_TEXT_T         t_file;
    CHAR*               pt_file                 = NULL;
    UINT32              file_size               = 0;
    UINT32              file_path_length        = 0;
    UINT8               start                   = 0;
    CHAR*               file_path               = NULL;
    
    #ifdef LINUX_TURNKEY_SOLUTION
    const CHAR ac_file_ppath[]= "/mnt/usb/sda1/";
    #else
    const CHAR ac_file_ppath[]= "/mnt/usb/Mass-000/";
    #endif

    if( i4_argc < 2 )
    {
        x_dbg_stmt("para invalid !\n");
        return CLIR_INV_ARG;
    }
    
    x_memset(&t_info, 0, sizeof(t_info));

    /* Get test_script file path */
    do
    {
        ui4_file_path_len = (x_strlen((CHAR*)ac_file_ppath)+ x_strlen(pps_argv[1]) + 1);
        pac_file_path = x_mem_alloc(ui4_file_path_len); 
        if( NULL == pac_file_path )
        {
            i4_ret = CLIR_INV_ARG;
            break;
        }
        
        x_memset(pac_file_path, 0, ui4_file_path_len);
        x_strcpy(pac_file_path, ac_file_ppath);
        x_strcpy(pac_file_path + x_strlen(ac_file_ppath), pps_argv[1]);

        /* open script file */
        
        x_fm_open( FM_ROOT_HANDLE,
                            pac_file_path,
                            FM_READ_ONLY,
                            FM_MODE_USR_READ,
                            FALSE,
                            &t_file.h_file); 
        
        /* get file info  */
        
        i4_ret = x_fm_get_info_by_handle(t_file.h_file, &t_info);
        
        if( FMR_OK != i4_ret )
        {
            x_dbg_stmt("get info(%d) Failed!\n",i4_ret);

            i4_ret = CLIR_INV_ARG;
            break;
        }
        
        if( 0x0 == t_info.ui8_size )
        {
            x_dbg_stmt(" script file is zero !\n");

            i4_ret = CLIR_INV_ARG;
            break;
        }
        
        /* copy  to memory */
        
        t_file.ui4_size = t_info.ui8_size;
        t_file.puc_start = x_mem_alloc( t_file.ui4_size + 1 ); 
        if( NULL == t_file.puc_start )
        {
            x_dbg_stmt("malloc memory Failed!\n");

            i4_ret = CLIR_INV_ARG;
            break;
        }
        
        i4_ret = x_fm_read(
                    t_file.h_file,
                    t_file.puc_start,
                    t_file.ui4_size,
                    &ui4_read_cnt);
        if (FMR_OK != i4_ret)
        {
            break;
        }

        file_size = t_file.ui4_size;
        pt_file = t_file.puc_start;
        file_path_length = 0;

        while(file_size--)
        {
            if(start == 0)
            {
                if(pt_file[file_path_length] == ';')
                {
                    pt_file++;
                    start = 1;
                    file_path_length = 0;
                }
                else
                {
                    pt_file++;
                }
                
            }
            else if(start == 1)
            {
                if(pt_file[file_path_length] == ';')
                {
                    file_path = x_mem_alloc(file_path_length + 1); 
                    x_strncpy(file_path, pt_file, file_path_length);
                    file_path[file_path_length] = '\0';
                    x_dbg_stmt("===the file path is: %s\n", file_path);
                    i4_ret = _mm_hdlr_mm_auto_test(i4_argc, pps_argv, file_path);
                    x_mem_free(file_path);
                    file_path = NULL;                
                    if(file_size)
                    {
                        pt_file += (file_path_length + 1);
                    }
                    start = 0;
                    file_path_length = 0;
                    if (FMR_OK != i4_ret)
                    {
                        x_dbg_stmt("===%s test failed\n", file_path);
                    }
                    
                }
                else
                {
                    file_path_length++;
                }               
            }
        }
        
    }while(0);
    
    /* free memory  */
    x_mem_free(t_file.puc_start);
    t_file.puc_start = NULL;
    
    x_mem_free(pac_file_path);
    pac_file_path = NULL;

    x_fm_close(t_file.h_file);
    
    if(i4_ret !=  SVCTXR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    
    return CLIR_OK;
}

#ifdef AUTOTEST_MEMLEAK_CHK
#ifdef LINUX_TURNKEY_SOLUTION
#include "libc/stdio.h"
static INT32 _mm_hdlr_mm_monitor_memory(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret = 0x0;
    BOOL b_update = FALSE;
    UINT32 ui4_read_len = 1024*512; 
    CHAR* puc_read_buf = NULL;
    SLAB_DETAIL_INFO_T* pt_slab_list_head = NULL;

    if(i4_argc > 1)
    {
        b_update = (x_strtoll(pps_argv[1], NULL, 10) > 0? TRUE : FALSE);
        if(b_update == TRUE)
        {
            x_dbg_stmt("update reference slab info list.\n");
        }
    }

    ui4_read_len = _scpt_parser_query_proc( &puc_read_buf, 
				ui4_read_len, "/proc/slabinfo");
    if( ui4_read_len == SCPT_U4_INVALID )
    {
        x_dbg_stmt("===Autotest /proc/slabinfo query failed!\n");
        return SCPT_U4_INVALID;
    }

    pt_slab_list_head = _scpt_parser_gen_slab_detail_list( 
					&puc_read_buf, ui4_read_len );
    if( pt_slab_list_head == NULL)
    {
        x_dbg_stmt("===Autotest gen slab info list failed!\n");
        _scpt_parser_release_proc( &puc_read_buf );
        return SCPT_U4_INVALID;
    }

    i4_ret = _scpt_update_slab_list( pt_slab_list_head, b_update);
    if( i4_ret == SCPT_U4_INVALID)
    {
        x_dbg_stmt("===Autotest update slab list failed!\n");
        _scpt_parser_release_proc( &puc_read_buf );
        return SCPT_U4_INVALID;
    }

    _scpt_parser_release_proc( &puc_read_buf );

    return CLIR_OK;
}

/* make a comparsion between the previous and current slab info, returns the delta value
 marking the memory leakage info */
static INT32 _scpt_update_slab_list( SLAB_DETAIL_INFO_T* pt_slab_list_head, 
												BOOL b_update_ref )
{
    INT32 i4_ret = 0x0;
    MEM_MONITOR_INFO_T* pt_mem_info = NULL;

    pt_mem_info = _scpt_get_meminfo();
    if(pt_mem_info->b_first_set == TRUE || b_update_ref == TRUE)
    {
        x_dbg_stmt("=====Set slab info first\n");
        pt_mem_info->b_first_set = FALSE;

	/* init slab list history record */
	 _scpt_free_slab_list(&pt_mem_info->pt_last_slab_list);
        pt_mem_info->pt_last_slab_list = _scpt_dup_slab_list(
		pt_slab_list_head);

        _scpt_free_slab_list(&pt_mem_info->pt_slab_list);
        pt_mem_info->pt_slab_list = _scpt_dup_slab_list(
		pt_slab_list_head);

        return CLIR_OK;
    }

    _scpt_free_slab_list(&pt_mem_info->pt_slab_list);
    pt_mem_info->pt_slab_list =  _scpt_dup_slab_list(
		pt_slab_list_head);

    i4_ret = _scpt_query_slab_list_length(pt_mem_info->pt_slab_list);
    //i4_ret = _scpt_query_slab_list_length(pt_slab_list_head);
    x_dbg_stmt("length=%d\n", i4_ret);

    return CLIR_OK;
}

/*this function returns the new allocated last element in link list*/
static SLAB_DETAIL_INFO_T* _scpt_alloc_slab_info( SLAB_DETAIL_INFO_T** ppt_head )
{
    SLAB_DETAIL_INFO_T* pt_temp1 = NULL;
    SLAB_DETAIL_INFO_T* pt_temp2 = NULL;

    if( *ppt_head == NULL )
    {
        /*init head*/
        *ppt_head = x_mem_alloc(sizeof(SLAB_DETAIL_INFO_T) );
        if( *ppt_head == NULL )
        {
            x_dbg_stmt("_scpt_alloc_slab_info init head failed!\n");
            goto ERROR_ALLOC_HEAD;
        }
        else
        {
            (*ppt_head)->pt_obj_info = x_mem_alloc(sizeof(SLAB_OBJ_INFO_T));
            if( (*ppt_head)->pt_obj_info == NULL )
            {
                goto ERROR_ALLOC_HEAD;
            }
			
            (*ppt_head)->pt_tunable_info = x_mem_alloc(sizeof(SLAB_TUNABLE_INFO_T));
            if( (*ppt_head)->pt_tunable_info == NULL )
            {
                goto ERROR_ALLOC_HEAD;
            }
			
            (*ppt_head)->pt_data_info = x_mem_alloc(sizeof(SLAB_DATA_INFO_T));
            if( (*ppt_head)->pt_data_info == NULL )
            {
                goto ERROR_ALLOC_HEAD;
            }
        }	
        (*ppt_head)->pt_next = NULL;
        _scpt_init_slab_info( *ppt_head );

        return *ppt_head;
    }
    else
    {
        /* find out the last element in link list */
        pt_temp1 = *ppt_head;
        while( pt_temp1->pt_next!= NULL )
        {
            pt_temp1 = pt_temp1->pt_next;
        }
        pt_temp2 = pt_temp1;
        pt_temp2->pt_next = x_mem_alloc( sizeof(SLAB_DETAIL_INFO_T));
        if( pt_temp2->pt_next == NULL )
        {
            x_dbg_stmt("_scpt_alloc_slab_info alloc new unit failed!\n");
            goto ERROR_ALLOC_NEXT;
        }
        else
        {
            pt_temp2->pt_next->pt_obj_info = x_mem_alloc( sizeof(SLAB_OBJ_INFO_T));
            if( pt_temp2->pt_next->pt_obj_info == NULL )
            {
                goto ERROR_ALLOC_NEXT;
            }
			
            pt_temp2->pt_next->pt_tunable_info = x_mem_alloc( sizeof(SLAB_TUNABLE_INFO_T));
            if( pt_temp2->pt_next->pt_tunable_info == NULL )
            {
                goto ERROR_ALLOC_NEXT;
            }
			
            pt_temp2->pt_next->pt_data_info = x_mem_alloc( sizeof(SLAB_DATA_INFO_T));
            if( pt_temp2->pt_next->pt_data_info == NULL )
            {
                goto ERROR_ALLOC_NEXT;
            }
        }
        pt_temp1 = pt_temp2->pt_next;
        pt_temp1->pt_next = NULL;
        _scpt_init_slab_info(pt_temp1);

        return pt_temp1;
    }

ERROR_ALLOC_HEAD:
    _scpt_free_slab_list( ppt_head );
    *ppt_head = NULL;
    return *ppt_head;

ERROR_ALLOC_NEXT:
    _scpt_free_slab_list( ppt_head );
    *ppt_head = NULL;
    return *ppt_head;
}

static UINT32 _scpt_query_slab_list_length( SLAB_DETAIL_INFO_T* pt_head )
{
    UINT32 ui4_loop	=	0x0;

    /* check slab deatil head is null */
    if( pt_head == NULL )
    {
        x_dbg_stmt("_scpt_query_slab_list_length ptr_head is null!\n");
        return ui4_loop;
    }

    /* scan the whole single list and its member elements */
    while ( pt_head != NULL )
    {
        if( (pt_head->pt_obj_info != NULL) && (pt_head->pt_tunable_info != NULL) 
            && (pt_head->pt_data_info != NULL) )
        {
            ui4_loop++;
        }

        pt_head = pt_head->pt_next;
    }

    return ui4_loop;
}

static INT32 _scpt_free_slab_list( SLAB_DETAIL_INFO_T** ppr_unit )
{
    SLAB_DETAIL_INFO_T* pt_temp = *ppr_unit;

    if( pt_temp == NULL )
    {
        return CLIR_NOT_INIT;
    }

    while( pt_temp != NULL )
    {
        if( pt_temp->pt_obj_info != NULL )
        {
            x_mem_free( pt_temp->pt_obj_info );
            pt_temp->pt_obj_info = NULL;
        }

        if( pt_temp->pt_tunable_info != NULL )
        {
            x_mem_free( pt_temp->pt_tunable_info );
            pt_temp->pt_tunable_info = NULL;
        }

        if( pt_temp->pt_data_info != NULL )
        {
            x_mem_free( pt_temp->pt_data_info );
            pt_temp->pt_data_info = NULL;
        }

        pt_temp = pt_temp->pt_next;
    }

    *ppr_unit = NULL;

    return CLIR_OK;
}

static SLAB_DETAIL_INFO_T*  _scpt_filter_delta_slab_info( SLAB_DETAIL_INFO_T* pt_delta_info )
{
    SLAB_DETAIL_INFO_T* pt_head = NULL;
    SLAB_DETAIL_INFO_T* pt_temp = NULL;

    if( NULL ==  pt_delta_info )
    {
        x_dbg_stmt("_scpt_filter_delta_slab_info get delta info error!\n");
        return NULL;
    }

    /* alloc a new link list containing the valid delta slab info */
    while( pt_delta_info != NULL )
    {
        if( (pt_delta_info->pt_obj_info->i4_active_objs != 0x0) 
		|| (pt_delta_info->pt_obj_info->i4_num_objs != 0x0) 
		|| (pt_delta_info->pt_data_info->i4_active_slabs != 0x0) 
		|| (pt_delta_info->pt_data_info->i4_num_slabs != 0x0) 
		|| (pt_delta_info->pt_data_info->i4_shared_avail != 0x0) )
        {
            pt_temp = _scpt_alloc_slab_info( &pt_head );
            if( pt_temp == NULL )
            {
                x_dbg_stmt("_scpt_filter_delta_slab_info allocate memory error!\n");
                return NULL;
            }
			
            _scpt_copy_slab_info( pt_temp, pt_delta_info);
        }

        pt_delta_info = pt_delta_info->pt_next;
    }

    /* empty the delta slab info containing the redundent info */
    _scpt_free_slab_list(&pt_delta_info);

    /* assign new valid slab delta list to pt_delta_info */
    x_dbg_stmt("_scpt_filter_delta_slab_info getting valid delta slab info.\n");

    if(pt_head == NULL) 
    {
        x_dbg_stmt("_scpt_filter_delta_slab_info getting delta slab unavailable.\n");
        return CLIR_OK;
    }

    return pt_head;
}

static SLAB_DETAIL_INFO_T* _scpt_calc_delta_slab_info( SLAB_DETAIL_INFO_T* pt_slab_last,  
														SLAB_DETAIL_INFO_T* pt_slab_current )
{
	UINT32 	ui4_len1 =	0x0;
	UINT32 	ui4_len2 =	0x0;
	UINT32 	ui4_loop =	0x0;
	SLAB_DETAIL_INFO_T*	pt_head	= NULL;
	SLAB_DETAIL_INFO_T* pt_temp = NULL;

	/* make sure the length of two compared list is positive  */
	ui4_len1 = _scpt_query_slab_list_length(pt_slab_last);
	if(ui4_len1 == 0x0)
	{
		x_dbg_stmt("_scpt_calc_delta_slab_info query last slab list error!\n");
		return NULL;
	}
	ui4_len2 = _scpt_query_slab_list_length(pt_slab_current);	
	if (ui4_len2 == 0x0)
	{
		x_dbg_stmt("_scpt_calc_delta_slab_info query current slab list error!\n");
		return NULL;
	}
	if(ui4_len1 != ui4_len2)
	{
		x_dbg_stmt("_scpt_calc_delta_slab_info compare two slab list error!\n");
		return NULL;
	}

	ui4_loop = ui4_len1;
	while (ui4_loop>0x0)
	{
		pt_temp = _scpt_alloc_slab_info( &pt_head );
		if( pt_temp == NULL )
		{
			x_dbg_stmt("_scpt_update_delta_slabinfo allocate meory error!\n");
			return NULL;
		}

		/* assign the delta info to link list */
		x_strncpy(pt_temp->pt_obj_info->uc_name, pt_slab_current->pt_obj_info->uc_name, 
					x_strlen(pt_slab_current->pt_obj_info->uc_name) );	
		
		pt_temp->pt_obj_info->i4_active_objs = 
			pt_slab_current->pt_obj_info->i4_active_objs - 
			pt_slab_last->pt_obj_info->i4_active_objs;
		pt_temp->pt_obj_info->i4_num_objs = 
			pt_slab_current->pt_obj_info->i4_num_objs - 
			pt_slab_last->pt_obj_info->i4_num_objs;
		pt_temp->pt_obj_info->i4_obj_size = 
			pt_slab_current->pt_obj_info->i4_obj_size;
		pt_temp->pt_obj_info->i4_obj_per_slab	= 
			pt_slab_current->pt_obj_info->i4_obj_per_slab;
		pt_temp->pt_obj_info->i4_pages_per_slab = 
			pt_slab_current->pt_obj_info->i4_pages_per_slab;
		
		pt_temp->pt_tunable_info->i4_batch_count =
			pt_slab_current->pt_tunable_info->i4_batch_count;
		pt_temp->pt_tunable_info->i4_limit = 
			pt_slab_current->pt_tunable_info->i4_limit;
		pt_temp->pt_tunable_info->i4_shared_factor = 
			pt_slab_current->pt_tunable_info->i4_shared_factor;
		
		pt_temp->pt_data_info->i4_active_slabs =
			pt_slab_current->pt_data_info->i4_active_slabs - 
			pt_slab_last->pt_data_info->i4_active_slabs;
		pt_temp->pt_data_info->i4_num_slabs = 
			pt_slab_current->pt_data_info->i4_num_slabs - 
			pt_slab_last->pt_data_info->i4_num_slabs;
		pt_temp->pt_data_info->i4_shared_avail = 
			pt_slab_current->pt_data_info->i4_shared_avail - 
			pt_slab_last->pt_data_info->i4_shared_avail;

		pt_slab_current = pt_slab_current->pt_next;
		pt_slab_last = pt_slab_last->pt_next;

		ui4_loop--;
	}

	return pt_head;
}

static SLAB_DETAIL_INFO_T* _scpt_dup_slab_list( SLAB_DETAIL_INFO_T* pt_src_list )
{
    UINT32 ui4_len = 0x0;
    SLAB_DETAIL_INFO_T* pt_temp1 = pt_src_list;
    SLAB_DETAIL_INFO_T* pt_temp2 = NULL;
    SLAB_DETAIL_INFO_T* pt_dst_list = NULL;

    if( pt_src_list == NULL )
    {
        x_dbg_stmt("_scpt_dup_slab_list pt_src_list is null!\n");
        return NULL;
    }

    /* alloc mem for dst slab list */
    ui4_len = _scpt_query_slab_list_length(pt_src_list);
    if(ui4_len == 0x0)
    {
        x_dbg_stmt("_scpt_dup_slab_list query pt_src_list null!\n");
        return NULL;
    }

    while(pt_temp1->pt_next)
    {
        pt_temp2 = _scpt_alloc_slab_info(&pt_dst_list);
        if(pt_temp2 == NULL)
        {
            x_dbg_stmt("_scpt_dup_slab_list alloc slab info error!\n");
            return NULL;
        }

       /* copy slab unit info to new link list */
        _scpt_copy_slab_info(pt_temp2, pt_temp1);
	pt_temp1 = pt_temp1->pt_next;
    }

    return pt_dst_list;
}

static INT32 _scpt_copy_slab_info( SLAB_DETAIL_INFO_T* pt_dst_info, 
									SLAB_DETAIL_INFO_T* pt_src_info )
{
    INT32 i4_ret =	0x0;
	
    if( pt_src_info == NULL )
    {
        return CLIR_NOT_INIT;
    }

    i4_ret = _scpt_init_slab_info( pt_dst_info );
    if( i4_ret == CLIR_NOT_INIT )
    {
        return SCPT_U4_INVALID;
    }

    i4_ret = _scpt_copy_slab_obj_info( pt_dst_info->pt_obj_info, 
									pt_src_info->pt_obj_info );
    if( i4_ret == CLIR_NOT_INIT )
    {
        return SCPT_U4_INVALID;
    }

    i4_ret = _scpt_copy_slab_tunable_info( pt_dst_info->pt_tunable_info, 
										pt_src_info->pt_tunable_info );
    if( i4_ret == CLIR_NOT_INIT )
    {
        return SCPT_U4_INVALID;
    }

    i4_ret = _scpt_copy_slab_data_info( pt_dst_info->pt_data_info, 
									pt_src_info->pt_data_info );
    if( i4_ret == CLIR_NOT_INIT )
    {
        return SCPT_U4_INVALID;
    }

    return CLIR_OK;    
}

static INT32 _scpt_copy_slab_obj_info( SLAB_OBJ_INFO_T* pt_dst_info, 
										SLAB_OBJ_INFO_T* pt_src_info )
{
    if( (pt_src_info == NULL) || (pt_dst_info == NULL) )
    {
        return CLIR_NOT_INIT;
    }

    x_strncpy(pt_dst_info->uc_name, pt_src_info->uc_name, 
					x_strlen(pt_src_info->uc_name));

    pt_dst_info->i4_active_objs 	= 	pt_src_info->i4_active_objs;
    pt_dst_info->i4_num_objs 	= 	pt_src_info->i4_num_objs;
    pt_dst_info->i4_obj_per_slab = 	pt_src_info->i4_obj_per_slab;
    pt_dst_info->i4_obj_size	= 	pt_src_info->i4_obj_size;	
    pt_dst_info->i4_pages_per_slab = pt_src_info->i4_pages_per_slab;

    return CLIR_OK;
}

static INT32 _scpt_copy_slab_tunable_info( SLAB_TUNABLE_INFO_T* pt_dst_info, 
											SLAB_TUNABLE_INFO_T* pt_src_info )
{
    if( (pt_src_info == NULL) || (pt_dst_info == NULL) )
    {
        return CLIR_NOT_INIT;
    }

    pt_dst_info->i4_batch_count = 	pt_src_info->i4_batch_count;
    pt_dst_info->i4_limit 		=	pt_src_info->i4_limit;
    pt_dst_info->i4_shared_factor =	pt_src_info->i4_shared_factor;

    return CLIR_OK;
}

static INT32 _scpt_copy_slab_data_info( SLAB_DATA_INFO_T* pt_dst_info, 
										SLAB_DATA_INFO_T* pt_src_info )
{
    if( (pt_src_info == NULL) || (pt_dst_info == NULL) )
    {
        return CLIR_NOT_INIT;
    }

    pt_dst_info->i4_active_slabs =	pt_src_info->i4_active_slabs;
    pt_dst_info->i4_num_slabs	=	pt_src_info->i4_num_slabs;
    pt_dst_info->i4_shared_avail =	pt_src_info->i4_shared_avail;

    return CLIR_OK;
}

static INT32  _scpt_init_slab_info( SLAB_DETAIL_INFO_T* pt_detail_info	)
{
    INT32 i4_ret = 0x0;
	
    if( pt_detail_info == NULL )
    {
        i4_ret = CLIR_NOT_INIT;
    }
	
    i4_ret = _scpt_init_slab_obj_info( pt_detail_info->pt_obj_info );
    if( i4_ret != CLIR_OK )
    {
        return i4_ret;
    }
	
    i4_ret = _scpt_init_slab_tunable_info( pt_detail_info->pt_tunable_info );
    if( i4_ret != CLIR_OK )
    {
        return i4_ret;
    }
	
    i4_ret = _scpt_init_slab_data_info( pt_detail_info->pt_data_info );
    if( i4_ret != CLIR_OK )
    {
        return i4_ret;
    }

    return CLIR_OK;
}

static INT32  _scpt_init_slab_obj_info( SLAB_OBJ_INFO_T* pt_obj_info	)
{
    INT32 i4_ret = 0x0;

    if( pt_obj_info == NULL )
    {
        i4_ret = CLIR_NOT_INIT;
    }

    x_memset(pt_obj_info->uc_name, SCPT_CHAR_ZERO, 30);
    pt_obj_info->i4_active_objs 	= 0x0;	
    pt_obj_info->i4_num_objs 	= 0x0;	
    pt_obj_info->i4_obj_size 	= 0x0;	
    pt_obj_info->i4_obj_per_slab = 0x0;	
    pt_obj_info->i4_pages_per_slab = 0x0;

    i4_ret = CLIR_OK;

    return i4_ret;
}

static INT32  _scpt_init_slab_tunable_info( SLAB_TUNABLE_INFO_T* pt_tunable_info	)
{
    INT32 i4_ret = 0x0;

    if( pt_tunable_info == NULL )
    {
        i4_ret = CLIR_NOT_INIT;
    }

    pt_tunable_info->i4_batch_count = 0x0;	
    pt_tunable_info->i4_limit 		= 0x0;	
    pt_tunable_info->i4_shared_factor = 0x0;

    i4_ret = CLIR_OK;

    return i4_ret;
}

static INT32  _scpt_init_slab_data_info( SLAB_DATA_INFO_T*  pt_data_info	)
{
    INT32 i4_ret = 0x0;

    if( pt_data_info == NULL )
    {
        i4_ret = CLIR_NOT_INIT;
    }

    pt_data_info->i4_active_slabs 	= 0x0;
    pt_data_info->i4_num_slabs 	= 0x0;
    pt_data_info->i4_shared_avail = 0x0;

    i4_ret = CLIR_OK;

    return i4_ret;
}

static UINT32 _scpt_parser_query_proc( CHAR** ppt_read_buf, 
								UINT32 ui4_len, CHAR* ptr_key )
{
    INT32 i4_ret = 0;
    UINT32 u4_read_times = 0;
    FILE *fp = NULL;
    CHAR uc_mode[] = "rb";
    CHAR uc_read;

    if( (fp = fopen(ptr_key, uc_mode)) == NULL )
    {
        x_dbg_stmt("open %s file failed!\n", ptr_key);
        i4_ret = SCPT_U4_INVALID;
    }

    *ppt_read_buf = x_mem_alloc(ui4_len);
    if( *ppt_read_buf == NULL )
    {
        x_dbg_stmt("x_mem_alloc memory request failed!\n");
        i4_ret = SCPT_U4_INVALID;
    }
	
    while(1)
    {
        if( (uc_read = fgetc(fp)) != EOF )
        {
            (*ppt_read_buf)[u4_read_times] = uc_read;
            u4_read_times++;
            continue;
        }
        else
        {
            break;
        }
    }
	
    (*ppt_read_buf)[u4_read_times] = SCPT_CHAR_ZERO;
    i4_ret = u4_read_times;

    return i4_ret;
}

static VOID _scpt_parser_release_proc( CHAR** ppt_read_buf )
{
    if( *ppt_read_buf == NULL )
    {
        return;
    }

    x_mem_free( *ppt_read_buf );
    *ppt_read_buf = NULL;
}

static INT32 _scpt_parser_read_slab_info( CHAR* puc_str, UINT32 ui4_len, 
									SLAB_DETAIL_INFO_T* pt_detail_info )
{
    CHAR* puc_filter 		= puc_str;
    INT32 i4_pos			= 0x0;
    INT32 i4_ret			= 0x0;

    if( pt_detail_info == NULL )
    {
        return CLIR_NOT_INIT;
    }

    if( (pt_detail_info->pt_obj_info == NULL) || (pt_detail_info->pt_tunable_info == NULL) || 
	(pt_detail_info->pt_data_info == NULL) )
    {
        return CLIR_NOT_INIT;
    }

    /* set obj info */
    i4_ret = _scpt_parser_read_obj_info(
    		puc_filter, ui4_len, 
    		pt_detail_info->pt_obj_info );
    if( i4_ret == SCPT_U4_INVALID )
    {
        x_dbg_stmt("_scpt_parser_read_obj_info set objinfo failed!\n");
        return SCPT_U4_INVALID;
    }

    /* find out the first index character ':' in this line */
    i4_pos = _scpt_find_first_char( puc_filter, ui4_len, SCPT_CHAR_COLON );
    if( i4_pos == SCPT_U4_INVALID )
    {
        x_dbg_stmt("_scpt_parser_read_slab_info get one line first data error!\n");
        return SCPT_U4_INVALID;
    }

    /* set tunable info */
    puc_filter += ++i4_pos;
    ui4_len -= (UINT32)i4_pos;

    i4_ret = _scpt_parser_read_tunable_info( 
		puc_filter, ui4_len, 
		pt_detail_info->pt_tunable_info );
    if( i4_ret == SCPT_U4_INVALID )
    {
        x_dbg_stmt("_scpt_parser_read_tunable_info read tunableinfo failed!\n");
        return SCPT_U4_INVALID;
    }

    /* find out the second index character ':' in this line */
    i4_pos = _scpt_find_first_char( puc_filter, ui4_len, SCPT_CHAR_COLON );
    if(  i4_pos == SCPT_U4_INVALID )
    {
        x_dbg_stmt("_scpt_parser_read_slab_info get one line second data error!\n");
        return SCPT_U4_INVALID;
    }

    /* set data info */
    puc_filter += ++i4_pos;
    ui4_len -= (UINT32)i4_pos;

    i4_ret = _scpt_parser_read_data_info( 
		puc_filter, ui4_len, 
		pt_detail_info->pt_data_info );
    if( i4_ret == SCPT_U4_INVALID )
    {
        x_dbg_stmt("_scpt_parser_read_data_info set datainfo failed!\n");
        return SCPT_U4_INVALID;
    }

    return CLIR_OK;
}

static INT32 _scpt_parser_read_obj_info( CHAR* puc_str, UINT32 ui4_len, 
									SLAB_OBJ_INFO_T* pt_obj_info )
{
    CHAR* puc_filter 		= puc_str;
    UINT32 ui4_vara_len	= 0x0;
    UINT32 ui4_loop_time	= 0x0;
    CHAR pc_num_str[30]	= "";

    /*set name*/
    ui4_vara_len = _scpt_query_str_len_ex(puc_filter, ui4_len);
    if( ui4_vara_len == 0x0 ){
        return SCPT_U4_INVALID;
    }	
    x_memset(pt_obj_info->uc_name, 0, 30 );
    x_strncpy(pt_obj_info->uc_name, puc_filter, ui4_vara_len);
    puc_filter += ui4_vara_len;

    /*set obj value*/
    do{
        puc_filter = _scpt_filter_space(puc_filter, ui4_len);
        ui4_vara_len = _scpt_query_number_str_len(puc_filter, ui4_len);
        if( ui4_vara_len == 0x0 ){
            return SCPT_U4_INVALID;
        }
        x_memset(pc_num_str, 0, ui4_vara_len+1 );
        x_strncpy(pc_num_str, puc_filter, ui4_vara_len);
		
        switch ( ui4_loop_time ){
        case 0:	
            pt_obj_info->i4_active_objs = x_strtoll(pc_num_str, NULL, 10);
            break;		
        case 1:
            pt_obj_info->i4_num_objs = x_strtoll(pc_num_str, NULL, 10);
            break;	
        case 2:  
            pt_obj_info->i4_obj_size = x_strtoll(pc_num_str, NULL, 10);
            break;	
        case 3:  
            pt_obj_info->i4_obj_per_slab = x_strtoll(pc_num_str, NULL, 10);
            break;
        case 4:  
            pt_obj_info->i4_pages_per_slab = x_strtoll(pc_num_str, NULL, 10);
            break;
        default:
            break;
        }	

        puc_filter += ui4_vara_len;

        ui4_loop_time++;
    }while( ui4_loop_time < 0x5 );

    return CLIR_OK;
}

static INT32 _scpt_parser_read_tunable_info( CHAR* puc_str, UINT32 ui4_len, 
										SLAB_TUNABLE_INFO_T* pt_tunable_info )
{
	CHAR* puc_filter 		= puc_str;
	UINT32 ui4_vara_len	= 0x0;
	UINT32 ui4_loop_time	 = 0x0;
	//CHAR uc_key_str[]	= "tunables";
	CHAR pc_num_str[30] = "";

	puc_filter = _scpt_filter_space(puc_filter, ui4_len);
	ui4_vara_len = _scpt_query_str_len(puc_filter, ui4_len);
	if( ui4_vara_len == 0x0 )
	{
		return SCPT_U4_INVALID;
	}
	
	x_memset(pc_num_str, 0, ui4_vara_len+1);
	x_strncpy(pc_num_str, puc_filter, ui4_vara_len);
	//if( 0x0 != x_memcmp(pc_num_str, uc_key_str, ui4_vara_len) );//make sure key string is "tunables"
	{
		//return SCPT_U4_INVALID;
	}
	puc_filter += ui4_vara_len;

	do{
		puc_filter = _scpt_filter_space(puc_filter, ui4_len);
		ui4_vara_len = _scpt_query_number_str_len(puc_filter, ui4_len);
		if( ui4_vara_len == 0x0 )
		{
			return SCPT_U4_INVALID;
		}
		x_memset(pc_num_str, 0, ui4_vara_len+1);
		x_strncpy(pc_num_str, puc_filter, ui4_vara_len);

		switch( ui4_loop_time )
		{
		case 0:
			pt_tunable_info->i4_batch_count = x_strtoll(pc_num_str, NULL, 10);
			break;
		case 1:
			pt_tunable_info->i4_limit = x_strtoll(pc_num_str, NULL, 10);
			break;
		case 2:
			pt_tunable_info->i4_shared_factor = x_strtoll(pc_num_str, NULL, 10);
			break;
		default:
			break;
		}
		
		puc_filter += ui4_vara_len;
		ui4_loop_time++;
	}while( ui4_loop_time < 0x3 );

	return CLIR_OK;
}

static INT32 _scpt_parser_read_data_info( CHAR* puc_str, UINT32 ui4_len, 
								SLAB_DATA_INFO_T* pt_data_info	)
{
	CHAR* puc_filter 		= puc_str;
	UINT32 ui4_vara_len		= 0x0;
	UINT32 ui4_loop_time		= 0x0;
	//CHAR uc_key_str[]		= "slabdata";
	CHAR pc_num_str[30]      	= "";

	puc_filter = _scpt_filter_space(puc_filter, ui4_len);
	ui4_vara_len = _scpt_query_str_len(puc_filter, ui4_len);
	if( ui4_vara_len == 0x0 )
	{
		x_dbg_stmt("_scpt_query_str_len ui4_vara_len == 0x0\n");
		return SCPT_U4_INVALID;
	}

	x_memset(pc_num_str, 0, ui4_vara_len+1);
	x_strncpy(pc_num_str, puc_filter, ui4_vara_len);
	//if( 0x0 == x_memcmp(pc_num_str, uc_key_str, ui4_vara_len) );//make sure key string is "slabdata"
	{
		//return SCPT_U4_INVALID;
	}
	puc_filter += ui4_vara_len;

	do{
		puc_filter = _scpt_filter_space(puc_filter, ui4_len);
		ui4_vara_len = _scpt_query_number_str_len(puc_filter, ui4_len);
		if( ui4_vara_len == 0x0 ){
			x_dbg_stmt("_scpt_query_number_str_len ui4_vara_len == 0x0, ui4_len=%u\n", ui4_len);
			return SCPT_U4_INVALID;
		}
		x_memset(pc_num_str, 0, ui4_vara_len+1);
		x_strncpy(pc_num_str, puc_filter, ui4_vara_len);

		switch( ui4_loop_time ){
		case 0:
			pt_data_info->i4_active_slabs = x_strtoll(pc_num_str, NULL, 10);
			break;
		case 1:
			pt_data_info->i4_num_slabs = x_strtoll(pc_num_str, NULL, 10);
			break;
		case 2:
			pt_data_info->i4_shared_avail = x_strtoll(pc_num_str, NULL, 10);
			break;
		default:
			break;
		}

		puc_filter += ui4_vara_len;

		ui4_loop_time++;
	}while( ui4_loop_time < 0x3 );

	return CLIR_OK;
}

static SLAB_DETAIL_INFO_T* _scpt_parser_gen_slab_detail_list( 
			CHAR** ppt_read_buf , UINT32 ui4_len )
{
    SLAB_DETAIL_INFO_T* pt_list_info = NULL;
    SLAB_DETAIL_INFO_T* pt_head = NULL;
    CHAR* puc_filter 	= *ppt_read_buf;
    CHAR* puc_filter_end	= (*ppt_read_buf + ui4_len);
    UINT32 ui4_loop = 0x0;
    UINT32 ui4_pos = 0x0;
    INT32 i4_ret 	= 0x0;

    /*  filter the first line: "slabinfo - version: 2.0" */
    /*  filter the second line: "uc_name <active objs> <i4_num_objs> ... ..." */
    do
    {
        ui4_pos = _scpt_find_first_char(puc_filter, ui4_len, SCPT_CHAR_LF);
        if( ui4_pos == SCPT_U4_INVALID )
        {
            x_dbg_stmt("=====_scpt_find_first_char find SCPT_CHAR_LF(%d) failed!\n", ui4_loop);
            goto EXIT;
        }

        puc_filter += ++ui4_pos;
        ui4_len -= ui4_pos;

        ui4_loop++;
    }while( ui4_loop < 0x2 );
	
    /* read slab info detail line by line until finish scaning the whole buffer */
    while( puc_filter < puc_filter_end )
    {
        ui4_pos = _scpt_find_first_char(puc_filter, ui4_len, SCPT_CHAR_LF);
        if( ui4_pos == SCPT_U4_INVALID )
        {
            x_dbg_stmt("======_scpt_parser_gen_slab_detail_list read buffer fetal error!\n");
            goto EXIT;
        }

        pt_list_info = _scpt_alloc_slab_info(&pt_head);
        if( pt_list_info == NULL)
        {
            x_dbg_stmt("=====_scpt_parser_gen_slab_detail_list alloc slab info error!\n");
            goto EXIT;
        }

        i4_ret = _scpt_parser_read_slab_info(puc_filter, ui4_pos, pt_list_info);
        if( i4_ret  == SCPT_U4_INVALID )
        {
            x_dbg_stmt("=====_scpt_parser_gen_slab_detail_list set slabinfo fetal error!\n");
            goto EXIT;
        }

        puc_filter += ++ui4_pos;
        ui4_len -= ui4_pos;
        if( ui4_len == 0x0 )
        {
            x_dbg_stmt("=====_scpt_parser_gen_slab_detail_list set slabinfo list complete!\n");
            goto EXIT;
        }
    }
	
EXIT:
    return pt_head;
}

static UINT32 _scpt_find_first_char( CHAR* puc_str , UINT32 ui4_len , CHAR uc_char )
{
	CHAR* puc_find			= puc_str;
	CHAR* puc_find_end		= (puc_str + ui4_len);
	INT32 i4_pos			= 0x0;

	if( NULL == puc_str )
	{
		i4_pos = SCPT_U4_INVALID;
	}

	if( 0x0 == ui4_len )
	{
		i4_pos = SCPT_U4_INVALID;
	}

	while( (uc_char != *puc_find) &&  (puc_find < puc_find_end) )
	{
		i4_pos++;
		puc_find++;
	}

	if( i4_pos > ui4_len-1 )
	{
		i4_pos = SCPT_U4_INVALID;
	}

	return i4_pos;
}

static BOOL _scpt_is_word_char_ex( CHAR* puc_str )
{
	if( NULL == puc_str )
	{
		return FALSE;
	}

	if( ( '(' == puc_str[0] ) || ( ')' == puc_str[0] ) )
	{
		return TRUE;
	}

	if( '-' == puc_str[0] )
	{
		return TRUE;
	}

	if( _scpt_is_word_char(puc_str) == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}

static UINT32 _scpt_query_str_len_ex( CHAR* puc_str , UINT32 ui4_len )
{
    UINT32 ui4_loop         = 0x0;
    UINT32 ui4_loop_len  = ui4_len;
    CHAR* puc_filter        = puc_str;

    if(NULL == puc_str)
    {
        return 0x0;
    }

    /* 1.  check is ward  char or not */

    if( TRUE != _scpt_is_word_char_ex(puc_str) )
    {
        return 0x0;
    }

    /* 2.  filter new line char */
    ui4_loop_len = ( ( puc_str + ui4_len) - puc_filter );
    while( (TRUE == _scpt_is_word_char_ex(puc_str)) && (ui4_loop<ui4_loop_len) )
    {
        puc_str++;
        ui4_loop++;
    }

    return ui4_loop;
}

#else //for nucleus situation
static INT32 _mm_hdlr_mm_monitor_memory(INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T h_mem_hdl = NULL_HANDLE;
    SIZE_T z_total_free = 0x0;
    SIZE_T z_biggest_free = 0x0;
    SIZE_T z_free_obj = 0x0;
    SIZE_T z_allocated_obj = 0x0;
    SIZE_T z_total_size = 0x0;
    SIZE_T z_alloc_size = 0x0;
    INT32 i4_ret = 0x0;
    CHAR s_name[32];
    BOOL b_update = FALSE;
	
	
    if (i4_argc > (INT32) 1)
    {
        b_update = (x_strtoull(pps_argv[1], NULL, 0)>0?TRUE:FALSE);
        if(i4_argc > (INT32)2)
        {
            h_mem_hdl = (HANDLE_T) x_strtoull(pps_argv[2], NULL, 0);
            if (h_mem_hdl == (HANDLE_T) 0)
            {
                h_mem_hdl = NULL_HANDLE;
            }
        }
    }
    else
    {
        h_mem_hdl = NULL_HANDLE;
    }
	
    i4_ret = os_mem_scan_mem(h_mem_hdl);
    if (i4_ret == OSR_OUT_BOUND)
    {
        x_dbg_stmt("Memory partition %d might have been corrupted\n",
					h_mem_hdl);
    }
	
    os_mem_cli_stats(h_mem_hdl,
					&z_total_free,
					&z_biggest_free,
					&z_free_obj,
					&z_allocated_obj,
					&z_total_size,
					&z_alloc_size,
					&(s_name[0]));
	
    x_dbg_stmt("\nMemory Partition Statistics:\n \
					Handle: %d\n \
					Name: %s\n \
					Allocation Type(%d): %s\n \
					Total Size: 		%8d Bytes\n \
					Total Free Size:	%8d Bytes\n \
					Biggest Free Block: %8d Bytes\n \
					Allocated Blocks: %d\n \
					Free Blocks: %d\n",
					h_mem_hdl, s_name,
					z_alloc_size,
					((z_alloc_size == 0) ? "Dynamic" : "Fixed"),
					z_total_size, z_total_free, z_biggest_free,
					z_allocated_obj, z_free_obj);
	
    _scpt_update_memory_leakage( (INT32)(z_total_free/1000), b_update);

    return CLIR_OK;
}

static INT32 _scpt_update_memory_leakage(INT32 i4_count, BOOL b_update_ref)
{
    MEM_MONITOR_INFO_T* pt_mem_info = NULL;
    pt_mem_info = _scpt_get_meminfo();

    if(pt_mem_info->b_first_set)
    {
        x_dbg_stmt("=====Update leakage info first\n");
        pt_mem_info->b_first_set = FALSE;
        pt_mem_info->i4_last_avail_mem = i4_count;
        pt_mem_info->i4_avail_mem = i4_count;
        return CLIR_OK;
    }
	
    if(b_update_ref)
    {
        pt_mem_info->i4_last_avail_mem = pt_mem_info->i4_avail_mem;
    }
    pt_mem_info->i4_avail_mem = i4_count;
    pt_mem_info->i4_delta_mem = pt_mem_info->i4_avail_mem - pt_mem_info->i4_last_avail_mem;

    return CLIR_OK;
}
#endif

static MEM_MONITOR_INFO_T* _scpt_get_meminfo( VOID )
{
    return &rMemInfo;
}

static INT32 _mm_hdlr_mm_reset_meminfo( INT32 i4_argc, const CHAR** pps_argv )
{
    MEM_MONITOR_INFO_T* pt_mem_info = NULL;
    pt_mem_info = _scpt_get_meminfo();

    pt_mem_info->b_first_set = TRUE;
    pt_mem_info->b_mem_leak = FALSE;

#ifndef LINUX_TURNKEY_SOLUTION
    pt_mem_info->i4_last_avail_mem = 0x0;
    pt_mem_info->i4_avail_mem = 0x0;
    pt_mem_info->i4_delta_mem = 0x0;
#else
    pt_mem_info->ui4_threshold = 0xfffffff;
    if(i4_argc > 1)
    {
        pt_mem_info->ui4_threshold = x_strtoll(pps_argv[1], NULL, 0);
    }
    /* release history slab list*/
    _scpt_free_slab_list(& (pt_mem_info->pt_last_slab_list));

    /* release current slab list */
    _scpt_free_slab_list(& (pt_mem_info->pt_slab_list));

    /* release delta slab list */
    _scpt_free_slab_list(& (pt_mem_info->pt_delta_list));

#endif

    return CLIR_OK;
}

/* this function performs an execution of listing the detail memory leakage info for both
 linux and nucleus environment. */
static INT32 _mm_hdlr_mm_show_meminfo( INT32 i4_argc, const CHAR** pps_argv )
{
    UINT32 			ui4_loop;
    MEM_MONITOR_INFO_T* pt_mem_info = NULL;
    pt_mem_info = _scpt_get_meminfo();

#ifdef LINUX_TURNKEY_SOLUTION
    SLAB_DETAIL_INFO_T* pt_delta_list = NULL;

    pt_delta_list = pt_mem_info->pt_delta_list;
    _scpt_free_slab_list(&pt_delta_list);

    pt_delta_list = _scpt_calc_delta_slab_info(pt_mem_info->pt_last_slab_list, 
    				pt_mem_info->pt_slab_list);
    if(pt_delta_list == NULL)
    {
        x_dbg_stmt("=====Calculate delta slab info error!\n");
        return SCPT_U4_INVALID;
    }

    pt_delta_list = _scpt_filter_delta_slab_info(pt_delta_list);
    ui4_loop =  _scpt_query_slab_list_length(pt_delta_list); 
    if(ui4_loop)
    {
        
        x_dbg_stmt("\n========[Linux Memory Leakage Result]========\n");
        x_dbg_stmt("========[name]\t [delta_slab_nums(active)]\t [obj_size]\n");
        while(pt_delta_list)
        {
            x_dbg_stmt("========%s\t %d\t %d\n", 
				pt_delta_list->pt_obj_info->uc_name,
				pt_delta_list->pt_obj_info->i4_active_objs,
				pt_delta_list->pt_obj_info->i4_obj_size);

	     pt_delta_list = pt_delta_list->pt_next;
        }
    }
    else
    {
        x_dbg_stmt("========There seems to be no memory leakage.\n");
    }
#else
    x_dbg_stmt("\n========[Nucleus Memory Leakage Result]========\n");
    x_dbg_stmt("========leaknums:(%d Kb).\n", 
    			pt_mem_info->i4_delta_mem);
#endif 

    x_dbg_stmt("========[Memory Leakage check out complete]========\n\n");

    return CLIR_OK;
}
#endif//!#ifdef AUTOTEST_MEMLEAK_CHK

#endif //!#ifdef MULTIMEDIA_AUTOTEST_SUPPORT

/* */
static INT32 svctx_cli_open_timecode(INT32 i4_argc, const CHAR** pps_argv)
{
    const CHAR*         s_str;

    s_str = pps_argv[1];

    if (x_strncmp(s_str, "on", 2) == 0)
    {
        x_dbg_stmt("\n-------timecode cli on---------\n");
        gb_tm_on_off = TRUE;
    }
    else if (x_strncmp(s_str, "off", 3) == 0)
    {
        x_dbg_stmt("\n--------timecode cli off--------\n");
        gb_tm_on_off = FALSE;
    }

    return CLIR_OK;
}

static INT32 _mm_hdlr_divx_test (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                i4_ret;
    UINT32               ui4_pl_id;
    UINT32               ui4_ch_id;
    MM_SVC_POS_INFO_T    t_pos_set;
    const CHAR* s_str;
    CHAR* s_endptr;
        
    if (i4_argc != 3)
    {
        x_dbg_stmt("Usage: divx pl_id ch_id\r\n");
        return CLIR_INV_ARG;
    }
    
    s_str = pps_argv[1];
    ui4_pl_id = x_strtoll(s_str,&s_endptr,10);
    if (s_endptr == s_str)
    {
       return CLIR_INV_ARG;
    }
    
    s_str = pps_argv[2];
    ui4_ch_id = x_strtoll(s_str,&s_endptr,10);
    if (s_endptr == s_str)
    {
       return CLIR_INV_ARG;
    }

    t_pos_set.t_pos_type = MM_SVC_POS_TYPE_CHAPTER;
    t_pos_set.ui8_pos_val = MM_DIVX_PLUS_ID_TO_POS(ui4_pl_id, ui4_ch_id);
    i4_ret = x_svctx_set(
                    h_g_main_svctx,
                    SVCTX_MM_SET_TYPE_SEEK,
                    (VOID*)&t_pos_set,
                    sizeof(t_pos_set)
                    );
    if(SVCTXR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    return CLIR_OK;
}
#else
/*----------------------------------------------------------------------------
 * Name:    mm_hdlr_cli_attach_cmd_tbl
 * Description:
 * Input arguments:  -
 * Output arguments: -
 * Returns:
 *----------------------------------------------------------------------------*/
INT32 mm_hdlr_cli_attach_cmd_tbl(VOID)
{
    return CLIR_OK;
}
#endif


