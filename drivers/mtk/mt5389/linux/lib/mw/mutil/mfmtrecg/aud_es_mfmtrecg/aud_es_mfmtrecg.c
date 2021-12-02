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
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains audio es recognizer public APIs  
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

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define AUD_ES_FILE_SEEK_SIZE                       (2*1024)
#define AUD_ES_LOOKUP_LENGTH                        (2*1024*1024)

#define RIFF_HEADER                     0x52494646
#define MP4_HEADER                      0x66747970
#define RIFF_AVI                        0x41565920
#define RIFF_WAVE                       0x57415645

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

#define AAC_HEAD_LENGTH                 (7)

#define ADIF_ID                         (UINT32)0x41444946
#define AAC_SYNCWORD                    (UINT16)0xfff0
#define AC3_SYNCWORD                    (UINT16)0x0b77
#define SPEC_AC3_SYNCWORD               (UINT16)0x770b
#define SPEC_DTS_SYNCWORD               (UINT32)0x7ffe8001
#define AC3_FRM_SIZE_COD_MAX            (38)
#define AC3_FREQ_SAMPLING_MAX           (3)

#define FRAME_REF_CNT                   (8)


#define LITTLE_TO_BIG_DWORD(dwValue)   (((dwValue&0x000000FF)<<24)|     \
                                        ((dwValue&0x0000FF00)<< 8)|     \
                                        ((dwValue&0x00FF0000)>> 8)|     \
                                        ((dwValue&0xFF000000)>>24))

#define AMR_HEAD_LEN                    (6)
#define AWB_HEAD_LEN                    (9)
const static UINT8 au1_amr_header[]   = {0x23, 0x21, 0x41, 0x4D, 0x52, 0x0A};
const static UINT8 au1_awb_header[]   = {0x23, 0x21, 0x41, 0x4D, 0x52, 0x2D, 0x57, 0x42, 0x0A};

/* This function is the comfirm function of a file  */
typedef INT32 (*aud_es_confirm_fct) (MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format);
typedef struct _AUD_ES_FMT_RECG_T
{
    MEDIA_AUD_SUBTYPE_T         t_subtype;
    aud_es_confirm_fct          pf_confirm;
} AUD_ES_FMT_RECG_T;

static UINT16 _ac3_frame_size_table[AC3_FRM_SIZE_COD_MAX][AC3_FREQ_SAMPLING_MAX]=
{
        {64,     69,     96},
        {64,     70,     96},
        {80,     87,     120},
        {80,     88,     120},
        {96,     104,    144},
        {96,     105,    144},
        {112,    121,    168},
        {112,    122,    168},
        {128,    139,    192},
        {128,    140,    192},
        {160,    174,    240},
        {160,    175,    240},
        {192,    208,    288},
        {192,    209,    288},
        {224,    243,    336},
        {224,    244,    336},
        {256,    278,    384},
        {256,    279,    384},
        {320,    348,    480},
        {320,    349,    480},
        {384,    417,    576},
        {384,    418,    576},
        {448,    487,    672},
        {448,    488,    672},
        {512,    557,    768},
        {512,    558,    768},
        {640,    696,    960},
        {640,    697,    960},
        {768,    835,    1152},
        {768,    836,    1152},
        {896,    975,    1344},
        {896,    976,    1344},
        {1024,   1114,   1536},
        {1024,   1115,   1536},
        {1152,   1253,   1728},
        {1152,   1254,   1728},
        {1280,   1393,   1920},
        {1280,   1394,   1920}
};

extern INT32 _aac_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format);
extern INT32 _awm_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format);
extern INT32 _ac3_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format);
extern INT32 _ec3_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format); 
extern INT32 _spec_ac3_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format);
extern INT32 _spec_ec3_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format);
extern INT32 _spec_dts_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format); 

