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
 * aif_parser.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * BingsaiJin
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the aif parser function
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

#include "inc/u_common.h"
#include "inc/x_mm_common.h"
#include "handle/u_handle.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/x_minfo.h"
#include "mutil/mm_util.h"
#include "dbg/u_dbg.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "unicode/x_uc_str.h"
#include "aif_parser.h"

#else

#include "x_os.h"

#include "u_common.h"
#include "x_mm_common.h"
#include "u_handle.h"
#include "../minfo.h"
#include "../x_minfo.h"
#include "../../mm_util.h"
#include "u_dbg.h"
#include "x_uc_str.h"
#include "aif_parser.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define INTEL_CPU
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
UINT8    _aui1_buf[1024];

/*-----------------------------------------------------------------------------
                     functions declarations
----------------------------------------------------------------------------*/
#ifdef INTEL_CPU
/* *************************** FlipLong() ******************************
* Converts a long in "Big Endian" format (ie, Motorola 68000) to Intel
* reverse-byte format, or vice versa if originally in Big Endian.
********************************************************************* */
static void _FlipLong(UINT8 * pui1_ptr)
{
    UINT8 ui1_val;
    
    /* Swap 1st and 4th bytes */
    ui1_val = *(pui1_ptr);
    *(pui1_ptr) = *(pui1_ptr+3);
    *(pui1_ptr+3) = ui1_val;
    
    /* Swap 2nd and 3rd bytes */
    pui1_ptr += 1;
    ui1_val = *(pui1_ptr);
    *(pui1_ptr) = *(pui1_ptr+1);
    *(pui1_ptr+1) = ui1_val;
}
#endif
    
/* ************************* FetchLong() *******************************
* Fools the compiler into fetching a long from a char array.
********************************************************************* */
static UINT32 _FetchLong(UINT32 * pui4_ptr)
{
    return(*pui4_ptr);
}
    
/* ************************* ConvertFloat() *****************************
* Converts an 80 bit IEEE Standard 754 floating point number to an unsigned
* long.
********************************************************************** */
static UINT32 _ConvertFloat(UINT8 * pui1_buffer)
{
    UINT32 ui4_mantissa;
    UINT32 ui4_last = 0;
    UINT8  ui1_exp;
    
#ifdef INTEL_CPU
    _FlipLong(pui1_buffer+2);
#endif
    
    ui4_mantissa = _FetchLong((UINT32 *)(pui1_buffer+2));
    ui1_exp = (UINT8)(30 - *(pui1_buffer+1));
    while (ui1_exp--)
    {
        ui4_last = ui4_mantissa;
        ui4_mantissa >>= 1;
    }
    if (ui4_last & 0x00000001) ui4_mantissa++;
    return(ui4_mantissa);
}
#if 0
/* ************************* StoreLong() ******************************
* Fools the compiler into storing a long into a char array.
******************************************************************** */
static void _StoreLong(UINT32 ui4_val, UINT32 * pui4_ptr)
{
    *pui4_ptr = ui4_val;
}
    
