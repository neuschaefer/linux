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
 * $RCSfile: ps_mfmtrecg.c,v $
 * $Revision: #2 $ 1.0
 * $Date: 2012/07/25 $ 2004/11/26
 * $Author: cong.wang $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains Video ES only media format recognizer public APIs  
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
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "dbg/x_dbg.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "util/x_lnk_list.h"
#include "inc/x_mm_common.h"
#include "mutil/mm_util_memory.h"
#include "mutil/mm_util_input.h"
#include "mutil/mfmtrecg/mfmtrecg.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_mfmtrecg.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"
#include "../../mm_util_memory.h"
#include "../../mm_util.h"
#include "../../mm_util_input.h"
#include "../mfmtrecg.h"
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define VID_ES_FILE_SEEK_SIZE                       (1024)
#define VID_ES_LOOKUP_PICTURE_COUNT                 3
#define VID_ES_LOOKUP_LENGTH                        (3*1024*1024)

#define CONTAINER_RIFF_HEAD                          0x52494646
#define CONTAINER_RIFF_MASK                          0xFFFFFFFF
#define CONTAINER_MOOV_HEAD                          0x6D6F6F76
#define CONTAINER_MOOV_MASK                          0xFFFFFFFF
#define CONTAINER_RMVB_HEAD_REV                      0x464D522E
#define CONTAINER_RIFF_HEAD_REV                      0x46464952
#define CONTAINER_FORM_HEAD_REV                      0x4D524F46
#define CONTAINER_ASF_HEAD_REV1                      0x75B22630
#define CONTAINER_ASF_HEAD_REV2                      0x11CF668E
#define CONTAINER_ASF_HEAD_REV3                      0xAA00D9A6
#define CONTAINER_ASF_HEAD_REV4                      0x6CCE6200
#define CONTAINER_MKV_HEAD_REV                       0xA3DF451A
#define CONTAINER_MP4_HEAD_REV                       0x70797466
#define CONTAINER_FLV_HEAD_REV                       0x00564C46
#define CONTAINER_FLV_REV_MASK                       0x00FFFFFF
#define CONTAINER_MKV_HEAD_OGG                       0x5367674F

#define MPEG_VIDEO_START_CODE_PREFIX                 0x000001

#define VIDEO_SEQUENCE_HEADER_CODE                   0x000001B3
#define VIDEO_EXTENSION_START_CODE                   0x000001B5

#define VISUAL_OBJECT_SEQUENCE_START_CODE            0x000001B0
#define VIDEO_OBJECT_START_VALUE_MIN                 0x00000100
#define VIDEO_OBJECT_START_VALUE_MAX                 0x0000011F
#define VIDEO_OBJECT_LAYER_START_VALUE_MIN           0x00000120
#define VIDEO_OBJECT_LAYER_START_VALUE_MAX           0x0000012F
#define VOP_START_CODE                               0x000001B6

#define IDR_CODE                                     0x0580
#define NON_IDR_CODE                                 0x0180
#define PPS_CODE                                     0x0800

#define IDR_MASK                                     0x1f80
#define PPS_MASK                                     0x1f00


#define VC1_SEQUENCE_HEADER_CODE                     0x0000010F
#define VC1_ENTRY_POINT_HEADER_CODE                  0x0000010E
#define VC1_FRAME_HEADER_CODE                        0x0000010D

#define VIDEO_EXTENSION_ID_MASK                      0xF0
#define VIDEO_SEQUENCE_EXTENSION                     0x10
#define VIDEO_PIC_EXTENSION                          0x80

#define VIDEO_GROUP_PICTURE_CODE                     0x000001B8
#define VIDEO_PICTURE_START_CODE                     0x00000100
#define VIDEO_SLICE_START                            0x00000101
#define VIDEO_SLICE_END                              0x000001AF

#define RIFF_HEADER                                  0x52494646
#define MP4_HEADER                                   0x66747970
#define RIFF_AVI                                     0x41565920
#define RIFF_WAVE                                    0x57415645
#define ID3_MP3                                      0x00334449

/* This function is the comfirm function of a file  */
typedef INT32 (*vid_es_confirm_fct) (MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format);
typedef struct _VID_ES_FMT_RECG_T
{
    MEDIA_VID_SUBTYPE_T         t_subtype;
    vid_es_confirm_fct          pf_confirm;
} VID_ES_FMT_RECG_T;
typedef struct _VID_ES_STARTING_CODE
{
    UINT32      ui4_code;
    UINT32      ui4_mask;
}VID_ES_STARTING_CODE;
extern INT32 _m1v2v_es_confirm_fct(MM_INPUT_STREAM_T*      pt_input,       
                                    MEDIA_FORMAT_T*       pt_format);
