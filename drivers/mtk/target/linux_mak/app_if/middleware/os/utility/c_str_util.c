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
 * $RCSfile: c_str_util.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains implementation of application level OS string
 *         utility functions.
 *      
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "aee/aee.h"
#include "aee/aee_mem.h"
#include "os/inc/os.h"
#include "os/utility/xsprintf.h"


/*--------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------
 * Function: c_strdup()
 *
 * Description:
 *      this API returns a pointer to a memory block that duplicates ps_str.
 *      1. parse argument and return NULL if it is a NULL string.
 *      2. get string ps_str length plus 1.
 *      3. allocate memory block of such size.
 *      4. memory copy from ps_str to the allocated memory.
 *
 * Inputs:
 *      ps_s : null ('\0') terminated character string.
 *
 * Outputs: None.
 *
 * Returns:
 *      pointer to copied string.
 *-------------------------------------------------------------------------*/
#ifdef SYS_MEM_CHK
VOID* c_strdup (const CHAR*  ps_str, const CHAR* s_file, UINT32 ui4_line)
{
    CHAR*   ps_copy;
    SIZE_T  z_len;
    
    z_len = x_strlen(ps_str);

#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
    ps_copy = aee_mem_dbg_alloc_for_aee(z_len + 1, s_file, ui4_line);
#else
    ps_copy = x_mem_dbg_alloc(z_len + 1, s_file, ui4_line);
#endif

    if (ps_copy != NULL)
    {
        x_memcpy(ps_copy, ps_str, z_len);
        
        ps_copy[z_len] = '\0';
    }
    
    return ps_copy;
}
#else
CHAR* c_strdup
(
    const CHAR*  ps_str
)
{
    CHAR*   ps_copy;
    SIZE_T  z_len;
    
    z_len = x_strlen(ps_str);

#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
    ps_copy = aee_mem_alloc_for_aee(z_len + 1);
#else
    ps_copy = x_mem_alloc(z_len + 1);
#endif

    if (ps_copy != NULL)
    {
        x_memcpy(ps_copy, ps_str, z_len);
        
        ps_copy[z_len] = '\0';
    }
    
    return ps_copy;
}
#endif

/*--------------------------------------------------------------------------
 * Function: c_strcpy()
 *
 * Description:
 *      this API copies ps-from to ps_to. it performs
 *      1. parse argument and return NULL if either is NULL.
 *      2. copy character by character from ps_from to ps_to until
 *         '\0' is encountered.
 *
 * Inputs:
 *      ps_to : memory block to be copied to.
 *      ps_from : null ('\0') terminated character string.
 *
 * Outputs: None.
 *
 * Returns:
 *      pointer to ps_to
 *-------------------------------------------------------------------------*/
CHAR* c_strcpy
(
    CHAR*        ps_to,
    const CHAR*  ps_from
)
{
    return x_strcpy(ps_to, ps_from);
}

/*--------------------------------------------------------------------------
 * Function: c_strncpy()
 *
 * Description:
 *      this API copies ps-from to ps_to up to z_l bytes. it performs
 *      1. parse argument and return ps_to on error.
 *      2. process to make the most UINT32 copies, then UINT16 copies, then
 *         UINT8 copies. 
 *
 * Inputs:
 *      ps_to : memory block to be copied to.
 *      ps_from : null ('\0') terminated character string.
 *      z_l : bytes to be copied
 *
 * Outputs: None.
 *
 * Returns:
 *      pointer to ps_to
 *-------------------------------------------------------------------------*/
CHAR* c_strncpy
(
    CHAR*        ps_to,
    const CHAR*  ps_from,
    SIZE_T       z_len
)
{
    return x_strncpy(ps_to, ps_from, z_len);
}

/*--------------------------------------------------------------------------
 * Function: c_strcmp()
 *
 * Description:
 *      this API compares 2 strings ps_str1 and ps_str2. it performs
 *      1. if both are NULL, return 0.
 *      2. if ps_s1 is NULL and ps_s2 is not NULL, return -1.
 *      3. if ps_s1 is not NULL and ps_s2 is NULL, return 1.
 *      4. compute the difference between ps_str1 and ps_str2 side by side.
 *      5. if difference is less than zero, return -1; if difference is
 *         positive, return 1; if diffenece is 0, move both pointers, repeat
 *         steps 4, 5.
 *      6. stop when either one moves to end of string.
 *
 * Inputs:
 *      ps_str1 : null ('\0') terminated character string.
 *      ps_str2 : null ('\0') terminated character string.
 *
 * Outputs: None.
 *
 * Returns:
 *      0 : ps_str1 is identical to ps_str2.
 *      1 : ps_str1 is greater than ps_str2.
 *      -1 : ps_str1 is less than ps_str2.
 *-------------------------------------------------------------------------*/