static AUD_ES_FMT_RECG_T        at_aud_es_recg[]  = 
{
    {
        MEDIA_AUD_SUBTYPE_AMR,
        _awm_es_confirm_fct,
    },
    {
         MEDIA_AUD_SUBTYPE_AWB,
        _awm_es_confirm_fct, 
    },
    {
        MEDIA_AUD_SUBTYPE_AAC,      
        _aac_es_confirm_fct,
    },
    {
        MEDIA_AUD_SUBTYPE_AC3,
        _ac3_es_confirm_fct,
    },
    {
        MEDIA_AUD_SUBTYPE_AC3,
        _ec3_es_confirm_fct,
    },
    {
        MEDIA_AUD_SUBTYPE_AC3,
        _spec_ac3_es_confirm_fct,  
    },
    {
        MEDIA_AUD_SUBTYPE_AC3,
        _spec_ec3_es_confirm_fct,  
    },
    {
        MEDIA_AUD_SUBTYPE_DTS,
        _spec_dts_es_confirm_fct, 
    },
    {
        MEDIA_AUD_SUBTYPE_UNKNOWN,
        NULL,
    }
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

/*-----------------------------------------------------------------------------
 * Name:  _aac_es_confirm_fct
 *
 * Description: this function confirm aac video es
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
INT32 _aac_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format)     
{

    UINT64                ui8_cur_pos;
    INT32                 i4_ret;
    UINT8*                  pui1_buf;
    UINT32                ui4_read_count;
    UINT32                  ui4_read_idx;
    UINT32                  ui4_adif_code;
    UINT16                  ui2_tmp_code;
    UINT8                  ui1_tmp;
    UINT32                ui4_frame_cnt;
    UINT16                ui2_frame_len;
    
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
        pui1_buf = (UINT8*)_mm_util_mem_alloc(AUD_ES_FILE_SEEK_SIZE);
        if (NULL == pui1_buf)
        {
            return MFMTRECGR_OUT_OF_MEMORY;
        }
        
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                      (INT64)0,
                                       MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }

        i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_cur_pos);
        if (i4_ret != INPUTR_OK)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        
        ui4_read_idx = 0;
        
        i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                        pui1_buf, 
                                        AUD_ES_FILE_SEEK_SIZE,
                                        AUD_ES_FILE_SEEK_SIZE,
                                        &ui4_read_count);
        if ((i4_ret != INPUTR_OK) || (ui4_read_count < AAC_HEAD_LENGTH))
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        ui4_adif_code = ((UINT32)pui1_buf[0] << 24) | ((UINT32)pui1_buf[1] << 16)
                         | ((UINT32)pui1_buf[2] << 8) | (UINT32)pui1_buf[3];
        
        /*Check if AAC ADIF: audio data interchaneg format*/
        if (ADIF_ID == ui4_adif_code)
        {
            pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
            pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AAC;
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_OK;
        }
        ui2_tmp_code  = pui1_buf[ui4_read_idx++];
        ui2_tmp_code  = ((ui2_tmp_code << 8) | pui1_buf[ui4_read_idx++]);

        ui8_cur_pos  += 2;
        ui4_frame_cnt = 0;
        
        /*remove garbage bits*/
        while (AAC_SYNCWORD != (ui2_tmp_code & 0xfff0))
        {
            if(ui8_cur_pos > AUD_ES_LOOKUP_LENGTH)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            if (ui4_read_idx >= ui4_read_count)
            {
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos,
                                               MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                AUD_ES_FILE_SEEK_SIZE,
                                                AUD_ES_FILE_SEEK_SIZE,
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
            ui8_cur_pos  += 1;
            ui2_tmp_code  = ((ui2_tmp_code << 8) | ui1_tmp);
        }
        
        /*check frames*/
        do
        {
            if (AAC_SYNCWORD != (ui2_tmp_code & 0xfff0))
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            ++ui4_frame_cnt;
            if (FRAME_REF_CNT == ui4_frame_cnt)
            {
                pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
                pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AAC;
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_OK;
            }
        
            if (ui4_read_idx >= ui4_read_count - 2)  /*frame len is around byte 5*/
            {
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)(ui8_cur_pos - 2),
                                               MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                AUD_ES_FILE_SEEK_SIZE,
                                                AAC_HEAD_LENGTH,
                                                &ui4_read_count);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
            
                if(ui4_read_count < AAC_HEAD_LENGTH)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INTERNAL_ERR;
                }
            }
            
            /*next frame*/
            ui2_frame_len = ((UINT16)(pui1_buf[ui4_read_idx+1] & 0x03) << 11) 
                             |((UINT16)(pui1_buf[ui4_read_idx+2]) << 3)
                             | ((UINT16)(pui1_buf[ui4_read_idx+3] & 0xE0) >> 5);
            if (0 == ui2_frame_len)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            ui8_cur_pos += (ui2_frame_len - 2);    /*the first two bytes has be read*/
            if (AUD_ES_LOOKUP_LENGTH < (UINT32)ui8_cur_pos)
            {   
                if (ui4_frame_cnt > 0)
                {
                    pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
                    pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AAC;
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_OK;
                }
                else
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INV_ARG;
                }
            }
        
            i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                          (INT64)ui8_cur_pos,
                                           MEDIA_SEEK_BGN);
            if (i4_ret != INPUTR_OK)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_FILE_ERR;
            }
            ui4_read_idx = 0;
            i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                            pui1_buf, 
                                            AUD_ES_FILE_SEEK_SIZE,
                                            AUD_ES_FILE_SEEK_SIZE,
                                            &ui4_read_count);
            if (i4_ret != INPUTR_OK)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_FILE_ERR;
            }
            
            if(ui4_read_count < 2)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            ui2_tmp_code  = pui1_buf[ui4_read_idx++];
            ui2_tmp_code  = ((ui2_tmp_code << 8) | pui1_buf[ui4_read_idx++]);
            ui8_cur_pos  += 2;
            
        } while (TRUE);
        
    } while(0);
    
}

