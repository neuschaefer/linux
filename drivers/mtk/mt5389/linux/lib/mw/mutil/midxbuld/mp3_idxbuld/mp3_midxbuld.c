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
 * $RCSfile: mp3_midxbuld.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description:
 *         This file contains all the media index builder public APIs
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

#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/midxbuld/u_midxbuld.h"
#include "mutil/midxbuld/midxbuld.h"
#include "mutil/midxbuld/mp3_idxbuld/mp3_midxbuld.h"
#include "mutil/mm_util_input.h"
#include "dbg/x_dbg.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "util/x_lnk_list.h"
#include "inc/x_mm_common.h"

#include "mutil/mm_util.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_midxbuld.h"
#include "../midxbuld.h"
#include "mp3_midxbuld.h"
#include "../../mm_util_input.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "../../mm_util.h"
#include "x_mm_common.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef  LIMIT_MP3_BUFFER
#define MP3_MIDXBULD_MAX_BUFF_LEN                          (UINT32)(200 * 1024)
#else
#define MP3_MIDXBULD_MAX_BUFF_LEN                          (UINT32)(512 * 1024)
#endif
#define MP3_MIDXBULD_MAX_ENTRY_NUM                               (UINT16)(1000)
#define MP3_MIDXBULD_MAX_CBR_PARSING_NUM                           (UINT8)(100)

#define MP3_MIDXBULD_MAX_BITRATE_IDX                                (UINT8)(14)

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/* VERSION_1 */
static UINT16 const MPEG_V1_BITRATE_TAB[3][MP3_MIDXBULD_MAX_BITRATE_IDX + 1] =
{
    {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448},     /* Layer I */
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384},        /* Layer II */
    {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320}, /* Layer III */
};

/* VERSION_2, VERSION_2_5 */
static UINT16 const MPEG_V2_BITRATE_TAB[2][MP3_MIDXBULD_MAX_BITRATE_IDX + 1] =
{
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256},        /* Layer I */
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160}      /* Layer II & III */
};

static UINT16 const MPEG_SAMPLERATE_TAB[3] = {44100, 48000, 32000};

