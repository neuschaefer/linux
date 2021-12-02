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
 * mp4_midx_parser.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * BingsaiJin
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the mp4 parser function
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif

#include "mutil/mm_util.h"

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/midxbuld/midxbuld.h"
#include "mutil/midxbuld/x_midxbuld.h"
#include "mutil/minfo/mp4_info/mp4_common.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "unicode/x_uc_str.h"
#include "mp4_midxbuld.h"
#include "mp4_midx_parser.h"


#else

#include "x_os.h"
#include "u_common.h"
#include "u_handle.h"
#include "../midxbuld.h"
#include "../x_midxbuld.h"

#include "../../mm_util.h"

#include "x_uc_str.h"
#include "mp4_midxbuld.h"
#include "mp4_midx_parser.h"
#include "../../minfo/mp4_info/mp4_common.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MIDX_MP4_PARSE_NEED_NEXT_LOOP        0x0000000F

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                     functions declarations
----------------------------------------------------------------------------*/
static BOOL _box_is_need_parser(MP4_BOX_ID_ENUM_T e_id)
{
    return (e_id == MP4_BOX_ID_MVHD 
            || e_id == MP4_BOX_ID_TKHD
            || e_id == MP4_BOX_ID_MDHD
            || e_id == MP4_BOX_ID_STTS
            || e_id == MP4_BOX_ID_STSC
            || e_id == MP4_BOX_ID_STSZ
            || e_id == MP4_BOX_ID_STCO
            || e_id == MP4_BOX_ID_CO64
            || e_id == MP4_BOX_ID_CTTS
            || e_id == MP4_BOX_ID_STSS);
}


static INT32 _add_box_to_tree(MIDXBULD_OBJ_T*           pt_midx_obj, 
                              MP4_BOX_ID_ENUM_T         e_id, 
                              UINT64                    u8Offset,
                              UINT32                    u4Size)
{
    MP4_IDXBULD_OBJ_T*      pt_mp4_hdlr_obj;
    MP4_BOX_T*         pt_box;
    MP4_BOX_T*         pt_par_box;
    MP4_BOX_T*         pt_last_Sub;
    
    pt_mp4_hdlr_obj = (MP4_IDXBULD_OBJ_T*)pt_midx_obj->pv_handler_obj;
    pt_box = NULL;
    pt_par_box = NULL;
    pt_last_Sub = NULL;

    _mp4_get_par_box_by_offset(pt_mp4_hdlr_obj->pt_tree_box, 
                           pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                           &pt_par_box);

    pt_box = (MP4_BOX_T *)(_mm_util_mem_alloc(sizeof(MP4_BOX_T)));

    if (pt_box == NULL)
    {
        return MIDXBULDR_OK;
    }
    
    pt_mp4_hdlr_obj->ui4_malloc_count += 1;

    if (pt_par_box!= NULL)
    {
        _mp4_get_last_sub_box(pt_par_box, &pt_last_Sub);
    } 
    else if (pt_mp4_hdlr_obj->pt_tree_box == NULL)
    {
        pt_mp4_hdlr_obj->pt_tree_box = pt_box;
    }
    if (pt_last_Sub == NULL)
    {
        if (pt_par_box != NULL)
        {
            pt_par_box->pt_first_sub_box = pt_box;
        }    
    }
    else
    {
        pt_last_Sub->pt_next_box= pt_box;
    }  
    pt_box ->ui8_offset= u8Offset;
    pt_box->e_box_id= e_id;
    pt_box->ui4_size= u4Size;
    pt_box->pt_parent_box= pt_par_box;
    pt_box->pt_first_sub_box= NULL;
    pt_box->pt_next_box= NULL;
    
    return MIDXBULDR_OK;
}

static INT32 _delete_track(MIDXBULD_OBJ_T* pt_midx_obj, MP4_BOX_T* pt_cur_box)
{
    MP4_BOX_T*     pt_par_box;
    MP4_IDXBULD_OBJ_T*  pt_mp4_hdlr_obj;
    
    pt_mp4_hdlr_obj = (MP4_IDXBULD_OBJ_T*)pt_midx_obj->pv_handler_obj;   

    while (pt_cur_box != NULL)
    {
        if (pt_cur_box->pt_first_sub_box != NULL)
        {
            pt_cur_box = pt_cur_box->pt_first_sub_box;
        }    
        else 
        {
            pt_par_box = pt_cur_box->pt_parent_box;
            if (pt_par_box == NULL)
            {               
                _mm_util_mem_free(pt_cur_box);
                pt_mp4_hdlr_obj->ui4_malloc_count -= 1;
                break;
            }
            if ((pt_par_box != NULL) && (pt_par_box->pt_first_sub_box == pt_cur_box))
            {
                pt_par_box->pt_first_sub_box = pt_cur_box ->pt_next_box;
            }   
            if (pt_cur_box->pt_first_sub_box == NULL)
            {                
                _mm_util_mem_free(pt_cur_box);
                pt_mp4_hdlr_obj->ui4_malloc_count -= 1;
            }
            pt_cur_box = pt_par_box;
        }
     }
    return MIDXBULDR_OK;
}

