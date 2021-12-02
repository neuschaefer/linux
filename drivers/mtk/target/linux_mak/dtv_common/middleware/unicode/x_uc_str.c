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
/*----------------------------------------------------------------------------*/
/*! @file x_uc_str.c 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file contains exported 'Unicode String' functions.
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#ifndef ANDROID
#include "os/inc/x_os.h"
#endif
#include "dbg/x_dbg.h"
#include "unicode/uc_str.h"
#include "unicode/x_uc_str.h"

#include "unicode/uc_cmn/unicode/utypes.h"
#include "unicode/uc_cmn/unicode/ustring.h"
#include "mutil_2_unicode/cvt_to_unicode.h"

/*-----------------------------------------------------------------------------
 * Name:  x_uc_ps_to_w2s
 *
 * Description: This API convert a UTF-8 string to UTF-16 string.
 *              String is NULL-terminated.
 *
 * Inputs:  ps_src                  Source string to be converted.
 *          ui4_dst_len             Buffer size of w2s_dst, in character units.
 *
 * Outputs: w2s_dst                 The converted string.
 *
 * Returns: UCR_ERR_ARG             Argument error.
 *          UCR_INV_CHAR            The source string contains invalid
 *                                  character(s).
 *          UCR_NO_ENOUGH_BUFFER    Buffer overflow.
 *          UCR_INTERNAL            Unicode internal error.
 *          UCR_OK                  Success.
 ----------------------------------------------------------------------------*/