/*-----------------------------------------------------------------------------
 * Name:  _awm_es_confirm_fct
 *
 * Description: this function confirm amr/awb audio es
 *
 * Inputs:  pt_input       specify input stream
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _awm_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format)     
{

    UINT64                ui8_cur_pos        =  0;
    INT32                 i4_ret             =  MFMTRECGR_INTERNAL_ERR;
    UINT8*                pui1_buf           =  NULL;
    UINT32                ui4_read_count     =  0;
    
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
        pui1_buf = (UINT8*)_mm_util_mem_alloc(AUD_ES_FILE_SEEK_SIZE);
        if (NULL == pui1_buf)
        {
            break;
        }
        
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                      (INT64)0,
                                       MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        
        i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_cur_pos);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        
        i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                        pui1_buf, 
                                        AUD_ES_FILE_SEEK_SIZE,
                                        AUD_ES_FILE_SEEK_SIZE,
                                        &ui4_read_count);
        if ((i4_ret != INPUTR_OK) || (ui4_read_count < 2))
        {
            break;
        }

        if (x_memcmp(pui1_buf, au1_amr_header, AMR_HEAD_LEN) == 0)
        {
            pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
            pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AMR;
            return MFMTRECGR_OK;
        }
        else if (x_memcmp(pui1_buf, au1_awb_header, AWB_HEAD_LEN) == 0)
        {
            pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
            pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AWB;
            return MFMTRECGR_OK;
        }
            
     }while(0);

    if(pui1_buf != NULL)
    {
        _mm_util_mem_free(pui1_buf);
        pui1_buf = NULL;
    }

    return MFMTRECGR_INTERNAL_ERR;
}
 
/*-----------------------------------------------------------------------------
 * Name:  _ac3_es_confirm_fct
 *
 * Description: this function confirm ac3 audio es
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
INT32 _ac3_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format)     
{

    UINT64                ui8_cur_pos;
    INT32                 i4_ret;
    UINT8*                  pui1_buf;
    UINT32                ui4_read_count;
    UINT32                  ui4_read_idx;
    UINT16                  ui2_tmp_code;
    UINT8                  ui1_tmp;
    UINT32                ui4_frame_cnt;
    UINT16                ui2_frame_len;
    
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
        pui1_buf = (UINT8*)_mm_util_mem_alloc(AUD_ES_FILE_SEEK_SIZE);
        if (NULL == pui1_buf)
        {
            return MFMTRECGR_OUT_OF_MEMORY;
        }
        
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                      (INT64)0,
                                       MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        
        i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_cur_pos);
        if (i4_ret != INPUTR_OK)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        
        ui4_read_idx = 0;
        i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                        pui1_buf, 
                                        AUD_ES_FILE_SEEK_SIZE,
                                        AUD_ES_FILE_SEEK_SIZE,
                                        &ui4_read_count);
        if ((i4_ret != INPUTR_OK) || (ui4_read_count < 2))
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        ui2_tmp_code  = pui1_buf[ui4_read_idx++];
        ui2_tmp_code  = ((ui2_tmp_code << 8) | pui1_buf[ui4_read_idx++]);
        ui8_cur_pos += 2;
        
        ui4_frame_cnt = 0;
        
        /*remove garbage bits*/
        while (AC3_SYNCWORD != ui2_tmp_code)
        {
            if(ui8_cur_pos > AUD_ES_LOOKUP_LENGTH)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            if (ui4_read_idx >= ui4_read_count)
            {
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos,
                                               MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                AUD_ES_FILE_SEEK_SIZE,
                                                AUD_ES_FILE_SEEK_SIZE,
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
            ui8_cur_pos  += 1;
            ui2_tmp_code  = ((ui2_tmp_code << 8) | ui1_tmp);
        }
        
        /*check frames*/
        do
        {
            if (AC3_SYNCWORD != ui2_tmp_code)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            ++ui4_frame_cnt;
            if (FRAME_REF_CNT == ui4_frame_cnt)
            {
                pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
                pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AC3;
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_OK;
            }
        
            if (ui4_read_idx + 2  >= ui4_read_count )  
            {
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos,
                                               MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                AUD_ES_FILE_SEEK_SIZE,
                                                3,
                                                &ui4_read_count);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
            
                if(ui4_read_count < 3)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INTERNAL_ERR;
                }
            }
            
            /*next frame*/
            ui1_tmp = pui1_buf[ui4_read_idx + 2];
            if (((ui1_tmp & 0x3f) >= AC3_FRM_SIZE_COD_MAX) 
                || (((ui1_tmp & 0xc0) >> 6) >= AC3_FREQ_SAMPLING_MAX))
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            ui2_frame_len = (UINT16)((_ac3_frame_size_table[ui1_tmp & 0x3f][(ui1_tmp & 0xc0) >> 6]) * 2);
        
            ui8_cur_pos += ui2_frame_len - 2; /*The first two bytes has been read*/
            if (AUD_ES_LOOKUP_LENGTH < (UINT32)ui8_cur_pos)
            {
                if (ui4_frame_cnt > 0)
                {
                    pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
                    pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AC3;
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_OK;
                }
                else
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INV_ARG;
                }
            }
        
            i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                          (INT64)ui8_cur_pos,
                                           MEDIA_SEEK_BGN);
            if (i4_ret != INPUTR_OK)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_FILE_ERR;
            }
            ui4_read_idx = 0;
            i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                            pui1_buf, 
                                            AUD_ES_FILE_SEEK_SIZE,
                                            AUD_ES_FILE_SEEK_SIZE,
                                            &ui4_read_count);
            if (i4_ret != INPUTR_OK)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_FILE_ERR;
            }
            
            if(ui4_read_count < 2)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            ui2_tmp_code  = pui1_buf[ui4_read_idx++];
            ui2_tmp_code  = ((ui2_tmp_code << 8) | pui1_buf[ui4_read_idx++]);
            ui8_cur_pos  += 2;
            
        } while (TRUE);
        
    }while(0);
    
}

