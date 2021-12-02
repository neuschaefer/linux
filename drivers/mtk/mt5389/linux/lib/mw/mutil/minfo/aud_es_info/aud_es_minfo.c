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
 * aud_es_minfo.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * willyhuang
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media info public APIs  
           Author: lianming mcn07123 
           History:
           (1)2008-3-10  lianming mcn07123  initial

 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


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
#include "inc/x_mm_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/minfo_dbg.h"

#include "dbg/x_dbg.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "util/x_lnk_list.h"
#include "dbg/u_dbg.h"
#include "mutil/mm_util_input.h"
#include "mutil/mm_util.h"

#include "drm/x_drm.h"
#include "unicode/x_uc_str.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "x_mm_common.h"

#include "handle.h"
#include "u_handle_grp.h"
#include "../u_minfo.h"
#include "../minfo.h"
#include "x_dbg.h"

#include "../../mm_util.h"
#include "../../minfo/minfo_dbg.h"

#include "x_lnk_list.h"
#include "u_dbg.h"
#include "../../mm_util_input.h"
#include "x_drm.h"
#include "x_uc_str.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define AUD_ES_MINFO_USE_CACHE                     (1)

#define AUD_ES_MINFO_FS_SIZE_THRESHOLD         (10*1024*1024)
#define AUD_ES_MINFO_INIT_BUFF_LEN             (UINT32)10000
#define AUD_ES_MINFO_FRAME_REF_CNT              (1)
#define AUD_ES_MINFO_LOOKUP_LENGTH             (1*1024*1024)

#define AUD_ES_MINFO_AC3_SYNCWORD                     (UINT16)0x0b77
#define AUD_ES_MINFO_DTS_SYNCWORD                     (UINT32)0x7ffe8001  
#define AUD_ES_MINFO_SPEC_AC3_SYNCWORD                (UINT16)0x770b  
#define AUD_ES_MINFO_AC3_FREQ_SAMPLE_CNT              (256*6)
#define AUD_ES_MINFO_AC3_FRM_SIZE_COD_MAX             (38)
#define AUD_ES_MINFO_AC3_FREQ_SAMPLING_MAX            (3)
#define AUD_ES_MINFO_AC3_FRAME_SIZE_OFFSET            (5)
#define AUD_ES_MINFO_DTS_FRAME_SIZE_OFFSET            (9)
#define AUD_ES_MINFO_SPEC_AC3_FRAME_SIZE_OFFSET       (6)

#define AUD_ES_MINFO_AAC_HEAD_LENGTH                  (7)
#define AUD_ES_MINFO_ADIF_ID                         (UINT32)0x41444946
#define AUD_ES_MINFO_AAC_SYNCWORD                    (UINT16)0xfff0
#define AUD_ES_MINFO_ADTS_FREQ_SAMPLING_MAX           (12)
#define AUD_ES_MINFO_ADTS_FREQ_SAMPLE_CNT            (256*4)
#define AUD_ES_MINFO_ADTS_FRAME_SIZE_OFFSET          (6)

#define LITTLE_TO_BIG_WORD(wValue)   (  ((wValue&0x00FF)<<8)|     \
                                        ((wValue&0xFF00)>> 8))
                                        
/*store all parsered infomation */
typedef struct _MINFO_AUD_ES_HANDLER_T
{
    MINFO_TYPE_GENERIC_T              t_generic;
    MINFO_ES_AUDIO_STM_ATRBT_T        t_es_audio_stm_attr;

    /* Current parser offset, file base */
    BOOL                               b_too_big_file;
    UINT64                             ui8_lookup_length;
    UINT64                             ui8_file_size;
    
    
    /*Header buffer size*/
    UINT64                             ui8_cache_pos;
    UINT32                             ui4_buf_length;
    /*store current parsed object buffer*/
    UINT8*                             pui1_cache_buf;

    UINT64                             ui8_cur_parser_offset;
    UINT32                             ui4_read_count;
    
} MINFO_AUD_ES_HANDLER_T;


/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static UINT16 _aud_es_minfo_ac3_frame_size_table[AUD_ES_MINFO_AC3_FRM_SIZE_COD_MAX][AUD_ES_MINFO_AC3_FREQ_SAMPLING_MAX] =
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
static UINT32 _aud_es_minfo_ac3_sampling_freq[4] = {48000, 44100, 32000, 0};
static UINT32 _aud_es_minfo_adts_sampling_freq[16] = 
{
    96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050,
    16000, 12000, 11025, 8000,   0,    0,    0,     0
};

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

static INT32 _aud_es_minfo_getpos(MINFO_OBJ_T*            pt_minfo_obj,
                                  UINT64*                 pui8_offset)
{
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return INPUTR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return INPUTR_INV_ARG;
    }
    
#if (AUD_ES_MINFO_USE_CACHE == 1)
    *pui8_offset = pt_aud_es_hdlr->ui8_cur_parser_offset;
#else
    pt_minfo_obj->t_input_fct_tbl.pf_get_pos(pt_minfo_obj, pui8_offset);
#endif
    return INPUTR_OK;
}

static INT32 _aud_es_minfo_setpos(MINFO_OBJ_T*           pt_minfo_obj,
                                  INT64                  i8_offset, 
                                  UINT8                  ui1_whence)
{
#if (AUD_ES_MINFO_USE_CACHE == 1)    
    INT64                       i8_pos;
    setpos_fct                  pf_set_pos;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return INPUTR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return INPUTR_INV_ARG;
    }
    
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    switch (ui1_whence)
    {
        case MEDIA_SEEK_BGN:
            i8_pos = i8_offset;
            break;
            
        case MEDIA_SEEK_CUR: 
            i8_pos = (INT64)(pt_aud_es_hdlr->ui8_cur_parser_offset);
            i8_pos = i8_pos + i8_offset;
            break;
            
        case MEDIA_SEEK_END:
            i8_pos = (INT64)(pt_aud_es_hdlr->ui8_file_size);
            i8_pos = i8_pos + i8_offset;
            break;
            
        default:
            return INPUTR_INV_ARG;
    }

    if (i8_pos < 0)
    {
        return INPUTR_FAIL;
    }

    if (i8_pos > (INT64)pt_aud_es_hdlr->ui8_file_size)
    {
        return INPUTR_EOF;
    }

    pt_aud_es_hdlr->ui8_cur_parser_offset = (UINT64)i8_pos;

    if (pt_aud_es_hdlr->ui8_cur_parser_offset < pt_aud_es_hdlr->ui8_cache_pos
        || (pt_aud_es_hdlr->ui8_cur_parser_offset + 1 
            > pt_aud_es_hdlr->ui8_cache_pos + pt_aud_es_hdlr->ui4_read_count))
    {
        pt_aud_es_hdlr->ui4_read_count = 0;
        pf_set_pos(pt_minfo_obj, i8_pos, MEDIA_SEEK_BGN);
    }

    return INPUTR_OK;
    
#else
    setpos_fct             pf_set_pos;

    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_set_pos(pt_minfo_obj, i8_offset, ui1_whence);
    return INPUTR_OK;
    
#endif
}

INT32 _aud_es_minfo_input1byte(MINFO_OBJ_T*            pt_minfo_obj,
                               UINT8*                  pui1_data)
{
#if (AUD_ES_MINFO_USE_CACHE == 1)
    UINT64                      ui8_pos;
    copybytes_fct               pf_copybytes;
    setpos_fct                  pf_set_pos;
    INT32                       i4_ret         = INPUTR_OK;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_minfo_obj == NULL) || (pui1_data == NULL))
    {
        return INPUTR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return INPUTR_INV_ARG;
    }
    
    i4_ret       = INPUTR_OK;
    pf_copybytes = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos   = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;

    _aud_es_minfo_getpos(pt_minfo_obj, &ui8_pos);

    if ((ui8_pos + 1 > pt_aud_es_hdlr->ui8_file_size)
         || (ui8_pos + 1 > pt_aud_es_hdlr->ui8_lookup_length))
    {   
        if (ui8_pos + 1 > pt_aud_es_hdlr->ui8_lookup_length)
        {
            pt_aud_es_hdlr->b_too_big_file = TRUE;
        }
        return INPUTR_EOF;
    }

    if ((0 == pt_aud_es_hdlr->ui4_read_count)
        || (pt_aud_es_hdlr->ui8_cur_parser_offset < pt_aud_es_hdlr->ui8_cache_pos)
        || (pt_aud_es_hdlr->ui8_cur_parser_offset + 1 
            > pt_aud_es_hdlr->ui8_cache_pos + pt_aud_es_hdlr->ui4_read_count))
    {
        i4_ret = pf_set_pos(pt_minfo_obj, 
                           (INT64)pt_aud_es_hdlr->ui8_cur_parser_offset, 
                           MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            return i4_ret;
        }
        i4_ret = pf_copybytes(pt_minfo_obj, 
                              pt_aud_es_hdlr->pui1_cache_buf, 
                              pt_aud_es_hdlr->ui4_buf_length, 
                              pt_aud_es_hdlr->ui4_buf_length, 
                              &(pt_aud_es_hdlr->ui4_read_count));
        if ((i4_ret != INPUTR_OK) && (i4_ret != INPUTR_EOF))
        {
            return i4_ret;
        }

        pt_aud_es_hdlr->ui8_cache_pos = pt_aud_es_hdlr->ui8_cur_parser_offset;
    }

    *pui1_data = pt_aud_es_hdlr->pui1_cache_buf[pt_aud_es_hdlr->ui8_cur_parser_offset 
                                                - pt_aud_es_hdlr->ui8_cache_pos];
    ++(pt_aud_es_hdlr->ui8_cur_parser_offset);

    return INPUTR_OK;

