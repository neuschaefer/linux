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
 * $RCSfile: fe_enc.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision:  $
 * $SWAuthor: Vincent Hsu $
 * $MD5HEX:  $
 *
 * Description:
 *         This header file contains font library specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/u_common.h"
#include "font/u_fe.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define GET_UINT24_FROM_PTR_BIG_END(_ptr)      \
        (((((UINT32) (((UINT8*) _ptr) [0])) << 16) | \
          (((UINT32) (((UINT8*) _ptr) [1])) <<  8) | \
          ((UINT32)  (((UINT8*) _ptr) [2]))) & 0x00ffffff)

/*-----------------------------------------------------------------------------
                    static variables
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static SIZE_T    _utf8_strlen          (const UINT8 *pui1_str);
static SIZE_T    _utf16_strlen         (const UINT8 *pui1_str);
static SIZE_T    _sjis_strlen          (const UINT8 *pui1_str);
static SIZE_T    _euckr_strlen         (const UINT8 *pui1_str);
static SIZE_T    _gb18030_strlen       (const UINT8 *pui1_str);
static SIZE_T    _gb2312_strlen        (const UINT8 *pui1_str);
static SIZE_T    _big5_strlen          (const UINT8 *pui1_str);

static CHAR_CODE _get_utf8_char        (const UINT8 *pui1_str, UINT32 ui4_index);
static CHAR_CODE _get_utf16_char       (const UINT8 *pui1_str, UINT32 ui4_index);
static CHAR_CODE _get_sjif_char        (const UINT8 *pui1_str, UINT32 ui4_index);
static CHAR_CODE _get_euckr_char       (const UINT8 *pui1_str, UINT32 ui4_index);
static CHAR_CODE _get_gb18030_char     (const UINT8 *pui1_str, UINT32 ui4_index);
static CHAR_CODE _get_gb2312_char      (const UINT8 *pui1_str, UINT32 ui4_index);
static CHAR_CODE _get_big5_char        (const UINT8 *pui1_str, UINT32 ui4_index);

static SIZE_T    _parse_utf8_string    (const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code);
static SIZE_T    _parse_utf16_string   (const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code);
static SIZE_T    _parse_sjis_string    (const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code);
static SIZE_T    _parse_euckr_string   (const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code);
static SIZE_T    _parse_gb18030_string (const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code);
static SIZE_T    _parse_gb2312_string  (const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code);
static SIZE_T    _parse_big5_string    (const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code);

SIZE_T fe_strlen(
    const UINT8        *pui1_str,
    FE_ENCODING_TYPE_T e_encoding)
{
    switch (e_encoding)
    {
        case FE_ENCODING_UTF8:
            return _utf8_strlen(pui1_str);

        case FE_ENCODING_UTF16:
            return _utf16_strlen(pui1_str);

        case FE_ENCODING_SHIFT_JIS:
            return _sjis_strlen(pui1_str);

        case FE_ENCODING_EUC_KR:
            return _euckr_strlen(pui1_str);

        case FE_ENCODING_GB18030:
            return _gb18030_strlen(pui1_str);

        case FE_ENCODING_GB2312:
            return _gb2312_strlen(pui1_str);

        case FE_ENCODING_BIG5:
            return _big5_strlen(pui1_str);

        default:
            return 0;
    }
}

CHAR_CODE fe_get_char(
    const UINT8        *pui1_str,
    UINT32             ui4_index,
    FE_ENCODING_TYPE_T e_encoding)
{
    switch (e_encoding)
    {
        case FE_ENCODING_UTF8:
            return _get_utf8_char(pui1_str, ui4_index);

        case FE_ENCODING_UTF16:
            return _get_utf16_char(pui1_str, ui4_index);

        case FE_ENCODING_SHIFT_JIS:
            return _get_sjif_char(pui1_str, ui4_index);

        case FE_ENCODING_EUC_KR:
            return _get_euckr_char(pui1_str, ui4_index);

        case FE_ENCODING_GB18030:
            return _get_gb18030_char(pui1_str, ui4_index);

        case FE_ENCODING_GB2312:
            return _get_gb2312_char(pui1_str, ui4_index);

        case FE_ENCODING_BIG5:
            return _get_big5_char(pui1_str, ui4_index);

        default:
            return 0;
    }
}

static SIZE_T _utf8_strlen(const UINT8 *pui1_str)
{
    return _parse_utf8_string(pui1_str, 0, NULL);
}

static SIZE_T _utf16_strlen(const UINT8 *pui1_str)
{
    return _parse_utf16_string(pui1_str, 0, NULL);
}

static SIZE_T _sjis_strlen(const UINT8 *pui1_str)
{
    return _parse_sjis_string(pui1_str, 0, NULL);
}

static SIZE_T _euckr_strlen(const UINT8 *pui1_str)
{
    return _parse_euckr_string(pui1_str, 0, NULL);
}

static SIZE_T _gb18030_strlen(const UINT8 *pui1_str)
{
    return _parse_gb18030_string(pui1_str, 0, NULL);
}

static SIZE_T _gb2312_strlen(const UINT8 *pui1_str)
{
    return _parse_gb2312_string(pui1_str, 0, NULL);
}

static SIZE_T _big5_strlen(const UINT8 *pui1_str)
{
    return _parse_big5_string(pui1_str, 0, NULL);
}

static CHAR_CODE _get_utf8_char(const UINT8 *pui1_str, UINT32 ui4_index)
{
    CHAR_CODE   t_code;

    _parse_utf8_string(pui1_str, ui4_index, &t_code);

    return t_code;
}

static CHAR_CODE _get_utf16_char(const UINT8 *pui1_str, UINT32 ui4_index)
{
    CHAR_CODE   t_code;

    _parse_utf16_string(pui1_str, ui4_index, &t_code);

    return t_code;
}

static CHAR_CODE _get_sjif_char(const UINT8 *pui1_str, UINT32 ui4_index)
{
    CHAR_CODE   t_code;

    _parse_sjis_string(pui1_str, ui4_index, &t_code);

    return t_code;
}

static CHAR_CODE _get_euckr_char(const UINT8 *pui1_str, UINT32 ui4_index)
{
    CHAR_CODE   t_code;

    _parse_euckr_string(pui1_str, ui4_index, &t_code);

    return t_code;
}

static CHAR_CODE _get_gb18030_char(const UINT8 *pui1_str, UINT32 ui4_index)
{
    CHAR_CODE   t_code;

    _parse_gb18030_string(pui1_str, ui4_index, &t_code);

    return t_code;
}

static CHAR_CODE _get_gb2312_char(const UINT8 *pui1_str, UINT32 ui4_index)
{
    CHAR_CODE   t_code;

    _parse_gb2312_string(pui1_str, ui4_index, &t_code);

    return t_code;
}

static CHAR_CODE _get_big5_char(const UINT8 *pui1_str, UINT32 ui4_index)
{
    CHAR_CODE   t_code;

    _parse_big5_string(pui1_str, ui4_index, &t_code);
    
    return t_code;
}

/*-----------------------------------------------------------------------------
 * Name:  _parse_utf8_string
 *
 * Description: This function gets the string length or the character code of 
 *              a specific character.
 *
 * Inputs:  pui1_str            pointer to the string.
 *          ui4_index           index of the character.
 *
 * Outputs: pt_code             character code of a specific character
 *
 * Returns: If pt_code is NULL, the string length would be returned.
 ----------------------------------------------------------------------------*/
