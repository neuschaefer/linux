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
 * $RCSfile: mp3_id3_parser.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description:
 *         This file contains the implementation of the mp3 id3 parser function
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
#include "handle/u_handle.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/x_minfo.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "unicode/x_uc_str.h"
#include "mp3_id3_parser.h"
#include "mp3_minfo.h"

#else

#include "x_os.h"

#include "u_common.h"
#include "u_handle.h"
#include "../minfo.h"
#include "x_minfo.h"
#include "x_uc_str.h"
#include "mp3_id3_parser.h"
#include "mp3_minfo.h"
#include "../../mm_util.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define MP3_ID3_TAG_ASCII_OFFSET                                   ((UINT8) 48)

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

const static UINT32 cui4_TAG = 0x00544147; /* "TAG" */
const static UINT32 cui4_ID3 = 0x00494433; /* "ID3" */
#ifdef MM_MINFO_ID3_CH_KOR_SUPPORT
const static ISO_639_LANG_T ce_korean              = "kor";
const static ISO_639_LANG_T ce_simplified_chinese  = "chi";
const static ISO_639_LANG_T ce_traditional_chinese = "zho";
const static ISO_639_LANG_T ce_japanese            = "jep";
#endif
/* ID3 tag v2 frame id -- title, artist, album, year, length, attached pic */
static UINT32 aui4_mp3_id3_tag_v2_2_frame_id_list[ID3_TAG_V2_MAX_FRAME_IDX] = {
    0x545432,        /* "TT2" */
    0x545031,        /* "TP1" */
    0x54414c,        /* "TAL" */
    0x545945,        /* "TYE" */
    0x544c45,        /* "TLE" */
    0x54434f,        /* "TCO" */
    0x54524b,        /* "TRK" */
    0x504943         /* "PIC" */
};

static UINT32 aui4_mp3_id3_tag_v2_3_frame_id_list[ID3_TAG_V2_MAX_FRAME_IDX] = {
    0x54495432,      /* "TIT2" */
    0x54504531,      /* "TPE1" */
    0x54414c42,      /* "TALB" */
    0x54594552,      /* "TYER" */
    0x544c454e,      /* "TLEN" */
    0x54434f4e,      /* "TCON" */
    0x5452434b,      /* "TRCK" */
    0x41504943       /* "APIC" */
};

static UINT32 aui4_mp3_id3_tag_v2_4_frame_id_list[ID3_TAG_V2_MAX_FRAME_IDX] = {
    0x54495432,      /* "TIT2" */
    0x54504531,      /* "TPE1" */
    0x54414c42,      /* "TALB" */
    0x54594552,      /* "TYER" */
    0x544c454e,      /* "TLEN" */
    0x54434f4e,      /* "TCON" */
    0x5452434b,      /* "TRCK" */
    0x41504943       /* "APIC" */
};

static const char *aps_mp3_id3_tag_genres[] =
{
    "Blues",                /* 0. Blues              */
    "Classic Rock",         /* 1. Classic Rock       */
    "Country",              /* 2. Country            */
    "Dance",                /* 3. Dance              */
    "Disco",                /* 4. Disco              */
    "Funk",                 /* 5. Funk               */
    "Grunge",               /* 6. Grunge             */
    "Hip-Hop",              /* 7. Hip-Hop            */
    "Jazz",                 /* 8. Jazz               */
    "Metal",                /* 9. Metal              */
    "New Age",              /* 10. New Age           */
    "Oldies",               /* 11. Oldies            */
    "Other",                /* 12. Other             */
    "Pop",                  /* 13. Pop               */
    "R&B",                  /* 14. R&B               */
    "Rap",                  /* 15. Rap               */
    "Reggae",               /* 16. Reggae            */
    "Rock",                 /* 17. Rock              */
    "Techno",               /* 18. Techno            */
    "Industrial",           /* 19. Industrial        */
    "Alternative",          /* 20. Alternative       */
    "Ska",                  /* 21. Ska               */
    "Death Metal",          /* 22. Death Metal       */
    "Pranks",               /* 23. Pranks            */
    "Soundtrack",           /* 24. Soundtrack        */
    "Euro-Techno",          /* 25. Euro-Techno       */
    "Ambient",              /* 26. Ambient           */
    "Trip-Hop",             /* 27. Trip-Hop          */
    "Vocal",                /* 28. Vocal             */
    "Jazz+Funk",            /* 29. Jazz+Funk         */
    "Fusion",               /* 30. Fusion            */
    "Trance",               /* 31. Trance            */
    "Classical",            /* 32. Classical         */
    "Instrumental",         /* 33. Instrumental      */
    "Acid",                 /* 34. Acid              */
    "House",                /* 35. House             */
    "Game",                 /* 36. Game              */
    "Sound Clip",           /* 37. Sound Clip        */
    "Gospel",               /* 38. Gospel            */
    "Noise",                /* 39. Noise             */
    "AlternRock",           /* 40. AlternRock        */
    "Bass",                 /* 41. Bass              */
    "Soul",                 /* 42. Soul              */
    "Punk",                 /* 43. Punk              */
    "Space",                /* 44. Space             */
    "Meditative",           /* 45. Meditative        */
    "Instrumental Pop",     /* 46. Instrumental Pop  */
    "Instrumental Rock",    /* 47. Instrumental Rock */
    "Ethnic",               /* 48. Ethnic            */
    "Gothic",               /* 49. Gothic            */
    "Darkwave",             /* 50. Darkwave          */
    "Techno-Industrial",    /* 51. Techno-Industrial */
    "Electronic",           /* 52. Electronic        */
    "Pop-Folk",             /* 53. Pop-Folk          */
    "Eurodance",            /* 54. Eurodance         */
    "Dream",                /* 55. Dream             */
    "Southern Rock",        /* 56. Southern Rock     */
    "Comedy",               /* 57. Comedy            */
    "Cult",                 /* 58. Cult              */
    "Gangsta",              /* 59. Gangsta           */
    "Top 40",               /* 60. Top 40            */
    "Christian Rap",        /* 61. Christian Rap     */
    "Pop/Funk",             /* 62. Pop/Funk          */
    "Jungle",               /* 63. Jungle            */
    "Native American",      /* 64. Native American   */
    "Cabaret",              /* 65. Cabaret           */
    "New Wave",             /* 66. New Wave          */
    "Psychadelic",          /* 67. Psychadelic       */
    "Rave",                 /* 68. Rave              */
    "Showtunes",            /* 69. Showtunes         */
    "Trailer",              /* 70. Trailer           */
    "Lo-Fi",                /* 71. Lo-Fi             */
    "Tribal",               /* 72. Tribal            */
    "Acid Punk",            /* 73. Acid Punk         */
    "Acid Jazz",            /* 74. Acid Jazz         */
    "Polka",                /* 75. Polka             */
    "Retro",                /* 76. Retro             */
    "Musical",              /* 77. Musical           */
    "Rock & Roll",          /* 78. Rock & Roll       */
    "Hard Rock",            /* 79. Hard Rock         */
    "Folk",                 /* 80. Folk              */
    "Folk-Rock",            /* 81. Folk-Rock         */
    "National Folk",        /* 82. National Folk     */
    "Swing",                /* 83. Swing             */
    "Fast Fusion",          /* 84. Fast Fusion       */
    "Bebob",                /* 85. Bebob             */
    "Latin",                /* 86. Latin             */
    "Revival",              /* 87. Revival           */
    "Celtic",               /* 88. Celtic            */
    "Bluegrass",            /* 89. Bluegrass         */
    "Avantgarde",           /* 90. Avantgarde        */
    "Gothic Rock",          /* 91. Gothic Rock       */
    "Progressive Rock",     /* 92. Progressive Rock  */
    "Psychedelic Rock",     /* 93. Psychedelic Rock  */
    "Symphonic Rock",       /* 94. Symphonic Rock    */
    "Slow Rock",            /* 95. Slow Rock         */
    "Big Band",             /* 96. Big Band          */
    "Chorus",               /* 97. Chorus            */
    "Easy Listening",       /* 98. Easy Listening    */
    "Acoustic",             /* 99. Acoustic          */
    "Humour",               /* 100. Humour           */
    "Speech",               /* 101. Speech           */
    "Chanson",              /* 102. Chanson          */
    "Opera",                /* 103. Opera            */
    "Chamber Music",        /* 104. Chamber Music    */
    "Sonata",               /* 105. Sonata           */
    "Symphony",             /* 106. Symphony         */
    "Booty Bass",           /* 107. Booty Bass       */
    "Primus",               /* 108. Primus           */
    "Porn Groove",          /* 109. Porn Groove      */
    "Satire",               /* 110. Satire           */
    "Slow Jam",             /* 111. Slow Jam         */
    "Club",                 /* 112. Club             */
    "Tango",                /* 113. Tango            */
    "Samba",                /* 114. Samba            */
    "Folklore",             /* 115. Folklore         */
    "Ballad",               /* 116. Ballad           */
    "Power Ballad",         /* 117. Power Ballad     */
    "Rhythmic Soul",        /* 118. Rhythmic Soul    */
    "Freestyle",            /* 119. Freestyle        */
    "Duet",                 /* 120. Duet             */
    "Punk Rock",            /* 121. Punk Rock        */
    "Drum Solo",            /* 122. Drum Solo        */
    "A Cappella",           /* 123. A capella        */
    "Euro-House",           /* 124. Euro-House       */
    "Dance Hall",           /* 125. Dance Hall       */
#if 0
    "Goa",
    "Drum & Bass",
    "Club-House",
    "Hardcore",
    "Terror",
    "Indie",
    "BritPop",
    "Negerpunk",
    "Polsk Punk",
    "Beat",
    "Christian Gangsta Rap",
    "Heavy Metal",
    "Black Metal",
    "Crossover",
    "Contemporary Christian",
    "Christian Rock",
    "Merengue",
    "Salsa",
    "Thrash Metal",
    "Anime",
    "JPop",
    "Synthpop"
#endif
};
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
UINT32 cvt_2_multi_char(UCHAR* ac_text, UINT16* awc_text, UINT32 ui4_len)
{
    UINT32 ui4_cnt;
    UINT32 ui4_dst_cnt;

    ui4_dst_cnt = 0;
    for(ui4_cnt = 0; ui4_cnt < ui4_len && ac_text[ui4_cnt]; )
    {
        if(ac_text[ui4_cnt] & 0x80)
        {
            awc_text[ui4_dst_cnt] = ac_text[ui4_cnt + 1];
            awc_text[ui4_dst_cnt] = awc_text[ui4_dst_cnt] << 8;
            awc_text[ui4_dst_cnt] |= ac_text[ui4_cnt];
            ui4_cnt += 2;
        }
        else
        {
            awc_text[ui4_dst_cnt] = ac_text[ui4_cnt];
            ui4_cnt ++;
        }
        ui4_dst_cnt ++;
    }
    awc_text[ui4_dst_cnt] = 0;

    return ui4_dst_cnt;
}

