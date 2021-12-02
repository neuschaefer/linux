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
 * $RCSfile: avi_parser.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0916b8010fb2d9ebf453b3d4efdbc5e9 $ Asa
 *
 * Description: 
 *         This file exports public API of the avi parser function
 *---------------------------------------------------------------------------*/

#ifndef _AVI_PARSER_H_
#define _AVI_PARSER_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/minfo_dbg.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "avi_menu_parser.h"
#else

#include "u_common.h"
#include "u_handle.h"
#include "../minfo.h"
#include "../minfo_dbg.h"
#include "../../mfmtrecg/u_mfmtrecg.h"
#include "avi_menu_parser.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
----------------------------------------------------------------------------*/

#define MAX_NS_AVI_AUD 8
#define MAX_NS_AVI_VID 2
#define MAX_NS_AVI_SP  8

#define AVI_HEADER_BUF_LENGTH     102400

typedef enum
{
    MINFO_AVI_PARSER_STATE_IDLE,

#ifdef MM_AVI_DIVX6
    /* The parser state for searching menu */
    MINFO_AVI_PARSER_STATE_DMC_SHOW_MENU,
    
    /* The parser state for parsing menu */
    MINFO_AVI_PARSER_STATE_DMC_PARSE_MENU,
    
    /* The parser state for getting sub-picture data */
    MINFO_AVI_PARSER_STATE_DMC_GET_SP,
    
    /* The parser state for getting audio/video/sub-picture data */
    MINFO_AVI_PARSER_STATE_DMC_GET_DATA,
#endif /* MM_AVI_DIVX6 */

    MINFO_AVI_PARSER_STATE_CHECK_CHUNK_HEADER,    
    MINFO_AVI_PARSER_STATE_CHECK_CHUNK_TYPE,    
    MINFO_AVI_PARSER_STATE_LOAD_INF,
    MINFO_AVI_PARSER_STATE_LOAD_INDEX_INF
}MINFO_AVI_PARSER_STATE_T;

typedef enum 
{
    MINFO_AVI_CHUNK_ID_NONE,
    MINFO_AVI_CHUNK_ID_RIFF,
    MINFO_AVI_CHUNK_ID_LIST,
    MINFO_AVI_CHUNK_ID_AVI,
    MINFO_AVI_CHUNK_ID_HDRL,
    MINFO_AVI_CHUNK_ID_AVIH,
    MINFO_AVI_CHUNK_ID_STRL,
    MINFO_AVI_CHUNK_ID_STRH,
    MINFO_AVI_CHUNK_ID_STRF,
    MINFO_AVI_CHUNK_ID_STRD,
    MINFO_AVI_CHUNK_ID_AUDS,
    MINFO_AVI_CHUNK_ID_MIDS,
    MINFO_AVI_CHUNK_ID_VIDS,
    MINFO_AVI_CHUNK_ID_TXTS,
    MINFO_AVI_CHUNK_ID_JUNK,
    MINFO_AVI_CHUNK_ID_RES1,
    MINFO_AVI_CHUNK_ID_PRMI,
    MINFO_AVI_CHUNK_ID_MTDA,
    MINFO_AVI_CHUNK_ID_MOV,
    MINFO_AVI_CHUNK_ID_IDX1,
    MINFO_AVI_CHUNK_ID_INDX,
    MINFO_AVI_CHUNK_ID_STRN,
    MINFO_AVI_CHUNK_ID_INFO,
    MINFO_AVI_CHUNK_ID_IART,
    MINFO_AVI_CHUNK_ID_ICOP,
    MINFO_AVI_CHUNK_ID_IGNR,
    MINFO_AVI_CHUNK_ID_INAM,
   /* MINFO_AVI_CHUNK_ID_ODML,
    MINFO_AVI_CHUNK_ID_IDIT,
    MINFO_AVI_CHUNK_ID_DMLH,*/
#ifdef MM_AVI_DIVX6
    MINFO_AVI_CHUNK_ID_MENU,
    MINFO_AVI_CHUNK_ID_MRIF,
    MINFO_AVI_CHUNK_ID_1IDX,
#endif    
    MINFO_AVI_CHUNK_ID_FMT,    /*add for wave support*/
    MINFO_AVI_CHUNK_ID_DATA,  /*add for wave support*/
    MINFO_AVI_CHUNK_ID_WAVE, /*add for wave support*/
    MINFO_AVI_CHUNK_ID_FACT,  /*add for wave support*/
    MINFO_AVI_CHUNK_ID_MAX
}MINFO_AVI_CHUNK_ID_ENUM_T;

typedef struct _MINFO_AVI_CHUNK_ID_T
{
    CHAR s_four_cc[5];
    MINFO_AVI_CHUNK_ID_ENUM_T e_chunk_id;
}MINFO_AVI_CHUNK_ID_T;