extern INT32 _m4v_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format);
extern INT32 _h264_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format);       
extern INT32 _vc1_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format);      


static VID_ES_FMT_RECG_T        at_vid_es_recg[]  = 
{
    {
        MEDIA_VID_SUBTYPE_M2V,      /*including MEDIA_VID_SUBTYPE_M1V*/
        _m1v2v_es_confirm_fct,
    },
    {
        MEDIA_VID_SUBTYPE_M4V,
        _m4v_es_confirm_fct,
    },
    {
        MEDIA_VID_SUBTYPE_H264,
        _h264_es_confirm_fct,
    },
    {
        MEDIA_VID_SUBTYPE_VC1,
        _vc1_es_confirm_fct,
    },
    {
        MEDIA_VID_SUBTYPE_UNKNOWN,
        NULL,
    }
};

static VID_ES_STARTING_CODE ga_contain_header_code[]=
{
    {CONTAINER_RIFF_HEAD, CONTAINER_RIFF_MASK}
};
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
static INT32 _es_seek_start_code(MM_INPUT_STREAM_T* pt_input,       /*IN*/
                                 UINT64*            pui8_cur_pos,    /*IN/OUT*/
                                 UINT32             ui4_code_prefix, /*IN*/
                                 UINT32*            pui4_code)       /*IN/OUT*/
{

    UINT64            ui8_file_pos;
    UINT32            ui4_read_count;
    UINT32            ui4_read_idx;
    UINT32            ui4_tmp_code;
    INT32             i4_ret;
    UINT8             ui1_tmp;
    UINT8*            pui1_buf;

    pui1_buf = (UINT8*)_mm_util_mem_alloc(VID_ES_FILE_SEEK_SIZE);
    if (NULL == pui1_buf)
    {
        return MFMTRECGR_OUT_OF_MEMORY;
    }
    
    i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                  (INT64)(*pui8_cur_pos),
                                   MEDIA_SEEK_BGN);
    if (i4_ret != INPUTR_OK)
    {
        _mm_util_mem_free(pui1_buf);
        return MFMTRECGR_FILE_ERR;
    }
    
    i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_file_pos);
    if (i4_ret != INPUTR_OK)
    {
        _mm_util_mem_free(pui1_buf);
        return MFMTRECGR_FILE_ERR;
    }
    
    ui4_read_idx = 0;
    i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                    pui1_buf, 
                                    VID_ES_FILE_SEEK_SIZE,
                                    VID_ES_FILE_SEEK_SIZE,
                                    &ui4_read_count);
    if (i4_ret != INPUTR_OK)
    {
        _mm_util_mem_free(pui1_buf);
        return MFMTRECGR_FILE_ERR;
    }
    if (ui4_read_count < 4)
    {
        _mm_util_mem_free(pui1_buf);
        return MFMTRECGR_INTERNAL_ERR;
    }
    ui4_tmp_code  = pui1_buf[ui4_read_idx++];
    ui4_tmp_code  = ((ui4_tmp_code << 8) | pui1_buf[ui4_read_idx++]);
    ui4_tmp_code  = ((ui4_tmp_code << 8) | pui1_buf[ui4_read_idx++]);
    ui4_tmp_code  = ((ui4_tmp_code << 8) | pui1_buf[ui4_read_idx++]);
    ui8_file_pos += 4;
    
    do
    {
        if (ui8_file_pos > VID_ES_LOOKUP_LENGTH)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_INTERNAL_ERR;
        }
        
        if ((ui4_tmp_code >> 8) == ui4_code_prefix)
        {
            *pui4_code = ui4_tmp_code;
            _mm_util_mem_free(pui1_buf);
            *pui8_cur_pos = ui8_file_pos;
            
            return MFMTRECGR_OK;
        }
        else
        {
            if (ui4_read_idx >= ui4_read_count)
            {
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                VID_ES_FILE_SEEK_SIZE,
                                                VID_ES_FILE_SEEK_SIZE,
                                                &ui4_read_count);
                
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }

                if(ui4_read_count < 1)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INTERNAL_ERR;
                }
            }

            ui1_tmp       = pui1_buf[ui4_read_idx++];
            ui8_file_pos += 1;

            ui4_tmp_code  = ((ui4_tmp_code << 8) | ui1_tmp);
        }
        
    } while (TRUE);

}

