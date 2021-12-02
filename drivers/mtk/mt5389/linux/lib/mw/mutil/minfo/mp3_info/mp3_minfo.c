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
 * $RCSfile: mp3_minfo.c,v $
 * $Revision: #3 $ 1.0
 * $Date: 2012/11/20 $ 2004/11/26
 * $Author: cong.wang $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description:
 *         This file contains all the media info public APIs
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
#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/mp3_info/mp3_id3_parser.h"
#include "mutil/minfo/mp3_info/mp3_minfo.h"
#include "mutil/mm_util_input.h"
#include "dbg/x_dbg.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "util/x_lnk_list.h"
#include "mutil/mm_util.h"

#include "inc/x_mm_common.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_minfo.h"
#include "../minfo.h"
#include "mp3_id3_parser.h"
#include "mp3_minfo.h"
#include "../../mm_util_input.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "../../mm_util.h"
#include "x_mm_common.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MINFO_MP3_MAX_PARSING_LEN                                   (1024 * 200)
#define MINFO_MP3_MAX_MIDXBULD_SZ                 ((UINT32)(1024 * 1024 * 10))

const static UINT32 cui4_TAG = 0x00544147;              /* "TAG" */
const static UINT32 cui4_ID3 = 0x00494433;              /* "ID3" */
#define MINFOR_MP3_NO_SIZE        0xFFFFFFFF
#define MINFOR_MP3_NO_SIZE_BY_DLNA 0xFFFFFFFFFFFFFFF

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/* VERSION_1 */
static UINT16 const MPEG_V1_BITRATE_TAB[3][15] =
{
    {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448},     /* Layer I */
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384},        /* Layer II */
    {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320},         /* Layer III */
};

/* VERSION_2, VERSION_2_5 */
static UINT16 const MPEG_V2_BITRATE_TAB[2][15] =
{
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256},        /* Layer I */
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160}              /* Layer II & III */
};

static UINT16 const MPEG_SAMPLERATE_TAB[3] = {44100, 48000, 32000};
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
 /*-----------------------------------------------------------------------------
 * Name:  avi_minfo_is_handler
 *
 * Description: This function confirm media format could be handled or not.
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object
 *
 * Outputs: pv_obj       modify data in the media info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_minfo_is_handler(VOID*    pv_obj, MEDIA_FORMAT_T* pt_format)
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL ||
        pt_format == NULL)
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    if((pt_format->e_media_type == MEDIA_TYPE_AUDIO_ES) &&
    ( pt_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3))
    {
       return MINFOR_OK;
    }
    else
    {
       return MINFOR_FMT_NOT_SUPPORT;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_minfo_fopen
 *
 * Description: This function open a handler for a file
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object
 *
 * Outputs: pv_obj       modify data in the media info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_minfo_fopen(
                        CHAR*              ps_path_name,
                        VOID*              pv_obj)
{
    MINFO_OBJ_T*        pt_minfo_obj;
    ID3_TAG_SET_T*      pt_id3_tag;
    MP3_STREAM_INFO_T*  pt_mp3_stream_info;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (ps_path_name == NULL))
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;

    pt_minfo_obj->t_input_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_minfo_obj->t_input_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_minfo_obj->t_input_fct_tbl.pf_input2 = mm_util_finput2bytes_b_fct;
    pt_minfo_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_b_fct;
    pt_minfo_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_b_fct;
    pt_minfo_obj->t_input_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_minfo_obj->t_input_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;

    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }

    pt_id3_tag = (ID3_TAG_SET_T*)_mm_util_mem_alloc(sizeof(ID3_TAG_SET_T));
    if (pt_id3_tag == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    pt_mp3_stream_info = (MP3_STREAM_INFO_T*)_mm_util_mem_alloc(sizeof(MP3_STREAM_INFO_T));
    if (pt_mp3_stream_info == NULL)
    {
        _mm_util_mem_free(pt_id3_tag);
        return MINFOR_OUT_OF_MEMORY;
    }

    x_memset(pt_id3_tag, 0, sizeof(ID3_TAG_SET_T));
    x_memset(pt_mp3_stream_info, 0, sizeof(MP3_STREAM_INFO_T));
    pt_minfo_obj->pv_handler_obj = pt_mp3_stream_info;
    pt_mp3_stream_info->pt_mp3_id3_tag = pt_id3_tag;

    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_minfo_mopen
 *
 * Description: This function open a handler for a memory buffer
 *
 * Inputs:  pv_mem_data  specify the address of the memory buffer
 *          z_mem_leng   specify the length of the buffer
 *          pv_obj       specify the media info object
 *
 * Outputs: pv_obj       modify data in the media info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_minfo_mopen(
    VOID*                 pv_mem_data,
    SIZE_T                z_mem_leng,
    VOID*                 pv_obj
)
{
    MINFO_OBJ_T*          pt_minfo_obj;
    ID3_TAG_SET_T*        pt_id3_tag;
    MP3_STREAM_INFO_T*    pt_mp3_stream_info;
    MEDIA_FEEDER_T*       pt_src_feeder;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL)
        || (pv_obj == NULL))
    {
        return MINFOR_INV_ARG;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*)pv_obj;

    if (z_mem_leng == 0)
    {
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        pt_minfo_obj->t_input_fct_tbl.pf_copybytes =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes;
        pt_minfo_obj->t_input_fct_tbl.pf_input1 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input1;
        pt_minfo_obj->t_input_fct_tbl.pf_input2 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input2;
        pt_minfo_obj->t_input_fct_tbl.pf_input3 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input3;
        pt_minfo_obj->t_input_fct_tbl.pf_input4 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input4;
        pt_minfo_obj->t_input_fct_tbl.pf_set_pos =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos;
        pt_minfo_obj->t_input_fct_tbl.pf_get_pos =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_get_pos;
    }
    else
    {
        pt_minfo_obj->t_input_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input2 = mm_util_finput2bytes_b_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_b_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;
    }

    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }

    pt_id3_tag = (ID3_TAG_SET_T*)_mm_util_mem_alloc(sizeof(ID3_TAG_SET_T));
    if (pt_id3_tag == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    pt_mp3_stream_info = (MP3_STREAM_INFO_T*)_mm_util_mem_alloc(sizeof(MP3_STREAM_INFO_T));
    if (pt_mp3_stream_info == NULL)
    {
        _mm_util_mem_free(pt_id3_tag);
        return MINFOR_OUT_OF_MEMORY;
    }

    x_memset(pt_id3_tag, 0, sizeof(ID3_TAG_SET_T));
    x_memset(pt_mp3_stream_info, 0, sizeof(MP3_STREAM_INFO_T));
    pt_minfo_obj->pv_handler_obj = pt_mp3_stream_info;
    pt_mp3_stream_info->pt_mp3_id3_tag = pt_id3_tag;

    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_minfo_close
 *
 * Description: This function close a handler
 *
 * Inputs:  pv_obj       specify the media info object
 *
 * Outputs:
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_minfo_close(
                        VOID*              pv_obj)
{
    MINFO_OBJ_T*    pt_minfo_obj;
    MP3_STREAM_INFO_T*  pt_mp3_stream_info;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    if (pt_minfo_obj->pv_handler_obj)
    {
        pt_mp3_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);

        if (pt_mp3_stream_info->pt_mp3_id3_tag)
            _mm_util_mem_free(pt_mp3_stream_info->pt_mp3_id3_tag);

        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }
    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _mp3_minfo_skip_v2_id3
 *
 * Description: This function skip v2_id3 tag
 *
 * Inputs:  pt_minfo_obj   specify the object of midxbuld
 *
 * Outputs:
 *
 * Returns: MINFO_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/

INT32 _mp3_minfo_skip_v2_id3(
    MINFO_OBJ_T*    pt_minfo_obj,
    UINT32*         pui4_v2_tag_size)
{
    setpos_fct          pf_set_pos;
    input3bytes_fct     pf_input3byte;
    input1bytes_fct     pf_input1byte;
    UINT32              ui4_tag;
    UINT8               ui1_tmp1, ui1_tmp2, ui1_tmp3, ui1_tmp4;
    BOOL                b_has_id3_tag_v2 = FALSE;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pf_input3byte = pt_minfo_obj->t_input_fct_tbl.pf_input3;
    pf_input1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    
    /*
    * check id3v2 magic word -- "ID3"
    * and parse header and set the following 4 fields in id3_tag:
    * (1) v2_size ( not include header & footer size )
    * (2) v2_unsync
    * (3) v2_exthdr
    * (4) v2_footer
    */

    /*
    ID3v2/file identifier      "ID3"
    ID3v2 version              $04 00
    ID3v2 flags                %abcd0000
    ID3v2 size             4 * %0xxxxxxx
    An ID3v2 tag can be detected with the following pattern:
    $49 44 33 yy yy xx zz zz zz zz
    Where yy is less than $FF, xx is the 'flags' byte and zz is less than
    $80.
    */
    *pui4_v2_tag_size = 0;
    do
    {
        UINT32 ui4_id3_tag_offset;

        IS_INPUT_OK(pf_set_pos((VOID*)pt_minfo_obj, *pui4_v2_tag_size, FM_SEEK_BGN));
        
        IS_INPUT_OK(pf_input3byte(pt_minfo_obj, &ui4_tag));
        if (cui4_ID3 != ui4_tag)
        {   /*no id3 tag*/
            if(!b_has_id3_tag_v2)
            {
                //IS_INPUT_OK(pf_set_pos((VOID*)pt_minfo_obj, 0, FM_SEEK_BGN));
                *pui4_v2_tag_size = 0;
            }
            break;
        }
        
        b_has_id3_tag_v2 = TRUE;
        
        IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_tmp1));   /* skip ui1_v2_version*/
        IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_tmp1));   /* skip revision*/
        IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_tmp1));   /* skip flags*/

        /* get tag size */
        IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_tmp1));
        IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_tmp2));
        IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_tmp3));
        IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_tmp4));
        ui4_id3_tag_offset = (((UINT32)ui1_tmp1 << 21)
                | ((UINT32)ui1_tmp2 << 14)
                | ((UINT32)ui1_tmp3 << 7)
                | ((UINT32)ui1_tmp4));
        *pui4_v2_tag_size += ui4_id3_tag_offset;
        *pui4_v2_tag_size += 10; /*header of id3 tag v2*/        
    }while(1);
    IS_INPUT_OK(pf_set_pos((VOID*)pt_minfo_obj, (INT64)(*pui4_v2_tag_size) /*header*/, FM_SEEK_BGN));
    
    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _mp3_minfo_parse_xing_header
 *
 * Description:
 *
 * Inputs:  pt_minfo_obj   specify the object of midxbuld
 *
 * Outputs:
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/