#else
    UINT64                      ui8_pos;  
    getpos_fct                  pf_get_pos;
    input1bytes_fct             pf_inp1byte;    
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_minfo_obj == NULL) || (pui1_data == NULL))
    {
        return INPUTR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return INPUTR_INV_ARG;
    }
    
    pf_inp1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pf_get_pos(pt_minfo_obj, &ui8_pos);

    if ((ui8_pos + 1 > pt_aud_es_hdlr->ui8_file_size)
        || (ui8_pos + 1 >= pt_aud_es_hdlr->ui8_lookup_length))
    {
        if (ui8_pos + 1 > pt_aud_es_hdlr->ui8_lookup_length)
        {
            pt_aud_es_hdlr->b_too_big_file = TRUE;
        }
        return INPUTR_EOF;
    }
    *pui1_data = pf_inp1byte(pt_minfo_obj, pui1_data);

    return INPUTR_OK;
    
#endif    
}

static INT32 _aud_es_minfo_input2byte(MINFO_OBJ_T*          pt_minfo_obj,
                                      UINT16*               pui2_data)
{
#if (AUD_ES_MINFO_USE_CACHE == 1)
    UINT8                   ui1_idx;
    UINT8                   ui1_tmp;
    INT32                   i4_ret;

    for (ui1_idx = 0; ui1_idx < 2; ui1_idx++)
    {
        i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp);
        if (i4_ret != INPUTR_OK)
        {
            return i4_ret;
        }
        
        *pui2_data = (((*pui2_data) << 8) | ui1_tmp);
    }

    return i4_ret;

#else    
    UINT8                       ui1_idx;
    UINT8                       ui1_tmp;
    UINT64                      ui8_pos;  
    getpos_fct                  pf_get_pos;
    input1bytes_fct             pf_inp1byte;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;
    INT32                       i4_ret;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_minfo_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return INPUTR_INV_ARG;
    }
    
    pf_inp1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    pf_get_pos(pt_minfo_obj, &ui8_pos);
    
    if ((ui8_pos + 2 > pt_aud_es_hdlr->ui8_file_size)
        || (ui8_pos + 2 > pt_aud_es_hdlr->ui8_lookup_length))
    {
        if (ui8_pos + 2 > pt_aud_es_hdlr->ui8_lookup_length)
        {
            pt_aud_es_hdlr->b_too_big_file = TRUE;
        }
        return INPUTR_EOF;
    }

    *pui2_data = 0;
    for (ui1_idx = 0; ui1_idx < 2; ui1_idx++)
    {
        i4_ret = pf_inp1byte(pt_minfo_obj, &ui1_tmp);
        if (i4_ret != INPUTR_OK)
        {
            return i4_ret;
        }
        *pui2_data = (((*pui2_data) << 8) | ui1_tmp);
    }

    return INPUTR_OK;
#endif
}

/*Not be used now so remark*/
#if 0
static INT32 _aud_es_minfo_input3byte(MINFO_OBJ_T*          pt_minfo_obj,
                                      UINT32*               pui4_data)
{
#if (AUD_ES_MINFO_USE_CACHE == 1)
    UINT8               ui1_idx;
    UINT8               ui1_tmp;
    INT32               i4_ret;

    *pui4_data = 0;
    for (ui1_idx = 0; ui1_idx < 3; ui1_idx++)
    {
        i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp);
        if (i4_ret != INPUTR_OK)
        {
            return i4_ret;
        }
        
        *pui4_data = (((*pui4_data) << 8)|ui1_tmp);
    }

    return i4_ret;

#else
    UINT8                       ui1_idx;
    UINT8                       ui1_tmp;    
    UINT64                      ui8_pos;   
    getpos_fct                  pf_get_pos;
    input1bytes_fct             pf_inp1byte;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;
    INT32                       i4_ret;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_minfo_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return INPUTR_INV_ARG;
    }
    
    pf_inp1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    pf_get_pos(pt_minfo_obj, &ui8_pos);
    
    if ((ui8_pos + 3 > pt_aud_es_hdlr->ui8_file_size)
         || (ui8_pos + 3 > pt_aud_es_hdlr->ui8_lookup_length))
    {
        if (ui8_pos + 3 > pt_aud_es_hdlr->ui8_lookup_length)
        {
            pt_aud_es_hdlr->b_too_big_file = TRUE;
        }
        return INPUTR_EOF;
    }

    *pui4_data = 0;
    for (ui1_idx = 0; ui1_idx < 3; ui1_idx++)
    {
        i4_ret = pf_inp1byte(pt_minfo_obj, &ui1_tmp);
        if (i4_ret != INPUTR_OK)
        {
            return i4_ret;
        }
        *pui4_data = (((*pui4_data) << 8) | ui1_tmp);
    }

    return INPUTR_OK;
#endif
}
#endif

static INT32 _aud_es_minfo_input4byte(MINFO_OBJ_T*          pt_minfo_obj,
                                      UINT32*               pui4_data)
{
#if (AUD_ES_MINFO_USE_CACHE == 1)
    UINT8                    ui1_idx;
    UINT8                    ui1_tmp;
    INT32                    i4_ret;

    for (ui1_idx = 0; ui1_idx < 4; ui1_idx++)
    {
        i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp);
        if (i4_ret != INPUTR_OK)
        {
            return i4_ret;
        }
        
        *pui4_data = (((*pui4_data) << 8) | ui1_tmp);
    }

    return i4_ret;

#else    
    UINT8                       ui1_idx;
    UINT8                       ui1_tmp;     
    UINT64                      ui8_pos;  
    getpos_fct                  pf_get_pos;
    input1bytes_fct             pf_inp1byte;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;
    INT32                       i4_ret;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_minfo_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return INPUTR_INV_ARG;
    }
    
    pf_inp1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    pf_get_pos(pt_minfo_obj, &ui8_pos);
    
    if (ui8_pos + 4 > pt_aud_es_hdlr->ui8_file_size
        || ui8_pos + 4 > pt_aud_es_hdlr->ui8_lookup_length)
    {
        if (ui8_pos + 4 > pt_aud_es_hdlr->ui8_lookup_length)
        {
            pt_aud_es_hdlr->b_too_big_file = TRUE;
        }
        return INPUTR_EOF;
    }

    *pui4_data = 0;
    for (ui1_idx = 0; ui1_idx < 4; ui1_idx++)
    {
        i4_ret = pf_inp1byte(pt_minfo_obj, &ui1_tmp);
        if (i4_ret != INPUTR_OK)
        {
            return i4_ret;
        }
        *pui4_data = (((*pui4_data) << 8) | ui1_tmp);
    }
    
    return INPUTR_OK;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  _aud_es_adif_minfo_parse
 *
 * Description: This function start parsing a audio es file
 *
 * Inputs:  pv_obj       specify the media info object  
 *          ui8_pos      
 *  
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-07-29 : initial
 ----------------------------------------------------------------------------*/
