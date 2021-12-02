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
 * $RCSfile: mp3_id3_parser.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0916b8010fb2d9ebf453b3d4efdbc5e9 $ Asa
 *
 * Description:
 *         This file exports public API of the mp3 id3 parser function
 *---------------------------------------------------------------------------*/

#ifndef _MP3_ID3_PARSER_H_
#define _MP3_ID3_PARSER_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/minfo/minfo.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "mutil/mm_util_table.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "../minfo.h"
#include "../../mfmtrecg/u_mfmtrecg.h"
#include "../../mm_util_table.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MAX_ID3_TAG_V1_FRAME_LENGTH             30
#define MAX_ID3_TAG_V2_PIC_DESCIPTION_LENGTH    64
#define MAX_ID3_TAG_GENRE_NUM                   126

#define MAX_ID3_TAG_TXT_FRAME_LENGTH            64
#define MAX_ID3_PIC_NUM                         20

/* txt */
#define ID3_TAG_V2_TITLE_FRAME_IDX      0
#define ID3_TAG_V2_ARTIST_FRAME_IDX     1
#define ID3_TAG_V2_ALBUM_FRAME_IDX      2
#define ID3_TAG_V2_YEAR_FRAME_IDX       3
#define ID3_TAG_V2_LENG_FRAME_IDX       4
#define ID3_TAG_V2_GENRE_FRAME_IDX      5
#define ID3_TAG_V2_TRACK_FRAME_IDX      6
#define ID3_TAG_V2_MAX_TXT_FRAME_IDX    7    /*indicate the lastest txt idx*/

#define ID3_TAG_V2_APIC_FRAME_IDX       7
#define ID3_TAG_V2_MAX_FRAME_IDX        8    /*indicate the lastest idx*/

typedef enum
{
    MP3_ID3_UTF16_BE,
    MP3_ID3_UTF16_LE
}MP3_ID3_TAG_UTF16_BYTE_ORDER_T;

typedef enum
{
    MP3_ID3_FRAME_TEXT_ENCODING_ISO_8859_1,
    MP3_ID3_FRAME_TEXT_ENCODING_UTF16_W_BOM,
    MP3_ID3_FRAME_TEXT_ENCODING_UTF16_WO_BOM,
    MP3_ID3_FRAME_TEXT_ENCODING_UTF8
}MP3_ID3_FRAME_TEXT_ENCODING_T;

typedef struct _ID3_TAG_TXT_FRAME_T
{
    UTF16_T awc_text[MAX_ID3_TAG_TXT_FRAME_LENGTH+1];/*NULL terminator*/
    SIZE_T z_size;    /* include null terminator */
    MP3_ID3_FRAME_TEXT_ENCODING_T e_encoding;
}ID3_TAG_TXT_FRAME_T;

typedef MINFO_INFO_PIC_T ID3_TAG_PIC_FRAME_T;

typedef struct _ID3_TAG_SET_T
{
    /* ID3 data */
    ID3_TAG_TXT_FRAME_T     at_txt_info[ID3_TAG_V2_MAX_TXT_FRAME_IDX];
    UINT32                  ui4_pic_bitmask;
    ID3_TAG_PIC_FRAME_T     at_pic[MAX_ID3_PIC_NUM];
    ISO_639_LANG_T          e_language;
    /* ID3 frame */
    UINT32                  ui4_frame_id;
    UINT32                  ui4_frame_size;
    UINT16                  ui2_frame_flag;
    BOOL                    b_v2_frame_unsync;
    /*ID3 info*/
    UINT32                  ui4_v2_length;
    UINT32                  ui4_v1_length;
    UINT8                   ui1_v2_version;
    BOOL                    b_v2_has_unsync;
    BOOL                    b_v2_has_exthdr;
    BOOL                    b_v2_has_footer;
    UINT32                  ui4_v2_tag_size;  /* not include header & footer size (original size) */
    /*dumy for unsynchronisation*/
    UINT8                   ui1_pre_char;
    /* parsing done? */
    BOOL                    b_is_parsing_done;
    /* has tag and is valid*/
    BOOL                    b_is_valid;
}ID3_TAG_SET_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
 
/*  convert mp3 id3 tag text to Unicode 16 function */
extern INT32 mp3_minfo_cvt_id3_txt (MINFO_OBJ_T*    pt_minfo_obj,  
                                        ISO_639_LANG_T   e_language);
/*  parse mp3 id3 tag function */
extern INT32 mp3_minfo_parse_id3 (MINFO_OBJ_T* pt_minfo_obj,
                                       BOOL b_parse_v2_tag);

#endif /* _MP3_ID3_PARSER_H_ */

