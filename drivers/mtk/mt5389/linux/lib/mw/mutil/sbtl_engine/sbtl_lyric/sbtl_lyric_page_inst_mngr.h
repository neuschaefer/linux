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
 * $RCSfile: sbtl_lrc_page_inst_mngr.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         
 *         
 *---------------------------------------------------------------------------*/

#ifndef _SBTL_LYRIC_PAGE_INST_MNGR_H_
#define _SBTL_LYRIC_PAGE_INST_MNGR_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define SBTTL_LYRIC_TIME_FMT_MAX            16

#define LYRIC_IDX_NULL                0xFFFF /**/
#define LYRIC_IDX_HEADER              0xFFFE /**/

typedef enum
{
    LYRIC_STATE_NONE,
    LYRIC_STATE_DATA_RDY,
    LYRIC_STATE_DISPLAY,
    LYRIC_STATE_END
} LYRIC_PAGE_STATE_T;

typedef enum
{
    LYRIC_HEADER_TAG_TITLE,
    LYRIC_HEADER_TAG_ARTIST,
    LYRIC_HEADER_TAG_ALBUM,
    LYRIC_HEADER_TAG_EDITOR,
    LYRIC_HEADER_TAG_OFFSET,
    LYRIC_HEADER_TAG_MAX_NUM
} LYRIC_TAB_INF_T;

typedef INT32 (*lyric_seek_time_stamp_fnc)(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                                  BOOL                    b_pts_neg,
                                  PTS_T                   t_pts_current,
                                  UINT16                  ui2_time_cnt,
                                  UINT16*                 pui2_found_cnt);

typedef INT32 (*lyric_load_string_fnc)(SBTL_PAGE_INST_MNGR_T*    pt_mngr,
                                             UINT16                    ui2_load_idx);

typedef UINT32 (*lyric_timer_period_fnc)(SBTL_PAGE_INST_MNGR_T*  pt_mngr);

typedef INT32 (*lyric_init_fnc)(SBTL_PAGE_INST_MNGR_T*   pt_mngr);

typedef INT32 (*lyric_deinit_fnc)(SBTL_PAGE_INST_MNGR_T* pt_mngr);

typedef BOOL (*lyric_parser_verify_fnc)(SBTL_ENGINE_T*   pt_engine);

typedef VOID (*lyric_parser_start)(SBTL_PAGE_INST_MNGR_T*    pt_mngr);

typedef VOID (*lyric_parser_stop)(SBTL_PAGE_INST_MNGR_T*     pt_mngr);

typedef INT32 (*lyric_config)( SBTL_PAGE_INST_MNGR_T*     pt_mngr,
                             VOID*                      pv_set_info,
                             UINT32                     ui4_set_type);

typedef INT32 (*lyric_get_info)( SBTL_PAGE_INST_MNGR_T*        pt_mngr,
                                UINT32                        ui4_get_type,
                                VOID*                         pv_get_info,
                                SIZE_T                        z_get_info_len);


typedef struct _SBTL_LYRIC_FNC_TBL_T
{
    MM_SBTL_TYPE_T               e_sbtl_text_type;
    lyric_seek_time_stamp_fnc    pf_seek_time;
    lyric_load_string_fnc        pf_load_string;
    lyric_timer_period_fnc       pf_timer_period;
    lyric_init_fnc               pf_init;
    lyric_deinit_fnc              pf_deinit;
    lyric_parser_start           pf_start;
    lyric_parser_stop            pf_stop;
    lyric_parser_verify_fnc       pf_verify;
    lyric_config                 pf_config;
    lyric_get_info               pf_get_info;
}SBTL_LYRIC_FNC_TBL_T;

typedef struct _SBTL_LYRIC_PAGE_MNGR_PRIVATE_T
{
    BOOL                                b_page_exist;
    /*It used for tag information format.True,it uses two byte,else it use one byte*/
    BOOL                                b_UTF16;
    MM_SBTL_LANG_T                      e_language;
    
    HANDLE_T                            h_sbtl_wgl;
    UINT16                              ui2_cur_idx;
    PTS_T                               t_pts_s;
    PTS_T                               t_pts_e;
    UINT16                              ui2_page_str_num;
    UINT16                              ui2_hlt_page_idx;
    UINT16                              ui2_page_s;
    UINT16                              ui2_page_e;
    UINT16                              ui2_disp_num;

    LYRIC_PAGE_STATE_T                  e_state;

    UINT32                              ui4_seek_offset;
    UINT8                               aui1_text_buf[MM_SBTL_CACHE_MAX];
    UTF16_T                             w2s_text[MM_SBTL_CACHE_MAX];

    SBTL_LYRIC_FNC_TBL_T*               pt_fnc_table;
    
    HANDLE_T                            h_timer;

    /*The ui1_time_fmt_cnt value must less than SBTTL_LYRIC_TIME_FMT_MAX*/
    UINT8                               ui1_time_fmt_cnt; 
    SBTL_TAG_FORMAT_T                   at_time_fmt[SBTTL_LYRIC_TIME_FMT_MAX];

    UINT16                              ui2_time_max_cnt;
    UINT16                              ui2_time_cnt;
    SBTL_TAG_INFO_T*                    pt_time_stamp;
    UINT16*                             pui2_time_order;

    SBTL_TAG_FORMAT_T                   at_header_tag[LYRIC_HEADER_TAG_MAX_NUM];
    SBTL_TAG_INFO_T                     at_header_inf[LYRIC_HEADER_TAG_MAX_NUM];
    SBTL_TAG_FORMAT_T                   t_end_tag;
    
    MM_SBTL_TM_OFST_MODE                e_tm_ofst_mode;
    INT64                               i8_pts_offset;
    INT64                               i8_usr_pts_ofst;
    
    VOID*                               pv_data;
}SBTL_LYRIC_PAGE_MNGR_PRIVATE_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

extern INT32 sbtl_lyric_alloc_time_stamp(
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*         pt_private,
    UINT16                                  ui2_time_stamp_cnt);

extern INT32 sbtl_lyric_free_time_stamp(SBTL_LYRIC_PAGE_MNGR_PRIVATE_T* pt_private);

extern INT32 sbtl_lyric_time_stamp_order(
    SBTL_PAGE_INST_MNGR_T*              pt_mngr,
    UINT16                              ui2_order_start,
    UINT16                              ui2_order_end);

#endif /* _SBTL_LYRIC_PAGE_INST_MNGR_H_ */






