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
 * asf_parser.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * willyhuang
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the asf parser function
           Author: lianming mcn07123 
           History:
           (1)2008-3-10  lianming mcn07123  initial

 *---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"  {
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

#include "inc/u_common.h"
#include "handle/u_handle.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "unicode/x_uc_str.h"
#include "dbg/x_dbg.h"
#include "asf_minfo.h"
#include "mutil/mm_util.h"
#include "asf_parser_common.h"

#else

#include "x_os.h"
#include "u_common.h"
#include "u_handle.h"
#include "../../mm_util.h"
#include "x_uc_str.h"
#include "x_dbg.h"
#include "asf_minfo.h"
#include "asf_parser_common.h"

#endif


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
                    data definition
 ----------------------------------------------------------------------------*/

/*define asf standard object GUID*/
const ASF_GUID_T gt_asf_object_null_guid =
{
    0x00000000,
    0x0000,
    0x0000,
    { 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x00 }
};


const ASF_GUID_T gt_asf_object_header_guid =
{0x75B22630, 0x668E, 0x11CF, {0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C}};
const ASF_GUID_T gt_asf_object_data_guid =
{0x75B22636, 0x668E, 0x11CF, {0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C}};
const ASF_GUID_T gt_asf_object_file_properties_guid =
{0x8cabdca1, 0xa947, 0x11cf, {0x8e, 0xe4, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65}};
const ASF_GUID_T gt_asf_object_stream_properties_guid =
{0xB7DC0791, 0xA9B7, 0x11CF, {0x8E, 0xE6, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65}};
const ASF_GUID_T gt_asf_object_header_extension_guid =
{0x5FBF03B5, 0xA92E, 0x11CF, {0x8E, 0xE3, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65}};
const ASF_GUID_T gt_asf_object_stream_type_audio =
{0xF8699E40, 0x5B4D, 0x11CF, {0xA8, 0xFD, 0x00, 0x80, 0x5F, 0x5C, 0x44, 0x2B}};
const ASF_GUID_T gt_asf_object_stream_type_video =
{0xbc19efc0, 0x5B4D, 0x11CF, {0xA8, 0xFD, 0x00, 0x80, 0x5F, 0x5C, 0x44, 0x2B}};
const ASF_GUID_T gt_asf_guid_no_error_correction =
{0x20FB5700, 0x5B55, 0x11CF, {0xA8, 0xFD, 0x00, 0x80, 0x5F, 0x5C, 0x44, 0x2B}};
const ASF_GUID_T gt_asf_guid_audio_spread =
{0xBFC3CD50, 0x618F, 0x11CF, {0x8B, 0xB2, 0x00, 0xAA, 0x00, 0xB4, 0xE2, 0x20}};
const ASF_GUID_T gt_asf_guid_reserved_1 =
{0xABD3D211, 0xA9BA, 0x11cf, {0x8E, 0xE6, 0x00, 0xC0, 0x0C ,0x20, 0x53, 0x65}};
const ASF_GUID_T gt_asf_guid_reserved_2 =
{0x86D15241, 0x311D, 0x11D0, {0xA3, 0xA4, 0x00, 0xA0, 0xC9 ,0x03, 0x48, 0xF6}};

const ASF_GUID_T gt_asf_guid_language_list =
{0x7C4346A9, 0xEFE0, 0x4BFC, {0xB2, 0x29, 0x39, 0x3E, 0xDE, 0x41, 0x5C, 0x85}};
const ASF_GUID_T gt_asf_object_codec_list_guid =
{0x86D15240, 0x311D, 0x11D0, {0xA3, 0xA4, 0x00, 0xA0, 0xC9, 0x03, 0x48, 0xF6}};
const ASF_GUID_T gt_asf_object_codec_list_reserved_guid =
{0x86D15241, 0x311D, 0x11D0, {0xA3, 0xA4, 0x00, 0xA0, 0xC9, 0x03, 0x48, 0xF6}};
const ASF_GUID_T gt_asf_object_content_description_guid =
{0x75B22633, 0x668E, 0x11CF, {0xa6, 0xd9, 0x00, 0xaa, 0x00, 0x62, 0xce, 0x6c}};
const ASF_GUID_T gt_asf_object_ext_content_description_guid =
{0xD2D0A440, 0xE307, 0x11D2, {0x97, 0xF0, 0x00, 0xA0, 0xC9, 0x5E, 0xA8, 0x50}};
const ASF_GUID_T gt_asf_object_extended_stream_properties =
{0x14E6A5CB, 0xC672, 0x4332, {0x83, 0x99, 0xA9, 0x69, 0x52, 0x06, 0x5B, 0x5A}};