static SIZE_T _parse_utf8_string(const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code)
{
    UINT32      ui4_i    = 0;
    SIZE_T      z_strlen = 0;

    if (pt_code != NULL)
    {
        *pt_code = 0;
    }
    
    if (pui1_str == NULL)
    {
        return 0;
    }

    while (1)
    {
        UINT8   ui1_first_byte;

        ui1_first_byte  = pui1_str[ui4_i];

        /* 1 byte */
        if (ui1_first_byte <= 0x7f && ui1_first_byte >= 0x01)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)ui1_first_byte;
                break;
            }

            ui4_i    += 1;
            z_strlen += 1;
        }
        /* 2 bytes */
        else if (ui1_first_byte <= 0xdf && ui1_first_byte >= 0xc2)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)GET_UINT16_FROM_PTR_BIG_END(&pui1_str[ui4_i]);
                break;
            }

            ui4_i    += 2;
            z_strlen += 1;
        }
        /* 3 bytes */
        else if (ui1_first_byte <= 0xef && ui1_first_byte >= 0xe0)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)GET_UINT24_FROM_PTR_BIG_END(&pui1_str[ui4_i]);
                break;
            }

            ui4_i    += 3;
            z_strlen += 1;
        }
        /* 4 bytes */
        else if (ui1_first_byte <= 0xf4 && ui1_first_byte >= 0xf0)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)GET_UINT32_FROM_PTR_BIG_END(&pui1_str[ui4_i]);
                break;
            }

            ui4_i    += 4;
            z_strlen += 1;
        }
        /* NULL terminated */
        else
        {
            break;
        }
    }

    return z_strlen;
}