INT32 c_strcmp
(
    const CHAR*  ps_str1,
    const CHAR*  ps_str2
)
{
    return x_strcmp(ps_str1, ps_str2);
}

/*-------------------------------------------------------------------------
 * Function: c_strncmp()
 *
 * Description:
 *      this API compares 2 strings ps_str1 and ps_str2 up to z_len bytes.
 *      it performs
 *      1. if both are NULL, return 0.
 *      2. if ps_str1 is NULL and ps_str2 is not NULL, return -1.
 *      3. if ps_str1 is not NULL and ps_str2 is NULL, return 1.
 *      4. compute the difference between ps_str1 and ps_str2 side by side.
 *      5. if difference is less than zero, return -1; if difference is
 *         positive, return 1; if diffenece is 0, move both pointers, repeat
 *         steps 4, 5.
 *      6. stop when either one moves to end of string or z_len bytes have met.
 *
 * Inputs:
 *      ps_str1 : null ('\0') terminated character string.
 *      ps_str2 : null ('\0') terminated character string.
 *      z_len : byte count to compare.
 *
 * Outputs: None
 *
 * Returns:
 *      0 : ps_str1 is identical to ps_str2.
 *      1 : ps_str1 is greater than ps_str2.
 *      -1 : ps_str1 is less than ps_str2.
 *------------------------------------------------------------------------*/
INT32 c_strncmp
(
    const CHAR*  ps_str1,
    const CHAR*  ps_str2,
    SIZE_T       z_len
)
{
    return x_strncmp(ps_str1, ps_str2, z_len);
}

/*----------------------------------------------------------------------------
 * Function: c_strcasecmp()
 *
 * Description:
 *      Similar to c_strcmp, but ignoring the case of the characters.
 *
 * Inputs:
 *      ps_s1 : null ('\0') terminated character string.
 *      ps_s2 : null ('\0') terminated character string.
 *
 * Outputs: -
 *
 * Returns:
 *      0 : ps_s1 is identical to ps_s2.
 *      1 : ps_s1 is greater than ps_s2.
 *      -1 : ps_s1 is less than ps_s2.
 *---------------------------------------------------------------------------*/
INT32 c_strcasecmp (const CHAR*  ps_s1,
                    const CHAR*  ps_s2)
{
    return x_strcasecmp(ps_s1, ps_s2);
}

/*----------------------------------------------------------------------------
 * Function: c_strncasecmp()
 *
 * Description:
 *      Similar to c_strncmp, but ignoring the case of the characters.
 *
 * Inputs:
 *      ps_s1 : null ('\0') terminated character string.
 *      ps_s2 : null ('\0') terminated character string.
 *
 * Outputs: -
 *
 * Returns:
 *      0 : ps_s1 is identical to ps_s2.
 *      1 : ps_s1 is greater than ps_s2.
 *      -1 : ps_s1 is less than ps_s2.
 *---------------------------------------------------------------------------*/
INT32 c_strncasecmp (const CHAR*  ps_s1,
                     const CHAR*  ps_s2,
                     SIZE_T       z_l)
{
    return x_strncasecmp(ps_s1, ps_s2, z_l);
}

/*--------------------------------------------------------------------------
 * Function: c_strcat()
 *
 * Description:
 *      this API concatenates ps_append to ps_to. it performs
 *      1. parse argument and return ps_to if either is NULL.
 *      2. move temp pointer to the end of ps_to.
 *      3. copy character by character from ps_append to temp pointer until
 *         '\0' is encountered.
 *
 * Inputs:
 *      ps_to : null ('\0') terminated character string.
 *      ps_append : null ('\0') terminated character string.
 *
 * Outputs: None.
 *
 * Returns:
 *      pointer to ps_to
 *-------------------------------------------------------------------------*/
