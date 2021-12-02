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
 * $RCSfile: xsprintf.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "os/inc/os.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

typedef enum
{
    SPRINTF_NONE,
    SPRINTF_CHAR,
    SPRINTF_SHORT,
    SPRINTF_LONG,
    SPRINTF_LONG_LONG,
    SPRINTF_LONG_DOUBLE
}   SPRINTF_LENGTH_MODIFIER_T;

#define XV_ZEROPAD     MAKE_BIT_MASK_32(0)     /* pad with zero */
#define XV_SIGN        MAKE_BIT_MASK_32(1)     /* unsigned/signed long */
#define XV_PLUS        MAKE_BIT_MASK_32(2)     /* show plus */
#define XV_SPACE       MAKE_BIT_MASK_32(3)     /* space if plus */
#define XV_LEFT        MAKE_BIT_MASK_32(4)     /* left justified */
#define XV_SPECIAL     MAKE_BIT_MASK_32(5)     /* 0x */
#define XV_LARGE       MAKE_BIT_MASK_32(6)     /* e upper case */

#define CVTBUFSIZE     64

#define CHAR_IS_DIGIT(_c) ((_c) >= '0' && (_c) <= '9')


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

static const CHAR  s_lowercase[] = "0123456789abcdef";
static const CHAR  s_uppercase[] = "0123456789ABCDEF";
static const CHAR  s_null_str[]  = "(null)";
static const CHAR  s_null_ptr[]  = "(nil)";


static VOID xvcdecpt(CHAR *buffer);
static VOID xvczeros(CHAR *buffer);
static long double xvmodf(long double d_d, long double *pd_d);
static VOID xvcfloat(long double value,
                     CHAR *buffer,
                     CHAR fmt,
                     INT32 precision);

static CHAR * xvfloat(CHAR*        str,
                      long double  num,
                      SIZE_T*      pz_printed,
                      SIZE_T       z_max,
                      UINT32       ui4_flags,
                      INT32        size,
                      INT32        precision,
                      CHAR         fmt,
                      BOOL         b_snprintf);

static CHAR * xvconvert(long double arg,
                        INT32 ndigits,
                        INT32 *decpt,
                        INT32 *sign,
                        CHAR *buf,
                        BOOL eflag);


/*-----------------------------------------------------------------------------
 * Name: sprintf_output_char
 *
 * Description: This API copies a single character into a buffer. If b_snprintf
 *              is TRUE then the character is copied only if z_max is greater
 *              than the number of characters printed so far.
 *
 * Inputs:  ps_buf      References the output buffer.
 *          pz_printed  References the number of characters printed so far.
 *          z_max       Contains the maximum number of characters to copy.
 *          c_char      Contains the character to copy.
 *          b_snprintf  Specifies whether z_max is relevant.
 *
 * Outputs: pz_printed  Contains the number of characters that have been copied
 *                      so far, or the number of characters that would have
 *                      been copied had the output buffer been big enough.
 *
 * Returns: The new position in the output buffer.
 ----------------------------------------------------------------------------*/
static CHAR* sprintf_output_char (CHAR*    ps_buf,
                                  SIZE_T*  pz_printed,
                                  SIZE_T   z_max,
                                  CHAR     c_char,
                                  BOOL     b_snprintf)
{
    if (!b_snprintf || (*pz_printed < z_max))
    {
        *ps_buf++ = c_char;
    }
    
    (*pz_printed)++;
    
    return ps_buf;
}

/*-----------------------------------------------------------------------------
 * Name: sprintf_output_n_chars
 *
 * Description: This API copies n characters into a buffer.
 *
 * Inputs:  ps_buf      References the output buffer.
 *          pz_printed  References the number of characters printed so far.
 *          z_max       Contains the maximum number of characters to copy.
 *          i4_n        Contains the number of times the character should be
 *                      copied.
 *          c_char      Contains the character to copy.
 *          b_snprintf  Specifies whether z_max is relevant.
 *
 * Outputs: pz_printed  Contains the number of characters that have been copied
 *                      so far, or the number of characters that would have
 *                      been copied had the output buffer been big enough.
 *
 * Returns: The new position in the output buffer.
 ----------------------------------------------------------------------------*/
static CHAR* sprintf_output_n_chars (CHAR*    ps_buf,
                                     SIZE_T*  pz_printed,
                                     SIZE_T   z_max,
                                     INT32    i4_n,
                                     CHAR     c_char,
                                     BOOL     b_snprintf)
{
    INT32  i4_i;
    
    for (i4_i = 0; i4_i < i4_n; i4_i++)
    {
        ps_buf = sprintf_output_char(ps_buf,
                                     pz_printed,
                                     z_max,
                                     c_char,
                                     b_snprintf);
    }
    
    return ps_buf;
}

/*-----------------------------------------------------------------------------
 * Name: sprintf_output_string
 *
 * Description: This API copies a character string into a buffer.
 *
 * Inputs:  ps_buf      References the output buffer.
 *          pz_printed  References the number of characters printed so far.
 *          z_max       Contains the maximum number of characters to copy.
 *          ps_str      References a character string.
 *          z_len       Contains the string length.
 *          b_snprintf  Specifies whether z_max is relevant.
 *
 * Outputs: pz_printed  Contains the number of characters that have been copied
 *                      so far, or the number of characters that would have
 *                      been copied had the output buffer been big enough.
 *
 * Returns: The new position in the output buffer.
 ----------------------------------------------------------------------------*/
static CHAR* sprintf_output_string (CHAR*        ps_buf,
                                    SIZE_T*      pz_printed,
                                    SIZE_T       z_max,
                                    const CHAR*  ps_str,
                                    SIZE_T       z_len,
                                    BOOL         b_snprintf)
{
    CHAR  c_char;
    
    while (((c_char = *ps_str++) != '\0')
           &&
           (z_len-- != 0))
    {
        ps_buf = sprintf_output_char(ps_buf,
                                     pz_printed,
                                     z_max,
                                     c_char,
                                     b_snprintf);
    }
    
    return ps_buf;
}