static INT32 _delete_track_tree(MIDXBULD_OBJ_T*  pt_midx_obj)
{
   MP4_IDXBULD_OBJ_T* pt_mp4_hdlr_obj;
   pt_mp4_hdlr_obj = (MP4_IDXBULD_OBJ_T*)pt_midx_obj->pv_handler_obj;   
    
    _delete_track(pt_midx_obj, pt_mp4_hdlr_obj->pt_tree_box);
    pt_mp4_hdlr_obj->pt_tree_box= NULL;
    return MIDXBULDR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _get_box_id
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MIDXBULDR_OK                 The API is successful. 
 *          MIDXBULDR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/         
static MP4_BOX_ID_ENUM_T _get_box_id(UINT32 ui4_type)
{
    UINT16 u2Idx;
    
    for (u2Idx = 0; u2Idx < MP4_BOX_ID_MAX; u2Idx++)
    {
        if (0 == x_memcmp(&ui4_type, _r_mp4_box_id_map[u2Idx].sz_four_cc, 4))
        {
            return _r_mp4_box_id_map[u2Idx].e_box_id;
        }
    }
    return MP4_BOX_ID_MAX;
}

/*-----------------------------------------------------------------------------
 * Name:  _b_is_track_in_src
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MIDXBULDR_OK                 The API is successful. 
 *          MIDXBULDR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/         
static BOOL _b_is_track_in_src(MIDXBULD_OBJ_T*  pt_midx_obj, UINT32 ui4_track_id)
{
    UINT8   ui1_idx;
    UINT8   ui1_total_strm_num;

    ui1_total_strm_num = pt_midx_obj->t_src_info.ui1_total_strm_num;

    for (ui1_idx = 0; ui1_idx < ui1_total_strm_num; ui1_idx++)
    {
        if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        if (ui4_track_id == pt_midx_obj->t_src_info.t_es_map[ui1_idx].ui4_strm_id)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_midx_moov_track_info
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MIDXBULDR_OK                 The API is successful. 
 *          MIDXBULDR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/                            
static INT32 _mp4_midx_moov_track_info(MIDXBULD_OBJ_T*  pt_midx_obj )
{
    setpos_fct              pf_set_pos;    
    input4bytes_fct         pf_inp4byte;
    input1bytes_fct         pf_inp1byte;
    MP4_IDXBULD_OBJ_T*      pt_mp4_hdlr_obj;

    MP4_IDXBULD_TRACK_T*    pt_curr_track_info;     
    MP4_BOX_T *        pt_tkhd_box;
    MP4_BOX_T *        pt_stts_box;
    MP4_BOX_T *        pt_stsc_box;
    MP4_BOX_T *        pt_stsz_box;
    MP4_BOX_T *        pt_stco_box;
    MP4_BOX_T *        pt_stss_box;
    MP4_BOX_T *        pt_ctts_box;
    MP4_BOX_ID_ENUM_T  e_id;
    UINT32                  ui4_track_id;
    UINT32                  ui4_box_size;
    UINT32                  ui4_fourcc;
    UINT64                  ui8_curr_offset;
    UINT8                   ui1_ver;
    UINT8                   ui1_track_idx;
    UINT32                  ui4_entry_num;
        
    /* check parameter */
    if (pt_midx_obj == NULL)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    if ((pt_midx_obj->t_input_fct_tbl.pf_input4 == NULL)
        || (pt_midx_obj->t_input_fct_tbl.pf_input1 == NULL)
        || (pt_midx_obj->t_input_fct_tbl.pf_set_pos == NULL))
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    
    pf_set_pos = pt_midx_obj->t_input_fct_tbl.pf_set_pos;
    pf_inp4byte = pt_midx_obj->t_input_fct_tbl.pf_input4;
    pf_inp1byte = pt_midx_obj->t_input_fct_tbl.pf_input1;
    
    pt_mp4_hdlr_obj = (MP4_IDXBULD_OBJ_T*)pt_midx_obj->pv_handler_obj;
    
    switch(pt_mp4_hdlr_obj->e_midx_parser_state)
    {
    case MP4_PARSER_STATE_IDLE:
        if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
                (pt_midx_obj->t_src_info.ui8_idx_offset + pt_midx_obj->t_src_info.ui8_idx_size))
        {
            return MIDXBULDR_OK;
        }
        else
        {
            pf_inp4byte(pt_midx_obj, &ui4_box_size);
            pf_inp4byte(pt_midx_obj, &ui4_fourcc);
            ui4_fourcc = _mp4_swap_4bytes(ui4_fourcc);
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
        }
        e_id = _get_box_id(ui4_fourcc);
        if (e_id == MP4_BOX_ID_TRAK)
        {
            pt_mp4_hdlr_obj->e_midx_parser_state = MP4_PARSER_STATE_CHECK_TRACK;
            if (pt_mp4_hdlr_obj->pt_tree_box != NULL)
            {
                _delete_track_tree(pt_midx_obj);
            }
            _add_box_to_tree(pt_midx_obj, 
                             e_id, 
                             pt_mp4_hdlr_obj->ui8_curr_parser_offset - 8, 
                             ui4_box_size);
        }
        if (!_mp4_box_is_need_check_sub(e_id))
        {
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += (ui4_box_size-8);
            pf_set_pos(pt_midx_obj, 
                          (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                          FM_SEEK_BGN);
        }
        break;
        
    case MP4_PARSER_STATE_CHECK_TRACK:
        pf_inp4byte(pt_midx_obj, &ui4_box_size);
        pf_inp4byte(pt_midx_obj, &ui4_fourcc);
        ui4_fourcc = _mp4_swap_4bytes(ui4_fourcc);
        pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
        e_id = _get_box_id(ui4_fourcc);
        if (_box_is_need_parser(e_id) || _mp4_box_is_need_check_sub(e_id) )
        {
            _add_box_to_tree(pt_midx_obj, 
                             e_id, 
                             pt_mp4_hdlr_obj->ui8_curr_parser_offset - 8, 
                             ui4_box_size);
        }
        if (!_mp4_box_is_need_check_sub(e_id))
        {
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += (ui4_box_size-8);
            pf_set_pos(pt_midx_obj, 
                        (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                        FM_SEEK_BGN);
        }
        
        if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
            (pt_mp4_hdlr_obj->pt_tree_box->ui8_offset + pt_mp4_hdlr_obj->pt_tree_box->ui4_size))
        {
            pt_mp4_hdlr_obj->e_midx_parser_state = MP4_PARSER_STATE_LOAD_TRACK_INF;
        }
        break;
        
    case MP4_PARSER_STATE_LOAD_TRACK_INF:
        /* track header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_TKHD, &pt_tkhd_box);
        if (pt_tkhd_box == NULL)
        {
            _delete_track_tree(pt_midx_obj);
            return MIDXBULDR_INTERNAL_ERR;
        }
        ui4_box_size = pt_tkhd_box->ui4_size;
        ui8_curr_offset = pt_tkhd_box->ui8_offset;
        pf_set_pos(pt_midx_obj , (INT64)ui8_curr_offset+8, FM_SEEK_BGN);
                
        pf_inp1byte(pt_midx_obj, &ui1_ver);
        if (ui1_ver == 1)
        {
            pf_set_pos(pt_midx_obj, (INT64)ui8_curr_offset+28, FM_SEEK_BGN);
        }
        else
        {
            pf_set_pos(pt_midx_obj, (INT64)ui8_curr_offset+20, FM_SEEK_BGN);
        }
        pf_inp4byte(pt_midx_obj, &ui4_track_id);
        if (_b_is_track_in_src(pt_midx_obj, ui4_track_id))
        {
            ui1_track_idx = pt_mp4_hdlr_obj->ui1_valid_track_cnt;
            if(ui1_track_idx >= MIDXBULD_ELMT_TBL_MAX)
            {
                return MIDXBULDR_INTERNAL_ERR;/*-----TBD-----*/
            }
            pt_curr_track_info = &(pt_mp4_hdlr_obj->t_track_info_box[ui1_track_idx]);
            pt_curr_track_info->ui4_track_id = ui4_track_id;

            /* time to sample box */
            _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STTS, &pt_stts_box);
            if (pt_stts_box == NULL)
            {
                _delete_track_tree(pt_midx_obj);
                return MIDXBULDR_INTERNAL_ERR;
            }
            else
            {
                /*Check whether this box is valid*/
                pf_set_pos(pt_midx_obj, (INT64)pt_stts_box->ui8_offset + 12, FM_SEEK_BGN);
                pf_inp4byte(pt_midx_obj, &ui4_entry_num);
                if(ui4_entry_num == 0)
                {
                    _delete_track_tree(pt_midx_obj);
                    return MIDXBULDR_FMT_NOT_SUPPORT;
                }
                
                pt_curr_track_info->t_stts.b_is_box_exist = TRUE;
                pt_curr_track_info->t_stts.ui8_box_offset = pt_stts_box->ui8_offset;
                pt_curr_track_info->t_stts.ui4_box_size = pt_stts_box->ui4_size;
            }
            /* sample to chunk box */
            _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STSC, &pt_stsc_box);
            if (pt_stsc_box == NULL)
            {
                _delete_track_tree(pt_midx_obj);
                return MIDXBULDR_INTERNAL_ERR;
            }
            else
            {
                /*Check whether this box is valid*/
                pf_set_pos(pt_midx_obj, (INT64)pt_stsc_box->ui8_offset + 12, FM_SEEK_BGN);
                pf_inp4byte(pt_midx_obj, &ui4_entry_num);
                if(ui4_entry_num == 0)
                {
                    _delete_track_tree(pt_midx_obj);
                    return MIDXBULDR_FMT_NOT_SUPPORT;
                }
                pt_curr_track_info->t_stsc.b_is_box_exist = TRUE;
                pt_curr_track_info->t_stsc.ui8_box_offset = pt_stsc_box->ui8_offset;
                pt_curr_track_info->t_stsc.ui4_box_size = pt_stsc_box->ui4_size;
            }
            /* sample size box */
            _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STSZ, &pt_stsz_box);
            if (pt_stsz_box == NULL)
            {
                _delete_track_tree(pt_midx_obj);
                return MIDXBULDR_INTERNAL_ERR;
            }
            else
            {
                pf_set_pos(pt_midx_obj, (INT64)pt_stsz_box->ui8_offset + 12, FM_SEEK_BGN);
                pf_inp4byte(pt_midx_obj, &pt_curr_track_info->ui4_default_sample_size);
                if(pt_curr_track_info->ui4_default_sample_size == 0)
                {
                    /*Check whether this box is valid*/
                    pf_set_pos(pt_midx_obj, (INT64)pt_stsz_box->ui8_offset + 16, FM_SEEK_BGN);
                    pf_inp4byte(pt_midx_obj, &ui4_entry_num);
                    if(ui4_entry_num == 0)
                    {
                        _delete_track_tree(pt_midx_obj);
                        return MIDXBULDR_FMT_NOT_SUPPORT;
                    }
                }
                
                pt_curr_track_info->t_stsz.b_is_box_exist = TRUE;
                pt_curr_track_info->t_stsz.ui8_box_offset = pt_stsz_box->ui8_offset;
                pt_curr_track_info->t_stsz.ui4_box_size = pt_stsz_box->ui4_size;
            }
            /* chunk offset box */
            _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STCO, &pt_stco_box);
            if (pt_stco_box == NULL)
            {
                _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_CO64, &pt_stco_box);
                if(pt_stco_box == NULL)
                {
                    _delete_track_tree(pt_midx_obj);
                    return MIDXBULDR_INTERNAL_ERR;
                }
                else
                {
                    /*Check whether this box is valid*/
                    pf_set_pos(pt_midx_obj, (INT64)pt_stco_box->ui8_offset + 12, FM_SEEK_BGN);
                    pf_inp4byte(pt_midx_obj, &ui4_entry_num);
                    if(ui4_entry_num == 0)
                    {
                        _delete_track_tree(pt_midx_obj);
                        return MIDXBULDR_FMT_NOT_SUPPORT;
                    }
                    pt_curr_track_info->t_co64.b_is_box_exist = TRUE;
                    pt_curr_track_info->t_co64.ui8_box_offset = pt_stco_box->ui8_offset;
                    pt_curr_track_info->t_co64.ui4_box_size = pt_stco_box->ui4_size;
                }
            }
            else
            {
                /*Check whether this box is valid*/
                pf_set_pos(pt_midx_obj, (INT64)pt_stco_box->ui8_offset + 12, FM_SEEK_BGN);
                pf_inp4byte(pt_midx_obj, &ui4_entry_num);
                if(ui4_entry_num == 0)
                {
                    _delete_track_tree(pt_midx_obj);
                    return MIDXBULDR_FMT_NOT_SUPPORT;
                }
                pt_curr_track_info->t_stco.b_is_box_exist = TRUE;
                pt_curr_track_info->t_stco.ui8_box_offset = pt_stco_box->ui8_offset;
                pt_curr_track_info->t_stco.ui4_box_size = pt_stco_box->ui4_size;
            }
            /* sync sample table box */
            _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STSS, &pt_stss_box);
            if (pt_stss_box == NULL)
            {
                pt_curr_track_info->t_stss.b_is_box_exist = FALSE;
            }
            else
            {
                /*Check whether this box is valid*/
                pf_set_pos(pt_midx_obj, (INT64)pt_stss_box->ui8_offset + 12, FM_SEEK_BGN);
                pf_inp4byte(pt_midx_obj, &ui4_entry_num);
                if(ui4_entry_num == 0)
                {
#if 0
                    _delete_track_tree(pt_midx_obj);
                    return MIDXBULDR_FMT_NOT_SUPPORT;
#else
                    pt_curr_track_info->t_stss.b_is_box_exist = FALSE;
#endif
                }
                else
                {
                    pt_curr_track_info->t_stss.b_is_box_exist = TRUE;
                    pt_curr_track_info->t_stss.ui8_box_offset = pt_stss_box->ui8_offset;
                    pt_curr_track_info->t_stss.ui4_box_size = pt_stss_box->ui4_size;
                }
            }
            
            /* composition time to sample box */
            _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_CTTS, &pt_ctts_box);
            if (pt_ctts_box == NULL)
            {
                pt_curr_track_info->t_ctts.b_is_box_exist = FALSE;
            }
            else
            {
                /*Check whether this box is valid*/
                pf_set_pos(pt_midx_obj, (INT64)pt_ctts_box->ui8_offset + 12, FM_SEEK_BGN);
                pf_inp4byte(pt_midx_obj, &ui4_entry_num);
                if(ui4_entry_num == 0)
                {
                    _delete_track_tree(pt_midx_obj);
                    return MIDXBULDR_FMT_NOT_SUPPORT;
                }
                pt_curr_track_info->t_ctts.b_is_box_exist = TRUE;
                pt_curr_track_info->t_ctts.ui8_box_offset = pt_ctts_box->ui8_offset;
                pt_curr_track_info->t_ctts.ui4_box_size = pt_ctts_box->ui4_size;
            }
            pt_mp4_hdlr_obj->ui1_valid_track_cnt++;
        }
        
        pf_set_pos(pt_midx_obj, (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, FM_SEEK_BGN);
        _delete_track_tree(pt_midx_obj);
        pt_mp4_hdlr_obj->e_midx_parser_state = MP4_PARSER_STATE_IDLE;
        break;
       
    default:
        break;
    }
    
    return MIDX_MP4_PARSE_NEED_NEXT_LOOP;
}