CHAR* c_strcat
(
    CHAR*        ps_to,
    const CHAR*  ps_append
)
{
    return x_strcat(ps_to, ps_append);
}

/*-------------------------------------------------------------------------
 * Function: c_strncat()
 *
 * Description:
 *      this API concatenates ps_append to ps_to up to z_len bytes. it performs
 *      1. parse argument and return ps_to if either is NULL.
 *      2. move temp pointer to the end of ps_to.
 *      3. call x_strncpy() to copy z_len bytes.
 *
 * Inputs:
 *      ps_to : null ('\0') terminated character string.
 *      ps_append : null ('\0') terminated character string.
 *      z_l : bytes to be concaatenated from ps_append.
 *
 * Outputs: None.
 *
 * Returns:
 *      pointer to ps_to
 *------------------------------------------------------------------------*/
CHAR* c_strncat
(
    CHAR*        ps_to,
    const CHAR*  ps_append,
    SIZE_T       z_len
)
{
    return x_strncat(ps_to, ps_append, z_len);
}

/*-------------------------------------------------------------------------
 * Function: c_strchr()
 *
 * Description:
 *      this API locates the first occurrence of c_c in ps_s. it performs
 *      1. parse argument and return NULL if it is a NULL string.
 *      2. move along ps_s until c_c is found and return.
 *      3. if not found, return NULL.
 *
 * Inputs:
 *      ps_s : null ('\0') terminated character string.
 *      c_c : character to locate.
 *
 * Outputs: None.
 *
 * Returns:
 *      pointer to the first occurrence of c_c in ps_s.
 *-------------------------------------------------------------------------*/
CHAR* c_strchr
(
    const CHAR*  ps_str,
    CHAR         c_char
)
{
    return x_strchr(ps_str, c_char);
}

/*-------------------------------------------------------------------------
 * Function: c_strrchr()
 *
 * Description:
 *      This API locates the last occurrence of a character in a string. It
 *      returns NULL if the character does not exist in the string.
 *
 * Inputs:
 *      ps_str  : null-terminated character string.
 *      c_cchar : character to locate.
 *
 * Outputs: None.
 *
 * Returns:
 *      pointer to the last occurrence of c_char in ps_str, or NULL.
 *------------------------------------------------------------------------*/
CHAR* c_strrchr
(
    const CHAR*  ps_str,
    CHAR         c_char
)
{
    return x_strrchr(ps_str, c_char);
}

/*-------------------------------------------------------------------------
 * Function: c_strstr()
 *
 * Description:
 *      this API locates the first occurrence of ps_find in ps_s. it performs
 *      1. parse arguments and return NULL if either is NULL string.
 *      2. if ps_find contains '\0', returns ps_s.
 *      3. move both ps_s and ps_find as long as they are identical, otherwise,
 *         strat over ps_find and continue to compare.
 *      4. if not found, return NULL.
 *
 * Inputs:
 *      ps_s : null ('\0') terminated character string.
 *      ps_find : null terminated character string to locate.
 *
 * Outputs: None.
 *
 * Returns:
 *      pointer to the first occurrence of ps_find in ps_s.
 *------------------------------------------------------------------------*/
CHAR* c_strstr
(
    const CHAR*  ps_str,
    const CHAR*  ps_find
)
{
    return x_strstr(ps_str, ps_find);
}

/*--------------------------------------------------------------------------
 * Function: c_strtoull()
 *
 * Description:
 *     This API converts the initial part of the string in 'pc_beg_ptr' to an
 *     unsigned 64-bit integer value according to the given base. This API is
 *     the equivalent of the ANSI C strtoull API.
 *
 * Inputs:
 *      pc_beg_ptr : Null-terminated character string.
 *      ui1_base :   The base of numeric conversion, which must be between 2
 *                   and 36 inclusive, or be the special value 0.
 *
 * Outputs:
 *      ppc_end_ptr : If ppc_end_ptr is not NULL, *ppc_end_ptr contains the
 *                    address of the first invalid character.
 *
 * Returns:
 *      Unsigned INT64 value
 *-------------------------------------------------------------------------*/
UINT64 c_strtoull
(
    const CHAR*  pc_beg_ptr,
    CHAR**       ppc_end_ptr,
    UINT8        ui1_base
)
{
    return x_strtoull(pc_beg_ptr, ppc_end_ptr, ui1_base);
}