/*-----------------------------------------------------------------------------
 * Name: sprintf_output_c
 *
 * Description: This API processes the %c format. In addition to the character
 *              some spaces may be copied as well (for alignment purpose).
 *
 * Inputs:  ps_buf          References the output buffer.
 *          c_char          Contains the character to output.
 *          pz_printed      References the number of characters printed so far.
 *          z_max           Contains the maximum number of characters to copy.
 *          ui4_flags       Contains the printf flags.
 *          i4_field_width  Contains the minimum field width.
 *          b_snprintf      Specifies whether z_max is relevant.
 *
 * Outputs: pz_printed  Contains the number of characters that have been copied
 *                      so far, or the number of characters that would have
 *                      been copied had the output buffer been big enough.
 *
 * Returns: The new position in the output buffer.
 ----------------------------------------------------------------------------*/
static CHAR* sprintf_output_c (CHAR*    ps_buf,
                               CHAR     c_char,
                               SIZE_T*  pz_printed,
                               SIZE_T   z_max,
                               UINT32   ui4_flags,
                               INT32    i4_field_width,
                               BOOL     b_snprintf)
{
    if ((ui4_flags & XV_LEFT) != 0)
    {
        ps_buf = sprintf_output_char(ps_buf,
                                     pz_printed, z_max,
                                     c_char,
                                     b_snprintf);
    }
    
    /* padding with spaces */
    ps_buf = sprintf_output_n_chars(ps_buf,
                                    pz_printed, z_max,
                                    i4_field_width - 1,
                                    ' ',
                                    b_snprintf);
    
    if ((ui4_flags & XV_LEFT) == 0)
    {
        ps_buf = sprintf_output_char(ps_buf,
                                     pz_printed, z_max,
                                     c_char,
                                     b_snprintf);
    }
    
    return ps_buf;
}

/*-----------------------------------------------------------------------------
 * Name: sprintf_output_s
 *
 * Description: This API processes the %s format. In addition to the string
 *              some spaces may be copied as well (for alignment purpose).
 *
 * Inputs:  ps_buf          References the output buffer.
 *          ps_str          Contains the string to output.
 *          pz_printed      References the number of characters printed so far.
 *          z_max           Contains the maximum number of characters to copy.
 *          ui4_flags       Contains the printf flags.
 *          i4_field_width  Contains the minimum field width.
 *          i4_precision    Contains the precision.
 *          b_snprintf      Specifies whether z_max is relevant.
 *
 * Outputs: pz_printed  Contains the number of characters that have been copied
 *                      so far, or the number of characters that would have
 *                      been copied had the output buffer been big enough.
 *
 * Returns: The new position in the output buffer.
 ----------------------------------------------------------------------------*/
static CHAR* sprintf_output_s (CHAR*        ps_buf,
                               const CHAR*  ps_str,
                               SIZE_T*      pz_printed,
                               SIZE_T       z_max,
                               UINT32       ui4_flags,
                               INT32        i4_field_width,
                               INT32        i4_precision,
                               BOOL         b_snprintf)
{
    INT32  i4_len;
    
    i4_len = (INT32) x_strlen(ps_str);
    
    if ((i4_precision >= 0) && (i4_precision < i4_len))
    {
        /* precision = the maximum number of characters to be printed */
        
        i4_len = i4_precision;
    }
    
    if ((ui4_flags & XV_LEFT) != 0)
    {
        ps_buf = sprintf_output_string(ps_buf,
                                       pz_printed, z_max,
                                       ps_str,
                                       (SIZE_T) i4_precision,
                                       b_snprintf);
    }
    
    /* padding with spaces */
    ps_buf = sprintf_output_n_chars(ps_buf,
                                    pz_printed, z_max,
                                    i4_field_width - i4_len,
                                    ' ',
                                    b_snprintf);
    
    if ((ui4_flags & XV_LEFT) == 0)
    {
        ps_buf = sprintf_output_string(ps_buf,
                                       pz_printed, z_max,
                                       ps_str,
                                       (SIZE_T) i4_precision,
                                       b_snprintf);
    }
    
    return ps_buf;
}

/*-----------------------------------------------------------------------------
 * Name: sprintf_output_unsigned
 *
 * Description: This API outputs an unsigned integer. The alternate form (0x...
 *              or 0...) is taken care of in this function as well as the
 *              alignment (with spaces) and padding (with zeros).
 *
 * Inputs:  ps_buf          References the output buffer.
 *          ui8_num         Contains the unsigned integer to output.
 *          ui4_base        Contains the base (8, 10 or 16).
 *          pz_printed      References the number of characters printed so far.
 *          z_max           Contains the maximum number of characters to copy.
 *          ui4_flags       Contains the printf flags.
 *          i4_field_width  Contains the minimum field width.
 *          i4_precision    Contains the precision.
 *          b_snprintf      Specifies whether z_max is relevant.
 *
 * Outputs: pz_printed  Contains the number of characters that have been copied
 *                      so far, or the number of characters that would have
 *                      been copied had the output buffer been big enough.
 *
 * Returns: The new position in the output buffer.
 ----------------------------------------------------------------------------*/
