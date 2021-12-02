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
 *   LIB_ARITH.H
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


#ifndef __LIB_ARITH_H__
#define __LIB_ARITH_H__

#if !defined(_LANGUAGE_ASM) /* Added to Bypass the C typedefs for assembler */

#include "lib_typedef.h"

/***
 * Following MACROS are used for converting complex structure to
 * integer and integer to complex structure 
 ***/ 	

#define CMPLX2INT(X) compose_fr1x32(X.im, X.re)

#define INT2CMPLX(R,X) \
  { \
    R.re = extract_lo_fr1x32(X); \
    R.im = extract_hi_fr1x32(X); \
  }

/***
 * addition of two 16 bit fract16 numbers and giving 
 * result of 16 bit fract16 type
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 add_fr1x16_f(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 add_fr1x16(fract16 var1, fract16 var2) 
  {
    return((fract16)__builtin_add_fr2x16(var1, var2));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 add_fr1x16(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Vector addition of two 32 bit fract2x16 numbers and giving 
 * result of 32 bit fract2x16 type
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 add_aa_fr2x16_f(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract2x16 add_aa_fr2x16(fract2x16 var1, fract2x16 var2) 
  {
    return(__builtin_add_fr2x16(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 add_aa_fr2x16(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Vector subtraction of two 32 bit fract2x16 numbers and giving 
 * result of 32 bit fract2x16 type
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 add_ss_fr2x16_f(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract2x16 add_ss_fr2x16(fract2x16 var1, fract2x16 var2) 
  {
    return(__builtin_sub_fr2x16(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
   fract2x16 add_ss_fr2x16(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Vector add/sub of two 32 bit fract2x16 numbers and giving 
 * result of 32 bit fract2x16 type
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
  fract2x16 add_as_fr2x16_f(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract2x16 add_as_fr2x16(fract2x16 var1, fract2x16 var2) 
  {
	fract2x16  _out;
  	asm("%0=%1 +|- %2(s);"  
	    :"=d" (_out)
	    :"d" (var1), "d" (var2)
	    :); 	  	
	return _out;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 add_as_fr2x16(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Vector sub/add of two 32 bit fract2x16 numbers and giving 
 * result of 32 bit fract2x16 type
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 add_sa_fr2x16_f(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract2x16 add_sa_fr2x16(fract2x16 var1, fract2x16 var2) 
  {
	fract2x16  _out;
  	asm("%0=%1 -|+ %2(s);"  
	    :"=d" (_out)
	    :"d" (var1), "d" (var2)
	    :); 	  	
	return _out;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 add_sa_fr2x16(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Subtraction of two 16 bit fract16 numbers and giving 
 * result of 16 bit fract16 type
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 sub_fr1x16_f(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 sub_fr1x16(fract16 var1, fract16 var2) 
  {
    return((fract16) __builtin_sub_fr2x16(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 sub_fr1x16(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Multiplying two 16 bit fract16 numbers and giving 
 * result of 16 bit fract16 type
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 mult_fr1x16_f(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 mult_fr1x16(fract16 var1, fract16 var2) 
  {
    return((fract16) __builtin_mult_fr2x16(var1, var2));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 mult_fr1x16(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Multiplying two 32 bit fract2x16 numbers and giving 
 * result of 32 bit fract2x16 type
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 mult_fr2x16_f(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract2x16 mult_fr2x16(fract2x16 var1, fract2x16 var2) 
  {
    return(__builtin_mult_fr2x16(var1, var2));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 mult_fr2x16(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Multiplying two 16 bit fract16 numbers and giving 
 * result of 16 bit with rounded fract16 type
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract16 multr_fr1x16(fract16 var1, fract16 var2) 
  {
    return((fract16) __builtin_multr_fr2x16(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 multr_fr1x16(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Multiplying two 32 bit fract2x16 numbers and giving 
 * result of 32 bit with rounded fract2x16 type
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract2x16 multr_fr2x16(fract2x16 var1, fract2x16 var2) 
  {
    return(__builtin_multr_fr2x16(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 multr_fr2x16(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Maximum value of two 16 bit fract16 numbers and giving 
 * result of 16 bit fract16 type
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 max_fr1x16_f(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 max_fr1x16(fract16 var1, fract16 var2) 
  {
    return((fract16) __builtin_max_fr2x16(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 max_fr1x16(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Maximum value of fract2x16 number and giving 
 * result of 16 bit fract16 type
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 max_fr2x16_f(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract2x16 max_fr2x16(fract2x16 var1, fract2x16 var2) 
  {
    return(__builtin_max_fr2x16(var1, var2));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 max_fr2x16(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for finding the absolute value of given fract16 variable
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 abs_fr1x16_f(fract16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 abs_fr1x16(fract16 var1) 
  {
    return((fract16) __builtin_abs_fr2x16(var1));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 abs_fr1x16(fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for finding the absolute value of given fract2x16 variable
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 abs_fr2x16_f(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract2x16 abs_fr2x16(fract2x16 var1) 
  {
    return(__builtin_abs_fr2x16(var1));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 abs_fr2x16(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for finding the minimum value of 
 * given two fract16 variables
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 min_fr1x16_f(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 min_fr1x16(fract16 var1, fract16 var2) 
  {
    return((fract16) __builtin_min_fr2x16(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 min_fr1x16(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Minimum value of fract2x16 number and giving 
 * result of fract2x16 type
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 min_fr2x16_f(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract2x16 min_fr2x16(fract2x16 var1, fract2x16 var2) 
  {
    return(__builtin_min_fr2x16(var1, var2));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 min_fr2x16(fract2x16, fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for finding the 2's complement(negetive) value of 
 * given fract16 variable
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 negate_fr1x16_f(fract16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 negate_fr1x16(fract16 var1) 
  {
    return((fract16) __builtin_negate_fr2x16(var1));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 negate_fr1x16(fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for finding the 2's complement(negetive)value of 
 * given fract2x16 variable
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 negate_fr2x16_f(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract2x16 negate_fr2x16(fract2x16 var1) 
  {
    return(__builtin_negate_fr2x16(var1));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 negate_fr2x16(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for shifting left the var1 by var2 times giving 
 * the result of 16bit 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 shl_fr1x16_f(fract16, int);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 shl_fr1x16(fract16 var1, int var2) 
  {
    return((fract16) __builtin_shl_fr2x16(var1, var2));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 shl_fr1x16(fract16, int);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for shifting left the var1 by var2 times giving 
 * the result of fract2x16 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 shl_fr2x16_f(fract2x16, int);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract2x16 shl_fr2x16(fract2x16 var1, int var2) 
  {
    return( __builtin_shl_fr2x16(var1, var2));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 shl_fr2x16(fract2x16, int);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for shifting right the var1 by var2 times giving 
 * the result of 16bit 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 shr_fr1x16_f(fract16, int);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 shr_fr1x16(fract16 var1, int var2) 
  {
    return((fract16) __builtin_shr_fr2x16(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 shr_fr1x16(fract16, int);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for shifting right the var1 by var2 times giving 
 * the result of 16bit 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 shr_fr2x16_f(fract2x16, int);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract2x16 shr_fr2x16(fract2x16 var1, int var2) 
  {
    return( __builtin_shr_fr2x16(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 shr_fr2x16(fract2x16, int);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for addition of the two fract32 varibles and giving 
 * output of 32 bit
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract32 add_fr1x32(fract32 var1, fract32 var2) 
  {
    return(__builtin_add_fr1x32(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 add_fr1x32(fract32, fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for subtraction of the two fract32 varibles and giving 
 * output of 32 bit
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract32 sub_fr1x32(fract32 var1, fract32 var2) 
  {
    return(__builtin_sub_fr1x32(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 sub_fr1x32(fract32, fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Multiplying two 16 bit fract16 numbers and giving 
 * result of 32 bit fract32 type
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract32 mult_fr1x32(fract16 var1, fract16 var2) 
  {
    return(__builtin_mult_fr1x32(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 mult_fr1x32(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 **  MAC32 operation  **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 mac_fr16_32_f( fract32 x, fract16 a, fract16 b);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract32 mac_fr16_32( fract32 acc,fract16 var1, fract16 var2)
  {
    return(__builtin_add_fr1x32(acc,__builtin_mult_fr1x32(var1,var2)));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 mac_fr16_32( fract32 , fract16 , fract16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 **  MAC40 operation  **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void mac_fr16_40_f(fract40 *acc, fract16 x, fract16 y);
  #ifdef __cplusplus
   }
  #endif

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void mac_fr16_40(fract40* , fract16 ,fract16 );
  #ifdef __cplusplus
   }
  #endif

#endif

/***
 **  MSU40 operation  **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void msu_fr16_40_f(fract40 *acc, fract16 x, fract16 y);
  #ifdef __cplusplus
   }
  #endif

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void msu_fr16_40(fract40* , fract16 ,fract16 );
  #ifdef __cplusplus
   }
  #endif

#endif

/***
 **  Shr40 operation  **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void shr_fr1x40_f(fract40* acc, int32 scale);
  #ifdef __cplusplus
   }
  #endif

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void shr_fr1x40(fract40* , int32 );
  #ifdef __cplusplus
   }
  #endif

#endif

/***
 **  Shl40 operation **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void shl_fr1x40_f(fract40* acc, int32 scale);
  #ifdef __cplusplus
   }
  #endif

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void shl_fr1x40(fract40* , int32 );
  #ifdef __cplusplus
   }
  #endif

#endif

/***
 **  Ashift40 operation **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void ashift_fr1x40_f(fract40* acc, int16 scale);
  #ifdef __cplusplus
   }
  #endif

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void ashift_fr1x40(fract40* acc, int16 scale);
  #ifdef __cplusplus
   }
  #endif

#endif

/***
 **  Other fract40 operations  **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void add_fr32_40_f(fract40* acc, fract32 x);
    void add_fr40_40_f(fract40* acc, fract40 acc2);
  #ifdef __cplusplus
   }
  #endif

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void add_fr32_40(fract40* acc, fract32 x);
    void add_fr40_40(fract40* acc, fract40 acc2);
  #ifdef __cplusplus
   }
  #endif

#endif

/***
 **  Norm fract40 operations  **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int32 norm_fr1x40_f(fract40 acc);
  #ifdef __cplusplus
   }
  #endif

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int32 norm_fr1x40(fract40 acc);
  #ifdef __cplusplus
   }
  #endif

#endif

/***
 **  fract data move operations  **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    float read_fr40_f(fract40 acc);
    fract32 fract40_to_fract32_f(fract40  acc);
    void fract16_to_fract40_f(fract16 v,fract40  *acc);
  #ifdef __cplusplus
   }
  #endif

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    float read_fr40(fract40 acc);
    fract32 fract40_to_fract32(fract40  acc);
    fract16 fract40_to_fract16(fract40  acc);
    void fract16_to_fract40(fract16 v,fract40  *acc);
    void fract32_to_fract40(fract32 v,fract40  *acc);
  #ifdef __cplusplus
   }
  #endif

#endif

/***
 **  complex fract40 operations  **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void mlt_fr16_40_f(fract40* , fract16 ,fract16 );
    void cmac_fr40_f(fract40 *acc1,fract40 *acc2,cmplx_fr16 v1,cmplx_fr16 v2);
    void conjmac_fr40_f(fract40 *acc1,fract40 *acc2,cmplx_fr16 v1,cmplx_fr16 v2);
    void conjmlt_fr40_f(fract40 *acc1,fract40 *acc2,cmplx_fr16 v1,cmplx_fr16 v2);
  #ifdef __cplusplus
   }
  #endif

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void mlt_fr16_40(fract40* , fract16 ,fract16 );
    void cmac_fr40(fract40 *acc1,fract40 *acc2,cmplx_fr16 v1,cmplx_fr16 v2);
    void conjmac_fr40(fract40 *acc1,fract40 *acc2,cmplx_fr16 v1,cmplx_fr16 v2);
    void conjmlt_fr40(fract40 *acc1,fract40 *acc2,cmplx_fr16 v1,cmplx_fr16 v2);
  #ifdef __cplusplus
   }
  #endif

#endif

/***
 * Crosscorrelation of two complex vectors and scaling operation 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 ccrosscorr_ashift_cfr16_f(cmplx_fr16* x,cmplx_fr16* y,int32 len, int16 shift);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS
  inline cmplx_fr16 ccrosscorr_ashift_cfr16 (cmplx_fr16* x,cmplx_fr16* y,int32 len, int16 shift)
    {
     cmplx_fr16 _out;
   asm(
     "A1 = A0 = 0 || R0 = [%1++] || R2 = [%2++];\n\t"
     "LSETUP(4,8) LC0 = %3;\n\t"
     "A1+=R0.H*R2.L,A0+=R0.L*R2.L;\n\t"
     "A1-=R0.L*R2.H,A0+=R0.H*R2.H || R0 = [%1++] || R2 = [%2++];\n\t"
     "A1 = ASHIFT A1 BY %4;\n\t"
     "A0 = ASHIFT A0 BY %4;\n\t"
     "%0.H=A1,%0.L=A0;\n\t"
   : "=d" (_out)
   : "#i" (x),"#i" (y),"a" (len), "l" (shift)
   : "R0","R2","A0","A1","LC0");
   return (_out);
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 ccrosscorr_ashift_cfr16(cmplx_fr16* x,cmplx_fr16* y,int32 len, int16 shift);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 ** Convolution of two complex vectors and scaling operation **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cconv_ashift_cfr16_f (cmplx_fr16* x,cmplx_fr16* y,int32 len, int16 shift);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS
  inline cmplx_fr16 cconv_ashift_cfr16 (cmplx_fr16* x,cmplx_fr16* y,int32 len, int16 shift)
    {
     cmplx_fr16 _out;
   asm(
     "A1 = A0 = 0 || R0 = [%1++] || R2 = [%2--];\n\t"
     "LSETUP(4,8) LC0 = %3;\n\t"
     "A1+=R0.H*R2.L,A0+=R0.L*R2.L;\n\t"
     "A1-=R0.L*R2.H,A0+=R0.H*R2.H || R0 = [%1++] || R2 = [%2--];\n\t"
     "A1 = ASHIFT A1 BY %4;\n\t"
     "A0 = ASHIFT A0 BY %4;\n\t"
     "%0.H=A1,%0.L=A0;\n\t"
   : "=d" (_out)
   : "#i" (x),"#i" (y),"a" (len), "l" (shift)
   : "R0","R2","A0","A1","LC0");
  return (_out);
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cconv_ashift_cfr16 (cmplx_fr16* x,cmplx_fr16* y,int32 len, int16 shift);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 ** Conjugate multiplication of two complex numbers and scaling operation **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 conjmlt_ashift_cfr16_f (cmplx_fr16 x,cmplx_fr16 y, int16 shift);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS
  inline cmplx_fr16 conjmlt_ashift_cfr16 (cmplx_fr16 x,cmplx_fr16 y, int16 shift)
    {
     cmplx_fr16 _out;
   asm( 
        "A1 =%1.H*%2.L,A0 =%1.L*%2.L;\n\t"
        "A1-=%1.L*%2.H,A0+=%1.H*%2.H;\n\t"
        "A1 = ASHIFT A1 BY %3;\n\t"
        "A0 = ASHIFT A0 BY %3;\n\t"
        "%0.H=A1,%0.L=A0;\n\t"
      : "=d" (_out)
      : "d" (x),"d" (y), "l" (shift)
      : "A0","A1");
   return (_out);
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 conjmlt_ashift_cfr16 (cmplx_fr16 x,cmplx_fr16 y, int16 shift);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 ** Complex square of two complex numbers and scaling operation **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 csqu_ashift_fr32_f (cmplx_fr16 x,int16 shift);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract32 csqu_ashift_fr32 (cmplx_fr16 x,int16 shift)
    {
     fract32 _out;
   asm(
       "A0 =%1.L*%1.L;\n\t"
       "A0+=%1.H*%1.H;\n\t"
       "A0 = ASHIFT A0 BY %2;\n\t"
       "%0 = A0;\n\t"
     : "=E" (_out)
     : "d" (x),"l" (shift)
     : "A0");
   return (_out);
  };
#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 csqu_ashift_fr32 (cmplx_fr16 x,int16 shift);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 ** Vector MAC32 operation  **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 vmac_fr16_32_f(fract16* , fract16* , int32 , int32 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract32 vmac_fr16_32 (fract16* x, fract16* y, int32 len, int32 scale)
    {
  	fract32 _out;
  	asm("%4 = -%4(V) || R0.L = W[%1++];\n\t" 
  	"A0 =0 || R0.H = W[%2++];\n\t"
  	"LSETUP(4,4) LC0 = %3;\n\t"
  	"A0+=R0.L*R0.H(W32)|| R0.L = W[%1++] || R0.H = W[%2++];\n\t"
  	"A0 = ASHIFT A0 BY %4.L;\n\t"
  	"%0 = A0.W;\n\t"
	: "=E" (_out)
	: "#i" (x) , "#i" (y),"a" (len), "d" (scale)
	:"R0","A0","LC0");
	return (_out);
     };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 vmac_fr16_32(fract16* , fract16* , int32 , int32 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 ** Vector MAC40 operation   **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 vmac_fr16_40_f(fract16* , fract16* , int32 , int32 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract32 vmac_fr16_40 (fract16* x, fract16* y, int32 len, int32 scale)
    {
  	fract32 _out;
  	asm("%4 = -%4(V) || R0.L = W[%1++];\n\t" 
  	"A0 =0 || R0.H = W[%2++];\n\t"
  	"LSETUP(4,4) LC0 = %3;\n\t"
  	"A0+=R0.L*R0.H || R0.L = W[%1++] || R0.H = W[%2++];\n\t"
  	"A0 = ASHIFT A0 BY %4.L;\n\t"
  	"%0 = A0.W;\n\t"
	: "=E" (_out)
	: "#i" (x) , "#i" (y),"a" (len), "d" (scale)
	:"R0","A0","LC0");
	return (_out);
     };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 vmac_fr16_40(fract16* , fract16* , int32 , int32 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 ** Vector cmls40 operation **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 vmac_cmls_40_f (cmplx_fr16* ,fract16* , int32 , int32 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline cmplx_fr16 vmac_cmls_40 (cmplx_fr16* x,fract16* y, int32 len, int32 scale)
    {
     cmplx_fr16 _out;
     asm("R1 = -%4(V);\n\t"
     "A1 = A0 = 0 || R0 = [%1++] || R2.L = W[%2++];\n\t"
     "LSETUP(4,4) LC0 = %3;\n\t"
     "A1+=R0.H*R2.L,A0+=R0.L*R2.L || R0 = [%1++] || R2.L = W[%2++];\n\t"
     "A1 = ASHIFT A1 BY R1.L;\n\t"
     "A0 = ASHIFT A0 BY R1.L;\n\t"
     "%0.H=A1,%0.L=A0(T);\n\t"
  : "=d" (_out)
  : "#i" (x),"#i" (y),"a" (len), "d" (scale)
  :"R0","R1","R2","A0","A1","LC0");
  return (_out);
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 vmac_cmls_40 (cmplx_fr16* ,fract16* , int32 , int32 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif 

/***
 ** Vector csqu40 operation **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 vmac_csqu_40_f(cmplx_fr16* , int32 , int32 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract32 vmac_csqu_40 (cmplx_fr16* x, int32 len, int32 scale)
    {
    fract32 _out;
    asm("R1 = -%3(V);\n\t"
    "A1 = A0 = 0 || R0 = [%1++];\n\t"
    "LSETUP(4,4) LC0 = %2;\n\t"
    "A1+=R0.H*R0.H,A0+=R0.L*R0.L || R0 = [%1++];\n\t"
    "A0 += A1;\n\t"
    "A0 = ASHIFT A0 BY R1.L;\n\t"
    "%0 = A0.W;\n\t"
    : "=E" (_out)
    : "#i" (x),"a" (len), "d" (scale)
    :"R0","R1","A0","LC0");
  
    return (_out);
    };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 vmac_csqu_40(cmplx_fr16* , int32 , int32 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif 

/***
 * This is for finding maximum of given two fract32 varibles and
 * giving output of 32 bit
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract32 max_fr1x32(fract32 var1, fract32 var2) 
  {
    return(__builtin_max_fr1x32(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 max_fr1x32(fract32, fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for finding the absolute value of given fract32 variable
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract32 abs_fr1x32(fract32 var1) 
  {
    return(__builtin_abs_fr1x32(var1));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 abs_fr1x32(fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for finding the minimum value of given two fract32 variables
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract32 min_fr1x32(fract32 var1, fract32 var2) 
  {
    return(__builtin_min_fr1x32(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 min_fr1x32(fract32, fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for finding the 2's complement(negetive)value of 
 * given fract32 variable
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS
  
  inline fract32 negate_fr1x32(fract32 var1) 
  {
    return(__builtin_sub_fr1x32(0,var1));
   };
   
#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 negate_fr1x32(fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for shifting left the var1 of 32  bit by var2 times 
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS
  
  inline fract32 shl_fr1x32(fract32 var1, int var2) 
  {
    return(__builtin_shl_fr1x32(var1, var2));
   };
   
#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 shl_fr1x32(fract32, int);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for shifting right the var1 of 32bit by var2 times, giving 
 * the result of 32bit 
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract32 shr_fr1x32(fract32 var1, int var2) 
  {
    return(__builtin_shr_fr1x32(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 shr_fr1x32(fract32, int);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * this is for saturating the  32bit value and returning result of 16bit 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 sat_fr1x16_f(fract32);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 sat_fr1x16(fract32 var1) 
  {
    return ((fract16)(__builtin_shl_fr1x32(var1,16)>>16));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 sat_fr1x16(fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 ** Bit setting for biased rounding  **
 ***/