static INT32 _aud_es_adif_minfo_parse(MINFO_OBJ_T*       pt_minfo_obj)
{
    INT32                       i4_ret         = MINFOR_OK;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;
    UINT8                       ui1_tmp_code;
    UINT16                      ui2_tmp_code;
    UINT32                      ui4_frame_cnt = 0; 
    UINT32                      ui4_bitrate;
    UINT32                      ui4_head;
    UINT64                      ui8_offset = 0;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        DBG_ERROR(("_aud_es_adif_minfo_parse: NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    i4_ret = MINFOR_OK;
    do
    {
        i4_ret = _aud_es_minfo_input4byte(pt_minfo_obj, &ui4_head);
        if ((i4_ret != INPUTR_OK) || (ui4_head != AUD_ES_MINFO_ADIF_ID))
        {
            break;
        }
    
        i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        if ((ui1_tmp_code & 0x80) >> 7)
        {
            /*copyright id 1 + 8*/
            i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 8, MEDIA_SEEK_CUR);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
        }
        
        ui4_bitrate = ui1_tmp_code & 0x0f;
        i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        ui4_bitrate = ((ui4_bitrate << 16) | ui2_tmp_code);
        i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        ui4_bitrate = ((ui4_bitrate << 8) | ((ui1_tmp_code & 0xE0) >> 5));


        ui4_frame_cnt = 0;
        do
        {
            /*exit at any time*/
            if(pt_minfo_obj->b_abort_flag == TRUE)
            {
                return MINFOR_INTERNAL_ERR;
            }
            DBG_INFO(("_aud_es_adif_minfo_parse: parsing all file !\r\n"));
            
            ui4_frame_cnt ++;
            i4_ret = _aud_es_minfo_getpos(pt_minfo_obj, &ui8_offset);
            if (i4_ret != INPUTR_OK)
            {
                i4_ret = INPUTR_OK;
                break;
            }
            else if (ui8_offset >= AUD_ES_MINFO_FS_SIZE_THRESHOLD)
            {
                break;
            }
            
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            if (ui1_tmp_code < 7)  /*id_syn_ele*/
            {
                i4_ret        = MINFOR_OK;
                break;
            }
            
            if (ui4_frame_cnt > 7 && pt_minfo_obj->b_filesize_non_gettable)
            {
                break;
            }
        } while (TRUE);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
        
        /*calculate bitrate*/
        if (ui4_bitrate != 0)
        {
            pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64(pt_minfo_obj->z_buff_leng 
                                          - pt_aud_es_hdlr->ui8_cur_parser_offset, 
                                      (UINT64)ui4_bitrate, 
                                      NULL); 
            DBG_INFO(("_aud_es_adif_minfo_parse:"
                      "{Raw data begin position:0x%llx, "
                      "bitrate  = %u,"
                      "duration = %llu}\r\n", 
                      pt_aud_es_hdlr->ui8_cur_parser_offset, 
                      ui4_bitrate,
                      pt_aud_es_hdlr->t_generic.ui8_pb_duration));
            pt_aud_es_hdlr->t_generic.ui8_pb_duration *= 90000;
        }
        
        pt_aud_es_hdlr->t_es_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_AAC;
        return MINFOR_OK;
        
    } while (0);
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _aud_es_adts_minfo_parse
 *
 * Description: This function start parsing a audio es file
 *
 * Inputs:  pv_obj       specify the media info object  
 *          ui8_pos      
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-07-29 : initial
 ----------------------------------------------------------------------------*/
static INT32 _aud_es_adts_minfo_parse(MINFO_OBJ_T*         pt_minfo_obj)
{
    INT32                       i4_ret         = MINFOR_OK;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;
    UINT16                      ui2_tmp_code;
    UINT16                      ui2_frame_len;
    UINT32                      ui4_frame_cnt; 
    UINT8                       ui1_tmp_code;
    UINT32                      ui4_sample_freq = 0;
    UINT64                      ui8_offset = 0;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        DBG_ERROR(("_aud_es_aac_minfo_parse: NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    DBG_INFO(("_aud_es_aac_minfo_parse:begin parse adts\r\n"));
    i4_ret = MINFOR_OK;
    do
    {
        i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        
        ui4_frame_cnt = 0;
        /*check frames*/
        do
        {
            /*exit at any time*/
            if(pt_minfo_obj->b_abort_flag == TRUE)
            {
                return MINFOR_INTERNAL_ERR;
            }
            DBG_INFO(("_aud_es_adts_minfo_parse: parsing all file !\r\n"));

            i4_ret = _aud_es_minfo_getpos(pt_minfo_obj, &ui8_offset);
            if (i4_ret != INPUTR_OK)
            {
                i4_ret = INPUTR_OK;
                break;
            }
            else if (ui8_offset >= AUD_ES_MINFO_FS_SIZE_THRESHOLD)
            {
                break;
            }
            
            if (AUD_ES_MINFO_AAC_SYNCWORD != (ui2_tmp_code & 0xfff0))
            {
                i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
                if (i4_ret != INPUTR_OK)
                {
                    if (INPUTR_EOF == i4_ret)
                    {
                        i4_ret = MINFOR_OK;
                    }
                    break;
                }
                continue;
            }
            ++ui4_frame_cnt;

            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            DBG_INFO(("{Pos[0x%llx], Frame[%u], Head:%2x %2x %2x ", 
                      pt_aud_es_hdlr->ui8_cur_parser_offset - (UINT64)3, 
                      ui4_frame_cnt,
                      (ui2_tmp_code & 0xFF00) >> 8, 
                      ui2_tmp_code & 0x00FF,
                      ui1_tmp_code));
            
            if (AUD_ES_MINFO_FRAME_REF_CNT == ui4_frame_cnt)
            {
                if (((ui1_tmp_code & 0x3c) >> 2) < AUD_ES_MINFO_ADTS_FREQ_SAMPLING_MAX)
                {
                    ui4_sample_freq = _aud_es_minfo_adts_sampling_freq[(ui1_tmp_code & 0x3c) >> 2];
                }
                DBG_INFO(("sample_freq = %u\r\n", ui4_sample_freq));
            }
            
            /*seek next frame*/
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            ui2_frame_len = ((UINT16)(ui1_tmp_code & 0x03) << 11);
            DBG_INFO(("%2x ", ui1_tmp_code));
            
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            ui2_frame_len |= ((UINT16)(ui1_tmp_code) << 3);
            DBG_INFO(("%2x ", ui1_tmp_code));
            
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            ui2_frame_len |= ((UINT16)(ui1_tmp_code & 0xE0) >> 5);
            DBG_INFO(("%2x, frame_len = %u}\r\n", ui1_tmp_code, ui2_frame_len));
            if (ui2_frame_len <= AUD_ES_MINFO_ADTS_FRAME_SIZE_OFFSET)
            {
                i4_ret = MINFOR_INITIAL_ERR;
                break;
            }

            i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 
                                    (INT64)(ui2_frame_len - AUD_ES_MINFO_ADTS_FRAME_SIZE_OFFSET),
                                    MEDIA_SEEK_CUR);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            
            /*read next frame*/
            i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                if (INPUTR_EOF == i4_ret)
                {
                    i4_ret = MINFOR_OK;
                }
                break;
            }
            if (ui4_frame_cnt > 7 && pt_minfo_obj->b_filesize_non_gettable)
            {
                i4_ret = MINFOR_OK;              
                break;
            }
        } while (TRUE);
        if (MINFOR_OK != i4_ret)
        {
            break;
        }
        
        /*calculate duration*/
        if ((ui4_sample_freq != 0) && !(pt_aud_es_hdlr->b_too_big_file) && !(pt_minfo_obj->b_filesize_non_gettable))
        {
            pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64((UINT64)(AUD_ES_MINFO_ADTS_FREQ_SAMPLE_CNT * ui4_frame_cnt), 
                                      ui4_sample_freq, 
                                      NULL); 
            DBG_INFO(("duration = %llu\r\n", pt_aud_es_hdlr->t_generic.ui8_pb_duration));
            pt_aud_es_hdlr->t_generic.ui8_pb_duration      *= 90000;
            pt_aud_es_hdlr->t_generic.ui4_sz_max_acces_unt  = ui4_frame_cnt;
        }

        pt_aud_es_hdlr->t_es_audio_stm_attr.ui4_sampling_frequency = ui4_sample_freq;
        pt_aud_es_hdlr->t_es_audio_stm_attr.e_enc                  = MINFO_INFO_AUD_ENC_AAC;
        DBG_INFO(("_aud_es_aac_minfo_parse:end parse adts\r\n"));
        return MINFOR_OK;
        
    } while (0);
    if (MINFOR_OK != i4_ret)
    {
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _aud_es_ac3_minfo_parse
 *
 * Description: This function start parsing a audio es file
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-07-29 : initial
 ----------------------------------------------------------------------------*/
static INT32 _aud_es_ac3_minfo_parse(VOID* pv_obj)
{
    INT32                       i4_ret         = MINFOR_OK;
    MINFO_OBJ_T*                pt_minfo_obj   = NULL;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;
    UINT16                      ui2_tmp_code;
    UINT16                      ui2_frame_len;
    UINT32                      ui4_frame_cnt; 
    UINT8                       ui1_tmp_code;
    UINT32                      ui4_sample_freq = 0;
    UINT32                      ui4_sample_idx;
    UINT32                      ui4_frm_sz_code_idx;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        DBG_ERROR(("_aud_es_ac3_minfo_parse: NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj   = (MINFO_OBJ_T*)pv_obj;
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return MINFOR_INTERNAL_ERR;
    }

    i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 
                                 0, 
                                 MEDIA_SEEK_BGN);
    if (i4_ret != INPUTR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    DBG_INFO(("_aud_es_ac3_minfo_parse:begin parse ac3\r\n"));
    do
    {
        i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        
        /*remove garbage bits*/
        while (AUD_ES_MINFO_AC3_SYNCWORD != ui2_tmp_code)
        {
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            ui2_tmp_code  = ((ui2_tmp_code << 8) | ui1_tmp_code);
        }
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
        
        ui4_frame_cnt = 0;
        
        /*check frames*/
        do
        {
            if (AUD_ES_MINFO_AC3_SYNCWORD != ui2_tmp_code)
            {
                i4_ret = MINFOR_FCT_NOT_SUPPORT;
                break;
            }
            ++ui4_frame_cnt;

            /*ignore crc*/
            i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, (INT64)2, MEDIA_SEEK_CUR);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            
            /*get sampling frequency*/
            ui4_sample_idx      = ((ui1_tmp_code & 0xc0) >> 6);
            ui4_frm_sz_code_idx = (ui1_tmp_code & 0x3f);
            
            if ((ui4_sample_idx >= AUD_ES_MINFO_AC3_FREQ_SAMPLING_MAX)
                || 
                (ui4_frm_sz_code_idx >= AUD_ES_MINFO_AC3_FRM_SIZE_COD_MAX))
            {
                i4_ret = MINFOR_INTERNAL_ERR;
                break;
            }
            
            if (AUD_ES_MINFO_FRAME_REF_CNT == ui4_frame_cnt)
            {
                ui4_sample_freq = _aud_es_minfo_ac3_sampling_freq[ui4_sample_idx];
                
                DBG_INFO(("sample_freq = %u\r\n", ui4_sample_freq));
            }
            
            ui2_frame_len = (UINT16)((_aud_es_minfo_ac3_frame_size_table
                                    [ui4_frm_sz_code_idx][ui4_sample_idx]) * 2);
            DBG_INFO(("{Pos[0x%llx], Frame[%u], Head:%2x %u}\r\n", 
                      pt_aud_es_hdlr->ui8_cur_parser_offset - (UINT64)5, 
                      ui4_frame_cnt,
                      ui1_tmp_code, 
                      ui2_frame_len));

            i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 
                                         (INT64)ui2_frame_len  
                                          - (INT64)AUD_ES_MINFO_AC3_FRAME_SIZE_OFFSET, 
                                         MEDIA_SEEK_CUR);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }

            /*next frame*/
            i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                if (INPUTR_EOF == i4_ret)
                {
                    i4_ret = MINFOR_OK;
                }
                break;
            }
            
            if (pt_minfo_obj->b_filesize_non_gettable && ui4_frame_cnt > 7)
            {
                break;
            }
        } while (TRUE);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }

        /*calculate duration*/
        if ((ui4_sample_freq != 0) && !(pt_aud_es_hdlr->b_too_big_file) && !(pt_minfo_obj->b_filesize_non_gettable))
        {
            pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64((UINT64)(AUD_ES_MINFO_AC3_FREQ_SAMPLE_CNT * ui4_frame_cnt), 
                                      ui4_sample_freq, 
                                      NULL); 
            DBG_INFO(("duration = %llu\r\n", pt_aud_es_hdlr->t_generic.ui8_pb_duration));
            pt_aud_es_hdlr->t_generic.ui8_pb_duration      *= 90000;
            pt_aud_es_hdlr->t_generic.ui4_sz_max_acces_unt  = ui4_frame_cnt;
        }
        
        pt_aud_es_hdlr->t_es_audio_stm_attr.ui4_sampling_frequency = ui4_sample_freq;
        pt_aud_es_hdlr->t_es_audio_stm_attr.e_enc                  = MINFO_INFO_AUD_ENC_AC3;

        pt_aud_es_hdlr->t_es_audio_stm_attr.b_big_endian = FALSE;
        DBG_INFO(("_aud_es_ac3_minfo_parse:end parse ac3\r\n"));
        return MINFOR_OK;
        
    } while(0);
    if (MINFOR_OK != i4_ret)
    {
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _aud_es_dts_minfo_parse
 *
 * Description: This function start parsing a audio es file
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : min bao(mtk40384)
 *
 * History:
 *   (1)2011-06-14 : initial
 ----------------------------------------------------------------------------*/
static INT32 _aud_es_dts_minfo_parse(VOID* pv_obj)
{
    INT32                       i4_ret         = MINFOR_OK;
    MINFO_OBJ_T*                pt_minfo_obj   = NULL;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;
    UINT8                       ui1_tmp_code = 0;
    UINT32                      ui4_tmp_code = 0;
    UINT32                      ui4_frame_len = 0;
    UINT32                      ui4_frame_cnt = 0; 
    UINT16                      ui2_nblk = 0;
    UINT16                      ui2_tmp1 = 0;
    UINT16                      ui2_tmp2 = 0;
    UINT32                      ui4_sample_freq = 0;
    UINT32                      ui4_sample_idx = 0;
    UINT32                      ui4_sample_num = 0;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        DBG_ERROR(("_aud_es_dts_minfo_parse: NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj   = (MINFO_OBJ_T*)pv_obj;
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    DBG_INFO(("_aud_es_dts_minfo_parse:begin parse ac3\r\n"));
    i4_ret = MINFOR_OK;
    do
    {
        i4_ret = _aud_es_minfo_input4byte(pt_minfo_obj, &ui4_tmp_code);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        
        /*remove garbage bits*/
        while (AUD_ES_MINFO_DTS_SYNCWORD != ui4_tmp_code)
        {
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            ui4_tmp_code  = ((ui4_tmp_code << 8) | ui1_tmp_code);
        }
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
        
        ui4_frame_cnt = 0;
        
        /*check frames*/
        do
        {
            if (AUD_ES_MINFO_DTS_SYNCWORD != ui4_tmp_code)
            {
                i4_ret = MINFOR_FCT_NOT_SUPPORT;
                break;
            }
            ++ui4_frame_cnt;

            /*ignore crc*/
            i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp1);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }

            ui2_nblk = (ui2_tmp1 & 0x01fc) >> 2;
            /*get sampling num*/
            ui4_sample_num      += (ui2_nblk + 1)*32;
            
            i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp2);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            
            ui4_frame_len = ui2_tmp1 << 16 | ui2_tmp2;
            ui4_frame_len = ((ui4_frame_len & 0x0003fff0) >> 4) + 1;
            
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            if (AUD_ES_MINFO_FRAME_REF_CNT == ui4_frame_cnt)
            {
                ui4_sample_idx = (ui1_tmp_code & 0x0000003c) >> 2;    
                switch (ui4_sample_idx)
                {
                    case 1:
                        ui4_sample_freq = 8000;
                        break;
                    case 2:
                        ui4_sample_freq = 16000;
                        break;
                    case 3:
                        ui4_sample_freq = 32000;
                        break;
                    case 6:
                        ui4_sample_freq = 11025;
                        break;
                    case 7:
                        ui4_sample_freq = 22050;
                        break;
                    case 8:
                        ui4_sample_freq = 44100;
                        break;
                    case 11:
                        ui4_sample_freq = 12000;
                        break;
                    case 12:
                        ui4_sample_freq = 24000;
                        break;
                    case 13:
                        ui4_sample_freq = 48000;
                        break;
                    default:
                        break;
                }                
                DBG_INFO(("sample_freq = %u\r\n", ui4_sample_freq));
            }
            
            DBG_INFO(("{Pos[0x%llx], Frame[%u], Head:%2x %u}\r\n", 
                      pt_aud_es_hdlr->ui8_cur_parser_offset - (UINT64)5, 
                      ui4_frame_cnt,
                      ui1_tmp_code, 
                      ui4_frame_len));

            i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 
                                         (INT64)ui4_frame_len  
                                          - (INT64)AUD_ES_MINFO_DTS_FRAME_SIZE_OFFSET, 
                                         MEDIA_SEEK_CUR);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }

            /*next frame*/
            i4_ret = _aud_es_minfo_input4byte(pt_minfo_obj, &ui4_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                if (INPUTR_EOF == i4_ret)
                {
                    i4_ret = MINFOR_OK;
                }
                break;
            }
            
            if (pt_minfo_obj->b_filesize_non_gettable && ui4_frame_cnt > 7)
            {
                break;
            }
            
        } while (TRUE);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }

        /*calculate duration*/
        if ((ui4_sample_freq != 0) && !(pt_aud_es_hdlr->b_too_big_file) && !(pt_minfo_obj->b_filesize_non_gettable))
        {
            pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64((UINT64)ui4_sample_num, 
                                      ui4_sample_freq, 
                                      NULL); 
            DBG_INFO(("duration = %llu\r\n", pt_aud_es_hdlr->t_generic.ui8_pb_duration));
            pt_aud_es_hdlr->t_generic.ui8_pb_duration      *= 90000;
            pt_aud_es_hdlr->t_generic.ui4_sz_max_acces_unt  = ui4_frame_cnt;
        }
        
        pt_aud_es_hdlr->t_es_audio_stm_attr.ui4_sampling_frequency = ui4_sample_freq;
        pt_aud_es_hdlr->t_es_audio_stm_attr.e_enc                  = MINFO_INFO_AUD_ENC_DTS;

        pt_aud_es_hdlr->t_es_audio_stm_attr.b_big_endian = FALSE;
        DBG_INFO(("_aud_es_dts_minfo_parse:end parse ac3\r\n"));
        return MINFOR_OK;
        
    } while(0);
    if (MINFOR_OK != i4_ret)
    {
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _aud_es_ec3_minfo_parse
 *
 * Description: This function start parsing a audio es file
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : Min Bao(mtk40384)
 *
 * History:
 *   (1)2010-12-8 : initial
 ----------------------------------------------------------------------------*/
static INT32 _aud_es_ec3_minfo_parse(VOID* pv_obj)
{
    INT32                       i4_ret         = MINFOR_OK;
    MINFO_OBJ_T*                pt_minfo_obj   = NULL;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;
    UINT16                      ui2_tmp_code;
    UINT16                      ui2_frame_len;
    UINT32                      ui4_frame_cnt; 
    UINT8                       ui1_tmp_code;
    UINT8                       ui1_tmp;
    UINT32                      ui4_sample_freq = 0;
    UINT32                      ui4_sample_idx;
   /* UINT32                      ui4_frm_sz_code_idx;*/
    UINT8                       ui1_num_blk_scod;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        DBG_ERROR(("_aud_es_ec3_minfo_parse: NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj   = (MINFO_OBJ_T*)pv_obj;
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return MINFOR_INTERNAL_ERR;
    }

    i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 
                                 0, 
                                 MEDIA_SEEK_BGN);
    if (i4_ret != INPUTR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    DBG_INFO(("_aud_es_ec3_minfo_parse:begin parse ac3\r\n"));
    i4_ret = MINFOR_OK;
    do
    {
        i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        
        /*remove garbage bits*/
        while (AUD_ES_MINFO_AC3_SYNCWORD != ui2_tmp_code)
        {
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            ui2_tmp_code  = ((ui2_tmp_code << 8) | ui1_tmp_code);
        }
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
        
        ui4_frame_cnt = 0;
        
        /*check frames*/
        do
        {
            if (AUD_ES_MINFO_AC3_SYNCWORD != ui2_tmp_code)
            {
                i4_ret = MINFOR_FCT_NOT_SUPPORT;
                break;
            }
            

            i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            /* I0 ----Shawn Hsiao provide*/
            ui1_tmp = (UINT8)((ui2_tmp_code & 0xF800) >> 11);
            if (ui1_tmp == 0)
            {
                ++ui4_frame_cnt;
            }

            /* fscod */
            ui2_tmp_code = ui2_tmp_code & 0x07FF;
            ui2_frame_len = (UINT16)((ui2_tmp_code + 1)*2);

            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            ui1_tmp = ((ui1_tmp_code & 0xc0) >> 6);
            if (ui1_tmp == 3)
            {
                ui4_sample_idx = ((ui1_tmp_code & 0x30) >> 4);
                ui1_num_blk_scod = 3;
            }
            else
            {
                ui4_sample_idx = ((ui1_tmp_code & 0xc0) >> 6);
                ui1_num_blk_scod = ((ui1_tmp_code & 0x30) >> 4);
            }
                
            /*get sampling frequency*/
            if (ui4_sample_idx >= AUD_ES_MINFO_AC3_FREQ_SAMPLING_MAX)
            {
                i4_ret = MINFOR_INTERNAL_ERR;
                break;
            }
            
            if (AUD_ES_MINFO_FRAME_REF_CNT == ui4_frame_cnt)
            {
                ui4_sample_freq = _aud_es_minfo_ac3_sampling_freq[ui4_sample_idx];
                
                DBG_INFO(("sample_freq = %u\r\n", ui4_sample_freq));
            }
            
            DBG_INFO(("{Pos[0x%llx], Frame[%u], Head:%2x %u}\r\n", 
                      pt_aud_es_hdlr->ui8_cur_parser_offset - (UINT64)5, 
                      ui4_frame_cnt,
                      ui1_tmp_code, 
                      ui2_frame_len));

            i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 
                                         (INT64)ui2_frame_len  
                                          - (INT64)AUD_ES_MINFO_AC3_FRAME_SIZE_OFFSET, 
                                         MEDIA_SEEK_CUR);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }

            /*next frame*/
            i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                if (INPUTR_EOF == i4_ret)
                {
                    i4_ret = MINFOR_OK;
                }
                break;
            }
            
            if (pt_minfo_obj->b_filesize_non_gettable && ui4_frame_cnt > 7)
            {
                break;
            }
            
        } while (TRUE);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }

        /*calculate duration*/
        if ((ui4_sample_freq != 0) && !(pt_aud_es_hdlr->b_too_big_file) && !(pt_minfo_obj->b_filesize_non_gettable))
        {
            switch (ui1_num_blk_scod)
            {
                case 0x00:
                    pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64((UINT64)(256 * ui4_frame_cnt), 
                                      ui4_sample_freq, 
                                      NULL); 
                    break;
                case 0x01:
                    pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64((UINT64)(256* 2 * ui4_frame_cnt), 
                                      ui4_sample_freq, 
                                      NULL); 
                    break;
                case 0x2:
                    pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64((UINT64)(256* 3 * ui4_frame_cnt), 
                                      ui4_sample_freq, 
                                      NULL); 
                    break;
                case 0x3:
                    pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64((UINT64)(256 * 6 * ui4_frame_cnt), 
                                      ui4_sample_freq, 
                                      NULL); 
                    break;
                default:
                    pt_aud_es_hdlr->t_generic.ui8_pb_duration = 0;
                    break;
            }
            
            DBG_INFO(("duration = %llu\r\n", pt_aud_es_hdlr->t_generic.ui8_pb_duration));
            pt_aud_es_hdlr->t_generic.ui8_pb_duration      *= 90000;
            pt_aud_es_hdlr->t_generic.ui4_sz_max_acces_unt  = ui4_frame_cnt;
        }
        
        pt_aud_es_hdlr->t_es_audio_stm_attr.ui4_sampling_frequency = ui4_sample_freq;
        pt_aud_es_hdlr->t_es_audio_stm_attr.e_enc                  = MINFO_INFO_AUD_ENC_AC3;

        pt_aud_es_hdlr->t_es_audio_stm_attr.b_big_endian = FALSE;
        DBG_INFO(("_aud_es_ec3_minfo_parse:end parse ac3\r\n"));
        return MINFOR_OK;
        
    } while(0);
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _aud_es_spec_ec3_minfo_parse
 *
 * Description: This function start parsing a audio es file
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : Min Bao(mtk40384)
 *
 * History:
 *   (1)2010-12-8 : initial
 ----------------------------------------------------------------------------*/