/*-----------------------------------------------------------------------------
 * Name:  _ec3_es_confirm_fct
 *
 * Description: this function confirm enhanced ac-3 audio es
 *
 * Inputs:  pt_input       specify input stream
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 * Author : Min Bao(mtk40384)
 *
 * History:
 *   (1)2010-12-08 : initial
 ----------------------------------------------------------------------------*/
INT32 _ec3_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format)     
{
    UINT64                ui8_cur_pos;
    INT32                 i4_ret;
    UINT8*                  pui1_buf;
    UINT32                ui4_read_count;
    UINT32                  ui4_read_idx;
    UINT16                  ui2_tmp_code;
    UINT8                  ui1_tmp;
    UINT32                ui4_frame_cnt;
    UINT16                ui2_frame_len;
    
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
        pui1_buf = (UINT8*)_mm_util_mem_alloc(AUD_ES_FILE_SEEK_SIZE);
        if (NULL == pui1_buf)
        {
            return MFMTRECGR_OUT_OF_MEMORY;
        }
        
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                      (INT64)0,
                                       MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        
        i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_cur_pos);
        if (i4_ret != INPUTR_OK)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        
        ui4_read_idx = 0;
        i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                        pui1_buf, 
                                        AUD_ES_FILE_SEEK_SIZE,
                                        AUD_ES_FILE_SEEK_SIZE,
                                        &ui4_read_count);
        if ((i4_ret != INPUTR_OK) || (ui4_read_count < 2))
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        ui2_tmp_code  = pui1_buf[ui4_read_idx++];
        ui2_tmp_code  = ((ui2_tmp_code << 8) | pui1_buf[ui4_read_idx++]);
        ui8_cur_pos += 2;
        
        ui4_frame_cnt = 0;
        
        /*remove garbage bits*/
        while (AC3_SYNCWORD != ui2_tmp_code)
        {
            if(ui8_cur_pos > AUD_ES_LOOKUP_LENGTH)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            if (ui4_read_idx >= ui4_read_count)
            {
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos,
                                               MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                AUD_ES_FILE_SEEK_SIZE,
                                                AUD_ES_FILE_SEEK_SIZE,
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
            ui8_cur_pos  += 1;
            ui2_tmp_code  = ((ui2_tmp_code << 8) | ui1_tmp);
        }
        
        /*check frames*/
        do
        {
            if (AC3_SYNCWORD != ui2_tmp_code)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            ++ui4_frame_cnt;
            if (FRAME_REF_CNT == ui4_frame_cnt)
            {
                pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
                pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AC3;
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_OK;
            }
        
            if (ui4_read_idx + 2  >= ui4_read_count )  
            {
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos,
                                               MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                AUD_ES_FILE_SEEK_SIZE,
                                                3,
                                                &ui4_read_count);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
            
                if(ui4_read_count < 3)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INTERNAL_ERR;
                }
            }
            
            /*next frame*/
            ui2_tmp_code = (pui1_buf[ui4_read_idx]<<8) | pui1_buf[ui4_read_idx + 1];
            ui2_tmp_code = ui2_tmp_code & 0x07FF;
            ui2_frame_len = (UINT16)((ui2_tmp_code + 1)*2);
            ui8_cur_pos += ui2_frame_len - 2; /*The first two bytes has been read*/
            if (AUD_ES_LOOKUP_LENGTH < (UINT32)ui8_cur_pos)
            {
                if (ui4_frame_cnt > 0)
                {
                    pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
                    pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AC3;
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_OK;
                }
                else
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INV_ARG;
                }
            }
        
            i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                          (INT64)ui8_cur_pos,
                                           MEDIA_SEEK_BGN);
            if (i4_ret != INPUTR_OK)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_FILE_ERR;
            }
            ui4_read_idx = 0;
            i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                            pui1_buf, 
                                            AUD_ES_FILE_SEEK_SIZE,
                                            AUD_ES_FILE_SEEK_SIZE,
                                            &ui4_read_count);
            if (i4_ret != INPUTR_OK)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_FILE_ERR;
            }
            
            if(ui4_read_count < 2)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            ui2_tmp_code  = pui1_buf[ui4_read_idx++];
            ui2_tmp_code  = ((ui2_tmp_code << 8) | pui1_buf[ui4_read_idx++]);
            ui8_cur_pos  += 2;
            
        } while (TRUE);
        
    }while(0);
}

