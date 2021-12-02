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
 * $RCSfile: sbtl_text_page_inst_mngr.h,v $
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

#ifndef _SBTL_MKV_TEXT_PAGE_INST_MNGR_H_
#define _SBTL_MKV_TEXT_PAGE_INST_MNGR_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define     TEXT_BUF_LEN_MAX    256
typedef enum
{
    MKV_TEXT_STATE_NONE,
    MKV_TEXT_STATE_DATA_RDY,
    MKV_TEXT_STATE_DISPLAY,
    MKV_TEXT_STATE_END
} MKV_TEXT_PAGE_STATE_T;

typedef INT32 (*sbtl_seek_time_stamp_fnc)(SBTL_PAGE_INST_MNGR_T*    pt_mngr,
                                          PTS_T                     t_pts_current,
                                          PTS_T*                    pt_pts_start,
                                          PTS_T*                    pt_pts_end,
                                          BOOL*                     pb_found);

typedef INT32 (*sbtl_load_string_fnc)(SBTL_PAGE_INST_MNGR_T*    pt_mngr);

typedef UINT32 (*sbtl_timer_period_fnc)(SBTL_PAGE_INST_MNGR_T*  pt_mngr);

typedef INT32 (*sbtl_init_fnc)(SBTL_PAGE_INST_MNGR_T*   pt_mngr);

typedef INT32 (*sbtl_deinit_fnc)(SBTL_PAGE_INST_MNGR_T* pt_mngr);

typedef BOOL (*text_parser_verify_fnc)(SBTL_ENGINE_T*   pt_engine);

typedef VOID (*text_parser_start)(SBTL_PAGE_INST_MNGR_T*    pt_mngr);

typedef VOID (*text_parser_stop)(SBTL_PAGE_INST_MNGR_T*     pt_mngr);

typedef struct _SBTL_MKV_TEXT_FNC_TBL_T
{
    MM_SBTL_TYPE_T              e_sbtl_text_type;
    sbtl_seek_time_stamp_fnc    pf_seek_time;
    sbtl_load_string_fnc        pf_load_string;
    sbtl_timer_period_fnc       pf_timer_period;
    sbtl_init_fnc               pf_init;
    sbtl_deinit_fnc             pf_deinit;
    text_parser_start           pf_start;
    text_parser_stop            pf_stop;
    text_parser_verify_fnc      pf_verify;
}SBTL_MKV_TEXT_FNC_TBL_T;


typedef struct _SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T
{
    BOOL                                b_page_exist;
    
    MKV_TEXT_PAGE_STATE_T               e_state;
    
    HANDLE_T                            h_sbtl_wgl;
    PTS_T                               t_pts_s;
    PTS_T                               t_pts_e;
    PTS_T                               t_append_stc;

    UINT32                              ui4_seek_offset;

    UINT8                               aui1_block_buf[TEXT_BUF_LEN_MAX];
    UINT8                               aui1_text_buf[TEXT_BUF_LEN_MAX];
    UTF16_T                             w2s_text[TEXT_BUF_LEN_MAX];

    SBTL_MKV_TEXT_FNC_TBL_T*            pt_fnc_table;
    
    HANDLE_T                            h_timer;

    UINT64                              ui8_pos;
    VOID*                               pv_data;
}SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
#endif /* _SBTL_MKV_TEXT_PAGE_INST_MNGR_H_ */