INT32 _mp3_minfo_parse_xing_header(
                                    MINFO_OBJ_T*     pt_minfo_obj)
{
    MP3_STREAM_INFO_T*  pt_mp3_stream_info;
    input1bytes_fct     pf_input1byte;
    input4bytes_fct     pf_input4byte;
    setpos_fct          pf_setpos;
    getpos_fct          pf_getpos;
    UINT64              ui8_pos;
    UINT8               ui1_second , ui1_first, ui1_input;
    UINT8               ui1_version;        /* version ID */
    UINT8               ui1_mode;           /* channel mode */
    UINT8               ui1_sr_index;       /* sampling rate idnex*/
    static UINT32       ui4_sr_table[4] = { 44100, 48000, 32000, 99999 };
    UINT32              ui4_read;
    INT32		i4_ret;

    UINT32              ui4_i = 0;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }
    pt_mp3_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_mp3_stream_info == NULL)
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pf_input1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_input4byte = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pf_setpos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_getpos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    /* find and check syncword */
    /* 111 1111 1111 */
    IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_first));

    IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_second));
    while((ui1_first != 0xFF)||((ui1_second & 0xE0) != 0xE0))
    {
        ui1_first = ui1_second;
	i4_ret = pf_input1byte(pt_minfo_obj, &ui1_second);
        IS_INPUT_OK(i4_ret);

        if (ui4_i > MINFO_MP3_MAX_PARSING_LEN || i4_ret == INPUTR_EOF)
        {
            return MINFOR_FMT_NOT_SUPPORT;
        }
        ui4_i++;
    }
    /*store (current-2) pos*/
    pf_getpos(pt_minfo_obj, &ui8_pos);
    ui8_pos = ui8_pos - 2;

    /* get selected MPEG header data */
    ui1_version = (ui1_second >> 3) & 3;
    IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_sr_index));
    ui1_sr_index = (ui1_sr_index >> 2) & 3;
    IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_mode));
    ui1_mode = (ui1_mode >> 6) & 3;

    /* determine offset of header based on channel mode information */
    if( ui1_version )
    {
        /* mpeg1 */
        if( ui1_mode != 3 )
        {
            pf_setpos(pt_minfo_obj, (32), FM_SEEK_CUR);
        }
        else
        {
            pf_setpos(pt_minfo_obj, (17), FM_SEEK_CUR);
        }
    }
    else
    {
        /* mpeg2 */
        if( ui1_mode != 3 )
        {
            pf_setpos(pt_minfo_obj, (17), FM_SEEK_CUR);
        }
        else
        {
            pf_setpos(pt_minfo_obj, (9), FM_SEEK_CUR);
        }
    }
    IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_input));
    if( ui1_input != 'X' )
    {
        IS_INPUT_OK(pf_setpos(pt_minfo_obj, (INT64)ui8_pos, FM_SEEK_BGN));
        return MINFOR_FMT_NOT_SUPPORT;
    }
    IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_input));
    if( ui1_input != 'i' )
    {
        IS_INPUT_OK(pf_setpos(pt_minfo_obj, (INT64)ui8_pos, FM_SEEK_BGN));
        return MINFOR_FMT_NOT_SUPPORT;
    }
    IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_input));
    if( ui1_input != 'n' )
    {
        IS_INPUT_OK(pf_setpos(pt_minfo_obj, (INT64)ui8_pos, FM_SEEK_BGN));
        return MINFOR_FMT_NOT_SUPPORT;
    }
    IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_input));
    if( ui1_input != 'g' )
    {
        IS_INPUT_OK(pf_setpos(pt_minfo_obj, (INT64)ui8_pos, FM_SEEK_BGN));
        return MINFOR_FMT_NOT_SUPPORT;
    }

    pt_mp3_stream_info->ui4_xing_sample_rate = ui4_sr_table[ui1_sr_index];
    if( ui1_version == 0 )
    {
        pt_mp3_stream_info->ui4_xing_sample_rate >>= 1;
    }
    /*get flags*/
    IS_INPUT_OK(pf_input4byte(pt_minfo_obj, &(pt_mp3_stream_info->ui4_xing_flags)));

    if( pt_mp3_stream_info->ui4_xing_flags & FRAMES_FLAG )
    {
        IS_INPUT_OK(pf_input4byte(pt_minfo_obj, &(pt_mp3_stream_info->ui4_xing_frames)));
    }

    if( pt_mp3_stream_info->ui4_xing_flags & BYTES_FLAG )
    {
        IS_INPUT_OK(pf_input4byte(pt_minfo_obj, &(pt_mp3_stream_info->ui4_xing_bytes)));
    }

    if( pt_mp3_stream_info->ui4_xing_flags & TOC_FLAG )
    {
        IS_INPUT_OK(pf_setpos(pt_minfo_obj, (100), FM_SEEK_CUR));
    }
    if( pt_mp3_stream_info->ui4_xing_flags & VBR_SCALE_FLAG )
    {
        IS_INPUT_OK(pf_input4byte(pt_minfo_obj, &ui4_read));
    }
    return MINFOR_OK;

}