const ASF_GUID_T gt_asf_object_index_parameters_guid =
{0xD6E229DF, 0x35DA, 0x11D1, {0x90, 0x34, 0x00, 0xA0, 0xC9, 0x03, 0x49, 0xBE}};

const ASF_GUID_T gt_asf_object_simple_index_guid =
{0x33000890, 0xE5B1, 0x11CF, {0x89, 0xF4, 0x00, 0xA0, 0xC9, 0x03, 0x49, 0xCB}};
const ASF_GUID_T gt_asf_object_index_guid =
{0xD6E229D3, 0x35DA, 0x11D1, {0x90, 0x34, 0x00, 0xA0, 0xC9, 0x03, 0x49, 0xBE}};

const ASF_GUID_T gt_asf_object_metadata_guid =
{0xC5F8CBEA, 0x5BAF, 0x4877, {0x84, 0x67, 0xAA, 0x8C, 0x44, 0xFA, 0x4C, 0xCA}};
const ASF_GUID_T gt_asf_object_index_paras_guid =
{0xD6E229EF, 0x35DA, 0x11D1, {0x90, 0x34, 0x00, 0xA0, 0xC9, 0x03, 0x49, 0xBE}};
const ASF_GUID_T gt_asf_object_bitrate_properties_guid =
{0x7bf875ce, 0x468d, 0x11d1, {0x8d, 0x82, 0x00, 0x60, 0x97, 0xc9, 0xa2, 0xb2}};

const ASF_GUID_T gt_asf_object_content_encryption_guid =
{0x2211b3fb, 0xbd23, 0x11d2, {0xb4, 0xb7, 0x00, 0xa0, 0xc9, 0x55, 0xfc, 0x6e}};

/*for extended AC3 codec*/
const ASF_GUID_T gt_asf_codec_ext_ac3_guid =
{0xA7FB87AF, 0x2D02, 0x42FB, {0xA4, 0xD4, 0x05, 0xCD, 0x93, 0x84, 0x3B, 0xDD}};

/*-----------------------------------------------------------------------------
                     functions declarations
----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name:  _asf_get_guid
 *
 * Description: This function gets a object's GUID from a buffer p_data
 *
 * Inputs:  p_data       specify the buffer
 *
 * Outputs: p_guid       a object's GUID
 *
 * Returns: NULL
 * 
 * Note : this function do not check memory boundary , be careful no less than 
 *        16 bytes!
 * Author : lianming lin(mcn07123)
 *
 * History:
 *   (1)2008-3-12 : initial
 ----------------------------------------------------------------------------*/