#ifdef INTRINSICS 

  inline void  set_rnd_mode_biased()
  {
	asm("R0=astat;bitset(r0,8);astat=r0;": : :"R0");
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
// Eddy 2008.06.23 Remove warning
    void   set_rnd_mode_biased(void);
  #ifdef __cplusplus
   }
  #endif

#endif

/***
 ** Bit setting for unbiased rounding  **
 ***/

#ifdef INTRINSICS 

  inline void   set_rnd_mode_unbiased()
  {
	asm("R0=astat;bitclr(r0,8);astat=r0;": : :"R0");
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
// Eddy 2008.06.23 Remove warning
    void   set_rnd_mode_unbiased(void);
  #ifdef __cplusplus
   }
  #endif

#endif

/***
 * This is for rounding the var1 of 32bit, giving the result of 16bit 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 round_fr1x32_f(fract32);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS
  inline fract16 round_fr1x32(fract32 var1) 
  {
/*  return(__builtin_round_fr1x32(var1)); */

  fract16 var_out;
  asm("a0=%1;\n\t"
	"%0.L=a0;\n"
	:"=E" (var_out)
	:"d" (var1)
	:"a0");
	return var_out;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 round_fr1x32(fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for normalizing the lower word of var1 of 32bit, 
 * giving the result of 16bit 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 norm_fr1x16_f(fract32);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline int16 norm_fr1x16(fract32 var1) 
  {
    return((int16) __builtin_norm_fr1x16(var1));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 norm_fr1x16(fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for normalizing the var1 of 32bit, 
 * giving the result of 16bit 
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline int16 norm_fr1x32(fract32 var1) 
  {
    return((int16) __builtin_norm_fr1x32(var1));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 norm_fr1x32(fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * This is for calculating the power of 2 for the given integer value 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 norm_i1x32_f(fract32);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline int16 norm_i1x32(fract32 var1) 
  {
  int16 exp;
  exp = __builtin_norm_fr1x32(var1);
  return ( ( var1 > 0 ) ? ( 30 - exp ): ( 31 - exp ) );
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 norm_i1x32(fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * 
 * The following functions have included as an intrinsics (builtin)
 * 
 * These are the functions which are included after correlating with
 * the web page, blackfin_coding
 *
 ***/

/***
 * Extracting the higher value of fract32 number  
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract16 extract_hi_fr1x32(fract32 var1) 
  {
    return((fract16) __builtin_extract_hi(var1));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 extract_hi_fr1x32(fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/*** 
 * Extracting the higher value of fract2x16 number  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 extract_hi_fr2x16_f(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS
  inline fract16 extract_hi_fr2x16(fract2x16 var1)
  {
    return((fract16) __builtin_extract_hi(var1));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 extract_hi_fr2x16(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Extracting the lower value of fract32 number
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract16 extract_lo_fr1x32(fract32 var1) 
  {
    return( (fract16) __builtin_extract_lo(var1));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 extract_lo_fr1x32(fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Extracting the lower value of fract2x16 number  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 extract_lo_fr2x16_f(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS
  
  inline fract16 extract_lo_fr2x16(fract2x16 var1)
  {
    return((fract16) __builtin_extract_lo(var1));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 extract_lo_fr2x16(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Composing the two fract16 values and giving 
 * result of fract32 
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS
  
  inline fract32 compose_fr1x32(fract16 var1, fract16 var2) 
  {
    return(__builtin_compose_2x16(var1,var2));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 compose_fr1x32(fract16,fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Composing the two fract16 values and giving 
 * result of fract2x16 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 compose_fr2x16_f(fract16,fract16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract2x16 compose_fr2x16(fract16 var1, fract16 var2)
  {
    return(__builtin_compose_2x16(var1,var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract2x16 compose_fr2x16(fract16,fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Extracting the imaginery value of cmplx fract number  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 extract_im_f(cmplx_fr16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 extract_im(cmplx_fr16 var1) 
  {
    return((fract16) __builtin_extract_hi(var1));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 extract_im(cmplx_fr16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Extracting the real value of cmplx fract number  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 extract_re_f(cmplx_fr16 a);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 extract_re(fract32 var1) 
  {
    return((fract16) __builtin_extract_lo(var1));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 extract_re(fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * compising the cmplx fract number  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 compose_cfr16_f(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS
  
  inline cmplx_fr16 compose_cfr16(fract16 real, fract16 imag) 
  {
    return(__builtin_compose_2x16(imag, real));
   };
#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 compose_cfr16(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Sum of hi, lo of the fract2x16 number  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 sum_fr2x16_f(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS
  inline fract16 sum_fr2x16(fract2x16 var1) 
  {
    return((fract16) __builtin_sum_fr2x16(var1));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 sum_fr2x16(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Difference of hi, lo of the fract2x16 number  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 dif_hl_fr2x16_f(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS
  inline fract16 dif_hl_fr2x16(fract2x16 var1) 
  {
   fract16  _out;
   asm("%0.L=%1.H - %1.L(s);"  
      :"=d" (_out)
      :"d" (var1)
      :); 	  	
   return _out;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 dif_hl_fr2x16(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Difference of lo, hi of the fract2x16 number  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 dif_lh_fr2x16_f(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 dif_lh_fr2x16(fract2x16 var1) 
  {
   fract16  _out;
   asm("%0.L=%1.L - %1.H(s);"  
      :"=d" (_out)
      :"d" (var1)
      :); 	  	
   return _out;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 dif_lh_fr2x16(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Interger Sum of lo, hi of the fract2x16 number  
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract16 sum_i2x16(fract2x16 var1) 
  {
    return((fract16) __builtin_sum_i2x16(var1));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 sum_i2x16(fract2x16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * 
 * The following functions have included as an intrinsics (builtin)
 * in the recent version. 
 *
 * These are the functions which are only works on frio environment
 *
 * Their prototypes are in the deffrio.h file, since they were misc
 * frio-only builtins that didn't fit anywhere else
 *
 ***/

/***
 * Counting number of ones in a given input  
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS
  
  inline fract16 ones(fract32 var1) 
  {
    return((fract16) __builtin_ones(var1));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 ones(fract32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Expadj of two inputs  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 expadj1x16_f(fract16, int16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline int16 expadj1x16(fract16 var1, int16 var2) 
  {
    return((int16) __builtin_expadj1x16(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 expadj1x16(fract16, int16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Expadj32 of two inputs  
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS
  
  inline int16 expadj1x32(fract32 var1, int16 var2) 
  {
    return((int16) __builtin_expadj1x32(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 expadj1x32(fract32, int16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Expadj2x16 of two inputs  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 expadj2x16_f(cmplx_fr16 in1, int16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline int16 expadj2x16(fract32 var1, int16 var2) 
  {
    return((int16) __builtin_expadj2x16(var1, var2));
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 expadj2x16(fract32, int16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 *  lvitmax1x16 function   
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void lvitmax1x16_f(fract2x16, int32,fract16*,int32*);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline void lvitmax1x16(fract2x16 var1, int32 var2, fract16* var3, int32* var4) 
  {
  fract32 a;
   a = __builtin_lvitmax1x16res1(var1, var2);
   *var4 = __builtin_vitmaxres1x16(a);
   *var3 = a;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void lvitmax1x16(fract2x16, int32,fract16*,int32*);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 *  rvitmax1x16 function   
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void rvitmax1x16_f(fract2x16, int32,fract16*,int32*);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline void rvitmax1x16(fract2x16 var1, int32 var2, fract16* var3, int32* var4) 
  {
  fract32 a;
   a = __builtin_rvitmax1x16res1(var1, var2);
   *var4 = __builtin_vitmaxres1x16(a);
   *var3 = a;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void rvitmax1x16(fract2x16, int32,fract16*,int32*);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 *  lvitmax2x16 function   
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void lvitmax2x16_f(fract2x16, fract2x16,int32,fract2x16*,int32*);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline void lvitmax2x16(fract2x16 var1, fract2x16 var2,int32 var3, fract2x16* var4, int32* var5) 
  {
   fract2x16 a;
   a = __builtin_lvitmax2x16res1(var1, var2, var3);
   *var5 = __builtin_vitmaxres2x16(a);
   *var4 = a;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void lvitmax2x16(fract2x16, fract2x16,int32,fract2x16*,int32*);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 *  rvitmax2x16 function   
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void rvitmax2x16_f(fract2x16, fract2x16,int32,fract2x16*,int32*);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline void rvitmax2x16(fract2x16 var1, fract2x16 var2,int32 var3, fract2x16* var4, int32* var5) 
  {
   fract2x16 a;
    a = __builtin_rvitmax2x16res1(var1, var2, var3);
    *var5 = __builtin_vitmaxres2x16(a);
    *var4 = a;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void rvitmax2x16(fract2x16, fract2x16,int32,fract2x16*,int32*);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 ** Functions for bit interleaving  **
 ***/

/***
 * Multiplexing through right 
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline void bitmux_shr(fract32 *var1, fract32 *var2, fract32 *RegA0,fract32 *RegA0_Hi) 
  {
   asm("%2.X = %3.L;\n\t"
       "BITMUX(%0,%1,%2)(ASR);\n\t"
       "%3 = %2.X;\n\t"
      :"+d" (*var1),"+d" (*var2),"+e"(*RegA0),"+d"(*RegA0_Hi)
      :
      :);
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void bitmux_shr(fract32* , fract32* , fract32* ,fract32*);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Multiplexing through left  
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline void bitmux_shl(fract32 *var1, fract32 *var2, fract32 *RegA0,fract32 *RegA0_Hi) 
  {
   asm("%2.X = %3.L;\n\t"
       "BITMUX(%0,%1,%2)(ASL);\n\t"
       "%3 = %2.X;\n\t"
       :"+d" (*var1),"+d" (*var2),"+e"(*RegA0),"+d"(*RegA0_Hi)
       :
       :);
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    void bitmux_shl(fract32* , fract32* , fract32* ,fract32*);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * 
 * The following functions for complex numbers, some do not have an intrinsic
 * (built_in)version. They are implemented in Frio assembly.
 *
 * It is expected that in a later revision of the compiler
 * they will be implemented as intrinsics.
 *
 ***/

/***
 * Cadd_fr16 function  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cadd_fr16_f (cmplx_fr16 _a, cmplx_fr16 _b);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline cmplx_fr16 cadd_fr16(cmplx_fr16 _a, cmplx_fr16 _b)
  {
    return(__builtin_cmplx_add(_a,_b));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cadd_fr16(cmplx_fr16 , cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Csub_fr16 function  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 csub_fr16_f (cmplx_fr16 , cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS
  inline  cmplx_fr16 csub_fr16 (cmplx_fr16 _a, cmplx_fr16 _b)
  {
     return(__builtin_cmplx_sub(_a,_b));
   }; 
#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 csub_fr16 (cmplx_fr16 , cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Csub_fr16 function  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16  cmlt_fr16_f(cmplx_fr16  , cmplx_fr16  );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline  cmplx_fr16 cmlt_fr16(cmplx_fr16 _a, cmplx_fr16 _b)
  {
    return( __builtin_cmplx_mul(_a,_b));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16  cmlt_fr16(cmplx_fr16  , cmplx_fr16  );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Complex multiply and accumulator function 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cmac_fr16_f(cmplx_fr16, cmplx_fr16, cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline cmplx_fr16  cmac_fr16(cmplx_fr16  _sum, cmplx_fr16  _x, cmplx_fr16  _y)
  {
    return(__builtin_cmplx_add(_sum,(__builtin_cmplx_mul(_x,_y))));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cmac_fr16(cmplx_fr16, cmplx_fr16, cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Complex multiply and subtractor function 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cmsu_fr16_f(cmplx_fr16 , cmplx_fr16 , cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline cmplx_fr16  cmsu_fr16(cmplx_fr16  _sum, cmplx_fr16  _x, cmplx_fr16  _y)
  {
    return(__builtin_cmplx_sub(_sum,(__builtin_cmplx_mul(_x,_y))));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cmsu_fr16(cmplx_fr16 , cmplx_fr16 , cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 *
 * These functions has no built in functions, they will be 
 * implemented as intrinsics in the coming versions.
 *
 ***/


/***
 * complex conjugate of a given variable 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16  conj_fr16_f (cmplx_fr16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS
  
  inline cmplx_fr16  conj_fr16 (cmplx_fr16 _a)
  {
	cmplx_fr16  _out,_b=0;
  	asm("%0=%2 -|+ %1(s);"  
	    :"=d" (_out)
	    :"d" (_a), "d" (_b)
	    :); 	  	
	return _out;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16  conj_fr16 (cmplx_fr16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * complex conjugate multiplication i.e _a * (_b*) of a given variables 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16  conjmlt_fr16_f(cmplx_fr16  , cmplx_fr16  );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline  cmplx_fr16 conjmlt_fr16(cmplx_fr16 _a, cmplx_fr16 _b)
  {
     return(__builtin_cmplx_mul(_a,conj_fr16(_b)));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16  conjmlt_fr16(cmplx_fr16  , cmplx_fr16  );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Complex conjugate multiply and accumulator function 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 conjmac_fr16_f(cmplx_fr16, cmplx_fr16, cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline cmplx_fr16  conjmac_fr16(cmplx_fr16  _sum, cmplx_fr16  _x, cmplx_fr16  _y)
  {
    return(__builtin_cmplx_add(_sum,(__builtin_cmplx_mul(_x, conj_fr16(_y)))));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 conjmac_fr16(cmplx_fr16, cmplx_fr16, cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/*** 
 * Complex left shift of real and imaginary part by given bits with saturation 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cshl_fr16_f(cmplx_fr16 , int );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline cmplx_fr16 cshl_fr16(cmplx_fr16 _x, int _b)
  {
    return((cmplx_fr16) __builtin_shl_fr2x16(_x,_b));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cshl_fr16(cmplx_fr16 , int );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Complex right shift of real and imaginary part by given bits  
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cshr_fr16_f(cmplx_fr16 , int );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline cmplx_fr16 cshr_fr16(cmplx_fr16 _x, int _b)
  {
    return((cmplx_fr16) __builtin_shr_fr2x16(_x,_b));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cshr_fr16(cmplx_fr16 , int );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Complex exponent adjust of real and imaginery values with gigen exp value 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 cexpadj_fr16_f(cmplx_fr16 in1, int16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline int16 cexpadj_fr16(cmplx_fr16 var1, int16 var2) 
  {
    return((int16) __builtin_expadj2x16(var1, var2));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 cexpadj_fr16(cmplx_fr16, int16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * vector negate of complex variable 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cnegate_fr16_f(cmplx_fr16 in1);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline cmplx_fr16 cnegate_fr16(cmplx_fr16 var1) 
  {
    return((cmplx_fr16) __builtin_negate_fr2x16(var1));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cnegate_fr16(cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Complex square function with 16bit return
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 csqu_fr16_f(cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 csqu_fr16 (cmplx_fr16  _c)
  {
  fract16 _out;
   asm("%0.h=%1.H*%1.H, %0.L=%1.L*%1.L(T);\n\t"
	"%0.L= %0.L+%0.H (s);\n"
	:"=d" (_out)
	:"d" (_c)
	:);
  return _out;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 csqu_fr16(cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Complex square function with 32bit return
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline fract32 csqu_fr32(cmplx_fr16 c)
  {
  fract32 out;
   asm("R0=%1.h*%1.H, R1=%1.L*%1.L;\n\t"
	"%0= R0+R1(s);\n"
	:"=d" (out)
	:"d" (c)
	:"R0", "R1");
  return out;
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 csqu_fr32(cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Complex distance function with 16bit return
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 cdst_fr16_f (cmplx_fr16 , cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 cdst_fr16 (cmplx_fr16 _x, cmplx_fr16 _y)
  {
   fract16 _out;
    asm("R4 = %1 -|- %2 (s);\n\t"
  	"R4.H=R4.H*R4.H, R4.L=R4.L*R4.L(T);\n\t"
  	"%0.L= R4.L+R4.H (s);\n"
  	:"=d" (_out)
  	"d" (_x), "d" (_y)
  	:"R4");
   return _out;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 cdst_fr16 (cmplx_fr16 , cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Complex distance function with 32bit return
 ***/

#ifdef FLTPNT
  
#else

#ifdef INTRINSICS

  inline fract32 cdst_fr32(cmplx_fr16 _x, cmplx_fr16 _y)
  {
   fract32 L_out;
    asm("R4 = %1 -|- %2 (s);\n\t"
	"R4=R4.H*R4.H,R5=R4.L*R4.L;\n\t"
  	"%0= R4+ R5(s);\n"
  	:"=d" (L_out)
  	"d" (_x), "d" (_y)
  	:"R4","R5");
   return L_out;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract32 cdst_fr32(cmplx_fr16 , cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif
 
/***
 * Complex multiply with fract value
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cmls_fr16_f(cmplx_fr16 , fract16 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline cmplx_fr16 cmls_fr16 (cmplx_fr16 _a, fract16 _b)
  {
   cmplx_fr16  _out;
    return((cmplx_fr16) __builtin_compose_2x16(
           (fract16) __builtin_multr_fr2x16(__builtin_extract_hi(_a), _b), 
           (fract16) __builtin_multr_fr2x16(__builtin_extract_lo(_a), _b)));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cmls_fr16 (cmplx_fr16 , fract16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Complex integer multiply with fract value
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline cmplx_fr16 cmls_i2x16(cmplx_fr16 _x, fract16 _b)
  {
   cmplx_fr16  _out;
    asm("%0.H = %1.H*%2.L, %0.L= %1.L*%2.L(IS);"
	:"=d" (_out)
	:"d"(_x), "d"(_b)
	:);
   return _out;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cmls_i2x16(cmplx_fr16 , fract16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * Complex integer multiply 
 ***/

#ifdef FLTPNT

#else

#ifdef INTRINSICS

  inline cmplx_fr16 cmlt_i16 (cmplx_fr16 _x, cmplx_fr16 _y)
  {
   cmplx_fr16  _out;
    asm("a1= %1.H*%2.L, a0= %1.L*%2.L(IS);\n\t" 
	"%0.H=(a1+= %1.L*%2.H), %0.L=(a0-= %1.H*%2.H)(IS);\n\t" 
	:"=d" (_out)
	:"d"(_x), "d"(_y)
	:"a0","a1");
   return _out;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cmlt_i16 (cmplx_fr16 , cmplx_fr16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * 
 * The following functions do not have an intrinsic (builtin)
 * version. They are implemented in Frio assembly.
 *
 ***/

/***
 * signedinteger Multiplication of two 16 bit numbers and giving 
 * result of 16 bit 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 mult_i1x16_f(int16, int16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS
  
  inline int16 mult_i1x16(int16 var1, int16 var2) 
  {
   int16 var_out;
	asm(" %0=%1.L * %2.L(IS);\n\t"
		"%0=%0.L;\n"
		:"=d" (var_out)
		:"d" (var1), "d" (var2)
		:);
   return var_out;
   };
#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    int16 mult_i1x16(int16, int16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * DIVS_16 implementation with asm which is accurate 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 divs_fr1x16_f(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 divs_fr1x16(fract16 _a, fract16 _b)
  {
   fract16 _out;

    asm( 
  	"P0 = 15;\n\t"		        /* TO PERFORM DIVQ 15 TIMES */
  	"R0 = ABS %1(V);\n\t"           /* ABS VALUE OF NUM.  */
  	"R1 = ABS %2(V);\n\t"           /* ABS VALUE OF DENOM. */
  	"R0 <<= 16;\n\t"                /* ARRANGING FOR PROPER DIVISION.   */
  	"R5 = %1 ^ %2;\n\t"             /* GET SIGN INFORMATION */
  	"R5 = R5.L (X);\n\t"            /* SIGN EXTEND */
        "DIVS (R0, R1);\n\t"	        /* DIVS IS  CALLED ONCE TO CLEAR AQ FLAG */
  	"LSETUP(4,4)LC0=P0;\n\t"
  	"DIVQ (R0, R1);\n\t"	        /* DIVQ IS DONE FOR 15 TIMES */
  	"R1 = -R0(V);\n\t"              /* NEGATED RESULT */	
  	"CC = R5 < 0;\n\t"
  	"IF CC R0 = R1;\n\t"		/* R0 = NEGATED RESULT IF RESULT SHOULD BE NEGATIVE */
        "%0 = R0.L (X);\n\t" 
    	:"=d" (_out)
  	:"d" (_a) , "d" (_b)
  	:"R0","R1","R5","P0","CC","LC0");
  
   return (_out);
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 divs_fr1x16(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * DIVS_16 implementation with built-ins divs and divq
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 divs_bi_fr1x16_f(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 divs_bi_fr1x16(fract16 _a, fract16 _b)
  {
    int x = (int)_a;
    int i;

    x <<= 16;
    x = __builtin_divs(x, _b);
    for (i=0; i<15; i++) {
      x = __builtin_divq(x, _b);
    }

    return x;
  };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 divs_bi_fr1x16(fract16, fract16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 * division of complex nuber with real value 
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cdivr_fr16_f(cmplx_fr16, fract16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline cmplx_fr16 cdivr_fr16(cmplx_fr16 _a, fract16 _b)
  {
   fract16 rel,img;  
    rel = divs_fr1x16(extract_lo_fr1x32( _a ), _b);
    img = divs_fr1x16(extract_hi_fr1x32( _a ), _b);
   return (compose_fr1x32(img,rel));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cdivr_fr16(cmplx_fr16 , fract16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 ** complex division **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cdiv_fr16_f(cmplx_fr16, cmplx_fr16);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline cmplx_fr16 cdiv_fr16(cmplx_fr16 _a, cmplx_fr16 _b)
  {
    return cdivr_fr16(conjmlt_fr16(_a,_b),csqu_fr16(_b));
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    cmplx_fr16 cdiv_fr16(cmplx_fr16, cmplx_fr16);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 ** SQRT_fr1x16 implementation **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 sqrt_fr1x16_f(fract16 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 sqrt_fr1x16(fract16 _x)
  {
   fract16 _out;
   fract16 sqrtcoef0[5]= {0x2D41,0xD2CE,0xE7E8,0xF848,0xAC7C}; 
   fract16 sqrtcoef1[5]= {0x2D42,0x2D31,0xEA5D,0x1021,0xF89E}; 
    asm( 
	"CC = %1 <= 0;\n\t"   		  /* CHECK  FOR _x <= 0   */
	"IF CC JUMP (96);\n\t"            /* SQRT_RET_ZERO;\n\t"	  */            
	"R6 = 0X2000;\n\t"
	"%0 = 1;\n\t"                     	  
	"R7 = 0X4000;\n"		  /* INITIALISE R3 = 0X2000  */

/* "SQRT_AGAIN:" */
	"R5 = %1;\n\t"
	" CC = %1 < R6;\n\t" 		  /* WHILE _x < 0X2000 DO  */
	"IF !CC JUMP (10) ;\n\t"           /* SQRT_BRANCH0;\n\t" */
	                                  /* IF R0 > 0X2000, BRANCH LOOP2 LOCATION */
	"R5 = R5 << 2;\n\t"     		  /* MULTIPLY 4 */
	"%0 += -1;\n\t"	  		  /* DIVIDE BY 2 LATER  */
	"JUMP (-10) ;\n\t"                 /* SQRT_AGAIN;\n"   JUMP TO AGAIN LOCATION */

/* "SQRT_BRANCH0:" */

	" R5 = R5 - R7;\n\t" 		  /* R0 = R0(_x) - 0X4000  */
	"CC = R5 < 0;\n\t" 		  /* IF R0(_x) < 0 */
	"IF CC %2 = %3;\n\t"              /* IF TRUE, USE COEFFICIENT 0 */
     	"R6 = -R5;\n\t"                         
	"IF CC R5 = R6; \n\t"    	  /* IF TRUE, NEGATE RESULT */
	"P5 = 2;\n\t"	  		  /* LOOP COUNTER */
	"R6 = PACK(R5.H,R5.L) || R7 = W[%2++];\n\t"			
		                          /* SAVE R0(_x) IN R3, FETCH FIRST COEFFICIENT */
	"%0 += -16;\n\t"                  /* R0 = R2 - 16 TO DIVIDE LATER  */
	"R7 <<= 16;\n\t"		  /* ARRANGE IN 32 BIT FORMAT */
	"A1 = R7 || R7 = W[%2++];\n\t"	  /* INITIALISE A1 WITH FIRST COEFFICIENT */
					  /* GET NEXT COEFFICIENT */
    	"LSETUP(4 ,20) LC0 = P5;\n"	  /* SET A LOOP FOR LOOP COUNTER VALUE = 2 */
	" R5.H = R5.L * R6.L;\n\t"        /* CALCULATES EVEN POWERS OF X AND TAKES NEXT COEFFICIENT */
	"A1 += R6.L * R7.L || R7 = W[%2++];\n\t"
	"R6.L = R5.L * R5.H;\n"           /* CALCULATES ODD POWERS OF X AND TAKES NEXT COEFFICIENT */
	"A1 += R5.H * R7.L || R7 = W[%2++];\n\t"
	"A1 = ASHIFT A1 BY %0.L;\n\t"     /* SAVE IN FRACT16   */
      "%0 = A1;\n\t"
	"JUMP (4);\n\t"                   /* SQRT_EXIT;\n" */			
/* "SQRT_RET_ZERO:" */
	"%0 = 0;\n"		 	  /* RETURN 0  */
/* "SQRT_EXIT : " */
	
	: "=O" (_out)
	: "d" (_x), "a" (&sqrtcoef1[0]),"a" (&sqrtcoef0[0])
	: "r6","r7","r5","p5","a1","CC","LC0");	

   return (_out);
   };
	
#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 sqrt_fr1x16(fract16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 ** ISQRT implementation **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    uint32 isqrt_f(uint32);
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline uint32 isqrt(uint32 val)
  {
   unsigned long est;
    asm("%0 = 0;\n\t"			/* est =0 */
	" R7 = 0X8000(Z); P0 = 16;\n\t"	/* bit = 1<<15  */
	" LSETUP(4,16)LC0=P0;\n\t"      /* for  (i = 0; i <16; i++)*/
	/* "START:" */
	"%0= %0 ^R7;\n\t"               /* est ^= bit;  */
	"R6=%0;"
	"R6 *=%0;\n\t"			/* est * est */
	"R5= %0^R7;\n\t"
	"CC = %1 < R6(IU);\n\t"		/* if (est * est > val ) */
	"IF CC %0=R5;\n\t"		/* est ^= bit;  */ 
	/* "END:" */
	"R7>>=1;\n\t"			/*  (bit >>=1) */
	: "=d" (est)
	:"d" (val)
	:"r5","r6","r7","p0","CC","LC0");
   return est;
   };

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    uint32 isqrt(uint32);
  #ifdef __cplusplus
   }
  #endif

#endif
#endif

/***
 ** Finding the COS of given radians input **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 cos_fr16_rad_f( fract16 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 cos_fr16_rad( fract16 xrad )
  {  
   fract16 out;
     asm("R4.L = %1.L * %1.L(T);\n\t"
	 "R5.H = 0X4000;\n\t"                       /* 1/2 in fract16 form */
	 "R4.H = R4.L * R4.L(T);\n\t"   
         "R5.L = 0X0555;\n\t"                       /* 1/24 in fract16 form */
         "R4.H = R5.L * R4.H, R4.L = R5.H*R4.L(T);\n\t"
         "R5.L = 0X7FFF;\n\t"
         "R4.H = R4.H - R4.L (S);\n\t"
         "%0.L = R4.H + R5.L(S);\n\t"
         : "=d" (out)
         : "d" (xrad)
         : "R4","R5");
   return out;
   };        

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 cos_fr16_rad( fract16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif   

/***
 ** Finding the COS(x*pi/2) of given input x **
 ***/

#ifdef FLTPNT

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 cos_fr16_f( fract16 );
  #ifdef __cplusplus
   }
  #endif

#else

#ifdef INTRINSICS

  inline fract16 cos_fr16( fract16 x )
  {  
   fract16 coscoef[5] ={ 0x6480,0x0059,0xD54D,0x0252,0x0388};
   fract16 out;
    asm(
    	"R0 = ABS %1;"                /* GET THE ABSOLUTE VALUE OF INPUT  */
	"R2 = 32767(X);"	      /* INITIALISE R3 = 1.0 */	
	"P1 = 2;"    		      /* SET LOOP COUNTER VALUE = 5 */
	"R0 = R2-R0;"		      /* R0 = 1 - R0 */
	"R1 = R0;"		      /* COPY RO TO R1 REG (Y = X) */
	"A0 = 0 || R2 = W[%2++];"     /* CLEAR ACCUMALATOR = 0 AND GET FIRST COEFFICIENT */
	"LSETUP(4,20) LC0 = P1;" /* SET A LOOP FOR LOOP COUNTER VALUE = 5*/
	    "R0.H = R0.L * R1.L;"     /* EVEN POWERS OF X  */
            "A0 += R1.L * R2.L || R2 = W[%2++];"
            "R1.L = R0.L * R0.H;"     /* ODD POWERS OF X */
	    "A0 += R0.H * R2.L || R2 = W[%2++];"
        "R2=(A0 += R1.L * R2.L);"
	"R0 = 0x7fff;"		      /* INITIALISE R3 TO 0X7FFF */ 	
	"R2 = R2 >>> 15;"             /* SAVE IN FRACT16 */  
	"CC = R2 < 0;"		      /* IF R2 < 0 */
	"IF CC R2 = R0;"	      /* IF TRUE THEN INITIALISE R2 = R3 */  
        "%0 = R2.L(X);"		      /* COPY OUTPUT VALUE TO R0 */
        :"=d" (out)
        :"d" (x),"a" (&coscoef[0])
        :"r0","r1","r2","p1","a0","LC0");		
   return out;
   };        

#else

  #ifdef __cplusplus
   extern "C"
   { 
  #endif
    fract16 cos_fr16( fract16 );
  #ifdef __cplusplus
   }
  #endif

#endif
#endif   

/***
 * Below code is added to get the capabilities of math.h file 
 ***/


    #ifdef __cplusplus
     extern "C"
     { 
    #endif
        double floor (double _x);
        float floorf (float _x);
        double pow (double _x, double _exp);
        float powf (float _x, float _exp);
        double frexp (double _x, int * _exp);
        float frexpf (float _x, int * _exp);
        double modf (double _x, double * _integral);
        float modff (float _x, float * _integral);
        double sqrt (double _x);
        float sqrtf (float _x);
        double exp (double _x);
        float expf (float _x);
        double ceil (double _x);
        float ceilf (float _x);
        double log (double _x);
    #ifdef __cplusplus
    }
    #endif



    #ifdef __cplusplus
     extern "C"
     { 
    #endif
      double cos(double);
      double sin(double);
    #ifdef __cplusplus
    }
    #endif



/* 
 * This set of macros maps the library functions used in
 * user code into two sets: (a) fixed point, and (b) floating
 * point.
 * The naming convention uses the same base name as the
 * fixed point functions, and for floating point it appends
 * a "_f". The macro which is user visible capitalizes
 * the first letter of the function.
 *
 */
/* Floating Point begin here
 *
 */
#ifdef FLTPNT
#define Add_fr1x16    add_fr1x16_f
#define Add_aa_fr2x16 add_aa_fr2x16_f
#define Add_ss_fr2x16 add_ss_fr2x16_f
#define Add_as_fr2x16 add_as_fr2x16_f
#define Add_sa_fr2x16 add_sa_fr2x16_f
#define Sub_fr1x16    sub_fr1x16_f
#define Sub_fr2x16    sub_fr2x16_f
#define Mult_fr1x16   mult_fr1x16_f
#define Mult_fr2x16   mult_fr2x16_f
#define Multr_fr1x16  mult_fr1x16_f
#define Multr_fr2x16  mult_fr2x16_f
#define Mult_i1x16    mult_i1x16_f
#define Abs_fr1x16    abs_fr1x16_f
#define Abs_fr2x16    abs_fr2x16_f
#define Min_fr1x16    min_fr1x16_f
#define Min_fr2x16    min_fr2x16_f
#define Max_fr1x16    max_fr1x16_f
#define Max_fr2x16    max_fr2x16_f
#define Negate_fr1x16 negate_fr1x16_f
#define Shl_fr1x16    shl_fr1x16_f
#define Shl_fr2x16    shl_fr2x16_f
#define Shr_fr1x16    shr_fr1x16_f
#define Shr_fr2x16    shr_fr2x16_f

#define Add_fr1x32    add_fr1x16_f
#define Add_fr32_40   add_fr32_40_f
#define Add_fr40_40   add_fr40_40_f
#define Ashift_fr1x40 ashift_fr1x40_f
#define Sub_fr1x32    sub_fr1x16_f
#define Mult_fr1x32   mult_fr1x16_f
#define Mac_fr16_32   mac_fr16_32_f
#define Mac_fr16_40   mac_fr16_40_f
#define Msu_fr16_40   msu_fr16_40_f
#define Mlt_fr16_40   mlt_fr16_40_f
#define Vmac_fr16_32  vmac_fr16_32_f
#define Vmac_fr16_40  vmac_fr16_40_f
#define Vmac_csqu_40  vmac_csqu_40_f
#define Vmac_cmls_40  vmac_cmls_40_f
#define Ccrosscorr_ashift_cfr16 ccrosscorr_ashift_cfr16_f
#define Cconv_ashift_cfr16      cconv_ashift_cfr16_f
#define Abs_fr1x32    abs_fr1x16_f
#define Min_fr1x32    min_fr1x16_f
#define Max_fr1x32    max_fr1x16_f
#define Negate_fr1x32 negate_fr1x16_f
#define Shl_fr1x32    shl_fr1x16_f
#define Shr_fr1x32    shr_fr1x16_f
#define Shr_fr1x40    shr_fr1x40_f
#define Shl_fr1x40    shl_fr1x40_f
#define Sat_fr1x16    sat_fr1x16_f
#define Round_fr1x32  round_fr1x32_f
#define Norm_fr1x16   norm_fr1x16_f
#define Norm_fr1x32   norm_fr1x16_f
#define Norm_i1x32    norm_i1x32_f
#define Norm_fr1x40   norm_fr1x40_f

#define Sum_fr2x16           sum_fr2x16_f
#define Dif_lh_fr2x16        dif_lh_fr2x16_f
#define Dif_hl_fr2x16        dif_hl_fr2x16_f
#define Fract32_to_fract40   fract16_to_fract40_f
#define Fract16_to_fract40   fract16_to_fract40_f
#define Fract40_to_fract32   fract40_to_fract32_f
#define Fract40_to_fract16   fract40_to_fract32_f
#define Sum_i2x16            sum_i2x16_f
#define Extract_hi_fr1x32    extract_hi_fr1x32_f
#define Extract_hi_fr2x16    extract_hi_fr2x16_f
#define Extract_im           extract_im_f
#define Extract_lo_fr1x32    extract_lo_fr1x32_f
#define Extract_lo_fr2x16    extract_lo_fr2x16_f
#define Extract_re           extract_re_f
#define Compose_fr1x32       compose_fr1x32_f
#define Compose_fr2x16       compose_fr2x16_f
#define Compose_cfr16        compose_cfr16_f
#define Read_fr40            read_fr40_f


#define Ones          ones_f
#define Expadj1x16    expadj1x16_f
#define Expadj1x32    expadj1x16_f
#define Expadj2x16    expadj2x16_f
#define Lvitmax1x16   lvitmax1x16_f
#define Rvitmax1x16   rvitmax1x16_f
#define Lvitmax2x16   lvitmax2x16_f
#define Rvitmax2x16   rvitmax2x16_f
#define Bitmux_shr    bitmux_shr_f
#define Bitmux_shl    bitmux_shl_f

#define Cadd_fr16     cadd_fr16_f
#define Csub_fr16     csub_fr16_f
#define Cmlt_fr16     cmlt_fr16_f
#define Conjmlt_fr16  conjmlt_fr16_f
#define Conjmlt_fr40  conjmlt_fr40_f
#define Conjmlt_ashift_cfr16 conjmlt_ashift_cfr16_f
#define Conj_fr16     conj_fr16_f
#define Cmac_fr16     cmac_fr16_f
#define Cmac_fr40     cmac_fr40_f
#define Cmsu_fr16     cmsu_fr16_f
#define Csqu_fr16     csqu_fr16_f
#define Csqu_fr32     csqu_fr16_f
#define Csqu_ashift_fr32  csqu_ashift_fr32_f
#define Cdst_fr16     cdst_fr16_f
#define Cdst_fr32     cdst_fr16_f
#define Conjmac_fr16  conjmac_fr16_f
#define Conjmac_fr40  conjmac_fr40_f

#define Cshl_fr16     cshl_fr16_f
#define Cshr_fr16     cshr_fr16_f
#define Cexpadj_fr16  cexpadj_fr16_f
#define Cnegate_fr16  cnegate_fr16_f
#define Cmls_fr16     cmls_fr16_f
#define Cmls_i16      cmls_fr16_f
#define Cmlt_i16      cmlt_fr16_f
#define Cdiv_fr16     cdiv_fr16_f
#define Cdivr_fr16    cdivr_fr16_f

#define Divs_fr1x16   divs_fr1x16_f
#define Divs_bi_fr1x16   divs_fr1x16_f
#define Sqrt_fr1x16   sqrt_fr1x16_f
#define Isqrt         isqrt_f
#define Cos_fr16_rad  cos_fr16_rad_f
#define Cos_fr16      cos_fr16_f

#define Set_rnd_mode_biased()  
#define Set_rnd_mode_unbiased() 

/* Fixed Point begin here
 *
 */
#else
#define Add_fr1x16    add_fr1x16
#define Add_aa_fr2x16 add_aa_fr2x16
#define Add_ss_fr2x16 add_ss_fr2x16
#define Add_as_fr2x16 add_as_fr2x16
#define Add_sa_fr2x16 add_sa_fr2x16
#define Sub_fr1x16    sub_fr1x16
#define Mult_fr1x16   mult_fr1x16
#define Mult_fr2x16   mult_fr2x16
#define Multr_fr1x16  multr_fr1x16
#define Multr_fr2x16  multr_fr2x16
#define Mult_i1x16    mult_i1x16
#define Abs_fr1x16    abs_fr1x16
#define Abs_fr2x16    abs_fr2x16
#define Min_fr1x16    min_fr1x16
#define Min_fr2x16    min_fr2x16
#define Max_fr1x16    max_fr1x16
#define Max_fr2x16    max_fr2x16
#define Negate_fr1x16 negate_fr1x16
#define Shl_fr1x16    shl_fr1x16
#define Shl_fr2x16    shl_fr2x16
#define Shr_fr1x16    shr_fr1x16
#define Shr_fr2x16    shr_fr2x16

#define Add_fr1x32    add_fr1x32
#define Add_fr32_40   add_fr32_40
#define Add_fr40_40   add_fr40_40
#define Ashift_fr1x40 ashift_fr1x40
#define Sub_fr1x32    sub_fr1x32
#define Mult_fr1x32   mult_fr1x32
#define Mac_fr16_32   mac_fr16_32
#define Mac_fr16_40   mac_fr16_40
#define Msu_fr16_40   msu_fr16_40
#define Mlt_fr16_40   mlt_fr16_40
#define Vmac_fr16_32  vmac_fr16_32
#define Vmac_fr16_40  vmac_fr16_40
#define Vmac_csqu_40  vmac_csqu_40
#define Vmac_cmls_40  vmac_cmls_40
#define Ccrosscorr_ashift_cfr16 ccrosscorr_ashift_cfr16
#define Cconv_ashift_cfr16      cconv_ashift_cfr16
#define Abs_fr1x32    abs_fr1x32
#define Min_fr1x32    min_fr1x32
#define Max_fr1x32    max_fr1x32
#define Negate_fr1x32 negate_fr1x32
#define Shl_fr1x32    shl_fr1x32
#define Shr_fr1x32    shr_fr1x32
#define Shr_fr1x40    shr_fr1x40
#define Shl_fr1x40    shl_fr1x40
#define Sat_fr1x16    sat_fr1x16
#define Set_rnd_mode_biased   set_rnd_mode_biased
#define Set_rnd_mode_unbiased set_rnd_mode_unbiased
#define Round_fr1x32  round_fr1x32
#define Norm_fr1x16   norm_fr1x16
#define Norm_fr1x32   norm_fr1x32
#define Norm_i1x32    norm_i1x32
#define Norm_fr1x40   norm_fr1x40

#define Sum_fr2x16           sum_fr2x16
#define Dif_lh_fr2x16        dif_lh_fr2x16
#define Dif_hl_fr2x16        dif_hl_fr2x16
#define Fract32_to_fract40   fract32_to_fract40
#define Fract16_to_fract40   fract16_to_fract40
#define Fract40_to_fract32   fract40_to_fract32
#define Fract40_to_fract16   fract40_to_fract16
#define Sum_i2x16            sum_i2x16
#define Extract_hi_fr1x32    extract_hi_fr1x32
#define Extract_hi_fr2x16    extract_hi_fr2x16
#define Extract_im           extract_im
#define Extract_lo_fr1x32    extract_lo_fr1x32
#define Extract_lo_fr2x16    extract_lo_fr2x16
#define Extract_re           extract_re
#define Compose_fr1x32       compose_fr1x32
#define Compose_fr2x16       compose_fr2x16
#define Compose_cfr16        compose_cfr16
#define Read_fr40            read_fr40

#define Ones          ones
#define Expadj1x16    expadj1x16
#define Expadj1x32    expadj1x32
#define Expadj2x16    expadj2x16
#define Sum_i2x16     sum_i2x16
#define Lvitmax1x16   lvitmax1x16
#define Rvitmax1x16   rvitmax1x16
#define Lvitmax2x16   lvitmax2x16
#define Rvitmax2x16   rvitmax2x16
#define Bitmux_shr    bitmux_shr
#define Bitmux_shl    bitmux_shl



#define Cadd_fr16     cadd_fr16
#define Csub_fr16     csub_fr16
#define Cmlt_fr16     cmlt_fr16
#define Conjmlt_fr16  conjmlt_fr16
#define Conjmlt_fr40  conjmlt_fr40
#define Conjmlt_ashift_cfr16 conjmlt_ashift_cfr16
#define Conj_fr16     conj_fr16
#define Cmac_fr16     cmac_fr16
#define Cmac_fr40     cmac_fr40
#define Cmsu_fr16     cmsu_fr16
#define Csqu_fr16     csqu_fr16
#define Csqu_fr32     csqu_fr32
#define Csqu_ashift_fr32  csqu_ashift_fr32
#define Cdst_fr16     cdst_fr16
#define Cdst_fr32     cdst_fr32
#define Conjmac_fr16  conjmac_fr16
#define Conjmac_fr40  conjmac_fr40

#define Cshl_fr16     cshl_fr16
#define Cshr_fr16     cshr_fr16
#define Cexpadj_fr16  cexpadj_fr16
#define Cnegate_fr16  cnegate_fr16
#define Cmls_fr16     cmls_fr16
#define Cmls_i16      cmls_i2x16
#define Cmlt_i16      cmlt_i2x16
#define Cdiv_fr16     cdiv_fr16
#define Cdivr_fr16    cdivr_fr16

#define Divs_fr1x16   divs_fr1x16
#define Divs_bi_fr1x16   divs_bi_fr1x16
#define Sqrt_fr1x16   sqrt_fr1x16
#define Isqrt         isqrt
#define Cos_fr16_rad  cos_fr16_rad
#define Cos_fr16      cos_fr16

#endif /* FLTPNT */

/* Eddy 2008.06.23 */

#include "awb_armdsp_m.h"
#include "awb_dspfns_m.h"
#define	add_fr1x16	AWB_add
#define	sub_fr1x16	AWB_sub
#define	mult_fr1x16	AWB_mult
#define	multr_fr1x16	AWB_mult_r
#define	abs_fr1x16	AWB_abs_s
#define	negate_fr1x16	AWB_negate
#define	shl_fr1x16	AWB_shl
#define	shr_fr1x16	AWB_shr
#define	add_fr1x32	AWB_L_add
#define	sub_fr1x32	AWB_L_sub
#define	mult_fr1x32	AWB_L_mult
#define	mac_fr16_32	AWB_L_mac
#define	abs_fr1x32	AWB_L_abs
#define	negate_fr1x32	AWB_L_negate
#define	shl_fr1x32	AWB_L_shl
#define	shr_fr1x32	AWB_L_shr
#define	round_fr1x32	AWB_round
#define	norm_fr1x32	AWB_norm_l
#define	extract_hi_fr1x32	AWB_extract_h
#define	extract_lo_fr1x32	AWB_extract_l
#define	divs_fr1x16	AWB_div_s

#endif /* _LANGUAGE_ASM */

#endif /* __LIB_ARITH_H__ */
