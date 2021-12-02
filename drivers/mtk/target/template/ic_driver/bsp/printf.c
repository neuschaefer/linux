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
/*
    Copyright 2001, 2002 Georges Menie (www.menie.org)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
    putchar is the only external dependency for this file,
    if you have a working putchar, just remove the following
    define. If the function should be called something else,
    replace outbyte(c) by your own function call.
*/

#include "x_lint.h"

LINT_SAVE_AND_DISABLE   // Do not lint this file

#include "x_serial.h"
#include "x_assert.h"

#define putchar(c) outbyte(c)

static void printchar(CHAR **str, INT32 c)
{
    if (str) {
        **str = c;
        ++(*str);
    }
    else (void)putchar((CHAR)c);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

INT32 prints(CHAR **out, const CHAR *string, INT32 width, INT32 pad)
{
    register INT32 pc = 0, padchar = ' ';

    if (width > 0) {
        register INT32 len = 0;
        register const CHAR *ptr;
        for (ptr = string; *ptr; ++ptr) ++len;
        if (len >= width) width = 0;
        else width -= len;
        if (pad & PAD_ZERO) padchar = '0';
    }
    if (!(pad & PAD_RIGHT)) {
        for ( ; width > 0; --width) {
            printchar (out, padchar);
            ++pc;
        }
    }
    for ( ; *string ; ++string) {
        printchar (out, *string);
        ++pc;
    }
    for ( ; width > 0; --width) {
        printchar (out, padchar);
        ++pc;
    }

    return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

INT32 printi(CHAR **out, INT32 i, INT32 b, INT32 sg, INT32 width, INT32 pad, INT32 letbase)
{
    CHAR print_buf[PRINT_BUF_LEN];
    register CHAR *s;
    register INT32 t, neg = 0, pc = 0;
    register UINT32 u = i;

    if (i == 0) {
        print_buf[0] = '0';
        print_buf[1] = '\0';
        return prints (out, print_buf, width, pad);
    }

    if (sg && b == 10 && i < 0) {
        neg = 1;
        u = -i;
    }

    s = print_buf + PRINT_BUF_LEN-1;
    *s = '\0';

    while (u) {
        t = u % b;
        if( t >= 10 )
            t += letbase - '0' - 10;
        *--s = t + '0';
        u /= b;
    }

    if (neg) {
        if( width && (pad & PAD_ZERO) ) {
            printchar (out, '-');
            ++pc;
            --width;
        }
        else {
            *--s = '-';
        }
    }

    return pc + prints (out, s, width, pad);
}

static INT32 print(CHAR **out, INT32 *varg)
{
    register INT32 width, pad;
    register INT32 pc = 0;
    register CHAR *format = (CHAR *)(*varg++);
    CHAR scr[2];

    for (; *format != 0; ++format) {
        if (*format == '%') {
            ++format;
            width = pad = 0;
            if (*format == '\0') break;
            if (*format == '%') goto out;
            if (*format == '-') {
                ++format;
                pad = PAD_RIGHT;
            }
            while (*format == '0') {
                ++format;
                pad |= PAD_ZERO;
            }
            for ( ; *format >= '0' && *format <= '9'; ++format) {
                width *= 10;
                width += *format - '0';
            }
            if( *format == 's' ) {
                register CHAR *s = *((CHAR **)varg++);
                pc += prints (out, s?s:"(null)", width, pad);
                continue;
            }
            if( *format == 'x' || *format == 'p' ) {
                pc += printi (out, *varg++, 16, 0, width, pad, 'a');
                continue;
            }
            if( *format == 'X' ) {
                pc += printi (out, *varg++, 16, 0, width, pad, 'A');
                continue;
            }
            if( *format == 'u' ) {
                pc += printi (out, *varg++, 10, 0, width, pad, 'a');
                continue;
            }
            if( *format == 'c' ) {
                /* char are converted to int then pushed on the stack */
                scr[0] = *varg++;
                scr[1] = '\0';
                pc += prints (out, scr, width, pad);
                continue;
            }
            if( *format == 'l' ) { ++format; }
            if( *format == 'd' ) {
                pc += printi (out, *varg++, 10, 1, width, pad, 'a');
                continue;
            }
        }
        else {
        out:
            printchar (out, *format);
            ++pc;
        }
    }
    if (out) **out = '\0';
    return pc;
}

/* assuming sizeof(void *) == sizeof(int) */

//int printf(const char *format, ...)
INT32 Printf(const CHAR *format, ...)
{
    INT32 nRet;
    register INT32 *varg = (INT32 *)(&format);

//  SerInit();
    nRet = print(0, varg);
//  SerEnd();

    return nRet;
}

//int sprintf(char *out, const char *format, ...)
INT32 SPrintf(CHAR *out, const CHAR *format, ...)
{
    register INT32 *varg = (INT32 *)(&format);
    return print(&out, varg);
}

//===========================================================================

// Translate unsigned long long (64 bits) value to ASCIIZ string
CHAR* lltoa(CHAR* dst, UINT64 u8Val, UINT32 u4Radix)
{
    CHAR buf[64];
    INT32 i, idx = 0;
    UINT32 u4Mod;

    if (u8Val)
    {
        while (u8Val > 0)
        {
            u4Mod = u8Val % u4Radix;
            u8Val /= u4Radix;

            if (u4Mod < 10)
                buf[idx] = '0' + u4Mod;
            else
                buf[idx] = 'a' + u4Mod - 10;

            idx++;
        }
    }
    else
    {
        buf[idx++] = '0';
    }

    if (dst != NULL)
    {
        for (i = 0; i < idx; i++)
            dst[i] = buf[idx - i - 1];

        dst[idx] = '\0';
    }

    return dst;
}

//===========================================================================
#if 0
CHAR* ftoa(CHAR* buf, DOUBLE fVal, CHAR decimal)
{
    UINT64 u8Int;
    DOUBLE fInt, fDec, fRef;
    CHAR bufInt[32], bufDec[32];
    UINT8 u1Digit = 0, i;

    // Process the integer part
    u8Int = (UINT64)fVal;
    lltoa(bufInt, u8Int, 10);

    // Process the decimal part
    fInt = (DOUBLE)u8Int;
    fDec = fVal - fInt;
    ASSERT(fDec < 1.0);
    fRef = 0.1;
    for (i = 0; i < decimal; i++)
    {
        u1Digit = (UINT8)(fDec / fRef);
        ASSERT(u1Digit < 10);

        fDec -= (fRef * u1Digit);
        fRef /= 10;
        bufDec[i] = '0' + u1Digit;
    }

    if (decimal > 1 && u1Digit >= 5)
    {
        INT32 u4CurDigit = decimal - 1;
        BOOL fgPropagate = TRUE;

        do
        {
            u1Digit = bufDec[u4CurDigit] - '0';
            u1Digit++;
            if (u1Digit >= 10)
            {
                u1Digit -= 10;
            }
            else
            {
                fgPropagate = FALSE;
            }

            bufDec[u4CurDigit] = u1Digit + '0';
            u4CurDigit--;
        } while (fgPropagate && u4CurDigit > 0);
    }

    bufDec[decimal] = '\0';

    // Concatenate integer part and decimal part
    SPrintf(buf, "%s.%s", bufInt, bufDec);

    return buf;
}
#endif

//===========================================================================

#ifdef TEST_PRINTF
INT32 main(void)
{
    CHAR *ptr = "Hello world!";
    CHAR *np = 0;
    INT32 i = 5;
    UINT32 bs = sizeof(INT32)*8;
    INT32 mi;
    CHAR buf[80];

    mi = (1 << (bs-1)) + 1;
    printf("%s\n", ptr);
    printf("printf test\n");
    printf("%s is null pointer\n", np);
    printf("%d = 5\n", i);
    printf("%d = - max int\n", mi);
    printf("char %c = 'a'\n", 'a');
    printf("hex %x = ff\n", 0xff);
    printf("hex %02x = 00\n", 0);
    printf("signed %d = unsigned %u = hex %x\n", -3, -3, -3);
    printf("%d %s(s)%", 0, "message");
    printf("\n");
    printf("%d %s(s) with %%\n", 0, "message");
    sprintf(buf, "justif: \"%-10s\"\n", "left"); printf("%s", buf);
    sprintf(buf, "justif: \"%10s\"\n", "right"); printf("%s", buf);
    sprintf(buf, " 3: %04d zero padded\n", 3); printf("%s", buf);
    sprintf(buf, " 3: %-4d left justif.\n", 3); printf("%s", buf);
    sprintf(buf, " 3: %4d right justif.\n", 3); printf("%s", buf);
    sprintf(buf, "-3: %04d zero padded\n", -3); printf("%s", buf);
    sprintf(buf, "-3: %-4d left justif.\n", -3); printf("%s", buf);
    sprintf(buf, "-3: %4d right justif.\n", -3); printf("%s", buf);

    return 0;
}

/*
 * if you compile this file with
 *   gcc -Wall $(YOUR_C_OPTIONS) -DTEST_PRINTF -c printf.c
 * you will get a normal warning:
 *   printf.c:214: warning: spurious trailing `%' in format
 * this line is testing an invalid % at the end of the format string.
 *
 * this should display (on 32bit int machine) :
 *
 * Hello world!
 * printf test
 * (null) is null pointer
 * 5 = 5
 * -2147483647 = - max int
 * char a = 'a'
 * hex ff = ff
 * hex 00 = 00
 * signed -3 = unsigned 4294967293 = hex fffffffd
 * 0 message(s)
 * 0 message(s) with %
 * justif: "left      "
 * justif: "     right"
 *  3: 0003 zero padded
 *  3: 3    left justif.
 *  3:    3 right justif.
 * -3: -003 zero padded
 * -3: -3   left justif.
 * -3:   -3 right justif.
 */

#endif

LINT_RESTORE

