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
/*! @file x_uc_str.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This contains exported 'Unicode String' manipulation functions.
 */
/*----------------------------------------------------------------------------*/
#ifndef _X_UC_STR_H_
#define _X_UC_STR_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_uc_str.h"


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/**
    Convert
 **/
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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
INT32 x_uc_ps_to_w2s(
    const CHAR*     ps_src,
    UTF16_T*        w2s_dst,
    UINT32          ui4_dst_len);


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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
INT32 x_uc_w2s_to_ps(
    const UTF16_T*  w2s_src,
    CHAR*           ps_dst,
    UINT32          ui4_dst_len);


/**
    Count
 **/
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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
SIZE_T x_uc_w2s_strlen(
    const UTF16_T*  w2s_str);



/**
    Compare
 **/
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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
INT32 x_uc_w2s_strcmp(
    const UTF16_T*  w2s_str1,
    const UTF16_T*  w2s_str2);


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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
INT32 x_uc_w2s_strncmp(
    const UTF16_T*  w2s_str1,
    const UTF16_T*  w2s_str2,
    UINT32          ui4_len);



/**
    Cascade
 **/
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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_strcat(
    UTF16_T*        w2s_dst,
    const UTF16_T*  w2s_src);


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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_strncat(
    UTF16_T*        w2s_dst,
    const UTF16_T*  w2s_src,
    UINT32          ui4_len);



/**
    Copy
 **/
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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_strcpy(
    UTF16_T*        w2s_dst,
    const UTF16_T*  w2s_src);


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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_strncpy(
    UTF16_T*        w2s_dst,
    const UTF16_T*  w2s_src,
    UINT32          ui4_len);



/**
    Index
 **/
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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
UTF16_T x_uc_w2s_get(
    const UTF16_T*  w2s_str,
    UINT32          ui4_idx);


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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_get_ptr(
    const UTF16_T*  w2s_str,
    UINT32          ui4_idx);


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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
INT32 x_uc_w2s_set(
    UTF16_T*        w2s_str,
    UINT32          ui4_idx,
    UTF16_T         w2_char);

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
 * Outputs: -.
 *
 * Returns: UCR_OK                  Success.
 *          UCR_ERR_ARG             Parameter error.
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
INT32 x_uc_w2s_reverse(
    UTF16_T*  w2s_str);


/**
    Search
 **/
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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_chr(
    const UTF16_T*  w2s_str,
    UTF16_T         w2_ch);


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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_chr_ex(
    const UTF16_T*  w2s_str,
    UTF16_T         w2_ch,
    UINT32*         pui4_pos);


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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_rchr(
    const UTF16_T*  w2s_str,
    UTF16_T         w2_ch);


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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
UTF16_T* x_uc_w2s_rchr_ex(
    const UTF16_T*  w2s_str_right,
    UINT32          ui4_max_length,
    UTF16_T         w2_ch,
    UINT32*         pui4_pos);


/**
    Case
 **/
/* Because upper/lower case is 'Locale-Dependent', clients should provide the
   locale parameter.
   Set locale to NULL will use the 'default locale'.
   Note! This parameter is ignored in current implementation */

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
 *          UCR_OK                  Success.
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
INT32 x_uc_w2s_toupper(
    UTF16_T*        w2s_dst,
    UINT32          ui4_dst_len,
    const UTF16_T*  w2s_src,
    const CHAR*     ps_locale);


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
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
INT32 x_uc_w2s_tolower(
    UTF16_T*        w2s_dst,
    UINT32          ui4_dst_len,
    const UTF16_T*  w2s_src,
    const CHAR*     ps_locale);


INT32 x_multi_byte_to_unicode(UINT16* pui2_multi_byte, UINT32 ui4_len, 
                           E_MUL_BYTE_TYPE e_multi_type, UINT16* pui2_unicode);

INT32 x_multi_byte_to_unicode_ex(VOID* pv_multi_byte, UINT32 ui4_src_len, UINT32* pui4_cut_pos, 
                           E_MUL_BYTE_TYPE e_multi_type, UINT16* pui2_unicode, UINT32* pui4_dst_len);


#endif  /* _X_UC_STR_H */