/* Check whether it is ts file with packet size 204*/
INT32 _es_handler_is_ts_204(MM_INPUT_STREAM_T* pt_input)
{
    UINT8   ui1_tmp = 0;
    INT32   i4_ret = 0;

    i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 0, FM_SEEK_BGN);
    if (i4_ret != FMR_OK)
    {
        return MFMTRECGR_FILE_ERR;
    }
    
    i4_ret = pt_input->pf_input1(pt_input->pv_obj, &ui1_tmp);
    if (i4_ret != FMR_OK)
    {
        return MFMTRECGR_FILE_ERR;
    }
    if (ui1_tmp == 0x47)
    {
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 204, FM_SEEK_BGN);
        if (i4_ret != FMR_OK)
        {
            return MFMTRECGR_FILE_ERR;
        }
        
        i4_ret = pt_input->pf_input1(pt_input->pv_obj, &ui1_tmp);
        if (i4_ret != FMR_OK)
        {
            return MFMTRECGR_FILE_ERR;
        }
        if (ui1_tmp == 0x47)
        {
            return MFMTRECGR_OK;
        }
    }
    return MFMTRECGR_INTERNAL_ERR;
}

static INT32 _es_chk_unknown_fmt(MM_INPUT_STREAM_T* pt_input)       /*IN*/
{
    INT32                   i4_ret;
    UINT32                  ui4_code[4];
    UINT32                  ui4_size;
    
    i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 0, FM_SEEK_BGN);
    if (i4_ret == INPUTR_OK)
    {
        i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, ui4_code, 16, 16, &ui4_size);
        if (i4_ret != INPUTR_OK)
        {
            return MFMTRECGR_FILE_ERR;
        }
        if(ui4_size < 16)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }
        
        if(   ui4_code[1] == (UINT32)CONTAINER_MP4_HEAD_REV  
           || ui4_code[0] == (UINT32)CONTAINER_RMVB_HEAD_REV 
           || ui4_code[0] == (UINT32)CONTAINER_RIFF_HEAD_REV 
           || ui4_code[0] == (UINT32)CONTAINER_FORM_HEAD_REV 
           || ui4_code[0] == (UINT32)CONTAINER_MKV_HEAD_REV 
           || ui4_code[0] == (UINT32)CONTAINER_MKV_HEAD_OGG 
           || (ui4_code[0] & CONTAINER_FLV_REV_MASK) == (UINT32)CONTAINER_FLV_HEAD_REV 
           ||  (  ui4_code[0] == (UINT32)CONTAINER_ASF_HEAD_REV1 
               && ui4_code[1] == (UINT32)CONTAINER_ASF_HEAD_REV2
               && ui4_code[2] == (UINT32)CONTAINER_ASF_HEAD_REV3
               && ui4_code[3] == (UINT32)CONTAINER_ASF_HEAD_REV4
               )
           || ((ui4_code[0] << 24 >> 24 == 0xFF)
                           && (((ui4_code[0] << 16) >> 24 & 0xE0) == 0xE0)
                           && (((ui4_code[0] << 16) >> 24 & 0x18) >> 3 != 1)
                           && (((ui4_code[0] << 16) >> 24 & 0x06) >> 1 != 0)
                           && ((ui4_code[0] << 8)  >> 28 != 0)
                           && ((ui4_code[0] << 8)  >> 28 != 15)
              )
           || ui4_code[0]<< 8 >> 8 == (UINT32)ID3_MP3
              
           )
        {

            #ifndef __KERNEL__
            x_dbg_stmt("%s ret MFMTRECGR_INTERNAL_ERR!!!\n", __func__);
            #endif
            return MFMTRECGR_INTERNAL_ERR;
        }
    }
    else
    {
        return MFMTRECGR_FILE_ERR;
    }
    
    if(_es_handler_is_ts_204(pt_input) == MFMTRECGR_OK)
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    
    i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 0, FM_SEEK_BGN);
    return MFMTRECGR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _m1v2v_es_file_confirm_fct
 *
 * Description: this function confirm mepg1/mepg2 video es
 *
 * Inputs:  pt_input         specify input stream
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 * Author : Lianming Lin(mtk40234)
 *
 * History:
 *   (1)2009-07-08 : initial
 ----------------------------------------------------------------------------*/