static CHAR*
sprintf_output_unsigned (CHAR*    ps_buf,
                         UINT64   ui8_num,
                         UINT32   ui4_base,
                         SIZE_T*  pz_printed,
                         SIZE_T   z_max,
                         UINT32   ui4_flags,
                         INT32    i4_field_width,
                         INT32    i4_precision,
                         BOOL     b_snprintf)
{
    UINT64       ui8_tmp;
    const CHAR*  ps_digits;
    const CHAR*  ps_prefix;
    INT32        i4_len;
    INT32        i4_alignment_len;
    INT32        i4_i;
    
    ui8_tmp   = ui8_num;
    ps_digits = (ui4_flags & XV_LARGE) ? s_uppercase : s_lowercase;
    ps_prefix = NULL;
    i4_len    = 0;
    
    if ((ui8_num != 0) || (i4_precision != 0))
    {
        /* length (in digits) of the number */
        do
        {
            i4_len++;
            ui8_tmp /= ui4_base;
        }
        while (ui8_tmp != 0);
    }
    
    /*
      if the actual length is greater than the precision the we use the
      length as the precision
    */
    if (i4_precision < i4_len)
    {
        i4_precision = i4_len;
    }
    
    /* alignment length (may be negative, i.e. no alignment) */
    i4_alignment_len = (i4_field_width - i4_precision);
    
    if (ui4_flags & XV_SPECIAL)
    {
        if (ui4_base == 16)
        {
            if (ui8_num != 0)
            {
                ps_prefix = (ui4_flags & XV_LARGE) ? "0X" : "0x";
                i4_alignment_len -= 2;
            }
        }
        else if (ui4_base == 8)
        {
            if ((i4_precision <= i4_len)
                &&
                ((ui8_num != 0) || (i4_precision == 0)))
            {
                ps_prefix = "0";
                i4_alignment_len--;
            }
        }
    }
    
    if ((ui4_flags & XV_ZEROPAD)
        &&
        (ps_prefix != NULL))
    {
        /* if alignment char is '0' then prefix must be written first */
        
        ps_buf = sprintf_output_string(ps_buf,
                                       pz_printed, z_max,
                                       ps_prefix, (SIZE_T) -1,
                                       b_snprintf);
        
        ps_prefix = NULL;
    }
    
    if ((ui4_flags & XV_LEFT) == 0) /* right alignment */
    {
        CHAR  c_alignment_char;
        
        c_alignment_char = (ui4_flags & XV_ZEROPAD) ? '0' : ' ';
        
        ps_buf = sprintf_output_n_chars(ps_buf,
                                        pz_printed, z_max,
                                        i4_alignment_len,
                                        c_alignment_char,
                                        b_snprintf);
    }
    
    /* prefix (0x or 0) */
    if (ps_prefix != NULL)
    {
        ps_buf = sprintf_output_string(ps_buf,
                                       pz_printed, z_max,
                                       ps_prefix, (SIZE_T) -1,
                                       b_snprintf);
    }
    
    /* padding zeros (if any) */
    ps_buf = sprintf_output_n_chars(ps_buf,
                                    pz_printed, z_max,
                                    i4_precision - i4_len,
                                    '0',
                                    b_snprintf);
    
    /* actual number */
    for (i4_i = 0, ui8_tmp = 1; i4_i < i4_len - 1; i4_i++)
    {
        ui8_tmp *= ui4_base;
    }
    
    for (i4_i = 0; i4_i < i4_len; i4_i++)
    {
        UINT32  ui4_digit;
        
        ui4_digit = (UINT32) (ui8_num / ui8_tmp);
        
        ui8_num -= (ui8_tmp * ui4_digit);
        
        ui8_tmp /= ui4_base;
        
        ps_buf = sprintf_output_char(ps_buf,
                                     pz_printed, z_max,
                                     ps_digits[ui4_digit],
                                     b_snprintf);
    }
    
    if ((ui4_flags & XV_LEFT) != 0) /* left alignment */
    {
        ps_buf = sprintf_output_n_chars(ps_buf,
                                        pz_printed, z_max,
                                        i4_alignment_len,
                                        ' ',
                                        b_snprintf);
    }
    
    return ps_buf;
}

/*-----------------------------------------------------------------------------
 * Name: sprintf_output_signed
 *
 * Description: This API outputs a signed integer. The alignment (with spaces)
 *              and padding (with zeros) are taken care of in this function.
 *
 * Inputs:  ps_buf          References the output buffer.
 *          i8_num          Contains the signed integer to output.
 *          pz_printed      References the number of characters printed so far.
 *          z_max           Contains the maximum number of characters to copy.
 *          ui4_flags       Contains the printf flags.
 *          i4_field_width  Contains the minimum field width.
 *          i4_precision    Contains the precision.
 *          b_snprintf      Specifies whether z_max is relevant.
 *
 * Outputs: pz_printed  Contains the number of characters that have been copied
 *                      so far, or the number of characters that would have
 *                      been copied had the output buffer been big enough.
 *
 * Returns: The new position in the output buffer.
 ----------------------------------------------------------------------------*/