/*-----------------------------------------------------------------------------
 * Name:  _parse_utf16_string
 *
 * Description: This function gets the string length or the character code of 
 *              a specific character.
 *
 * Inputs:  pui1_str            pointer to the string.
 *          ui4_index           index of the character.
 *
 * Outputs: pt_code             character code of a specific character
 *
 * Returns: If pt_code is NULL, the string length would be returned.
 ----------------------------------------------------------------------------*/
static SIZE_T _parse_utf16_string(const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code)
{
    UINT32      ui4_i    = 0;
    SIZE_T      z_strlen = 0;

    if (pt_code != NULL)
    {
        *pt_code = 0;
    }
    
    if (pui1_str == NULL)
    {
        return 0;
    }

    while (1)
    {
        UINT16  ui2_first_word, ui2_second_word;

        ui2_first_word  = GET_UINT16_FROM_PTR_BIG_END(&pui1_str[ui4_i]);
        ui2_second_word = GET_UINT16_FROM_PTR_BIG_END(&pui1_str[ui4_i + 2]);

        /* UCS-2 */
        if (ui2_first_word < 0xd800 && ui2_first_word > 0xdfff)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)ui2_first_word;
                break;
            }

            ui4_i    += 2;
            z_strlen += 1;
        }
        /* UTF-16 */
        else if (ui2_first_word != 0x0000 || ui2_second_word != 0x0000)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)GET_UINT32_FROM_PTR_BIG_END(&pui1_str[ui4_i]);
                break;
            }

            ui4_i    += 4;
            z_strlen += 1;
        }
        /* NULL terminated */
        else
        {
            break;
        }
    }

    return z_strlen;
}

/*-----------------------------------------------------------------------------
 * Name:  _parse_sjis_string
 *
 * Description: This function gets the double-byte-character-set string length 
 *              or the character code of a specific character. Based on Windows
 *              Codepage 932.
 *
 * Inputs:  pui1_str            pointer to the string.
 *          ui4_index           index of the character.
 *
 * Outputs: pt_code             character code of a specific character
 *
 * Returns: If pt_code is NULL, the string length would be returned.
 ----------------------------------------------------------------------------*/
static SIZE_T _parse_sjis_string(const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code)
{
    UINT32      ui4_i    = 0;
    SIZE_T      z_strlen = 0;

    if (pt_code != NULL)
    {
        *pt_code = 0;
    }
    
    if (pui1_str == NULL)
    {
        return 0;
    }

    while (1)
    {
        UINT8   ui1_first_byte, ui1_second_byte;

        ui1_first_byte  = pui1_str[ui4_i];
        ui1_second_byte = pui1_str[ui4_i + 1];

        /* SBCS */
        if ((ui1_first_byte <= 0x7f && ui1_first_byte >= 0x01) ||
            (ui1_first_byte <= 0xdf && ui1_first_byte >= 0xa1))
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)ui1_first_byte;
                break;
            }

            ui4_i    += 1;
            z_strlen += 1;
        }
        /* DBCS */
        else if (ui1_first_byte != 0x00 || ui1_second_byte != 0x00)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)GET_UINT16_FROM_PTR_BIG_END(&pui1_str[ui4_i]);
                break;
            }

            ui4_i    += 2;
            z_strlen += 1;
        }
        /* NULL terminated */
        else
        {
            break;
        }
    }

    return z_strlen;
}

