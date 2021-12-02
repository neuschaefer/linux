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
 *   amr_dspfns_m.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR codec implementation
 *
 * Author:
 * -------
 *   Stan Huang
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

/* dspfns.h
 *
 * Copyright 2001 ARM Limited. All rights reserved.
 *
 * RCS $Revision: #2 $
 * Checkin $Date: 2012/06/28 $
 * Revising $Author: kai.wang $
 */

/* ----------------------------------------------------------------------
 * This header file provides a set of DSP-type primitive
 * operations, such as 16-bit and 32-bit saturating arithmetic. The
 * operations it provides are similar to the ones used by the ITU
 * for publishing specifications of DSP algorithms.
 *
 * This header file is intended as an example implementation. It
 * demonstrates how to use the inline assembly feature in the ARM
 * compilers, to construct intrinsic functions that provide
 * C-language access to the ARM9E's DSP capability.
 *
 * This header file is NOT SUFFICIENT to be used alone - you need
 * to include "dspfns.c" from the examples\dsp directory as well.
 */
#include "typedef.h"
#include "basic_op.h"
#include "amr_dspfns_m.h"
#include "amr_dspfns_m_extend.h"


/*
 * This global variable needs to exist somewhere in the compiled
 * program in order for the flag-using functions to work. You can
 * either include the declaration
 *
 *   armdsp_flagdata_union armdsp_flagdata;
 *
 * in at least one of your source files (that includes this
 * header), or compile in "dspfns.c" from the examples\dsp
 * directory.
 */

/*
 * Convert a 32-bit signed integer into a 16-bit signed integer by
 * saturation.
 */
INLINE Word16 sature(Word32 x)
{
    if (x >= (ARMDSP_WORD16_MAX+1))
        /*Overflow = 1, */x = ARMDSP_WORD16_MAX;
    else if (x < ARMDSP_WORD16_MIN)
        /*Overflow = 1, */x = ARMDSP_WORD16_MIN;
    return (Word16) x;
}

/*
 * Add two 16-bit signed integers with saturation.
 */
INLINE Word16 add(Word32 x, Word32 y)
{
#ifdef __NO_ARM_ASM__
    return( qadd( x << 16, y << 16 ) >> 16 );
#else
    Word32 xs, ys, rs;
    __asm {
        mov xs, x, lsl #16;
        mov ys, y, lsl #16;
        qadd rs, xs, ys;
    }
    return (Word16) (rs >> 16);
#endif
}

/*
 * Subtract one 16-bit signed integer from another with saturation.
 */
INLINE Word16 sub(Word32 x, Word32 y)
{
#ifdef __NO_ARM_ASM__
    return( qsub( x << 16, y << 16 ) >> 16 );
#else
    Word32 xs, ys, rs;
    __asm {
        mov xs, x, lsl #16;
        mov ys, y, lsl #16;
        qsub rs, xs, ys;
    }
    return (Word16) (rs >> 16);
#endif
}

/*
 * Absolute value of a 16-bit signed integer. Saturating, so
 * abs(-0x8000) becomes +0x7FFF.
 */
INLINE Word16 abs_s(Word32 x)
{
    if (x >= 0)
        return x;
    else if (x == ARMDSP_WORD16_MIN)
        return ARMDSP_WORD16_MAX;
    else
        return (Word16) -x;
}

/*
 * Shift a 16-bit signed integer left (or right, if the shift count
 * is negative). Saturate on overflow.
 */
INLINE Word16 shl(Word32 x, Word32 shift)
{
    if (shift <= 0 || x == 0)
        return (Word16) (x >> (-shift));
    if (shift > 15)
        shift = 16;
    return sature(x << shift);
}

/*
 * Shift a 16-bit signed integer right (or left, if the shift count
 * is negative). Saturate on overflow.
 */
INLINE Word16 shr(Word32 x, Word32 shift)
{
    if (shift >= 0 || x == 0)
        return (Word16) (x >> shift);
    if (shift < -15)
        shift = -16;
    return sature(x << (-shift));
}

/*
 * Multiply two 16-bit signed integers, shift the result right by
 * 15 and saturate it. (Saturation is only necessary if both inputs
 * were -0x8000, in which case the result "should" be 0x8000 and is
 * saturated to 0x7FFF.)
 */
INLINE Word16 mult(Word32 x, Word32 y)
{
    Word32 product;
#ifdef __NO_ARM_ASM__
    product = smulbb(x, y);
    return( qadd(product, product) >> 16 );
#else
    __asm {
        smulbb product, x, y;
        qadd product, product, product;
    }
    return (Word16) (product >> 16);   /* the qadd added one to the 15 */
#endif
}