static CHAR*
sprintf_output_signed (CHAR*    ps_buf,
                       INT64    i8_num,
                       SIZE_T*  pz_printed,
                       SIZE_T   z_max,
                       UINT32   ui4_flags,
                       INT32    i4_field_width,
                       INT32    i4_precision,
                       BOOL     b_snprintf)
{
    INT64        i8_tmp;
    const CHAR*  ps_digits;
    INT32        i4_len;
    INT32        i4_alignment_len;
    INT32        i4_i;
    CHAR         c_sign;
    
    i8_tmp    = i8_num;
    ps_digits = s_lowercase;
    i4_len    = 0;
    c_sign    = '\0';
    
    if ((i8_num != 0) || (i4_precision != 0))
    {
        /* length (in digits) of the number, not including the sign */
        do
        {
            i4_len++;
            i8_tmp /= 10;
        }
        while (i8_tmp != 0);
    }
    
    /*
      if the actual length is greater than the precision the we use the
      length as the precision
    */
    if (i4_precision < i4_len)
    {
        i4_precision = i4_len;
    }
    
    /* sign */
    if (i8_num < 0)
    {
        c_sign = '-';
    }
    else
    {
        if (ui4_flags & XV_PLUS)
        {
            c_sign = '+';
        }
        else if (ui4_flags & XV_SPACE)
        {
            c_sign = ' ';
        }
    }
    
    /* alignment length (may be negative, i.e. no alignment) */
    i4_alignment_len = (i4_field_width - i4_precision);
    
    if (c_sign != '\0')
    {
        i4_alignment_len--;
    }
    
    if ((ui4_flags & XV_LEFT) == 0) /* right alignment */
    {
        CHAR  c_alignment_char;
        
        c_alignment_char = (ui4_flags & XV_ZEROPAD) ? '0' : ' ';
        
        if ((ui4_flags & XV_ZEROPAD) && (c_sign != '\0'))
        {
            /* if padding is done with 0s, sign must be printed first */
            
            ps_buf = sprintf_output_char(ps_buf,
                                         pz_printed, z_max,
                                         c_sign,
                                         b_snprintf);
            
            c_sign = '\0';
        }
        
        ps_buf = sprintf_output_n_chars(ps_buf,
                                        pz_printed, z_max,
                                        i4_alignment_len,
                                        c_alignment_char,
                                        b_snprintf);
    }
    
    /* sign */
    if (c_sign != '\0')
    {
        ps_buf = sprintf_output_char(ps_buf,
                                     pz_printed, z_max,
                                     c_sign,
                                     b_snprintf);
    }
    
    /* padding zeros (if any) */
    ps_buf = sprintf_output_n_chars(ps_buf,
                                    pz_printed, z_max,
                                    i4_precision - i4_len,
                                    '0',
                                    b_snprintf);
    
    /* actual number */
    for (i4_i = 0, i8_tmp = 1; i4_i < i4_len - 1; i4_i++)
    {
        i8_tmp *= 10;
    }
    
    for (i4_i = 0; i4_i < i4_len; i4_i++)
    {
        INT32  i4_digit;
        CHAR   c_char;
        
        i4_digit = (INT32) (i8_num / i8_tmp);
        
        i8_num -= (i8_tmp * i4_digit);
        
        i8_tmp /= 10;
        
        c_char = (i4_digit >= 0) ? ps_digits[i4_digit]: ps_digits[-i4_digit];
        
        ps_buf = sprintf_output_char(ps_buf,
                                     pz_printed, z_max,
                                     c_char,
                                     b_snprintf);
    }
    
    if ((ui4_flags & XV_LEFT) != 0) /* left alignment */
    {
        ps_buf = sprintf_output_n_chars(ps_buf,
                                        pz_printed, z_max,
                                        i4_alignment_len,
                                        ' ',
                                        b_snprintf);
    }
    
    return ps_buf;
}

/*-----------------------------------------------------------------------------
 * Name: sprintf_output_p
 *
 * Description: This API processes the %p format. If the pointer is non-NULL
 *              then the pointer is printed as an unsigned integer in the form
 *              0xabc. If the pointer is NULL then (nil) is printed.
 *
 * Inputs:  ps_buf          References the output buffer.
 *          pv_ptr          Contains the pointer to output.
 *          pz_printed      References the number of characters printed so far.
 *          z_max           Contains the maximum number of characters to copy.
 *          ui4_flags       Contains the printf flags.
 *          i4_field_width  Contains the minimum field width.
 *          i4_precision    Contains the precision.
 *          b_snprintf      Specifies whether z_max is relevant.
 *
 * Outputs: pz_printed  Contains the number of characters that have been copied
 *                      so far, or the number of characters that would have
 *                      been copied had the output buffer been big enough.
 *
 * Returns: The new position in the output buffer.
 ----------------------------------------------------------------------------*/
static CHAR* sprintf_output_p (CHAR*    ps_buf,
                               VOID*    pv_ptr,
                               SIZE_T*  pz_printed,
                               SIZE_T   z_max,
                               UINT32   ui4_flags,
                               INT32    i4_field_width,
                               INT32    i4_precision,
                               BOOL     b_snprintf)
{
    if (pv_ptr != NULL)
    {
        ps_buf = sprintf_output_unsigned(ps_buf,
                                         (UINT64) ((UINT32)pv_ptr),
                                         16,
                                         pz_printed, z_max,
                                         ui4_flags | XV_SPECIAL,
                                         i4_field_width,
                                         i4_precision,
                                         b_snprintf);
    }
    else
    {
        ps_buf = sprintf_output_string(ps_buf,
                                       pz_printed, z_max,
                                       s_null_ptr, (SIZE_T) -1,
                                       b_snprintf);
    }
    
    return ps_buf;
}

/*-----------------------------------------------------------------------------
 * Name: sprintf_output_n
 *
 * Description: This API processes the %n format. Nothing is printed but the
 *              number of characters printed so far is copied in *pv_ptr.
 *
 * Inputs:  e_length_modifier  Contains the length specifier.
 *          z_printed          Contains the number of characters printed.
 *
 * Outputs: pv_ptr  Contains the number of characters printed so far.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID sprintf_output_n (VOID*                      pv_ptr,
                              SPRINTF_LENGTH_MODIFIER_T  e_length_modifier,
                              SIZE_T                     z_printed)
{
    switch (e_length_modifier)
    {
        case SPRINTF_NONE:
        case SPRINTF_LONG:
        {
            INT32*  pi4_arg = (INT32*) pv_ptr;
            
            *pi4_arg = (INT32) z_printed;
        }
        break;
        
        case SPRINTF_CHAR:
        {
            INT8*  pi1_arg = (INT8*) pv_ptr;
            
            *pi1_arg = (INT8) z_printed;
        }
        break;
        
        case SPRINTF_SHORT:
        {
            INT16*  pi2_arg = (INT16*) pv_ptr;
            
            *pi2_arg = (INT16) z_printed;
        }
        break;
        
        case SPRINTF_LONG_LONG:
        {
            INT64*  pi8_arg = (INT64*) pv_ptr;
            
            *pi8_arg = (INT64) z_printed;
        }
        break;
        
        default:
            break;
    }
}

/*----------------------------------------------------------------------------
 * Function: vsnprintf_generic
 *
 * Description: This API is a generic implementation of sprintf and is used by
 *              all the sprintf flavors (x_sprintf, x_snprintf, x_vsprintf and
 *              x_vsnprintf).
 *
 * Inputs:  ps_buf      References the output buffer.
 *          ps_fmt      References the format string.
 *          t_ap        Contains the list of parameters.
 *          z_max       Contains the maximum number of characters to copy.
 *          b_snprintf  Specifies whether z_max is relevant
 *
 * Outputs: -
 *
 * Returns: The number of characters written or that would have been written,
 *          not including the NULL character.
 *---------------------------------------------------------------------------*/