static INT32 _aud_es_spec_ec3_minfo_parse(VOID* pv_obj)
{
    INT32                       i4_ret         = MINFOR_OK;
    MINFO_OBJ_T*                pt_minfo_obj   = NULL;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;
    UINT16                      ui2_tmp_code;
    UINT16                      ui2_frame_len;
    UINT32                      ui4_frame_cnt; 
    UINT8                       ui1_tmp_code;
    UINT8                       ui1_tmp;
    UINT32                      ui4_sample_freq = 0;
    UINT32                      ui4_sample_idx;
   /* UINT32                      ui4_frm_sz_code_idx;*/
    UINT8                       ui1_num_blk_scod;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        DBG_ERROR(("_aud_es_ec3_minfo_parse: NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj   = (MINFO_OBJ_T*)pv_obj;
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return MINFOR_INTERNAL_ERR;
    }

    i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 
                                 0, 
                                 MEDIA_SEEK_BGN);
    if (i4_ret != INPUTR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    DBG_INFO(("_aud_es_ec3_minfo_parse:begin parse ac3\r\n"));
    i4_ret = MINFOR_OK;
    do
    {
        i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        
        /*remove garbage bits*/
        while (AUD_ES_MINFO_SPEC_AC3_SYNCWORD != ui2_tmp_code)
        {
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            ui2_tmp_code  = ((ui2_tmp_code << 8) | ui1_tmp_code);
        }
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
        
        ui4_frame_cnt = 0;
        
        /*check frames*/
        do
        {
            if (AUD_ES_MINFO_SPEC_AC3_SYNCWORD != ui2_tmp_code)
            {
                i4_ret = MINFOR_FCT_NOT_SUPPORT;
                break;
            }
            

            i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            /* I0 ----Shawn Hsiao provide*/
            ui2_tmp_code = LITTLE_TO_BIG_WORD(ui2_tmp_code);
            ui1_tmp = (UINT8)((ui2_tmp_code & 0xF800) >> 11);
            if (ui1_tmp == 0)
            {
                ++ui4_frame_cnt;
            }

            /* fscod */
            ui2_tmp_code = ui2_tmp_code & 0x07FF;
            ui2_frame_len = (UINT16)((ui2_tmp_code + 1)*2);

            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }

            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            
            ui1_tmp = ((ui1_tmp_code & 0xc0) >> 6);
            if (ui1_tmp == 3)
            {
                ui4_sample_idx = ((ui1_tmp_code & 0x30) >> 4);
                ui1_num_blk_scod = 3;
            }
            else
            {
                ui4_sample_idx = ((ui1_tmp_code & 0xc0) >> 6);
                ui1_num_blk_scod = ((ui1_tmp_code & 0x30) >> 4);
            }
                
            /*get sampling frequency*/
            if (ui4_sample_idx >= AUD_ES_MINFO_AC3_FREQ_SAMPLING_MAX)
            {
                i4_ret = MINFOR_INTERNAL_ERR;
                break;
            }
            
            if (AUD_ES_MINFO_FRAME_REF_CNT == ui4_frame_cnt)
            {
                ui4_sample_freq = _aud_es_minfo_ac3_sampling_freq[ui4_sample_idx];
                
                DBG_INFO(("sample_freq = %u\r\n", ui4_sample_freq));
            }
            
            DBG_INFO(("{Pos[0x%llx], Frame[%u], Head:%2x %u}\r\n", 
                      pt_aud_es_hdlr->ui8_cur_parser_offset - (UINT64)5, 
                      ui4_frame_cnt,
                      ui1_tmp_code, 
                      ui2_frame_len));

            i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 
                                         (INT64)ui2_frame_len  
                                          - (INT64)AUD_ES_MINFO_SPEC_AC3_FRAME_SIZE_OFFSET, 
                                         MEDIA_SEEK_CUR);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }

            /*next frame*/
            i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                if (INPUTR_EOF == i4_ret)
                {
                    i4_ret = MINFOR_OK;
                }
                break;
            }
            
            if (pt_minfo_obj->b_filesize_non_gettable && ui4_frame_cnt > 7)
            {
                break;
            }
            
        } while (TRUE);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }

        /*calculate duration*/
        if ((ui4_sample_freq != 0) && !(pt_aud_es_hdlr->b_too_big_file) && !(pt_minfo_obj->b_filesize_non_gettable))
        {
            switch (ui1_num_blk_scod)
            {
                case 0x00:
                    pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64((UINT64)(256 * ui4_frame_cnt), 
                                      ui4_sample_freq, 
                                      NULL); 
                    break;
                case 0x01:
                    pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64((UINT64)(256* 2 * ui4_frame_cnt), 
                                      ui4_sample_freq, 
                                      NULL); 
                    break;
                case 0x2:
                    pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64((UINT64)(256* 3 * ui4_frame_cnt), 
                                      ui4_sample_freq, 
                                      NULL); 
                    break;
                case 0x3:
                    pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64((UINT64)(256 * 6 * ui4_frame_cnt), 
                                      ui4_sample_freq, 
                                      NULL); 
                    break;
                default:
                    pt_aud_es_hdlr->t_generic.ui8_pb_duration = 0;
                    break;
            }
            
            DBG_INFO(("duration = %llu\r\n", pt_aud_es_hdlr->t_generic.ui8_pb_duration));
            pt_aud_es_hdlr->t_generic.ui8_pb_duration      *= 90000;
            pt_aud_es_hdlr->t_generic.ui4_sz_max_acces_unt  = ui4_frame_cnt;
        }
        
        pt_aud_es_hdlr->t_es_audio_stm_attr.ui4_sampling_frequency = ui4_sample_freq;
        pt_aud_es_hdlr->t_es_audio_stm_attr.e_enc                  = MINFO_INFO_AUD_ENC_AC3;

        pt_aud_es_hdlr->t_es_audio_stm_attr.b_big_endian = TRUE;
        DBG_INFO(("_aud_es_ec3_minfo_parse:end parse ac3\r\n"));
        return MINFOR_OK;
        
    } while(0);
    
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name:  _aud_es_spec_ac3_minfo_parse
 *
 * Description: This function start parsing a audio es file
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-07-29 : initial
 ----------------------------------------------------------------------------*/