typedef enum
{
    MINFO_AVI_STRM_TYPE_VID,
    MINFO_AVI_STRM_TYPE_AUD,
    MINFO_AVI_STRM_TYPE_SP,
    MINFO_AVI_STRM_TYPE_MAX
}MINFO_AVI_STRM_TYPE_ENUM_T;


typedef struct _MINFO_AVI_CHUNK_T
{
    MINFO_AVI_CHUNK_ID_ENUM_T   e_chunk_id;
    MINFO_AVI_STRM_TYPE_ENUM_T  e_strm_type;
    UINT32                      ui4_offset;
    UINT32                      ui4_size; 
    struct _MINFO_AVI_CHUNK_T*  pt_first_sub_chunk;
    struct _MINFO_AVI_CHUNK_T*  pt_par_chunk;
    struct _MINFO_AVI_CHUNK_T*  pt_next_chunk;
}MINFO_AVI_CHUNK_T;    



typedef struct _MINFO_AVI_STRM_INF_T 
{
    MINFO_AVI_VIDEO_STM_ATRBT_T t_vid_inf_strh[MAX_NS_AVI_VID];
    MINFO_AVI_VIDEO_STM_ATRBT_T t_vid_strm_inf[MAX_NS_AVI_VID];
    MINFO_AVI_AUDIO_STM_ATRBT_T t_aud_strm_inf[MAX_NS_AVI_AUD];
    MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T t_sp_strm_inf[MAX_NS_AVI_SP];
    UINT8 ui1_aud_strm_id_list[MAX_NS_AVI_AUD];
    UINT8 ui1_vid_strm_id_list[MAX_NS_AVI_VID];
    UINT8 ui1_sp_strm_id_list[MAX_NS_AVI_SP];
    UINT8 ui1_audio_strm_num;
    UINT8 ui1_video_strm_num;
    UINT8 ui1_sp_strm_num;
}MINFO_AVI_STRM_INF_T;


typedef struct _MINFO_AVI_RIFF_INF_T
{
    /*store current parsed chunk header*/
    UINT8* pui1_header_buf;
    /*header buffer size*/
    UINT32 ui4_buf_length;
    /* The pointer to the chunk list */
    MINFO_AVI_CHUNK_T*  pt_head_chunk;
    UINT32 ui4_malloc_count;

    /* Divx Menu */
    /* store parsed menu data */
    UINT8*                      pui1_menu_buf;
    /* pointer of menu object tree */
    DIVX_MENU_OBJECT_T*         pt_menu_object;
    /* buffer to store menu object tree data */
    DIVX_MENU_OBJ_BUF_LIST_T*   pt_menu_obj_buf_list;

    /* Base offset for RIFF focc, if it is Avi1.0 file, the value is zero */
    UINT64 ui8_base_offset;
    /* Riff size, if file is Avi1.0, the value is file size */
    UINT64 ui8_riff_size;
    /* The parser state, it is only useful in build riff tree */
    MINFO_AVI_PARSER_STATE_T e_parser_state;
    /* Current parser offset, file base */
    UINT32 ui4_cur_parser_offset;
    /*index tbl file offset , provide for index builder*/
    /*Start offset include idxl 4cc code*/ 
    UINT32 ui4_indxtbl_offset;
    /*  idx table size */
    /*size doesn't include idxl 4cc code*/
    UINT32 ui4_indxtbl_sz;

    /* Stream information */
    MINFO_AVI_STRM_INF_T t_strm_inf;
    /* video stream id (fourCC) */
    UINT32 aui4_video_strm_id[MAX_NS_AVI_VID];
    /* meta data infomation */
    MINFO_TYPE_META_DATA_T t_meta_data_info;
    /* MPC2 struct */
    /*MPCCmdConfigCFAParm m_rMPCCmdConfigCfaParm;*/
    /* MPC2 struct */
    /*CfaAviConfigInfo m_rAviConfigInf;*/
    
    UINT8 ui1_cur_strm_num;
    /* File length base on frame */
    UINT64 ui8_duration;
    /* Movi offset base ,start from 4cc code */
    UINT32 ui4_mov_start_base;
    /* Movi chunk size, include 4cc code  */
    UINT64 ui8_mov_sz;

    /* File length base on PTS */
    UINT32 ui4_total_pts;
    /* If audio stream is not interleave with video stream, the value is FALSE */
    BOOL b_is_bad_interleaved;
    BOOL b_is_menu_riff;
    /* for some DLNA server, FALSE:the server support the seek operation*/
    BOOL b_non_seekable;

    UINT32 ui4_frm_period;

}MINFO_AVI_RIFF_INF_T;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern INT32 i4_avi_minfo_chunk_parse (MINFO_OBJ_T*    pt_minfo_obj);

#endif /* _AVI_PARSER_H_ */