/*-----------------------------------------------------------------------------
 * Name:  _spec_ac3_es_confirm_fct
 *
 * Description: this function confirm ac3 audio es anti byte oreder
 *
 * Inputs:  pt_input       specify input stream
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 * Author : Min Bao(mtk40384)
 *
 * History:
 *   (1)2011-02-26 : initial
 ----------------------------------------------------------------------------*/
INT32 _spec_ac3_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format)     
{

    UINT64                  ui8_cur_pos;
    INT32                   i4_ret;
    UINT8*                  pui1_buf;
    UINT32                  ui4_read_count;
    UINT32                  ui4_read_idx;
    UINT16                  ui2_tmp_code;
    UINT8                   ui1_tmp;
    UINT32                  ui4_frame_cnt;
    UINT16                  ui2_frame_len;
    
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
        pui1_buf = (UINT8*)_mm_util_mem_alloc(AUD_ES_FILE_SEEK_SIZE);
        if (NULL == pui1_buf)
        {
            return MFMTRECGR_OUT_OF_MEMORY;
        }
        
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                      (INT64)0,
                                       MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        
        i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_cur_pos);
        if (i4_ret != INPUTR_OK)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        
        ui4_read_idx = 0;
        i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                        pui1_buf, 
                                        AUD_ES_FILE_SEEK_SIZE,
                                        AUD_ES_FILE_SEEK_SIZE,
                                        &ui4_read_count);
        if ((i4_ret != INPUTR_OK) || (ui4_read_count < 2))
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }

        ui2_tmp_code  = pui1_buf[ui4_read_idx++];
        ui2_tmp_code  = ((ui2_tmp_code << 8) | pui1_buf[ui4_read_idx++]);
        ui8_cur_pos += 2;
        
        ui4_frame_cnt = 0;
        
        /*remove garbage bits*/
        while (SPEC_AC3_SYNCWORD != ui2_tmp_code)
        {
            if(ui8_cur_pos > AUD_ES_LOOKUP_LENGTH)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            if (ui4_read_idx >= ui4_read_count)
            {
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos,
                                               MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                AUD_ES_FILE_SEEK_SIZE,
                                                AUD_ES_FILE_SEEK_SIZE,
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
            ui8_cur_pos  += 1;
            ui2_tmp_code  = ((ui2_tmp_code << 8) | ui1_tmp);
        }
        
        /*check frames*/
        do
        {
            if (SPEC_AC3_SYNCWORD != ui2_tmp_code)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            ++ui4_frame_cnt;
            if (FRAME_REF_CNT == ui4_frame_cnt)
            {
                pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
                pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AC3;
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_OK;
            }
        
            if (ui4_read_idx + 2  >= ui4_read_count )  
            {
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos,
                                               MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                AUD_ES_FILE_SEEK_SIZE,
                                                3,
                                                &ui4_read_count);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
            
                if(ui4_read_count < 3)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INTERNAL_ERR;
                }
            }
            
            /*next frame*/
            ui1_tmp = pui1_buf[ui4_read_idx + 3];
            if (((ui1_tmp & 0x3f) >= AC3_FRM_SIZE_COD_MAX) 
                || (((ui1_tmp & 0xc0) >> 6) >= AC3_FREQ_SAMPLING_MAX))
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            ui2_frame_len = (UINT16)((_ac3_frame_size_table[ui1_tmp & 0x3f][(ui1_tmp & 0xc0) >> 6]) * 2);
        
            ui8_cur_pos += ui2_frame_len - 2; /*The first two bytes has been read*/
            if (AUD_ES_LOOKUP_LENGTH < (UINT32)ui8_cur_pos)
            {
                if (ui4_frame_cnt > 0)
                {
                    pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
                    pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AC3;
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_OK;
                }
                else
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INV_ARG;
                }
            }
        
            i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                          (INT64)ui8_cur_pos,
                                           MEDIA_SEEK_BGN);
            if (i4_ret != INPUTR_OK)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_FILE_ERR;
            }
            ui4_read_idx = 0;
            i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                            pui1_buf, 
                                            AUD_ES_FILE_SEEK_SIZE,
                                            AUD_ES_FILE_SEEK_SIZE,
                                            &ui4_read_count);
            if (i4_ret != INPUTR_OK)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_FILE_ERR;
            }
            
            if(ui4_read_count < 2)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            ui2_tmp_code  = pui1_buf[ui4_read_idx++];
            ui2_tmp_code  = ((ui2_tmp_code << 8) | pui1_buf[ui4_read_idx++]);
            ui8_cur_pos  += 2;
            
        } while (TRUE);
        
    }while(0);
    
}