INT32 _m1v2v_es_confirm_fct(MM_INPUT_STREAM_T*    pt_input,       
                            MEDIA_FORMAT_T*       pt_format)     
{
    UINT64                ui8_cur_pos;
    UINT64                ui8_pre_file_pos;
    INT32                 i4_ret;
    UINT32                ui4_code;
    
    UINT8                 ui1_ext_id;
    BOOL                  b_seq_ext;
    BOOL                  b_pic_ext;
    /*BOOL                  b_gop;*/
    BOOL                  b_picture;
    /*UINT32                ui4_read_count;*/
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_format) || (NULL == pt_input))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    do
    {
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 0, FM_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }
        
        i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_cur_pos);
        if (i4_ret != INPUTR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }

        do
        {
            i4_ret = _es_seek_start_code(pt_input, 
                                         &ui8_cur_pos, 
                                         MPEG_VIDEO_START_CODE_PREFIX, 
                                         &ui4_code);
            if (i4_ret != MFMTRECGR_OK)
            {
                return i4_ret;
            }
        
            if (VIDEO_SEQUENCE_HEADER_CODE == ui4_code)
            {
                UINT32 ui4_hv = 0;
                UINT32 ui4_h = 0;   /*horizontal size*/
                UINT32 ui4_v = 0;   /*vertical size*/
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos, 
                                              FM_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    return MFMTRECGR_FILE_ERR;
                }
                i4_ret = pt_input->pf_input4(pt_input->pv_obj, 
                                             &ui4_hv);
                if (i4_ret != INPUTR_OK)
                {
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_h = (ui4_hv >> 20) & 0xFFF;
                ui4_v = (ui4_hv >> 8) & 0xFFF;
                if((ui4_v > 1088) || (ui4_h > 1920) || ui4_v < 1 || ui4_h < 1)
                {
                    return MFMTRECGR_INTERNAL_ERR;
                }
                break;
            }
            
        } while (TRUE);
        
        do
        {
            b_seq_ext = FALSE;
            b_pic_ext = FALSE;
            /*b_gop      = FALSE;*/
            b_picture = FALSE;
            
            ui8_pre_file_pos = ui8_cur_pos;
            do
            {
                i4_ret = _es_seek_start_code(pt_input, 
                                               &ui8_cur_pos, 
                                               MPEG_VIDEO_START_CODE_PREFIX, 
                                               &ui4_code);
                if (i4_ret != MFMTRECGR_OK)
                {
                    return i4_ret;
                }

                if (!b_seq_ext)
                {
                    if (VIDEO_EXTENSION_START_CODE == ui4_code)
                    {
                        /*get extension identifier*/
                        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                                      (INT64)ui8_cur_pos, 
                                                      FM_SEEK_BGN);
                        if (i4_ret != INPUTR_OK)
                        {
                            return MFMTRECGR_FILE_ERR;
                        }
                        #if 1
                        i4_ret = pt_input->pf_input1(pt_input->pv_obj, 
                                                     &ui1_ext_id);
                        if (i4_ret != INPUTR_OK)
                        {
                            return MFMTRECGR_FILE_ERR;
                        }
                        #else
                        i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                        (VOID*)&ui1_ext_id, 
                                                        1,
                                                        1,
                                                        &ui4_read_count);
                        if ((i4_ret != INPUTR_OK) || (ui4_read_count < 1))
                        {
                            return MFMTRECGR_INTERNAL_ERR;
                        }
                        #endif
                        
                        ++ui8_cur_pos;
                        
                        ui1_ext_id &= VIDEO_EXTENSION_ID_MASK;
                        if (VIDEO_SEQUENCE_EXTENSION == ui1_ext_id)
                        {
                            b_seq_ext = TRUE;
                        }
                        
                    }
                    #if 0
                    else if (VIDEO_GROUP_PICTURE_CODE == ui4_code)
                    {
                        b_gop = TRUE;
                    }
                    #endif
                    else if (VIDEO_PICTURE_START_CODE == ui4_code)
                    {
                        b_picture = TRUE;
                    }
                    else
                    {
                    }

                }
                else
                {

                    if (VIDEO_EXTENSION_START_CODE == ui4_code)
                    {
                        /*get extension identifier*/
                        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                                      (INT64)ui8_cur_pos, 
                                                      FM_SEEK_BGN);
                        if (i4_ret != INPUTR_OK)
                        {
                            return MFMTRECGR_FILE_ERR;
                        }
                        #if 1
                        i4_ret = pt_input->pf_input1(pt_input->pv_obj, 
                                                     &ui1_ext_id);
                        if (i4_ret != INPUTR_OK)
                        {
                            return MFMTRECGR_FILE_ERR;
                        }
                        #else
                        i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                        (VOID*)&ui1_ext_id, 
                                                        1,
                                                        1,
                                                        &ui4_read_count);
                        if ((i4_ret != INPUTR_OK) || (ui4_read_count < 1))
                        {
                            return MFMTRECGR_INTERNAL_ERR;
                        }
                        #endif
                        ++ui8_cur_pos;
                        
                        ui1_ext_id &= VIDEO_EXTENSION_ID_MASK;
                        if (VIDEO_PIC_EXTENSION == ui1_ext_id)
                        {
                            b_pic_ext = TRUE;
                        }
                        
                    }
                    #if 0
                    else if (VIDEO_GROUP_PICTURE_CODE == ui4_code)
                    {
                        b_gop = TRUE;
                    }
                    #endif
                    else if (VIDEO_PICTURE_START_CODE == ui4_code)
                    {
                        b_picture = TRUE;
                    }
                    else
                    {
                    }
                    
                }
                
                if (VIDEO_SEQUENCE_HEADER_CODE == ui4_code)
                {
                    break;      /*next sequence*/
                }
                else
                {
                }
        
                /*
                  Note: If find sequence extension and picture extension should be MPEG-2
                        If only no sequence extension and picture extension then be MPEG-1
                        sequence precedes picture
                */
                
                /*If mpeg-2 video stream*/
                /*if (b_seq_ext && b_gop && b_pic_ext && b_picture)*/
                if (b_seq_ext && b_pic_ext && b_picture)
                {
                    pt_format->e_media_type                    = MEDIA_TYPE_VIDEO_ES;
                    pt_format->t_media_subtype.u.e_vid_subtype = MEDIA_VID_SUBTYPE_M2V;
                    return MFMTRECGR_OK;
                }
                
                /*If mpeg-1 video stream*/
                /*if (b_gop && b_picture)*/
                if (!b_seq_ext && !b_pic_ext && b_picture)
                {
                    pt_format->e_media_type                    = MEDIA_TYPE_VIDEO_ES;
                    pt_format->t_media_subtype.u.e_vid_subtype = MEDIA_VID_SUBTYPE_M1V;
                    return MFMTRECGR_OK;
                }
                
                if (VID_ES_LOOKUP_LENGTH < (UINT32)(ui8_cur_pos - ui8_pre_file_pos))
                {
                    return MFMTRECGR_INV_ARG;
                }
                
            } while (TRUE);
            
        } while (TRUE);

    }while(0);
    
    return i4_ret;
}
                           