/*
 * Multiply two 16-bit signed integers to give a 32-bit signed
 * integer. Shift left by one, and saturate the result. (Saturation
 * is only necessary if both inputs were -0x8000, in which case the
 * result "should" be 0x40000000 << 1 = +0x80000000, and is
 * saturated to +0x7FFFFFFF.)
 */
INLINE Word32 L_mult(Word32 x, Word32 y)
{
    Word32 product;
#ifdef __NO_ARM_ASM__
    product = smulbb(x, y);
    return( qadd(product, product) );
#else
    __asm {
        smulbb product, x, y;
        qadd product, product, product;
    }
    return product;
#endif
}

/*
 * Negate a 16-bit signed integer, with saturation. (Saturation is
 * only necessary when the input is -0x8000.)
 */
INLINE Word16 negate(Word32 x)
{
    if (x == ARMDSP_WORD16_MIN)
        return ARMDSP_WORD16_MAX;
    return (Word16) -x;
}

/*
 * Return the top 16 bits of a 32-bit signed integer.
 */
INLINE Word16 extract_h(Word32 x)
{
    return (Word16)(x >> 16);
}

/*
 * Return the bottom 16 bits of a 32-bit signed integer, with no
 * saturation, just coerced into a two's complement 16 bit
 * representation.
 */
INLINE Word16 extract_l(Word32 x)
{
    return (Word16) x;
}

/*
 * Divide a 32-bit signed integer by 2^16, AMR_rounding to the nearest
 * integer (AMR_round up on a tie). Equivalent to adding 0x8000 with
 * saturation, then shifting right by 16.
 */
INLINE Word32 AMR_round(Word32 x)
{
#ifdef __NO_ARM_ASM__
    x = qadd(x, 0x8000);
#else
    __asm {
        qadd x, x, 0x8000;
    }
#endif
    return (Word32) extract_h(x);
}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one with saturation, and add to
 * another 32-bit integer with saturation.
 *
 * Note the intermediate saturation operation in the definition:
 *
 *    L_mac(-1, -0x8000, -0x8000)
 *
 * will give 0x7FFFFFFE and not 0x7FFFFFFF:
 *    the unshifted product is:   0x40000000
 *    shift left with saturation: 0x7FFFFFFF
 *    add to -1 with saturation:  0x7FFFFFFE
 */
INLINE Word32 L_mac(Word32 accumulator, Word32 x, Word32 y)
{
    Word32 product;
#ifdef __NO_ARM_ASM__
    product = smulbb(x, y);
    accumulator = qdadd(accumulator, product);
#else
    __asm {
        smulbb product, x, y;
        qdadd accumulator, accumulator, product;
    }
#endif
    return accumulator;
}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one with saturation, and subtract
 * from another 32-bit integer with saturation.
 *
 * Note the intermediate saturation operation in the definition:
 *
 *    L_msu(1, -0x8000, -0x8000)
 *
 * will give 0x80000002 and not 0x80000001:
 *    the unshifted product is:         0x40000000
 *    shift left with saturation:       0x7FFFFFFF
 *    subtract from 1 with saturation:  0x80000002
 */
INLINE Word32 L_msu(Word32 accumulator, Word32 x, Word32 y)
{
    Word32 product;
#ifdef __NO_ARM_ASM__
    product = smulbb(x, y);
    accumulator = qdsub(accumulator, product);
#else
    __asm {
        smulbb product, x, y;
        qdsub accumulator, accumulator, product;
    }
#endif
    return accumulator;
}

/*
 * Add two 32-bit signed integers with saturation.
 */
INLINE Word32 L_add(Word32 x, Word32 y)
{
    Word32 result;
#ifdef __NO_ARM_ASM__
    result = qadd(x, y);
#else
    __asm {
        qadd result, x, y;
    }
#endif
    return result;
}

/*
 * Subtract one 32-bit signed integer from another with saturation.
 */
INLINE Word32 L_sub(Word32 x, Word32 y)
{
    Word32 result;
#ifdef __NO_ARM_ASM__
    result = qsub(x, y);
#else
    __asm {
        qsub result, x, y;
    }
#endif
    return result;
}

#if 0
/*
 * Add together the Carry variable and two 32-bit signed integers,
 * without saturation.
 */
INLINE Word32 L_add_c(Word32 x, Word32 y)
{
    Word32 result, flags;

    __asm {
        msr CPSR_f, armdsp_flagdata_G729.armdsp_flags_word;
        adcs result, x, y;
        mrs flags, CPSR;
        orrvs flags, flags, #0x08000000; // set Q if V is set
        mov armdsp_flagdata_G729.armdsp_flags_word, flags;
    }
    return result;
}

/*
 * Subtract one 32-bit signed integer, together with the Carry
 * variable, from another 32-bit signed integer, without
 * saturation.
 */