/*-----------------------------------------------------------------------------
 * Name:  _spec_ec3_es_confirm_fct
 *
 * Description: this function confirm enhanced spec ac-3 audio es
 *
 * Inputs:  pt_input       specify input stream
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 * Author : Min Bao(mtk40384)
 *
 * History:
 *   (1)2010-12-08 : initial
 ----------------------------------------------------------------------------*/
INT32 _spec_ec3_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format)     
{
    UINT64                ui8_cur_pos;
    INT32                 i4_ret;
    UINT8*                  pui1_buf;
    UINT32                ui4_read_count;
    UINT32                  ui4_read_idx;
    UINT16                  ui2_tmp_code;
    UINT8                  ui1_tmp;
    UINT32                ui4_frame_cnt;
    UINT16                ui2_frame_len;
    
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
        pui1_buf = (UINT8*)_mm_util_mem_alloc(AUD_ES_FILE_SEEK_SIZE);
        if (NULL == pui1_buf)
        {
            return MFMTRECGR_OUT_OF_MEMORY;
        }
        
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                      (INT64)0,
                                       MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        
        i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_cur_pos);
        if (i4_ret != INPUTR_OK)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        
        ui4_read_idx = 0;
        i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                        pui1_buf, 
                                        AUD_ES_FILE_SEEK_SIZE,
                                        AUD_ES_FILE_SEEK_SIZE,
                                        &ui4_read_count);
        if ((i4_ret != INPUTR_OK) || (ui4_read_count < 2))
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        ui2_tmp_code  = pui1_buf[ui4_read_idx++];
        ui2_tmp_code  = ((ui2_tmp_code << 8) | pui1_buf[ui4_read_idx++]);
        ui8_cur_pos += 2;
        
        ui4_frame_cnt = 0;
        
        /*remove garbage bits*/
        while (SPEC_AC3_SYNCWORD != ui2_tmp_code)
        {
            if(ui8_cur_pos > AUD_ES_LOOKUP_LENGTH)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            if (ui4_read_idx >= ui4_read_count)
            {
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos,
                                               MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                AUD_ES_FILE_SEEK_SIZE,
                                                AUD_ES_FILE_SEEK_SIZE,
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
            ui8_cur_pos  += 1;
            ui2_tmp_code  = ((ui2_tmp_code << 8) | ui1_tmp);
        }
        
        /*check frames*/
        do
        {
            if (SPEC_AC3_SYNCWORD != ui2_tmp_code)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            ++ui4_frame_cnt;
            if (FRAME_REF_CNT == ui4_frame_cnt)
            {
                pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
                pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AC3;
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_OK;
            }
        
            if (ui4_read_idx + 2  >= ui4_read_count )  
            {
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos,
                                               MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                AUD_ES_FILE_SEEK_SIZE,
                                                3,
                                                &ui4_read_count);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
            
                if(ui4_read_count < 3)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INTERNAL_ERR;
                }
            }
            
            /*next frame*/
            ui2_tmp_code = (pui1_buf[ui4_read_idx + 1]<<8) | pui1_buf[ui4_read_idx];
            ui2_tmp_code = ui2_tmp_code & 0x07FF;
            ui2_frame_len = (UINT16)((ui2_tmp_code + 1)*2);
            ui8_cur_pos += ui2_frame_len - 2; /*The first two bytes has been read*/
            if (AUD_ES_LOOKUP_LENGTH < (UINT32)ui8_cur_pos)
            {
                if (ui4_frame_cnt > 0)
                {
                    pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
                    pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AC3;
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_OK;
                }
                else
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INV_ARG;
                }
            }
        
            i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                          (INT64)ui8_cur_pos,
                                           MEDIA_SEEK_BGN);
            if (i4_ret != INPUTR_OK)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_FILE_ERR;
            }
            ui4_read_idx = 0;
            i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                            pui1_buf, 
                                            AUD_ES_FILE_SEEK_SIZE,
                                            AUD_ES_FILE_SEEK_SIZE,
                                            &ui4_read_count);
            if (i4_ret != INPUTR_OK)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_FILE_ERR;
            }
            
            if(ui4_read_count < 2)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            ui2_tmp_code  = pui1_buf[ui4_read_idx++];
            ui2_tmp_code  = ((ui2_tmp_code << 8) | pui1_buf[ui4_read_idx++]);
            ui8_cur_pos  += 2;
            
        } while (TRUE);
        
    }while(0);
}