/* ************************** StoreFloat() ******************************
* Converts an unsigned long to 80 bit IEEE Standard 754 floating point
* number.
********************************************************************** */
static void _StoreFloat(UINT8 * pui1_buffer, UINT32 ui4_value)
{
    UINT32 ui4_exp;
    UINT8  ui1_i;
    
    x_memset(pui1_buffer, 0, 10);
    
    ui4_exp = ui4_value;
    ui4_exp >>= 1;
    for (ui1_i=0; ui1_i<32; ui1_i++) 
    { 
        ui4_exp>>= 1;
           if (!ui4_exp) 
        { 
            break;
        }
    }
    *(pui1_buffer+1) = ui1_i;
    
    for (ui1_i=32; ui1_i; ui1_i--)
    {
           if (ui4_value & 0x80000000) 
           {
               break;
           }
           ui4_value <<= 1; 
    } 
    _StoreLong(ui4_value, (UINT32*)(pui1_buffer+2)); 
#ifdef INTEL_CPU 
    _FlipLong(pui1_buffer+2); 
#endif 
} 
#endif
static INT32 _buff_load_data(MINFO_OBJ_T* pt_minfo_obj,
                                UINT8*         pui1_buff,
                                UINT32         ui4_size)
{
    INT32                   i4_ret             = 0;
    UINT32                  ui4_read_size     = 0;
    setpos_fct              pf_set_pos;  
    copybytes_fct           pf_copybyte;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    pf_set_pos         = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_copybyte     = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    
    x_memset(pui1_buff, 0, ui4_size);
    pf_set_pos(pt_minfo_obj, 
               (INT64)pt_aif_info_obj->ui4_curr_parser_offset, 
               FM_SEEK_BGN);
    i4_ret = pf_copybyte(pt_minfo_obj, 
                         pui1_buff, 
                         (SIZE_T)pt_minfo_obj->z_buff_leng, 
                         ui4_size, 
                         &ui4_read_size);
    if ((i4_ret != INPUTR_OK)
        || (ui4_read_size < ui4_size))
    {
        return MINFOR_INTERNAL_ERR;
    }
    return MINFOR_OK;
}