INLINE Word32 L_sub_c(Word32 x, Word32 y)
{
    Word32 result, flags;

    __asm {
        msr CPSR_f, armdsp_flagdata_G729.armdsp_flags_word;
        sbcs result, x, y;
        mrs flags, CPSR;
        orrvs flags, flags, #0x08000000; // set Q if V is set
        mov armdsp_flagdata_G729.armdsp_flags_word, flags;
    }
    return result;
}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one _with_ saturation, and add
 * with carry to another 32-bit integer _without_ saturation.
 */
INLINE Word32 L_macNs(Word32 accumulator, Word32 x, Word32 y)
{
    return L_add_c(accumulator, L_mult(x, y));
}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one _with_ saturation, and
 * subtract with carry from another 32-bit integer _without_
 * saturation.
 */
INLINE Word32 L_msuNs(Word32 accumulator, Word32 x, Word32 y)
{
    return L_sub_c(accumulator, L_mult(x, y));
}
#endif

/*
 * Negate a 32-bit signed integer, with saturation. (Saturation is
 * only necessary when the input is -0x80000000.)
 */
INLINE Word32 L_negate(Word32 x)
{
    if (x == ARMDSP_WORD32_MIN)
        return ARMDSP_WORD32_MAX;
    return -x;
}

/*
 * Multiply two 16-bit signed integers, shift the result right by
 * 15 with AMR_rounding, and saturate it. (Saturation is only necessary
 * if both inputs were -0x8000, in which case the result "should"
 * be 0x8000 and is saturated to 0x7FFF.)
 */
INLINE Word16 mult_r(Word32 x, Word32 y)
{
    Word32 product;
#ifdef __NO_ARM_ASM__
    product = qdadd(0x8000, smulbb(x, y));
#else
    __asm {
        smulbb product, x, y;
        qdadd product, 0x8000, product;
    }
#endif
    return (Word16) (product >> 16);   /* qdadd adds one to the 15 */
}

/*
 * Shift a 32-bit signed integer left (or right, if the shift count
 * is negative). Saturate on overflow.
 */
INLINE Word32 L_shl(Word32 x, Word32 shift)
{
    int lz;
    int absx;
    if (shift <= 0)
        return x >> (-shift);

///    absx = (x < 0 ? -x : x);
		absx = x ^ (x>>31);

#ifdef __NO_ARM_ASM__
    lz = clz(absx);
#else
    __asm {
        clz lz, absx;
    }
#endif
    if (shift < lz || x == 0)
        return x << shift;
    if (x < 0)
        return ARMDSP_WORD32_MIN;
    else
        return ARMDSP_WORD32_MAX;
}

/*
 * Shift a 32-bit signed integer left (or right, if the shift count
 * is negative). Saturate on overflow.
 */
INLINE Word32 L_shl_p(Word32 x, Word32 shift)
{
    int lz;
    int absx;

	absx = x ^ (x>>31);

#ifdef __NO_ARM_ASM__
    lz = clz(absx);
#else
    __asm {
        clz lz, absx;
    }
#endif
    if (shift < lz || x == 0)
        return x << shift;
    if (x < 0)
        return ARMDSP_WORD32_MIN;
    else
        return ARMDSP_WORD32_MAX;
}


/*
 * Shift a 32-bit signed integer right (or left, if the shift count
 * is negative). Saturate on overflow.
 */
INLINE Word32 L_shr(Word32 x, Word32 shift)
{
    int lz;
    int absx;
#ifdef __NO_ARM_ASM__
    if (shift >= 0)
        return x >> shift;
#else
    if (shift >= 31) {
        return 0;
    } else if (shift >= 0) {
        return x >> shift;
    }
#endif

///    absx = (x < 0 ? -x : x);
		absx = x ^ (x>>31);

#ifdef __NO_ARM_ASM__
    lz = clz(absx);
#else
    __asm {
        clz lz, absx;
    }
#endif
    if (-shift < lz || x == 0)
        return x << (-shift);
    if (x < 0)
        return ARMDSP_WORD32_MIN;
    else
        return ARMDSP_WORD32_MAX;
}

/*
 * Shift a 16-bit signed integer right, with AMR_rounding. Shift left
 * with saturation if the shift count is negative.
 */
INLINE Word16 shr_r(Word32 x, Word32 shift)
{
    if (shift == 0 || x == 0)
        return (Word16) (x >> shift);
    if (shift > 0)
        return (Word16) (((x >> (shift-1)) + 1) >> 1);
    if (shift < -15)
        shift = -16;
    return sature(x << (-shift));
}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one with saturation, and add to
 * another 32-bit integer with saturation (like L_mac). Then shift
 * the result right by 15 bits with AMR_rounding (like AMR_round).
 */
INLINE Word16 mac_r(Word32 accumulator, Word32 x, Word32 y)
{
    return AMR_round(L_mac(accumulator, x, y));
}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one with saturation, and subtract
 * from another 32-bit integer with saturation (like L_msu). Then
 * shift the result right by 15 bits with AMR_rounding (like AMR_round).
 */