/*-----------------------------------------------------------------------------
 * Name:  _mp3_minfo_find_parse_frame_header
 *
 * Description: this function will try to parse frame header from current input stream position to
 *  the range of DAF_FRAME_CHECK_SIZE so even if the target is not a valid daf file,
 *  the processing time is bounded.
 *
 * Inputs:  pt_minfo_obj   specify the object of midxbuld
 *
 * Outputs:
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/

INT32 _mp3_minfo_find_parse_frame_header(
    MINFO_OBJ_T*          pt_minfo_obj)
{
    copybytes_fct         pf_copybytes;
    getpos_fct            pf_getpos;
    setpos_fct            pf_setpos;
    MP3_STREAM_INFO_T*    pt_mp3_stream_info;
    UINT64                ui8_pos;

    CHAR*                 ac_buff;
    UINT8                 ui1_first, ui1_second, ui1_third, ui1_fourth;
    CHAR                  ac_tmp[3];
    UINT32                ui4_read, ui4_actual_read;
    UINT32                ui4_idx = 0;
    UINT8                 ui1_rate_index;
    UINT8                 ui1_padding;                /* 1 -> yes, 0 -> no */
    UINT8                 ui1_slot_bytes;
    UINT32                ui4_frame_slots;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }
    pt_mp3_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_mp3_stream_info == NULL)
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pf_copybytes = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_getpos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pf_setpos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;

    /* 1. store current pos */
    pf_getpos(pt_minfo_obj, &ui8_pos);

    ac_buff = (CHAR*)_mm_util_mem_alloc(
                         sizeof(CHAR) * MINFO_MP3_MAX_PARSING_LEN);
    if (ac_buff == NULL)
    {
        return MINFOR_INSUFFICIENT_BUFFER;
    }

    /* 3. read 1024 bytes for parsing */
    IS_INPUT_OK(pf_copybytes((VOID*)pt_minfo_obj,
                (VOID*)ac_buff,
                MINFO_MP3_MAX_PARSING_LEN,
                MINFO_MP3_MAX_PARSING_LEN,
                &ui4_read));

    ui4_actual_read = ui4_read;
    /* 4. find a possible audio frame header */
    while (ui4_idx < ui4_actual_read)
    {
        /* 4(a). check the magic word */
        ui1_first  = (UINT8)(ac_buff[ui4_idx]);
        ui1_second = (UINT8)(ac_buff[ui4_idx + 1]);
        ui1_third  = (UINT8)(ac_buff[ui4_idx + 2]);
        ui1_fourth = (UINT8)(ac_buff[ui4_idx + 3]);

        if ((ui1_first != 0xFF)
            || ((ui1_second & 0xE0) != 0xE0)
            || ((ui1_second & 0x18) >> 3 == 1)      /* version description is set to reserved */
            || ((ui1_second & 0x06) >> 1 == 0)      /* layer description is set to reserved */
            || ((ui1_third >> 4) == 0)
            || ((ui1_third >> 4) == 15))
        {
            ui4_idx++;
        }
        /* 4(b). check this audio frame header is valid or NOT */
        else
        {
            pt_mp3_stream_info->ui1_version = (UINT8)(3 - ((ui1_second & 0x18) >> 3));
            pt_mp3_stream_info->ui1_layer = (UINT8)(3 - ((ui1_second & 0x6) >> 1));
            /* bitrate index */
            ui1_rate_index = ui1_third >> 4;
            pt_mp3_stream_info->ui4_bitrate = 0;
            switch (pt_mp3_stream_info->ui1_version)
            {
                case MP3_MINFO_VERSION_1:
                    pt_mp3_stream_info->ui4_bitrate =
                        MPEG_V1_BITRATE_TAB[pt_mp3_stream_info->ui1_layer][ui1_rate_index];           /* kbps */
                    break;
                case MP3_MINFO_VERSION_2:
                case MP3_MINFO_VERSION_2_5:
                    pt_mp3_stream_info->ui4_bitrate =
                       MPEG_V2_BITRATE_TAB[(pt_mp3_stream_info->ui1_layer + 1) >> 1][ui1_rate_index]; /* kbps */
                    break;
            }
            pt_mp3_stream_info->ui4_bitrate *= 1000;    /* bps */

            /* sampling frequency index */
            ui1_rate_index = ((ui1_third & 0xc) >> 2);
            pt_mp3_stream_info->ui4_sample_rate = 0;
            if (ui1_rate_index != 3)    /* not reserved */
            {
                pt_mp3_stream_info->ui4_sample_rate = MPEG_SAMPLERATE_TAB[ui1_rate_index];
            }
            switch (pt_mp3_stream_info->ui1_version)
            {
                case MP3_MINFO_VERSION_2:
                    pt_mp3_stream_info->ui4_sample_rate >>= 1;
                    break;
                case MP3_MINFO_VERSION_2_5:
                    pt_mp3_stream_info->ui4_sample_rate >>= 2;
                    break;
            }
            if (pt_mp3_stream_info->ui4_sample_rate == 0)
            {
                /* read next 1 byte */
                ui4_idx++;
                continue;
            }
            /* padding bit */
            ui1_padding = ((ui1_third & 0x2)>>1);

            /* Channel Number:0:stereo, 1:stereo, 2:dual, 3:mono */
            pt_mp3_stream_info->ui1_channels = ui1_fourth >> 6;

            /* calculate frame length */
            pt_mp3_stream_info->ui4_frame_length = 0;
            if (pt_mp3_stream_info->ui4_bitrate != 0)
            {
                ui1_slot_bytes = 1;
                if (pt_mp3_stream_info->ui1_layer == MP3_MINFO_LAYER_I)
                {
                    ui4_frame_slots = 12;
                    ui1_slot_bytes = 4;
                }
                else if ((pt_mp3_stream_info->ui1_layer == MP3_MINFO_LAYER_III)
                    && (pt_mp3_stream_info->ui1_version == MP3_MINFO_VERSION_2
                    || pt_mp3_stream_info->ui1_version == MP3_MINFO_VERSION_2_5))
                {
                    ui4_frame_slots = 72;
                }
                else
                {
                    ui4_frame_slots = 144;
                }
                /* calculate frame length */
                if (pt_mp3_stream_info->ui4_sample_rate != 0)
                {
                    pt_mp3_stream_info->ui4_frame_length =
                        (ui4_frame_slots * pt_mp3_stream_info->ui4_bitrate / pt_mp3_stream_info->ui4_sample_rate + ui1_padding) * ui1_slot_bytes;
                }
            }
            /* jump to check next frame header file is valid or invalid */
            if (ui4_idx + pt_mp3_stream_info->ui4_frame_length < ui4_actual_read)
            {
                x_memcpy(ac_tmp, (ac_buff + ui4_idx + pt_mp3_stream_info->ui4_frame_length), 3);
            }
            else
            {
                IS_INPUT_OK(pf_setpos((VOID*)pt_minfo_obj, (INT64)(ui8_pos + ui4_idx + pt_mp3_stream_info->ui4_frame_length), FM_SEEK_BGN));
                IS_INPUT_OK(pf_copybytes((VOID*)pt_minfo_obj, (VOID*)ac_tmp, 3, 3, &ui4_read));
            }
            /* check if next frame header is also valid */
            ui1_first  = (UINT8)(ac_tmp[0]);
            ui1_second = (UINT8)(ac_tmp[1]);
            ui1_third  = (UINT8)(ac_tmp[2]);
            if ((ui1_first == 0xFF)
                && ((ui1_second & 0xE0) == 0xE0)
                && ((ui1_second & 0x18) >> 3 != 1)      /* version description is set to reserved */
                && ((ui1_second & 0x06) >> 1 != 0)      /* layer description is set to reserved */
                && ((ui1_third >> 4) != 0)
                && ((ui1_third >> 4) != 15))
            {
                pt_mp3_stream_info->ui8_frame_bng_pos = ui8_pos + (UINT64)ui4_idx;
                /* free memory */
                if (ac_buff != NULL)
                {
                    _mm_util_mem_free((VOID*)ac_buff);
                }

                return MINFOR_OK;
            }
            /* read next 1 byte */
            ui4_idx++;
        }
    }
    /* free memory */
    if (ac_buff != NULL)
    {
        _mm_util_mem_free((VOID*)ac_buff);
    }

    return MINFOR_FMT_NOT_SUPPORT;
}