/*-----------------------------------------------------------------------------
 * Name:  _spec_dts_es_confirm_fct
 *
 * Description: this function confirm dts audio es
 *
 * Inputs:  pt_input       specify input stream
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 * Author : Min Bao(mtk40384)
 *
 * History:
 *   (1)2011-06-14 : initial
 ----------------------------------------------------------------------------*/
INT32 _spec_dts_es_confirm_fct(MM_INPUT_STREAM_T* pt_input, MEDIA_FORMAT_T* pt_format)     
{
    UINT64                  ui8_cur_pos = 0;
    INT32                   i4_ret = 0;
    UINT8*                  pui1_buf = NULL;    
    UINT32                  ui4_tmp_code = 0;
    UINT32                  ui4_read_count = 0;
    UINT32                  ui4_read_idx = 0;
    UINT32                  ui4_frame_cnt = 0;
    UINT16                  ui2_frame_len = 0;
    
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
        pui1_buf = (UINT8*)_mm_util_mem_alloc(AUD_ES_FILE_SEEK_SIZE);
        if (NULL == pui1_buf)
        {
            return MFMTRECGR_OUT_OF_MEMORY;
        }
        
        i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                      (INT64)0,
                                       MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        
        i4_ret = pt_input->pf_get_pos(pt_input->pv_obj, &ui8_cur_pos);
        if (i4_ret != INPUTR_OK)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        
        ui4_read_idx = 0;
        i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                        pui1_buf, 
                                        AUD_ES_FILE_SEEK_SIZE,
                                        AUD_ES_FILE_SEEK_SIZE,
                                        &ui4_read_count);
        if ((i4_ret != INPUTR_OK) || (ui4_read_count < 4))
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_FILE_ERR;
        }
        x_memcpy(&ui4_tmp_code, pui1_buf, 4);
        
        ui4_tmp_code = LITTLE_TO_BIG_DWORD(ui4_tmp_code);        
        ui8_cur_pos += 4;
        ui4_read_idx += 4;
        ui4_frame_cnt = 0;
        
        /*remove garbage bits*/
        while (SPEC_DTS_SYNCWORD != ui4_tmp_code)
        {
            if(ui8_cur_pos > AUD_ES_LOOKUP_LENGTH)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            if (ui4_read_idx >= ui4_read_count)
            {
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos,
                                               MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                AUD_ES_FILE_SEEK_SIZE,
                                                AUD_ES_FILE_SEEK_SIZE,
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
            
            x_memcpy(&ui4_tmp_code, pui1_buf + ui4_read_idx, 4);
            ui4_tmp_code = LITTLE_TO_BIG_DWORD(ui4_tmp_code);        
            ui8_cur_pos  += 4;
            ui4_read_idx += 4;
        }
        
        /*check frames*/
        do
        {
            if (SPEC_DTS_SYNCWORD != ui4_tmp_code)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            ++ui4_frame_cnt;
            if (FRAME_REF_CNT == ui4_frame_cnt)
            {
                pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
                pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_DTS;
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_OK;
            }
        
            if (ui4_read_idx + 4  >= ui4_read_count )  
            {
                i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                              (INT64)ui8_cur_pos,
                                               MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
                ui4_read_idx = 0;
                i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                                pui1_buf, 
                                                AUD_ES_FILE_SEEK_SIZE,
                                                14,
                                                &ui4_read_count);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }
            
                if(ui4_read_count < 10)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INTERNAL_ERR;
                }
            }
            
            /*next frame*/
            ui2_frame_len = (pui1_buf[ui4_read_idx + 1] & 0x03) << 10 | pui1_buf[ui4_read_idx + 2] << 4 | (pui1_buf[ui4_read_idx + 3] & 0xf0) >> 4;
            ui8_cur_pos += ui2_frame_len - 3; /*The first four bytes has been read*/
            if (AUD_ES_LOOKUP_LENGTH < (UINT32)ui8_cur_pos)
            {
                if (ui4_frame_cnt > 0)
                {
                    pt_format->e_media_type                    = MEDIA_TYPE_AUDIO_ES;
                    pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AC3;
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_OK;
                }
                else
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INV_ARG;
                }
            }
        
            i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 
                                          (INT64)ui8_cur_pos,
                                           MEDIA_SEEK_BGN);
            if (i4_ret != INPUTR_OK)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_FILE_ERR;
            }
            ui4_read_idx = 0;
            i4_ret = pt_input->pf_copybytes(pt_input->pv_obj, 
                                            pui1_buf, 
                                            AUD_ES_FILE_SEEK_SIZE,
                                            AUD_ES_FILE_SEEK_SIZE,
                                            &ui4_read_count);
            if (i4_ret != INPUTR_OK)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_FILE_ERR;
            }
            
            if(ui4_read_count < 4)
            {
                _mm_util_mem_free(pui1_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            x_memcpy(&ui4_tmp_code, pui1_buf, 4);
            ui4_tmp_code = LITTLE_TO_BIG_DWORD(ui4_tmp_code);  
            ui4_read_idx += 4;
            ui8_cur_pos  += 4;
            
        } while (TRUE);
        
    }while(0);
}

