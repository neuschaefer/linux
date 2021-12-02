/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*******************************************************************************
 *
 * Filename:
 * ---------
 *   LIB_TYPEDEF.H
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR-WB codec
 *
 * Author:
 * -------
 *   Eddy Wu
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/

#ifndef __LIB_TYPEDEF__
#define __LIB_TYPEDEF__

#include    "typedef.h"

#if !defined(_LANGUAGE_ASM) /* Added to Bypass the C typedefs for assembler */

/*
 * Target dependent macros.
 *
 * The macros __LITTLE_END__  and __BIGEND__ are set here
 * based on target platform, and indicate if the smallest indexed
 * byte comes first, or last.
 *
 */

// Eddy 2008.06.23 Remove warning
#if defined (__WIN32__) || defined (__linux__) || defined (i386) || defined (i486) ||  defined (_MSC_VER)|| defined (__arm)
#define __LITTLE_END__
#undef __BIG_END__
#elif defined (__sun__) || defined (sun) || defined (SUN)
#define __BIG_END__
#undef __LITTLE_END__
#else
#pragma "ERROR: cannot determine platform"
#endif


/* When switch FLTPNT is on, all types are mapped to
 * floating point. This is for floating point simulations.
 */
#ifdef FLTPNT
typedef float fract16;
typedef float fract32;
typedef double fract40;

typedef struct {
float hi, lo;
} fract2x16;

typedef struct {
 float re, im;
} cmplx_fr16;


/* When switch FLTPNT is off, the library does
 * fixed point simulations
 */
#else

// Eddy
#if __CC_ARM
typedef short  fract16;
typedef long   fract32;
typedef long   cmplx_fr16;
typedef long   fract2x16;
#else
typedef signed short  fract16;
typedef signed int    fract32;
typedef signed int    cmplx_fr16;
typedef signed int    fract2x16;
#endif

typedef  struct {
 long	 accw32;
 unsigned char accx8;
}fract40;

#endif

/*
 * The following types are ints for both fixed
 * and float systems
 */
#if __CC_ARM
typedef char   int8;
typedef short  int16;
typedef long   int32;
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;
typedef int int_native;
typedef unsigned short bit;
typedef unsigned short boolean;
#else
typedef signed char   int8;
typedef signed short  int16;
typedef signed int    int32;
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef signed int int_native;
typedef unsigned short bit;
typedef unsigned short boolean;
#endif

#endif /* _LANGUAGE_ASM */

#endif /* __LIB_TYPEDEF__ */