INLINE Word16 msu_r(Word32 accumulator, Word32 x, Word32 y)
{
    return AMR_round(L_msu(accumulator, x, y));
}

/*
 * Shift a 16-bit signed integer left by 16 bits to generate a
 * 32-bit signed integer. The bottom 16 bits are zeroed.
 */
INLINE Word32 L_deposit_h(Word32 x)
{
    return x << 16;
}

/*
 * Sign-extend a 16-bit signed integer by 16 bits to generate a
 * 32-bit signed integer.
 */
INLINE Word32 L_deposit_l(Word32 x)
{
    return x;
}

/*
 * Shift a 32-bit signed integer right, with AMR_rounding. Shift left
 * with saturation if the shift count is negative.
 */
INLINE Word32 L_shr_r(Word32 x, Word32 shift)
{
    int lz;
    int absx;
    if (shift == 0 || x == 0)
        return x >> shift;
    if (shift > 0) {
        Word32 x2 = x >> (shift-1);
        return (x2 >> 1) + (x2 & 1);
    }

///    absx = (x < 0 ? -x : x);
		absx = x ^ (x>>31);


#ifdef __NO_ARM_ASM__
    lz = clz(absx);
#else
    __asm {
        clz lz, absx;
    }
#endif
    if (-shift < lz || x == 0)
        return x << (-shift);
    if (x < 0)
        return ARMDSP_WORD32_MIN;
    else
        return ARMDSP_WORD32_MAX;
}

/*
 * Absolute value of a 32-bit signed integer. Saturating, so
 * abs(-0x80000000) becomes +0x7FFFFFFF.
 */
INLINE Word32 L_abs(Word32 x)
{
    if (x >= 0)
        return x;
    else if (x == ARMDSP_WORD32_MIN)
        return ARMDSP_WORD32_MAX;
    else
        return -x;

}

/*
 * Return a saturated value appropriate to the most recent carry-
 * affecting operation (L_add_c, L_macNs, L_sub_c, L_msuNs).
 *
 * In other words: return the argument if the Q flag is clear.
 * Otherwise, return -0x80000000 or +0x7FFFFFFF depending on
 * whether the Carry flag is set or clear (respectively).
 */
 #if 0
INLINE Word32 L_sat(Word32 x)
{
    int qflag;
    __asm {
        mrs qflag, CPSR;
    }
    if (!(qflag & 0x08000000))
        return x;
    if (Carry)
        return ARMDSP_WORD32_MIN;
    else
        return ARMDSP_WORD32_MAX;
}
 #endif

/*
 * Return the number of bits of left shift needed to arrange for a
 * 16-bit signed integer to have value >= 0x4000 or <= -0x4001.
 *
 * Returns zero if x is zero.
 */
INLINE Word32 norm_s(Word32 x)
{
    int lz;

    if (x == 0)
        return 0;                      /* 0 is a special case */

    x = (Word16) (x ^ (x >> 15));      /* invert x if it's negative */
#ifdef __NO_ARM_ASM__
    lz = clz(x);
#else
    __asm {
        clz lz, x;
    }
#endif
    return (lz - 17);
}

/*
 * Divide one 16-bit signed integer by another, and produce a
 * 15-bit fixed point fractional result (by multiplying the true
 * mathematical result by 0x8000). The divisor (denominator) is
 * assumed to be non-zero and also assumed to be greater or equal
 * to the dividend (numerator). Hence the (unscaled) result is
 * necessarily within the range [0,1].
 *
 * Both operands are assumed to be positive.
 *
 * After division, the result is saturated to fit into a 16-bit
 * signed integer. (The only way this can happen is if the operands
 * are equal, so that the result would be 1, i.e. +0x8000 in 15-bit
 * fixed point.)
 */
INLINE Word16 div_s(Word16 x, Word16 y)
{
    Word32 quot=0;
    if (y!=0) quot = (Word32)(0x8000 * (Word32)x) / y;
    if (y==0 || quot > (ARMDSP_WORD16_MAX))
        return ARMDSP_WORD16_MAX;
    else
        return (Word16)quot;
}

/*
 * Return the number of bits of left shift needed to arrange for a
 * 32-bit signed integer to have value >= 0x40000000 (if
 *
 * Returns zero if x is zero.
 */
INLINE Word32 norm_l(Word32 x)
{
    int lz;

    if (x == 0)
        return 0;                      /* 0 is a special case */

    x ^= (x >> 31);                    /* invert x if it's negative */
#ifdef __NO_ARM_ASM__
    lz = clz(x);
#else
    __asm {
        clz lz, x;
    }
#endif
    return (lz - 1);
}

#endif