INT32 x_uc_ps_to_w2s(
    const CHAR*     ps_src,
    UTF16_T*        w2s_dst,
    UINT32          ui4_dst_len)
{
    INT32 i4_length;
    UErrorCode errorCode;

    /*
    DBG_API(("[UC] ps_to_w2s: ps_src=%s, ui4_dst_len=%d\n", ps_src, ui4_dst_len));
    */

    if( ps_src  == (CHAR*)NULL ||
        w2s_dst == (UTF16_T*)NULL )
    {
        return UCR_ERR_ARG;
    }

    errorCode = U_ZERO_ERROR;
    u_strFromUTF8(w2s_dst, ui4_dst_len, (int32_t*)&i4_length, ps_src, -1, &errorCode);
    if(U_FAILURE(errorCode))
    {
        switch(errorCode)
        {
            case U_ILLEGAL_ARGUMENT_ERROR:
                return UCR_ERR_ARG;
            case U_INVALID_CHAR_FOUND:
                return UCR_INV_CHAR;
            case U_BUFFER_OVERFLOW_ERROR:
                return UCR_NO_ENOUGH_BUFFER;
            default:    /* never happen */
                return UCR_INTERNAL;
        }
    }

    return UCR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_to_ps
 *
 * Description: This API convert a UTF-16 string to UTF-8 string.
 *              String is NULL-terminated.
 *
 * Inputs:  w2s_src                 Source string to be converted.
 *          ui4_dst_len             Buffer size of ps_dst, in character units.
 *
 * Outputs: ps_dst                  The converted string.
 *
 * Returns: UCR_ERR_ARG             Argument error.
 *          UCR_INV_CHAR            The source string contains invalid
 *                                  character(s).
 *          UCR_NO_ENOUGH_BUFFER    Buffer overflow.
 *          UCR_INTERNAL            Unicode internal error.
 *          UCR_OK                  Success.
 ----------------------------------------------------------------------------*/
INT32 x_uc_w2s_to_ps(
    const UTF16_T*  w2s_src,
    CHAR*           ps_dst,
    UINT32          ui4_dst_len)
{
    int32_t length;
    UErrorCode errorCode;

    /*
    DBG_API(("[UC] w2s_to_ps: ui4_dst_len=%d\n", ui4_dst_len));
    */

    if( w2s_src == (UTF16_T*)NULL ||
        ps_dst  == (CHAR*)NULL )
    {
        return UCR_ERR_ARG;
    }

    errorCode = U_ZERO_ERROR;
    u_strToUTF8(ps_dst, ui4_dst_len, &length, w2s_src, -1, &errorCode);
    if(U_FAILURE(errorCode))
    {
        switch(errorCode)
        {
            case U_ILLEGAL_ARGUMENT_ERROR:
                return UCR_ERR_ARG;
            case U_INVALID_CHAR_FOUND:
                return UCR_INV_CHAR;
            case U_BUFFER_OVERFLOW_ERROR:
                return UCR_NO_ENOUGH_BUFFER;
            default:    /* never happen */
                return UCR_INTERNAL;
        }
    }

    return UCR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_strlen
 *
 * Description: This API counts the number of UTF16_T characters in a string.
 *              String is NULL-terminated.
 *
 * Inputs:  w2s_str                 UTF-16 string to be counted.
 *
 * Outputs: -
 *
 * Returns: string length (UTF16_T characters).
 *          return 0 if w2s_str is NULL
 ----------------------------------------------------------------------------*/
SIZE_T x_uc_w2s_strlen(
    const UTF16_T*  w2s_str)
{
    /*
    DBG_API(("[UC] w2s_strlen:\n"));
    */

    if(w2s_str == (UTF16_T*)NULL)
    {
        return (SIZE_T)0;
    }

    return (SIZE_T)u_strlen(w2s_str);
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_strcmp
 *
 * Description: This API compare two UTF-16 strings.
 *
 * Inputs:  w2s_str1                UTF-16 string 1 to be compared.
 *          w2s_str2                UTF-16 string 2 to be compared.
 *
 * Outputs: -
 *
 * Returns: > 0, w2s_str1 is larger than w2s_str2
 *          = 0, w2s_str1 is equal to w2s_str2
 *          < 0, w2s_str2 is larger than w2s_str1
 ----------------------------------------------------------------------------*/
INT32 x_uc_w2s_strcmp(
    const UTF16_T*  w2s_str1,
    const UTF16_T*  w2s_str2)
{
    /*
    DBG_API(("[UC] w2s_strcmp:\n"));
    */

    if (w2s_str1 == (UTF16_T *)NULL)
    {
        if (w2s_str2 != (UTF16_T *)NULL)
        {
            return(-1);
        }
        else
        {
            return(0);
        }
    }
    else if (w2s_str2 == (UTF16_T *)NULL)
    {
        return(1);
    }

    return (INT32)u_strcmp(w2s_str1, w2s_str2);
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_strncmp
 *
 * Description: This API compare the first ui4_len characters of two
 *              UTF-16 strings.
 *
 * Inputs:  w2s_str1                UTF-16 string 1 to be compared.
 *          w2s_str2                UTF-16 string 2 to be compared.
 *          ui4_len                 The maximum length to be compared.
 *
 * Outputs: -
 *
 * Returns: > 0, w2s_str1 is larger than w2s_str2
 *          = 0, w2s_str1 is equal to w2s_str2
 *          < 0, w2s_str2 is larger than w2s_str1
 ----------------------------------------------------------------------------*/
INT32 x_uc_w2s_strncmp(
    const UTF16_T*  w2s_str1,
    const UTF16_T*  w2s_str2,
    UINT32          ui4_len)
{
    /*
    DBG_API(("[UC] w2s_strncmp: ui4_len=%d\n", ui4_len));
    */

    if (ui4_len == 0)
    {
        return 0;
    }

    if (w2s_str1 == (UTF16_T *)NULL)
    {
        if (w2s_str2 != (UTF16_T *)NULL)
        {
            return(-1);
        }
        else
        {
            return(0);
        }
    }
    else if (w2s_str2 == (UTF16_T *)NULL)
    {
        return(1);
    }

    return (INT32)u_strncmp(w2s_str1, w2s_str2, ui4_len);
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_strcat
 *
 * Description: This API cascade source string to the end of destination string.
 *
 * Inputs:  w2s_dst                 Null-terminated destination string.
 *          w2s_src                 Null-terminated source string.
 *
 * Outputs: w2s_dst                 Null-terminated combined string.
 *
 * Returns: pointer of combined string
 ----------------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_strcat(
    UTF16_T*        w2s_dst,
    const UTF16_T*  w2s_src)
{
    /*
    DBG_API(("[UC] w2s_strcat:\n"));
    */

    if( (w2s_dst == (UTF16_T *)NULL) ||
        (w2s_src == (UTF16_T *)NULL) )
    {
        return(w2s_dst);
    }

    return (UTF16_T *)u_strcat(w2s_dst, w2s_src);
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_strncat
 *
 * Description: This API cascade ui4_len characters of source string to
 *              the end of destination string.
 *
 * Inputs:  w2s_dst                 Null-terminated destination string.
 *          w2s_src                 Null-terminated source string.
 *          ui4_len                 Maximum length of source strings to be
 *                                  cascaded.
 *
 * Outputs: w2s_dst                 Null-terminated combined string.
 *
 * Returns: pointer of combined string
 ----------------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_strncat(
    UTF16_T*        w2s_dst,
    const UTF16_T*  w2s_src,
    UINT32          ui4_len)
{
    /*
    DBG_API(("[UC] w2s_strncat: ui4_len=%d\n", ui4_len));
    */

    if( (w2s_dst == (UTF16_T *)NULL) ||
        (w2s_src == (UTF16_T *)NULL) )
    {
        return(w2s_dst);
    }

    return (UTF16_T *)u_strncat(w2s_dst, w2s_src, ui4_len);
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_strcpy
 *
 * Description: The x_uc_w2s_strcpy function copies w2s_src, including the
 *              terminating null character, to the location w2s_dst.
 *              The original data in w2s_dst (if any) will be overrided.
 *
 * Inputs:  w2s_dst                 Null-terminated destination string.
 *          w2s_src                 Null-terminated source string.
 *
 * Outputs: w2s_dst                 Null-terminated combined string.
 *
 * Returns: pointer of w2s_dst
 ----------------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_strcpy(
    UTF16_T*        w2s_dst,
    const UTF16_T*  w2s_src)
{
    /*
    DBG_API(("[UC] w2s_strcpy:\n"));
    */

    if( (w2s_dst == (UTF16_T *)NULL) ||
        (w2s_src == (UTF16_T *)NULL) )
    {
        return(w2s_dst);
    }

    return (UTF16_T *)u_strcpy(w2s_dst, w2s_src);
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_strncpy
 *
 * Description: The x_uc_w2s_strncpy function copies the initial ui4_len
 *              characters of w2s_src to w2s_dst and returns w2s_dst. If
 *              ui4_len is less than or equal to the length of w2s_src, a null
 *              character is NOT appended automatically to the copied string.
 *              If ui4_len is greater than the length of w2s_src, the
 *              destination string is padded with null characters up to length
 *              ui4_len.
 *
 * Inputs:  w2s_dst                 Null-terminated destination string.
 *          w2s_src                 Null-terminated source string.
 *          ui4_len                 Number of characters to be copied.
 *
 * Outputs: w2s_dst                 Null-terminated destination string.
 *
 * Returns: pointer of w2s_dst
 ----------------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_strncpy(
    UTF16_T*        w2s_dst,
    const UTF16_T*  w2s_src,
    UINT32          ui4_len)
{
    /*
    DBG_API(("[UC] w2s_strncpy: ui4_len=%d\n", ui4_len));
    */

    if( (w2s_dst == (UTF16_T *)NULL) ||
        (w2s_src == (UTF16_T *)NULL) )
    {
        return(w2s_dst);
    }

    return (UTF16_T *)u_strncpy(w2s_dst, w2s_src, ui4_len);
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_get
 *
 * Description: Get the specified index character of a string.
 *
 * Inputs:  w2s_str                 String to be indexed.
 *          ui4_idx                 The index of the needed character.
 *                                  Begin from 0.
 *
 * Outputs: -
 *
 * Returns: The character in the specified index.
 *          if w2s_str is NULL, return 0
 ----------------------------------------------------------------------------*/
UTF16_T x_uc_w2s_get(
    const UTF16_T*  w2s_str,
    UINT32          ui4_idx)
{
    /*
    DBG_API(("[UC] w2s_get: ui4_idx=%d\n", ui4_idx));
    */

    if(w2s_str == (UTF16_T*)NULL)
    {
        return (UTF16_T)0;
    }

    return w2s_str[ui4_idx];
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_rget
 *
 * Description: Get the specified index character of a string from right.
 *
 * Inputs:  w2s_str                 String to be indexed.
            ui4_len                 length of w2s_str
 *          ui4_idx                 The index of the needed character.
 *                                  Begin from 0.
 *
 * Outputs: -
 *
 * Returns: The character in the specified index.
 *          if w2s_str is NULL, return 0
 ----------------------------------------------------------------------------*/
UTF16_T x_uc_w2s_rget(
    const UTF16_T*  w2s_str,
    UINT32          ui4_len,
    UINT32          ui4_idx)
{
    /*
    DBG_API(("[UC] w2s_get: ui4_idx=%d\n", ui4_idx));
    */

    if(w2s_str == (UTF16_T*)NULL)
    {
        return (UTF16_T)0;
    }

    return w2s_str[ui4_idx];
}



/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_get_ptr
 *
 * Description: Get pointer of the specified index character of a string.
 *
 * Inputs:  w2s_str                 String to be indexed.
 *          ui4_idx                 The index of the needed character.
 *                                  Begin from 0.
 *
 * Outputs: -
 *
 * Returns: The pointer of the character in the specified index.
 ----------------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_get_ptr(
    const UTF16_T*  w2s_str,
    UINT32          ui4_idx)
{
    /*
    DBG_API(("[UC] w2s_get_ptr: ui4_idx=%d\n", ui4_idx));
    */

    if(w2s_str == (UTF16_T*)NULL)
    {
        return (UTF16_T*)NULL;
    }

    return (UTF16_T*)(w2s_str+ui4_idx);
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_set
 *
 * Description: Modify the specified index character of a string.
 *
 * Inputs:  w2s_str                 String to be modified.
 *          ui4_idx                 The character index to be modified.
 *                                  Begin from 0.
 *          w2_char                 New character to replace the old one.
 *
 * Outputs: -
 *
 * Returns: UCR_OK                  Success.
 *          UCR_ERR_ARG             Parameter error.
 ----------------------------------------------------------------------------*/
INT32 x_uc_w2s_set(
    UTF16_T*        w2s_str,
    UINT32          ui4_idx,
    UTF16_T         w2_char)
{
    /*
    DBG_API(("[UC] w2s_set: ui4_idx=%d\n", ui4_idx));
    */

    if(w2s_str == (UTF16_T*)NULL)
    {
        return UCR_ERR_ARG;
    }

    w2s_str[ui4_idx] = w2_char;
    return UCR_OK;
}

/**
    Reverse
 **/
/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_reverse
 *
 * Description: Reverse a string.
 *
 * Inputs:  w2s_str                 String to be reversed.
 *
 * Outputs: w2s_str                 String has been reversed.
 *
 * Returns: UCR_OK                  Success.
 *          UCR_ERR_ARG             Parameter error.
 ----------------------------------------------------------------------------*/

INT32 x_uc_w2s_reverse(
    UTF16_T*  w2s_str)
{
    INT32       i4_len;
    UTF16_T     *pw2c_first, *pw2c_last;
    if (w2s_str == (UTF16_T *)NULL)
    {
        return UCR_ERR_ARG;
    }
    
    i4_len = (INT32)x_uc_w2s_strlen(w2s_str);
    if (i4_len == 0)
    {
        return UCR_OK;
    }
    pw2c_first = w2s_str;
    pw2c_last = (UTF16_T*)(w2s_str + i4_len - 1);

    for (;pw2c_first < pw2c_last;pw2c_first++, pw2c_last--)
    {
        *pw2c_first = *pw2c_first + *pw2c_last;
        *pw2c_last = *pw2c_first - *pw2c_last;
        *pw2c_first = *pw2c_first - *pw2c_last;
    } 
    return UCR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_chr
 *
 * Description: Find a character in a string.
 *
 * Inputs:  w2s_str                 String to be searched in.
 *          w2_ch                   The character to find.
 *
 * Outputs: -
 *
 * Returns: if found                The pointer of the character.
 *          if not                  NULL
 ----------------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_chr(
    const UTF16_T*  w2s_str,
    UTF16_T         w2_ch)
{
    INT32       /*i4_i, */i4_len;
    UTF16_T     *w2s_tmp = (UTF16_T *)w2s_str, *w2s_end;

    /*
    DBG_API(("[UC] w2s_chr:\n"));
    */

    if(w2s_str == (UTF16_T*)NULL)
    {
        return((UTF16_T*)NULL);
    }

    i4_len = (INT32)x_uc_w2s_strlen(w2s_str);

    w2s_end = w2s_tmp + i4_len;
    
    for(/*i4_i=0; i4_i<i4_len; i4_i++*/ ; w2s_tmp < w2s_end; ++w2s_tmp)
    {
        if(/*x_uc_w2s_get(w2s_str, (UINT32)i4_i)*/(*w2s_tmp) == w2_ch)
        {
            /*return x_uc_w2s_get_ptr(w2s_str, (UINT32)i4_i);*/
            return w2s_tmp;
        }
    }

    return NULL;
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_chr_ex
 *
 * Description: Find a character in a string.
 *
 * Inputs:  w2s_str                 String to be searched in.
 *          w2_ch                   The character to find.
 *
 * Outputs: pui4_pos                Position of the character. Begin from 0.
 *
 * Returns: if found                The pointer of the character.
 *          if not or error         NULL
 ----------------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_chr_ex(
    const UTF16_T*  w2s_str,
    UTF16_T         w2_ch,
    UINT32*         pui4_pos)
{
    INT32       i4_i, i4_len;

    /*
    DBG_API(("[UC] w2s_chr_ex:\n"));
    */

    if( w2s_str  == (UTF16_T*)NULL ||
        pui4_pos == (UINT32*)NULL )
    {
        return((UTF16_T*)NULL);
    }

    i4_len = (INT32)x_uc_w2s_strlen(w2s_str);
    for(i4_i=0; i4_i<i4_len; i4_i++)
    {
        if(x_uc_w2s_get(w2s_str, (UINT32)i4_i) == w2_ch)
        {
            *pui4_pos = (UINT32)i4_i;
            return x_uc_w2s_get_ptr(w2s_str, (UINT32)i4_i);
        }
    }

    return NULL;
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_rchr
 *
 * Description: Find a character in a string from end of string.
 *
 * Inputs:  w2s_str                 String to be searched in.
 *          w2_ch                   The character to find.
 *
 * Outputs: -
 *
 * Returns: if found                The pointer of the character.
 *          if not                  NULL
 ----------------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_rchr(
    const UTF16_T*  w2s_str,
    UTF16_T         w2_ch)
{
    INT32       i4_i, i4_len;

    /*
    DBG_API(("[UC] w2s_rchr:\n"));
    */

    if(w2s_str == (UTF16_T*)NULL)
    {
        return (UTF16_T*)NULL;
    }

    i4_len = (INT32)x_uc_w2s_strlen(w2s_str);
    for(i4_i=i4_len-1; i4_i>=0; i4_i--)
    {
        if(x_uc_w2s_get(w2s_str, (UINT32)i4_i) == w2_ch)
        {
            return x_uc_w2s_get_ptr(w2s_str, (UINT32)i4_i);
        }
    }

    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_rchr_ex
 *
 * Description: Find a character in a string from end of string.
 *
 * Inputs:  w2s_str                 String to be searched in.
 *          w2_ch                   The character to find.
 *
 * Outputs: pui4_pos                Position of the character. Begin from 0.
 *
 * Returns: if found                The pointer of the character.
 *          if not                  NULL
 ----------------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_rchr_ex(
    const UTF16_T*  w2s_str_right,
    UINT32          i4_string_length,
    UTF16_T         w2_ch,
    UINT32*         pui4_pos)
{
    INT32       i4_i;

    /*
    DBG_API(("[UC] w2s_rchr:\n"));
    */

    if(w2s_str_right == (UTF16_T*)NULL ||
        pui4_pos == (UINT32*)NULL )
    {
        return (UTF16_T*)NULL;
    }   
    for(i4_i=0; i4_i < (INT32)i4_string_length; i4_i++)
    {
        if(*(w2s_str_right - i4_i) == w2_ch)
        {
            *pui4_pos = (UINT32)i4_i;
            return (UTF16_T*)(w2s_str_right - i4_i);
        }
    }

    return NULL;
}



/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_toupper
 *
 * Description: Convert the string to Upper case.
 *
 * Inputs:  ui4_dst_len             Destination string buffer length.
 *          w2s_src                 Null-terminated source string.
 *          ps_locale               Specify the locale when convert case.
 *                                  Reserved for future. Only NULL is valid.
 *
 * Outputs: w2s_dst                 Converted string.
 *
 * Returns: UCR_NO_SUPPORT          Specified locale is not supported.
 *          UCR_NO_ENOUGH_BUFFER    Size of w2s_dst is not enough.
 *          UCR_NO_MEMORY           Run out of system memory.
 *          UCR_CNV_FAIL            Fail to convert.
 *          UCR_ERR_ARG             Parameter error.
 *          UCR_OK                  Success
 ----------------------------------------------------------------------------*/
INT32 x_uc_w2s_toupper(
    UTF16_T*        w2s_dst,
    UINT32          ui4_dst_len,
    const UTF16_T*  w2s_src,
    const CHAR*     ps_locale)
{
    /*
    DBG_API(("[UC] w2s_toupper: ui4_dst_len=%d\n", ui4_dst_len));
    */

    if( (w2s_dst == (UTF16_T*)NULL) ||
        (w2s_src == (UTF16_T*)NULL) )
    {
        return UCR_ERR_ARG;
    }

    return uc_w2s_case_cnv(w2s_dst,
                           ui4_dst_len,
                           w2s_src,
                           ps_locale,
                           UC_CASE_UPPER);
}


/*-----------------------------------------------------------------------------
 * Name:  x_uc_w2s_tolower
 *
 * Description: Convert the string to Lower case.
 *
 * Inputs:  ui4_dst_len             Destination string buffer length.
 *          w2s_src                 Null-terminated source string.
 *          ps_locale               Specify the locale when convert case.
 *                                  Reserved for future. Only NULL is valid.
 *
 * Outputs: w2s_dst                 Converted string.
 *
 * Returns: UCR_NO_SUPPORT          Specified locale is not supported.
 *          UCR_NO_ENOUGH_BUFFER    Size of w2s_dst is not enough.
 *          UCR_NO_MEMORY           Run out of system memory.
 *          UCR_CNV_FAIL            Fail to convert.
 *          UCR_OK                  Success
 ----------------------------------------------------------------------------*/
INT32 x_uc_w2s_tolower(
    UTF16_T*        w2s_dst,
    UINT32          ui4_dst_len,
    const UTF16_T*  w2s_src,
    const CHAR*     ps_locale)
{
    /*
    DBG_API(("[UC] w2s_tolower: ui4_dst_len=%d\n", ui4_dst_len));
    */

    if( (w2s_dst == (UTF16_T*)NULL) ||
        (w2s_src == (UTF16_T*)NULL) )
    {
        return UCR_ERR_ARG;
    }

    return uc_w2s_case_cnv(w2s_dst,
                           ui4_dst_len,
                           w2s_src,
                           ps_locale,
                           UC_CASE_LOWER);
}

INT32 x_multi_byte_to_unicode(UINT16* pui2_multi_byte, UINT32 ui4_len, 
                           E_MUL_BYTE_TYPE e_multi_type, UINT16* pui2_unicode)
{
    return multi_byte_to_unicode(pui2_multi_byte, ui4_len, 
                           e_multi_type, pui2_unicode);
}

INT32 x_multi_byte_to_unicode_ex(VOID* pv_multi_byte, UINT32 ui4_src_len, UINT32* pui4_cut_pos, 
                           E_MUL_BYTE_TYPE e_multi_type, UINT16* pui2_unicode, UINT32* pui4_dst_len)
{
    return multi_byte_to_unicode_ex(pv_multi_byte, ui4_src_len, pui4_cut_pos,
                                    e_multi_type, pui2_unicode, pui4_dst_len);
}