VOID _asf_get_guid( ASF_GUID_T* pt_guid, UINT8* puc_data )
{
    if ((NULL == pt_guid) || (NULL == puc_data))
    {
        return;
    }
    
    ASF_GET_DWORD(pt_guid->ui4_v1, puc_data );    /*i4_v1: 4 bytes*/
    ASF_GET_WORD(pt_guid->ui2_v2, puc_data + 4);  /*i2_v2: 2 bytes*/
    ASF_GET_WORD(pt_guid->ui2_v3, puc_data + 6);  /*i2_v3: 2 bytes*/
    /*auc_v4: 8 bytes*/
    x_memcpy( (VOID *)(pt_guid->auc_v4), (VOID *)(puc_data + 8), 8 ); 
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_guid_to_str
 *
 * Description: This function translates a object's GUID into a string
 *
 * Inputs:  pt_guid       a object's GUID
 *          pui4_len      buffer length
 *          psz_str       buffer address
 *
 * Outputs: psz_str       string format of GUID
 *
 * Returns: NULL
 *
 * Note : this function check memory boundary , be careful no less than 
 *        37 bytes!
 * Author : lianming lin(mcn07123)
 *
 * History:
    (1)2008-3-12 : initial
 ----------------------------------------------------------------------------*/

VOID _asf_guid_to_str( ASF_GUID_T* pt_guid, CHAR* psz_str, UINT32* pui4_len)
{
#ifndef __KERNEL__
    UINT32 ui4_count = 0;
    UINT32 ui4_temp  = 0;
    
    if ((NULL == pt_guid) || (NULL == psz_str))
    {
        DBG_ERROR(("_asf_guid_to_str: NULL\r\n"));
        return;
    }

    /*format:5FBF03B5-A92E-11CF-8EE3-00C00C205365*/
    if (*pui4_len < 40)
    {
        DBG_ERROR(("_asf_guid_to_str: at least 37 char spaces\r\n"));
        return;
    }

    ui4_count += x_sprintf(psz_str, 
                           "%08x-%04x-%04x", 
                           pt_guid->ui4_v1, 
                           pt_guid->ui2_v2,
                           pt_guid->ui2_v3);

    ui4_count += x_sprintf(psz_str + ui4_count, 
                           "-%02x", 
                           pt_guid->auc_v4[0]);
     
    ui4_count += x_sprintf(psz_str + ui4_count, 
                           "%02x-", 
                           pt_guid->auc_v4[1]);
    
    for (ui4_temp= 2; ui4_temp < 8; ++ui4_temp)
    {
        ui4_count += x_sprintf(psz_str + ui4_count, 
                               "%02x", 
                               pt_guid->auc_v4[ui4_temp]);
    }
    psz_str[ui4_count] = 0;
    *pui4_len = ui4_count;
#else
    UNUSED(pt_guid);
    UNUSED(psz_str);
    UNUSED(pui4_len);
    return;
#endif    
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_cmp_guid
 *
 * Description: This function compares two GUIDs whether is equal or not
 *
 * Inputs:  pt_guid1   one GUID
 *          pt_guid2   another one
 *
 * Outputs: NULL 
 *
 * Returns: ASFR_GUID_EQUAL       if the two are the same
 *          ASFR_GUID_NOT_EQUAL   if the two are different
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *  (1)2008-3-12 : initial

 ----------------------------------------------------------------------------*/

INT32 _asf_cmp_guid( const ASF_GUID_T* pt_guid1, const ASF_GUID_T* pt_guid2 )
{

    if ((NULL == pt_guid1) || (NULL == pt_guid2))
    {
        return ASFR_INV_ARG;
    }
    
    if ((pt_guid1->ui4_v1 != pt_guid2->ui4_v1) 
        || (pt_guid1->ui2_v2 != pt_guid2->ui2_v2)
        || (pt_guid1->ui2_v3 != pt_guid2->ui2_v3)
        || ( x_memcmp( pt_guid1->auc_v4, pt_guid2->auc_v4,8 )))/*auc_v4: 8 bytes*/
    {
        return ASFR_GUID_NOT_EQUAL;   /* unmatched  */
    }
    
    return ASFR_GUID_EQUAL;   /* match */
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_lang_rfc1766_to_639
 *
 * Description: transfert FourCC to our private enumerate value
 *
 * Inputs:  pwsz_lang 
 *          ui4_len   bytes
 *          
 * Outputs: t_iso_639_lang 
 * 
 * Returns: NULL
 *
 * Note:  NULL
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
    (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/

 VOID _asf_lang_rfc1766_to_639( UINT8*          psz_lang,  
                                UINT32          ui4_len, 
                                ISO_639_LANG_T* pt_iso_639_lang )
{
   
    CHAR*   psz_language;
    UINT16  ui2_temp;
    
    DBG_INFO(("_asf_lang_rfc1766_to_639:begin lang_rfc1766_to_639\r\n"));
   
    if ((NULL == psz_lang) || (NULL == pt_iso_639_lang))
    {
        DBG_ERROR(("_asf_lang_rfc1766_to_639:para is NULL\r\n"));
        return;
    }

    /* RFC1766 syntax:
       Language-Tag = Primary-tag *( "-" Subtag )
       Primary-tag = 1*8ALPHA
       Subtag = 1*8ALPHA

       (1)In the primary language tag:
         All 2-letter tags are interpreted according to ISO standard
         639, "Code for the representation of names of languages" [ISO639].
       (2)In the first subtag:
         All 2-letter codes are interpreted as ISO 3166 alpha-2
         country codes denoting the area in which the language is
         used.
   */ 

    /*get primary tag*/
    psz_language = (CHAR*)pt_iso_639_lang;
    ASF_GET_WORD(ui2_temp, psz_lang);
    psz_language[0] = (CHAR)ui2_temp;
    ASF_GET_WORD(ui2_temp, psz_lang + 2);
    psz_language[1] = (CHAR)ui2_temp;
    psz_language[2] = 0;
    psz_language[3] = 0;
    
    DBG_INFO(("_asf_lang_rfc1766_to_639:end lang_rfc1766_to_639\r\n"));
    
    return ;
}


/********************simulate clip buffer *************************************/
#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name:  _cli_asf_mcopybytes_fct
 *
 * Description: memory clip copybytes function
 *
 * Inputs:  pv_feeder_obj
 *          pv_buf
 *          z_buf_leng
 *          z_copy_size
 *          pui4_size
 *          
 * Outputs: NULL
 *
 * Returns: copied bytes number     if successful
 *          ASFR_CLIP_ERROR         if fail
 ----------------------------------------------------------------------------*/                            
UINT32 _cli_asf_mcopybytes_fct( VOID*  pv_feeder_obj, 
                                VOID*  pv_buf, 
                                SIZE_T z_buf_leng, 
                                SIZE_T z_copy_size,
                                UINT32* pui4_size)
{
    SIZE_T        z_copy;
    ASF_CLIP_OBJ* pt_clip_obj;  
    CHAR*         pc_current_pt;
    
    if ((NULL == pv_feeder_obj) || (NULL == pv_buf))
    {
        return ASFR_CLIP_ERROR;
    }
    
    if (z_buf_leng < z_copy_size)
    {
        z_copy = z_buf_leng;
    }
    else
    {
        z_copy = z_copy_size;
    }
    
    /*get clip object*/
    pt_clip_obj = (ASF_CLIP_OBJ*)pv_feeder_obj;

    /*current position*/
    pc_current_pt = (CHAR*)pt_clip_obj->pv_clip_buf 
                    + pt_clip_obj->i8_buff_pos;
    
    if ((pt_clip_obj->i8_clip_len - pt_clip_obj->i8_buff_pos) 
        < (INT64)z_copy)
    {
        z_copy = (SIZE_T)(pt_clip_obj->i8_clip_len - pt_clip_obj->i8_buff_pos);
    }
    /*copy content*/
    x_memcpy(pv_buf, (VOID*)pc_current_pt, z_copy);
    pt_clip_obj->i8_buff_pos = pt_clip_obj->i8_buff_pos + z_copy;

    *pui4_size = z_copy;
    
    return ASFR_CLIP_OK;
}



/*-----------------------------------------------------------------------------
 * Name:  _cli_asf_msetpos_fct
 *
 * Description: memory clip set postion function
 *
 * Inputs:  pv_feeder_obj
 *          i8_offset
 *          ui1_whence
 *          
 * Outputs: NULL
 *
 * Returns: ASFR_CLIP_OK       if successful
 *          ASFR_CLIP_ERROR    if fail
 ----------------------------------------------------------------------------*/                            
INT32 _cli_asf_msetpos_fct(VOID* pv_feeder_obj, 
                            INT64 i8_offset, 
                            UINT8 ui1_whence)
{
    ASF_CLIP_OBJ*  pt_mfmtrecg_obj;  

    if (NULL == pv_feeder_obj)
    {
        return ASFR_CLIP_ERROR;
    }
    
    pt_mfmtrecg_obj = (ASF_CLIP_OBJ*)pv_feeder_obj;
    
    switch (ui1_whence)
    {
        case FM_SEEK_BGN:
            pt_mfmtrecg_obj->i8_buff_pos = i8_offset;
            break;
            
        case FM_SEEK_CUR: 
            pt_mfmtrecg_obj->i8_buff_pos = pt_mfmtrecg_obj->i8_buff_pos 
                                          + i8_offset;
            break;
            
        case FM_SEEK_END:
            pt_mfmtrecg_obj->i8_buff_pos = pt_mfmtrecg_obj->i8_clip_len 
                                          + i8_offset;
            break;
            
    }
    
    if (pt_mfmtrecg_obj->i8_buff_pos < 0)
    {
        pt_mfmtrecg_obj->i8_buff_pos = 0;
    }
    else if (pt_mfmtrecg_obj->i8_buff_pos > pt_mfmtrecg_obj->i8_clip_len)
    {
        pt_mfmtrecg_obj->i8_buff_pos = pt_mfmtrecg_obj->i8_clip_len;            
    }
    
    return ASFR_CLIP_OK;

}

/*-----------------------------------------------------------------------------
 * Name:  _cli_asf_mgetpos_fct
 *
 * Description: memory clip get postion function
 *
 * Inputs:  pv_feeder_obj
 *          pui8_cur_pos
 *          
 * Outputs: NULL
 *
 * Returns: current position       if successful
 *          ASFR_CLIP_ERROR        if fail
 ----------------------------------------------------------------------------*/  
INT32 _cli_asf_mgetpos_fct( VOID* pv_feeder_obj, UINT64* pui8_cur_pos )
{
    ASF_CLIP_OBJ*  pt_mfmtrecg_obj;  
    
    if (NULL == pv_feeder_obj)
    {
        return ASFR_CLIP_ERROR;
    }
    
    pt_mfmtrecg_obj = (ASF_CLIP_OBJ*)pv_feeder_obj;
    
    *pui8_cur_pos = (UINT64)(pt_mfmtrecg_obj->i8_buff_pos);
    return ASFR_CLIP_OK; 
}

/*-----------------------------------------------------------------------------
 * Name:  _cli_asf_get_obj
 *
 * Description: get buffer clip simulate object
 *
 * Inputs:  psz_path_name   filename
 *          
 * Outputs: ppt_feeder_obj
 *
 * Returns: ASFR_CLIP_OK           if successful
 *          ASFR_CLIP_ERROR        if fail
 ----------------------------------------------------------------------------*/                            
INT32 _cli_asf_get_obj( CHAR* psz_path_name , ASF_CLIP_OBJ** ppt_feeder_obj )
{

#ifndef __NO_FM__

    FM_FILE_INFO_T  t_fm_info;
    VOID*           pv_clip_buf;
    SIZE_T          t_buf_size;
    HANDLE_T        h_file;
    UINT64          ui8_cur_pos;    
    UINT32          ui4_read;  
    INT32           i4_ret;
    ASF_CLIP_OBJ*   pt_clip_obj = NULL; 
    
    /*check parameters*/
    if ((NULL == psz_path_name) || (NULL == ppt_feeder_obj))
    {
        DBG_ERROR(("_cli_asf_get_obj:NULL\r\n"));
        return ASFR_CLIP_ERROR;
    }
    
    /* Open file */
    i4_ret = x_fm_open(FM_ROOT_HANDLE, 
                       psz_path_name, 
                       FM_READ_ONLY, 
                       0777,
                       FALSE, 
                       &h_file);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("_cli_asf_get_obj: Failed to open %s (%d)!\r\n", 
                   psz_path_name, 
                   i4_ret));
        return ASFR_CLIP_ERROR;
    }
    
    /*get file size info*/
    x_fm_get_info_by_handle(h_file, &t_fm_info);
    t_buf_size = (SIZE_T)t_fm_info.ui8_size;
    
    /*allocate clip buffer according to file size*/
    pv_clip_buf = _mm_util_mem_alloc((UINT32)t_buf_size);
    if (NULL == pv_clip_buf)
    {
        DBG_ERROR(("_cli_asf_get_obj:Fail to allocate memory\r\n"));
        x_fm_close(h_file);
        return  ASFR_CLIP_ERROR;
    }
    
    /*read file to a clip buffer*/
    i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
    if (FMR_OK == i4_ret)
    {
        i4_ret = x_fm_read(h_file, pv_clip_buf, t_buf_size, 
                           &ui4_read);
        if (i4_ret != FMR_OK)
        {
            DBG_ERROR(("_cli_asf_get_obj: x_fm_read %s "
                       "failed(%d)\r\n",
                       psz_path_name,
                       i4_ret));
            if (NULL != pv_clip_buf)
            {
                _mm_util_mem_free(pv_clip_buf);
                pv_clip_buf = NULL;
            }
            x_fm_close(h_file);
            return ASFR_CLIP_ERROR;
        }
    }
    else
    {
        if (NULL != pv_clip_buf)
        {
            _mm_util_mem_free(pv_clip_buf);
            pv_clip_buf = NULL;
        }
        x_fm_close(h_file);
        return ASFR_CLIP_ERROR;
    }

    /*close file*/
    x_fm_close(h_file);   

    pt_clip_obj = _mm_util_mem_alloc(sizeof(ASF_CLIP_OBJ));
    if (NULL == pt_clip_obj)
    {
        if (NULL != pv_clip_buf)
        {
            _mm_util_mem_free(pv_clip_buf);
            pv_clip_buf = NULL;
            DBG_ERROR(("_cli_asf_get_obj:Fail to allocate memory for clip\r\n"));
            return ASFR_CLIP_ERROR;
        }
    }
    /*construct feeder object*/
    x_memset(pt_clip_obj, 0, sizeof(ASF_CLIP_OBJ));
    pt_clip_obj->i8_buff_pos = MEDIA_SEEK_BGN;
    pt_clip_obj->pv_clip_buf =  pv_clip_buf;
    pt_clip_obj->i8_clip_len =  (INT64)t_buf_size;
    
    /*set handler as the pointer to t_feeder_obj ,here use UINT32 store address*/
    pt_clip_obj->t_src_feeder.h_feeder = ASF_CLIP_HANDLE_FOR_CLI;
    /*asf only use copybytes,setpos,getpos*/
    pt_clip_obj->t_src_feeder.t_feeder_le_fct_tbl.pf_copybytes = 
                                            (feeder_copybytes_fct)_cli_asf_mcopybytes_fct;
    pt_clip_obj->t_src_feeder.t_feeder_le_fct_tbl.pf_set_pos = 
                                            (feeder_setpos_fct)_cli_asf_msetpos_fct;
    pt_clip_obj->t_src_feeder.t_feeder_le_fct_tbl.pf_get_pos =
                                            (feeder_getpos_fct)_cli_asf_mgetpos_fct;
    
    *ppt_feeder_obj = pt_clip_obj;
    
    return ASFR_CLIP_OK;
#else
    return ASFR_CLIP_ERROR;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  _cli_asf_free_obj
 *
 * Description: free buffer clip simulate object
 *
 * Inputs:  
 *          
 * Outputs: ppt_feeder_obj
 *
 * Returns: NULL
 ----------------------------------------------------------------------------*/                            
VOID _cli_asf_free_obj(ASF_CLIP_OBJ** ppt_clip_obj)
{
    VOID*           pv_clip_buf;
    
    if ((NULL != ppt_clip_obj) && (NULL != *ppt_clip_obj))
    {

        pv_clip_buf = (*ppt_clip_obj)->pv_clip_buf;
        if (NULL != pv_clip_buf)
        {
            _mm_util_mem_free(pv_clip_buf);
            pv_clip_buf = NULL;
        }
        
        _mm_util_mem_free(*ppt_clip_obj);
        *ppt_clip_obj = NULL;
    }
    
    return;
}

#endif


/*-----------------------------------------------------------------------------
 * Name:  _asf_get_value_by_len_type
 *
 * Description: get value by  length type
 *
 * Inputs:  ppui1_head:  2-level buffer pointer 
 *          ui1_len_type: length type 00/01/10/11
 *          
 * Outputs: ppui1_head
 *
 * Returns: value
 ----------------------------------------------------------------------------*/                            

UINT32 _asf_get_value_by_len_type(UINT8** ppui1_head,
                                 UINT8   ui1_len_type)
{
    UINT32 ui4_ret;

    /*parameter checking*/
    if ((NULL == ppui1_head) || (NULL == *ppui1_head))
    {
        ui4_ret =  0;
        
    }
    else if (ASF_LEN_NULL == ui1_len_type)
    {
        ui4_ret = 0;
    }
    else if (ASF_LEN_BYTE == ui1_len_type)
    {
        ASF_GET_BYTE(ui4_ret, (*ppui1_head));
        ++(*ppui1_head);
    }
    else if (ASF_LEN_WORD == ui1_len_type)
    {
        ASF_GET_WORD(ui4_ret, (*ppui1_head));       
        (*ppui1_head) += 2;
    }
    else if (ASF_LEN_DWORD == ui1_len_type)
    {
        ASF_GET_DWORD(ui4_ret, (*ppui1_head));       
        (*ppui1_head) += 4;
    }
    else
    {
        ui4_ret =  0;

    }    
    return ui4_ret;

}

/*-----------------------------------------------------------------------------
 * Name:  _asf_bit_map_set
 *
 * Description: set bit 
 *
 * Inputs:  pui1_map:  bit-map header 
 *             ui4_len: bit map length
 *             ui4_idx: bit-map index :from 1 ...
 *          
 * Outputs: pui1_map
 *
 * Returns: value
 ----------------------------------------------------------------------------*/                            
INT32 _asf_bit_map_set( UINT8* pui1_map, UINT32 ui4_len, UINT32 ui4_idx )
{
    UINT32          ui4_bit_map_arr_idx;
    UINT32          ui4_bit_idx;
    UINT8*          pui1_idx_strm_map;
    
    if ( ( !pui1_map ) || ( ui4_len == 0 ) || ( ui4_idx > ui4_len * 8 ) )
    {
        return ASFR_INV_ARG;
    }
    
    pui1_idx_strm_map = pui1_map;
    
    ui4_bit_map_arr_idx = ui4_len - ( ( ui4_idx - 1 )  / 8 ) ;
    ui4_bit_idx         = ( ui4_idx - 1 ) % 8;
    pui1_idx_strm_map[ui4_bit_map_arr_idx -1 ] |= ( 1 << ui4_bit_idx );
    
    return ASFR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _asf_bit_map_is_set
 *
 * Description: if  bit is set
 *
 * Inputs:  pui1_map:  bit-map header 
 *             ui4_len: bit map length
 *             ui4_idx: bit-map index :from 1 ...
 *          
 * Outputs: 
 *
 * Returns: value
 ----------------------------------------------------------------------------*/                            
BOOL _asf_bit_map_is_set( UINT8* pui1_map, UINT32 ui4_len, UINT32 ui4_idx )
{
    UINT32          ui4_bit_map_arr_idx;
    UINT32          ui4_bit_idx;
    UINT8*          pui1_idx_strm_map;
    
    if ( ( !pui1_map ) || ( ui4_len == 0 ) || ( ui4_idx > ui4_len * 8 ) )
    {
        return FALSE;
    }
    
    pui1_idx_strm_map = pui1_map;
    
    ui4_bit_map_arr_idx = ui4_len - ( ( ui4_idx - 1 ) / 8 );
    ui4_bit_idx         = ( ui4_idx - 1 ) % 8;
    if ( ( pui1_idx_strm_map[ui4_bit_map_arr_idx -1 ] & ( 1 << ui4_bit_idx ) ) != 0 )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

UINT32 _asf_wide_char_2_str( UTF16_T* pui2_wide_char, 
                                    UINT32   ui4_len,
                                    CHAR*    pc_str, 
                                    UINT32   ui4_buf_len )
{
    UINT32   ui4_cnt;
    
    if ( ( NULL == pui2_wide_char )
        || ( NULL == pc_str )
        || ( 0 == ui4_len ) 
        || ( 0 == ui4_buf_len ) )
    {
        return 0;
    }

    for ( ui4_cnt = 0; ( ui4_cnt < ui4_len ) && ( ui4_cnt < ui4_buf_len ); ui4_cnt++ )
    {
        pc_str[ui4_cnt] = ( CHAR )pui2_wide_char[ui4_cnt];
    }

    pc_str[ui4_cnt] = 0;

    return ui4_cnt;
}


#ifdef __cplusplus
}
#endif