/*-----------------------------------------------------------------------------
 * Name:  _m4v_es_confirm_fct
 *
 * Description: this function confirm mepg4 video es
 *
 * Inputs:  pt_input       specify input stream
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 * Author : Lianming Lin(mtk40234)
 *
 * History:
 *   (1)2009-07-08 : initial
 ----------------------------------------------------------------------------*/
INT32 _m4v_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format)     
{

    UINT64                ui8_cur_pos;
    UINT64                ui8_pre_file_pos;
    INT32                 i4_ret;
    UINT32                ui4_code;
    
    BOOL                  b_vid_obj;
    BOOL                  b_vid_obj_layer;
    BOOL                  b_vop;
        
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_format) || (NULL == pt_input))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    do
    {
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 0, FM_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }
        
        i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_cur_pos);
        if (i4_ret != INPUTR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }

        #if 0
        do 
        {
            i4_ret = _es_seek_start_code(pt_input, 
                                         &ui8_cur_pos, 
                                         MPEG_VIDEO_START_CODE_PREFIX, 
                                         &ui4_code);
            if (i4_ret != MFMTRECGR_OK)
            {
                return i4_ret;
            }
            
            if (VISUAL_OBJECT_SEQUENCE_START_CODE == ui4_code)
            {
                break;
            }
            
        } while (TRUE);
        #endif
        
        do 
        {
            b_vid_obj        = FALSE;
            b_vid_obj_layer  = FALSE;
            b_vop            = FALSE;
            
            ui8_pre_file_pos = ui8_cur_pos;
            do 
            {
                i4_ret = _es_seek_start_code(pt_input, 
                                             &ui8_cur_pos, 
                                                MPEG_VIDEO_START_CODE_PREFIX, 
                                                &ui4_code);
                if (i4_ret != MFMTRECGR_OK)
                {
                    return i4_ret;
                }

                if (!b_vid_obj)
                {
                    if ((ui4_code >= VIDEO_OBJECT_START_VALUE_MIN) 
                        && (ui4_code <= VIDEO_OBJECT_START_VALUE_MAX))
                    {
                        /*The first start code must be at the beginning of file.
                        if(ui8_cur_pos > 4)
                        {
                            return MFMTRECGR_INTERNAL_ERR;
                        }*/
                        b_vid_obj = TRUE;
                    }
                }
                else
                {
                    if (!b_vid_obj_layer)
                    {
                        if ((ui4_code >= VIDEO_OBJECT_LAYER_START_VALUE_MIN) 
                            && (ui4_code <= VIDEO_OBJECT_LAYER_START_VALUE_MAX))
                        {
                            b_vid_obj_layer = TRUE;
                        }
                    }
                    else
                    {
                        if (!b_vop)
                        {
                            if (VOP_START_CODE == ui4_code)
                            {
                                b_vop = TRUE;
                            }
                        }
                    }
                }
                
                #if 0
                /*if a sequence break*/
                if (VISUAL_OBJECT_SEQUENCE_START_CODE == ui4_code)
                {
                    break;
                }
                #endif
                
                if (b_vid_obj && b_vid_obj_layer && b_vop)
                {
                    pt_format->e_media_type                    = MEDIA_TYPE_VIDEO_ES;
                    pt_format->t_media_subtype.u.e_vid_subtype = MEDIA_VID_SUBTYPE_M4V;
                    return MFMTRECGR_OK;
                }
                
                if (VID_ES_LOOKUP_LENGTH < (UINT32)(ui8_cur_pos - ui8_pre_file_pos))
                {
                    return MFMTRECGR_INV_ARG;
                }
                
            } while (TRUE);
            
        } while (0);
        
    }while(0);
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _h264_es_confirm_fct
 *
 * Description: this function confirm H264 video es
 *
 * Inputs:  pt_input       specify input stream
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 * Author : Lianming Lin(mtk40234)
 *
 * History:
 *   (1)2009-07-08 : initial
 ----------------------------------------------------------------------------*/