/*-----------------------------------------------------------------------------
 * Name:  _parse_euckr_string
 *
 * Description: This function gets the double-byte-character-set string length 
 *              or the character code of a specific character. Based on Windows
 *              Codepage 949.
 *
 * Inputs:  pui1_str            pointer to the string.
 *          ui4_index           index of the character.
 *
 * Outputs: pt_code             character code of a specific character
 *
 * Returns: If pt_code is NULL, the string length would be returned.
 ----------------------------------------------------------------------------*/
static SIZE_T _parse_euckr_string(const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code)
{
    UINT32      ui4_i    = 0;
    SIZE_T      z_strlen = 0;

    if (pt_code != NULL)
    {
        *pt_code = 0;
    }
    
    if (pui1_str == NULL)
    {
        return 0;
    }

    while (1)
    {
        UINT8   ui1_first_byte, ui1_second_byte;

        ui1_first_byte  = pui1_str[ui4_i];
        ui1_second_byte = pui1_str[ui4_i + 1];

        /* SBCS */
        if (ui1_first_byte <= 0x7f && ui1_first_byte >= 0x01)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)ui1_first_byte;
                break;
            }

            ui4_i    += 1;
            z_strlen += 1;
        }
        /* DBCS */
        else if (ui1_first_byte != 0x00 || ui1_second_byte != 0x00)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)GET_UINT16_FROM_PTR_BIG_END(&pui1_str[ui4_i]);
                break;
            }

            ui4_i    += 2;
            z_strlen += 1;
        }
        /* NULL terminated */
        else
        {
            break;
        }
    }

    return z_strlen;
}

/*-----------------------------------------------------------------------------
 * Name:  _parse_gb18030_string
 *
 * Description: This function gets the double-byte-character-set string length 
 *              or the character code of a specific character. Based on IBM
 *              GB 18030 mega codepage.
 *
 * Inputs:  pui1_str            pointer to the string.
 *          ui4_index           index of the character.
 *
 * Outputs: pt_code             character code of a specific character
 *
 * Returns: If pt_code is NULL, the string length would be returned.
 ----------------------------------------------------------------------------*/
static SIZE_T _parse_gb18030_string(const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code)
{
    UINT32      ui4_i    = 0;
    SIZE_T      z_strlen = 0;

    if (pt_code != NULL)
    {
        *pt_code = 0;
    }
    
    if (pui1_str == NULL)
    {
        return 0;
    }

    while (1)
    {
        UINT8   ui1_first_byte, ui1_second_byte, ui1_third_byte, ui1_fourth_byte;

        ui1_first_byte  = pui1_str[ui4_i];
        ui1_second_byte = pui1_str[ui4_i + 1];
        ui1_third_byte  = pui1_str[ui4_i + 2];
        ui1_fourth_byte = pui1_str[ui4_i + 3];

        /* SBCS */
        if (ui1_first_byte <= 0x80 && ui1_first_byte >= 0x01)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)ui1_first_byte;
                break;
            }

            ui4_i    += 1;
            z_strlen += 1;
        }
        /* DBCS */
        else if ((ui1_first_byte >= 0x81 && ui1_first_byte <= 0xfe) &&
                 ((ui1_second_byte >= 0x40 && ui1_second_byte <= 0x7e) ||
                  (ui1_second_byte >= 0x80 && ui1_second_byte <= 0xfe)))
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)GET_UINT16_FROM_PTR_BIG_END(&pui1_str[ui4_i]);
                break;
            }

            ui4_i    += 2;
            z_strlen += 1;
        }
        /* four bytes */
        else if (ui1_first_byte != 0 || ui1_second_byte != 0 || ui1_third_byte != 0 || ui1_fourth_byte != 0)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)GET_UINT32_FROM_PTR_BIG_END(&pui1_str[ui4_i]);
                break;
            }

            ui4_i    += 4;
            z_strlen += 1;
        }
        /* NULL terminated */
        else
        {
            break;
        }
    }

    return z_strlen;
}