static INT32 _aud_es_spec_ac3_minfo_parse(VOID* pv_obj)
{
    INT32                       i4_ret         = MINFOR_OK;
    MINFO_OBJ_T*                pt_minfo_obj   = NULL;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;
    UINT16                      ui2_tmp_code;
    UINT16                      ui2_frame_len;
    UINT32                      ui4_frame_cnt; 
    UINT8                       ui1_tmp_code;
    UINT32                      ui4_sample_freq = 0;
    UINT32                      ui4_sample_idx;
    UINT32                      ui4_frm_sz_code_idx;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        DBG_ERROR(("_aud_es_ac3_minfo_parse: NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj   = (MINFO_OBJ_T*)pv_obj;
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return MINFOR_INTERNAL_ERR;
    }

    i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 
                                 0, 
                                 MEDIA_SEEK_BGN);
    if (i4_ret != INPUTR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    DBG_INFO(("_aud_es_ac3_minfo_parse:begin parse ac3\r\n"));
    i4_ret = MINFOR_OK;
    do
    {
        i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        
        /*remove garbage bits*/
        while (AUD_ES_MINFO_SPEC_AC3_SYNCWORD != ui2_tmp_code)
        {
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            ui2_tmp_code  = ((ui2_tmp_code << 8) | ui1_tmp_code);
        }
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
        
        ui4_frame_cnt = 0;
        
        /*check frames*/
        do
        {
            if (AUD_ES_MINFO_SPEC_AC3_SYNCWORD != ui2_tmp_code)
            {
                i4_ret = MINFOR_FCT_NOT_SUPPORT;
                break;
            }
            ++ui4_frame_cnt;

            /*ignore crc*/
            i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, (INT64)2, MEDIA_SEEK_CUR);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            
            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }

            i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }
            
            /*get sampling frequency*/
            ui4_sample_idx      = ((ui1_tmp_code & 0xc0) >> 6);
            ui4_frm_sz_code_idx = (ui1_tmp_code & 0x3f);
            
            if ((ui4_sample_idx >= AUD_ES_MINFO_AC3_FREQ_SAMPLING_MAX)
                || 
                (ui4_frm_sz_code_idx >= AUD_ES_MINFO_AC3_FRM_SIZE_COD_MAX))
            {
                i4_ret = MINFOR_INTERNAL_ERR;
                break;
            }
            
            if (AUD_ES_MINFO_FRAME_REF_CNT == ui4_frame_cnt)
            {
                ui4_sample_freq = _aud_es_minfo_ac3_sampling_freq[ui4_sample_idx];
                
                DBG_INFO(("sample_freq = %u\r\n", ui4_sample_freq));
            }
            
            ui2_frame_len = (UINT16)((_aud_es_minfo_ac3_frame_size_table
                                    [ui4_frm_sz_code_idx][ui4_sample_idx]) * 2);
            DBG_INFO(("{Pos[0x%llx], Frame[%u], Head:%2x %u}\r\n", 
                      pt_aud_es_hdlr->ui8_cur_parser_offset - (UINT64)5, 
                      ui4_frame_cnt,
                      ui1_tmp_code, 
                      ui2_frame_len));

            i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 
                                         (INT64)ui2_frame_len  
                                          - (INT64)AUD_ES_MINFO_SPEC_AC3_FRAME_SIZE_OFFSET, 
                                         MEDIA_SEEK_CUR);
            if (i4_ret != INPUTR_OK)
            {
                break;
            }

            /*next frame*/
            i4_ret = _aud_es_minfo_input2byte(pt_minfo_obj, &ui2_tmp_code);
            if (i4_ret != INPUTR_OK)
            {
                if (INPUTR_EOF == i4_ret)
                {
                    i4_ret = MINFOR_OK;
                }
                break;
            }
            
            if (pt_minfo_obj->b_filesize_non_gettable && ui4_frame_cnt > 7)
            {
                break;
            }
            
        } while (TRUE);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }

        /*calculate duration*/
        if ((ui4_sample_freq != 0) && !(pt_aud_es_hdlr->b_too_big_file) && !(pt_minfo_obj->b_filesize_non_gettable))
        {
            pt_aud_es_hdlr->t_generic.ui8_pb_duration
                          = _mm_div64((UINT64)(AUD_ES_MINFO_AC3_FREQ_SAMPLE_CNT * ui4_frame_cnt), 
                                      ui4_sample_freq, 
                                      NULL); 
            DBG_INFO(("duration = %llu\r\n", pt_aud_es_hdlr->t_generic.ui8_pb_duration));
            pt_aud_es_hdlr->t_generic.ui8_pb_duration      *= 90000;
            pt_aud_es_hdlr->t_generic.ui4_sz_max_acces_unt  = ui4_frame_cnt;
        }
        
        pt_aud_es_hdlr->t_es_audio_stm_attr.ui4_sampling_frequency = ui4_sample_freq;
        pt_aud_es_hdlr->t_es_audio_stm_attr.e_enc                  = MINFO_INFO_AUD_ENC_AC3;

        pt_aud_es_hdlr->t_es_audio_stm_attr.b_big_endian = TRUE;
        DBG_INFO(("_aud_es_ac3_minfo_parse:end parse ac3\r\n"));
        return MINFOR_OK;
        
    } while(0);
    if (MINFOR_OK != i4_ret)
    {
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  aud_es_aac_minfo_parse
 *
 * Description: This function start parsing a audio es file
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation
 *                       has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-07-29 : initial
 ----------------------------------------------------------------------------*/
static INT32 _aud_es_aac_minfo_parse(VOID* pv_obj)
{
    INT32                       i4_ret         = MINFOR_OK;
    MINFO_OBJ_T*                pt_minfo_obj   = NULL;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;
    UINT32                      ui4_head_code;
    UINT8                       ui1_temp;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        DBG_ERROR(("_aud_es_aac_minfo_parse: NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj   = (MINFO_OBJ_T*)pv_obj;
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (!pt_aud_es_hdlr)
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    i4_ret         = MINFOR_OK;
    do
    {
        i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, (INT64)0, MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }

        i4_ret = _aud_es_minfo_input4byte(pt_minfo_obj, &ui4_head_code);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        
        do 
        {
            /*Check if AAC ADIF: audio data interchaneg format*/
            if (AUD_ES_MINFO_ADIF_ID == ui4_head_code)
            {
                i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, (INT64)-4, MEDIA_SEEK_CUR);
                if (i4_ret != INPUTR_OK)
                {
                    break;
                }
                i4_ret = _aud_es_adif_minfo_parse(pt_minfo_obj);
                break;
            }
            else if (AUD_ES_MINFO_AAC_SYNCWORD == (UINT16)((ui4_head_code & 0xfff00000) >> 16))
            {
                i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, (INT64)-4, MEDIA_SEEK_CUR);
                if (i4_ret != INPUTR_OK)
                {
                    break;
                }
                i4_ret = _aud_es_adts_minfo_parse(pt_minfo_obj);
                break;
            }
            else
            {
                i4_ret = _aud_es_minfo_input1byte(pt_minfo_obj, &ui1_temp);
                if (i4_ret != INPUTR_OK)
                {
                    break;
                }

                ui4_head_code = ((ui4_head_code << 8) | (UINT32)ui1_temp);
            }
        } while (TRUE);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
        
        return MINFOR_OK;
        
    } while (0);
    
    return i4_ret;

}