INT32 _h264_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format)     
{

    UINT64                ui8_cur_pos;
    UINT64                ui8_pre_file_pos;
    INT32                 i4_ret;
    UINT32                ui4_code;
    UINT32                ui4_i;
    UINT8                 ui1_nextbyte;                
    
    BOOL                  b_idr;
    BOOL                  b_non_idr;
    BOOL                  b_pps;
        
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_format) || (NULL == pt_input))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    do
    {
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 0, FM_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }
        
        i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_cur_pos);
        if (i4_ret != INPUTR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }

        /*Judgement the starting code*/
        i4_ret = pt_input->pf_input4(pt_input->pv_obj, &ui4_code);
        if (i4_ret != INPUTR_OK)
        {
            return MFMTRECGR_FILE_ERR;
        }
        for(ui4_i = 0; 
            ui4_i < sizeof(ga_contain_header_code)/sizeof(VID_ES_STARTING_CODE); 
            ui4_i++)
        {
            if((ui4_code & ga_contain_header_code[ui4_i].ui4_mask) 
                == ga_contain_header_code[ui4_i].ui4_code)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
        }
        /*judge if it is mp4*/
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, (INT64)(ui4_code+4), FM_SEEK_BGN);
        if (i4_ret == INPUTR_OK)
        {
            i4_ret = pt_input->pf_input4(pt_input->pv_obj, &ui4_code);
            if (i4_ret != INPUTR_OK)
            {
                return MFMTRECGR_FILE_ERR;
            }
            if((ui4_code & CONTAINER_MOOV_MASK) == CONTAINER_MOOV_HEAD)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
        }
        else
        {
            return MFMTRECGR_FILE_ERR;
        }
        
            
            
        /*what is sequence header?*/
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 0, FM_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }
        
        i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_cur_pos);
        if (i4_ret != INPUTR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }
        
        do 
        {
            b_idr        = FALSE;
            b_non_idr    = FALSE;
            b_pps        = FALSE;
            
            ui8_pre_file_pos = ui8_cur_pos;
            do 
            {
                i4_ret = _es_seek_start_code(pt_input, 
                                             &ui8_cur_pos, 
                                                MPEG_VIDEO_START_CODE_PREFIX, 
                                                &ui4_code);
                if (i4_ret != MFMTRECGR_OK)
                {
                    return i4_ret;
                }
                
                /*get next byte */
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos, 
                                              FM_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    return MFMTRECGR_FILE_ERR;
                }
                i4_ret = pt_input->pf_input1(pt_input->pv_obj, 
                                             &ui1_nextbyte);
                if (i4_ret != INPUTR_OK)
                {
                    return MFMTRECGR_FILE_ERR;
                }
                ++ui8_cur_pos;
                
                ui4_code  &= 0x000000ff;
                ui4_code   = ((ui4_code << 8) | ui1_nextbyte);
                
                /*modify : check pps first then check idr*/
                if(!b_pps)
                {
                    if (PPS_CODE == (ui4_code & PPS_MASK))
                    {
                        b_pps = TRUE;
                    }
                }
                else
                {
                    if (!b_idr && !b_non_idr)
                    {
                        ui4_code &= IDR_MASK;
                        if (IDR_CODE == ui4_code)
                        {
                            b_idr = TRUE;
                        }
                        else if (NON_IDR_CODE == ui4_code)
                        {
                            b_non_idr = TRUE;
                        }
                        else
                        {
                        }
                    }
                }
                
                if ((b_idr || b_non_idr ) && b_pps)
                {
                    pt_format->e_media_type                    = MEDIA_TYPE_VIDEO_ES;
                    pt_format->t_media_subtype.u.e_vid_subtype = MEDIA_VID_SUBTYPE_H264;
                    return MFMTRECGR_OK;
                }
                
                if (VID_ES_LOOKUP_LENGTH < (UINT32)(ui8_cur_pos - ui8_pre_file_pos))
                {
                    return MFMTRECGR_INV_ARG;
                }
                
            } while (TRUE);
            
        } while (0);
        
    }while(0);
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _vc1_es_confirm_fct
 *
 * Description: this function confirm VC-1 video es
 *
 * Inputs:  pt_input       specify input stream
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 * Author : Lianming Lin(mtk40234)
 *
 * History:
 *   (1)2009-07-08 : initial
 ----------------------------------------------------------------------------*/