/*-------------------------------------------------------------------------
 * Function: c_strtoll()
 *
 * Description:
 *     This API converts the initial part of the string in 'pc_beg_ptr' to a
 *     signed 64-bit integer value according to the given base. This API is
 *     the equivalent of the ANSI C strtoll API.
 *
 * Inputs:
 *      pc_beg_ptr : Null-terminated character string.
 *      ui1_base :   The base of numeric conversion, which must be between 2
 *                   and 36 inclusive, or be the special value 0.
 *
 * Outputs:
 *      ppc_end_ptr : If ppc_end_ptr is not NULL, *ppc_end_ptr contains the
 *                    address of the first invalid character.
 *
 * Returns:
 *      Signed INT64 value
 *------------------------------------------------------------------------*/
INT64 c_strtoll
(
    const CHAR*  pc_beg_ptr,
    CHAR**       ppc_end_ptr,
    UINT8        ui1_base
)
{
    return x_strtoll(pc_beg_ptr, ppc_end_ptr, ui1_base);
}

/*----------------------------------------------------------------------------
 * Function: c_strlen()
 *
 * Description:
 *      this API returns length of a character string. it performs
 *      1. parse argument and return 0 if it is a NULL string.
 *      2. move temp pointer to null character.
 *      3. return distance between temp pointer and original pointer.
 *
 * Inputs:
 *      ps_s : null ('\0') terminated character string.
 *
 * Outputs: None
 *
 * Returns:
 *      length of character string excluding null ('\0') character.
 *---------------------------------------------------------------------------*/
SIZE_T c_strlen
(
    const CHAR*  ps_str
)
{
    return x_strlen(ps_str);
}

/*--------------------------------------------------------------------------
 * Function: c_sprintf()
 *
 * Description:
 *      this API is duplicate Middleware's version of x_sprintf().
 *
 * Inputs:
 *      ps_buf : output buffer.
 *      ps_fmt : format string.
 *      ... : variable arguments
 *
 * Outputs: None.
 *
 * Returns:
 *      length of ps_buf.
 *-------------------------------------------------------------------------*/
INT32 c_sprintf
(
    CHAR*        ps_str,
    const CHAR*  ps_fmt,
    ...
)
{
    VA_LIST t_ap;
    INT32   i4_len;

    if ((ps_str == (CHAR *) NULL) || (ps_fmt == (CHAR *) NULL) ||
        (ps_fmt[0] == '\0'))
    {
        return(0);
    }
        
    VA_START(t_ap, ps_fmt);

    i4_len = x_vsprintf(ps_str, ps_fmt, t_ap);

    VA_END(t_ap);
        
    return(i4_len);
}

/*--------------------------------------------------------------------------
 * Function: c_snprintf()
 *
 * Description:
 *      this API is duplicate Middleware's version of x_snprintf().
 *
 * Inputs:
 *      ps_str : output buffer.
 *      z_size : size of the output buffer.
 *      ps_fmt : format string.
 *      ... : variable arguments
 *
 * Outputs: None.
 *
 * Returns:
 *      length of ps_buf.
 *-------------------------------------------------------------------------*/
INT32 c_snprintf
(
    CHAR*        ps_str,
    SIZE_T       z_size,
    const CHAR*  ps_fmt,
    ...
)
{
    VA_LIST t_ap;
    INT32   i4_len;
    
    if ((ps_str == (CHAR *) NULL) || (ps_fmt == (CHAR *) NULL) ||
        (ps_fmt[0] == '\0'))
    {
        return(0);
    }
    
    VA_START(t_ap, ps_fmt);
    
    i4_len = x_vsnprintf(ps_str, z_size, ps_fmt, t_ap);
    
    VA_END(t_ap);
    
    return(i4_len);
}

/*-------------------------------------------------------------------------
 * Function: c_vsprintf()
 *
 * Description:
 *      this API implement the vsprintf() functionality.
 *
 * Inputs:
 *      ps_str    : output buffer.
 *      ps_fmt    : format string.
 *      va_list   : variable arguments
 *
 * Outputs: None.
 *
 * Returns:
 *      length of ps_str.
 *------------------------------------------------------------------------*/