/*-----------------------------------------------------------------------------
 * Name:  _es_chk_unknown_fmt
 *
 * Description: this function confirm unknown format
 *
 * Inputs:  pt_input       specify input stream
 *
 * Outputs: none
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 * Author : Min Bao(mtk40384)
 *
 * History:
 *   (1)2011-02-26 : initial
 ----------------------------------------------------------------------------*/

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
           || (ui4_code[0] & CONTAINER_FLV_REV_MASK) == (UINT32)CONTAINER_FLV_HEAD_REV 
           ||  (  ui4_code[0] == (UINT32)CONTAINER_ASF_HEAD_REV1 
               && ui4_code[1] == (UINT32)CONTAINER_ASF_HEAD_REV2
               && ui4_code[2] == (UINT32)CONTAINER_ASF_HEAD_REV3
               && ui4_code[3] == (UINT32)CONTAINER_ASF_HEAD_REV4
               )
           )
        {
            return MFMTRECGR_INTERNAL_ERR;
        }
    }
    else
    {
        return MFMTRECGR_FILE_ERR;
    }
    i4_ret = pt_input->pf_set_pos(pt_input->pv_obj, 0, FM_SEEK_BGN);
    return MFMTRECGR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  aud_es_handler_file_confirm_fct
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
INT32 aud_es_handler_file_confirm_fct(HANDLE_T              h_file,        
                                      MEDIA_FORMAT_T*       pt_format)     
{

#ifndef __NO_FM__
    INT32                   i4_ret;
    AUD_ES_FMT_RECG_T*      pt_es_recg;
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
    t_input_stream.pf_set_pos            = mm_util_fsetpos_fct;
    t_input_stream.pf_get_pos            = mm_util_fgetpos_fct;
    
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
    pt_es_recg = &(at_aud_es_recg[0]);
    while (pt_es_recg->t_subtype != MEDIA_AUD_SUBTYPE_UNKNOWN)
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
 * Name:  aud_es_handler_mem_confirm_fct
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
INT32 aud_es_handler_mem_confirm_fct(const VOID*        pv_mem_data, 
                                     SIZE_T             z_mem_leng,   
                                     MEDIA_FORMAT_T*    pt_format) 
{
    INT32                   i4_ret;
    AUD_ES_FMT_RECG_T*      pt_es_recg;
    
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
        t_input_stream.pf_input1    = pt_src_feeder->t_feeder_le_fct_tbl.pf_input1;
        t_input_stream.pf_input2    = pt_src_feeder->t_feeder_le_fct_tbl.pf_input2;
        t_input_stream.pf_input3    = pt_src_feeder->t_feeder_le_fct_tbl.pf_input3;
        t_input_stream.pf_input4    = pt_src_feeder->t_feeder_le_fct_tbl.pf_input4;
        t_input_stream.pf_set_pos   = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos;
        t_input_stream.pf_get_pos   = pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos;

        t_input_obj.h_src_feeder = pt_src_feeder->h_feeder;
    }
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
    pt_es_recg = &(at_aud_es_recg[0]);
    while (pt_es_recg->t_subtype != MEDIA_AUD_SUBTYPE_UNKNOWN)
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