#if (ENABLE_CMOV == 1)
/*-----------------------------------------------------------------------------
 * Name:  _mp4_midx_cmov_track_info
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MIDXBULDR_OK                 The API is successful. 
 *          MIDXBULDR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/                            
static INT32 _mp4_midx_cmov_track_info(MIDXBULD_OBJ_T*  pt_midx_obj )
{
    UINT8                   ui1_ver;
    UINT8                   ui1_track_idx;
    UINT8*                  pui1_cmov_data;
    UINT32                  ui4_track_id;
    UINT32                  ui4_box_size;
    UINT32                  ui4_fourcc;
    UINT32                  ui4_curr_offset;   
    MP4_BOX_T *             pt_tkhd_box;
    MP4_BOX_T *             pt_stts_box;
    MP4_BOX_T *             pt_stsc_box;
    MP4_BOX_T *             pt_stsz_box;
    MP4_BOX_T *             pt_stco_box;
    MP4_BOX_T *             pt_stss_box;
    MP4_BOX_T *             pt_ctts_box;
    MP4_BOX_ID_ENUM_T       e_id;
    MP4_IDXBULD_OBJ_T*      pt_mp4_hdlr_obj;
    MP4_IDXBULD_TRACK_T*    pt_curr_track_info;
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info;
    UINT32                  ui4_entry_num;
        
    /* check parameter */
    if (pt_midx_obj == NULL)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    pt_mp4_hdlr_obj = (MP4_IDXBULD_OBJ_T*)pt_midx_obj->pv_handler_obj;
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midx_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
    if(!pt_cmov_info->b_is_cmov_box_exist)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    pt_mp4_hdlr_obj->ui8_curr_parser_offset = 8;/* here to skip 8(size+'moov')*/
    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data + 8;
    
 while(1)
 {
    switch(pt_mp4_hdlr_obj->e_midx_parser_state)
    {
    case MP4_PARSER_STATE_IDLE:
        if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
                pt_cmov_info->ui4_cmov_box_decomp_size)
        {
            return MIDXBULDR_OK;
        }
        else
        {
            LOADB_DWRD(pui1_cmov_data, ui4_box_size);
            LOADL_DWRD(pui1_cmov_data + 4, ui4_fourcc);
        }
        e_id = _get_box_id(ui4_fourcc);
        if (e_id == MP4_BOX_ID_TRAK)
        {
            pt_mp4_hdlr_obj->e_midx_parser_state = MP4_PARSER_STATE_CHECK_TRACK;
            if (pt_mp4_hdlr_obj->pt_tree_box != NULL)
            {
                _delete_track_tree(pt_midx_obj);
            }
            _add_box_to_tree(pt_midx_obj, 
                             e_id, 
                             pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                             ui4_box_size);
            pui1_cmov_data += 8;
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
        }
        if (!_mp4_box_is_need_check_sub(e_id))
        {
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += ui4_box_size;
            pui1_cmov_data += ui4_box_size;
        }
        break;
        
    case MP4_PARSER_STATE_CHECK_TRACK:
        LOADB_DWRD(pui1_cmov_data, ui4_box_size);
        LOADL_DWRD(pui1_cmov_data + 4, ui4_fourcc);
        e_id = _get_box_id(ui4_fourcc);
        if(ui4_box_size == 0)
        {
            if(pt_mp4_hdlr_obj->ui8_curr_parser_offset 
                >= pt_cmov_info->ui4_cmov_box_decomp_size)
            {
                pt_mp4_hdlr_obj->e_midx_parser_state = MP4_PARSER_STATE_LOAD_TRACK_INF;
                continue;
            }
            return MINFOR_INTERNAL_ERR;
        }
        if (_box_is_need_parser(e_id) || _mp4_box_is_need_check_sub(e_id) )
        {
            _add_box_to_tree(pt_midx_obj, 
                             e_id, 
                             pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                             ui4_box_size);
        }
        pui1_cmov_data += 8;
        pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
        if (!_mp4_box_is_need_check_sub(e_id))
        {
            pui1_cmov_data += (ui4_box_size - 8);
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += (ui4_box_size - 8);
        }

        if(pt_mp4_hdlr_obj->pt_tree_box == NULL)
        {
            return MINFOR_INTERNAL_ERR;
        }
        if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
            (pt_mp4_hdlr_obj->pt_tree_box->ui8_offset + pt_mp4_hdlr_obj->pt_tree_box->ui4_size))
        {
            pt_mp4_hdlr_obj->e_midx_parser_state = MP4_PARSER_STATE_LOAD_TRACK_INF;
        }
        
        break;
        
    case MP4_PARSER_STATE_LOAD_TRACK_INF:
        /* track header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_TKHD, &pt_tkhd_box);
        if (pt_tkhd_box == NULL)
        {
            _delete_track_tree(pt_midx_obj);
            return MIDXBULDR_INTERNAL_ERR;
        }
        ui4_box_size = pt_tkhd_box->ui4_size;
        ui4_curr_offset = pt_tkhd_box->ui8_offset;
        pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data + ui4_curr_offset;
                
        LOAD_BYTE(pui1_cmov_data + 8, ui1_ver);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pui1_cmov_data + 28, ui4_track_id);
        }
        else
        {
            LOADB_DWRD(pui1_cmov_data + 20, ui4_track_id);
        }
        if (_b_is_track_in_src(pt_midx_obj, ui4_track_id))
        {
            ui1_track_idx = pt_mp4_hdlr_obj->ui1_valid_track_cnt;
            if(ui1_track_idx >= MIDXBULD_ELMT_TBL_MAX)
            {
                return MIDXBULDR_INTERNAL_ERR;/*-----TBD-----*/
            }
            pt_curr_track_info = &(pt_mp4_hdlr_obj->t_track_info_box[ui1_track_idx]);
            pt_curr_track_info->ui4_track_id = ui4_track_id;

            /* time to sample box */
            _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STTS, &pt_stts_box);
            if (pt_stts_box == NULL)
            {
                _delete_track_tree(pt_midx_obj);
                return MIDXBULDR_INTERNAL_ERR;
            }
            else
            {
                /*Check whether this box is valid*/
                pui1_cmov_data = (pt_cmov_info->pui1_decomp_cmov_data
                                    + pt_stts_box->ui8_offset + 12);
                LOADB_DWRD(pui1_cmov_data, ui4_entry_num);
                if(ui4_entry_num == 0)
                {
                    return MIDXBULDR_FMT_NOT_SUPPORT;
                }
                pt_curr_track_info->t_stts.b_is_box_exist = TRUE;
                pt_curr_track_info->t_stts.ui8_box_offset = pt_stts_box->ui8_offset;
                pt_curr_track_info->t_stts.ui4_box_size = pt_stts_box->ui4_size;
            }
            /* sample to chunk box */
            _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STSC, &pt_stsc_box);
            if (pt_stsc_box == NULL)
            {
                _delete_track_tree(pt_midx_obj);
                return MIDXBULDR_INTERNAL_ERR;
            }
            else
            {
                /*Check whether this box is valid*/
                pui1_cmov_data = (pt_cmov_info->pui1_decomp_cmov_data
                                    + pt_stsc_box->ui8_offset + 12);
                LOADB_DWRD(pui1_cmov_data, ui4_entry_num);
                if(ui4_entry_num == 0)
                {
                    _delete_track_tree(pt_midx_obj);
                    return MIDXBULDR_FMT_NOT_SUPPORT;
                }
                pt_curr_track_info->t_stsc.b_is_box_exist = TRUE;
                pt_curr_track_info->t_stsc.ui8_box_offset = pt_stsc_box->ui8_offset;
                pt_curr_track_info->t_stsc.ui4_box_size = pt_stsc_box->ui4_size;
            }
            /* sample size box */
            _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STSZ, &pt_stsz_box);
            if (pt_stsz_box == NULL)
            {
                _delete_track_tree(pt_midx_obj);
                return MIDXBULDR_INTERNAL_ERR;
            }
            else
            {
                pui1_cmov_data = (pt_cmov_info->pui1_decomp_cmov_data
                                    + pt_stsz_box->ui8_offset + 12);
                LOADB_DWRD(pui1_cmov_data, pt_curr_track_info->ui4_default_sample_size);
                if(pt_curr_track_info->ui4_default_sample_size == 0)
                {
                    /*Check whether this box is valid*/
                    pui1_cmov_data = (pt_cmov_info->pui1_decomp_cmov_data
                                        + pt_stsz_box->ui8_offset + 16);
                    LOADB_DWRD(pui1_cmov_data, ui4_entry_num);
                    if(ui4_entry_num == 0)
                    {
                        _delete_track_tree(pt_midx_obj);
                        return MIDXBULDR_FMT_NOT_SUPPORT;
                    }
                }
                pt_curr_track_info->t_stsz.b_is_box_exist = TRUE;
                pt_curr_track_info->t_stsz.ui8_box_offset = pt_stsz_box->ui8_offset;
                pt_curr_track_info->t_stsz.ui4_box_size = pt_stsz_box->ui4_size;
            }
            /* chunk offset box */
            _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STCO, &pt_stco_box);
            if (pt_stco_box == NULL)
            {
                _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_CO64, &pt_stco_box);
                if(pt_stco_box == NULL)
                {
                    _delete_track_tree(pt_midx_obj);
                    return MIDXBULDR_INTERNAL_ERR;
                }
                else
                {
                    /*Check whether this box is valid*/
                    pui1_cmov_data = (pt_cmov_info->pui1_decomp_cmov_data
                                        + pt_stco_box->ui8_offset + 12);
                    LOADB_DWRD(pui1_cmov_data, ui4_entry_num);
                    if(ui4_entry_num == 0)
                    {
                        _delete_track_tree(pt_midx_obj);
                        return MIDXBULDR_FMT_NOT_SUPPORT;
                    }
                    pt_curr_track_info->t_co64.b_is_box_exist = TRUE;
                    pt_curr_track_info->t_co64.ui8_box_offset = pt_stco_box->ui8_offset;
                    pt_curr_track_info->t_co64.ui4_box_size = pt_stco_box->ui4_size;
                }
            }
            else
            {
                /*Check whether this box is valid*/
                pui1_cmov_data = (pt_cmov_info->pui1_decomp_cmov_data
                                    + pt_stco_box->ui8_offset + 12);
                LOADB_DWRD(pui1_cmov_data, ui4_entry_num);
                if(ui4_entry_num == 0)
                {
                    _delete_track_tree(pt_midx_obj);
                    return MIDXBULDR_FMT_NOT_SUPPORT;
                }
                pt_curr_track_info->t_stco.b_is_box_exist = TRUE;
                pt_curr_track_info->t_stco.ui8_box_offset = pt_stco_box->ui8_offset;
                pt_curr_track_info->t_stco.ui4_box_size = pt_stco_box->ui4_size;
            }
            /* sync sample table box */
            _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STSS, &pt_stss_box);
            if (pt_stss_box == NULL)
            {
                pt_curr_track_info->t_stss.b_is_box_exist = FALSE;
            }
            else
            {
                /*Check whether this box is valid*/
                pui1_cmov_data = (pt_cmov_info->pui1_decomp_cmov_data
                                    + pt_stss_box->ui8_offset + 12);
                LOADB_DWRD(pui1_cmov_data, ui4_entry_num);
                if(ui4_entry_num == 0)
                {
                    _delete_track_tree(pt_midx_obj);
                    return MIDXBULDR_FMT_NOT_SUPPORT;
                }
                pt_curr_track_info->t_stss.b_is_box_exist = TRUE;
                pt_curr_track_info->t_stss.ui8_box_offset = pt_stss_box->ui8_offset;
                pt_curr_track_info->t_stss.ui4_box_size = pt_stss_box->ui4_size;
            }
            
            /* composition time to sample box */
            _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_CTTS, &pt_ctts_box);
            if (pt_ctts_box == NULL)
            {
                pt_curr_track_info->t_ctts.b_is_box_exist = FALSE;
            }
            else
            {
                /*Check whether this box is valid*/
                pui1_cmov_data = (pt_cmov_info->pui1_decomp_cmov_data
                                    + pt_ctts_box->ui8_offset + 12);
                LOADB_DWRD(pui1_cmov_data, ui4_entry_num);
                if(ui4_entry_num == 0)
                {
                    _delete_track_tree(pt_midx_obj);
                    return MIDXBULDR_FMT_NOT_SUPPORT;
                }
                pt_curr_track_info->t_ctts.b_is_box_exist = TRUE;
                pt_curr_track_info->t_ctts.ui8_box_offset = pt_ctts_box->ui8_offset;
                pt_curr_track_info->t_ctts.ui4_box_size = pt_ctts_box->ui4_size;
            }
            pt_mp4_hdlr_obj->ui1_valid_track_cnt++;
        }
        
        pui1_cmov_data = (pt_cmov_info->pui1_decomp_cmov_data
                          + pt_mp4_hdlr_obj->ui8_curr_parser_offset);
        _delete_track_tree(pt_midx_obj);
        pt_mp4_hdlr_obj->e_midx_parser_state = MP4_PARSER_STATE_IDLE;
        break;
       
    default:
        break;
    }
 }
}
#endif