/*-----------------------------------------------------------------------------
 * Name:  _parse_gb2312_string
 *
 * Description: This function gets the double-byte-character-set string length 
 *              or the character code of a specific character. Based on Windows
 *              Codepage 936.
 *
 * Inputs:  pui1_str            pointer to the string.
 *          ui4_index           index of the character.
 *
 * Outputs: pt_code             character code of a specific character
 *
 * Returns: If pt_code is NULL, the string length would be returned.
 ----------------------------------------------------------------------------*/
static SIZE_T _parse_gb2312_string(const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code)
{
    UINT32      ui4_i    = 0;
    SIZE_T      z_strlen = 0;

    if (pt_code != NULL)
    {
        *pt_code = 0;
    }
    
    if (pui1_str == NULL)
    {
        return 0;
    }

    while (1)
    {
        UINT8   ui1_first_byte, ui1_second_byte;

        ui1_first_byte  = pui1_str[ui4_i];
        ui1_second_byte = pui1_str[ui4_i + 1];

        /* SBCS */
        if (ui1_first_byte <= 0x80 && ui1_first_byte >= 0x01)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)ui1_first_byte;
                break;
            }

            ui4_i    += 1;
            z_strlen += 1;
        }
        /* DBCS */
        else if (ui1_first_byte != 0x00 || ui1_second_byte != 0x00)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)GET_UINT16_FROM_PTR_BIG_END(&pui1_str[ui4_i]);
                break;
            }

            ui4_i    += 2;
            z_strlen += 1;
        }
        /* NULL terminated */
        else
        {
            break;
        }
    }

    return z_strlen;
}

/*-----------------------------------------------------------------------------
 * Name:  _parse_big5_string
 *
 * Description: This function gets the double-byte-character-set string length 
 *              or the character code of a specific character. Based on Windows
 *              Codepage 950.
 *
 * Inputs:  pui1_str            pointer to the string.
 *          ui4_index           index of the character.
 *
 * Outputs: pt_code             character code of a specific character
 *
 * Returns: If pt_code is NULL, the string length would be returned.
 ----------------------------------------------------------------------------*/
static SIZE_T _parse_big5_string(const UINT8 *pui1_str, UINT32 ui4_index, CHAR_CODE *pt_code)
{
    UINT32      ui4_i    = 0;
    SIZE_T      z_strlen = 0;

    if (pt_code != NULL)
    {
        *pt_code = 0;
    }
    
    if (pui1_str == NULL)
    {
        return 0;
    }

    while (1)
    {
        UINT8   ui1_first_byte, ui1_second_byte;

        ui1_first_byte  = pui1_str[ui4_i];
        ui1_second_byte = pui1_str[ui4_i + 1];

        /* ASCII [1, 127] */
        if (ui1_first_byte <= 0x7f && ui1_first_byte >= 0x01)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)ui1_first_byte;
                break;
            }

            ui4_i    += 1;
            z_strlen += 1;
        }
        /* DBCS */
        else if (ui1_first_byte != 0x00 || ui1_second_byte != 0x00)
        {
            if (pt_code != NULL && z_strlen == (SIZE_T)ui4_index)
            {
                *pt_code = (CHAR_CODE)GET_UINT16_FROM_PTR_BIG_END(&pui1_str[ui4_i]);
                break;
            }

            ui4_i    += 2;
            z_strlen += 1;
        }
        /* NULL terminated */
        else
        {
            break;
        }
    }

    return z_strlen;
}