static INT32 vsnprintf_generic (CHAR*        ps_buf,
                                const CHAR*  ps_fmt,
                                VA_LIST      t_ap,
                                SIZE_T       z_max,
                                BOOL         b_snprintf)
{
    SIZE_T  z_printed;
    BOOL    b_null_char;
    BOOL    b_error;
    
    z_printed   = 0;
    b_null_char = !b_snprintf || (z_max > 0);
    b_error     = (ps_buf == NULL) || (ps_fmt == NULL);
    
    if (! b_error)
    {
        CHAR  c_fmt_char;
        
        /* the number of bytes that can be written (not including '\0') */
        if (z_max > 0)
        {
            z_max--;
        }
        
        while ((c_fmt_char = *ps_fmt) != '\0')
        {
            if (c_fmt_char != '%')
            {
                /* ordinary character */
                
                ps_buf = sprintf_output_char(ps_buf,
                                             & z_printed, z_max,
                                             c_fmt_char,
                                             b_snprintf);
            }
            else
            {
                const CHAR*                ps_fmt_start;
                SPRINTF_LENGTH_MODIFIER_T  e_length_modifier;
                UINT32                     ui4_flags;
                INT32                      i4_field_width;
                INT32                      i4_precision;
                
                ps_fmt_start      = ps_fmt;
                e_length_modifier = SPRINTF_NONE;
                ui4_flags         = 0;
                i4_field_width    = -1; /* -1 means it is not set */
                i4_precision      = -1; /* -1 means it is not set */
                
                /* Flags (zero or more) */
                while ((c_fmt_char = *++ps_fmt) != '\0')
                {
                    BOOL  b_exit = FALSE;
                    
                    switch (c_fmt_char)
                    {
                        case '-': ui4_flags |= XV_LEFT;     break;
                        case '+': ui4_flags |= XV_PLUS;     break;
                        case ' ': ui4_flags |= XV_SPACE;    break;
                        case '#': ui4_flags |= XV_SPECIAL;  break;
                        case '0': ui4_flags |= XV_ZEROPAD;  break;
                        default : b_exit = TRUE;            break;
                    }
                    
                    if (b_exit)
                    {
                        /* If both 0 and - appear, 0 is ignored */
                        if (ui4_flags & XV_LEFT)
                        {
                            ui4_flags &= ~ XV_ZEROPAD;
                        }
                        
                        break;
                    }
                }
                
                /* Field width */
                if (CHAR_IS_DIGIT(c_fmt_char))
                {
                    i4_field_width = 0;
                    
                    do
                    {
                        i4_field_width *= 10;
                        i4_field_width += (c_fmt_char - '0');
                        
                        c_fmt_char = *++ps_fmt;
                    }
                    while (CHAR_IS_DIGIT(c_fmt_char));
                }
                else if (c_fmt_char == '*')
                {
                    i4_field_width = VA_ARG(t_ap, INT32);
                    
                    if (i4_field_width < 0)
                    {
                        i4_field_width = - i4_field_width;
                        ui4_flags |= XV_LEFT;
                    }
                    
                    c_fmt_char = *++ps_fmt;
                }
                
                /* Precision (optional) */
                if (c_fmt_char == '.')
                {
                    i4_precision = 0;
                    
                    c_fmt_char = *++ps_fmt;
                    
                    if (CHAR_IS_DIGIT(c_fmt_char))
                    {
                        do
                        {
                            i4_precision *= 10;
                            i4_precision += (c_fmt_char - '0');
                            
                            c_fmt_char = *++ps_fmt;
                        }
                        while (CHAR_IS_DIGIT(c_fmt_char));
                    }
                    else if (c_fmt_char == '*')
                    {
                        i4_precision = VA_ARG(t_ap, INT32);
                        
                        if (i4_precision < 0)
                        {
                            i4_precision = 0;
                        }
                        
                        c_fmt_char = *++ps_fmt;
                    }
                }
                
                /* Length modifier (optional) */
                switch (c_fmt_char)
                {
                    case 'h':
                    {
                        e_length_modifier = SPRINTF_SHORT;
                        c_fmt_char = *++ps_fmt;
                        
                        if (c_fmt_char == 'h')
                        {
                            e_length_modifier = SPRINTF_CHAR;
                            c_fmt_char = *++ps_fmt;
                        }
                    }
                    break;
                    
                    case 'l':
                    {
                        e_length_modifier = SPRINTF_LONG;
                        c_fmt_char = *++ps_fmt;
                        
                        if (c_fmt_char == 'l')
                        {
                            e_length_modifier = SPRINTF_LONG_LONG;
                            c_fmt_char = *++ps_fmt;
                        }
                    }
                    break;
                    
                    case 'L':
                    {
                        e_length_modifier = SPRINTF_LONG_DOUBLE;
                        c_fmt_char = *++ps_fmt;
                    }
                    break;
                    
                    default:
                        /* nothing to do */
                        break;
                }
                
                /* Conversion specifier */
                switch (c_fmt_char)
                {
                    case 'c':
                    {
                        CHAR  c_char;
                        
                        c_char = (CHAR) VA_ARG(t_ap, INT32);
                        
                        ps_buf = sprintf_output_c(ps_buf,
                                                  c_char,
                                                  & z_printed, z_max,
                                                  ui4_flags,
                                                  i4_field_width,
                                                  b_snprintf);
                    }
                    break;
                    
                    case 's':
                    {
                        const CHAR*  ps_str;
                        
                        ps_str = (const CHAR*) VA_ARG(t_ap, CHAR*);
                        
                        if (ps_str == NULL)
                        {
                            ps_str = s_null_str;
                        }
                        
                        ps_buf = sprintf_output_s(ps_buf,
                                                  ps_str,
                                                  & z_printed, z_max,
                                                  ui4_flags,
                                                  i4_field_width,
                                                  i4_precision,
                                                  b_snprintf);
                    }
                    break;
                    
                    case 'p':
                    {
                        VOID*  pv_ptr;
                        
                        pv_ptr = VA_ARG(t_ap, VOID*);
                        
                        ps_buf = sprintf_output_p(ps_buf,
                                                  pv_ptr,
                                                  & z_printed, z_max,
                                                  ui4_flags,
                                                  i4_field_width,
                                                  i4_precision,
                                                  b_snprintf);
                    }
                    break;
                    
                    case 'n':
                    {
                        VOID*  pv_ptr;
                        
                        pv_ptr = VA_ARG(t_ap, VOID*);
                        
                        sprintf_output_n(pv_ptr, e_length_modifier, z_printed);
                    }
                    break;
                    
                    case 'd':
                    case 'i':
                    {
                        INT64  i8_num;
                        
                        if (e_length_modifier != SPRINTF_LONG_LONG)
                        {
                            i8_num = (INT64) VA_ARG(t_ap, INT32);
                            
                            /* Note: INT8 and INT16 are promoted to INT32 */
                        }
                        else
                        {
                            i8_num = VA_ARG(t_ap, INT64);
                        }
                        
                        if (i4_precision != -1)
                        {
                            ui4_flags &= ~ XV_ZEROPAD;
                        }
                        
                        ps_buf = sprintf_output_signed(ps_buf,
                                                       i8_num,
                                                       & z_printed, z_max,
                                                       ui4_flags,
                                                       i4_field_width,
                                                       i4_precision,
                                                       b_snprintf);
                    }
                    break;
                    
                    case 'o':
                    case 'u':
                    case 'x':
                    case 'X':
                    {
                        UINT64  ui8_num;
                        UINT32  ui4_base;
                        
                        ui4_base =
                            (c_fmt_char == 'u') ? (UINT32) 10 :
                            ((c_fmt_char == 'o') ? (UINT32) 8 : (UINT32) 16);
                        
                        if (c_fmt_char == 'X')
                        {
                            ui4_flags |= XV_LARGE;
                        }
                        
                        if (e_length_modifier != SPRINTF_LONG_LONG)
                        {
                            ui8_num = (UINT64) VA_ARG(t_ap, UINT32);
                            
                            /* Note: UINT8 and UINT16 are promoted to UINT32 */
                        }
                        else
                        {
                            ui8_num = VA_ARG(t_ap, UINT64);
                        }
                        
                        if (i4_precision != -1)
                        {
                            ui4_flags &= ~ XV_ZEROPAD;
                        }
                        
                        ps_buf = sprintf_output_unsigned(ps_buf,
                                                         ui8_num,
                                                         ui4_base,
                                                         & z_printed, z_max,
                                                         ui4_flags,
                                                         i4_field_width,
                                                         i4_precision,
                                                         b_snprintf);
                    }
                    break;
                    
                    case 'E':
                    case 'G':
                    case 'e':
                    case 'f':
                    case 'g':
                    {
                        long double  ld_num;
                        
                        ld_num = (e_length_modifier == SPRINTF_LONG_DOUBLE) ?
                            VA_ARG(t_ap, long double) :
                            VA_ARG(t_ap, DOUBLE);
                        
                        ps_buf = xvfloat(ps_buf,
                                         ld_num,
                                         & z_printed, z_max,
                                         ui4_flags | XV_SIGN,
                                         i4_field_width,
                                         i4_precision,
                                         c_fmt_char,
                                         b_snprintf);
                    }
                    break;
                    
                    case '%':
                    {
                        ps_buf = sprintf_output_c(ps_buf,
                                                  '%',
                                                  & z_printed, z_max,
                                                  ui4_flags, 1,
                                                  b_snprintf);
                    }
                    break;
                    
                    case '\0':
                    {
                        b_error = TRUE;
                        ps_fmt--;
                    }
                    break;
                    
                    default:
                    {
                        /* unrecognized pattern */
                        
                        i4_precision = (ps_fmt - ps_fmt_start) + 1;
                        
                        ps_buf = sprintf_output_s(ps_buf,
                                                  ps_fmt_start,
                                                  & z_printed, z_max,
                                                  ui4_flags,
                                                  -1, i4_precision,
                                                  b_snprintf);
                    }
                    break;
                }
            }
            
            ps_fmt++;
        }
    }
    
    if (b_null_char && !b_error)
    {
        *ps_buf = '\0';
    }
    
    return (b_error ? -1 : (INT32) z_printed);
}