static INT32 _mp4_minfo_get_mln_moov_info(MIDXBULD_OBJ_T*  pt_midx_obj)
{
    UINT8                       ui1_i                   = 0;
    UINT32                      ui4_tmp                 = 0;
    MP4_IDXBULD_OBJ_T*          pt_mp4_hdlr_obj         = NULL;
    setpos_fct                  pf_set_pos              = NULL; 
    input4bytes_fct             pf_inp4byte             = NULL;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midx_obj == NULL)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    if ((pt_midx_obj->t_input_fct_tbl.pf_input4 == NULL)
        || (pt_midx_obj->t_input_fct_tbl.pf_set_pos == NULL))
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    /*pf_inp4byte = pt_midx_obj->t_input_fct_tbl.pf_input4;*/
    pf_set_pos = pt_midx_obj->t_input_fct_tbl.pf_set_pos;
    pf_inp4byte = pt_midx_obj->t_input_fct_tbl.pf_input4;
    pt_mp4_hdlr_obj = (MP4_IDXBULD_OBJ_T*)pt_midx_obj->pv_handler_obj;

    if (pt_mp4_hdlr_obj == NULL)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_mp4_hdlr_obj->t_mln_moov_info.b_is_moov_empty = TRUE;
    
    for(ui1_i=0; ui1_i < pt_mp4_hdlr_obj->ui1_valid_track_cnt; ui1_i++)
    {/* only check one video track, assume it has stss box */
        if(pt_mp4_hdlr_obj->t_track_info_box[ui1_i].t_stss.b_is_box_exist)
        {/* Get key cnd*/
            pf_set_pos(pt_midx_obj, 
                       (INT64)pt_mp4_hdlr_obj->t_track_info_box[ui1_i].t_stss.ui8_box_offset + 12, 
                       FM_SEEK_BGN);
            pf_inp4byte(pt_midx_obj, &pt_mp4_hdlr_obj->t_mln_moov_info.ui4_mln_moov_key_cnt);

            /* Get sample cnt*/
            pf_set_pos(pt_midx_obj, 
                       (INT64)pt_mp4_hdlr_obj->t_track_info_box[ui1_i].t_stsz.ui8_box_offset + 16, 
                       FM_SEEK_BGN);
            pf_inp4byte(pt_midx_obj, &pt_mp4_hdlr_obj->t_mln_moov_info.ui4_mln_moov_sample_cnt);

            pf_set_pos(pt_midx_obj, 
                       (INT64)(pt_midx_obj->t_src_info.ui8_idx_offset + 12), 
                       FM_SEEK_BGN);
            pf_inp4byte(pt_midx_obj, &ui4_tmp);
            if(ui4_tmp == 0x6D766864)/* check whether mvhd box first*/
            {
                UINT32  ui4_timescale = 0;
                UINT32  ui4_duration  = 0;
                pf_set_pos(pt_midx_obj, 
                           (INT64)(pt_midx_obj->t_src_info.ui8_idx_offset + 28), 
                           FM_SEEK_BGN);
                pf_inp4byte(pt_midx_obj, &ui4_timescale);
                pf_inp4byte(pt_midx_obj, &ui4_duration);
                pt_mp4_hdlr_obj->t_mln_moov_info.ui4_mln_moov_dur = 
                        _mm_div64((UINT64)ui4_duration * MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                                  ui4_timescale, 
                                  NULL);
            }
            
            pt_mp4_hdlr_obj->t_mln_moov_info.b_is_moov_empty = FALSE;
        }
    }
    return MIDXBULDR_OK;  
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_midx_box_parse
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MIDXBULDR_OK                 The API is successful. 
 *          MIDXBULDR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/                            