/*-----------------------------------------------------------------------------
 * Name:  _mp3_midxbuld_parse_frame_attr
 *
 * Description: This function open a handler for a file
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object
 *
 * Outputs: pv_obj       modify data in the media info object
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mp3_midxbuld_parse_frame_attr(
    UINT8                   ui1_first,
    UINT8                   ui1_second,
    UINT8                   ui1_third,
    MP3_IDXBULD_OBJ_T*      pt_mp3_handler
)
{
    UINT8                   ui1_version;
    UINT8                   ui1_layer;
    UINT8                   ui1_rate_idx;
    UINT32                  ui4_sample_rate = 0;
    UINT8       ui1_slot_bytes;
    UINT32      ui4_frame_slots;
    UINT64                  ui8_frame_duration;

    /* check parameter */
    if (pt_mp3_handler == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }

    /* check audiio frame header is valid or NOT */
    if ((ui1_first != 0xFF)
        || ((ui1_second & 0xE0) != 0xE0)
        || ((ui1_second & 0x18) >> 3 == 1)      /* version description is set to reserved */
        || ((ui1_second & 0x06) >> 1 == 0)      /* layer description is set to reserved */
        || ((ui1_third >> 4) == 0)
        || ((ui1_third >> 4) == 15))
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    else
    {
        ui1_version = (UINT8)(3 - ((ui1_second & 0x18) >> 3));
        ui1_layer = (UINT8)(3 - ((ui1_second & 0x6) >> 1));

    /* sampling frequency index */
        ui1_rate_idx = ((ui1_third & 0xc) >> 2);

        if (ui1_rate_idx == 3)    /* reserved => return error! */
    {
            return MIDXBULDR_INTERNAL_ERR;
        }
        else
        {
            ui4_sample_rate = MPEG_SAMPLERATE_TAB[ui1_rate_idx];
        }
        switch (ui1_version)
        {
            case MP3_IDXBULD_VERSION_2:
                ui4_sample_rate >>= 1;
                break;
            case MP3_IDXBULD_VERSION_2_5:
                ui4_sample_rate >>= 2;
                break;
        }

        /* frame slot, slot bytes, frame duration */
            ui1_slot_bytes = 1;
        if (ui1_layer == MP3_IDXBULD_LAYER_I)
            {
                ui4_frame_slots = 12;
                ui1_slot_bytes = 4;
            ui8_frame_duration = _mm_div64(384 * (1000 * 1000), ui4_sample_rate, NULL);
            }
        else if ((ui1_layer == MP3_IDXBULD_LAYER_III)
                 && (ui1_version == MP3_IDXBULD_VERSION_2 || ui1_version == MP3_IDXBULD_VERSION_2_5))
            {
                ui4_frame_slots = 72;
            ui8_frame_duration = _mm_div64(576 * (1000 * 1000), ui4_sample_rate, NULL);
            }
            else
            {
                ui4_frame_slots = 144;
            ui8_frame_duration = _mm_div64(1152 * (1000 * 1000), ui4_sample_rate, NULL);
            }
    }

    /* fill data */
    pt_mp3_handler->ui1_version = ui1_version;
    pt_mp3_handler->ui1_layer = ui1_layer;
    pt_mp3_handler->ui4_sample_rate = ui4_sample_rate;
    pt_mp3_handler->ui4_frame_slots = ui4_frame_slots;
    pt_mp3_handler->ui1_slot_bytes = ui1_slot_bytes;
    pt_mp3_handler->ui8_frame_duration = ui8_frame_duration;

    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp3_midxbuld_get_frame_length
 *
 * Description: This function open a handler for a file
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object
 *
 * Outputs: pv_obj       modify data in the media info object
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mp3_midxbuld_get_frame_length(
    MP3_IDXBULD_OBJ_T*      pt_mp3_handler,
    UINT8                   ui1_rate_idx,
    BOOL                    b_is_padded,
    UINT32*                 pui4_frame_length
)
{
    UINT32                  ui4_bitrate = 0;
    UINT8                   ui1_padding;

    /* check parameter */
    if ((pt_mp3_handler == NULL) || (pui4_frame_length == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    /* check array index */
    if ((pt_mp3_handler->ui1_layer > MP3_IDXBULD_LAYER_III)
        || (ui1_rate_idx > MP3_MIDXBULD_MAX_BITRATE_IDX))
    {
        return MIDXBULDR_INV_ARG;
    }

    switch (pt_mp3_handler->ui1_version)
    {
        case MP3_IDXBULD_VERSION_1:
            ui4_bitrate = MPEG_V1_BITRATE_TAB[pt_mp3_handler->ui1_layer][ui1_rate_idx];
            break;
        case MP3_IDXBULD_VERSION_2:
        case MP3_IDXBULD_VERSION_2_5:
            ui4_bitrate = MPEG_V2_BITRATE_TAB[(pt_mp3_handler->ui1_layer + 1) >> 1][ui1_rate_idx];
            break;
    }
    ui4_bitrate *= 1000;    /* kbps => bps */

    ui1_padding = (b_is_padded == TRUE) ? (UINT8)1 : (UINT8)0;

    /* calculate frame length */
    if (ui4_bitrate != 0)
    {
        /* calculate frame length */
        if (pt_mp3_handler->ui4_sample_rate != 0)
        {
            *pui4_frame_length =
                (UINT32)(
                 _mm_div64(pt_mp3_handler->ui4_frame_slots * ui4_bitrate, pt_mp3_handler->ui4_sample_rate, NULL)
                 + ui1_padding
                ) * pt_mp3_handler->ui1_slot_bytes;
        }
    }
    else
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    return MIDXBULDR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_midxbuld_fopen
 *
 * Description: This function open a handler for a file
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object
 *
 * Outputs: pv_obj       modify data in the media info object
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_midxbuld_fopen(
                        CHAR*                  ps_path_name,
                        MIDXBULD_KEY_INFO_T*   pt_key_info,
    VOID*                   pv_obj
)
{

#ifndef __NO_FM__

    INT32               i4_ret;
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    MP3_IDXBULD_OBJ_T*     pt_mp3_handler = NULL;
    FM_FILE_INFO_T      t_info;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) || (pt_key_info == NULL) || (ps_path_name == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*)pv_obj;

    pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_b_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_b_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;

    pt_mp3_handler = (MP3_IDXBULD_OBJ_T*)_mm_util_mem_alloc(sizeof(MP3_IDXBULD_OBJ_T));
    if (pt_mp3_handler == NULL)
                {
        return MIDXBULDR_OUT_OF_MEMORY;
                }
    x_memset(pt_mp3_handler, 0, sizeof(MP3_IDXBULD_OBJ_T));
    pt_midxbuld_obj->pv_handler_obj  = pt_mp3_handler;
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path_name, FM_READ_ONLY, 0777, FALSE, &pt_midxbuld_obj->h_file);
    if (i4_ret != FMR_OK)
                {
        return MIDXBULDR_INITIAL_ERR;
                }
    i4_ret = x_fm_get_info_by_handle(pt_midxbuld_obj->h_file, &t_info);
    if (i4_ret != FMR_OK)
                {
        return MIDXBULDR_INITIAL_ERR;
    }
    pt_mp3_handler->ui8_mp3_length_in_byte = t_info.ui8_size;

#endif
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_midxbuld_mopen
 *
 * Description: This function open a handler for a memory buffer
 *
 * Inputs:  pv_mem_data  specify the address of the memory buffer
 *          z_mem_leng   specify the length of the buffer
 *          pv_obj       specify the media info object
 *
 * Outputs: pv_obj       modify data in the media info object
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_midxbuld_mopen(
    VOID*                   pv_mem_data,
    SIZE_T                  z_mem_leng,
    MIDXBULD_KEY_INFO_T*    pt_key_info,
    VOID*                   pv_obj
)
{
    MIDXBULD_OBJ_T*         pt_midxbuld_obj;
    MP3_IDXBULD_OBJ_T*      pt_mp3_handler = NULL;
    MEDIA_FEEDER_T*         pt_src_feeder;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL) || (pv_obj == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*)pv_obj;

    if (z_mem_leng == 0)
    {
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input1 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input1;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input3 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input3;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input4 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input4;
        pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos;
        pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_get_pos;
    }
    else
    {
        pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_b_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_b_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;
    }

    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;

    pt_mp3_handler = (MP3_IDXBULD_OBJ_T*)_mm_util_mem_alloc(sizeof(MP3_IDXBULD_OBJ_T));
    if (pt_mp3_handler == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_mp3_handler, 0, sizeof(MP3_IDXBULD_OBJ_T));
    pt_midxbuld_obj->pv_handler_obj = pt_mp3_handler;

    pt_mp3_handler->ui8_mp3_length_in_byte = pt_midxbuld_obj->z_buff_leng;

    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_midxbuld_close
 *
 * Description: This function close a handler
 *
 * Inputs:  pv_obj       specify the media info object
 *
 * Outputs:
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_midxbuld_close(
    VOID*                 pv_obj
)
{
    MIDXBULD_OBJ_T*    pt_midxbuld_obj;
    MP3_IDXBULD_OBJ_T*             pt_mp3_handler;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*)pv_obj;

    if (pt_midxbuld_obj->h_file)
    {
#ifndef __NO_FM__
        x_fm_close(pt_midxbuld_obj->h_file);
#endif
        pt_midxbuld_obj->h_file = NULL_HANDLE;
    }

    /* free memory */
    pt_mp3_handler = (MP3_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;

    if ((pt_mp3_handler != NULL)
        && (pt_mp3_handler->t_tbl_ext.t_elmt_tbl.u_fmt.t_es.pt_idx_entry != NULL))
    {
        _mm_util_mem_free((VOID*)(pt_mp3_handler->t_tbl_ext.t_elmt_tbl.u_fmt.t_es.pt_idx_entry));
    }

    if (pt_mp3_handler != NULL)
    {
        _mm_util_mem_free((VOID*)pt_mp3_handler);
        pt_midxbuld_obj->pv_handler_obj = NULL;
    }

    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_midxbuld_start_indexing
 *
 * Description: This function start indexing
 *
 * Inputs:  pv_obj       specify the media info object
 *
 * Outputs:
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_midxbuld_start_indexing(
                                  VOID*              pv_obj,
    UINT32*                        pui4_range_id
)
{
    MIDXBULD_OBJ_T*             pt_midxbuld_obj;
    MP3_IDXBULD_OBJ_T*          pt_mp3_handler;
    setpos_fct                     pf_set_pos;
    copybytes_fct                  pf_copybyte;
    getpos_fct                     pf_get_pos;
    UINT64                         ui8_data_pos;
    UINT64                         ui8_data_sz;
    UINT8*                         pui1_buff = NULL;
    UINT32                         ui4_read_sz;
    UINT64                         ui8_buff_pos;
    UINT32                         ui4_frame_length = 0;
    MIDXBULD_ELMT_STRM_ENTRY_T*    pt_idx_entry = NULL;
    UINT16                         ui2_entry_idx = 0;
    UINT64                         ui8_curr_dur = 0;
    UINT64                         ui8_frame_bng_pos;
    INT32                          i4_ret;
    MIDXBULD_KEY_TBL_EXT_T*        pt_tbl_ext;
    MIDXBULD_ELMT_TBL_OBJ_T*       pt_elmt_tbl;
    BOOL                           b_is_cbr = FALSE;
    BOOL                           b_xing_info;
    UINT32                         ui4_len_per_sec;
    UINT32                         ui4_idx = 0;
    UINT32                         ui4_total_duration;
    UINT8                          aui1_rate_idx[MP3_MIDXBULD_MAX_CBR_PARSING_NUM] = {0};
    INT32                          ui4_i = 0;
    BOOL                           b_idx_entry_overflow = FALSE;

#if 0
    x_dbg_stmt("S: %d\n", x_os_get_sys_tick() * 5);
#endif
    if (pv_obj == NULL)
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*)pv_obj;
    pt_mp3_handler = (MP3_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp3_handler == NULL)
    {
        return MIDXBULDR_INITIAL_ERR;
    }

    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;
    if ((pf_copybyte == NULL) || (pf_set_pos == NULL) || (pf_get_pos == NULL))
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    pt_idx_entry = (MIDXBULD_ELMT_STRM_ENTRY_T*)_mm_util_mem_alloc(
                        sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * MP3_MIDXBULD_MAX_ENTRY_NUM);
    if (pt_idx_entry == NULL)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    x_memset(pt_idx_entry, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * MP3_MIDXBULD_MAX_ENTRY_NUM);

    /* get frame begin position */
    ui8_frame_bng_pos = pt_midxbuld_obj->t_src_info.ui8_data_offset;

    /* set current data posistion to frame begin position */
    ui8_data_pos = pt_midxbuld_obj->t_src_info.ui8_data_offset;
    ui8_data_sz = pt_midxbuld_obj->t_src_info.ui8_data_size;
    ui4_total_duration = (pt_midxbuld_obj->t_src_info.ui4_total_duration) / 90000;
    if (ui4_total_duration == 0)
    {
        /* free memory */
        if (pt_idx_entry != NULL)
        {
            _mm_util_mem_free((VOID*)pt_idx_entry);
        }

        return MIDXBULDR_INTERNAL_ERR;
    }

#if 0
    /* ui8_idx_size is used to indicate that there is XING frame or NOT */
    b_xing_info = (pt_midxbuld_obj->t_src_info.ui8_idx_size != 0) ? TRUE : FALSE;
#else
    b_xing_info = FALSE;
#endif

#if 0
    /* ONLY build index table if XING frame is not exist */
    if (b_xing_info == FALSE)
    {
#endif
    /* allocate memory for buffer */
    pui1_buff = (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * MP3_MIDXBULD_MAX_BUFF_LEN);
    if (pui1_buff == NULL)
    {
        /* free memory */
        if (pt_idx_entry != NULL)
        {
            _mm_util_mem_free((VOID*)pt_idx_entry);
        }

        return MIDXBULDR_INTERNAL_ERR;
    }
    x_memset(pui1_buff, 0, sizeof(UINT8) * MP3_MIDXBULD_MAX_BUFF_LEN);

    /* seek to the position of first audio frame */
    IS_INPUT_OK(pf_set_pos(pt_midxbuld_obj, (INT64)ui8_frame_bng_pos, FM_SEEK_BGN));

    /* read first block of buffer */
    IS_INPUT_OK(pf_copybyte(pt_midxbuld_obj,
                            pui1_buff,
                            MP3_MIDXBULD_MAX_BUFF_LEN,
                            MP3_MIDXBULD_MAX_BUFF_LEN,
                            &ui4_read_sz));

    /* reset buffer position */
    ui8_buff_pos = 0;

    /* parse audio attribute */
    i4_ret = _mp3_midxbuld_parse_frame_attr(pui1_buff[ui8_buff_pos],
                                               pui1_buff[ui8_buff_pos + 1],
                                               pui1_buff[ui8_buff_pos + 2],
                                               pt_mp3_handler);
    if (i4_ret != MIDXBULDR_OK)
    {
        /* free memory */
        if (pt_idx_entry != NULL)
        {
            _mm_util_mem_free((VOID*)pt_idx_entry);
        }
        if (pui1_buff != NULL)
        {
            _mm_util_mem_free((VOID*)pui1_buff);
        }

        return MIDXBULDR_INTERNAL_ERR;
    }

    /* parse frame */
    while(ui8_data_pos < (ui8_frame_bng_pos + ui8_data_sz))
    {
        /* check if need to stop current indexing */
        if (pt_midxbuld_obj->b_abort_flag == TRUE)
        {
            /* free memory */
            if (pt_idx_entry != NULL)
            {
                _mm_util_mem_free((VOID*)pt_idx_entry);
            }
            if (pui1_buff != NULL)
            {
                _mm_util_mem_free((VOID*)pui1_buff);
            }
            return MIDXBULDR_ABORT_INDEXING;
        }

        /* check if need to read next block of buffer */
        if ((ui8_buff_pos + 2) > (ui4_read_sz - 1))
        {
            IS_INPUT_OK(pf_set_pos(pt_midxbuld_obj,
                                   (INT64)(ui8_buff_pos - ui4_read_sz),
                                    FM_SEEK_CUR));

            IS_INPUT_OK(pf_copybyte(pt_midxbuld_obj,
                                    pui1_buff,
                                    MP3_MIDXBULD_MAX_BUFF_LEN,
                                    MP3_MIDXBULD_MAX_BUFF_LEN,
                                    &ui4_read_sz));
            /* reset current buffer position */
            ui8_buff_pos = 0;
        }

        /* check frame header first */
        if ((pui1_buff[ui8_buff_pos] != 0xFF)
             || ((pui1_buff[ui8_buff_pos + 1] & 0xE0) != 0xE0)
             || ((pui1_buff[ui8_buff_pos + 1] & 0x18) >> 3 == 1) /* version description is set to reserved */
             || ((pui1_buff[ui8_buff_pos + 1] & 0x06) >> 1 == 0) /* layer description is set to reserved */
             || ((pui1_buff[ui8_buff_pos + 2] >> 4) == 0)
             || ((pui1_buff[ui8_buff_pos + 2] >> 4) == 15))
        {
            /* there are some garbage data, try to parse next byte */
            ui8_buff_pos += 1;
            ui8_data_pos += 1;

            continue;
        }

        /* get frame length of current audio frame */
        i4_ret = _mp3_midxbuld_get_frame_length(pt_mp3_handler,
                                                (pui1_buff[ui8_buff_pos + 2]) >> 4,
                                                (((pui1_buff[ui8_buff_pos + 2] & 0x2) >> 1)) ? TRUE : FALSE,
                                                &ui4_frame_length);

        if ((i4_ret != MIDXBULDR_OK) || (ui4_frame_length == 0))
        {
            /* there are some garbage data, try to parse next byte */
            ui8_buff_pos += 1;  
            ui8_data_pos += 1;
            
            continue;
        }
        
        ui8_curr_dur += pt_mp3_handler->ui8_frame_duration;

        if (ui4_idx < MP3_MIDXBULD_MAX_CBR_PARSING_NUM)
        {
            aui1_rate_idx[ui4_idx] = (pui1_buff[ui8_buff_pos + 2]) >> 4;
        }
        
        if ((UINT16)_mm_div64(ui8_curr_dur, (1000 * 1000), NULL) >= ui2_entry_idx)
        {
            pt_idx_entry[ui2_entry_idx].ui4_pts = (UINT32)(ui2_entry_idx * 90000);
            pt_idx_entry[ui2_entry_idx].ui4_relative_offset = (UINT32)(ui8_data_pos - ui8_frame_bng_pos);

            if ((ui2_entry_idx - 1) >= 0)
            {
                pt_idx_entry[ui2_entry_idx - 1].ui4_size = (pt_idx_entry[ui2_entry_idx].ui4_relative_offset) -
                                                            (pt_idx_entry[ui2_entry_idx - 1].ui4_relative_offset);
            }
#if 0
            x_dbg_stmt("PTS: %3d, ", ui2_entry_idx);
            x_dbg_stmt("Offset: %d\n", pt_idx_entry[ui2_entry_idx].ui4_relative_offset);
#endif
            ui2_entry_idx++;
            if (ui2_entry_idx >= MP3_MIDXBULD_MAX_ENTRY_NUM)
            {
                /* free memory */
                if (pt_idx_entry != NULL)
                {
                    _mm_util_mem_free((VOID*)pt_idx_entry);
                }
                if (pui1_buff != NULL)
                {
                    _mm_util_mem_free((VOID*)pui1_buff);
                }

                /* if index entry is more than MP3_MIDXBULD_MAX_ENTRY_NUM, 
                                do not return fail, so that this file can be played*/
                b_idx_entry_overflow = TRUE;
                break;
                /*return MIDXBULDR_INTERNAL_ERR;*/
            }
        }
        /* when MP3_MIDXBULD_MAX_CBR_PARSING_NUM frames are parsed => check VBR or CBR */
        if (ui4_idx == (MP3_MIDXBULD_MAX_CBR_PARSING_NUM - 1))
        {
            while (ui4_i < MP3_MIDXBULD_MAX_CBR_PARSING_NUM)
            {
                if (aui1_rate_idx[0] != aui1_rate_idx[ui4_i])
                {
                    break;
                }
                ui4_i++;
            }
            if (ui4_i == MP3_MIDXBULD_MAX_CBR_PARSING_NUM)
            {
                b_is_cbr = TRUE;
                break;
            }
        }
#if 0
        x_dbg_stmt("ID: %d, Offset: %d", ui4_idx++, ui8_data_pos);
        x_dbg_stmt(", Frame length: %d\n", ui4_frame_length);
#endif
        ui8_buff_pos += ui4_frame_length;
        ui8_data_pos += ui4_frame_length;
        ui4_idx++;
    }

    /*
       1. When escape the while loop, fill the size of the last entry for the VBR case
       2. The total number of entry is "ui2_entry_idx", so the index of the last entry is "ui2_entry_idx - 1"
    */

    if(!b_idx_entry_overflow)
    {
        if (b_is_cbr == FALSE)
        {
            pt_idx_entry[ui2_entry_idx - 1].ui4_size = 
                (UINT32)(ui8_data_sz) - pt_idx_entry[ui2_entry_idx - 1].ui4_relative_offset;
        }
    }

    /* free memory */
    if (pui1_buff != NULL)
    {
        _mm_util_mem_free((VOID*)pui1_buff);
    }
#if 0
    }
#endif

    if (b_xing_info == TRUE || b_is_cbr == TRUE)
    {
        ui4_len_per_sec = (UINT32)_mm_div64(ui8_data_sz, ui4_total_duration, NULL);
        /* force reset entry index to 0 */
        ui2_entry_idx = 0;

        while ((UINT32)ui2_entry_idx < ui4_total_duration)
        {
            pt_idx_entry[ui2_entry_idx].ui4_pts             = (UINT32)(ui2_entry_idx * 90000);
            pt_idx_entry[ui2_entry_idx].ui4_relative_offset = ui4_len_per_sec * ui2_entry_idx;

            if ((UINT32)ui2_entry_idx == (ui4_total_duration - 1))
            {
                pt_idx_entry[ui2_entry_idx].ui4_size = (UINT32)(ui8_data_sz) - pt_idx_entry[ui2_entry_idx].ui4_relative_offset;
            }
            else
            {
                pt_idx_entry[ui2_entry_idx].ui4_size = ui4_len_per_sec;
            }

#if 0
            x_dbg_stmt("PTS: %3d, ", ui2_entry_idx);
            x_dbg_stmt("Offset: %d\n", pt_idx_entry[ui2_entry_idx].ui4_relative_offset);
#endif
            ui2_entry_idx++;
            if (ui2_entry_idx >= MP3_MIDXBULD_MAX_ENTRY_NUM)
            {
                /* free memory */
                if (pt_idx_entry != NULL)
                {
                    _mm_util_mem_free((VOID*)pt_idx_entry);
                }
                /* fill data */
                /* if index entry is more than MP3_MIDXBULD_MAX_ENTRY_NUM, 
                                do not return fail, so that this file can be played*/
                b_idx_entry_overflow = TRUE;
                break;
                /*return MIDXBULDR_INTERNAL_ERR;*/
            }
        }
    }

    if(!b_idx_entry_overflow)
    {
        if(ui2_entry_idx < MP3_MIDXBULD_MAX_ENTRY_NUM)
        {
            pt_idx_entry[ui2_entry_idx].ui4_pts = (UINT32)(ui2_entry_idx * 90000);
            pt_idx_entry[ui2_entry_idx].ui4_relative_offset = (UINT32)ui8_data_sz;
            pt_idx_entry[ui2_entry_idx].ui4_size = 0;
        }
    }
    else
    {
        ui2_entry_idx = 0;
        pt_idx_entry  = NULL;
    }
    
    pt_tbl_ext = &(pt_mp3_handler->t_tbl_ext);

    /* fill data */
    pt_elmt_tbl = &(pt_tbl_ext->t_elmt_tbl);
    pt_elmt_tbl->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    pt_elmt_tbl->ui4_number_of_entry = 
        (ui2_entry_idx == 0) ? 0 : (UINT32)ui2_entry_idx + 1;
    pt_elmt_tbl->ui8_base_offset = ui8_frame_bng_pos;
    pt_elmt_tbl->u_fmt.t_es.t_info.e_es_type = MIDXBULD_ELEM_STRM_TYPE_KEY;
    pt_elmt_tbl->u_fmt.t_es.pt_idx_entry = pt_idx_entry;

#if 0
    x_dbg_stmt("E: %d\n", x_os_get_sys_tick() * 5);
    x_dbg_stmt("Total duration: %d\n", pt_elmt_tbl->ui4_number_of_entry);
#endif

    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_midxbuld_get_info
 *
 * Description: This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  pv_obj       specify the media info object
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_midxbuld_get_info (
    VOID*                       pv_obj,
    MIDXBULD_GET_INFO_TYPE_T    e_info_type,
    VOID*                       pv_buf,
    SIZE_T*                     pz_buf_leng
)
{
    MIDXBULD_OBJ_T*             pt_midxbuld_obj;
    MP3_IDXBULD_OBJ_T*          pt_mp3_handler;
    MIDXBULD_KEY_TBL_INFO_T*    pt_tbl_info;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) || (pv_buf == NULL) || (pz_buf_leng == NULL))
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*)pv_obj;
    pt_mp3_handler = (MP3_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp3_handler == NULL)
    {
        return MIDXBULDR_INITIAL_ERR;
        }

    switch (e_info_type)
    {
        case MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST:
            pt_tbl_info = (MIDXBULD_KEY_TBL_INFO_T*)pv_buf;
            *pz_buf_leng = sizeof(MIDXBULD_KEY_TBL_INFO_T);
            /* copy data */
            pt_tbl_info->pt_key_tbl_ext = &(pt_mp3_handler->t_tbl_ext);
            break;
        case MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL:
    default:
        return MIDXBULDR_INV_ARG;
    }

    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_midxbuld_set_info
 *
 * Description: This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  pv_obj       specify the media info object
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_midxbuld_set_info(
                            VOID*                        pv_obj,
                            MIDXBULD_SET_INFO_TYPE_T     e_info_type,
                            VOID*                        pv_buf,
    SIZE_T*                     pz_buf_leng
)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    MP3_IDXBULD_OBJ_T*  pt_mp3_handler;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) || (pv_buf == NULL) || (pz_buf_leng== NULL))
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    pt_mp3_handler = (MP3_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp3_handler == NULL)
    {
        return MIDXBULDR_INITIAL_ERR;
    }

    return MIDXBULDR_OK;
}