static INT32 _get_file_header(MINFO_OBJ_T*    pt_minfo_obj)
{
    AIF_FILE_HEADER_T*         pt_file_head     = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_file_head     = &pt_aif_info_obj->r_file_layout.r_file_head;
    
    if(MINFOR_OK !=
        _buff_load_data(pt_minfo_obj, _aui1_buf, sizeof(AIF_FILE_HEADER_T)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_file_head->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_file_head->ui4_ck_size);
    LOADB_DWRD(_aui1_buf + 8,    pt_file_head->ui4_type);

    pt_aif_info_obj->ui4_curr_parser_offset += sizeof(AIF_FILE_HEADER_T);
    return MINFOR_OK;
}

static INT32 _get_comm_ck(MINFO_OBJ_T*    pt_minfo_obj)
{ 
    AIF_COMM_CK_T*          pt_comm_ck         = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_comm_ck         = &pt_aif_info_obj->r_file_layout.r_comm_ck;

    if(MINFOR_OK !=
        _buff_load_data(pt_minfo_obj, _aui1_buf, sizeof(AIF_COMM_CK_T)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_comm_ck->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_comm_ck->ui4_ck_size);
    LOADB_WORD(_aui1_buf + 8,    pt_comm_ck->ui2_num_channels);
    LOADB_DWRD(_aui1_buf + 10,   pt_comm_ck->ui4_num_sample_frames);
    LOADB_WORD(_aui1_buf + 14,   pt_comm_ck->ui2_sample_size);
    pt_comm_ck->ui4_sample_rate = _ConvertFloat(_aui1_buf + 16);
    pt_comm_ck->ui4_sample_rate_scale = 1;
    if(pt_aif_info_obj->r_file_layout.r_file_head.ui4_type == AIF_CK_AIFC)
    {
        LOADB_DWRD(_aui1_buf + 26,   pt_comm_ck->ui4_comp_type);
        pt_comm_ck->pui1_comp_name = NULL;
    }

    pt_aif_info_obj->ui4_curr_parser_offset += pt_comm_ck->ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _get_ssnd_ck(MINFO_OBJ_T*    pt_minfo_obj)
{
    AIF_SSND_CK_T*             pt_ssnd_ck         = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_ssnd_ck         = &pt_aif_info_obj->r_file_layout.r_ssnd_ck;

    if(MINFOR_OK != _buff_load_data(pt_minfo_obj, _aui1_buf, 
                                    sizeof(AIF_SSND_CK_T)-sizeof(UINT32*)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_ssnd_ck->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_ssnd_ck->ui4_ck_size);
    LOADB_DWRD(_aui1_buf + 8,    pt_ssnd_ck->ui4_offset);
    LOADB_DWRD(_aui1_buf + 12,   pt_ssnd_ck->ui4_block_size);
    pt_ssnd_ck->pui4_waveform_data = NULL;
    /*change to absolute position offset*/
    pt_ssnd_ck->ui4_offset += pt_aif_info_obj->ui4_curr_parser_offset + 16;
    
    pt_aif_info_obj->ui4_curr_parser_offset += pt_ssnd_ck->ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _get_mark_ck(MINFO_OBJ_T*    pt_minfo_obj)
{   
    AIF_MARK_CK_T*             pt_mark_ck         = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_mark_ck         = &pt_aif_info_obj->r_file_layout.r_mark_ck;

    if(MINFOR_OK != _buff_load_data(pt_minfo_obj, _aui1_buf, 
                                    sizeof(AIF_SSND_CK_T)-sizeof(AIF_MARK_T*)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_mark_ck->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_mark_ck->ui4_ck_size);
    LOADB_WORD(_aui1_buf + 8,    pt_mark_ck->ui2_num_markers);
    pt_mark_ck->pt_markers = NULL;
    
    pt_aif_info_obj->ui4_curr_parser_offset += pt_mark_ck->ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _get_midi_ck(MINFO_OBJ_T*    pt_minfo_obj)
{
    AIF_MIDI_CK_T*             pt_midi_ck         = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_midi_ck         = &pt_aif_info_obj->r_file_layout.r_midi_ck;

    if(MINFOR_OK != _buff_load_data(pt_minfo_obj, _aui1_buf, 
                                    sizeof(AIF_MIDI_CK_T)-sizeof(UINT8*)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_midi_ck->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_midi_ck->ui4_ck_size);
    pt_midi_ck->pui1_midi_data = NULL;
    /*To do*/
    
    pt_aif_info_obj->ui4_curr_parser_offset += pt_midi_ck->ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _get_inst_ck(MINFO_OBJ_T*    pt_minfo_obj)
{
    AIF_INST_CK_T*             pt_inst_ck         = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_inst_ck         = &pt_aif_info_obj->r_file_layout.r_inst_ck;

    if(MINFOR_OK != 
        _buff_load_data(pt_minfo_obj, _aui1_buf, sizeof(AIF_INST_CK_T)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_inst_ck->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_inst_ck->ui4_ck_size);
    /*To do*/
    
    pt_aif_info_obj->ui4_curr_parser_offset += pt_inst_ck->ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _get_aesd_ck(MINFO_OBJ_T*    pt_minfo_obj)
{
    AIF_AESD_CK_T*             pt_aesd_ck         = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_aesd_ck         = &pt_aif_info_obj->r_file_layout.r_aesd_ck;

    if(MINFOR_OK != _buff_load_data(pt_minfo_obj, _aui1_buf, 
                                    sizeof(AIF_AESD_CK_T)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_aesd_ck->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_aesd_ck->ui4_ck_size);
    /*To do*/
    
    pt_aif_info_obj->ui4_curr_parser_offset += pt_aesd_ck->ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _get_appl_ck(MINFO_OBJ_T*    pt_minfo_obj)
{
    AIF_APPL_CK_T*             pt_appl_ck         = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_appl_ck         = &pt_aif_info_obj->r_file_layout.r_appl_ck;

    if(MINFOR_OK != _buff_load_data(pt_minfo_obj, _aui1_buf, 
                                    sizeof(AIF_APPL_CK_T)-sizeof(UINT8*)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_appl_ck->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_appl_ck->ui4_ck_size);
    pt_appl_ck->pui1_data = NULL;
    /*To do*/
    
    pt_aif_info_obj->ui4_curr_parser_offset += pt_appl_ck->ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _get_comt_ck(MINFO_OBJ_T*    pt_minfo_obj)
{
    AIF_COMT_CK_T*             pt_comt_ck         = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_comt_ck         = &pt_aif_info_obj->r_file_layout.r_comt_ck;

    if(MINFOR_OK != _buff_load_data(pt_minfo_obj, _aui1_buf, 
                                    sizeof(AIF_COMT_CK_T)-sizeof(UINT8*)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_comt_ck->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_comt_ck->ui4_ck_size);
    pt_comt_ck->pt_comments = NULL;
    /*To do*/
    
    pt_aif_info_obj->ui4_curr_parser_offset += pt_comt_ck->ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _get_name_ck(MINFO_OBJ_T*    pt_minfo_obj)
{
    AIF_TEXT_CK_T*             pt_name_ck         = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_name_ck         = &pt_aif_info_obj->r_file_layout.r_name_ck;

    if(MINFOR_OK != _buff_load_data(pt_minfo_obj, _aui1_buf, 
                                    sizeof(AIF_TEXT_CK_T)-sizeof(UINT8*)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_name_ck->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_name_ck->ui4_ck_size);
    pt_name_ck->pui1_text = NULL;
    /*To do*/
    
    pt_aif_info_obj->ui4_curr_parser_offset += pt_name_ck->ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _get_auth_ck(MINFO_OBJ_T*    pt_minfo_obj)
{
    AIF_TEXT_CK_T*             pt_auth_ck         = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_auth_ck         = &pt_aif_info_obj->r_file_layout.r_auth_ck;

    if(MINFOR_OK != _buff_load_data(pt_minfo_obj, _aui1_buf, 
                                    sizeof(AIF_TEXT_CK_T)-sizeof(UINT8*)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_auth_ck->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_auth_ck->ui4_ck_size);
    pt_auth_ck->pui1_text = NULL;
    /*To do*/
    
    pt_aif_info_obj->ui4_curr_parser_offset += pt_auth_ck->ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _get_anno_ck(MINFO_OBJ_T*    pt_minfo_obj)
{
    AIF_TEXT_CK_T*             pt_anno_ck         = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_anno_ck         = &pt_aif_info_obj->r_file_layout.r_anno_ck;

    if(MINFOR_OK != _buff_load_data(pt_minfo_obj, _aui1_buf, 
                                    sizeof(AIF_TEXT_CK_T)-sizeof(UINT8*)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_anno_ck->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_anno_ck->ui4_ck_size);
    pt_anno_ck->pui1_text = NULL;
    /*To do*/
    
    pt_aif_info_obj->ui4_curr_parser_offset += pt_anno_ck->ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _get_copyright_ck(MINFO_OBJ_T*    pt_minfo_obj)
{
    AIF_TEXT_CK_T*             pt_copyright_ck = NULL;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;
    
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_copyright_ck = &pt_aif_info_obj->r_file_layout.r_copyright_ck;

    if(MINFOR_OK != _buff_load_data(pt_minfo_obj, _aui1_buf, 
                                    sizeof(AIF_TEXT_CK_T)-sizeof(UINT8*)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        pt_copyright_ck->ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    pt_copyright_ck->ui4_ck_size);
    pt_copyright_ck->pui1_text = NULL;
    /*To do*/
    
    pt_aif_info_obj->ui4_curr_parser_offset += pt_copyright_ck->ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _get_def_iff_ck(MINFO_OBJ_T*    pt_minfo_obj)
{
    AIF_IFF_CK_T             r_iff_ck;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj = NULL;

    x_memset(&r_iff_ck, 0, sizeof(AIF_IFF_CK_T));
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 

    if(MINFOR_OK != _buff_load_data(pt_minfo_obj, _aui1_buf, 
                                    sizeof(AIF_IFF_CK_T)))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_DWRD(_aui1_buf,        r_iff_ck.ui4_ck_id);
    LOADB_DWRD(_aui1_buf + 4,    r_iff_ck.ui4_ck_size);
    
    pt_aif_info_obj->ui4_curr_parser_offset += r_iff_ck.ui4_ck_size + 8;
    return MINFOR_OK;
}

static INT32 _aif_minfo_ck_parse(MINFO_OBJ_T*    pt_minfo_obj)
{
    UINT32                     ui4_ck_id = 0;
    INT32                      i4_ret      = 0;
    INT32                      i4_state  = MINFOR_OK;
    setpos_fct              pf_set_pos;  
    input4bytes_fct         pf_inp4byte;
    MINFO_AIF_FILE_INF_T*   pt_aif_info_obj;
    
    pf_set_pos      = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_inp4byte     = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pt_aif_info_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    
    while(1)
    {
        i4_ret = pf_set_pos(pt_minfo_obj, 
                    (INT64)pt_aif_info_obj->ui4_curr_parser_offset, 
                    FM_SEEK_BGN);
        i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_ck_id);
        if(i4_ret != INPUTR_OK)
        {
            if(i4_ret == INPUTR_EOF)
            {
                return MINFOR_OK;
            }
            return MINFOR_INTERNAL_ERR;
        }
        if((UINT64)pt_aif_info_obj->ui4_curr_parser_offset >=
            pt_aif_info_obj->r_generic_info.ui8_media_sz)
        {
            return MINFOR_OK;
        }
        switch(ui4_ck_id)
        { 
        case AIF_CK_FORM:
            i4_state = _get_file_header(pt_minfo_obj);
            break;
        case AIF_CK_COMM:
            i4_state = _get_comm_ck(pt_minfo_obj);
            break;
        case AIF_CK_SSND:
            i4_state = _get_ssnd_ck(pt_minfo_obj);
            break;
        case AIF_CK_MARK:
            i4_state = _get_mark_ck(pt_minfo_obj);
            break;
        case AIF_CK_MIDI:
            i4_state = _get_midi_ck(pt_minfo_obj);
            break;
        case AIF_CK_INST:
            i4_state = _get_inst_ck(pt_minfo_obj);
            break;
        case AIF_CK_AESD:
            i4_state = _get_aesd_ck(pt_minfo_obj);
            break;
        case AIF_CK_APPL:
            i4_state = _get_appl_ck(pt_minfo_obj);
            break;
        case AIF_CK_COMT:
            i4_state = _get_comt_ck(pt_minfo_obj);
            break;
        case AIF_CK_NAME:
            i4_state = _get_name_ck(pt_minfo_obj);
            break;
        case AIF_CK_AUTH:
            i4_state = _get_auth_ck(pt_minfo_obj);
            break;
        case AIF_CK_ANNO:
            i4_state = _get_anno_ck(pt_minfo_obj);
            break;
        case AIF_CK_CPRT:
            i4_state = _get_copyright_ck(pt_minfo_obj);
            break;
        default:
            i4_state = _get_def_iff_ck(pt_minfo_obj);
            break;
        }/* end switch */
        if(i4_state != MINFOR_OK)
        {
            break;
        }
    }/* end while(1) */
    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _aif_minfo_box_parse
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/                            
INT32 aif_file_minfo_parse (MINFO_OBJ_T*    pt_minfo_obj)
{
    setpos_fct              pf_set_pos;
    getpos_fct              pf_get_pos;   
    MINFO_AIF_FILE_INF_T*   pt_aif_hdlr_obj;
    INT32                   i4_ret;
        

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        DBG_ERROR(("ERROR: aif_file_minfo_parse>pt_minfo_obj invalid\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    if ((pt_minfo_obj->t_input_fct_tbl.pf_input4 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_copybytes == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_get_pos == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL))
    {
        DBG_ERROR(("ERROR: aif_file_minfo_parse>t_input_fct_tbl invalid\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    pt_aif_hdlr_obj = (MINFO_AIF_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 

    i4_ret = MINFOR_INTERNAL_ERR;

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*seek to beginning*/
    i4_ret = pf_set_pos(pt_minfo_obj, 0, FM_SEEK_BGN);
    /* store generic info */
    i4_ret = pf_set_pos(pt_minfo_obj , 0, FM_SEEK_END);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("ERROR: seek to file end fail. \r\n"));
        return i4_ret;
    }
    pf_get_pos(pt_minfo_obj, &(pt_aif_hdlr_obj->r_generic_info.ui8_media_sz));    

    pt_aif_hdlr_obj->ui4_curr_parser_offset = 0;
    i4_ret = _aif_minfo_ck_parse(pt_minfo_obj);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("ERROR:_aif_minfo_box_parse fail.\r\n"));
        return i4_ret;
    }

    pt_aif_hdlr_obj->r_generic_info.ui1_strm_ns = (UINT8)
                pt_aif_hdlr_obj->r_file_layout.r_comm_ck.ui2_num_channels;
    return MINFOR_OK;  
}