static UINT32 mp3_id3_pickout_ascii(CHAR* ac_src, UINT16* awc_dst, UINT32 ui4_str_leng)
{
    UINT32 ui4_src_cnt = 0;
    UINT32 ui4_dst_cnt = 0;

    if(NULL == ac_src || 
       NULL == awc_dst || 
       ui4_str_leng == 0)
    {
        return 0;
    }

    while(ui4_src_cnt < ui4_str_leng)
    {
        if(ac_src[ui4_src_cnt] > 0)
        {
            awc_dst[ui4_dst_cnt++] = (UINT16)ac_src[ui4_src_cnt++];
        }
        else if(ac_src[ui4_src_cnt] < 0) /*skip multi-bytes charactors*/
        {
            ui4_src_cnt += 2;
        }
        else
        {
            break;
        }
    }
    awc_dst[ui4_dst_cnt] = 0;

    return ui4_dst_cnt;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_trim_trailing_spaces
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
void  mp3_id3_trim_trailing_spaces (CHAR *ps_s)
{
    INT32     i4_i;
    CHAR      c_char;
    UINT32    ui4_space_bgn_idx;

    /* find the last non-space character */
    for (i4_i = ((INT32)(x_strlen(ps_s)) - 1); i4_i >= 0 ; i4_i--)
    {
        c_char = *(ps_s + i4_i);
        if ((c_char != ' ')        /* space */
            && (c_char != '\f')    /* form feed */
            && (c_char != '\n')    /* new line */
            && (c_char != '\r')    /* carriage return */
            && (c_char != '\t')    /* horizontal tab */
            && (c_char != '\v'))   /* vertical tab */
        {
            /* trim the trailing spaces */
            ui4_space_bgn_idx = (UINT32)(i4_i + 1);
            x_memset((VOID*)(ps_s + ui4_space_bgn_idx),
                     0,
                     x_strlen(ps_s) - ui4_space_bgn_idx);
            return;
        }
    }
    return;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_v2_input1bytes
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
UINT8  mp3_id3_v2_input1bytes (MINFO_OBJ_T*    pt_minfo_obj)
{
    MP3_STREAM_INFO_T*    pt_stream_info;
    input1bytes_fct       pf_input1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    ID3_TAG_SET_T*        pt_id3_set;
    UINT8                 ui1_input;
    INT32                 i4_ret;

    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return 0;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;
    i4_ret = pf_input1byte(pt_minfo_obj, &ui1_input);
    if (i4_ret != INPUTR_OK)
    {
        return 0;
    }
    if ((pt_id3_set->b_v2_has_unsync)
        || (pt_id3_set->b_v2_frame_unsync))
    {
        if ((pt_id3_set->ui1_pre_char == 0xff)
            && (ui1_input == 0))
        { /* skip unsynchronisation padding*/
            i4_ret = pf_input1byte(pt_minfo_obj, &ui1_input);
            if (i4_ret != INPUTR_OK)
            {
                return 0;
            }
        }
    }

    pt_id3_set->ui1_pre_char = ui1_input;
    return ui1_input;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_v2_get_synchsafe_3bytes
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
UINT32 mp3_id3_v2_get_synchsafe_3bytes (MINFO_OBJ_T*    pt_minfo_obj)
{
    input1bytes_fct pf_input1byte;
    UINT8           ui1_tmp1, ui1_tmp2, ui1_tmp3;

    pf_input1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_input1byte(pt_minfo_obj, &ui1_tmp1);
    pf_input1byte(pt_minfo_obj, &ui1_tmp2);
    pf_input1byte(pt_minfo_obj, &ui1_tmp3);
    return (((UINT32)ui1_tmp1 << 14)
            | ((UINT32)ui1_tmp2 << 7)
            | ((UINT32)ui1_tmp3));
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_v2_get_synchsafe_integer
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
UINT32 mp3_id3_v2_get_synchsafe_integer (MINFO_OBJ_T*    pt_minfo_obj)
{
    input1bytes_fct pf_input1byte;
    UINT8           ui1_tmp1, ui1_tmp2, ui1_tmp3, ui1_tmp4;

    pf_input1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_input1byte(pt_minfo_obj, &ui1_tmp1);
    pf_input1byte(pt_minfo_obj, &ui1_tmp2);
    pf_input1byte(pt_minfo_obj, &ui1_tmp3);
    pf_input1byte(pt_minfo_obj, &ui1_tmp4);
    return (((UINT32)ui1_tmp1 << 21)
            | ((UINT32)ui1_tmp2 << 14)
            | ((UINT32)ui1_tmp3 << 7)
            | ((UINT32)ui1_tmp4));
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_v2_input4bytes
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
UINT32 mp3_id3_v2_input4bytes (MINFO_OBJ_T*    pt_minfo_obj)
{
    return (((UINT32)mp3_id3_v2_input1bytes(pt_minfo_obj) << 24)
            | ((UINT32)mp3_id3_v2_input1bytes(pt_minfo_obj) << 16)
            | ((UINT32)mp3_id3_v2_input1bytes(pt_minfo_obj) << 8)
            | ((UINT32)mp3_id3_v2_input1bytes(pt_minfo_obj)));
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_v2_input3bytes
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
UINT32 mp3_id3_v2_input3bytes (MINFO_OBJ_T*    pt_minfo_obj)
{
    return (((UINT32)mp3_id3_v2_input1bytes(pt_minfo_obj) << 16)
            | ((UINT32)mp3_id3_v2_input1bytes(pt_minfo_obj) << 8)
            | ((UINT32)mp3_id3_v2_input1bytes(pt_minfo_obj)));
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_v2_input2bytes
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
UINT16 mp3_id3_v2_input2bytes (MINFO_OBJ_T*    pt_minfo_obj)
{
    return (((UINT16)mp3_id3_v2_input1bytes(pt_minfo_obj) << 8)
            | ((UINT16)mp3_id3_v2_input1bytes(pt_minfo_obj)));
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_tag_parse_v2_tag_header
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_tag_parse_v2_tag_header(MINFO_OBJ_T*    pt_minfo_obj)
{
    ID3_TAG_SET_T*      pt_id3_set;
    MP3_STREAM_INFO_T*  pt_stream_info;
    input3bytes_fct     pf_input3byte;
    input1bytes_fct     pf_input1byte;
    UINT8               ui1_flags, ui1_tmp;
    UINT32              ui4_tag;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }
    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;
    if (pt_id3_set == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pf_input3byte = pt_minfo_obj->t_input_fct_tbl.pf_input3;
    pf_input1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;

    /************************************************************************/
    /* header                                                                   */
    /************************************************************************/

    /*
    * check id3v2 magic word -- "ID3"
    * and parse header and set the following 4 fields in id3_tag:
    * (1) v2_size  (not include header & footer size)
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
    IS_INPUT_OK(pf_input3byte(pt_minfo_obj, &ui4_tag));
    if (cui4_ID3 != ui4_tag)
    {
        return MINFOR_FMT_NOT_SUPPORT;
    }
    IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &(pt_id3_set->ui1_v2_version)));
    IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_tmp));    /* skip revision*/
    IS_INPUT_OK(pf_input1byte(pt_minfo_obj, &ui1_flags));

    switch (pt_id3_set->ui1_v2_version)
    {
        case 4:
            if (ui1_flags & 0x0F)
            {
                return MINFOR_FMT_NOT_SUPPORT;
            }
            if (ui1_flags & 0x10)   /* check footer flag */
            {
                pt_id3_set->b_v2_has_footer = TRUE;
            }
        case 3:
            if (ui1_flags & 0x1F)
            {
                return MINFOR_FMT_NOT_SUPPORT;
            }
            if (ui1_flags & 0x20)   /* check extended header flag */
            {
                pt_id3_set->b_v2_has_exthdr = TRUE;
            }
            break;
        case 2:
            if (ui1_flags & 0x3F)
            {
                return MINFOR_FMT_NOT_SUPPORT;
            }
            if (ui1_flags & 0x40)   /* check compression flag, if it is set, ignore this tag */
            {
                return MINFOR_FMT_NOT_SUPPORT;
            }
            break;
        default:
           return MINFOR_FMT_NOT_SUPPORT;
    }
    if (ui1_flags & 0x80)   /* check unsynchronisation flag */
    {
        pt_id3_set->b_v2_has_unsync = TRUE;
    }

    /* get tag size */
    pt_id3_set->ui4_v2_tag_size = mp3_id3_v2_get_synchsafe_integer(pt_minfo_obj);
    pt_id3_set->ui4_v2_length = pt_id3_set->ui4_v2_tag_size + 10;
    return MINFOR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_tag_parse_v2_tag_extended_header
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_tag_parse_v2_tag_extended_header(MINFO_OBJ_T*    pt_minfo_obj)
{
    ID3_TAG_SET_T*      pt_id3_set;
    MP3_STREAM_INFO_T*  pt_stream_info;
    UINT32              ui4_extended_header_size, ui4_padding_size;
    UINT8               ui1_flag;
    UINT16              ui2_flag;
    UINT32              ui4_i;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }
    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;
    if (pt_id3_set == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/

    switch (pt_id3_set->ui1_v2_version)
    {
        case 4:
            /*
            Extended header size   4 * %0xxxxxxx
            Number of flag bytes       $01
            Extended Flags             $xx
            */
            ui4_extended_header_size = mp3_id3_v2_get_synchsafe_integer(pt_minfo_obj);  /* 0, 1, 2, 3 */
            /* check size */
            if (ui4_extended_header_size >= pt_id3_set->ui4_v2_tag_size)
            {
                return MINFOR_INTERNAL_ERR;
            }
            if (mp3_id3_v2_input1bytes(pt_minfo_obj) != 0x01)    /* flag bytes must be 1 */
            {
                return MINFOR_INTERNAL_ERR;
            }
            ui1_flag = mp3_id3_v2_input1bytes(pt_minfo_obj);
            if (ui1_flag & 0x8F)   /* check unset flags */
            {
                return MINFOR_INTERNAL_ERR;
            }
            if (ui1_flag & 0x40)   /* check tag is updated flag */
            {
                if (mp3_id3_v2_input1bytes(pt_minfo_obj) != 0x00)    /* flag data length must be 0 */
                {
                    return MINFOR_INTERNAL_ERR;
                }
            }
            if (ui1_flag & 0x20)   /* check CRC data present flag */
            {
                /* skip CRC check*/
                if (mp3_id3_v2_input1bytes(pt_minfo_obj) != 0x05)    /* flag data length must be 5 */
                {
                    return MINFOR_INTERNAL_ERR;
                }
                for (ui4_i = 0; ui4_i < 5; ui4_i++)
                {
                    mp3_id3_v2_input1bytes(pt_minfo_obj);      /* 7, 8, 9, 10, 11 */
                }
            }
            if (ui1_flag & 0x10)   /* check tag restriction flag */
            {
                if (mp3_id3_v2_input1bytes(pt_minfo_obj) != 0x01)    /* flag data length must be 1 */
                {
                    return MINFOR_INTERNAL_ERR;
                }
                mp3_id3_v2_input1bytes(pt_minfo_obj);/*skip restrictions*/
            }
            break;

        case 3:
            /*
            Extended header size   $xx xx xx xx
            Extended Flags         $xx xx
            Size of padding        $xx xx xx xx
            */
            ui4_extended_header_size = mp3_id3_v2_get_synchsafe_integer(pt_minfo_obj); /* 0, 1, 2, 3 */
            ui2_flag = mp3_id3_v2_input2bytes(pt_minfo_obj);
            ui4_padding_size = mp3_id3_v2_input4bytes(pt_minfo_obj); /* 6, 7, 8, 9 */
            /* check size */
            if (ui4_extended_header_size + ui4_padding_size >= pt_id3_set->ui4_v2_tag_size)
            {
                return MINFOR_INTERNAL_ERR;
            }
            if (ui2_flag & 0x8000)   /* check CRC data present flag */
            {
                /* skip CRC check*/
                mp3_id3_v2_get_synchsafe_integer(pt_minfo_obj);
            }
            break;

        default:
            return MINFOR_INTERNAL_ERR;
    }

    return MINFOR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_tag_parse_v2_frame_header
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_tag_parse_v2_frame_header(MINFO_OBJ_T*    pt_minfo_obj)
{
    ID3_TAG_SET_T*      pt_id3_set;
    MP3_STREAM_INFO_T*  pt_stream_info;
    UINT8               ui1_flags;
    UINT64              ui8_end_frame_pos;
    setpos_fct          pf_set_pos;
    getpos_fct          pf_get_pos;
    UINT64              ui8_pos;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }
    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;
    if (pt_id3_set == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    if ((pf_get_pos == NULL)||(pf_set_pos == NULL))
    {
        return MINFOR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    IS_INPUT_OK(pf_get_pos(pt_minfo_obj, &ui8_pos));
    if (ui8_pos >= pt_id3_set->ui4_v2_tag_size)
    {

        return MINFOR_EOT;
    }
    /*
    Frame ID       $xx xx xx xx (four characters)
    Size           $xx xx xx xx
    Flags          $xx xx
    */
    /* clean frame data */
    pt_id3_set->b_v2_frame_unsync = FALSE;
    pt_id3_set->ui4_frame_id = 0;
    pt_id3_set->ui4_frame_size = 0;
    pt_id3_set->ui2_frame_flag = 0;
    switch (pt_id3_set->ui1_v2_version)
    {
        case 4:
            pt_id3_set->ui4_frame_id = mp3_id3_v2_input4bytes(pt_minfo_obj);  /* 0, 1, 2, 3 */
            pt_id3_set->ui4_frame_size = mp3_id3_v2_get_synchsafe_integer(pt_minfo_obj);        /* 4, 5, 6, 7 */
            pt_id3_set->ui2_frame_flag = mp3_id3_v2_input2bytes(pt_minfo_obj);
            IS_INPUT_OK(pf_get_pos(pt_minfo_obj, &ui8_pos));
            ui8_end_frame_pos = ui8_pos + (pt_id3_set->ui4_frame_size);

            ui1_flags = (UINT8)pt_id3_set->ui2_frame_flag >> 8;
            /* get status flags */
            if (ui1_flags & 0x80)   /* check unset flags */
            {
                goto SKIP_FRAME;
            }
            if (ui1_flags & 0x60)   /* check file and tag alter perservation flags */
            {
                goto SKIP_FRAME;
            }

            /* get format flags */
            ui1_flags = (UINT8)(pt_id3_set->ui2_frame_flag & 0x00ff);

            if (ui1_flags & 0xB0)   /* check unset flags */
            {
                goto SKIP_FRAME;
            }

            if (ui1_flags & 0x0C)   /* check compress and encrypt flags */
            {
                goto SKIP_FRAME;
            }

            if (ui1_flags & 0x40)   /* check group id flag */
            {
                mp3_id3_v2_input1bytes(pt_minfo_obj);
            }

            if (ui1_flags & 0x20)   /* check unsyncrhonisation flag */
            {
                pt_id3_set->b_v2_frame_unsync = TRUE;
            }

            if (ui1_flags & 0x01)   /* check data length indicator flag */
            {
                mp3_id3_v2_get_synchsafe_integer(pt_minfo_obj);    /* 10, 11, 12, 13 */
            }
            /*check padding*/
            if ((pt_id3_set->ui4_frame_id & 0xff000000) == 0x0)
            {
                return MINFOR_EOT;
            }
            break;

        case 3:
            pt_id3_set->ui4_frame_id = mp3_id3_v2_input4bytes(pt_minfo_obj);  /* 0, 1, 2, 3 */
            pt_id3_set->ui4_frame_size = mp3_id3_v2_input4bytes(pt_minfo_obj);        /* 4, 5, 6, 7 */
            pt_id3_set->ui2_frame_flag = mp3_id3_v2_input2bytes(pt_minfo_obj);
            IS_INPUT_OK(pf_get_pos(pt_minfo_obj, &ui8_pos));
            ui8_end_frame_pos = ui8_pos + pt_id3_set->ui4_frame_size;

            /* get status flags */
            ui1_flags = (UINT8)pt_id3_set->ui2_frame_flag >> 8;

            if (ui1_flags & 0x1F)   /* check unset flags */
            {
                goto SKIP_FRAME;
            }

            if (ui1_flags & 0xC0)   /* check file and tag alter perservation flags */
            {
                goto SKIP_FRAME;
            }

            /* get format flags */
            ui1_flags = (UINT8)(pt_id3_set->ui2_frame_flag & 0x00ff);

            if (ui1_flags & 0x1F)   /* check unset flags */
            {
                goto SKIP_FRAME;
            }

            if (ui1_flags & 0xC0)   /* check compress and encrypt flags */
            {
                goto SKIP_FRAME;
            }

            if (ui1_flags & 0x20)   /* check group id flag */
            {
                mp3_id3_v2_input1bytes(pt_minfo_obj);
            }
            /*check padding*/
            if ((pt_id3_set->ui4_frame_id & 0xff000000) == 0x0)
            {
                return MINFOR_EOT;
            }
            break;

        case 2:
            pt_id3_set->ui4_frame_id = mp3_id3_v2_input3bytes(pt_minfo_obj);  /* 0, 1, 2 */
            pt_id3_set->ui4_frame_size = mp3_id3_v2_input3bytes(pt_minfo_obj);       /* 3, 4, 5 */
            IS_INPUT_OK(pf_get_pos(pt_minfo_obj, &ui8_pos));
            ui8_end_frame_pos = ui8_pos + pt_id3_set->ui4_frame_size;
            /*check padding*/
            if ((pt_id3_set->ui4_frame_id & 0x00ff0000) == 0x0)
            {
                return MINFOR_EOT;
            }
            break;

        default:
            return MINFOR_INTERNAL_ERR;
    }
    if (pt_id3_set->ui4_frame_size > pt_id3_set->ui4_v2_tag_size )
    {
        return MINFOR_INTERNAL_ERR;
    }
    else
    {
        return MINFOR_OK;
    }

SKIP_FRAME:
    /*seek to end of frame*/
    IS_INPUT_OK(pf_set_pos(pt_minfo_obj, (INT64)ui8_end_frame_pos, FM_SEEK_BGN));
    return MINFOR_SKIP_FRAME;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_tag_match_v2_frame_id_from_list
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_tag_match_v2_frame_id_from_list(MINFO_OBJ_T*    pt_minfo_obj, UINT32* pui4_idx)
{
    ID3_TAG_SET_T*      pt_id3_set;
    MP3_STREAM_INFO_T*  pt_stream_info;
    UINT32*             pui4_idx_list;
    UINT32              ui4_i;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }
    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;
    if (pt_id3_set == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    switch (pt_id3_set->ui1_v2_version)
    {
        case 4:
            pui4_idx_list = aui4_mp3_id3_tag_v2_4_frame_id_list;
            break;
        case 3:
            pui4_idx_list = aui4_mp3_id3_tag_v2_3_frame_id_list;
            break;
        case 2:
            pui4_idx_list = aui4_mp3_id3_tag_v2_2_frame_id_list;
            break;
        default:
            return MINFOR_INTERNAL_ERR;
    }
    for (ui4_i = 0; ui4_i < ID3_TAG_V2_MAX_FRAME_IDX; ui4_i++)
    {
        if (pt_id3_set->ui4_frame_id == pui4_idx_list[ui4_i])
        {
            *pui4_idx = ui4_i;
            return MINFOR_OK;
        }
    }
    return MINFOR_INTERNAL_ERR;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_v2_tag_parse_pic_info
 *
 * Description: the function for getting the width and hight of the picture
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/

static INT32 mp3_id3_v2_tag_parse_pic_info(MINFO_OBJ_T*    pt_minfo_obj, MINFO_INFO_PIC_T* pt_pic)
{
    setpos_fct      pf_set_pos;
    getpos_fct      pf_get_pos;
    UINT64          ui8_pos       = 0;
    UINT32          ui4_total_len = 0;
    UINT16          ui2_len       = 0;
    UINT16          ui2_tag       = 0;
    BOOL            b_found       = FALSE;

    if (pt_minfo_obj == NULL || pt_pic == NULL)
    {
        return MINFOR_INV_ARG;
    }

    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    if ((pf_get_pos == NULL)||(pf_set_pos == NULL))
    {
        return MINFOR_INTERNAL_ERR;
    }

    /* rememory the old position */
    IS_INPUT_OK(pf_get_pos(pt_minfo_obj, &ui8_pos));
    do
    {
        if (pt_pic->t_format.e_media_type == MEDIA_TYPE_IMAGE)
        {
           /* for the png image*/
            if (pt_pic->t_format.t_media_subtype.u.e_img_subtype == MEDIA_IMG_SUBTYPE_PNG)
            {
                /* set the picture width directly */
                IS_INPUT_OK(pf_set_pos(pt_minfo_obj, (INT64)(pt_pic->ui4_starting_offset + 16), FM_SEEK_BGN));
                pt_pic->ui4_width  = mp3_id3_v2_input4bytes(pt_minfo_obj);
                pt_pic->ui4_height = mp3_id3_v2_input4bytes(pt_minfo_obj);
                b_found            = TRUE;
                break;
            }
            /* for the jpg image*/
            if (pt_pic->t_format.t_media_subtype.u.e_img_subtype == MEDIA_IMG_SUBTYPE_JPG)
            {
                  /* */
                IS_INPUT_OK(pf_set_pos(pt_minfo_obj, (INT64)pt_pic->ui4_starting_offset, FM_SEEK_BGN));
                ui2_tag = mp3_id3_v2_input2bytes(pt_minfo_obj);
                if (ui2_tag != 0xFFD8)
                {
                   break;
                }
                while (!b_found)
                {
                ui2_tag = mp3_id3_v2_input2bytes(pt_minfo_obj);
                if ((ui2_tag == 0xFFC0) || (ui2_tag == 0xFFC1) || (ui2_tag == 0xFFC2) || (ui2_tag == 0xFFC3))
                {
                    IS_INPUT_OK(pf_set_pos(pt_minfo_obj, (INT64)3, FM_SEEK_CUR));
                    pt_pic->ui4_width  = (UINT32)mp3_id3_v2_input2bytes(pt_minfo_obj);
                    pt_pic->ui4_height = (UINT32)mp3_id3_v2_input2bytes(pt_minfo_obj);
                    b_found            = TRUE;
                }
                else
                {
                    ui2_len  = mp3_id3_v2_input2bytes(pt_minfo_obj);
                    /* the total len which had parse*/
                    ui4_total_len += (UINT32)ui2_len;
                    if (ui2_len > 2)
                    {
                        IS_INPUT_OK(pf_set_pos(pt_minfo_obj, (INT64)(ui2_len - 2), FM_SEEK_CUR));
                    }
                    else
                    {
                        break;
                    }
                }
                /* defend  beyond the pic len*/
                if(ui4_total_len > pt_pic->ui4_length)
                {
                    break;
                }
            }                
         }
      }
    }while(FALSE);
    if (!b_found)
    {
        pt_pic->ui4_width = 0;
        pt_pic->ui4_height = 0;
    }
#ifndef __KERNEL__
    x_dbg_stmt("\n<<<<<<<<width:%d,height:%d>>>>>>>>\n", pt_pic->ui4_width, pt_pic->ui4_height);
#endif
    /* resume the file postion*/
    IS_INPUT_OK(pf_set_pos(pt_minfo_obj, ui8_pos, FM_SEEK_BGN));
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_tag_parse_v2_pic_data
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_tag_parse_v2_pic_data(MINFO_OBJ_T*    pt_minfo_obj, UINT32 ui4_frame_id_index)
{
    ID3_TAG_SET_T*          pt_id3_set;
    MP3_STREAM_INFO_T*      pt_stream_info;
    setpos_fct              pf_set_pos;
    getpos_fct              pf_get_pos;
    UINT64                  ui8_pos;
    UINT64                  ui8_end_frame_pos;
    UINT8                   ui1_pic_type;
    ID3_TAG_PIC_FRAME_T*    pt_pic;
    INT32                   i4_i;
    CHAR                    ac_txt[MAX_ID3_TAG_V2_PIC_DESCIPTION_LENGTH+1], c_read;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }
    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;
    if (pt_id3_set == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    if ((pf_get_pos == NULL)||(pf_set_pos == NULL))
    {
        return MINFOR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    IS_INPUT_OK(pf_get_pos(pt_minfo_obj, &ui8_pos));
    ui8_end_frame_pos = ui8_pos + pt_id3_set->ui4_frame_size;
    mp3_id3_v2_input1bytes(pt_minfo_obj); /* skip text encoding */

    /* MIME type text string */
    i4_i = 0;
    x_memset(ac_txt, 0, (MAX_ID3_TAG_V2_PIC_DESCIPTION_LENGTH+1) * sizeof(CHAR));
    do{
        c_read = (CHAR)(mp3_id3_v2_input1bytes(pt_minfo_obj));
        ac_txt[i4_i] = c_read;
        i4_i ++;
        
        if (i4_i >= MAX_ID3_TAG_V2_PIC_DESCIPTION_LENGTH)
        {
            break;
        }
    }
    while (c_read != '\0');

    /* PIC type */
    ui1_pic_type = mp3_id3_v2_input1bytes(pt_minfo_obj);

    /* check if PIC type is invalid */
    if (ui1_pic_type > (MAX_ID3_PIC_NUM - 1))
    {
        return MINFOR_INTERNAL_ERR;
    }

    /*PIC bit mask*/
    pt_id3_set->ui4_pic_bitmask = pt_id3_set->ui4_pic_bitmask | ((UINT32)(1<<ui1_pic_type));

    pt_pic = &(pt_id3_set->at_pic[ui1_pic_type]);
    pt_pic->e_pic_type = ((MINFO_ID3_PIC_TYPE_T)ui1_pic_type);

    /* upper string */
    x_str_toupper(ac_txt);
    if (x_strstr(ac_txt, "PNG"))
    {
        pt_pic->t_format.e_media_type = MEDIA_TYPE_IMAGE;
        pt_pic->t_format.t_media_subtype.u.e_img_subtype = MEDIA_IMG_SUBTYPE_PNG;
    }
    else if ((x_strstr(ac_txt, "JPG"))
           ||(x_strstr(ac_txt, "JPEG"))
           ||(x_strstr(ac_txt, "JFIF")))
    {
        pt_pic->t_format.e_media_type = MEDIA_TYPE_IMAGE;
        pt_pic->t_format.t_media_subtype.u.e_img_subtype = MEDIA_IMG_SUBTYPE_JPG;
    }
    else
    {
        pt_pic->t_format.e_media_type = MEDIA_TYPE_IMAGE;
        pt_pic->t_format.t_media_subtype.u.e_img_subtype = MEDIA_IMG_SUBTYPE_UNKNOWN;
    }


    /* description */
    i4_i = 0;
    x_memset(ac_txt, 0, (MAX_ID3_TAG_V2_PIC_DESCIPTION_LENGTH+1) * sizeof (CHAR));
    do{
        c_read = (CHAR)(mp3_id3_v2_input1bytes(pt_minfo_obj));
        ac_txt[i4_i] = c_read;
        i4_i ++;
        
        if (i4_i >= MAX_ID3_TAG_V2_PIC_DESCIPTION_LENGTH)
        {
            break;
        }
    }
    while (c_read != '\0');
    /* ignore description text */

    IS_INPUT_OK(pf_get_pos(pt_minfo_obj, &ui8_pos));
    pt_pic->ui4_starting_offset = (UINT32)ui8_pos;
    pt_pic->ui4_length = (UINT32)ui8_end_frame_pos - pt_pic->ui4_starting_offset + 1;/*1 base*/

    /* parse the width and height of pic */
    mp3_id3_v2_tag_parse_pic_info(pt_minfo_obj, pt_pic);

    /*seek to end of frame*/
    IS_INPUT_OK(pf_set_pos(pt_minfo_obj, (INT64)ui8_end_frame_pos, FM_SEEK_BGN));
    return MINFOR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_tag_parse_number
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_tag_parse_number(CHAR*    ps_text, SIZE_T z_txt_size, INT32* pi4_read_size)
{
    INT32 i4_ret               = -1;
    BOOL  b_have_left_bracket  = FALSE; /*if not have the left bracket*/
    BOOL  b_have_right_bracket = FALSE; /*if not have the right bracket*/
    BOOL  b_is_first_numeral   = FALSE; /*if not the first numeral */
    BOOL  b_have_character     = FALSE; /*if not have the character*/
    
    *pi4_read_size = 0;
    
    while(z_txt_size > 0)
    {
        /*skip space before left bracket*/
        if(!b_have_left_bracket && *ps_text == 0x20)
        {
            *pi4_read_size += 1;
            z_txt_size--;
            ps_text++;
            continue;
        }
        /*the first left bracket is special, but the next one is the character*/
        if(*ps_text == '(' && !b_have_left_bracket)
        {   
            b_have_left_bracket = TRUE;  
        }
        else if (*ps_text == ')')
        {   
            if(b_have_left_bracket)
            {
                b_have_right_bracket = TRUE;
                *pi4_read_size += 1;
            }
            /*meeting the right bracket, skip the loops*/
            break;
        }
        else 
        {
            if(b_have_left_bracket)
            {
                if(*ps_text == 0x20 && !b_is_first_numeral && !b_have_character)
                {
                    *pi4_read_size += 1;
                    z_txt_size--;
                    ps_text++;
                    continue;
                }
                if (*ps_text >= '0' && *ps_text <= '9' && !b_have_character)
                {
                    if (i4_ret < 0)    /* check if it's the first time */
                    {
                        i4_ret = 0;
                    }
                    i4_ret = i4_ret * 10 + (*ps_text - '0');
                    b_is_first_numeral = TRUE;
                }
                else
                {
                    b_have_character = TRUE;
                }
            }
            else
            {
                break;
            }
        }
        *pi4_read_size += 1;
        z_txt_size--;
        ps_text++;
    }
    if(b_have_right_bracket && !b_is_first_numeral)
    {
        i4_ret = 0;
    }
    if(!b_have_right_bracket)
    {
        i4_ret = -1;
        *pi4_read_size = 0;
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_convert_genre
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_convert_genre(MINFO_OBJ_T*    pt_minfo_obj)
{
    ID3_TAG_SET_T*      pt_id3_set;
    MP3_STREAM_INFO_T*  pt_stream_info;
    INT32               i4_genre_idx;
    INT32               i4_ret;
    INT32               i4_read_size;
    INT32               i4_offsize,i4_tag_size;
    INT32               i4_i, i4_j = 0;
    CHAR                ac_text[MAX_ID3_TAG_TXT_FRAME_LENGTH+1];
    CHAR                ac_text_temp[MAX_ID3_TAG_TXT_FRAME_LENGTH+1];
    BOOL                b_is_first_space = FALSE;  

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }
    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;
    if (pt_id3_set == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    if (pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].z_size > 0)
    {
        /*genre present*/
        x_memset(ac_text, 0, MAX_ID3_TAG_TXT_FRAME_LENGTH + 1);
        x_memcpy((VOID*)ac_text, pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text, MAX_ID3_TAG_TXT_FRAME_LENGTH);
        
        if (x_strstr(ac_text, "RX"))
        {
            x_memset(pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text, 0, (MAX_ID3_TAG_TXT_FRAME_LENGTH+1)*sizeof(UTF16_T));
            x_uc_w2s_strcpy(pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text, (const UTF16_T*)L"Remix");
            pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].z_size = x_uc_w2s_strlen(pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text);
            pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].e_encoding = MP3_ID3_FRAME_TEXT_ENCODING_UTF16_WO_BOM;
        }
        else if (x_strstr(ac_text, "CR"))
        {
            x_memset(pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text, 0, (MAX_ID3_TAG_TXT_FRAME_LENGTH+1)*sizeof(UTF16_T));
            x_uc_w2s_strcpy(pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text, (const UTF16_T*) L"Cover");
            pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].z_size = x_uc_w2s_strlen(pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text);
            pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].e_encoding = MP3_ID3_FRAME_TEXT_ENCODING_UTF16_WO_BOM;
        }
        else
        {
            if (pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].e_encoding == MP3_ID3_FRAME_TEXT_ENCODING_UTF8 ||
                pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].e_encoding == MP3_ID3_FRAME_TEXT_ENCODING_ISO_8859_1)
            {
                /* warning */
                ac_text[MAX_ID3_TAG_TXT_FRAME_LENGTH] = 0;
                if (x_strlen(ac_text) == 1 && ac_text[0]>0 && ac_text[0]<MAX_ID3_TAG_GENRE_NUM)
                {
                    i4_genre_idx = ac_text[0];
                    x_strcpy(ac_text, aps_mp3_id3_tag_genres[i4_genre_idx]); 
                }
                else 
                {
                i4_genre_idx = mp3_id3_tag_parse_number(ac_text, x_strlen(ac_text), &i4_read_size);
                if ((i4_genre_idx != -1) && ((i4_genre_idx >= 0) && (i4_genre_idx < MAX_ID3_TAG_GENRE_NUM)))
                {
                    if(x_strlen(ac_text) >= (UINT32)i4_read_size)
                    {
                        i4_offsize =  x_strlen(ac_text) - i4_read_size;
                    }
                    else
                    {
                        i4_offsize = 0;
                    }
                        
                    x_memset(ac_text_temp, 0, MAX_ID3_TAG_TXT_FRAME_LENGTH + 1);
                    /* if the first character is the space, will skip the all the space character */
                    if(ac_text[i4_read_size] == 0x20)
                    {
                        b_is_first_space = TRUE;
                    }
                    for(i4_i=0; i4_i<i4_offsize; i4_i++)
                    {
                        /* if the character is the character except the space, will stop skipping the the space characters*/
                        if(ac_text[i4_i + i4_read_size] != 0x20)
                        {
                            b_is_first_space = FALSE;
                        }
                        if(!b_is_first_space)
                        {
                            if(i4_j + 1 < MAX_ID3_TAG_TXT_FRAME_LENGTH)
                            {
                                ac_text_temp[i4_j++] = ac_text[i4_i + i4_read_size];   
                            }
                        }
                    }
                    x_memset(ac_text, 0, MAX_ID3_TAG_TXT_FRAME_LENGTH + 1);
                    if(i4_genre_idx == 0x0C && i4_offsize > 0)
                    {
                        i4_tag_size= 0;
                    }
                    else
                    {
                        if (x_strlen(aps_mp3_id3_tag_genres[i4_genre_idx]) <= MAX_ID3_TAG_TXT_FRAME_LENGTH)
                        {
                            x_memcpy(ac_text, aps_mp3_id3_tag_genres[i4_genre_idx], x_strlen(aps_mp3_id3_tag_genres[i4_genre_idx])); 
                        }
                        i4_tag_size = x_strlen(ac_text);
                        if(i4_tag_size + 1 < MAX_ID3_TAG_TXT_FRAME_LENGTH)
                        {
                            ac_text[i4_tag_size++] = 0x20;
                        }   
                    } 
                    for(i4_i=0; i4_i<i4_offsize; i4_i++)
                    {
                        if(i4_tag_size + i4_i + 1 < MAX_ID3_TAG_TXT_FRAME_LENGTH)
                        {
                            ac_text[i4_tag_size + i4_i] = ac_text_temp[i4_i];
                        }
                    }     
                }
                }
                x_memset(pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text, 0, (MAX_ID3_TAG_TXT_FRAME_LENGTH + 1)*sizeof(UTF16_T));
                i4_ret = x_uc_ps_to_w2s(ac_text, pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text, MAX_ID3_TAG_TXT_FRAME_LENGTH);
                if (i4_ret == UCR_OK)
                {
                    pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].z_size = x_uc_w2s_strlen(pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text);
                }
                else
                {
                    pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].z_size = 
                        mp3_id3_pickout_ascii(ac_text, pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text, MAX_ID3_TAG_TXT_FRAME_LENGTH);
                }
                pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].e_encoding = MP3_ID3_FRAME_TEXT_ENCODING_UTF16_WO_BOM;
            }
        }
        return MINFOR_OK;
    }
    return MINFOR_INTERNAL_ERR;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_tag_parse_v2_txt_data
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_tag_parse_v2_txt_data(MINFO_OBJ_T*    pt_minfo_obj, UINT32 ui4_frame_id_index)
{
    ID3_TAG_SET_T*      pt_id3_set;
    MP3_STREAM_INFO_T*  pt_stream_info;
    UINT8               ui1_encoding;
    UINT32              ui4_str_leng;
    UINT32              ui4_i;
    UINT8               ui1_byte1, ui1_byte2;
    UINT8*              pui1_dst;
    setpos_fct          pf_set_pos;
    getpos_fct          pf_get_pos;
    UINT64              ui8_pos;
    UINT64              ui8_end_frame_pos;
    CHAR                ac_text[MAX_ID3_TAG_TXT_FRAME_LENGTH + 1];
    MP3_ID3_TAG_UTF16_BYTE_ORDER_T e_byte_order = MP3_ID3_UTF16_BE;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }
    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;
    if (pt_id3_set == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    if ((pf_get_pos == NULL)||(pf_set_pos == NULL))
    {
        return MINFOR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    x_memset(pt_id3_set->at_txt_info[ui4_frame_id_index].awc_text, 0, (MAX_ID3_TAG_TXT_FRAME_LENGTH + 1) * sizeof(UTF16_T));
    IS_INPUT_OK(pf_get_pos(pt_minfo_obj, &ui8_pos));
    ui8_end_frame_pos = ui8_pos + pt_id3_set->ui4_frame_size;
    ui1_encoding = mp3_id3_v2_input1bytes(pt_minfo_obj);
    switch (pt_id3_set->ui1_v2_version)
    {
        case 4:
            if (ui1_encoding > 3)
            {
                return MINFOR_INTERNAL_ERR;
            }
            break;
        case 3:
        case 2:
            if (ui1_encoding > 1)
            {
                return MINFOR_INTERNAL_ERR;
            }
            break;
        default:
            return MINFOR_INTERNAL_ERR;
    }
    switch (ui1_encoding)
    {
        case MP3_ID3_FRAME_TEXT_ENCODING_UTF8:
        case MP3_ID3_FRAME_TEXT_ENCODING_ISO_8859_1:
            x_memset(ac_text, 0, MAX_ID3_TAG_TXT_FRAME_LENGTH+1);
            ui4_str_leng = pt_id3_set->ui4_frame_size - 1/*text encoding*/;
            if (MAX_ID3_TAG_TXT_FRAME_LENGTH <= ui4_str_leng)
            {
                ui4_str_leng = MAX_ID3_TAG_TXT_FRAME_LENGTH;
            }
            for (ui4_i = 0; ui4_i < ui4_str_leng; ui4_i++)
            {
                ac_text[ui4_i] = (CHAR)(mp3_id3_v2_input1bytes(pt_minfo_obj));
            }
            if (ui4_str_leng > 0)
            {
                x_memcpy((VOID*)pt_id3_set->at_txt_info[ui4_frame_id_index].awc_text,
                         (const VOID*)ac_text, ui4_str_leng);
                pt_id3_set->at_txt_info[ui4_frame_id_index].z_size = ui4_str_leng;
            }
            break;
        case MP3_ID3_FRAME_TEXT_ENCODING_UTF16_W_BOM:
            /* check BOM */
            ui1_byte1 = mp3_id3_v2_input1bytes(pt_minfo_obj);
            ui1_byte2 = mp3_id3_v2_input1bytes(pt_minfo_obj);
            if (ui1_byte1 == 0xFF && ui1_byte2 == 0xFE)
            {
                e_byte_order = MP3_ID3_UTF16_LE;
            }
            else if (ui1_byte1 == 0xFE && ui1_byte2 == 0xFF)
            {
                e_byte_order = MP3_ID3_UTF16_BE;
            }
            else
            {
                return MINFOR_INTERNAL_ERR;
            }
            ui4_str_leng = pt_id3_set->ui4_frame_size - 3; /* text encoding, FF FE */
            ui4_str_leng = ui4_str_leng / 2;
            if (MAX_ID3_TAG_TXT_FRAME_LENGTH <= ui4_str_leng)
            {
                ui4_str_leng = MAX_ID3_TAG_TXT_FRAME_LENGTH; /*NULL terminator*/
            }
            for (ui4_i = 0; ui4_i < ui4_str_leng; ui4_i++)
            {
                pui1_dst = (UINT8*)(&pt_id3_set->at_txt_info[ui4_frame_id_index].awc_text[ui4_i]);
                switch(e_byte_order)
                {
                    case MP3_ID3_UTF16_LE:
                        *pui1_dst = mp3_id3_v2_input1bytes(pt_minfo_obj);
                        *((UINT8*)(pui1_dst+1)) = mp3_id3_v2_input1bytes(pt_minfo_obj);
                        pt_id3_set->at_txt_info[ui4_frame_id_index].z_size ++;
                        break;

                    case MP3_ID3_UTF16_BE:
                        *((UINT8*)(pui1_dst+1)) = mp3_id3_v2_input1bytes(pt_minfo_obj);
                        *pui1_dst = mp3_id3_v2_input1bytes(pt_minfo_obj);
                        pt_id3_set->at_txt_info[ui4_frame_id_index].z_size ++;
                        break;
                }
            }
            break;
        case MP3_ID3_FRAME_TEXT_ENCODING_UTF16_WO_BOM:
            ui4_str_leng = pt_id3_set->ui4_frame_size - 1/*text encoding*/;
            ui4_str_leng = ui4_str_leng/2;
            if (MAX_ID3_TAG_TXT_FRAME_LENGTH <= ui4_str_leng)
            {
                ui4_str_leng = MAX_ID3_TAG_TXT_FRAME_LENGTH; /*NULL terminator*/
            }
            for (ui4_i = 0; ui4_i < ui4_str_leng; ui4_i++)
            {
                pui1_dst = (UINT8*)(&pt_id3_set->at_txt_info[ui4_frame_id_index].awc_text[ui4_i]);

                *((UINT8*)(pui1_dst+1)) = mp3_id3_v2_input1bytes(pt_minfo_obj);
                *pui1_dst = mp3_id3_v2_input1bytes(pt_minfo_obj);
                pt_id3_set->at_txt_info[ui4_frame_id_index].z_size ++;
            }
            break;
    }
    pt_id3_set->at_txt_info[ui4_frame_id_index].e_encoding = (MP3_ID3_FRAME_TEXT_ENCODING_T)ui1_encoding; 
    
    /*seek to end of frame*/
    IS_INPUT_OK(pf_set_pos(pt_minfo_obj, (INT64)ui8_end_frame_pos, FM_SEEK_BGN));
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_tag_parse_v2_frame_data
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_tag_parse_v2_frame_data(MINFO_OBJ_T*    pt_minfo_obj)
{
    ID3_TAG_SET_T*      pt_id3_set;
    MP3_STREAM_INFO_T*  pt_stream_info;
    UINT32              ui4_frame_id_index = (UINT32)-1;
    setpos_fct          pf_set_pos;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }

    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;

    if (pt_id3_set == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;

    if (pf_set_pos == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    if (mp3_id3_tag_match_v2_frame_id_from_list(pt_minfo_obj, &ui4_frame_id_index) == MINFOR_OK)
    {   /* the frame is what we want*/
        if (ui4_frame_id_index < ID3_TAG_V2_MAX_TXT_FRAME_IDX)
        {   /* TEXT frame */
            mp3_id3_tag_parse_v2_txt_data(pt_minfo_obj, ui4_frame_id_index);
        }
        else if (ui4_frame_id_index == ID3_TAG_V2_APIC_FRAME_IDX)
        {   /*PIC frame*/
            mp3_id3_tag_parse_v2_pic_data(pt_minfo_obj, ui4_frame_id_index);
        }
        else
        {
            /* skip frame data */
            IS_INPUT_OK(pf_set_pos(pt_minfo_obj, (INT64)(pt_id3_set->ui4_frame_size), FM_SEEK_CUR));
        }
    }
    else
    {   /* skip frame data */
        IS_INPUT_OK(pf_set_pos(pt_minfo_obj, (INT64)pt_id3_set->ui4_frame_size, FM_SEEK_CUR));
    }
    return MINFOR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_tag_parse_v2_tag
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_tag_parse_v2_tag(MINFO_OBJ_T*    pt_minfo_obj)
{
    ID3_TAG_SET_T*      pt_id3_set;
    MP3_STREAM_INFO_T*  pt_stream_info;
    getpos_fct          pf_get_pos;
    UINT64              ui8_pos;
    INT32               i4_ret = MINFOR_INTERNAL_ERR;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }

    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;

    if (pt_id3_set == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    if (pf_get_pos == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*
    +-----------------------------+
    |      Header (10 bytes)      |
    +-----------------------------+
    |       Extended Header       |
    | (variable length, OPTIONAL) |
    +-----------------------------+
    |   Frames (variable length)  |
    +-----------------------------+
    |           Padding           |
    | (variable length, OPTIONAL) |
    +-----------------------------+
    | Footer (10 bytes, OPTIONAL) |
    +-----------------------------+*/
    if( mp3_id3_tag_parse_v2_tag_header(pt_minfo_obj) == MINFOR_OK)
    {
        /* check if tag size > our buffer size */
        /*
        if (pt_id3_set->b_v2_has_footer)
        {
            if (((SIZE_T) (pt_id3_set->ui4_v2_tag_size + 20)) > pt_minfo_obj->z_buff_leng)
            {
                return MINFOR_INTERNAL_ERR;
            }
        }
        else
        {
            if (((SIZE_T) (pt_id3_set->ui4_v2_tag_size + 10)) > pt_minfo_obj->z_buff_leng)
            {
                return MINFOR_INTERNAL_ERR;
            }
        }
        */
        if (pt_id3_set->b_v2_has_exthdr)
        {
            /*
             * parse extended header and set the following 2 fields in id3_tag:
             * (1) v2_exthdr_size
             * (2) v2_crc
             */
            if( mp3_id3_tag_parse_v2_tag_extended_header(pt_minfo_obj) != MINFOR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
        }
        /* parse frame */
        IS_INPUT_OK(pf_get_pos(pt_minfo_obj, &ui8_pos));
        while(ui8_pos < pt_id3_set->ui4_v2_tag_size)
        {
            i4_ret = mp3_id3_tag_parse_v2_frame_header(pt_minfo_obj);
            switch (i4_ret)
            {
                case MINFOR_OK:
                    i4_ret = mp3_id3_tag_parse_v2_frame_data(pt_minfo_obj);
                    break;

                case MINFOR_SKIP_FRAME:
                    break;

                case MINFOR_EOT:
                    pt_id3_set->b_is_valid = TRUE;
                    return MINFOR_OK;

                default:
                    return MINFOR_FMT_NOT_SUPPORT;

            }
            IS_INPUT_OK(pf_get_pos(pt_minfo_obj, &ui8_pos));
        }
        if (i4_ret == MINFOR_OK)
        {
            pt_id3_set->b_is_valid = TRUE;
            return MINFOR_OK;
        }
    }
    return MINFOR_FMT_NOT_SUPPORT;

}

/*-----------------------------------------------------------------------------
 * Name: mp3_id3_tag_get_digits
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns:
 *
 ----------------------------------------------------------------------------*/
extern UINT8 mp3_id3_tag_get_digits(
    UINT32    ui4_number
)
{
    if (ui4_number < 10)
    {
        return 1;
    }
    return (UINT8)(mp3_id3_tag_get_digits(ui4_number / 10) + 1);
}

/*-----------------------------------------------------------------------------
 * Name: _mp3_id3_tag_power10
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns:
 *
 ----------------------------------------------------------------------------*/
extern UINT32 mp3_id3_tag_power10(
    UINT8    ui1_power
)
{
    if (ui1_power == 0)
    {
        return 1;
    }
    return (10 * (mp3_id3_tag_power10((UINT8)(ui1_power - 1))));
}

/*-----------------------------------------------------------------------------
 * Name: mp3_id3_tag_num_to_ascii
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns:
 *
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_tag_num_to_ascii(
    UINT32    ui4_number,
    UINT8*    pui1_ascii_buff,
    UINT8     ui1_buff_len
)
{
    UINT8     ui1_digits;
    UINT8     ui1_idx;
    UINT8     ui1_power;

    ui1_digits = mp3_id3_tag_get_digits(ui4_number);

    if (ui1_digits > ui1_buff_len
        || ui1_digits == 0)
    {
        return MINFOR_INV_ARG;
    }

    for (ui1_idx = 0 ; ui1_idx < ui1_digits ; ui1_idx++)
    {
        ui1_power = (UINT8)(ui1_digits - ui1_idx - 1);
        pui1_ascii_buff[ui1_idx] = (UINT8)(ui4_number / mp3_id3_tag_power10(ui1_power))
                                    + MP3_ID3_TAG_ASCII_OFFSET;
        ui4_number %= mp3_id3_tag_power10(ui1_power);
    }

    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_tag_parse_v1_tag
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_tag_parse_v1_tag(MINFO_OBJ_T*    pt_minfo_obj)
{
    input3bytes_fct     pf_input3byte;
    copybytes_fct       pf_copybytes;
    setpos_fct          pf_setpos;
    UINT32              ui4_read;
    INT32               i4_ret = MINFOR_INTERNAL_ERR;
    CHAR                ac_text[MAX_ID3_TAG_V1_FRAME_LENGTH + 1]; /*NULL terminator*/
    ID3_TAG_SET_T*      pt_id3_set;
    MP3_STREAM_INFO_T*  pt_stream_info;

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
    pf_copybytes = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_setpos    = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;

    /*
     * check id3v1 magic word -- "TAG"
     * and parse header and set the following 5 frames in id3_tag:
     * (1) title             30
     * (2) artist            30
     * (3) album             30
     * (4) Comment           28
     * (5) year              4
     * (6) track             1
     * (7) genre             1
     */
    IS_INPUT_OK(pf_input3byte(pt_minfo_obj, &ui4_read));
    if (cui4_TAG != ui4_read)
    {
        return MINFOR_FMT_NOT_SUPPORT;
    }

    /* (1) title (2) artist (3) album (4) year*/
    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;
    if (pt_id3_set == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }

    if (pt_id3_set->at_txt_info[ID3_TAG_V2_TITLE_FRAME_IDX].z_size == 0)
    {
        x_memset(ac_text, 0, MAX_ID3_TAG_V1_FRAME_LENGTH+1);
        x_memset(pt_id3_set->at_txt_info[ID3_TAG_V2_TITLE_FRAME_IDX].awc_text, 0, (MAX_ID3_TAG_TXT_FRAME_LENGTH+1)*sizeof(UTF16_T));
        IS_INPUT_OK(pf_copybytes((VOID*)pt_minfo_obj,
                                 (VOID*)ac_text,
                                 MAX_ID3_TAG_V1_FRAME_LENGTH,
                                 MAX_ID3_TAG_V1_FRAME_LENGTH,
                                 &ui4_read));
        if (ui4_read)
        {
            /* warning */
            ac_text[MAX_ID3_TAG_V1_FRAME_LENGTH] = 0;
            mp3_id3_trim_trailing_spaces(ac_text);
            pt_id3_set->at_txt_info[ID3_TAG_V2_TITLE_FRAME_IDX].z_size = x_strlen(ac_text);

            if (pt_id3_set->at_txt_info[ID3_TAG_V2_TITLE_FRAME_IDX].z_size > 0)
            {
                x_memcpy((VOID*)pt_id3_set->at_txt_info[ID3_TAG_V2_TITLE_FRAME_IDX].awc_text,
                         (const VOID*)ac_text, MAX_ID3_TAG_V1_FRAME_LENGTH);
                pt_id3_set->at_txt_info[ID3_TAG_V2_TITLE_FRAME_IDX].e_encoding = MP3_ID3_FRAME_TEXT_ENCODING_ISO_8859_1;
            }
        }
    }
    else
    {
        pf_setpos((VOID*)pt_minfo_obj, 30, FM_SEEK_CUR); /*skip 30 byte*/
    }

    if (pt_id3_set->at_txt_info[ID3_TAG_V2_ARTIST_FRAME_IDX].z_size == 0)
    {
        x_memset(ac_text, 0, MAX_ID3_TAG_V1_FRAME_LENGTH+1);
        x_memset(pt_id3_set->at_txt_info[ID3_TAG_V2_ARTIST_FRAME_IDX].awc_text, 0, (MAX_ID3_TAG_TXT_FRAME_LENGTH+1)*sizeof(UTF16_T));
        IS_INPUT_OK(pf_copybytes((VOID*)pt_minfo_obj,
                                 (VOID*)ac_text,
                                 MAX_ID3_TAG_V1_FRAME_LENGTH,
                                 MAX_ID3_TAG_V1_FRAME_LENGTH,
                                 &ui4_read));
        if (ui4_read)
        {
            /* warning */
            ac_text[MAX_ID3_TAG_V1_FRAME_LENGTH] = 0;
            mp3_id3_trim_trailing_spaces(ac_text);
            pt_id3_set->at_txt_info[ID3_TAG_V2_ARTIST_FRAME_IDX].z_size = x_strlen(ac_text) ;
            if (pt_id3_set->at_txt_info[ID3_TAG_V2_ARTIST_FRAME_IDX].z_size > 0)
            {
                x_memcpy((VOID*)pt_id3_set->at_txt_info[ID3_TAG_V2_ARTIST_FRAME_IDX].awc_text,
                         (const VOID*)ac_text, MAX_ID3_TAG_V1_FRAME_LENGTH);
                pt_id3_set->at_txt_info[ID3_TAG_V2_ARTIST_FRAME_IDX].e_encoding = MP3_ID3_FRAME_TEXT_ENCODING_ISO_8859_1;
            }
        }
    }
    else
    {
        pf_setpos((VOID*)pt_minfo_obj, 30, FM_SEEK_CUR); /*skip 30 byte*/
    }

    if (pt_id3_set->at_txt_info[ID3_TAG_V2_ALBUM_FRAME_IDX].z_size == 0)
    {
        x_memset(ac_text, 0, (MAX_ID3_TAG_V1_FRAME_LENGTH+1));
        x_memset(pt_id3_set->at_txt_info[ID3_TAG_V2_ALBUM_FRAME_IDX].awc_text, 0, (MAX_ID3_TAG_TXT_FRAME_LENGTH+1)*sizeof(UTF16_T));
        IS_INPUT_OK(pf_copybytes((VOID*)pt_minfo_obj,
                     (VOID*)ac_text,
                     MAX_ID3_TAG_V1_FRAME_LENGTH,
                     MAX_ID3_TAG_V1_FRAME_LENGTH,
                     &ui4_read));
        if (ui4_read)
        {
            /* warning */
            ac_text[MAX_ID3_TAG_V1_FRAME_LENGTH] = 0;
            mp3_id3_trim_trailing_spaces(ac_text);
            pt_id3_set->at_txt_info[ID3_TAG_V2_ALBUM_FRAME_IDX].z_size = x_strlen(ac_text) ;
            if (pt_id3_set->at_txt_info[ID3_TAG_V2_ALBUM_FRAME_IDX].z_size > 0)
            {
                x_memcpy((VOID*)pt_id3_set->at_txt_info[ID3_TAG_V2_ALBUM_FRAME_IDX].awc_text,
                         (const VOID*)ac_text, MAX_ID3_TAG_V1_FRAME_LENGTH);
                pt_id3_set->at_txt_info[ID3_TAG_V2_ALBUM_FRAME_IDX].e_encoding = MP3_ID3_FRAME_TEXT_ENCODING_ISO_8859_1;
            }
        }
    }
    else
    {
        pf_setpos((VOID*)pt_minfo_obj, 30, FM_SEEK_CUR); /*skip 30 byte*/
    }

    if (pt_id3_set->at_txt_info[ID3_TAG_V2_YEAR_FRAME_IDX].z_size == 0)
    {
        x_memset(ac_text, 0, (MAX_ID3_TAG_V1_FRAME_LENGTH+1));
        x_memset(pt_id3_set->at_txt_info[ID3_TAG_V2_YEAR_FRAME_IDX].awc_text, 0, (MAX_ID3_TAG_TXT_FRAME_LENGTH+1)*sizeof(UTF16_T));
        IS_INPUT_OK(pf_copybytes((VOID*)pt_minfo_obj,
                                  (VOID*)ac_text,
                                  MAX_ID3_TAG_V1_FRAME_LENGTH,
                                  4,
                                  &ui4_read)); /*4 byte*/
        if (ui4_read)
        {
            /* warning */
            ac_text[MAX_ID3_TAG_V1_FRAME_LENGTH] = 0;
            mp3_id3_trim_trailing_spaces(ac_text);
            pt_id3_set->at_txt_info[ID3_TAG_V2_YEAR_FRAME_IDX].z_size = x_strlen(ac_text);
            if (pt_id3_set->at_txt_info[ID3_TAG_V2_YEAR_FRAME_IDX].z_size > 0)
            {
                x_memcpy((VOID*)pt_id3_set->at_txt_info[ID3_TAG_V2_YEAR_FRAME_IDX].awc_text,
                         (const VOID*)ac_text, MAX_ID3_TAG_V1_FRAME_LENGTH);
                pt_id3_set->at_txt_info[ID3_TAG_V2_YEAR_FRAME_IDX].e_encoding = MP3_ID3_FRAME_TEXT_ENCODING_ISO_8859_1;
            }
        }
    }
    else
    {
        pf_setpos((VOID*)pt_minfo_obj, 4, FM_SEEK_CUR); /*skip 4 byte*/
    }
    /* skip 28 + 1 bytes comment */
    pf_setpos((VOID*)pt_minfo_obj, 29, FM_SEEK_CUR); /* 29 byte*/

    if (pt_id3_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].z_size == 0)
    {
        x_memset(ac_text, 0, (MAX_ID3_TAG_V1_FRAME_LENGTH + 1));
        x_memset(pt_id3_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].awc_text, 0, (MAX_ID3_TAG_TXT_FRAME_LENGTH + 1) * sizeof(UTF16_T));
        IS_INPUT_OK(pf_copybytes((VOID*)pt_minfo_obj,
                                  (VOID*)ac_text,
                                  MAX_ID3_TAG_V1_FRAME_LENGTH,
                                  1,
                                  &ui4_read)); /* 1 byte */
        if (ui4_read)
        {
            /* convert number to ASCII string */
            i4_ret = mp3_id3_tag_num_to_ascii((UINT32)(ac_text[0]), (UINT8*)ac_text, MAX_ID3_TAG_V1_FRAME_LENGTH + 1);
            if (i4_ret != MINFOR_OK)
            {
                pt_id3_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].z_size = 0;
            }
            else
            {
                /* warning */
                ac_text[MAX_ID3_TAG_V1_FRAME_LENGTH] = 0;
                pt_id3_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].z_size = x_strlen(ac_text);
                if (pt_id3_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].z_size > 0)
                {
                    x_memcpy((VOID*)pt_id3_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].awc_text,
                             (const VOID*)ac_text, MAX_ID3_TAG_V1_FRAME_LENGTH);
                    pt_id3_set->at_txt_info[ID3_TAG_V2_TRACK_FRAME_IDX].e_encoding = MP3_ID3_FRAME_TEXT_ENCODING_ISO_8859_1;
                }
            }
        }
    }
    else
    {
        pf_setpos((VOID*)pt_minfo_obj, 1, FM_SEEK_CUR); /* skip 1 byte */
    }

    /* genre */
    if (pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].z_size == 0)
    {
        x_memset(ac_text, 0, (MAX_ID3_TAG_V1_FRAME_LENGTH + 1));
        x_memset(pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text, 0, (MAX_ID3_TAG_TXT_FRAME_LENGTH+1)*sizeof(UTF16_T));
        IS_INPUT_OK(pf_copybytes((VOID*)pt_minfo_obj,
                                  (VOID*)ac_text,
                                  MAX_ID3_TAG_V1_FRAME_LENGTH,
                                  1,
                                  &ui4_read)); /*1 byte*/
        if ((ui4_read) && (((UCHAR)(ac_text[0]))< MAX_ID3_TAG_GENRE_NUM) && (ac_text[0] >= 0))
        {
            /* warning */
            ac_text[MAX_ID3_TAG_V1_FRAME_LENGTH] = 0;
            pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].z_size = x_strlen(ac_text);
            if (pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].z_size > 0)
            {
                x_memcpy((VOID*)pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].awc_text,
                             (const VOID*)ac_text, MAX_ID3_TAG_V1_FRAME_LENGTH);
                pt_id3_set->at_txt_info[ID3_TAG_V2_GENRE_FRAME_IDX].e_encoding = MP3_ID3_FRAME_TEXT_ENCODING_ISO_8859_1;
            }
        }
    }

    if (i4_ret == MINFOR_OK)
    {
        pt_id3_set->b_is_valid = TRUE;
    }
    pt_id3_set->ui4_v1_length = 128;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_minfo_cvt_id3
 * Description:
 * Inputs:  -
 * Outputs: -
 * Returns: MINFOR_OK                          The API is successful.
 *              MINFOR_INTERNAL_ERR         The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_id3_cvt_id3_item (ID3_TAG_SET_T* pt_id3_set, UINT32 ui4_frame_id_index, ISO_639_LANG_T e_language)
{
    INT32               i4_ret          = MINFOR_OK;
    CHAR                ac_text[MAX_ID3_TAG_TXT_FRAME_LENGTH + 1];
#ifdef MM_MINFO_ID3_CH_KOR_SUPPORT    
    UINT32              ui4_count;
#endif
    
    if ((pt_id3_set == NULL) ||
        (ui4_frame_id_index >= ID3_TAG_V2_MAX_TXT_FRAME_IDX))
    {
        return MINFOR_INV_ARG;
    }

    /* Just Convert  8859_* and UTF8 */
    if ((pt_id3_set->at_txt_info[ui4_frame_id_index].e_encoding != MP3_ID3_FRAME_TEXT_ENCODING_ISO_8859_1) &&
        (pt_id3_set->at_txt_info[ui4_frame_id_index].e_encoding != MP3_ID3_FRAME_TEXT_ENCODING_UTF8))
    {
        return MINFOR_OK;
    }

    /*UTF-16 -> CHAR*/
    x_memset((VOID*)ac_text, 0, MAX_ID3_TAG_TXT_FRAME_LENGTH + 1);
    x_memcpy((VOID*)ac_text, pt_id3_set->at_txt_info[ui4_frame_id_index].awc_text, MAX_ID3_TAG_TXT_FRAME_LENGTH);
    x_memset((VOID*)pt_id3_set->at_txt_info[ui4_frame_id_index].awc_text, 0, (MAX_ID3_TAG_TXT_FRAME_LENGTH+1)*sizeof(UTF16_T));
    pt_id3_set->at_txt_info[ui4_frame_id_index].z_size = 0;
    
    do 
    {
#ifdef MINFO_ID3_ENCODE_BY_LANGUAGE
        UINT16              ui2_src_len;
        UINT16              ui2_dst_len;
        E_MM_UTIL_CHAR_TBL  ctable          = MM_UTIL_TABLE_ISO_UNKNOWN;
        
        ui2_src_len = x_strlen(ac_text);
        ui2_dst_len = MAX_ID3_TAG_TXT_FRAME_LENGTH;
        ctable = mm_cvt_lang_to_chartbl(e_language);
        
        if (ctable != MM_UTIL_TABLE_ISO_UNKNOWN)
        {
            i4_ret = mm_cvt_8859_ps_to_w2s( (UINT16)ctable,
                                            (const CHAR*)ac_text,
                                             ui2_src_len,
                                            (UTF16_T *) pt_id3_set->at_txt_info[ui4_frame_id_index].awc_text,
                                             &ui2_dst_len);
            if (i4_ret == MM_UTILR_OK)
            {
                pt_id3_set->at_txt_info[ui4_frame_id_index].z_size = ui2_dst_len;
            }
        }
        else
#endif 
        {       
#ifndef __KERNEL__
#ifdef MM_MINFO_ID3_CH_KOR_SUPPORT
            E_MUL_BYTE_TYPE e_byte_type;
            if (x_strcmp(e_language, ce_simplified_chinese) == 0)
            {
                e_byte_type = MUL_BYTE_GB2312;
            }
            else if (x_strcmp(e_language, ce_traditional_chinese) == 0)
            {
                e_byte_type = MUL_BYTE_BIG5;
            }
            else if (x_strcmp(e_language, ce_japanese) == 0)
            {
                e_byte_type = MUL_BYTE_SHIFT_JIS;
            }
            else if(x_strcmp(e_language, ce_korean) == 0)
            {
                e_byte_type = MUL_BYTE_KR;
            }
            else
#endif                
            {
                i4_ret = x_uc_ps_to_w2s(ac_text, pt_id3_set->at_txt_info[ui4_frame_id_index].awc_text, MAX_ID3_TAG_TXT_FRAME_LENGTH);
                if (i4_ret == UCR_OK)
                {
                    ac_text[MAX_ID3_TAG_TXT_FRAME_LENGTH] = 0;
                    pt_id3_set->at_txt_info[ui4_frame_id_index].z_size = x_strlen(ac_text);
                    i4_ret = MINFOR_OK;
                }
                else
                {
                    i4_ret = MINFOR_INFO_NOT_FOUND;
                }
                break;
            }
#endif  
#ifdef MM_MINFO_ID3_CH_KOR_SUPPORT
            ui4_count = cvt_2_multi_char((UCHAR*)ac_text,
                                          pt_id3_set->at_txt_info[ui4_frame_id_index].awc_text, 
                                          MAX_ID3_TAG_TXT_FRAME_LENGTH );
            #ifndef __KERNEL__
            i4_ret = x_multi_byte_to_unicode( pt_id3_set->at_txt_info[ui4_frame_id_index].awc_text,
                                              ui4_count,
                                              e_byte_type,
                                              pt_id3_set->at_txt_info[ui4_frame_id_index].awc_text );
            if (i4_ret != UCR_OK)
            {
                i4_ret = MINFOR_INFO_NOT_FOUND;
                break;
            }
            #endif
            pt_id3_set->at_txt_info[ui4_frame_id_index].z_size = ui4_count;
#endif
        }
    }while(FALSE);
    if (i4_ret != MINFOR_OK)
    {
        pt_id3_set->at_txt_info[ui4_frame_id_index].z_size = 
                    mp3_id3_pickout_ascii(ac_text, pt_id3_set->at_txt_info[ui4_frame_id_index].awc_text, MAX_ID3_TAG_TXT_FRAME_LENGTH);
    }
    pt_id3_set->at_txt_info[ui4_frame_id_index].e_encoding = MP3_ID3_FRAME_TEXT_ENCODING_UTF16_WO_BOM;
    
    return MINFOR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_minfo_cvt_id3_txt
 * Description: Convert the text info by language to Unicode 16
 * Inputs:  -
 * Outputs: -
 * Returns: MINFOR_OK                          The API is successful.
 *              MINFOR_INTERNAL_ERR         The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_minfo_cvt_id3_txt (MINFO_OBJ_T* pt_minfo_obj,  ISO_639_LANG_T e_language)
{
    INT32               i4_ret          = MINFOR_OK;
    UINT8               ui1_idx;
    ID3_TAG_SET_T*      pt_id3_set;
    MP3_STREAM_INFO_T*  pt_stream_info;

    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INV_ARG;
    }

    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;
    if (pt_id3_set == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    
    for (ui1_idx=ID3_TAG_V2_TITLE_FRAME_IDX; ui1_idx<ID3_TAG_V2_MAX_TXT_FRAME_IDX; ui1_idx++)
    {
        /* Genre: Must convert Number to String*/
        if (ui1_idx == ID3_TAG_V2_GENRE_FRAME_IDX)
        {
            mp3_id3_convert_genre(pt_minfo_obj);
            continue;
        }
        i4_ret = mp3_id3_cvt_id3_item(pt_id3_set, (UINT32)ui1_idx, e_language);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_minfo_parse_id3
 *
 * Description:
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful.
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/
INT32 mp3_minfo_parse_id3 (MINFO_OBJ_T* pt_minfo_obj, BOOL b_parse_v2_tag)
{

    setpos_fct pf_set_pos;
    INT32 i4_ret = MINFOR_INTERNAL_ERR;
    ID3_TAG_SET_T*      pt_id3_set;
    MP3_STREAM_INFO_T*  pt_stream_info;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    if ((pt_minfo_obj->t_input_fct_tbl.pf_input1 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_input2 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_input4 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL))
    {
        return MINFOR_INTERNAL_ERR;
    }

    pt_stream_info = (MP3_STREAM_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (pt_stream_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    pt_id3_set = pt_stream_info->pt_mp3_id3_tag;
    if (pt_id3_set == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* seek to beginning */
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    if(b_parse_v2_tag)
    {
        IS_INPUT_OK(pf_set_pos(pt_minfo_obj , 0, FM_SEEK_BGN));
        /*
         * check id3v2 magic word -- "ID3"
         * and parse & save data to frames
         */
        i4_ret = mp3_id3_tag_parse_v2_tag(pt_minfo_obj);
        if ((i4_ret != MINFOR_OK)
            && (i4_ret != MINFOR_FMT_NOT_SUPPORT))
        {
            return i4_ret;
        }
    }
    else
    {
        /* move to last 128 bytes before file end for checking id3v1 tag */
        IS_INPUT_OK(pf_set_pos(pt_minfo_obj , -128, FM_SEEK_END));

        i4_ret = mp3_id3_tag_parse_v1_tag(pt_minfo_obj);
        if ((i4_ret != MINFOR_OK)
            && (i4_ret != MINFOR_FMT_NOT_SUPPORT))
        {
            return i4_ret;
        }
    }

    pt_id3_set->b_is_parsing_done = TRUE ;

    return MINFOR_OK;
}