INT32 mp4_midx_box_parse (MIDXBULD_OBJ_T*    pt_midx_obj)
{
    MP4_IDXBULD_OBJ_T*          pt_mp4_hdlr_obj         = NULL;
#if (ENABLE_CMOV == 1)
    MP4_BOX_CMOV_INFO_T*        pt_cmov_info            = NULL;
#endif
    PIFF_BOX_INET_INFO_T*       pt_moof_info            = NULL;
    setpos_fct                  pf_set_pos; 
    INT32                       i4_ret                  = MIDXBULDR_INTERNAL_ERR;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midx_obj == NULL)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    if ((pt_midx_obj->t_input_fct_tbl.pf_input4 == NULL)
        || (pt_midx_obj->t_input_fct_tbl.pf_set_pos == NULL))
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    /*pf_inp4byte = pt_midx_obj->t_input_fct_tbl.pf_input4;*/
    pf_set_pos = pt_midx_obj->t_input_fct_tbl.pf_set_pos;
    pt_mp4_hdlr_obj = (MP4_IDXBULD_OBJ_T*)pt_midx_obj->pv_handler_obj;   
    pt_moof_info    = (PIFF_BOX_INET_INFO_T*)(pt_midx_obj->t_src_info.t_mp4_extra_info.pt_piff_inet_info);

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/

    if(!pt_moof_info->b_is_piff_file || pt_moof_info->b_is_bivl_file)
    {       
        /* seek to moov container first sub box */
        pt_mp4_hdlr_obj->ui8_curr_parser_offset = pt_midx_obj->t_src_info.ui8_idx_offset + 8;
        pf_set_pos(pt_midx_obj, 
                   (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                   FM_SEEK_BGN);
     
        pt_mp4_hdlr_obj->e_midx_parser_state = MP4_PARSER_STATE_IDLE;
        pt_mp4_hdlr_obj->ui1_valid_track_cnt = 0;
#if (ENABLE_CMOV == 1)
        pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midx_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
        if(pt_cmov_info->b_is_cmov_box_exist)
        {
            i4_ret = _mp4_midx_cmov_track_info(pt_midx_obj);
        }
        else
#endif
        {
            do
            {
                i4_ret = _mp4_midx_moov_track_info(pt_midx_obj);
            }while (i4_ret == MIDX_MP4_PARSE_NEED_NEXT_LOOP);
        }

        if (i4_ret != MIDXBULDR_OK)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }

        if(pt_moof_info->b_is_bivl_file)
        {
            _mp4_minfo_get_mln_moov_info(pt_midx_obj);
        }
    }
    return MIDXBULDR_OK;  
}