/*----------------------------------------------------------------------------
 * Function: x_vsprintf
 *
 * Description: This API calls vsnprintf_generic, specifying that there is no
 *              limit on the output buffer (i.e. b_snprintf is FALSE).
 *
 * Inputs:  ps_buf  References the output buffer.
 *          ps_fmt  References the format string.
 *          t_ap    Contains the list of parameters.
 *
 * Outputs: -
 *
 * Returns: The number of characters written, not including the NULL character.
 *---------------------------------------------------------------------------*/
INT32 x_vsprintf (CHAR*        ps_buf,
                  const CHAR*  ps_fmt,
                  VA_LIST      t_ap)
{
    return vsnprintf_generic(ps_buf, ps_fmt, t_ap, 0, FALSE);
}

/*----------------------------------------------------------------------------
 * Function: x_vsnprintf
 *
 * Description: This API calls vsnprintf_generic, specifying that there is a
 *              limit on the output buffer (i.e. b_snprintf is TRUE).
 *
 * Inputs:  ps_buf  References the output buffer.
 *          z_size  Contains the size of the output buffer.
 *          ps_fmt  References the format string.
 *          t_ap    Contains the list of parameters.
 *
 * Outputs: -
 *
 * Returns: The number of characters written or that would have been written,
 *          not including the NULL character.
 *---------------------------------------------------------------------------*/
INT32 x_vsnprintf (CHAR*        ps_buf,
                   SIZE_T       z_size,
                   const CHAR*  ps_fmt,
                   VA_LIST      t_ap)
{
    return vsnprintf_generic(ps_buf, ps_fmt, t_ap, z_size, TRUE);
}