static INT32 mp3_minfo_parse_tag1(VOID*   pv_obj)
{
    MINFO_OBJ_T*        pt_minfo_obj;
    INT32               i4_ret = MINFOR_OK;
    setpos_fct          pf_set_pos;
    input3bytes_fct     pf_input3byte;
    UINT32              ui4_tag;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/

    /* parse stream info */
    pt_minfo_obj = (MINFO_OBJ_T*)pv_obj;
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_input3byte = pt_minfo_obj->t_input_fct_tbl.pf_input3;

    if (!pf_set_pos || !(pt_minfo_obj->h_file || pt_minfo_obj->h_src_feeder))
    {
        return MINFOR_INITIAL_ERR;        
    }

    if(pt_minfo_obj->b_parsed_id3)
    {
        return MINFOR_OK;
    }

    /* check if ID3v1 tag exists */
    IS_INPUT_OK(pf_set_pos((VOID*)pt_minfo_obj, (INT64)(-128), FM_SEEK_END));
    IS_INPUT_OK(pf_input3byte(pt_minfo_obj, &ui4_tag));
    if (cui4_TAG == ui4_tag)
    {   /*has id3 tag v1*/
        i4_ret = mp3_minfo_parse_id3(pt_minfo_obj, FALSE);
    }

    pt_minfo_obj->b_parsed_id3 = TRUE;

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_minfo_parse
 *
 * Description: This function start parsing the infomation
 *
 * Inputs:  pv_obj       specify the media info object
 *
 * Outputs:
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_minfo_parse(
                        VOID*              pv_obj)
{

    MINFO_OBJ_T*        pt_minfo_obj;
    MP3_STREAM_INFO_T*  pt_mp3_stream_info;
    INT32               i4_ret;
    /*
    FM_FILE_INFO_T      t_info;
    */
    setpos_fct          pf_set_pos;
    getpos_fct          pf_get_pos;
    /*input3bytes_fct     pf_input3byte;*/
    UINT64              ui8_size;
    UINT64              ui8_size_wo_tag;
    UINT32              ui4_v2_tag_size;
    /*UINT32              ui4_v1_tag_size;
          UINT32              ui4_tag;*/
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/

    /* parse stream info */
    pt_minfo_obj = (MINFO_OBJ_T*)pv_obj;
    pt_mp3_stream_info = (MP3_STREAM_INFO_T* )(pt_minfo_obj->pv_handler_obj);
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    /*pf_input3byte = pt_minfo_obj->t_input_fct_tbl.pf_input3;*/

    if (!pf_set_pos || !pf_get_pos)
    {
        return MINFOR_INITIAL_ERR;        
    }   

    /* parse ID3 v2*/
#ifndef DISABLE_MM_ID3_V2    
    mp3_minfo_parse_id3((MINFO_OBJ_T*)pv_obj, TRUE);
#endif
    i4_ret = _mp3_minfo_skip_v2_id3(pt_minfo_obj, &ui4_v2_tag_size);    
  
    if (i4_ret == MINFOR_OK)
    {
        _mp3_minfo_parse_xing_header(pt_minfo_obj);
        i4_ret = _mp3_minfo_find_parse_frame_header(pt_minfo_obj);

        IS_INPUT_OK(pf_set_pos(pt_minfo_obj, 0, FM_SEEK_END));
        IS_INPUT_OK(pf_get_pos(pt_minfo_obj, &ui8_size));
        if (ui8_size == ((UINT64)-1))
        {
            pt_minfo_obj->b_filesize_non_gettable = TRUE;
        }

        pt_mp3_stream_info->ui4_mp3_length_in_byte = (UINT32)ui8_size;
        
        /*remove for network read, it willy cause much more time to read once,   place to funcion mp3_minfo_parse_tag1()*/
        /* because last 128 byte seldom affect duration,it can be ignored when calculate duration */
        #if 0  
        /* check if ID3v1 tag exists */
        IS_INPUT_OK(pf_set_pos((VOID*)pt_minfo_obj, (INT64)(ui8_size - 128), FM_SEEK_BGN));
        IS_INPUT_OK(pf_input3byte(pt_minfo_obj, &ui4_tag));
        if (cui4_TAG != ui4_tag)
        {   /*no id3 tag*/
            ui4_v1_tag_size = 0;
        }
        else
        {
            ui4_v1_tag_size = 128;
        }

        /* get file duration */
        ui8_size_wo_tag = ui8_size - ui4_v1_tag_size- ui4_v2_tag_size;
        #else
        /* get file duration */
        ui8_size_wo_tag = ui8_size - ui4_v2_tag_size;
        #endif        
    
        if (i4_ret == MINFOR_OK)
        {   /* calculate duration */
            if (pt_mp3_stream_info->ui4_xing_frames == 0)
            {
                if (pt_mp3_stream_info->ui1_layer == MP3_MINFO_LAYER_I)
                {
                    UINT64 ui8_temp;
                    ui8_temp = _mm_div64(ui8_size_wo_tag, pt_mp3_stream_info->ui4_frame_length, NULL);
                    ui8_temp *= 384;
                    pt_mp3_stream_info->ui4_mp3_length_in_sec =
                        (UINT32)_mm_div64(ui8_temp, pt_mp3_stream_info->ui4_sample_rate, NULL);

                }
                else if (((pt_mp3_stream_info->ui1_version == MP3_MINFO_VERSION_2)
                       || (pt_mp3_stream_info->ui1_version == MP3_MINFO_VERSION_2_5))
                       && (pt_mp3_stream_info->ui1_layer == MP3_MINFO_LAYER_III))
                {
                    UINT64 ui8_temp;
                    ui8_temp = _mm_div64(ui8_size_wo_tag, pt_mp3_stream_info->ui4_frame_length, NULL);
                    ui8_temp *= 576;
                    pt_mp3_stream_info->ui4_mp3_length_in_sec =
                        (UINT32)_mm_div64(ui8_temp, pt_mp3_stream_info->ui4_sample_rate, NULL);
                }
                else
                {
                    UINT64 ui8_temp;
                    ui8_temp = _mm_div64(ui8_size_wo_tag, pt_mp3_stream_info->ui4_frame_length, NULL);
                    ui8_temp *= 1152;
                    pt_mp3_stream_info->ui4_mp3_length_in_sec =
                        (UINT32)_mm_div64(ui8_temp, pt_mp3_stream_info->ui4_sample_rate, NULL);
                }
            }
            else
            {   /*Xing is here*/
                if (pt_mp3_stream_info->ui1_layer == MP3_MINFO_LAYER_I)
                {
                    pt_mp3_stream_info->ui4_mp3_length_in_sec =
                        (UINT32)_mm_div64(((pt_mp3_stream_info->ui4_xing_frames)*384),
                                            pt_mp3_stream_info->ui4_sample_rate,
                                            NULL);
                }
                else if (((pt_mp3_stream_info->ui1_version == MP3_MINFO_VERSION_2)
                       || (pt_mp3_stream_info->ui1_version == MP3_MINFO_VERSION_2_5))
                       && (pt_mp3_stream_info->ui1_layer == MP3_MINFO_LAYER_III))
                {
                    pt_mp3_stream_info->ui4_mp3_length_in_sec =
                        (UINT32)_mm_div64(((pt_mp3_stream_info->ui4_xing_frames)*576),
                                            pt_mp3_stream_info->ui4_sample_rate,
                                            NULL);
                }
                else
                {
                    pt_mp3_stream_info->ui4_mp3_length_in_sec =
                        (UINT32)_mm_div64(((pt_mp3_stream_info->ui4_xing_frames)*1152),
                                            pt_mp3_stream_info->ui4_sample_rate,
                                            NULL);
                }
            }
            if (pt_mp3_stream_info->ui4_mp3_length_in_sec == 0)
            {
                /* force to set the duration of a very short MP3 file to 1 sec */
                pt_mp3_stream_info->ui4_mp3_length_in_sec = 1;
            }
            /*cr:315779  no file size DLNA */
            if (ui8_size == MINFOR_MP3_NO_SIZE_BY_DLNA)
            {
                pt_mp3_stream_info->ui4_mp3_length_in_sec = MINFOR_MP3_NO_SIZE;
            }
        }
    }    

    #if 0 /*remove for network read, it willy cause much more time to read once,  place to funcion mp3_minfo_parse_tag1()*/
    /* parse ID3 v1*/
    if(pt_minfo_obj->b_parsed_id3)
    i4_ret = mp3_minfo_parse_id3((MINFO_OBJ_T*)pv_obj, FALSE);
    #endif

    if (i4_ret == MINFOR_OK)
    {
        pt_minfo_obj->b_parsing_successful = TRUE;
    }

    return i4_ret;

}
/*-----------------------------------------------------------------------------
 * Name:  mp3_minfo_get_txt_info
 *
 * Description:This function get information from a handler
 *
 * Inputs:  pv_obj       specify the media info object
 *
 * Outputs:
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_minfo_get_txt_info(
                        ID3_TAG_SET_T*  pt_id3_tag_set,
                        UINT32          ui4_index,
                        VOID**          ppv_buf,
                        SIZE_T*         pz_buf_leng)
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_id3_tag_set == NULL)
        /*|| (pz_buf_leng == NULL)*/)
    {
        return MINFOR_INITIAL_ERR;
    }

    *pz_buf_leng = pt_id3_tag_set->at_txt_info[ui4_index].z_size;
    *ppv_buf = (VOID*)(pt_id3_tag_set->at_txt_info[ui4_index].awc_text);

    #if 0
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    if (pv_buf == NULL)
    { /*get lengthy only*/
        *pz_buf_leng = pt_id3_tag_set->at_txt_info[ui4_index].z_size;
    }
    else
    {
        if (*pz_buf_leng >= pt_id3_tag_set->at_txt_info[ui4_index].z_size)
        {   /*enough*/
            x_memcpy(pv_buf,
                    (VOID*)pt_id3_tag_set->at_txt_info[ui4_index].awc_text,
                    pt_id3_tag_set->at_txt_info[ui4_index].z_size * sizeof (UTF16_T));
        }
        else
        {  /* not enough*/
            x_memcpy(pv_buf,
                    (VOID*)pt_id3_tag_set->at_txt_info[ui4_index].awc_text,
                    *pz_buf_leng * sizeof (UTF16_T));
        }
    }
    #endif
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_minfo_get_info
 *
 * Description:This function get information from a handler
 *
 * Inputs:  pv_obj       specify the media info object
 *
 * Outputs:
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_minfo_get_info(
    VOID*                           pv_obj,
    MINFO_INFO_TYPE_T               e_info_type,
    MINFO_REC_IDX_T                 ui2_index,
    VOID*                           pv_buf,
    SIZE_T*                         pz_buf_leng
)
{
    MINFO_OBJ_T*                    pt_minfo_obj;
    MP3_STREAM_INFO_T*              pt_mp3_stream_info;
    ID3_TAG_SET_T*                  pt_id3_tag_set;
    MINFO_INFO_PIC_T*               pt_pic;
    UINT32                          ui4_idx;

    MINFO_INFO_T*                   pt_minfo_rec;
    MINFO_TYPE_ID3_TAG_T*           pt_id3_tag_info; /* output */
    MINFO_TYPE_GENERIC_T*           pt_generic_info; /* output */
    MINFO_MP3_AUDIO_STM_ATRBT_T*    pt_mp3_audio_atrbt; /* output */
    MINFO_TYPE_META_DATA_T*         pt_meta_data_info;     /* output */

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (pv_buf == NULL)
        || (pz_buf_leng == NULL))
    {
        return MINFOR_INITIAL_ERR;
    }

    if (*pz_buf_leng < sizeof(MINFO_INFO_T))
    {
        return MINFOR_INV_ARG;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    *pz_buf_leng = sizeof(MINFO_INFO_T);
    /*x_memset(pv_buf,0,*pz_buf_leng);*/

    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    pt_minfo_rec = (MINFO_INFO_T*)pv_buf;

    pt_mp3_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_mp3_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }

    pt_id3_tag_set = pt_mp3_stream_info->pt_mp3_id3_tag;
    if (pt_id3_tag_set == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }

    if (pt_minfo_obj->b_parsing_successful == FALSE)
    {
        return MINFOR_INFO_NOT_READY;
    }

    switch (e_info_type)
    {
        case MINFO_INFO_TYPE_ID3_DATA :
            if (pt_id3_tag_set->b_is_parsing_done == FALSE)
            {
                return MINFOR_INFO_NOT_READY;
            }
            /*Not parse file footer data if filesize not get*/
            if (!pt_minfo_obj->b_filesize_non_gettable)
            {
                mp3_minfo_parse_tag1((VOID*)pt_minfo_obj);
            }

            pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
            pt_minfo_rec->e_info_type = e_info_type;
            pt_minfo_rec->ui1_rec_idx = 1;
            pt_id3_tag_info = (MINFO_TYPE_ID3_TAG_T*)(&(pt_minfo_rec->u.t_id3_data));
            if (pt_id3_tag_set)
            {
                BOOL    fg_first_one = TRUE;
                /* ID3_ATTACHED_PIC BITMASK */
                pt_id3_tag_info->ui4_pic_bitmask =  pt_id3_tag_set->ui4_pic_bitmask;
                /* ID3_ATTACHED_PIC */
                pt_pic = (MINFO_INFO_PIC_T*)(&(pt_id3_tag_info->at_id3_attatched_pic));

                for (ui4_idx= 0; ui4_idx < MINFO_MAX_ID3_PIC_NUM; ui4_idx++)
                {
                    pt_pic[ui4_idx].e_pic_type  = pt_id3_tag_set->at_pic[ui4_idx].e_pic_type;
                    pt_pic[ui4_idx].t_format    = pt_id3_tag_set->at_pic[ui4_idx].t_format;
                    pt_pic[ui4_idx].ui4_length  = pt_id3_tag_set->at_pic[ui4_idx].ui4_length;
                    pt_pic[ui4_idx].ui4_starting_offset = pt_id3_tag_set->at_pic[ui4_idx].ui4_starting_offset;
                    pt_pic[ui4_idx].ui4_width   = pt_id3_tag_set->at_pic[ui4_idx].ui4_width;
                    pt_pic[ui4_idx].ui4_height  = pt_id3_tag_set->at_pic[ui4_idx].ui4_height;
                    
                    /* only get first item */
                    if (((pt_id3_tag_set->ui4_pic_bitmask & ((UINT32)1 << ui4_idx)) != 0)
                        && fg_first_one
                        && (pt_id3_tag_info->pui1_img_data_buf != NULL))
                    {
                        UINT32              ui4_read = 0;
                        setpos_fct          pf_set_pos;
                        copybytes_fct       pf_copybytes;
                        
                        pf_set_pos  = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
                        pf_copybytes= pt_minfo_obj->t_input_fct_tbl.pf_copybytes;

                        fg_first_one = FALSE;
                        IS_INPUT_OK(pf_set_pos (pt_minfo_obj, 
                                                pt_pic[ui4_idx].ui4_starting_offset, 
                                                FM_SEEK_BGN));
                        
                        IS_INPUT_OK(pf_copybytes((VOID*)pt_minfo_obj,
                                    (VOID*)pt_id3_tag_info->pui1_img_data_buf,
                                    pt_pic[ui4_idx].ui4_length,
                                    pt_pic[ui4_idx].ui4_length,
                                    &ui4_read));
                    }
                }
                mp3_minfo_cvt_id3_txt(pt_minfo_obj, pt_id3_tag_set->e_language);
                /* ID3_YEAR */
                if(pt_id3_tag_set->at_txt_info[ID3_TAG_V2_YEAR_FRAME_IDX].z_size <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
                {
                    COPY_DATA(  pt_id3_tag_set->at_txt_info[ID3_TAG_V2_YEAR_FRAME_IDX].awc_text, 
                                pt_id3_tag_set->at_txt_info[ID3_TAG_V2_YEAR_FRAME_IDX].z_size, 
                                pt_id3_tag_info->pwc_str_id3_year, 
                                           &(pt_id3_tag_info->z_id3_year_len));
                }

                /* ID3 TITLE */
                if(pt_id3_tag_set->at_txt_info[ID3_TAG_V2_TITLE_FRAME_IDX].z_size <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
                {
                    COPY_DATA(  pt_id3_tag_set->at_txt_info[ID3_TAG_V2_TITLE_FRAME_IDX].awc_text, 
                                pt_id3_tag_set->at_txt_info[ID3_TAG_V2_TITLE_FRAME_IDX].z_size, 
                                pt_id3_tag_info->pwc_str_id3_title, 
                                           &(pt_id3_tag_info->z_id3_title_len));
                }

                /* ID3 ARTIST */
                if(pt_id3_tag_set->at_txt_info[ID3_TAG_V2_ARTIST_FRAME_IDX].z_size <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
                {
                    COPY_DATA(  pt_id3_tag_set->at_txt_info[ID3_TAG_V2_ARTIST_FRAME_IDX].awc_text, 
                                pt_id3_tag_set->at_txt_info[ID3_TAG_V2_ARTIST_FRAME_IDX].z_size, 
                                pt_id3_tag_info->pwc_str_id3_artist, 
                                           &(pt_id3_tag_info->z_id3_artist_len));
                }

                /* ID3 ALBUM */
                if(pt_id3_tag_set->at_txt_info[ID3_TAG_V2_ALBUM_FRAME_IDX].z_size <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
                {
                    COPY_DATA(  pt_id3_tag_set->at_txt_info[ID3_TAG_V2_ALBUM_FRAME_IDX].awc_text, 
                                pt_id3_tag_set->at_txt_info[ID3_TAG_V2_ALBUM_FRAME_IDX].z_size, 
                                pt_id3_tag_info->pwc_str_id3_album, 
                                           &(pt_id3_tag_info->z_id3_album_len));
                }

                /* ID3 GENRE */
                if(pt_id3_tag_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].z_size <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
                {
                    COPY_DATA(  pt_id3_tag_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text, 
                                pt_id3_tag_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].z_size, 
                                pt_id3_tag_info->pwc_str_id3_genre, 
                                           &(pt_id3_tag_info->z_id3_genre_len));
                }

                /* ID3 TRACK */
                if(pt_id3_tag_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].z_size <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
                {
                    COPY_DATA(  pt_id3_tag_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].awc_text, 
                                pt_id3_tag_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].z_size, 
                                pt_id3_tag_info->pwc_str_id3_track, 
                                           &(pt_id3_tag_info->z_id3_track_len));
                }

                pt_id3_tag_info->ui4_id3_v1_len = pt_id3_tag_set->ui4_v1_length;
                pt_id3_tag_info->ui4_id3_v2_len = pt_id3_tag_set->ui4_v2_length;
                return MINFOR_OK;
            }
            else
            {
                return MINFOR_INV_ARG;
            }
        case MINFO_INFO_TYPE_ID3_RAW_DATA:
            {                
                MINFO_ID3_RAW_DATA_T       t_id3_raw_data;
                UINT32                     ui4_read        = 0;
                setpos_fct                 pf_set_pos;
                copybytes_fct              pf_copybytes;

                pt_id3_tag_info = (MINFO_TYPE_ID3_TAG_T*)(&(pt_minfo_rec->u.t_id3_data));
                t_id3_raw_data  = pt_id3_tag_info->t_id3_raw_data;        
                pf_set_pos  = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
                pf_copybytes= pt_minfo_obj->t_input_fct_tbl.pf_copybytes;

                if (pf_set_pos == NULL || pf_copybytes == NULL)
                {
                    return MINFOR_INITIAL_ERR;
                }
                switch (t_id3_raw_data.e_id3_type)
                {
                case MINFO_INFO_ID3_V1:
                    if (pt_id3_tag_set->ui4_v1_length == 0)
                    {
                        return MINFOR_INFO_NOT_FOUND;
                    }
                    if (t_id3_raw_data.pui1_raw_data == NULL ||
                        *(t_id3_raw_data.pz_data_size) == pt_id3_tag_set->ui4_v1_length)
                    {
                        return MINFOR_INV_ARG;
                    }
                    /* Search the start position */
                    IS_INPUT_OK(pf_set_pos (pt_minfo_obj, 
                                            -(pt_id3_tag_set->ui4_v1_length), 
                                            FM_SEEK_END) );
                    break;
                case MINFO_INFO_ID3_V2:
                    if (pt_id3_tag_set->ui4_v2_length == 0)
                    {
                        return MINFOR_INFO_NOT_FOUND;
                    }
                    if (t_id3_raw_data.pui1_raw_data == NULL ||
                        *(t_id3_raw_data.pz_data_size) == pt_id3_tag_set->ui4_v2_length)
                    {
                        return MINFOR_INV_ARG;
                    }
                    /* Search the start position, just */
                    IS_INPUT_OK(pf_set_pos (pt_minfo_obj, 
                                            3, 
                                            FM_SEEK_BGN) );
                    break;
                default:
                    return MINFOR_INV_ARG;
                }

                IS_INPUT_OK(pf_copybytes((VOID*)pt_minfo_obj,
                                              (VOID*)t_id3_raw_data.pui1_raw_data,
                                              *(t_id3_raw_data.pz_data_size),
                                              *(t_id3_raw_data.pz_data_size),
                                              &ui4_read));
                *(t_id3_raw_data.pz_data_size) = ui4_read;
            }
            return MINFOR_OK;

        case MINFO_INFO_TYPE_GENERIC:
            pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
            pt_minfo_rec->e_info_type = e_info_type;
            pt_minfo_rec->ui1_rec_idx = 1;
            pt_generic_info = (MINFO_TYPE_GENERIC_T*)(&(pt_minfo_rec->u.t_general_info));
            if (pt_generic_info)
            {
                pt_generic_info->ui8_pb_duration = (UINT64)pt_mp3_stream_info->ui4_mp3_length_in_sec * 90000;
                if(pt_mp3_stream_info->ui4_mp3_length_in_byte == MINFOR_MP3_NO_SIZE)
                {
                    pt_generic_info->ui8_media_sz = MINFOR_MP3_NO_SIZE_BY_DLNA;
                }
                else
                {
                    pt_generic_info->ui8_media_sz = (UINT64)(pt_mp3_stream_info->ui4_mp3_length_in_byte);
                }
                pt_generic_info->ui1_strm_ns     = 1;
                return MINFOR_OK;
            }
            else
            {
                return MINFOR_INV_ARG;
            }

        case MINFO_INFO_TYPE_AUDIO_ATTR:
            if(ui2_index > 1)
            {
                return MINFOR_OUT_OF_RECORD;
            }
            pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
            pt_minfo_rec->e_info_type = e_info_type;
            pt_minfo_rec->ui1_rec_idx = 1;
            pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = TRUE;
            pt_mp3_audio_atrbt = (MINFO_MP3_AUDIO_STM_ATRBT_T*)(&(pt_minfo_rec->u.t_stm_attribute.u.t_mp3_audio_stm_attr));
            if (pt_mp3_audio_atrbt)
            {
                pt_mp3_audio_atrbt->e_enc = MINFO_INFO_AUD_ENC_MPEG_1;
                switch (pt_mp3_stream_info->ui1_layer)
                {
                    case MP3_MINFO_LAYER_I:
                        pt_mp3_audio_atrbt->e_layer = 1; /* AUD_LAYER_1 */
                        break;
                    case MP3_MINFO_LAYER_II:
                        pt_mp3_audio_atrbt->e_layer = 2; /* AUD_LAYER_2 */
                        break;
                    case MP3_MINFO_LAYER_III:
                        pt_mp3_audio_atrbt->e_layer = 3; /* AUD_LAYER_3 */
                        break;
                    default:
                        pt_mp3_audio_atrbt->e_layer = 0; /* AUD_LAYER_UNKNOWN */
                        break;
                }
                pt_mp3_audio_atrbt->ui4_bitrate = pt_mp3_stream_info->ui4_bitrate;
                pt_mp3_audio_atrbt->ui4_sample_rate = pt_mp3_stream_info->ui4_sample_rate;
                pt_mp3_audio_atrbt->ui1_version = pt_mp3_stream_info->ui1_version;
                pt_mp3_audio_atrbt->ui4_frame_length = pt_mp3_stream_info->ui4_frame_length;
                pt_mp3_audio_atrbt->ui1_channels = pt_mp3_stream_info->ui1_channels;
                return MINFOR_OK;
            }
            else
            {
                return MINFOR_INV_ARG;
            }

        case MINFO_INFO_TYPE_IDX_INFO:
            pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
            pt_minfo_rec->e_info_type = e_info_type;
            pt_minfo_rec->ui1_rec_idx = 1;

            /* only build index table when fle size < MINFO_MP3_MAX_MIDXBULD_SZ */
            if (pt_mp3_stream_info->ui4_mp3_length_in_byte <  MINFO_MP3_MAX_MIDXBULD_SZ
                /*&& pt_mp3_stream_info->ui4_xing_frames != 0*/)
            {
                pt_minfo_rec->u.t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_COMMON;
            }
            else
            {
                pt_minfo_rec->u.t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_NONE;
            }
            pt_minfo_rec->u.t_idx_info.ui8_idx_offset = 0;
#if 0
            /* use ui8_idx_sz to tell index builder that XING frame is exist or NOT */
            pt_minfo_rec->u.t_idx_info.ui8_idx_sz = (pt_mp3_stream_info->ui4_xing_frames != 0) ? 1 : 0;
#else
            pt_minfo_rec->u.t_idx_info.ui8_idx_sz = 0;
#endif
            pt_minfo_rec->u.t_idx_info.ui8_data_offset = pt_mp3_stream_info->ui8_frame_bng_pos;
            pt_minfo_rec->u.t_idx_info.ui8_data_sz = (UINT64)(pt_mp3_stream_info->ui4_mp3_length_in_byte -
                                                                pt_mp3_stream_info->ui8_frame_bng_pos -
                                                                pt_id3_tag_set->ui4_v1_length);
            return MINFOR_OK;
        case MINFO_INFO_TYPE_META_DATA:
            /*Not parse file footer data if filesize not get*/
            if (!pt_minfo_obj->b_filesize_non_gettable)
            {
                mp3_minfo_parse_tag1((VOID*)pt_minfo_obj);
            }
            pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
            pt_minfo_rec->e_info_type = e_info_type;
            pt_minfo_rec->ui1_rec_idx = 1;

            pt_meta_data_info = (MINFO_TYPE_META_DATA_T*)(&(pt_minfo_rec->u.t_meta_data));
            mp3_minfo_cvt_id3_txt(pt_minfo_obj, pt_id3_tag_set->e_language);
            /* Title */
            if(pt_id3_tag_set->at_txt_info[ID3_TAG_V2_TITLE_FRAME_IDX].z_size <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
            {
                COPY_DATA(  pt_id3_tag_set->at_txt_info[ID3_TAG_V2_TITLE_FRAME_IDX].awc_text, 
                            pt_id3_tag_set->at_txt_info[ID3_TAG_V2_TITLE_FRAME_IDX].z_size, 
                            pt_meta_data_info->pwc_str_title, 
                            &(pt_meta_data_info->z_title_len));
            }
            /* Artist */
            if(pt_id3_tag_set->at_txt_info[ID3_TAG_V2_ARTIST_FRAME_IDX].z_size <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
            {
                COPY_DATA(  pt_id3_tag_set->at_txt_info[ID3_TAG_V2_ARTIST_FRAME_IDX].awc_text, 
                            pt_id3_tag_set->at_txt_info[ID3_TAG_V2_ARTIST_FRAME_IDX].z_size, 
                            pt_meta_data_info->pwc_str_artist, 
                            &(pt_meta_data_info->z_artist_len));
            }
            /* Album */
            if(pt_id3_tag_set->at_txt_info[ID3_TAG_V2_ALBUM_FRAME_IDX].z_size <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
            {
                COPY_DATA(  pt_id3_tag_set->at_txt_info[ID3_TAG_V2_ALBUM_FRAME_IDX].awc_text, 
                            pt_id3_tag_set->at_txt_info[ID3_TAG_V2_ALBUM_FRAME_IDX].z_size, 
                            pt_meta_data_info->pwc_str_album, 
                            &(pt_meta_data_info->z_album_len));
            }
            /* Genre */
            if(pt_id3_tag_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].z_size <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
            {
                COPY_DATA(  pt_id3_tag_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text, 
                            pt_id3_tag_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].z_size, 
                            pt_meta_data_info->pwc_str_genre, 
                            &(pt_meta_data_info->z_genre_len));
            }
            /* Track */
            if(pt_id3_tag_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].z_size <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
            {
                COPY_DATA(  pt_id3_tag_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].awc_text, 
                            pt_id3_tag_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].z_size, 
                            pt_meta_data_info->pwc_str_track, 
                            &(pt_meta_data_info->z_track_len));
            }
            /* Year */
            if(pt_id3_tag_set->at_txt_info[ID3_TAG_V2_YEAR_FRAME_IDX].z_size <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
            {
                COPY_DATA(  pt_id3_tag_set->at_txt_info[ID3_TAG_V2_YEAR_FRAME_IDX].awc_text, 
                            pt_id3_tag_set->at_txt_info[ID3_TAG_V2_YEAR_FRAME_IDX].z_size, 
                            pt_meta_data_info->pwc_str_year, 
                            &(pt_meta_data_info->z_year_len));
            }
            /* Duration */
            if(pt_mp3_stream_info->ui4_mp3_length_in_sec == MINFOR_MP3_NO_SIZE)
            {
                pt_meta_data_info->ui8_pb_duration = 0;
            }
            else
            {
                pt_meta_data_info->ui8_pb_duration = (UINT64)pt_mp3_stream_info->ui4_mp3_length_in_sec * 90000;
            }
            /* Bitrate */
            pt_meta_data_info->ui4_bitrate = pt_mp3_stream_info->ui4_bitrate;
            /* Attached PIC */
            pt_pic = &(pt_meta_data_info->t_id3_attatched_pic);
            for (ui4_idx = 0 ; ui4_idx < MINFO_MAX_ID3_PIC_NUM ; ui4_idx++)
            {
                /* only get first item */
                if ((pt_id3_tag_set->ui4_pic_bitmask & ((UINT32)1 << ui4_idx)) != 0)
                {
                    pt_pic->e_pic_type = pt_id3_tag_set->at_pic[ui4_idx].e_pic_type;
                    pt_pic->t_format = pt_id3_tag_set->at_pic[ui4_idx].t_format;
                    pt_pic->ui4_length = pt_id3_tag_set->at_pic[ui4_idx].ui4_length;
                    pt_pic->ui4_starting_offset = pt_id3_tag_set->at_pic[ui4_idx].ui4_starting_offset;
                    break;
                }
            }
            return MINFOR_OK;

        default :
            return MINFOR_INFO_NOT_FOUND;
    }


}

/*-----------------------------------------------------------------------------
 * Name:  mp3_minfo_set_info
 * Description:This function set information to a handler
 * Inputs:  pv_obj       specify the media info object
 * Outputs:
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_minfo_set_info(
    VOID*                           pv_obj,
    MINFO_INFO_TYPE_T               e_info_type,
    MINFO_REC_IDX_T                 ui2_index,
    VOID*                           pv_buf,
    SIZE_T*                         pz_buf_leng)
{
    MINFO_OBJ_T*                    pt_minfo_obj;
    MP3_STREAM_INFO_T*              pt_mp3_stream_info;
    ID3_TAG_SET_T*                  pt_id3_tag_set;

    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    
    pt_mp3_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_mp3_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }

    pt_id3_tag_set = pt_mp3_stream_info->pt_mp3_id3_tag;
    if (pt_id3_tag_set == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }

    if (pv_buf == NULL || pz_buf_leng == NULL)
    {
        return MINFOR_INV_ARG;
    }

    switch (e_info_type)
    {
    case MINFO_INFO_TYPE_SET_LANGUAGE:
        if (*pz_buf_leng != sizeof(ISO_639_LANG_T))
        {
            return MINFOR_INV_ARG;
        }
        x_memcpy(pt_id3_tag_set->e_language, pv_buf, *pz_buf_leng);
        break;
    default:
        break;
    }

    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_minfo_get_num_recs
 *
 * Description:This function get information from a handler
 *
 * Inputs:  pv_obj       specify the media info object
 *
 * Outputs:
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_minfo_get_num_recs(
                                VOID*                pv_obj,
                                MINFO_INFO_TYPE_T    e_info_type,
                                UINT16*              pui2_num_recs)
{
     MINFO_OBJ_T*        pt_minfo_obj;


     /************************************************************************/
     /* Check parameter                                                      */
     /************************************************************************/
     if ((pv_obj == NULL)
         || (pui2_num_recs == NULL))
     {
         return MINFOR_INITIAL_ERR;
     }
     /************************************************************************/
     /* do                                                                   */
     /************************************************************************/
     pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;


     if (pt_minfo_obj->pv_handler_obj)
     {
         if (pt_minfo_obj->b_parsing_successful == FALSE)
         {
             return MINFOR_INFO_NOT_READY;
         }

         switch(e_info_type)
         {
             case MINFO_INFO_TYPE_GENERIC:
                *pui2_num_recs = 1;
                break;
            case MINFO_INFO_TYPE_VIDEO_ATTR:
                *pui2_num_recs = 0;
                break;
            case MINFO_INFO_TYPE_AUDIO_ATTR:
                *pui2_num_recs = 1;
                break;
            case MINFO_INFO_TYPE_SP_ATTR:
                *pui2_num_recs = 0;
                break;
            case MINFO_INFO_TYPE_IDX_INFO:
                *pui2_num_recs = 1;
                 break;
             default:
              return MINFOR_INFO_NOT_FOUND;
         }
         return MINFOR_OK;
     }
     return MINFOR_FMT_NOT_SUPPORT;
}


