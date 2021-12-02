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
 Unix snprintf implementation.
 Version 1.3

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   It can be redistribute also under the terms of GNU Library General
   Public Lincense.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Revision History:
                   see header of snprintf.c.

format:
  int snprintf(holder, sizeof_holder, format, ...)

Return values:
  (sizeof_holder - 1)


 THANKS(for the patches and ideas):
     Miles Bader
     Cyrille Rustom
     Jacek Slabocewiz
     Mike Parker(mouse)

Alain Magloire: alainm@rcsm.ee.mcgill.ca
*/

#ifdef _PSOS_
#define HAVE_STDARG_H
#elif defined(_VXWORKS_)
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#elif defined(_UCOS_)
#define HAVE_STDARG_H
#elif defined(_LINUX_)
#define HAVE_STDARG_H
#endif

#if defined(HAVE_STDARG_H) && defined(__STDC__) && __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <string.h>
#ifndef _UCOS_
#include <stdlib.h>    /* for atoi() */
#endif	/* _UCOS_ */
#include <ctype.h>


/*
 * For the FLOATING POINT FORMAT :
 *  the challenge was finding a way to
 *  manipulate the Real numbers without having
 *  to resort to mathematical function(it
 *  would require to link with -lm) and not
 *  going down to the bit pattern(not portable)
 *
 *  so a number, a real is:

      real = integral + fraction

      integral = ... + a(2)*10^2 + a(1)*10^1 + a(0)*10^0
      fraction = b(1)*10^-1 + b(2)*10^-2 + ...

      where:
       0 <= a(i) => 9
       0 <= b(i) => 9

    from then it was simple math
 */

/*
 * size of the buffer for the integral part
 * and the fraction part
 */
#define MAX_INT  99 + 1 /* 1 for the null */
#define MAX_FRACT 29 + 1

/*
 * numtoa() uses PRIVATE buffers to store the results,
 * So this function is not reentrant
 */
#define itoa(n) numtoa(n, 10, 0, (char **)0)
#define otoa(n) numtoa(n, 8, 0, (char **)0)
#define htoa(n) numtoa(n, 16, 0, (char **)0)
#define dtoa(n, p, f) numtoa(n, 10, p, f)

#define SWAP_INT(a,b) {int t; t = (a); (a) = (b); (b) = t;}

/* this struct holds everything we need */
struct DATA {
  int length;
  char *holder;
  int counter;
#ifdef __STDC__
  const char *pf;
#else
  char *pf;
#endif
/* FLAGS */
  int width, precision;
  int justify; char pad;
  int square, space, star_w, star_p, a_long ;
};

#define PRIVATE static
#define PUBLIC
/* signature of the functions */
#ifdef __STDC__
/* the floating point stuff */
  PRIVATE double pow_10(int);
  PRIVATE int log_10(double);
  PRIVATE double integral(double, double *);
  PRIVATE char * numtoa(double, int, int, char **);

/* for the format */
  PRIVATE void conv_flag(char *, struct DATA *);
  PRIVATE void floating(struct DATA *, double);
  PRIVATE void exponent(struct DATA *, double);
  PRIVATE void decimal(struct DATA *, double);
  PRIVATE void octal(struct DATA *, double);
  PRIVATE void hexa(struct DATA *, double);
  PRIVATE void strings(struct DATA *, char *, int);

#else
/* the floating point stuff */
  PRIVATE double pow_10();
  PRIVATE int log_10();
  PRIVATE double integral();
  PRIVATE char * numtoa();

/* for the format */
  PRIVATE void conv_flag();
  PRIVATE void floating();
  PRIVATE void exponent();
  PRIVATE void decimal();
  PRIVATE void octal();
  PRIVATE void hexa();
  PRIVATE void strings();
#endif

/* those are defines specific to snprintf to hopefully
 * make the code clearer :-)
 */
#define RIGHT 1
#define LEFT  0
#define NOT_FOUND -1
#define FOUND 1
#define MAX_FIELD 15

/* the conversion flags */
#define isflag(c) ((c) == '#' || (c) == ' ' || \
                   (c) == '*' || (c) == '+' || \
                   (c) == '-' || (c) == '.' || \
                   isdigit(c))

/* round off to the precision */
#define ROUND(d, p) \
            (d < 0.) ? \
             d - pow_10(-(p)->precision) * 0.5 : \
             d + pow_10(-(p)->precision) * 0.5

/* set default precision */
#define DEF_PREC(p) \
            if ((p)->precision == NOT_FOUND) \
              (p)->precision = 6

/* put a char */
#define PUT_CHAR(c, p) \
            if ((p)->counter < (p)->length) { \
              *(p)->holder++ = (c); \
              (p)->counter++; \
            }

#define PUT_PLUS(d, p) \
            if ((d) > 0. && (p)->justify == RIGHT) \
              PUT_CHAR('+', p)

#define PUT_SPACE(d, p) \
            if ((p)->space == FOUND && (d) > 0.) \
              PUT_CHAR(' ', p)

/* pad right */
#define PAD_RIGHT(p) \
            if ((p)->width > 0 && (p)->justify != LEFT) \
              for (; (p)->width > 0; (p)->width--) \
                 PUT_CHAR((p)->pad, p)

/* pad left */
#define PAD_LEFT(p) \
            if ((p)->width > 0 && (p)->justify == LEFT) \
              for (; (p)->width > 0; (p)->width--) \
                 PUT_CHAR((p)->pad, p)

/* if width and prec. in the args */
#define STAR_ARGS(p) \
            if ((p)->star_w == FOUND) \
			{ \
              (p)->width = va_arg(args, int); \
              if ((p)->width < 0) \
			  { \
      		    (p)->width = -(p)->width; \
      		    (p)->justify = LEFT; \
			  } \
			} \
            if ((p)->star_p == FOUND) \
              (p)->precision = va_arg(args, int)
