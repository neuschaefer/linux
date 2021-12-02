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
 * $RCSfile: sbtl_struct.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         This header file contains Subtitle Stream handler specific 
 *         definitions, which are exported to applications.
 *---------------------------------------------------------------------------*/

#ifndef _SBTL_STRUCT_H_
#define _SBTL_STRUCT_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#include "inc/x_mm_common.h"
#include "mutil/sbtl_engine/x_mm_sbtl_engine.h"
#include "mutil/sbtl_engine/sbtl_state_handler.h"

#else /*__KERNEL__*/
#include "x_mm_common.h"
#include "x_mm_sbtl_engine.h"
#include "sbtl_state_handler.h"

#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define SBTL_DMX_DATA_QUE_MAX_SIZE                                  30
#define FEEDER_CACHE_SIZE                                           512
#define SBTL_TIME_FORMAT_LEN                                        12

#define SBTL_TIME_TAG_NONE            0x0
#define SBTL_TIME_TAG_NOT_FOUND       0x1
#define SBTL_TIME_TAG_FOUND              0x2
#define SBTL_TIME_TAG_OUT              0x3


typedef struct _SBTL_ENGINE_T SBTL_ENGINE_T;
typedef struct _SBTL_PAGE_INST_MNGR_T SBTL_PAGE_INST_MNGR_T;
typedef struct _SBTL_DMX_DATA_QUE_T SBTL_DMX_DATA_QUE_T;

typedef SBTL_PAGE_INST_MNGR_T* (*page_inst_mngr_constructor)( SBTL_ENGINE_T*    pt_engine);

typedef VOID (*page_inst_mngr_destructor)( SBTL_PAGE_INST_MNGR_T*   pt_mngr);

typedef BOOL (*parser_verify)( SBTL_ENGINE_T*   pt_engine,
                               UINT8*           pt_type);

typedef struct _SBTL_INTER_CTR_T
{
    BOOL                        b_has_style;/*True:it has subtitle font style*/ 
    FE_FNT_STYLE                e_font_style;/*the new subtitle font style*/
    FE_FNT_STYLE                e_base_style;/*the base style, it is not sbutitle font style*/
    FE_FNT_STYLE                e_prev_style;/*the prev subtitle font style style*/
}SBTL_INTER_CTR_T;

#define SBTL_SET_FONT_STYLE(_pt_engine,_e_style)        \
    do{                                                 \
        _pt_engine->t_inter_ctr.e_font_style |= _e_style;          \
    }while(0)

#define SBTL_GET_FONT_STYLE(_pt_engine)     (_pt_engine->t_inter_ctr.e_font_style)

#define SBTL_SET_FONT_STYLE_FLAG(_pt_engine, _b_flag)            \
    do{                                                 \
        _pt_engine->t_inter_ctr.b_has_style = _b_flag;                 \
    }while(0)

#define SBTL_GET_FONT_STYLE_FLAG(_pt_engine)  (_pt_engine->t_inter_ctr.b_has_style) 

#define SBTL_SET_BASE_FONT_STYLE(_pt_engine, _e_style)        \
    do{                                                 \
        _pt_engine->t_inter_ctr.e_base_style |= _e_style;          \
    }while(0)

#define SBTL_GET_BASE_FONT_STYLE(_pt_engine)     (_pt_engine->t_inter_ctr.e_base_style)

#define SBTL_SET_PREV_FONT_STYLE(_pt_engine,_e_style)        \
    do{                                                 \
        _pt_engine->t_inter_ctr.e_prev_style |= _e_style;          \
    }while(0)

#define SBTL_GET_PREV_FONT_STYLE(_pt_engine)     (_pt_engine->t_inter_ctr.e_prev_style)

typedef struct _SBTL_MNGR_FCT_TBL_T
{
    BOOL                                b_need_verify;
    page_inst_mngr_constructor          pf_mngr_constructor;
    page_inst_mngr_destructor           pf_mngr_destructor;
    parser_verify                       pf_verify;
} SBTL_MNGR_FCT_TBL_T;