INT32 _vc1_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format)     
{

    UINT64                ui8_cur_pos;
    UINT64                ui8_pre_file_pos;
    INT32                 i4_ret;
    UINT32                ui4_code;
    
    BOOL                  b_entry_point;
    BOOL                  b_frame;
        
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_format) || (NULL == pt_input))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    do
    {
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 0, FM_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }
        
        i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_cur_pos);
        if (i4_ret != INPUTR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }
        
        do 
        {
            i4_ret = _es_seek_start_code(pt_input, 
                                         &ui8_cur_pos, 
                                         MPEG_VIDEO_START_CODE_PREFIX, 
                                         &ui4_code);
            if (i4_ret != MFMTRECGR_OK)
            {
                return i4_ret;
            }
            
            if (VC1_SEQUENCE_HEADER_CODE == ui4_code)
            {
                break;
            }
            
        } while (TRUE);
        
        do 
        {
            b_entry_point    = FALSE;
            b_frame          = FALSE;
            
            ui8_pre_file_pos = ui8_cur_pos;
            do 
            {
                i4_ret = _es_seek_start_code(pt_input, 
                                             &ui8_cur_pos, 
                                                MPEG_VIDEO_START_CODE_PREFIX, 
                                                &ui4_code);
                if (i4_ret != MFMTRECGR_OK)
                {
                    return i4_ret;
                }

                if (!b_entry_point)
                {
                    if (VC1_ENTRY_POINT_HEADER_CODE == ui4_code)
                    {
                        b_entry_point = TRUE;
                    }
                }
                else
                {
                    if (!b_frame)
                    {
                        if (VC1_FRAME_HEADER_CODE == ui4_code)
                        {
                            b_frame = TRUE;
                        }
                    }
                }

                /*if a sequence break*/
                if (VC1_SEQUENCE_HEADER_CODE == ui4_code)
                {
                    break;
                }
                
                if (b_entry_point && b_frame)
                {
                    pt_format->e_media_type                    = MEDIA_TYPE_VIDEO_ES;
                    pt_format->t_media_subtype.u.e_vid_subtype = MEDIA_VID_SUBTYPE_VC1;
                    return MFMTRECGR_OK;
                }
                
                if (VID_ES_LOOKUP_LENGTH < (UINT32)(ui8_cur_pos - ui8_pre_file_pos))
                {
                    return MFMTRECGR_INV_ARG;
                }
                
            } while (TRUE);
            
        } while (TRUE);
        
    }while(0);
    
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name:  vid_es_handler_file_confirm_fct
 *
 * Description: this function will ask favor handler 
 *              which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 * Author : Lianming Lin(mtk40234)
 *
 * History:
 *   (1)2009-07-08 : initial
 ----------------------------------------------------------------------------*/
INT32 vid_es_handler_file_confirm_fct(HANDLE_T              h_file,        
                                      MEDIA_FORMAT_T*       pt_format)     
{

#ifndef __NO_FM__
    INT32                   i4_ret;
    VID_ES_FMT_RECG_T*      pt_es_recg;
    MM_INPUT_STREAM_T       t_input_stream;
    MM_UTIL_INPUT_T         t_input_obj;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_format) || (NULL_HANDLE == h_file))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    
    t_input_stream.pf_copybytes          = mm_util_fcopybytes_fct;
    t_input_stream.pf_input1             = mm_util_finput1bytes_fct;
    t_input_stream.pf_input2             = mm_util_finput2bytes_b_fct;
    t_input_stream.pf_input3             = mm_util_finput3bytes_b_fct;
    t_input_stream.pf_input4             = mm_util_finput4bytes_b_fct;
    t_input_stream.pf_set_pos             = mm_util_fsetpos_fct;
    t_input_stream.pf_get_pos             = mm_util_fgetpos_fct;
    
    t_input_obj.h_file                   = h_file;
    t_input_stream.pv_obj = (VOID*)(&t_input_obj);
    
    /*judge if it is known format*/
    i4_ret = _es_chk_unknown_fmt(&t_input_stream);
    if(MFMTRECGR_OK != i4_ret)
    {
        return i4_ret;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_es_recg = &(at_vid_es_recg[0]);
    while (pt_es_recg->t_subtype != MEDIA_VID_SUBTYPE_UNKNOWN)
    {
        if (pt_es_recg->pf_confirm != NULL)
        {
            i4_ret = pt_es_recg->pf_confirm(&t_input_stream, pt_format);
            if (MFMTRECGR_OK == i4_ret)
            {
                return i4_ret;
            }
            else if(i4_ret == MFMTRECGR_FILE_ERR)
            {
                UINT32 ui4_read;
                UINT32 ui4_code;
                i4_ret = t_input_stream.pf_set_pos(t_input_stream.pv_obj, 
                                      (INT64)0,
                                       MEDIA_SEEK_BGN);
                if (i4_ret == FMR_OK)
                {
                    i4_ret = t_input_stream.pf_copybytes(t_input_stream.pv_obj, 
                                                &ui4_code, 
                                                4,
                                                4,
                                                &ui4_read);
                    if (i4_ret != FMR_OK)
                    {
                        return MFMTRECGR_FILE_ERR;
                    }
                }
                else
                {
                    return MFMTRECGR_FILE_ERR;
                }
                i4_ret = t_input_stream.pf_set_pos(t_input_stream.pv_obj, 
                                      (INT64)0,
                                       MEDIA_SEEK_BGN);
            }
        }
        ++pt_es_recg;
    }
    
    return MFMTRECGR_HANDLER_NOT_FOUND;
#else
    return MFMTRECGR_INTERNAL_ERR;
#endif
}
                            