INT32 c_vsprintf
(
    CHAR*        ps_str,
    const CHAR*  ps_fmt,
    VA_LIST      va_list
)
{
    return x_vsprintf(ps_str, ps_fmt, va_list);
}


/*-------------------------------------------------------------------------
 * Function: c_vsnprintf()
 *
 * Description:
 *      this API implement the vsnprintf() functionality.
 *
 * Inputs:
 *      ps_str    : output buffer.
 *      z_size    : size of the output buffer.
 *      ps_fmt    : format string.
 *      va_list   : variable arguments
 *
 * Outputs: None.
 *
 * Returns:
 *      length of ps_str.
 *------------------------------------------------------------------------*/
INT32 c_vsnprintf
(
    CHAR*        ps_str,
    SIZE_T       z_size,
    const CHAR*  ps_fmt,
    VA_LIST      va_list
)
{
    return x_vsnprintf(ps_str, z_size, ps_fmt, va_list);
}

/*----------------------------------------------------------------------------
 * Function: c_strspn
 *
 * Description:
 *     This API calculates the length of the initial segment of a string which
 *     consists entirely of characters in another string.
 *
 * Inputs:
 *      ps_str     Null-terminated character string.
 *      ps_accept  Character string containing the accepted character set.
 *
 * Outputs:  -
 *
 * Returns:  Length of the segment. If either ps_str or ps_accept is NULL, the
 *           API returns 0.
 *---------------------------------------------------------------------------*/
SIZE_T c_strspn (const CHAR*  ps_str,
                 const CHAR*  ps_accept)
{
    return x_strspn(ps_str, ps_accept);
}

/*----------------------------------------------------------------------------
 * Function: c_strcspn
 *
 * Description:
 *     This API calculates the length of the initial segment of a string which
 *     consists entirely of characters not in another string.
 *
 * Inputs:
 *      ps_str     Null-terminated character string.
 *      ps_reject  Character string containing the rejected character set.
 *
 * Outputs:  -
 *
 * Returns:  Length of the segment. If either ps_str or ps_accept is NULL, the
 *           API returns 0.
 *---------------------------------------------------------------------------*/
SIZE_T c_strcspn (const CHAR*  ps_str,
                  const CHAR*  ps_reject)
{
    return x_strcspn(ps_str, ps_reject);
}

/*----------------------------------------------------------------------------
 * Function: c_str_toupper
 *
 * Description:
 *     This API converts a string to uppercase.
 *
 * Inputs:
 *      ps_str     Null-terminated character string.
 *
 * Outputs:  -
 *
 * Returns:  the pointer passed in ps_str.
 *---------------------------------------------------------------------------*/
CHAR* c_str_toupper (CHAR*  ps_str)
{
    return x_str_toupper(ps_str);
}

/*----------------------------------------------------------------------------
 * Function: c_str_tolower
 *
 * Description:
 *     This API converts a string to lowercase.
 *
 * Inputs:
 *      ps_str     Null-terminated character string.
 *
 * Outputs:  -
 *
 * Returns:  the pointer passed in ps_str.
 *---------------------------------------------------------------------------*/
CHAR* c_str_tolower (CHAR*  ps_str)
{
    return x_str_tolower(ps_str);
}

/*----------------------------------------------------------------------------
 * Function: c_strtok
 *
 * Description:
 *      This API returns a pointer to the next 'token' in ps_str. Tokens are
 *      separated by delimiters (specified by ps_delimiter). This API returns
 *      NULL if no token is found (including if the string is empty).
 *
 * Inputs:
 *      ps_str        Null-terminated character string.
 *      ps_delimiter  Null-terminated list of delimiters.
 *
 * Outputs:  pps_str       Will point to the next string to parse.
 *           pz_token_len  Will contain the length of the found token. If
 *                         pz_token_len is NULL then the passed string is
 *                         modified so it will contain a NULL character at the
 *                         end of the token.
 *
 * Returns:  pointer to the first token in ps_str.
 *---------------------------------------------------------------------------*/
CHAR* c_strtok (CHAR*        ps_str,
                const CHAR*  ps_delimiters,
                CHAR**       pps_str,
                SIZE_T*      pz_token_len)
{
    return x_strtok(ps_str, ps_delimiters, pps_str, pz_token_len);
}