typedef struct _SBTL_TAG_FORMAT_T
{    
    BOOL                                b_check_num;//need to check number
    UINT8                               ui1_fmt_len; /*The format length,it is larger or equal ui1_len*/
    UINT8                               ui1_len;/*The content information length*/
    CHAR                                s_content[SBTL_TIME_FORMAT_LEN];
    UINT8                               ui1_content_pos[SBTL_TIME_FORMAT_LEN];
} SBTL_TAG_FORMAT_T;

typedef struct _SBTL_TAG_INFO_T
{
    PTS_T                       t_pts_s;
    PTS_T                       t_pts_e;
    UINT32                      ui4_offset;
    UINT8                       ui1_length;
}SBTL_TAG_INFO_T;

struct _SBTL_DMX_DATA_QUE_T
{
    UINT16                      ui2_used_entry_cnt;

    UINT16                      ui2_first_entry_idx;

    UINT16                      ui2_last_entry_idx;

    UINT32                      aui4_data1[SBTL_DMX_DATA_QUE_MAX_SIZE];

    UINT32                      aui4_data2[SBTL_DMX_DATA_QUE_MAX_SIZE];

    UINT32                      aui4_pts[SBTL_DMX_DATA_QUE_MAX_SIZE];

    UINT32                      aui4_duration[SBTL_DMX_DATA_QUE_MAX_SIZE];

    PTS_T                       at_stc[SBTL_DMX_DATA_QUE_MAX_SIZE];
};


struct _SBTL_ENGINE_T
{
    SBTL_CFG_T                  t_cfg;

    SBTL_STATE_T                t_state;

    SBTL_DMX_DATA_QUE_T         t_data_q;

    MEDIA_FEEDER_T*             pt_feeder;

    SBTL_PAGE_INST_MNGR_T*      pt_page_mngr;

    SBTL_DMX_DATA_QUE_T         t_data_que;

    SBTL_MNGR_FCT_TBL_T*        pt_fct_tbl;
    
    UINT64                      aui8_file_size[MM_SBTL_FEEDER_MAX];
    UINT32                      aui4_read_count[MM_SBTL_FEEDER_MAX];
    UINT64                      aui8_pos[MM_SBTL_FEEDER_MAX];
    UINT64                      aui8_cache_pos[MM_SBTL_FEEDER_MAX];
    UINT8                       aui1_cache_buf[MM_SBTL_FEEDER_MAX][FEEDER_CACHE_SIZE];
    
    SBTL_INTER_CTR_T            t_inter_ctr;

    HANDLE_T                    h_sbtl_wgl;
    HANDLE_T                    h_dmx;
    HANDLE_T                    h_stc;
    HANDLE_T                    h_get_sem;
};

typedef INT32 (*sbtl_start)( SBTL_PAGE_INST_MNGR_T*     pt_mngr);

typedef INT32 (*sbtl_stop)( SBTL_PAGE_INST_MNGR_T*      pt_mngr);

typedef INT32 (*sbtl_need_dmx)( SBTL_PAGE_INST_MNGR_T*      pt_mngr, 
                                BOOL*                       pb_need);

typedef INT32 (*sbtl_handle_timing)( SBTL_PAGE_INST_MNGR_T*     pt_mngr, 
                                     PTS_T                      t_pts);

typedef INT32 (*sbtl_config)( SBTL_PAGE_INST_MNGR_T*        pt_mngr,
                               VOID*                         pv_cfg_info,
                              UINT32                        ui4_set_type);

typedef INT32 (*sbtl_get)( SBTL_PAGE_INST_MNGR_T*        pt_mngr,
                           UINT32                        ui4_get_type,
                           VOID*                         pv_get_info,
                           SIZE_T                        z_get_info_len);

typedef INT32 (*sbtl_new_page)( SBTL_PAGE_INST_MNGR_T*      pt_mngr);

struct _SBTL_PAGE_INST_MNGR_T
{
    sbtl_start              pf_start;
    sbtl_stop               pf_stop;
    sbtl_need_dmx           pf_need_dmx;
    sbtl_handle_timing      pf_handle_timing;
    sbtl_config             pf_cfg;
    sbtl_new_page           pf_new_page;
    sbtl_get                pf_get;

    BOOL                    b_started;
    SBTL_ENGINE_T*          pt_engine;

    VOID*                   pv_private;
};

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

#endif /* _SBTL_STRUCT_H_ */