/*----------------------------------------------------------------------------
 * Function: x_sprintf
 *
 * Description: This API calls vsnprintf_generic, specifying that there is no
 *              limit on the output buffer (i.e. b_snprintf is FALSE).
 *
 * Inputs:  ps_buf  References the output buffer.
 *          ps_fmt  References the format string.
 *          ...     Additional parameters (i.e. elements to output).
 *
 * Outputs: -
 *
 * Returns: The number of characters written, not including the NULL character.
 *---------------------------------------------------------------------------*/
INT32 x_sprintf(CHAR *ps_buf, const CHAR *ps_fmt, ...)
{
    VA_LIST t_ap;
    INT32   i4_len;
    
    VA_START(t_ap, ps_fmt);
    
    i4_len = vsnprintf_generic(ps_buf, ps_fmt, t_ap, 0, FALSE);
    
    VA_END(t_ap);
    
    return i4_len;
}

/*----------------------------------------------------------------------------
 * Function: x_snprintf
 *
 * Description: This API calls vsnprintf_generic, specifying that there is a
 *              limit on the output buffer (i.e. b_snprintf is TRUE).
 *
 * Inputs:  ps_buf  References the output buffer.
 *          z_size  Contains the size of the output buffer.
 *          ps_fmt  References the format string.
 *          ...     Additional parameters (i.e. elements to output).
 *
 * Outputs: -
 *
 * Returns: The number of characters written, not including the NULL character.
 *---------------------------------------------------------------------------*/
INT32 x_snprintf(CHAR*        ps_buf,
                 SIZE_T       z_size,
                 const CHAR*  ps_fmt, ...)
{
    VA_LIST t_ap;
    INT32   i4_len;
    
    VA_START(t_ap, ps_fmt);
    
    i4_len = vsnprintf_generic(ps_buf, ps_fmt, t_ap, z_size, TRUE);
    
    VA_END(t_ap);
    
    return i4_len;
}

static VOID
xvcfloat(long double value, CHAR *buffer, CHAR fmt, INT32 precision)
{
    INT32 decpt, sign, exp, pos;
    INT32 magnitude;
    INT32 capexp = 0;
    CHAR *digits = NULL;
    CHAR cvtbuf[CVTBUFSIZE];


    if ((fmt == 'G') || (fmt == 'E'))
    {
        capexp = 1;
        fmt += 'a' - 'A';
    }

    if (fmt == 'g')
    {
        digits = xvconvert(value, precision, &decpt, &sign, cvtbuf, TRUE);
        magnitude = decpt - 1;
        if ((magnitude < -4) || (magnitude > (precision - 1)))
        {
            fmt = 'e';
            precision -= 1;
        }
        else
        {
            fmt = 'f';
            precision -= decpt;
        }
    }

    if (fmt == 'e')
    {
        digits = xvconvert(value, (precision + 1), &decpt, &sign, cvtbuf, TRUE);
        if (sign)
        {
            *buffer++ = '-';
        }
        *buffer++ = *digits;
        if (precision > 0)
        {
            *buffer++ = '.';
        }
        x_memcpy(buffer, digits + 1, (SIZE_T) precision);
        buffer += precision;
        *buffer++ = capexp ? 'E' : 'e';

        exp = (decpt == 0) ? ((value == 0.0) ? 0 : -1) : (decpt - 1);
        if (exp < 0)
        {
            *buffer++ = '-';
            exp = -exp;
        }
        else
        {
            *buffer++ = '+';
        }

        buffer[2] = (CHAR) ((exp % 10) + '0');
        exp = exp / 10;
        buffer[1] = (CHAR) ((exp % 10) + '0');
        exp = exp / 10;
        buffer[0] = (CHAR) ((exp % 10) + '0');
        buffer += 3;
    }
    else if (fmt == 'f')
    {
        digits = xvconvert(value, precision, &decpt, &sign, cvtbuf, FALSE);
        if (sign)
        {
            *buffer++ = '-';
        }
        if (*digits)
        {
            if (decpt <= 0)
            {
                *buffer++ = '0';
                *buffer++ = '.';
                for (pos = 0; pos < -decpt; pos++)
                {
                    *buffer++ = '0';
                }
                while (*digits)
                {
                    *buffer++ = *digits++;
                }
            }
            else
            {
                pos = 0;
                while (*digits)
                {
                    if (pos++ == decpt)
                    {
                        *buffer++ = '.';
                    }
                    *buffer++ = *digits++;
                }
            }
        }
        else
        {
            *buffer++ = '0';
            if (precision > 0)
            {
                *buffer++ = '.';
                for (pos = 0; pos < precision; pos++)
                {
                    *buffer++ = '0';
                }
            }
        }
    }

    *buffer = '\0';
}


static VOID 
xvcdecpt(CHAR *buffer)
{
    while (*buffer)
    {
        if (*buffer == '.')
        {
            return;
        }
        if ((*buffer == 'e') || (*buffer == 'E'))
        {
            break;
        }
        buffer++;
    }

    if (*buffer)
    {
        INT32 n = (INT32) x_strlen(buffer);

        while (n > 0) 
        {
            buffer[n + 1] = buffer[n];
            n--;
        }

        *buffer = '.';
    }
    else
    {
        *buffer++ = '.';
        *buffer = '\0';
    }
}


static VOID
xvczeros(CHAR *buffer)
{
    CHAR *stop;


    while (*buffer && (*buffer != '.'))
    {
        buffer++;
    }
    if (*buffer++)
    {
        while (*buffer && (*buffer != 'e') && (*buffer != 'E'))
        {
            buffer++;
        }
        stop = buffer--;
        while (*buffer == '0')
        {
            buffer--;
        }
        if (*buffer == '.')
        {
            buffer--;
        }
        /*while (*++buffer = *stop++)*/
        while ((*++buffer = *stop++) != '\0')
        {
            /* blank */;
        }
    }
}