/*-----------------------------------------------------------------------------
 * Name:  aud_es_minfo_is_handler
 *
 * Description: This function confirm media format could be handled or not.
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                       has been completed.
 *          Other non-zero value :unsuccessful
 ----------------------------------------------------------------------------*/
INT32 aud_es_minfo_is_handler(VOID* pv_obj, MEDIA_FORMAT_T* pt_format)
{

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == pt_format))
    {
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/    
    if ((MEDIA_TYPE_AUDIO_ES == pt_format->e_media_type) 
        && ((MEDIA_AUD_SUBTYPE_AAC == pt_format->t_media_subtype.u.e_aud_subtype)
            || (MEDIA_AUD_SUBTYPE_AC3 == pt_format->t_media_subtype.u.e_aud_subtype)))
    {
       return MINFOR_OK;
    }
    else
    {
       return MINFOR_FMT_NOT_SUPPORT;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  aud_es_minfo_fopen
 *
 * Description: This function open a handler for a file
 *
 * Inputs:  ps_path_name  specify the path name of the file
 *          pv_obj        specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
                         the operation has been completed.
           other non-zero value :unsuccessful

 * Author : lianming lin(mcnmtk40234)
 *
 * History:
    (1)2009-7-29 : initial

 ----------------------------------------------------------------------------*/
INT32 aud_es_minfo_fopen(CHAR*  ps_path_name,  VOID* pv_obj)
{
    MINFO_OBJ_T*              pt_minfo_obj   = NULL;
    MINFO_AUD_ES_HANDLER_T*   pt_aud_es_hdlr = NULL;

    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) || (ps_path_name == NULL))
    {
        DBG_ERROR(("aud_es_minfo_fopen: NULL\r\n"));
        return MINFOR_INV_ARG;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }

    pt_aud_es_hdlr = _mm_util_mem_alloc(sizeof(MINFO_AUD_ES_HANDLER_T));
    if (NULL == pt_aud_es_hdlr)
    {
        DBG_ERROR(("aud_es_minfo_fopen:pv_handler_obj allocate mem(%d) failed\r\n",
                   sizeof(MINFO_AUD_ES_HANDLER_T)));
        return MINFOR_OUT_OF_MEMORY;
    }
    pt_minfo_obj->pv_handler_obj = pt_aud_es_hdlr;
    x_memset(pt_minfo_obj->pv_handler_obj, 0, sizeof(MINFO_AUD_ES_HANDLER_T));

    /*temp buffer used to store parse data , should be verified later. */
    pt_aud_es_hdlr->pui1_cache_buf = (UINT8*)
                  _mm_util_mem_alloc(sizeof(UINT8) * AUD_ES_MINFO_INIT_BUFF_LEN);
    if (NULL == pt_aud_es_hdlr->pui1_cache_buf)
    {
        DBG_ERROR(("aud_es_minfo_fopen:pui1_header_buf allocate memory failed\r\n"));
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_aud_es_hdlr->pui1_cache_buf, 
             0, 
             sizeof(UINT8) * AUD_ES_MINFO_INIT_BUFF_LEN);
    
    pt_aud_es_hdlr->ui4_buf_length    = sizeof(UINT8) * AUD_ES_MINFO_INIT_BUFF_LEN;
    pt_aud_es_hdlr->ui8_file_size     = pt_minfo_obj->z_buff_leng;
    pt_aud_es_hdlr->ui8_lookup_length = AUD_ES_MINFO_FS_SIZE_THRESHOLD;
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  aud_es_minfo_mopen
 *
 * Description: This function open a handler for a memory buffer
 *
 * Inputs:  pv_mem_data  specify the address of the memory buffer
 *          z_mem_leng   specify the length of the buffer
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                       has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-7-29 : initial

 ----------------------------------------------------------------------------*/
INT32 aud_es_minfo_mopen(VOID*    pv_mem_data, 
                         SIZE_T   z_mem_leng, 
                         VOID*    pv_obj)
{
    MINFO_OBJ_T*              pt_minfo_obj   = NULL;
    INT32                     i4_ret;
    UINT64                    ui8_clip_pos;
    MINFO_AUD_ES_HANDLER_T*   pt_aud_es_hdlr = NULL;
    UINT64                    ui8_size = 0;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) || (pv_mem_data == NULL))
    {
        DBG_ERROR(("aud_es_minfo_mopen:NULL\r\n"));
        return MINFOR_INV_ARG;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;

    /*set input functions again for asf litter endian*/
    if (z_mem_leng != 0)
    {
        pt_minfo_obj->t_input_fct_tbl.pf_copybytes = mm_util_mcopybytes_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input1    = mm_util_minput1bytes_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input2    = mm_util_minput2bytes_b_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input3    = mm_util_minput3bytes_b_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input4    = mm_util_minput4bytes_b_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_set_pos   = mm_util_msetpos_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_get_pos   = mm_util_mgetpos_fct;
    }
    else
    {
        MEDIA_FEEDER_T* pt_src_feeder; 
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        pt_minfo_obj->h_src_feeder = pt_src_feeder->h_feeder;
        pt_minfo_obj->t_input_fct_tbl.pf_copybytes = 
               pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes;
        pt_minfo_obj->t_input_fct_tbl.pf_input1    = 
               pt_src_feeder->t_feeder_le_fct_tbl.pf_input1;
        pt_minfo_obj->t_input_fct_tbl.pf_input2    = 
               pt_src_feeder->t_feeder_le_fct_tbl.pf_input2;
        pt_minfo_obj->t_input_fct_tbl.pf_input3    = 
               pt_src_feeder->t_feeder_le_fct_tbl.pf_input3;
        pt_minfo_obj->t_input_fct_tbl.pf_input4    = 
               pt_src_feeder->t_feeder_le_fct_tbl.pf_input4;
        pt_minfo_obj->t_input_fct_tbl.pf_set_pos   = 
               pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos;
        pt_minfo_obj->t_input_fct_tbl.pf_get_pos   = 
               pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos;

        /*To get clip size*/
        i4_ret = pt_minfo_obj->t_input_fct_tbl.pf_set_pos(pt_src_feeder, 
                                                          0, 
                                                          MEDIA_SEEK_END);
        if (MINFOR_OK != i4_ret)
        {
            DBG_ERROR(("aud_es_minfo_mopen: set error\r\n"));
            return MINFOR_INITIAL_ERR;
        }
        /*get the end position*/
        pt_minfo_obj->t_input_fct_tbl.pf_get_pos(pt_src_feeder, &ui8_clip_pos);
        pt_minfo_obj->z_buff_leng = (SIZE_T)ui8_clip_pos;
        
    }

    /*allocate asf handler memory*/
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }

    pt_aud_es_hdlr = _mm_util_mem_alloc(sizeof(MINFO_AUD_ES_HANDLER_T));
    if (NULL == pt_aud_es_hdlr)
    {
        DBG_ERROR(("aud_es_minfo_mopen:pv_handler_obj allo memory(%d) failed\r\n",
                   sizeof(MINFO_AUD_ES_HANDLER_T)));
        return MINFOR_OUT_OF_MEMORY;
    }
    pt_minfo_obj->pv_handler_obj = pt_aud_es_hdlr;
    x_memset(pt_minfo_obj->pv_handler_obj, 0, sizeof(MINFO_AUD_ES_HANDLER_T));

    /*temp buffer used to store parse data , should be verified later. */
    pt_aud_es_hdlr->pui1_cache_buf = (UINT8*)
                  _mm_util_mem_alloc(sizeof(UINT8) * AUD_ES_MINFO_INIT_BUFF_LEN);
    if (NULL == pt_aud_es_hdlr->pui1_cache_buf)
    {
        DBG_ERROR(("aud_es_minfo_mopen:pui1_header_buf allocate memory failed\r\n"));
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_aud_es_hdlr->pui1_cache_buf, 
             0, 
             sizeof(UINT8) * AUD_ES_MINFO_INIT_BUFF_LEN);
    
    pt_aud_es_hdlr->ui4_buf_length    = sizeof(UINT8) * AUD_ES_MINFO_INIT_BUFF_LEN;
    pt_aud_es_hdlr->ui8_file_size     = pt_minfo_obj->z_buff_leng;
    pt_aud_es_hdlr->ui8_lookup_length = AUD_ES_MINFO_FS_SIZE_THRESHOLD;


    i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 
                                    (INT64)0,
                                    MEDIA_SEEK_END);

    i4_ret = _aud_es_minfo_getpos(pt_minfo_obj, &ui8_size);