/*-----------------------------------------------------------------------------
 * Name:  vid_es_handler_mem_confirm_fct
 *
 * Description: this function will ask all handler
 *
 * Inputs:  pv_mem_data     specify the address of memory buffer 
 *                          which want to retrieve the format.
 *
 *          z_mem_leng      specify the length of memory buffer 
 *                          which want to retrieve the format.
 *
 * Outputs: pt_format       retrieve the format of the media memory buffer.
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 * Author : Lianming Lin(mtk40234)
 *
 * History:
 *   (1)2009-07-08 : initial
 ----------------------------------------------------------------------------*/
INT32 vid_es_handler_mem_confirm_fct(const VOID*        pv_mem_data, 
                                     SIZE_T             z_mem_leng,   
                                     MEDIA_FORMAT_T*    pt_format) 
{
    INT32                   i4_ret;
    VID_ES_FMT_RECG_T*      pt_es_recg;
    
    MEDIA_FEEDER_T*         pt_src_feeder;

    MM_INPUT_STREAM_T       t_input_stream;
    MM_UTIL_INPUT_T         t_input_obj;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_format) || (NULL == pv_mem_data))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }

    if (0 != z_mem_leng)
    {
        t_input_stream.pf_copybytes          = mm_util_mcopybytes_fct;
        t_input_stream.pf_input1             = mm_util_minput1bytes_fct;
        t_input_stream.pf_input2             = mm_util_minput2bytes_b_fct;
        t_input_stream.pf_input3             = mm_util_minput3bytes_b_fct;
        t_input_stream.pf_input4             = mm_util_minput4bytes_b_fct;
        t_input_stream.pf_set_pos            = mm_util_msetpos_fct;
        t_input_stream.pf_get_pos            = mm_util_mgetpos_fct;

        t_input_obj.pv_buff_or_file_path_name = (VOID*)pv_mem_data;
        t_input_obj.z_buff_leng               = z_mem_leng;
        t_input_obj.i8_buff_pos               = 0;
    }
    else
    {
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        t_input_stream.pf_copybytes = pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes;
        t_input_stream.pf_input1    = pt_src_feeder->t_feeder_be_fct_tbl.pf_input1;
        t_input_stream.pf_input2    = pt_src_feeder->t_feeder_be_fct_tbl.pf_input2;
        t_input_stream.pf_input3    = pt_src_feeder->t_feeder_be_fct_tbl.pf_input3;
        t_input_stream.pf_input4    = pt_src_feeder->t_feeder_be_fct_tbl.pf_input4;
        t_input_stream.pf_set_pos   = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos;
        t_input_stream.pf_get_pos   = pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos;

        t_input_obj.h_src_feeder = pt_src_feeder->h_feeder;
    }
    t_input_stream.pv_obj = (VOID*)(&t_input_obj);

    /*judge if it is known format*/
    if(MFMTRECGR_OK != _es_chk_unknown_fmt(&t_input_stream))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_es_recg = &(at_vid_es_recg[0]);
    while (pt_es_recg->t_subtype != MEDIA_VID_SUBTYPE_UNKNOWN)
    {
        if (pt_es_recg->pf_confirm != NULL)
        {
            i4_ret = pt_es_recg->pf_confirm(&t_input_stream, pt_format);
            if (MFMTRECGR_OK == i4_ret)
            {
                return MFMTRECGR_OK;
            }
        }
        ++pt_es_recg;
    }
    
    return MFMTRECGR_HANDLER_NOT_FOUND;
}