static CHAR* xvfloat (CHAR*        str,
                      long double  num,
                      SIZE_T*      pz_printed,
                      SIZE_T       z_max,
                      UINT32       ui4_flags,
                      INT32        size,
                      INT32        precision,
                      CHAR         fmt,
                      BOOL         b_snprintf)
{
    INT32 n;
    CHAR tmp[CVTBUFSIZE];
    CHAR c, sign;

    /* Left align means no zero padding */
    if (ui4_flags & XV_LEFT)
    {
        ui4_flags &= ~XV_ZEROPAD;
    }

    /* Determine padding and sign char */
    c = (ui4_flags & XV_ZEROPAD) ? '0' : ' ';
    sign = 0;
    if (ui4_flags & XV_SIGN)
    {
        if (num < 0.0)
        {
            sign = '-';
            num = -num;
            size--;
        }
        else if (ui4_flags & XV_PLUS)
        {
            sign = '+';
            size--;
        }
        else if (ui4_flags & XV_SPACE)
        {
            sign = ' ';
            size--;
        }
    }

    /* Compute the precision value */
    if (precision < 0)
    {
        precision = 6;  /* default */
    }
    else if ((precision == 0) && (fmt == 'g'))
    {
        precision = 1;  /* ANSI specified */
    }

    /* Convert floating point number to string */
    xvcfloat(num, tmp, fmt, precision);

    /* '#' and precision == 0 means force a decimal point */
    if ((ui4_flags & XV_SPECIAL) && (precision == 0))
    {
        xvcdecpt(tmp);
    }

    /* 'g' format means crop zero unless '#' given */
    if ((fmt == 'g') && !(ui4_flags & XV_SPECIAL))
    {
        xvczeros(tmp);
    }

    n = (INT32) x_strlen(tmp);

    /* Output number with alignment and padding */
    size -= n;
    if (!(ui4_flags & (XV_ZEROPAD | XV_LEFT)))
    {
        while (size-- > 0)
        {
            str = sprintf_output_char(str,
                                      pz_printed, z_max,
                                      ' ',
                                      b_snprintf);
        }
    }
    if (sign)
    {
        str = sprintf_output_char(str,
                                  pz_printed, z_max,
                                  sign,
                                  b_snprintf);
    }
    if (!(ui4_flags & XV_LEFT))
    {
        while (size-- > 0)
        {
            str = sprintf_output_char(str,
                                      pz_printed, z_max,
                                      c,
                                      b_snprintf);
        }
    }
    
    str = sprintf_output_string(str,
                                pz_printed, z_max,
                                tmp, (SIZE_T) -1,
                                b_snprintf);
    
    while (size-- > 0)
    {
        str = sprintf_output_char(str,
                                  pz_printed, z_max,
                                  ' ',
                                  b_snprintf);
    }

    return str;
}


static CHAR *
xvconvert(long double arg,
          INT32 ndigits,
          INT32 *decpt,
          INT32 *sign,
          CHAR *buf,
          BOOL eflag)
{
    INT32 i;
    CHAR *p, *pl;
    long double  fi, fj;


    if (ndigits < 0)
    {
        ndigits = 0;
    }
    if (ndigits >= CVTBUFSIZE - 1)
    {
        ndigits = CVTBUFSIZE - 2;
    }

    i = 0;
    *sign = 0;
    p = &buf[0];
    if (arg < 0)
    {
        *sign = 1;
        arg = -arg;
    }
    arg = xvmodf(arg, &fi);
    pl = &buf[CVTBUFSIZE];

    if (fi != 0) 
    {
        pl = &buf[CVTBUFSIZE];
        while (fi != 0) 
        {
            fj = xvmodf(fi / 10, &fi);
            *--pl = (int)((fj + .03) * 10) + '0';
            i++;
        }
        while (pl < &buf[CVTBUFSIZE])
        {
            *p++ = *pl++;
        }
    }
    else if (arg > 0)
    {
        while ((fj = (arg * 10)) < 1) 
        {
            arg = fj;
            i--;
        }
    }

    pl = &buf[ndigits];
    if (eflag != TRUE)
    {
        pl += i;
    }
    *decpt = i;
    if (pl < &buf[0]) 
    {
        buf[0] = '\0';
        return buf;
    }
    while ((p <= pl) && (p < &buf[CVTBUFSIZE]))
    {
        arg *= 10;
        arg = xvmodf(arg, &fj);
        *p++ = (int) fj + '0';
    }
    if (pl >= &buf[CVTBUFSIZE]) 
    {
        buf[CVTBUFSIZE - 1] = '\0';
        return buf;
    }

    p = pl;
    *pl += 5;
    while (*pl > '9') 
    {
        *pl = '0';
        if (pl > buf)
        {
            ++*--pl;
        }
        else 
        {
            *pl = '1';
            (*decpt)++;
            if (eflag != TRUE) 
            {
                if (p > buf)
                {
                    *p = '0';
                }
                p++;
            }
        }
    }
    *p = '\0';

    return buf;
}


static long double
xvmodf(long double d_d, long double *pd_d)
{
    INT32 i;
    INT32 l;
    long double d_e = d_d;
    long double d_f = 1.0;


#define MODF_LOOP       50     /* well enough; worst cases seen in mid 20's */
#define MODF_DEVIATION  (0.1)

    for (i = 0; i < MODF_LOOP; i++)
    {
        l = (INT32) d_e;
        if (((long double) l < (d_e + 1)) && ((long double) l > (d_e - 1)))
        {
            break;
        }
        d_e *= MODF_DEVIATION;
        d_f *= 10.0;
    }

    if (i == MODF_LOOP)
    {
        (*pd_d) = 0;
        return 0;
    }

    if (i != 0)
    {
        long double  i2;
        long double  ret;

        ret = xvmodf((d_d - (l * d_f)), &i2);
        (*pd_d) = (l * d_f) + i2;
        return ret;
    } 

    (*(long double *) pd_d) = (long double) l;
    return(d_e - (*pd_d));
}