#ifndef __KERNEL__
    x_dbg_stmt("\n--------------ui8_size = %llu--------------------\n", ui8_size);
#endif
    if (ui8_size == (UINT64)(-1) || ui8_size == (UINT32)(-1))
    {
        DBG_INFO(("aud es get file size is -1 !\r\n"));
        pt_minfo_obj->b_filesize_non_gettable = TRUE;
    }

    i4_ret = _aud_es_minfo_setpos(pt_minfo_obj, 
                                 0, 
                                 MEDIA_SEEK_BGN);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("aud es set pos to 0 fail!!\r\n"));
    }
    
    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  aud_es_minfo_close
 *
 * Description: This function close a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-07-29: initial
 ----------------------------------------------------------------------------*/
INT32 aud_es_minfo_close(VOID* pv_obj)
{
    MINFO_OBJ_T*                pt_minfo_obj   = NULL;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;  

   
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)  
    {
        DBG_ERROR(("aud_es_minfo_close:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj   = (MINFO_OBJ_T*)pv_obj;
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    if (NULL == pt_aud_es_hdlr)
    {
        DBG_ERROR(("aud_es_minfo_close:pt_aud_es_hdlr is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*free temp buffer*/
    if (pt_aud_es_hdlr->pui1_cache_buf != NULL)
    {
        _mm_util_mem_free(pt_aud_es_hdlr->pui1_cache_buf);
        pt_aud_es_hdlr->pui1_cache_buf = NULL;
    }
    
    /*free object handler*/
    if (pt_aud_es_hdlr)
    {
        _mm_util_mem_free(pt_aud_es_hdlr);
        pt_aud_es_hdlr = NULL;
    }
    
    return MINFOR_OK;
} 



/*-----------------------------------------------------------------------------
 * Name:  aud_es_minfo_parse
 *
 * Description: This function start parsing a audio es file
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-07-29 : initial
 ----------------------------------------------------------------------------*/
INT32 aud_es_minfo_parse(VOID* pv_obj)
{
    INT32                 i4_ret       = MINFOR_OK;
    MINFO_OBJ_T*          pt_minfo_obj = NULL;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        DBG_ERROR(("aud_es_minfo_parse: NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*)pv_obj;
    if (pt_minfo_obj->t_media_format.e_media_type != MEDIA_TYPE_AUDIO_ES)
    {
        DBG_ERROR(("aud_es_minfo_parse: not audio es!\r\n"));
        return MINFOR_INV_ARG;
    }
    
    if (pt_minfo_obj->t_media_format.t_media_subtype.u.e_aud_subtype 
        == MEDIA_AUD_SUBTYPE_AAC)
    {
        i4_ret = _aud_es_aac_minfo_parse(pv_obj);
    }
    else if (pt_minfo_obj->t_media_format.t_media_subtype.u.e_aud_subtype 
             == MEDIA_AUD_SUBTYPE_AC3)
    {
        i4_ret = _aud_es_ac3_minfo_parse(pv_obj);
    }
    else if (pt_minfo_obj->t_media_format.t_media_subtype.u.e_aud_subtype 
             == MEDIA_AUD_SUBTYPE_DTS)
    {
        i4_ret = _aud_es_dts_minfo_parse(pv_obj);
    }
    else
    {
    }
    
    if (pt_minfo_obj->t_media_format.t_media_subtype.u.e_aud_subtype 
             == MEDIA_AUD_SUBTYPE_AC3 && i4_ret != MINFOR_OK)
    {
        i4_ret = _aud_es_ec3_minfo_parse(pv_obj);
    }

    if (pt_minfo_obj->t_media_format.t_media_subtype.u.e_aud_subtype 
             == MEDIA_AUD_SUBTYPE_AC3 && i4_ret != MINFOR_OK)
    {
        i4_ret = _aud_es_spec_ac3_minfo_parse(pv_obj);
    }

    if (pt_minfo_obj->t_media_format.t_media_subtype.u.e_aud_subtype 
             == MEDIA_AUD_SUBTYPE_AC3 && i4_ret != MINFOR_OK)
    {
        i4_ret = _aud_es_spec_ec3_minfo_parse(pv_obj);
    }
    
    return i4_ret;

} 

/*-----------------------------------------------------------------------------
 * Name:  aud_es_minfo_get_num_recs
 *
 * Description:This function get information from a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *          e_info_type  
 *
 * Outputs: pui2_num_recs
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-07-29 : initial
 ----------------------------------------------------------------------------*/
INT32 aud_es_minfo_get_num_recs(VOID*                pv_obj, 
                                MINFO_INFO_TYPE_T    e_info_type, 
                                UINT16*              pui2_num_recs)
{
    MINFO_OBJ_T*                pt_minfo_obj   = NULL;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;  
    INT32                       i4_ret;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj ) || (NULL == pui2_num_recs ))
    {
        DBG_ERROR(("aud_es_minfo_get_num_recs:NULL\r\n"));
        return MINFOR_INV_ARG;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj   = (MINFO_OBJ_T*)pv_obj;
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)pt_minfo_obj->pv_handler_obj;
    
    if (pt_aud_es_hdlr)
    {
        /*whether parser is done? if not can not return info*/
        if (FALSE == pt_minfo_obj->b_parsing_successful)
        {
            DBG_ERROR(("aud_es_minfo_get_num_recs:minfo is not ready\r\n"));
            return MINFOR_INFO_NOT_READY;    
        }
        *pui2_num_recs = 0;
        i4_ret         = MINFOR_OK;
        switch (e_info_type)
        {
             case MINFO_INFO_TYPE_GENERIC:
                 break;                
             case MINFO_INFO_TYPE_VIDEO_ATTR:
                 break;
             case MINFO_INFO_TYPE_AUDIO_ATTR:
                *pui2_num_recs = 1;
                break;
             case MINFO_INFO_TYPE_SP_ATTR:
                break;
             case MINFO_INFO_TYPE_IDX_INFO:
                break;
             case MINFO_INFO_TYPE_META_DATA:
                break;
             default:
                ;                
        }
        return i4_ret;
    }
    else
    {
        return MINFOR_FMT_NOT_SUPPORT;
    }
}
                        

/*-----------------------------------------------------------------------------
 * Name:  aud_es_minfo_get_info
 *
 * Description:This function get information from a handler
 *
 * Inputs:  pv_obj    specify the media info object  
 *           e_info_type
 *          ui2_index
 * Outputs: pv_buf   output media stream infomation
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-07-29 : initial
 ----------------------------------------------------------------------------*/
INT32 aud_es_minfo_get_info(VOID*               pv_obj, 
                            MINFO_INFO_TYPE_T   e_info_type, 
                            MINFO_REC_IDX_T     ui2_index,                    
                            VOID*               pv_buf, 
                            SIZE_T*             pz_buf_leng)
{
    MINFO_OBJ_T*                pt_minfo_obj   = NULL;
    MINFO_INFO_T*               pt_minfo_rec   = NULL;
    MINFO_AUD_ES_HANDLER_T*     pt_aud_es_hdlr = NULL;  
    MINFO_TYPE_GENERIC_T*       pt_generic     = NULL;
    MINFO_TYPE_STM_ATRBT_T*     pt_strm_atrbt  = NULL;
    INT32                       i4_ret;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == pz_buf_leng))
    {
        DBG_ERROR(("aud_es_minfo_get_info:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    pt_aud_es_hdlr = (MINFO_AUD_ES_HANDLER_T*)(pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_aud_es_hdlr)
    {
        DBG_ERROR(("aud_es_minfo_get_info:pt_asf_handler NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    if (NULL == pv_buf)
    {   
        /*get length only*/
        *pz_buf_leng = sizeof (MINFO_INFO_T);
        return MINFOR_OK;
    }
    else
    {
        /* get data */
        if (*pz_buf_leng < sizeof (MINFO_INFO_T))
        {
            DBG_ERROR(("ERROR: aud_es_minfo_get_info,insufficient buffer!\r\n"));  
            return MINFOR_INSUFFICIENT_BUFFER;
        }
    }    
    
    pt_minfo_rec = (MINFO_INFO_T*)pv_buf;
    x_memset(pt_minfo_rec, 0, sizeof(MINFO_INFO_T));
    
    if (NULL != pt_aud_es_hdlr)
    {
        if (FALSE == pt_minfo_obj->b_parsing_successful)
        {
            DBG_ERROR(("aud_es_minfo_get_info:minfo is not ready\r\n"));
            return MINFOR_INFO_NOT_READY;    
        }
        x_memset(pt_minfo_rec, 0, sizeof(MINFO_INFO_T));
        x_memcpy((VOID *)&(pt_minfo_rec->t_format), 
                 (VOID *)&(pt_minfo_obj->t_media_format), 
                 sizeof(MEDIA_FORMAT_T));
        
        pt_minfo_rec->e_info_type = e_info_type;
        pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;

        i4_ret = MINFOR_OK;
        switch (e_info_type)
        {
            case MINFO_INFO_TYPE_GENERIC:
                pt_generic = &(pt_minfo_rec->u.t_general_info);
                x_memcpy(pt_generic, 
                        (VOID*)&(pt_aud_es_hdlr->t_generic),
                         sizeof(MINFO_TYPE_GENERIC_T));
                pt_generic->ui8_media_sz = pt_aud_es_hdlr->ui8_file_size;
                pt_generic->ui1_strm_ns  = 1;
                break;
                

            case MINFO_INFO_TYPE_AUDIO_ATTR:
                if(ui2_index > 1)
                {
                   return MINFOR_OUT_OF_RECORD;
                }
                pt_strm_atrbt = &(pt_minfo_rec->u.t_stm_attribute);
                pt_strm_atrbt->b_is_codec_support = TRUE;
                x_memcpy(&(pt_strm_atrbt->u.t_es_audio_stm_attr),
                         &(pt_aud_es_hdlr->t_es_audio_stm_attr),
                         sizeof(MINFO_ES_AUDIO_STM_ATRBT_T));
                break;
            case MINFO_INFO_TYPE_IDX_INFO:
                pt_minfo_rec->u.t_idx_info.ui4_idx_type    = MM_MINFO_IDX_TYPE_NONE;
                pt_minfo_rec->u.t_idx_info.ui8_data_offset = 0;
                pt_minfo_rec->u.t_idx_info.ui8_data_sz     = pt_aud_es_hdlr->ui8_file_size;
                
                break;

            case MINFO_INFO_TYPE_META_DATA:
                x_memset(&(pt_minfo_rec->u.t_meta_data), 0, sizeof(MINFO_TYPE_META_DATA_T));
                pt_minfo_rec->u.t_meta_data.ui8_pb_duration 
                    = pt_aud_es_hdlr->t_generic.ui8_pb_duration;
                break;
            default:
                i4_ret = MINFOR_INFO_NOT_FOUND;
                break;
        }
        return i4_ret;
    }
    else
    {
        return MINFOR_FMT_NOT_SUPPORT;
    }
} 

#ifdef __cplusplus
}
#endif

